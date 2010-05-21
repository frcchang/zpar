/***************************************************************
 *                                                             *
 * weight_base.h - the constituent parser weights              *
 *                                                             *
 * Yue Zhang, 2007.10                                          *
 *                                                             *
 ***************************************************************/

#ifndef _CONPARSER_WEIGHTS_BASE_H
#define _CONPARSER_WEIGHTS_BASE_H

#include "base_include.h"

namespace TARGET_LANGUAGE {
namespace conparser {

/*===============================================================
 *
 * CWeightBase - the definition of weights
 *
 *==============================================================*/

class CWeightBase {

protected:
   bool m_bTrain;
//   bool m_bModified;
   bool m_bEmpty;
 
//   string m_sRecordPath;

public:
   // CONSTRUCTOR 
   CWeightBase(bool bTrain) : m_bTrain(bTrain) , m_bEmpty(true) {}
   virtual ~CWeightBase() { }

   virtual void loadScores(const ifstream &is) = 0 ;
   virtual void saveScores(ofstream &os) = 0 ; 

   bool empty() const {return m_bEmpty;}
};

};
};

#endif
