// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                              *
 * agendaplus.cpp - the chinese segmentor                       *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2006.10                        *
 *                                                              *
 ****************************************************************/

#include "segmentor.h"

using namespace chinese;
using namespace chinese::segmentor;

static CWord g_emptyWord("");
#define _cache_word(x,y) (amount == 0 ? segmentor->findWordFromCache(x, y, sentence) : segmentor->replaceWordToCache(x, y, sentence))

/*===============================================================
 *
 * CFeatureHandle - handles the features for the segmentor
 *
 *==============================================================*/

/*---------------------------------------------------------------
 *
 * getGlobalScore - get the global score for sentence
 *
 *--------------------------------------------------------------*/

SCORE_TYPE CFeatureHandle::getGlobalScore(const CStringVector* sentence, const CStateItem* item){
   cerr << "Not implemented" << endl;
   assert(0==1);
}

/*---------------------------------------------------------------
 *
 * getLocalScore - get the local score for a word in sentence
 *
 * When bigram is needed from the beginning of sentence, the empty word are used. 
 *
 * This implies that empty words should not be used in other 
 * situations. 
 *
 *--------------------------------------------------------------*/

SCORE_TYPE CFeatureHandle::getLocalScore(const CStringVector* sentence, const CStateItem* item, int index){
   cerr << "Not implemented" << endl;
   assert(0==1);
}

//===============================================================

unsigned long groupCharTypes(CSegmentor *segmentor, const CStringVector *sentence, const unsigned &index, const unsigned &count, const SCORE_TYPE &amount) {
   unsigned ret=0;
   for (unsigned i=0; i<count; ++i) {
      if (amount==0)
         ret = (ret<<CHAR_TYPE_SIZE) | segmentor->charType(segmentor->findWordFromCache(index+i, 1, sentence));
      else
         ret = (ret<<CHAR_TYPE_SIZE) | segmentor->charType(segmentor->replaceWordToCache(index+i, 1, sentence));
   }
   return ret;
}

/*---------------------------------------------------------------
 *
 * getOrUpdateSeparateScore - get the local score when a word is done
 *
 * The last word of the input state item is a complete word in
 * the sentence. 
 *
 *--------------------------------------------------------------*/

