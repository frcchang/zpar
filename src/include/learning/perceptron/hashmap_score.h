// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                              *
 * hashmap_score.h - the definitions for hash map with score v. *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2008.03                        *
 *                                                              *
 ****************************************************************/

#ifndef _HASHMAP_SCORE_H
#define _HASHMAP_SCORE_H

#include "hash.h"
#include "score.h"

/*===============================================================
 *
 * CScoreMap - map to score definition
 *
 *==============================================================*/

template <typename K, typename SCORE_TYPE>
class CScoreMap : public CHashMap< K , CScore<SCORE_TYPE> > {

protected:
   const CScore<SCORE_TYPE> m_zero ;

#ifdef NO_NEG_FEATURE
protected:
   const CScoreMap *m_positive;
#endif

public:
   const std::string name ;
   bool initialized ;
   unsigned count ;

public:
//   CScoreMap(std::string input_name) : name(input_name) , m_zero() {}
   CScoreMap(std::string input_name, int TABLE_SIZE, bool bInitMap=true) : CHashMap<K,CScore<SCORE_TYPE> >(TABLE_SIZE, bInitMap) , m_zero() , name(input_name) , initialized(bInitMap) , count(0)
#ifdef NO_NEG_FEATURE
, m_positive(this)
#endif
{ }

public:
   virtual inline void init() {
      initialized = true;
      CHashMap<K, CScore<SCORE_TYPE> >::init();
   }

#ifdef NO_NEG_FEATURE
   virtual inline void setPositiveFeature(const CScoreMap &positive) { m_positive = &positive; }
   virtual inline void addPositiveFeature(const K &key) { (*this)[key]; }
#endif

   virtual inline SCORE_TYPE getScore( const K &key , const int &which ) {
      return this->find( key , m_zero ).score( which );
   }

   virtual inline void updateScore( const K &key , const SCORE_TYPE &amount , const int &round ) {
#ifdef NO_NEG_FEATURE
      if (m_positive->element(key))
#endif // update can only happen with defined features
			{
//				fprintf(stderr, "update feature by %.2f\n", amount);
      	(*this)[ key ].updateCurrent( amount , round );
			}
   }

   virtual inline SCORE_TYPE getOrUpdateScore( const K &key , const int &which, const SCORE_TYPE &amount=0 , const int &round=0 ) {
#ifdef NO_NEG_FEATURE
      if ( round == -1 ) {
         addPositiveFeature( key );
         return 0;
      }
#endif
      if ( amount == 0 ) {
         return getScore( key , which ) ;
      }
      else {
         assert( round != 0 );
         updateScore( key , amount , round ) ;
         return 0;
      }
   }

   void computeAverage(unsigned long int round) {
      count = 0;
      typename CHashMap< K, CScore<SCORE_TYPE> >::iterator it = this->begin();
      while (it != this->end()) {
         it.second().updateAverage(round) ;
         ++ it;
         ++ count;
      }
   }

//   void clearScores() {
//      typename CHashMap< K, CScore<SCORE_TYPE> >::iterator it = this->begin();
//      while (it != this->end()) {
//         it.second().reset() ;
//         ++ it;
//      }
//   }

public:
   SCORE_TYPE dotProduct(CScoreMap &mp) {
      const CScore<SCORE_TYPE> sc_zero;
      SCORE_TYPE retval = 0;
      typename CHashMap< K, CScore<SCORE_TYPE> >::iterator it = this->begin();
      while (it != this->end()) {
         retval += it.second().score() * mp.find(it.first(), sc_zero).score();
         ++ it;
      }
      return retval;
   }

   void scaleCurrent(const SCORE_TYPE &scale, const int &round) {
      typename CHashMap< K, CScore<SCORE_TYPE> >::iterator it = this->begin();
      while (it != this->end()) {
         it.second().scaleCurrent(scale, round);
         ++ it;
      }
   }

   SCORE_TYPE addCurrent(CScoreMap &mp, const int &round) {
      const CScore<SCORE_TYPE> sc_zero;
      typename CHashMap< K, CScore<SCORE_TYPE> >::iterator it = mp.begin();
      while (it != mp.end()) {
         (*this)[it.first()].updateCurrent((it.second()).score(), round);
         ++ it;
      }
   }

   SCORE_TYPE subtractCurrent(CScoreMap &mp, const int &round) {
      const CScore<SCORE_TYPE> sc_zero;
      typename CHashMap< K, CScore<SCORE_TYPE> >::iterator it = mp.begin();
      while (it != mp.end()) {
         (*this)[it.first()].updateCurrent(-(it.second()).score(), round);
         ++ it;
      }
   }

   SCORE_TYPE squareNorm() {
      SCORE_TYPE retval = 0;
      typename CHashMap< K, CScore<SCORE_TYPE> >::iterator it = this->begin();
      while (it != this->end()) {
         retval += (it.second()).score() * (it.second()).score();
         ++ it;
      }
      return retval;
   }

#ifdef DEBUG
public:
   void trace() {
      std::cerr << name << ": ";
      CHashMap< K , CScore<SCORE_TYPE> >::trace();
   }
#endif
};

//===============================================================

template<typename K, typename SCORE_TYPE>
inline
std::istream & operator >> (std::istream &is, CScoreMap<K, SCORE_TYPE> &score_map) {
   if (!is) return is ;
   std::string s ;
   if (!getline(is, s)) THROW("incorrectly formatted scoremap (no title).") ;
   // match name
   const unsigned &size = score_map.name.size();
   if ( s.substr(0, size)!=score_map.name ) THROW("the expected score map " << score_map.name << " is not matched.");
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
   is >> static_cast< CHashMap< K, CScore<SCORE_TYPE> > &>(score_map) ;
   return is ;
}

template<typename K, typename SCORE_TYPE>
inline
std::ostream & operator << (std::ostream &os, CScoreMap<K, SCORE_TYPE> &score_map) {
   assert(os);
   if (score_map.count)
      os << score_map.name << ' ' << score_map.count << std::endl ;
   else
      os << score_map.name << std::endl ;

   typename CHashMap< K, CScore<SCORE_TYPE> >::iterator it = score_map.begin() ;
   while ( it != score_map.end() ) {
#ifndef NO_NEG_FEATURE
      if ( it.second().score(CScore<SCORE_TYPE>::eNonAverage)!=0 ||
           it.second().score(CScore<SCORE_TYPE>::eAverage)!=0 )
#endif
         os << it.first() << "\t:\t" << it.second() << std::endl ;
      ++ it;
   }
   os << std::endl ;
   return os ;
}

#endif
