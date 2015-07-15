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

static const CWord g_emptyWord("");
static const CTag g_beginTag(CTag::SENTENCE_BEGIN);

#define find_or_replace_word_cache(tmp_start, tmp_end) ( amount ? m_WordCache.replace(tmp_start, tmp_end, sentence) : m_WordCache.find(tmp_start, tmp_end, sentence) )
#define refer_or_allocate(x, y, z) {if(amount==0) x.refer(&y, &z); else x.allocate(y, z);}

inline int encodeCharSegmentation(const bool &b, const bool &e) {
   return ( b ? 2 : 0 ) + ( e ? 1 : 0 ) ; // 00 01 10 11
}

inline int encodeCharInfoAndPosition(const int &char_info, const int &pos) {
   assert( pos >= -3 && pos <= 3 ); // pos+3 0->6
   return (char_info<<3) | (pos+3);
}

/*===============================================================
 *
 * CTagger - handles the features for the tagger
 *
 *==============================================================*/

/*---------------------------------------------------------------
 *
 * getOrUpdateSeparateScore - get or update the local score for a word in sentence
 *
 * When bigram is needed from the beginning of sentence, the
 * -BEGIN- tag and the empty word are used.
 *
 * This implies that empty words should not be used in other
 * situations.
 *
 *--------------------------------------------------------------*/

