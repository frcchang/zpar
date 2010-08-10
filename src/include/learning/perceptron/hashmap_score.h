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

public:
   const string name ;

public:
   CScoreMap(string input_name) : name(input_name) , m_zero() {}
   CScoreMap(string input_name, int TABLE_SIZE) : name(input_name) , m_zero() , 
                                                  CHashMap<K,CScore<SCORE_TYPE> >(TABLE_SIZE) { }

public:
   virtual inline SCORE_TYPE getScore( const K &key , const int &which ) {
      return this->find( key , m_zero ).score( which );
   }

   virtual inline void updateScore( const K &key , const SCORE_TYPE &amount , const int &round ) {
      (*this)[ key ].updateCurrent( amount , round );
   }

   virtual inline SCORE_TYPE getOrUpdateScore( const K &key , const int &which, const SCORE_TYPE &amount=0 , const int &round=0 ) {
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
      typename CHashMap< K, CScore<SCORE_TYPE> >::iterator it = this->begin();
      while (it != this->end()) {
         it.second().updateAverage(round) ;
         ++ it;
      }
   }

   void clearScores() {
      typename CHashMap< K, CScore<SCORE_TYPE> >::iterator it = this->begin();
      while (it != this->end()) {
         it.second().reset() ;
         ++ it;
      }
   }

public:
   SCORE_TYPE dotProductCurrent(CScoreMap &mp) {
      const CScore<SCORE_TYPE> sc_zero;
      SCORE_TYPE retval = 0;
      typename CHashMap< K, CScore<SCORE_TYPE> >::iterator it = this->begin();
      while (it != this->end()) {
         retval += it.second().score() * mp.find(it.first(), sc_zero).score();
         ++ it;
      }
      return retval;
   }

   SCORE_TYPE dotProductCurrentSelf() {
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
      cout << name << ": ";
      CHashMap< K , CScore<SCORE_TYPE> >::trace();
   }
#endif
};

//===============================================================

template<typename K, typename SCORE_TYPE>
inline
istream & operator >> (istream &is, CScoreMap<K, SCORE_TYPE> &score_map) {
   if (!is) return is ;
   string s ;
   getline(is, s) ;
   if ( s!=score_map.name ) THROW("the expected score map " << score_map.name << " is not matched.");
   is >> static_cast< CHashMap< K, CScore<SCORE_TYPE> > &>(score_map) ;
   return is ;
}

template<typename K, typename SCORE_TYPE>
inline
ostream & operator << (ostream &os, CScoreMap<K, SCORE_TYPE> &score_map) {
   assert(os);
   os << score_map.name << endl ;

   typename CHashMap< K, CScore<SCORE_TYPE> >::iterator it = score_map.begin() ;
   while ( it != score_map.end() ) {
      if ( it.second().score(CScore<SCORE_TYPE>::eNonAverage)!=0 ||
           it.second().score(CScore<SCORE_TYPE>::eAverage)!=0 )
         os << it.first() << "\t:\t" << it.second() << endl ;
      ++ it;
   }
   os << endl ;
   return os ;
}

#endif
