/****************************************************************
 *                                                              *
 * reranker.h - the reranker for segmented and tagged outputs   *
 *                                                              *
 * It updates the segmentor and tagger in a unified manner      *
 *                                                              *
 * Yue Zhang                                                    *
 * 2008.03                                                      *
 *                                                              *
 ****************************************************************/

#ifndef _CHINESE_SEGMENTOR_TAGGER_PERCEPTRON_RERANKING_H
#define _CHINESE_SEGMENTOR_TAGGER_PERCEPTRON_RERANKING_H

#include "segmentor.h"
#include "tagger.h"

namespace chinese {

const int MAX_SENTENCE_SIZE = segmentor::MAX_SENTENCE_SIZE;

class CReranker {
protected:
   CSegmentor *m_segmentor;
   CTagger *m_tagger;
   double *m_scores_seg;
   double *m_scores_tag;
   CSentenceRaw *m_scratch_seg;
   CSentenceTagged *m_scratch_tag;
   int m_nBest;
   bool m_bTrain;
   int m_nTrainingRound;
   int m_nTrainingErrors;
   int m_nTrainingErrort;

public:
   CReranker(const string &sFeatureDB, const int &nBest, const bool &bTrain) {
      m_segmentor = new CSegmentor(sFeatureDB+".segmentor", bTrain);
      m_tagger = new CTagger(sFeatureDB+".tagger", bTrain);
      m_nBest = nBest; 
      m_scores_seg = new double[nBest];
      m_scores_tag = new double[nBest];
      m_scratch_seg = new CSentenceRaw[nBest];
      m_scratch_tag = new CSentenceTagged[nBest];
      m_bTrain = bTrain;  
      m_nTrainingRound = 0;
      m_nTrainingErrors = 0;
      m_nTrainingErrort = 0;
   }
   virtual ~CReranker() {
      delete m_segmentor; delete[] m_scores_seg;
      delete m_tagger; delete[] m_scores_tag;
      delete[] m_scratch_seg;
      delete[] m_scratch_tag;
   }

protected:
   int findBest(const CSentenceRaw &raw);

public:
   void decode(const CSentenceRaw &raw, CSentenceTagged &tagged) ;
   void train(const CSentenceTagged &correct) ;
   void train_separate(const CSentenceTagged &correct) ;
   void finishTraining();

};

}
#endif
