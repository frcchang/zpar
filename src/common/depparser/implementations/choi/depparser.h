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

// guard the state with certain namespace
namespace TARGET_LANGUAGE {
namespace depparser {
#include "state.h"
};  //  namespace depparser
};  //  namespace TARGET_LANGUAGE

#include "depparser_weight.h"

namespace TARGET_LANGUAGE {

/*===============================================================
 *
 * CDepParser - the dependency parser for English
 *
 *==============================================================*/

class CDepParser : public CDepParserBase {
private:
  static depparser::CStateItem * lattice_;
  static int                     max_lattice_size_;
private:
  CAgendaSimple<depparser::action::CScoredAction> *m_Beam;
  // input
  std::vector< CTaggedWord<CTag, TAG_SEPARATOR> > m_lCache;
  std::vector< CLemma >                           m_lCacheCoNLLLemma; // conll
  std::vector< CCoNLLCPOS >                       m_lCacheCoNLLCPOS;  // conll
  std::vector< std::vector<CCoNLLFeats> >         m_lCacheCoNLLFeats; // conll

#ifdef LABELED
  std::vector< CDependencyLabel > m_lCacheLabel;
#endif

  int   m_nTrainingRound;
  int   m_nTotalErrors;
  int   m_nScoreIndex;
  bool  m_bScoreModified;

public:
  // constructor and destructor
  CDepParser(
      const std::string & sFeatureDBPath,
      bool                bTrain,
      bool                bCoNLL = false);

  ~CDepParser() {
    delete m_Beam;
    delete m_weights;
    if (lattice_) {
      delete [] lattice_;
      lattice_ = 0;
    }
  }

  CDepParser( CDepParser &depparser) : CDepParserBase(depparser) {
    assert(1==0);
  }

public:
  void parse(
      const CTwoStringVector & sentence,
      CDependencyParse       * retval,
      int                      n_best = 1,
      depparser::SCORE_TYPE  * scores = 0);

  void parse_conll(
      const CCoNLLInput      & sentence,
      CCoNLLOutput           * retval,
      int                      n_best = 1,
      depparser::SCORE_TYPE *  scores = 0);

  void train(
      const CDependencyParse & oracle_tree,
      int                      round);

  /* training wrapper for CoNLLx format data. this function convert
   * CoNLLOutput into dependency parse
   *
   *   @param[in]    correct
   *   @param[in]    round
   */
  void train_conll(
      const CCoNLLOutput &     oracle_conll_tree,
      int                      round);

  void extract_features(
      const CDependencyParse & input);

  void extract_features_conll(
      const CCoNLLOutput &     input);

  void finishtraining();

  depparser::SCORE_TYPE getGlobalScore(const CDependencyParse &parsed);

  void updateScores(
      const CDependencyParse &parse, 
      const CDependencyParse &correct,
      int round=0);

private:
  enum SCORE_UPDATE {
    eAdd = 0,
    eSubtract
  };

  depparser::CStateItem * GetLattice(int max_lattice_size);

  // If CoNLL format is confirmed, lemma, coase postag and conll specified
  // features are cached according to the given sentence
  template<typename CCoNLLInputOrOutput>
    void InitializeCoNLLCache(const CCoNLLInputOrOutput & sentence);

  void ConvertCoNLLToTwoStringVector(
      const CCoNLLInput  & conll_tree,
      CTwoStringVector   & sentence);

  void ConvertCoNLLToDependency(
      const CCoNLLOutput & conll_tree,
      CDependencyParse   & tree);

  void MoveDummyRootToTail(
      const CCoNLLOutput & conll_tree_with_dummy_root,
      CCoNLLOutput       & conll_tree_with_dummy_root_in_tail);

  void MoveDummyRootToTail(
      const CCoNLLInput  & conll_tree_with_dummy_root,
      CCoNLLInput        & conll_tree_with_dummy_root_in_tail);

  void MoveDummyRootToFront(
      const CCoNLLOutput & conll_tree_with_dummy_root,
      CCoNLLOutput       & conll_tree_with_dummy_root_in_front);

  void MoveDummyRootToFront(
      const CCoNLLInput  & conll_tree_with_dummy_root,
      CCoNLLInput        & conll_tree_with_dummy_root_in_front);

  void MoveDummyRootToFront(
      const CDependencyParse & parse_endswith_dummy_root,
      CDependencyParse       & parse_startswith_dummy_root);
  /* add a pseudo root to dependency tree
   *
   *  @param[in]  tree                    the tree without pseudo root
   *  @param[out] tree_with_pseudo_root   the tree with pseudo root
   */
  void AddPseudoRootToDependency (
      const CDependencyParse & tree,
      CDependencyParse       & tree_with_pseudo_root);

