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
//const CScore<SCORE_TYPE> g_zeroScore;

#define cast_weights static_cast<CWeight*>(m_weights)
#define refer_or_allocate_tuple3(x, o1, o2, o3) { if (amount == 0) x.refer(o1, o2, o3); else x.allocate(o1, o2, o3); }
#define refer_or_allocate_tuple4(x, o1, o2, o3, o4) { if (amount == 0) x.refer(o1, o2, o3, o4); else x.allocate(o1, o2, o3, o4); }

/*===============================================================
 *
 * CDepParser - the depparser for TARGET_LANGUAGE
 *
 *==============================================================*/

/*---------------------------------------------------------------
 *
 * getOrUpdateArcScore - get or update the score from a single dependency link
 *
 *---------------------------------------------------------------*/

SCORE_TYPE CDepParser::getOrUpdateArcScore( const int &head_index , const int &dep_index, const int &sibling_index, SCORE_TYPE amount, int round ) {

   SCORE_TYPE retval = 0 ;

#ifdef COMBINED

#include "templates/shared.cpp"
#include "templates/getorupdate.cpp"

   //-------------------------Features from McDonald------------------------

   getOrUpdateUnigramScoreTemplate(0) ;
   getOrUpdateUnigramScoreTemplate(link_distance_encode) ;
   getOrUpdateUnigramScoreTemplate(link_direction_encode) ;

   getOrUpdateBigramScoreTemplate(0) ;
   getOrUpdateBigramScoreTemplate(link_distance_encode) ;
   getOrUpdateBigramScoreTemplate(link_direction_encode) ;

   getOrUpdateSurroundingScoreTemplate(link_distance_encode) ;
   getOrUpdateSurroundingScoreTemplate(link_direction_encode) ;

   //-------------------------sibling features from McDonald------------------------

   getOrUpdateSiblingScoreTemplate(0) ;
   getOrUpdateSiblingScoreTemplate(sibling_distance_encode) ;
   getOrUpdateSiblingScoreTemplate(sibling_direction_encode) ;

#endif

   return retval;
}

/*---------------------------------------------------------------
 *
 * getOrUpdateTwoArcScore - the score from two linked arcs
 *
 * this score is computed and updated in a special method because
 * it can't be computed together with an arc all the time.
 * the score is only computed when a node is popped off the stack
 * and therefore its leftmost and rightmost children are clear.
 * not all cases with parent, child and grandchild are scored,
 * but only those with the parent, child and left/right most
 * grandchild. this is for the easiness of computation.
 *
 * the definition of this score is like this:
 *         \          /          \               /
 *         /    or   /     or     \      or      \
 *
 *---------------------------------------------------------------*/

inline SCORE_TYPE CDepParser::getOrUpdateTwoArcScore( const int &head_index , const int &dep_index , const int &parent_index , SCORE_TYPE amount , int round ) {

#ifdef COMBINED

   assert(parent_index!=DEPENDENCY_LINK_NO_HEAD) ;

   static int tags;
   static int dir;
   tags = ( m_lCache[head_index].tag.code()<<(CTag::SIZE*2) ) +
          ( m_lCache[parent_index].tag.code()<<CTag::SIZE ) +
          m_lCache[dep_index].tag.code() ;
   dir = (getLinkDirection(parent_index, head_index)<<1) + getLinkDirection(head_index, dep_index) ;

   return cast_weights->m_mapGrandChildTags.getOrUpdateScore( std::make_pair(tags, dir) , m_nScoreIndex , amount , round ) ;

#else

   return 0;

#endif
}

/*---------------------------------------------------------------
 *
 * getOrUpdateArityScore - get or udpate the score from word tag arity
 *
 *---------------------------------------------------------------*/

inline SCORE_TYPE CDepParser::getOrUpdateArityScore( const int &word_index , const int &arity , const int &arity_direction , SCORE_TYPE amount , int round ) {

#ifdef COMBINED
   static int arity_combined ;
   arity_combined = arity_direction == ARITY_DIRECTION_LEFT ? arity : -arity-1 ; // -1 for arity=0 right

   std::pair<CTaggedWord<CTag, TAG_SEPARATOR>, int> taggedword_arity =
      std::make_pair( static_cast<const CTaggedWord<CTag, TAG_SEPARATOR>&>(m_lCache[word_index]) , arity_combined );
   std::pair<CTag, int> tag_arity =
      std::make_pair( m_lCache[word_index].tag , arity_combined );

   static SCORE_TYPE retval;
   retval=0;

   retval += cast_weights->m_mapHeadWordTagArity.getOrUpdateScore( taggedword_arity , m_nScoreIndex , amount , round ) ;
   retval += cast_weights->m_mapHeadTagArity.getOrUpdateScore( tag_arity , m_nScoreIndex , amount , round ) ;

   return retval;
#else
   return 0;
#endif
}

#ifdef LABELED
/*---------------------------------------------------------------
 *
 * getOrUpdateArcLabelScore - the label score of dependency link
 *
 *---------------------------------------------------------------*/

