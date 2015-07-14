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

   CAgendaBeam<depparser::CStateItem> *m_Agenda;
   std::vector< CTaggedWord<CTag, TAG_SEPARATOR> > m_lCache;
   int m_nTrainingRound;
   int m_nTotalErrors;
   bool m_bScoreModified;
   int m_nScoreIndex;

public:
   // constructor and destructor
   CDepParser( const std::string &sFeatureDBPath , bool bTrain , bool bCoNLL=false ) : CDepParserBase(sFeatureDBPath, bTrain, bCoNLL) {
      m_Agenda = new CAgendaBeam<depparser::CStateItem>(depparser::AGENDA_SIZE);
      m_weights = new depparser :: CWeight(sFeatureDBPath, bTrain );
      m_nTrainingRound = 0;
      m_nTotalErrors = 0;
      if (bTrain) m_nScoreIndex = CScore<depparser::SCORE_TYPE>::eNonAverage ; else m_nScoreIndex = CScore<depparser::SCORE_TYPE>::eAverage ;
      assert(DEPENDENCY_LINK_NO_HEAD==-1); // used in the decoder
   }
   ~CDepParser() {
      delete m_Agenda;
      delete m_weights;
   }
   CDepParser( CDepParser &depparser) : CDepParserBase(depparser) {
      assert(1==0);
   }

public:
   void parse( const CTwoStringVector &sentence , CDependencyParse *retval , int nBest=1 , depparser::SCORE_TYPE *scores=0 ) ;
   void train( const CDependencyParse &correct , int round ) ;

   void finishtraining() {
      static_cast<depparser::CWeight*>(m_weights)->computeAverageFeatureWeights(m_nTrainingRound);
      static_cast<depparser::CWeight*>(m_weights)->saveScores();
      std::cerr << "Total number of training errors are: " << m_nTotalErrors << std::endl;
   }

private:
   enum SCORE_UPDATE {eAdd=0, eSubtract};

   void work( const CTwoStringVector &sentence , CDependencyParse *retval, const CDependencyParse &correct, int nBest, depparser::SCORE_TYPE *scores ) ;

   inline void addLink( depparser::CStateItem *item , const int &head , const int &dep ) ;
   inline void finishWord( depparser::CStateItem *item ) ;
   inline void finishWordOnTheLeft( depparser::CStateItem *item , const int &index ) ;
   inline void finishWordOnTheRight( depparser::CStateItem *item , const int &index ) ;
   inline void finishSentence( depparser::CStateItem *item ) ;

   inline depparser::SCORE_TYPE getOrUpdateArcScore(const depparser::CStateItem *item, const int &head_index, const int &dep_index, depparser::SCORE_TYPE amount=0, int round=0 ) ;
   inline depparser::SCORE_TYPE getOrUpdateArityScore(const depparser::CStateItem *item, const int &word_index, const int &arity_direction, depparser::SCORE_TYPE amount=0, int round=0 ) ;
   inline depparser::SCORE_TYPE getOrUpdateTwoArcScore(const int &head_index, const int &dep_index, const int &parent_index, depparser::SCORE_TYPE amount=0, int round=0);

   void updateScoresForStates(const depparser::CStateItem *output , const depparser::CStateItem *correct ,
                              const bool &bCompleteSentence ) ;
   inline void updateScoreForState(const depparser::CStateItem *output , const bool &bCompleteSentence ,
                                   const depparser::SCORE_TYPE &amount) ;

};

}; // namespace TARGET_LANGUAGE

#endif


