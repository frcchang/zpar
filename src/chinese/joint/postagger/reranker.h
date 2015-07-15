// Copyright (C) University of Oxford 2010
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
   CStringVector *m_scratch_seg;
   CTwoStringVector *m_scratch_tag;
   int m_nBest;
   bool m_bTrain;
   int m_nTrainingRound;
   int m_nTrainingErrors;
   int m_nTrainingErrort;
   bool m_bRankIncludeSeg;

public:
   CReranker(const std::string &sFeatureDB, const int &nBest, const bool &bTrain, const bool ranking_segmentor=false) : m_nBest(nBest), m_bTrain(bTrain), m_nTrainingRound(0), m_nTrainingErrors(0), m_nTrainingErrort(0), m_bRankIncludeSeg(false) {
      m_segmentor = new CSegmentor(sFeatureDB+".segmentor", bTrain);
      m_tagger = new CTagger(sFeatureDB+".tagger", bTrain);
      m_scores_seg = new double[nBest];
      m_scores_tag = new double[nBest];
      m_scratch_seg = new CStringVector[nBest];
      m_scratch_tag = new CTwoStringVector[nBest];
   }
   virtual ~CReranker() {
      delete m_segmentor; delete[] m_scores_seg;
      delete m_tagger; delete[] m_scores_tag;
      delete[] m_scratch_seg;
      delete[] m_scratch_tag;
   }

protected:
   int findBest(const CStringVector &raw);

public:
   void decode(const CStringVector &raw, CTwoStringVector &tagged) ;
   void train(const CTwoStringVector &correct) ;
   void train_separate(const CTwoStringVector &correct) ;
   void finishTraining();

};

}
#endif