SCORE_TYPE getOrUpdateSeparateScore(CSegmentor *segmentor, const CStringVector* sentence, const CStateItem* item, int index, SCORE_TYPE amount=0, int round=0){

   static SCORE_TYPE nReturn; 
   // about score
   static int which_score;
   which_score = segmentor->isTraining() ? CScore<SCORE_TYPE>::eNonAverage : CScore<SCORE_TYPE>::eAverage ;
   // abbreviation weight
   CWeight &weight = segmentor->getWeights();

   // temporary variables
   static int tmp_i;
   static int length, last_length, word_length;
   static int start, end, last_start, last_end;
   start = item->getWordStart(index);
   end = item->getWordEnd(index);
   length = item->getWordLength(index); 
   last_start = index>0 ? item->getWordStart(index-1) : 0; // make sure that this is only used when index>0
   last_end = index>0 ? item->getWordEnd(index-1) : 0; // make sure that this is only used when index>0
   last_length = index>0 ? item->getWordLength(index-1) : 0;  // similar to the above
   word_length = length ; 

   // about the words
   const CWord &word=_cache_word(start, length);
   const CWord &last_word = index>0 ? _cache_word(last_start, last_length) : g_emptyWord; // use empty word for sentence beginners. 
   static CTwoWords two_word;
   if (amount==0)
      two_word.refer(&word, &last_word);
   else
      two_word.allocate(word, last_word);

   // about the chars
   static bool bWordStart; // whether the last char starts new word
   static int char_info; // start, middle, end or standalone
   bWordStart = (start==end);
   char_info = encodeCharSegmentation(bWordStart, true);
   const CWord &first_char=_cache_word(start, 1);
   const CWord &last_char=_cache_word(end, 1);
   const CWord &first_char_last_word = index>0 ? _cache_word(last_start, 1) : g_emptyWord;
   const CWord &last_char_last_word = index>0 ? _cache_word(last_end, 1) : g_emptyWord;
   const CWord &first_char_next_word = end<sentence->size()-1 ? _cache_word(end+1, 1) : g_emptyWord;
   static CTwoWords first_and_last_char, word_nextchar, word_lastchar, first_chars_two_words, lastword_lastchar;
   if (amount==0)
      first_and_last_char.refer(&first_char, &last_char);
   else
      first_and_last_char.allocate(first_char, last_char);
   const CWord &two_char = end<sentence->size()-1 ? _cache_word(end, 1) : g_emptyWord;
   if (index>0) {
      if (amount==0) {
         word_lastchar.refer(&word, &last_char_last_word);
         lastword_lastchar.refer(&last_char_last_word, &last_char);
      }
      else {
         word_lastchar.allocate(word, last_char_last_word);
         lastword_lastchar.allocate(last_char_last_word, last_char);
      }
   }
   if (end<sentence->size()-1) {
      if (amount==0) {
         word_nextchar.refer(&word, &first_char_next_word);
         first_chars_two_words.refer(&first_char, &first_char_next_word);
      }
      else {
         word_nextchar.allocate(word, first_char_next_word);
         first_chars_two_words.allocate(first_char, first_char_next_word);
      }
   }

   // about the length
   length = normalizeLength(length);
   last_length = normalizeLength(last_length);
   
   nReturn = 0;

   // ===================================================================================
   // character scores -- with end-1 middled
   for (tmp_i = max(0, static_cast<int>(end)-1); tmp_i < min(static_cast<int>(sentence->size()), end+2); ++tmp_i) {
      nReturn += weight.m_mapCharUnigram.getOrUpdateScore( make_pair( _cache_word(tmp_i, 1), encodeCharInfoAndPosition(char_info, tmp_i-end) ), which_score, amount, round);
      if (segmentor->hasCharTypeKnowledge()) nReturn += weight.m_mapCharCatUnigram.getOrUpdateScore( make_pair( groupCharTypes(segmentor, sentence, tmp_i, 1, amount), encodeCharInfoAndPosition(char_info, tmp_i-end) ), which_score, amount, round);
   }

   for (tmp_i = max(0, static_cast<int>(end)-1); tmp_i < min(static_cast<int>(sentence->size())-1, end+1); ++tmp_i) {
      nReturn += weight.m_mapCharBigram.getOrUpdateScore( make_pair( _cache_word(tmp_i, 2), encodeCharInfoAndPosition(char_info, tmp_i-end) ), which_score, amount, round);
      if (segmentor->hasCharTypeKnowledge()) nReturn += weight.m_mapCharCatBigram.getOrUpdateScore( make_pair( groupCharTypes(segmentor, sentence, tmp_i, 2, amount), encodeCharInfoAndPosition(char_info, tmp_i-end) ), which_score, amount, round);
   }

   for (tmp_i = max(0, static_cast<int>(end)-1); tmp_i < min(static_cast<int>(sentence->size())-2, end); ++tmp_i) {
      nReturn += weight.m_mapCharTrigram.getOrUpdateScore( make_pair( _cache_word(tmp_i, 3), encodeCharInfoAndPosition(char_info, tmp_i-end) ), which_score, amount, round);
      if (segmentor->hasCharTypeKnowledge()) nReturn += weight.m_mapCharCatTrigram.getOrUpdateScore( make_pair( groupCharTypes(segmentor, sentence, tmp_i, 3, amount), encodeCharInfoAndPosition(char_info, tmp_i-end) ), which_score, amount, round);
   }

   // ===================================================================================
   // word scores 
   nReturn += weight.m_mapSeenWords.getOrUpdateScore(word, which_score, amount, round); 
   if (length==1) 
      nReturn += weight.m_mapOneCharWord.getOrUpdateScore(word, which_score, amount, round);
   if (length>1) {
      nReturn += weight.m_mapFirstAndLastChars.getOrUpdateScore(first_and_last_char, which_score, amount, round);

      nReturn += weight.m_mapLengthByFirstChar.getOrUpdateScore(make_pair(first_char, length), which_score, amount, round);
      nReturn += weight.m_mapLengthByLastChar.getOrUpdateScore(make_pair(last_char, length), which_score, amount, round);
   }

   if (index>0) {
      nReturn += weight.m_mapLastWordByWord.getOrUpdateScore(two_word, which_score, amount, round);

      nReturn += weight.m_mapWordAndPrevChar.getOrUpdateScore(word_lastchar, which_score, amount, round);
      nReturn += weight.m_mapLastWordByLastChar.getOrUpdateScore(lastword_lastchar, which_score, amount, round);
    
      nReturn += weight.m_mapLengthByLastWord.getOrUpdateScore(make_pair(last_word, length), which_score, amount, round);
      nReturn += weight.m_mapLastLengthByWord.getOrUpdateScore(make_pair(word, last_length), which_score, amount, round);
   }
   if ( end < sentence->size()-1 ) {
      nReturn += weight.m_mapSeparateChars.getOrUpdateScore(two_char, which_score, amount, round); 
    
      nReturn += weight.m_mapWordAndNextChar.getOrUpdateScore(word_nextchar, which_score, amount, round);
      nReturn += weight.m_mapFirstCharLastWordByWord.getOrUpdateScore(first_chars_two_words, which_score, amount, round);
   }

   // ===================================================================================
   // word scores from knowledge
//   if ( segmentor->wordInLexicon(word) ) {
//      nReturn += weight.m_mapLexiconWord.getOrUpdateScore(length, which_score, amount, round) ;
//   }

   return nReturn;
}

