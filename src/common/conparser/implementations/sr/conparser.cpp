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
#define s0_node(x) item->nodes[ctxt->s0_unbinarized[x]]
#define s1_node(x) item->nodes[ctxt->s1_unbinarized[x]]

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

   const unsigned long ht = encodeTorC(m_lCache[head].tag.code(), true);
   const CWord &hw = m_lCache[head];
   const unsigned long mt = encodeTorC(m_lCache[mod].tag.code(), true);
   const CWord &mw = m_lCache[mod];

   cf.clear(); cf+=ht; cf+=mt; cf+=0; cf+=0;
   nReturn += cast_weights->m_mapHtMt.getOrUpdateScore(cf, m_nScoreIndex, amount, round);
   cf.clear(); cf+=ht; cf+=mt; cf+=direction; cf+=0;
   nReturn += cast_weights->m_mapHtMt.getOrUpdateScore(cf, m_nScoreIndex, amount, round);
   cf.clear(); cf+=ht; cf+=mt; cf+=direction; cf+=dist;
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
   nReturn += cast_weights->m_mapSiblingTags.getOrUpdateScore(k | (direction<<(PENN_TAG_COUNT_BITS*3)), m_nScoreIndex, amount, round);
   nReturn += cast_weights->m_mapSiblingTags.getOrUpdateScore(k | (dist<<(PENN_TAG_COUNT_BITS*3)), m_nScoreIndex, amount, round);

   k = encodeTags(ht, mt);
   nReturn += cast_weights->m_mapSiblingWordTag.getOrUpdateScore(make_pair(sw, k), m_nScoreIndex, amount, round);
   nReturn += cast_weights->m_mapSiblingWordTag.getOrUpdateScore(make_pair(sw, k|(direction<<(PENN_TAG_COUNT_BITS*2))), m_nScoreIndex, amount, round);
   nReturn += cast_weights->m_mapSiblingWordTag.getOrUpdateScore(make_pair(sw, k|(dist<<(PENN_TAG_COUNT_BITS*2))), m_nScoreIndex, amount, round);

   k = encodeTags(ht, st);
   nReturn += cast_weights->m_mapSiblingTagWord.getOrUpdateScore(make_pair(mw, k), m_nScoreIndex, amount, round);
   nReturn += cast_weights->m_mapSiblingTagWord.getOrUpdateScore(make_pair(mw, k|(direction<<(PENN_TAG_COUNT_BITS*2))), m_nScoreIndex, amount, round);
   nReturn += cast_weights->m_mapSiblingTagWord.getOrUpdateScore(make_pair(mw, k|(dist<<(PENN_TAG_COUNT_BITS*2))), m_nScoreIndex, amount, round);

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
   assert(item->stack.size()>0);
   assert(item->nodes[item->stack.back()].is_constituent);
   const CStateNode &nd = item->nodes[item->stack.back()];
   if (nd.temp) return 0; // no constituent score for temp nodes

   static unsigned long i, j, k;
   static CCFGSet cf;
   static CCFGSet ct;
   cf.clear();
   ct.clear();

   const CContext *ctxt = item->context;

   assert( ctxt->s0_unbinarized.size() == ctxt->s0_unbinarized_cs.size() );
   assert( ctxt->s1_unbinarized.size() == ctxt->s1_unbinarized_cs.size() );
   const unsigned long &s0_size = ctxt->s0_unbinarized.size();
   const unsigned long &s1_size = ctxt->s1_unbinarized.size();

   if (!nd.single_child) {
      assert( (!nd.head_left||s0_size==1) && (nd.head_left||s1_size==1) );
      assert( nd.left_child==ctxt->s1 && nd.right_child==ctxt->s0);
   }
   else {
      assert( nd.left_child==ctxt->s0 );
   }

   const CWord &w = m_lCache[nd.lexical_head]; // lexical head
   const CTag &t = m_lCache[nd.lexical_head].tag; // head tag

   static unsigned long n; // number of constituents in the rule
   static unsigned long hd; // head node
   hd = nd.single_child?ctxt->s0_head_node:nd.head_left?ctxt->s1_head_node:ctxt->s0_head_node;
   n = nd.single_child?s0_size:s1_size+s0_size;

   static SCORE_TYPE nReturn;

   nReturn = 0;

   //const int bracket_start = getStartingBracket(m_lCache[item->nodes[nd.single_child?ctxt->s0_unbinarized.front():ctxt->s1_unbinarized.front()].lexical_head]);
   //const int bracket_end = getEndingBracket(m_lCache[item->nodes[ctxt->s0_unbinarized.back()].lexical_head]);
   //const unsigned long bracket = (bracket_start!=-1||bracket_end!=-1) ? (bracket_start==bracket_end) ? 1 : 2 : 0;
   //if (bracket != 0) 
   //   nReturn += cast_weights->m_mapBracket.getOrUpdateScore(encodeTorCs(bracket, nd.constituent), m_nScoreIndex, amount, round);

   // Rules
   //if (n<6) {
   //   ct = cf;
   //   j = 0;
   //   if (!nd.single_child) {
   //      for (i=0; i<s1_size; ++i) {
   //         ct += ctxt->s1_unbinarized_cs[i];
            //if(nd.norv)j = encodeRhythms(j, encodeRhythm(item->nodes[s1_unbinarized[i], m_lWordLen));
   //      }
   //   }
   //   for (i=0; i<s0_size; ++i) {
   //      ct += ctxt->s0_unbinarized_cs[i];
         //if(nd.norv)j = encodeRhythms(j, encodeRhythm(item->nodes[s0_unbinarized[i], m_lWordLen));
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