inline SCORE_TYPE CDepParser::getOrUpdateArcLabelScore( const int &head_index, const int &dep_index, const unsigned long &label, SCORE_TYPE amount, int round) {
   const CTaggedWord<CTag, TAG_SEPARATOR> &head_word_tag = m_lCache[head_index];
   const CTaggedWord<CTag, TAG_SEPARATOR> &dep_word_tag = m_lCache[dep_index];
   const CWord &head_word = head_word_tag.word;
   const CWord &dep_word = dep_word_tag.word;
   const CTag &head_tag = head_word_tag.tag;
   const CTag &dep_tag = dep_word_tag.tag;

   const CTag &head_tag_l = ( head_index > 0 ) ? m_lCache[ head_index-1 ].tag : CTag::SENTENCE_BEGIN ;
   const CTag &head_tag_r = ( head_index+1 < m_lCache.size() ) ? m_lCache[ head_index+1 ].tag : CTag::SENTENCE_END ;
   const CTag &dep_tag_l = ( dep_index > 0 ) ? m_lCache[ dep_index-1 ].tag : CTag::SENTENCE_BEGIN ;
   const CTag &dep_tag_r = ( dep_index+1 < m_lCache.size() ) ? m_lCache[ dep_index+1 ].tag : CTag::SENTENCE_END ;

   static unsigned long head_tag_lm, head_tag_mr, head_tag_lmr, dep_tag_lm, dep_tag_mr, dep_tag_lmr;

   head_tag_lm = encodeTags(head_tag_l, head_tag, g_noneTag);
   head_tag_mr = encodeTags(g_noneTag, head_tag, head_tag_r);
   head_tag_lm = encodeTags(head_tag_l, head_tag, head_tag_r);
   dep_tag_lm = encodeTags(dep_tag_l, dep_tag, g_noneTag);
   dep_tag_mr = encodeTags(g_noneTag, dep_tag, dep_tag_r);
   dep_tag_lm = encodeTags(dep_tag_l, dep_tag, dep_tag_r);

#include "templates/labeled.h"

   const int link_distance_encode = getLinkSizeAndDirection(head_index, dep_index) ;
   const int link_direction_encode = getLinkDirectionEncode(head_index, dep_index);

   static SCORE_TYPE retval;

   retval = 0;

   getOrUpdateLabeledScoreTemplate(getLabelAndDirectionOrDistanceEncode(label,link_distance_encode));
   getOrUpdateLabeledScoreTemplate(getLabelAndDirectionOrDistanceEncode(label,link_direction_encode));

   return retval;
}
#endif

/*---------------------------------------------------------------
 *
 * getOrUpdateStackScore - manipulate the score from stack
 *
 *---------------------------------------------------------------*/

