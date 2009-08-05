/***************************************************************
 *
 * weight_base.h - the base weight definition for the chinese pos
 *                 tagging modules
 *
 * Yue Zhang, 2007.10
 *
 ***************************************************************/

#ifndef _CHINESE_TAGGER_WEIGHT_BASE_H
#define _CHINESE_TAGGER_WEIGHT_BASE_H

#include "base_include.h"

namespace chinese {

namespace tagger {

/*===============================================================
 *
 * CWeightBase - the definition of weights, in feature
 *
 *==============================================================*/

class CWeightBase {

protected:
   bool m_bTrain;
   string m_sFeatureDB;

public:

   CWeightBase(const string &sFeatureDB, bool bTrain) : m_sFeatureDB(sFeatureDB) , m_bTrain(bTrain) { }
   CWeightBase(const CWeightBase &weightbase) { cerr<<"CWeightBase cannot be copied by constructor"; assert(0==1); }
   virtual ~CWeightBase() { }

};

} // namespace tagger
} // namespace chinese

#endif
