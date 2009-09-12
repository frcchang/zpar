/****************************************************************
 *                                                              *
 * agendachart.cpp - the joint segmentor tagger and depparser   *
 *                   implementation using the agendachart method. *
 *                                                              *
 * The combination of agenda tagging and covington              *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2008.7                         *
 *                                                              *
 ****************************************************************/

#include "depparser.h"

using namespace chinese;
using namespace chinese::depparser;

// this is used to mark whether a word is correct
static CWord g_emptyWord("");

#define cast_weights static_cast<CWeight*>(m_weights)

/*===============================================================
 *
 * CDepParser - handles the features for the depparser
 *
 *==============================================================*/

/*---------------------------------------------------------------
 *
 * getOrUpdateLocalScore - get or update the local score for a word in sentence
 *
 * When bigram is needed from the beginning of sentence, the
 * -BEGIN- tag and the empty word are used. 
 *
 * This implies that empty words should not be used in other 
 * situations. 
 *
 *--------------------------------------------------------------*/

SCORE_TYPE CDepParser::getOrUpdateLocalScore( const CStringVector *sentence, const CStateItem *item, int index, SCORE_TYPE amount, int round ) {
   static SCORE_TYPE nReturn ; 
   static unsigned long int last_start , last_length ;
   static unsigned long int start , end , length , word_length ; // word length is the un-normalised version
   // about the words
   start = item->getWordStart( index ) ;
   end = item->getWordEnd( index ) ;
   length = item->getWordLength( index ) ; 

   last_start = index > 0 ? item->getWordStart( index-1 ) : 0 ;
   last_length = index > 0 ? item->getWordLength( index-1 ) : 0 ;
   word_length = length ;  // use word_length instead of item->getWordLength() because the length can include " ".

   const CWord &word = amount==0 ? m_WordCache.find( start , end , sentence )
                                 : m_WordCache.replace( start , end , sentence ) ; 

   const CWord &last_word =  index > 0 ? ( amount==0 ? m_WordCache.find( last_start , start-1 , sentence )
                                                     : m_WordCache.replace( last_start , start-1 , sentence ) )
                                       : g_emptyWord ; 

   // about the length
   if( length > LENGTH_MAX-1 ) length = LENGTH_MAX-1 ;
   if( last_length > LENGTH_MAX-1 ) last_length = LENGTH_MAX-1 ;

   // about the chars
   const CWord &first_char = amount==0 ? m_WordCache.find( start , start , sentence )
                                      : m_WordCache.replace( start , start , sentence ) ;
   const CWord &last_char = amount==0 ? m_WordCache.find( end , end , sentence )
                                     : m_WordCache.replace( end , end , sentence ) ;
   const CWord &first_char_last_word = index > 0 ? ( amount==0 ? m_WordCache.find( last_start , last_start , sentence )
                                                               : m_WordCache.replace( last_start , last_start , sentence ) )
                                                 : g_emptyWord ;
   const CWord &last_char_last_word = index > 0 ? ( amount==0 ? m_WordCache.find( start-1 , start-1 , sentence) 
                                                              : m_WordCache.replace( start-1 , start-1 , sentence) )
                                                : g_emptyWord ;
   const CWord &two_char = index > 0 ? ( amount == 0 ? m_WordCache.find( start-1 , start, sentence) 
                                                     : m_WordCache.replace( start-1 , start, sentence) )
                                     : g_emptyWord ;
   const CWord &lastword_firstchar = index > 0 ? ( amount==0 ? m_WordCache.find( last_start , start , sentence ) 
                                                             : m_WordCache.replace( last_start , start , sentence ) )
                                               : g_emptyWord ; 
   const CWord &currentword_lastchar = index > 0 ? ( amount==0 ? m_WordCache.find( start-1 , end , sentence) 
                                                               : m_WordCache.replace( start-1 , end , sentence) )
                                                 : g_emptyWord ;
   const CWord &three_char = ( length == 1 && start > 0 && end < sentence->size()-1 )                   ? 
                                      ( amount==0 ? m_WordCache.find( start-1 , end+1 , sentence ) 
                                                  : m_WordCache.replace( start-1 , end+1 , sentence ) ) : g_emptyWord ;

   static CTwoWords two_word , first_and_last_char , firstchars_twoword , lastchars_twoword ;
   if (amount==0) {
      two_word.refer( &word , &last_word ) ;
      first_and_last_char.refer( &first_char , &last_char ) ;
      firstchars_twoword.refer( &first_char_last_word , &first_char ) ;
      lastchars_twoword.refer( &last_char_last_word , &last_char ) ;
   }
   else {
      two_word.allocate( word, last_word ) ;
      first_and_last_char.allocate( first_char, last_char ) ;
      firstchars_twoword.allocate( first_char_last_word, first_char ) ;
      lastchars_twoword.allocate( last_char_last_word, last_char ) ;
   }

   // about the tags 
   const unsigned long &tag = item->getTag( index ) ;
   const unsigned long &last_tag = index>0 ? item->getTag( index-1 ) : PENN_TAG_BEGIN ;
   const unsigned long &second_last_tag = index>1 ? item->getTag(index-2) : PENN_TAG_BEGIN ;

   static CTaggedWord<CTag> wt1, wt2;
   static CTwoTaggedWords wt12;

   unsigned long first_char_cat = cast_weights->m_mapCharTagDictionary.lookup(first_char) | (static_cast<unsigned long>(1)<<tag) ;
   unsigned long last_char_cat = cast_weights->m_mapCharTagDictionary.lookup(last_char) | (static_cast<unsigned long>(1)<<tag) ;

   static int j ; 

   // adding scores with features
   nReturn = cast_weights->m_mapSeenWords.getOrUpdateScore( word , m_nScoreIndex , amount , round ) ; 
   nReturn += cast_weights->m_mapLastWordByWord.getOrUpdateScore( two_word , m_nScoreIndex , amount , round ) ;

   if ( length == 1 ) {
      nReturn += cast_weights->m_mapOneCharWord.getOrUpdateScore( word , m_nScoreIndex , amount , round ) ;
   }
   else {
      nReturn += cast_weights->m_mapFirstAndLastChars.getOrUpdateScore( first_and_last_char , m_nScoreIndex , amount , round ) ;

      nReturn += cast_weights->m_mapLengthByFirstChar.getOrUpdateScore( make_pair(first_char, length) , m_nScoreIndex , amount , round ) ;
      nReturn += cast_weights->m_mapLengthByLastChar.getOrUpdateScore( make_pair(last_char, length) , m_nScoreIndex , amount , round ) ;

      for (j=0; j<word_length-1; j++)
         nReturn += cast_weights->m_mapConsecutiveChars.getOrUpdateScore( 
                                    amount==0 ? m_WordCache.find(start+j, start+j+1, sentence) 
                                              : m_WordCache.replace(start+j, start+j+1, sentence) , 
                               m_nScoreIndex, amount, round ) ; 
   }
   if ( start > 0 ) {
      nReturn += cast_weights->m_mapSeparateChars.getOrUpdateScore( two_char , m_nScoreIndex , amount , round ) ; 

      nReturn += cast_weights->m_mapCurrentWordLastChar.getOrUpdateScore( currentword_lastchar , m_nScoreIndex , amount , round ) ;
      nReturn += cast_weights->m_mapLastWordFirstChar.getOrUpdateScore( lastword_firstchar , m_nScoreIndex , amount , round ) ;

      nReturn += cast_weights->m_mapFirstCharLastWordByWord.getOrUpdateScore( firstchars_twoword , m_nScoreIndex , amount , round ) ;
      nReturn += cast_weights->m_mapLastWordByLastChar.getOrUpdateScore( lastchars_twoword , m_nScoreIndex , amount , round ) ;

      nReturn += cast_weights->m_mapLengthByLastWord.getOrUpdateScore( make_pair(last_word, length) , m_nScoreIndex , amount , round ) ;
      nReturn += cast_weights->m_mapLastLengthByWord.getOrUpdateScore( make_pair(word, last_length), m_nScoreIndex , amount , round ) ;
   }
  
   nReturn += cast_weights->m_mapCurrentTag.getOrUpdateScore( make_pair(word, tag) , m_nScoreIndex , amount , round ) ; 
   nReturn += cast_weights->m_mapLastTagByTag.getOrUpdateScore( encodeTags( tag, last_tag ), m_nScoreIndex , amount , round ) ;
   nReturn += cast_weights->m_mapLastTwoTagsByTag.getOrUpdateScore( encodeTags( tag, last_tag, second_last_tag ), m_nScoreIndex , amount , round ) ;
   if ( start > 0 ) {
      if ( last_length <= 2 ) nReturn += cast_weights->m_mapTagByLastWord.getOrUpdateScore( make_pair(last_word, tag) , m_nScoreIndex , amount , round ) ;
      if ( length <= 2 ) nReturn += cast_weights->m_mapLastTagByWord.getOrUpdateScore( make_pair(word, last_tag) , m_nScoreIndex , amount , round ) ;
      if ( length <= 2 ) nReturn += cast_weights->m_mapTagByWordAndPrevChar.getOrUpdateScore( make_pair(currentword_lastchar, tag) , m_nScoreIndex , amount , round ) ;
      if ( last_length <= 2 ) nReturn += cast_weights->m_mapTagByWordAndNextChar.getOrUpdateScore( make_pair(lastword_firstchar, last_tag) , m_nScoreIndex , amount , round ) ;
   }
   if ( length == 1 ) {
      if ( start > 0 && end < sentence->size()-1 )
         nReturn += cast_weights->m_mapTagOfOneCharWord.getOrUpdateScore( make_pair(three_char, tag) , m_nScoreIndex , amount , round ) ;
   }
   else {
      nReturn += cast_weights->m_mapTagByFirstChar.getOrUpdateScore( make_pair(first_char, tag) , m_nScoreIndex , amount , round ) ; 
      nReturn += cast_weights->m_mapTagByLastChar.getOrUpdateScore( make_pair(last_char, tag) , m_nScoreIndex , amount , round ) ;
      nReturn += cast_weights->m_mapTagByFirstCharCat.getOrUpdateScore( make_pair(first_char_cat, tag) , m_nScoreIndex , amount , round ) ; 
      nReturn += cast_weights->m_mapTagByLastCharCat.getOrUpdateScore( make_pair(last_char_cat, tag) , m_nScoreIndex , amount , round ) ;

      for ( j = 0 ; j < word_length ; j ++ ) {

         if ( j > 0 && j < word_length-1 )
            nReturn += cast_weights->m_mapTagByChar.getOrUpdateScore( 
                                    make_pair( amount==0 ? m_WordCache.find(start+j, start+j, sentence)
                                                         : m_WordCache.replace(start+j, start+j, sentence), tag), 
                                    m_nScoreIndex , amount , round ) ;

         if ( j > 0 ) {

            if (amount==0) {
               wt1.load( m_WordCache.find(start+j, start+j, sentence) , tag ); 
               wt2.load(first_char); 
               wt12.refer(&wt1, &wt2); 
            }
            else {
               wt1.load( m_WordCache.replace(start+j, start+j, sentence) , tag ); 
               wt2.load(first_char); 
               wt12.allocate(wt1, wt2);
            }
            nReturn += cast_weights->m_mapTaggedCharByFirstChar.getOrUpdateScore(wt12, m_nScoreIndex, amount, round) ;

            if ( m_WordCache.find(start+j, start+j, sentence) == m_WordCache.find(start+j-1, start+j-1, sentence)) 
               nReturn += cast_weights->m_mapRepeatedCharByTag.getOrUpdateScore( 
                                       make_pair( amount==0 ? m_WordCache.find(start+j, start+j, sentence)
                                                            : m_WordCache.replace(start+j, start+j, sentence), tag), 
                                       m_nScoreIndex, amount, round) ;
         }
         if ( j < word_length-1 ) {
            if (amount==0) {
               wt1.load( m_WordCache.find(start+j, start+j, sentence) , tag ); 
               wt2.load(last_char); 
               wt12.refer(&wt1, &wt2); 
            }
            else {
               wt1.load( m_WordCache.replace(start+j, start+j, sentence) , tag ); 
               wt2.load(last_char); 
               wt12.allocate(wt1, wt2); 
            }
            nReturn += cast_weights->m_mapTaggedCharByLastChar.getOrUpdateScore(wt12, m_nScoreIndex, amount, round) ;
         }
      }
   }

   return nReturn;
}