/*---------------------------------------------------------------
 *
 * getOrUpdateAppendScore - get the local score when a word is appended
 *
 * When bigram is needed from the beginning of sentence, the empty word are used. 
 *
 * This implies that empty words should not be used in other 
 * situations. 
 *
 *--------------------------------------------------------------*/

SCORE_TYPE getOrUpdateAppendScore(CSegmentor *segmentor, const CStringVector* sentence, const CStateItem* item, int index, SCORE_TYPE amount=0, int round=0){

   static SCORE_TYPE retval;
   // about score
   static int which_score;
   which_score = segmentor->isTraining() ? CScore<SCORE_TYPE>::eNonAverage : CScore<SCORE_TYPE>::eAverage ;
   // abbreviation weight
   CWeight &weight = segmentor->getWeights();
   // temporary vals
   static int tmp_i;

   // about the chars
   const unsigned long start = item->getWordStart(index);
   const unsigned long end = item->getWordEnd(index);
   assert( start>=0 && start<sentence->size()-1 && end<sentence->size()-1 );
   static bool bWordStart; // the last char starts a word?
   static int char_info; // start, middle, end or standalone
   bWordStart = ( start == end);
   char_info = encodeCharSegmentation(bWordStart, false);

   static CWord first_char, current_char;
   static CTwoWords first_char_and_char;
   first_char = _cache_word( start, 1 );
   current_char = _cache_word(end+1, 1);
   if (amount==0) {
      first_char_and_char.refer(&first_char, &current_char);
   }
   else {
      first_char_and_char.allocate(first_char, current_char);
   }

   retval = 0;
   // ===================================================================================
   // character scores -- the middle character is end-1
   for (tmp_i = max(0, static_cast<int>(end)-1); tmp_i < min(static_cast<unsigned long>(sentence->size()), end+2); ++tmp_i) {
      retval += weight.m_mapCharUnigram.getOrUpdateScore( make_pair( _cache_word(tmp_i, 1), encodeCharInfoAndPosition(char_info, tmp_i-end) ), which_score, amount, round);
      if (segmentor->hasCharTypeKnowledge()) retval += weight.m_mapCharCatUnigram.getOrUpdateScore( make_pair( groupCharTypes(segmentor, sentence, tmp_i, 1, amount), encodeCharInfoAndPosition(char_info, tmp_i-end) ), which_score, amount, round);
   }

   for (tmp_i = max(0, static_cast<int>(end)-1); tmp_i < min(static_cast<unsigned long>(sentence->size())-1, end+1); ++tmp_i) {
      retval += weight.m_mapCharBigram.getOrUpdateScore( make_pair( _cache_word(tmp_i, 2), encodeCharInfoAndPosition(char_info, tmp_i-end) ), which_score, amount, round);
      if (segmentor->hasCharTypeKnowledge()) retval += weight.m_mapCharCatBigram.getOrUpdateScore( make_pair( groupCharTypes(segmentor, sentence, tmp_i, 2, amount), encodeCharInfoAndPosition(char_info, tmp_i-end) ), which_score, amount, round);
   }

   for (tmp_i = max(0, static_cast<int>(end)-1); tmp_i < min(static_cast<unsigned long>(sentence->size())-2, end); ++tmp_i) {
      retval += weight.m_mapCharTrigram.getOrUpdateScore( make_pair( _cache_word(tmp_i, 3), encodeCharInfoAndPosition(char_info, tmp_i-end) ), which_score, amount, round);
      if (segmentor->hasCharTypeKnowledge()) retval += weight.m_mapCharCatTrigram.getOrUpdateScore( make_pair( groupCharTypes(segmentor, sentence, tmp_i, 3, amount), encodeCharInfoAndPosition(char_info, tmp_i-end) ), which_score, amount, round);
   }

   retval += weight.m_mapConsecutiveChars.getOrUpdateScore( _cache_word(end, 2), which_score, amount, round); 
   retval += weight.m_mapFirstCharAndChar.getOrUpdateScore( first_char_and_char, which_score, amount, round);

   return retval;
}

