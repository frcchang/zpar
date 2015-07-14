// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                              *
 * depparser.cpp - the span driven chart parser implementation. *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2007.10                        *
 *                                                              *
 ****************************************************************/

#include "depparser.h"
#include "depparser_weight.h"

using namespace TARGET_LANGUAGE;
using namespace TARGET_LANGUAGE::depparser;

const CWord g_emptyWord("");
const CScore<SCORE_TYPE> g_zeroScore;
const CTag g_noneTag = CTag::NONE;

#define cast_weights static_cast<CWeight*>(m_weights)

/*===============================================================
 *
 * CDepParser - the depparser for English
 *
 *==============================================================*/

/*---------------------------------------------------------------
 *
 * getCrossLinkScore - get the score from a single dependency link
 *
 *---------------------------------------------------------------*/

SCORE_TYPE CDepParser::getCrossLinkScore( const CSpan *span ) {

#include "templates/define.cpp"

   assert( span->getType()!=CSpan::BF ) ;

   const int &left_index = span->getLeftBoundary() ;
   const int &right_index = span->getRightBoundary() ;
   head_index = span->getType() == CSpan::LF ? left_index : right_index ;
   dep_index = span->getType() == CSpan::LF ? right_index : left_index ;

   return getCrossLinkScore(head_index, dep_index);
}

/*---------------------------------------------------------------
 *
 * getCrossLinkScore - get the score from a single dependency link
 *
 *---------------------------------------------------------------*/

SCORE_TYPE CDepParser::getCrossLinkScore( int head_index, int dep_index ) {

   SCORE_TYPE retval = 0 ;
#include "templates/shared.cpp"
#include "templates/get.cpp"

   static int both_sides; // cache the sum of two tags
   static int index;

   //-------------------------Features from McDonald------------------------

   retval = 0;

   getUnigramScoreTemplate(0) ;
   getUnigramScoreTemplate(link_distance_encode) ;
   getUnigramScoreTemplate(link_direction_encode) ;

   getBigramScoreTemplate(0) ;
   getBigramScoreTemplate(link_distance_encode) ;
   getBigramScoreTemplate(link_direction_encode) ;

   //getSurroundingScoreTemplate(0) ;
   getSurroundingScoreTemplate(link_distance_encode) ;
   getSurroundingScoreTemplate(link_direction_encode) ;

   return retval ;
}

/*---------------------------------------------------------------
 *
 * updateCrossLinkScore - update the score from a dependency link
 *
 *---------------------------------------------------------------*/

void CDepParser::updateCrossLinkScore( int head_index, int dep_index, const CDependencyParse &tree, SCORE_UPDATE method, int round ) {

   CSpan::SPAN_TYPE sptype = head_index < dep_index ? CSpan::LF : CSpan::RF ;

#include "templates/shared.cpp"
#include "templates/update.cpp"

   int amount = method == eAdd ? 1 : -1 ;

   static int both_sides; // cache the sum of two tags
   static int index;

   //------------------Features from McDonald----------------------

   updateUnigrams(0) ;
   updateUnigrams(link_distance_encode) ;
   updateUnigrams(link_direction_encode) ;

   updateBigrams(0) ;
   updateBigrams(link_distance_encode) ;
   updateBigrams(link_direction_encode) ;

   //updateSurrounding(0) ;
   updateSurrounding(link_distance_encode) ;
   updateSurrounding(link_direction_encode) ;

}

/*---------------------------------------------------------------
 *
 * getGlobalScore - get the score value
 *
 * Inputs: the parsed example
 *
 *---------------------------------------------------------------*/

SCORE_TYPE CDepParser::getGlobalScore(const CDependencyParse & parsed) {

   SCORE_TYPE retval=0;

   m_lCache.clear();
   for ( int index=0; index<parsed.size(); index++ )
      m_lCache.push_back( CTaggedWord<CTag, TAG_SEPARATOR>(parsed[index].word , CTag(parsed[index].tag)) );

   for ( int i=0; i<parsed.size(); ++i ) {
      if ( parsed[i].head != DEPENDENCY_LINK_NO_HEAD )
         retval += getCrossLinkScore( parsed[i].head, i );
   }

   return retval;

}

/*---------------------------------------------------------------
 *
 * updateScoreVector - update the score std::vector by input
 *                     this is used in training to adjust params
 *
 * Inputs: the parsed and the correct example
 *
 *---------------------------------------------------------------*/

