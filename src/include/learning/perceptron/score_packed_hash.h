/****************************************************************
 *                                                              *
 * score_packed_hash.h - the definitions for packed score v.    *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 ****************************************************************/

#ifndef _SCORE_PACKED_HASH_H
#define _SCORE_PACKED_HASH_H

#include "hash.h"
#include "hash_small.h"
#include "score.h"

/*===============================================================
 *
 * CPackedScore - packed score definition
 *
 *==============================================================*/

template <typename SCORE_TYPE, unsigned PACKED_SIZE>
class CPackedScoreType;

template <typename SCORE_TYPE, unsigned PACKED_SIZE>
class CPackedScore;
template <typename SCORE_TYPE, unsigned PACKED_SIZE>
inline
std::istream & operator >> (std::istream &is, CPackedScore<SCORE_TYPE, PACKED_SIZE> &score) {
   assert(PACKED_SIZE>0);
   score.reset();
   assert(score.empty());
   is >> score.scores;
/*
   static CScore<SCORE_TYPE> sco;
   static std::string s;
   is >> s;
   ASSERT(s=="[", "hashmap_score_packed.h: not well formatted CPackedScore");
   is >> sco;
   if (!sco.zero()) score[0] = sco;
   for (unsigned index=1; index<PACKED_SIZE; ++index) {
      is >> s;
      ASSERT(s==",", "hashmap_score_packed.h: not well formatted CPackedScore");
      is >> sco;
      if (!sco.zero()) score[index] = sco;
   }
   is >> s;
   ASSERT(s=="]", "hashmap_score_packed.h: not well formatted CPackedScore");
*/
   return is;
}

template <typename SCORE_TYPE, unsigned PACKED_SIZE>
std::ostream & operator << (std::ostream &os, const CPackedScore<SCORE_TYPE, PACKED_SIZE> &score) {
   assert(PACKED_SIZE>0);
/*
   os << " [ ";
   os << score.find(0);
   for (unsigned index=1; index<PACKED_SIZE; ++index)
      os << " , " << score.find(index);
   os << " ] ";
*/ os << score.scores;
   return os;
}

template <typename SCORE_TYPE, unsigned PACKED_SIZE>
class CPackedScore {
protected:
   const static unsigned HASHMAP_SIZE=PACKED_SIZE<1?PACKED_SIZE:1;
   CSmallHashMap<unsigned, CScore<SCORE_TYPE>, HASHMAP_SIZE> scores;
public:
   CPackedScore() : scores() {}
public:
   const SCORE_TYPE score(const unsigned &index, const int &n) const {
      return scores.find(index, CScore<SCORE_TYPE>()).score(n);
   }
   void updateCurrent(const unsigned &index, const SCORE_TYPE &added, const int &round) {
      scores[index].updateCurrent(added, round);
   }
   void updateAverage(const int &round) {
      typename CSmallHashMap<unsigned, CScore<SCORE_TYPE>, HASHMAP_SIZE>::iterator it;
      it = scores.begin();
      while (it != scores.end()) {
         it.second().updateAverage(round);
         ++it;
      }
   }
   void reset() {
      typename CSmallHashMap<unsigned, CScore<SCORE_TYPE>, HASHMAP_SIZE>::iterator it;
      it = scores.begin();
      while (it != scores.end()) {
         it.second().reset();
         ++it;
      }
   }
   bool empty() const {
      typename CSmallHashMap<unsigned, CScore<SCORE_TYPE>, HASHMAP_SIZE>::const_iterator it;
      it = scores.begin();
      while (it != scores.end()) {
         if (!it.second().zero()) return false;
         ++it;
      }
      return true;
   }
   void add(CPackedScoreType<SCORE_TYPE, PACKED_SIZE> &o, const int &which) const {
      typename CSmallHashMap<unsigned, CScore<SCORE_TYPE>, HASHMAP_SIZE>::const_iterator it;
      it = scores.begin();
      while (it != scores.end()) {
         o[it.first()]+=it.second().score(which);
         ++it;
      }
   }
   void trace() const {
      typename CSmallHashMap<unsigned, CScore<SCORE_TYPE>, HASHMAP_SIZE>::const_iterator it;
      it = scores.begin();
      while (it != scores.end()) {
         (std::cerr) << it.first();
         std::cerr << ' ';
         (std::cerr) << (it.second());
         std::cerr << std::endl;
         ++it;
      }
   }
public:
   CScore<SCORE_TYPE> & operator [](const unsigned &index) { return scores[index]; }
   const CScore<SCORE_TYPE> & find(const unsigned &index) const { return scores.find(index,CScore<SCORE_TYPE>()); }
   friend std::istream & operator >> <> (std::istream &is, CPackedScore &score);
   friend std::ostream & operator << <> (std::ostream &os, const CPackedScore &score);
};

#endif
