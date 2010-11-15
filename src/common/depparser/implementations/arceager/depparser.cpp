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

inline void CDepParser::getOrUpdateStackScore( const CStateItem *item, CPackedScoreType<SCORE_TYPE, action::MAX> &retval, const unsigned &action, SCORE_TYPE amount , int round ) {

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

   static CTwoTaggedWords st_word_tag_n0_word_tag ;
   static CTwoTaggedWords st_word_tag_n0_word ;
   static CTwoTaggedWords st_word_n0_word_tag ;
   static CTwoTaggedWords st_word_tag_n0_tag ;
   static CTwoTaggedWords st_tag_n0_word_tag ;
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
   static CTuple3<CWord, CTag, CTag> word_tag_tag;
   static CTuple3<CWord, CWord, CTag> word_word_tag;

   cast_weights->m_mapSTwt.getOrUpdateScore( retval, st_word_tag, action, m_nScoreIndex, amount, round) ;
   cast_weights->m_mapSTw.getOrUpdateScore( retval, st_word, action, m_nScoreIndex, amount, round) ;
   cast_weights->m_mapSTt.getOrUpdateScore( retval, st_tag, action, m_nScoreIndex, amount, round ) ;

   cast_weights->m_mapN0wt.getOrUpdateScore( retval, n0_word_tag, action, m_nScoreIndex, amount, round) ;
   cast_weights->m_mapN0w.getOrUpdateScore( retval, n0_word, action, m_nScoreIndex, amount, round ) ;
   cast_weights->m_mapN0t.getOrUpdateScore( retval, n0_tag, action, m_nScoreIndex, amount, round ) ;

   if (n1_index > -1) cast_weights->m_mapN1wt.getOrUpdateScore( retval, n1_word_tag, action, m_nScoreIndex, amount, round) ;
   if (n1_index > -1) cast_weights->m_mapN1w.getOrUpdateScore( retval, n1_word, action, m_nScoreIndex, amount, round ) ;
   if (n1_index > -1) cast_weights->m_mapN1t.getOrUpdateScore( retval, n1_tag, action, m_nScoreIndex, amount, round ) ;

   cast_weights->m_mapSTwtN0wt.getOrUpdateScore( retval, st_word_tag_n0_word_tag, action, m_nScoreIndex, amount, round ); 
   refer_or_allocate_tuple3(word_word_tag, &st_word, &n0_word, &st_tag);
   cast_weights->m_mapSTwtN0w.getOrUpdateScore( retval, word_word_tag, action, m_nScoreIndex, amount, round ) ; 
   refer_or_allocate_tuple3(word_word_tag, &st_word, &n0_word, &n0_tag);
   cast_weights->m_mapSTwN0wt.getOrUpdateScore( retval, word_word_tag, action, m_nScoreIndex, amount, round ) ; 
   refer_or_allocate_tuple3(word_tag_tag, &st_word, &st_tag, &n0_tag);
   cast_weights->m_mapSTwtN0t.getOrUpdateScore( retval, word_tag_tag, action, m_nScoreIndex, amount, round ) ; 
   refer_or_allocate_tuple3(word_tag_tag, &n0_word, &st_tag, &n0_tag);
   cast_weights->m_mapSTtN0wt.getOrUpdateScore( retval, word_tag_tag, action, m_nScoreIndex, amount, round ) ;
   cast_weights->m_mapSTwN0w.getOrUpdateScore( retval, st_word_n0_word, action, m_nScoreIndex, amount, round ) ; 
   cast_weights->m_mapSTtN0t.getOrUpdateScore( retval, CTagSet<CTag, 2>(encodeTags(st_tag,n0_tag)), action, m_nScoreIndex, amount, round ) ; 

   if (n1_index>-1) cast_weights->m_mapN0tN1t.getOrUpdateScore( retval, CTagSet<CTag, 2>(encodeTags(n0_tag,n1_tag)), action, m_nScoreIndex, amount, round ) ; 
   if (n2_index>-1) cast_weights->m_mapN0tN1tN2t.getOrUpdateScore( retval, CTagSet<CTag, 3>(encodeTags(n0_tag,n1_tag,n2_tag)), action, m_nScoreIndex, amount, round ) ; 
   if (n1_index>-1) cast_weights->m_mapSTtN0tN1t.getOrUpdateScore( retval, CTagSet<CTag, 3>(encodeTags(st_tag,n0_tag,n1_tag)), action, m_nScoreIndex, amount, round ) ; 
   if (n0ld_index!=-1) cast_weights->m_mapSTtN0tN0LDt.getOrUpdateScore( retval, CTagSet<CTag, 3>(encodeTags(st_tag,n0_tag,n0ld_tag)), action, m_nScoreIndex, amount, round ) ; 
   if (sth_index!=-1) cast_weights->m_mapSTHtSTtN0t.getOrUpdateScore( retval, CTagSet<CTag, 3>(encodeTags(sth_tag,st_tag,n0_tag)), action, m_nScoreIndex, amount, round ) ; 
   if (stld_index!=-1) cast_weights->m_mapSTtSTLDtN0t.getOrUpdateScore( retval, CTagSet<CTag, 3>(encodeTags(st_tag,stld_tag,n0_tag)), action, m_nScoreIndex, amount, round ) ; 
   if (strd_index!=-1) cast_weights->m_mapSTtSTRDtN0t.getOrUpdateScore( retval, CTagSet<CTag, 3>(encodeTags(st_tag,strd_tag,n0_tag)), action, m_nScoreIndex, amount, round ) ; 

   if (n1_index!=-1) {
      refer_or_allocate_tuple2(word_tag, &n0_word, &n1_tag);
      cast_weights->m_mapN0wN1t.getOrUpdateScore( retval, word_tag, action, m_nScoreIndex, amount, round ) ; 
   }
   if (n2_index!=-1) {
      refer_or_allocate_tuple3(word_tag_tag, &n0_word, &n1_tag, &n2_tag);
      cast_weights->m_mapN0wN1tN2t.getOrUpdateScore( retval, word_tag_tag, action, m_nScoreIndex, amount, round ) ; 
   }
   refer_or_allocate_tuple3(word_tag_tag, &n0_word, &st_tag, &n1_tag);
   cast_weights->m_mapSTtN0wN1t.getOrUpdateScore( retval, word_tag_tag, action, m_nScoreIndex, amount, round ) ; 
   if (n0ld_index!=-1) {
      refer_or_allocate_tuple3(word_tag_tag, &n0_word, &st_tag, &n0ld_tag);
      cast_weights->m_mapSTtN0wN0LDt.getOrUpdateScore( retval, word_tag_tag, action, m_nScoreIndex, amount, round ) ; 
   }
   if (sth_index!=-1) {
      refer_or_allocate_tuple3(word_tag_tag, &n0_word, &sth_tag, &st_tag);
      cast_weights->m_mapSTHtSTtN0w.getOrUpdateScore( retval, word_tag_tag, action, m_nScoreIndex, amount, round ) ; 
   }
   if (stld_index!=-1) {
      refer_or_allocate_tuple3(word_tag_tag, &n0_word, &st_tag, &stld_tag);
      cast_weights->m_mapSTtSTLDtN0w.getOrUpdateScore( retval, word_tag_tag, action, m_nScoreIndex, amount, round ) ; 
   }
   if (strd_index!=-1) {
      refer_or_allocate_tuple3(word_tag_tag, &n0_word, &st_tag, &strd_tag);
      cast_weights->m_mapSTtSTRDtN0w.getOrUpdateScore( retval, word_tag_tag, action, m_nScoreIndex, amount, round ) ; 
   }

   if (m_bCoNLL) {
      static unsigned i;
      if (st_index!=-1) {
         cast_weights->m_mapSTl.getOrUpdateScore( retval, m_lCacheCoNLLLemma[st_index], action, m_nScoreIndex, amount, round) ;
         cast_weights->m_mapSTc.getOrUpdateScore( retval, m_lCacheCoNLLCPOS[st_index], action, m_nScoreIndex, amount, round) ;
         for (i=0; i<m_lCacheCoNLLFeats[st_index].size(); ++i)
            cast_weights->m_mapSTf.getOrUpdateScore( retval, m_lCacheCoNLLFeats[st_index][i], action, m_nScoreIndex, amount, round) ;
      } // if (st_index!=-1)
      if (n0_index!=-1) {
         cast_weights->m_mapN0l.getOrUpdateScore( retval, m_lCacheCoNLLLemma[n0_index], action, m_nScoreIndex, amount, round) ;
         cast_weights->m_mapN0c.getOrUpdateScore( retval, m_lCacheCoNLLCPOS[n0_index], action, m_nScoreIndex, amount, round) ;
         for (i=0; i<m_lCacheCoNLLFeats[n0_index].size(); ++i)
            cast_weights->m_mapN0f.getOrUpdateScore( retval, m_lCacheCoNLLFeats[n0_index][i], action, m_nScoreIndex, amount, round) ;
      } // if (n0_index!=-1)
      if (n1_index!=-1) {
         cast_weights->m_mapN1l.getOrUpdateScore( retval, m_lCacheCoNLLLemma[n1_index], action, m_nScoreIndex, amount, round) ;
         cast_weights->m_mapN1c.getOrUpdateScore( retval, m_lCacheCoNLLCPOS[n1_index], action, m_nScoreIndex, amount, round) ;
         for (i=0; i<m_lCacheCoNLLFeats[n1_index].size(); ++i)
            cast_weights->m_mapN1f.getOrUpdateScore( retval, m_lCacheCoNLLFeats[n1_index][i], action, m_nScoreIndex, amount, round) ;
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

void CDepParser::updateScores(const CDependencyParse & parsed , const CDependencyParse & correct , int round ) {
   THROW("depparser.cpp: updateScores unsupported");
}

/*---------------------------------------------------------------
 *
 * updateScoreForState - update a single positive or negative outout
 *
 *--------------------------------------------------------------*/

inline void CDepParser::updateScoreForState( const CStateItem &from, const CStateItem *outout , const SCORE_TYPE &amount ) {
   static CStateItem item;
   static unsigned action;
   static CPackedScoreType<SCORE_TYPE, action::MAX> empty;
   item = from;
   while ( item != *outout ) {
      action = item.FollowMove( outout );
      getOrUpdateStackScore( &item, empty, action, amount, m_nTrainingRound );
#ifdef LABELED
      if ( action::getLabel(action) )
         getOrUpdateStackScore( &item, action::getUnlabeledAction(action), amount, m_nTrainingRound );
#endif
      item.Move( action );
   }
}

/*---------------------------------------------------------------
 *
 * updateScoresForStates - update scores for states
 *
 *--------------------------------------------------------------*/

void CDepParser::updateScoresForStates( const CStateItem *outout , const CStateItem *correct , SCORE_TYPE amount_add, SCORE_TYPE amount_subtract ) {

   // do not update those steps where they are correct
   static CStateItem item;
   static unsigned action, correct_action;
   item.clear();
   while ( item != *outout ) {
      action = item.FollowMove( outout );
      correct_action = item.FollowMove( correct );
      if ( action == correct_action )
         item.Move( action );
      else break;
   }

   // for the necessary information for the correct and outout parsetree
   updateScoreForState( item, correct , amount_add ) ;
   updateScoreForState( item, outout , amount_subtract ) ;

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
      scoredaction.action = action::encodeAction(action::ARC_LEFT, label);
      scoredaction.score = item->score + 
                           scores[scoredaction.action] +
                           scores[action::ARC_LEFT];
      m_Beam->insertItem(&scoredaction);
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
      scoredaction.action = action::encodeAction(action::ARC_RIGHT, label);
      scoredaction.score = item->score + 
                           scores[scoredaction.action] +
                           scores[action::ARC_RIGHT];
      m_Beam->insertItem(&scoredaction);
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
   static CPackedScoreType<SCORE_TYPE, action::MAX> packed_scores;

   ASSERT(length<MAX_SENTENCE_SIZE, "The size of the sentence is larger than the system configuration.");

   TRACE("Initialising the decoding process...") ;
   // initialise word cache
   bContradictsRules = false;
   m_lCache.clear();
   for ( index=0; index<length; ++index ) {
      m_lCache.push_back( CTaggedWord<CTag, TAG_SEPARATOR>(sentence[index].first , sentence[index].second) );
      // filter std::cout training examples with rules
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
      std::cout << "Skipping training example because it contradicts rules..." <<std::endl;
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

         // for the state items that already contain all words
         m_Beam->clear();
         packed_scores.reset();
         getOrUpdateStackScore( pGenerator, packed_scores, action::NO_ACTION );
         if ( pGenerator->size() == length ) {
            assert( pGenerator->stacksize() != 0 );
            if ( pGenerator->stacksize()>1 ) 
               reduce(pGenerator, packed_scores) ;
            else
               poproot(pGenerator, packed_scores); 
         }
         // for the state items that still need more words
         else {  
            if ( !pGenerator->afterreduce() ) { // there are many ways when there are many arcrighted items on the stack and the root need arcleft. force this.               
               if ( ( pGenerator->size() < length-1 || pGenerator->stackempty() ) && // keep only one global root
                    ( pGenerator->stackempty() || m_supertags == 0 || m_supertags->canShift( pGenerator->size() ) ) && // supertags
                    ( pGenerator->stackempty() || !m_weights->rules() || canBeRoot( m_lCache[pGenerator->size()].tag.code() ) || hasRightHead(m_lCache[pGenerator->size()].tag.code()) ) // rules
                  ) {
                  shift(pGenerator, packed_scores) ;
               }
            }
            if ( !pGenerator->stackempty() ) {
               if ( ( pGenerator->size() < length-1 || pGenerator->numberoflocalheads() == 1 ) && // one root
                    ( m_supertags == 0 || m_supertags->canArcRight(pGenerator->stacktop(), pGenerator->size()) ) && // supertags conform to this action
                    ( !m_weights->rules() || hasLeftHead(m_lCache[pGenerator->size()].tag.code()) ) // rules
                  ) { 
#ifdef LABELED
                  if ( !m_weights->rules() || canAssignLabel(m_lCache, pGenerator->stacktop(), pGenerator->size(), label) ) {
                     arcright(pGenerator, packed_scores);
                  }
#else
                  arcright(pGenerator, packed_scores) ;
#endif
               }
            }
            if ( !pGenerator->stackempty() ) {
               if ( pGenerator->head( pGenerator->stacktop() ) != DEPENDENCY_LINK_NO_HEAD ) {
                  reduce(pGenerator, packed_scores) ;
               }
               else {
                  if ( (m_supertags == 0 || m_supertags->canArcLeft(pGenerator->size(), pGenerator->stacktop())) && // supertags
                       (!m_weights->rules() || hasRightHead(m_lCache[pGenerator->stacktop()].tag.code())) // rules
                     ) {
#ifdef LABELED
                     if ( !m_weights->rules() || canAssignLabel(m_lCache, pGenerator->size(), pGenerator->stacktop(), label) ) {
                        arcleft(pGenerator, packed_scores);
                     }
#else
                     arcleft(pGenerator, packed_scores) ;
#endif
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

void CDepParser::parse( const CTwoStringVector &sentence , CDependencyParse *retval , int nBest , SCORE_TYPE *scores ) {

   static CDependencyParse empty ;

   assert( !m_bCoNLL );

   for (int i=0; i<nBest; ++i) {
      // clear the outout sentences
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

   static CTwoStringVector sentence ;
   static CDependencyParse outout ; 

   assert( !m_bCoNLL );
   assert( IsProjectiveDependencyTree(correct) ) ;
   UnparseSentence( &correct, &sentence ) ;

   // The following code does update for each processing stage
   m_nTrainingRound = round ;
   work( true , sentence , &outout , correct , 1 , 0 ) ; 

};

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
      m_lCacheCoNLLLemma[i].load(sentence.at(i).lemma);
      m_lCacheCoNLLCPOS[i].load(sentence.at(i).ctag);
      m_lCacheCoNLLFeats[i].clear();
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

void CDepParser::parse_conll( const CCoNLLInput &sentence , CCoNLLOutput *retval , int nBest, SCORE_TYPE *scores ) {

   static CDependencyParse empty ;
   static CTwoStringVector input ;
   static CDependencyParse outout[AGENDA_SIZE] ;

   assert( m_bCoNLL ) ;

   initCoNLLCache(sentence);

   sentence.toTwoStringVector(input);

   for (int i=0; i<nBest; ++i) {
      // clear the outout sentences
      retval[i].clear();
      outout[i].clear();
      if (scores) scores[i] = 0; //pGenerator->score;
   }

   work(false, input, outout, empty, nBest, scores ) ;

   for (int i=0; i<std::min(nBest, m_Agenda->generatorSize()); ++i) {
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
   static CDependencyParse outout ; 
   static CDependencyParse reference ;

   assert( m_bCoNLL ) ;
   assert( IsProjectiveDependencyTree(correct) ) ;

   initCoNLLCache(correct);

   correct.toDependencyTree( reference );
   UnparseSentence( &reference, &sentence ) ;

   // The following code does update for each processing stage
   m_nTrainingRound = round ;
   work( true , sentence , &outout , reference , 1 , 0 ) ; 

}
