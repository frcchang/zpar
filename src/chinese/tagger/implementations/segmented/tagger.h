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
   tagger::CWordIntMap *m_mapCharCat;
   int m_nScoreIndex;

public:
   CTagger(string sFeatureDBPath, bool bTrain=false, unsigned nMaxSentSize=tagger::MAX_SENTENCE_SIZE) : m_Agenda(tagger::AGENDA_SIZE) , m_WordCache(nMaxSentSize) , CTaggerBase(sFeatureDBPath, bTrain, nMaxSentSize) { 
      m_mapCharCat = NULL;
      if (bTrain) m_nScoreIndex = CScore<tagger::SCORE_TYPE>::eNonAverage; else m_nScoreIndex = CScore<tagger::SCORE_TYPE>::eAverage;
   }
   virtual ~CTagger() { if (m_mapCharCat) delete m_mapCharCat ; }
   
public:
   virtual void train(const CStringVector *sentence, const CTwoStringVector *correct, int round);
   virtual void tag(const CStringVector *sentence, CTwoStringVector *retval, double *out_scores=NULL, int nBest=1, const CBitArray *prunes=NULL);

   void loadKnowledge(const string &sKnowledgePath) {
   }

   enum SCORE_UPDATE {eSubtract=0, eAdd};

   tagger::SCORE_TYPE getLocalScore(const CStringVector*, tagger::CStateItem*, int);

   void updateScores(const CTwoStringVector* tagged, const CTwoStringVector* correct, int round);
   void updateLocalFeatureVector(SCORE_UPDATE method, const CTwoStringVector* output, int index, int round);

   void finishTraining(int nTotalNumberOfTrainingExamples) { 
      m_weights->computeAverageFeatureWeights(nTotalNumberOfTrainingExamples);
      m_weights->saveScores(); 
   }
};

} // namespace chinese





#endif
