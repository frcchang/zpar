/***************************************************************
 *                                                             *
 * weight_base.h - the dependency parser weights               *
 *                                                             *
 * Yue Zhang, 2007.10                                          *
 *                                                             *
 ***************************************************************/

#ifndef _DEPPARSER_WEIGHTS_BASE_H
#define _DEPPARSER_WEIGHTS_BASE_H

#include "depparser_base_include.h"

namespace TARGET_LANGUAGE {
namespace depparser {

/*===============================================================
 *
 * CWeightBase - the definition of weights
 *
 *==============================================================*/

class CWeightBase {

protected:
   bool m_bTrain;
   bool m_bModified;
 
   string m_sRecordPath;

public:
   // CONSTRUCTOR 
   CWeightBase(const string &sFile, bool bTrain) : m_bTrain(bTrain) , m_sRecordPath(sFile) { }
   virtual ~CWeightBase() { }

   virtual void loadScores() = 0 ;
   virtual void saveScores() = 0 ; 

};

};
};

#endif
