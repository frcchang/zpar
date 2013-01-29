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
#define refer_or_allocate_tuple3(x, o1, o2, o3) { if (amount == 0) x.refer(o1, o2, o3); else x.allocate(o1, o2, o3); }
#define refer_or_allocate_tuple4(x, o1, o2, o3, o4) { if (amount == 0) x.refer(o1, o2, o3, o4); else x.allocate(o1, o2, o3, o4); }

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

   //if (m_Context.stacksize==0) return;

   static unsigned long j;


   static CTuple2<CWord, CConstituent> word_constituent; 
   static CTuple2<CTag, CConstituent> tag_constituent;
   static CTuple2<CTwoWords, CCFGSet> twoword_cfgset;
   static CTuple2<CWord, CCFGSet> word_cfgset;

   static CTuple2<CWord, CWord> biword;
   static CTuple3<CWord, CWord, CWord> triword;

   static CTuple2<CTag, CTag> bitag;
   static CTuple3<CTag, CTag, CTag> tritag;

   static CTuple2<CWord, CTag> wordtag;
   static CTuple3<CWord, CTag, CTag> wordtagtag;
   static CTuple3<CWord, CWord, CTag> wordwordtag;
   static CTuple4<CTag, CTag, CTag, int> tritagint;
   static CTuple4<CWord, CTag, CTag, CTag> wordtagtagtag;

   static CTuple3<CWord, CWord, CConstituent> biword_constituent;

   static CTuple3<CTag, CTag, int> bitag_int;
   static CTuple2<CTag, unsigned long long> tagint;


   static CActionType actionType;
   actionType.code = action.type();

   const CAction &a1 = item->action;
   const CAction &a2 = item->statePtr->action;

   static CTuple2<CAction, CAction> tuple_action2;

   static CTwoTaggedWords wt12;
   static CTaggedWord<CTag, TAG_SEPARATOR> wt1, wt2;