SCORE_TYPE CTagger::getOrUpdateSeparateScore( const CStringVector *sentence, const CSubStateItem *item, unsigned long index, SCORE_TYPE amount, unsigned long round ) {
   static SCORE_TYPE nReturn ;
   static unsigned long start_0;
   static unsigned long start_1, end_1, length_1;
   static unsigned long start_2, end_2, length_2;

   // about the words
   assert(amount!=0||index==item->size()-1||index==item->size());
   start_0 = index==item->size() ? 0 : item->getWordStart( index ) ;

   start_1 = index > 0 ? item->getWordStart( index-1 ) : 0 ;
   end_1 = index > 0 ? item->getWordEnd( index-1 ) : 0 ;
   assert(index==item->size()||index==0 || end_1 == start_0-1);
   length_1 = index > 0 ? item->getWordLength( index-1 ) : 0;

   start_2 = index > 1 ? item->getWordStart( index-2 ) : 0 ;
   end_2 = index > 1 ? item->getWordEnd( index-2 ) : 0 ;
   assert(index<2 || end_2 == start_1-1);
   length_2 = index > 1 ? item->getWordLength( index-2 ) : 0;

   const CWord &word_1 = index>0 ? find_or_replace_word_cache( start_1, end_1 ) : g_emptyWord;
   const CWord &word_2 = index>1 ? find_or_replace_word_cache( start_2, end_2 ) : g_emptyWord;

   // about the length
   if (length_1>LENGTH_MAX) length_1 = LENGTH_MAX;
   if (length_2>LENGTH_MAX) length_2 = LENGTH_MAX;

   // about the chars
   const CWord &first_char_0 = index<item->size() ? find_or_replace_word_cache( start_0, start_0 ) : g_emptyWord ;
   const CWord &first_char_1 = index>0 ? find_or_replace_word_cache( start_1, start_1 ) : g_emptyWord;

   const CWord &last_char_1 = index>0 ? find_or_replace_word_cache( end_1, end_1 ) : g_emptyWord;
   const CWord &last_char_2 = index>1 ? find_or_replace_word_cache( end_2, end_2 ) : g_emptyWord;
   const CWord &two_char = index>0&&index<item->size() ? find_or_replace_word_cache( end_1, start_0 ) : g_emptyWord ;
   const CWord &word_1_first_char_0 = index>0&&index<item->size() ? find_or_replace_word_cache( start_1, start_0 ) : g_emptyWord;
   const CWord &word_1_last_char_2 = index>1 ? find_or_replace_word_cache( end_2, end_1 ) : g_emptyWord;
   const CWord &three_char = ( length_1==1 && index>1 && index<item->size() ) ? find_or_replace_word_cache( end_2, start_0 ) : g_emptyWord;

   static CTwoWords word_2_word_1, first_char_1_last_char_1, first_char_0_first_char_1, last_char_1_last_char_2 ;
   if (amount==0&&index>0) {
      word_2_word_1.refer( &word_1 , &word_2 ) ;
      first_char_1_last_char_1.refer( &first_char_1 , &last_char_1 ) ;
      first_char_0_first_char_1.refer( &first_char_0 , &first_char_1 ) ;
      last_char_1_last_char_2.refer( &last_char_1 , &last_char_2 ) ;
   }
   else {
      word_2_word_1.allocate( word_1, word_2 ) ;
      first_char_1_last_char_1.allocate( first_char_1, last_char_1 ) ;
      first_char_0_first_char_1.allocate( first_char_0, first_char_1 ) ;
      last_char_1_last_char_2.allocate( last_char_1, last_char_2 ) ;
   }

   // about the tags
   const CTag &tag_0 = index<item->size() ? item->getTag( index ) : g_beginTag;
   const CTag &tag_1 = index>0 ? item->getTag(index-1) : g_beginTag;
   const CTag &tag_2 = index>1 ? item->getTag(index-2) : g_beginTag;

   static CTaggedWord<CTag, TAG_SEPARATOR> wt1, wt2;
   static CTwoTaggedWords wt12;

   unsigned long long first_char_cat_0 = m_weights->m_mapCharTagDictionary.lookup(first_char_0) | (static_cast<unsigned long long>(1)<<tag_0.code()) ;
   unsigned long long last_char_cat_1 = m_weights->m_mapCharTagDictionary.lookup(last_char_1) | (static_cast<unsigned long long>(1)<<tag_1.code()) ;

   static CTagSet<CTag, 2> tag_0_tag_1, tag_0_tag_2, tag_1_tag_2;
   static CTagSet<CTag, 3> tag_0_tag_1_tag_2;
   tag_0_tag_1.load( encodeTags(tag_0, tag_1) );
   tag_0_tag_2.load( encodeTags(tag_0, tag_2) );
   tag_1_tag_2.load( encodeTags(tag_1, tag_2) );
   tag_0_tag_1_tag_2.load( encodeTags(tag_0, tag_1, tag_2) );

   static int j ;

   // adding scores with features for last word
   if (index>0) {
      nReturn = m_weights->m_mapSeenWords.getOrUpdateScore( word_1 , m_nScoreIndex , amount , round ) ;
      if (index>1) nReturn += m_weights->m_mapLastWordByWord.getOrUpdateScore( word_2_word_1 , m_nScoreIndex , amount , round ) ;

      if ( length_1 == 1 ) {
         nReturn += m_weights->m_mapOneCharWord.getOrUpdateScore( word_1 , m_nScoreIndex , amount , round ) ;
      }
      else {
         nReturn += m_weights->m_mapFirstAndLastChars.getOrUpdateScore( first_char_1_last_char_1 , m_nScoreIndex , amount , round ) ;

         nReturn += m_weights->m_mapLengthByFirstChar.getOrUpdateScore( std::make_pair(first_char_1, length_1) , m_nScoreIndex , amount , round ) ;
         nReturn += m_weights->m_mapLengthByLastChar.getOrUpdateScore( std::make_pair(last_char_1, length_1) , m_nScoreIndex , amount , round ) ;

//         nReturn += m_weights->m_mapLengthByTagAndFirstChar.getOrUpdateScore( std::make_pair(first_char_1, (length_1<<CTag::SIZE)|tag_1.code()) , m_nScoreIndex , amount , round ) ;
//         nReturn += m_weights->m_mapLengthByTagAndLastChar.getOrUpdateScore( std::make_pair(last_char_1, (length_1<<CTag::SIZE)|tag_1.code()) , m_nScoreIndex , amount , round ) ;
      }

      if (index>1) {
         nReturn += m_weights->m_mapCurrentWordLastChar.getOrUpdateScore( word_1_last_char_2 , m_nScoreIndex , amount , round ) ;
         nReturn += m_weights->m_mapLastWordByLastChar.getOrUpdateScore( last_char_1_last_char_2 , m_nScoreIndex , amount , round ) ;

         nReturn += m_weights->m_mapLengthByLastWord.getOrUpdateScore( std::make_pair(word_2, length_1) , m_nScoreIndex , amount , round ) ;
         nReturn += m_weights->m_mapLastLengthByWord.getOrUpdateScore( std::make_pair(word_1, length_2), m_nScoreIndex , amount , round ) ;
      }

      nReturn += m_weights->m_mapCurrentTag.getOrUpdateScore( std::make_pair(word_1, tag_1) , m_nScoreIndex , amount , round ) ;

      if ( length_1 <= 2 ) nReturn += m_weights->m_mapLastTagByWord.getOrUpdateScore( std::make_pair(word_1, tag_2) , m_nScoreIndex , amount , round ) ;

      if (index>1) {
         if ( length_1 <= 2 ) nReturn += m_weights->m_mapTagByWordAndPrevChar.getOrUpdateScore( std::make_pair(word_1_last_char_2, tag_1) , m_nScoreIndex , amount , round ) ;
         if ( length_1 == 1 && index<item->size() ) nReturn += m_weights->m_mapTagOfOneCharWord.getOrUpdateScore( std::make_pair(three_char, tag_1) , m_nScoreIndex , amount , round ) ;
      }

      nReturn += m_weights->m_mapTagByLastChar.getOrUpdateScore( std::make_pair(last_char_1, tag_1) , m_nScoreIndex , amount , round ) ;
      nReturn += m_weights->m_mapTagByLastCharCat.getOrUpdateScore( std::make_pair(last_char_cat_1, tag_1) , m_nScoreIndex , amount , round ) ;

      for (j=0; j<length_1-1; ++j) {
         wt1.load(find_or_replace_word_cache(start_1+j, start_1+j), tag_1);
         wt2.load(last_char_1);//
         if (amount==0) { wt12.refer(&wt1, &wt2); } else { wt12.allocate(wt1, wt2); }
         nReturn += m_weights->m_mapTaggedCharByLastChar.getOrUpdateScore(wt12, m_nScoreIndex, amount, round) ;
      }
   }

   // all about the current word
   nReturn += m_weights->m_mapLastTagByTag.getOrUpdateScore( tag_0_tag_1, m_nScoreIndex , amount , round ) ;
   if (index>0) nReturn += m_weights->m_mapTag0Tag1Size1.getOrUpdateScore( std::make_pair( tag_0_tag_1, length_1 ), m_nScoreIndex , amount , round ) ;
   if (index>0) nReturn += m_weights->m_mapTag1Tag2Size1.getOrUpdateScore( std::make_pair( tag_1_tag_2, length_1 ), m_nScoreIndex , amount , round ) ;
   if (index>0) nReturn += m_weights->m_mapTag0Tag1Tag2Size1.getOrUpdateScore( std::make_pair( tag_0_tag_1_tag_2, length_1 ), m_nScoreIndex , amount , round ) ;

   if ( length_1 <= 2 ) nReturn += m_weights->m_mapTagByLastWord.getOrUpdateScore( std::make_pair(word_1, tag_0) , m_nScoreIndex , amount , round ) ;

   if ( index > 0 ) {
      nReturn += m_weights->m_mapLastTwoTagsByTag.getOrUpdateScore( tag_0_tag_1_tag_2, m_nScoreIndex , amount , round ) ;
   }

if (index<item->size()) {
   if ( index>0 ) {
      nReturn += m_weights->m_mapSeparateChars.getOrUpdateScore( two_char , m_nScoreIndex , amount , round ) ;

      nReturn += m_weights->m_mapLastWordFirstChar.getOrUpdateScore( word_1_first_char_0 , m_nScoreIndex , amount , round ) ;

      nReturn += m_weights->m_mapFirstCharLastWordByWord.getOrUpdateScore( first_char_0_first_char_1 , m_nScoreIndex , amount , round ) ;

      if ( length_1 <= 2 ) nReturn += m_weights->m_mapTagByWordAndNextChar.getOrUpdateScore( std::make_pair(word_1_first_char_0, tag_1) , m_nScoreIndex , amount , round ) ;

//      nReturn += m_weights->m_mapSepCharAndNextChar.getOrUpdateScore( find_or_replace_word_cache(start_0, start_0==sentence->size()-1?start_0:start_0+1) , m_nScoreIndex , amount , round ) ;

   }

   nReturn += m_weights->m_mapTagByFirstChar.getOrUpdateScore( std::make_pair(first_char_0, tag_0) , m_nScoreIndex , amount , round ) ;
   nReturn += m_weights->m_mapTagByFirstCharCat.getOrUpdateScore( std::make_pair(first_char_cat_0, tag_0) , m_nScoreIndex , amount , round ) ;

   nReturn += m_weights->m_mapFirstCharBy2Tags.getOrUpdateScore( std::make_pair(first_char_0, tag_0_tag_1) , m_nScoreIndex , amount , round ) ;
   if (index>0)nReturn += m_weights->m_mapFirstCharBy3Tags.getOrUpdateScore( std::make_pair(first_char_0, tag_0_tag_1_tag_2) , m_nScoreIndex , amount , round ) ;

   nReturn += m_weights->m_mapTagByChar.getOrUpdateScore( std::make_pair(first_char_0, tag_0), m_nScoreIndex , amount , round ) ;

   if (index>0) {
      wt1.load(last_char_1, tag_1);
      wt2.load(first_char_0, tag_0);
      if (amount==0) { wt12.refer(&wt1, &wt2); } else { wt12.allocate(wt1, wt2); }
      nReturn += m_weights->m_mapTaggedSeparateChars.getOrUpdateScore( wt12, m_nScoreIndex , amount , round ) ;

   }

   if (index>0) nReturn += m_weights->m_mapTagWordTag.getOrUpdateScore( std::make_pair( word_1, tag_0_tag_2 ), m_nScoreIndex, amount, round);
   if (index>1) nReturn += m_weights->m_mapWordTagTag.getOrUpdateScore( std::make_pair( word_2, tag_0_tag_1 ), m_nScoreIndex, amount, round);
}

   // ===================================================================================
   // character scores -- with end_1-1 middled
//   static int char_info;
//   char_info = encodeCharSegmentation(start_1==end_1, true);
//   if (index>0) {
//      for (j = std::max(0, static_cast<int>(end_1)-1); j < std::min(static_cast<unsigned long>(sentence->size()), end_1+2); ++j) {
//         nReturn += m_weights->m_mapCharUnigram.getOrUpdateScore( std::make_pair( find_or_replace_word_cache(j, j), encodeCharInfoAndPosition(char_info, j-end_1) ), m_nScoreIndex, amount, round);
//         if (hasCharTypeKnowledge()) nReturn += m_weights->m_mapCharCatUnigram.getOrUpdateScore( std::make_pair( groupCharTypes(segmentor, sentence, j, 1, amount), encodeCharInfoAndPosition(char_info, j-end_1) ), m_nScoreIndex, amount, round);
//      }

//      for (j = std::max(0, static_cast<int>(end_1)-1); j < std::min(static_cast<unsigned long>(sentence->size())-1, end_1+1); ++j) {
//         nReturn += m_weights->m_mapCharBigram.getOrUpdateScore( std::make_pair( find_or_replace_word_cache(j, j+1), encodeCharInfoAndPosition(char_info, j-end_1) ), m_nScoreIndex, amount, round);
//         if (hasCharTypeKnowledge()) nReturn += m_weights->m_mapCharCatBigram.getOrUpdateScore( std::make_pair( groupCharTypes(segmentor, sentence, j, 2, amount), encodeCharInfoAndPosition(char_info, j-end_1) ), m_nScoreIndex, amount, round);
//      }

//      for (j = std::max(0, static_cast<int>(end_1)-1); j < std::min(static_cast<unsigned long>(sentence->size())-2, end_1); ++j) {
//         nReturn += m_weights->m_mapCharTrigram.getOrUpdateScore( std::make_pair( find_or_replace_word_cache(j, j+2), encodeCharInfoAndPosition(char_info, j-end_1) ), m_nScoreIndex, amount, round);
//         if (hasCharTypeKnowledge()) nReturn += m_weights->m_mapCharCatTrigram.getOrUpdateScore( std::make_pair( groupCharTypes(segmentor, sentence, j, 3, amount), encodeCharInfoAndPosition(char_info, j-end_1) ), m_nScoreIndex, amount, round);
//      }
//   }

   return nReturn;
}

