// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                              *
 * conparser.h - the definitions for the constituent parser.    *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2007.8                         *
 *                                                              *
 ****************************************************************/

#ifndef _CONPARSER_IMPL_H
#define _CONPARSER_IMPL_H

#include "conparser_base.h"
#include "weight.h"

namespace TARGET_LANGUAGE {

/*===============================================================
 *
 * CConParser - the constituent parser
 *
 *==============================================================*/

class CConParser : public CConParserBase {

private:
   CWordCache m_lCache;
   std::vector<unsigned long> m_lWordLen;
   int m_nTrainingRound;
   int m_nTotalErrors;
   bool m_bScoreModified;
   int m_nScoreIndex;
   conparser::CRule *m_rule;
   conparser::CContext m_Context;
#ifdef TRAIN_MULTI
   conparser::CWeight *m_gold;
   conparser::CWeight *m_delta[conparser::MIRA_SIZE];
   conparser::SCORE_TYPE m_dist[conparser::MIRA_SIZE];
   conparser::SCORE_TYPE m_alpha[conparser::MIRA_SIZE];
#else
   conparser::CWeight *m_delta;
#endif

public:
   // constructor and destructor
   CConParser( const std::string &sFeatureDBPath, unsigned long nMaxSentSize , bool bTrain ) : CConParserBase(sFeatureDBPath, bTrain), m_lCache(nMaxSentSize) {
      // and initialize the weith module loading content
      m_weights = new conparser :: CWeight( bTrain );
      conparser :: CWeight * m_weights = dynamic_cast<conparser :: CWeight*>(this->m_weights);
      m_rule = new conparser::CRule(m_weights->m_maxlengthByTag, &(m_weights->m_nMaxWordFrequency), &(m_weights->m_mapTagDictionary), &(m_weights->m_mapWordFrequency), &(m_weights->m_mapCanStart)
      		, &(m_lCache), m_weights->m_Knowledge, &(m_weights->m_mapPartWordFrequency), &(m_weights->m_mapWordHeadDictionary));
      if (bTrain) {

#ifdef TRAIN_MULTI //===
         m_gold = new conparser::CWeight( bTrain, 256 );
         for (unsigned i=0; i<conparser::MIRA_SIZE; ++i) {
            m_delta[i] = new conparser::CWeight( bTrain, 256 );
         }
#ifdef NO_NEG_FEATURE
         m_gold->setPositiveFeature(static_cast<conparser::CWeight*>(m_weights));
         for (unsigned i=0; i<conparser::MIRA_SIZE; ++i) {
            m_delta[i]->setPositiveFeature(static_cast<conparser::CWeight*>(m_weights));
         }
#endif // NO_NEG_FEATURE

#else // ===
         m_delta = new conparser::CWeight( bTrain, 256 );
#ifdef NO_NEG_FEATURE
         m_delta->setPositiveFeature(static_cast<conparser::CWeight*>(m_weights));
#endif // NO_NEG_FEATURE

#endif // === TRAIN_MULTI
      }
      else {
#ifdef TRAIN_MULTI
         m_gold = 0;
         for (unsigned i=0; i<conparser::MIRA_SIZE; ++i)
            m_delta[i] = 0;
#else
         m_delta = 0;
#endif
      }

      std::ifstream file;
      file.open(sFeatureDBPath.c_str());
      m_weights->loadScores(file);
      // load rules
      CConstituent c;
      file.close();
      // initialize
      if (!bTrain && m_weights->empty()) { // when decoding, model must be found
         THROW("The model file " << sFeatureDBPath<< " is not found.")
      }
      m_nTrainingRound = 0;
      m_nTotalErrors = 0;
      if (bTrain) m_nScoreIndex = CScore<conparser::SCORE_TYPE>::eNonAverage ; else m_nScoreIndex = CScore<conparser::SCORE_TYPE>::eAverage ;
//      ASSERT(conparser::CAction::MAX<=(1LL<<(sizeof(unsigned)*8)), "conparser.h: The size of action is too big for the packed scoretype");
   }

   ~CConParser() {
#ifdef TRAIN_MULTI
      if (m_gold) { delete m_gold; m_gold=0; }
      for (unsigned i=0; i<conparser::MIRA_SIZE; ++i) {
         if (m_delta[i]) {
            delete m_delta[i];
            m_delta[i] = 0;
         }
      }
#else
      if (m_delta) { delete m_delta; m_delta=0; }
#endif
      delete m_rule;
   }

   /*
   CConParser( CConParser &conparser) : CConParserBase(conparser){
      assert(1==0);
      //m_weights = new conparser :: CWeight( bTrain );
      //m_rule = new conparser::CRule(m_weights->m_maxlengthByTag,&m_weights->m_mapWordFrequency, &m_weights->m_mapTagDictionary, &m_weights->m_mapWordFrequency, &m_weights->m_mapCanStart, &m_lCache);

   }
*/

public:
   bool parse( const CStringVector &sentence , CSentenceParsed *retval , int bUsegGoldSeg, int nBest=1,  conparser::SCORE_TYPE *scores=0, const CStringVector *charcandpos=NULL ) ;
   void train( const CSentenceParsed &correct , int round ) ;
#ifdef NO_NEG_FEATURE
   void getPositiveFeatures( const CSentenceParsed &correct ) ;
#endif

   void finishtraining() {
      // compute average
      static_cast<conparser::CWeight*>(m_weights)->computeAverageFeatureWeights(m_nTrainingRound);
      // save scores
      std::ofstream file ;
      file.open(m_sFeatureDB.c_str()) ;
      static_cast<conparser::CWeight*>(m_weights)->saveScores(file);
      // save rules
      std::cerr << "Total number of training errors are: " << m_nTotalErrors << std::endl;
   }
   conparser::SCORE_TYPE getGlobalScore(const CSentenceParsed &parsed);
   void updateScores(const CSentenceParsed &parse, const CSentenceParsed &correct, int round=0);

private:
   enum SCORE_UPDATE {eAdd=0, eSubtract};

   bool work( const bool bTrain, const CStringVector &sentence , CSentenceParsed *retval, const CSentenceParsed &correct, int nBest, conparser::SCORE_TYPE *scores, const CStringVector *charcandpos=NULL ) ;

   // get the global score for a parsed sentence or section
   inline void getOrUpdateStackScore( conparser::CWeight *cast_weights, CPackedScoreType<conparser::SCORE_TYPE, conparser::CAction::MAX> &retval, const conparser::CStateItem *item, const conparser::CAction &action=conparser::CAction(), conparser::SCORE_TYPE amount=0, int round=0 );
   inline void getOrUpdateScore( CPackedScoreType<conparser::SCORE_TYPE, conparser::CAction::MAX> &retval, const conparser::CStateItem &item, const conparser::CAction &action=conparser::CAction(), conparser::SCORE_TYPE amount=0, int round=0 );

   // update the built-in weight vector for this feature object specifically
   void updateScoresForState( conparser::CWeight *cast_weights , const conparser::CStateItem *output , const CStringVector &sentence , SCORE_UPDATE update) ;

   void updateScoresForStates( const conparser::CStateItem *output , const CStringVector &sentence , const conparser::CStateItem *correct ) ;

#ifdef TRAIN_LOSS
   // loss functions
//   double computeLossF(const conparser::CStateItem* item);
   void getLabeledBrackets(const CSentenceParsed &parse_tree, CStack<CLabeledBracket> &brackets);
void updateScoresByLoss( const conparser::CStateItem *output , const conparser::CStateItem *correct );
#endif

};

}; // namespace TARGET_LANGUAGE

#endif


