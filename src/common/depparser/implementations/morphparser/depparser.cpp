// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                              *
 * depparser.cpp - the action based parser implementation       *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2007.12                        *
 *                                                              *
 ****************************************************************/

#include "depparser.h"
#include "depparser_weight.h"

using namespace TARGET_LANGUAGE;
using namespace TARGET_LANGUAGE::depparser;

const CWord g_emptyWord("");
const CTaggedWord<CTag, TAG_SEPARATOR> g_emptyTaggedWord;
const CTag g_noneTag = CTag::NONE;

#define cast_weights static_cast<CWeight*>(m_weights)
#define refer_or_allocate_tuple2(x, o1, o2) { if (amount == 0) x.refer(o1, o2); else x.allocate(o1, o2); }
#define refer_or_allocate_tuple3(x, o1, o2, o3) { if (amount == 0) x.refer(o1, o2, o3); else x.allocate(o1, o2, o3); }
#define refer_or_allocate_tuple4(x, o1, o2, o3, o4) { if (amount == 0) x.refer(o1, o2, o3, o4); else x.allocate(o1, o2, o3, o4); }
#define _conll_or_empty(x) (x == "_" ? "" : x)


/*===============================================================
 *
 * CDepParser - the depparser for TARGET_LANGUAGE
 *
 *==============================================================*/

/*---------------------------------------------------------------
 *
 * getOrUpdateStackScore - manipulate the score from stack
 *
 *---------------------------------------------------------------*/

