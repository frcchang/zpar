// Copyright (C) University of Oxford 2010
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

public:
   bool m_bTrain;
   std::string m_sFeatureDB;

public:

   CWeightBase(const std::string &sFeatureDB, bool bTrain) : m_sFeatureDB(sFeatureDB), m_bTrain(bTrain) { }
   CWeightBase(const CWeightBase &weightbase) { THROW("CWeightBase cannot be copied by constructor"); }
   virtual ~CWeightBase() { }

};

} // namespace tagger
} // namespace chinese

#endif