#ifdef NO_NEG_FEATURE

/*---------------------------------------------------------------
 *
 * extractPosFeatures - extract features
 *                      from one training example
 *
 *--------------------------------------------------------------*/

void CFeatureHandle::extractPosFeatures(const CStringVector *sent) {

   CStateItem temp;
   CStringVector chars; 

   int i, j;

   m_parent->clearWordCache();

   for ( i=0; i<sent->size(); ++i)
      getCharactersFromUTF8String(sent->at(i), &chars);

   int end=-1;

   // when amount is not zero, getOrUpdateAppendScore and getOrUpdateSeparateScore will update
   // when round is -1, getOrUpdateScore from the score map will collect features
   for (i=0; i<sent->size(); ++i) {
      j = end+1;
      temp.append(j);
      while (j<end+getUTF8StringLength(sent->at(i))) {
         getOrUpdateAppendScore(m_parent, &chars, &temp, i, 1, -1);
         ++j;
         temp.replace(j);
      }
      getOrUpdateSeparateScore(m_parent, &chars, &temp, i, 1, -1);
      end = end+getUTF8StringLength(sent->at(i));
   }

}

#endif

/*---------------------------------------------------------------
 *
 * updateScoreVector - update the score vector 
 *                     this is standard training 
 *
 * Inputs: the output and the correct examples
 *
 * Affects: m_bScoreModified, which leads to saveScores on destructor
 *
 *--------------------------------------------------------------*/

void CFeatureHandle::updateScoreVector(const CStringVector* output, const CStringVector* correct, int round) {

   TRACE("Updating feature vector...");

   if ( *output == *correct ) return;
   m_parent->clearWordCache();

   CStateItem temp;
   CStringVector chars; 
   for (int i=0; i<output->size(); ++i)
      getCharactersFromUTF8String(output->at(i), &chars);

   int end=-1; 
   int index;
   for (int i=0; i<output->size(); ++i) {
      index = end+1;
      temp.append(index);
      while (index<end+getUTF8StringLength(output->at(i))) {
         getOrUpdateAppendScore(m_parent, &chars, &temp, i, -1, round);
         index++;
         temp.replace(index);
      }
      getOrUpdateSeparateScore(m_parent, &chars, &temp, i, -1, round);
      end = end+getUTF8StringLength(output->at(i));
   }

   temp.clear();
   end=-1;
   for (int i=0; i<correct->size(); ++i) {
      index = end+1;
      temp.append(index);
      while (index<end+getUTF8StringLength(correct->at(i))) {
         getOrUpdateAppendScore(m_parent, &chars, &temp, i, 1, round);
         index++;
         temp.replace(index);
      }
      getOrUpdateSeparateScore(m_parent, &chars, &temp, i, 1, round);
      end = end+getUTF8StringLength(correct->at(i));
   }
   //
   m_bScoreModified = true;
   TRACE("done");
}