//   CWeight* cast_weights = (amount&&(round!=-1)) ? m_delta : static_cast<CWeight*>(m_weights);
   if(actionType.code == CActionType::REDUCE_BINARY || actionType.code == CActionType::REDUCE_UNARY
   || actionType.code == CActionType::POP_ROOT || actionType.code == CActionType::IDLE || actionType.code == CActionType::SHIFT_S)
   {
		cast_weights->m_mapS0w.getOrUpdateScore(retval, m_Context.s0wt, action.code(), m_nScoreIndex, amount, round);
		if (!m_Context.s0c.empty()) cast_weights->m_mapS0c.getOrUpdateScore(retval, m_Context.s0c, action.code(), m_nScoreIndex, amount, round);
		refer_or_allocate_tuple2(tag_constituent, &(m_Context.s0t), &(m_Context.s0c));
		cast_weights->m_mapS0tc.getOrUpdateScore(retval, tag_constituent, action.code(), m_nScoreIndex, amount, round);
		refer_or_allocate_tuple2(word_constituent, &(m_Context.s0w), &(m_Context.s0c));
		cast_weights->m_mapS0wc.getOrUpdateScore(retval, word_constituent, action.code(), m_nScoreIndex, amount, round);

		if (m_Context.s1!=0) {
		      cast_weights->m_mapS1w.getOrUpdateScore(retval, m_Context.s1wt, action.code(), m_nScoreIndex, amount, round);
		      if (!m_Context.s1c.empty()) cast_weights->m_mapS1c.getOrUpdateScore(retval, m_Context.s1c, action.code(), m_nScoreIndex, amount, round);
		      refer_or_allocate_tuple2(tag_constituent, &(m_Context.s1t), &(m_Context.s1c));
		      cast_weights->m_mapS1tc.getOrUpdateScore(retval, tag_constituent, action.code(), m_nScoreIndex, amount, round);
		      refer_or_allocate_tuple2(word_constituent, &(m_Context.s1w), &(m_Context.s1c));
		      cast_weights->m_mapS1wc.getOrUpdateScore(retval, word_constituent, action.code(), m_nScoreIndex, amount, round);
		   }

	   if (m_Context.s2!=0) {
	//      cast_weights->m_mapS2w.getOrUpdateScore(retval, *(m_Context.s2w), action.code(), m_nScoreIndex, amount, round);
	//      cast_weights->m_mapS2c.getOrUpdateScore(retval, s2c_action, action.code(), m_nScoreIndex, amount, round);
	      refer_or_allocate_tuple2(tag_constituent, &(m_Context.s2t), &(m_Context.s2c));
	      cast_weights->m_mapS2tc.getOrUpdateScore(retval, tag_constituent, action.code(), m_nScoreIndex, amount, round);
	      refer_or_allocate_tuple2(word_constituent, &(m_Context.s2w), &(m_Context.s2c));
	      cast_weights->m_mapS2wc.getOrUpdateScore(retval, word_constituent, action.code(), m_nScoreIndex, amount, round);
	   }

	   if (m_Context.s3!=0) {
	//      cast_weights->m_mapS3w.getOrUpdateScore(retval, *(m_Context.s3w), action.code(), m_nScoreIndex, amount, round);
	//      cast_weights->m_mapS3c.getOrUpdateScore(retval, s3c_action, action.code(), m_nScoreIndex, amount, round);
	      refer_or_allocate_tuple2(tag_constituent, &(m_Context.s3t), &(m_Context.s3c));
	      cast_weights->m_mapS3tc.getOrUpdateScore(retval, tag_constituent, action.code(), m_nScoreIndex, amount, round);
	      refer_or_allocate_tuple2(word_constituent, &(m_Context.s3w), &(m_Context.s3c));
	      cast_weights->m_mapS3wc.getOrUpdateScore(retval, word_constituent, action.code(), m_nScoreIndex, amount, round);
	   }

	   // N0
	   if (m_Context.n0!=-1) {
	   	cast_weights->m_mapN0w.getOrUpdateScore(retval, m_Context.n0z, action.code(), m_nScoreIndex, amount, round);
	//      cast_weights->m_mapN0t.getOrUpdateScore(retval, n0t_action, action.code(), m_nScoreIndex, amount, round);
	      //cast_weights->m_mapN0wt.getOrUpdateScore(retval, m_Context.n0wt, action.code(), m_nScoreIndex, amount, round);
	   }

	   // N1
	   if (m_Context.n1!=-1) {
	   	cast_weights->m_mapN1w.getOrUpdateScore(retval, m_Context.n1z, action.code(), m_nScoreIndex, amount, round);
	   	refer_or_allocate_tuple2(biword, &(m_Context.n0z), &(m_Context.n1z));
	   	cast_weights->m_mapN01w.getOrUpdateScore(retval, biword, action.code(), m_nScoreIndex, amount, round);
	//      cast_weights->m_mapN1t.getOrUpdateScore(retval, n1t_action, action.code(), m_nScoreIndex, amount, round);
	     // cast_weights->m_mapN1wt.getOrUpdateScore(retval, m_Context.n1wt, action.code(), m_nScoreIndex, amount, round);
	   }

	   // N2
	   if (m_Context.n2!=-1) {
	      cast_weights->m_mapN2w.getOrUpdateScore(retval, m_Context.n2z, action.code(), m_nScoreIndex, amount, round);
	      refer_or_allocate_tuple2(biword, &(m_Context.n1z), &(m_Context.n2z));
	      cast_weights->m_mapN12w.getOrUpdateScore(retval, biword, action.code(), m_nScoreIndex, amount, round);
	      refer_or_allocate_tuple3(triword, &(m_Context.n0z), &(m_Context.n1z), &(m_Context.n2z));
	      cast_weights->m_mapN123w.getOrUpdateScore(retval, triword, action.code(), m_nScoreIndex, amount, round);
	//      cast_weights->m_mapN2t.getOrUpdateScore(retval, n2t_action, action.code(), m_nScoreIndex, amount, round);
	      //cast_weights->m_mapN2wt.getOrUpdateScore(retval, m_Context.n2wt, action.code(), m_nScoreIndex, amount, round);
	   }

	   // N3
	   if (m_Context.n3!=-1) {
	      cast_weights->m_mapN3w.getOrUpdateScore(retval, m_Context.n3z, action.code(), m_nScoreIndex, amount, round);
	      refer_or_allocate_tuple2(biword, &(m_Context.n2z), &(m_Context.n3z));
	      cast_weights->m_mapN23w.getOrUpdateScore(retval, biword, action.code(), m_nScoreIndex, amount, round);
	      refer_or_allocate_tuple3(triword, &(m_Context.n1z), &(m_Context.n2z), &(m_Context.n3z));
	      cast_weights->m_mapN123w.getOrUpdateScore(retval, triword, action.code(), m_nScoreIndex, amount, round);
	//      cast_weights->m_mapN3w.getOrUpdateScore(retval, *(m_Context.n3w), action.code(), m_nScoreIndex, amount, round);
	//      cast_weights->m_mapN3t.getOrUpdateScore(retval, n3t_action, action.code(), m_nScoreIndex, amount, round);
	      //cast_weights->m_mapN3wt.getOrUpdateScore(retval, m_Context.n3wt, action.code(), m_nScoreIndex, amount, round);
	   }

	   if (m_Context.s0l!=0) {
	      refer_or_allocate_tuple2(tag_constituent, &(m_Context.s0lt), &(m_Context.s0lc));
	      cast_weights->m_mapS0Ltc.getOrUpdateScore(retval, tag_constituent, action.code(), m_nScoreIndex, amount, round);
	      refer_or_allocate_tuple2(word_constituent, &(m_Context.s0lw), &(m_Context.s0lc));
	      cast_weights->m_mapS0Lwc.getOrUpdateScore(retval, word_constituent, action.code(), m_nScoreIndex, amount, round);
	   }

	   // S0R
	   if (m_Context.s0r!=0) {
	      refer_or_allocate_tuple2(tag_constituent, &(m_Context.s0rt), &(m_Context.s0rc));
	      cast_weights->m_mapS0Rtc.getOrUpdateScore(retval, tag_constituent, action.code(), m_nScoreIndex, amount, round);
	      refer_or_allocate_tuple2(word_constituent, &(m_Context.s0rw), &(m_Context.s0rc));
	      cast_weights->m_mapS0Rwc.getOrUpdateScore(retval, word_constituent, action.code(), m_nScoreIndex, amount, round);
	   }

	   // S0U
	   if (m_Context.s0u!=0) {
	      refer_or_allocate_tuple2(tag_constituent, &(m_Context.s0ut), &(m_Context.s0uc));
	      cast_weights->m_mapS0Utc.getOrUpdateScore(retval, tag_constituent, action.code(), m_nScoreIndex, amount, round);
	      refer_or_allocate_tuple2(word_constituent, &(m_Context.s0uw), &(m_Context.s0uc));
	      cast_weights->m_mapS0Uwc.getOrUpdateScore(retval, word_constituent, action.code(), m_nScoreIndex, amount, round);
	   }


	   // S1L
	   if (m_Context.s1l!=0) {
	      refer_or_allocate_tuple2(tag_constituent, &(m_Context.s1lt), &(m_Context.s1lc));
	      cast_weights->m_mapS1Ltc.getOrUpdateScore(retval, tag_constituent, action.code(), m_nScoreIndex, amount, round);
	      refer_or_allocate_tuple2(word_constituent, &(m_Context.s1lw), &(m_Context.s1lc));
	      cast_weights->m_mapS1Lwc.getOrUpdateScore(retval, word_constituent, action.code(), m_nScoreIndex, amount, round);
	   }

	   // S1R
	   if (m_Context.s1r!=0) {
	      refer_or_allocate_tuple2(tag_constituent, &(m_Context.s1rt), &(m_Context.s1rc));
	      cast_weights->m_mapS1Rtc.getOrUpdateScore(retval, tag_constituent, action.code(), m_nScoreIndex, amount, round);
	      refer_or_allocate_tuple2(word_constituent, &(m_Context.s1rw), &(m_Context.s1rc));
	      cast_weights->m_mapS1Rwc.getOrUpdateScore(retval, word_constituent, action.code(), m_nScoreIndex, amount, round);
	   }

	   // S1U
	   if (m_Context.s1u!=0) {
	      refer_or_allocate_tuple2(tag_constituent, &(m_Context.s1ut), &(m_Context.s1uc));
	      cast_weights->m_mapS1Utc.getOrUpdateScore(retval, tag_constituent, action.code(), m_nScoreIndex, amount, round);
	      refer_or_allocate_tuple2(word_constituent, &(m_Context.s1uw), &(m_Context.s1uc));
	      cast_weights->m_mapS1Uwc.getOrUpdateScore(retval, word_constituent, action.code(), m_nScoreIndex, amount, round);
	   }

	   // S0 S1
	   if (m_Context.s1!=0) {
	      refer_or_allocate_tuple2(twoword_cfgset, &(m_Context.s0ws1w), &(m_Context.s0cs1c));
	      cast_weights->m_mapS0wcS1wc.getOrUpdateScore(retval, twoword_cfgset, action.code(), m_nScoreIndex, amount, round);
	      refer_or_allocate_tuple2(word_cfgset, &(m_Context.s1w), &(m_Context.s0cs1c));
	      cast_weights->m_mapS0cS1w.getOrUpdateScore(retval, word_cfgset, action.code(), m_nScoreIndex, amount, round);
	      refer_or_allocate_tuple2(word_cfgset, &(m_Context.s0w), &(m_Context.s0cs1c));
	      cast_weights->m_mapS0wS1c.getOrUpdateScore(retval, word_cfgset, action.code(), m_nScoreIndex, amount, round);
	      cast_weights->m_mapS0cS1c.getOrUpdateScore(retval, m_Context.s0cs1c, action.code(), m_nScoreIndex, amount, round);

	   }


	   // S0 N0
	   if (m_Context.n0!=-1) {
	      refer_or_allocate_tuple3(biword_constituent, &(m_Context.s0w), &(m_Context.n0z), &(m_Context.s0c));
	      cast_weights->m_mapS0wN0w.getOrUpdateScore(retval, biword_constituent, action.code(), m_nScoreIndex, amount, round);
	      refer_or_allocate_tuple2(word_constituent, &(m_Context.n0z), &(m_Context.s0c));
	      cast_weights->m_mapS0cN0w.getOrUpdateScore(retval, word_constituent, action.code(), m_nScoreIndex, amount, round);
	      //refer_or_allocate_tuple2(word_constituent, m_Context.s0w, &(m_Context.s0c));
	      //cast_weights->m_mapS0wN0t.getOrUpdateScore(retval, word_constituent, action.code(), m_nScoreIndex, amount, round);
	      //cast_weights->m_mapS0cN0t.getOrUpdateScore(retval, m_Context.s0cn0t, action.code(), m_nScoreIndex, amount, round);
	   }

	   // S1 N0
	   if (m_Context.s1!=0 && m_Context.n0!=-1) {
	      refer_or_allocate_tuple3(biword_constituent, &(m_Context.s1w), &(m_Context.n0z), &(m_Context.s1c));
	      cast_weights->m_mapS1wN0w.getOrUpdateScore(retval, biword_constituent, action.code(), m_nScoreIndex, amount, round);
	      refer_or_allocate_tuple2(word_constituent, &(m_Context.n0z), &(m_Context.s1c));
	      cast_weights->m_mapS1cN0w.getOrUpdateScore(retval, word_constituent, action.code(), m_nScoreIndex, amount, round);
	      //refer_or_allocate_tuple2(word_constituent, m_Context.s1w, &(m_Context.s1c));
	      //cast_weights->m_mapS1wN0t.getOrUpdateScore(retval, word_constituent, action.code(), m_nScoreIndex, amount, round);
	      //cast_weights->m_mapS1cN0t.getOrUpdateScore(retval, m_Context.s1cn0t, action.code(), m_nScoreIndex, amount, round);
	   }

	   // S0 S1 N0
	   //refer_or_allocate_tuple2(word_cfgset, m_Context.s0w, &(m_Context.s0cs1cn0t));
	   //cast_weights->m_mapS0wS1cN0t.getOrUpdateScore(retval, word_cfgset, action.code(), m_nScoreIndex, amount, round);
	   //if (m_Context.s1!=0)  {
	   //   refer_or_allocate_tuple2(word_cfgset, m_Context.s1w, &(m_Context.s0cs1cn0t));
	   //   cast_weights->m_mapS0cS1wN0t.getOrUpdateScore(retval, word_cfgset, action.code(), m_nScoreIndex, amount, round);
	   //}
	   if (m_Context.n0!=-1) {
	      refer_or_allocate_tuple2(word_cfgset, &(m_Context.n0z), &(m_Context.s0cs1c));
	      cast_weights->m_mapS0cS1cN0w.getOrUpdateScore(retval, word_cfgset, action.code(), m_nScoreIndex, amount, round);
	   }
	  // cast_weights->m_mapS0cS1cN0t.getOrUpdateScore(retval, m_Context.s0cs1cn0t, action.code(), m_nScoreIndex, amount, round);

	  // cast_weights->m_mapS0tS1tN0t.getOrUpdateScore(retval, m_Context.s0ts1tn0t, action.code(), m_nScoreIndex, amount, round);

	   // S0 N0 N1
	   if (m_Context.n0!=-1) {
	      //refer_or_allocate_tuple2(word_cfgset, m_Context.s0w, &(m_Context.s0cn0tn1t));
	      //cast_weights->m_mapS0wN0tN1t.getOrUpdateScore(retval, word_cfgset, action.code(), m_nScoreIndex, amount, round);
	      //refer_or_allocate_tuple2(word_cfgset, m_Context.n0w, &(m_Context.s0cn0tn1t));
	      //cast_weights->m_mapS0cN0wN1t.getOrUpdateScore(retval, word_cfgset, action.code(), m_nScoreIndex, amount, round);
	      if (m_Context.n1!=-1) {
	         refer_or_allocate_tuple2(word_constituent, &(m_Context.n1z), &(m_Context.s0c));
	         cast_weights->m_mapS0cN1w.getOrUpdateScore(retval, word_constituent, action.code(), m_nScoreIndex, amount, round);
	         refer_or_allocate_tuple3(biword_constituent, &(m_Context.n0z), &(m_Context.n1z), &(m_Context.s0c));
	         cast_weights->m_mapS0cN0tN1w.getOrUpdateScore(retval, biword_constituent, action.code(), m_nScoreIndex, amount, round);
	      }
	      //cast_weights->m_mapS0cN0tN1t.getOrUpdateScore(retval, m_Context.s0cn0tn1t, action.code(), m_nScoreIndex, amount, round); // m_Context.n0

	      //cast_weights->m_mapS0tN0tN1t.getOrUpdateScore(retval, m_Context.s0tn0tn1t, action.code(), m_nScoreIndex, amount, round); // m_Context.n0
	   }

	   // S0 S1 S2
	   if (m_Context.s1!=0) {
	      refer_or_allocate_tuple2(word_cfgset, &(m_Context.s0w), &(m_Context.s0cs1cs2c));
	      cast_weights->m_mapS0wS1cS2c.getOrUpdateScore(retval, word_cfgset, action.code(), m_nScoreIndex, amount, round);
	      refer_or_allocate_tuple2(word_cfgset, &(m_Context.s1w), &(m_Context.s0cs1cs2c));
	      cast_weights->m_mapS0cS1wS2c.getOrUpdateScore(retval, word_cfgset, action.code(), m_nScoreIndex, amount, round);
	      if (m_Context.s2!=0) {
	         refer_or_allocate_tuple2(word_cfgset, &(m_Context.s2w), &(m_Context.s0cs1cs2c));
	         cast_weights->m_mapS0cS1cS2w.getOrUpdateScore(retval, word_cfgset, action.code(), m_nScoreIndex, amount, round);
	      }
	      cast_weights->m_mapS0cS1cS2c.getOrUpdateScore(retval, m_Context.s0cs1cs2c, action.code(), m_nScoreIndex, amount, round);

	      cast_weights->m_mapS0tS1tS2t.getOrUpdateScore(retval, m_Context.s0ts1ts2t, action.code(), m_nScoreIndex, amount, round);
	   }

		if (m_Context.n0!=-1 && m_Context.s0r!=0) {
			//cast_weights->m_mapS0cS0RcN0t.getOrUpdateScore(retval, m_Context.s0cs0rcn0t, action.code(), m_nScoreIndex, amount, round);
			//cast_weights->m_mapS0cS0RjN0t.getOrUpdateScore(retval, m_Context.s0cs0rjn0t, action.code(), m_nScoreIndex, amount, round);
			refer_or_allocate_tuple2(word_cfgset, &(m_Context.n0z), &(m_Context.s0cs0rc));
			cast_weights->m_mapS0cS0RcN0w.getOrUpdateScore(retval, word_cfgset, action.code(), m_nScoreIndex, amount, round);
		}

	   // S0 S0LRUS1
	   if (m_Context.s1!=0 && m_Context.s0l!=0) {
	      cast_weights->m_mapS0cS0LcS1c.getOrUpdateScore(retval, m_Context.s0cs0lcs1c, action.code(), m_nScoreIndex, amount, round);
	      cast_weights->m_mapS0cS0LjS1j.getOrUpdateScore(retval, m_Context.s0cs0ljs1j, action.code(), m_nScoreIndex, amount, round);
	      refer_or_allocate_tuple2(word_cfgset, &(m_Context.s1w), &(m_Context.s0cs0lc));
	      cast_weights->m_mapS0cS0LcS1w.getOrUpdateScore(retval, word_cfgset, action.code(), m_nScoreIndex, amount, round);
	   }
	   if (m_Context.s1 != 0 && m_Context.s1r != 0) {
	      cast_weights->m_mapS0cS1cS1Rc.getOrUpdateScore(retval, m_Context.s0cs1cs1rc, action.code(), m_nScoreIndex, amount, round);
	      cast_weights->m_mapS0jS1cS1Rj.getOrUpdateScore(retval, m_Context.s0js1cs1rj, action.code(), m_nScoreIndex, amount, round);
	      refer_or_allocate_tuple2(word_cfgset, &(m_Context.s0w), &(m_Context.s1cs1rc));
	      cast_weights->m_mapS0wS1cS1Rc.getOrUpdateScore(retval, word_cfgset, action.code(), m_nScoreIndex, amount, round);
	   }



   }

   if(actionType.code == CActionType::WORD_XYZ)
   {
   	cast_weights->m_mapWSS0w.getOrUpdateScore(retval, m_Context.s0w, action.code(), m_nScoreIndex, amount, round);
   	cast_weights->m_mapWSS1w.getOrUpdateScore(retval, m_Context.s1w, action.code(), m_nScoreIndex, amount, round);
   	refer_or_allocate_tuple2(biword, &(m_Context.s0w), &(m_Context.s1w));
   	cast_weights->m_mapWSS0wS1w.getOrUpdateScore(retval, biword, action.code(), m_nScoreIndex, amount, round);

   	refer_or_allocate_tuple2(wordtag, &(m_Context.s0w), &(m_Context.s0t));
   	cast_weights->m_mapWSS0wTag.getOrUpdateScore(retval, wordtag, action.code(), m_nScoreIndex, amount, round);
   	refer_or_allocate_tuple2(wordtag, &(m_Context.s1w), &(m_Context.s0t));
		cast_weights->m_mapWSS1wTag.getOrUpdateScore(retval, wordtag, action.code(), m_nScoreIndex, amount, round);
		refer_or_allocate_tuple3(wordwordtag, &(m_Context.s0w), &(m_Context.s1w), &(m_Context.s0t));
		cast_weights->m_mapWSS0wS1wTag.getOrUpdateScore(retval, wordwordtag, action.code(), m_nScoreIndex, amount, round);

		refer_or_allocate_tuple2(biword, &(m_Context.s0z), &(m_Context.s1w));
		cast_weights->m_mapWSS0zS1w.getOrUpdateScore(retval, biword, action.code(), m_nScoreIndex, amount, round);
		refer_or_allocate_tuple2(biword, &(m_Context.s0w), &(m_Context.s1z));
		cast_weights->m_mapWSS0wS1z.getOrUpdateScore(retval, biword, action.code(), m_nScoreIndex, amount, round);
		refer_or_allocate_tuple2(biword, &(m_Context.s0z), &(m_Context.s1z));
		cast_weights->m_mapWSS0zS1z.getOrUpdateScore(retval, biword, action.code(), m_nScoreIndex, amount, round);
		refer_or_allocate_tuple3(wordwordtag, &(m_Context.s0z), &(m_Context.s1z), &(m_Context.s0t));
		cast_weights->m_mapWSS0zS1zTag.getOrUpdateScore(retval, wordwordtag, action.code(), m_nScoreIndex, amount, round);

   }

   if(actionType.code == CActionType::SHIFT_S || actionType.code == CActionType::POP_ROOT)
   {
   	CTag tag_0 = actionType.code == CActionType::SHIFT_S ? CTag(action.getPOS()) : g_beginTag;
   	refer_or_allocate_tuple2(bitag, &(tag_0), &(m_Context.tag_1));
      cast_weights->m_mapLastTagByTag.getOrUpdateScore(retval, bitag,  action.code(), m_nScoreIndex , amount , round ) ;
      if (m_Context.start_0 >= 0)
   	{
      	int length_1 = m_Context.length_1;
   		refer_or_allocate_tuple3(bitag_int, &(tag_0), &(m_Context.tag_1), &(length_1));
   		cast_weights->m_mapTag0Tag1Size1.getOrUpdateScore(retval, bitag_int,  action.code(), m_nScoreIndex , amount , round ) ;
   	}
   	if (m_Context.start_1 >= 0)
   	{
   		int length_1 = m_Context.length_1;
   		refer_or_allocate_tuple4(tritagint, &(tag_0), &(m_Context.tag_1), &(m_Context.tag_2), &(length_1));
   		cast_weights->m_mapTag0Tag1Tag2Size1.getOrUpdateScore(retval, tritagint,  action.code(), m_nScoreIndex , amount , round ) ;
   	}

		if ( m_Context.length_1 <= 2 )
      {
			refer_or_allocate_tuple2(wordtag, &(m_Context.word_1), &(tag_0));
      	cast_weights->m_mapTagByLastWord.getOrUpdateScore(retval, wordtag ,  action.code(), m_nScoreIndex , amount , round ) ;
      }

		if ( m_Context.start_1 >= 0 ) {
			refer_or_allocate_tuple3(tritag, &(tag_0), &(m_Context.tag_1), &(m_Context.tag_2));
		   cast_weights->m_mapLastTwoTagsByTag.getOrUpdateScore(retval, tritag,  action.code(), m_nScoreIndex , amount , round ) ;
		}
		if (m_Context.start_0 >= 0) {
			refer_or_allocate_tuple2(wordtag, &(m_Context.first_char_0), &(tag_0));
			cast_weights->m_mapTagByFirstChar.getOrUpdateScore(retval, wordtag ,  action.code(), m_nScoreIndex , amount , round ) ;
			unsigned long long first_char_cat_0 = m_weights->m_mapCharTagDictionary.lookup(m_Context.first_char_0) | (static_cast<unsigned long long>(1)<<tag_0.code()) ;
			refer_or_allocate_tuple2(tagint, &(tag_0) , &(first_char_cat_0));
			cast_weights->m_mapTagByFirstCharCat.getOrUpdateScore(retval, tagint ,  action.code(), m_nScoreIndex , amount , round ) ;

			refer_or_allocate_tuple3(wordtagtag, &(m_Context.first_char_0), &(tag_0), &(m_Context.tag_1));
			cast_weights->m_mapFirstCharBy2Tags.getOrUpdateScore(retval, wordtagtag ,  action.code(), m_nScoreIndex , amount , round ) ;
			if (m_Context.start_1 >= 0)
			{
				refer_or_allocate_tuple4(wordtagtagtag, &(m_Context.first_char_0), &(tag_0), &(m_Context.tag_1), &(m_Context.tag_2));
				cast_weights->m_mapFirstCharBy3Tags.getOrUpdateScore(retval, wordtagtagtag ,  action.code(), m_nScoreIndex , amount , round ) ;
			}

			refer_or_allocate_tuple2(wordtag, &(m_Context.first_char_0), &(tag_0));
			cast_weights->m_mapTagByChar.getOrUpdateScore(retval, wordtag,  action.code(), m_nScoreIndex , amount , round ) ;

			if (m_Context.start_1 >= 0) {
				wt1.load(m_Context.last_char_1, m_Context.tag_1);
				wt2.load(m_Context.first_char_0, tag_0);
				if (std::abs(amount) < 1e-20) { wt12.refer(&wt1, &wt2); } else { wt12.allocate(wt1, wt2); }
				cast_weights->m_mapTaggedSeparateChars.getOrUpdateScore(retval, wt12,  action.code(), m_nScoreIndex , amount , round ) ;
			}

			if (m_Context.start_1 >= 0)
			{
				refer_or_allocate_tuple3(wordtagtag, &(m_Context.word_1), &(tag_0), &(m_Context.tag_2));
				cast_weights->m_mapTagWordTag.getOrUpdateScore(retval, wordtagtag,  action.code(), m_nScoreIndex, amount, round);
			}
			if (m_Context.start_2 >= 0)
			{
				refer_or_allocate_tuple3(wordtagtag, &(m_Context.word_2), &(tag_0), &(m_Context.tag_1));
				cast_weights->m_mapWordTagTag.getOrUpdateScore(retval, wordtagtag,  action.code(), m_nScoreIndex, amount, round);
			}
		}
   }

   if(actionType.code == CActionType::WORD_T)
   {
   	unsigned long long last_char_cat_1 = m_weights->m_mapCharTagDictionary.lookup(m_Context.last_char_1) | (static_cast<unsigned long long>(1)<<m_Context.tag_1.code()) ;
      cast_weights->m_mapSeenWords.getOrUpdateScore(retval, m_Context.word_1 , action.code(), m_nScoreIndex , amount , round ) ;
      cast_weights->m_mapLastWordByWord.getOrUpdateScore(retval, m_Context.word_2_word_1 , action.code(), m_nScoreIndex , amount , round ) ;

      if ( m_Context.length_1 == 1 ) {
    	  cast_weights->m_mapOneCharWord.getOrUpdateScore(retval, m_Context.word_1 , action.code(), m_nScoreIndex , amount , round ) ;
      }
      else {
    	  cast_weights->m_mapFirstAndLastChars.getOrUpdateScore(retval, m_Context.first_char_1_last_char_1 , action.code(), m_nScoreIndex , amount , round ) ;

    	  cast_weights->m_mapLengthByFirstChar.getOrUpdateScore(retval, std::make_pair(m_Context.first_char_1, m_Context.length_1) , action.code(), m_nScoreIndex , amount , round ) ;
    	  cast_weights->m_mapLengthByLastChar.getOrUpdateScore(retval, std::make_pair(m_Context.last_char_1, m_Context.length_1) , action.code(), m_nScoreIndex , amount , round ) ;

      }

      if (m_Context.length_2 > 0) {
    	  cast_weights->m_mapCurrentWordLastChar.getOrUpdateScore(retval, m_Context.word_1_last_char_2 , action.code(), m_nScoreIndex , amount , round ) ;
    	  cast_weights->m_mapLastWordByLastChar.getOrUpdateScore(retval, m_Context.last_char_1_last_char_2 , action.code(), m_nScoreIndex , amount , round ) ;

    	  cast_weights->m_mapLengthByLastWord.getOrUpdateScore(retval, std::make_pair(m_Context.word_2, m_Context.length_1) , action.code(), m_nScoreIndex , amount , round ) ;
    	  cast_weights->m_mapLastLengthByWord.getOrUpdateScore(retval, std::make_pair(m_Context.word_1, m_Context.length_2), action.code(), m_nScoreIndex , amount , round ) ;
      }

      cast_weights->m_mapCurrentTag.getOrUpdateScore(retval, std::make_pair(m_Context.word_1, m_Context.tag_1), action.code(), m_nScoreIndex , amount , round ) ;

      if ( m_Context.length_1 <= 2 ) cast_weights->m_mapLastTagByWord.getOrUpdateScore(retval, std::make_pair(m_Context.word_1, m_Context.tag_2), action.code(), m_nScoreIndex , amount , round ) ;

      if (m_Context.length_2 > 0) {
         if ( m_Context.length_1 <= 2 ) cast_weights->m_mapTagByWordAndPrevChar.getOrUpdateScore(retval, std::make_pair(m_Context.word_1_last_char_2, m_Context.tag_1) , action.code(), m_nScoreIndex , amount , round ) ;
         if ( m_Context.length_1 == 1 && m_Context.start_0 > 0 ) cast_weights->m_mapTagOfOneCharWord.getOrUpdateScore(retval, std::make_pair(m_Context.three_char, m_Context.tag_1) , action.code(), m_nScoreIndex , amount , round ) ;
      }

      cast_weights->m_mapTagByLastChar.getOrUpdateScore(retval, std::make_pair(m_Context.last_char_1, m_Context.tag_1) , action.code(), m_nScoreIndex , amount , round ) ;
      cast_weights->m_mapTagByLastCharCat.getOrUpdateScore(retval, std::make_pair(last_char_cat_1, m_Context.tag_1) , action.code(), m_nScoreIndex , amount , round ) ;

      for (j=0; j<m_Context.length_1-1; ++j) {
         cast_weights->m_mapTaggedCharByLastChar.getOrUpdateScore(retval, m_Context.wt12_collection[j], action.code(), m_nScoreIndex, amount, round) ;
      }

      if (m_Context.start_1 >= 0)
      {
      	cast_weights->m_mapTag1Tag2Size1.getOrUpdateScore(retval, std::make_pair( m_Context.tag_1_tag_2, m_Context.length_1 ),  action.code(), m_nScoreIndex , amount , round ) ;
      }


		if (m_Context.start_0 >= 0) {
			if ( m_Context.start_1 >= 0 ) {
				cast_weights->m_mapSeparateChars.getOrUpdateScore(retval, m_Context.two_char ,  action.code(), m_nScoreIndex , amount , round ) ;

				cast_weights->m_mapLastWordFirstChar.getOrUpdateScore(retval, m_Context.word_1_first_char_0 ,  action.code(), m_nScoreIndex , amount , round ) ;

				cast_weights->m_mapFirstCharLastWordByWord.getOrUpdateScore(retval, m_Context.first_char_0_first_char_1 ,  action.code(), m_nScoreIndex , amount , round ) ;

				if ( m_Context.length_1 <= 2 ) cast_weights->m_mapTagByWordAndNextChar.getOrUpdateScore(retval, std::make_pair(m_Context.word_1_first_char_0, m_Context.tag_1) ,  action.code(), m_nScoreIndex , amount , round ) ;

			}
		}
   }

   if(actionType.code == CActionType::SHIFT_A)
   {

   	refer_or_allocate_tuple2(wordtag, &(m_Context.first_char_0), &(m_Context.tag_1));
   	cast_weights->m_mapTagByChar.getOrUpdateScore(retval, wordtag, action.code(), m_nScoreIndex , amount , round ) ;


		cast_weights->m_mapTaggedCharByFirstChar.getOrUpdateScore(retval, m_Context.wt12_app, action.code(), m_nScoreIndex, amount, round ) ;

		cast_weights->m_mapConsecutiveChars.getOrUpdateScore(retval, m_Context.two_char, action.code(), m_nScoreIndex, amount, round ) ;

		cast_weights->m_mapTaggedConsecutiveChars.getOrUpdateScore(retval, std::make_pair(m_Context.two_char, m_Context.tag_1), action.code(), m_nScoreIndex, amount, round ) ;
   }

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