inline SCORE_TYPE CDepParser::getOrUpdateStackScore( const CStateItem *item, const unsigned long &action, SCORE_TYPE amount , int round ) {

   const int &st_index = item->stackempty() ? -1 : item->stacktop(); // stack top
   const int &sth_index = st_index == -1 ? -1 : item->head(st_index); // stack top head
   const int &stld_index = st_index == -1 ? -1 : item->leftdep(st_index); // leftmost dep of stack
   const int &strd_index = st_index == -1 ? -1 : item->rightdep(st_index); // rightmost dep st
   const int &n0_index = item->size()==m_lCache.size() ? -1 : item->size(); // next
   const int &n0ld_index = n0_index==-1 ? -1 : item->leftdep(n0_index); // leftmost dep of next
   assert(n0_index<static_cast<int>(m_lCache.size())); // the next index shouldn't exceed sentence
   static int n1_index;
   static int n2_index;
   static int n3_index;
   n1_index = n0_index+1<m_lCache.size() ? n0_index+1 : -1 ;
   n2_index = n0_index+2<m_lCache.size() ? n0_index+2 : -1 ;
   n3_index = n0_index+3<m_lCache.size() ? n0_index+3 : -1 ;

   const CTaggedWord<CTag, TAG_SEPARATOR> &st_word_tag = st_index==-1 ? g_emptyTaggedWord : m_lCache[st_index];
   const CTaggedWord<CTag, TAG_SEPARATOR> &sth_word_tag = sth_index==-1 ? g_emptyTaggedWord : m_lCache[sth_index];
   const CTaggedWord<CTag, TAG_SEPARATOR> &n0_word_tag = n0_index==-1 ? g_emptyTaggedWord : m_lCache[n0_index];
   const CTaggedWord<CTag, TAG_SEPARATOR> &n1_word_tag = n1_index==-1 ? g_emptyTaggedWord : m_lCache[n1_index];
   const CWord &st_word = st_word_tag.word;
   const CWord &n0_word = n0_word_tag.word;
   const CWord &n1_word = n1_word_tag.word;
   const CTag &st_tag = st_word_tag.tag;
   const CTag &n0_tag = n0_word_tag.tag;
   const CTag &n1_tag = n1_word_tag.tag;

   const CTag &sth_tag = sth_word_tag.tag;
   static CTag stld_tag, strd_tag;
   stld_tag = stld_index == -1 ? CTag::SENTENCE_END : m_lCache[stld_index].tag;
   strd_tag = strd_index == -1 ? CTag::SENTENCE_END : m_lCache[strd_index].tag;
   static CTag n0ld_tag;
   static CTag n2_tag, n3_tag ;
   n0ld_tag = n0ld_index==-1 ? CTag::SENTENCE_END : m_lCache[n0ld_index].tag;
   n2_tag = n2_index==-1 ? CTag::SENTENCE_END : m_lCache[n2_index].tag;
   n3_tag = n3_index==-1 ? CTag::SENTENCE_END : m_lCache[n3_index].tag;

   const CTaggedWord<CTag, TAG_SEPARATOR> st_word_nil(st_word, CTag::NONE);
   const CTaggedWord<CTag, TAG_SEPARATOR> st_nil_tag(g_emptyWord, st_tag);
   const CTaggedWord<CTag, TAG_SEPARATOR> n0_word_nil(n0_word, CTag::NONE);
   const CTaggedWord<CTag, TAG_SEPARATOR> n0_nil_tag(g_emptyWord, n0_tag);

   static CTwoTaggedWords st_word_tag_n0_word_tag ;
   static CTwoTaggedWords st_word_tag_n0_word ;
   static CTwoTaggedWords st_word_n0_word_tag ;
   static CTwoTaggedWords st_word_tag_n0_tag ;
   static CTwoTaggedWords st_tag_n0_word_tag ;
   static CTwoWords st_word_n0_word ;

   if ( amount == 0 ) {
      st_word_tag_n0_word_tag.refer( &st_word_tag, &n0_word_tag );
      st_word_tag_n0_word.refer( &st_word_tag, &n0_word_nil );
      st_word_n0_word_tag.refer( &st_word_nil, &n0_word_tag );
      st_word_tag_n0_tag.refer( &st_word_tag, &n0_nil_tag );
      st_tag_n0_word_tag.refer( &st_nil_tag, &n0_word_tag );
      st_word_n0_word.refer( &st_word, &n0_word );
   }
   else {
      st_word_tag_n0_word_tag.allocate( st_word_tag, n0_word_tag );
      st_word_tag_n0_word.allocate( st_word_tag, n0_word_nil );
      st_word_n0_word_tag.allocate( st_word_nil, n0_word_tag );
      st_word_tag_n0_tag.allocate( st_word_tag, n0_nil_tag );
      st_tag_n0_word_tag.allocate( st_nil_tag, n0_word_tag );
      st_word_n0_word.allocate( st_word, n0_word );
   }

   static CTuple3<CWord, CTag, unsigned long> word_tag_int;
   static CTuple4<CWord, CTag, CTag, unsigned long> word_tag_tag_int;

   static SCORE_TYPE retval;
   retval=0;

   retval += cast_weights->m_mapSTwt.getOrUpdateScore( std::make_pair(st_word_tag, action), m_nScoreIndex, amount, round) ;
   retval += cast_weights->m_mapSTw.getOrUpdateScore( std::make_pair(st_word, action), m_nScoreIndex, amount, round) ;
   retval += cast_weights->m_mapSTt.getOrUpdateScore( std::make_pair(st_tag, action), m_nScoreIndex, amount, round ) ;

   retval += cast_weights->m_mapN0wt.getOrUpdateScore( std::make_pair(n0_word_tag, action), m_nScoreIndex, amount, round) ;
   retval += cast_weights->m_mapN0w.getOrUpdateScore( std::make_pair(n0_word, action), m_nScoreIndex, amount, round ) ;
   retval += cast_weights->m_mapN0t.getOrUpdateScore( std::make_pair(n0_tag, action), m_nScoreIndex, amount, round ) ;

   if (n1_index > -1) retval += cast_weights->m_mapN1wt.getOrUpdateScore( std::make_pair(n1_word_tag, action), m_nScoreIndex, amount, round) ;
   if (n1_index > -1) retval += cast_weights->m_mapN1w.getOrUpdateScore( std::make_pair(n1_word, action), m_nScoreIndex, amount, round ) ;
   if (n1_index > -1) retval += cast_weights->m_mapN1t.getOrUpdateScore( std::make_pair(n1_tag, action), m_nScoreIndex, amount, round ) ;

   retval += cast_weights->m_mapSTwtN0wt.getOrUpdateScore( std::make_pair(st_word_tag_n0_word_tag,action), m_nScoreIndex, amount, round );
   retval += cast_weights->m_mapSTwtN0w.getOrUpdateScore( std::make_pair(st_word_tag_n0_word,action), m_nScoreIndex, amount, round ) ;
   retval += cast_weights->m_mapSTwN0wt.getOrUpdateScore( std::make_pair(st_word_n0_word_tag,action), m_nScoreIndex, amount, round ) ;
   retval += cast_weights->m_mapSTwtN0t.getOrUpdateScore( std::make_pair(st_word_tag_n0_tag,action), m_nScoreIndex, amount, round ) ;
   retval += cast_weights->m_mapSTtN0wt.getOrUpdateScore( std::make_pair(st_tag_n0_word_tag,action), m_nScoreIndex, amount, round ) ;
   retval += cast_weights->m_mapSTwN0w.getOrUpdateScore( std::make_pair(st_word_n0_word,action), m_nScoreIndex, amount, round ) ;
   retval += cast_weights->m_mapSTtN0t.getOrUpdateScore( std::make_pair(CTagSet<CTag, 2>(encodeTags(st_tag,n0_tag)),action), m_nScoreIndex, amount, round ) ;

   if (n1_index>-1) retval += cast_weights->m_mapN0tN1t.getOrUpdateScore( std::make_pair(CTagSet<CTag, 2>(encodeTags(n0_tag,n1_tag)),action), m_nScoreIndex, amount, round ) ;
   if (n2_index>-1) retval += cast_weights->m_mapN0tN1tN2t.getOrUpdateScore( std::make_pair(CTagSet<CTag, 3>(encodeTags(n0_tag,n1_tag,n2_tag)),action), m_nScoreIndex, amount, round ) ;
   if (n1_index>-1) retval += cast_weights->m_mapSTtN0tN1t.getOrUpdateScore( std::make_pair(CTagSet<CTag, 3>(encodeTags(st_tag,n0_tag,n1_tag)),action), m_nScoreIndex, amount, round ) ;
   if (n0ld_index!=-1) retval += cast_weights->m_mapSTtN0tN0LDt.getOrUpdateScore( std::make_pair(CTagSet<CTag, 3>(encodeTags(st_tag,n0_tag,n0ld_tag)),action), m_nScoreIndex, amount, round ) ;
   if (sth_index!=-1) retval += cast_weights->m_mapSTHtSTtN0t.getOrUpdateScore( std::make_pair(CTagSet<CTag, 3>(encodeTags(sth_tag,st_tag,n0_tag)),action), m_nScoreIndex, amount, round ) ;
   if (stld_index!=-1) retval += cast_weights->m_mapSTtSTLDtN0t.getOrUpdateScore( std::make_pair(CTagSet<CTag, 3>(encodeTags(st_tag,stld_tag,n0_tag)),action), m_nScoreIndex, amount, round ) ;
   if (strd_index!=-1) retval += cast_weights->m_mapSTtSTRDtN0t.getOrUpdateScore( std::make_pair(CTagSet<CTag, 3>(encodeTags(st_tag,strd_tag,n0_tag)),action), m_nScoreIndex, amount, round ) ;

   if (n1_index!=-1) {
      refer_or_allocate_tuple3(word_tag_int, &n0_word, &n1_tag, &action);
      retval += cast_weights->m_mapN0wN1t.getOrUpdateScore( word_tag_int, m_nScoreIndex, amount, round ) ;
   }
   if (n2_index!=-1) {
      refer_or_allocate_tuple4(word_tag_tag_int, &n0_word, &n1_tag, &n2_tag, &action);
      retval += cast_weights->m_mapN0wN1tN2t.getOrUpdateScore( word_tag_tag_int, m_nScoreIndex, amount, round ) ;
   }
   refer_or_allocate_tuple4(word_tag_tag_int, &n0_word, &st_tag, &n1_tag, &action);
   retval += cast_weights->m_mapSTtN0wN1t.getOrUpdateScore( word_tag_tag_int, m_nScoreIndex, amount, round ) ;
   if (n0ld_index!=-1) {
      refer_or_allocate_tuple4(word_tag_tag_int, &n0_word, &st_tag, &n0ld_tag, &action);
      retval += cast_weights->m_mapSTtN0wN0LDt.getOrUpdateScore( word_tag_tag_int, m_nScoreIndex, amount, round ) ;
   }
   if (sth_index!=-1) {
      refer_or_allocate_tuple4(word_tag_tag_int, &n0_word, &sth_tag, &st_tag, &action);
      retval += cast_weights->m_mapSTHtSTtN0w.getOrUpdateScore( word_tag_tag_int, m_nScoreIndex, amount, round ) ;
   }
   if (stld_index!=-1) {
      refer_or_allocate_tuple4(word_tag_tag_int, &n0_word, &st_tag, &stld_tag, &action);
      retval += cast_weights->m_mapSTtSTLDtN0w.getOrUpdateScore( word_tag_tag_int, m_nScoreIndex, amount, round ) ;
   }
   if (strd_index!=-1) {
      refer_or_allocate_tuple4(word_tag_tag_int, &n0_word, &st_tag, &strd_tag, &action);
      retval += cast_weights->m_mapSTtSTRDtN0w.getOrUpdateScore( word_tag_tag_int, m_nScoreIndex, amount, round ) ;
   }

   return retval;
}

