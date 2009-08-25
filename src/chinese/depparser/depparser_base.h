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

#include "base_include.h"

#include "weight_base.h"

namespace chinese {

/*===============================================================
 *
 * CDepParserBase - the general dependency parser def
 *
 *==============================================================*/

class CDepParserBase {

protected:

   depparser::CWeightBase *m_weights;

   bool m_bTrain ; // the system runs either at training mode or decoding mode

public:
   // constructor and destructor
   CDepParserBase( string sFeatureDBPath , bool bTrain ) : m_bTrain(bTrain) { 
      // do nothing
   }
   virtual ~CDepParserBase() {
      // do nothing
   }
   CDepParserBase( CDepParserBase &depparser) { 
      THROW("CDepParser does not support copy constructor!"); 
   }

public:

   virtual void parse( const CStringVector &sentence , CDependencyTree &retval ) = 0 ;
   virtual void train( const CDependencyTree &correct , int round ) = 0 ;

   virtual void finishtraining() = 0 ;  

};

}; // namespace chinese

#endif


