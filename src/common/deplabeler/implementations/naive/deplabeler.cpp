/****************************************************************
 *                                                              *
 * deplabeler.cpp - the naive labeler implementation            *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2007.12                        *
 *                                                              *
 ****************************************************************/

#include "deplabeler.h"
#include "weight.h"

using namespace TARGET_LANGUAGE;
using namespace TARGET_LANGUAGE::deplabeler;

const CWord g_emptyWord("");
const CTaggedWord<CTag, TAG_SEPARATOR> g_emptyTaggedWord("", CTag::NONE);
const CTag g_noneTag = CTag::NONE;
//const CScore<SCORE_TYPE> g_zeroScore;

#define cast_weights static_cast<CWeight*>(m_weights)
#define refer_or_allocate_tuple3(x, o1, o2, o3) { if (amount == 0) x.refer(o1, o2, o3); else x.allocate(o1, o2, o3); }
#define refer_or_allocate_tuple4(x, o1, o2, o3, o4) { if (amount == 0) x.refer(o1, o2, o3, o4); else x.allocate(o1, o2, o3, o4); }

/*===============================================================
 *
 * CDepLabeler - the deplabeler for TARGET_LANGUAGE 
 *
 *==============================================================*/

/*---------------------------------------------------------------
 *
 * getOrUpdateArcLabelScore - the label score of dependency link
 *
 *---------------------------------------------------------------*/

inline SCORE_TYPE CDepLabeler::getOrUpdateArcLabelScore( const int &head_index, const int &dep_index, const unsigned long &label, SCORE_TYPE amount, int round) {
   const CTaggedWord<CTag, TAG_SEPARATOR> &head_word_tag = m_lCache[head_index];
   const CTaggedWord<CTag, TAG_SEPARATOR> &dep_word_tag = m_lCache[dep_index];
   const CWord &head_word = static_cast<const CWord&>(head_word_tag);
   const CWord &dep_word = static_cast<const CWord&>(dep_word_tag);
   const unsigned long &head_tag = head_word_tag.tag.code();
   const unsigned long &dep_tag = dep_word_tag.tag.code();

   const unsigned long &head_tag_l = ( head_index > 0 ) ? m_lCache[ head_index-1 ].tag.code() : CTag::SENTENCE_BEGIN ;
   const unsigned long &head_tag_r = ( head_index+1 < m_lCache.size() ) ? m_lCache[ head_index+1 ].tag.code() : CTag::SENTENCE_END ;
   const unsigned long &dep_tag_l = ( dep_index > 0 ) ? m_lCache[ dep_index-1 ].tag.code() : CTag::SENTENCE_BEGIN ;
   const unsigned long &dep_tag_r = ( dep_index+1 < m_lCache.size() ) ? m_lCache[ dep_index+1 ].tag.code() : CTag::SENTENCE_END ;

   static unsigned long head_tag_lm, head_tag_mr, head_tag_lmr, dep_tag_lm, dep_tag_mr, dep_tag_lmr;

   head_tag_lm = encodeTags(head_tag_l, head_tag, CTag::NONE);
   head_tag_mr = encodeTags(CTag::NONE, head_tag, head_tag_r);
   head_tag_lm = encodeTags(head_tag_l, head_tag, head_tag_r);
   dep_tag_lm = encodeTags(dep_tag_l, dep_tag, CTag::NONE);
   dep_tag_mr = encodeTags(CTag::NONE, dep_tag, dep_tag_r);
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

/*---------------------------------------------------------------
 *
 * work - the working process shared by training and parsing
 *
 * Returns: makes a new instance of CLabeledDependencyTree 
 *
 *--------------------------------------------------------------*/

void CDepLabeler::work( const CDependencyTree &sentence , CLabeledDependencyTree *retval , const CLabeledDependencyTree *correct , const unsigned long &index ) {

#ifdef DEBUG
   clock_t total_start_time = clock();
#endif
   const int length = sentence.size() ; 

   const CStateItem *pGenerator ;
   CStateItem pCandidate ;
   bool bCorrect ;  // used in learning for early update
   static CStateItem correctState ;

   assert(length<MAX_SENTENCE_SIZE);

   TRACE("Initialising the decoding process...") ;
   // initialise word cache
   m_lCache.clear();
   for ( int index=0; index<length; index++ )
      m_lCache.push_back( CTaggedWord<CTag, TAG_SEPARATOR>(sentence[index].first , sentence[index].second) );
   // initialise agenda
   m_Agenda->clear();
//   pCandidate = m_Agenda->candidateItem();      // make the first item
   pCandidate.clear();                          // restore state using clean
   m_Agenda->pushCandidate(&pCandidate);           // and push it back
   m_Agenda->nextRound();                       // as the generator item
   if (bTrain) correctState.clear();

#ifdef LABELED
   unsigned long label;
#endif

   TRACE("Decoding started"); 
   // loop with the next word to process in the sentence
   for (int index=0; index<length*2; index++) {
      
      if (bTrain) bCorrect = false ; 

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
               if ( pGenerator->size() < length-1 || pGenerator->stackempty() ) { // keep only one global root
                  //pCandidate = m_Agenda->candidateItem() ;
                  pCandidate = *pGenerator ;
                  shift(&pCandidate) ;
                  m_Agenda->pushCandidate(&pCandidate) ;
               }
            }
            if ( !pGenerator->stackempty() ) {
               if ( pGenerator->size() < length-1 || pGenerator->numberoflocalheads() == 1 ) { // keep only one global root
#ifdef LABELED
                  for (label=CDependencyLabel::FIRST; label<CDependencyLabel::COUNT; label++) {
                     if (label != CDependencyLabel::ROOT) {
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
#ifdef LABELED
                  for (label=CDependencyLabel::FIRST; label<CDependencyLabel::COUNT; label++) {
                     if (label != CDependencyLabel::ROOT) {
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
         correctState.StandardMoveStep(correct);
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
      retval[i].clear();
      if (scores) scores[i] = 0; //pGenerator->score();
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
 * label - do dependency parsing to a sentence
 *
 *--------------------------------------------------------------*/

void CDepLabeler::label( const CDependencyTree &sentence , CLabeledDependencyTree *retval ) {

   for (unsigned long i=0; i<correct.size(); ++i)
      work( sentence, retval, 0, i ) ;

}

/*---------------------------------------------------------------
 *
 * train - train the models with an example
 *
 *---------------------------------------------------------------*/

void CDepLabeler::train( const CLabeledDependencyTree &correct ) {

   static CDependencyTree parse ;
   static CLabeledDependencyTree label ; 

   UnlabelSentence( &correct, &parse ) ;

   // The following code does update for each processing stage

   for (unsigned long i=0; i<correct.size(); ++i) {
      ++m_nTrainingRound ;
      work( parse , &label , &correct , i ) ; 
   }

};


