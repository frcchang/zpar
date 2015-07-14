// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                              *
 * conparser.cpp - the action based parser implementation       *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2007.12                        *
 *                                                              *
 ****************************************************************/

#include "conparser.h"
#include "weight.h"

using namespace TARGET_LANGUAGE;
using namespace TARGET_LANGUAGE::conparser;

#define refer_or_allocate_tuple2(x, o1, o2) { if (amount == 0) x.refer(o1, o2); else x.allocate(o1, o2); }

/*===============================================================
 *
 * CConParser - the conparser for TARGET_LANGUAGE
 *
 *==============================================================*/

/*---------------------------------------------------------------
 *
 * getOrUpdateStackScore - manipulate the score from stack
 *
 *---------------------------------------------------------------*/

inline void CConParser::getOrUpdateStackScore( CWeight *cast_weights, CPackedScoreType<SCORE_TYPE, CAction::MAX> &retval, const CStateItem *item, const CAction &action, SCORE_TYPE amount , int round ) {

   retval.reset();

   if (m_Context.stacksize==0) return;

   static unsigned long j;

   static CCFGSet s0ts1tbt;
   s0ts1tbt.copy(m_Context.s0ts1tbt);

#ifdef _CHINESE_CFG_H
//   static unsigned long s0c_bracket_action;
//   static unsigned long s1c_bracket_action;
//   static unsigned long n0t_bracket_action;
//   static unsigned long s0cs1c_bracket_action;
//   static unsigned long s0cn0t_bracket_action;
#endif

//   static unsigned long s0cs1c_distaction;

#ifdef _CHINESE_CFG_H
//   s0c_bracket_action = encodeAction(action, m_Context.s0c_bracket);
//   s1c_bracket_action = encodeAction(action, m_Context.s1c_bracket);
//   n0t_bracket_action = encodeAction(action, m_Context.n0t_bracket);
//   s0cs1c_bracket_action = encodeAction(action, m_Context.s0cs1c_bracket);
//   s0cn0t_bracket_action = encodeAction(action, m_Context.s0cn0t_bracket);
#endif

//   s0cs1c_distaction = encodeAction(action, m_Context.s0cs1c_dist);

   static CTuple2<CWord, CConstituent> word_constituent;
   static CTuple2<CTag, CConstituent> tag_constituent;
   static CTuple2<CTwoWords, CCFGSet> twoword_cfgset;
   static CTuple2<CWord, CCFGSet> word_cfgset;

   static CActionType actionType;
   actionType.code = action.type();

   const CAction &a1 = item->action;
   const CAction &a2 = item->statePtr->action;

   static CTuple2<CAction, CAction> tuple_action2;

//   CWeight* cast_weights = (amount&&(round!=-1)) ? m_delta : static_cast<CWeight*>(m_weights);

   // S0
   cast_weights->m_mapS0w.getOrUpdateScore(retval, *(m_Context.s0wt), action.code(), m_nScoreIndex, amount, round);
   if (!m_Context.s0c.empty()) cast_weights->m_mapS0c.getOrUpdateScore(retval, m_Context.s0c, action.code(), m_nScoreIndex, amount, round);
   refer_or_allocate_tuple2(tag_constituent, &(m_Context.s0t), &(m_Context.s0c));
   cast_weights->m_mapS0tc.getOrUpdateScore(retval, tag_constituent, action.code(), m_nScoreIndex, amount, round);
   refer_or_allocate_tuple2(word_constituent, m_Context.s0w, &(m_Context.s0c));
   cast_weights->m_mapS0wc.getOrUpdateScore(retval, word_constituent, action.code(), m_nScoreIndex, amount, round);

   // S1
   if (m_Context.s1!=0) {
      cast_weights->m_mapS1w.getOrUpdateScore(retval, *(m_Context.s1wt), action.code(), m_nScoreIndex, amount, round);
      if (!m_Context.s1c.empty()) cast_weights->m_mapS1c.getOrUpdateScore(retval, m_Context.s1c, action.code(), m_nScoreIndex, amount, round);
      refer_or_allocate_tuple2(tag_constituent, &(m_Context.s1t), &(m_Context.s1c));
      cast_weights->m_mapS1tc.getOrUpdateScore(retval, tag_constituent, action.code(), m_nScoreIndex, amount, round);
      refer_or_allocate_tuple2(word_constituent, m_Context.s1w, &(m_Context.s1c));
      cast_weights->m_mapS1wc.getOrUpdateScore(retval, word_constituent, action.code(), m_nScoreIndex, amount, round);
   }

   // S2
   if (m_Context.s2!=0) {
//      cast_weights->m_mapS2w.getOrUpdateScore(retval, *(m_Context.s2w), action.code(), m_nScoreIndex, amount, round);
//      cast_weights->m_mapS2c.getOrUpdateScore(retval, s2c_action, action.code(), m_nScoreIndex, amount, round);
      refer_or_allocate_tuple2(tag_constituent, &(m_Context.s2t), &(m_Context.s2c));
      cast_weights->m_mapS2tc.getOrUpdateScore(retval, tag_constituent, action.code(), m_nScoreIndex, amount, round);
      refer_or_allocate_tuple2(word_constituent, m_Context.s2w, &(m_Context.s2c));
      cast_weights->m_mapS2wc.getOrUpdateScore(retval, word_constituent, action.code(), m_nScoreIndex, amount, round);
   }

   // S3
   if (m_Context.s3!=0) {
//      cast_weights->m_mapS3w.getOrUpdateScore(retval, *(m_Context.s3w), action.code(), m_nScoreIndex, amount, round);
//      cast_weights->m_mapS3c.getOrUpdateScore(retval, s3c_action, action.code(), m_nScoreIndex, amount, round);
      refer_or_allocate_tuple2(tag_constituent, &(m_Context.s3t), &(m_Context.s3c));
      cast_weights->m_mapS3tc.getOrUpdateScore(retval, tag_constituent, action.code(), m_nScoreIndex, amount, round);
      refer_or_allocate_tuple2(word_constituent, m_Context.s3w, &(m_Context.s3c));
      cast_weights->m_mapS3wc.getOrUpdateScore(retval, word_constituent, action.code(), m_nScoreIndex, amount, round);
   }

   // N0
   if (m_Context.n0!=-1) {
//      cast_weights->m_mapN0w.getOrUpdateScore(retval, *(m_Context.n0w), action.code(), m_nScoreIndex, amount, round);
//      cast_weights->m_mapN0t.getOrUpdateScore(retval, n0t_action, action.code(), m_nScoreIndex, amount, round);
      cast_weights->m_mapN0wt.getOrUpdateScore(retval, *(m_Context.n0wt), action.code(), m_nScoreIndex, amount, round);
   }

   // N1
   if (m_Context.n1!=-1) {
//      cast_weights->m_mapN1w.getOrUpdateScore(retval, *(m_Context.n1w), action.code(), m_nScoreIndex, amount, round);
//      cast_weights->m_mapN1t.getOrUpdateScore(retval, n1t_action, action.code(), m_nScoreIndex, amount, round);
      cast_weights->m_mapN1wt.getOrUpdateScore(retval, *(m_Context.n1wt), action.code(), m_nScoreIndex, amount, round);
   }

   // N2
   if (m_Context.n2!=-1) {
//      cast_weights->m_mapN2w.getOrUpdateScore(retval, *(m_Context.n2w), action.code(), m_nScoreIndex, amount, round);
//      cast_weights->m_mapN2t.getOrUpdateScore(retval, n2t_action, action.code(), m_nScoreIndex, amount, round);
      cast_weights->m_mapN2wt.getOrUpdateScore(retval, *(m_Context.n2wt), action.code(), m_nScoreIndex, amount, round);
   }

   // N3
   if (m_Context.n3!=-1) {
//      cast_weights->m_mapN3w.getOrUpdateScore(retval, *(m_Context.n3w), action.code(), m_nScoreIndex, amount, round);
//      cast_weights->m_mapN3t.getOrUpdateScore(retval, n3t_action, action.code(), m_nScoreIndex, amount, round);
      cast_weights->m_mapN3wt.getOrUpdateScore(retval, *(m_Context.n3wt), action.code(), m_nScoreIndex, amount, round);
   }

   // S0L
   if (m_Context.s0l!=0) {
      refer_or_allocate_tuple2(tag_constituent, &(m_Context.s0lt), &(m_Context.s0lc));
      cast_weights->m_mapS0Ltc.getOrUpdateScore(retval, tag_constituent, action.code(), m_nScoreIndex, amount, round);
      refer_or_allocate_tuple2(word_constituent, m_Context.s0lw, &(m_Context.s0lc));
      cast_weights->m_mapS0Lwc.getOrUpdateScore(retval, word_constituent, action.code(), m_nScoreIndex, amount, round);
   }

   // S0R
   if (m_Context.s0r!=0) {
      refer_or_allocate_tuple2(tag_constituent, &(m_Context.s0rt), &(m_Context.s0rc));
      cast_weights->m_mapS0Rtc.getOrUpdateScore(retval, tag_constituent, action.code(), m_nScoreIndex, amount, round);
      refer_or_allocate_tuple2(word_constituent, m_Context.s0rw, &(m_Context.s0rc));
      cast_weights->m_mapS0Rwc.getOrUpdateScore(retval, word_constituent, action.code(), m_nScoreIndex, amount, round);
   }

   // S0U
   if (m_Context.s0u!=0) {
      refer_or_allocate_tuple2(tag_constituent, &(m_Context.s0ut), &(m_Context.s0uc));
      cast_weights->m_mapS0Utc.getOrUpdateScore(retval, tag_constituent, action.code(), m_nScoreIndex, amount, round);
      refer_or_allocate_tuple2(word_constituent, m_Context.s0uw, &(m_Context.s0uc));
      cast_weights->m_mapS0Uwc.getOrUpdateScore(retval, word_constituent, action.code(), m_nScoreIndex, amount, round);
   }


   // S1L
   if (m_Context.s1l!=0) {
      refer_or_allocate_tuple2(tag_constituent, &(m_Context.s1lt), &(m_Context.s1lc));
      cast_weights->m_mapS1Ltc.getOrUpdateScore(retval, tag_constituent, action.code(), m_nScoreIndex, amount, round);
      refer_or_allocate_tuple2(word_constituent, m_Context.s1lw, &(m_Context.s1lc));
      cast_weights->m_mapS1Lwc.getOrUpdateScore(retval, word_constituent, action.code(), m_nScoreIndex, amount, round);
   }

   // S1R
   if (m_Context.s1r!=0) {
      refer_or_allocate_tuple2(tag_constituent, &(m_Context.s1rt), &(m_Context.s1rc));
      cast_weights->m_mapS1Rtc.getOrUpdateScore(retval, tag_constituent, action.code(), m_nScoreIndex, amount, round);
      refer_or_allocate_tuple2(word_constituent, m_Context.s1rw, &(m_Context.s1rc));
      cast_weights->m_mapS1Rwc.getOrUpdateScore(retval, word_constituent, action.code(), m_nScoreIndex, amount, round);
   }

   // S1U
   if (m_Context.s1u!=0) {
      refer_or_allocate_tuple2(tag_constituent, &(m_Context.s1ut), &(m_Context.s1uc));
      cast_weights->m_mapS1Utc.getOrUpdateScore(retval, tag_constituent, action.code(), m_nScoreIndex, amount, round);
      refer_or_allocate_tuple2(word_constituent, m_Context.s1uw, &(m_Context.s1uc));
      cast_weights->m_mapS1Uwc.getOrUpdateScore(retval, word_constituent, action.code(), m_nScoreIndex, amount, round);
   }

   // S0 S1
   if (m_Context.s1!=0) {
      refer_or_allocate_tuple2(twoword_cfgset, &(m_Context.s0ws1w), &(m_Context.s0cs1c));
      cast_weights->m_mapS0wcS1wc.getOrUpdateScore(retval, twoword_cfgset, action.code(), m_nScoreIndex, amount, round);
      refer_or_allocate_tuple2(word_cfgset, m_Context.s1w, &(m_Context.s0cs1c));
      cast_weights->m_mapS0cS1w.getOrUpdateScore(retval, word_cfgset, action.code(), m_nScoreIndex, amount, round);
      refer_or_allocate_tuple2(word_cfgset, m_Context.s0w, &(m_Context.s0cs1c));
      cast_weights->m_mapS0wS1c.getOrUpdateScore(retval, word_cfgset, action.code(), m_nScoreIndex, amount, round);
      cast_weights->m_mapS0cS1c.getOrUpdateScore(retval, m_Context.s0cs1c, action.code(), m_nScoreIndex, amount, round);

   }

   // S0 N0
   if (m_Context.n0!=-1) {
      refer_or_allocate_tuple2(twoword_cfgset, &(m_Context.s0wn0w), &(m_Context.s0cn0t));
      cast_weights->m_mapS0wN0w.getOrUpdateScore(retval, twoword_cfgset, action.code(), m_nScoreIndex, amount, round);
      refer_or_allocate_tuple2(word_cfgset, m_Context.n0w, &(m_Context.s0cn0t));
      cast_weights->m_mapS0cN0w.getOrUpdateScore(retval, word_cfgset, action.code(), m_nScoreIndex, amount, round);
      refer_or_allocate_tuple2(word_cfgset, m_Context.s0w, &(m_Context.s0cn0t));
      cast_weights->m_mapS0wN0t.getOrUpdateScore(retval, word_cfgset, action.code(), m_nScoreIndex, amount, round);
      cast_weights->m_mapS0cN0t.getOrUpdateScore(retval, m_Context.s0cn0t, action.code(), m_nScoreIndex, amount, round);
   }

   // S1 N0
   if (m_Context.s1!=0 && m_Context.n0!=-1) {
      refer_or_allocate_tuple2(twoword_cfgset, &(m_Context.s1wn0w), &(m_Context.s1cn0t));
      cast_weights->m_mapS1wN0w.getOrUpdateScore(retval, twoword_cfgset, action.code(), m_nScoreIndex, amount, round);
      refer_or_allocate_tuple2(word_cfgset, m_Context.n0w, &(m_Context.s1cn0t));
      cast_weights->m_mapS1cN0w.getOrUpdateScore(retval, word_cfgset, action.code(), m_nScoreIndex, amount, round);
      refer_or_allocate_tuple2(word_cfgset, m_Context.s1w, &(m_Context.s1cn0t));
      cast_weights->m_mapS1wN0t.getOrUpdateScore(retval, word_cfgset, action.code(), m_nScoreIndex, amount, round);
      cast_weights->m_mapS1cN0t.getOrUpdateScore(retval, m_Context.s1cn0t, action.code(), m_nScoreIndex, amount, round);
   }

   // N0 N1
   if (m_Context.n1!=-1) {
      refer_or_allocate_tuple2(twoword_cfgset, &(m_Context.n0wn1w), &(m_Context.n0tn1t));
      cast_weights->m_mapN0wN1w.getOrUpdateScore(retval, twoword_cfgset, action.code(), m_nScoreIndex, amount, round);
      refer_or_allocate_tuple2(word_cfgset, m_Context.n1w, &(m_Context.n0tn1t));
      cast_weights->m_mapN0tN1w.getOrUpdateScore(retval, word_cfgset, action.code(), m_nScoreIndex, amount, round);
      refer_or_allocate_tuple2(word_cfgset, m_Context.n0w, &(m_Context.n0tn1t));
      cast_weights->m_mapN0wN1t.getOrUpdateScore(retval, word_cfgset, action.code(), m_nScoreIndex, amount, round);
      cast_weights->m_mapN0tN1t.getOrUpdateScore(retval, m_Context.n0tn1t, action.code(), m_nScoreIndex, amount, round);
   }

#ifdef _CHINESE_CFG_H
/* BRACKET PUNC */
#endif

   // S0 S1 N0
   refer_or_allocate_tuple2(word_cfgset, m_Context.s0w, &(m_Context.s0cs1cn0t));
   cast_weights->m_mapS0wS1cN0t.getOrUpdateScore(retval, word_cfgset, action.code(), m_nScoreIndex, amount, round);
   if (m_Context.s1!=0)  {
      refer_or_allocate_tuple2(word_cfgset, m_Context.s1w, &(m_Context.s0cs1cn0t));
      cast_weights->m_mapS0cS1wN0t.getOrUpdateScore(retval, word_cfgset, action.code(), m_nScoreIndex, amount, round);
   }
   if (m_Context.n0!=-1) {
      refer_or_allocate_tuple2(word_cfgset, m_Context.n0w, &(m_Context.s0cs1cn0t));
      cast_weights->m_mapS0cS1cN0w.getOrUpdateScore(retval, word_cfgset, action.code(), m_nScoreIndex, amount, round);
   }
   cast_weights->m_mapS0cS1cN0t.getOrUpdateScore(retval, m_Context.s0cs1cn0t, action.code(), m_nScoreIndex, amount, round);

   cast_weights->m_mapS0tS1tN0t.getOrUpdateScore(retval, m_Context.s0ts1tn0t, action.code(), m_nScoreIndex, amount, round);

   // S0 N0 N1
   if (m_Context.n0!=-1) {
      refer_or_allocate_tuple2(word_cfgset, m_Context.s0w, &(m_Context.s0cn0tn1t));
      cast_weights->m_mapS0wN0tN1t.getOrUpdateScore(retval, word_cfgset, action.code(), m_nScoreIndex, amount, round);
      refer_or_allocate_tuple2(word_cfgset, m_Context.n0w, &(m_Context.s0cn0tn1t));
      cast_weights->m_mapS0cN0wN1t.getOrUpdateScore(retval, word_cfgset, action.code(), m_nScoreIndex, amount, round);
      if (m_Context.n1!=-1) {
         refer_or_allocate_tuple2(word_cfgset, m_Context.n1w, &(m_Context.s0cn0tn1t));
         cast_weights->m_mapS0cN0tN1w.getOrUpdateScore(retval, word_cfgset, action.code(), m_nScoreIndex, amount, round);
      }
      cast_weights->m_mapS0cN0tN1t.getOrUpdateScore(retval, m_Context.s0cn0tn1t, action.code(), m_nScoreIndex, amount, round); // m_Context.n0

      cast_weights->m_mapS0tN0tN1t.getOrUpdateScore(retval, m_Context.s0tn0tn1t, action.code(), m_nScoreIndex, amount, round); // m_Context.n0
   }

   // S0 S1 S2
   if (m_Context.s1!=0) {
      refer_or_allocate_tuple2(word_cfgset, m_Context.s0w, &(m_Context.s0cs1cs2c));
      cast_weights->m_mapS0wS1cS2c.getOrUpdateScore(retval, word_cfgset, action.code(), m_nScoreIndex, amount, round);
      refer_or_allocate_tuple2(word_cfgset, m_Context.s1w, &(m_Context.s0cs1cs2c));
      cast_weights->m_mapS0cS1wS2c.getOrUpdateScore(retval, word_cfgset, action.code(), m_nScoreIndex, amount, round);
      if (m_Context.s2!=0) {
         refer_or_allocate_tuple2(word_cfgset, m_Context.s2w, &(m_Context.s0cs1cs2c));
         cast_weights->m_mapS0cS1cS2w.getOrUpdateScore(retval, word_cfgset, action.code(), m_nScoreIndex, amount, round);
      }
      cast_weights->m_mapS0cS1cS2c.getOrUpdateScore(retval, m_Context.s0cs1cs2c, action.code(), m_nScoreIndex, amount, round);

      cast_weights->m_mapS0tS1tS2t.getOrUpdateScore(retval, m_Context.s0ts1ts2t, action.code(), m_nScoreIndex, amount, round);
   }

      if (m_Context.n0!=-1 && m_Context.s0r!=0) {
         cast_weights->m_mapS0cS0RcN0t.getOrUpdateScore(retval, m_Context.s0cs0rcn0t, action.code(), m_nScoreIndex, amount, round);
         cast_weights->m_mapS0cS0RjN0t.getOrUpdateScore(retval, m_Context.s0cs0rjn0t, action.code(), m_nScoreIndex, amount, round);
         refer_or_allocate_tuple2(word_cfgset, m_Context.n0w, &(m_Context.s0cs0rc));
         cast_weights->m_mapS0cS0RcN0w.getOrUpdateScore(retval, word_cfgset, action.code(), m_nScoreIndex, amount, round);
      }

   // S0 S0LRUS1
   if (m_Context.s1!=0 && m_Context.s0l!=0) {
      cast_weights->m_mapS0cS0LcS1c.getOrUpdateScore(retval, m_Context.s0cs0lcs1c, action.code(), m_nScoreIndex, amount, round);
      cast_weights->m_mapS0cS0LjS1j.getOrUpdateScore(retval, m_Context.s0cs0ljs1j, action.code(), m_nScoreIndex, amount, round);
      refer_or_allocate_tuple2(word_cfgset, m_Context.s1w, &(m_Context.s0cs0lc));
      cast_weights->m_mapS0cS0LcS1w.getOrUpdateScore(retval, word_cfgset, action.code(), m_nScoreIndex, amount, round);
   }
   if (m_Context.s1 != 0 && m_Context.s1r != 0) {
      cast_weights->m_mapS0cS1cS1Rc.getOrUpdateScore(retval, m_Context.s0cs1cs1rc, action.code(), m_nScoreIndex, amount, round);
      cast_weights->m_mapS0jS1cS1Rj.getOrUpdateScore(retval, m_Context.s0js1cs1rj, action.code(), m_nScoreIndex, amount, round);
      refer_or_allocate_tuple2(word_cfgset, m_Context.s0w, &(m_Context.s1cs1rc));
      cast_weights->m_mapS0wS1cS1Rc.getOrUpdateScore(retval, word_cfgset, action.code(), m_nScoreIndex, amount, round);
   }

//   cast_weights->m_mapA1.getOrUpdateScore(retval, a1, action.code(), m_nScoreIndex, amount, round);
//   refer_or_allocate_tuple2(tuple_action2, &a1, &a2);
//   cast_weights->m_mapA1A2.getOrUpdateScore(retval, tuple_action2, action.code(), m_nScoreIndex, amount, round);
}

