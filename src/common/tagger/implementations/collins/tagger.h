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

   CTagDict<CWord, CTag> *m_TagDict;   // the dictionary object

   bool m_bTrain;              // whether training
   int m_nTrainingRound;       // the number of sentence

   bool m_bScoreModified;      // whether the weights are modified by now

   int m_nScoreIndex;

   std::vector<CWord> m_Cache;      // word cache for decoding
   std::vector<unsigned> m_CacheTags; // tag cache for training

   unsigned m_nMaxSentenceSize;
   tagger::CStateItem *stateitems;
   unsigned *stateindice;

   unsigned long long m_opentags;

public:
   CTagger(const std::string &sFeatureDBPath, bool bTrain=false) : CTaggerImpl() , m_sFeatureDB(sFeatureDBPath) , m_bTrain(bTrain) , m_TagDict(0), m_nMaxSentenceSize(tagger::MAX_SENTENCE_SIZE), m_opentags(~0LL) { 
      m_weights = new tagger::CWeight(m_sFeatureDB, bTrain); 
      loadScores();
      if (m_bTrain) m_nTrainingRound = 0;
      if (m_bTrain) m_nScoreIndex = CScore<tagger::SCORE_TYPE>::eNonAverage; else m_nScoreIndex = CScore<tagger::SCORE_TYPE>::eAverage;
      stateitems = new tagger::CStateItem[tagger::AGENDA_SIZE*m_nMaxSentenceSize];
      stateindice = new unsigned[m_nMaxSentenceSize];
   }
   ~CTagger() { 
      delete m_weights; 
      delete []stateitems;
      delete []stateindice;
      if (m_TagDict) delete m_TagDict;
   }
   CTagger(CTagger& tagger) : m_TagDict(0) { ASSERT(1==0, "No copy constructor."); }

public:
   void loadTagDictionary(const std::string &sTagDictPath) {
      // load dictionary
      ASSERT(m_TagDict==0, "The tag dict has already been loaded");
      m_TagDict = new CTagDict<CWord, CTag>(CTag::COUNT);
      m_TagDict->load(sTagDictPath);
      // enumerate only open set tags
      m_opentags = ~m_opentags;
      ASSERT(m_opentags==0LL, "Open tags must be unitialized");
      for (unsigned tag=CTag::FIRST; tag<CTag::COUNT; ++tag)
         if (CTag(tag).closed()) {std::cout << CTag(tag) << std::endl; m_opentags |= (1LL << tag); }
      m_opentags = ~m_opentags;
   }

public:
   bool train(const CTwoStringVector *correct);
   void tag(CStringVector *sentence, CTwoStringVector *retval, int nBest=1, double *out_scores=NULL);

protected:
  inline void getLocalScore(CPackedScoreType<tagger::SCORE_TYPE, CTag::MAX_COUNT> &retval, const CStringVector* sentence, const tagger::CStateItem* item, const unsigned long &index);

  void loadScores();
  void saveScores();

public:
  enum SCORE_UPDATE {eAdd=0, eSubtract};
  // update the built-in weight std::vector for this feature object specifically
  void updateScoreVector(const CTwoStringVector* tagged, const CTwoStringVector* correct, int round=0);
  // compute the total or average feature vector after update
  void finishTraining();

  inline unsigned long long getPossibleTagsForWord(const CWord &word);
protected:

   // add local features to a global feature vector (first param)
   void updateLocalFeatureVector(SCORE_UPDATE method, const CTwoStringVector* tagged, int index, int round=0);
};
}; // namespace TARGET_LANGUAGE

#endif