inline void CDepParser::getOrUpdateStackScore( const CStateItem *item, CPackedScoreType<SCORE_TYPE, action::MAX> &retval, const unsigned long &action, SCORE_TYPE amount , int round ) {

   const int &st_index = item->stackempty() ? -1 : item->stacktop(); // stack top
   const int &st1_index = item->stacksize() < 2 ? -1 : item->stacksecond(); //stack second
   const int &sth_index = st_index == -1 ? -1 : item->head(st_index); // stack top head
   const int &sthh_index = sth_index == -1 ? -1 : item->head(sth_index); // stack top head
   const int &stld_index = st_index == -1 ? -1 : item->leftdep(st_index); // leftmost dep of stack
   const int &strd_index = st_index == -1 ? -1 : item->rightdep(st_index); // rightmost dep st
   const int &stl2d_index = stld_index == -1 ? -1 : item->sibling(stld_index); // left 2ndmost dep of stack
   const int &str2d_index = strd_index == -1 ? -1 : item->sibling(strd_index); // right 2ndmost dep st
   const int &n0_index = item->size()==m_lCache.size() ? -1 : item->size(); // next in cache
   const int &b0_index = (item->size()+item->cachesize()) < m_lCache.size() ? (item->size()+item->cachesize()) : -1; //next in buffer (first word without morph analysis yet)
   assert(n0_index<static_cast<int>(m_lCache.size())); // the next index shouldn't exceed sentence
   const int &n0ld_index = n0_index==-1 ? -1 : item->leftdep(n0_index); // leftmost dep of next
   const int &n0l2d_index = n0ld_index==-1 ? -1 : item->sibling(n0ld_index); // leftmost dep of next
   const int &ht_index = item->headstackempty() ? -1 : item->headstacktop(); // headstack
   const int &ht2_index = item->headstacksize()<2 ? -1 : item->headstackitem(item->headstacksize()-2); // headstack 2nd
   static int n1_index;
   static int n2_index;
   static int n3_index;
   static int b1_index;
   static int b2_index;
   static int b3_index;
   n1_index = (n0_index != -1 && n0_index+1<m_lCache.size()) ? n0_index+1 : -1 ; //cache 1
   n2_index = (n0_index != -1 && n0_index+2<m_lCache.size()) ? n0_index+2 : -1 ; //cache 2
   n3_index = (n0_index != -1 && n0_index+3<m_lCache.size()) ? n0_index+3 : -1 ; //cache 3
   b1_index = (b0_index != -1 && b0_index+1<m_lCache.size()) ? b0_index+1 : -1 ; //buffer 1
   b2_index = (b0_index != -1 && b0_index+2<m_lCache.size()) ? b0_index+2 : -1 ; //buffer 2
   b3_index = (b0_index != -1 && b0_index+3<m_lCache.size()) ? b0_index+3 : -1 ; //buffer 3
   //buffer -x features
   static int bm1_index;
   static int bm2_index;
   static int bm3_index;
   bm1_index = (b0_index != -1 && b0_index-1>=0) ? b0_index-1 : -1 ; //buffer -1
   bm2_index = (b0_index != -1 && b0_index-2>=0) ? b0_index-2 : -1 ; //buffer -2
   bm3_index = (b0_index != -1 && b0_index-3>=0) ? b0_index-3 : -1 ; //buffer -3
   const int &bm2h_index = (bm2_index == -1 || bm2_index > item->size()) ? -1 : item->head(bm2_index); // buffer minus 2 head
   const int &bm3h_index = (bm3_index == -1 || bm3_index > item->size()) ? -1 : item->head(bm3_index); // buffer minus 3 head

   //cache -x features
   static int nm1_index;
   static int nm2_index;
   static int nm3_index;
   nm1_index = (n0_index != -1 && n0_index-1>=0) ? n0_index-1 : -1 ; //cache -1
   nm2_index = (n0_index != -1 && n0_index-2>=0) ? n0_index-2 : -1 ; //cache -2
   nm3_index = (n0_index != -1 && n0_index-3>=0) ? n0_index-3 : -1 ; //cache -3

   //version without lemmas as words:
/*
   const CWord &st_word = st_index == -1 ? g_emptyWord : m_lCache[st_index];
   const CWord &sth_word = sth_index==-1 ? g_emptyWord : m_lCache[sth_index];
   const CWord &sthh_word = sthh_index==-1 ? g_emptyWord : m_lCache[sthh_index];
   const CWord &stld_word = stld_index==-1 ? g_emptyWord : m_lCache[stld_index];
   const CWord &strd_word = strd_index==-1 ? g_emptyWord : m_lCache[strd_index];
   const CWord &stl2d_word = stl2d_index==-1 ? g_emptyWord : m_lCache[stl2d_index];
   const CWord &str2d_word = str2d_index==-1 ? g_emptyWord : m_lCache[str2d_index];
   const CWord &n0_word = n0_index==-1 ? g_emptyWord : m_lCache[n0_index];
   const CWord &n0ld_word = n0ld_index==-1 ? g_emptyWord : m_lCache[n0ld_index];
   const CWord &n0l2d_word = n0l2d_index==-1 ? g_emptyWord : m_lCache[n0l2d_index];
   const CWord &n1_word = n1_index==-1 ? g_emptyWord : m_lCache[n1_index];
   const CWord &n2_word = n2_index==-1 ? g_emptyWord : m_lCache[n2_index];
   const CWord &ht_word = ht_index==-1 ? g_emptyWord : m_lCache[ht_index];
   const CWord &ht2_word = ht2_index==-1 ? g_emptyWord : m_lCache[ht2_index];
*/

   //version with lemmas as words:
   //TODO This probably can be optimized by using class CLemma directly and avoiding the conversion
   const CWord &st_word = st_index == -1 ? g_emptyWord : item->lemmaascword(st_index);
   const CWord &st1_word = st1_index == -1 ? g_emptyWord : item->lemmaascword(st1_index);
   const CWord &sth_word = sth_index==-1 ? g_emptyWord : item->lemmaascword(sth_index);
   const CWord &sthh_word = sthh_index==-1 ? g_emptyWord : item->lemmaascword(sthh_index);
   const CWord &stld_word = stld_index==-1 ? g_emptyWord : item->lemmaascword(stld_index);
   const CWord &strd_word = strd_index==-1 ? g_emptyWord : item->lemmaascword(strd_index);
   const CWord &stl2d_word = stl2d_index==-1 ? g_emptyWord : item->lemmaascword(stl2d_index);
   const CWord &str2d_word = str2d_index==-1 ? g_emptyWord : item->lemmaascword(str2d_index);
   const CWord &n0ld_word = n0ld_index==-1 ? g_emptyWord : item->lemmaascword(n0ld_index);
   const CWord &n0l2d_word = n0l2d_index==-1 ? g_emptyWord : item->lemmaascword(n0l2d_index);
   const CWord &ht_word = ht_index==-1 ? g_emptyWord : item->lemmaascword(ht_index);
   const CWord &ht2_word = ht2_index==-1 ? g_emptyWord : item->lemmaascword(ht2_index);
   //these words may or may not have been lemmatized already, depending on cache size.
   //TODO alternative: if they haven't, use forms (m_lCache[n1_index], etc.) instead of the empty word.
   const CWord &n0_word = n0_index==-1 ? g_emptyWord : item->cachesize() > 0 ? item->lemmaascword(n0_index) : g_emptyWord;
   const CWord &n1_word = n1_index==-1 ? g_emptyWord : item->cachesize() > 1 ? item->lemmaascword(n1_index) : g_emptyWord;
   const CWord &n2_word = n2_index==-1 ? g_emptyWord : item->cachesize() > 2 ? item->lemmaascword(n2_index) : g_emptyWord;

   const CWord &b0_word = b0_index==-1 ? g_emptyWord : m_lCache[b0_index];
   const CWord &b1_word = b1_index==-1 ? g_emptyWord : m_lCache[b1_index];
   const CWord &b2_word = b2_index==-1 ? g_emptyWord : m_lCache[b2_index];

   const CWord &bm1_word = bm1_index==-1 ? g_emptyWord : item->lemmaascword(bm1_index);
   const CWord &bm2_word = bm2_index==-1 ? g_emptyWord : item->lemmaascword(bm2_index);
   const CWord &bm3_word = bm3_index==-1 ? g_emptyWord : item->lemmaascword(bm3_index);

   const CWord &nm1_word = nm1_index==-1 ? g_emptyWord : item->lemmaascword(nm1_index);
   const CWord &nm2_word = nm2_index==-1 ? g_emptyWord : item->lemmaascword(nm2_index);
   const CWord &nm3_word = nm3_index==-1 ? g_emptyWord : item->lemmaascword(nm3_index);

   const CWord &bm2h_word = bm2h_index==-1 ? g_emptyWord : item->lemmaascword(bm2h_index);
   const CWord &bm3h_word = bm3h_index==-1 ? g_emptyWord : item->lemmaascword(bm3h_index);

   const CWord &b0_pref1 = b0_index==-1 ? g_emptyWord : m_lCacheP1[b0_index];
   const CWord &b0_pref2 = b0_index==-1 ? g_emptyWord : m_lCacheP2[b0_index];
   const CWord &b0_pref3 = b0_index==-1 ? g_emptyWord : m_lCacheP3[b0_index];
   const CWord &b0_pref4 = b0_index==-1 ? g_emptyWord : m_lCacheP4[b0_index];

   const CWord &b0_suff1 = b0_index==-1 ? g_emptyWord : m_lCacheS1[b0_index];
   const CWord &b0_suff2 = b0_index==-1 ? g_emptyWord : m_lCacheS2[b0_index];
   const CWord &b0_suff3 = b0_index==-1 ? g_emptyWord : m_lCacheS3[b0_index];
   const CWord &b0_suff4 = b0_index==-1 ? g_emptyWord : m_lCacheS4[b0_index];

   const CWord &b1_pref1 = b1_index==-1 ? g_emptyWord : m_lCacheP1[b1_index];
   const CWord &b1_pref2 = b1_index==-1 ? g_emptyWord : m_lCacheP1[b1_index];
   const CWord &b1_pref3 = b1_index==-1 ? g_emptyWord : m_lCacheP1[b1_index];
   const CWord &b1_pref4 = b1_index==-1 ? g_emptyWord : m_lCacheP1[b1_index];

   const CWord &b1_suff1 = b1_index==-1 ? g_emptyWord : m_lCacheS1[b1_index];
   const CWord &b1_suff2 = b1_index==-1 ? g_emptyWord : m_lCacheS1[b1_index];
   const CWord &b1_suff3 = b1_index==-1 ? g_emptyWord : m_lCacheS1[b1_index];
   const CWord &b1_suff4 = b1_index==-1 ? g_emptyWord : m_lCacheS1[b1_index];

   const bool &b0_hasDigit = b0_index==-1 ? false : m_lCacheHasDigit[b0_index];
   const bool &b0_hasUpper = b0_index==-1 ? false : m_lCacheHasUpper[b0_index];
   const bool &b0_hasHyphen = b0_index==-1 ? false : m_lCacheHasHyphen[b0_index];

   const int &st_label = st_index==-1 ? CDependencyLabel::NONE : item->label(st_index);
   const int &sth_label = sth_index==-1 ? CDependencyLabel::NONE : item->label(sth_index);
   const int &stld_label = stld_index==-1 ? CDependencyLabel::NONE : item->label(stld_index);
   const int &strd_label = strd_index==-1 ? CDependencyLabel::NONE : item->label(strd_index);
   const int &stl2d_label = stl2d_index==-1 ? CDependencyLabel::NONE : item->label(stl2d_index);
   const int &str2d_label = str2d_index==-1 ? CDependencyLabel::NONE : item->label(strd_index); //bug location
   const int &n0ld_label = n0ld_index==-1 ? CDependencyLabel::NONE : item->label(n0ld_index);
   const int &n0l2d_label = n0l2d_index==-1 ? CDependencyLabel::NONE : item->label(n0l2d_index);


   const CMorph &st_morph = st_index==-1 ? CMorph::NONE : item->morph(st_index);
   const CMorph &st1_morph = st_index==-1 ? CMorph::NONE : item->morph(st1_index);
   const CMorph &sth_morph = sth_index==-1 ? CMorph::NONE : item->morph(sth_index);
   const CMorph &sthh_morph = sth_index==-1 ? CMorph::NONE : item->morph(sthh_index);
   const CMorph &stld_morph = stld_index==-1 ? CMorph::NONE : item->morph(stld_index);
   const CMorph &strd_morph = strd_index==-1 ? CMorph::NONE : item->morph(strd_index);
   const CMorph &stl2d_morph = stl2d_index==-1 ? CMorph::NONE : item->morph(stl2d_index);
   const CMorph &str2d_morph = str2d_index==-1 ? CMorph::NONE : item->morph(str2d_index);
   const CMorph &n0ld_morph = n0ld_index==-1 ? CMorph::NONE : item->morph(n0ld_index);
   const CMorph &n0l2d_morph = n0l2d_index==-1 ? CMorph::NONE : item->morph(n0l2d_index);
   //these words may or may not have been lemmatized already, depending on cache size.
   const CMorph &n0_morph = (n0_index==-1 || item->cachesize()<=0) ? CMorph::NONE : item->morph(n0_index);
   const CMorph &n1_morph = (n1_index==-1 || item->cachesize()<=1) ? CMorph::NONE : item->morph(n1_index);
   const CMorph &n2_morph = (n2_index==-1 || item->cachesize()<=2) ? CMorph::NONE : item->morph(n2_index);

   const CMorph &bm1_morph = (bm1_index==-1) ? CMorph::NONE : item->morph(bm1_index);
   const CMorph &bm2_morph = (bm2_index==-1) ? CMorph::NONE : item->morph(bm2_index);
   const CMorph &bm3_morph = (bm3_index==-1) ? CMorph::NONE : item->morph(bm3_index);

   const CMorph &nm1_morph = (nm1_index==-1) ? CMorph::NONE : item->morph(nm1_index);
   const CMorph &nm2_morph = (nm2_index==-1) ? CMorph::NONE : item->morph(nm2_index);
   const CMorph &nm3_morph = (nm3_index==-1) ? CMorph::NONE : item->morph(nm3_index);

   const CMorph &bm2h_morph = (bm2h_index==-1) ? CMorph::NONE : item->morph(bm2h_index);
   const CMorph &bm3h_morph = (bm3h_index==-1) ? CMorph::NONE : item->morph(bm3h_index);

   const int &n0_mpostag = (int) n0_morph.getField(MORPH_POSTAG);
   const int &n1_mpostag = (int) n1_morph.getField(MORPH_POSTAG);
   const int &st_mpostag = (int) st_morph.getField(MORPH_POSTAG);
   const int &st1_mpostag = (int) st1_morph.getField(MORPH_POSTAG);

   const int &n0_number = (int) n0_morph.getField(MORPH_NUMBER);
   const int &n1_number = (int) n1_morph.getField(MORPH_NUMBER);
   const int &st_number = (int) st_morph.getField(MORPH_NUMBER);
   const int &st1_number = (int) st1_morph.getField(MORPH_NUMBER);

   const int &n0_person = (int) n0_morph.getField(MORPH_PERSON);
   const int &n1_person = (int) n1_morph.getField(MORPH_PERSON);
   const int &st_person = (int) st_morph.getField(MORPH_PERSON);
   const int &st1_person = (int) st1_morph.getField(MORPH_PERSON);

   const int &n0_vtype = (int) n0_morph.getField(MORPH_VERB_TYPE);
   const int &n1_vtype = (int) n1_morph.getField(MORPH_VERB_TYPE);
   const int &st_vtype = (int) st_morph.getField(MORPH_VERB_TYPE);
   const int &st1_vtype = (int) st1_morph.getField(MORPH_VERB_TYPE);

   const int &n0_vform = (int) n0_morph.getField(MORPH_VERB_FORM);
   const int &n1_vform = (int) n1_morph.getField(MORPH_VERB_FORM);
   const int &st_vform = (int) st_morph.getField(MORPH_VERB_FORM);
   const int &st1_vform = (int) st1_morph.getField(MORPH_VERB_FORM);

   static int st_n0_dist;
   st_n0_dist = encodeLinkDistance(st_index, n0_index);

   const int st_rarity = st_index==-1?0:item->rightarity(st_index);
   const int st_larity = st_index==-1?0:item->leftarity(st_index);
   const int n0_larity = n0_index==-1?0:item->leftarity(n0_index);

   const CSetOfTags<CDependencyLabel> &st_rtagset = st_index==-1?CSetOfTags<CDependencyLabel>():item->righttagset(st_index);
   const CSetOfTags<CDependencyLabel> &st_ltagset = st_index==-1?CSetOfTags<CDependencyLabel>():item->lefttagset(st_index);
   const CSetOfTags<CDependencyLabel> &n0_ltagset = n0_index==-1?CSetOfTags<CDependencyLabel>():item->lefttagset(n0_index);

   //static CTwoTaggedWords st_word_tag_n0_word_tag ;
   static CTwoWords st_word_n0_word ;
   if ( amount == 0 ) {
      //st_word_tag_n0_word_tag.refer( &st_word_tag, &n0_word_tag );
      st_word_n0_word.refer( &st_word, &n0_word );
   }
   else {
      //st_word_tag_n0_word_tag.allocate( st_word_tag, n0_word_tag );
      st_word_n0_word.allocate( st_word, n0_word );
   }

   static CTuple2<CWord, CTag> word_tag;
   static CTuple2<CWord, int> word_int;
   static CTuple2<CTag, int> tag_int;
   static CTuple3<CWord, CTag, CTag> word_tag_tag;
   static CTuple3<CWord, CWord, CTag> word_word_tag;
   static CTuple3<CWord, CWord, int> word_word_int;
   static CTuple3<CTag, CTag, int> tag_tag_int;
   static CTuple2<CWord, CSetOfTags<CDependencyLabel> > word_tagset;
   static CTuple2<CTag, CSetOfTags<CDependencyLabel> > tag_tagset;

   static CTuple2<CWord, CMorph> word_morph;
   static CTuple2<CMorph, CMorph> morph_morph;
   static CTuple3<CWord, CMorph, CMorph> word_morph_morph;
   static CTuple3<CWord, CWord, CMorph> word_word_morph;
   static CTuple4<CWord, CWord, CMorph, CMorph> word_word_morph_morph;

   static CTuple2<CWord, CWord> word_word;

   static CTuple3<CMorph, CMorph, CMorph> morph_morph_morph;

   static CTuple2<CMorph, int> morph_int;
   static CTuple3<CMorph, CMorph, int> morph_morph_int;
   static CTuple2<int, int> int_int;

   static CTuple2<CMorph, CSetOfTags<CDependencyLabel> > morph_tagset;

   //Modify together with depparser_weight.h.

   //TODO:
   //Different features for tense, number, etc.

   // single
   if (st_index != -1) {
      cast_weights->m_mapSTw.getOrUpdateScore( retval, st_word, action, m_nScoreIndex, amount, round) ;
      cast_weights->m_mapSTm.getOrUpdateScore( retval, st_morph, action, m_nScoreIndex, amount, round) ;
      refer_or_allocate_tuple2(word_morph, &st_word, &st_morph);
      cast_weights->m_mapSTwm.getOrUpdateScore( retval, word_morph , action, m_nScoreIndex, amount, round ) ;
   }

   if (n0_index != -1) {
      cast_weights->m_mapN0w.getOrUpdateScore( retval, n0_word, action, m_nScoreIndex, amount, round ) ;
      cast_weights->m_mapN0m.getOrUpdateScore( retval, n0_morph, action, m_nScoreIndex, amount, round) ;
      refer_or_allocate_tuple2(word_morph, &n0_word, &n0_morph);
      cast_weights->m_mapN0wm.getOrUpdateScore( retval, word_morph , action, m_nScoreIndex, amount, round ) ;
   }

   if (n1_index != -1) {
      cast_weights->m_mapN1w.getOrUpdateScore( retval, n1_word, action, m_nScoreIndex, amount, round ) ;
      cast_weights->m_mapN1m.getOrUpdateScore( retval, n1_morph, action, m_nScoreIndex, amount, round) ;
      refer_or_allocate_tuple2(word_morph, &n1_word, &n1_morph);
      cast_weights->m_mapN1wm.getOrUpdateScore( retval, word_morph , action, m_nScoreIndex, amount, round ) ;
   }

   if (n2_index != -1) {
      cast_weights->m_mapN2w.getOrUpdateScore( retval, n2_word, action, m_nScoreIndex, amount, round ) ;
      cast_weights->m_mapN2m.getOrUpdateScore( retval, n2_morph, action, m_nScoreIndex, amount, round) ;
      refer_or_allocate_tuple2(word_morph, &n2_word, &n2_morph);
      cast_weights->m_mapN2wm.getOrUpdateScore( retval, word_morph , action, m_nScoreIndex, amount, round ) ;
   }

   if (sth_index != -1) {
      cast_weights->m_mapSTHw.getOrUpdateScore( retval, sth_word, action, m_nScoreIndex, amount, round) ;
      cast_weights->m_mapSTHm.getOrUpdateScore( retval, sth_morph, action, m_nScoreIndex, amount, round ) ;
      cast_weights->m_mapSTi.getOrUpdateScore( retval, st_label, action, m_nScoreIndex, amount, round) ;
   }

   if (sthh_index != -1) {
      cast_weights->m_mapSTHHw.getOrUpdateScore( retval, sthh_word, action, m_nScoreIndex, amount, round) ;
      cast_weights->m_mapSTHHm.getOrUpdateScore( retval, sthh_morph, action, m_nScoreIndex, amount, round ) ;
      cast_weights->m_mapSTHi.getOrUpdateScore( retval, sth_label, action, m_nScoreIndex, amount, round) ;
   }

   if (stld_index != -1) {
      cast_weights->m_mapSTLDw.getOrUpdateScore( retval, stld_word, action, m_nScoreIndex, amount, round ) ;
      cast_weights->m_mapSTLDm.getOrUpdateScore( retval, stld_morph, action, m_nScoreIndex, amount, round ) ;
      cast_weights->m_mapSTLDi.getOrUpdateScore( retval, stld_label, action, m_nScoreIndex, amount, round) ;
   }

   if (strd_index != -1) {
      cast_weights->m_mapSTRDw.getOrUpdateScore( retval, strd_word, action, m_nScoreIndex, amount, round ) ;
      cast_weights->m_mapSTRDm.getOrUpdateScore( retval, strd_morph, action, m_nScoreIndex, amount, round ) ;
      cast_weights->m_mapSTRDi.getOrUpdateScore( retval, strd_label, action, m_nScoreIndex, amount, round) ;
   }

   if (n0ld_index != -1) {
      cast_weights->m_mapN0LDw.getOrUpdateScore( retval, n0ld_word, action, m_nScoreIndex, amount, round ) ;
      cast_weights->m_mapN0LDm.getOrUpdateScore( retval, n0ld_morph, action, m_nScoreIndex, amount, round ) ;
      cast_weights->m_mapN0LDi.getOrUpdateScore( retval, n0ld_label, action, m_nScoreIndex, amount, round) ;
   }

   if (stl2d_index != -1) {
      cast_weights->m_mapSTL2Dw.getOrUpdateScore( retval, stl2d_word, action, m_nScoreIndex, amount, round ) ;
      cast_weights->m_mapSTL2Dm.getOrUpdateScore( retval, stl2d_morph, action, m_nScoreIndex, amount, round ) ;
      cast_weights->m_mapSTL2Di.getOrUpdateScore( retval, stl2d_label, action, m_nScoreIndex, amount, round) ;
   }

   if (str2d_index != -1) {
      cast_weights->m_mapSTR2Dw.getOrUpdateScore( retval, str2d_word, action, m_nScoreIndex, amount, round ) ;
      cast_weights->m_mapSTR2Dm.getOrUpdateScore( retval, str2d_morph, action, m_nScoreIndex, amount, round ) ;
      cast_weights->m_mapSTR2Di.getOrUpdateScore( retval, str2d_label, action, m_nScoreIndex, amount, round) ; //the bugged feature
   }

   if (n0l2d_index != -1) {
      cast_weights->m_mapN0L2Dw.getOrUpdateScore( retval, n0l2d_word, action, m_nScoreIndex, amount, round ) ;
      cast_weights->m_mapN0L2Dm.getOrUpdateScore( retval, n0l2d_morph, action, m_nScoreIndex, amount, round ) ;
      cast_weights->m_mapN0L2Di.getOrUpdateScore( retval, n0l2d_label, action, m_nScoreIndex, amount, round) ;
   }

   //st1 single
   /*
   if (st1_index != -1) {
      cast_weights->m_mapST1w.getOrUpdateScore( retval, st1_word, action, m_nScoreIndex, amount, round) ;
      cast_weights->m_mapST1m.getOrUpdateScore( retval, st1_morph, action, m_nScoreIndex, amount, round) ;
      refer_or_allocate_tuple2(word_morph, &st1_word, &st1_morph);
      cast_weights->m_mapST1wm.getOrUpdateScore( retval, word_morph , action, m_nScoreIndex, amount, round ) ;
   }
   */

   //buffer minus x features
   /*
   if (bm1_index != -1) {
      cast_weights->m_mapBM1w.getOrUpdateScore( retval, bm1_word, action, m_nScoreIndex, amount, round ) ;
      cast_weights->m_mapBM1m.getOrUpdateScore( retval, bm1_morph, action, m_nScoreIndex, amount, round) ;
      refer_or_allocate_tuple2(word_morph, &bm1_word, &bm1_morph);
      cast_weights->m_mapBM1wm.getOrUpdateScore( retval, word_morph , action, m_nScoreIndex, amount, round ) ;
   }

   if (bm2_index != -1) {
      cast_weights->m_mapBM2w.getOrUpdateScore( retval, bm2_word, action, m_nScoreIndex, amount, round ) ;
      cast_weights->m_mapBM2m.getOrUpdateScore( retval, bm2_morph, action, m_nScoreIndex, amount, round) ;
      refer_or_allocate_tuple2(word_morph, &bm2_word, &bm2_morph);
      cast_weights->m_mapBM2wm.getOrUpdateScore( retval, word_morph , action, m_nScoreIndex, amount, round ) ;
   }

   if (bm3_index != -1) {
      cast_weights->m_mapBM3w.getOrUpdateScore( retval, bm3_word, action, m_nScoreIndex, amount, round ) ;
      cast_weights->m_mapBM3m.getOrUpdateScore( retval, bm3_morph, action, m_nScoreIndex, amount, round) ;
      refer_or_allocate_tuple2(word_morph, &bm3_word, &bm3_morph);
      cast_weights->m_mapBM3wm.getOrUpdateScore( retval, word_morph , action, m_nScoreIndex, amount, round ) ;
   }
   */

   //cache minus x features
   /*
   if (nm1_index != -1) {
      cast_weights->m_mapNM1w.getOrUpdateScore( retval, nm1_word, action, m_nScoreIndex, amount, round ) ;
      cast_weights->m_mapNM1m.getOrUpdateScore( retval, nm1_morph, action, m_nScoreIndex, amount, round) ;
      refer_or_allocate_tuple2(word_morph, &nm1_word, &nm1_morph);
      cast_weights->m_mapNM1wm.getOrUpdateScore( retval, word_morph , action, m_nScoreIndex, amount, round ) ;
   }

   if (nm2_index != -1) {
      cast_weights->m_mapNM2w.getOrUpdateScore( retval, nm2_word, action, m_nScoreIndex, amount, round ) ;
      cast_weights->m_mapNM2m.getOrUpdateScore( retval, nm2_morph, action, m_nScoreIndex, amount, round) ;
      refer_or_allocate_tuple2(word_morph, &nm2_word, &nm2_morph);
      cast_weights->m_mapNM2wm.getOrUpdateScore( retval, word_morph , action, m_nScoreIndex, amount, round ) ;
   }

   if (nm3_index != -1) {
      cast_weights->m_mapNM3w.getOrUpdateScore( retval, nm3_word, action, m_nScoreIndex, amount, round ) ;
      cast_weights->m_mapNM3m.getOrUpdateScore( retval, nm3_morph, action, m_nScoreIndex, amount, round) ;
      refer_or_allocate_tuple2(word_morph, &nm3_word, &nm3_morph);
      cast_weights->m_mapNM3wm.getOrUpdateScore( retval, word_morph , action, m_nScoreIndex, amount, round ) ;
   }
   */

   //didn't help: cache head features
   /*
   if (bm2h_index != -1) {
      cast_weights->m_mapBM2Hw.getOrUpdateScore( retval, bm2h_word, action, m_nScoreIndex, amount, round ) ;
      cast_weights->m_mapBM2Hm.getOrUpdateScore( retval, bm2h_morph, action, m_nScoreIndex, amount, round) ;
      refer_or_allocate_tuple2(word_morph, &bm2h_word, &bm2h_morph);
      cast_weights->m_mapBM2Hwm.getOrUpdateScore( retval, word_morph , action, m_nScoreIndex, amount, round ) ;
   }

   if (bm3h_index != -1) {
      cast_weights->m_mapBM3Hw.getOrUpdateScore( retval, bm3h_word, action, m_nScoreIndex, amount, round ) ;
      cast_weights->m_mapBM3Hm.getOrUpdateScore( retval, bm3h_morph, action, m_nScoreIndex, amount, round) ;
      refer_or_allocate_tuple2(word_morph, &bm3h_word, &bm3h_morph);
      cast_weights->m_mapBM3Hwm.getOrUpdateScore( retval, word_morph , action, m_nScoreIndex, amount, round ) ;
   }
   */

   //b0, b1, b2 word forms (we have no lemmas or morph yet)
   if (b0_index != -1) {
         cast_weights->m_mapB0w.getOrUpdateScore( retval, b0_word, action, m_nScoreIndex, amount, round ) ;
   }

   if (b1_index != -1) {
	   cast_weights->m_mapB1w.getOrUpdateScore( retval, b1_word, action, m_nScoreIndex, amount, round ) ;
   }

   if (b2_index != -1) {
         cast_weights->m_mapB2w.getOrUpdateScore( retval, b2_word, action, m_nScoreIndex, amount, round ) ;
   }

   //prefixes and suffixes of b0 for tagging, as well as whether it has digits, uppercase or hyphens
   if (b0_index != -1) {
         cast_weights->m_mapB0p1.getOrUpdateScore( retval, b0_pref1, action, m_nScoreIndex, amount, round ) ;
         cast_weights->m_mapB0p2.getOrUpdateScore( retval, b0_pref2, action, m_nScoreIndex, amount, round ) ;
         cast_weights->m_mapB0p3.getOrUpdateScore( retval, b0_pref3, action, m_nScoreIndex, amount, round ) ;
         cast_weights->m_mapB0p4.getOrUpdateScore( retval, b0_pref4, action, m_nScoreIndex, amount, round ) ;

         cast_weights->m_mapB0s1.getOrUpdateScore( retval, b0_suff1, action, m_nScoreIndex, amount, round ) ;
         cast_weights->m_mapB0s2.getOrUpdateScore( retval, b0_suff2, action, m_nScoreIndex, amount, round ) ;
         cast_weights->m_mapB0s3.getOrUpdateScore( retval, b0_suff3, action, m_nScoreIndex, amount, round ) ;
         cast_weights->m_mapB0s4.getOrUpdateScore( retval, b0_suff4, action, m_nScoreIndex, amount, round ) ;

         cast_weights->m_mapB0hd.getOrUpdateScore( retval, b0_hasDigit, action, m_nScoreIndex, amount, round ) ;
         cast_weights->m_mapB0hu.getOrUpdateScore( retval, b0_hasUpper, action, m_nScoreIndex, amount, round ) ;
         cast_weights->m_mapB0hh.getOrUpdateScore( retval, b0_hasHyphen, action, m_nScoreIndex, amount, round ) ;
   }

   //prefixes and suffixes of b1
   /*
   if (b1_index != -1) {
         cast_weights->m_mapB1p1.getOrUpdateScore( retval, b1_pref1, action, m_nScoreIndex, amount, round ) ;
         cast_weights->m_mapB1p2.getOrUpdateScore( retval, b1_pref2, action, m_nScoreIndex, amount, round ) ;
         cast_weights->m_mapB1p3.getOrUpdateScore( retval, b1_pref3, action, m_nScoreIndex, amount, round ) ;
         cast_weights->m_mapB1p4.getOrUpdateScore( retval, b1_pref4, action, m_nScoreIndex, amount, round ) ;

         cast_weights->m_mapB1s1.getOrUpdateScore( retval, b1_suff1, action, m_nScoreIndex, amount, round ) ;
         cast_weights->m_mapB1s2.getOrUpdateScore( retval, b1_suff2, action, m_nScoreIndex, amount, round ) ;
         cast_weights->m_mapB1s3.getOrUpdateScore( retval, b1_suff3, action, m_nScoreIndex, amount, round ) ;
         cast_weights->m_mapB1s4.getOrUpdateScore( retval, b1_suff4, action, m_nScoreIndex, amount, round ) ;
   }
   */

   //piecemeal single morph features

   /*
   if (st_index != -1) {
      cast_weights->m_mapSTmp.getOrUpdateScore( retval, st_mpostag, action, m_nScoreIndex, amount, round) ;
      cast_weights->m_mapSTnum.getOrUpdateScore( retval, st_number, action, m_nScoreIndex, amount, round) ;
      cast_weights->m_mapSTper.getOrUpdateScore( retval, st_person, action, m_nScoreIndex, amount, round) ;
   }

   if (n0_index != -1) {
      cast_weights->m_mapN0mp.getOrUpdateScore( retval, n0_mpostag, action, m_nScoreIndex, amount, round ) ;
      cast_weights->m_mapN0num.getOrUpdateScore( retval, n0_number, action, m_nScoreIndex, amount, round) ;
      cast_weights->m_mapN0per.getOrUpdateScore( retval, n0_person , action, m_nScoreIndex, amount, round ) ;
   }
   */

   //concordance single morph features
   if (st_index != -1 && n0_index != -1) {
	  refer_or_allocate_tuple2(int_int, &st_number, &n0_number);
      cast_weights->m_mapSTnumN0num.getOrUpdateScore( retval, int_int, action, m_nScoreIndex, amount, round );
	  refer_or_allocate_tuple2(int_int, &st_person, &n0_person);
      cast_weights->m_mapSTperN0per.getOrUpdateScore( retval, int_int, action, m_nScoreIndex, amount, round );

	  //refer_or_allocate_tuple2(int_int, &st_vtype, &n0_vtype);
      //cast_weights->m_mapSTvtyN0vty.getOrUpdateScore( retval, int_int, action, m_nScoreIndex, amount, round );
	  //refer_or_allocate_tuple2(int_int, &st_vform, &n0_vform);
      //cast_weights->m_mapSTvfoN0vfo.getOrUpdateScore( retval, int_int, action, m_nScoreIndex, amount, round );

	  //refer_or_allocate_tuple2(int_int, &st1_number, &n0_number);
      //cast_weights->m_mapST1numN0num.getOrUpdateScore( retval, int_int, action, m_nScoreIndex, amount, round );
	  //refer_or_allocate_tuple2(int_int, &st1_person, &n0_person);
      //cast_weights->m_mapST1perN0per.getOrUpdateScore( retval, int_int, action, m_nScoreIndex, amount, round );

	  //refer_or_allocate_tuple2(int_int, &st_number, &n1_number);
      //cast_weights->m_mapSTnumN1num.getOrUpdateScore( retval, int_int, action, m_nScoreIndex, amount, round );
	  //refer_or_allocate_tuple2(int_int, &st_person, &n1_person);
      //cast_weights->m_mapSTperN1per.getOrUpdateScore( retval, int_int, action, m_nScoreIndex, amount, round );
   }



   // s0 and n0
   if (st_index != -1) {
	  refer_or_allocate_tuple4(word_word_morph_morph, &st_word, &n0_word, &st_morph, &n0_morph);
      cast_weights->m_mapSTwmN0wm.getOrUpdateScore( retval, word_word_morph_morph, action, m_nScoreIndex, amount, round );
      refer_or_allocate_tuple3(word_word_morph, &st_word, &n0_word, &st_morph);
      cast_weights->m_mapSTwmN0w.getOrUpdateScore( retval, word_word_morph, action, m_nScoreIndex, amount, round ) ;
      refer_or_allocate_tuple3(word_word_morph, &st_word, &n0_word, &n0_morph);
      cast_weights->m_mapSTwN0wm.getOrUpdateScore( retval, word_word_morph, action, m_nScoreIndex, amount, round ) ;
      refer_or_allocate_tuple3(word_morph_morph, &st_word, &st_morph, &n0_morph);
      cast_weights->m_mapSTwmN0m.getOrUpdateScore( retval, word_morph_morph, action, m_nScoreIndex, amount, round ) ;
      refer_or_allocate_tuple3(word_morph_morph, &n0_word, &st_morph, &n0_morph);
      cast_weights->m_mapSTmN0wm.getOrUpdateScore( retval, word_morph_morph, action, m_nScoreIndex, amount, round ) ;
      cast_weights->m_mapSTwN0w.getOrUpdateScore( retval, st_word_n0_word, action, m_nScoreIndex, amount, round ) ;
      refer_or_allocate_tuple2(morph_morph, &st_morph, &n0_morph);
      cast_weights->m_mapSTmN0m.getOrUpdateScore( retval, morph_morph, action, m_nScoreIndex, amount, round ) ;
   }

   //buffer -x morph
   /*
   if (bm1_index != -1 && bm2_index != -1) {
	  refer_or_allocate_tuple2(morph_morph, &bm1_morph, &bm2_morph);
      cast_weights->m_mapBM1mBM2m.getOrUpdateScore( retval, morph_morph, action, m_nScoreIndex, amount, round ) ;
      refer_or_allocate_tuple3(morph_morph_morph, &bm1_morph, &bm2_morph, &bm3_morph);
      cast_weights->m_mapBM1mBM2mBM3m.getOrUpdateScore( retval, morph_morph_morph, action, m_nScoreIndex, amount, round ) ;
   }
   */

   if (st_index != -1 && n0_index != -1) {
	  //TODO: Use a morph set instead of tuples? Probably not, wouldn't fit in an unsigned long.
	  refer_or_allocate_tuple2(morph_morph, &n0_morph, &n1_morph);
      cast_weights->m_mapN0mN1m.getOrUpdateScore( retval, morph_morph, action, m_nScoreIndex, amount, round ) ;
      refer_or_allocate_tuple3(morph_morph_morph, &n0_morph, &n1_morph, &n2_morph);
      cast_weights->m_mapN0mN1mN2m.getOrUpdateScore( retval, morph_morph_morph, action, m_nScoreIndex, amount, round ) ;
      refer_or_allocate_tuple3(morph_morph_morph, &st_morph, &n0_morph, &n1_morph);
      cast_weights->m_mapSTmN0mN1m.getOrUpdateScore( retval, morph_morph_morph, action, m_nScoreIndex, amount, round ) ;
      refer_or_allocate_tuple3(morph_morph_morph, &st_morph, &n0_morph, &n0ld_morph);
      cast_weights->m_mapSTmN0mN0LDm.getOrUpdateScore( retval, morph_morph_morph, action, m_nScoreIndex, amount, round ) ;
      refer_or_allocate_tuple3(morph_morph_morph, &n0_morph, &n0ld_morph, &n0l2d_morph);
      cast_weights->m_mapN0mN0LDmN0L2Dm.getOrUpdateScore( retval, morph_morph_morph, action, m_nScoreIndex, amount, round ) ;
   }

   if (st_index!=-1) {
	  refer_or_allocate_tuple3(morph_morph_morph, &sth_morph, &st_morph, &n0_morph);
      cast_weights->m_mapSTHmSTmN0m.getOrUpdateScore( retval, morph_morph_morph, action, m_nScoreIndex, amount, round ) ;
      refer_or_allocate_tuple3(morph_morph_morph, &sthh_morph, &sth_morph, &st_morph);
      cast_weights->m_mapSTHHmSTHmSTm.getOrUpdateScore( retval, morph_morph_morph, action, m_nScoreIndex, amount, round ) ;
      refer_or_allocate_tuple3(morph_morph_morph, &st_morph, &stld_morph, &n0_morph);
      cast_weights->m_mapSTmSTLDmN0m.getOrUpdateScore( retval, morph_morph_morph, action, m_nScoreIndex, amount, round ) ;
      refer_or_allocate_tuple3(morph_morph_morph, &st_morph, &stld_morph, &stl2d_morph);
      cast_weights->m_mapSTmSTLDmSTL2Dm.getOrUpdateScore( retval, morph_morph_morph, action, m_nScoreIndex, amount, round ) ;
      refer_or_allocate_tuple3(morph_morph_morph, &st_morph, &strd_morph, &n0_morph);
      cast_weights->m_mapSTmSTRDmN0m.getOrUpdateScore( retval, morph_morph_morph, action, m_nScoreIndex, amount, round ) ;
      refer_or_allocate_tuple3(morph_morph_morph, &st_morph, &strd_morph, &str2d_morph);
      cast_weights->m_mapSTmSTRDmSTR2Dm.getOrUpdateScore( retval, morph_morph_morph, action, m_nScoreIndex, amount, round ) ;
   }

   //Lemma bigrams in the cache
   if ( n1_index != -1 )
   {
	   refer_or_allocate_tuple2(word_word, &n0_word, &n1_word);
	   cast_weights->m_mapN0wN1w.getOrUpdateScore( retval, word_word, action, m_nScoreIndex, amount, round ) ;
	   refer_or_allocate_tuple3(word_word_morph, &n0_word, &n1_word, &n1_morph);
	   cast_weights->m_mapN0wN1wN1m.getOrUpdateScore( retval, word_word_morph, action, m_nScoreIndex, amount, round ) ;
   }
   if ( n2_index != -1 )
   {
	   refer_or_allocate_tuple2(word_word, &n1_word, &n2_word);
	   cast_weights->m_mapN1wN2w.getOrUpdateScore( retval, word_word, action, m_nScoreIndex, amount, round ) ;
   }

   // distance
   if (st_index!=-1 && n0_index!=-1) {
      refer_or_allocate_tuple2(word_int, &st_word, &st_n0_dist);
      cast_weights->m_mapSTwd.getOrUpdateScore( retval, word_int, action, m_nScoreIndex, amount, round) ;
      refer_or_allocate_tuple2(morph_int, &st_morph, &st_n0_dist);
      cast_weights->m_mapSTmd.getOrUpdateScore( retval, morph_int, action, m_nScoreIndex, amount, round ) ;
      refer_or_allocate_tuple2(word_int, &n0_word, &st_n0_dist);
      cast_weights->m_mapN0wd.getOrUpdateScore( retval, word_int, action, m_nScoreIndex, amount, round ) ;
      refer_or_allocate_tuple2(morph_int, &n0_morph, &st_n0_dist);
      cast_weights->m_mapN0md.getOrUpdateScore( retval, morph_int, action, m_nScoreIndex, amount, round ) ;
      refer_or_allocate_tuple3(word_word_int, &st_word, &n0_word, &st_n0_dist);
      cast_weights->m_mapSTwN0wd.getOrUpdateScore( retval, word_word_int, action, m_nScoreIndex, amount, round ) ;
      refer_or_allocate_tuple3(morph_morph_int, &st_morph, &n0_morph, &st_n0_dist);
      cast_weights->m_mapSTmN0md.getOrUpdateScore( retval, morph_morph_int, action, m_nScoreIndex, amount, round ) ;
   }

   // st arity
   if (st_index != -1) {
      refer_or_allocate_tuple2(word_int, &st_word, &st_rarity);
      cast_weights->m_mapSTwra.getOrUpdateScore( retval, word_int, action, m_nScoreIndex, amount, round) ;
      refer_or_allocate_tuple2(morph_int, &st_morph, &st_rarity);
      cast_weights->m_mapSTmra.getOrUpdateScore( retval, morph_int, action, m_nScoreIndex, amount, round ) ;
      refer_or_allocate_tuple2(word_int, &st_word, &st_larity);
      cast_weights->m_mapSTwla.getOrUpdateScore( retval, word_int, action, m_nScoreIndex, amount, round) ;
      refer_or_allocate_tuple2(morph_int, &st_morph, &st_larity);
      cast_weights->m_mapSTmla.getOrUpdateScore( retval, morph_int, action, m_nScoreIndex, amount, round ) ;
   }

   // n0 arity
   if (n0_index!=-1) {
      refer_or_allocate_tuple2(word_int, &n0_word, &n0_larity);
      cast_weights->m_mapN0wla.getOrUpdateScore( retval, word_int, action, m_nScoreIndex, amount, round) ;
      refer_or_allocate_tuple2(morph_int, &n0_morph, &n0_larity);
      cast_weights->m_mapN0mla.getOrUpdateScore( retval, morph_int, action, m_nScoreIndex, amount, round ) ;
   }

   // st labelset
   if (st_index != -1){
      refer_or_allocate_tuple2(word_tagset, &st_word, &st_rtagset);
      cast_weights->m_mapSTwrp.getOrUpdateScore( retval, word_tagset, action, m_nScoreIndex, amount, round) ;
      refer_or_allocate_tuple2(morph_tagset, &st_morph, &st_rtagset);
      cast_weights->m_mapSTmrp.getOrUpdateScore( retval, morph_tagset, action, m_nScoreIndex, amount, round ) ;

      refer_or_allocate_tuple2(word_tagset, &st_word, &st_ltagset);
      cast_weights->m_mapSTwlp.getOrUpdateScore( retval, word_tagset, action, m_nScoreIndex, amount, round) ;
      refer_or_allocate_tuple2(morph_tagset, &st_morph, &st_ltagset);
      cast_weights->m_mapSTmlp.getOrUpdateScore( retval, morph_tagset, action, m_nScoreIndex, amount, round ) ;
   }

   // n0 labelset
   if (n0_index != -1){
      refer_or_allocate_tuple2(word_tagset, &n0_word, &n0_ltagset);
      cast_weights->m_mapN0wlp.getOrUpdateScore( retval, word_tagset, action, m_nScoreIndex, amount, round) ;
      refer_or_allocate_tuple2(morph_tagset, &n0_morph, &n0_ltagset);
      cast_weights->m_mapN0mlp.getOrUpdateScore( retval, morph_tagset, action, m_nScoreIndex, amount, round ) ;
   }

   //uncomment to use conll features

   //we don't use them as the morph parser generates its own lemmas and morphological information

   /*
   if (m_bCoNLL) {

      static unsigned i;

      if (st_index!=-1) {
         if (!m_lCacheCoNLLLemma[st_index].empty()) cast_weights->m_mapSTl.getOrUpdateScore( retval, m_lCacheCoNLLLemma[st_index], action, m_nScoreIndex, amount, round) ;
         if (m_lCacheCoNLLCPOS[st_index] != CCoNLLCPOS()) cast_weights->m_mapSTc.getOrUpdateScore( retval, m_lCacheCoNLLCPOS[st_index], action, m_nScoreIndex, amount, round) ;
         for (i=0; i<m_lCacheCoNLLFeats[st_index].size(); ++i)
            cast_weights->m_mapSTf.getOrUpdateScore( retval, m_lCacheCoNLLFeats[st_index][i], action, m_nScoreIndex, amount, round) ;
      } // if (st_index!=-1)

      if (n0_index!=-1) {
         if (!m_lCacheCoNLLLemma[n0_index].empty()) cast_weights->m_mapN0l.getOrUpdateScore( retval, m_lCacheCoNLLLemma[n0_index], action, m_nScoreIndex, amount, round) ;
         if (m_lCacheCoNLLCPOS[n0_index] != CCoNLLCPOS()) cast_weights->m_mapN0c.getOrUpdateScore( retval, m_lCacheCoNLLCPOS[n0_index], action, m_nScoreIndex, amount, round) ;
         for (i=0; i<m_lCacheCoNLLFeats[n0_index].size(); ++i)
            cast_weights->m_mapN0f.getOrUpdateScore( retval, m_lCacheCoNLLFeats[n0_index][i], action, m_nScoreIndex, amount, round) ;
      } // if (n0_index!=-1)

      if (n1_index!=-1) {
         if (!m_lCacheCoNLLLemma[n1_index].empty()) cast_weights->m_mapN1l.getOrUpdateScore( retval, m_lCacheCoNLLLemma[n1_index], action, m_nScoreIndex, amount, round) ;
         if (m_lCacheCoNLLCPOS[n1_index] != CCoNLLCPOS()) cast_weights->m_mapN1c.getOrUpdateScore( retval, m_lCacheCoNLLCPOS[n1_index], action, m_nScoreIndex, amount, round) ;
         for (i=0; i<m_lCacheCoNLLFeats[n1_index].size(); ++i)
            cast_weights->m_mapN1f.getOrUpdateScore( retval, m_lCacheCoNLLFeats[n1_index][i], action, m_nScoreIndex, amount, round) ;
      } // if (n1_index!=-1)
   }
   */


}

