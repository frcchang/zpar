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
istream & operator >> (istream &is, CScore<SCORE_TYPE> &score) {
   char c ;
   is >> score[0] >> c >> score[1] ;
   return is ;
}

template<typename SCORE_TYPE>
ostream & operator << (ostream &os, CScore<SCORE_TYPE> &score) {
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
   CScore() {current=0; total=0; lastupdate=0;}
   CScore(const CScore &s1) {current=s1.current; total=s1.total; lastupdate=s1.lastupdate;}
   virtual ~CScore() {};

   void reset() {current=0; total=0; lastupdate=0;}
   void operator ++ (int) {current++;}
   void operator -- (int) {current--;}
   SCORE_TYPE &operator [] (int n) {if (n==eNonAverage) return current; else if (n==eAverage) return total; else { REPORT("SCore only has two component, " << n << " required."); assert(1==0);}}
   const SCORE_TYPE score(int n=eNonAverage) const {if (n==eNonAverage) return current; return total;}
   void updateCurrent(SCORE_TYPE added, int round=0) {assert(round>=lastupdate); if(round>lastupdate){updateAverage(round);lastupdate=round;}current+=added;total+=added; }
   //void updateCurrent(SCORE_TYPE added, int round=0) {if (round>=lastupdate){updateAverage(round);total+=added;lastupdate=round;}current+=added; }
   void updateAverage(int round=0) {if (round>lastupdate)total += current*(round-lastupdate);
   else if (round<lastupdate) cout << "Round is: "<<round<<"<"<<lastupdate<<endl;}
};

#endif
