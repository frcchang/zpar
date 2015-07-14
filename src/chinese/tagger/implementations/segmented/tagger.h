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
   CAgendaBeam<tagger::CStateItem> m_Agenda;
   CWordCache m_WordCache;
   int m_nScoreIndex;

public:
   CTagger(std::string sFeatureDBPath, bool bTrain, unsigned long nMaxSentSize, bool bSegmentationRules) : m_Agenda(tagger::AGENDA_SIZE) , m_WordCache(nMaxSentSize) , CTaggerBase(sFeatureDBPath, bTrain, nMaxSentSize, bSegmentationRules) {
      if (bTrain) m_nScoreIndex = CScore<tagger::SCORE_TYPE>::eNonAverage; else m_nScoreIndex = CScore<tagger::SCORE_TYPE>::eAverage;
   }
   virtual ~CTagger() {}

public:
   void loadKnowledge(const std::string &sKnowledgePath) {
      THROW("The segmented pos tagger does not support knowledges.");
   }

   virtual bool train(const CStringVector *sentence, const CTwoStringVector *correct);
   virtual void tag(const CStringVector *sentence, CTwoStringVector *retval, double *out_scores=NULL, unsigned long nBest=1, const CBitArray *prunes=NULL);

   enum SCORE_UPDATE {eSubtract=0, eAdd};

   tagger::SCORE_TYPE getLocalScore(const CStringVector*, tagger::CStateItem*, unsigned long);

   void updateScores(const CTwoStringVector* tagged, const CTwoStringVector* correct, unsigned long round);
   void updateLocalFeatureVector(SCORE_UPDATE method, const CTwoStringVector* output, unsigned long index, unsigned long round);

   void finishTraining(unsigned long nTotalNumberOfTrainingExamples) {
      m_weights->computeAverageFeatureWeights(nTotalNumberOfTrainingExamples);
      m_weights->saveScores();
   }
};

} // namespace chinese





#endif
