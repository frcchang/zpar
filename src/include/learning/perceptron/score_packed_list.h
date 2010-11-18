/****************************************************************
 *                                                              *
 * score_packed_hash.h - the definitions for packed score v.    *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 ****************************************************************/

#ifndef _SCORE_PACKED_LIST_H
#define _SCORE_PACKED_LIST_H

#include "hash.h"
#include "linkedlist.h"
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
   if (!is) return is ;
   static std::string s ;
   static unsigned key;
   static CScore<SCORE_TYPE> value;
   is >> s;
   ASSERT(s=="{"||s=="{}", "The packed score does not start with {");
   if (s=="{}")
      return is;
   while (true) {
      is >> key;
      is >> s;
//std::cout<<(s)<<endl;std::cout.flush();
      ASSERT(s==":", "The packed score does not have : after key: "<<key);
      is >> value;
      score.scores[key] = value;
      is >> s;
      ASSERT(s==","||s=="}", "The packed score does not have a , or } after value: "<<value);
      if (s=="}")
         return is;
   }
   THROW("score_packed_list.h: the program should not have reached here.");
   return is ;
}

template <typename SCORE_TYPE, unsigned PACKED_SIZE>
std::ostream & operator << (std::ostream &os, const CPackedScore<SCORE_TYPE, PACKED_SIZE> &score) {
   assert(PACKED_SIZE>0);
   os << "{";
   typename CLinkedList< unsigned, CScore<SCORE_TYPE> >::const_iterator it = score.scores.begin();
   bool bBegin=true;
   while (it!=score.scores.end()) {
      if (!it.second().zero()) {
         if (bBegin) 
            os << ' ';
         else
            os << " , ";
         bBegin=false;
         os << it.first() << " : " << it.second();
      }
      ++it;
   }
   if (!bBegin) os << ' ';
   os << "}";
   return os;
}

template <typename SCORE_TYPE, unsigned PACKED_SIZE>
class CPackedScore {
protected:
   CLinkedList< unsigned, CScore<SCORE_TYPE> > scores;
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
      typename CLinkedList< unsigned, CScore<SCORE_TYPE> >::iterator it;
      it = scores.begin();
      while (it != scores.end()) {
         it.second().updateAverage(round);
         ++it;
      }
   }
   void reset() {
      typename CLinkedList< unsigned, CScore<SCORE_TYPE> >::iterator it;
      it = scores.begin();
      while (it != scores.end()) {
         it.second().reset();
         ++it;
      }
   }
   bool empty() const {
      typename CLinkedList< unsigned, CScore<SCORE_TYPE> >::const_iterator it;
      it = scores.begin();
      while (it != scores.end()) {
         if (!it.second().zero()) return false;
         ++it;
      }
      return true;
   }
   void add(CPackedScoreType<SCORE_TYPE, PACKED_SIZE> &o, const int &which) const {
      typename CLinkedList<unsigned, CScore<SCORE_TYPE> > ::const_iterator it;
      it = scores.begin();
      while (it != scores.end()) {
         o[it.first()]+=it.second().score(which);
         ++it;
      }
   }
   void trace() const {
      typename CLinkedList< unsigned, CScore<SCORE_TYPE> >::const_iterator it;
      it = scores.begin();
      while (it != scores.end()) {
         (std::cout) << it.first();
         std::cout << ' ';
         (std::cout) << (it.second());
         std::cout << std::endl;
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
