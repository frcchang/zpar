// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                              *
 * hashmap_score_nnf.h - the definition with no negative scores *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2008.03                        *
 *                                                              *
 ****************************************************************/

#ifndef _HASHMAP_SCORE_NNP_H
#define _HASHMAP_SCORE_NNP_H

#include "hashmap_score.h"

/*===============================================================
 *
 * CScoreMap - map to score definition
 *
 *==============================================================*/

template <typename K, typename SCORE_TYPE>
class CScoreMapNoNegFeatures : public CScoreMap< K , SCORE_TYPE > {

public:
   CScoreMapNoNegFeatures(std::string input_name) : CScoreMap< K, SCORE_TYPE >(input_name) {}
   CScoreMapNoNegFeatures(std::string input_name, int TABLE_SIZE) : CScoreMap< K, SCORE_TYPE >(input_name, TABLE_SIZE) {}

public:
   inline void addPositiveFeature( const K &key ) {
      if (!(this->element(key)))
         (*this)[key].reset();
   }

   virtual inline void updateScore( const K &key , const SCORE_TYPE &amount , const int &round ) {
      if (this->element(key))
         (*this)[ key ].updateCurrent( amount , round );
   }

   virtual inline SCORE_TYPE getOrUpdateScore( const K &key , const int &which, const SCORE_TYPE &amount=0 , const int &round=0 ) {
      if ( round == -1) {
         addPositiveFeature( key );
         return 0;
      }
      else {
         return CScoreMap< K, SCORE_TYPE >::getOrUpdateScore(key, which, amount, round);
      }
   }
};

//===============================================================

template<typename K, typename SCORE_TYPE>
inline
std::istream & operator >> (std::istream &is, CScoreMapNoNegFeatures<K, SCORE_TYPE> &score_map) {
   if (!is) return is ;
   std::string s ;
   getline(is, s) ;
   assert( s==score_map.name );
   is >> static_cast< CHashMap< K, CScore<SCORE_TYPE> > &>(score_map) ;
   return is ;
}

template<typename K, typename SCORE_TYPE>
inline
std::ostream & operator << (std::ostream &os, CScoreMapNoNegFeatures<K, SCORE_TYPE> &score_map) {
   assert(os);
   os << score_map.name << std::endl ;

   typename CHashMap< K, CScore<SCORE_TYPE> >::iterator it = score_map.begin() ;
   while ( it != score_map.end() ) {
      os << it.first() << "\t:\t" << it.second() << std::endl ;
      ++ it;
   }
   os << std::endl ;
   return os ;
}

#endif