/*---------------------------------------------------------------
 *
 * getGlobalScore - get the score of a parse tree
 *
 * Inputs: parse graph
 *
 *---------------------------------------------------------------*/

SCORE_TYPE CConParser::getGlobalScore(const CSentenceParsed &parsed) {
}

/*---------------------------------------------------------------
 *
 * updateScores - update the score std::vector
 *
 * This method is different from updateScoreVector in that
 * 1. It is for external call
 * 2. The tagging sequences for parsed and correct may differ
 *
 * Inputs: the parsed and the correct example
 *
 *---------------------------------------------------------------*/

void CConParser::updateScores(const CSentenceParsed & parsed , const CSentenceParsed & correct , int round ) {

   THROW("Unsupported method");
}

/*---------------------------------------------------------------
 *
 * updateScoresForState - update score
 *
 *--------------------------------------------------------------*/

void CConParser::updateScoresForState( CWeight *cast_weights , const CStateItem *item , const SCORE_UPDATE update) {

   const SCORE_TYPE amount = (update==eAdd ? 1 : -1);
   const static CStateItem* states[MAX_SENTENCE_SIZE*(2+UNARY_MOVES)+2];

   static int count;
   const CStateItem *current;

   static CPackedScoreType<SCORE_TYPE, CAction::MAX> scores;

   count = 0;
   current = item;
   while (current) {
      states[count] = current;
      ++count ; //updating
      current = current->statePtr;
   }
   --count; // state [0..count] are the reverse lifecycle of item.

   // for each
   while (count>0) {
      m_Context.load(states[count], m_lCache, m_lWordLen, true);
      // update action
      const CAction &action = states[count-1]->action;
      getOrUpdateStackScore(cast_weights, scores, states[count], action, amount, m_nTrainingRound );
      --count;
   }
}

