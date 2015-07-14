// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                              *
 * agendachart.cpp - the agendachart tagger implementation.     *
 *                                                              *
 * The combination of agenda and rt                             *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2007.5                         *
 *                                                              *
 ****************************************************************/

#include "tagger.h"

using namespace chinese;
using namespace chinese::tagger;

// this is used to mark whether a word is correct
static CWord g_emptyWord("");

/*===============================================================
 *
 * CTagger - handles the features for the tagger
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

SCORE_TYPE CTagger::getOrUpdateLocalScore( const CStringVector *sentence, const CStateItem *item, unsigned long index, SCORE_TYPE amount, unsigned long round ) {
   static SCORE_TYPE nReturn ;
   static unsigned long last_start , last_length ;
   static unsigned long start , end , length , word_length ; // word length is the un-normalised version
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
   const CTag &tag = item->getTag( index ) ;
   const CTag &last_tag = index>0 ? item->getTag( index-1 ) : CTag(CTag::SENTENCE_BEGIN) ;
   const CTag &second_last_tag = index>1 ? item->getTag(index-2) : CTag(CTag::SENTENCE_BEGIN) ;

   static CTaggedWord<CTag, TAG_SEPARATOR> wt1, wt2;
   static CTwoTaggedWords wt12;

   unsigned long long first_char_cat = m_weights->m_mapCharTagDictionary.lookup(first_char) | (static_cast<unsigned long long>(1)<<tag.code()) ;
   unsigned long long last_char_cat = m_weights->m_mapCharTagDictionary.lookup(last_char) | (static_cast<unsigned long long>(1)<<tag.code()) ;

   static int j ;

   // adding scores with features
   nReturn = m_weights->m_mapSeenWords.getOrUpdateScore( word , m_nScoreIndex , amount , round ) ;
   nReturn += m_weights->m_mapLastWordByWord.getOrUpdateScore( two_word , m_nScoreIndex , amount , round ) ;

   if ( length == 1 ) {
      nReturn += m_weights->m_mapOneCharWord.getOrUpdateScore( word , m_nScoreIndex , amount , round ) ;
   }
   else {
      nReturn += m_weights->m_mapFirstAndLastChars.getOrUpdateScore( first_and_last_char , m_nScoreIndex , amount , round ) ;

      nReturn += m_weights->m_mapLengthByFirstChar.getOrUpdateScore( std::make_pair(first_char, length) , m_nScoreIndex , amount , round ) ;
      nReturn += m_weights->m_mapLengthByLastChar.getOrUpdateScore( std::make_pair(last_char, length) , m_nScoreIndex , amount , round ) ;

      for (j=0; j<word_length-1; ++j)
         nReturn += m_weights->m_mapConsecutiveChars.getOrUpdateScore(
                                    amount==0 ? m_WordCache.find(start+j, start+j+1, sentence)
                                              : m_WordCache.replace(start+j, start+j+1, sentence) ,
                               m_nScoreIndex, amount, round ) ;
   }
   if ( start > 0 ) {
      nReturn += m_weights->m_mapSeparateChars.getOrUpdateScore( two_char , m_nScoreIndex , amount , round ) ;

      nReturn += m_weights->m_mapCurrentWordLastChar.getOrUpdateScore( currentword_lastchar , m_nScoreIndex , amount , round ) ;
      nReturn += m_weights->m_mapLastWordFirstChar.getOrUpdateScore( lastword_firstchar , m_nScoreIndex , amount , round ) ;

      nReturn += m_weights->m_mapFirstCharLastWordByWord.getOrUpdateScore( firstchars_twoword , m_nScoreIndex , amount , round ) ;
      nReturn += m_weights->m_mapLastWordByLastChar.getOrUpdateScore( lastchars_twoword , m_nScoreIndex , amount , round ) ;

      nReturn += m_weights->m_mapLengthByLastWord.getOrUpdateScore( std::make_pair(last_word, length) , m_nScoreIndex , amount , round ) ;
      nReturn += m_weights->m_mapLastLengthByWord.getOrUpdateScore( std::make_pair(word, last_length), m_nScoreIndex , amount , round ) ;
   }

   nReturn += m_weights->m_mapCurrentTag.getOrUpdateScore( std::make_pair(word, tag) , m_nScoreIndex , amount , round ) ;
   nReturn += m_weights->m_mapLastTagByTag.getOrUpdateScore( CTagSet<CTag, 2>(encodeTags( tag, last_tag )), m_nScoreIndex , amount , round ) ;
   nReturn += m_weights->m_mapLastTwoTagsByTag.getOrUpdateScore( CTagSet<CTag, 3>(encodeTags( tag, last_tag, second_last_tag )), m_nScoreIndex , amount , round ) ;
   if ( start > 0 ) {
      if ( last_length <= 2 ) nReturn += m_weights->m_mapTagByLastWord.getOrUpdateScore( std::make_pair(last_word, tag) , m_nScoreIndex , amount , round ) ;
      if ( length <= 2 ) nReturn += m_weights->m_mapLastTagByWord.getOrUpdateScore( std::make_pair(word, last_tag) , m_nScoreIndex , amount , round ) ;
      if ( length <= 2 ) nReturn += m_weights->m_mapTagByWordAndPrevChar.getOrUpdateScore( std::make_pair(currentword_lastchar, tag) , m_nScoreIndex , amount , round ) ;
      if ( last_length <= 2 ) nReturn += m_weights->m_mapTagByWordAndNextChar.getOrUpdateScore( std::make_pair(lastword_firstchar, last_tag) , m_nScoreIndex , amount , round ) ;
   }
   if ( length == 1 ) {
      if ( start > 0 && end < sentence->size()-1 )
         nReturn += m_weights->m_mapTagOfOneCharWord.getOrUpdateScore( std::make_pair(three_char, tag) , m_nScoreIndex , amount , round ) ;
   }
   else {
      nReturn += m_weights->m_mapTagByFirstChar.getOrUpdateScore( std::make_pair(first_char, tag) , m_nScoreIndex , amount , round ) ;
      nReturn += m_weights->m_mapTagByLastChar.getOrUpdateScore( std::make_pair(last_char, tag) , m_nScoreIndex , amount , round ) ;
      nReturn += m_weights->m_mapTagByFirstCharCat.getOrUpdateScore( std::make_pair(first_char_cat, tag) , m_nScoreIndex , amount , round ) ;
      nReturn += m_weights->m_mapTagByLastCharCat.getOrUpdateScore( std::make_pair(last_char_cat, tag) , m_nScoreIndex , amount , round ) ;

      for ( j = 0 ; j < word_length ; ++j ) {

         if ( j > 0 && j < word_length-1 )
            nReturn += m_weights->m_mapTagByChar.getOrUpdateScore(
                                    std::make_pair( amount==0 ? m_WordCache.find(start+j, start+j, sentence)
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
            nReturn += m_weights->m_mapTaggedCharByFirstChar.getOrUpdateScore(wt12, m_nScoreIndex, amount, round) ;

            if ( m_WordCache.find(start+j, start+j, sentence) == m_WordCache.find(start+j-1, start+j-1, sentence))
               nReturn += m_weights->m_mapRepeatedCharByTag.getOrUpdateScore(
                                       std::make_pair( amount==0 ? m_WordCache.find(start+j, start+j, sentence)
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
            nReturn += m_weights->m_mapTaggedCharByLastChar.getOrUpdateScore(wt12, m_nScoreIndex, amount, round) ;
         }
      }
   }

   return nReturn;
}

/*---------------------------------------------------------------
 *
 * buildStateItem - builds item
 *
 * Inputs: the raw sentence in input format
 *         the tagged sentence in output format
 *         the state item which is retval
 *
 *--------------------------------------------------------------*/