void CConParser::updateScoresForState( CWeight *cast_weights , const CStateItem *item , const CStringVector &sentence , const SCORE_UPDATE update) {

   static SCORE_TYPE amount;
   amount = (update==eAdd ? 1 : -1);
#ifdef SCALE
   amount /= item->size;
#endif
   const static CStateItem* states[MAX_SENTENCE_SIZE*(2+UNARY_MOVES)+2];

   static int count, exc_count;
   const CStateItem *current;

   static CPackedScoreType<SCORE_TYPE, CAction::MAX> scores;

   count = 0;
   exc_count = 0;
   current = item;
   while (current) {
#ifdef SCALE
      if (current->IsIdle()) ++exc_count; // exclude idle actions
#endif
      states[count] = current;
      ++count ; //updating
      current = current->statePtr;
   }
   --count; // state [0..count] are the reverse lifecycle of item.
#ifdef SCALE
   assert(item->size + exc_count == count);
#endif

   // for each
   while (count>exc_count) {
      m_Context.load(states[count], m_lCache, sentence, true);
      // update action
      const CAction &action = states[count-1]->action;
      getOrUpdateStackScore(cast_weights, scores, states[count], action, amount, m_nTrainingRound );
      --count;
   }
}

/*---------------------------------------------------------------
 *
 * updateScoresForStates - update scores for states
 *
 *--------------------------------------------------------------*/