#ifndef TRAIN_MULTI
/*---------------------------------------------------------------
 *
 * updateScoresForStates - update scores for states
 *
 *--------------------------------------------------------------*/

void CConParser::updateScoresForStates( const CStateItem *output , const CStateItem *correct ) {

   std::cerr << "updating parameters ... " ;

   static double F;
#ifdef TRAIN_LOSS
//   F = correct->FLoss();
   F = correct->HammingLoss();
   ASSERT(F==0,"correct"<<F);

//   F = output->FLoss();
   F = output->HammingLoss();
   updateScoresForState( m_delta, correct, eAdd );
   updateScoresForState( m_delta, output, eSubtract );
#else
   F = 1.0;
   updateScoresForState( m_delta, correct, eAdd );
   updateScoresForState( m_delta, output, eSubtract );
#endif

#ifdef TRAIN_MARGIN
//   double tou = (std::sqrt(F)+output->score-correct->score)/(m_delta->squareNorm());
   double tou = (F+output->score-correct->score)/(m_delta->squareNorm());
   m_delta->scaleCurrent(tou, m_nTrainingRound);
#endif
   static_cast<CWeight*>(m_weights)->addCurrent(m_delta, m_nTrainingRound);
   m_delta->clear();

   m_nTotalErrors++;
}