/*---------------------------------------------------------------
 *
 * updateScoreVectorForState - update scores
 *
 * This method is called by the segmentor itself 
 *
 *--------------------------------------------------------------*/

inline void updateScoreVectorForState(CSegmentor *segmentor, const CStringVector* sentence, unsigned long index, CStateItem* state, SCORE_TYPE amount, int round) {
   static CStateItem temp;
   static int char_index;
   static int word_index; static unsigned next_start;
   temp.clear();
   word_index = -1; next_start = 0;
   for (char_index=0; char_index<index+1; ++char_index) {
      if (char_index>0 && char_index<next_start) {
         getOrUpdateAppendScore(segmentor, sentence, &temp, word_index, amount, round); // update score
	 temp.replace(char_index); // then take action
      }
      if (char_index==next_start) {
         if (char_index>0) getOrUpdateSeparateScore(segmentor, sentence, &temp, word_index, amount, round);
         temp.append(char_index); // next char
         ++word_index; next_start = state->getWordEnd(word_index)+1;
      }
   }
   if (index == sentence->size()) getOrUpdateSeparateScore(segmentor, sentence, &temp, word_index, amount, round);
}
/*---------------------------------------------------------------
 *
 * updateScoreVectorForStates - update scores
 *
 * This method is called by the segmentor itself 
 *
 *--------------------------------------------------------------*/

void updateScoreVectorForStates(CSegmentor *segmentor, const CStringVector* sentence, CStateItem* output, CStateItem* correct, int round) {
   static unsigned long index; index = output->getWordEnd(output->m_nLength-1); 
   assert(index==correct->getWordEnd(correct->m_nLength-1));
   updateScoreVectorForState(segmentor, sentence, index, output, -1, round);
   updateScoreVectorForState(segmentor, sentence, index, correct, 1, round);
}



/*===============================================================
 *
 * CSegmentor - the segmentor for Chinese 
 *
 *==============================================================*/

/*--------------------------------------------------------------
 *
 * trace_candidate - print detail
 *
 *--------------------------------------------------------------*/

void trace_candidate(CStateItem *pCandidate, const CStringVector &sentence) {
   for ( int j=0; j<pCandidate->m_nLength; ++j ) {
      string temp = "";
      for ( int l = pCandidate->getWordStart(j); l <= pCandidate->getWordEnd(j); ++l ) {
         assert(sentence.at(l)!=" "); // [SPACE]
         temp += sentence.at(l);
      }
      cout<<temp<<" ";
   }
   cout<<"|score="<<pCandidate->m_nScore<<endl;
}

/*---------------------------------------------------------------
 *
 * work - do the segmentation and if there is a correct
 *        reference then do early update learning
 *
 *--------------------------------------------------------------*/

