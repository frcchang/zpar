// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                              *
 * deplabeler_base.h - the definition for the dependency labeler.*
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2007.10                        *
 *                                                              *
 ****************************************************************/

#ifndef _DEPLABELER_BASE_H
#define _DEPLABELER_BASE_H

#include "deplabeler_base_include.h"

#include "weight_base.h"

namespace TARGET_LANGUAGE {

/*===============================================================
 *
 * CDepLabelerBase - the general dependency labeler def
 *
 *==============================================================*/

class CDepLabelerBase {

protected:

   deplabeler::CWeightBase *m_weights;

   bool m_bTrain ; // the system runs either at training mode or decoding mode

public:
   // constructor and destructor
   CDepLabelerBase( std::string sFeatureDBPath , bool bTrain ) : m_bTrain(bTrain) { 
      // do nothing
   }
   virtual ~CDepLabelerBase() {
      // do nothing
   }
   CDepLabelerBase( CDepLabelerBase &deplabeler) { 
      THROW("CDepLabeler does not support copy constructor!"); 
   }

public:

   virtual void label( const CDependencyTree &sentence , CLabeledDependencyTree *retval ) = 0 ;
   virtual void train( const CLabeledDependencyTree &correct ) = 0 ;

   virtual void label_conll( const CCoNLLOutput &sentence , CCoNLLOutput *retval ) = 0 ;
   virtual void train_conll( const CCoNLLOutput &correct ) = 0 ;

   virtual void finishtraining() = 0 ;  

};

}; // namespace TARGET_LANGUAGE

#endif


