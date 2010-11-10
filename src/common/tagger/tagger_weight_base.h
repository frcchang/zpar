// Copyright (C) University of Oxford 2010
/***************************************************************
 *
 * weight_base.h - the base weight definition for the pos
 *                 tagging modules
 *
 * Yue Zhang, 2007.10
 *
 ***************************************************************/

#ifndef _COMMON_TAGGER_WEIGHT_BASE_H
#define _COMMON_TAGGER_WEIGHT_BASE_H

namespace TARGET_LANGUAGE {

namespace tagger {

/*===============================================================
 *
 * CWeightBase - the definition of weights, in feature
 *
 *==============================================================*/

class CWeightBase {

protected:
   bool m_bTrain;
   std::string m_sFeatureDB;

public:

   CWeightBase(const std::string &sFeatureDB, bool bTrain) : m_sFeatureDB(sFeatureDB) , m_bTrain(bTrain) { }
   CWeightBase(const CWeightBase &weightbase) { std::cerr<<"CWeightBase cannot be copied by constructor"; assert(0==1); }
   virtual ~CWeightBase() { }

};

}; // namespace tagger
}; // namespace TARGET_LANGUAGE

#endif