void CConParser::updateScoresForStates( const CStateItem *outout , const CStringVector &sentence , const CStateItem *correct ) {

   TRACE_WORD( "updating parameters ... ") ;

   static double F;
#ifdef TRAIN_LOSS
//   F = correct->FLoss();
   F = correct->HammingLoss();
   ASSERT(F==0,"correct"<<F);

//   F = outout->FLoss(); 
   F = outout->HammingLoss(); 
   updateScoresForState( m_delta, correct, sentence, eAdd );
   updateScoresForState( m_delta, outout, sentence, eSubtract );
#else
   F = 1.0;
   updateScoresForState( m_delta, correct, sentence, eAdd );
   updateScoresForState( m_delta, outout, sentence, eSubtract );
#endif

#ifdef TRAIN_MARGIN
//   double tou = (std::sqrt(F)+outout->score-correct->score)/(m_delta->squareNorm());
   double tou = (F+outout->score-correct->score)/(m_delta->squareNorm());
   m_delta->scaleCurrent(tou, m_nTrainingRound);
#endif
   static_cast<CWeight*>(m_weights)->addCurrent(m_delta, m_nTrainingRound);
   m_delta->clear();

   m_nTotalErrors++;
}




#ifdef TRAIN_LOSS
/*---------------------------------------------------------------
 *
 * computeLossF - computer the F-score loss functions
 *
 *--------------------------------------------------------------*/

