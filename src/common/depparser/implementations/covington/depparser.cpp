// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                              *
 * depparser.cpp - the covington depparser implementation       *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2008.04                        *
 *                                                              *
 ****************************************************************/

#include "depparser.h"
#include "depparser_weight.h"

using namespace TARGET_LANGUAGE;
using namespace TARGET_LANGUAGE::depparser;

const CWord g_emptyWord("");
const CTaggedWord<CTag, TAG_SEPARATOR> g_emptyTaggedWord("", CTag::NONE);
const CTag g_noneTag = CTag::NONE;
const CScore<SCORE_TYPE> g_zeroScore;

#define cast_weights static_cast<CWeight*>(m_weights)

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

SCORE_TYPE CDepParser::getOrUpdateArcScore( const CStateItem *item, const int &head_index , const int &dep_index, SCORE_TYPE amount, int round ) {

   SCORE_TYPE retval = 0 ;

#include "templates/shared.cpp"
#include "templates/getorupdate.cpp"

   //-------------------------Features from McDonald------------------------

   retval = 0;

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

   //-------------------------the expanded two sibling features------------------------
   if (sibling_index != DEPENDENCY_LINK_NO_HEAD) {
/*
      between_tags = (dep_tag.code()<<CTag::SIZE*2) +
                     (sibling_tag.code()<<CTag::SIZE) +
                     next_sibling_tag.code() ;
      retval += cast_weights->m_mapTwoSiblingTags.getScore(std::make_pair(between_tags,sibling_distance_encode), m_nScoreIndex);
      retval += cast_weights->m_mapTwoSiblingTags.getScore(std::make_pair(between_tags,sibling_direction_encode), m_nScoreIndex);

      between_tags += (head_tag.code()<<CTag::SIZE*3) ;
      retval += cast_weights->m_mapTwoSiblingAndParentTags.getScore(std::make_pair(between_tags,sibling_distance_encode), m_nScoreIndex);
      retval += cast_weights->m_mapTwoSiblingAndParentTags.getScore(std::make_pair(between_tags,sibling_direction_encode), m_nScoreIndex);
*/
   }

   //--------------------- The two link features ----------------------
/*
   between_tags = (head_tag.code()<<CTag::SIZE*2)+(dep_tag.code()<<CTag::SIZE);
   retval += cast_weights->m_mapGrandChildTags.getScore( std::make_pair(between_tags+dep_lmd_tag, (link_direction<<1)+0), m_nScoreIndex );
   if (head_index>dep_index)
      retval += cast_weights->m_mapGrandChildTags.getScore( std::make_pair(between_tags+dep_rmd_tag, (link_direction<<1)+1), m_nScoreIndex );
*/
   return retval ;
}

/*---------------------------------------------------------------
 *
 * getOrUpdateArityScore - get or udpate the score from word tag arity
 *
 *---------------------------------------------------------------*/

inline SCORE_TYPE CDepParser::getOrUpdateArityScore( const CStateItem *item, const int &word_index, const int &arity_direction, SCORE_TYPE amount, int round ) {

   static int arity;
//   static map<unsigned long, unsigned long> arity_by_tag;
   // compute arity
   static int dep;
   arity = 0;
//   arity_by_tag.clear();
   if ( arity_direction == ARITY_DIRECTION_LEFT )
      dep = item->leftmostdep( word_index );
   else
      dep = item->rightmostdep( word_index );
   while ( dep != DEPENDENCY_LINK_NO_HEAD ) {
      arity++;
//      arity_by_tag[m_lCache[dep].tag.code()]++;
      dep = item->sibling( dep );
   }
   if ( arity_direction == ARITY_DIRECTION_RIGHT )
      arity = -arity-1; // -1 to avoid collision with arity=0 left

   std::pair<CTaggedWord<CTag, TAG_SEPARATOR>, int> taggedword_arity = std::make_pair( m_lCache[word_index] , arity );
   std::pair<unsigned long int, int> tag_arity = std::make_pair( m_lCache[word_index].tag.code() , arity );

   // compute score
   static SCORE_TYPE retval;
   retval=0;

   retval += cast_weights->m_mapHeadWordTagArity.getOrUpdateScore( taggedword_arity , m_nScoreIndex , amount , round ) ;
   retval += cast_weights->m_mapHeadTagArity.getOrUpdateScore( tag_arity , m_nScoreIndex , amount , round ) ;
/*
   static map<unsigned long, unsigned long>::iterator it;
   static int sub_arity;
   for ( it = arity_by_tag.begin(); it != arity_by_tag.end(); it++ ) {
      sub_arity = (arity_direction==ARITY_DIRECTION_LEFT ? it->second : -(it->second)-1) ;
      sub_arity <<= CTag::SIZE;
      retval += cast_weights->m_mapHeadWordTagArityByTag.getOrUpdateScore( std::make_pair(m_lCache[word_index], sub_arity+it->first) , m_nScoreIndex , amount , round ) ;
      retval += cast_weights->m_mapHeadTagArityByTag.getOrUpdateScore( std::make_pair(m_lCache[word_index].tag.code(), sub_arity+it->first) , m_nScoreIndex , amount , round ) ;
   }
*/
   return retval;
}

