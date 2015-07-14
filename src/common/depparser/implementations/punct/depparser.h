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
#ifdef LABELED
   std::vector< CDependencyLabel > m_lCacheLabel;
#endif
#ifdef PUNCT
   std::vector< unsigned > m_lLeftPunct;
   std::vector< unsigned > m_lRightPunct;
   std::vector< unsigned > m_lOriginalInex;
   std::vector< int > m_lDerivedIndex;
#endif
   int m_nTrainingRound;
   int m_nTotalErrors;
   bool m_bScoreModified;
   int m_nScoreIndex;

public:
   // constructor and destructor
   CDepParser( const std::string &sFeatureDBPath , bool bTrain , bool bCoNLL ) : CDepParserBase(sFeatureDBPath, bTrain, bCoNLL) {
      m_Agenda = new CAgendaBeam<depparser::CStateItem>(depparser::AGENDA_SIZE);
      m_weights = new depparser :: CWeight(sFeatureDBPath, bTrain );
      m_nTrainingRound = 0;
      m_nTotalErrors = 0;
      if (bTrain) m_nScoreIndex = CScore<depparser::SCORE_TYPE>::eNonAverage ; else m_nScoreIndex = CScore<depparser::SCORE_TYPE>::eAverage ;
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
   depparser::SCORE_TYPE getGlobalScore(const CDependencyParse &parsed);
   void updateScores(const CDependencyParse &parse, const CDependencyParse &correct, int round=0);

private:
   enum SCORE_UPDATE {eAdd=0, eSubtract};

   void work( const bool bTrain, const CTwoStringVector &sentence , CDependencyParse *retval, const CDependencyParse &correct, int nBest, depparser::SCORE_TYPE *scores ) ;

   // get the global score for a parsed sentence or section
   inline depparser::SCORE_TYPE getOrUpdateArcScore( const int &head_index, const int &dep_index, const int &sibling_index, depparser::SCORE_TYPE amount=0, int round=0 );
   inline depparser::SCORE_TYPE getOrUpdateTwoArcScore(const int &head_index, const int &dep_index, const int &parent_index, depparser::SCORE_TYPE amount=0, int round=0);
   inline depparser::SCORE_TYPE getOrUpdateArityScore(const int &word_index, const int &arity, const int &arity_direction, depparser::SCORE_TYPE amount=0, int round=0);
   inline depparser::SCORE_TYPE getOrUpdateStackScore( const depparser::CStateItem *item, const unsigned long &action, depparser::SCORE_TYPE amount=0, int round=0 );
#ifdef LABELED
   inline depparser::SCORE_TYPE getOrUpdateArcLabelScore( const int &head_index, const int &dep_index, const unsigned long &label, depparser::SCORE_TYPE amount=0, int round=0 );
#endif

   // update the built-in weight std::vector for this feature object specifically
   void updateScoresForStates( const depparser::CStateItem *output , const depparser::CStateItem *correct ,
                               depparser::SCORE_TYPE amount_add , depparser::SCORE_TYPE amount_subtract ) ;
   inline void updateScoreForState( const depparser::CStateItem *output , const depparser::SCORE_TYPE &amount ) ;


   // helper method
   inline void reduce( depparser::CStateItem *item ) ;
   inline void shift( depparser::CStateItem *item ) ;
#ifdef LABELED
   inline void arcleft( depparser::CStateItem *item, const unsigned long &label ) ;
   inline void arcright( depparser::CStateItem *item, const unsigned long &label ) ;
#else
   inline void arcleft( depparser::CStateItem *item ) ;
   inline void arcright( depparser::CStateItem *item ) ;
#endif
   inline void poproot( depparser::CStateItem *item ) ;

};

}; // namespace TARGET_LANGUAGE

#endif


