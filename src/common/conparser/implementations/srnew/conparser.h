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

   CAgendaBeam<conparser::CStateItem> *m_Agenda;
   vector< CTaggedWord<CTag, TAG_SEPARATOR> > m_lCache;
   vector<unsigned long> m_lWordLen;
   int m_nTrainingRound;
   int m_nTotalErrors;
   bool m_bScoreModified;
   int m_nScoreIndex;
   conparser::CRule m_rule;

public:
   // constructor and destructor
   CConParser( const string &sFeatureDBPath , bool bTrain ) : CConParserBase(sFeatureDBPath, bTrain) { 
      // initialize agenda
      m_Agenda = new CAgendaBeam<conparser::CStateItem>(conparser::AGENDA_SIZE);
      // and initialize the weith module laoding content
      m_weights = new conparser :: CWeight( bTrain );
      ifstream file;
      file.open(sFeatureDBPath.c_str());
      m_weights->loadScores(file);
      // load rules
      m_rule.loadRules(file);
      file.close();
      // initialize 
      if (!bTrain && m_weights->empty()) { // when decoding, model must be found
         THROW("The model file is not found.")
      }
      m_nTrainingRound = 0; 
      m_nTotalErrors = 0;
      if (bTrain) m_nScoreIndex = CScore<conparser::SCORE_TYPE>::eNonAverage ; else m_nScoreIndex = CScore<conparser::SCORE_TYPE>::eAverage ;
      assert(PENN_CON_COUNT_BITS == PENN_TAG_COUNT_BITS-1); // for encoding token tags
      assert(PACKED_CON_OR_TAG_SIZE * 3 + PENN_CON_COUNT_BITS+4<=32); // the largest encoded feature
      assert(CFGSET_SIZE*PACKED_CON_OR_TAG_SIZE<sizeof(unsigned long long)*8); // the cfg set can hold it
   }
   ~CConParser() {
      delete m_Agenda;
      delete m_weights;
   }
   CConParser( CConParser &conparser) : CConParserBase(conparser) { 
      assert(1==0);
   }

public:
   void LoadBinaryRules(const string &sBinaryRulePath) {
      ASSERT(!m_bTrain, "Rules can only be loaded during training!");
      if (!m_weights->empty() || m_nTrainingRound !=0 ) {
         WARNING("Ignored binary rules from " << sBinaryRulePath << " because it was not loaded when the model is empty and before any training sentence is read");
      }
      // load rule from the specified file 
      ifstream file ; 
      file.open(sBinaryRulePath.c_str()) ;
      m_rule.LoadBinaryRules(file);
      file.close();
   }
   void LoadUnaryRules(const string &sUnaryRulePath) {
      ASSERT(!m_bTrain, "Rules can only be loaded during training!");
      if (!m_weights->empty() || m_nTrainingRound !=0 ) {
         WARNING("Ignored unary rules from " << sUnaryRulePath << " because it was not loaded when the model is empty and before any training sentence is read");
      }
      // load rule from the file specified
      ifstream file;
      file.open(sUnaryRulePath.c_str());
      m_rule.LoadUnaryRules(file);
      file.close();
   }

public:
   void parse( const CTwoStringVector &sentence , CSentenceParsed *retval , int nBest=1 , conparser::SCORE_TYPE *scores=0 ) ;
   void train( const CSentenceParsed &correct , int round ) ;

   void finishtraining() {
      // compute average
      static_cast<conparser::CWeight*>(m_weights)->computeAverageFeatureWeights(m_nTrainingRound);
      // save scores
      ofstream file ;
      file.open(m_sFeatureDB.c_str()) ;
      static_cast<conparser::CWeight*>(m_weights)->saveScores(file);
      // save rules
      m_rule.saveRules(file);
      file.close();
      cout << "Total number of training errors are: " << m_nTotalErrors << endl;
   }
   conparser::SCORE_TYPE getGlobalScore(const CSentenceParsed &parsed);
   void updateScores(const CSentenceParsed &parse, const CSentenceParsed &correct, int round=0);

private:
   enum SCORE_UPDATE {eAdd=0, eSubtract};

   void work( const bool bTrain, const CTwoStringVector &sentence , CSentenceParsed *retval, const CSentenceParsed &correct, int nBest, conparser::SCORE_TYPE *scores ) ; 

   // get the global score for a parsed sentence or section
   inline conparser::SCORE_TYPE getOrUpdateLinkScore( const unsigned long &head, const unsigned long &mod, conparser::SCORE_TYPE amount=0, int round=0 );
   inline conparser::SCORE_TYPE getOrUpdateSiblingLinkScore( const unsigned long &head, const unsigned long &mod, const unsigned long &sibling, conparser::SCORE_TYPE amount=0, int round=0 );
   inline conparser::SCORE_TYPE getOrUpdateArityScore( const unsigned long &head, const unsigned long &arityleft, const unsigned long &arityright, conparser::SCORE_TYPE amount=0, int round=0 );
   inline conparser::SCORE_TYPE getOrUpdateHeadScore( const unsigned long &parent, const unsigned long &child, const unsigned long &head, conparser::SCORE_TYPE amount=0, int round=0 );
   inline conparser::SCORE_TYPE getOrUpdateHeadModifierScore( const unsigned long &parent, const unsigned long &child, const unsigned long &child1, const unsigned long &head, const unsigned long &mod, const bool &left, const bool &neighbour, conparser::SCORE_TYPE amount=0, int round=0 );
   inline conparser::SCORE_TYPE getOrUpdateBigramScore( const unsigned long &parent, const unsigned long &child, const unsigned long &child1, const bool &left, conparser::SCORE_TYPE amount=0, int round=0 );
   inline conparser::SCORE_TYPE getOrUpdateTrigramScore( const unsigned long &parent, const unsigned long &child, const unsigned long &child1, const unsigned long &childo, const bool &left, conparser::SCORE_TYPE amount=0, int round=0 );
   inline conparser::SCORE_TYPE getOrUpdateGraphScore( const conparser::CStateItem *item, conparser::SCORE_TYPE amount=0, int round=0 );
   inline conparser::SCORE_TYPE getOrUpdateStackScore( const conparser::CStateItem *item, const conparser::CAction &action, conparser::SCORE_TYPE amount=0, int round=0 );
   inline conparser::SCORE_TYPE getOrUpdateScore( const conparser::CStateItem &item, const conparser::CAction &action, conparser::SCORE_TYPE amount=0, int round=0 );

   // update the built-in weight vector for this feature object specifically
   void updateScoresForStates( const conparser::CStateItem *output , const conparser::CStateItem *correct ) ;
   void updateScoresForState( const conparser::CStateItem *output , SCORE_UPDATE update ) ;

};

}; // namespace TARGET_LANGUAGE

#endif


