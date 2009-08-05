/***************************************************************
 *
 * weight.h - the dependency parser weights 
 *
 * Yue Zhang, 2008.10
 *
 ***************************************************************/

#ifndef _RERANKING_IMPL_WEIGHTS_H
#define _RERANKING_IMPL_WEIGHTS_H

#include "definitions.h"
#include "learning/perceptron/score.h"
#include "learning/perceptron/hashmap_score.h"

namespace chinese {

namespace reranker {

typedef CScoreMap< int, double > CIntMap;

/*===============================================================
 *
 * CWeight - the definition of weights, in feature
 *
 *==============================================================*/

class CWeight {

protected:

   string m_sRecordPath;

public:

   CIntMap m_mapPriors;

public:

   CWeight(const string &sPath, bool bTrain) : m_mapPriors("Priors", 2)
   { 
      m_sRecordPath = sPath;
      loadScores(); 
   }

   virtual void loadScores(); 
   virtual void saveScores(); 
   void computeAverageFeatureWeights(int round);
 
};

};
};

#endif
