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

#define cast_weights static_cast<CWeight*>(m_weights)

#define refer_or_allocate_tuple3(x, o1, o2, o3) { if (amount == 0) x.refer(o1, o2, o3); else x.allocate(o1, o2, o3); }

/*===============================================================
 *
 * CConParser - the conparser for TARGET_LANGUAGE 
 *
 *==============================================================*/

/*---------------------------------------------------------------
 *
 * getOrUpdateLinkScore - get or update the score from a link.
 *
 *---------------------------------------------------------------*/

SCORE_TYPE CConParser::getOrUpdateLinkScore( const unsigned long &head, const unsigned long &mod, SCORE_TYPE amount, int round ) {

   static SCORE_TYPE nReturn;
   nReturn = 0;
return 0;
   unsigned long direction = encodeLinkDirection(head, mod);
   unsigned long dist = encodeLinkSize(head, mod);
   static CCFGSet cf;

   const CTag &ht = m_lCache[head].tag;
   const CWord &hw = m_lCache[head];
   const CTag &mt = m_lCache[mod].tag;
   const CWord &mw = m_lCache[mod];

/*
   cf.clear(); cf.add(ht); cf.add(mt); cf.add(0); cf.add(0);
   nReturn += cast_weights->m_mapHtMt.getOrUpdateScore(cf, m_nScoreIndex, amount, round);
   cf.clear(); cf.add(ht); cf.add(mt); cf.add(direction); cf.add(0);
   nReturn += cast_weights->m_mapHtMt.getOrUpdateScore(cf, m_nScoreIndex, amount, round);
   cf.clear(); cf.add(ht); cf.add(mt); cf.add(direction); cf.add(dist);
   nReturn += cast_weights->m_mapHtMt.getOrUpdateScore(cf, m_nScoreIndex, amount, round);

   cf.clear(); cf+=mt; cf+=0; cf+=0;
   nReturn += cast_weights->m_mapHwMt.getOrUpdateScore(make_pair(hw, cf), m_nScoreIndex, amount, round);
   cf.clear(); cf+=mt; cf+=direction; cf+=0;
   nReturn += cast_weights->m_mapHwMt.getOrUpdateScore(make_pair(hw, cf), m_nScoreIndex, amount, round);
   cf.clear(); cf+=mt; cf+=direction; cf+=dist;
   nReturn += cast_weights->m_mapHwMt.getOrUpdateScore(make_pair(hw, cf), m_nScoreIndex, amount, round);

   cf.clear(); cf+=ht; cf+=0; cf+=0;
   nReturn += cast_weights->m_mapHtMw.getOrUpdateScore(make_pair(mw, cf), m_nScoreIndex, amount, round);
   cf.clear(); cf+=ht; cf+=direction; cf+=0;
   nReturn += cast_weights->m_mapHtMw.getOrUpdateScore(make_pair(mw, cf), m_nScoreIndex, amount, round);
   cf.clear(); cf+=ht; cf+=direction;cf+=dist;
   nReturn += cast_weights->m_mapHtMw.getOrUpdateScore(make_pair(mw, cf), m_nScoreIndex, amount, round);
*/
   return nReturn;

}

/*---------------------------------------------------------------
 *
 * getOrUpdateSiblingLinkScore - get or update the score from two links.
 *
 *---------------------------------------------------------------*/

SCORE_TYPE CConParser::getOrUpdateSiblingLinkScore( const unsigned long &head, const unsigned long &mod, const unsigned long &sibling, SCORE_TYPE amount, int round ) {
   static SCORE_TYPE nReturn;
   nReturn = 0;
return 0;
   unsigned long direction = encodeLinkDirection(mod, sibling);
   unsigned long dist = encodeLinkDirectionAndSize(mod, sibling);

   const CTag &ht = m_lCache[head].tag;
   const CWord &hw = m_lCache[head];
   const CTag &mt = m_lCache[mod].tag;
   const CWord &mw = m_lCache[mod];
   const CTag &st = m_lCache[sibling].tag;
   const CWord &sw = m_lCache[sibling];

   unsigned long k = encodeTags(ht, mt, st);
   nReturn += cast_weights->m_mapSiblingTags.getOrUpdateScore(k, m_nScoreIndex, amount, round);
   nReturn += cast_weights->m_mapSiblingTags.getOrUpdateScore(k | (direction<<(CTag::SIZE*3)), m_nScoreIndex, amount, round);
   nReturn += cast_weights->m_mapSiblingTags.getOrUpdateScore(k | (dist<<(CTag::SIZE*3)), m_nScoreIndex, amount, round);

   k = encodeTags(ht, mt);
   nReturn += cast_weights->m_mapSiblingWordTag.getOrUpdateScore(make_pair(sw, k), m_nScoreIndex, amount, round);
   nReturn += cast_weights->m_mapSiblingWordTag.getOrUpdateScore(make_pair(sw, k|(direction<<(CTag::SIZE*2))), m_nScoreIndex, amount, round);
   nReturn += cast_weights->m_mapSiblingWordTag.getOrUpdateScore(make_pair(sw, k|(dist<<(CTag::SIZE*2))), m_nScoreIndex, amount, round);

   k = encodeTags(ht, st);
   nReturn += cast_weights->m_mapSiblingTagWord.getOrUpdateScore(make_pair(mw, k), m_nScoreIndex, amount, round);
   nReturn += cast_weights->m_mapSiblingTagWord.getOrUpdateScore(make_pair(mw, k|(direction<<(CTag::SIZE*2))), m_nScoreIndex, amount, round);
   nReturn += cast_weights->m_mapSiblingTagWord.getOrUpdateScore(make_pair(mw, k|(dist<<(CTag::SIZE*2))), m_nScoreIndex, amount, round);

   return nReturn;
}

/*---------------------------------------------------------------
 * 
 * getOrUpdateArityScore - get or update score for modifier count
 *
 *--------------------------------------------------------------*/

SCORE_TYPE CConParser::getOrUpdateArityScore( const unsigned long &head, const unsigned long &arityleft, const unsigned long &arityright, SCORE_TYPE amount, int round ) {
   static SCORE_TYPE nReturn;
   nReturn = 0;return nReturn;

   const CWord &w = m_lCache[head];
   const CTag &t = m_lCache[head].tag;

   nReturn += cast_weights->m_mapHwArityL.getOrUpdateScore(make_pair(w, arityleft), m_nScoreIndex, amount, round);
   nReturn += cast_weights->m_mapHtArityL.getOrUpdateScore(make_pair(t.code(), arityleft), m_nScoreIndex, amount, round);
   nReturn += cast_weights->m_mapHwArityR.getOrUpdateScore(make_pair(w, arityright), m_nScoreIndex, amount, round);
   nReturn += cast_weights->m_mapHtArityR.getOrUpdateScore(make_pair(t.code(), arityright), m_nScoreIndex, amount, round);

   return nReturn;
}

/*---------------------------------------------------------------
 * 
 * getOrUpdateHeadScore - get or update score for head label
 *
 *--------------------------------------------------------------*/

