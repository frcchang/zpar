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

template <typename SCORE_TYPE, unsigned long PACKED_SIZE>
class CPackedScoreType;

template <typename SCORE_TYPE, unsigned long PACKED_SIZE>
class CPackedScore;
template <typename SCORE_TYPE, unsigned long PACKED_SIZE>
inline
std::istream & operator >> (std::istream &is, CPackedScore<SCORE_TYPE, PACKED_SIZE> &score) {
   assert(PACKED_SIZE>0);
   score.clear();
   assert(score.empty());
   if (!is) return is ;
   static std::string s ;
   static unsigned long key;
//   static CScore<SCORE_TYPE> value;
   is >> s;
   ASSERT(s=="{"||s=="{}", "The packed score does not start with {");
   if (s=="{}")
      return is;
   while (true) {
      is >> key;
      is >> s;
//std::cout<<(s)<<endl;std::cout.flush();
      ASSERT(s==":", "The packed score does not have : after key: "<<key);
      is >> score.scores[key];
//      is >> value;
//      score.scores[key] = value;
      is >> s;
      ASSERT(s==","||s=="}", "The packed score does not have a , or } after value: "<<score.scores[key]);
      if (s=="}")
         return is;
   }
   THROW("score_packed_list.h: the program should not have reached here.");
   return is ;
}

template <typename SCORE_TYPE, unsigned long PACKED_SIZE>
std::ostream & operator << (std::ostream &os, const CPackedScore<SCORE_TYPE, PACKED_SIZE> &score) {
   assert(PACKED_SIZE>0);
   os << "{";
   typename CLinkedList< unsigned long, CScore<SCORE_TYPE> >::const_iterator it = score.scores.begin();
   bool bBegin=true;
   while (it!=score.scores.end()) {
#ifndef NO_NEG_FEATURE
      if (!it.second().zero()) {
#endif // do not print zero scores when allow negative feature
         if (bBegin)
            os << ' ';
         else
            os << " , ";
         bBegin=false;
         os << it.first() << " : " << it.second();
#ifndef NO_NEG_FEATURE
      }
#endif // but have to when disallow because features are static
      ++it;
   }
   if (!bBegin) os << ' ';
   os << "}";
   return os;
}

template <typename SCORE_TYPE, unsigned long PACKED_SIZE>
class CPackedScore {
protected:
   CLinkedList< unsigned long, CScore<SCORE_TYPE> > scores;
public:
   CPackedScore() : scores() {}
public:
   const SCORE_TYPE score(const unsigned long &index, const int &n) const {
      return scores.find(index, CScore<SCORE_TYPE>()).score(n);
   }
   void updateCurrent(const unsigned long &index, const SCORE_TYPE &added, const int &round) {
      scores[index].updateCurrent(added, round);
   }
   void updateAverage(const int &round) {
      typename CLinkedList< unsigned long, CScore<SCORE_TYPE> >::iterator it;
      it = scores.begin();
      while (it != scores.end()) {
         it.second().updateAverage(round);
         ++it;
      }
   }
//   void reset() {
//      typename CLinkedList< unsigned long, CScore<SCORE_TYPE> >::iterator it;
//      it = scores.begin();
//      while (it != scores.end()) {
//         it.second().reset();
//         ++it;
//      }
//   }
   void clear() {
      scores.clear();
   }
   bool empty() const {
      typename CLinkedList< unsigned long, CScore<SCORE_TYPE> >::const_iterator it;
      it = scores.begin();
      while (it != scores.end()) {
         if (!it.second().zero()) return false;
         ++it;
      }
      return true;
   }
   void add(CPackedScoreType<SCORE_TYPE, PACKED_SIZE> &o, const int &which) const {
      typename CLinkedList<unsigned long, CScore<SCORE_TYPE> > ::const_iterator it;
      it = scores.begin();
      while (it != scores.end()) {
         o[it.first()]+=it.second().score(which);
         ++it;
      }
   }
   void trace() const {
      typename CLinkedList< unsigned long, CScore<SCORE_TYPE> >::const_iterator it;
      it = scores.begin();
      while (it != scores.end()) {
         (std::cerr) << it.first();
         std::cerr << ' ';
         (std::cerr) << (it.second());
         std::cerr << std::endl;
         ++it;
      }
   }
   bool element(const unsigned long &index) const {
      return scores.element(index);
   }

public:

   void addCurrent(CPackedScore &s, const int &round) {
      typename CLinkedList<unsigned long, CScore<SCORE_TYPE> > ::iterator it;
      it = s.scores.begin();
      while (it != s.scores.end()) {
         scores[it.first()].updateCurrent(it.second().score(), round);
         ++it;
      }
   }

   void subtractCurrent(CPackedScore &s, const int &round) {
      typename CLinkedList<unsigned long, CScore<SCORE_TYPE> > ::iterator it;
      it = s.scores.begin();
      while (it != s.scores.end()) {
         scores[it.first()].updateCurrent(-it.second().score(), round);
         ++it;
      }
   }

   SCORE_TYPE dotProduct(const CPackedScore &s) {
      static const CScore<SCORE_TYPE> sc_zero;
      SCORE_TYPE retval = 0;
      if (s.empty()) return retval;
      typename CLinkedList<unsigned long, CScore<SCORE_TYPE> > ::iterator it;
      it = this->scores.begin();
      while (it != this->scores.end()) {
         retval += it.second().score() * s.scores.find(it.first(), sc_zero).score();
         ++ it;
      }
      return retval;
   }

   SCORE_TYPE squareNorm() {
      typename CLinkedList<unsigned long, CScore<SCORE_TYPE> > ::iterator it;
      it = scores.begin();
      SCORE_TYPE retval = 0;
      while (it != scores.end()) {
         retval += it.second().score() * it.second().score();
         ++it;
      }
      return retval;
   }

   void scaleCurrent(SCORE_TYPE scale, const int &round) {
      typename CLinkedList<unsigned long, CScore<SCORE_TYPE> > ::iterator it;
      it = scores.begin();
      while (it != scores.end()) {
         it.second().scaleCurrent(scale, round);
         ++it;
      }
   }

public:
   CScore<SCORE_TYPE> & operator [](const unsigned long &index) { return scores[index]; }
   const CScore<SCORE_TYPE> & find(const unsigned long &index) const { return scores.find(index,CScore<SCORE_TYPE>()); }
   friend std::istream & operator >> <> (std::istream &is, CPackedScore &score);
   friend std::ostream & operator << <> (std::ostream &os, const CPackedScore &score);
public:
   static void freePoolMemory() { // call after all instances clean!
      CLinkedList< unsigned long, CScore<SCORE_TYPE> >::freePoolMemory();
   }
};

#endif