/*---------------------------------------------------------------
 * 
 * getOrUpdateArcScore - get or update the score from a single dependency link
 *
 *---------------------------------------------------------------*/

SCORE_TYPE CDepParser::getOrUpdateArcScore( const CStringVector *sentence, const CStateItem *item, const int &head_index , const int &dep_index, SCORE_TYPE amount, int round ) {

   static SCORE_TYPE retval ; 

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

   //-------------------------sibling features from McDonald-----------------------
/*
   getOrUpdateSiblingScoreTemplate(0) ;
   getOrUpdateSiblingScoreTemplate(sibling_distance_encode) ;
   getOrUpdateSiblingScoreTemplate(sibling_direction_encode) ;
*/

   return retval;
}

/*---------------------------------------------------------------
 *
 * updateScoreForState - update a single positive or negative output
 *
 *--------------------------------------------------------------*/

inline void CDepParser::updateScoreForState( const CStringVector *sent , const CStateItem *output , bool bComplete , const SCORE_TYPE &amount ) {
   static CStateItem item;
   static int index, prev;
   static int end_index;
   item.clear();

   for ( index=0; index<output->size(); index++ )
      item.append( output->getWordEnd(index), output->getTag(index) );

   end_index = bComplete ? output->size() : output->size()-1;
   // rebuild parsetrees
   for ( index=0; index<end_index; index++ ) {
      prev = index-1;
      while ( prev != -1 ) {
         if ( output->head( index ) == prev ) {
            // add link from index to prev point
            item.addLink( prev, index );
            getOrUpdateArcScore( sent, &item, prev, index, amount, m_nTrainingRound );
            // break to finish index
            break;
         }
         else if ( output->head( prev ) == index ) {
            // add link from prev to index
            item.addLink( index, prev ) ;
            getOrUpdateArcScore( sent, &item, index, prev, amount, m_nTrainingRound );
         }
         // break if the prev is still local head -- can't over
         if ( item.head( prev ) == DEPENDENCY_LINK_NO_HEAD )
            break;
         // move to the prev
         prev = item.findPreviousLinkPoint( prev );
      }
   }
}