SCORE_TYPE CConParser::getOrUpdateHeadScore( const unsigned long &parent, const unsigned long &child, const unsigned long &head, SCORE_TYPE amount, int round ) {
   static SCORE_TYPE nReturn;
   nReturn = 0;
return 0;
   const CWord &w = m_lCache[head];
   const CTag &t = m_lCache[head].tag;
   static CCFGSet cs; cs.clear();
   cs += parent; cs += child;

   nReturn += cast_weights->m_mapHw.getOrUpdateScore(make_pair(w, parent), m_nScoreIndex, amount, round);
   nReturn += cast_weights->m_mapHt.getOrUpdateScore(make_pair(t.code(), parent), m_nScoreIndex, amount, round);
   nReturn += cast_weights->m_mapHc.getOrUpdateScore(cs, m_nScoreIndex, amount, round);
   nReturn += cast_weights->m_mapHwc.getOrUpdateScore(make_pair(w, cs), m_nScoreIndex, amount, round);
   
   return nReturn;
}

/*---------------------------------------------------------------
 * 
 * getOrUpdateHeadModifierScore - get or update score for head 
 *                                and modifier labels
 *
 *--------------------------------------------------------------*/

SCORE_TYPE CConParser::getOrUpdateHeadModifierScore( const unsigned long &parent, const unsigned long &child, const unsigned long &child1, const unsigned long &head, const unsigned long &mod, const bool &left, const bool &neighbour, SCORE_TYPE amount, int round ) {
   static SCORE_TYPE nReturn;
   nReturn = 0;
return 0;
   assert(left&&head<mod || !left&&head>mod);

   const CWord &hw = m_lCache[head];
   const CWord &mw = m_lCache[mod];
   static CTwoWords hwmw;
   if (amount!=0)
      hwmw.allocate(hw, mw);
   else
      hwmw.refer(&hw, &mw);
   static unsigned long pos;

   static CCFGSet cs; 

   if ( left && neighbour ) {
      pos = 1;
   }
   else if ( left && !neighbour ) {
      pos = 2;
   }
   else if (!left && neighbour) {
      pos = 3;
   }
   else {
      assert(!left && !neighbour);
      pos = 4;
   }
   
   cs.clear(); cs += parent; cs += child; cs += child1; cs += pos;
   nReturn += cast_weights->m_mapHcMc.getOrUpdateScore(cs, m_nScoreIndex, amount, round);
   nReturn += cast_weights->m_mapHcMw.getOrUpdateScore(make_pair(mw, cs), m_nScoreIndex, amount, round);
   nReturn += cast_weights->m_mapHwMc.getOrUpdateScore(make_pair(hw, cs), m_nScoreIndex, amount, round);
   nReturn += cast_weights->m_mapHwMw.getOrUpdateScore(make_pair(hwmw, cs), m_nScoreIndex, amount, round);

   return nReturn;
}

/*---------------------------------------------------------------
 * 
 * getOrUpdateBigramScore - get or update score for bigram con
 *
 *--------------------------------------------------------------*/

SCORE_TYPE CConParser::getOrUpdateBigramScore( const unsigned long &parent, const unsigned long &child, const unsigned long &child1, const bool &left, SCORE_TYPE amount, int round ) {

   static SCORE_TYPE nReturn;
   nReturn = 0;
return 0;
   static CCFGSet cs; cs.clear();
   cs += parent; cs += child; cs += child1;

   if (left) nReturn += cast_weights->m_mapBigramsL.getOrUpdateScore(cs, m_nScoreIndex, amount, round);
   else nReturn += cast_weights->m_mapBigramsR.getOrUpdateScore(cs, m_nScoreIndex, amount, round);
   
   return nReturn;
}

/*---------------------------------------------------------------
 *
 * getOrUpdateGraphScore - get or update the score from a single constituent link
 *
 *---------------------------------------------------------------*/

SCORE_TYPE CConParser::getOrUpdateGraphScore( const CStateItem *item, SCORE_TYPE amount, int round ) {
   return 0;

   static SCORE_TYPE nReturn;

   nReturn = 0;

   //const int bracket_start =
   //getStartingBracket(m_lCache[item->nodes[nd.single_child()?m_Context.s0_unbinarized.front():m_Context.s1_unbinarized.front()].lexical_head]);
   //const int bracket_end = getEndingBracket(m_lCache[item->nodes[m_Context.s0_unbinarized.back()].lexical_head]);
   //const unsigned long bracket = (bracket_start!=-1||bracket_end!=-1) ? (bracket_start==bracket_end) ? 1 : 2 : 0;
   //if (bracket != 0) 
   //   nReturn += cast_weights->m_mapBracket.getOrUpdateScore(encodeTorCs(bracket, nd.constituent), m_nScoreIndex, amount, round);

   // Rules
   //if (n<6) {
   //   ct = cf;
   //   j = 0;
   //   if (!nd.single_child()) {
   //      for (i=0; i<s1_size; ++i) {
   //         ct += m_Context.s1_unbinarized_cs[i];
   //      }
   //   }
   //   for (i=0; i<s0_size; ++i) {
   //      ct += m_Context.s0_unbinarized_cs[i];
   //   }
   //   nReturn += cast_weights->m_mapRules.getOrUpdateScore(ct, m_nScoreIndex, amount, round);
      //if (nd.norv) nReturn += cast_weights->m_mapRulesWithSizes.getOrUpdateScore(make_pair(ct, j), m_nScoreIndex, amount, round);
   //}

   return nReturn;
}

/*---------------------------------------------------------------
 * 
 * getOrUpdateStackScore - manipulate the score from stack 
 *
 *---------------------------------------------------------------*/

