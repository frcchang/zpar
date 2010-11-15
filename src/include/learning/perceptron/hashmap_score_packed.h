/****************************************************************
 *                                                              *
 * hashmap_score_packed.h - the definitions for hash map with   *
 *                          packed score v.                     *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 ****************************************************************/

#ifndef _HASHMAP_SCORE_PACKED_H
#define _HASHMAP_SCORE_PACKED_H

#include "hash.h"
#include "score.h"

/*===============================================================
 *
 * CPackedScore - packed score definition
 *
 *==============================================================*/

template <typename SCORE_TYPE, unsigned PACKED_SIZE>
class CPackedScore {
protected:
   CScore<SCORE_TYPE> scores[PACKED_SIZE];
public:
   const SCORE_TYPE score(const unsigned &index, const int &n) const {
      return scores[index].score(n);
   }
   void updateCurrent(const unsigned &index, const SCORE_TYPE &added, const int &round) {
      scores[index].updateCurrent(added, round);
   }
   void updateAverage(const int &round) {
      for (unsigned index=0; index<PACKED_SIZE; ++index)
         scores[index].updateAverage(round);
   }
   void reset() {
      for (unsigned index=0; index<PACKED_SIZE; ++index)
         scores[index].reset();
   }
   bool empty() const {
      for (unsigned index=0; index<PACKED_SIZE; ++index)
         if (!scores[index].zero()) return false;
      return true;
   }
public:
   CScore<SCORE_TYPE> & operator [](const unsigned &index) { return scores[index]; }
   const CScore<SCORE_TYPE> & operator [](const unsigned &index) const { return scores[index]; }
};

template <typename SCORE_TYPE, unsigned PACKED_SIZE>
std::istream & operator >> (std::istream &is, CPackedScore<SCORE_TYPE, PACKED_SIZE> &score) {
   assert(PACKED_SIZE>0);
   std::string s;
   is >> s;
   ASSERT(s=="[", "hashmap_score_packed.h: not well formatted CPackedScore");
   is >> score[0];
   for (unsigned index=1; index<PACKED_SIZE; ++index) {
      is >> s;
      ASSERT(s==",", "hashmap_score_packed.h: not well formatted CPackedScore");
      is >> score[index];
   }
   is >> s;
   ASSERT(s=="]", "hashmap_score_packed.h: not well formatted CPackedScore");
   return is;
}

template <typename SCORE_TYPE, unsigned PACKED_SIZE>
std::ostream & operator << (std::ostream &os, CPackedScore<SCORE_TYPE, PACKED_SIZE> &score) {
   assert(PACKED_SIZE>0);
   os << " [ ";
   os << score[0];
   for (unsigned index=1; index<PACKED_SIZE; ++index)
      os << " , " << score[index];
   os << " ] ";
   return os;
}

/*===============================================================
 *
 * CPackedScoreType - packed score type definition
 *
 *==============================================================*/

template <typename SCORE_TYPE, unsigned PACKED_SIZE>
class CPackedScoreType {
protected:
   SCORE_TYPE scores[PACKED_SIZE];
public:
   void reset() {
      for (unsigned index=0; index<PACKED_SIZE; ++index)
         scores[index]=0;
   }
   bool empty() const {
      for (unsigned index=0; index<PACKED_SIZE; ++index)
         if (scores[index]!=0) return false;
      return true;
   }
public:
   SCORE_TYPE &operator [](const unsigned &index) {
      assert(index<PACKED_SIZE);
      return scores[index];
   }
   const SCORE_TYPE &operator [](const unsigned &index) const {
      assert(index<PACKED_SIZE);
      return scores[index];
   }
   void operator +=(const CPackedScoreType &i) {
      for (unsigned index=0; index<PACKED_SIZE; ++index)
         scores[index]+=i.scores[index];
   }
   void add(const CPackedScore<SCORE_TYPE, PACKED_SIZE> &i, const int &which) {
      for (unsigned index=0; index<PACKED_SIZE; ++index)
         scores[index]+=i.score(index, which);
   }
};

template <typename SCORE_TYPE, unsigned PACKED_SIZE>
std::istream & operator >> (std::istream &is, CPackedScoreType<SCORE_TYPE, PACKED_SIZE> &score) {
   assert(PACKED_SIZE>0);
   std::string s;
   is >> s;
   ASSERT(s=="[", "hashmap_score_packed.h: not well formatted CPackedScore");
   is >> score[0];
   for (unsigned index=1; index<PACKED_SIZE; ++index) {
      is >> s;
      ASSERT(s==",", "hashmap_score_packed.h: not well formatted CPackedScore");
      is >> score[index];
   }
   is >> s;
   ASSERT(s=="]", "hashmap_score_packed.h: not well formatted CPackedScore");
   return is;
}

