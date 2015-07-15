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

};  // end for namespace depparser
};  // end for namespace TARGET_LANGUAGE

#include "depparser_weight.h"

namespace TARGET_LANGUAGE {


class CDepParser : public CDepParserBase {
  private:
    CAgendaSimple<depparser::action::CScoredAction> *m_Beam;

    // caches for input
    std::vector< CTaggedWord<CTag, TAG_SEPARATOR> > m_lCache;
#ifdef LABELED
    std::vector< CDependencyLabel > m_lCacheLabel;
#endif
    std::vector< CLemma > m_lCacheCoNLLLemma; // conll
    std::vector< CCoNLLCPOS > m_lCacheCoNLLCPOS; // conll
    std::vector< std::vector<CCoNLLFeats> > m_lCacheCoNLLFeats; // conll

    int  m_nTrainingRound;
    int  m_nTotalErrors;
    int  m_nScoreIndex;
    bool m_bScoreModified;

  public:
    // constructor and destructor
    CDepParser(const std::string &sFeatureDBPath,
               bool bTrain,
               bool bCoNLL = false)
      : CDepParserBase(sFeatureDBPath, bTrain, bCoNLL) {
      m_Beam      = new CAgendaSimple<depparser::action::CScoredAction>(AGENDA_SIZE);
      m_weights   = new depparser :: CWeight(sFeatureDBPath, bTrain );

      m_nTrainingRound = 0;
      m_nTotalErrors   = 0;

      if (bTrain) {
        m_nScoreIndex = CScore<depparser::SCORE_TYPE>::eNonAverage;
      } else {
        m_nScoreIndex = CScore<depparser::SCORE_TYPE>::eAverage ;
      }
    }

    ~CDepParser() {
      delete m_Beam;
      delete m_weights;
    }

    CDepParser( CDepParser &depparser) : CDepParserBase(depparser) {
      assert(1==0);
    }

  public:

    void parse(const CTwoStringVector & sentence,
               CDependencyParse *       retval,
               int                      nBest = 1 ,
               depparser::SCORE_TYPE  * scores = 0);

    void train(const CDependencyParse &correct,
               int round) ;

    void extract_features(const CDependencyParse &input);

    void parse_conll(const CCoNLLInput &     sentence,
                     CCoNLLOutput *          retval,
                     int                     nBest = 1,
                     depparser::SCORE_TYPE * scores = 0);

    void train_conll(const CCoNLLOutput & correct,
                     int round);

    void extract_features_conll(const CCoNLLOutput & input) ;

    void finishtraining() {
        static_cast<depparser::CWeight*>(m_weights)->computeAverageFeatureWeights(
                m_nTrainingRound);
        static_cast<depparser::CWeight*>(m_weights)->saveScores();
        std::cerr << "Total number of training errors are: " << m_nTotalErrors << std::endl;
    }

    depparser::SCORE_TYPE getGlobalScore(const CDependencyParse &parsed);

    void updateScores(const CDependencyParse & parse,
                      const CDependencyParse & correct,
                      int round = 0);

  private:
    enum SCORE_UPDATE {eAdd=0, eSubtract};

    template<typename CCoNLLInputOrOutput>
    void initCoNLLCache( const CCoNLLInputOrOutput &sentence );

    void transit(const depparser::CStateItem * item,
                 const CPackedScoreType<depparser::SCORE_TYPE,
                 depparser::action::MAX> & packed_scores);

    int insertIntoLattice(depparser::CStateItem * item,
                          depparser::CStateItem * lattice,
                          int currentLatticeSize,
                          int maxLatticeSize);

    int work(const bool              bTrain,
             const CTwoStringVector &sentence,
             CDependencyParse *      retval,
             const CDependencyParse &correct,
             int                     nBest,
             depparser::SCORE_TYPE * scores );

    inline void getOrUpdateStackScore(
            const depparser::CStateItem *item,
            CPackedScoreType<depparser::SCORE_TYPE,
            depparser::action::MAX> &retval,
            const unsigned &action,
            depparser::SCORE_TYPE amount=0,
            int round=0);

    // update the built-in weight std::vector for this feature object specifically
    void updateScoresForStates(
            const depparser::CStateItem *   predicatedState,
            const depparser::CStateItem *   correctState,
            depparser::SCORE_TYPE           amount_add ,
            depparser::SCORE_TYPE           amount_subtract ) ;

    inline void updateScoreForState(
            const depparser::CStateItem ** reversedStateChain,
            int numStates,
            const depparser::SCORE_TYPE & amount ) ;

    // helper method
    inline void reduce(
            const depparser::CStateItem *item,
            const CPackedScoreType<depparser::SCORE_TYPE, depparser::action::MAX> &scores);

    inline void shift(
            const depparser::CStateItem *item,
            const CPackedScoreType<depparser::SCORE_TYPE, depparser::action::MAX> &scores);

    inline void arcleft(
            const depparser::CStateItem *item,
            const CPackedScoreType<depparser::SCORE_TYPE, depparser::action::MAX> &scores);

    inline void arcright(
            const depparser::CStateItem *item,
            const CPackedScoreType<depparser::SCORE_TYPE, depparser::action::MAX> &scores);

    inline void poproot(
            const depparser::CStateItem *item,
            const CPackedScoreType<depparser::SCORE_TYPE, depparser::action::MAX> &scores);

};

}; // namespace TARGET_LANGUAGE

#endif


