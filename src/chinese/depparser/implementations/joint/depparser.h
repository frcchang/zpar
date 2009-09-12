/****************************************************************
 *                                                              *
 * depparser.h - the definitions for the dependency parser.     *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2008.7                         *
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

namespace chinese {

namespace depparser {

#include "stateitem.h"

}; // namespace depparser
};

#include "weight.h"

namespace chinese {

#include "tagger/wordcache.h"
#include "tagger/segmentationprune.h"

/*===============================================================
 *
 * CDepParser - the dependency parser for English 
 *
 *==============================================================*/

class CDepParser : public CDepParserBase {

private:

   CAgendaChart<depparser::CStateItem> m_Chart;
   CWordCache m_WordCache;

   int m_nTrainingRound;
   int m_nTotalErrors;
   bool m_bScoreModified;
   int m_nScoreIndex;

   CCharCatDictionary m_Knowledge;
   bool m_bKnowledgeLoaded;

public:
   // constructor and destructor
   CDepParser( const string &sFeatureDBPath , bool bTrain ) : CDepParserBase(sFeatureDBPath, bTrain), m_Chart(depparser::AGENDA_SIZE), m_WordCache(depparser::MAX_SENTENCE_SIZE), m_bKnowledgeLoaded(false) { 
      m_weights = new depparser :: CWeight(sFeatureDBPath, bTrain );
      m_nTrainingRound = 0; 
      m_nTotalErrors = 0;
      m_nScoreIndex = bTrain ? CScore<depparser::SCORE_TYPE>::eNonAverage : CScore<depparser::SCORE_TYPE>::eAverage ;
      assert( DEPENDENCY_LINK_NO_HEAD == -1 ) ;
   }
   ~CDepParser() {
      delete m_weights;
   }
   CDepParser( CDepParser &depparser ) : CDepParserBase(depparser), m_Chart(depparser::AGENDA_SIZE), m_WordCache(0) { 
      assert(1==0);
   }

public:
   void parse( const CStringVector &sentence , CDependencyTree &retval ) ;
   void train( const CDependencyTree &correct , int round ) ;

   void finishtraining() {
      static_cast<depparser::CWeight*>(m_weights)->computeAverageFeatureWeights(m_nTrainingRound);
      static_cast<depparser::CWeight*>(m_weights)->saveScores();
      cout << "Total number of training errors are: " << m_nTotalErrors << endl;
   }

public:
   void loadKnowledge(const string &sKnowledgePath) {
      m_Knowledge.load( sKnowledgePath );
      m_bKnowledgeLoaded = true;
   }

private:
   enum SCORE_UPDATE {eAdd=0, eSubtract};

   void addLink( const CStringVector *sentence, depparser::CStateItem *item, const int head, const int dep ) ;
   void enumerateCandidates( const CStringVector *sentence, const depparser::CStateItem *item, const int agenda_index, const unsigned long tag );
   void work( const CStringVector *sentence , CDependencyTree *retval, const CDependencyTree *correct ) ; 

   depparser::SCORE_TYPE getOrUpdateLocalScore(const CStringVector *s, const depparser::CStateItem *item, int index, depparser::SCORE_TYPE amount=0, int round=0);
   depparser::SCORE_TYPE getOrUpdateArcScore( const CStringVector *sentence, const depparser::CStateItem *item, const int &head_index, const int &dep_index, depparser::SCORE_TYPE amount=0, int round=0 );

   // update the built-in weight vector for this feature object specifically
   void updateScoresForStates( const CStringVector *raw , const depparser::CStateItem *output , const depparser::CStateItem *correct , bool bComplete ) ;
   void updateInfo( const CDependencyTree *sent ) ;
   inline void updateScoreForState( const CStringVector *sent , const depparser::CStateItem *output , bool bComplete , const depparser::SCORE_TYPE &amount ) ;

};

}; // namespace chinese

#endif