#else
/*---------------------------------------------------------------
 *
 * updateScoresForMultipleStates - update multi
 *
 *--------------------------------------------------------------*/

void CConParser::updateScoresForMultipleStates( const CStateItem *output_start , const CStateItem *output_end , const CStateItem  *candidate , const CStateItem *correct ) {
   std::cerr << "updating parameters ... " ;
   // computateDeltasDist
   unsigned K = 0;
   updateScoresForState(m_gold, correct, eAdd, 0);
   for (const CStateItem *item = output_start; item<output_end; ++item) {
      if (item->score >= correct->score) {
         updateScoresForState(m_delta[K], item, eSubtract, 0);
         m_delta[K]->addCurrent(m_gold, m_nTrainingRound);
         m_dist[K] = 1.0 + item->score - correct->score;
         ++K;
      }
   }
   if ( candidate && candidate->score > correct->score ) {
      updateScoresForState(m_delta[K], candidate, eSubtract, 0);
      m_delta[K]->addCurrent(m_gold, m_nTrainingRound);
      m_dist[K] = 1.0 + candidate->score - correct->score;
      ++K;
   }
   assert(K);
   // compuateAlpha
   computeAlpha(K);
   // update
   for (unsigned i=0; i<K; ++i) {
      m_delta[i]->scaleCurrent(m_alpha[i], m_nTrainingRound);
      static_cast<CWeight*>(m_weights)->addCurrent(m_delta[i], m_nTrainingRound);
   }
   // clear
   for (unsigned i=0; i<K; ++i)
      m_delta[i]->clear();
   m_gold->clear();
}

