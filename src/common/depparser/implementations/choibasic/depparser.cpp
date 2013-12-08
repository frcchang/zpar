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

#include <functional>
#include "depparser.h"
#include "depparser_weight.h"

using namespace TARGET_LANGUAGE;
using namespace TARGET_LANGUAGE::depparser;

typedef CWord                             Word;
typedef CTag                              Tag;
typedef CTaggedWord<CTag, TAG_SEPARATOR>  TaggedWord;

const Word        EmptyWord("");
const Tag         NoneTag(CTag::NONE);
const TaggedWord  EmptyTaggedWord;

const int         kMaxSentenceSize  = MAX_SENTENCE_SIZE;
const int         kAgendaSize       = AGENDA_SIZE;


#define cast_weights static_cast<CWeight*>(m_weights)
#define refer_or_allocate_tuple2(x, o1, o2) do { \
  if (amount == 0) { \
    x.refer(o1, o2); \
  } else { \
    x.allocate(o1, o2); \
  } \
} while (0);
#define refer_or_allocate_tuple3(x, o1, o2, o3) do { \
  if (amount == 0) { \
    x.refer(o1, o2, o3); \
  } else { \
    x.allocate(o1, o2, o3); \
  } \
} while (0);

#define travel(x) do { \
  const CStateItem * __p = (x); \
  int __deb = 0; \
  while (__p) { \
    __deb ++; \
    __p = __p->previous_;\
  } \
  std::cout << __deb << std::endl; \
} while (0);

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
inline
void CDepParser::GetOrUpdateStackScore(
    const CStateItem * item,
    CPackedScoreType<SCORE_TYPE, action::kMax> & retval,
    const unsigned & action,
    SCORE_TYPE amount,
    int round) {
  const int &st_index = item->stackempty() ? -1 : item->stacktop(); // stack top
  const int &sth_index = st_index == -1 ? -1 : item->head(st_index); // stack top head
  const int &sthh_index = sth_index == -1 ? -1 : item->head(sth_index); // stack top head
  const int &stld_index = st_index == -1 ? -1 : item->leftdep(st_index); // leftmost dep of stack
  const int &strd_index = st_index == -1 ? -1 : item->rightdep(st_index); // rightmost dep st
  const int &stl2d_index = stld_index == -1 ? -1 : item->left2dep(st_index); // left 2ndmost dep of stack
  const int &str2d_index = strd_index == -1 ? -1 : item->right2dep(st_index); // right 2ndmost dep st
  const int &n0_index = item->size()==m_lCache.size() ? -1 : item->size(); // next
  assert(n0_index<static_cast<int>(m_lCache.size())); // the next index shouldn't exceed sentence
  const int &n0ld_index = n0_index==-1 ? -1 : item->leftdep(n0_index); // leftmost dep of next
  const int &n0l2d_index = n0ld_index==-1 ? -1 : item->left2dep(n0_index); // leftmost dep of next

  static int n1_index;
  static int n2_index;
  static int n3_index;
  n1_index = (n0_index != -1 && n0_index+1<m_lCache.size()) ? n0_index+1 : -1;
  n2_index = (n0_index != -1 && n0_index+2<m_lCache.size()) ? n0_index+2 : -1;
  n3_index = (n0_index != -1 && n0_index+3<m_lCache.size()) ? n0_index+3 : -1;

  const TaggedWord &st_word_tag    = st_index==-1 ?   EmptyTaggedWord : m_lCache[st_index];
  const TaggedWord &sth_word_tag   = sth_index==-1 ?  EmptyTaggedWord : m_lCache[sth_index];
  const TaggedWord &sthh_word_tag  = sthh_index==-1 ? EmptyTaggedWord : m_lCache[sthh_index];
  const TaggedWord &stld_word_tag  = stld_index==-1 ? EmptyTaggedWord : m_lCache[stld_index];
  const TaggedWord &strd_word_tag  = strd_index==-1 ? EmptyTaggedWord : m_lCache[strd_index];
  const TaggedWord &stl2d_word_tag = stl2d_index==-1 ? EmptyTaggedWord : m_lCache[stl2d_index];
  const TaggedWord &str2d_word_tag = str2d_index==-1 ? EmptyTaggedWord : m_lCache[str2d_index];
  const TaggedWord &n0_word_tag    = n0_index==-1 ?   EmptyTaggedWord : m_lCache[n0_index];
  const TaggedWord &n0ld_word_tag  = n0ld_index==-1 ? EmptyTaggedWord : m_lCache[n0ld_index];
  const TaggedWord &n0l2d_word_tag = n0l2d_index==-1 ? EmptyTaggedWord : m_lCache[n0l2d_index];
  const TaggedWord &n1_word_tag    = n1_index==-1 ?   EmptyTaggedWord : m_lCache[n1_index];
  const TaggedWord &n2_word_tag    = n2_index==-1 ?   EmptyTaggedWord : m_lCache[n2_index];

  const CWord &st_word = st_word_tag.word;
  const CWord &sth_word = sth_word_tag.word;
  const CWord &sthh_word = sthh_word_tag.word;
  const CWord &stld_word = stld_word_tag.word;
  const CWord &strd_word = strd_word_tag.word;
  const CWord &stl2d_word = stl2d_word_tag.word;
  const CWord &str2d_word = str2d_word_tag.word;
  const CWord &n0_word = n0_word_tag.word;
  const CWord &n0ld_word = n0ld_word_tag.word;
  const CWord &n0l2d_word = n0l2d_word_tag.word;
  const CWord &n1_word = n1_word_tag.word;
  const CWord &n2_word = n2_word_tag.word;

  const CTag &st_tag = st_word_tag.tag;
  const CTag &sth_tag = sth_word_tag.tag;
  const CTag &sthh_tag = sthh_word_tag.tag;
  const CTag &stld_tag = stld_word_tag.tag;
  const CTag &strd_tag = strd_word_tag.tag;
  const CTag &stl2d_tag = stl2d_word_tag.tag;
  const CTag &str2d_tag = str2d_word_tag.tag;
  const CTag &n0_tag = n0_word_tag.tag;
  const CTag &n0ld_tag = n0ld_word_tag.tag;
  const CTag &n0l2d_tag = n0l2d_word_tag.tag;
  const CTag &n1_tag = n1_word_tag.tag;
  const CTag &n2_tag = n2_word_tag.tag;

  const int &st_label = st_index==-1 ? CDependencyLabel::NONE : item->label(st_index);
  const int &sth_label = sth_index==-1 ? CDependencyLabel::NONE : item->label(sth_index);
  const int &stld_label = stld_index==-1 ? CDependencyLabel::NONE : item->label(stld_index);
  const int &strd_label = strd_index==-1 ? CDependencyLabel::NONE : item->label(strd_index);
  const int &stl2d_label = stl2d_index==-1 ? CDependencyLabel::NONE : item->label(stl2d_index);
  const int &str2d_label = str2d_index==-1 ? CDependencyLabel::NONE : item->label(str2d_index);
  const int &n0ld_label = n0ld_index==-1 ? CDependencyLabel::NONE : item->label(n0ld_index);
  const int &n0l2d_label = n0l2d_index==-1 ? CDependencyLabel::NONE : item->label(n0l2d_index);

  static int st_n0_dist;
  //st_n0_dist = encodeLinkDistance(st_index, n0_index);

  const int st_rarity = st_index==-1?0:item->rightarity(st_index);
  const int st_larity = st_index==-1?0:item->leftarity(st_index);
  const int n0_larity = n0_index==-1?0:item->leftarity(n0_index);

  const CSetOfTags<CDependencyLabel> &st_rtagset = st_index==-1?CSetOfTags<CDependencyLabel>():item->righttagset(st_index);
  const CSetOfTags<CDependencyLabel> &st_ltagset = st_index==-1?CSetOfTags<CDependencyLabel>():item->lefttagset(st_index);
  const CSetOfTags<CDependencyLabel> &n0_ltagset = n0_index==-1?CSetOfTags<CDependencyLabel>():item->lefttagset(n0_index);

  static CTwoTaggedWords st_word_tag_n0_word_tag ;
  static CTwoWords st_word_n0_word ;
  if ( amount == 0 ) {
     st_word_tag_n0_word_tag.refer( &st_word_tag, &n0_word_tag );
     st_word_n0_word.refer( &st_word, &n0_word );
  }
  else {
     st_word_tag_n0_word_tag.allocate( st_word_tag, n0_word_tag );
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

  // single
  if (st_index != -1) {
    cast_weights->m_mapSTw.getOrUpdateScore(
        retval, st_word,     action, m_nScoreIndex, amount, round);
    cast_weights->m_mapSTt.getOrUpdateScore(
        retval, st_tag,      action, m_nScoreIndex, amount, round);
    cast_weights->m_mapSTwt.getOrUpdateScore(
        retval, st_word_tag, action, m_nScoreIndex, amount, round);
  }

  if (n0_index != -1) {
    cast_weights->m_mapN0w.getOrUpdateScore(
        retval, n0_word,     action, m_nScoreIndex, amount, round);
    cast_weights->m_mapN0t.getOrUpdateScore(
        retval, n0_tag,      action, m_nScoreIndex, amount, round);
    cast_weights->m_mapN0wt.getOrUpdateScore(
        retval, n0_word_tag, action, m_nScoreIndex, amount, round);
  }

  if (n1_index != -1) {
     cast_weights->m_mapN1w.getOrUpdateScore(
         retval, n1_word,     action, m_nScoreIndex, amount, round);
     cast_weights->m_mapN1t.getOrUpdateScore(
         retval, n1_tag,      action, m_nScoreIndex, amount, round);
     cast_weights->m_mapN1wt.getOrUpdateScore(
         retval, n1_word_tag, action, m_nScoreIndex, amount, round);
  }

  if (n2_index != -1) {
     cast_weights->m_mapN2w.getOrUpdateScore(
         retval, n2_word,     action, m_nScoreIndex, amount, round);
     cast_weights->m_mapN2t.getOrUpdateScore(
         retval, n2_tag,      action, m_nScoreIndex, amount, round);
     cast_weights->m_mapN2wt.getOrUpdateScore(
         retval, n2_word_tag, action, m_nScoreIndex, amount, round);
  }

  if (sth_index != -1) {
     cast_weights->m_mapSTHw.getOrUpdateScore(
         retval, sth_word,  action, m_nScoreIndex, amount, round);
     cast_weights->m_mapSTHt.getOrUpdateScore(
         retval, sth_tag,   action, m_nScoreIndex, amount, round);
     cast_weights->m_mapSTi.getOrUpdateScore(
         retval, st_label,  action, m_nScoreIndex, amount, round);
  }

  if (sthh_index != -1) {
     cast_weights->m_mapSTHHw.getOrUpdateScore(
         retval, sthh_word, action, m_nScoreIndex, amount, round);
     cast_weights->m_mapSTHHt.getOrUpdateScore(
         retval, sthh_tag,  action, m_nScoreIndex, amount, round);
     cast_weights->m_mapSTHi.getOrUpdateScore(
         retval, sth_label, action, m_nScoreIndex, amount, round);
  }

  if (stld_index != -1) {
     cast_weights->m_mapSTLDw.getOrUpdateScore(
         retval, stld_word,   action, m_nScoreIndex, amount, round);
     cast_weights->m_mapSTLDt.getOrUpdateScore(
         retval, stld_tag,    action, m_nScoreIndex, amount, round);
     cast_weights->m_mapSTLDi.getOrUpdateScore(
         retval, stld_label,  action, m_nScoreIndex, amount, round);
  }

  if (strd_index != -1) {
     cast_weights->m_mapSTRDw.getOrUpdateScore(
         retval, strd_word,  action, m_nScoreIndex, amount, round);
     cast_weights->m_mapSTRDt.getOrUpdateScore(
         retval, strd_tag,   action, m_nScoreIndex, amount, round);
     cast_weights->m_mapSTRDi.getOrUpdateScore(
         retval, strd_label, action, m_nScoreIndex, amount, round);
  }

  if (n0ld_index != -1) {
     cast_weights->m_mapN0LDw.getOrUpdateScore(
         retval, n0ld_word,  action, m_nScoreIndex, amount, round);
     cast_weights->m_mapN0LDt.getOrUpdateScore(
         retval, n0ld_tag,   action, m_nScoreIndex, amount, round);
     cast_weights->m_mapN0LDi.getOrUpdateScore(
         retval, n0ld_label, action, m_nScoreIndex, amount, round);
  }

  if (stl2d_index != -1) {
     cast_weights->m_mapSTL2Dw.getOrUpdateScore(
         retval, stl2d_word,  action, m_nScoreIndex, amount, round);
     cast_weights->m_mapSTL2Dt.getOrUpdateScore(
         retval, stl2d_tag,   action, m_nScoreIndex, amount, round);
     cast_weights->m_mapSTL2Di.getOrUpdateScore(
         retval, stl2d_label, action, m_nScoreIndex, amount, round);
  }

  if (str2d_index != -1) {
     cast_weights->m_mapSTR2Dw.getOrUpdateScore(
         retval, str2d_word,  action, m_nScoreIndex, amount, round);
     cast_weights->m_mapSTR2Dt.getOrUpdateScore(
         retval, str2d_tag,   action, m_nScoreIndex, amount, round);
     cast_weights->m_mapSTR2Di.getOrUpdateScore(
         retval, str2d_label, action, m_nScoreIndex, amount, round);
  }

  if (n0l2d_index != -1) {
     cast_weights->m_mapN0L2Dw.getOrUpdateScore(
         retval, n0l2d_word,  action, m_nScoreIndex, amount, round);
     cast_weights->m_mapN0L2Dt.getOrUpdateScore(
         retval, n0l2d_tag,   action, m_nScoreIndex, amount, round);
     cast_weights->m_mapN0L2Di.getOrUpdateScore(
         retval, n0l2d_label, action, m_nScoreIndex, amount, round);
  }

  // s0 and n0
  if (st_index != -1) {
     cast_weights->m_mapSTwtN0wt.getOrUpdateScore(
         retval, st_word_tag_n0_word_tag, action, m_nScoreIndex, amount, round);
     refer_or_allocate_tuple3(word_word_tag, &st_word, &n0_word, &st_tag);

     cast_weights->m_mapSTwtN0w.getOrUpdateScore(
         retval, word_word_tag, action, m_nScoreIndex, amount, round);
     refer_or_allocate_tuple3(word_word_tag, &st_word, &n0_word, &n0_tag);

     cast_weights->m_mapSTwN0wt.getOrUpdateScore(
         retval, word_word_tag, action, m_nScoreIndex, amount, round );
     refer_or_allocate_tuple3(word_tag_tag, &st_word, &st_tag, &n0_tag);

     cast_weights->m_mapSTwtN0t.getOrUpdateScore(
         retval, word_tag_tag, action, m_nScoreIndex, amount, round );
     refer_or_allocate_tuple3(word_tag_tag, &n0_word, &st_tag, &n0_tag);

     cast_weights->m_mapSTtN0wt.getOrUpdateScore(
         retval, word_tag_tag, action, m_nScoreIndex, amount, round);
     cast_weights->m_mapSTwN0w.getOrUpdateScore(
         retval, st_word_n0_word, action, m_nScoreIndex, amount, round );
     cast_weights->m_mapSTtN0t.getOrUpdateScore(
         retval, CTagSet<CTag, 2>(encodeTags(st_tag,n0_tag)),
         action, m_nScoreIndex, amount, round );
  }

  if (st_index != -1 && n0_index != -1) {
     cast_weights->m_mapN0tN1t.getOrUpdateScore(
         retval, CTagSet<CTag, 2>(encodeTags(n0_tag,n1_tag)),
         action, m_nScoreIndex, amount, round );
     cast_weights->m_mapN0tN1tN2t.getOrUpdateScore(
         retval, CTagSet<CTag, 3>(encodeTags(n0_tag,n1_tag,n2_tag)),
         action, m_nScoreIndex, amount, round );
     cast_weights->m_mapSTtN0tN1t.getOrUpdateScore(
         retval, CTagSet<CTag, 3>(encodeTags(st_tag,n0_tag,n1_tag)),
         action, m_nScoreIndex, amount, round );
     cast_weights->m_mapSTtN0tN0LDt.getOrUpdateScore(
         retval, CTagSet<CTag, 3>(encodeTags(st_tag,n0_tag,n0ld_tag)),
         action, m_nScoreIndex, amount, round );
     cast_weights->m_mapN0tN0LDtN0L2Dt.getOrUpdateScore(
         retval, CTagSet<CTag, 3>(encodeTags(n0_tag,n0ld_tag,n0l2d_tag)),
         action, m_nScoreIndex, amount, round );
  }

  if (st_index != -1) {
     cast_weights->m_mapSTHtSTtN0t.getOrUpdateScore(
         retval, CTagSet<CTag, 3>(encodeTags(sth_tag,st_tag,n0_tag)),
         action, m_nScoreIndex, amount, round);
     cast_weights->m_mapSTHHtSTHtSTt.getOrUpdateScore(
         retval, CTagSet<CTag, 3>(encodeTags(sthh_tag, sth_tag,st_tag)),
         action, m_nScoreIndex, amount, round);
     cast_weights->m_mapSTtSTLDtN0t.getOrUpdateScore(
         retval, CTagSet<CTag, 3>(encodeTags(st_tag,stld_tag,n0_tag)),
         action, m_nScoreIndex, amount, round);
     cast_weights->m_mapSTtSTLDtSTL2Dt.getOrUpdateScore(
         retval, CTagSet<CTag, 3>(encodeTags(st_tag,stld_tag,stl2d_tag)),
         action, m_nScoreIndex, amount, round );
     cast_weights->m_mapSTtSTRDtN0t.getOrUpdateScore(
         retval, CTagSet<CTag, 3>(encodeTags(st_tag,strd_tag,n0_tag)),
         action, m_nScoreIndex, amount, round );
     cast_weights->m_mapSTtSTRDtSTR2Dt.getOrUpdateScore(
         retval, CTagSet<CTag, 3>(encodeTags(st_tag,strd_tag,str2d_tag)),
         action, m_nScoreIndex, amount, round );
  }

  // distance
  /*if (st_index!=-1 && n0_index!=-1) {
     refer_or_allocate_tuple2(word_int, &st_word, &st_n0_dist);
     cast_weights->m_mapSTwd.getOrUpdateScore(
         retval, word_int, action, m_nScoreIndex, amount, round);
     refer_or_allocate_tuple2(tag_int, &st_tag, &st_n0_dist);
     cast_weights->m_mapSTtd.getOrUpdateScore(
         retval, tag_int, action, m_nScoreIndex, amount, round );
     refer_or_allocate_tuple2(word_int, &n0_word, &st_n0_dist);
     cast_weights->m_mapN0wd.getOrUpdateScore(
         retval, word_int, action, m_nScoreIndex, amount, round);
     refer_or_allocate_tuple2(tag_int, &n0_tag, &st_n0_dist);
     cast_weights->m_mapN0td.getOrUpdateScore(
         retval, tag_int, action, m_nScoreIndex, amount, round);
     refer_or_allocate_tuple3(word_word_int, &st_word, &n0_word, &st_n0_dist);
     cast_weights->m_mapSTwN0wd.getOrUpdateScore(
         retval, word_word_int, action, m_nScoreIndex, amount, round);
     refer_or_allocate_tuple3(tag_tag_int, &st_tag, &n0_tag, &st_n0_dist);
     cast_weights->m_mapSTtN0td.getOrUpdateScore(
         retval, tag_tag_int, action, m_nScoreIndex, amount, round);
  }*/

  // st arity
  if (st_index != -1) {
    refer_or_allocate_tuple2(word_int, &st_word, &st_rarity);
    cast_weights->m_mapSTwra.getOrUpdateScore(
        retval, word_int, action, m_nScoreIndex, amount, round);
    refer_or_allocate_tuple2(tag_int, &st_tag, &st_rarity);
    cast_weights->m_mapSTtra.getOrUpdateScore(
        retval, tag_int, action, m_nScoreIndex, amount, round );
    refer_or_allocate_tuple2(word_int, &st_word, &st_larity);
    cast_weights->m_mapSTwla.getOrUpdateScore(
        retval, word_int, action, m_nScoreIndex, amount, round);
    refer_or_allocate_tuple2(tag_int, &st_tag, &st_larity);
    cast_weights->m_mapSTtla.getOrUpdateScore(
        retval, tag_int, action, m_nScoreIndex, amount, round );
  }

  // n0 arity
  if (n0_index!=-1) {
     refer_or_allocate_tuple2(word_int, &n0_word, &n0_larity);
     cast_weights->m_mapN0wla.getOrUpdateScore(
         retval, word_int, action, m_nScoreIndex, amount, round);
     refer_or_allocate_tuple2(tag_int, &n0_tag, &n0_larity);
     cast_weights->m_mapN0tla.getOrUpdateScore(
         retval, tag_int, action, m_nScoreIndex, amount, round );
  }

  // st labelset
  if (st_index != -1){
     refer_or_allocate_tuple2(word_tagset, &st_word, &st_rtagset);
     cast_weights->m_mapSTwrp.getOrUpdateScore(
         retval, word_tagset, action, m_nScoreIndex, amount, round);
     refer_or_allocate_tuple2(tag_tagset, &st_tag, &st_rtagset);
     cast_weights->m_mapSTtrp.getOrUpdateScore(
         retval, tag_tagset, action, m_nScoreIndex, amount, round );

     refer_or_allocate_tuple2(word_tagset, &st_word, &st_ltagset);
     cast_weights->m_mapSTwlp.getOrUpdateScore(
         retval, word_tagset, action, m_nScoreIndex, amount, round);
     refer_or_allocate_tuple2(tag_tagset, &st_tag, &st_ltagset);
     cast_weights->m_mapSTtlp.getOrUpdateScore(
         retval, tag_tagset, action, m_nScoreIndex, amount, round );
  }

  // n0 labelset
  if (n0_index != -1){
     refer_or_allocate_tuple2(word_tagset, &n0_word, &n0_ltagset);
     cast_weights->m_mapN0wlp.getOrUpdateScore(
         retval, word_tagset, action, m_nScoreIndex, amount, round);
     refer_or_allocate_tuple2(tag_tagset, &n0_tag, &n0_ltagset);
     cast_weights->m_mapN0tlp.getOrUpdateScore(
         retval, tag_tagset, action, m_nScoreIndex, amount, round );
  }

  if (m_bCoNLL) {
     static unsigned i;
     if (st_index!=-1) {
        if (!m_lCacheCoNLLLemma[st_index].empty()) {
          cast_weights->m_mapSTl.getOrUpdateScore(
              retval, m_lCacheCoNLLLemma[st_index], action, m_nScoreIndex, amount, round);
        }
        if (m_lCacheCoNLLCPOS[st_index] != CCoNLLCPOS()) {
          cast_weights->m_mapSTc.getOrUpdateScore(
              retval, m_lCacheCoNLLCPOS[st_index], action, m_nScoreIndex, amount, round);
        }
        for (i=0; i<m_lCacheCoNLLFeats[st_index].size(); ++i) {
          cast_weights->m_mapSTf.getOrUpdateScore(
              retval, m_lCacheCoNLLFeats[st_index][i], action, m_nScoreIndex, amount, round);
        }
     } // if (st_index!=-1)

     if (n0_index!=-1) {
        if (!m_lCacheCoNLLLemma[n0_index].empty()) {
          cast_weights->m_mapN0l.getOrUpdateScore(
              retval, m_lCacheCoNLLLemma[n0_index], action, m_nScoreIndex, amount, round);
        }

        if (m_lCacheCoNLLCPOS[n0_index] != CCoNLLCPOS()) {
          cast_weights->m_mapN0c.getOrUpdateScore(
              retval, m_lCacheCoNLLCPOS[n0_index], action, m_nScoreIndex, amount, round);
        }

        for (i=0; i<m_lCacheCoNLLFeats[n0_index].size(); ++i) {
          cast_weights->m_mapN0f.getOrUpdateScore(
              retval, m_lCacheCoNLLFeats[n0_index][i], action, m_nScoreIndex, amount, round);
        }
     } // if (n0_index!=-1)

     if (n1_index!=-1) {
        if (!m_lCacheCoNLLLemma[n1_index].empty()) {
          cast_weights->m_mapN1l.getOrUpdateScore(
              retval, m_lCacheCoNLLLemma[n1_index], action, m_nScoreIndex, amount, round);
        }

        if (m_lCacheCoNLLCPOS[n1_index] != CCoNLLCPOS()) {
          cast_weights->m_mapN1c.getOrUpdateScore(
              retval, m_lCacheCoNLLCPOS[n1_index], action, m_nScoreIndex, amount, round);
        }

        for (i=0; i<m_lCacheCoNLLFeats[n1_index].size(); ++i) {
          cast_weights->m_mapN1f.getOrUpdateScore(
              retval, m_lCacheCoNLLFeats[n1_index][i], action, m_nScoreIndex, amount, round);
        }
     } // if (n1_index!=-1)
  }
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

void CDepParser::updateScores(
    const CDependencyParse & parsed,
    const CDependencyParse & correct,
    int round ) {
  THROW("depparser.cpp: updateScores unsupported");
}

/*---------------------------------------------------------------
 *
 * updateScoreForState - update a single positive or negative outout
 *
 *--------------------------------------------------------------*/

inline
void CDepParser::updateScoreForState(
    const CStateItem &from,
    const CStateItem *outout,
    const SCORE_TYPE &amount ) {
  static CStateItem item;
  static unsigned action;
  static CPackedScoreType<SCORE_TYPE, action::kMax> empty;
  item = from;
  while ( item != *outout ) {
     action = item.FollowMove( outout );
     GetOrUpdateStackScore( &item, empty, action, amount, m_nTrainingRound);
     item.Move( action );
  }
}

/*---------------------------------------------------------------
 *
 * updateScoresForStates - update scores for states
 *
 *--------------------------------------------------------------*/

void CDepParser::UpdateScoresForStates(
    const CStateItem *  predicated,
    const CStateItem *  correct,
    SCORE_TYPE          amount_add,
    SCORE_TYPE          amount_subtract) {

  static CPackedScoreType<SCORE_TYPE, action::kMax> empty;

  // do not update those steps where they are correct
  const CStateItem * predicated_state_chain[kMaxSentenceSize * kMaxSentenceSize];
  const CStateItem * correct_state_chain[kMaxSentenceSize * kMaxSentenceSize];

  int num_predicated_states = 0;
  int num_correct_states    = 0;

  for (const CStateItem * p = predicated; p; p = p->previous_) {
    predicated_state_chain[num_predicated_states] = p;
    ++ num_predicated_states;
  }

  for (const CStateItem * p = correct; p; p = p->previous_) {
    correct_state_chain[num_correct_states] = p;
    ++ num_correct_states;
  }

  ASSERT(num_correct_states == num_predicated_states,
      "Number of predicated action don't equals the correct one");

  int i;
  for (i = num_correct_states - 1; i >= 0; -- i) {
    // if the action is different, do the update
    unsigned predicated_action  = predicated_state_chain[i]->last_action;
    unsigned correct_action     = correct_state_chain[i]->last_action;
    if (predicated_action != correct_action) {
      break;
    }
  }

  // for (int j = i + 1; j > 0; -- j) { std::cout << (*predicated_state_chain[j]); }
  // for (int j = i + 1; j > 0; -- j) { std::cout << (*correct_state_chain[j]);    }

  for (i = i + 1; i > 0; -- i) {
    unsigned predicated_action  = predicated_state_chain[i - 1]->last_action;
    unsigned correct_action     = correct_state_chain[i - 1]->last_action;

    GetOrUpdateStackScore(predicated_state_chain[i],
        empty, predicated_action, amount_subtract, m_nTrainingRound);
    GetOrUpdateStackScore(correct_state_chain[i],
        empty, correct_action,    amount_add,      m_nTrainingRound);
  }
  m_nTotalErrors++;
}

/* --------------------------------------------------------------
 *
 *
 * ------------------------------------------------------------- */
inline
void CDepParser::idle(
    const CStateItem * item,
    const CPackedScoreType<SCORE_TYPE, action::kMax> & scores) {
  static action::CScoredAction scored_action;
  scored_action.action = action::kIdle;
  scored_action.score = item->score + scores[scored_action.action];
  m_Beam->insertItem(&scored_action);
}

/*---------------------------------------------------------------
 *
 * reduce - helper function
 *
 *--------------------------------------------------------------*/
inline
void CDepParser::reduce(
    const CStateItem * item,
    const CPackedScoreType<SCORE_TYPE, action::kMax> & scores) {
  static action::CScoredAction scored_action;
  // update stack score
  scored_action.action = action::kReduce;
  scored_action.score = item->score + scores[scored_action.action];
  m_Beam->insertItem(&scored_action);
}

/*---------------------------------------------------------------
 *
 * arcleft - helping function
 *
 *--------------------------------------------------------------*/
inline
void CDepParser::arcleft(
    const CStateItem * item,
    const CPackedScoreType<SCORE_TYPE, action::kMax> & scores) {
  static action::CScoredAction scored_action;
#ifdef LABELED
  for (unsigned label = CDependencyLabel::FIRST;
      label < CDependencyLabel::COUNT; ++ label) {
    scored_action.action = action::EncodeAction(action::kArcLeft, label);
    scored_action.score = item->score + scores[scored_action.action];
    m_Beam->insertItem(&scored_action);
  }
#else
  scored_action.action = action::kArcLeft;
  scored_action.score = item->score + scores[scored_action.action];
  m_Beam->insertItem(&scored_action);
#endif
}

/*---------------------------------------------------------------
 *
 * arcright - helping function
 *
 *--------------------------------------------------------------*/
inline
void CDepParser::arcright(
    const CStateItem *item,
    const CPackedScoreType<SCORE_TYPE, action::kMax> & scores) {
  static action::CScoredAction scored_action;
#ifdef LABELED
  for (unsigned label = CDependencyLabel::FIRST;
      label < CDependencyLabel::COUNT; ++ label) {
    scored_action.action = action::EncodeAction(action::kArcRight, label);
    scored_action.score = item->score + scores[scored_action.action];
    m_Beam->insertItem(&scored_action);
  }
#else
  scored_action.action = action::kArcRight;
  scored_action.score = item->score + scores[scored_action.action];
  m_Beam->insertItem(&scored_action);
#endif
}

/*---------------------------------------------------------------
 *
 * shift - help function
 *
 *--------------------------------------------------------------*/

inline
void CDepParser::shift(
    const CStateItem *item,
    const CPackedScoreType<SCORE_TYPE, action::kMax> & scores) {
  static action::CScoredAction scored_action;
  scored_action.action = action::kShift;
  scored_action.score  = item->score + scores[scored_action.action];
  m_Beam->insertItem(&scored_action);
}

/*---------------------------------------------------------------
 *
 * poproot - help function
 *
 *--------------------------------------------------------------*/
inline
void CDepParser::poproot(
    const CStateItem *item,
    const CPackedScoreType<SCORE_TYPE, action::kMax> & scores) {
  static action::CScoredAction scored_action;
  scored_action.action = action::kPopRoot;
  scored_action.score  = item->score + scores[scored_action.action];
  m_Beam->insertItem(&scored_action);
}

/* --------------------------------------------------------------
 *
 * nopass - help function
 *
 * -------------------------------------------------------------- */
inline
void CDepParser::nopass(
    const CStateItem * item,
    const CPackedScoreType<SCORE_TYPE, action::kMax> & scores) {
  static action::CScoredAction scored_action;
  scored_action.action = action::kNoPass;
  scored_action.score  = item->score + scores[scored_action.action];
  m_Beam->insertItem(&scored_action);
}

/* -------------------------------------------------------------
 *
 * left pass - help function
 *
 * ------------------------------------------------------------- */
inline
void CDepParser::leftpass(
    const CStateItem * item,
    const CPackedScoreType<SCORE_TYPE, action::kMax> & scores) {
  static action::CScoredAction scored_action;
#ifdef LABELED
  for (unsigned label = CDependencyLabel::FIRST;
      label < CDependencyLabel::COUNT; ++ label) {
    scored_action.action = action::EncodeAction(action::kLeftPass, label);
    scored_action.score  = item->score + scores[scored_action.action];
    m_Beam->insertItem(&scored_action);
  }
#else
  scored_action.action = action::kLeftPass;
  scored_action.score  = item->score + scores[scored_action.action];
  m_Beam->insertItem(&scored_action);
#endif  //  end for LABELED
}

/* -------------------------------------------------------------
 *
 * arcright - the helper function
 *
 * ------------------------------------------------------------- */
inline
void CDepParser::rightpass(
    const CStateItem * item,
    const CPackedScoreType<SCORE_TYPE, action::kMax> & scores) {
  static action::CScoredAction scored_action;
#ifdef LABELED
  for (unsigned label = CDependencyLabel::FIRST;
      label < CDependencyLabel::COUNT; ++ label) {
    scored_action.action = action::EncodeAction(action::kRightPass, label);
    scored_action.score  = item->score + scores[scored_action.action];
    m_Beam->insertItem(&scored_action);
  }
#else
  scored_action.action = action::kRightPass;
  scored_action.score  = item->score + scores[scored_action.action];
  m_Beam->insertItem(&scored_action);
#endif  //  end for LABELED
}

bool StateHeapMore(const CStateItem & x, const CStateItem & y) {
  return x.score > y.score;
}

int CDepParser::insert_into_beam(
    const CStateItem * item,
    CStateItem *       beam,
    const int          current_beam_size,
    const int          max_beam_size) {
  if (current_beam_size == max_beam_size) {
    if (*item > *beam) {
      std::pop_heap(beam, beam + max_beam_size, StateHeapMore);
      *(beam + max_beam_size - 1) = *item;
      std::push_heap(beam, beam + max_beam_size, StateHeapMore);
    }
    return 0;
  }

  *(beam + current_beam_size) = *(item);
  std::push_heap(beam, beam + current_beam_size + 1, StateHeapMore);
  return 1;
}

/*
 * enumerate all the possible transition from the given state
 */
void CDepParser::transit(
    const CStateItem * generator,
    const CPackedScoreType<SCORE_TYPE, action::kMax> & packed_scores) {

  // no possible transtion, return to guarentee
  if (generator->terminated()) {
    idle(generator, packed_scores);
    return;
  }

  // precondition:
  //  1. stack is not empty;
  //  2. stack size equals 1
  if (generator->complete()) {
    poproot(generator, packed_scores);
  }


  // std::cout << " * generator: " << (*generator);
  // precondition: 1. buffer is not empty
  if (generator->size() < generator->len_) {
    shift(generator, packed_scores);
  }

  // precondition:
  //  1. stack is not empty
  //  2. stack top has head.
  if ((false == generator->stackempty()) &&
      (DEPENDENCY_LINK_NO_HEAD != generator->head(generator->stacktop()))) {
    reduce(generator, packed_scores);
  }

  // precondition: 1. stack is not empty
  if (false == generator->stackempty()) {
    nopass(generator, packed_scores);
  }

  // precondition:
  //  1. stack is not empty;
  //  2. buffer is not empty
  if (false == generator->stackempty() &&
      generator->size() < generator->len_ &&
      (DEPENDENCY_LINK_NO_HEAD == generator->head(generator->stacktop()))) {
    arcleft(generator, packed_scores);
  }

  // precondition:
  //  1. stack is not empty;
  //  2. buffer is not empty
  if (false == generator->stackempty() &&
      generator->size() < generator->len_ &&
      (DEPENDENCY_LINK_NO_HEAD == generator->head(generator->size()))) {
    arcright(generator, packed_scores);
  }

  // precondition:
  //  1. stack is not empty;
  //  2. buffer is not empty
  if (false == generator->stackempty() &&
      generator->size() < generator->len_ &&
      (DEPENDENCY_LINK_NO_HEAD == generator->head(generator->stacktop()))) {
    leftpass(generator, packed_scores);
  }


  if (false == generator->stackempty() &&
      generator->size() < generator->len_ &&
      (DEPENDENCY_LINK_NO_HEAD == generator->head(generator->size()))) {
    rightpass(generator, packed_scores);
  }
}

/*---------------------------------------------------------------
 *
 * work - the working process shared by training and parsing
 *
 * Returns: makes a new instance of CDependencyParse
 *
 *--------------------------------------------------------------*/

int CDepParser::work(
    const bool               bTrain,
    const CTwoStringVector & sentence,
    CDependencyParse *       retval,
    const CDependencyParse & correct,
    int                      nBest,
    SCORE_TYPE *             scores) {

#ifdef DEBUG
  clock_t total_start_time = clock();
#endif
  const int length           = sentence.size();
  const int max_round        = (length + 1) * (length + 1);
  const int max_lattice_size = (kAgendaSize + 1) * max_round;

  // the memory pool
  CStateItem * lattice = new CStateItem[max_lattice_size];

  for (int i = 0; i < max_lattice_size; ++ i) { lattice[i].len_ = length; }

  CStateItem * lattice_index[max_round];
  const CStateItem * correct_state;       // use to point the position in the
                                          // lattice as the correct_state

  lattice_index[0] = lattice;
  lattice_index[1] = lattice_index[0] + 1;

  // clear the start state.
  lattice[0].clear();
  correct_state = lattice;
  // used only for training
  bool bCorrect ;              // used in learning for early update
  static CPackedScoreType<SCORE_TYPE, action::kMax> packed_scores;

  ASSERT(length < kMaxSentenceSize,
      "The size of the sentence is larger than the system configuration.");

  TRACE("Initialising the decoding process...");
  m_lCache.clear();
  for (int index = 0; index < length; ++ index) {
     m_lCache.push_back(TaggedWord(sentence[index].first , sentence[index].second));
     // there use to be a rule check here, but it is not necessary in non-projective
     // parser.
  }

  // verifying supertags
  if (m_supertags) {
     ASSERT(m_supertags->getSentenceSize() == length,
         "Sentence size does not match supertags size");
  }

#ifdef LABELED
  m_lCacheLabel.clear();
  if (bTrain) {
    for (int index=0; index < length; ++ index) {
      // std::cout << correct[index].label << std::endl;
      m_lCacheLabel.push_back(CDependencyLabel(correct[index].label));
    }
  }
#endif

  TRACE("Decoding started");
  int num_results = 0;
  int round = 0;
  bool reach_standard_finished = false;
  // loop with the next word to process in the sentence,
  // `round` represent the generators, and the condidates should be inserted
  // into the `round + 1`
  for (round = 1; round < (length+1) * (length+1); ++ round) {
    if (bTrain) { bCorrect = false; }
    if (lattice_index[round - 1] == lattice_index[round]) {
      // there is nothing in generators, the proning has cut all legel
      // generator, so raise an exception and return
      WARNING("Parsing Failed!");
      return -1;
    }

    int current_beam_size = 0;
    // loop over the generator states
    // std::cout << "round : " << round << std::endl;
    for (const CStateItem * generator = lattice_index[round - 1];
        generator != lattice_index[round];
        ++ generator) {

      m_Beam->clear();  packed_scores.reset();
      GetOrUpdateStackScore(generator, packed_scores, action::kNoAction);

      transit(generator, packed_scores);

      for (unsigned i = 0; i < m_Beam->size(); ++ i) {
        CStateItem candidate; candidate = (*generator);
        // generate candidate state according to the states in beam
        candidate.score     = m_Beam->item(i)->score;
        candidate.previous_ = generator;
        candidate.Move(m_Beam->item(i)->action);

        current_beam_size += insert_into_beam(
            &candidate,
            lattice_index[round],
            current_beam_size,
            kAgendaSize);
      }
    }

    lattice_index[round + 1] = lattice_index[round] + current_beam_size;

    bool all_finished = true;
    for (CStateItem * p = lattice_index[round];
        p != lattice_index[round + 1]; ++ p) {
      if (false == p->terminated()) { all_finished = false; }
    }

    if (all_finished) {
      break;
    }

    if (bTrain) {
      CStateItem next_correct_state;
      next_correct_state = (*correct_state);
      next_correct_state.StandardMoveStep(correct
#ifdef LABELED
          , m_lCacheLabel
#endif  //  end for LABELED
          );
      next_correct_state.previous_ = correct_state;

      correct_state = NULL;
      bCorrect = false;
      for (CStateItem * p = lattice_index[round];
          p != lattice_index[round + 1];
          ++ p) {
        if (next_correct_state == (*p)) {
          correct_state = p;
          bCorrect = true;
        }
      }
#ifdef EARLY_UPDATE
      if (!bCorrect) {
        TRACE("ERROR at the " << next_correct_state.size() << "th word;"
            << " Total is " << correct.size());
        CStateItem * best_generator = lattice_index[round];
        for (CStateItem * p = lattice_index[round];
            p != lattice_index[round + 1];
            ++ p) {
          if (best_generator->score < p->score) {
            best_generator = p;
          }
        }

        // std::cout << "predicated: " << (*best_generator);
        // std::cout << "correct:    " << (next_correct_state);
        UpdateScoresForStates(best_generator, &next_correct_state, 1, -1);
        delete [] lattice;

        return -1;
      }
#endif  //  end for EARLY_UPDATE

      if (correct_state->terminated()) {
        reach_standard_finished = true;
        break;
      }
    }
  }

  if (!retval) {
    delete [] lattice;
    return -1;
  }

  TRACE("Output sentence");
  std::sort(lattice_index[round - 1], lattice_index[round], std::greater<CStateItem>());
  num_results = lattice_index[round] - lattice_index[round - 1];
  for (int i = 0; i < std::min(num_results, nBest); ++ i) {
    (lattice_index[round - 1] + 1)->GenerateTree(sentence, retval[i]);
    if (scores) { scores[i] = (lattice_index[round - 1] + i)->score; }
  }

  TRACE("Done, total time spent: " << double(clock() - total_start_time) / CLOCKS_PER_SEC);
  delete [] lattice;
  return num_results;
}

/*---------------------------------------------------------------
 *
 * parse - do dependency parsing to a sentence
 *
 * Returns: makes a new instance of CDependencyParse
 *
 *--------------------------------------------------------------*/

void CDepParser::parse(
    const CTwoStringVector & sentence,
    CDependencyParse *       retval,
    int                      nBest,
    SCORE_TYPE *             scores) {

  static CDependencyParse empty ;
  assert( !m_bCoNLL );

  for (int i=0; i < nBest; ++ i) {
     // clear the outout sentences
     retval[i].clear();
     if (scores) {
       scores[i] = 0; //pGenerator->score;
     }
  }

  work(false, sentence, retval, empty, nBest, scores );

}

/*---------------------------------------------------------------
 *
 * train - train the models with an example
 *
 *---------------------------------------------------------------*/

void CDepParser::train(
    const CDependencyParse &correct , int round ) {
  static CTwoStringVector sentence ;

  assert(!m_bCoNLL);
  UnparseSentence( &correct, &sentence );

  // The following code does update for each processing stage
#ifndef LOCAL_LEARNING
  ++m_nTrainingRound;
  ASSERT(m_nTrainingRound == round, "Training round error");
#endif
  work(true , sentence , NULL, correct , 1 , 0);
};

/*---------------------------------------------------------------
 *
 * extract_features - extract features from an example (counts
 * recorded to parser model as weights)
 *
 *---------------------------------------------------------------*/

void CDepParser::extract_features(const CDependencyParse &input) {

  CStateItem item;
  CStateItem tmp;
  unsigned action;
  CPackedScoreType<SCORE_TYPE, action::kMax> empty;

  // word and pos
  m_lCache.clear();
#ifdef LABELED
  m_lCacheLabel.clear();
#endif
  for (int i=0; i<input.size(); ++i) {
    m_lCache.push_back(TaggedWord(input[i].word, input[i].tag));
#ifdef LABELED
    m_lCacheLabel.push_back(CDependencyLabel(input[i].label));
#endif
  }

  // make standard item
  item.clear();
  for (int i=0; i<input.size() * 2; ++i) {
#ifdef LABELED
    item.StandardMoveStep(input, m_lCacheLabel);
#else
    item.StandardMoveStep(input);
#endif
  }

  // extract feature now with another step less efficient yet easier here
  tmp.clear();
  while (tmp != item) {
    action = tmp.FollowMove(&item);
    GetOrUpdateStackScore(&tmp, empty, action, 1, 0);
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
}

/*---------------------------------------------------------------
 *
 * parse_conll - do dependency parsing to a sentence
 *
 * Returns: makes a new instance of CDependencyParse
 *
 *--------------------------------------------------------------*/

void CDepParser::parse_conll(
    const CCoNLLInput & sentence,
    CCoNLLOutput *      retval,
    int                 nBest,
    SCORE_TYPE *        scores) {

  static CDependencyParse empty;
  static CTwoStringVector input;
  static CDependencyParse outout[AGENDA_SIZE];

  assert(m_bCoNLL);
  initCoNLLCache(sentence);
  sentence.toTwoStringVector(input);

  for (int i=0; i<nBest; ++i) {
     // clear the outout sentences
     retval[i].clear();
     outout[i].clear();
     if (scores) scores[i] = 0; //pGenerator->score;
  }

  int num_results = work(false, input, outout, empty, nBest, scores );

  for (int i=0; i<std::min(nBest, num_results); ++i) {
     // now make the conll format stype outout
     retval[i].fromCoNLLInput(sentence);
     retval[i].copyDependencyHeads(outout[i]);
  }
}

/*---------------------------------------------------------------
 *
 * train_conll - train the models with an example
 *
 *---------------------------------------------------------------*/

void CDepParser::train_conll( const CCoNLLOutput &correct , int round ) {

  static CTwoStringVector sentence ;
  static CDependencyParse reference ;

  assert( m_bCoNLL );

  initCoNLLCache(correct);

  correct.toDependencyTree( reference );
  UnparseSentence( &reference, &sentence );

  // The following code does update for each processing stage
  m_nTrainingRound = round ;
  work(true , sentence , NULL, reference , 1 , 0);

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