inline void buildStateItem(const CStringVector *raw, const CTwoStringVector *tagged, CStateItem *item) {
   static int i, ri, rawlen, taggedlen;
   item->clear();
   // add each output word
   rawlen = 0; ri = 0; // raw index
   taggedlen = 0;
   for (i=0; i<tagged->size(); ++i) {
      taggedlen += tagged->at(i).first.size();
      while ( rawlen < taggedlen ) {
         rawlen += raw->at(ri).size();
         ++ri;
      }
      item->append(ri-1, CTag(tagged->at(i).second).code());
   }
}

/*---------------------------------------------------------------
 *
 * getGlobalScore - get the score of particular outputs
 *
 * Inputs: the tagged and segmented output
 *
 *--------------------------------------------------------------*/

SCORE_TYPE CTagger::getGlobalScore(const CTwoStringVector* tagged) {

   static int i;

   static CStateItem item ;
   static CStringVector raw;

   static SCORE_TYPE rv; rv=0;

   raw.clear();
   for (i=0; i<tagged->size(); ++i)
      getCharactersFromUTF8String(tagged->at(i).first, &raw);

   buildStateItem( &raw, tagged, &item );
   for (i=0; i<tagged->size(); ++i)
      rv += getOrUpdateLocalScore(&raw, &item, i);

   return rv;
}

