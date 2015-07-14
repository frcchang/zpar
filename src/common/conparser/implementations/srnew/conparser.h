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

   std::vector< CTaggedWord<CTag, TAG_SEPARATOR> > m_lCache;
   std::vector<unsigned long> m_lWordLen;
   int m_nTrainingRound;
   int m_nTotalErrors;
   bool m_bScoreModified;
   int m_nScoreIndex;
   conparser::CRule m_rule;
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
   CConParser( const std::string &sFeatureDBPath , bool bTrain ) : CConParserBase(sFeatureDBPath, bTrain), m_rule(&m_lCache) {
      // and initialize the weith module loading content
      m_weights = new conparser :: CWeight( bTrain );
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
      m_rule.loadRules(file);
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
      delete m_weights; m_weights=0;
   }

   CConParser( CConParser &conparser) : CConParserBase(conparser), m_rule(&m_lCache) {
      assert(1==0);
   }

public:
   void LoadBinaryRules(const std::string &sBinaryRulePath) {
      ASSERT(m_bTrain, "Rules can only be loaded during training!");
      if (!m_weights->empty() || m_nTrainingRound !=0 ) {
         WARNING("Ignored binary rules from " << sBinaryRulePath << " because it was not loaded when the model is empty and before any training sentence is read");
      }
      // load rule from the specified file
      std::ifstream file ;
      file.open(sBinaryRulePath.c_str()) ;
      m_rule.LoadBinaryRules(file);
      file.close();
   }
   void LoadUnaryRules(const std::string &sUnaryRulePath) {
      ASSERT(m_bTrain, "Rules can only be loaded during training!");
      if (!m_weights->empty() || m_nTrainingRound !=0 ) {
         WARNING("Ignored unary rules from " << sUnaryRulePath << " because it was not loaded when the model is empty and before any training sentence is read");
      }
      // load rule from the file specified
      std::ifstream file;
      file.open(sUnaryRulePath.c_str());
      m_rule.LoadUnaryRules(file);
      file.close();
   }

public:
   void parse( const CTwoStringVector &sentence , CSentenceParsed *retval , int nBest=1 , conparser::SCORE_TYPE *scores=0 ) ;
   void parse( const CSentenceMultiCon<CConstituent> &sentence , CSentenceParsed *retval , int nBest=1 , conparser::SCORE_TYPE *scores=0 ) ;
   void train( const CSentenceParsed &correct , int round ) ;
   void train( const CSentenceMultiCon<CConstituent> &con_input, const CSentenceParsed &correct , int round ) ;
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
      m_rule.saveRules(file);
      file.close();
      std::cerr << "Total number of training errors are: " << m_nTotalErrors << std::endl;
   }
   conparser::SCORE_TYPE getGlobalScore(const CSentenceParsed &parsed);
   void updateScores(const CSentenceParsed &parse, const CSentenceParsed &correct, int round=0);

private:
   enum SCORE_UPDATE {eAdd=0, eSubtract};

   void work( const bool bTrain, const CTwoStringVector &sentence , CSentenceParsed *retval, const CSentenceParsed &correct, int nBest, conparser::SCORE_TYPE *scores ) ;

   // get the global score for a parsed sentence or section
   inline void getOrUpdateStackScore( conparser::CWeight *cast_weights, CPackedScoreType<conparser::SCORE_TYPE, conparser::CAction::MAX> &retval, const conparser::CStateItem *item, const conparser::CAction &action=conparser::CAction(), conparser::SCORE_TYPE amount=0, int round=0 );
   inline void getOrUpdateScore( CPackedScoreType<conparser::SCORE_TYPE, conparser::CAction::MAX> &retval, const conparser::CStateItem &item, const conparser::CAction &action=conparser::CAction(), conparser::SCORE_TYPE amount=0, int round=0 );

   // update the built-in weight vector for this feature object specifically
   void updateScoresForState( conparser::CWeight *cast_weights , const conparser::CStateItem *output , SCORE_UPDATE update) ;
#ifdef TRAIN_MULTI
   void updateScoresForMultipleStates( const conparser::CStateItem *output_start , const conparser::CStateItem *output_end , const conparser::CStateItem  *candidate , const conparser::CStateItem *correct ) ;
   void computeAlpha( const unsigned K );
#else
   void updateScoresForStates( const conparser::CStateItem *output , const conparser::CStateItem *correct ) ;
#endif

#ifdef TRAIN_LOSS
   void getLabeledBrackets(const CSentenceParsed &parse_tree, CStack<CLabeledBracket> &brackets);
#endif
};

}; // namespace TARGET_LANGUAGE

#endif