void CDepParser::updateScoreVector(const CDependencyParse & parsed , const CDependencyParse & correct , int round ) {

   static bool bCorrect;
   bCorrect = true;

   assert( m_bTrain );

   for ( int i=0; i<correct.size(); ++i ) {
      if ( parsed[i].head != correct[i].head ) {
         if ( correct[i].head != DEPENDENCY_LINK_NO_HEAD )
            updateCrossLinkScore( correct[i].head, i, correct, eAdd, round );
         if ( parsed[i].head != DEPENDENCY_LINK_NO_HEAD )
            updateCrossLinkScore( parsed[i].head, i, parsed, eSubtract, round );
         bCorrect = false;
      }
   }

   if ( round > m_nTrainingRound )
      m_nTrainingRound = round ;

   if ( !bCorrect ) {
      m_nTotalErrors++ ;
//      TRACE( parsed.str() << "------" << std::endl << correct.str() );
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

   m_lCache.clear();
   for ( int index=0; index<correct.size(); index++ )
      m_lCache.push_back( CTaggedWord<CTag, TAG_SEPARATOR>(correct[index].word , CTag(correct[index].tag)) );
   m_lCache.push_back( CTaggedWord<CTag, TAG_SEPARATOR>( "", CTag(CTag::SENTENCE_END).str() ) ); // EOS is appended to the tail of sentence
   for ( int i=0; i<correct.size(); ++i)
      if (correct[i].head != DEPENDENCY_LINK_NO_HEAD) updateCrossLinkScore( correct[i].head, i, correct, eAdd, round );

   m_lCache.clear();
   for ( int index=0; index<parsed.size(); index++ )
      m_lCache.push_back( CTaggedWord<CTag, TAG_SEPARATOR>(parsed[index].word, CTag(parsed[index].tag)) );
   m_lCache.push_back( CTaggedWord<CTag, TAG_SEPARATOR>( "", CTag(CTag::SENTENCE_END).str()));
   for ( int i=0; i<parsed.size(); ++i)
      if (parsed[i].head != DEPENDENCY_LINK_NO_HEAD ) updateCrossLinkScore( parsed[i].head, i, parsed, eSubtract, round );

    m_nTotalErrors++;

}

/*---------------------------------------------------------------
 *
 * generate - helper function that generates parsed output
 *
 *---------------------------------------------------------------*/

void generate(const CSpan &span, const CTwoStringVector &sentence, CDepParser *depparser, CDependencyParse &retval) {
   retval.clear() ;
   if (!span.isActive()) return;
   assert ( span.getLeftBoundary() == 0 && span.getRightBoundary() == sentence.size() ) ;
   bool bFoundHead = false;
   for ( int i=0; i<sentence.size(); ++i ) {
      int head = span.getLink(i);
      if (head==sentence.size()) {
         head = DEPENDENCY_LINK_NO_HEAD;
         assert( !bFoundHead );
         bFoundHead = true;
      }
      retval.push_back( CDependencyTreeNode( sentence.at(i).first , sentence.at(i).second, head ) ) ;
   }
}

/*---------------------------------------------------------------
 *
 * parse - do dependency parsing to a sentence
 *
 * Returns: makes a new instance of CDependencyParse
 *
 *--------------------------------------------------------------*/

void CDepParser::parse( const CTwoStringVector &sentence , CDependencyParse *retval , int nBest , SCORE_TYPE *scores ) {

   clock_t total_start_time = clock();
   const int length = sentence.size() ; // length of the sentence

   static CSpan chart[depparser::MAX_SENTENCE_SIZE][depparser::MAX_SENTENCE_SIZE][3];

   int span_length, left_length ;
   int span_starting_index ;

   CSpan *left_span, *right_span, *total_span ;
   CSpan temp_span;

   assert(length<MAX_SENTENCE_SIZE);
   assert(nBest==1);

   TRACE("Initialising the decoding process...");
   // clear all spans from length 3 to sentence len + 1
   // note that the end of sentence is extended with the special EOS token; therefore length+1
   for ( span_length = 3; span_length <= length + 1; ++span_length )
      for ( span_starting_index = 0; span_starting_index <= length + 1 - span_length; ++span_starting_index ) {
      chart[span_starting_index][span_length][CSpan::LF].clear();
      chart[span_starting_index][span_length][CSpan::RF].clear();
      chart[span_starting_index][span_length][CSpan::BF].clear();
   }

   // put the sentence into a cache
   m_lCache.clear();
   for ( int index=0; index<length; index++ )
      m_lCache.push_back( CTaggedWord<CTag, TAG_SEPARATOR>(sentence[index].first , sentence[index].second) );
   m_lCache.push_back( CTaggedWord<CTag, TAG_SEPARATOR>( "", CTag(CTag::SENTENCE_END).str() ) ); // EOS is appended to the tail of sentence

   TRACE("Decoding started");

   // initialise binary
   // note that because of the special EOS token, span_start reaches length-1
   for ( span_starting_index = 0; span_starting_index < length; ++span_starting_index ) {
      total_span = &(chart[span_starting_index][2][CSpan::LF]) ;
      if ( span_starting_index+1 == length || m_supertags == 0 || m_supertags->getSuperTag(span_starting_index, span_starting_index+1) ) {
         total_span->setBinarySpan(span_starting_index, CSpan::LF);
         total_span->score() = getCrossLinkScore(total_span);
      }
      else
         total_span->clear();

      total_span = &(chart[span_starting_index][2][CSpan::RF]) ;
      if ( span_starting_index+1 == length || m_supertags == 0 || m_supertags->getSuperTag(span_starting_index+1, span_starting_index) ) {
         total_span->setBinarySpan(span_starting_index, CSpan::RF);
         total_span->score() = getCrossLinkScore(total_span);
      }
      else
         total_span->clear();

      total_span = &(chart[span_starting_index][2][CSpan::BF]) ;
      total_span->setBinarySpan(span_starting_index, CSpan::BF);
      total_span->score() = 0 ;
   }

   // compute multiple
   // build spans from the smaller spans up to the biggest one
   // note that the span length can be sentence length+1 (EOS)
   for ( span_length = 3 ; span_length <= length + 1 ; ++ span_length ) {

      // the start index is the end index of the last word
      for ( left_length = 2 ; left_length < span_length ; ++ left_length ) {

         // for each generator
         for ( span_starting_index = 0 ; span_starting_index <= length + 1 - span_length ; ++ span_starting_index ) {

            int span_ending_index = span_starting_index + span_length - 1;

            // LF + LF = LF
            left_span = &(chart[span_starting_index][left_length][CSpan::LF]) ;
            right_span = &(chart[span_starting_index+left_length-1][span_length-left_length+1][CSpan::LF]) ;
            total_span = &(chart[span_starting_index][span_length][CSpan::LF]) ;

            if ( left_span->isActive() && right_span->isActive() && left_span->isMinimal() ) {
               if ( !total_span->isActive() || left_span->score() + right_span->score() > total_span->score() ) {
                  total_span->setCombinedSpan(*left_span, *right_span, CSpan::LF) ;
                  total_span->score() = left_span->score() + right_span->score() ;
               }
            }

            // LF + BF = LF / BF / RF for the spans that do not include EOS
            //           RF for the cover EOS span
            left_span = &(chart[span_starting_index][left_length][CSpan::LF]) ;
            right_span = &(chart[span_starting_index+left_length-1][span_length-left_length+1][CSpan::BF]) ;

            if ( left_span->isActive() && right_span->isActive() && left_span->isMinimal() ) {

               // for a span that does not include the end of the sentence
               if ( span_ending_index < length ) {

                  // LF
                  if ( m_supertags == 0 || m_supertags->getSuperTag(span_starting_index, span_ending_index) ) {
                     total_span = &(chart[span_starting_index][span_length][CSpan::LF]) ;
                     temp_span.setCombinedSpan(*left_span, *right_span, CSpan::LF) ;
                     temp_span.score() = left_span->score() + right_span->score() ;
                     temp_span.score() += getCrossLinkScore(&temp_span) ;
                     if ( !total_span->isActive() || temp_span.score() > total_span->score() ) {
                        total_span->copy( temp_span ) ;
                     }
                  }
               }

               // BF
               total_span = &(chart[span_starting_index][span_length][CSpan::BF]) ;
               if ( !total_span->isActive() || left_span->score() + right_span->score() > total_span->score() ) {
                  total_span->setCombinedSpan(*left_span, *right_span, CSpan::BF) ;
                  total_span->score() = left_span->score() + right_span->score() ;
               }

               // RF
               // When adding right link to EOS, make sure on one has linked to it.
               if ( ( span_ending_index < length || !right_span->isRightLinkedTo() ) &&
                    ( span_ending_index == length || m_supertags == 0 || m_supertags->getSuperTag(span_ending_index, span_starting_index) )
                  ) {
                  total_span = &(chart[span_starting_index][span_length][CSpan::RF]) ;
                  temp_span.setCombinedSpan(*left_span, *right_span, CSpan::RF) ;
                  temp_span.score() = left_span->score() + right_span->score() ;
                  temp_span.score() += getCrossLinkScore(&temp_span) ;
                  if ( !total_span->isActive() || temp_span.score() > total_span->score() ) {
                     total_span->copy( temp_span ) ;
                  }
               }
            }

            // BF + RF = LF / BF / RF for the spans that do not include EOS
            //           RF for the cover EOS span
            left_span = &(chart[span_starting_index][left_length][CSpan::BF]) ;
            right_span = &(chart[span_starting_index+left_length-1][span_length-left_length+1][CSpan::RF]) ;
            //           for a span that does not include the end of the sentence
            if ( left_span->isActive() && right_span->isActive() && left_span->isMinimal() ) {

               // for a span that does not include the end of the sentence
               if ( span_ending_index < length ) {

                  // LF
                  if ( m_supertags == 0 || m_supertags->getSuperTag(span_starting_index, span_ending_index) ) {
                     total_span = &(chart[span_starting_index][span_length][CSpan::LF]) ;
                     temp_span.setCombinedSpan(*left_span, *right_span, CSpan::LF) ;
                     temp_span.score() = left_span->score() + right_span->score() ;
                     temp_span.score() += getCrossLinkScore(&temp_span) ;
                     if ( !total_span->isActive() || temp_span.score() > total_span->score() ) {
                        total_span->copy( temp_span ) ;
                     }
                  }

                  // BF
                  total_span = &(chart[span_starting_index][span_length][CSpan::BF]) ;
                  if ( !total_span->isActive() || left_span->score() + right_span->score() > total_span->score() ) {
                     total_span->setCombinedSpan(*left_span, *right_span, CSpan::BF) ;
                     total_span->score() = left_span->score() + right_span->score() ;
                  }
               }

               // RF
               // When adding right link to EOS, make sure on one has linked to it.
               if ( ( span_ending_index < length || !right_span->isRightLinkedTo() ) &&
                    ( span_ending_index == length || m_supertags == 0 || m_supertags->getSuperTag(span_ending_index, span_starting_index) )
                  ) {
                  total_span = &(chart[span_starting_index][span_length][CSpan::RF]) ;
                  temp_span.setCombinedSpan(*left_span, *right_span, CSpan::RF) ;
                  temp_span.score() = left_span->score() + right_span->score() ;
                  temp_span.score() += getCrossLinkScore(&temp_span) ;
                  if ( !total_span->isActive() || temp_span.score() > total_span->score() ) {
                     total_span->copy( temp_span ) ;
                  }
               }
            }

            // RF + RF = RF
            left_span = &(chart[span_starting_index][left_length][CSpan::RF]) ;
            right_span = &(chart[span_starting_index+left_length-1][span_length-left_length+1][CSpan::RF]) ;
            total_span = &(chart[span_starting_index][span_length][CSpan::RF]) ;

            if ( left_span->isActive() && right_span->isActive() && left_span->isMinimal() ) {
               if ( !total_span->isActive() || left_span->score() + right_span->score() > total_span->score() ) {
                  total_span->setCombinedSpan(*left_span, *right_span, CSpan::RF) ;
                  total_span->score() = left_span->score() + right_span->score() ;
               }
            }
         } // for span_starting_index

      } // for left_length

   } // for span_length

   TRACE("Outputing sentence");
   generate( chart[ 0 ][ length+1 ][ CSpan::RF ] , sentence , this , *retval ) ;
   if (scores) scores[ 0 ] = chart[ 0 ][ length+1 ][ CSpan::RF ].score() ;
   TRACE("Done, the highest score is: " << chart[ 0 ][ length ][ CSpan::RF ].score() ) ;
   TRACE("The total time spent: " << double(clock() - total_start_time)/CLOCKS_PER_SEC) ;
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

   // now update the feature vector when the output doesn't match
   parse( sentence , &output ) ;
   updateScoreVector( output, correct , round ) ;

};


