// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                              *
 * depparser.h - the definitions for the dependency parser.     *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2007.8                         *
 *                                                              *
 ****************************************************************/

#ifndef _DEPPARSER_IMPL_H
#define _DEPPARSER_IMPL_H

#include "depparser_base.h"

/*===============================================================
 *
 * Global items
 *
 *==============================================================*/

namespace TARGET_LANGUAGE {

namespace depparser {

#include "state.h"

}; // namespace depparser
};

#include "depparser_weight.h"

namespace TARGET_LANGUAGE {

/*===============================================================
 *
 * CDepParser - the dependency parser for English
 *
 *==============================================================*/

class CDepParser : public CDepParserBase {

private:

   std::vector< CTaggedWord<CTag, TAG_SEPARATOR> > m_lCache;
   int m_nTrainingRound;
   int m_nTotalErrors;
   bool m_bScoreModified;
   int m_nScoreIndex;

public:
   // constructor and destructor
   CDepParser( const std::string &sFeatureDBPath , bool bTrain , bool bCoNLL=false ) : CDepParserBase(sFeatureDBPath, bTrain, bCoNLL) {
      m_weights = new depparser :: CWeight(sFeatureDBPath, bTrain );
      m_nTrainingRound = 0;
      m_nTotalErrors = 0;
      m_nScoreIndex = CScore<depparser::SCORE_TYPE>::eNonAverage;
      if (bTrain==false) m_nScoreIndex = CScore<depparser::SCORE_TYPE>::eAverage;
   }
   ~CDepParser() {
      delete m_weights;
   }
   CDepParser( CDepParser &depparser) : CDepParserBase(depparser) {
      THROW("CDepParser: copy constructor Not supported!");
   }

public:
   void parse( const CTwoStringVector &sentence , CDependencyParse *retval , int nBest=1, depparser::SCORE_TYPE *scores=0 ) ;
   void train( const CDependencyParse &correct , int round ) ;

   void finishtraining() {
      static_cast<depparser::CWeight*>(m_weights)->computeAverageFeatureWeights(m_nTrainingRound);
      static_cast<depparser::CWeight*>(m_weights)->saveScores();
      std::cerr << "Total number of training errors are: " << m_nTotalErrors << std::endl;
   }

   depparser::SCORE_TYPE getGlobalScore(const CDependencyParse &parsed);
   void updateScores(const CDependencyParse &parse, const CDependencyParse &correct, int round=0);

private:
   enum SCORE_UPDATE {eAdd=0, eSubtract};

   // get the global score for a parsed sentence or section
   inline depparser::SCORE_TYPE getCrossLinkScore(const depparser::CSpan *span);
   inline depparser::SCORE_TYPE getCrossLinkScore(const int head, const int dep);

   // update the built-in weight std::vector for this feature object specifically
   void updateScoreVector(const CDependencyParse &parse, const CDependencyParse &correct, int round=0);
   void updateCrossLinkScore(int left_index, int right_index, const CDependencyParse &tree, SCORE_UPDATE method, int round=0);

};

}; // namespace TARGET_LANGUAGE

#endif


