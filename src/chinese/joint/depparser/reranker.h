// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                              *
 * reranker.h - the reranker for tagged and depparsed outputs   *
 *                                                              *
 * It updates the tagger and deppraser in a unified manner      *
 *                                                              *
 * Yue Zhang                                                    *
 * 2008.03                                                      *
 *                                                              *
 ****************************************************************/

#ifndef _CHINESE_TAGGER_DEPPARSER_PERCEPTRON_RERANKING_H
#define _CHINESE_TAGGER_DEPPARSER_PERCEPTRON_RERANKING_H

#include "tagger.h"
#include "depparser.h"

#include "weight.h"

namespace chinese {

const int MAX_SENTENCE_SIZE = tagger::MAX_SENTENCE_SIZE;

class CReranker {
protected:
   CDepParser *m_depparser;
   CTagger *m_tagger;
   bool m_bTrain;
   int m_nTrainingRound;
   int m_nErrors;
   reranker::CWeight *m_weights;
   int m_nScoreIndex;

public:
   CReranker(const std::string &sFeatureDB, const bool &bTrain) {
      m_depparser = new CDepParser(sFeatureDB+".depparser", bTrain);
      m_tagger = new CTagger(sFeatureDB+".tagger", bTrain);
      m_bTrain = bTrain;
      m_nTrainingRound = 0;
      m_nErrors = 0;
      m_weights = new reranker::CWeight(sFeatureDB+".reranker", bTrain);
      m_nScoreIndex == bTrain ? CScore<double>::eNonAverage : CScore<double>::eAverage;
   }
   virtual ~CReranker() {
      delete m_depparser;
      delete m_tagger;
      delete m_weights;
   }

protected:
   int findBest(const CSentenceParsed *parsed, int nBest, double *prior_scores);
   double getOrUpdatePriorScores(double *prior_scores, int amount);

public:
   void rerank(const CSentenceParsed *parsed, CSentenceParsed *std::cout, int nBest, double *prior_scores) ;
   void train(const CSentenceParsed *parsed, const CSentenceParsed *correct, int nBest, double *prior_scores) ;
   void finishTraining();

};

}

void getFScore(const CSentenceParsed &output, const CSentenceParsed &correct, double &fSeg, double &fTag, double &fPar, double &fParUnlabeled, double &fParIncPunc);




#endif
