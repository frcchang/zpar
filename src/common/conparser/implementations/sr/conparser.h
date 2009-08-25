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
   vector< CTaggedWord<CTag> > m_lCache;
   vector<unsigned> m_lWordLen;
   int m_nTrainingRound;
   int m_nTotalErrors;
   bool m_bScoreModified;
   int m_nScoreIndex;

public:
   // constructor and destructor
   CConParser( const string &sFeatureDBPath , bool bTrain ) : CConParserBase(sFeatureDBPath, bTrain) { 
      m_Agenda = new CAgendaBeam<conparser::CStateItem>(conparser::AGENDA_SIZE);
      m_weights = new conparser :: CWeight(sFeatureDBPath, bTrain );
      m_nTrainingRound = 0; 
      m_nTotalErrors = 0;
      if (bTrain) m_nScoreIndex = CScore<conparser::SCORE_TYPE>::eNonAverage ; else m_nScoreIndex = CScore<conparser::SCORE_TYPE>::eAverage ;
      assert(PENN_CON_COUNT_BITS == PENN_TAG_COUNT_BITS-1); // for encoding token tags
      assert(PENN_CON_TAG_COUNT_BITS * 3 + PENN_CON_COUNT_BITS+4<=32); // the largest encoded feature
      assert(CFGSET_SIZE*PENN_CON_TAG_COUNT_BITS<sizeof(unsigned long)*8); // the cfg set can hold it
   }
   ~CConParser() {
      delete m_Agenda;
      delete m_weights;
   }
   CConParser( CConParser &conparser) : CConParserBase(conparser) { 
      assert(1==0);
   }

public:
   void parse( const CTwoStringVector &sentence , CSentenceParsed *retval , int nBest=1 , conparser::SCORE_TYPE *scores=0 ) ;
   void train( const CSentenceParsed &correct , int round ) ;

   void finishtraining() {
      static_cast<conparser::CWeight*>(m_weights)->computeAverageFeatureWeights(m_nTrainingRound);
      static_cast<conparser::CWeight*>(m_weights)->saveScores();
      cout << "Total number of training errors are: " << m_nTotalErrors << endl;
   }
   conparser::SCORE_TYPE getGlobalScore(const CSentenceParsed &parsed);
   void updateScores(const CSentenceParsed &parse, const CSentenceParsed &correct, int round=0);

private:
   enum SCORE_UPDATE {eAdd=0, eSubtract};

   inline void shift(conparser::CStateItem &item);
   inline void reduce(conparser::CStateItem &item);
   inline void reduce_unary(conparser::CStateItem &item);
   inline void terminate(conparser::CStateItem &item);

   void work( const bool bTrain, const CTwoStringVector &sentence , CSentenceParsed *retval, const CSentenceParsed &correct, int nBest, conparser::SCORE_TYPE *scores ) ; 

   // get the global score for a parsed sentence or section
   inline conparser::SCORE_TYPE getOrUpdateLinkScore( const unsigned &head, const unsigned &mod, conparser::SCORE_TYPE amount=0, int round=0 );
   inline conparser::SCORE_TYPE getOrUpdateSiblingLinkScore( const unsigned &head, const unsigned &mod, const unsigned &sibling, conparser::SCORE_TYPE amount=0, int round=0 );
   inline conparser::SCORE_TYPE getOrUpdateArityScore( const unsigned &head, const unsigned &arityleft, const unsigned &arityright, conparser::SCORE_TYPE amount=0, int round=0 );
   inline conparser::SCORE_TYPE getOrUpdateHeadScore( const unsigned &parent, const unsigned &child, const unsigned &head, conparser::SCORE_TYPE amount=0, int round=0 );
   inline conparser::SCORE_TYPE getOrUpdateHeadModifierScore( const unsigned &parent, const unsigned &child, const unsigned &child1, const unsigned &head, const unsigned &mod, const bool &left, const bool &neighbour, conparser::SCORE_TYPE amount=0, int round=0 );
   inline conparser::SCORE_TYPE getOrUpdateBigramScore( const unsigned &parent, const unsigned &child, const unsigned &child1, const bool &left, conparser::SCORE_TYPE amount=0, int round=0 );
   inline conparser::SCORE_TYPE getOrUpdateTrigramScore( const unsigned &parent, const unsigned &child, const unsigned &child1, const unsigned &childo, const bool &left, conparser::SCORE_TYPE amount=0, int round=0 );
   inline conparser::SCORE_TYPE getOrUpdateGraphScore( const conparser::CStateItem *item, conparser::SCORE_TYPE amount=0, int round=0 );
   inline conparser::SCORE_TYPE getOrUpdateStackScore( const conparser::CStateItem *item, const unsigned &action, conparser::SCORE_TYPE amount=0, int round=0 );

   // update the built-in weight vector for this feature object specifically
   void updateScoresForStates( const conparser::CStateItem *output , const conparser::CStateItem *correct ) ;
   void updateScoresForState( const conparser::CStateItem *output , SCORE_UPDATE update ) ;

};

}; // namespace TARGET_LANGUAGE

#endif


