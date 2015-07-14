// Copyright (C) University of Oxford 2010
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
   unsigned m_nScoreIndex;

public:
   CTagger(const std::string &sFeatureDBPath, bool bTrain, unsigned long nMaxSentSize, const std::string &sKnowledgePath, bool bSegmentationRules) : m_Chart(tagger::AGENDA_SIZE) , CTaggerBase(sFeatureDBPath, bTrain, nMaxSentSize, sKnowledgePath, bSegmentationRules) , m_WordCache(nMaxSentSize) {
      if (bTrain) m_nScoreIndex = CScore<tagger::SCORE_TYPE>::eNonAverage; else m_nScoreIndex = CScore<tagger::SCORE_TYPE>::eAverage;
   }
   virtual ~CTagger() {}

protected:
   void loadKnowledge(const std::string &sKnowledgePath) {
      std::cerr << "Loading knowledge ... ";
      m_weights->newKnowledge();
      std::ifstream ifs(sKnowledgePath.c_str());
      if (!ifs) THROW("Knowledge file " << sKnowledgePath << " is not accessible.");
      ifs >> (*m_weights->m_Knowledge);
      ifs.close();
      std::cerr << "done." << std::endl;
   }

public:
   enum SCORE_UPDATE {eSubtract=0, eAdd};

   virtual bool train(const CStringVector *sentence, const CTwoStringVector *correct);
   virtual void tag(const CStringVector *sentence, CTwoStringVector *retval, double *out_scores=NULL, unsigned long nBest=1, const CBitArray *prunes=NULL);

   tagger::SCORE_TYPE getGlobalScore(const CTwoStringVector* tagged);

   void updateScores(const CTwoStringVector* tagged, const CTwoStringVector* correct, unsigned long round);

   tagger::SCORE_TYPE getOrUpdateLocalScore(const CStringVector *tagged, const tagger::CStateItem *item, unsigned long index, tagger::SCORE_TYPE amount=0, unsigned long round=0);

   void finishTraining(unsigned long nTotalNumberOfTrainingExamples) {
      m_weights->computeAverageFeatureWeights(nTotalNumberOfTrainingExamples);
      m_weights->saveScores();
   }
};

} // namespace chinese





#endif