/*---------------------------------------------------------------
 *
 * updateScoresForStates - update scores for states
 *
 *--------------------------------------------------------------*/

void CDepParser::updateScoresForStates( const CStringVector *raw, const CStateItem *output, const CStateItem *correct, bool bComplete ) {
   assert( output->charactersize() == correct->charactersize() );

   static int i, j;

   static CDependencyTree tree;
   output->GenerateTree(*raw, tree); TRACE(tree); 

   for (i=0; i<correct->size(); ++i)
      getOrUpdateLocalScore(raw, correct, i, 1, m_nTrainingRound);
   for (i=0; i<output->size(); ++i)
      getOrUpdateLocalScore(raw, output, i, -1, m_nTrainingRound);

   updateScoreForState( raw, correct, bComplete, 1 ) ;
   updateScoreForState( raw, output, bComplete, -1 ) ;

   m_nTotalErrors++;
}

/*---------------------------------------------------------------
 *
 * updateInfo - update the info like maximum length by tag etc.
 *
 *--------------------------------------------------------------*/

void CDepParser::updateInfo( const CDependencyTree* sent ) {

   static int i , j ;

   // Updates that are common for all example
   for ( i=0; i<sent->size(); ++i ) {

      const CWord &word = sent->at(i).word ;
      unsigned long tag = CTag( sent->at(i).tag ).code() ;

      CStringVector chars;
      chars.clear(); 
      getCharactersFromUTF8String(sent->at(i).word, &chars);

      cast_weights->m_mapWordFrequency[word]++;
      if (cast_weights->m_mapWordFrequency[word]>cast_weights->m_nMaxWordFrequency) cast_weights->m_nMaxWordFrequency = cast_weights->m_mapWordFrequency[word];

      cast_weights->m_mapTagDictionary.add(word, tag);
      for ( j = 0 ; j < chars.size() ; j++ ) cast_weights->m_mapCharTagDictionary.add(chars[j], tag) ;

      if ( !m_bKnowledgeLoaded ||
          (!m_Knowledge.isFWorCD(chars[0])&&!m_Knowledge.isFWorCD(chars[chars.size()-1])))
      cast_weights->setMaxLengthByTag( tag , chars.size() ) ;
   }

}

