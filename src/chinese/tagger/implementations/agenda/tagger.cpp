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

SCORE_TYPE CTagger::getOrUpdateLocalScore( const CStringVector *sentence, const CSubStateItem *item, unsigned long index, SCORE_TYPE amount, unsigned long round ) {
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

   static CTaggedWord<CTag> wt1, wt2;
   static CTwoTaggedWords wt12;

   unsigned long first_char_cat = m_weights->m_mapCharTagDictionary.lookup(first_char) | (static_cast<unsigned long>(1)<<tag.code()) ;
   unsigned long last_char_cat = m_weights->m_mapCharTagDictionary.lookup(last_char) | (static_cast<unsigned long>(1)<<tag.code()) ;

   static int j ; 

   // adding scores with features
   nReturn = m_weights->m_mapSeenWords.getOrUpdateScore( word , m_nScoreIndex , amount , round ) ; 
   nReturn += m_weights->m_mapLastWordByWord.getOrUpdateScore( two_word , m_nScoreIndex , amount , round ) ;

   if ( length == 1 ) {
      nReturn += m_weights->m_mapOneCharWord.getOrUpdateScore( word , m_nScoreIndex , amount , round ) ;
   }
   else {
      nReturn += m_weights->m_mapFirstAndLastChars.getOrUpdateScore( first_and_last_char , m_nScoreIndex , amount , round ) ;

      nReturn += m_weights->m_mapLengthByFirstChar.getOrUpdateScore( make_pair(first_char, length) , m_nScoreIndex , amount , round ) ;
      nReturn += m_weights->m_mapLengthByLastChar.getOrUpdateScore( make_pair(last_char, length) , m_nScoreIndex , amount , round ) ;

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

      nReturn += m_weights->m_mapLengthByLastWord.getOrUpdateScore( make_pair(last_word, length) , m_nScoreIndex , amount , round ) ;
      nReturn += m_weights->m_mapLastLengthByWord.getOrUpdateScore( make_pair(word, last_length), m_nScoreIndex , amount , round ) ;
   }
  
   nReturn += m_weights->m_mapCurrentTag.getOrUpdateScore( make_pair(word, tag) , m_nScoreIndex , amount , round ) ; 
   nReturn += m_weights->m_mapLastTagByTag.getOrUpdateScore( CTagSet<2>(encodeTags( tag, last_tag )), m_nScoreIndex , amount , round ) ;
   nReturn += m_weights->m_mapLastTwoTagsByTag.getOrUpdateScore( CTagSet<3>(encodeTags( tag, last_tag, second_last_tag )), m_nScoreIndex , amount , round ) ;
   if ( start > 0 ) {
      if ( last_length <= 2 ) nReturn += m_weights->m_mapTagByLastWord.getOrUpdateScore( make_pair(last_word, tag) , m_nScoreIndex , amount , round ) ;
      if ( length <= 2 ) nReturn += m_weights->m_mapLastTagByWord.getOrUpdateScore( make_pair(word, last_tag) , m_nScoreIndex , amount , round ) ;
      if ( length <= 2 ) nReturn += m_weights->m_mapTagByWordAndPrevChar.getOrUpdateScore( make_pair(currentword_lastchar, tag) , m_nScoreIndex , amount , round ) ;
      if ( last_length <= 2 ) nReturn += m_weights->m_mapTagByWordAndNextChar.getOrUpdateScore( make_pair(lastword_firstchar, last_tag) , m_nScoreIndex , amount , round ) ;
   }
   if ( length == 1 ) {
      if ( start > 0 && end < sentence->size()-1 )
         nReturn += m_weights->m_mapTagOfOneCharWord.getOrUpdateScore( make_pair(three_char, tag) , m_nScoreIndex , amount , round ) ;
   }
   else {
      nReturn += m_weights->m_mapTagByFirstChar.getOrUpdateScore( make_pair(first_char, tag) , m_nScoreIndex , amount , round ) ; 
      nReturn += m_weights->m_mapTagByLastChar.getOrUpdateScore( make_pair(last_char, tag) , m_nScoreIndex , amount , round ) ;
      nReturn += m_weights->m_mapTagByFirstCharCat.getOrUpdateScore( make_pair(first_char_cat, tag) , m_nScoreIndex , amount , round ) ; 
      nReturn += m_weights->m_mapTagByLastCharCat.getOrUpdateScore( make_pair(last_char_cat, tag) , m_nScoreIndex , amount , round ) ;

      for ( j = 0 ; j < word_length ; ++j ) {

         if ( j > 0 && j < word_length-1 )
            nReturn += m_weights->m_mapTagByChar.getOrUpdateScore( 
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
            nReturn += m_weights->m_mapTaggedCharByFirstChar.getOrUpdateScore(wt12, m_nScoreIndex, amount, round) ;

            if ( m_WordCache.find(start+j, start+j, sentence) == m_WordCache.find(start+j-1, start+j-1, sentence)) 
               nReturn += m_weights->m_mapRepeatedCharByTag.getOrUpdateScore( 
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

inline void buildStateItem(const CStringVector *raw, const CTwoStringVector *tagged, CSubStateItem *item) {
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

   static CSubStateItem item ;
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
 * updateScores - update the score vector by input
 *                this is used in training to adjust params
 *
 * Inputs: the tagged and the correct example
 *
 *--------------------------------------------------------------*/

void CTagger::updateScores(const CTwoStringVector* tagged, const CTwoStringVector* correct, unsigned long round) {

   static int i , j ;
   static CSubStateItem item ;
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

void generate(const CSubStateItem *stateItem, CStringVector *sentence, CTagger *tagger, CTwoStringVector *vReturn) {
   string s;
   for (int j=0; j<stateItem->size(); ++j) { 
      s.clear();
      for (int k=stateItem->getWordStart(j); k<stateItem->getWordEnd(j)+1; ++k) {
         assert(sentence->at(k)!=" "); //[--SPACE--]
         s += sentence->at(k);
      }
      vReturn->push_back(make_pair(s, stateItem->getTag(j).str()));
   }
}

/*---------------------------------------------------------------
 *
 * train - train the module auto
 *
 * Since we rely on external trainer, this method is empty
 *
 *--------------------------------------------------------------*/

void CTagger::train( const CStringVector * sentence , const CTwoStringVector * correct) {
   ++m_nTrainingRound ;
   tag( sentence, NULL, NULL, 1, NULL );
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
   int temp_index;
   const CSubStateItem *pGenerator;
   CSubStateItem tempState;
   int j, k;
   unsigned tag;
   unsigned index, last_tag;
 
   static CSubStateItem uniqueItems[AGENDA_SIZE];
   unsigned uniqueIndex;

   static CStringVector sentence;
   static CRule rules(m_weights->m_bSegmentationRules);
   rules.segment(sentence_input, &sentence);
   const unsigned length=sentence.size();

   static CSubStateItem goldState;
   goldState.clear();

   TRACE("Initialising the tagging process...");
   m_WordCache.clear(); 
   tempState.clear();
   m_Agenda.clear();
   m_Agenda.pushCandidate(&tempState);                   
   m_Agenda.nextRound();

   TRACE("Tagging started");
   //TRACE("initialisation time: " << clock() - start_time);
   for (index=0; index<length; index++) {

      static bool bAnyCorrect;
      bAnyCorrect = false;
      // 2. generate by replacing items
      pGenerator = m_Agenda.generatorStart();
      for (j=0; j<m_Agenda.generatorSize(); ++j) {
         if ( m_bTrain )
            if ( *pGenerator == goldState ) bAnyCorrect = true;
         if ( ( index > 0 ) && ( rules.canAppend(index) ) ) {
            tempState.copy(pGenerator);
            tempState.replace(index, CTag::NONE);
            m_Agenda.pushCandidate(&tempState);
         } // if
         if ( index == 0 ) {
            tempState.copy(pGenerator);
            tempState.append(index, CTag::NONE);
            m_Agenda.pushCandidate(&tempState);;
         }
         pGenerator = m_Agenda.generatorNext();  // next generator
      }
      if ( m_bTrain ) {
         pGenerator = m_Agenda.bestGenerator();
         if ( *pGenerator != goldState ) {
            TRACE("Training error before last word");
            for (temp_index=0; temp_index<pGenerator->size()-1; ++temp_index)
               getOrUpdateLocalScore(&sentence, pGenerator, temp_index, -1, m_nTrainingRound);
            for (temp_index=0; temp_index<goldState.size()-1; ++temp_index)
               getOrUpdateLocalScore(&sentence, &goldState, temp_index, 1, m_nTrainingRound);
            return;
         }
      }

   //_
   // 1. generate new items according to each previous item. 
   // iterate postags
      for (tag=CTag::FIRST; tag<CTag::COUNT; ++tag) {
         pGenerator = m_Agenda.generatorStart();
         uniqueIndex=0;
         for (j=0; j<m_Agenda.generatorSize(); ++j) {
            static unsigned start_index, end_index;
            if ( index > 0 ) {
               start_index = pGenerator->getWordStart(pGenerator->size()-1);
            }
            if ( index > 0 && rules.canSeparate( index ) && 
                 (  (  m_weights->m_mapWordFrequency.find( m_WordCache.find( start_index , index-1 , &sentence ) , 0 ) < 
                       m_weights->m_nMaxWordFrequency/5000+5 && 
                       PENN_TAG_CLOSED[ tag ] == false  ) ||
                    m_weights->m_mapTagDictionary.lookup( m_WordCache.find( start_index , index-1 , &sentence ), tag )
                 )
               ) {  
               tempState.copy(pGenerator);
               tempState.append(index, CTag::NONE);
               tempState.setTag(tempState.size()-2, tag);
               tempState.score += getOrUpdateLocalScore(&sentence, &tempState, tempState.size()-2);
               if (nBest==1) {
                  bool bDuplicate = false;
                  for (temp_index=0; temp_index<uniqueIndex; ++temp_index) {
                     if (uniqueItems[temp_index].size() > 2 && 
                         tempState.size() > 2 && 
                         uniqueItems[temp_index].getWordStart(uniqueItems[temp_index].size()-3) == tempState.getWordStart(tempState.size()-3) &&
                         uniqueItems[temp_index].getTag(uniqueItems[temp_index].size()-3) == tempState.getTag(tempState.size()-3) 
                        ) {
                          bDuplicate = true;
                          if (uniqueItems[temp_index].score < tempState.score) {
                             uniqueItems[temp_index].copy(&tempState);
                          }
                          break;
                     }
                  }
                  if (!bDuplicate) {
                     // the number of generators is surely fewer than list size
                     uniqueItems[uniqueIndex++].copy(&tempState);
                  }
               }
               else {
                  m_Agenda.pushCandidate(&tempState);
               }
            }
            pGenerator = m_Agenda.generatorNext();  // next generator
         }
         // push candidates
         if (nBest == 1) {
            for (temp_index=0; temp_index<uniqueIndex; ++temp_index) {
               m_Agenda.pushCandidate(&uniqueItems[temp_index]);
            }
         }
      }//tag

      m_Agenda.nextRound(); // move round
      if (m_bTrain) goldState.follow(m_goldState);
   }
   
   for (tag=CTag::FIRST; tag<CTag::COUNT; ++tag) {
      pGenerator = m_Agenda.generatorStart();
      uniqueIndex=0;
      for (j=0; j<m_Agenda.generatorSize(); ++j) {
         static unsigned start_index; start_index=pGenerator->getWordStart(pGenerator->size()-1);
        
         if ( (  (  m_weights->m_mapWordFrequency.find( m_WordCache.find( start_index , length-1 , &sentence ) , 0 ) < 
                    m_weights->m_nMaxWordFrequency/5000+5 && 
                    PENN_TAG_CLOSED[ tag ] == false  ) ||
                 m_weights->m_mapTagDictionary.lookup( m_WordCache.find( start_index , length-1 , &sentence ), tag )
              )
            ) {  
            tempState.copy(pGenerator);
            tempState.setTag(tempState.size()-1, tag);
            tempState.score += getOrUpdateLocalScore(&sentence, &tempState, tempState.size()-1);
            if (nBest==1) {
               bool bDuplicate = false;
               for (temp_index=0; temp_index<uniqueIndex; ++temp_index) {
                  if (uniqueItems[temp_index].size() > 1 && 
                      tempState.size() > 1 && 
                      uniqueItems[temp_index].getWordStart(uniqueItems[temp_index].size()-2) == tempState.getWordStart(tempState.size()-2) &&
                      uniqueItems[temp_index].getWordEnd(uniqueItems[temp_index].size()-2) == tempState.getWordEnd(tempState.size()-2) &&
                      uniqueItems[temp_index].getTag(uniqueItems[temp_index].size()-2) == tempState.getTag(tempState.size()-2) 
                     ) {
                       bDuplicate = true;
                       if (uniqueItems[temp_index].score < tempState.score) {
                          uniqueItems[temp_index].copy(&tempState);
                       }
                       break;
                     }
               }
               if (!bDuplicate) {
                  // the number of generators is surely fewer than list size
                  uniqueItems[uniqueIndex++].copy(&tempState);
               }
            }
            else {
               m_Agenda.pushCandidate(&tempState);
            }
         }
         pGenerator = m_Agenda.generatorNext();
      }
      if (nBest==1) {
	 for (temp_index=0; temp_index<uniqueIndex; ++temp_index) {
	    m_Agenda.pushCandidate(&uniqueItems[temp_index]);
	 }
      }
   }
   m_Agenda.nextRound();
   if ( m_bTrain && 1 ) {
      goldState.follow(m_goldState);
      pGenerator = m_Agenda.bestGenerator();
      if ( *pGenerator != goldState ) {
         TRACE("Training error at the last word");
         for (temp_index=0; temp_index<pGenerator->size(); ++temp_index)
            getOrUpdateLocalScore(&sentence, pGenerator, temp_index, -1, m_nTrainingRound);
         for (temp_index=0; temp_index<goldState.size(); ++temp_index)
            getOrUpdateLocalScore(&sentence, &goldState, temp_index, 1, m_nTrainingRound);
      }
      return;
   }
   TRACE("Outputing sentence");
   vReturn->clear();
   if (nBest == 1) {
      generate( m_Agenda.bestGenerator() , &sentence , this , vReturn ) ; 
      if (out_scores) out_scores[ 0 ] = m_Agenda.bestGenerator( )->score ;
   }
   else {
      m_Agenda.sortGenerators();
      for ( temp_index = 0 ; temp_index < nBest ; ++ temp_index ) {
         vReturn[ temp_index ].clear() ; 
         if (out_scores) out_scores[ temp_index ] = 0 ;
         if ( temp_index < m_Agenda.generatorSize() ) {
            generate( m_Agenda.generator( temp_index ) , &sentence , this , &(vReturn[ temp_index ]) ) ; 
            if (out_scores) out_scores[ temp_index ] = m_Agenda.bestGenerator( )->score ;
         }
      }
   }
   TRACE("Done, the highest score is: " << m_Agenda.bestGenerator()->score) ;
   TRACE("The total time spent: " << double(clock() - total_start_time)/CLOCKS_PER_SEC) ;
}