/*---------------------------------------------------------------
 *
 * compuateAlpha - hildreth
 *
 *--------------------------------------------------------------*/

void CConParser::computeAlpha( const unsigned K ) {
   static unsigned i;
   static unsigned iter;
   static double diff_alpha;
   static double add_alpha;

   static const unsigned max_iter = 1e4;
   static const double eps = 1e-7;
   static const double zero = 1e-11;

   static double kkt[MIRA_SIZE];
   static double max_kkt;
   static int max_kkt_i;
   static double A[MIRA_SIZE][MIRA_SIZE];
   static bool computed[MIRA_SIZE];

   for (i=0; i<K; ++i) {
      A[i][i] = m_delta[i]->squareNorm();
      computed[i] = false;
   }

   for (i=0; i<K; ++i) {
      m_alpha[i] = 0;
      kkt[i] = m_dist[i];
      if (i==0||kkt[i]>max_kkt) {
         max_kkt = kkt[i];
         max_kkt_i = i;
      }
   }

   iter = 0;
   while (max_kkt >= eps && iter<max_iter){
      diff_alpha = A[max_kkt_i][max_kkt_i] <= zero ? 0 : m_dist[max_kkt_i]/A[max_kkt_i][max_kkt_i];
      if (m_alpha[max_kkt_i]+diff_alpha<0) {
         add_alpha = -m_alpha[max_kkt_i];
      }
      else {
         add_alpha = diff_alpha;
      }
      m_alpha[max_kkt_i] += add_alpha;

      if (!computed[max_kkt_i]) {
         for (i=0; i<K; ++i) {
            A[i][max_kkt_i] = m_delta[i]->dotProduct(*(m_delta[max_kkt_i]));
         }
         computed[max_kkt_i] = true;
      }

      for (i=0; i<K; ++i) {
         m_dist[i] -= (add_alpha * A[i][max_kkt_i]);
         kkt[i] = m_dist[i];
         if (m_alpha[i] > zero)
            kkt[i] = abs(m_dist[i]);
      }

      for (i=0; i<K; ++i) {
         kkt[i] = m_dist[i];
         if (i==0||kkt[i]>max_kkt) {
            max_kkt_i = i;
            max_kkt = kkt[i];
         }
      }

      ++iter;
   }
}
#endif // TRAIN_MULTI