/*---------------------------------------------------------------
 *
 * updateScores - update the score std::vector by input
 *                this is used in training to adjust params
 *
 * Inputs: the tagged and the correct example
 *
 *--------------------------------------------------------------*/

void CTagger::updateScores(const CTwoStringVector* tagged, const CTwoStringVector* correct, unsigned long round) {

   static int i , j ;
   static CStateItem item ;
   static CStringVector raw;

   if ( *tagged != *correct ) {

      // get raw sentence from tagged output
      raw.clear();
      for (i=0; i<tagged->size(); ++i)
         getCharactersFromUTF8String(tagged->at(i).first, &raw);

      buildStateItem( &raw, tagged, &item );
      for (i=0; i<tagged->size(); ++i)
         getOrUpdateLocalScore(&raw, &item, i, -1, round);


      buildStateItem( &raw, correct, &item );
      for (i=0; i<correct->size(); ++i)
         getOrUpdateLocalScore(&raw, &item, i, 1, round);

   }

   if ( round > m_nNumberOfCurrentTrainingExample ) {

      m_nNumberOfCurrentTrainingExample = round ;

      // Updates that are common for all example
      for ( i=0; i<correct->size(); ++i ) {

         const CWord &word = correct->at(i).first ;
         unsigned long tag = CTag( correct->at(i).second ).code() ;

         CStringVector chars;
         chars.clear();
         getCharactersFromUTF8String(correct->at(i).first, &chars);

         m_weights->m_mapWordFrequency[word]++;
         if (m_weights->m_mapWordFrequency[word]>m_weights->m_nMaxWordFrequency) m_weights->m_nMaxWordFrequency = m_weights->m_mapWordFrequency[word];

         m_weights->m_mapTagDictionary.add(word, tag);
         for ( j = 0 ; j < chars.size() ; ++j ) m_weights->m_mapCharTagDictionary.add(chars[j], tag) ;

         if ( !m_weights->m_Knowledge ||
             (!m_weights->m_Knowledge->isFWorCD(chars[0])&&!m_weights->m_Knowledge->isFWorCD(chars[chars.size()-1])))
         m_weights->setMaxLengthByTag( tag , chars.size() ) ;
      }

   }

}

/*---------------------------------------------------------------
 *
 * generate - helper function that generates tagged output
 *
 *--------------------------------------------------------------*/

void generate(const CStateItem *stateItem, CStringVector *sentence, CTagger *tagger, CTwoStringVector *vReturn) {
   std::string s;
   for (int j=0; j<stateItem->size(); ++j) {
      s.clear();
      for (int k=stateItem->getWordStart(j); k<stateItem->getWordEnd(j)+1; ++k) {
         assert(sentence->at(k)!=" "); //[--SPACE--]
         s += sentence->at(k);
      }
      vReturn->push_back(std::make_pair(s, stateItem->getTag(j).str()));
   }
}

/*---------------------------------------------------------------
 *
 * train - train the module auto
 *
 * Since we rely on external trainer, this method is empty
 *
 *--------------------------------------------------------------*/

bool CTagger::train( const CStringVector * sentence , const CTwoStringVector * correct) {
   static CTwoStringVector output;
   ++m_nTrainingRound;
   tag( sentence, &output, NULL, 1, NULL );
   updateScores(&output, correct, m_nTrainingRound);
   if (output != *correct) {
      return true;
   }
   return false;
}

/*---------------------------------------------------------------
 *
 * tag - assign POS tags to a sentence
 *
 * Returns: makes a new instance of CTwoStringVector
 *
 *--------------------------------------------------------------*/