/*---------------------------------------------------------------
 *
 * addLink - add link to stateitem
 *
 *---------------------------------------------------------------*/

void CDepParser::addLink( const CStringVector *sentence, CStateItem *item, const int head_index, const int dep_index){
   item->addLink(head_index, dep_index);
   item->score += getOrUpdateArcScore( sentence, item, head_index, dep_index );
}

/*---------------------------------------------------------------
 * 
 * enumerateCandidates - add possible links to a segmented and tagged item
 *
 *--------------------------------------------------------------*/

void CDepParser::enumerateCandidates( const CStringVector *sentence, const CStateItem *item, const int agenda_index, const unsigned long tag ) {

   static int index, prev;
   static int first_head;
   static CStateItem temp, copy;
   const bool bLast = (tag == PENN_TAG_END);

   // for the last action, we add links to the final word in the sentence; 
   // for all others, we add links to the second last word. 
   index = item->size()-1 ;
   temp = *item ; 
   if (!bLast) { 
      temp.append(agenda_index-1, tag);
      temp.score += getOrUpdateLocalScore( sentence, &temp, temp.size()-1 );
   }
   first_head = temp.findFirstHead() ;

   // no link to left (head /dep)
   if ( !bLast || temp.size() == 1 ) {
      m_Chart[agenda_index]->insertItem(&temp);
   }

   prev = index - 1 ; // start from the previous word, because no links have been made to index
   while ( prev > DEPENDENCY_LINK_NO_HEAD ) {             // asserted DEPENDENCY_LINK_NO_HEAD==-1 at constructor
      assert( temp.head(prev) < prev ) ;                  // no head or head on the left for a possible linkpoint
      // link from index to prev
      if ( !bLast || first_head == prev ) {
         copy = temp ;
         addLink(sentence, &copy, prev, index) ;
         m_Chart[agenda_index]->insertItem(&copy);
      }
      // move prev backward and add link from prev to index
      if ( temp.head(prev) == DEPENDENCY_LINK_NO_HEAD ) { // if the word currently has no head
         addLink(sentence, &temp, index, prev) ;  // it must be linked to index
         if ( !bLast || first_head == prev ) {  // and if it is allowed 
            m_Chart[agenda_index]->insertItem(&temp);
         }
      } 
      prev = temp.findPreviousLinkPoint( prev ) ;          // move to prev linkpoints
   }
}