/*---------------------------------------------------------------
 *
 * getOrUpdateTwoArcScore - the score from two linked arcs
 *
 *---------------------------------------------------------------*/

inline SCORE_TYPE CDepParser::getOrUpdateTwoArcScore( const int &head_index , const int &dep_index , const int &parent_index , SCORE_TYPE amount , int round ) {

   assert(parent_index!=DEPENDENCY_LINK_NO_HEAD) ;
   if ( dep_index==DEPENDENCY_LINK_NO_HEAD ) return 0;

   static int tags;
   static int dir;
   tags = ( m_lCache[head_index].tag.code()<<(CTag::SIZE*2) ) +
          ( m_lCache[parent_index].tag.code()<<CTag::SIZE ) +
          m_lCache[dep_index].tag.code() ;
   dir = (getLinkDirection(parent_index, head_index)<<1) + getLinkDirection(head_index, dep_index) ;

   return cast_weights->m_mapGrandChildTags.getOrUpdateScore( std::make_pair(tags, dir) , m_nScoreIndex , amount , round ) ;
}

/*---------------------------------------------------------------
 *
 * updateScoreForState - update a single positive or negative output
 *
 *--------------------------------------------------------------*/

inline void CDepParser::updateScoreForState( const CStateItem *output , const bool &bCompleteSentence ,
                                             const SCORE_TYPE &amount ) {
   static CStateItem item;
   static int index, prev, rmd;
   item.clear();
   // rebuild parsetrees
   for ( index=0; index<output->size(); index++ ) {
      prev = index-1;
      while ( prev != -1 ) {
         if ( output->head( index ) == prev ) {
            // finish all words that are the rightmost deps of prev -- they are crossed over
            rmd = item.rightmostdep( prev );
            while ( rmd != DEPENDENCY_LINK_NO_HEAD ) {
               getOrUpdateArityScore( &item, rmd, ARITY_DIRECTION_RIGHT, amount, m_nTrainingRound );
               getOrUpdateTwoArcScore( rmd, item.rightmostdep(rmd), item.head(rmd), amount, m_nTrainingRound ) ;
               rmd = item.rightmostdep( rmd );
            }
            // add link from index to prev point
            item.addLink( prev, index );
            getOrUpdateArcScore( &item, prev, index, amount, m_nTrainingRound );
            getOrUpdateTwoArcScore( index, item.leftmostdep(index), prev, amount, m_nTrainingRound );
            // break to finish index
            break;
         }
         else if ( output->head( prev ) == index ) {
            // add link from prev to index
            item.addLink( index, prev ) ;
            getOrUpdateArcScore( &item, index, prev, amount, m_nTrainingRound );
            getOrUpdateTwoArcScore( prev, item.leftmostdep(prev), index, amount, m_nTrainingRound ) ;
            // finish prev because it links to its right
            getOrUpdateArityScore( &item, prev, ARITY_DIRECTION_RIGHT, amount, m_nTrainingRound );
            getOrUpdateTwoArcScore( prev, item.rightmostdep(prev), index, amount, m_nTrainingRound ) ;
            // finish all words that are the rightmost deps of prev -- they are crossed over
            rmd = item.rightmostdep( prev );
            while ( rmd != DEPENDENCY_LINK_NO_HEAD ) {
               getOrUpdateArityScore( &item, rmd, ARITY_DIRECTION_RIGHT, amount, m_nTrainingRound );
               getOrUpdateTwoArcScore( rmd, item.rightmostdep(rmd), item.head(rmd), amount, m_nTrainingRound ) ;
               rmd = item.rightmostdep( rmd );
            }
         }
         // break if the prev is still local head -- can't over
         if ( item.head( prev ) == DEPENDENCY_LINK_NO_HEAD )
            break;
         // move to the prev
         prev = item.findPreviousLinkPoint( prev );
      }
      // finish index on the left
      getOrUpdateArityScore( &item, index, ARITY_DIRECTION_LEFT, amount, m_nTrainingRound );
      item.Move();
   }
   // finish all words that are the rightmost deps of the root (including itself)
   if ( bCompleteSentence ) {
      index = item.size()-1;
      while ( index != DEPENDENCY_LINK_NO_HEAD ) {
         getOrUpdateArityScore( &item, index, ARITY_DIRECTION_RIGHT, amount, m_nTrainingRound );
         index = item.head( index );
      }
   }
}

