// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                              *
 * score.h - the definitions for the score for averaged ptrn.   *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2007.10                        *
 *                                                              *
 ****************************************************************/

#ifndef _SCORE_H
#define _SCORE_H

template<typename SCORE_TYPE>
class CScore;

template<typename SCORE_TYPE>
std::istream & operator >> (std::istream &is, CScore<SCORE_TYPE> &score) {
   char c ;
   ASSERT(is >> score[0], "The first element of CScore cannot be read.");
   ASSERT((is >> c) && c=='/', "The separator CScore cannot be read");
   ASSERT(is >> score[1], "The second element of CScore cannot be read");
   return is ;
}

template<typename SCORE_TYPE>
std::ostream & operator << (std::ostream &os, const CScore<SCORE_TYPE> &score) {
   return os << score[0] << " / " << score[1] ;
}

/*===============================================================
 *
 * CScore - score definition
 *
 *==============================================================*/

template<typename SCORE_TYPE>
class CScore {

public:
   enum SCORE_AVERAGE { eNonAverage=0 , eAverage=1 } ;

protected:
   SCORE_TYPE current;
   SCORE_TYPE total;
   int lastupdate; // the round number for the last update action

public:
   CScore() : current(0), total(0), lastupdate(0) {/*current=0; total=0; lastupdate=0;*/}
   CScore(const CScore &s1) : current(s1.current), total(s1.total), lastupdate(s1.lastupdate) {/*current=s1.current; total=s1.total; lastupdate=s1.lastupdate;*/}
   virtual ~CScore() {};

   void reset() {current=0; total=0; lastupdate=0;}
   bool empty() const { return current==0 && total==0 && lastupdate==0; }
   bool zero() const { return current==0 && total==0; }
   void operator ++ (int) {current++;}
   void operator -- (int) {current--;}
   SCORE_TYPE &operator [] (const int &n) {if (n==eNonAverage) return current; else if (n==eAverage) return total; else { THROW("SCore only has two component, " << n << " required."); }}
   const SCORE_TYPE &operator [] (const int &n) const {if (n==eNonAverage) return current; else if (n==eAverage) return total; else { THROW("SCore only has two component, " << n << " required."); }}
   const SCORE_TYPE score(const int &n=eNonAverage) const {if (n==eNonAverage) return current; return total;}
   void updateCurrent(const SCORE_TYPE &added, const int &round=0) {assert(round>=lastupdate); if(round>lastupdate){updateAverage(round);lastupdate=round;}current+=added;total+=added; }
   void scaleCurrent(const SCORE_TYPE &scale, const int &round=0) {assert(round>=lastupdate); if(round>lastupdate){updateAverage(round);lastupdate=round;}total-=current;current*=scale;total+=current;}
   //void updateCurrent(SCORE_TYPE added, int round=0) {if (round>=lastupdate){updateAverage(round);total+=added;lastupdate=round;}current+=added; }
   void updateAverage(const int &round=0) {if (round>lastupdate)total += current*(round-lastupdate);
   else if (round<lastupdate) std::cerr << "Round is: "<<round<<"<"<<lastupdate<<std::endl;}
};

#endif
