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

#include "weight_base.h"

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

   const depparser::CSuperTag *m_supertags;

public:
   // constructor and destructor
   CDepParserBase( string sFeatureDBPath , bool bTrain ) : m_bTrain(bTrain) , m_supertags(0) { 
      // do nothing
   }
   virtual ~CDepParserBase() {
      // do nothing
   }
   CDepParserBase( CDepParserBase &depparser) { 
      THROW("CDepParser does not support copy constructor!"); 
   }

public:
   void setSuperTags(const depparser::CSuperTag *supertags) {
      // set sueprtags to 0 if no supertags are to be used
      // set supertags before parsing
      m_supertags = supertags;
   }

public:

   virtual void parse( const CTwoStringVector &sentence , CDependencyParse *retval , int nBest=1, depparser::SCORE_TYPE *scores=0 ) = 0 ;
   virtual void train( const CDependencyParse &correct , int round ) = 0 ;

   virtual void finishtraining() = 0 ;  

};

}; // namespace TARGET_LANGUAGE

#endif