/*---------------------------------------------------------------
 *
 * getOrUpdateAppendScore - get or update the local score for a word in sentence
 *
 * When bigram is needed from the beginning of sentence, the
 * -BEGIN- tag and the empty word are used.
 *
 * This implies that empty words should not be used in other
 * situations.
 *
 *--------------------------------------------------------------*/

SCORE_TYPE CTagger::getOrUpdateAppendScore( const CStringVector *sentence, const CSubStateItem *item, unsigned long index, unsigned long char_index, SCORE_TYPE amount, unsigned long round ) {
   static SCORE_TYPE nReturn ;
   assert(char_index>0);

   static unsigned long start, length;
   static unsigned long prev_char_index;
   start = item->getWordStart( index ) ;
   length = item->getWordLength( index ) ;
   prev_char_index = char_index-1;
   const CTag &tag_0 = item->getTag(index);

   const CTag &tag_1 = index>0? item->getTag(index-1): g_beginTag;
   const CTag &tag_2 = index>1? item->getTag(index-2): g_beginTag;
   static CTagSet<CTag, 2> tag_0_tag_1;
   static CTagSet<CTag, 2> tag_1_tag_2;
   static CTagSet<CTag, 3> tag_0_tag_1_tag_2;
   tag_1_tag_2.load( encodeTags(tag_1, tag_2) );
   tag_0_tag_1.load( encodeTags(tag_0, tag_1) );
   tag_0_tag_1_tag_2.load( encodeTags(tag_0, tag_1, tag_2) );

   const CWord &char_unigram = find_or_replace_word_cache( char_index, char_index );
   const CWord &char_bigram = find_or_replace_word_cache( char_index-1, char_index );

   const CWord &first_char = find_or_replace_word_cache( start, start );
   const CWord &prev_char = find_or_replace_word_cache( char_index-1, char_index-1 );

   static CTaggedWord<CTag, TAG_SEPARATOR> wt1, wt2;
   static CTwoTaggedWords wt12;
   static CTwoWords first_char_and_char;

   static unsigned i;

   // adding scores with features
   nReturn = 0;

//   nReturn += m_weights->m_mapLastTagByTag.getOrUpdateScore( tag_0_tag_1, m_nScoreIndex , amount , round ) ;
//   if(index>0)nReturn += m_weights->m_mapLastTwoTagsByTag.getOrUpdateScore( tag_0_tag_1_tag_2, m_nScoreIndex, amount, round);

   nReturn += m_weights->m_mapTagByChar.getOrUpdateScore( std::make_pair(char_unigram, tag_0), m_nScoreIndex , amount , round ) ;

   wt1.load(char_unigram, tag_0);
   wt2.load(first_char);
   refer_or_allocate(wt12, wt1, wt2);
   nReturn += m_weights->m_mapTaggedCharByFirstChar.getOrUpdateScore( wt12, m_nScoreIndex, amount, round ) ;

//   if (char_unigram == prev_char)
//      nReturn += m_weights->m_mapRepeatedCharByTag.getOrUpdateScore( std::make_pair(char_unigram, tag_0), m_nScoreIndex, amount, round) ;

   nReturn += m_weights->m_mapConsecutiveChars.getOrUpdateScore( char_bigram, m_nScoreIndex, amount, round ) ;

//   nReturn += m_weights->m_mapAppCharAndNextChar.getOrUpdateScore( find_or_replace_word_cache( char_index, char_index==sentence->size()-1?char_index:char_index+1 ), m_nScoreIndex, amount, round );

   nReturn += m_weights->m_mapTaggedConsecutiveChars.getOrUpdateScore( std::make_pair(char_bigram, tag_0), m_nScoreIndex, amount, round ) ;

//   refer_or_allocate(first_char_and_char, first_char, char_unigram);
//  nReturn += m_weights->m_mapFirstCharAndChar.getOrUpdateScore( first_char_and_char, m_nScoreIndex , amount , round ) ;

   //nReturn += m_weights->m_mapPartialWord.getOrUpdateScore( find_or_replace_word_cache( start, char_index ), m_nScoreIndex, amount, round );

   // character scores -- the middle character is char_index-1
//   static int char_info;
//   char_info = encodeCharInfoAndPosition(start==prev_char_index, false);
//   for (i = std::max(0, static_cast<int>(prev_char_index)-1); i < std::min(static_cast<unsigned long>(sentence->size()), prev_char_index+2); ++i) {
//      nReturn += m_weights->m_mapCharUnigram.getOrUpdateScore( std::make_pair( find_or_replace_word_cache(i, i), encodeCharInfoAndPosition(char_info, i-prev_char_index) ), m_nScoreIndex, amount, round);
//      if (hasCharTypeKnowledge()) nReturn += m_weights->m_mapCharCatUnigram.getOrUpdateScore( std::make_pair( groupCharTypes(segmentor, sentence, i, 1, amount), encodeCharInfoAndPosition(char_info, i-prev_char_index) ), m_nScoreIndex, amount, round);
//   }

//   for (i = std::max(0, static_cast<int>(prev_char_index)-1); i < std::min(static_cast<unsigned long>(sentence->size())-1, prev_char_index+1); ++i) {
//      nReturn += m_weights->m_mapCharBigram.getOrUpdateScore( std::make_pair( find_or_replace_word_cache(i, i+1), encodeCharInfoAndPosition(char_info, i-prev_char_index) ), m_nScoreIndex, amount, round);
//      if (hasCharTypeKnowledge()) nReturn += m_weights->m_mapCharCatBigram.getOrUpdateScore( std::make_pair( groupCharTypes(segmentor, sentence, i, 2, amount), encodeCharInfoAndPosition(char_info, i-prev_char_index) ), m_nScoreIndex, amount, round);
//   }

//   for (i = std::max(0, static_cast<int>(prev_char_index)-1); i < std::min(static_cast<unsigned long>(sentence->size())-2, prev_char_index); ++i) {
//      nReturn += m_weights->m_mapCharTrigram.getOrUpdateScore( std::make_pair( find_or_replace_word_cache(i, i+2), encodeCharInfoAndPosition(char_info, i-prev_char_index) ), m_nScoreIndex, amount, round);
//      if (hasCharTypeKnowledge()) nReturn += m_weights->m_mapCharCatTrigram.getOrUpdateScore( std::make_pair( groupCharTypes(segmentor, sentence, i, 3, amount), encodeCharInfoAndPosition(char_info, i-prev_char_index) ), m_nScoreIndex, amount, round);
//   }

   return nReturn;
}