template <typename SCORE_TYPE, unsigned PACKED_SIZE>
std::ostream & operator << (std::ostream &os, CPackedScoreType<SCORE_TYPE, PACKED_SIZE> &score) {
   assert(PACKED_SIZE>0);
   os << " [ ";
   os << score[0];
   for (unsigned index=1; index<PACKED_SIZE; ++index)
      os << " , " << score[index];
   os << " ] ";
   return os;
}

/*===============================================================
 *
 * CPackedScoreMap - map to packed score definition
 *
 *==============================================================*/

template <typename K, typename SCORE_TYPE, unsigned PACKED_SIZE>
class CPackedScoreMap : public CHashMap< K , CPackedScore<SCORE_TYPE, PACKED_SIZE> > {

protected:
   const CPackedScore<SCORE_TYPE, PACKED_SIZE> m_zero ;

public:
   const std::string name ;
   bool initialized ;
   unsigned count ;

public:
   CPackedScoreMap(std::string input_name, int TABLE_SIZE, bool bInitMap=true) : name(input_name) , initialized(bInitMap) , count(0) , m_zero() , CHashMap<K,CPackedScore<SCORE_TYPE, PACKED_SIZE> >(TABLE_SIZE, bInitMap) {
      assert(m_zero.empty());
   }

public:
   virtual inline void init() {
      initialized = true;
      CHashMap<K, CPackedScore<SCORE_TYPE, PACKED_SIZE> >::init();
   }

   virtual inline SCORE_TYPE getScore( const K &key , const unsigned &index , const int &which ) {
      return this->find( key , m_zero ).score( index , which );
   }

   virtual inline void updateScore( const K &key , const unsigned &index , const SCORE_TYPE &amount , const int &round ) {
      (*this)[ key ].updateCurrent( index , amount , round );
   }

   virtual inline void getOrUpdateScore( CPackedScoreType<SCORE_TYPE, PACKED_SIZE> &out , const K &key , const unsigned &index , const int &which , const SCORE_TYPE &amount=0 , const int &round=0 ) {
      if ( amount == 0 ) {
         out.add(this->find(key, m_zero), which) ;
      }
      else {
         assert( round != 0 );
         updateScore( key , index , amount , round ) ;
      }
   }

   void computeAverage(unsigned long int round) {
      count = 0;
      typename CHashMap< K, CPackedScore<SCORE_TYPE, PACKED_SIZE> >::iterator it = this->begin();
      while (it != this->end()) {
         it.second().updateAverage(round) ;
         ++ it;
         ++ count;
      }
   }

   void clearScores() {
      typename CHashMap< K, CPackedScore<SCORE_TYPE, PACKED_SIZE> >::iterator it = this->begin();
      while (it != this->end()) {
         it.second().reset() ;
         ++ it;
      }
   }

#ifdef DEBUG
public:
   void trace() {
      std::cout << name << ": ";
      CHashMap< K , CPackedScore<SCORE_TYPE, PACKED_SIZE> >::trace();
   }
#endif
};

//===============================================================

template<typename K, typename SCORE_TYPE, unsigned PACKED_SIZE>
inline
std::istream & operator >> (std::istream &is, CPackedScoreMap<K, SCORE_TYPE, PACKED_SIZE> &score_map) {
   if (!is) return is ;
   std::string s ;
   getline(is, s) ;
   // match name
   const unsigned &size = score_map.name.size();
   if ( s.substr(0, size)!=score_map.name ) THROW("hashmap_score_packed.h: the expected score map " << score_map.name << " is not matched.");
   if ( !score_map.initialized ) {
      // match size
      if ( s.size()>size ) {
         unsigned table_size = 0;
         std::istringstream buffer(s.substr(size));
         buffer >> table_size;
         if (table_size) {
            unsigned hash_size = 1;
            while (hash_size<table_size) hash_size <<= 1;
            hash_size <<= 1;
            score_map.resize(hash_size);
         }
      }
      score_map.init();
   }
   is >> static_cast< CHashMap< K, CPackedScore<SCORE_TYPE, PACKED_SIZE> > &>(score_map) ;
   return is ;
}

template<typename K, typename SCORE_TYPE, unsigned PACKED_SIZE>
inline
std::ostream & operator << (std::ostream &os, CPackedScoreMap<K, SCORE_TYPE, PACKED_SIZE> &score_map) {
   assert(os);
   if (score_map.count)
      os << score_map.name << ' ' << score_map.count << std::endl ;
   else
      os << score_map.name << std::endl ;

   typename CHashMap< K, CPackedScore<SCORE_TYPE, PACKED_SIZE> >::iterator it = score_map.begin() ;
   while ( it != score_map.end() ) {
      if ( !it.second().empty() ) 
         os << it.first() << "\t:\t" << it.second() << std::endl ;
      ++ it;
   }
   os << std::endl ;
   return os ;
}

#endif