#ifdef TRAIN_LOSS

/*---------------------------------------------------------------
 *
 * getLabeledBrackets - turn a parse tree into a set of labeled brackets
 *
 *--------------------------------------------------------------*/

void CConParser::getLabeledBrackets(const CSentenceParsed &parse_tree, CStack<CLabeledBracket> &brackets) {
   std::vector<CLabeledBracket> vec;
   unsigned i;
   unsigned begin, end;
   unsigned long constituent;
   brackets.clear();
   for (i=0; i<parse_tree.nodes.size(); ++i) {
      const CCFGTreeNode &node = parse_tree.nodes[i];
      if (!node.is_constituent) {
         begin = node.token;
         end = begin;
      }
      else {
         begin = vec[node.left_child].begin;
         if (node.right_child == -1)
            end = vec[node.left_child].end;
         else
            end = vec[node.right_child].end;
      }
#ifdef NO_TEMP_CONSTITUENT
      constituent = node.constituent;
#else
      constituent = CConstituent::encodeTmp(node.constituent.code(), node.temp);
#endif
      vec.push_back(CLabeledBracket(begin, end, constituent));
      if (node.is_constituent)
         brackets.push(vec.back());
   } //for
}
#endif

/*---------------------------------------------------------------
 *
 * getOrUpdateScore - get or update the score of a state and act
 *
 *--------------------------------------------------------------*/

void CConParser::getOrUpdateScore( CPackedScoreType<SCORE_TYPE, CAction::MAX> &retval, const conparser::CStateItem &item, const conparser::CAction &action, conparser::SCORE_TYPE amount, int round ) {
   THROW("Not implemented");
}

/*---------------------------------------------------------------
 *
 * work - the working process shared by training and parsing
 *
 * Returns: makes a new instance of CSentenceParsed
 *
 *--------------------------------------------------------------*/