inline SCORE_TYPE CConParser::getOrUpdateStackScore( const CStateItem *item, const CAction &action, SCORE_TYPE amount , int round ) {

   if (m_Context.stacksize==0) return 0;

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

   static CTuple3<CWord, CConstituent, CAction> word_constituent_action; 
   static CTuple3<CTag, CConstituent, CAction> tag_constituent_action;
   static CTuple3<CTwoWords, CCFGSet, CAction> twoword_cfgset_action;
   static CTuple3<CTwoWords, CTag, CAction> twoword_tag_action;
//   static CTuple3<CWord, CTag, CAction> word_tag_action;
   static CTuple3<CWord, CCFGSet, CAction> word_cfgset_action;
   static CTuple3<CTwoWords, CTag, CActionType> twoword_tag_actiontype;
   static CTuple3<CWord, CCFGSet, CActionType> word_cfgset_actiontype;

   static CActionType actionType;
   actionType.code = action.type();

   static SCORE_TYPE nReturn;

   nReturn = 0;
//
   // action type features
//   if (m_Context.s1!=-1) {
//      nReturn +=
//      cast_weights->m_mapS0wtS1wt.getOrUpdateScore(make_pair(m_Context.s0wts1wt, actionType), m_nScoreIndex, amount, round);
//      refer_or_allocate_tuple3(twoword_tag_actiontype, &(m_Context.s0ws1w),
//      &(m_Context.s0t), &actionType); 
//      nReturn += cast_weights->m_mapS0wtS1w.getOrUpdateScore(twoword_tag_actiontype, m_nScoreIndex, amount, round);
//      refer_or_allocate_tuple3(twoword_tag_actiontype, &(m_Context.s0ws1w),
//      &(m_Context.s1t), &actionType); 
//      nReturn += cast_weights->m_mapS0wS1wt.getOrUpdateScore(twoword_tag_actiontype, m_nScoreIndex, amount, round);
//      nReturn +=
//      cast_weights->m_mapS0wS1w.getOrUpdateScore(make_pair(m_Context.s0ws1w, actionType), m_nScoreIndex, amount, round);
//      refer_or_allocate_tuple3(word_cfgset_actiontype, m_Context.s0w,
//      &(m_Context.s0ts1t), &actionType);
//      nReturn += cast_weights->m_mapS0wtS1t.getOrUpdateScore(word_cfgset_actiontype, m_nScoreIndex, amount, round);
//      refer_or_allocate_tuple3(word_cfgset_actiontype, m_Context.s1w,
//      &(m_Context.s0ts1t), &actionType); 
//      nReturn += cast_weights->m_mapS0tS1wt.getOrUpdateScore(word_cfgset_actiontype, m_nScoreIndex, amount, round);
//      nReturn +=
//      cast_weights->m_mapS0tS1t.getOrUpdateScore(make_pair(m_Context.s0ts1t, actionType), m_nScoreIndex, amount, round);
//
//      for (j=0; j<m_Context.between_tag.size(); j++) {
//         s0ts1tbt.setLast(m_Context.between_tag[j]);
//         nReturn += cast_weights->m_mapBetweenTags.getOrUpdateScore(make_pair(s0ts1tbt, actiontype), m_nScoreIndex, amount, round);
//      }
//   }

   // S0
   nReturn += cast_weights->m_mapS0w.getOrUpdateScore(make_pair(*(m_Context.s0wt), action), m_nScoreIndex, amount, round);
   if (!m_Context.s0c.empty()) nReturn += cast_weights->m_mapS0c.getOrUpdateScore(make_pair(m_Context.s0c, action), m_nScoreIndex, amount, round);
   refer_or_allocate_tuple3(tag_constituent_action, &(m_Context.s0t), &(m_Context.s0c), &action); 
   nReturn += cast_weights->m_mapS0tc.getOrUpdateScore(tag_constituent_action, m_nScoreIndex, amount, round);
   refer_or_allocate_tuple3(word_constituent_action, m_Context.s0w, &(m_Context.s0c), &action); 
   nReturn += cast_weights->m_mapS0wc.getOrUpdateScore(word_constituent_action, m_nScoreIndex, amount, round);

   // S1
   if (m_Context.s1!=0) {
      nReturn += cast_weights->m_mapS1w.getOrUpdateScore(make_pair(*(m_Context.s1wt), action), m_nScoreIndex, amount, round);
      if (!m_Context.s1c.empty()) nReturn += cast_weights->m_mapS1c.getOrUpdateScore(make_pair(m_Context.s1c, action), m_nScoreIndex, amount, round);
      refer_or_allocate_tuple3(tag_constituent_action, &(m_Context.s1t), &(m_Context.s1c), &action); 
      nReturn += cast_weights->m_mapS1tc.getOrUpdateScore(tag_constituent_action, m_nScoreIndex, amount, round);
      refer_or_allocate_tuple3(word_constituent_action, m_Context.s1w, &(m_Context.s1c), &action); 
      nReturn += cast_weights->m_mapS1wc.getOrUpdateScore(word_constituent_action, m_nScoreIndex, amount, round);
   }

   // S2
   if (m_Context.s2!=0) {
//      nReturn += cast_weights->m_mapS2w.getOrUpdateScore(make_pair(*(m_Context.s2w), action), m_nScoreIndex, amount, round);
//      nReturn += cast_weights->m_mapS2c.getOrUpdateScore(s2c_action, m_nScoreIndex, amount, round);
      refer_or_allocate_tuple3(tag_constituent_action, &(m_Context.s2t), &(m_Context.s2c), &action); 
      nReturn += cast_weights->m_mapS2tc.getOrUpdateScore(tag_constituent_action, m_nScoreIndex, amount, round);
      refer_or_allocate_tuple3(word_constituent_action, m_Context.s2w, &(m_Context.s2c), &action); 
      nReturn += cast_weights->m_mapS2wc.getOrUpdateScore(word_constituent_action, m_nScoreIndex, amount, round);
   }

   // S3
   if (m_Context.s3!=0) {
//      nReturn += cast_weights->m_mapS3w.getOrUpdateScore(make_pair(*(m_Context.s3w), action), m_nScoreIndex, amount, round);
//      nReturn += cast_weights->m_mapS3c.getOrUpdateScore(s3c_action, m_nScoreIndex, amount, round);
      refer_or_allocate_tuple3(tag_constituent_action, &(m_Context.s3t), &(m_Context.s3c), &action); 
      nReturn += cast_weights->m_mapS3tc.getOrUpdateScore(tag_constituent_action, m_nScoreIndex, amount, round);
      refer_or_allocate_tuple3(word_constituent_action, m_Context.s3w, &(m_Context.s3c), &action); 
      nReturn += cast_weights->m_mapS3wc.getOrUpdateScore(word_constituent_action, m_nScoreIndex, amount, round);
   }

   // N0
   if (m_Context.n0!=-1) {
//      nReturn += cast_weights->m_mapN0w.getOrUpdateScore(make_pair(*(m_Context.n0w), action), m_nScoreIndex, amount, round);
//      nReturn += cast_weights->m_mapN0t.getOrUpdateScore(n0t_action, m_nScoreIndex, amount, round);
      nReturn += cast_weights->m_mapN0wt.getOrUpdateScore(make_pair(*(m_Context.n0wt), action), m_nScoreIndex, amount, round);
   }

   // N1
   if (m_Context.n1!=-1) {
//      nReturn += cast_weights->m_mapN1w.getOrUpdateScore(make_pair(*(m_Context.n1w), action), m_nScoreIndex, amount, round);
//      nReturn += cast_weights->m_mapN1t.getOrUpdateScore(n1t_action, m_nScoreIndex, amount, round);
      nReturn += cast_weights->m_mapN1wt.getOrUpdateScore(make_pair(*(m_Context.n1wt), action), m_nScoreIndex, amount, round);
   }

   // N2
   if (m_Context.n2!=-1) {
//      nReturn += cast_weights->m_mapN2w.getOrUpdateScore(make_pair(*(m_Context.n2w), action), m_nScoreIndex, amount, round);
//      nReturn += cast_weights->m_mapN2t.getOrUpdateScore(n2t_action, m_nScoreIndex, amount, round);
      nReturn += cast_weights->m_mapN2wt.getOrUpdateScore(make_pair(*(m_Context.n2wt), action), m_nScoreIndex, amount, round);
   }

   // N3
   if (m_Context.n3!=-1) {
//      nReturn += cast_weights->m_mapN3w.getOrUpdateScore(make_pair(*(m_Context.n3w), action), m_nScoreIndex, amount, round);
//      nReturn += cast_weights->m_mapN3t.getOrUpdateScore(n3t_action, m_nScoreIndex, amount, round);
      nReturn += cast_weights->m_mapN3wt.getOrUpdateScore(make_pair(*(m_Context.n3wt), action), m_nScoreIndex, amount, round);
   }

   // S0L
   if (m_Context.s0l!=0) {
      refer_or_allocate_tuple3(tag_constituent_action, &(m_Context.s0lt), &(m_Context.s0lc), &action); 
      nReturn += cast_weights->m_mapS0Ltc.getOrUpdateScore(tag_constituent_action, m_nScoreIndex, amount, round);
      refer_or_allocate_tuple3(word_constituent_action, m_Context.s0lw, &(m_Context.s0lc), &action); 
      nReturn += cast_weights->m_mapS0Lwc.getOrUpdateScore(word_constituent_action, m_nScoreIndex, amount, round);
   }

   // S0R
   if (m_Context.s0r!=0) {         
      refer_or_allocate_tuple3(tag_constituent_action, &(m_Context.s0rt), &(m_Context.s0rc), &action); 
      nReturn += cast_weights->m_mapS0Rtc.getOrUpdateScore(tag_constituent_action, m_nScoreIndex, amount, round);
      refer_or_allocate_tuple3(word_constituent_action, m_Context.s0rw, &(m_Context.s0rc), &action); 
      nReturn += cast_weights->m_mapS0Rwc.getOrUpdateScore(word_constituent_action, m_nScoreIndex, amount, round);
   }

   // S0U
   if (m_Context.s0u!=0) {         
      refer_or_allocate_tuple3(tag_constituent_action, &(m_Context.s0ut), &(m_Context.s0uc), &action); 
      nReturn += cast_weights->m_mapS0Utc.getOrUpdateScore(tag_constituent_action, m_nScoreIndex, amount, round);
      refer_or_allocate_tuple3(word_constituent_action, m_Context.s0uw, &(m_Context.s0uc), &action); 
      nReturn += cast_weights->m_mapS0Uwc.getOrUpdateScore(word_constituent_action, m_nScoreIndex, amount, round);
   }


   // S1L
   if (m_Context.s1l!=0) {         
      refer_or_allocate_tuple3(tag_constituent_action, &(m_Context.s1lt), &(m_Context.s1lc), &action); 
      nReturn += cast_weights->m_mapS1Ltc.getOrUpdateScore(tag_constituent_action, m_nScoreIndex, amount, round);
      refer_or_allocate_tuple3(word_constituent_action, m_Context.s1lw, &(m_Context.s1lc), &action); 
      nReturn += cast_weights->m_mapS1Lwc.getOrUpdateScore(word_constituent_action, m_nScoreIndex, amount, round);
   }

   // S1R
   if (m_Context.s1r!=0) {         
      refer_or_allocate_tuple3(tag_constituent_action, &(m_Context.s1rt), &(m_Context.s1rc), &action); 
      nReturn += cast_weights->m_mapS1Rtc.getOrUpdateScore(tag_constituent_action, m_nScoreIndex, amount, round);
      refer_or_allocate_tuple3(word_constituent_action, m_Context.s1rw, &(m_Context.s1rc), &action); 
      nReturn += cast_weights->m_mapS1Rwc.getOrUpdateScore(word_constituent_action, m_nScoreIndex, amount, round);
   }

   // S1U
   if (m_Context.s1u!=0) {         
      refer_or_allocate_tuple3(tag_constituent_action, &(m_Context.s1ut), &(m_Context.s1uc), &action); 
      nReturn += cast_weights->m_mapS1Utc.getOrUpdateScore(tag_constituent_action, m_nScoreIndex, amount, round);
      refer_or_allocate_tuple3(word_constituent_action, m_Context.s1uw, &(m_Context.s1uc), &action); 
      nReturn += cast_weights->m_mapS1Uwc.getOrUpdateScore(word_constituent_action, m_nScoreIndex, amount, round);
   }

   // S0 S1
   if (m_Context.s1!=0) {
      refer_or_allocate_tuple3(twoword_cfgset_action, &(m_Context.s0ws1w), &(m_Context.s0cs1c), &action); 
      nReturn += cast_weights->m_mapS0wcS1wc.getOrUpdateScore(twoword_cfgset_action, m_nScoreIndex, amount, round);
      refer_or_allocate_tuple3(word_cfgset_action, m_Context.s1w, &(m_Context.s0cs1c), &action); 
      nReturn += cast_weights->m_mapS0cS1w.getOrUpdateScore(word_cfgset_action, m_nScoreIndex, amount, round);
      refer_or_allocate_tuple3(word_cfgset_action, m_Context.s0w, &(m_Context.s0cs1c), &action); 
      nReturn += cast_weights->m_mapS0wS1c.getOrUpdateScore(word_cfgset_action, m_nScoreIndex, amount, round);
      nReturn += cast_weights->m_mapS0cS1c.getOrUpdateScore(make_pair(m_Context.s0cs1c, action), m_nScoreIndex, amount, round);

   }

   // S0 N0
   if (m_Context.n0!=-1) {
      refer_or_allocate_tuple3(twoword_cfgset_action, &(m_Context.s0wn0w), &(m_Context.s0cn0t), &action); 
      nReturn += cast_weights->m_mapS0wN0w.getOrUpdateScore(twoword_cfgset_action, m_nScoreIndex, amount, round);
      refer_or_allocate_tuple3(word_cfgset_action, m_Context.n0w, &(m_Context.s0cn0t), &action); 
      nReturn += cast_weights->m_mapS0cN0w.getOrUpdateScore(word_cfgset_action, m_nScoreIndex, amount, round);
      refer_or_allocate_tuple3(word_cfgset_action, m_Context.s0w, &(m_Context.s0cn0t), &action); 
      nReturn += cast_weights->m_mapS0wN0t.getOrUpdateScore(word_cfgset_action, m_nScoreIndex, amount, round);
      nReturn += cast_weights->m_mapS0cN0t.getOrUpdateScore(make_pair(m_Context.s0cn0t, action), m_nScoreIndex, amount, round);
      //if (item->nodes[m_Context.s0].norv&&m_Context.n0norv) nReturn +=
      //cast_weights->m_mapS0cmN0tm.getOrUpdateScore(make_pair(s0cn0t_action,
      //encodeRhythms(m_Context.s0c, m_Context.n0m)), m_nScoreIndex, amount, round);
   }

   // S1 N0
   if (m_Context.s1!=0 && m_Context.n0!=-1) {
      refer_or_allocate_tuple3(twoword_cfgset_action, &(m_Context.s1wn0w), &(m_Context.s1cn0t), &action); 
      nReturn += cast_weights->m_mapS1wN0w.getOrUpdateScore(twoword_cfgset_action, m_nScoreIndex, amount, round);
      refer_or_allocate_tuple3(word_cfgset_action, m_Context.n0w, &(m_Context.s1cn0t), &action); 
      nReturn += cast_weights->m_mapS1cN0w.getOrUpdateScore(word_cfgset_action, m_nScoreIndex, amount, round);
      refer_or_allocate_tuple3(word_cfgset_action, m_Context.s1w, &(m_Context.s1cn0t), &action); 
      nReturn += cast_weights->m_mapS1wN0t.getOrUpdateScore(word_cfgset_action, m_nScoreIndex, amount, round);
      nReturn += cast_weights->m_mapS1cN0t.getOrUpdateScore(make_pair(m_Context.s1cn0t, action), m_nScoreIndex, amount, round);
   }

   // N0 N1
//   if (m_Context.n1!=-1) {
//      refer_or_allocate_tuple3(twoword_cfgset_action, &(m_Context.n0wn1w),
//      &(m_Context.n0tn1t), &action); 
//      nReturn += cast_weights->m_mapN0wN1w.getOrUpdateScore(twoword_cfgset_action, m_nScoreIndex, amount, round);
//      refer_or_allocate_tuple3(word_cfgset_action, m_Context.n1w, &(m_Context.n0tn1t), &action); 
//      nReturn += cast_weights->m_mapN0tN1w.getOrUpdateScore(word_cfgset_action, m_nScoreIndex, amount, round);
//      refer_or_allocate_tuple3(word_cfgset_action, m_Context.n0w, &(m_Context.n0tn1t), &action); 
//      nReturn += cast_weights->m_mapN0wN1t.getOrUpdateScore(word_cfgset_action, m_nScoreIndex, amount, round);
//      nReturn +=
//      cast_weights->m_mapN0tN1t.getOrUpdateScore(make_pair(m_Context.n0tn1t, action), m_nScoreIndex, amount, round);
//   }
//   
#ifdef _CHINESE_CFG_H
/*
   if (m_Context.open_bracket_match_type!=0) {
if (!m_Context.s0c.empty() && !m_Context.s1c.empty()) {
      nReturn += cast_weights->m_mapBracketS0c.getOrUpdateScore(s0c_bracket_action, m_nScoreIndex, amount, round);
      nReturn += cast_weights->m_mapBracketS0w.getOrUpdateScore(make_pair(*(m_Context.s0w), encodeAction(action, m_Context.open_bracket_match_type)), m_nScoreIndex, amount, round);
      if (m_Context.s1!=0) {
         nReturn += cast_weights->m_mapBracketS0wS1c.getOrUpdateScore(make_pair(*(m_Context.s0w), s1c_bracket_action), m_nScoreIndex, amount, round);
         nReturn +=
cast_weights->m_mapBracketS0cS1w.getOrUpdateScore(make_pair(*(m_Context.s1w), s0c_bracket_action), m_nScoreIndex, amount, round);
         nReturn += cast_weights->m_mapBracketS0cS1c.getOrUpdateScore(s0cs1c_bracket_action, m_nScoreIndex, amount, round);
      }
      if (m_Context.n0!=-1) {
         nReturn +=
cast_weights->m_mapBracketS0cN0w.getOrUpdateScore(make_pair(*(m_Context.n0w), s0c_bracket_action), m_nScoreIndex, amount, round);
         nReturn +=
cast_weights->m_mapBracketS0wN0t.getOrUpdateScore(make_pair(*(m_Context.s0w), n0t_bracket_action), m_nScoreIndex, amount, round);
         nReturn += cast_weights->m_mapBracketS0cN0t.getOrUpdateScore(s0cn0t_bracket_action, m_nScoreIndex, amount, round);
      }
}
   }

   if (m_Context.s1!=0) {
if (!m_Context.s0c.empty() && !m_Context.s1c.empty()) {
      static unsigned long i;
      for (i=0; i<m_Context.s0c_separator.size(); ++i) {
         nReturn +=
cast_weights->m_mapS0cSeparator.getOrUpdateScore(encodeAction(action, m_Context.s0c_separator[i]), m_nScoreIndex, amount, round);
         nReturn +=
cast_weights->m_mapS0wcSeparator.getOrUpdateScore(make_pair(*(m_Context.s0w),
encodeAction(action, m_Context.s0c_separator[i])), m_nScoreIndex, amount, round);
      }
      nReturn +=
cast_weights->m_mapS0cSepCount.getOrUpdateScore(encodeAction(action, m_Context.s0c_sepcount), m_nScoreIndex, amount, round);
      nReturn +=
cast_weights->m_mapS0wcSepCount.getOrUpdateScore(make_pair(*(m_Context.s0w),
encodeAction(action, m_Context.s0c_sepcount)), m_nScoreIndex, amount, round);
      for (i=0; i<m_Context.s1c_separator.size(); ++i) {
         nReturn +=
cast_weights->m_mapS1cSeparator.getOrUpdateScore(encodeAction(action, m_Context.s1c_separator[i]), m_nScoreIndex, amount, round);
         nReturn +=
cast_weights->m_mapS1wcSeparator.getOrUpdateScore(make_pair(*(m_Context.s1w),
encodeAction(action, m_Context.s1c_separator[i])), m_nScoreIndex, amount, round);
      }
      nReturn +=
cast_weights->m_mapS1cSepCount.getOrUpdateScore(encodeAction(action, m_Context.s1c_sepcount), m_nScoreIndex, amount, round);
      nReturn +=
cast_weights->m_mapS1wcSepCount.getOrUpdateScore(make_pair(*(m_Context.s1w),
encodeAction(action, m_Context.s1c_sepcount)), m_nScoreIndex, amount, round);
      for (i=0; i<m_Context.s0cs1c_separator.size(); ++i)
         nReturn +=
cast_weights->m_mapS0cS1cSeparator.getOrUpdateScore(encodeAction(action,
m_Context.s0cs1c_separator[i]), m_nScoreIndex, amount, round);
      nReturn +=
cast_weights->m_mapS0cS1cSepCount.getOrUpdateScore(encodeAction(action,
m_Context.s0cs1c_sepcount), m_nScoreIndex, amount, round);
}
   }
*/



#endif

//   if (m_Context.s1!=0){
//      nReturn +=
//      cast_weights->m_mapS0wS1Dist.getOrUpdateScore(make_pair(*(m_Context.s0w),
//      encodeAction(action, m_Context.s0s1_dist)), m_nScoreIndex, amount, round);
//      nReturn +=
//      cast_weights->m_mapS0cS1Dist.getOrUpdateScore(encodeAction(action,
//      m_Context.s0cs1_dist), m_nScoreIndex, amount, round);
//      nReturn +=
//      cast_weights->m_mapS0S1wDist.getOrUpdateScore(make_pair(*(m_Context.s1w),
//      encodeAction(action, m_Context.s0s1_dist)), m_nScoreIndex, amount, round);
//      nReturn +=
//      cast_weights->m_mapS0S1cDist.getOrUpdateScore(encodeAction(action,
//      m_Context.s0s1c_dist), m_nScoreIndex, amount, round);
//      nReturn +=
//      cast_weights->m_mapS0wS1wDist.getOrUpdateScore(make_pair(m_Context.s0ws1w, s0cs1c_distaction), m_nScoreIndex, amount, round);
//      nReturn +=
//      cast_weights->m_mapS0wS1cDist.getOrUpdateScore(make_pair(*(m_Context.s0w), s0cs1c_distaction), m_nScoreIndex, amount, round);
//      nReturn +=
//      cast_weights->m_mapS0cS1wDist.getOrUpdateScore(make_pair(*(m_Context.s1w), s0cs1c_distaction), m_nScoreIndex, amount, round);
//      nReturn += cast_weights->m_mapS0cS1cDist.getOrUpdateScore(s0cs1c_distaction, m_nScoreIndex, amount, round);
//   }

   // S0 S1 N0
   refer_or_allocate_tuple3(word_cfgset_action, m_Context.s0w, &(m_Context.s0cs1cn0t), &action); 
   nReturn += cast_weights->m_mapS0wS1cN0t.getOrUpdateScore(word_cfgset_action, m_nScoreIndex, amount, round);
   if (m_Context.s1!=0)  {
      refer_or_allocate_tuple3(word_cfgset_action, m_Context.s1w, &(m_Context.s0cs1cn0t), &action); 
      nReturn += cast_weights->m_mapS0cS1wN0t.getOrUpdateScore(word_cfgset_action, m_nScoreIndex, amount, round);
   }
   if (m_Context.n0!=-1) {
      refer_or_allocate_tuple3(word_cfgset_action, m_Context.n0w, &(m_Context.s0cs1cn0t), &action); 
      nReturn += cast_weights->m_mapS0cS1cN0w.getOrUpdateScore(word_cfgset_action, m_nScoreIndex, amount, round);
   }
   nReturn += cast_weights->m_mapS0cS1cN0t.getOrUpdateScore(make_pair(m_Context.s0cs1cn0t, action), m_nScoreIndex, amount, round); 

   nReturn += cast_weights->m_mapS0tS1tN0t.getOrUpdateScore(make_pair(m_Context.s0ts1tn0t, action), m_nScoreIndex, amount, round); 
//   nReturn +=
//   cast_weights->m_mapS0jS1jN0t.getOrUpdateScore(make_pair(m_Context.s0js1jn0t, action), m_nScoreIndex, amount, round); 

   // S0 N0 N1
   if (m_Context.n0!=-1) {
      refer_or_allocate_tuple3(word_cfgset_action, m_Context.s0w, &(m_Context.s0cn0tn1t), &action); 
      nReturn += cast_weights->m_mapS0wN0tN1t.getOrUpdateScore(word_cfgset_action, m_nScoreIndex, amount, round);
      refer_or_allocate_tuple3(word_cfgset_action, m_Context.n0w, &(m_Context.s0cn0tn1t), &action); 
      nReturn += cast_weights->m_mapS0cN0wN1t.getOrUpdateScore(word_cfgset_action, m_nScoreIndex, amount, round);
      if (m_Context.n1!=-1) {
         refer_or_allocate_tuple3(word_cfgset_action, m_Context.n1w, &(m_Context.s0cn0tn1t), &action); 
         nReturn += cast_weights->m_mapS0cN0tN1w.getOrUpdateScore(word_cfgset_action, m_nScoreIndex, amount, round);
      }
      nReturn +=
cast_weights->m_mapS0cN0tN1t.getOrUpdateScore(make_pair(m_Context.s0cn0tn1t,
action), m_nScoreIndex, amount, round); // m_Context.n0

      nReturn +=
cast_weights->m_mapS0tN0tN1t.getOrUpdateScore(make_pair(m_Context.s0tn0tn1t,
action), m_nScoreIndex, amount, round); // m_Context.n0
//      nReturn +=
//      cast_weights->m_mapS0jN0tN1t.getOrUpdateScore(make_pair(m_Context.s0jn0tn1t,
//      action), m_nScoreIndex, amount, round); // m_Context.n0
   }

   // S0 S1 S2
   if (m_Context.s1!=0) {
      refer_or_allocate_tuple3(word_cfgset_action, m_Context.s0w, &(m_Context.s0cs1cs2c), &action); 
      nReturn += cast_weights->m_mapS0wS1cS2c.getOrUpdateScore(word_cfgset_action, m_nScoreIndex, amount, round);
      refer_or_allocate_tuple3(word_cfgset_action, m_Context.s1w, &(m_Context.s0cs1cs2c), &action); 
      nReturn += cast_weights->m_mapS0cS1wS2c.getOrUpdateScore(word_cfgset_action, m_nScoreIndex, amount, round);
      if (m_Context.s2!=0) {
         refer_or_allocate_tuple3(word_cfgset_action, m_Context.s2w, &(m_Context.s0cs1cs2c), &action); 
         nReturn += cast_weights->m_mapS0cS1cS2w.getOrUpdateScore(word_cfgset_action, m_nScoreIndex, amount, round);
      }
      nReturn +=
cast_weights->m_mapS0cS1cS2c.getOrUpdateScore(make_pair(m_Context.s0cs1cs2c, action), m_nScoreIndex, amount, round);

      nReturn +=
cast_weights->m_mapS0tS1tS2t.getOrUpdateScore(make_pair(m_Context.s0ts1ts2t, action), m_nScoreIndex, amount, round);
//      nReturn +=
//      cast_weights->m_mapS0jS1jS2j.getOrUpdateScore(make_pair(m_Context.s0js1js2j, action), m_nScoreIndex, amount, round);
   }

   // Tag bigrams
//   nReturn += cast_weights->m_mapS1cS2c.getOrUpdateScore(encodeAction(action,
//   encodeTorCs(m_Context.s1c, m_Context.s2c)), m_nScoreIndex, amount, round);
//   nReturn += cast_weights->m_mapS2cS3c.getOrUpdateScore(encodeAction(action,
//   encodeTorCs(m_Context.s2c, m_Context.s3c)), m_nScoreIndex, amount, round);
//   nReturn += cast_weights->m_mapN1tN2t.getOrUpdateScore(encodeAction(action,
//   encodeTorCs(m_Context.n1t, m_Context.n2t)), m_nScoreIndex, amount, round);
//   nReturn += cast_weights->m_mapN2tN3t.getOrUpdateScore(encodeAction(action,
//   encodeTorCs(m_Context.n2t, m_Context.n3t)), m_nScoreIndex, amount, round);
   
//   nReturn += cast_weights->m_mapS0cS2c.getOrUpdateScore(encodeAction(action,
//   encodeTorCs(m_Context.s0c, m_Context.s2c)), m_nScoreIndex, amount, round);
//   nReturn += cast_weights->m_mapS1cS3c.getOrUpdateScore(encodeAction(action,
//   encodeTorCs(m_Context.s1c, m_Context.s3c)), m_nScoreIndex, amount, round);
//   nReturn += cast_weights->m_mapS0cN1t.getOrUpdateScore(encodeAction(action,
//   encodeTorCs(m_Context.s0c, m_Context.n1t)), m_nScoreIndex, amount, round);
//   nReturn += cast_weights->m_mapN0tN2t.getOrUpdateScore(encodeAction(action,
//   encodeTorCs(m_Context.n0t, m_Context.n2t)), m_nScoreIndex, amount, round);
//   nReturn += cast_weights->m_mapN1tN3t.getOrUpdateScore(encodeAction(action,
//   encodeTorCs(m_Context.n1t, m_Context.n3t)), m_nScoreIndex, amount, round);

   // S0 S0LRUN0
//   if (m_Context.s0l!=0) {
//      assert(m_Context.s0r!=0);
//      if (!item->nodes[m_Context.s0].temp||!item->nodes[m_Context.s0].head_left()) {
//      nReturn +=
//      cast_weights->m_mapS0cS0LcN0t.getOrUpdateScore(encodeAction(action,
//      encodeTorCs(m_Context.s0cn0t, m_Context.s0lc)), m_nScoreIndex, amount, round);
//      nReturn +=
//      cast_weights->m_mapS0wS0LcN0t.getOrUpdateScore(make_pair(*(m_Context.s0w),
//      encodeAction(action, encodeTorCs(m_Context.n0t, m_Context.s0lc))), m_nScoreIndex, amount, round);
//      if (m_Context.n0!=0) nReturn +=
//      cast_weights->m_mapS0cS0LcN0w.getOrUpdateScore(make_pair(*(m_Context.n0w),
//      encodeAction(action, encodeTorCs(m_Context.s0c, m_Context.s0lc))), m_nScoreIndex, amount, round);
//      }
//      if (item->nodes[m_Context.s0].head_left()) {
      if (m_Context.n0!=-1 && m_Context.s0r!=0) {
         nReturn += cast_weights->m_mapS0cS0RcN0t.getOrUpdateScore(make_pair(m_Context.s0cs0rcn0t, action), m_nScoreIndex, amount, round);
         nReturn += cast_weights->m_mapS0cS0RjN0t.getOrUpdateScore(make_pair(m_Context.s0cs0rjn0t, action), m_nScoreIndex, amount, round);
         refer_or_allocate_tuple3(word_cfgset_action, m_Context.n0w, &(m_Context.s0cs0rc), &action); 
         nReturn += cast_weights->m_mapS0cS0RcN0w.getOrUpdateScore(word_cfgset_action, m_nScoreIndex, amount, round);
      }
//      nReturn +=
//      cast_weights->m_mapS0wS0RcN0t.getOrUpdateScore(make_pair(*(m_Context.s0w),
//      encodeAction(action, encodeTorCs(m_Context.n0t, m_Context.s0rc))), m_nScoreIndex, amount, round);
//      }
//   }
//   if (m_Context.s0u!=0) {
//      assert(m_Context.s0l==0);
//      assert(item->nodes[m_Context.s0].temp==false);
//      nReturn +=
//      cast_weights->m_mapS0cS0UcN0t.getOrUpdateScore(encodeAction(action,
//      encodeTorCs(m_Context.s0cn0t, m_Context.s0uc)), m_nScoreIndex, amount, round);
//      nReturn +=
//      cast_weights->m_mapS0wS0UcN0t.getOrUpdateScore(make_pair(*(m_Context.s0w),
//      encodeAction(action, encodeTorCs(m_Context.s0uc, m_Context.n0t))), m_nScoreIndex, amount, round);
//      if (m_Context.n0!=-1) nReturn +=
//      cast_weights->m_mapS0cS0UcN0w.getOrUpdateScore(make_pair(*(m_Context.n0w),
//      encodeAction(action, encodeTorCs(m_Context.s0c, m_Context.s0uc))), m_nScoreIndex, amount, round);
//   }

   // S0 S0LRUS1
   if (m_Context.s1!=0 && m_Context.s0l!=0) {
      nReturn += cast_weights->m_mapS0cS0LcS1c.getOrUpdateScore(make_pair(m_Context.s0cs0lcs1c, action), m_nScoreIndex, amount, round);
      nReturn += cast_weights->m_mapS0cS0LjS1j.getOrUpdateScore(make_pair(m_Context.s0cs0ljs1j, action), m_nScoreIndex, amount, round);
      refer_or_allocate_tuple3(word_cfgset_action, m_Context.s1w, &(m_Context.s0cs0lc), &action); 
      nReturn += cast_weights->m_mapS0cS0LcS1w.getOrUpdateScore(word_cfgset_action, m_nScoreIndex, amount, round);
   }
   if (m_Context.s1 != 0 && m_Context.s1r != 0) {
      nReturn += cast_weights->m_mapS0cS1cS1Rc.getOrUpdateScore(make_pair(m_Context.s0cs1cs1rc, action), m_nScoreIndex, amount, round);
      nReturn += cast_weights->m_mapS0jS1cS1Rj.getOrUpdateScore(make_pair(m_Context.s0js1cs1rj, action), m_nScoreIndex, amount, round);
      refer_or_allocate_tuple3(word_cfgset_action, m_Context.s0w, &(m_Context.s1cs1rc), &action); 
      nReturn += cast_weights->m_mapS0wS1cS1Rc.getOrUpdateScore(word_cfgset_action, m_nScoreIndex, amount, round);
   }
//   if (m_Context.s0l!=0) {
//      if (!item->nodes[m_Context.s0].temp||!item->nodes[m_Context.s0].head_left()) {
//      nReturn +=
//      cast_weights->m_mapS0cS0LcS1c.getOrUpdateScore(encodeAction(action,
//      encodeTorCs(m_Context.s0cs1c, m_Context.s0lc)), m_nScoreIndex, amount, round);
//      nReturn +=
//      cast_weights->m_mapS0wS0LcS1c.getOrUpdateScore(make_pair(*(m_Context.s0w),
//      encodeAction(action, encodeTorCs(m_Context.s0lc, m_Context.s1c))), m_nScoreIndex, amount, round);
//      }
//      if (!item->nodes[m_Context.s0].temp||item->nodes[m_Context.s0].head_left()) {
//         nReturn +=
//         cast_weights->m_mapS0cS0RcS1c.getOrUpdateScore(encodeAction(action,
//         encodeTorCs(m_Context.s0cs1c, m_Context.s0rc)), m_nScoreIndex, amount, round);
//      nReturn +=
//      cast_weights->m_mapS0wS0RcS1c.getOrUpdateScore(make_pair(*(m_Context.s0w),
//      encodeAction(action, encodeTorCs(m_Context.s0rc, m_Context.s1c))), m_nScoreIndex, amount, round);
//      if (m_Context.s1!=0) nReturn +=
//      cast_weights->m_mapS0cS0RcS1w.getOrUpdateScore(make_pair(*(m_Context.s1w),
//      encodeAction(action, encodeTorCs(m_Context.s0c, m_Context.s0rc))), m_nScoreIndex, amount, round);
//      }
//   }
//   if (m_Context.s0u!=0) {
//      nReturn +=
//      cast_weights->m_mapS0cS0UcS1c.getOrUpdateScore(encodeAction(action,
//      encodeTorCs(m_Context.s0cs1c, m_Context.s0uc)), m_nScoreIndex, amount, round);
//      nReturn +=
//      cast_weights->m_mapS0wS0UcS1c.getOrUpdateScore(make_pair(*(m_Context.s0w),
//      encodeAction(action, encodeTorCs(m_Context.s0uc, m_Context.s1c))), m_nScoreIndex, amount, round);
//      if (m_Context.s1!=) nReturn +=
//      cast_weights->m_mapS0cS0UcS1w.getOrUpdateScore(make_pair(*(m_Context.s1w),
//      encodeAction(action, encodeTorCs(m_Context.s0c, m_Context.s0uc))), m_nScoreIndex, amount, round);
//   }

   // Tag trigram
   //if (m_Context.n0!=-1) {
   //   if (m_Context.n1!=-1) nReturn +=
   //   cast_weights->m_mapN0tN1tN2t.getOrUpdateScore(encodeAction(action,
   //   encodeTorCs(m_Context.n0tn1t, m_Context.n2t)), m_nScoreIndex, amount, round);
   //}
//   if (m_Context.s1l!=-1) {
//      assert(m_Context.s1r!=-1);
//      assert(!item->nodes[m_Context.s1].temp||item->nodes[m_Context.s1].head_left());
//      if (!item->nodes[m_Context.s1].temp) {
//         nReturn +=
//         cast_weights->m_mapS0cS1cS1Lc.getOrUpdateScore(encodeAction(action,
//         encodeTorCs(m_Context.s0cs1c, m_Context.s1lc)), m_nScoreIndex, amount, round);
//      nReturn +=
//      cast_weights->m_mapS0wS1cS1Lc.getOrUpdateScore(make_pair(*(m_Context.s0w),
//      encodeAction(action, encodeTorCs(m_Context.s1c, m_Context.s1lc))), m_nScoreIndex, amount, round);
//      nReturn +=
//      cast_weights->m_mapS0cS1wS1Lc.getOrUpdateScore(make_pair(*(m_Context.s1w),
//      encodeAction(action, encodeTorCs(m_Context.s0c, m_Context.s1lc))), m_nScoreIndex, amount, round);
//      }
//      nReturn +=
//      cast_weights->m_mapS0cS1cS1Rc.getOrUpdateScore(encodeAction(action,
//      encodeTorCs(m_Context.s0cs1c, m_Context.s1rc)), m_nScoreIndex, amount, round);
//      nReturn +=
//      cast_weights->m_mapS0cS1wS1Rc.getOrUpdateScore(make_pair(*(m_Context.s1w),
//      encodeAction(action, encodeTorCs(m_Context.s0c, m_Context.s1rc))), m_nScoreIndex, amount, round);
//   }
//   if (m_Context.s1u!=-1) {
//      nReturn +=
//      cast_weights->m_mapS0cS1cS1Uc.getOrUpdateScore(encodeAction(action,
//      encodeTorCs(m_Context.s0cs1c, m_Context.s1uc)), m_nScoreIndex, amount, round);
//      nReturn +=
//      cast_weights->m_mapS0wS1cS1Uc.getOrUpdateScore(make_pair(*(m_Context.s0w),
//      encodeAction(action, encodeTorCs(m_Context.s1c, m_Context.s1uc))), m_nScoreIndex, amount, round);
//      nReturn +=
//      cast_weights->m_mapS0cS1wS1Uc.getOrUpdateScore(make_pair(*(m_Context.s1w),
//      encodeAction(action, encodeTorCs(m_Context.s0c, m_Context.s1uc))), m_nScoreIndex, amount, round);
//   }
   return nReturn;
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
 * updateScores - update the score vector 
 *
 * This method is different from updateScoreVector in that
 * 1. It is for external call
 * 2. The tagging sequences for parsed and correct may differ
 *
 * Inputs: the parsed and the correct example
 *
 *---------------------------------------------------------------*/

void CConParser::updateScores(const CSentenceParsed & parsed , const CSentenceParsed & correct , int round ) {
   
   assert( m_bTrain );

   if ( round > m_nTrainingRound )
      m_nTrainingRound = round ;

   if ( parsed == correct ) 
      return;


    m_nTotalErrors++;

}

/*---------------------------------------------------------------
 *
 * updateScoresForState - update score
 *
 *--------------------------------------------------------------*/

void CConParser::updateScoresForState( const CStateItem *item , const SCORE_UPDATE update ) {

   static unsigned long stacksize;

   const SCORE_TYPE amount = (update==eAdd ? 1 : -1);

   const static CStateItem* states[MAX_SENTENCE_SIZE*(2+UNARY_MOVES)+2];
   static int count;
   const CStateItem *current;
   count = 0;
   current = item;
   while (current) {
      if (current) states[count] = current;
      ++count ; //updating
      current = current->statePtr;
   }
   // for each
   while (count>0) {
      m_Context.load(states[count], m_lCache, m_lWordLen, true);
      stacksize = states[count]->stacksize();
      // update action
      getOrUpdateStackScore(states[count], states[count-1]->action, amount, m_nTrainingRound );
   }
}

/*---------------------------------------------------------------
 *
 * updateScoresForStates - update scores for states
 *
 *--------------------------------------------------------------*/

void CConParser::updateScoresForStates( const CStateItem *output , const CStateItem *correct ) {

   cout << "updating parameters ... " ; 

   updateScoresForState( output, eSubtract );
   updateScoresForState( correct, eAdd );

   m_nTotalErrors++;
}

/*---------------------------------------------------------------
 *
 * getOrUpdateScore - get or update the score of a state and act
 *
 *--------------------------------------------------------------*/

SCORE_TYPE CConParser::getOrUpdateScore( const conparser::CStateItem &item, const conparser::CAction &action, conparser::SCORE_TYPE amount, int round ) {
   return getOrUpdateStackScore(&item, action, amount, round);
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
   static bool bCorrect ;  // used in learning for early update
   static int tmp_i, tmp_j;
   const static CStateItem *pBestGen;
   const static CStateItem *correctState ;
   static CAction action;
   static vector<CAction> actions; // actions to apply for a candidate
   static CAgendaSimple<CScoredStateAction> beam(AGENDA_SIZE);
   static CScoredStateAction scored_action; // used rank actions
   ASSERT(nBest=1, "currently only do 1 best parse");
   // TODO: it is easy to extend this into N-best; just use a vector for candidate_output. during train maybe use the best to adjust
   const static CStateItem *candidate_output;
   static unsigned index;

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
            for (tmp_j=0; tmp_j<actions.size(); ++tmp_j) {
               scored_action.load(actions[tmp_j], pGenerator, getOrUpdateScore(*pGenerator, actions[tmp_j]));
               beam.insertItem(&scored_action);
            }
         }
   
         // compare generator to corr
//         if (bTrain && *pGenerator == correctState) { 
//            bCorrect = true ;
//         }
      } // done iterating generator item

      // insertItems
      if (bTrain) { // initialize bestgen and correct next move
         pBestGen = 0;
         if (!correctState->IsTerminated()) {
            correctState->StandardMove(correct, action);
            bCorrect = false;
         }
      }
      for (tmp_j=0; tmp_j<beam.size(); ++tmp_j) { // insert from
         pGenerator = beam.item(tmp_j)->item;
         pGenerator->Move(lattice_index[index+1], beam.item(tmp_j)->action);
         lattice_index[index+1]->score += beam.item(tmp_j)->score;
         // update bestgen
         if (bTrain) {
            if ( pBestGen == 0 || lattice_index[index+1]->score > pBestGen->score ) {
               pBestGen = lattice_index[index+1];
            }
            if ( pGenerator == correctState && beam.item(tmp_j)->action == action ) {
               // pGenerator==correctState implies correctState not finished
               // since a finished generator will not be put into beam
               // and thus it implis that action is valid
               correctState = lattice_index[index+1];
               assert (correctState.unaryreduces()<=UNARY_MOVES) ; //TODO
               bCorrect = true;
            }
         }
         ++lattice_index[index+1];
      }

      // update items if correct item jump out of the agenda
      if (bTrain) { 
#ifdef EARLY_UPDATE
         if (!bCorrect && candidate_output!=correctState) {
            // no generator, nor the candidate output is error-free.
            correctState->trace(&sentence);
            pBestGen->trace(&sentence);
            TRACE("Error at the "<<correctState->current_word<<"th word; total is "<<m_lCache.size())
            updateScoresForStates(pBestGen, correctState) ; 
            return ;
         }
#endif
      }  // end of update

      m_Agenda->nextRound(); // move round
   }

   if (bTrain) {
      // make sure that the correct item is stack top finally
      if ( candidate_output != correctState ) {
         correctState->trace(&sentence);
         candidate_output->trace(&sentence);
         TRACE("The best item is not the correct one")
         updateScoresForStates(candidate_output, correctState) ; 
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

