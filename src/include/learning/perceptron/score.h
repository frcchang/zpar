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
#ifdef PERCEPTRON_FOR_DECODING
   SCORE_TYPE tmp;
   ASSERT(is >> tmp, "The first element of CScore cannot be read.");
#else
   ASSERT(is >> score[0], "The first element of CScore cannot be read.");
#endif
   ASSERT((is >> c) && c=='/', "The separator CScore cannot be read");
   ASSERT(is >> score[1], "The second element of CScore cannot be read");
   return is ;
}

template<typename SCORE_TYPE>
std::ostream & operator << (std::ostream &os, const CScore<SCORE_TYPE> &score) {
#ifdef PERCEPTRON_FOR_DECODING
   THROW("Internal error: score.h; cannot store score value when the class is decoding only.");
   return os;
#else
   return os << score[0] << " / " << score[1] ;
#endif
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

   SCORE_TYPE total;

#ifndef PERCEPTRON_FOR_DECODING
   SCORE_TYPE current;
   int lastupdate; // the round number for the last update action
#endif

public:
   CScore() : total(0)
#ifndef PERCEPTRON_FOR_DECODING
               , current(0)
               , lastupdate(0)
#endif
   {/*current=0; total=0; lastupdate=0;*/}

   CScore(const CScore &s1) : total(s1.total)
#ifndef PERCEPTRON_FOR_DECODING
                              , current(s1.current)
                              , lastupdate(s1.lastupdate)
#endif
   {/*current=s1.current; total=s1.total; lastupdate=s1.lastupdate;*/}

   virtual ~CScore() {};

   void reset() {
      total=0;
#ifndef PERCEPTRON_FOR_DECODING
      current=0;
      lastupdate=0;
#endif
   }

   bool empty() const {
      return total==0
#ifdef PERCEPTRON_FOR_DECODING
             ;
#else
             && current==0 && lastupdate==0;
#endif
   }

   bool zero() const {
      return total == 0
#ifdef PERCEPTRON_FOR_DECODING
             ;
#else
             && current==0;
#endif
}

   void operator ++ (int) {
#ifdef PERCEPTRON_FOR_DECODING
      THROW("Internal error: score.h; cannot modify the content of score in decoding mode.");
#else
      current++;
#endif
   }
   void operator -- (int) {
#ifdef PERCEPTRON_FOR_DECODING
      THROW("Internal error: score.h; cannot modify the content of score in decoding mode.");
#else
      current--;
#endif
   }

   SCORE_TYPE &operator [] (const int &n) {
#ifdef PERCEPTRON_FOR_DECODING
      return total;
#else
      if (n==eNonAverage) return current; else if (n==eAverage) return total; else { THROW("SCore only has two component, " << n << " required."); }
#endif
   }
   const SCORE_TYPE &operator [] (const int &n) const {
#ifdef PERCEPTRON_FOR_DECODING
      return total;
#else
      if (n==eNonAverage) return current; else if (n==eAverage) return total; else { THROW("SCore only has two component, " << n << " required."); }
#endif
   }
   const SCORE_TYPE &score(const int &n=eNonAverage) const {
#ifdef PERCEPTRON_FOR_DECODING
      return total;
#else
      if (n==eNonAverage) return current; return total;
#endif
   }

   void updateCurrent(const SCORE_TYPE &added, const int &round=0) {
#ifdef PERCEPTRON_FOR_DECODING
      THROW("Internal error: score.h; cannot modify the content of score in decoding mode.");
#else
      assert(round>=lastupdate); if(round>lastupdate){updateAverage(round);lastupdate=round;}current+=added;total+=added;
#endif
   }

   void scaleCurrent(const SCORE_TYPE &scale, const int &round=0) {
#ifdef PERCEPTRON_FOR_DECODING
      THROW("Internal error: score.h; cannot modify the content of score in decoding mode.");
#else
      assert(round>=lastupdate); if(round>lastupdate){updateAverage(round);lastupdate=round;}total-=current;current*=scale;total+=current;
#endif
   }

   void updateAverage(const int &round=0) {
#ifdef PERCEPTRON_FOR_DECODING
      THROW("Internal error: score.h; cannot modify the content of score in decoding mode.");
#else
      if (round>lastupdate)total += current*(round-lastupdate) ;
      else if (round<lastupdate) std::cerr << "Round is: "<<round<<"<"<<lastupdate<<std::endl;
#endif
   }

};

#endif