void CTagger::tag( const CStringVector * sentence_input , CTwoStringVector * vReturn , SCORE_TYPE * out_scores , unsigned long nBest , const CBitArray * prunes ) {
   clock_t total_start_time = clock();;
   int index , start_index , generator_index , temp_index, word_length;
   const CStateItem * generator_item ;
   CStateItem *candidate_item , tempState , maxState ;
   static CStateItem best_bigram[ 1<<CTag::SIZE ] ;
   unsigned long long best_bigram_mask = 0; // and the count
   unsigned long tag, last_tag ;

   static CStringVector sentence;
   static CRule rules(m_weights->m_bSegmentationRules);
   rules.segment(sentence_input, &sentence);
   const int length = sentence.size() ;

   if (length>=m_nMaxSentSize)
      THROW("the length of the sentence is bigger than the maximum sentence size "<<m_nMaxSentSize<<"; try changing the option");

   assert(vReturn!=NULL);

   TRACE("Initialising the tagging process...");
   m_WordCache.clear() ;
   m_Chart.clear() ;
   // put an empty sentence to the beginning
   tempState.clear() ;
   m_Chart[ 0 ]->insertItem( &tempState ) ;

   TRACE("Tagging started");
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
      for ( tag = CTag::FIRST ; tag <= CTag::LAST ; ++ tag ) {

         start_index = index-1 ; // the end index of last word
         word_length = 1 ; // current word length

         // enumerating the start index
         // ===========================
         // the start index of the word is actually start_index + 1
         while( start_index >= -1 && word_length <= m_weights->m_maxLengthByTag[ tag ] ) {

            // control for the starting character of the candidate
            // ---------------------------------------------------
            while ( start_index >= 0 && rules.canSeparate(start_index+1)==false )
               start_index-- ;

            // start the search process
            // ------------------------
            // with pruning
            if (  ( prunes==NULL || prunes->isset( ( start_index+1 ) * m_nMaxSentSize + index ) ) && // not pruned
                  (  (  m_weights->m_mapWordFrequency.find( m_WordCache.find( start_index+1 , index , &sentence ) , 0 ) <
                        m_weights->m_nMaxWordFrequency/5000+5 &&
                        PENN_TAG_CLOSED[ tag ] == false  ) ||
                     m_weights->m_mapTagDictionary.lookup( m_WordCache.find( start_index+1 , index , &sentence ), tag )
                  ) // wordtag match
               ) {

               if (nBest==1) best_bigram_mask=0LL;

               for ( generator_index = 0 ; generator_index < m_Chart[ start_index+1 ]->size() ; ++ generator_index ) {
                  generator_item = m_Chart[ start_index+1 ]->item( generator_index ) ;
                  tempState.copy( generator_item ) ;
                  tempState.append( index , tag ) ;
                  tempState.score += getOrUpdateLocalScore( &sentence , &tempState , tempState.size()-1 ) ;
                  if (nBest==1) {
                     last_tag = tempState.size()>1 ? tempState.getTag(tempState.size()-2).code() : CTag::SENTENCE_BEGIN;
                     if ( ((best_bigram_mask&(1LL<<last_tag))==0LL) || best_bigram[last_tag].score < tempState.score ) {
                        best_bigram_mask|=(1LL<<last_tag);
                        best_bigram[last_tag].copy(&tempState);
                     }
                  }
                  else {
                     m_Chart[ index+1 ]->insertItem( &tempState );
                  }
               }
               if (nBest==1) {
                  for ( last_tag=0; last_tag<CTag::COUNT; ++last_tag ) {
                     if ( (best_bigram_mask&(1LL<<last_tag)) )
                        m_Chart[ index+1 ]->insertItem( &(best_bigram[last_tag]) );
                  }
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
   }//index

   TRACE("Outputing sentence");
   for ( temp_index = 0 ; temp_index < nBest ; ++ temp_index ) {
      vReturn[ temp_index ].clear() ;
         if (out_scores) out_scores[ temp_index ] = 0 ;
      if ( temp_index < m_Chart[length]->size() ) {
         generate( m_Chart[ length ]->bestItem( temp_index ) , &sentence , this , &(vReturn[ temp_index ]) ) ;
         if (out_scores) out_scores[ temp_index ] = m_Chart[ length ]->bestItem( temp_index )->score ;
      }
   }
   TRACE("Done, the highest score is: " << m_Chart[ length ]->bestItem( 0 )->score) ;
   TRACE("The total time spent: " << double(clock() - total_start_time)/CLOCKS_PER_SEC) ;
}