  void AddPseudoRootToTwoStringVector (
      const CTwoStringVector & sentence,
      CTwoStringVector       & sentence_with_pseudo_root);
  /*
   * Main process for performing the work process. If is_train is specified
   * perform early update
   *
   *  @param[out]   retval      the result parsing tree
   *  @param[out]   score       the result of scores, output nothing if is NULL
   *  @param[in]    sentence    the input sentence
   *  @param[in]    oracle_tree the gold standard parsing tree
   *  @param[in]    nbest       the number of top n best result to return
   *  @param[in]    is_train    use to specify the training process
   *  @return       int         the number of results, -1 when failed
   */
  int Work(
      CDependencyParse       * retval,
      depparser::SCORE_TYPE  * scores,
      const CTwoStringVector & sentence,
      const CDependencyParse & oracle_tree,
      int                      nbest,
      bool                     is_train);

  /*
   * Get or update the scores according to the stack states
   *
   *  @param[out]   retval      the packed score result
   *  @param[in]    item        the state item
   *  @param[in]    action      the action to update
   *  @param[in]    amount      the amount to update
   *  @param[in]    round
   */
  void GetOrUpdateStackScore(
      const depparser::CStateItem                                       * item,
      CPackedScoreType<depparser::SCORE_TYPE, depparser::action::kMax>  & retval,
      const unsigned                                                    & action,
      const depparser::SCORE_TYPE                                       & amount = 0,
      const int                                                           round  = 0);

  // update the built-in weight std::vector for this feature object specifically
  /* Travel through the state chain and update scores
   *
   *  @param[in]  predicated_state    the predicated state
   *  @param[in]  correct_state       the correct state
   *  @param[in]  amount_add          the 
   *  @param[in]  amount_substract    the punishment on wrong state
   */
  void UpdateScoresForStates(
      const depparser::CStateItem * predicated_state,
      const depparser::CStateItem * correct_state,
      const depparser::SCORE_TYPE   amount_add,
      const depparser::SCORE_TYPE   amount_subtract);

  /* Insert the state into beam, perform some heap operation
   *
   *  @param[in/out]  beam                the beam
   *  @param[in]      item                the item to be inserted
   *  @param[in]      current_beam_size   the current beam size
   *  @param[in]      max_beam_size       the max volumn of beam
   *  @return         int                 1 if the beam increased otherwise 0
   */
  int InsertIntoBeam(
      depparser::CStateItem *       beam,
      const depparser::CStateItem * item,
      const int                     current_beam_size,
      const int                     max_beam_size);

  // helper method
  inline void Transit(
      const depparser::CStateItem * item,
      const CPackedScoreType<depparser::SCORE_TYPE, depparser::action::kMax> & scores);

  inline void Idle(
      const depparser::CStateItem * item,
      const CPackedScoreType<depparser::SCORE_TYPE, depparser::action::kMax> & scores);

  inline void NoReduce(
      const depparser::CStateItem * item,
      const CPackedScoreType<depparser::SCORE_TYPE, depparser::action::kMax> & scores);

  inline void NoPass(
      const depparser::CStateItem * item,
      const CPackedScoreType<depparser::SCORE_TYPE, depparser::action::kMax> & scores);

  inline void NoShift(
      const depparser::CStateItem * item,
      const CPackedScoreType<depparser::SCORE_TYPE, depparser::action::kMax> & scores);

  inline void LeftReduce(
      const depparser::CStateItem * item,
      const CPackedScoreType<depparser::SCORE_TYPE, depparser::action::kMax> & scores);

  inline void RightShift(
      const depparser::CStateItem * item,
      const CPackedScoreType<depparser::SCORE_TYPE, depparser::action::kMax> & scores);

  inline void PopRoot(
      const depparser::CStateItem * item,
      const CPackedScoreType<depparser::SCORE_TYPE, depparser::action::kMax> & scores);

  inline void LeftPass(
      const depparser::CStateItem * item,
      const CPackedScoreType<depparser::SCORE_TYPE, depparser::action::kMax> & scores);

  inline void RightPass(
      const depparser::CStateItem * item,
      const CPackedScoreType<depparser::SCORE_TYPE, depparser::action::kMax> & scores);

public:
   void TestSuite1();
};

}; // namespace TARGET_LANGUAGE

#endif
