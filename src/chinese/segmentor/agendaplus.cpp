/****************************************************************
 *                                                              *
 * agendaplus.cpp - the chinese segmentor                       *
 *                                                              *
 * This implementation uses beam search ageanda, see chart.cpp  *
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

SCORE_TYPE CFeatureHandle::getGlobalScore(const CSentenceRaw* sentence, const CStateItem* item){
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

SCORE_TYPE CFeatureHandle::getLocalScore(const CSentenceRaw* sentence, const CStateItem* item, int index){
   cerr << "Not implemented" << endl;
   assert(0==1);
}

/*---------------------------------------------------------------
 *
 * getOrUpdateSeparateScore - get the local score when a word is done
 *
 * The last word of the input state item is a complete word in
 * the sentence. 
 *
 *--------------------------------------------------------------*/

SCORE_TYPE getOrUpdateSeparateScore(CSegmentor *segmentor, const CSentenceRaw* sentence, const CStateItem* item, int index, bool bWordStart, SCORE_TYPE amount=0, int round=0){

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
   const CWord &word=amount==0 ? segmentor->findWordFromCache(start, length, sentence) 
                               : segmentor->replaceWordToCache(start, length, sentence) ; 
   const CWord &last_word = index>0 ? ( amount == 0 ? segmentor->findWordFromCache(last_start, last_length, sentence) 
                                                    : segmentor->replaceWordToCache(last_start, last_length, sentence) )
                                    : g_emptyWord; // use empty word for sentence beginners. 
   static CTwoWords two_word;
   if (amount==0)
      two_word.refer(&word, &last_word);
   else
      two_word.allocate(word, last_word);

   // about the chars
   static int char_info; // start, middle, end or standalone
   char_info = encodeCharSegmentation(bWordStart, true);
   const CWord &first_char=amount == 0 ? segmentor->findWordFromCache(start, 1, sentence)
                                       : segmentor->replaceWordToCache(start, 1, sentence);
   const CWord &last_char=amount==0 ? segmentor->findWordFromCache(end, 1, sentence)
                                    : segmentor->replaceWordToCache(end, 1, sentence) ;
   const CWord &first_char_last_word = index>0 ? ( amount==0 ? segmentor->findWordFromCache(last_start, 1, sentence) 
                                                             : segmentor->replaceWordToCache(last_start, 1, sentence) )
                                               : g_emptyWord;
   const CWord &last_char_last_word = index>0 ? ( amount==0 ? segmentor->findWordFromCache(last_end, 1, sentence) 
                                                            : segmentor->replaceWordToCache(last_end, 1, sentence) )
                                              : g_emptyWord;
   const CWord &first_char_next_word = end<sentence->size()-1 ? ( amount==0 ? segmentor->findWordFromCache(end+1, 1, sentence) 
                                                                            : segmentor->replaceWordToCache(end+1, 1, sentence) )
                                                              : g_emptyWord;
   static CTwoWords first_and_last_char, word_nextchar, word_lastchar, first_chars_two_words, lastword_lastchar;
   if (amount==0)
      first_and_last_char.refer(&first_char, &last_char);
   else
      first_and_last_char.allocate(first_char, last_char);
   const CWord &two_char = end<sentence->size()-1 ? ( amount==0 ? segmentor->findWordFromCache(end, 2, sentence) 
                                                                : segmentor->replaceWordToCache(end, 2, sentence) )
                                                  : g_emptyWord;
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
   // character scores 
   for (tmp_i = max(0, static_cast<int>(end)-0); tmp_i < min(static_cast<int>(sentence->size()), end+1); ++tmp_i) 
      nReturn += weight.m_mapCharUnigram.getOrUpdateScore( make_pair( ( amount==0 ? segmentor->findWordFromCache(tmp_i,1,sentence)
                                                                                  : segmentor->replaceWordToCache(tmp_i,1,sentence) ),
                                                   encodeCharInfoAndPosition(char_info, tmp_i-end) ), which_score, 
                                                   amount, round);

   for (tmp_i = max(0, static_cast<int>(end)-1); tmp_i < min(static_cast<int>(sentence->size())-1, end+1); ++tmp_i) 
      nReturn += weight.m_mapCharBigram.getOrUpdateScore( make_pair( ( amount==0 ? segmentor->findWordFromCache(tmp_i,2,sentence)
                                                                                 : segmentor->replaceWordToCache(tmp_i,2,sentence) ),
                                                  encodeCharInfoAndPosition(char_info, tmp_i-end) ), which_score, 
                                                  amount, round);

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
   if ( segmentor->hasCharTypeKnowledge() ) {
      if ( end>0 && end<sentence->size()-1 ) {
         tmp_i = encodeCharInfoAndType( 
                    char_info , 
                    segmentor->charType( ( amount==0 ? segmentor->findWordFromCache(end-1, 1, sentence)
                                                     : segmentor->replaceWordToCache(end-1, 1, sentence) ) ) ,
                    segmentor->charType( ( amount==0 ? segmentor->findWordFromCache(end, 1, sentence)
                                                     : segmentor->replaceWordToCache(end, 1, sentence) ) ) ,
                    segmentor->charType( ( amount==0 ? segmentor->findWordFromCache(end+1, 1, sentence)
                                                     : segmentor->replaceWordToCache(end+1, 1, sentence) ) ) 
                 ) ;
         nReturn += weight.m_mapCharCatTrigram.getOrUpdateScore(tmp_i, which_score, amount, round) ;
      }
   }

   // ===================================================================================
   // word scores from knowledge
   if ( segmentor->wordInLexicon(word) ) {
      nReturn += weight.m_mapLexiconWord.getOrUpdateScore(length, which_score, amount, round) ;
   }

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

SCORE_TYPE getOrUpdateAppendScore(CSegmentor *segmentor, const CSentenceRaw* sentence, const CStateItem* item, int index, bool bWordStart, SCORE_TYPE amount=0, int round=0){

   static SCORE_TYPE retval;
   // about score
   static int which_score;
   which_score = segmentor->isTraining() ? CScore<SCORE_TYPE>::eNonAverage : CScore<SCORE_TYPE>::eAverage ;
   // abbreviation weight
   CWeight &weight = segmentor->getWeights();
   // temporary vals
   static int tmp_i;

   // about the chars
   static int char_info; // start, middle, end or standalone
   char_info = encodeCharSegmentation(bWordStart, false);
   const unsigned int end = item->getWordEnd(index);
   assert( end<sentence->size()-1 );

   retval = 0;
   // ===================================================================================
   // character scores 
   for (tmp_i = max(0, static_cast<int>(end)-0); tmp_i < min(static_cast<unsigned>(sentence->size()), end+1); ++tmp_i) 
      retval += weight.m_mapCharUnigram.getOrUpdateScore( make_pair( ( amount==0 ? segmentor->findWordFromCache(tmp_i,1,sentence)
                                                                                 : segmentor->replaceWordToCache(tmp_i,1,sentence) ),
                                                          encodeCharInfoAndPosition(char_info, tmp_i-end) ), which_score, 
                                                          amount, round);

   for (tmp_i = max(0, static_cast<int>(end)-1); tmp_i < min(static_cast<unsigned>(sentence->size())-1, end+1); ++tmp_i) 
      retval += weight.m_mapCharBigram.getOrUpdateScore( make_pair( ( amount==0 ? segmentor->findWordFromCache(tmp_i,2,sentence)
                                                                                : segmentor->replaceWordToCache(tmp_i,2,sentence) ),
                                                         encodeCharInfoAndPosition(char_info, tmp_i-end) ), which_score, 
                                                         amount, round);

   retval += weight.m_mapConsecutiveChars.getOrUpdateScore((amount==0 ? segmentor->findWordFromCache(end, 2, sentence)
                                                                      : segmentor->replaceWordToCache(end, 2, sentence) ), 
                                                           which_score, amount, round); 

   // ===================================================================================
   // word scores from knowledge
   if ( segmentor->hasCharTypeKnowledge() ) {
      if ( end>0 && end<sentence->size()-1 ) {
         tmp_i = encodeCharInfoAndType( 
                    char_info , 
                    segmentor->charType((amount==0 ? segmentor->findWordFromCache(end-1, 1, sentence)
                                                   : segmentor->replaceWordToCache(end-1, 1, sentence))) ,
                    segmentor->charType((amount==0 ? segmentor->findWordFromCache(end, 1, sentence)
                                                   : segmentor->replaceWordToCache(end, 1, sentence))) ,
                    segmentor->charType((amount==0 ? segmentor->findWordFromCache(end+1, 1, sentence)
                                                   : segmentor->replaceWordToCache(end+1, 1, sentence))) 
                 ) ;
         retval += weight.m_mapCharCatTrigram.getOrUpdateScore(tmp_i, which_score, amount, round) ;
      }
   }

   return retval;
}

#ifdef NO_NEG_FEATURE

/*---------------------------------------------------------------
 *
 * extractPosFeatures - extract features
 *                      from one training example
 *
 *--------------------------------------------------------------*/

void CFeatureHandle::extractPosFeatures(const CSentenceRaw *sent) {

   CStateItem temp;
   CSentenceRaw chars; 

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
         getOrUpdateAppendScore(m_parent, &chars, &temp, i, j==end+1, 1, -1);
         j++;
         temp.replace(j);
      }
      getOrUpdateSeparateScore(m_parent, &chars, &temp, i, j==end+1, 1, -1);
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

void CFeatureHandle::updateScoreVector(const CSentenceRaw* output, const CSentenceRaw* correct, int round) {

   if ( *output == *correct ) return;

   m_parent->clearWordCache();

   CStateItem temp;
   CSentenceRaw chars; 
   for (int i=0; i<output->size(); ++i)
      getCharactersFromUTF8String(output->at(i), &chars);

   int end=-1; 
   int index;
   for (int i=0; i<output->size(); ++i) {
      index = end+1;
      temp.append(index);
      while (index<end+getUTF8StringLength(output->at(i))) {
         getOrUpdateAppendScore(m_parent, &chars, &temp, i, index==end+1, -1, round);
         index++;
         temp.replace(index);
      }
      getOrUpdateSeparateScore(m_parent, &chars, &temp, i, index==end+1, -1, round);
      end = end+getUTF8StringLength(output->at(i));
   }

   temp.clear();
   end=-1;
   for (int i=0; i<correct->size(); ++i) {
      index = end+1;
      temp.append(index);
      while (index<end+getUTF8StringLength(correct->at(i))) {
         getOrUpdateAppendScore(m_parent, &chars, &temp, i, index==end+1, 1, round);
         index++;
         temp.replace(index);
      }
      getOrUpdateSeparateScore(m_parent, &chars, &temp, i, index==end+1, 1, round);
      end = end+getUTF8StringLength(correct->at(i));
   }
   //
   m_bScoreModified = true;
}

/*---------------------------------------------------------------
 *
 * updateScoreVector - update scores
 *
 * This method is called by the segmentor itself 
 *
 *--------------------------------------------------------------*/

void updateScoreVector(CSegmentor *segmentor, const CSentenceRaw* sentence, unsigned int index, CStateItem* output, CStateItem* correct, int round) {
   static CStateItem temp;
   static int char_index;
   static int word_index;
   temp.clear();
   char_index = -1; 
   word_index = -1; 
   while ( char_index<index ) {
      word_index++ ;
      char_index++;
      temp.append(char_index);
      while ( char_index < min(output->getWordEnd(word_index),index) ) {
         if ( char_index < output->getWordEnd(word_index) ) {
            getOrUpdateAppendScore(segmentor, sentence, &temp, word_index, char_index==output->getWordStart(word_index), -1, round);
            char_index++;
            temp.replace(char_index);//
         }
      }
      if (char_index==index&&char_index<output->getWordEnd(word_index)) {
         getOrUpdateAppendScore(segmentor, sentence, &temp, word_index, char_index==output->getWordStart(word_index), -1, round);
      }
      else {
         getOrUpdateSeparateScore(segmentor, sentence, &temp, word_index, char_index==output->getWordStart(word_index), -1, round);
      }
   }
   temp.clear();
   char_index = -1;
   word_index = -1;
   while ( char_index<index ) {
      word_index++;
      char_index++ ;
      temp.append(char_index);
      while (char_index< correct->getWordEnd(word_index)) {
         getOrUpdateAppendScore(segmentor, sentence, &temp, word_index, char_index==correct->getWordStart(word_index), 1, round);
         char_index++;
         temp.replace(char_index);
      }
      getOrUpdateSeparateScore(segmentor, sentence, &temp, word_index, char_index==correct->getWordStart(word_index), 1, round);
   }
   assert(char_index==correct->getWordEnd(word_index));
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

void trace_candidate(CStateItem *pCandidate, const CSentenceRaw &sentence) {
   for ( int j=0; j<pCandidate->m_nLength; j++ ) {
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

bool work(CSegmentor *segmentor, const CSentenceRaw &sentence, CPrune &rules, CStateItem *pCorrect, int nBest, int round) {
   CStateItem *pGenerator, *pCandidate;
   int nScore;
   int j, k;                                    // temporary index
   int m, n;                                    // temporary nums
   int index, temp_index;                       // the index of the current char
   int add_score, subtract_score;               // the score to be subtracted (previous item)
   static unsigned int doneWordRnd[MAX_SENTENCE_SIZE];  // mask whether candidate with the last word has been cached
   static unsigned int doneWordLink[MAX_SENTENCE_SIZE]; // link to the corresponding cache state item from word_length + 1
   static CStateItem doneWordItems[BEAM_SIZE];          // the allocated cache state items stores the candidates
   static int doneItemPointer; 
   static CStateItem temp_it, candidate;
   unsigned int word_length;
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

   TRACE("Decoding started");
   //TRACE("initialisation time: " << clock() - start_time)
   for (index=0; index<length; index++) {
      // generate new state itmes for each character
      pGenerator = segmentor->m_Agenda->generatorStart();
      if (pCorrect) bCompatible = pCorrect->isCompatibleWith(pGenerator);
      doneItemPointer = 0;

      for (j=0; j<segmentor->m_Agenda->generatorSize(); ++j) {

         // 1. generate new items according to each previous item. 
         if ( rules.canSeparate( index ) &&
              pGenerator->canSeparate() 
            ) {  

            // push a candidate with the current char as the start of a word
            if ( index < sentence.size()-1 ) {
               pCandidate->copy(pGenerator);
               pCandidate->append(index);
               pCandidate->m_nScore += getOrUpdateAppendScore(segmentor, &sentence, pCandidate, pCandidate->m_nLength-1, true);
               pCandidate->setWordEnd(false);
               segmentor->m_Agenda->pushCandidate(&candidate); // $$$
            }

            // now explore candidate with the current char being standalone
            temp_it.copy(pGenerator);
            temp_it.append(index);
            temp_it.m_nScore += getOrUpdateSeparateScore(segmentor, &sentence, &temp_it, temp_it.m_nLength-1, true);
            temp_it.setWordEnd(true);

            // maybe we need to cache the current word for single best
            if ( nBest == 1 ) {
               if ( doneWordRnd[0] < index+1 ) {
                  doneWordLink[0] = doneItemPointer;             // doneWordLink[i] caches the last word with length i+1
                  doneWordItems[doneItemPointer].copy(&temp_it); // copy item to cache.
                  ++doneItemPointer;
                  doneWordRnd[0] = index+1;
               }
               else if ( temp_it > doneWordItems[doneWordLink[0]] )
                  doneWordItems[doneWordLink[0]].copy(&temp_it);
            }
            else {
               segmentor->m_Agenda->pushCandidate(&temp_it); // $$$
            }
         }
         // 2. generate by replacing items
         if ( index > 0 && rules.canAppend(index) && pGenerator->canAppend() ) {

            // push a candidate with the current char as the middle of a word
            if ( index < sentence.size()-1 ) {
               pCandidate->copy(pGenerator);
               pCandidate->replace(index) ;
               pCandidate->m_nScore += getOrUpdateAppendScore(segmentor, &sentence, pCandidate, pCandidate->m_nLength-1, false);
               pCandidate->setWordEnd(false);
               segmentor->m_Agenda->pushCandidate(pCandidate); // $$$
            }

            // now explore candidate with the current char the end of a word
            word_length = pGenerator->getWordLength( pGenerator->m_nLength-1 );
            temp_it.copy(pGenerator);
            temp_it.replace(index);
            temp_it.m_nScore += getOrUpdateSeparateScore(segmentor, &sentence, &temp_it, temp_it.m_nLength-1, false);
            temp_it.setWordEnd(true);

            // maybe we need to cache the current word for single best
            if ( nBest == 1) {
               if ( doneWordRnd[word_length] < index+1 ) {
                  doneWordLink[word_length] = doneItemPointer;   // doneWordLink[i] caches the last word with length i+1
                  doneWordItems[doneItemPointer].copy(&temp_it); // copy item to cache.
                  ++doneItemPointer;
                  doneWordRnd[word_length] = index+1;
               }
               else if ( temp_it > doneWordItems[doneWordLink[word_length]] )
                  doneWordItems[doneWordLink[word_length]].copy(&temp_it);
            }
            else {
               segmentor->m_Agenda->pushCandidate(&temp_it); // $$$
            }
         }
         pGenerator = segmentor->m_Agenda->generatorNext();  // next generator
         if (pCorrect&&pGenerator) bCompatible |= pCorrect->isCompatibleWith(pGenerator);
      }
      // 3. recollect the items for separate
      if (nBest == 1) {
         for (temp_index = 0; temp_index<doneItemPointer; temp_index++) {
            pCandidate->copy(&(doneWordItems[temp_index]));
            segmentor->m_Agenda->pushCandidate(&doneWordItems[temp_index]); // $$$
         }
      }

      if (pCorrect && !bCompatible) {
         TRACE("Decoding error, updating the weight vector");

         pGenerator = segmentor->m_Agenda->bestGenerator() ;

         updateScoreVector(segmentor, &sentence, index-1, segmentor->m_Agenda->bestGenerator(), pCorrect, round);
         return false;
      }
      segmentor->m_Agenda->nextRound(); // move round
   }

   if (pCorrect) {
      pGenerator = segmentor->m_Agenda->bestGenerator();
      if ((*pGenerator)!=(*pCorrect)) {
         TRACE("Decoding error, updating the weight vector");
         updateScoreVector(segmentor, &sentence, index-1, pGenerator, pCorrect, round);
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

void CSegmentor::train(const CSentenceRaw* sentence_input, const CSentenceRaw* correct, int & round) {
#ifdef DEBUG
   clock_t total_start_time = clock();;
#endif
   TRACE("Starting training using a sentence...");
   static CSentenceRaw sentence;
   static CPrune rules;                         // 0 - no rules; 1 - append; 2 - separate
   segmentationRules(sentence_input, m_CharCatForRules, &sentence, &rules);
   const unsigned int length = sentence.size();

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
   work(this, sentence, rules, &item_correct, BEAM_SIZE, round);

   TRACE("Done");
   TRACE("total time spent: " << double(clock() - total_start_time)/CLOCKS_PER_SEC);
}

/*---------------------------------------------------------------
 *
 * segment - do word segmentation on  a sentence
 *
 *--------------------------------------------------------------*/

void CSegmentor::segment(const CSentenceRaw* sentence_input, CSentenceRaw *vReturn, double *out_scores, int nBest) {
#ifdef DEBUG
   clock_t total_start_time = clock();;
#endif
   TRACE("Starting segmenting a sentence...");
   int j, k, l; 
   CStateItem *pGenerator;

   // turn the spaces in the input sentence into rules that separate corresponding characters
   static CSentenceRaw sentence;
   static CPrune rules;                         // 0 - no rules; 1 - append; 2 - separate
   segmentationRules(sentence_input, m_CharCatForRules, &sentence, &rules); 
   const unsigned int length = sentence.size();

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
         for ( j=0; j<pGenerator->m_nLength; j++ ) {
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