void CConParser::work( const bool bTrain , const CTwoStringVector &sentence , CSentenceParsed *retval , const CSentenceParsed &correct , int nBest , SCORE_TYPE *scores ) {

   static CStateItem lattice[(MAX_SENTENCE_SIZE*(2+UNARY_MOVES)+2)*AGENDA_SIZE];
   static CStateItem *lattice_index[MAX_SENTENCE_SIZE*(2+UNARY_MOVES)+2];

#ifdef DEBUG
   clock_t total_start_time = clock();
#endif
   const int length = sentence.size() ;

   const static CStateItem *pGenerator ;
   const static CStateItem *pBestGen;
   const static CStateItem *correctState ;
   static bool bCorrect ;  // used in learning for early update
   static int tmp_i, tmp_j;
   static CAction correct_action;
   static CScoredStateAction scored_correct_action;
   static std::vector<CAction> actions; // actions to apply for a candidate
   static CAgendaSimple<CScoredStateAction> beam(AGENDA_SIZE);
   static CScoredStateAction scored_action; // used rank actions
   ASSERT(nBest=1, "currently only do 1 best parse");
   // TODO: it is easy to extend this into N-best; just use a std::vector for candidate_output. during train maybe use the best to adjust
   const static CStateItem *candidate_output;
   static unsigned index;

   static CPackedScoreType<SCORE_TYPE, CAction::MAX> packedscores;

   assert(length<MAX_SENTENCE_SIZE);

   TRACE("Initialising the decoding process ... ") ;
   // initialise word cache
   m_lCache.clear();
   m_lWordLen.clear();
   candidate_output = 0;
   for ( tmp_i=0; tmp_i<length; tmp_i++ ) {
      m_lCache.push_back( CTaggedWord<CTag, TAG_SEPARATOR>(sentence[tmp_i].first , sentence[tmp_i].second) );
      m_lWordLen.push_back( getUTF8StringLength(sentence[tmp_i].first) );
   }
   // initialise agenda
   lattice_index[0] = lattice;
   lattice_index[0]->clear();
#ifdef TRAIN_LOSS
   getLabeledBrackets(correct, lattice_index[0]->gold_lb);
   TRACE(lattice_index[0]->gold_lb << std::endl);
#endif
   lattice_index[1] = lattice+1;
   if (bTrain) {
      correctState = lattice_index[0];
   }
   index=0;

   TRACE("Decoding start ... ") ;
   while (true) { // for each step

      ++index;
      if (lattice_index[index] == lattice_index[index-1]) { // no more generators
         if (candidate_output==0) {
             WARNING("Parser failed: cannot find a parse for the sentence.");
             ASSERT(retval, "Internal error: parser failed during training");
             retval->clear();
             return;
         }
         break; // finish
      }
      lattice_index[index+1] = lattice_index[index];

      beam.clear();

      if (bTrain) {
         pBestGen = 0;
         bCorrect = false;
         if (!correctState->IsTerminated()) {
            correctState->StandardMove(correct, correct_action);
         }
      }
      for (pGenerator=lattice_index[index-1]; pGenerator!=lattice_index[index]; ++pGenerator) { // for each generator

         // when an item is terminated, move it to potential outputs.
         if (pGenerator->IsTerminated()) {
            if (candidate_output==0 || pGenerator->score > candidate_output->score )
              candidate_output = pGenerator;
         }
         else {
            // load context
            m_Context.load(pGenerator, m_lCache, m_lWordLen, false);

            // get actions
            m_rule.getActions(*pGenerator, actions);
            getOrUpdateStackScore(static_cast<CWeight*>(m_weights), packedscores, pGenerator);
            for (tmp_j=0; tmp_j<actions.size(); ++tmp_j) {
               scored_action.load(actions[tmp_j], pGenerator, packedscores[actions[tmp_j].code()]);
               beam.insertItem(&scored_action);
               if (bTrain && !correctState->IsTerminated() && pGenerator == correctState && actions[tmp_j] == correct_action) {
                  scored_correct_action = scored_action;
               }
            }
         }

      } // done iterating generator item

      // insertItems
      for (tmp_j=0; tmp_j<beam.size(); ++tmp_j) { // insert from
         pGenerator = beam.item(tmp_j)->item;
         pGenerator->Move(lattice_index[index+1], beam.item(tmp_j)->action);
         lattice_index[index+1]->score = beam.item(tmp_j)->score;
         // update bestgen
         if (bTrain) {
            if ( pBestGen == 0 || lattice_index[index+1]->score > pBestGen->score ) {
               pBestGen = lattice_index[index+1];
            }
            if ( pGenerator == correctState && beam.item(tmp_j)->action == correct_action ) {
               // pGenerator==correctState implies correctState not finished
               // since a finished generator will not be put into beam
               // and thus it implis that action is valid
               correctState = lattice_index[index+1];
               assert (correctState->unaryreduces()<=UNARY_MOVES) ;
               bCorrect = true;
            }
         }
         ++lattice_index[index+1];
      }

      // update items if correct item jump out of the agenda
      if (bTrain) {
#ifdef EARLY_UPDATE
         // note that if bCorrect == true then the correct state has
         // already been updated, and the new value is one of the new states
         // among the newly produced from lattice[index+1].
         if (!bCorrect && candidate_output!=correctState) {
            if (!correctState->IsTerminated()) {
               correctState->Move(lattice_index[index+1], correct_action);
               correctState = lattice_index[index+1];
               lattice_index[index+1]->score = scored_correct_action.score;
            }
            TRACE("Error at the "<<correctState->current_word<<"th word; total is "<<m_lCache.size())
            // update
#ifdef TRAIN_MULTI
            updateScoresForMultipleStates(lattice_index[index], lattice_index[index+1], candidate_output, correctState) ;
#else
            if (pBestGen == 0 || candidate_output && candidate_output->score > pBestGen->score )
               pBestGen = candidate_output;
            // trace
            correctState->trace(&sentence);
            pBestGen->trace(&sentence);
            updateScoresForStates(pBestGen, correctState) ;
#endif // TRAIN_MULTI
            return ;
         }
#endif
      }  // end of update
   } // while

   if (bTrain) {
      // make sure that the correct item is stack top finally
      if ( candidate_output != correctState ) {
//         if (!correctState->IsTerminated()) {
//            correctState->Move(lattice_index[index+1], correct_action);
//            correctState = lattice_index[index+1];
//            lattice_index[index+1]->score = scored_correct_action.score;
//         }
         TRACE("The best item is not the correct one")
#ifdef TRAIN_MULTI
         updateScoresForMultipleStates(lattice_index[index], lattice_index[index+1], candidate_output, correctState) ;
#else // TRAIN_MULTI
         correctState->trace(&sentence);
         candidate_output->trace(&sentence);
         updateScoresForStates(candidate_output, correctState) ;
#endif // TRAIN_MULTI
         return ;
      }
   }

   if (!retval)
      return;

   TRACE("Outputing sentence");
   candidate_output->GenerateTree( sentence, retval[0] );
   if (scores) scores[0] = candidate_output->score;

   TRACE("Done, the highest score is: " << candidate_output->score ) ;
   TRACE("The total time spent: " << double(clock() - total_start_time)/CLOCKS_PER_SEC) ;
}

