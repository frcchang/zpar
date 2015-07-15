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

/*===============================================================
 *
 * Global items
 *
 *==============================================================*/

//namespace TARGET_LANGUAGE {
//
//namespace conparser {
//
//#include "stateitem.h"
//
//}; // namespace conparser
//};

#include "weight.h"

namespace TARGET_LANGUAGE {

/*===============================================================
 *
 * CConParser - the constituent parser for English
 *
 *==============================================================*/

class CConParser : public CConParserBase {

private:

//   CAgendaBeam<conparser::CStateItem> *m_Agenda;
   std::vector< CTaggedWord<CTag, TAG_SEPARATOR> > m_lCache;
   std::vector<unsigned long> m_lWordLen;
   int m_nTrainingRound;
   int m_nTotalErrors;
   bool m_bScoreModified;
   int m_nScoreIndex;
   conparser::CRule m_rule;
   conparser::CContext m_Context;
   CHashMap< CWord, std::vector<int> > m_mapClusterV;
   CHashMap< CWord, std::vector<int> > m_mapClusterN;

public:
   // constructor and destructor
   CConParser( const std::string &sFeatureDBPath , bool bTrain ) : CConParserBase(sFeatureDBPath, bTrain), m_rule(&m_lCache), m_mapClusterV(1<<16), m_mapClusterN(1<<16) {
      // initialize agenda
//      m_Agenda = new CAgendaBeam<conparser::CStateItem>(conparser::AGENDA_SIZE);
      // and initialize the weith module laoding content
      m_weights = new conparser :: CWeight( bTrain );
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
      // load cluster
      std::ifstream ifsv("clusterv.txt");
      ASSERT(ifsv.is_open(), "The cluster file (clusterv.txt) is not accessible");
      ifsv >> m_mapClusterV;
      std::ifstream ifsn("clustern.txt");
      ASSERT(ifsn.is_open(), "The cluster file (clustern.txt) is not accessible");
      ifsn >> m_mapClusterN;
      ifsn.close();

      m_nTrainingRound = 0;
      m_nTotalErrors = 0;
      if (bTrain) m_nScoreIndex = CScore<conparser::SCORE_TYPE>::eNonAverage ; else m_nScoreIndex = CScore<conparser::SCORE_TYPE>::eAverage ;
      ASSERT(conparser::CAction::MAX<=(1L<<(sizeof(unsigned)*8)), "conparser.h: The size of action is too big for the packed scoretype");
   }
   ~CConParser() {
//      delete m_Agenda;
      delete m_weights;
   }
   CConParser( CConParser &conparser) : CConParserBase(conparser), m_rule(&m_lCache), m_mapClusterV(1), m_mapClusterN(1) {
      THROW("Copy constructor called");
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
   inline void getOrUpdateStackScore( CPackedScoreType<conparser::SCORE_TYPE, conparser::CAction::MAX> &retval, const conparser::CStateItem *item, const conparser::CAction &action, conparser::SCORE_TYPE amount=0, int round=0 );
   inline void getOrUpdateScore( CPackedScoreType<conparser::SCORE_TYPE, conparser::CAction::MAX> &retval, const conparser::CStateItem &item, const conparser::CAction &action=conparser::CAction(), conparser::SCORE_TYPE amount=0, int round=0 );

   // update the built-in weight std::vector for this feature object specifically
   void updateScoresForStates( const conparser::CStateItem *output , const conparser::CStateItem *correct ) ;
   void updateScoresForState( const conparser::CStateItem *output , SCORE_UPDATE update ) ;

};

}; // namespace TARGET_LANGUAGE

#endif