/*---------------------------------------------------------------
 *
 * train - train the module with a correct output
 *
 *--------------------------------------------------------------*/

void CDepParser::train( const CDependencyTree &correct , int round ) {
   static CStringVector input;

   assert(IsProjectiveDependencyTree(correct));
   UnparseAndDesegmentSentence( &correct, &input );

   m_nTrainingRound = round ;

   work( &input, 0, &correct );
   updateInfo( &correct );
}

/*---------------------------------------------------------------
 *
 * parse - segment tag and parse sentence
 *
 * Returns: makes a new instance of CTwoStringVector 
 *
 *--------------------------------------------------------------*/

void CDepParser::parse( const CStringVector &sentence_input , CDependencyTree &vReturn ) {
   work( &sentence_input, &vReturn, 0 );
}

/*---------------------------------------------------------------
 *
 * work - the working func for both train and parse
 *
 * Returns: makes a new instance of CTwoStringVector 
 *
 *--------------------------------------------------------------*/

void CDepParser::work( const CStringVector *sentence_input , CDependencyTree *vReturn , const CDependencyTree *correct ) {

   clock_t total_start_time = clock();;
   int index , start_index , generator_index , temp_index, word_length;
   const CStateItem * generator_item ; 
   CStateItem tempState , maxState , correctState;
   unsigned long tag, last_tag ; 

   static CStringVector sentence;
   static CSegmentationPrune rules(MAX_SENTENCE_SIZE); 
   rules.load(*sentence_input, sentence);
   const int length = sentence.size() ;

   assert(length<MAX_SENTENCE_SIZE);

   TRACE("Initialising the parsing process...");
   m_WordCache.clear() ; 
   m_Chart.clear() ;
   // put an empty sentence to the beginning 
   tempState.clear() ;
   m_Chart[ 0 ]->insertItem( &tempState ) ;

   // correct stat
   static bool bEndOfWord ; // current char
   if (correct) {
      correctState.clear();
   }
   static bool bCorrect;

   TRACE("Parsing started"); 
   // enumerating the end index
   // =========================
   // index is the word index starting from 0
   for ( index = 0 ; index < length ; ++ index ) {

      // m_Chart index 1 correspond to the first char
      m_Chart[ index + 1 ] ; // this is to make some necessary initialisation for each agenda, when pruning

      // control for the ending character of the candidate 
      if ( index < length-1 && rules.canSeparate(index+1)==false ) 
         continue ; 

      // enumerating the possible tags
      // =============================
      // the tag 0 is the NONE tag, and tag 1 is the BEGIN tag
      for ( tag = PENN_TAG_FIRST ; tag < PENN_TAG_COUNT ; ++ tag ) {

         start_index = index-1 ; // the end index of last word
         word_length = 1 ; // current word length

         // enumerating the start index
         // ===========================
         // the start index of the word is actually start_index + 1
         while( start_index >= -1 && word_length <= cast_weights->m_maxLengthByTag[ tag ] ) {

            // control for the starting character of the candidate
            // ---------------------------------------------------
            while ( start_index >= 0 && rules.canSeparate(start_index+1)==false )
               start_index-- ; 

            // start the search process, generating words
            // ------------------------------------------
            if (  (  (  cast_weights->m_mapWordFrequency.find( m_WordCache.find( start_index+1 , index , &sentence ) , 0 ) < 
                        cast_weights->m_nMaxWordFrequency/5000+5 && 
                        PENN_TAG_CLOSED[ tag ] == false  ) ||
                     cast_weights->m_mapTagDictionary.lookup( m_WordCache.find( start_index+1 , index , &sentence ), tag ) 
                  ) // wordtag match
               ) {

               for ( generator_index = 0 ; generator_index < m_Chart[ start_index+1 ]->size() ; ++ generator_index ) {
                  // for each possible tagged word generate dependency
                  enumerateCandidates( &sentence, m_Chart[ start_index+1 ]->item( generator_index ), index+1, tag );
               }

            }//if

            // control the first character of the candidate
            if ( rules.canAppend(start_index+1)==false ) 
               break ; 

            // update start index and word len
            --start_index ;
            ++word_length ;

         }//start_index
      }//tag

      // update scores during the training process when necessary
      if (correct) {
         bEndOfWord = correctState.StandardMove( *correct, index );
         // update states when the current index is at the end of a word in the correct sent
         if (bEndOfWord) {
            bCorrect = false;
            for ( generator_index = 0 ; generator_index < m_Chart[index+1]->size() ; ++ generator_index ) {
               if ( *(m_Chart[index+1]->item( generator_index )) == correctState )
                  bCorrect = true;
            }
            if (!bCorrect) {
               cout << "Error at the " << index << "th character. " << endl;
               updateScoresForStates( &sentence, m_Chart[index+1]->bestItem(), &correctState, false );
               return;
            }
         }
      }

   }//index

   for ( generator_index = 0 ; generator_index < m_Chart[ length ]->size() ; ++ generator_index ) {
      enumerateCandidates( &sentence, m_Chart[ length ]->item( generator_index ), length+1, PENN_TAG_END );
   }
   if (correct) {
      // update states since this is the finish of a sent
      correctState.StandardFinish( *correct );
      if (*(m_Chart[length+1]->bestItem())!=correctState) {
         cout << "Error at the last processing step here. " << endl;
         updateScoresForStates( &sentence, m_Chart[length+1]->bestItem(), &correctState, true );
         return;
      }
   }

   if (vReturn) {
      TRACE("Outputing sentence");
      vReturn->clear() ; 
      m_Chart[ length+1 ]->bestItem()->GenerateTree( sentence , *vReturn ) ; 
   }
   TRACE("Done, the highest score is: " << m_Chart[ length+1 ]->bestItem( 0 )->score) ;
   TRACE("The total time spent: " << double(clock() - total_start_time)/CLOCKS_PER_SEC) ;
}




