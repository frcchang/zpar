/****************************************************************
 *                                                              *
 * score_packed_array.h - the array implementation of packed score. *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 ****************************************************************/

#ifndef _SCORE_PACKED_ARRAY_H
#define _SCORE_PACKED_ARRAY_H

/*===============================================================
 *
 * CPackedScore - packed score definition
 *
 *==============================================================*/

template <typename SCORE_TYPE, unsigned PACKED_SIZE>
class CPackedScoreType;

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
   void add(CPackedScoreType<SCORE_TYPE, PACKED_SIZE> &o, const int &which) const {
      for (unsigned index=0; index<PACKED_SIZE; ++index)
         o[index] += scores[index].score(which);
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


#endif
