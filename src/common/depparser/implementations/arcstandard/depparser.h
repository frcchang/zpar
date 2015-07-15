// Copyright (C) University of Oxford 2010
#ifndef DEPPARSER_ARC_STANDARD_DEPPARSER_H
#define DEPPARSER_ARC_STANDARD_DEPPARSER_H

#include "depparser_base.h"

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
  typedef CPackedScoreType<depparser::SCORE_TYPE, depparser::action::kMax> CPackedScore;

  //! The scored transition beam.
  depparser::CScoredTransition* m_kBestTransitions;

  //! The caches for input sentence.
  std::vector< CTaggedWord<CTag, TAG_SEPARATOR> > m_lCache;

#ifdef LABELED
  //! The cache for input labels.
  std::vector< CDependencyLabel > m_lCacheLabel;
#endif

  //! The conll lemma cache.
  std::vector< CLemma > m_lCacheCoNLLLemma;
  //! The conll coarse postag cache.
  std::vector< CCoNLLCPOS > m_lCacheCoNLLCPOS;
  //! The conll feature cache.
  std::vector< std::vector<CCoNLLFeats> > m_lCacheCoNLLFeats;

  int m_nTrainingRound;
  int m_nTotalErrors;
  int m_nScoreIndex;
  bool m_bScoreModified;

  //! The actual memory for storing the StateItems
  depparser::CStateItem * lattice_;
  //! Control the current number of elements in beam.
  int current_beam_size_;
  int max_beam_size_;
  int max_lattice_size_;
public:
  // constructor and destructor
  CDepParser(const std::string &sFeatureDBPath,
             bool bTrain,
             bool bCoNLL = false);

  ~CDepParser();

  /**
   * Perform the parsing.
   *
   *  @param[in]  sentence  The input sentence, a list of (word, tag) pair.
   *  @param[out] retval    The output parse(s). Number of outputs is govern by
   *                        the nBest parameter.
   *  @param[in]  nBest     The nBest number.
   *  @param[out] scores    The corresponding scores for the number.
   */
  void parse(const CTwoStringVector &sentence,
             CDependencyParse *retval,
             int nBest = 1,
             depparser::SCORE_TYPE *scores = 0);

  /**
   * Perform the training.
   *
   *  @param[in]  correct   The oracle dependency parse.
   *  @param[in]  round     The number of training round.
   */
  void train(const CDependencyParse &correct,
             int round);

  /**
   * Perform the feature extraction.
   *
   *  @param[in]  input     The input dependency parse.
   */
  void extract_features(const CDependencyParse &input);

  //! CoNLL version parsing facility.
  void parse_conll(const CCoNLLInput & sentence,
                   CCoNLLOutput * retval,
                   int nBest = 1,
                   depparser::SCORE_TYPE *scores = 0);

  //! CoNLL version trainig facility.
  void train_conll(const CCoNLLOutput &correct,
                   int round);

  //! CoNLL version feature extraction facility.
  void extract_features_conll(const CCoNLLOutput &input);

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
  //! Guarded.
  CDepParser(CDepParser &depparser);

  enum SCORE_UPDATE {eAdd=0, eSubtract};

  depparser::CStateItem * GetLattice(int size);

  template<typename CCoNLLInputOrOutput>
    void initCoNLLCache(const CCoNLLInputOrOutput &sentence);

  int InsertIntoBeam(const depparser::CScoredTransition & transition);

  inline void Transit(const depparser::CStateItem * item,
                      const CPackedScore& scores);

  int work(const bool bTrain,
           const CTwoStringVector & sentence,
           CDependencyParse * retval,
           const CDependencyParse & correct,
           int nBest,
           depparser::SCORE_TYPE *scores);

  /**
   * The function for extract features.
   *
   *
   *
   */
  inline void GetOrUpdateStackScore(const depparser::CStateItem* item,
                                    CPackedScore& retval,
                                    const unsigned& action,
                                    depparser::SCORE_TYPE amount=0,
                                    int round=0);

  void UpdateScoresForStates(const depparser::CStateItem * output,
                             const depparser::CStateItem * correct,
                             depparser::SCORE_TYPE amount_add,
                             depparser::SCORE_TYPE amount_subtract);

  inline void shift(const depparser::CStateItem * item,
                    const CPackedScore& scores);

  inline void arcleft(const depparser::CStateItem *item,
                      const CPackedScore& scores);

  inline void arcright(const depparser::CStateItem *item,
                       const CPackedScore& scores);

  inline void poproot(const depparser::CStateItem *item,
                      const CPackedScore& scores);

};

}; // namespace TARGET_LANGUAGE

#endif