/*---------------------------------------------------------------
 *
 * parse - do constituent parsing to a sentence
 *
 * Returns: makes a new instance of CSentenceParsed
 *
 *--------------------------------------------------------------*/

void CConParser::parse( const CTwoStringVector &sentence , CSentenceParsed *retval , int nBest , SCORE_TYPE *scores ) {

   static CSentenceParsed empty ;

   work(false, sentence, retval, empty, nBest, scores ) ;

}

/*---------------------------------------------------------------
 *
 * parse - do constituent parsing to a sentence
 *
 * Returns: makes a new instance of CSentenceParsed
 *
 *--------------------------------------------------------------*/

void CConParser::parse( const CSentenceMultiCon<CConstituent> &sentence , CSentenceParsed *retval , int nBest , SCORE_TYPE *scores ) {

   static CSentenceParsed empty ;

   m_rule.SetLexConstituents( sentence.constituents );
   work(false, sentence.words, retval, empty, nBest, scores ) ;
   m_rule.UnsetLexConstituents();

}

/*---------------------------------------------------------------
 *
 * train - train the models with an example
 *
 *---------------------------------------------------------------*/

void CConParser::train( const CSentenceParsed &correct , int round ) {

   static CTwoStringVector sentence ;
//   static CSentenceParsed output ;

   UnparseSentence( &correct, &sentence ) ;

   // The following code does update for each processing stage
   m_nTrainingRound = round ;
//   work( true , sentence , &output , correct , 1 , 0 ) ;
   work( true , sentence , 0 , correct , 1 , 0 ) ;

};

/*---------------------------------------------------------------
 *
 * train - train the models with an example
 *
 *---------------------------------------------------------------*/

void CConParser::train( const CSentenceMultiCon<CConstituent> &con_input, const CSentenceParsed &correct , int round ) {

   // The following code does update for each processing stage
   m_nTrainingRound = round ;

   m_rule.SetLexConstituents( con_input.constituents );
   work( true , con_input.words , 0 , correct , 1 , 0 ) ;
   m_rule.UnsetLexConstituents();

};

#ifdef NO_NEG_FEATURE
/*---------------------------------------------------------------
 *
 * getPositiveFeatures - get positive features
 *
 *---------------------------------------------------------------*/

void CConParser::getPositiveFeatures( const CSentenceParsed &correct ) {
   static CTwoStringVector sentence;
   static CStateItem states[MAX_SENTENCE_SIZE*(1+UNARY_MOVES)+2];
   static CPackedScoreType<SCORE_TYPE, CAction::MAX> scores;
   static int current;
   static CAction action;

   states[0].clear();
   current = 0;
   UnparseSentence( &correct, &sentence ) ;
   m_lCache.clear();
   m_lWordLen.clear();
   for (unsigned i=0; i<sentence.size(); ++i) {
      m_lCache.push_back( CTaggedWord<CTag, TAG_SEPARATOR>(sentence[i].first , sentence[i].second) );
      m_lWordLen.push_back( getUTF8StringLength(sentence[i].first) );
   }

   while ( !states[current].IsTerminated() ) {
      states[current].StandardMove(correct, action);
//std::cerr << action << std::endl;
      m_Context.load(states+current, m_lCache, m_lWordLen, true);
      getOrUpdateStackScore(static_cast<CWeight*>(m_weights), scores, states+current, action, 1, -1);
      states[current].Move(states+current+1, action);
      ++current;
   }
}
#endif