/*---------------------------------------------------------------
 *
 * buildStateItem - builds item
 *
 * Inputs: the raw sentence in input format
 *         the tagged sentence in output format
 *         the state item which is nReturn
 *
 *--------------------------------------------------------------*/

inline void buildStateItem(const CStringVector *raw, const CTwoStringVector *tagged, CSubStateItem *item) {
   static int i, ri, rawlen, taggedlen;
   item->clear();
   // add each output word
   rawlen = 0; ri = 0; // raw index
//   taggedlen = 0;
   for (i=0; i<tagged->size(); ++i) {
//      taggedlen += tagged->at(i).first.size();
//      while ( rawlen < taggedlen ) {
//         rawlen += raw->at(ri).size();
//         ++ri;
//      }
      ri += getUTF8StringLength(tagged->at(i).first);
      item->append(ri-1, CTag(tagged->at(i).second).code());
   }
}

/*---------------------------------------------------------------
 *
 * generate - helper function that generates tagged output
 *
 *--------------------------------------------------------------*/

void generate(const CSubStateItem *stateItem, const CStringVector *sentence, CTagger *tagger, CTwoStringVector *vReturn) {
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

bool CTagger::train( const CStringVector * sentence_input , const CTwoStringVector * correct) {
   ++m_nTrainingRound ;
   static CStringVector sentence;
   m_weights->m_rules.record( correct, &sentence );
   buildStateItem( &sentence, correct, &m_goldState);
//   for (int i=0; i<sentence.size(); ++i)
//      std::cout << m_weights->m_rules.canSeparate(i) << std::endl;

   static unsigned total_size, local_size;
   total_size=0;
   // Updates that are common for all example
   for ( unsigned i=0; i<correct->size(); ++i ) {

      const CWord &word = correct->at(i).first ;
      unsigned long tag = CTag( correct->at(i).second ).code() ;

      static CStringVector chars;
      static unsigned j;
      chars.clear();
      getCharactersFromUTF8String(correct->at(i).first, &chars);
      local_size = chars.size();

      m_weights->m_mapWordFrequency[word]++;
      if (m_weights->m_mapWordFrequency[word]>m_weights->m_nMaxWordFrequency)
         m_weights->m_nMaxWordFrequency = m_weights->m_mapWordFrequency[word];

      m_weights->m_mapTagDictionary.add(word, tag);
      for ( j=0 ; j<local_size; ++j ) {
         m_weights->m_mapCharTagDictionary.add(chars[j], tag) ;
      }

      if ( PENN_TAG_CLOSED[tag] || tag==PENN_TAG_CD ) {
           m_weights->m_mapCanStart.add(chars[0], tag);
      }

//      if ( !m_weights->m_Knowledge ||
//          (!m_weights->m_Knowledge->isFWorCD(chars[0])&&
//           !m_weights->m_Knowledge->isFWorCD(chars[chars.size()-1])))
      bool bNoSep=false;
      for ( j=total_size+1; j<total_size+local_size; ++j)
         if (!m_weights->m_rules.canSeparate(j)) bNoSep = true;
      if (!bNoSep)
         m_weights->setMaxLengthByTag( tag , local_size ) ;

      total_size += chars.size();
   }
   work( &sentence, NULL, NULL, 1, NULL );
   return m_bTrainingError;
}

/*---------------------------------------------------------------
 *
 * tag - assign POS tags to a sentence
 *
 * Returns: makes a new instance of CTwoStringVector
 *
 *--------------------------------------------------------------*/

void CTagger::tag( const CStringVector * sentence_input , CTwoStringVector * vReturn , SCORE_TYPE * out_scores , unsigned long nBest , const CBitArray * prunes ) {
   static CStringVector sentence;
   m_weights->m_rules.segment(sentence_input, &sentence);
   work( &sentence, vReturn, out_scores, nBest, prunes ) ;
}

/*---------------------------------------------------------------
 *
 * work - shared between train and tag for running
 *
 *--------------------------------------------------------------*/

void CTagger::work( const CStringVector * sentence , CTwoStringVector * vReturn , SCORE_TYPE * out_scores , unsigned long nBest , const CBitArray * prunes ) {
   clock_t total_start_time = clock();;
   int temp_index;
   const CSubStateItem *pGenerator;
   CSubStateItem tempState;
   static int j;
   static unsigned tag;
   static unsigned index, last_tag;
   static unsigned last_nosep;

   static CSubStateItem uniqueItems[AGENDA_SIZE];
   unsigned long uniqueIndex;
   static bool bUnique;

   const unsigned length=sentence->size();

   static CSubStateItem goldState;
   goldState.clear();

   TRACE("Initialising the tagging process...");
   m_WordCache.clear();
   tempState.clear();
   m_Agenda.clear();
   m_Agenda.pushCandidate(&tempState);
   m_Agenda.nextRound();
   last_nosep = MAX_SENTENCE_SIZE;

   TRACE("Tagging started");
   //TRACE("initialisation time: " << clock() - start_time);
   for (index=0; index<length; index++) {

      if ( !m_weights->m_rules.canSeparate( index ) )
         last_nosep = index;

      // decide correction
      if ( m_bTrain ) {
         static bool bAnyCorrect;
         bAnyCorrect = false;
         pGenerator = m_Agenda.generatorStart();
         for (j=0; j<m_Agenda.generatorSize(); ++j) {
// DEBUG: stateitems
//            static CTwoStringVector stm;
//            stm.clear();
//            generate(pGenerator, sentence, this, &stm);
//            static CSentenceWriter writer("");
//            writer.writeSentence(&stm);
            if ( *pGenerator == goldState ) bAnyCorrect = true;
            pGenerator = m_Agenda.generatorNext();  // next generator
         }
//         std::cout <<std::endl;
         if ( !bAnyCorrect ) {
            TRACE("Training error at character " << index);
            pGenerator = m_Agenda.bestGenerator();
// DEBUG: gold
//            CTwoStringVector tmp;
//            tmp.clear();
//            generate( pGenerator , sentence , this , &tmp ) ;
//            CSentenceWriter writer("");
//            writer.writeSentence(&tmp);
//            tmp.clear();
//            generate( &goldState , sentence , this , &tmp ) ;
//            writer.writeSentence(&tmp);
            updateScoreForState(sentence, pGenerator, -1);
            updateScoreForState(sentence, &goldState, 1);
            m_bTrainingError = true;
            return;
         }
      }

      // 2. generate by replacing items
      if ( index > 0 ) {
         pGenerator = m_Agenda.generatorStart();
         for (j=0; j<m_Agenda.generatorSize(); ++j) {
            assert(pGenerator->size()>0);
            if ( ( m_weights->m_rules.canAppend(index) ) && // ( index > 0 ) &&
                 ( pGenerator->getWordStart(pGenerator->size()-1)<last_nosep || pGenerator->getTag(pGenerator->size()-1).code() == PENN_TAG_CD || pGenerator->getWordLength(pGenerator->size()-1) < m_weights->m_maxLengthByTag[pGenerator->getTag(pGenerator->size()-1).code()] )
               ) {
               tempState.copy(pGenerator);
               tempState.replaceIndex(index);
               tempState.score += getOrUpdateAppendScore(sentence, &tempState, tempState.size()-1, index);
               if (index+1==length) tempState.score += getOrUpdateSeparateScore(sentence, &tempState, tempState.size());
               m_Agenda.pushCandidate(&tempState);
            } // if
            pGenerator = m_Agenda.generatorNext();  // next generator
         }
      }

   //_
   // 1. generate new items according to each previous item.
   // iterate postags
      for (tag=CTag::FIRST; tag<CTag::COUNT; ++tag) {

         pGenerator = m_Agenda.generatorStart();
         uniqueIndex=0;

         for (j=0; j<m_Agenda.generatorSize(); ++j) {

            last_tag = pGenerator->size()==0 ? CTag::SENTENCE_BEGIN : pGenerator->getTag(pGenerator->size()-1).code();

            if ( m_weights->m_rules.canSeparate( index ) &&
                (index == 0 || canAssignTag( m_WordCache.find( pGenerator->getWordStart(pGenerator->size()-1), index-1, sentence ), last_tag )) && // last word
                 canStartWord(*sentence, tag, index) // word
               ) {

               tempState.copy(pGenerator);
               tempState.append(index, tag);
               tempState.score += getOrUpdateSeparateScore(sentence, &tempState, tempState.size()-1);
               if (index+1==length) tempState.score += getOrUpdateSeparateScore(sentence, &tempState, tempState.size());

               if (nBest==1) {
                  bUnique = true;
                  for (temp_index=0; temp_index<uniqueIndex; ++temp_index) {
                     // only one new when index=zero.
                     assert(index>0&&uniqueItems[temp_index].size()>1);
                     if (uniqueItems[temp_index].getTag(uniqueItems[temp_index].size()-2) == tempState.getTag(tempState.size()-2) &&
                         uniqueItems[temp_index].getWordStart(uniqueItems[temp_index].size()-2) == tempState.getWordStart(tempState.size()-2)
                        ) {
                        bUnique = false;
                        if (uniqueItems[temp_index].score < tempState.score )
                           uniqueItems[temp_index].copy(&tempState);
                     }//if
                  }//for
                  if (bUnique) {
                     uniqueItems[uniqueIndex++].copy(&tempState);
                  }//if
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
              m_Agenda.pushCandidate(&(uniqueItems[temp_index]));
           }//for
         }
      }//tag

      m_Agenda.nextRound(); // move round
      if (m_bTrain) goldState.follow(m_goldState);
   }

   if ( m_bTrain && 1 ) {
      pGenerator = m_Agenda.bestGenerator();
      if ( *pGenerator != goldState ) {
         TRACE("Training error at the last word");
         updateScoreForState(sentence, pGenerator, -1);
         updateScoreForState(sentence, &goldState, 1);
         m_bTrainingError = true;
      }
      else {
         m_bTrainingError = false;
      }
      return;
   }
   TRACE("Outputing sentence");
   vReturn->clear();
   if (nBest == 1) {
      generate( m_Agenda.bestGenerator() , sentence , this , vReturn ) ;
      if (out_scores) out_scores[ 0 ] = m_Agenda.bestGenerator( )->score ;
   }
   else {
      m_Agenda.sortGenerators();
      for ( temp_index = 0 ; temp_index < nBest ; ++ temp_index ) {
         vReturn[ temp_index ].clear() ;
         if (out_scores) out_scores[ temp_index ] = 0 ;
         if ( temp_index < m_Agenda.generatorSize() ) {
            generate( m_Agenda.generator( temp_index ) , sentence , this , &(vReturn[ temp_index ]) ) ;
            if (out_scores) out_scores[ temp_index ] = m_Agenda.bestGenerator( )->score ;
         }
      }
   }
   TRACE("Done, the highest score is: " << m_Agenda.bestGenerator()->score) ;
   TRACE("The total time spent: " << double(clock() - total_start_time)/CLOCKS_PER_SEC) ;
}




