// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                              *
 * conparser_base.h - the definition for the constituent parser.*
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2007.10                        *
 *                                                              *
 ****************************************************************/

#ifndef _CONPARSER_BASE_H
#define _CONPARSER_BASE_H

#include "base_include.h"

#include "weight_base.h"

namespace TARGET_LANGUAGE {

/*===============================================================
 *
 * CConParserBase - the general constituent parser def
 *
 *==============================================================*/

class CConParserBase {

protected:

   conparser::CWeightBase *m_weights;

   bool m_bTrain ; // the system runs either at training mode or decoding mode
   std::string m_sFeatureDB;

public:
   // constructor and destructor
   CConParserBase( std::string sFeatureDBPath , bool bTrain ) : m_bTrain(bTrain), m_sFeatureDB(sFeatureDBPath) { 
      // do nothing
   }
   virtual ~CConParserBase() {
      // do nothing
   }
   CConParserBase( CConParserBase &conparser) { 
      THROW("CConParser does not support copy constructor!"); 
   }

public:

//   virtual void parse( const CTwoStringVector &sentence , CSentenceParsed *retval , int nBest=1, conparser::SCORE_TYPE *scores=0 ) = 0 ;
   virtual void train( const CSentenceParsed &correct , int round ) = 0 ;

   virtual void finishtraining() = 0 ;  

};

}; // namespace TARGET_LANGUAGE

#endif