/*---------------------------------------------------------------
 *
 * updateScoresForStates - update scores for states
 *
 *--------------------------------------------------------------*/

void CDepParser::updateScoresForStates( const CStateItem *output, const CStateItem *correct, const bool &bCompleteSentence ) {
   assert( output->size() == correct->size() );

   updateScoreForState( correct, bCompleteSentence, 1 ) ;
   updateScoreForState( output, bCompleteSentence, -1 ) ;

   m_nTotalErrors++;
}

/*---------------------------------------------------------------
 *
 * addLink - add link adding score
 *
 *--------------------------------------------------------------*/

void CDepParser::addLink( CStateItem *item, const int &head, const int &dep ) {

   item->addLink( head, dep );

   // add one arc sco
   item->score() += getOrUpdateArcScore( item, head, dep );
   item->score() += getOrUpdateTwoArcScore( dep, item->leftmostdep(dep), head );
}

/*----------------------------------------------------------------
 *
 * finishWordOnTheLeft - finish the processing of a word
 *
 *---------------------------------------------------------------*/

void CDepParser::finishWordOnTheLeft( CStateItem *item, const int &index ) {
   item->score() += getOrUpdateArityScore( item, index, ARITY_DIRECTION_LEFT );
}

/*----------------------------------------------------------------
 *
 * finishWordOnTheRight - finish the processing of a word
 *
 *---------------------------------------------------------------*/

void CDepParser::finishWordOnTheRight( CStateItem *item, const int &index ) {
   item->score() += getOrUpdateArityScore( item, index, ARITY_DIRECTION_RIGHT );
   if ( item->head(index) != DEPENDENCY_LINK_NO_HEAD )
      item->score() += getOrUpdateTwoArcScore( index, item->rightmostdep(index), item->head(index));
}

/*----------------------------------------------------------------
 *
 * finishWord - finish the processing of a word
 *
 * Note that the candidate must be generated from m_Agenda->candidateItem
 *
 *---------------------------------------------------------------*/

void CDepParser::finishWord( CStateItem *candidate ) {
   finishWordOnTheLeft( candidate, candidate->size() );
   candidate->Move();    // move current word incrementally
}

/*----------------------------------------------------------------
 *
 * finishSentence - finish the processing of a sentence
 *
 * Note that the candidate must be generated from m_Agenda->candidateItem
 *
 *---------------------------------------------------------------*/

void CDepParser::finishSentence( CStateItem *candidate ) {
   static int index;
   index = candidate->size()-1;
   while (index!=DEPENDENCY_LINK_NO_HEAD) {
      finishWordOnTheRight( candidate, index );
      index=candidate->head( index );
   }
}

/*---------------------------------------------------------------
 *
 * work - the working process shared by training and parsing
 *
 * Returns: makes a new instance of CDependencyParse
 *
 *--------------------------------------------------------------*/

