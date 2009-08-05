/***************************************************************
 *
 * The tagger's agenda n chart implementation 
 *
 * Yue Zhang, 2007
 *
 ***************************************************************/

#ifndef TAGGER_AGENDACHART_IMPL_H
#define TAGGER_AGENDACHART_IMPL_H

#include "tagger_base.h"

namespace chinese {

/*===============================================================
 *
 * The implementation-specific part of tagger is defined here.
 *
 *==============================================================*/

class CTagger : public CTaggerBase {

protected:
   CAgendaChart<tagger::CStateItem> m_Chart;
   CWordCache m_WordCache;
   CCharCatDictionary m_Knowledge;
   bool m_bKnowledgeLoaded;
   int m_nScoreIndex;

public:
   CTagger(string sFeatureDBPath, bool bTrain=false) : m_Chart(tagger::AGENDA_SIZE) , m_WordCache(tagger::MAX_SENTENCE_SIZE) , CTaggerBase(sFeatureDBPath, bTrain) , m_bKnowledgeLoaded(false) { 
      if (bTrain) m_nScoreIndex = CScore<tagger::SCORE_TYPE>::eNonAverage; else m_nScoreIndex = CScore<tagger::SCORE_TYPE>::eAverage;
   }
   virtual ~CTagger() { }
   
public:
   virtual void train(const CSentenceRaw *sentence, const CSentenceTagged *correct, int round);
   virtual void tag(const CSentenceRaw *sentence, CSentenceTagged *retval, double *out_scores=NULL, int nBest=1, const CBitArray *prunes=NULL);

   void loadKnowledge(const string &sKnowledgePath) {
      m_Knowledge.load( sKnowledgePath );
      m_bKnowledgeLoaded = true;
   }

   enum SCORE_UPDATE {eSubtract=0, eAdd};

   tagger::SCORE_TYPE getGlobalScore(const CSentenceTagged* tagged);

   void updateScores(const CSentenceTagged* tagged, const CSentenceTagged* correct, int round);

   tagger::SCORE_TYPE getOrUpdateLocalScore(const CSentenceRaw *tagged, const tagger::CStateItem *item, int index, tagger::SCORE_TYPE amount=0, int round=0);

   void finishTraining(int nTotalNumberOfTrainingExamples) { 
      m_weights->computeAverageFeatureWeights(nTotalNumberOfTrainingExamples);
      m_weights->saveScores(); 
   }
};

} // namespace chinese





#endif