inline SCORE_TYPE CConParser::getOrUpdateStackScore( const CStateItem *item, const unsigned long &action, SCORE_TYPE amount , int round ) {

   const CContext *ctxt = item->context;
   if (ctxt->stacksize==0) return 0;

   static unsigned long s0c_action;
   static unsigned long s1c_action;
   static unsigned long s2c_action;
   static unsigned long s3c_action;

   static unsigned long n0t_action;
   static unsigned long n1t_action;
   static unsigned long n2t_action;
   static unsigned long n3t_action;

   static unsigned long s0cs1c_action;
   static unsigned long s0cn0t_action;
   static unsigned long n0tn1t_action;
   static unsigned long s1cn0t_action;

   static unsigned long s0cs1cs2c_action;
   static unsigned long s0cs1cn0t_action;
   static unsigned long s0cn0tn1t_action;

#ifdef _CHINESE_CFG_H
   static unsigned long s0c_bracket_action;
   static unsigned long s1c_bracket_action;
   static unsigned long n0t_bracket_action;
   static unsigned long s0cs1c_bracket_action;
   static unsigned long s0cn0t_bracket_action;
#endif

   static unsigned long s0cs1c_distaction;

   s0c_action = encodeAction(action, ctxt->s0c);
   s1c_action = encodeAction(action, ctxt->s1c);
   s2c_action = encodeAction(action, ctxt->s2c);
   s3c_action = encodeAction(action, ctxt->s3c);

   n0t_action = encodeAction(action, ctxt->n0t);
   n1t_action = encodeAction(action, ctxt->n1t);
   n2t_action = encodeAction(action, ctxt->n2t);
   n3t_action = encodeAction(action, ctxt->n3t);

   s0cs1c_action = encodeAction(action, ctxt->s0cs1c);
   s0cn0t_action = encodeAction(action, ctxt->s0cn0t);
   n0tn1t_action = encodeAction(action, ctxt->n0tn1t);
   s1cn0t_action = encodeAction(action, ctxt->s1cn0t);

   s0cs1cs2c_action=encodeAction(action, encodeTorCs(ctxt->s0cs1c, ctxt->s2c));
   s0cs1cn0t_action=encodeAction(action, encodeTorCs(ctxt->s0cs1c, ctxt->n0t));
   s0cn0tn1t_action=encodeAction(action, encodeTorCs(ctxt->s0cn0t, ctxt->n1t));

#ifdef _CHINESE_CFG_H
   s0c_bracket_action = encodeAction(action, ctxt->s0c_bracket);
   s1c_bracket_action = encodeAction(action, ctxt->s1c_bracket);
   n0t_bracket_action = encodeAction(action, ctxt->n0t_bracket);
   s0cs1c_bracket_action = encodeAction(action, ctxt->s0cs1c_bracket);
   s0cn0t_bracket_action = encodeAction(action, ctxt->s0cn0t_bracket);
#endif

   s0cs1c_distaction = encodeAction(action, ctxt->s0cs1c_dist);

   static SCORE_TYPE nReturn;

   nReturn = 0;
//
   // S0
//   nReturn += cast_weights->m_mapS0w.getOrUpdateScore(make_pair(*(ctxt->s0w), action), m_nScoreIndex, amount, round);
//   nReturn += cast_weights->m_mapS0c.getOrUpdateScore(s0c_action, m_nScoreIndex, amount, round);
   //if (item->nodes[ctxt->s0].norv) nReturn += cast_weights->m_mapS0cm.getOrUpdateScore(encodeAction(action, encodeTorCs(ctxt->s0c, ctxt->s0m)), m_nScoreIndex, amount, round);
   nReturn += cast_weights->m_mapS0t.getOrUpdateScore(encodeAction(action, encodeTorCs(ctxt->s0t, ctxt->s0c)), m_nScoreIndex, amount, round);
   nReturn += cast_weights->m_mapS0wc.getOrUpdateScore(make_pair(*(ctxt->s0w), s0c_action), m_nScoreIndex, amount, round);

   // S1
   if (ctxt->s1!=-1) {
//      nReturn += cast_weights->m_mapS1w.getOrUpdateScore(make_pair(*(ctxt->s1w), action), m_nScoreIndex, amount, round);
//      nReturn += cast_weights->m_mapS1c.getOrUpdateScore(s1c_action, m_nScoreIndex, amount, round);
      //if (item->nodes[ctxt->s1].norv) nReturn += cast_weights->m_mapS1cm.getOrUpdateScore(encodeAction(action, encodeTorCs(ctxt->s1c, ctxt->s1m)), m_nScoreIndex, amount, round);
      nReturn += cast_weights->m_mapS1t.getOrUpdateScore(encodeAction(action, encodeTorCs(ctxt->s1t, ctxt->s1c)), m_nScoreIndex, amount, round);
      nReturn += cast_weights->m_mapS1wc.getOrUpdateScore(make_pair(*(ctxt->s1w), s1c_action), m_nScoreIndex, amount, round);
   }

   // S2
   if (ctxt->s2!=-1) {
//      nReturn += cast_weights->m_mapS2w.getOrUpdateScore(make_pair(*(ctxt->s2w), action), m_nScoreIndex, amount, round);
//      nReturn += cast_weights->m_mapS2c.getOrUpdateScore(s2c_action, m_nScoreIndex, amount, round);
      nReturn += cast_weights->m_mapS2t.getOrUpdateScore(encodeAction(action, encodeTorCs(ctxt->s2t, ctxt->s2c)), m_nScoreIndex, amount, round);
      nReturn += cast_weights->m_mapS2wc.getOrUpdateScore(make_pair(*(ctxt->s2w), s2c_action), m_nScoreIndex, amount, round);
   }

   // S3
   if (ctxt->s3!=-1) {
//      nReturn += cast_weights->m_mapS3w.getOrUpdateScore(make_pair(*(ctxt->s3w), action), m_nScoreIndex, amount, round);
//      nReturn += cast_weights->m_mapS3c.getOrUpdateScore(s3c_action, m_nScoreIndex, amount, round);
      nReturn += cast_weights->m_mapS3t.getOrUpdateScore(encodeAction(action, encodeTorCs(ctxt->s3t, ctxt->s3c)), m_nScoreIndex, amount, round);
      nReturn += cast_weights->m_mapS3wc.getOrUpdateScore(make_pair(*(ctxt->s3w), s3c_action), m_nScoreIndex, amount, round);
   }

   // N0
   if (ctxt->n0!=-1) {
//      nReturn += cast_weights->m_mapN0w.getOrUpdateScore(make_pair(*(ctxt->n0w), action), m_nScoreIndex, amount, round);
//      nReturn += cast_weights->m_mapN0t.getOrUpdateScore(n0t_action, m_nScoreIndex, amount, round);
      nReturn += cast_weights->m_mapN0wt.getOrUpdateScore(make_pair(*(ctxt->n0w), n0t_action), m_nScoreIndex, amount, round);
   }

   // N1
   if (ctxt->n1!=-1) {
//      nReturn += cast_weights->m_mapN1w.getOrUpdateScore(make_pair(*(ctxt->n1w), action), m_nScoreIndex, amount, round);
//      nReturn += cast_weights->m_mapN1t.getOrUpdateScore(n1t_action, m_nScoreIndex, amount, round);
      nReturn += cast_weights->m_mapN1wt.getOrUpdateScore(make_pair(*(ctxt->n1w), n1t_action), m_nScoreIndex, amount, round);
   }

   // N2
   if (ctxt->n2!=-1) {
//      nReturn += cast_weights->m_mapN2w.getOrUpdateScore(make_pair(*(ctxt->n2w), action), m_nScoreIndex, amount, round);
//      nReturn += cast_weights->m_mapN2t.getOrUpdateScore(n2t_action, m_nScoreIndex, amount, round);
      nReturn += cast_weights->m_mapN2wt.getOrUpdateScore(make_pair(*(ctxt->n2w), n2t_action), m_nScoreIndex, amount, round);
   }

   // N3
   if (ctxt->n3!=-1) {
//      nReturn += cast_weights->m_mapN3w.getOrUpdateScore(make_pair(*(ctxt->n3w), action), m_nScoreIndex, amount, round);
//      nReturn += cast_weights->m_mapN3t.getOrUpdateScore(n3t_action, m_nScoreIndex, amount, round);
      nReturn += cast_weights->m_mapN3wt.getOrUpdateScore(make_pair(*(ctxt->n3w), n3t_action), m_nScoreIndex, amount, round);
   }

   // S0L
   if (ctxt->s0l!=-1) {
      nReturn += cast_weights->m_mapS0Lw.getOrUpdateScore(make_pair(*(ctxt->s0lw), encodeAction(action, ctxt->s0lc)), m_nScoreIndex, amount, round);
//      nReturn += cast_weights->m_mapS0Lc.getOrUpdateScore(encodeAction(action, ctxt->s0lc), m_nScoreIndex, amount, round);
   }

   // S0R
   if (ctxt->s0r!=-1) {         
      nReturn += cast_weights->m_mapS0Rw.getOrUpdateScore(make_pair(*(ctxt->s0rw), encodeAction(action, ctxt->s0rc)), m_nScoreIndex, amount, round);
//      nReturn += cast_weights->m_mapS0Rc.getOrUpdateScore(encodeAction(action, ctxt->s0rc), m_nScoreIndex, amount, round);
   }

   // S0U
   if (ctxt->s0u!=-1) {         
      nReturn += cast_weights->m_mapS0Uw.getOrUpdateScore(make_pair(*(ctxt->s0uw), encodeAction(action, ctxt->s0uc)), m_nScoreIndex, amount, round);
//      nReturn += cast_weights->m_mapS0Uc.getOrUpdateScore(encodeAction(action, ctxt->s0uc), m_nScoreIndex, amount, round);
   }


   // S1L
   if (ctxt->s1l!=-1) {         
      nReturn += cast_weights->m_mapS1Lw.getOrUpdateScore(make_pair(*(ctxt->s1lw), encodeAction(action, ctxt->s1lc)), m_nScoreIndex, amount, round);
//      nReturn += cast_weights->m_mapS1Lc.getOrUpdateScore(encodeAction(action, ctxt->s1lc), m_nScoreIndex, amount, round);
   }

   // S1R
   if (ctxt->s1r!=-1) {         
      nReturn += cast_weights->m_mapS1Rw.getOrUpdateScore(make_pair(*(ctxt->s1rw), encodeAction(action, ctxt->s1rc)), m_nScoreIndex, amount, round);
//      nReturn += cast_weights->m_mapS1Rc.getOrUpdateScore(encodeAction(action, ctxt->s1rc), m_nScoreIndex, amount, round);
   }

   // S1U
   if (ctxt->s1u!=-1) {         
      nReturn += cast_weights->m_mapS1Uw.getOrUpdateScore(make_pair(*(ctxt->s1uw), encodeAction(action, ctxt->s1uc)), m_nScoreIndex, amount, round);
//      nReturn += cast_weights->m_mapS1Uc.getOrUpdateScore(encodeAction(action, ctxt->s1uc), m_nScoreIndex, amount, round);
   }

   // S0LD
   if (ctxt->s0ld!=-1) {
//      nReturn += cast_weights->m_mapS0LDw.getOrUpdateScore(make_pair(*(ctxt->s0ldw), encodeAction(action, ctxt->s0ldt)), m_nScoreIndex, amount, round);
//      nReturn += cast_weights->m_mapS0LDt.getOrUpdateScore(encodeAction(action, ctxt->s0ldt), m_nScoreIndex, amount, round);
   }

   // S0RD
   if (ctxt->s0rd!=-1) {
//      nReturn += cast_weights->m_mapS0RDw.getOrUpdateScore(make_pair(*(ctxt->s0rdw), encodeAction(action, ctxt->s0rdt)), m_nScoreIndex, amount, round);
//      nReturn += cast_weights->m_mapS0RDt.getOrUpdateScore(encodeAction(action, ctxt->s0rdt), m_nScoreIndex, amount, round);
   }

   // S1LD
   if (ctxt->s1ld!=-1) {         
//      nReturn += cast_weights->m_mapS1LDw.getOrUpdateScore(make_pair(*(ctxt->s1ldw), encodeAction(action, ctxt->s1ldt)), m_nScoreIndex, amount, round);
//      nReturn += cast_weights->m_mapS1LDt.getOrUpdateScore(encodeAction(action, ctxt->s1ldt), m_nScoreIndex, amount, round);
   }

   // S1RD
   if (ctxt->s1rd!=-1) {         
//      nReturn += cast_weights->m_mapS1RDw.getOrUpdateScore(make_pair(*(ctxt->s1rdw), encodeAction(action, ctxt->s1rdt)), m_nScoreIndex, amount, round);
//      nReturn += cast_weights->m_mapS1RDt.getOrUpdateScore(encodeAction(action, ctxt->s1rdt), m_nScoreIndex, amount, round);
   }

   // S0 S1
   if (ctxt->s1!=-1) {
      nReturn += cast_weights->m_mapS0wS1w.getOrUpdateScore(make_pair(ctxt->s0ws1w, s0cs1c_action), m_nScoreIndex, amount, round);
      nReturn += cast_weights->m_mapS0cS1w.getOrUpdateScore(make_pair(*(ctxt->s1w), s0cs1c_action), m_nScoreIndex, amount, round);
      nReturn += cast_weights->m_mapS0wS1c.getOrUpdateScore(make_pair(*(ctxt->s0w), s0cs1c_action), m_nScoreIndex, amount, round);
//      nReturn += cast_weights->m_mapS0cS1t.getOrUpdateScore(make_pair(ctxt->s1t, s0c_action), m_nScoreIndex, amount, round);
//      nReturn += cast_weights->m_mapS0tS1c.getOrUpdateScore(make_pair(ctxt->s0t, s1c_action), m_nScoreIndex, amount, round);
      nReturn += cast_weights->m_mapS0cS1c.getOrUpdateScore(s0cs1c_action, m_nScoreIndex, amount, round);
      //if (item->nodes[ctxt->s0].norv && item->nodes[ctxt->s1].norv) nReturn += cast_weights->m_mapS0cmS1cm.getOrUpdateScore(make_pair(s0cs1c_action, encodeRhythms(ctxt->s0m, ctxt->s1m)), m_nScoreIndex, amount, round);
   }

   // S0 N0
   if (ctxt->n0!=-1) {
      nReturn += cast_weights->m_mapS0wN0w.getOrUpdateScore(make_pair(ctxt->s0wn0w, s0cn0t_action), m_nScoreIndex, amount, round);
      nReturn += cast_weights->m_mapS0cN0w.getOrUpdateScore(make_pair(*(ctxt->n0w), s0cn0t_action), m_nScoreIndex, amount, round);
      nReturn += cast_weights->m_mapS0wN0t.getOrUpdateScore(make_pair(*(ctxt->s0w), s0cn0t_action), m_nScoreIndex, amount, round);
      nReturn += cast_weights->m_mapS0cN0t.getOrUpdateScore(s0cn0t_action, m_nScoreIndex, amount, round);
      //if (item->nodes[ctxt->s0].norv&&ctxt->n0norv) nReturn += cast_weights->m_mapS0cmN0tm.getOrUpdateScore(make_pair(s0cn0t_action, encodeRhythms(ctxt->s0c, ctxt->n0m)), m_nScoreIndex, amount, round);
   }

   // S1 N0
   if (ctxt->s1!=-1 && ctxt->n0!=-1) {
      nReturn += cast_weights->m_mapS1wN0w.getOrUpdateScore(make_pair(ctxt->s1wn0w, s1cn0t_action), m_nScoreIndex, amount, round);
      nReturn += cast_weights->m_mapS1cN0w.getOrUpdateScore(make_pair(*(ctxt->n0w), s1cn0t_action), m_nScoreIndex, amount, round);
      nReturn += cast_weights->m_mapS1wN0t.getOrUpdateScore(make_pair(*(ctxt->s1w), s1cn0t_action), m_nScoreIndex, amount, round);
      nReturn += cast_weights->m_mapS1cN0t.getOrUpdateScore(s1cn0t_action, m_nScoreIndex, amount, round);
   }

   // N0 N1
   if (ctxt->n1!=-1) {
//      nReturn += cast_weights->m_mapN0wN1w.getOrUpdateScore(make_pair(ctxt->n0wn1w, n0tn1t_action), m_nScoreIndex, amount, round);
//      nReturn += cast_weights->m_mapN0tN1w.getOrUpdateScore(make_pair(*(ctxt->n1w), n0tn1t_action), m_nScoreIndex, amount, round);
//      nReturn += cast_weights->m_mapN0wN1t.getOrUpdateScore(make_pair(*(ctxt->n0w), n0tn1t_action), m_nScoreIndex, amount, round);
//      nReturn += cast_weights->m_mapN0tN1t.getOrUpdateScore(n0tn1t_action, m_nScoreIndex, amount, round);
   }
   
#ifdef _CHINESE_CFG_H
   if (ctxt->open_bracket_match_type!=0) {
      nReturn += cast_weights->m_mapBracketS0c.getOrUpdateScore(s0c_bracket_action, m_nScoreIndex, amount, round);
      nReturn += cast_weights->m_mapBracketS0w.getOrUpdateScore(make_pair(*(ctxt->s0w), encodeAction(action, ctxt->open_bracket_match_type)), m_nScoreIndex, amount, round);
      if (ctxt->s1!=-1) {
         nReturn += cast_weights->m_mapBracketS0wS1c.getOrUpdateScore(make_pair(*(ctxt->s0w), s1c_bracket_action), m_nScoreIndex, amount, round);
         nReturn += cast_weights->m_mapBracketS0cS1w.getOrUpdateScore(make_pair(*(ctxt->s1w), s0c_bracket_action), m_nScoreIndex, amount, round);
         nReturn += cast_weights->m_mapBracketS0cS1c.getOrUpdateScore(s0cs1c_bracket_action, m_nScoreIndex, amount, round);
      }
      if (ctxt->n0!=-1) {
         nReturn += cast_weights->m_mapBracketS0cN0w.getOrUpdateScore(make_pair(*(ctxt->n0w), s0c_bracket_action), m_nScoreIndex, amount, round);
         nReturn += cast_weights->m_mapBracketS0wN0t.getOrUpdateScore(make_pair(*(ctxt->s0w), n0t_bracket_action), m_nScoreIndex, amount, round);
         nReturn += cast_weights->m_mapBracketS0cN0t.getOrUpdateScore(s0cn0t_bracket_action, m_nScoreIndex, amount, round);
      }
   }

   if (ctxt->s1!=-1) {
      static unsigned long i;
      for (i=0; i<ctxt->s0c_separator.size(); ++i) {
         nReturn += cast_weights->m_mapS0cSeparator.getOrUpdateScore(encodeAction(action, ctxt->s0c_separator[i]), m_nScoreIndex, amount, round);
         nReturn += cast_weights->m_mapS0wcSeparator.getOrUpdateScore(make_pair(*(ctxt->s0w), encodeAction(action, ctxt->s0c_separator[i])), m_nScoreIndex, amount, round);
      }
      nReturn += cast_weights->m_mapS0cSepCount.getOrUpdateScore(encodeAction(action, ctxt->s0c_sepcount), m_nScoreIndex, amount, round);
      nReturn += cast_weights->m_mapS0wcSepCount.getOrUpdateScore(make_pair(*(ctxt->s0w), encodeAction(action, ctxt->s0c_sepcount)), m_nScoreIndex, amount, round);
      for (i=0; i<ctxt->s1c_separator.size(); ++i) {
         nReturn += cast_weights->m_mapS1cSeparator.getOrUpdateScore(encodeAction(action, ctxt->s1c_separator[i]), m_nScoreIndex, amount, round);
         nReturn += cast_weights->m_mapS1wcSeparator.getOrUpdateScore(make_pair(*(ctxt->s1w), encodeAction(action, ctxt->s1c_separator[i])), m_nScoreIndex, amount, round);
      }
      nReturn += cast_weights->m_mapS1cSepCount.getOrUpdateScore(encodeAction(action, ctxt->s1c_sepcount), m_nScoreIndex, amount, round);
      nReturn += cast_weights->m_mapS1wcSepCount.getOrUpdateScore(make_pair(*(ctxt->s1w), encodeAction(action, ctxt->s1c_sepcount)), m_nScoreIndex, amount, round);
      for (i=0; i<ctxt->s0cs1c_separator.size(); ++i)
         nReturn += cast_weights->m_mapS0cS1cSeparator.getOrUpdateScore(encodeAction(action, ctxt->s0cs1c_separator[i]), m_nScoreIndex, amount, round);
      nReturn += cast_weights->m_mapS0cS1cSepCount.getOrUpdateScore(encodeAction(action, ctxt->s0cs1c_sepcount), m_nScoreIndex, amount, round);
   }
#endif

   if (ctxt->s1!=-1){
//      nReturn += cast_weights->m_mapS0wS1Dist.getOrUpdateScore(make_pair(*(ctxt->s0w), encodeAction(action, ctxt->s0s1_dist)), m_nScoreIndex, amount, round);
//      nReturn += cast_weights->m_mapS0cS1Dist.getOrUpdateScore(encodeAction(action, ctxt->s0cs1_dist), m_nScoreIndex, amount, round);
//      nReturn += cast_weights->m_mapS0S1wDist.getOrUpdateScore(make_pair(*(ctxt->s1w), encodeAction(action, ctxt->s0s1_dist)), m_nScoreIndex, amount, round);
//      nReturn += cast_weights->m_mapS0S1cDist.getOrUpdateScore(encodeAction(action, ctxt->s0s1c_dist), m_nScoreIndex, amount, round);
//      nReturn += cast_weights->m_mapS0wS1wDist.getOrUpdateScore(make_pair(ctxt->s0ws1w, s0cs1c_distaction), m_nScoreIndex, amount, round);
//      nReturn += cast_weights->m_mapS0wS1cDist.getOrUpdateScore(make_pair(*(ctxt->s0w), s0cs1c_distaction), m_nScoreIndex, amount, round);
//      nReturn += cast_weights->m_mapS0cS1wDist.getOrUpdateScore(make_pair(*(ctxt->s1w), s0cs1c_distaction), m_nScoreIndex, amount, round);
//      nReturn += cast_weights->m_mapS0cS1cDist.getOrUpdateScore(s0cs1c_distaction, m_nScoreIndex, amount, round);
   }

   // S0 S1 N0
   nReturn += cast_weights->m_mapS0wS1cN0t.getOrUpdateScore(make_pair(*(ctxt->s0w), s0cs1cn0t_action), m_nScoreIndex, amount, round);
   if (ctxt->s1!=-1)  nReturn += cast_weights->m_mapS0cS1wN0t.getOrUpdateScore(make_pair(*(ctxt->s1w), s0cs1cn0t_action), m_nScoreIndex, amount, round);
   if (ctxt->n0!=-1) nReturn += cast_weights->m_mapS0cS1cN0w.getOrUpdateScore(make_pair(*(ctxt->n0w), s0cs1cn0t_action), m_nScoreIndex, amount, round);
   nReturn += cast_weights->m_mapS0cS1cN0t.getOrUpdateScore(s0cs1cn0t_action, m_nScoreIndex, amount, round); 
   //if (ctxt->s1!=-1 && ctxt->n0!=-1 && item->nodes[ctxt->s1].norv && ctxt->n0norv) nReturn += cast_weights->m_mapS0cmS1cmN0tm.getOrUpdateScore(make_pair(encodeAction(action, encodeTorCs(ctxt->s0cs1c, ctxt->n0t)), encodeRhythms(encodeRhythms(ctxt->s0m, ctxt->s1m), ctxt->n0m)), m_nScoreIndex, amount, round);

   // S0 N0 N1
   if (ctxt->n0!=-1) {
      nReturn += cast_weights->m_mapS0wN0tN1t.getOrUpdateScore(make_pair(*(ctxt->s0w), s0cn0tn1t_action), m_nScoreIndex, amount, round);
      nReturn += cast_weights->m_mapS0cN0wN1t.getOrUpdateScore(make_pair(*(ctxt->n0w), s0cn0tn1t_action), m_nScoreIndex, amount, round);
      if (ctxt->n1!=-1) nReturn += cast_weights->m_mapS0cN0tN1w.getOrUpdateScore(make_pair(*(ctxt->n1w), s0cn0tn1t_action), m_nScoreIndex, amount, round);
      nReturn += cast_weights->m_mapS0cN0tN1t.getOrUpdateScore(s0cn0tn1t_action, m_nScoreIndex, amount, round); // ctxt->n0
   }

   // S0 S1 S2
   if (ctxt->s1!=-1) {
      nReturn += cast_weights->m_mapS0wS1cS2c.getOrUpdateScore(make_pair(*(ctxt->s0w), s0cs1cs2c_action), m_nScoreIndex, amount, round);
      nReturn += cast_weights->m_mapS0cS1wS2c.getOrUpdateScore(make_pair(*(ctxt->s1w), s0cs1cs2c_action), m_nScoreIndex, amount, round);
      if (ctxt->s2!=-1) nReturn += cast_weights->m_mapS0cS1cS2w.getOrUpdateScore(make_pair(*(ctxt->s2w), s0cs1cs2c_action), m_nScoreIndex, amount, round);
      nReturn += cast_weights->m_mapS0cS1cS2c.getOrUpdateScore(s0cs1cs2c_action, m_nScoreIndex, amount, round);
      //if (ctxt->s2!=-1 && item->nodes[ctxt->s0].norv && item->nodes[ctxt->s1].norv && item->nodes[ctxt->s2].norv) nReturn += cast_weights->m_mapS0cmS1cmS2cm.getOrUpdateScore(make_pair(encodeAction(action, encodeTorCs(ctxt->s0cs1c, ctxt->s2c)), encodeRhythms(encodeRhythms(ctxt->s0m, ctxt->s1m), ctxt->s2m)), m_nScoreIndex, amount, round);
   }

   // Tag bigrams
//   nReturn += cast_weights->m_mapS1cS2c.getOrUpdateScore(encodeAction(action, encodeTorCs(ctxt->s1c, ctxt->s2c)), m_nScoreIndex, amount, round);
//   nReturn += cast_weights->m_mapS2cS3c.getOrUpdateScore(encodeAction(action, encodeTorCs(ctxt->s2c, ctxt->s3c)), m_nScoreIndex, amount, round);
//   nReturn += cast_weights->m_mapN1tN2t.getOrUpdateScore(encodeAction(action, encodeTorCs(ctxt->n1t, ctxt->n2t)), m_nScoreIndex, amount, round);
//   nReturn += cast_weights->m_mapN2tN3t.getOrUpdateScore(encodeAction(action, encodeTorCs(ctxt->n2t, ctxt->n3t)), m_nScoreIndex, amount, round);
   
//   nReturn += cast_weights->m_mapS0cS2c.getOrUpdateScore(encodeAction(action, encodeTorCs(ctxt->s0c, ctxt->s2c)), m_nScoreIndex, amount, round);
//   nReturn += cast_weights->m_mapS1cS3c.getOrUpdateScore(encodeAction(action, encodeTorCs(ctxt->s1c, ctxt->s3c)), m_nScoreIndex, amount, round);
//   nReturn += cast_weights->m_mapS0cN1t.getOrUpdateScore(encodeAction(action, encodeTorCs(ctxt->s0c, ctxt->n1t)), m_nScoreIndex, amount, round);
//   nReturn += cast_weights->m_mapN0tN2t.getOrUpdateScore(encodeAction(action, encodeTorCs(ctxt->n0t, ctxt->n2t)), m_nScoreIndex, amount, round);
//   nReturn += cast_weights->m_mapN1tN3t.getOrUpdateScore(encodeAction(action, encodeTorCs(ctxt->n1t, ctxt->n3t)), m_nScoreIndex, amount, round);

   // S0 S0LRUN0
//   if (ctxt->s0l!=-1) {
//      assert(ctxt->s0r!=-1);
//      if (!item->nodes[ctxt->s0].temp||!item->nodes[ctxt->s0].head_left) {
//      nReturn += cast_weights->m_mapS0cS0LcN0t.getOrUpdateScore(encodeAction(action, encodeTorCs(ctxt->s0cn0t, ctxt->s0lc)), m_nScoreIndex, amount, round);
//      nReturn += cast_weights->m_mapS0wS0LcN0t.getOrUpdateScore(make_pair(*(ctxt->s0w), encodeAction(action, encodeTorCs(ctxt->n0t, ctxt->s0lc))), m_nScoreIndex, amount, round);
//      if (ctxt->n0!=-1) nReturn += cast_weights->m_mapS0cS0LcN0w.getOrUpdateScore(make_pair(*(ctxt->n0w), encodeAction(action, encodeTorCs(ctxt->s0c, ctxt->s0lc))), m_nScoreIndex, amount, round);
//      }
//      if (!item->nodes[ctxt->s0].temp||item->nodes[ctxt->s0].head_left) {
//      nReturn += cast_weights->m_mapS0cS0RcN0t.getOrUpdateScore(encodeAction(action, encodeTorCs(ctxt->s0cn0t, ctxt->s0rc)), m_nScoreIndex, amount, round);
//      nReturn += cast_weights->m_mapS0wS0RcN0t.getOrUpdateScore(make_pair(*(ctxt->s0w), encodeAction(action, encodeTorCs(ctxt->n0t, ctxt->s0rc))), m_nScoreIndex, amount, round);
//      if (ctxt->n0!=-1) nReturn += cast_weights->m_mapS0cS0RcN0w.getOrUpdateScore(make_pair(*(ctxt->n0w), encodeAction(action, encodeTorCs(ctxt->s0c, ctxt->s0rc))), m_nScoreIndex, amount, round);
//      }
//   }
//   if (ctxt->s0u!=-1) {
//      assert(ctxt->s0l==-1);
//      assert(item->nodes[ctxt->s0].temp==false);
//      nReturn += cast_weights->m_mapS0cS0UcN0t.getOrUpdateScore(encodeAction(action, encodeTorCs(ctxt->s0cn0t, ctxt->s0uc)), m_nScoreIndex, amount, round);
//      nReturn += cast_weights->m_mapS0wS0UcN0t.getOrUpdateScore(make_pair(*(ctxt->s0w), encodeAction(action, encodeTorCs(ctxt->s0uc, ctxt->n0t))), m_nScoreIndex, amount, round);
//      if (ctxt->n0!=-1) nReturn += cast_weights->m_mapS0cS0UcN0w.getOrUpdateScore(make_pair(*(ctxt->n0w), encodeAction(action, encodeTorCs(ctxt->s0c, ctxt->s0uc))), m_nScoreIndex, amount, round);
//   }

   // S0 S0LRUS1
//   if (ctxt->s0l!=-1) {
//      if (!item->nodes[ctxt->s0].temp||!item->nodes[ctxt->s0].head_left) {
//      nReturn += cast_weights->m_mapS0cS0LcS1c.getOrUpdateScore(encodeAction(action, encodeTorCs(ctxt->s0cs1c, ctxt->s0lc)), m_nScoreIndex, amount, round);
//      nReturn += cast_weights->m_mapS0wS0LcS1c.getOrUpdateScore(make_pair(*(ctxt->s0w), encodeAction(action, encodeTorCs(ctxt->s0lc, ctxt->s1c))), m_nScoreIndex, amount, round);
//      if (ctxt->s1!=-1) nReturn += cast_weights->m_mapS0cS0LcS1w.getOrUpdateScore(make_pair(*(ctxt->s1w), encodeAction(action, encodeTorCs(ctxt->s0c, ctxt->s0lc))), m_nScoreIndex, amount, round);
//      }
//      if (!item->nodes[ctxt->s0].temp||item->nodes[ctxt->s0].head_left) {
//         nReturn += cast_weights->m_mapS0cS0RcS1c.getOrUpdateScore(encodeAction(action, encodeTorCs(ctxt->s0cs1c, ctxt->s0rc)), m_nScoreIndex, amount, round);
//      nReturn += cast_weights->m_mapS0wS0RcS1c.getOrUpdateScore(make_pair(*(ctxt->s0w), encodeAction(action, encodeTorCs(ctxt->s0rc, ctxt->s1c))), m_nScoreIndex, amount, round);
//      if (ctxt->s1!=-1) nReturn += cast_weights->m_mapS0cS0RcS1w.getOrUpdateScore(make_pair(*(ctxt->s1w), encodeAction(action, encodeTorCs(ctxt->s0c, ctxt->s0rc))), m_nScoreIndex, amount, round);
//      }
//   }
//   if (ctxt->s0u!=-1) {
//      nReturn += cast_weights->m_mapS0cS0UcS1c.getOrUpdateScore(encodeAction(action, encodeTorCs(ctxt->s0cs1c, ctxt->s0uc)), m_nScoreIndex, amount, round);
//      nReturn += cast_weights->m_mapS0wS0UcS1c.getOrUpdateScore(make_pair(*(ctxt->s0w), encodeAction(action, encodeTorCs(ctxt->s0uc, ctxt->s1c))), m_nScoreIndex, amount, round);
//      if (ctxt->s1!=-1) nReturn += cast_weights->m_mapS0cS0UcS1w.getOrUpdateScore(make_pair(*(ctxt->s1w), encodeAction(action, encodeTorCs(ctxt->s0c, ctxt->s0uc))), m_nScoreIndex, amount, round);
//   }

   // Tag trigram
   //if (ctxt->n0!=-1) {
   //   if (ctxt->n1!=-1) nReturn += cast_weights->m_mapN0tN1tN2t.getOrUpdateScore(encodeAction(action, encodeTorCs(ctxt->n0tn1t, ctxt->n2t)), m_nScoreIndex, amount, round);
   //}
//   if (ctxt->s1l!=-1) {
//      assert(ctxt->s1r!=-1);
//      assert(!item->nodes[ctxt->s1].temp||item->nodes[ctxt->s1].head_left);
//      if (!item->nodes[ctxt->s1].temp) {
//         nReturn += cast_weights->m_mapS0cS1cS1Lc.getOrUpdateScore(encodeAction(action, encodeTorCs(ctxt->s0cs1c, ctxt->s1lc)), m_nScoreIndex, amount, round);
//      nReturn += cast_weights->m_mapS0wS1cS1Lc.getOrUpdateScore(make_pair(*(ctxt->s0w), encodeAction(action, encodeTorCs(ctxt->s1c, ctxt->s1lc))), m_nScoreIndex, amount, round);
//      nReturn += cast_weights->m_mapS0cS1wS1Lc.getOrUpdateScore(make_pair(*(ctxt->s1w), encodeAction(action, encodeTorCs(ctxt->s0c, ctxt->s1lc))), m_nScoreIndex, amount, round);
//      }
//      nReturn += cast_weights->m_mapS0cS1cS1Rc.getOrUpdateScore(encodeAction(action, encodeTorCs(ctxt->s0cs1c, ctxt->s1rc)), m_nScoreIndex, amount, round);
//      nReturn += cast_weights->m_mapS0wS1cS1Rc.getOrUpdateScore(make_pair(*(ctxt->s0w), encodeAction(action, encodeTorCs(ctxt->s1c, ctxt->s1rc))), m_nScoreIndex, amount, round);
//      nReturn += cast_weights->m_mapS0cS1wS1Rc.getOrUpdateScore(make_pair(*(ctxt->s1w), encodeAction(action, encodeTorCs(ctxt->s0c, ctxt->s1rc))), m_nScoreIndex, amount, round);
//   }
//   if (ctxt->s1u!=-1) {
//      nReturn += cast_weights->m_mapS0cS1cS1Uc.getOrUpdateScore(encodeAction(action, encodeTorCs(ctxt->s0cs1c, ctxt->s1uc)), m_nScoreIndex, amount, round);
//      nReturn += cast_weights->m_mapS0wS1cS1Uc.getOrUpdateScore(make_pair(*(ctxt->s0w), encodeAction(action, encodeTorCs(ctxt->s1c, ctxt->s1uc))), m_nScoreIndex, amount, round);
//      nReturn += cast_weights->m_mapS0cS1wS1Uc.getOrUpdateScore(make_pair(*(ctxt->s1w), encodeAction(action, encodeTorCs(ctxt->s0c, ctxt->s1uc))), m_nScoreIndex, amount, round);
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

   static CStateItem st;
   static unsigned long action;
   st.clear();
   st.sent = &m_lCache;
   static CContext context;
   st.context = &context;
   static unsigned long constituent;
   static bool single_child, head_left, temporary;
   static unsigned long stacksize;

   const SCORE_TYPE amount = (update==eAdd ? 1 : -1);

   while (st != *item)  {
      // make sure 
      context.load(&st, m_lCache, m_lWordLen, true);
      stacksize = st.stack.size();
      if (stacksize>0) assert(st.stack.back()==context.s0);
      if (stacksize>1) assert(st.stack[stacksize-2]==context.s1);
      // update action
      action = st.FollowMove(*item);
      getOrUpdateStackScore(&st, action, amount, m_nTrainingRound );
      // make action
      st.Move(action);
      // update graph
      if ( !isShift(action) && !isReduceRoot(action) ) {
         // reduce
         getOrUpdateGraphScore(&st, amount, m_nTrainingRound);
         single_child = singleChild(action);
         head_left = headLeft(action);
         temporary = isTemporary(action);
         constituent = getConstituent(action);
         if ( !single_child ) {
            const CStateNode &left = st.nodes[context.s1];
            const CStateNode &right = st.nodes[context.s0];
            assert(!left.temp||!right.temp);
            assert(!left.temp||(head_left&&constituent==left.constituent));
            assert(!right.temp||(!head_left&&constituent==right.constituent));
            if (head_left) {
               const bool neighbour = context.s1_head_node==context.s1_unbinarized.back();
               getOrUpdateLinkScore( left.lexical_head, 
                                     right.lexical_head, 
                                     amount, m_nTrainingRound );
               if (context.s1rd!=-1) getOrUpdateSiblingLinkScore( left.lexical_head, 
                                                                  right.lexical_head, 
                                                                  st.nodes[st.context->s1rd].lexical_head, 
                                                                  amount, m_nTrainingRound );
               getOrUpdateArityScore( right.lexical_head, 
                                      context.s0ln, 
                                      context.s0rn, 
                                      amount, m_nTrainingRound);
               if (!left.temp) getOrUpdateHeadScore( constituent, 
                                                     context.s1c, 
                                                     left.lexical_head, 
                                                     amount, m_nTrainingRound );
               getOrUpdateHeadModifierScore( constituent, 
                                             context.s1_unbinarized_cs[context.s1_head_index], 
                                             context.s0c, 
                                             left.lexical_head, right.lexical_head,
                                             true, 
                                             neighbour, 
                                             amount, m_nTrainingRound );
               if (!neighbour) getOrUpdateBigramScore( constituent, 
                                                       context.s1_unbinarized_cs.back(), 
                                                       context.s0c, 
                                                       true, 
                                                       amount, m_nTrainingRound );
            }
            else {
               const bool neighbour = context.s0_head_node==context.s0_unbinarized[0];
               getOrUpdateLinkScore( right.lexical_head, 
                                     left.lexical_head, 
                                     amount, 
                                     m_nTrainingRound );
               if (context.s0ld!=-1) getOrUpdateSiblingLinkScore( right.lexical_head, 
                                                                  left.lexical_head, 
                                                                  st.nodes[st.context->s0ld].lexical_head, 
                                                                  amount, m_nTrainingRound );
               getOrUpdateArityScore( left.lexical_head, 
                                      context.s1ln, 
                                      context.s1rn, 
                                      amount, m_nTrainingRound);
               if (!right.temp)getOrUpdateHeadScore( constituent, 
                                                     context.s0c, 
                                                     right.lexical_head, 
                                                     amount, m_nTrainingRound );
               getOrUpdateHeadModifierScore( constituent, 
                                             context.s0_unbinarized_cs[context.s0_head_index], 
                                             context.s1c, 
                                             right.lexical_head, left.lexical_head,
                                             false, 
                                             neighbour, 
                                             amount, m_nTrainingRound );
               if (!neighbour) getOrUpdateBigramScore( constituent, 
                                                       context.s0_unbinarized_cs[0], 
                                                       context.s1c, 
                                                       false, 
                                                       amount, m_nTrainingRound);
            }
         }
         else {
            const CStateNode &child = st.nodes[context.s0]; 
            getOrUpdateHeadScore( constituent, 
                                  context.s0c, 
                                  child.lexical_head, 
                                  amount, m_nTrainingRound);
         }
      }
      if ( isReduceRoot(action) ) {
         // finish
         const CStateNode &root = st.nodes[context.s0];
         getOrUpdateArityScore( root.lexical_head, 
                                context.s0ln, 
                                context.s0rn, 
                                amount, m_nTrainingRound);
      }
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
 * shift - shift action
 *
 * Input: the generator item
 *
 *--------------------------------------------------------------*/

void CConParser::shift(CStateItem &st) {
   if (st.stack.size()>0&&st.nodes[st.stack.back()].temp&&st.nodes[st.stack.back()].head_left==false)
      return;
   // try the shift action
   static SCORE_TYPE original_score;
   static int unary_reduce;
   original_score = st.score;
   unary_reduce = st.unary_reduce;
   st.score += getOrUpdateStackScore(&st, encodeShift());
   st.shift();
   m_Agenda->pushCandidate(&st);
   st.unshift(original_score, unary_reduce);
}

/*---------------------------------------------------------------
 *
 * reduce - reduce action
 *
 * Input: the generator item
 *
 *--------------------------------------------------------------*/

void CConParser::reduce(CStateItem &st) {
   static unsigned long constituent;
   static unsigned long i, j;
   //static CStateItem st;
   //st = *item;
   static SCORE_TYPE original_score;
   static int unary_reduce;
   original_score = st.score;
   unary_reduce = st.unary_reduce;
   const unsigned long &stacksize = st.stack.size();
   const bool prev_temp = stacksize>2 ? st.nodes[st.stack[stacksize-3]].temp:false;
   for (constituent=PENN_CON_FIRST; constituent<PENN_CON_COUNT; constituent++) {
      for (i=0; i<=1; ++i) {
         for (j=0; j<=1; ++j) {
            const bool &head_left = static_cast<bool>(i);
            const bool &temporary = static_cast<bool>(j);
            const CStateNode &right = st.nodes[st.stack.back()];
            const CStateNode  &left = st.nodes[st.stack[stacksize-2]];
            assert( st.stack.back() == st.context->s0 );
            assert( st.stack[stacksize-2] == st.context ->s1 );
            const CWord &head_wd = m_lCache[ (head_left?left:right).lexical_head ];
            if ( ( !left.temp || !right.temp ) &&
                 ( !(stacksize==2&&st.current_word==m_lCache.size()) || !temporary ) &&
                 ( !(stacksize==2) || (!temporary||head_left) ) &&
                 ( !(prev_temp&&st.current_word==m_lCache.size()) || !temporary ) &&
                 ( !(prev_temp) || (!temporary||head_left) ) &&
                 ( !left.temp || (head_left&&constituent==left.constituent) ) &&
                 ( !right.temp || (!head_left&&constituent==right.constituent) ) &&
                 ( !temporary || PENN_CON_TEMP[constituent] ) 
               ) {
               st.score += getOrUpdateStackScore(&st, encodeReduce(constituent, false, head_left, temporary));
               st.reduce(constituent, false, head_left, temporary);
               if (head_left) {
                  assert(st.context->s0_unbinarized.size()==1 && st.context->s0==st.context->s0_unbinarized[0]);
                  const bool neighbour = st.context->s1_head_node==st.context->s1_unbinarized.back();
                  st.score += getOrUpdateLinkScore( left.lexical_head, 
                                                    right.lexical_head );
                  if (st.context->s1rd!=-1) 
                     st.score += getOrUpdateSiblingLinkScore( left.lexical_head, 
                                                              right.lexical_head, 
                                                              st.nodes[st.context->s1rd].lexical_head );
                  st.score += getOrUpdateArityScore( right.lexical_head, 
                                                     st.context->s0ln, 
                                                     st.context->s0rn );
                  if (!left.temp) 
                     st.score += getOrUpdateHeadScore( constituent, 
                                                       st.context->s1c, 
                                                       left.lexical_head );
                  st.score += getOrUpdateHeadModifierScore( constituent, 
                                                            st.context->s1_unbinarized_cs[st.context->s1_head_index], 
                                                            st.context->s0c, 
                                                            left.lexical_head, right.lexical_head,
                                                            true, 
                                                            neighbour );
                  if (!neighbour) 
                     st.score += getOrUpdateBigramScore( constituent, 
                                                         st.context->s1_unbinarized_cs.back(), 
                                                         st.context->s0c, 
                                                         true );
               }
               else { 
                  assert(st.context->s1_unbinarized.size()==1 && st.context->s1==st.context->s1_unbinarized[0]);
                  const bool neighbour = st.context->s0_head_node==st.context->s0_unbinarized[0];
                  st.score += getOrUpdateLinkScore( right.lexical_head, 
                                                    left.lexical_head );
                  if (st.context->s0ld!=-1) 
                     st.score += getOrUpdateSiblingLinkScore( right.lexical_head, 
                                                              left.lexical_head, 
                                                              st.nodes[st.context->s0ld].lexical_head );
                  st.score += getOrUpdateArityScore( left.lexical_head, 
                                                     st.context->s1ln, 
                                                     st.context->s1rn );
                  if (!right.temp) 
                     st.score += getOrUpdateHeadScore( constituent, 
                                                       st.context->s0c, 
                                                       right.lexical_head );
                  st.score += getOrUpdateHeadModifierScore( constituent, 
                                                            st.context->s0_unbinarized_cs[st.context->s0_head_index], 
                                                            st.context->s1c, 
                                                            right.lexical_head, left.lexical_head,
                                                            false, 
                                                            neighbour );
                  if (!neighbour) 
                     st.score += getOrUpdateBigramScore( constituent, 
                                                         st.context->s0_unbinarized_cs[0], 
                                                         st.context->s1c, 
                                                         false );
               }
               st.score += getOrUpdateGraphScore(&st);
               m_Agenda->pushCandidate(&st);
               st.unreduce(original_score, unary_reduce);
            }
         }
      }
   }
}

/*---------------------------------------------------------------
 *
 * reduce_unary - reduce action
 *
 * Input: the generator item
 *
 *--------------------------------------------------------------*/

void CConParser::reduce_unary(CStateItem &st) {
   // reduce unary can't be with a temp node
   if (st.nodes[st.stack.back()].temp) return;
   static SCORE_TYPE original_score;
   static int unary_reduce;
   original_score = st.score;
   unary_reduce = st.unary_reduce;
   static CCFGSet cf;
   for (unsigned long constituent=PENN_CON_FIRST; constituent<PENN_CON_COUNT; ++constituent){
      const CStateNode &child = st.nodes[st.stack.back()];
      const CWord &hw = m_lCache[child.lexical_head];
      assert(st.context->s0==st.stack.back());
      if (constituent != child.constituent
         ) { 
         st.score += getOrUpdateStackScore(&st, encodeReduce(constituent, true, false, false));
         st.reduce(constituent, true, false, false);
         st.score += getOrUpdateHeadScore( constituent, 
                                           st.context->s0c, 
                                           child.lexical_head );
         st.score += getOrUpdateGraphScore(&st);
         m_Agenda->pushCandidate(&st);
         st.unreduce(original_score, unary_reduce);
      }
   }
}

/*---------------------------------------------------------------
 *
 * terminate - termination action
 *
 * Input: the generator item
 *
 *--------------------------------------------------------------*/

void CConParser::terminate(CStateItem &st) {
   //static CStateItem st;
   //st = *item;
   static SCORE_TYPE original_score;
   static int unary_reduce;
   original_score = st.score;
   unary_reduce = st.unary_reduce;
   assert(unary_reduce!=-1);
   assert(st.IsComplete());
   st.score += getOrUpdateStackScore(&st, encodeReduceRoot());
   st.terminate();
   st.score += getOrUpdateArityScore( st.nodes[st.context->s0].lexical_head, 
                                      st.context->s0ln, 
                                      st.context->s0rn );
   m_Agenda->pushCandidate(&st);
   st.unterminate(original_score, unary_reduce);
}

/*---------------------------------------------------------------
 *
 * work - the working process shared by training and parsing
 *
 * Returns: makes a new instance of CSentenceParsed 
 *
 *--------------------------------------------------------------*/

void CConParser::work( const bool bTrain , const CTwoStringVector &sentence , CSentenceParsed *retval , const CSentenceParsed &correct , int nBest , SCORE_TYPE *scores ) {

#ifdef DEBUG
   clock_t total_start_time = clock();
#endif
   const int length = sentence.size() ; 

   const static CStateItem *pGenerator ;
   static CStateItem oCandidate ;
   static bool bCorrect ;  // used in learning for early update
   static CStateItem correctState ;
   static unsigned long stack_size;
   static int x;
   const static CStateItem *pBestGen;
   static CContext context;
   static bool bParsingDone;

   assert(length<MAX_SENTENCE_SIZE);

   TRACE("Initialising the decoding process ... ") ;
   // initialise word cache
   m_lCache.clear();
   m_lWordLen.clear();
   for ( x=0; x<length; x++ ) {
      m_lCache.push_back( CTaggedWord<CTag, TAG_SEPARATOR>(sentence[x].first , sentence[x].second) );
      m_lWordLen.push_back( normalize3(getUTF8StringLength(sentence[x].first)) );
   }
   // initialise agenda
   m_Agenda->clear();
   oCandidate.clear();                             // restore state using clean
   oCandidate.sent = &m_lCache;
   m_Agenda->pushCandidate(&oCandidate);           // and push it back
   m_Agenda->nextRound();                       // as the generator item
   if (bTrain) { 
      correctState.clear();
      correctState.sent = &m_lCache;
   }

   TRACE("Decoding start ... ") ;
   while (true) { // for each step

      if (bTrain) bCorrect = false ; 

      pGenerator = m_Agenda->generatorStart();
      pBestGen = m_Agenda->bestGenerator();

      if (pBestGen->IsTerminated()) break; // if the first item is complete
      bParsingDone = false;
      for (x=0; x<m_Agenda->generatorSize(); ++x) {
         if (m_Agenda->generator(x)->score == pBestGen->score && 
               m_Agenda->generator(x)->IsTerminated()) {
             pBestGen = m_Agenda->generator(x);
             bParsingDone = true; break;
         }
      }
      if (bParsingDone) break;
         
      for (x=0; x<m_Agenda->generatorSize(); ++x) { // for each generator

         if (pGenerator->IsTerminated()) { // don't do anything for completed items
            m_Agenda->pushCandidate(pGenerator); // push it back intacit
         }
         else {
            oCandidate = *pGenerator;
            context.load(&oCandidate, m_lCache, m_lWordLen, false);
            oCandidate.context = &context;
            if (pGenerator->IsComplete()) { // finish parsing
               terminate(oCandidate);
            }
            if ( pGenerator->current_word < length ) { // some words hvnt processed
               shift(oCandidate);
            }
            stack_size = pGenerator->stack.size();
            if ( stack_size > 1 ) {
               reduce(oCandidate);
            }
            if ( stack_size && pGenerator->unary_reduce<UNARY_MOVES ) {
                  reduce_unary(oCandidate);
            }
         }
   
         if (bTrain && *pGenerator == correctState) { // compare generator to corr
            bCorrect = true ;
         }

         pGenerator = m_Agenda->generatorNext() ; // next loop
      } // done iterating generator item

      if (bTrain) { // update items if correct item jump out of the agenda
#ifdef EARLY_UPDATE
         if (!bCorrect) {
            correctState.trace(&sentence);
            pBestGen->trace(&sentence);
            TRACE("Error at the "<<correctState.current_word<<"th word; total is "<<correct.words.size())
            updateScoresForStates(pBestGen, &correctState) ; 
            return ;
         }
#endif
         if (!correctState.IsTerminated())
            correctState.Move(correctState.StandardMove(correct));
         assert (correctState.unary_reduce<=UNARY_MOVES) ;
      }  // end of update

      m_Agenda->nextRound(); // move round
   }

   if (bTrain) {
      // make sure that the correct item is stack top finally
      if ( *(pBestGen) != correctState ) {
         correctState.trace(&sentence);
         pBestGen->trace(&sentence);
         TRACE("The best item is not the correct one")
         updateScoresForStates(pBestGen, &correctState) ; 
         return ;
      }
   } 

   if (!retval) 
      return;

   TRACE("Outputing sentence");
   pBestGen->GenerateTree( sentence, retval[0] );
   if (scores) scores[0] = pBestGen->score;
/*
   m_Agenda->sortGenerators();
   for (int i=0; i<nBest; ++i) {
      retval[i].clear();
      if (scores) scores[i] = 0; //pGenerator->score();
      pGenerator = m_Agenda->generator(i) ; 
      if (pGenerator) {
         pGenerator->GenerateTree( sentence , retval[i] ) ; 
         if (scores) scores[i] = pGenerator->score;
      }
   }
*/
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

void CConParser::parse( const CTwoStringVector &sentence , CSentenceParsed *retval , int nBest , SCORE_TYPE *scores ) {

   static CSentenceParsed empty ;

   work(false, sentence, retval, empty, nBest, scores ) ;

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

//   cerr << output;

};