void CDepParser::work( const CTwoStringVector &sentence , CDependencyParse *retval , const CDependencyParse &correct , int nBest , SCORE_TYPE *scores ) {

#ifdef DEBUG
   clock_t total_start_time = clock();
#endif
   const int length = sentence.size() ;

   const CStateItem *pGenerator ;
   CStateItem *pCandidate ;
   CStateItem temp ;
   static int index ;
   static int prev, first_head ;
   static int igen, iroot ;
   static bool bCorrect ;  // used in learning for early update
   static CStateItem correctState ;

   assert(length<MAX_SENTENCE_SIZE);

   TRACE("Initialising the decoding process...") ;
   // initialise word cache
   m_lCache.clear();
   for ( index=0; index<length; index++ )
      m_lCache.push_back( CTaggedWord<CTag, TAG_SEPARATOR>(sentence[index].first , sentence[index].second) );
   // initialise agenda
   m_Agenda->clear();
   pCandidate = m_Agenda->candidateItem();      // make the first item
   pCandidate->clear();                         // restore state using clean
   m_Agenda->pushCandidate();                   // and push it back
   m_Agenda->nextRound();                       // as the generator item
   if (m_bTrain) correctState.clear();

   TRACE("Decoding started");
   // --------------------------------------------------------------------------
   // loop with the next word to process in the sentence                        |
   for (index=0; index<length; index++) {

      if (m_bTrain) bCorrect = false ;

      // ---------- iterate generators ----------
      pGenerator = m_Agenda->generatorStart(); //|
      for ( igen = 0; igen < m_Agenda->generatorSize(); ++igen ) {
         if ( m_bTrain && *pGenerator==correctState )
            bCorrect=true;
         temp = *pGenerator ;
         first_head = temp.findFirstHead() ;
         assert( temp.size() == index ) ;

         // no link to left (head /dep)
         if ( index != length-1 || length == 1 ) {
            pCandidate = m_Agenda->candidateItem() ;
            *pCandidate = temp ;
            finishWord(pCandidate) ;
            if ( index == length-1 ) finishSentence( pCandidate );
            m_Agenda->pushCandidate();
         }

         prev = index - 1 ; // start from the previous word, because no links have been made to index
         while ( prev!=DEPENDENCY_LINK_NO_HEAD ) {              // asserted DEPENDENCY_LINK_NO_HEAD==-1 at constructor
            assert( temp.head(prev) < prev ) ;                  // no head or head on the left for a possible linkpoint
            // link from index to prev
            if ( index != length-1 || first_head == prev ) {
               pCandidate = m_Agenda->candidateItem() ;
               *pCandidate = temp ;
               addLink(pCandidate, prev, index) ;
               finishWord( pCandidate ) ;
               if ( index == length-1 ) finishSentence( pCandidate );
               m_Agenda->pushCandidate();
            }
            // move prev backward and add link from prev to index
	    finishWordOnTheRight( &temp, prev ); // finish the word no matter if its linked to index or jumped across
            if ( temp.head(prev) == DEPENDENCY_LINK_NO_HEAD ) { // if the word currently has no head
               addLink(&temp, index, prev) ;                    // it must be linked to index
               if ( index < length-1 || first_head == prev ) {  // and if it is allowed
                  pCandidate = m_Agenda->candidateItem() ;      // also take the current status as candidate
                  *pCandidate = temp ;
                  finishWord(pCandidate) ;
                  if ( index == length-1 ) finishSentence( pCandidate );
                  m_Agenda->pushCandidate();
               }
            }
            prev = temp.findPreviousLinkPoint( prev ) ;          // move to prev linkpoints
         }

         pGenerator = m_Agenda->generatorNext() ;
      } //                                       |
      // ----------------------------------------

      // when we are doing training, we need to consider the standard move and update
      if (m_bTrain) {
         #ifdef EARLY_UPDATE
         if (!bCorrect) {
            TRACE("Error at the "<<index<<"th word; total is "<<correct.size())
            updateScoresForStates(m_Agenda->bestGenerator(), &correctState, false) ;
            return ;
         }
         #endif
         correctState.StandardMove(correct);
      }

      m_Agenda->nextRound(); // move round
   } //                                                                         |
   // --------------------------------------------------------------------------

/*
   // --------------------------------------------------------------------------
   //
   // when we are doing training, we need to consider the standard move and update
   if (m_bTrain) {
      #ifdef EARLY_UPDATE
      if (!bCorrect) {
         TRACE("Error at the "<<index<<"th word; total is "<<correct.size())
         updateScoresForStates(m_Agenda->bestGenerator(), &correctState, false) ;
         return ;
      }
      #endif
   }
   pGenerator = m_Agenda->generatorStart();
   for (igen=0; igen<m_Agenda->generatorSize(); ++igen) {
      temp = *pGenerator ;

//      for (iroot = 0; iroot < temp.size(); iroot++) {
//         if ( temp.head(iroot) == DEPENDENCY_LINK_NO_HEAD ) {
            pCandidate = m_Agenda->candidateItem() ;
            *pCandidate = temp ;
            finishSentence( pCandidate ) ;
//         }
//      }

      pGenerator = m_Agenda->generatorNext() ;
   }
   m_Agenda->nextRound() ;
   //
   // --------------------------------------------------------------------------
*/

   if (m_bTrain) {
      if ( *(m_Agenda->bestGenerator()) != correctState ) {
         TRACE("The best item is not the correct one")
         updateScoresForStates(m_Agenda->bestGenerator(), &correctState, true) ;
         return ;
      }
   }

   TRACE("Outputing sentence");
   m_Agenda->sortGenerators();
   for (int i=0; i<nBest; ++i) {
      retval[i].clear();
      if (scores) scores[i] = 0;
      pGenerator = m_Agenda->generator(i) ;
      if (pGenerator) {
         pGenerator->GenerateTree( sentence , retval[i] ) ;
         if (scores) scores[i] = pGenerator->const_score();
      }
   }
   TRACE("Done, the highest score is: " << m_Agenda->bestGenerator()->const_score() ) ;
   TRACE("The total time spent: " << double(clock() - total_start_time)/CLOCKS_PER_SEC) ;
}

//==============================================================================================


/*---------------------------------------------------------------
 *
 * parse - do dependency parsing to a sentence
 *
 * Returns: makes a new instance of CDependencyParse
 *
 *--------------------------------------------------------------*/

void CDepParser::parse( const CTwoStringVector &sentence , CDependencyParse *retval , int nBest , SCORE_TYPE *scores ) {

   static CDependencyParse empty ;

   work(sentence, retval, empty, nBest, scores ) ;

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
   work( sentence , &output , correct , 1 , 0 ) ;

};