/*---------------------------------------------------------------
 *
 * getGlobalScore - get the score of a parse tree
 *
 * Inputs: parse graph
 *
 *---------------------------------------------------------------*/

SCORE_TYPE CDepParser::getGlobalScore(const CDependencyParse &parsed) {
   THROW("depparser.cpp: getGlobalScore unsupported");
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

void CDepParser::updateScores(const CDependencyParse & parsed , const CDependencyParse & correct , int round ) {
   THROW("depparser.cpp: updateScores unsupported");
}

/*---------------------------------------------------------------
 *
 * updateScoreForState - update a single positive or negative output
 *
 *--------------------------------------------------------------*/

inline void CDepParser::updateScoreForState( const CStateItem &from, const CStateItem *output , const SCORE_TYPE &amount ) {
   static CStateItem item(&m_lCache);
   static unsigned long action;
   static CPackedScoreType<SCORE_TYPE, action::MAX> empty;
   item = from;
   while ( item != *output ) {
      action = item.FollowMove( output );
      getOrUpdateStackScore( &item, empty, action, amount, m_nTrainingRound );
#ifdef LABELED
//      if ( action::getLabel(action) )
//         getOrUpdateStackScore( &item, empty, action::getUnlabeledAction(action), amount, m_nTrainingRound );
#endif
      item.Move( action );
   }
}

/*---------------------------------------------------------------
 *
 * updateScoresForStates - update scores for states
 *
 *--------------------------------------------------------------*/

void CDepParser::updateScoresForStates( const CStateItem *output , const CStateItem *correct , SCORE_TYPE amount_add, SCORE_TYPE amount_subtract ) {

   // do not update those steps where they are correct
   static CStateItem item(&m_lCache);
   static unsigned long action, correct_action;
   item.clear();
   while ( item != *output ) {
      action = item.FollowMove( output );
      correct_action = item.FollowMove( correct );
      if ( action == correct_action )
         item.Move( action );
      else break;
   }

   // for the necessary information for the correct and output parsetree
   updateScoreForState( item, correct , amount_add ) ;
   updateScoreForState( item, output , amount_subtract ) ;

   m_nTotalErrors++;
}

/*---------------------------------------------------------------
 *
 * reduce - helper function
 *
 *--------------------------------------------------------------*/

inline void CDepParser::reduce( const CStateItem *item, const CPackedScoreType<SCORE_TYPE, action::MAX> &scores ) {
   static action::CScoredAction scoredaction;
   // update stack score
   scoredaction.action = action::REDUCE;
   scoredaction.score = item->score + scores[scoredaction.action];
   m_Beam->insertItem(&scoredaction);
}

/*---------------------------------------------------------------
 *
 * arcleft - helping function
 *
 *--------------------------------------------------------------*/

inline void CDepParser::arcleft( const CStateItem *item, const CPackedScoreType<SCORE_TYPE, action::MAX> &scores ) {
   static action::CScoredAction scoredaction;
   static unsigned label;
#ifdef LABELED
   for (label=CDependencyLabel::FIRST; label<CDependencyLabel::COUNT; ++label) {
      //if ( !m_weights->rules() || canAssignLabel(m_lCache, item->size(), item->stacktop(), label) )
      {
         scoredaction.action = action::encodeAction(action::ARC_LEFT, label);
         scoredaction.score = item->score + scores[scoredaction.action];
                               //+scores[action::ARC_LEFT];
         m_Beam->insertItem(&scoredaction);
      }
   }
#else
   scoredaction.action = action::ARC_LEFT;
   scoredaction.score = item->score + scores[scoredaction.action];
      m_Beam->insertItem(&scoredaction);
#endif
}

/*---------------------------------------------------------------
 *
 * arcright - helping function
 *
 *--------------------------------------------------------------*/

inline void CDepParser::arcright( const CStateItem *item, const CPackedScoreType<SCORE_TYPE, action::MAX> &scores ) {
   static action::CScoredAction scoredaction;
   static unsigned label;
#ifdef LABELED
   for (label=CDependencyLabel::FIRST; label<CDependencyLabel::COUNT; ++label) {
      //if ( !m_weights->rules() || canAssignLabel(m_lCache, item->stacktop(), item->size(), label) )
      {
         scoredaction.action = action::encodeAction(action::ARC_RIGHT, label);
         scoredaction.score = item->score + scores[scoredaction.action];
                              //+scores[action::ARC_RIGHT];
         m_Beam->insertItem(&scoredaction);
      }
   }
#else
   scoredaction.action = action::ARC_RIGHT;
   scoredaction.score = item->score + scores[scoredaction.action];
   m_Beam->insertItem(&scoredaction);
#endif
}

/*---------------------------------------------------------------
 *
 * shift - help function
 *
 *--------------------------------------------------------------*/

inline void CDepParser::shift( const CStateItem *item, const CPackedScoreType<SCORE_TYPE, action::MAX> &scores ) {
   static action::CScoredAction scoredaction;
   // update stack score
   scoredaction.action = action::SHIFT;
   scoredaction.score = item->score + scores[scoredaction.action];
   m_Beam->insertItem(&scoredaction);
}

/*---------------------------------------------------------------
 *
 * shiftcache - help function
 *
 *--------------------------------------------------------------*/

inline void CDepParser::shiftcache( const CStateItem *item, unsigned long morph , const CPackedScoreType<SCORE_TYPE, action::MAX> &scores ) {
   static action::CScoredAction scoredaction;
   // update stack score
   scoredaction.action = action::encodeAction(action::SHIFT_CACHE, morph);
   scoredaction.score = item->score + scores[scoredaction.action];
   m_Beam->insertItem(&scoredaction);
}

/*---------------------------------------------------------------
 *
 * poproot - help function
 *
 *--------------------------------------------------------------*/

inline void CDepParser::poproot( const CStateItem *item, const CPackedScoreType<SCORE_TYPE, action::MAX> &scores ) {
   static action::CScoredAction scoredaction;
   // update stack score
   scoredaction.action = action::POP_ROOT;
   scoredaction.score = item->score + scores[scoredaction.action];
   m_Beam->insertItem(&scoredaction);
}


/*---------------------------------------------------------------
 *
 * work - the working process shared by training and parsing
 *
 * Returns: makes a new instance of CDependencyParse
 *
 *--------------------------------------------------------------*/

void CDepParser::work( const bool bTrain , const CStringVector &sentence , CDependencyParse *retval , const CDependencyParse &correct , int nBest , SCORE_TYPE *scores ) {

#ifdef DEBUG
   clock_t total_start_time = clock();
#endif
   static int index;
   const int length = sentence.size() ;

   const CStateItem *pGenerator ;
   static CStateItem pCandidate(&m_lCache) ;

   // used only for training
   static bool bCorrect ;  // used in learning for early update
   static bool bContradictsRules;
   static CStateItem correctState(&m_lCache) ;
   static CPackedScoreType<SCORE_TYPE, action::MAX> packed_scores;

   ASSERT(length<MAX_SENTENCE_SIZE, "The size of the sentence is larger than the system configuration.");

   TRACE("Initialising the decoding process...") ;
   // initialise word cache
   bContradictsRules = false;
   m_lCache.clear();
   m_lCacheP1.clear(); m_lCacheP2.clear(); m_lCacheP3.clear(); m_lCacheP4.clear();
   m_lCacheS1.clear(); m_lCacheS2.clear(); m_lCacheS3.clear(); m_lCacheS4.clear();

   for ( index=0; index<length; ++index ) {
      m_lCache.push_back( CWord(sentence[index]) );

      m_lCacheP1.push_back( CWord(sentence[index].substr(0,1)) );
      m_lCacheP2.push_back( CWord(sentence[index].substr(0,2)) );
      m_lCacheP3.push_back( CWord(sentence[index].substr(0,3)) );
      m_lCacheP4.push_back( CWord(sentence[index].substr(0,4)) );

      m_lCacheS1.push_back( sentence[index].length() < 1 ? CWord(sentence[index]) : CWord(sentence[index].substr(sentence[index].length()-1,1)) );
      m_lCacheS2.push_back( sentence[index].length() < 2 ? CWord(sentence[index]) : CWord(sentence[index].substr(sentence[index].length()-2,2)) );
      m_lCacheS3.push_back( sentence[index].length() < 3 ? CWord(sentence[index]) : CWord(sentence[index].substr(sentence[index].length()-3,3)) );
      m_lCacheS4.push_back( sentence[index].length() < 4 ? CWord(sentence[index]) : CWord(sentence[index].substr(sentence[index].length()-4,4)) );

      m_lCacheHasDigit.push_back ( std::string::npos != sentence[index].find_first_of("0123456789") );
      m_lCacheHasUpper.push_back ( std::string::npos != sentence[index].find_first_of("ABCDEFGHIJKLMNOPQRSTUVWXYZ") );
      m_lCacheHasHyphen.push_back ( std::string::npos != sentence[index].find_first_of("-") );

      // filter out training examples with rules
      if (bTrain && m_weights->rules()) {
    	  //we don't use these rules as they use postags
         // the root
    	  /*
         if ( correct[index].head == DEPENDENCY_LINK_NO_HEAD && canBeRoot(m_lCache[index].tag.code())==false) {
            TRACE("Rule contradiction: " << m_lCache[index].tag.code() << " can be root.");
            bContradictsRules = true;
         }
         // head left
         if ( correct[index].head < index && hasLeftHead(m_lCache[index].tag.code())==false) {
            TRACE("Rule contradiction: " << m_lCache[index].tag.code() << " has left head.");
            bContradictsRules = true;
         }
         // head right
         if ( correct[index].head > index && hasRightHead(m_lCache[index].tag.code())==false) {
            TRACE("Rule contradiction: " << m_lCache[index].tag.code() << " has right head.");
            bContradictsRules = true;
         }
         */
      }
   }

   m_lCacheMorph.clear();
   //init gold standard morphs and lemmas
   if ( bTrain )
   {
	   for (index=0; index<length; ++index)
	   {
		   CMorph correctMorph = conllToMorph(correct[index].word, correct[index].tag, correct[index].feats);
	       m_lCacheMorph.push_back(correctMorph);
	       //std::cout << "Correct word/morph: " << m_lCache[index] << ": " << m_lCacheMorph[index].str() << "\n";
	   }
   }

   // initialise agenda
   m_Agenda->clear();
   pCandidate.clear();                          // restore state using clean
   m_Agenda->pushCandidate(&pCandidate);           // and push it back
   m_Agenda->nextRound();                       // as the generator item
   if (bTrain) correctState.clear();

   // verifying supertags
   if (m_supertags) {
      ASSERT(m_supertags->getSentenceSize()==length, "Sentence size does not match supertags size");
   }

#ifdef LABELED
   unsigned long label;
   m_lCacheLabel.clear();
   if (bTrain) {
      for (index=0; index<length; ++index) {
         m_lCacheLabel.push_back(CDependencyLabel(correct[index].label));
//         if (m_weights->rules() && !canAssignLabel(m_lCache, correct[index].head, index, m_lCacheLabel[index])) {
//            TRACE("Rule contradiction: " << correct[index].label << " on link head " << m_lCache[correct[index].head].tag.code() << " dep " << m_lCache[index].tag.code());
//            bContradictsRules = true;
//         }
      }
   }
#endif

   // skip the training example if contradicts
   if (bTrain && m_weights->rules() && bContradictsRules) {
      std::cerr << "Skipping training example because it contradicts rules..." <<std::endl;
      return;
   }

   TRACE("Decoding started");
   // loop with the next word to process in the sentence
   for (index=0; index<length*3; ++index) { //length*3 due to shiftcache transition


	   //std::cout << "Iter " << index << "\n";

      if (bTrain) bCorrect = false ;

      // none can this find with pruning ???
      if (m_Agenda->generatorSize() == 0) {
         WARNING("parsing failed");
         return;
      }

      pGenerator = m_Agenda->generatorStart();

      //std::cout << "Our generator: " << pGenerator->debugstring() << "\n";

      // iterate generators
      for (int j=0; j<m_Agenda->generatorSize(); ++j) {

    	  //std::cout << "Subiter " << j << " with item " << pGenerator->size() << " " << pGenerator->stacksize() << " " << pGenerator->cachesize() << "\n";

         // for the state items that already contain all words
         m_Beam->clear();
         packed_scores.reset();
         getOrUpdateStackScore( pGenerator, packed_scores, action::NO_ACTION );
         if ( pGenerator->size() == length ) {
            assert( pGenerator->stacksize() != 0 );
            if ( pGenerator->stacksize()>1 ) {
#ifdef FRAGMENTED_TREE
               if (pGenerator->head(pGenerator->stacktop()) == DEPENDENCY_LINK_NO_HEAD)
                  poproot(pGenerator, packed_scores);
               else
#endif
               reduce(pGenerator, packed_scores) ;
            }
            else {
               poproot(pGenerator, packed_scores);
            }
         }
         // for the state items that still need more words
         else {
        	bool needToShiftCache = ( pGenerator->cachesize() < MORPH_CACHE_LIMIT && pGenerator->morphanalyzed() < length );
        	if ( needToShiftCache ) {

        		//std::cout << "I'm in a position to shift cache\n"; std::cout.flush();

        		CWord wordToAnalyze = m_lCache[pGenerator->morphanalyzed()]; //morphanalyzed() points to first word that hasn't been subject to morphological analysis

        		//std::cout << "I'm analyzing" << wordToAnalyze.str() << "\n"; std::cout.flush();

        		std::set<CMorph> setOfMorphs = getPossibleMorph(wordToAnalyze.str());
        		for ( std::set<CMorph>::iterator it = setOfMorphs.begin() ; it != setOfMorphs.end() ; ++it )
        		{
        			CMorph morph = *it;
        			//std::cout << "Word " << wordToAnalyze.str() << " Morph" << morph.str() << "\n"; std::cout.flush();
        			shiftcache(pGenerator, morph.code() , packed_scores) ;
        		}

        	}

            if ( !pGenerator->afterreduce() ) { // there are many ways when there are many arcrighted items on the stack and the root need arcleft. force this.
               if (
#ifndef FRAGMENTED_TREE
                    ( pGenerator->size() < length-1 || pGenerator->stackempty() ) && // keep only one global root
#endif
                    ( pGenerator->stackempty() || m_supertags == 0 || m_supertags->canShift( pGenerator->size() ) ) && // supertags
                    ( pGenerator->stackempty() || !m_weights->rules() /*|| canBeRoot( m_lCache[pGenerator->size()].tag.code() ) || hasRightHead(m_lCache[pGenerator->size()].tag.code())*/ ) && // rules
                    ( pGenerator->cachesize() > 0 ) && //need something in the cache to shift
                    ( !needToShiftCache ) //cannot do anything else if there are words pending to shift to cache
                  ) {

            	   //std::cout << "I'm in a position to just shift\n"; std::cout.flush();

            	   shift(pGenerator, packed_scores) ;
               }
            }
            if ( !pGenerator->stackempty() ) {
               if (
#ifndef FRAGMENTED_TREE
                    ( pGenerator->size() < length-1 || pGenerator->headstacksize() == 1 ) && // one root
#endif
                    ( m_supertags == 0 || m_supertags->canArcRight(pGenerator->stacktop(), pGenerator->size()) ) && // supertags conform to this action
                    ( !m_weights->rules() /*|| hasLeftHead(m_lCache[pGenerator->size()].tag.code())*/ ) && // rules
                    ( !needToShiftCache ) //cannot do anything else if there are words pending to shift to cache
                  ) {

            	   //std::cout << "I'm in a position to arc right\n"; std::cout.flush();

                  arcright(pGenerator, packed_scores) ;
               }
            }
            if ( ((!m_bCoNLL && !pGenerator->stackempty()) ||
                 (m_bCoNLL && pGenerator->stacksize()>1)) && // make sure that for conll the first item is not popped
            		( !needToShiftCache ) //cannot do anything else if there are words pending to shift to cache
               ) {
               if ( pGenerator->head( pGenerator->stacktop() ) != DEPENDENCY_LINK_NO_HEAD ) {
            	   //std::cout << "I'm in a position to reduce\n"; std::cout.flush();
                  reduce(pGenerator, packed_scores) ;
               }
               else {
                  if ( (m_supertags == 0 || m_supertags->canArcLeft(pGenerator->size(), pGenerator->stacktop())) && // supertags
                       (!m_weights->rules() /*|| hasRightHead(m_lCache[pGenerator->stacktop()].tag.code())*/ ) // rules
                     ) {
                	  //std::cout << "I'm in a position to arc left\n"; std::cout.flush();
                     arcleft(pGenerator, packed_scores) ;
                  }
               }
            }
         }

         // insert item
         for (unsigned i=0; i<m_Beam->size(); ++i) {
            pCandidate = *pGenerator;
            pCandidate.score = m_Beam->item(i)->score;
            pCandidate.Move( m_Beam->item(i)->action );
            m_Agenda->pushCandidate(&pCandidate);
         }

     	//std::cout << "Generator " << pGenerator->size() << " " << pGenerator->stacksize() << " " << pGenerator->cachesize() << "\n";
     	//std::cout << "Correct state " << correctState.size() << " " << correctState.stacksize() << " " << correctState.cachesize() << "\n";
     	//std::cout << "Equal? " << (*pGenerator == correctState) << "\n";

         //std::cout << "Generator " << pGenerator->debugstring();
         //std::cout << "Correct state " << correctState.debugstring();
         //std::cout << "Equal? " << (*pGenerator == correctState) << "\n";

         if (bTrain && *pGenerator == correctState) {
            bCorrect = true ;
         }
         pGenerator = m_Agenda->generatorNext() ;

      } //end for int j...
      // when we are doing training, we need to consider the standard move and update
      if (bTrain) {
#ifdef EARLY_UPDATE
         if (!bCorrect) {
            TRACE("Error at the "<<correctState.size()<<"th word; total is "<<correct.size())
            updateScoresForStates(m_Agenda->bestGenerator(), &correctState, 1, -1) ;
#ifndef LOCAL_LEARNING
            return ;
#else
            m_Agenda->clearCandidates();
            m_Agenda->pushCandidate(&correctState);
#endif
         }
#endif

         if (bCorrect) {
#ifdef LABELED
            correctState.StandardMoveStep(correct, m_lCacheLabel, m_lCacheMorph);
#else
            correctState.StandardMoveStep(correct);
#endif
         }
#ifdef LOCAL_LEARNING
         ++m_nTrainingRound; // each training round is one transition-action
#endif
      }

      m_Agenda->nextRound(); // move round
   }

   if (bTrain) {
      correctState.StandardFinish(); // pop the root that is left
      // then make sure that the correct item is stack top finally
      if ( *(m_Agenda->bestGenerator()) != correctState ) {
         TRACE("The best item is not the correct one")
         updateScoresForStates(m_Agenda->bestGenerator(), &correctState, 1, -1) ;
         return ;
      }
   }

   TRACE("Outputing sentence");
   m_Agenda->sortGenerators();
   for (int i=0; i<std::min(m_Agenda->generatorSize(), nBest); ++i) {
      pGenerator = m_Agenda->generator(i) ;
      if (pGenerator) {
         pGenerator->GenerateTree( sentence , retval[i] ) ;
         if (scores) scores[i] = pGenerator->score;
      }
   }
   TRACE("Done, the highest score is: " << m_Agenda->bestGenerator()->score ) ;
   TRACE("The total time spent: " << double(clock() - total_start_time)/CLOCKS_PER_SEC) ;
}

/*---------------------------------------------------------------
 *
 * parse - do dependency parsing to a sentence
 *
 * Returns: makes a new instance of CDependencyParse
 *
 *--------------------------------------------------------------*/

void CDepParser::parse( const CStringVector &sentence , CDependencyParse *retval , int nBest , SCORE_TYPE *scores ) {

   static CDependencyParse empty ;

   assert( !m_bCoNLL );

   for (int i=0; i<nBest; ++i) {
      // clear the output sentences
      retval[i].clear();
      if (scores) scores[i] = 0; //pGenerator->score;
   }

   work(false, sentence, retval, empty, nBest, scores ) ;

}

/*---------------------------------------------------------------
 *
 * train - train the models with an example
 *
 *---------------------------------------------------------------*/

void CDepParser::train( const CDependencyParse &correct , int round ) {

   static CStringVector sentence ;
   static CDependencyParse output ;

   assert( !m_bCoNLL );
#ifndef FRAGMENTED_TREE
   assert( IsProjectiveDependencyTree(correct) ) ;
#endif
   UnparseSentence( &correct, &sentence ) ;

   // The following code does update for each processing stage
#ifndef LOCAL_LEARNING
   ++m_nTrainingRound;
   ASSERT(m_nTrainingRound == round, "Training round error") ;
#endif
   work( true , sentence , &output , correct , 1 , 0 ) ;

};

/*---------------------------------------------------------------
 *
 * extract_features - extract features from an example (counts recorded to parser model as weights)
 *
 *---------------------------------------------------------------*/

void CDepParser::extract_features(const CDependencyParse &input) {

   CStateItem item(&m_lCache);
   CStateItem tmp(&m_lCache);
   unsigned long action;
   CPackedScoreType<SCORE_TYPE, action::MAX> empty;

   // word and pos
   m_lCache.clear();
#ifdef LABELED
   m_lCacheLabel.clear();
#endif
   for (int i=0; i<input.size(); ++i) {
      //m_lCache.push_back(CTaggedWord<CTag, TAG_SEPARATOR>(input[i].word, input[i].tag));
	   m_lCache.push_back(input[i].word);
#ifdef LABELED
   m_lCacheLabel.push_back(CDependencyLabel(input[i].label));
#endif
   }

   //init gold standard morphs and lemmas
   for (int index=0; index<input.size(); ++index)
   {
	   CMorph correctMorph = conllToMorph(input[index].word, input[index].tag, input[index].feats);
	   m_lCacheMorph.push_back(correctMorph);
   }

   // make standard item
   item.clear();
   for (int i=0; i<input.size() * 2; ++i) {
#ifdef LABELED
   item.StandardMoveStep(input, m_lCacheLabel, m_lCacheMorph);
#else
   item.StandardMoveStep(input);
#endif
   }

   // extract feature now with another step less efficient yet easier here
   tmp.clear();
   while (tmp != item) {
      action = tmp.FollowMove(&item );
      getOrUpdateStackScore(&tmp, empty, action, 1, 0);
      tmp.Move(action);
   }
}

/*---------------------------------------------------------------
 *
 * initCoNLLCache
 *
 *---------------------------------------------------------------*/

template<typename CCoNLLInputOrOutput>
void CDepParser::initCoNLLCache( const CCoNLLInputOrOutput &sentence ) {

	//unneeded, as this version of the parser generates its own lemmas and feats
	/*
   m_lCacheCoNLLLemma.resize(sentence.size());
   m_lCacheCoNLLCPOS.resize(sentence.size());
   m_lCacheCoNLLFeats.resize(sentence.size());
   for (unsigned i=0; i<sentence.size(); ++i) {
      m_lCacheCoNLLLemma[i].load(_conll_or_empty(sentence.at(i).lemma));
      m_lCacheCoNLLCPOS[i].load(_conll_or_empty(sentence.at(i).ctag));
      m_lCacheCoNLLFeats[i].clear();
      if (sentence.at(i).feats != "_")
         readCoNLLFeats(m_lCacheCoNLLFeats[i], sentence.at(i).feats);
   }
   	 */

}

/*---------------------------------------------------------------
 *
 * parse_conll - do dependency parsing to a sentence
 *
 * Returns: makes a new instance of CDependencyParse
 *
 *--------------------------------------------------------------*/

void CDepParser::parse_conll( const CCoNLLInput &sentence , CCoNLLOutput *retval , int nBest, SCORE_TYPE *scores ) {

   static CDependencyParse empty ;
   static CStringVector input ;
   static CDependencyParse output[AGENDA_SIZE] ;

   assert( m_bCoNLL ) ;

   initCoNLLCache(sentence);

   sentence.toStringVector(input);

   for (int i=0; i<nBest; ++i) {
      // clear the output sentences
      retval[i].clear();
      output[i].clear();
      if (scores) scores[i] = 0; //pGenerator->score;
   }

   work(false, input, output, empty, nBest, scores ) ;

   for (int i=0; i<std::min(nBest, m_Agenda->generatorSize()); ++i) {
      // now make the conll format style output
      retval[i].fromCoNLLInput(sentence);
      retval[i].copyDependencyHeads(output[i]);
      retval[i].copyTagsFeatsAndLemmas(output[i]); //morph parser can also output this.
   }

}

/*---------------------------------------------------------------
 *
 * train_conll - train the models with an example
 *
 *---------------------------------------------------------------*/

void CDepParser::train_conll( const CCoNLLOutput &correct , int round ) {

   static CStringVector sentence ;
   static CDependencyParse output ;
   static CDependencyParse reference ;

   assert( m_bCoNLL ) ;

//todo: is this ifndef correct?
#ifndef FRAGMENTED_TREE
   assert( IsProjectiveDependencyTree(correct) ) ;
#endif

   initCoNLLCache(correct);

   correct.toDependencyTree( reference );
   UnparseSentence( &reference, &sentence ) ;

   // The following code does update for each processing stage
   m_nTrainingRound = round ;
   work( true , sentence , &output , reference , 1 , 0 ) ;

}

/*---------------------------------------------------------------
 *
 * extract_features_conll - extract features from an example
 *
 *---------------------------------------------------------------*/

void CDepParser::extract_features_conll( const CCoNLLOutput &input) {

   CDependencyParse dep;
   assert( m_bCoNLL );
   initCoNLLCache( input );
   input.toDependencyTree( dep );
   extract_features(dep);

}