/*---------------------------------------------------------------
 *
 * getGlobalScore - get the score of a parse tree
 *
 * Inputs: parse graph
 *
 *---------------------------------------------------------------*/

SCORE_TYPE CDepParser::getGlobalScore(const CDependencyParse &parsed) {
   static CStateItem item, temp;
   static int index;
   // read cache
   m_lCache.clear();
   for ( index=0; index<parsed.size(); ++index )
      m_lCache.push_back( CTaggedWord<CTag, TAG_SEPARATOR>(parsed[index].word , CTag(parsed[index].tag)) );
#ifdef LABELED
   m_lCacheLabel.clear();
   for (index=0; index<parsed.size(); ++index )
      m_lCacheLabel.push_back( CDependencyLabel(parsed[index].label) );
#endif
   // make an item from the parsed
   item.clear();
   for (index=0; index<parsed.size()*2; ++index) {
#ifdef LABELED
      item.StandardMoveStep(parsed, m_lCacheLabel);
#else
      item.StandardMoveStep(parsed);
#endif
   }
   item.StandardFinish();
   // now follow item to make temp and update its scores
   static unsigned long action;
   temp.clear();
   while ( temp != item ) {
      action = temp.FollowMove( &item );
#ifdef LABELED
      switch ( CStateItem::getAction(action) ) {
#else
      switch ( action ) {
#endif
      case CStateItem::SHIFT:
         shift(&temp);
         break;
      case CStateItem::REDUCE:
         reduce(&temp);
         break;
      case CStateItem::ARC_LEFT:
#ifdef LABELED
         arcleft( &temp, CStateItem::getLabel(action) );
#else
         arcleft( &temp );
#endif
         break;
      case CStateItem::ARC_RIGHT:
#ifdef LABELED
         arcright( &temp, CStateItem::getLabel(action) );
#else
         arcright( &temp );
#endif
         break;
      case CStateItem::POP_ROOT:
         poproot( &temp );
         break;
      default:
         THROW("action unexpected: " << action << ".");
      }
   }
   return temp.score();
}

/*---------------------------------------------------------------
 *
 * find_information - auxilary function
 *                    finds necessary information for a status
 *
 *---------------------------------------------------------------*/

inline int find_information( const CStateItem *item, int *stack ) {
   // initialise the arrays
   for ( int i=0; i<item->size(); ++i ) {
      stack[i] = CStateItem::OFF_STACK;
   }
   // stack information
   for ( int i=0; i<item->stacksize(); ++i ) {
      if (item->head(item->stackitem(i))==DEPENDENCY_LINK_NO_HEAD)
         stack[item->stackitem(i)] = CStateItem::ON_STACK_SHIFT;
      else
         stack[item->stackitem(i)] = CStateItem::ON_STACK_ARCRIGHT;
   }
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

   assert( m_bTrain );

   if ( round > m_nTrainingRound )
      m_nTrainingRound = round ;

   if ( parsed == correct )
      return;

   CStateItem state;
   static int index;

   assert(correct.size()==parsed.size());
   m_lCache.clear();
   for ( index=0; index<correct.size(); ++index ) {
      assert(correct[index]==parsed[index]);
      m_lCache.push_back( CTaggedWord<CTag, TAG_SEPARATOR>(correct[index].word , CTag(correct[index].tag)) );
   }
#ifdef LABELED
   m_lCacheLabel.clear();
   for (index=0; index<correct.size(); ++index )
      m_lCacheLabel.push_back( CDependencyLabel(correct[index].label) );
#endif
   state.clear();
   for (index=0; index<correct.size()*2; ++index) {
#ifdef LABELED
      state.StandardMoveStep(correct, m_lCacheLabel);
#else
      state.StandardMoveStep(correct);
#endif
}

   state.StandardFinish();
   updateScoreForState(&state, 1);

//   m_lCache.clear();
//   for ( index=0; index<parsed.size(); ++index )
//      m_lCache.push_back( CTaggedWord<CTag, TAG_SEPARATOR>(parsed[index].word, CTag(parsed[index].tag)) );
#ifdef LABELED
   m_lCacheLabel.clear();
   for (index=0; index<parsed.size(); ++index )
      m_lCacheLabel.push_back( CDependencyLabel(parsed[index].label) );
#endif
   state.clear();
   for (index=0; index<parsed.size()*2; ++index) {
#ifdef LABELED
      state.StandardMoveStep(parsed, m_lCacheLabel);
#else
      state.StandardMoveStep(parsed);
#endif
   }
   state.StandardFinish();
   updateScoreForState(&state, -1);

    m_nTotalErrors++;

}

/*---------------------------------------------------------------
 *
 * updateScoreForState - update a single positive or negative output
 *
 *--------------------------------------------------------------*/

inline void CDepParser::updateScoreForState( const CStateItem *output , const SCORE_TYPE &amount ) {
   static int output_stackstatus[MAX_SENTENCE_SIZE] ;
   static int child_index ;

   find_information( output, output_stackstatus ) ;
   // update scores for the outputs parsetree
   for ( int i=0; i<output->size(); ++i ) {
      if ( output->head(i) != DEPENDENCY_LINK_NO_HEAD ) {
         // link score
         getOrUpdateArcScore( output->head(i), i, output->sibling(i), amount, m_nTrainingRound ) ;
#ifdef LABELED
         getOrUpdateArcLabelScore( output->head(i), i, output->label(i), amount, m_nTrainingRound );
#endif
         // left dependencies are available when the word is off stack or on stack by arc right
         child_index = output->leftdep(i) ;
         while ( child_index != DEPENDENCY_LINK_NO_HEAD ) {
            getOrUpdateTwoArcScore( i , child_index , output->head(i) , amount , m_nTrainingRound ) ; break;
            child_index = output->sibling(child_index) ;
         }
         // right dependencies are fixed only after the item is popped off stack
         if ( output_stackstatus[i] == CStateItem::OFF_STACK ) {
            child_index = output->rightdep(i) ;
            while ( child_index != DEPENDENCY_LINK_NO_HEAD ) {
               getOrUpdateTwoArcScore( i , child_index , output->head(i) , amount , m_nTrainingRound ) ; break;
               child_index = output->sibling(child_index) ;

            }
         }
      }
      // left arity have been fixed for any word that is pushed on
      getOrUpdateArityScore( i , output->leftarity(i) , ARITY_DIRECTION_LEFT , amount , m_nTrainingRound ) ;
      // right arity are fixed only when the item is popped off stack
      if ( output_stackstatus[i] == CStateItem::OFF_STACK ) {
         getOrUpdateArityScore( i , output->rightarity(i) , ARITY_DIRECTION_RIGHT , amount , m_nTrainingRound ) ;
      }
   }

   static CStateItem item;
   static unsigned long action;
   item.clear();
   while ( item != *output ) {
      action = item.FollowMove( output );
#ifdef LABELED
      if ( CStateItem::getAction(action) != CStateItem::POP_ROOT ) {
#else
      if ( action != CStateItem::POP_ROOT ) {
#endif
         getOrUpdateStackScore( &item, action, amount, m_nTrainingRound );
#ifdef LABELED
         if ( CStateItem::getLabel(action) )
            getOrUpdateStackScore( &item, CStateItem::removeLabelFromEncodedAction(action), amount, m_nTrainingRound );
#endif
      }
      item.Move( action );
   }
}

/*---------------------------------------------------------------
 *
 * updateScoresForStates - update scores for states
 *
 *--------------------------------------------------------------*/

void CDepParser::updateScoresForStates( const CStateItem *output , const CStateItem *correct , SCORE_TYPE amount_add, SCORE_TYPE amount_subtract ) {

   //assert( output->size() == correct->size() );
   // for the necessary information for the correct and output parsetree
   updateScoreForState( correct , amount_add ) ;
   updateScoreForState( output , amount_subtract ) ;

   m_nTotalErrors++;
}

/*---------------------------------------------------------------
 *
 * reduce - helper function
 *
 *--------------------------------------------------------------*/

inline void CDepParser::reduce( CStateItem *item ) {
   const int &word_index = item->stacktop() ;
   static int child_index ;
   // add score for the rightmost child of the popped off item, because they are fixed
   child_index = item->rightdep(word_index) ;
   while ( child_index != DEPENDENCY_LINK_NO_HEAD ) {
      item->linkscore() += getOrUpdateTwoArcScore(word_index, child_index, item->head(word_index)) ; break;
      child_index = item->sibling(child_index) ;
   }
   // similarly add right arities
   item->linkscore() += getOrUpdateArityScore( word_index, item->rightarity(word_index), ARITY_DIRECTION_RIGHT ) ;
   // update stack score
#ifdef LABELED
   item->stackscore() += getOrUpdateStackScore( item, CStateItem::encodeAction(CStateItem::REDUCE, CDependencyLabel::NONE) );
#else
   item->stackscore() += getOrUpdateStackScore( item, CStateItem::REDUCE );
#endif
   // do reduce after adding scores because the computing of scores used prev stack_top
   item->Reduce();
}

/*---------------------------------------------------------------
 *
 * arcleft - helping function
 *
 *--------------------------------------------------------------*/

#ifdef LABELED
inline void CDepParser::arcleft( CStateItem *item, const unsigned long &label ) {
#else
inline void CDepParser::arcleft( CStateItem *item ) {
#endif
   const int &word_index = item->stacktop() ;
   const int &next_index = item->size() ; // the last active item
   static int child_index ;
   static int sibling_index ;
   // add score for the link info
   sibling_index = item->leftdep(next_index) ;
   item->linkscore() += getOrUpdateArcScore( next_index, word_index, sibling_index ) ;
#ifdef LABELED
   item->linkscore() += getOrUpdateArcLabelScore( next_index, word_index, label ) ;
#endif
   // add score for the left and right most child as well, because they are fixed
   child_index = item->leftdep(word_index) ;
   while ( child_index != DEPENDENCY_LINK_NO_HEAD ) {
      item->linkscore() += getOrUpdateTwoArcScore(word_index, child_index, next_index) ; break;
      child_index = item->sibling(child_index) ;
   }
   child_index = item->rightdep(word_index) ;
   while ( child_index != DEPENDENCY_LINK_NO_HEAD ) {
      item->linkscore() += getOrUpdateTwoArcScore(word_index, child_index, next_index) ; break;
      child_index = item->sibling(child_index) ;
   }
   // add right arity score for it is fixed now, but left arity added (by shifting)
   item->linkscore() += getOrUpdateArityScore( word_index , item->rightarity(word_index) , ARITY_DIRECTION_RIGHT ) ;
#ifdef LABELED
   // update stack score
   item->stackscore() += getOrUpdateStackScore( item, CStateItem::encodeAction(CStateItem::ARC_LEFT, label) );
   item->stackscore() += getOrUpdateStackScore( item, CStateItem::encodeAction(CStateItem::ARC_LEFT, CDependencyLabel::NONE) );
   // add score before doing arcleft action!
   item->ArcLeft(label) ;
#else
   // update stack score
   item->stackscore() += getOrUpdateStackScore( item, CStateItem::ARC_LEFT );
   // add score before doing arcleft action!
   item->ArcLeft() ;
#endif
}

/*---------------------------------------------------------------
 *
 * arcright - helping function
 *
 *--------------------------------------------------------------*/

#ifdef LABELED
inline void CDepParser::arcright( CStateItem *item, const unsigned long &label ) {
#else
inline void CDepParser::arcright( CStateItem *item ) {
#endif
   const int &word_index = item->stacktop() ;
   const int &next_index = item->size() ;
   static int child_index ;
   static int sibling_index ;
   // add score about the dependency
   sibling_index = item->rightdep(word_index) ;
   item->linkscore() += getOrUpdateArcScore( word_index, next_index, sibling_index ) ;
#ifdef LABELED
   item->linkscore() += getOrUpdateArcLabelScore( word_index, next_index, label ) ;
#endif
   // add two arc score
   child_index = item->leftdep(next_index) ;
   while ( child_index != DEPENDENCY_LINK_NO_HEAD ) {
      item->linkscore() += getOrUpdateTwoArcScore(next_index, child_index, word_index) ; break;
      child_index = item->sibling(child_index) ;
   }
   // add arity score
   item->linkscore() += getOrUpdateArityScore( next_index, item->leftarity(next_index), ARITY_DIRECTION_LEFT ) ;
#ifdef LABELED
   // add stack score
   item->stackscore() += getOrUpdateStackScore( item, CStateItem::encodeAction(CStateItem::ARC_RIGHT, label) );
   item->stackscore() += getOrUpdateStackScore( item, CStateItem::encodeAction(CStateItem::ARC_RIGHT, CDependencyLabel::NONE) );
   // add score before doing arcright action!
   item->ArcRight(label) ;
#else
   // add stack score
   item->stackscore() += getOrUpdateStackScore( item, CStateItem::ARC_RIGHT );
   // add score before doing arcright action!
   item->ArcRight() ;
#endif
}

/*---------------------------------------------------------------
 *
 * shift - help function
 *
 *--------------------------------------------------------------*/

inline void CDepParser::shift( CStateItem *item ) {
   const int &next_index = item->size() ;
   item->linkscore() += getOrUpdateArityScore( next_index , item->leftarity(next_index) , ARITY_DIRECTION_LEFT ) ;
#ifdef LABELED
   item->stackscore() += getOrUpdateStackScore( item, CStateItem::encodeAction(CStateItem::SHIFT, CDependencyLabel::NONE) );
#else
   item->stackscore() += getOrUpdateStackScore( item, CStateItem::SHIFT );
#endif
   item->Shift() ;
}

/*---------------------------------------------------------------
 *
 * poproot - help function
 *
 *--------------------------------------------------------------*/

inline void CDepParser::poproot( CStateItem *item ) {
   const int &word_index = item->stacktop() ;
   item->linkscore() += getOrUpdateArityScore( word_index , item->rightarity(word_index) , ARITY_DIRECTION_RIGHT ) ;
   item->PopRoot() ;
}


/*---------------------------------------------------------------
 *
 * work - the working process shared by training and parsing
 *
 * Returns: makes a new instance of CDependencyParse
 *
 *--------------------------------------------------------------*/

void CDepParser::work( const bool bTrain , const CTwoStringVector &sentence , CDependencyParse *retval , const CDependencyParse &correct , int nBest , SCORE_TYPE *scores ) {

#ifdef DEBUG
   clock_t total_start_time = clock();
#endif
   static int index;
   const int length = sentence.size() ;

   const CStateItem *pGenerator ;
   static CStateItem pCandidate ;

   // used only for training
   static bool bCorrect ;  // used in learning for early update
   static bool bContradictsRules;
   static CStateItem correctState ;

   ASSERT(length<MAX_SENTENCE_SIZE, "The size of the sentence is larger than the system configuration.");

   TRACE("Initialising the decoding process...") ;
   // initialise word cache
   bContradictsRules = false;
   m_lCache.clear();
   for ( index=0; index<length; ++index ) {
      m_lCache.push_back( CTaggedWord<CTag, TAG_SEPARATOR>(sentence[index].first , sentence[index].second) );
      // filter out training examples with rules
      if (bTrain && m_weights->rules()) {
         // the root
         if ( correct[index].head == DEPENDENCY_LINK_NO_HEAD && canBeRoot(m_lCache[index].tag.code())==false) {
            TRACE("Rule contradiction: " << m_lCache[index].tag << " can be root.");
            bContradictsRules = true;
         }
         // head left
         if ( correct[index].head < index && hasLeftHead(m_lCache[index].tag.code())==false) {
            TRACE("Rule contradiction: " << m_lCache[index].tag << " has left head.");
            bContradictsRules = true;
         }
         // head right
         if ( correct[index].head > index && hasRightHead(m_lCache[index].tag.code())==false) {
            TRACE("Rule contradiction: " << m_lCache[index].tag << " has right head.");
            bContradictsRules = true;
         }
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
         if (m_weights->rules() && !canAssignLabel(m_lCache, correct[index].head, index, m_lCacheLabel[index])) {
            TRACE("Rule contradiction: " << correct[index].label << " on link head " << m_lCache[correct[index].head].tag << " dep " << m_lCache[index].tag);
            bContradictsRules = true;
         }
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
   for (index=0; index<length*2; ++index) {

      if (bTrain) bCorrect = false ;

      // none can this find with pruning ???
      if (m_Agenda->generatorSize() == 0) {
         WARNING("parsing failed");
         return;
      }

      pGenerator = m_Agenda->generatorStart();
      // iterate generators
      for (int j=0; j<m_Agenda->generatorSize(); ++j) {

         // try to generate all possible candidates with the new word pushed into our stack

         // for the state items that already contain all words
         if ( pGenerator->size() == length ) {
            assert( pGenerator->stacksize() != 0 );
            //pCandidate = m_Agenda->candidateItem() ;
            pCandidate = *pGenerator ;
            if ( pCandidate.stacksize()>1 )
               reduce(&pCandidate) ;
            else
               poproot(&pCandidate);
            m_Agenda->pushCandidate(&pCandidate) ;
         }
         // for the state items that still need more words
         else {
            if ( !pGenerator->afterreduce() ) { // there are many ways when there are many arcrighted items on the stack and the root need arcleft. force this.
               if ( ( pGenerator->size() < length-1 || pGenerator->stackempty() ) && // keep only one global root
                    ( pGenerator->stackempty() || m_supertags == 0 || m_supertags->canShift( pGenerator->size() ) ) && // supertags
                    ( pGenerator->stackempty() || !m_weights->rules() || canBeRoot( m_lCache[pGenerator->size()].tag.code() ) || hasRightHead(m_lCache[pGenerator->size()].tag.code()) ) // rules
                  ) {
                  pCandidate = *pGenerator ;
                  shift(&pCandidate) ;
                  m_Agenda->pushCandidate(&pCandidate) ;
               }
            }
            if ( !pGenerator->stackempty() ) {
               if ( ( pGenerator->size() < length-1 || pGenerator->numberoflocalheads() == 1 ) && // one root
                    ( m_supertags == 0 || m_supertags->canArcRight(pGenerator->stacktop(), pGenerator->size()) ) && // supertags conform to this action
                    ( !m_weights->rules() || hasLeftHead(m_lCache[pGenerator->size()].tag.code()) ) // rules
                  ) {
#ifdef LABELED
                  for (label=CDependencyLabel::FIRST; label<CDependencyLabel::COUNT; label++) {
                     if ( !m_weights->rules() || canAssignLabel(m_lCache, pGenerator->stacktop(), pGenerator->size(), label) ) {
                        pCandidate = *pGenerator ;
                        arcright(&pCandidate, label);
                        m_Agenda->pushCandidate(&pCandidate);
                     }
                  }
#else
                  pCandidate = *pGenerator ;
                  arcright(&pCandidate) ;
                  m_Agenda->pushCandidate(&pCandidate) ;
#endif
               }
            }
            if ( !pGenerator->stackempty() ) {
               if ( pGenerator->head( pGenerator->stacktop() ) != DEPENDENCY_LINK_NO_HEAD ) {
                  pCandidate = *pGenerator ;
                  reduce(&pCandidate) ;
                  m_Agenda->pushCandidate(&pCandidate) ;
               }
               else {
                  if ( (m_supertags == 0 || m_supertags->canArcLeft(pGenerator->size(), pGenerator->stacktop())) && // supertags
                       (!m_weights->rules() || hasRightHead(m_lCache[pGenerator->stacktop()].tag.code())) // rules
                     ) {
#ifdef LABELED
                     for (label=CDependencyLabel::FIRST; label<CDependencyLabel::COUNT; label++) {
                        if ( !m_weights->rules() || canAssignLabel(m_lCache, pGenerator->size(), pGenerator->stacktop(), label) ) {
                           pCandidate = *pGenerator ;
                           arcleft(&pCandidate, label);
                           m_Agenda->pushCandidate(&pCandidate);
                        }
                     }
#else
                     pCandidate = *pGenerator ;
                     arcleft(&pCandidate) ;
                     m_Agenda->pushCandidate(&pCandidate) ;
#endif
                  }
               }
            }
         }

         if (bTrain && *pGenerator == correctState) {
            bCorrect = true ;
         }
         pGenerator = m_Agenda->generatorNext() ;

      }
      // when we are doing training, we need to consider the standard move and update
      if (bTrain) {
#ifdef EARLY_UPDATE
         if (!bCorrect) {
            TRACE("Error at the "<<correctState.size()<<"th word; total is "<<correct.size())
            updateScoresForStates(m_Agenda->bestGenerator(), &correctState, 1, -1) ;
            return ;
         }
#endif

#ifdef LABELED
         correctState.StandardMoveStep(correct, m_lCacheLabel);
#else
         correctState.StandardMoveStep(correct);
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
   for (int i=0; i<nBest; ++i) {
      pGenerator = m_Agenda->generator(i) ;
      if (pGenerator) {
         pGenerator->GenerateTree( sentence , retval[i] ) ;
         if (scores) scores[i] = pGenerator->score();
      }
   }
   TRACE("Done, the highest score is: " << m_Agenda->bestGenerator()->score() ) ;
   TRACE("The total time spent: " << double(clock() - total_start_time)/CLOCKS_PER_SEC) ;
}

/*---------------------------------------------------------------
 *
 * parse - do dependency parsing to a sentence
 *
 * Returns: makes a new instance of CDependencyParse
 *
 *--------------------------------------------------------------*/

void CDepParser::parse( const CTwoStringVector &sentence , CDependencyParse *retval , int nBest , SCORE_TYPE *scores ) {

   static CDependencyParse empty ;

   for (int i=0; i<nBest; ++i) {
      // clear the output sentences
      retval[i].clear();
      if (scores) scores[i] = 0; //pGenerator->score();
   }

   work(false, sentence, retval, empty, nBest, scores ) ;

}

/*---------------------------------------------------------------
 *
 * train - train the models with an example
 *
 *---------------------------------------------------------------*/

void CDepParser::train( const CDependencyParse &correct , int round ) {

   static CTwoStringVector sentence ;
   static CDependencyParse output ;

   assert( IsProjectiveDependencyTree(correct) ) ;
   UnparseSentence( &correct, &sentence ) ;

   // The following code does update for each processing stage
   m_nTrainingRound = round ;
   work( true , sentence , &output , correct , 1 , 0 ) ;

};