bool work(CSegmentor *segmentor, const CStringVector &sentence, CRule &rules, CStateItem *pCorrect, int nBest, int round) {
   CStateItem *pGenerator, *pCandidate;
   int j, k;                                    // temporary index
   int index, temp_index;                       // the index of the current char
   static unsigned long int doneWordRnd[MAX_SENTENCE_SIZE];  // mask whether candidate with the last word has been cached
   static unsigned long int doneWordLink[MAX_SENTENCE_SIZE]; // link to the corresponding cache state item from word_length + 1
   static CStateItem doneWordItems[BEAM_SIZE];          // the allocated cache state items stores the candidates
   static int doneItemPointer; 
   static CStateItem temp_it, candidate, correct;
   unsigned long word_length;
   bool bCompatible; 
   const int length = sentence.size();

   pCandidate = &candidate; // $$$

   //clock_t start_time = clock();
   TRACE("Initialising the decoding process...");
   segmentor->clearWordCache(); 
   segmentor->m_Agenda->clear();

   pCandidate->clear();                         // restore state using clean
   segmentor->m_Agenda->pushCandidate(pCandidate); // $$$
   segmentor->m_Agenda->nextRound();            // as the generator item
   if (nBest == 1)                              // optimization for one best
      for (j=0; j<MAX_SENTENCE_SIZE; ++j)       // note we use MAX_SENTENCE_SIZE though the index is word length
         doneWordRnd[j] = 0;                    // so that we don't have to be limited by a fixed max word size.

   if (pCorrect) correct.clear();                             

   TRACE("Decoding started");
   //TRACE("initialisation time: " << clock() - start_time)
   for (index=0; index<length; index++) {
      // generate new state itmes for each character
      pGenerator = segmentor->m_Agenda->generatorStart();
      bCompatible = false;
      doneItemPointer = 0;

      for (j=0; j<segmentor->m_Agenda->generatorSize(); ++j) {

         if (pCorrect && (*pGenerator)==correct) bCompatible = true;

         // 1. generate new items according to each previous item. 
         if ( rules.canSeparate( index ) ) {  
            temp_it.copy( pGenerator );
            if (index>0) temp_it.m_nScore += getOrUpdateSeparateScore(segmentor, &sentence, &temp_it, temp_it.m_nLength-1);
            temp_it.append(index);
            if (index==length-1) temp_it.m_nScore += getOrUpdateSeparateScore(segmentor, &sentence, &temp_it, temp_it.m_nLength-1);

            word_length = index>0 ? pGenerator->getWordLength( pGenerator->m_nLength-1 ) : 0 ;
            if ( nBest == 1) {
               if ( doneWordRnd[word_length] < index+1 ) {
                  doneWordLink[word_length] = doneItemPointer;   // doneWordLink[i] caches the last word with length i+1
                  doneWordItems[doneItemPointer].copy(&temp_it); // copy item to cache.
                  ++doneItemPointer;
                  doneWordRnd[word_length] = index+1;
               }
               else {
                  assert(doneWordRnd[word_length] == index+1);
                  if ( temp_it > doneWordItems[doneWordLink[word_length]] )
                     doneWordItems[doneWordLink[word_length]].copy(&temp_it);
               }
            }
            else {
               segmentor->m_Agenda->pushCandidate(&temp_it); // $$$
            }
         }

         // 2. generate by replacing items
         if ( index > 0 && rules.canAppend(index) ) {
            pCandidate->copy(pGenerator);
            pCandidate->m_nScore += getOrUpdateAppendScore(segmentor, &sentence, pCandidate, pCandidate->m_nLength-1);
            pCandidate->replace(index) ;
            if (index==length-1) temp_it.m_nScore += getOrUpdateSeparateScore(segmentor, &sentence, pCandidate, pCandidate->m_nLength-1);
            segmentor->m_Agenda->pushCandidate(pCandidate); // $$$
         }

         pGenerator = segmentor->m_Agenda->generatorNext();  // next generator
      }

      // update scores if none from the agenda is correct state.
      if (pCorrect && !bCompatible) {
         TRACE("Decoding error, updating the weight vector");

         pGenerator = segmentor->m_Agenda->bestGenerator() ;

         updateScoreVectorForStates(segmentor, &sentence, segmentor->m_Agenda->bestGenerator(), &correct, round);
         return false;
      }

      // 3. recollect the items for separate
      if (nBest == 1) {
         for (temp_index = 0; temp_index<doneItemPointer; temp_index++) {
            pCandidate->copy(&(doneWordItems[temp_index]));
            segmentor->m_Agenda->pushCandidate(&doneWordItems[temp_index]); // $$$
         }
      }

      // standard move
      if (pCorrect) {
         if (correct.m_nLength==0) { // separate for the first word
            correct.append(index);
         }
         else { // for others, separate or append according to pCorrect
            static unsigned correct_end, this_end;
            correct_end = pCorrect->getWordEnd(correct.m_nLength-1);
            this_end = correct.getWordEnd(correct.m_nLength-1);
            if (correct_end > this_end ) {
               correct.replace(index);
            }
            else {
               assert(correct_end == this_end );
               correct.append(index);
            }
         }
      }

      segmentor->m_Agenda->nextRound(); // move round
   }

   if (pCorrect) {
      pGenerator = segmentor->m_Agenda->bestGenerator();
      if ((*pGenerator)!=correct) {
         TRACE("Decoding error, updating the weight vector");
         updateScoreVectorForStates(segmentor, &sentence, pGenerator, &correct, round);
         return false;
      }
   }

   TRACE("Decoding finished");
   return true;
}

