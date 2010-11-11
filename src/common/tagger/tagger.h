// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                              *
 * tagger.h - the sentence POS tagger.                          *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2007.10                        *
 *                                                              *
 ****************************************************************/

#ifndef _COMMON_TAGGER_H
#define _COMMON_TAGGER_H

#include "tagger_include.h"

/*===============================================================
 *
 * Global items
 *
 *==============================================================*/

namespace TARGET_LANGUAGE {

namespace tagger {
#include "tagger_impl.h"
#include "tagger_stateitem.h"
}; // namespace tagger
};

#include "tagger_weight.h"

#include "knowledge/tagdict.h" 

namespace TARGET_LANGUAGE {

/*===============================================================
 *
 * CTagger - the tagger for Chinese 
 *
 * Tag a sentence character by character, generating possible
 * tagged sentences into a beam agenda. 
 *
 *==============================================================*/

class CTagger : public tagger::CTaggerImpl {

private:

   std::string m_sFeatureDB;        // the path of the feature database
   tagger::CWeight *m_weights; // weight object

   std::string m_sTagDictPath;      // path for tag dict
   CTagDict<CWord, CTag> m_TagDict;   // the dictionary object

   bool m_bUpdateKnowledgeBase; // update knowledge base when the training example contradicts knowledge base ?

   bool m_bTrain;              // whether training
   int m_nTrainingRound;       // the number of sentence

   bool m_bScoreModified;      // whether the weights are modified by now

   int m_nScoreIndex;

   std::vector<CWord> m_Cache;      // word cache for decoding

public:
   CTagger(const std::string &sFeatureDBPath, bool bTrain=false) : CTaggerImpl() , m_sFeatureDB(sFeatureDBPath) , m_bTrain(bTrain) , m_TagDict(CTag::COUNT) { 
      m_weights = new tagger::CWeight(m_sFeatureDB, bTrain); 
      loadScores();
      if (m_bTrain) m_nTrainingRound = 0;
      if (m_bTrain) m_nScoreIndex = CScore<tagger::SCORE_TYPE>::eNonAverage; else m_nScoreIndex = CScore<tagger::SCORE_TYPE>::eAverage;
   }
   ~CTagger() { delete m_weights; }
   CTagger(CTagger& tagger) : m_TagDict(CTag::COUNT) { std::cerr<<"CTagger does not support copy constructor!"; std::cerr.flush(); assert(1==0); }

public:
   void loadTagDictionary(const std::string &sTagDictPath, bool bUpdateKnowledgeBase=false) {
      m_sTagDictPath = sTagDictPath;
      m_TagDict.load(sTagDictPath);
      m_bUpdateKnowledgeBase = bUpdateKnowledgeBase;
   }

public:
   bool train(const CTwoStringVector *correct);
   void tag(CStringVector *sentence, CTwoStringVector *retval, int nBest=1, double *out_scores=NULL);

protected:
  inline tagger::SCORE_TYPE getGlobalScore(CStringVector* sentence, tagger::CStateItem* item);
  inline tagger::SCORE_TYPE getLocalScore(CStringVector* sentence, tagger::CStateItem* item, unsigned long int index);

  void loadScores();
  void saveScores();

public:
  enum SCORE_UPDATE {eAdd=0, eSubtract};
  // update the built-in weight std::vector for this feature object specifically
  void updateScoreVector(const CTwoStringVector* tagged, const CTwoStringVector* correct, int round=0);
  // compute the total or average feature vector after update
  void finishTraining();

  inline unsigned long long getPossibleTagsForWord(const CWord &word);
  void updateTagDict(const CTwoStringVector* correct);
protected:

   // add local features to a global feature vector (first param)
   void updateLocalFeatureVector(SCORE_UPDATE method, const CTwoStringVector* tagged, int index, int round=0);
};
}; // namespace TARGET_LANGUAGE

#endif