//double CConParser::computeLossF(const CStateItem* item) {
//   double retval = 0;
//   const CStateItem *next;
//   next = item;
//   while (next) {
//      retval += next->lost_lb;
//      next = next->statePtr;
//   }
//   return retval;
//}

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

/*---------------------------------------------------------------
 *
 * updateScoresByLoss - update scores 
 *
 *--------------------------------------------------------------*/

void CConParser::updateScoresByLoss( const CStateItem *output , const CStateItem *correct ) {

   TRACE_WORD("updating parameters ... ") ;

   // TODO
   const static CStateItem* oitems[MAX_SENTENCE_SIZE*(2+UNARY_MOVES)+2];
   const static CStateItem* citems[MAX_SENTENCE_SIZE*(2+UNARY_MOVES)+2];

   static int oi, ci;
   static const CStateItem *item;
   static CPackedScoreType<SCORE_TYPE, CAction::MAX> scores;
   static double L, tou;
   SCORE_TYPE oscore, cscore;

   // list output in reverse order each step
   oi = -1; // the index
   item = output; // ptr
   while (item) {
      ++oi;
      oitems[oi] = item;
      item = item->statePtr;
   }
   // list correct in reverse order each step
   ci = -1; // the index
   item = correct; // ptr
   while (item) {
      ++ci;
      citems[ci] = item;
      item = item->statePtr;
   }
   ASSERT(oitems[oi]==citems[ci], "Initial items unqueal");

   // do not consider those steps in which output did perfect
   while (oitems[oi] == citems[ci]) {
      --oi;
      --ci;
   } // here -- mean ++ in step
   ASSERT(oi >= 0 && ci >= 0, "output correct same"); // change to assert
   ++ci;
   ++oi;

   // now the main loop updating pair of action
   // note that output must contain a smaller 
   // number of actions, due to early-update
   while (oi > 0 && ci > 0) {
      m_delta->clear();
      // load output
      m_Context.load(oitems[oi], m_lCache, m_lWordLen, true);
      const CAction &oaction = oitems[oi-1]->action; //-1 means +1 step
      getOrUpdateStackScore(m_delta, scores, oitems[oi], oaction, -1, m_nTrainingRound);
      // load correct
      m_Context.load(citems[ci], m_lCache, m_lWordLen, true);
      const CAction &caction = citems[ci-1]->action;
      getOrUpdateStackScore(m_delta, scores, citems[ci], caction, 1, m_nTrainingRound);
      // update scores
      L = oitems[oi-1]->stepHammingLoss();
      if (L==0) L = 1.0; //ASSERT(L, "no loss");
      ASSERT(L>0, "neg");
      if (m_delta->squareNorm()) {
         oscore = oitems[oi-1]->score-oitems[oi]->score;
         cscore = citems[ci-1]->score - citems[ci]->score;
//         tou = (std::sqrt(L)+oscore-cscore)/m_delta->squareNorm();
         tou = (L+oscore-cscore)/m_delta->squareNorm();
         m_delta->scaleCurrent(tou, m_nTrainingRound);
         static_cast<CWeight*>(m_weights)->addCurrent(m_delta, m_nTrainingRound);
      }
      // next round
      --oi;
      --ci;
   }
   ++m_nTotalErrors;
   m_delta->clear(); // clear delta and free memory
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

void CConParser::work( const bool bTrain , const CStringVector &sentence , CSentenceParsed *retval , const CSentenceParsed &correct , int nBest , SCORE_TYPE *scores ) {

   static CStateItem lattice[(MAX_SENTENCE_SIZE*(2+UNARY_MOVES)+2)*(AGENDA_SIZE+1)];
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
   static bool correct_action_scored;
   static std::vector<CAction> actions; // actions to apply for a candidate
   static CAgendaSimple<CScoredStateAction> beam(AGENDA_SIZE);
   static CScoredStateAction scored_action; // used rank actions
   ASSERT(nBest=1, "currently only do 1 best parse");
   static unsigned index;
   static bool bSkipLast;
#ifdef SCALE
   bool bAllTerminated;
#endif

   static CPackedScoreType<SCORE_TYPE, CAction::MAX> packedscores;

   if(length >= MAX_SENTENCE_SIZE) return;
   assert(length<MAX_SENTENCE_SIZE);

   TRACE("Initialising the decoding process ... ") ;
   // initialise word cache
   m_lCache.clear();
   // initialise agenda
   lattice_index[0] = lattice;
   lattice_index[0]->clear();
#ifdef TRAIN_LOSS
   lattice_index[0]->bTrain = m_bTrain;
   getLabeledBrackets(correct, lattice_index[0]->gold_lb);
   TRACE(lattice_index[0]->gold_lb << std::endl);
#endif

#ifndef EARLY_UPDATE
   if (bTrain) bSkipLast = false;
#endif
   lattice_index[1] = lattice+1;
   if (bTrain) { 
      correctState = lattice_index[0];
   }
   index=0;

   TRACE("Decoding start ... ") ;
   while (true) { // for each step

      ++index;
      lattice_index[index+1] = lattice_index[index];

      /*
      if(index == 39)
      {
      	TRACE("debug started.....");
      }
		*/
      beam.clear();

      pBestGen = 0;

      if (bTrain) {
         bCorrect = false;
         correctState->StandardMove(correct, correct_action);
         //TRACE("gold action:" + correct_action.str() + "\t");
         correct_action_scored = false;
      }

      for (pGenerator=lattice_index[index-1]; pGenerator!=lattice_index[index]; ++pGenerator) { // for each generator

#ifndef EARLY_UPDATE
         if (bTrain && bSkipLast && pGenerator == lattice_index[index]-1) {
            getOrUpdateStackScore(static_cast<CWeight*>(m_weights), packedscores, pGenerator);
            scored_correct_action.load(correct_action, pGenerator, packedscores[correct_action.code()]);
            correct_action_scored = true;
            break;
         }
#endif

         // load context
         m_Context.load(pGenerator, m_lCache, sentence, false);
   
         //pGenerator->trace();
         // get actions
         m_rule->getActions(*pGenerator, &sentence, actions);

         /*
         if(actions.size() > 0)
         {
				for(int idx = 0; idx < actions.size(); idx++)
				{
					TRACE_WORD(actions[idx].str() + "\t");
				}
				TRACE("");
         }
			*/

         if (actions.size() > 0)
            getOrUpdateStackScore(static_cast<CWeight*>(m_weights), packedscores, pGenerator);
         
         for (tmp_j=0; tmp_j<actions.size(); ++tmp_j) {
            scored_action.load(actions[tmp_j], pGenerator, packedscores[actions[tmp_j].code()]);
            beam.insertItem(&scored_action);
            if (bTrain && pGenerator == correctState && actions[tmp_j] == correct_action) {
               scored_correct_action = scored_action;
               correct_action_scored = true;
            }
         }
   
      } // done iterating generator item

#ifdef SCALE
      bAllTerminated = true;
#endif
      if(beam.size() == 0)
		{
			TRACE("error");
			for(int idx = 0; idx < sentence.size();idx++)
			{
				std::cout << sentence[idx];
			}
			std::cout << std::endl;
			return;
		}
      // insertItems
      for (tmp_j=0; tmp_j<beam.size(); ++tmp_j) { // insert from
         pGenerator = beam.item(tmp_j)->item;
         pGenerator->Move(lattice_index[index+1], beam.item(tmp_j)->action);
         lattice_index[index+1]->score = beam.item(tmp_j)->score;
#ifdef SCALE
         if ( ! lattice_index[index+1]->IsTerminated() )
            bAllTerminated = false;
#endif

         if ( pBestGen == 0 || lattice_index[index+1]->score > pBestGen->score ) {
            pBestGen = lattice_index[index+1];
         }

         // update bestgen
         if (bTrain) {
            if ( pGenerator == correctState && beam.item(tmp_j)->action == correct_action ) {
               correctState = lattice_index[index+1];
               assert (correctState->unaryreduces()<=UNARY_MOVES) ; 
               bCorrect = true;
            }
         }
         ++lattice_index[index+1];
      }

#ifdef SCALE
      if (bAllTerminated)
         break; // while
#else
      if (pBestGen->IsTerminated())
         break; // while
#endif

      // update items if correct item jump out of the agenda
      if (bTrain) { 
         if (!bCorrect ) {
            // note that if bCorrect == true then the correct state has 
            // already been updated, and the new value is one of the new states
            // among the newly produced from lattice[index+1].
            correctState->Move(lattice_index[index+1], correct_action); 
            correctState = lattice_index[index+1];
            lattice_index[index+1]->score = scored_correct_action.score;
            ++lattice_index[index+1];
            assert(correct_action_scored); // scored_correct_act valid
            /*
            if(!correct_action_scored)
            {
            	TRACE("error");
					for(int idx = 0; idx < sentence.size();idx++)
					{
						std::cout << sentence[idx];
					}
					std::cout << std::endl;
					return;
            }*/
#ifdef EARLY_UPDATE
//         if (!bCorrect ) {
            //TRACE("Error at the "<<correctState->current_word<<"th word; total is "<<m_lCache.size())
            // update
//#ifdef TRAIN_MULTI
//            updateScoresForMultipleStates(lattice_index[index], lattice_index[index+1], candidate_outout, correctState) ;
//#else
            // trace
            correctState->trace(&sentence);
            pBestGen->trace(&sentence);
//            updateScoresByLoss(pBestGen, correctState) ; 
            updateScoresForStates(pBestGen, sentence, correctState) ;
#//endif // TRAIN_MULTI
            return ;
//         } // bCorrect
#else // EARLY UDPATE
            bSkipLast = true;
#endif
         } // bCorrect
      }  // bTrain
   } // while

   if (bTrain) {
      // make sure that the correct item is stack top finally
      if ( pBestGen != correctState ) {
         if (!bCorrect) {
            correctState->Move(lattice_index[index+1], correct_action); 
            correctState = lattice_index[index+1];
            lattice_index[index+1]->score = scored_correct_action.score;
            assert(correct_action_scored); // scored_correct_act valid
            /*
            if(!correct_action_scored)
				{
					TRACE("error");
					for(int idx = 0; idx < sentence.size();idx++)
					{
						std::cout << sentence[idx];
					}
					std::cout << std::endl;
					return;
				}
				*/
         }
         TRACE("The best item is not the correct one")
//#ifdef TRAIN_MULTI
//         updateScoresForMultipleStates(lattice_index[index], lattice_index[index+1], pBestGen, correctState) ;
//#else // TRAIN_MULTI
         correctState->trace(&sentence);
         pBestGen->trace(&sentence);
//         updateScoresByLoss(pBestGen, correctState) ; 
         updateScoresForStates(pBestGen, sentence, correctState) ;
//#endif // TRAIN_MULTI
         return ;
      }
      else {
         TRACE("correct");
         correctState->trace(&sentence);
         pBestGen->trace(&sentence);
      }
   } 

   if (!retval) 
      return;

   TRACE("Outputing sentence");
   pBestGen->GenerateTree( sentence, retval[0] );
   if (scores) scores[0] = pBestGen->score;

   TRACE("Done, the highest score is: " << pBestGen->score ) ;
   TRACE("The total time spent: " << double(clock() - total_start_time)/CLOCKS_PER_SEC) ;
}

/*---------------------------------------------------------------
 *
 * parse - do constituent parsing to a sentence
 *
 * Returns: makes a new instance of CSentenceParsed 
 *
 *--------------------------------------------------------------*/

void CConParser::parse( const CStringVector &sentence_input , CSentenceParsed *retval , int nBest , SCORE_TYPE *scores ) {

   static CSentenceParsed empty ;

   static CStringVector sentence;
   m_rule->segment(&sentence_input, &sentence);

   work(false, sentence, retval, empty, nBest, scores ) ;

}



/*---------------------------------------------------------------
 *
 * train - train the models with an example
 *
 *---------------------------------------------------------------*/

void CConParser::train( const CSentenceParsed &correct , int round ) {

   static CStringVector sentence_input ;
   static CStringVector sentence;
   static CTwoStringVector wordtags;
//   static CSentenceParsed outout ; 

   UnparseSentence( &correct, &sentence_input ) ;
   UnparseSentence( &correct, &wordtags ) ;

   m_rule->record(&wordtags, &sentence );

   /*
   for(int idx = 0; idx < sentence.size(); idx++)
   {
   	TRACE_WORD(sentence[idx] << " ");
   }
   TRACE("");

   for(int idx = 0; idx < wordtags.size(); idx++)
	{
		TRACE_WORD(wordtags.at(idx).first << "_" << wordtags.at(idx).second << " ");
	}
   TRACE("");
	*/
   static unsigned total_size, local_size;
   total_size=0;

   m_rule->record(&wordtags, &sentence );
   // Updates that are common for all example
   for ( unsigned i=0; i<wordtags.size(); ++i ) {

      const CWord &word = wordtags.at(i).first ;
      unsigned long tag = CTag( wordtags.at(i).second ).code() ;

      static CStringVector chars;
      static unsigned j;
      chars.clear();
      getCharactersFromUTF8String(wordtags.at(i).first, &chars);
      local_size = chars.size();

      m_weights->m_mapWordFrequency[word]++;
      if (m_weights->m_mapWordFrequency[word]>m_weights->m_nMaxWordFrequency)
         m_weights->m_nMaxWordFrequency = m_weights->m_mapWordFrequency[word];

      m_weights->m_mapTagDictionary.add(word, tag);
      for ( j=0 ; j<local_size; ++j ) {
         m_weights->m_mapCharTagDictionary.add(chars[j], tag) ;
      }

      if ( PENN_TAG_CLOSED[tag] || tag==PENN_TAG_CD ) {
           m_weights->m_mapCanStart.add(chars[0], tag);
      }

//      if ( !m_weights->m_Knowledge ||
//          (!m_weights->m_Knowledge->isFWorCD(chars[0])&&
//           !m_weights->m_Knowledge->isFWorCD(chars[chars.size()-1])))
      bool bNoSep=false;
      for ( j=total_size+1; j<total_size+local_size; ++j)
         if (!m_rule->canSeparate(j)) bNoSep = true;
      if (!bNoSep)
         m_weights->setMaxLengthByTag( tag , local_size ) ;

      total_size += chars.size();
   }

   // The following code does update for each processing stage
   m_nTrainingRound = round ;
   if(sentence.size() != sentence_input.size())
   {
   	TRACE("error training data");
   	return;
   }
//   work( true , sentence , &outout , correct , 1 , 0 ) ; 
   work( true , sentence , 0 , correct , 1 , 0 ) ; 

};

/*---------------------------------------------------------------
 *
 * train - train the models with an example
 *
 *---------------------------------------------------------------*/


#ifdef NO_NEG_FEATURE
/*---------------------------------------------------------------
 *
 * getPositiveFeatures - get positive features
 *
 *---------------------------------------------------------------*/

void CConParser::getPositiveFeatures( const CSentenceParsed &correct ) {
   static CStringVector sentence;
   static CStateItem states[MAX_SENTENCE_SIZE*(1+UNARY_MOVES)+2];
   static CPackedScoreType<SCORE_TYPE, CAction::MAX> scores;
   static int current;
   static CAction action;

   states[0].clear();
   current = 0;
   UnparseSentence( &correct, &sentence ) ;
   m_lCache.clear();
   while ( !states[current].IsTerminated() ) {
      states[current].StandardMove(correct, action);
//std::cout << action << std::endl;
      m_Context.load(states+current, m_lCache, sentence, true);
      getOrUpdateStackScore(static_cast<CWeight*>(m_weights), scores, states+current, action, 1, -1);
      states[current].Move(states+current+1, action);
      ++current;
   }
}
#endif
