// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                              *
 * agenda.cpp - the chinese segmentor                       *
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
   std::cerr << "Not implemented" << std::endl;
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
   std::cerr << "Not implemented" << std::endl;
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
 * the sentence. getOrUpdateSeparateScore is defined at the
 * end of each word in the sentence. It calculates the score of the next char
 * being separated.
 *
 *--------------------------------------------------------------*/

SCORE_TYPE getOrUpdateSeparateScore(CSegmentor *segmentor, const CStringVector* sentence, const CStateItem* item, SCORE_TYPE amount=0, int round=0){

   if (item->empty()) return 0;
   assert(item->prev()); // non-empty items have

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
   start = item->getWordStart();
   end = item->getWordEnd();
   length = item->getWordLength();
   last_start = item->prev()->getWordStart(); // make sure that this is only used when index>0
   last_end = item->prev()->getWordEnd(); // make sure that this is only used when index>0
   last_length = item->prev()->getWordLength();  // similar to the above
   word_length = length ;

   // about the words
   const CWord &word=_cache_word(start, length);
   const CWord &last_word = item->prev()->empty() ? g_emptyWord : _cache_word(last_start, last_length); // use empty word for sentence beginners.
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
   const CWord &first_char_last_word = item->prev()->empty() ? g_emptyWord : _cache_word(last_start, 1);
   const CWord &last_char_last_word = item->prev()->empty() ? g_emptyWord : _cache_word(last_end, 1);
   const CWord &first_char_next_word = end<sentence->size()-1 ? _cache_word(end+1, 1) : g_emptyWord;
   static CTwoWords first_and_last_char, word_nextchar, word_lastchar, first_chars_two_words, lastword_lastchar;
   if (amount==0)
      first_and_last_char.refer(&first_char, &last_char);
   else
      first_and_last_char.allocate(first_char, last_char);
   const CWord &two_char = end<sentence->size()-1 ? _cache_word(end, 2) : _cache_word(end, 1);
   if (!item->prev()->empty()) {
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
   for (tmp_i = std::max(0, static_cast<int>(end)-1); tmp_i < std::min(static_cast<int>(sentence->size()), end+2); ++tmp_i) {
      nReturn += weight.m_mapCharUnigram.getOrUpdateScore( std::make_pair( _cache_word(tmp_i, 1), encodeCharInfoAndPosition(char_info, tmp_i-end) ), which_score, amount, round);
      if (segmentor->hasCharTypeKnowledge()) nReturn += weight.m_mapCharCatUnigram.getOrUpdateScore( std::make_pair( groupCharTypes(segmentor, sentence, tmp_i, 1, amount), encodeCharInfoAndPosition(char_info, tmp_i-end) ), which_score, amount, round);
   }

   for (tmp_i = std::max(0, static_cast<int>(end)-1); tmp_i < std::min(static_cast<int>(sentence->size())-1, end+1); ++tmp_i) {
      nReturn += weight.m_mapCharBigram.getOrUpdateScore( std::make_pair( _cache_word(tmp_i, 2), encodeCharInfoAndPosition(char_info, tmp_i-end) ), which_score, amount, round);
      if (segmentor->hasCharTypeKnowledge()) nReturn += weight.m_mapCharCatBigram.getOrUpdateScore( std::make_pair( groupCharTypes(segmentor, sentence, tmp_i, 2, amount), encodeCharInfoAndPosition(char_info, tmp_i-end) ), which_score, amount, round);
   }

   for (tmp_i = std::max(0, static_cast<int>(end)-1); tmp_i < std::min(static_cast<int>(sentence->size())-2, end); ++tmp_i) {
      nReturn += weight.m_mapCharTrigram.getOrUpdateScore( std::make_pair( _cache_word(tmp_i, 3), encodeCharInfoAndPosition(char_info, tmp_i-end) ), which_score, amount, round);
      if (segmentor->hasCharTypeKnowledge()) nReturn += weight.m_mapCharCatTrigram.getOrUpdateScore( std::make_pair( groupCharTypes(segmentor, sentence, tmp_i, 3, amount), encodeCharInfoAndPosition(char_info, tmp_i-end) ), which_score, amount, round);
   }

   // ===================================================================================
   // word scores
   nReturn += weight.m_mapSeenWords.getOrUpdateScore(word, which_score, amount, round);
   if (length==1)
      nReturn += weight.m_mapOneCharWord.getOrUpdateScore(word, which_score, amount, round);
   if (length>1) {
      nReturn += weight.m_mapFirstAndLastChars.getOrUpdateScore(first_and_last_char, which_score, amount, round);

      nReturn += weight.m_mapLengthByFirstChar.getOrUpdateScore(std::make_pair(first_char, length), which_score, amount, round);
      nReturn += weight.m_mapLengthByLastChar.getOrUpdateScore(std::make_pair(last_char, length), which_score, amount, round);
   }

   if (!item->prev()->empty()) {
      nReturn += weight.m_mapLastWordByWord.getOrUpdateScore(two_word, which_score, amount, round);

      nReturn += weight.m_mapWordAndPrevChar.getOrUpdateScore(word_lastchar, which_score, amount, round);
      nReturn += weight.m_mapLastWordByLastChar.getOrUpdateScore(lastword_lastchar, which_score, amount, round);

      nReturn += weight.m_mapLengthByLastWord.getOrUpdateScore(std::make_pair(last_word, length), which_score, amount, round);
      nReturn += weight.m_mapLastLengthByWord.getOrUpdateScore(std::make_pair(word, last_length), which_score, amount, round);
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
 * getOrUpdateAppendScore is defined for each char excpet the last char
 * in each word. It calculates the score for the next char being appended.
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
   const unsigned long start = item->getWordStart();
   const unsigned long end = index;
   assert( start>=0 && start<sentence->size() && end<sentence->size() );
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
   for (tmp_i = std::max(0, static_cast<int>(end)-1); tmp_i < std::min(static_cast<unsigned long>(sentence->size()), end+2); ++tmp_i) {
      retval += weight.m_mapCharUnigram.getOrUpdateScore( std::make_pair( _cache_word(tmp_i, 1), encodeCharInfoAndPosition(char_info, tmp_i-end) ), which_score, amount, round);
      if (segmentor->hasCharTypeKnowledge()) retval += weight.m_mapCharCatUnigram.getOrUpdateScore( std::make_pair( groupCharTypes(segmentor, sentence, tmp_i, 1, amount), encodeCharInfoAndPosition(char_info, tmp_i-end) ), which_score, amount, round);
   }

   for (tmp_i = std::max(0, static_cast<int>(end)-1); tmp_i < std::min(static_cast<unsigned long>(sentence->size())-1, end+1); ++tmp_i) {
      retval += weight.m_mapCharBigram.getOrUpdateScore( std::make_pair( _cache_word(tmp_i, 2), encodeCharInfoAndPosition(char_info, tmp_i-end) ), which_score, amount, round);
      if (segmentor->hasCharTypeKnowledge()) retval += weight.m_mapCharCatBigram.getOrUpdateScore( std::make_pair( groupCharTypes(segmentor, sentence, tmp_i, 2, amount), encodeCharInfoAndPosition(char_info, tmp_i-end) ), which_score, amount, round);
   }

   for (tmp_i = std::max(0, static_cast<int>(end)-1); tmp_i < std::min(static_cast<unsigned long>(sentence->size())-2, end); ++tmp_i) {
      retval += weight.m_mapCharTrigram.getOrUpdateScore( std::make_pair( _cache_word(tmp_i, 3), encodeCharInfoAndPosition(char_info, tmp_i-end) ), which_score, amount, round);
      if (segmentor->hasCharTypeKnowledge()) retval += weight.m_mapCharCatTrigram.getOrUpdateScore( std::make_pair( groupCharTypes(segmentor, sentence, tmp_i, 3, amount), encodeCharInfoAndPosition(char_info, tmp_i-end) ), which_score, amount, round);
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

   static CStateItem temp[MAX_SENTENCE_SIZE+2];
   static CStringVector chars;
   static int i, j, end;

   m_parent->clearWordCache();

   for ( i=0; i<sent->size(); ++i)
      getCharactersFromUTF8String(sent->at(i), &chars);

   // when amount is not zero, getOrUpdateAppendScore and getOrUpdateSeparateScore will update
   // when round is -1, getOrUpdateScore from the score map will collect features
   temp[0].clear();
   end=-1;
   for (i=1; i<=sent->size(); ++i) {
      j = end+1;
      temp[j-1].separate(temp+j);
      while (j<end+getUTF8StringLength(sent->at(i))) {
         getOrUpdateAppendScore(m_parent, &chars, temp+j, j, 1, -1);
         ++j;
         temp[j-1].append(temp+j);
      }
      getOrUpdateSeparateScore(m_parent, &chars, temp+j, j, 1, -1);
      end = end+getUTF8StringLength(sent->at(i));
   }

}

#endif

/*---------------------------------------------------------------
 *
 * updateScoreVector - update the score std::vector
 *                     this is standard training
 *
 * Inputs: the output and the correct examples
 *
 * Affects: m_bScoreModified, which leads to saveScores on destructor
 *
 *--------------------------------------------------------------*/

void CFeatureHandle::updateScoreVector(const CStringVector* output, const CStringVector* correct, int round) {
   THROW("This version of the segmentor does not support updating model with sentences directly; it uses early-update.");
}

/*---------------------------------------------------------------
 *
 * updateScoreVectorForState - update scores
 *
 * This method is called by the segmentor itself
 *
 *--------------------------------------------------------------*/

inline void updateScoreVectorForState(CSegmentor *segmentor, const CStringVector* sentence, const CStateItem* state, SCORE_TYPE amount, int round) {
   static const CStateItem *temp;
   static int char_index;

   temp = state;
   while (!temp->empty()) {
      getOrUpdateSeparateScore(segmentor, sentence, temp, amount, round);
      for (char_index=temp->getWordStart(); char_index<temp->getWordEnd(); ++char_index) {
         getOrUpdateAppendScore(segmentor, sentence, temp, char_index, amount, round); // update score
      }
      temp = temp->prev();
   }
}

/*---------------------------------------------------------------
 *
 * updateScoreVectorForStates - update scores
 *
 * This method is called by the segmentor itself
 *
 *--------------------------------------------------------------*/

void updateScoreVectorForStates(CSegmentor *segmentor, const CStringVector* sentence, const CStateItem* output, const CStateItem* correct, int round) {
   updateScoreVectorForState(segmentor, sentence, output, -1, round);
   updateScoreVectorForState(segmentor, sentence, correct, 1, round);
}



/*===============================================================
 *
 * CSegmentor - the segmentor for Chinese
 *
 *==============================================================*/

/*---------------------------------------------------------------
 *
 * work - do the segmentation and if there is a correct
 *        reference then do early update learning
 *
 *--------------------------------------------------------------*/

bool work(CSegmentor *segmentor, const CStringVector &sentence, CStringVector *vReturn, double *out_scores, CRule &rules, std::vector<unsigned> *correct_starts, unsigned nBest, int round) {
   static CStateItem lattice[(MAX_SENTENCE_SIZE+2)*BEAM_SIZE];
   static CStateItem *lattice_index[MAX_SENTENCE_SIZE+2];
   static const CStateItem *pGenerator, *pBestGen;
   static const CStateItem *correct, *temp;
   static int index, temp_index;                       // the index of the current char
   static unsigned long int doneWordRnd[MAX_SENTENCE_SIZE];  // mask whether candidate with the last word has been cached
   static unsigned long int doneWordLink[MAX_SENTENCE_SIZE]; // link to the corresponding cache state item from word_length + 1
   static CScoredAct doneWordItems[BEAM_SIZE];
   static int doneItemPointer;
   static unsigned correct_word;
   static bool correct_append;
   static unsigned long word_length;
   static bool bCompatible;
   const int length = sentence.size();
   static CAgendaSimple<CScoredAct> beam(BEAM_SIZE);
   static CScoredAct action;
   static const CStateItem *best[BEAM_SIZE];
   static unsigned nBestGen;

   //clock_t start_time = clock();
   TRACE("Initialising the decoding process...");
   segmentor->clearWordCache();

   lattice[0].clear();
   lattice_index[0] = lattice;
   lattice_index[1] = lattice+1;

   if (correct_starts) {
      correct = lattice;
      correct_word=0;
      correct_append=false;
   }

   if (nBest == 1) // optimization for one best
      memset(doneWordRnd, 0, MAX_SENTENCE_SIZE*sizeof(doneWordRnd[0]));

   TRACE("Decoding started");
   // index is character index and lattice index shifts 1 right
   for (index=0; index<length; ++index) {

      lattice_index[index+2] = lattice_index[index+1];

      // generate new state itmes for each character
      beam.clear();

      doneItemPointer = 0;

      for (pGenerator=lattice_index[index]; pGenerator!=lattice_index[index+1]; ++pGenerator) { // for each generator

         // 1. generate new items according to each previous item.
         if ( rules.canSeparate( index ) ) {
            action.load(pGenerator, false, getOrUpdateSeparateScore(segmentor, &sentence, pGenerator));
            if ( nBest == 1 ) {
               word_length = pGenerator->getWordLength();
               if ( doneWordRnd[word_length] < index+1 ) {
                  doneWordLink[word_length] = doneItemPointer;   // doneWordLink[i] caches the last word with length i+1
                  doneWordItems[doneItemPointer]=action; // copy item to cache.
                  ++doneItemPointer;
                  doneWordRnd[word_length] = index+1;
               }
               else {
                  assert(doneWordRnd[word_length] == index+1);
                  if ( action > doneWordItems[doneWordLink[word_length]] )
                     doneWordItems[doneWordLink[word_length]]=action;
               }
            }
            else {
               beam.insertItem(&action);
            }
         }

         // 2. generate by replacing items
         if ( index > 0 && rules.canAppend(index) ) {
            action.load(pGenerator, true, getOrUpdateAppendScore(segmentor, &sentence, pGenerator, index-1));
            beam.insertItem(&action);
         }

      }

      // 3. recollect the items for separate
      if (nBest == 1) {
         for (temp_index = 0; temp_index<doneItemPointer; ++temp_index) {
            beam.insertItem(&doneWordItems[temp_index]);
         }
      }

      // build new items in decode
      if (correct_starts) {
         bCompatible = false;
         if (index==correct_starts->at(correct_word)) {
            correct_append = false;
            ++correct_word;
         }
         else {
            assert(correct_word==correct_starts->size()||index<correct_starts->at(correct_word));
            correct_append = true;
         }
         pBestGen = 0;
      }
      for (temp_index=0; temp_index<beam.size(); ++temp_index) {
         pGenerator = beam.item(temp_index)->item;
         if (beam.item(temp_index)->append)
            pGenerator->append(lattice_index[index+2]);
         else
            pGenerator->separate(lattice_index[index+2]);
         lattice_index[index+2]->score = beam.item(temp_index)->score;
         if (correct_starts) {
            if (pBestGen==0 || lattice_index[index+2]->score > pBestGen->score)
               pBestGen = lattice_index[index+2];
            if (correct == pGenerator && correct_append == beam.item(temp_index)->append) {
               bCompatible = true;
               correct = lattice_index[index+2];
            }
         }
         ++lattice_index[index+2];
      }

      // update scores if none from the agenda is correct state.
      if (correct_starts && !bCompatible) {
         TRACE("Decoding error, updating the weight std::vector");
         if (correct_append)
            correct->append(lattice_index[index+2]);
         else
            correct->separate(lattice_index[index+2]);
         updateScoreVectorForStates(segmentor, &sentence, pBestGen, lattice_index[index+2], round);
         return false;
      }

   }

   // a final step adding the last separate score for items.
   beam.clear();
   for (pGenerator=lattice_index[length]; pGenerator!=lattice_index[length+1]; ++pGenerator) {
      action.load(pGenerator, false, getOrUpdateSeparateScore(segmentor, &sentence, pGenerator));
      beam.insertItem(&action);
   }
   beam.sortItems(); // sort final items
   nBestGen = beam.size();
   for (temp_index=0; temp_index<nBestGen; ++temp_index) {
      best[temp_index] = beam.item(temp_index)->item;
   }

   if (correct_starts) {
      assert(bCompatible);
      if (correct!=best[0]) {
         TRACE("Decoding error, updating the weight std::vector");
         updateScoreVectorForStates(segmentor, &sentence, best[0], correct, round);
         return false;
      }
   }

   TRACE("Decoding finished");

   // now generate output sentence
   // n-best list will be stored in array
   if (!correct_starts){
      TRACE("Outputing sentence");
      for ( index=0; index<std::min(nBest, nBestGen); ++index ) {
         // clear
         vReturn[index].clear();
         if ( out_scores ) out_scores[index] = 0;
         // assign retval
         static unsigned count;
         static unsigned start;
         count = 0;
         temp = best[index];
         while (!temp->empty()) {
            ++count;
            temp = temp->prev();
         }
         vReturn[index].resize(count);
         --count;
         temp = best[index];
         while (!temp->empty()) {
            for (temp_index=temp->getWordStart(); temp_index<=temp->getWordEnd(); ++temp_index) {
               vReturn[index].at(count) += sentence.at(temp_index);
            }
            --count;
            temp = temp->prev();
         }
         if ( out_scores!=NULL )
            out_scores[index] = best[index]->score;
      }
   }
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

   static std::vector<unsigned> correct_starts;
   static int word_length, word_index, char_length, char_index; // word_xxx are from correct, char_xxx from sentence

   char_index = 0;
   int count = 0;
   correct_starts.clear();
   correct_starts.push_back(count);
   for (word_index=0; word_index<correct->size(); word_index++) {
      word_length = correct->at(word_index).size();
      char_length = 0;
      while (char_length<word_length) {
         char_length += sentence[char_index++].size();
         count += 1;
      }
      assert(char_length==word_length);
      correct_starts.push_back(count);
   }

   // the main learning process with update
   work(this, sentence, 0, 0, rules, &correct_starts, 1, round);

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

   // turn the spaces in the input sentence into rules that separate corresponding characters
   static CStringVector sentence;
   static CRule rules(m_Feature->m_bRule);
   rules.segment(sentence_input, &sentence);
   const unsigned long length = sentence.size();

   assert(length<MAX_SENTENCE_SIZE);
   assert(vReturn!=NULL);
   vReturn->clear();

   // try to work std::cout the best item with the
   // correct output reference param as NULL
   work(this, sentence, vReturn, out_scores, rules, NULL, nBest, -1);

   TRACE("total time spent: " << double(clock() - total_start_time)/CLOCKS_PER_SEC);
}

