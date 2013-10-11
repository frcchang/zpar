// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                              *
 * depparser_base.h - the definition for the dependency parser. *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2007.10                        *
 *                                                              *
 ****************************************************************/

#ifndef _DEPPARSER_BASE_H
#define _DEPPARSER_BASE_H

#include "depparser_base_include.h"

#include "depparser_weight_base.h"

namespace TARGET_LANGUAGE {

/*===============================================================
 *
 * CDepParserBase - the general dependency parser def
 *
 *==============================================================*/

class CDepParserBase {

protected:

   depparser::CWeightBase *m_weights;

   bool m_bTrain ; // the system runs either at training mode or decoding mode
   bool m_bCoNLL ;

   const depparser::CSuperTag *m_supertags;

public:
   // constructor and destructor
   CDepParserBase( std::string sFeatureDBPath , bool bTrain , bool bCoNLL ) : m_bTrain(bTrain) , m_bCoNLL(bCoNLL) , m_supertags(0) { 
      // do nothing
   }
   virtual ~CDepParserBase() {
      // do nothing
      m_supertags = 0;
   }
   CDepParserBase( CDepParserBase &depparser) { 
      THROW("CDepParser does not support copy constructor!"); 
   }

public:
   void setRules(const bool &bRules) {
      m_weights->setRules(bRules);
   }
   void setSuperTags(const depparser::CSuperTag *supertags) {
      // set sueprtags to 0 if no supertags are to be used
      // set supertags before parsing
      m_supertags = supertags;
   }

public:

#ifdef JOINT_MORPH
   virtual void parse( const CStringVector &sentence , CDependencyParse *retval , int nBest=1, depparser::SCORE_TYPE *scores=0 ) = 0 ;
#else
   virtual void parse( const CTwoStringVector &sentence , CDependencyParse *retval , int nBest=1, depparser::SCORE_TYPE *scores=0 ) = 0 ;
#endif
   virtual void train( const CDependencyParse &correct , int round ) = 0 ;

   virtual void parse_conll( const CCoNLLInput &sentence , CCoNLLOutput *retval , int nBest=1, depparser::SCORE_TYPE *scores=0 ) {
      THROW("depparser_base.h: the method parse_conll is not implemented");
   }
   virtual void train_conll( const CCoNLLOutput &correct , int round ) {
      THROW("depparser_base.h: the method train_conll is not implemented");
   }

   virtual void finishtraining() = 0 ;  

};

}; // namespace TARGET_LANGUAGE

#endif