/*---------------------------------------------------------------
 *
 * train - do automatic training process
 *
 *--------------------------------------------------------------*/

void CSegmentor::train(const CStringVector* sentence_input, const CStringVector* correct, int & round) {
#ifdef DEBUG
   clock_t total_start_time = clock();;
#endif
   TRACE("Starting training using a sentence...");
   static CStringVector sentence;
   static CRule rules(m_Feature->m_bRule);
   rules.segment(sentence_input, &sentence);
   const unsigned long int length = sentence.size();

   assert(length<MAX_SENTENCE_SIZE);

   static CStateItem item_correct;
   static int word_length, word_index, char_length, char_index; // word_xxx are from correct, char_xxx from sentence
   item_correct.clear();

   char_index = 0;
   int count = 0; 
   for (word_index=0; word_index<correct->size(); word_index++) {
      word_length = correct->at(word_index).size();
      char_length = 0; 
      while (char_length<word_length) {
         char_length += sentence[char_index++].size();
         count += 1;
      }
      assert(char_length==word_length);
      item_correct.append(count-1);
   }

   // the main learning process with update
   //while( work(this, sentence, rules, &item_correct, BEAM_SIZE, round )==false );
   work(this, sentence, rules, &item_correct, 1, round);

   TRACE("Done");
   TRACE("total time spent: " << double(clock() - total_start_time)/CLOCKS_PER_SEC);
}

/*---------------------------------------------------------------
 *
 * segment - do word segmentation on  a sentence
 *
 *--------------------------------------------------------------*/

void CSegmentor::segment(const CStringVector* sentence_input, CStringVector *vReturn, double *out_scores, int nBest) {
#ifdef DEBUG
   clock_t total_start_time = clock();;
#endif
   TRACE("Starting segmenting a sentence...");
   int j, k, l; 
   CStateItem *pGenerator;

   // turn the spaces in the input sentence into rules that separate corresponding characters
   static CStringVector sentence;
   static CRule rules(m_Feature->m_bRule); 
   rules.segment(sentence_input, &sentence); 
   const unsigned long length = sentence.size();

   assert(length<MAX_SENTENCE_SIZE);
   assert(vReturn!=NULL);
   vReturn->clear();

   // try to work out the best item with the
   // correct output reference param as NULL
   work(this, sentence, rules, NULL, nBest, -1);


   // now generate output sentence
   // n-best list will be stored in array
   // from the addr vReturn
   TRACE("Outputing sentence");
   m_Agenda->sortGenerators();
   for ( k=0; k<nBest; ++k ) {
      // clear
      vReturn[k].clear();
      if ( out_scores!=NULL ) 
         out_scores[k] = 0;
      // assign retval
      if ( k < m_Agenda->generatorSize() ) {
         pGenerator = m_Agenda->generator(k) ;
         for ( j=0; j<pGenerator->m_nLength; ++j ) {
            string temp = "";
            for ( l = pGenerator->getWordStart(j); l <= pGenerator->getWordEnd(j); ++l ) {
               assert(sentence.at(l)!=" "); // [SPACE]
               temp += sentence.at(l);
            }
            vReturn[k].push_back( temp );
         }
         if ( out_scores!=NULL )
            out_scores[k] = pGenerator->m_nScore;
      }
   }
   TRACE("Done, the best score: " << pGenerator->m_nScore);
   TRACE("total time spent: " << double(clock() - total_start_time)/CLOCKS_PER_SEC);
}

