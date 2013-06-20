// Copyright (C) University of Oxford 2010
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
   unsigned m_nNumberOfCurrentTrainingExample;
   bool m_bTrain;
   const unsigned long m_nMaxSentSize;
   int m_nTrainingRound;

public:
   CTaggerBase(const std::string &sFeatureDBPath, bool bTrain, unsigned long nMaxSentenceSize, bool bSegmentationRules) : m_bTrain(bTrain), m_nMaxSentSize(nMaxSentenceSize), m_nNumberOfCurrentTrainingExample(0) , m_nTrainingRound(0) { 
      // load features
      m_weights = new tagger::CWeight(sFeatureDBPath, bTrain, bSegmentationRules, 65537);
      // load knowledge
//      if (!bTrain) {
//         // when decoding
//         if (!sKnowledgePath.empty())
//            THROW("CTaggerBase::CTaggerBase received sKnowledgePath file in decoding mode, which is unexpected.");
//      }
//      else {
//         // first time training
//         if (FileExists(sFeatureDBPath)==false) {
//            // define items for new db
//            if (!sKnowledgePath.empty()) loadKnowledge(sKnowledgePath);
//         }
//         else { // model already exists
//            if (!sKnowledgePath.empty())
//               THROW("CTaggerBase::CTaggerBase received sKnowledgePath file, but model exists (with knowledge)");
//         }
//      }
   }
   virtual ~CTaggerBase() { delete m_weights; }
   CTaggerBase(CTaggerBase& tagger) : m_nMaxSentSize(0)  { THROW("CTagger does not support copy constructor!"); }

public:
   virtual void loadKnowledge(const std::string &sKnowledgePath) {
      THROW("CTaggerBase: the tagger class didn't override loadKnowledge."); // load knowledge should be implemented by sub class
   }

   virtual bool train(const CStringVector *sentence, const CTwoStringVector *correct) = 0;
   void dumpfeatures(const CStringVector *sentence, const CTwoStringVector *correct)  {
      THROW("Not supported.");
   }
   // The input sentence to tag() must be a raw sentence of characters
   // For the tagger that processes segmented input sent, we must set word_ends
   virtual void tag(const CStringVector *sentence, CTwoStringVector *retval, double *out_scores=NULL, unsigned long nBest=1, const CBitArray *word_ends=NULL) = 0;
   virtual void finishTraining(unsigned long nTotalNumberOfTrainingExamples) = 0 ;
   virtual void updateScores(const CTwoStringVector* tagged, const CTwoStringVector* correct, unsigned long round) = 0;
};

}; // namespace chinese

#endif


