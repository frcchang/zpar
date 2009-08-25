/****************************************************************
 *                                                              *
 * tagger_base.h - the sentence POS tagger.                     *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2006.10                        *
 *                                                              *
 ****************************************************************/

#ifndef _CHINESE_TAGGER_BASE_H
#define _CHINESE_TAGGER_BASE_H 

#include "base_include.h" // the common included modules

/*===============================================================
 *
 * Global items
 *
 *==============================================================*/

namespace chinese {

namespace tagger {

#include "segmentationprune.h"

}; // namespace tagger
}; // namespace chinese

#include "weight.h"

namespace chinese {

#include "wordcache.h"

/*===============================================================
 *
 * CTagger - the tagger for Chinese 
 *
 * Tag a sentence character by character, generating possible
 * tagged sentences into a beam agenda. 
 *
 *==============================================================*/

class CTaggerBase {

protected:
   tagger::CWeight *m_weights;
   int m_nNumberOfCurrentTrainingExample;
   bool m_bTrain;
   const unsigned m_nMaxSentSize;

public:
   CTaggerBase(string sFeatureDBPath, bool bTrain, unsigned nMaxSentenceSize) : m_bTrain(bTrain), m_nMaxSentSize(nMaxSentenceSize), m_nNumberOfCurrentTrainingExample(0) { 
      m_weights = new tagger::CWeight(sFeatureDBPath, bTrain); 
   }
   virtual ~CTaggerBase() { }
   CTaggerBase(CTaggerBase& tagger) : m_nMaxSentSize(0)  { THROW("CTagger does not support copy constructor!"); }

public:
   virtual void loadKnowledge(const string &sKnowledgePath) = 0 ; // load knowledge should be implemented by sub class
   virtual void train(const CStringVector *sentence, const CTwoStringVector *correct, int round) = 0;
   // The input sentence to tag() must be a raw sentence of characters
   // For the tagger that processes segmented input sent, we must set word_ends
   virtual void tag(const CStringVector *sentence, CTwoStringVector *retval, double *out_scores=NULL, int nBest=1, const CBitArray *word_ends=NULL) = 0;
   virtual void finishTraining(int nTotalNumberOfTrainingExamples) = 0 ;
   virtual void updateScores(const CTwoStringVector* tagged, const CTwoStringVector* correct, int round) = 0;
};

}; // namespace chinese

#endif


