/****************************************************************
 *                                                              *
 * hashmap_weighted_score.h - the map itself has a weight to adjust. *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2008.03                        *
 *                                                              *
 ****************************************************************/

#ifndef _HASHMAP_WEIGHTED_SCORE_H
#define _HASHMAP_WEIGHTED_SCORE_H

#include "hashmap_score.h"

template<typename K, typename SCORE_TYPE>
class CWeightedScoreMap;

//===============================================================

template<typename K, typename SCORE_TYPE>
inline
istream & operator >> (istream &is, CWeightedScoreMap<K, SCORE_TYPE> &score_map) {
   if (!is) return is ;
   string s ;
   getline(is, s) ;
   assert( s==score_map.name );
   getline(is, s) ;
   istringstream iss;
   iss.str(s);
   iss >> score_map.m_weight;
   is >> static_cast< CHashMap< K, CScore<SCORE_TYPE> > &>(score_map) ;
   return is ;
}

template<typename K, typename SCORE_TYPE>
inline
ostream & operator << (ostream &os, CWeightedScoreMap<K, SCORE_TYPE> &score_map) {
   assert(os);
   os << score_map.name << endl ;
   os << score_map.m_weight << endl ;

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

/*===============================================================
 *
 * CWeightedScoreMap - map to score definition
 *
 *==============================================================*/

template <typename K, typename SCORE_TYPE>
class CWeightedScoreMap : public CScoreMap<K , SCORE_TYPE> {

protected:
   CScore<SCORE_TYPE> m_weight ;

public:
   CWeightedScoreMap(string input_name) : CScoreMap<K, SCORE_TYPE>(input_name) {}
   CWeightedScoreMap(string input_name, int TABLE_SIZE) : CScoreMap<K, SCORE_TYPE>(input_name, TABLE_SIZE) { }

public:
   inline SCORE_TYPE getScore( const K &key , const int &which ) {
      return m_weight.score( which ) * CScoreMap<K, SCORE_TYPE>::getScore(key, which);
   }

   inline void updateScore( const K &key , const SCORE_TYPE &amount , const int &round ) {
      m_weight.updateCurrent( amount , round );
      CScoreMap<K, SCORE_TYPE>::updateScore(key, m_weight.score()>0?amount:-amount, round);
   }

   void computeAverage(unsigned int round) {
      m_weight.updateAverage(round);
      CScoreMap<K, SCORE_TYPE>::computeAverage(round);
   }

   friend istream & operator >> <> (istream &is, CWeightedScoreMap &score_map) ;
   friend ostream & operator << <> (ostream &os, CWeightedScoreMap &score_map) ;
};

#endif
