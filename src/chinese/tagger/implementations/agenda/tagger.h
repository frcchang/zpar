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

namespace tagger {

class CSubStateItem : public CStateItem {

protected:
   SCORE_TYPE sub_score; // score of the last word

public:
   void follow(const CSubStateItem &gold) {
      assert(size()<=gold.size());
      if (size() < gold.size()) {
         const unsigned &last_character = getWordEnd(size()-1);
         const unsigned &gold_last = gold.getWordEnd(size()-1);
         assert(last_character<=gold_last);
         if (last_character<gold_last) {
            replace(last_character+1, CTag::NONE);
            return;
         }
         else {
            append(last_character+1, CTag::NONE);
            setTag(size()-1, gold.getTag(size()-1).code());
            return;
         }
      }
      else { // size() == gold.size()
         setTag(size()-1, gold.getTag(size()-1).code());
         return;
      }
   }

};

}

/*===============================================================
 *
 * The implementation-specific part of tagger is defined here.
 *
 *==============================================================*/

class CTagger : public CTaggerBase {

protected:
   CAgendaBeam<tagger::CSubStateItem> m_Agenda;
   CWordCache m_WordCache;
   tagger::CSubStateItem m_goldState;
   unsigned m_nScoreIndex;

public:
   CTagger(const string &sFeatureDBPath, bool bTrain, unsigned long nMaxSentSize, const string &sKnowledgePath, bool bSegmentationRules) : m_Agenda(tagger::AGENDA_SIZE) , CTaggerBase(sFeatureDBPath, bTrain, nMaxSentSize, sKnowledgePath, bSegmentationRules) , m_WordCache(nMaxSentSize) {
      if (bTrain) m_nScoreIndex = CScore<tagger::SCORE_TYPE>::eNonAverage; else m_nScoreIndex = CScore<tagger::SCORE_TYPE>::eAverage;
      ASSERT(sizeof(unsigned long long)>=CTag::SIZE, "The tagger requires the size of unsigned-long greater than" << CTag::SIZE); // tag dict
   }
   virtual ~CTagger() {}
   
protected:
   void loadKnowledge(const string &sKnowledgePath) {
      cout << "Loading knowledge ... ";
      m_weights->newKnowledge();
      ifstream ifs(sKnowledgePath.c_str());
      if (!ifs) THROW("Knowledge file " << sKnowledgePath << " is not accessible.");
      ifs >> (*m_weights->m_Knowledge); 
      ifs.close();
      cout << "done." << endl;
   }

public:
   enum SCORE_UPDATE {eSubtract=0, eAdd};

   virtual void train(const CStringVector *sentence, const CTwoStringVector *correct);
   virtual void tag(const CStringVector *sentence, CTwoStringVector *retval, double *out_scores=NULL, unsigned long nBest=1, const CBitArray *prunes=NULL);

   tagger::SCORE_TYPE getGlobalScore(const CTwoStringVector* tagged);

   void updateScores(const CTwoStringVector* tagged, const CTwoStringVector* correct, unsigned long round);

   tagger::SCORE_TYPE getOrUpdateLocalScore(const CStringVector *tagged, const tagger::CSubStateItem *item, unsigned long index, tagger::SCORE_TYPE amount=0, unsigned long round=0);

   void finishTraining(unsigned long nTotalNumberOfTrainingExamples) { 
      m_weights->computeAverageFeatureWeights(nTotalNumberOfTrainingExamples);
      m_weights->saveScores(); 
   }
};

} // namespace chinese





#endif
