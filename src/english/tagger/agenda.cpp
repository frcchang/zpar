/****************************************************************
 *                                                              *
 * agenda.cpp - the agenda pos tagger implementation.           *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2007.10                        *
 *                                                              *
 ****************************************************************/

#include "tagger.h"

using namespace english;
using namespace english::tagger;

const CWord g_emptyWord("");
const CScore<SCORE_TYPE> g_zeroScore;

#include "morphology.h"

/*---------------------------------------------------------------
 *
 * getGlobalScore - get the global score for sentence
 *
 *--------------------------------------------------------------*/

SCORE_TYPE CTagger::getGlobalScore(CStringVector* sentence, CStateItem* item){
   SCORE_TYPE nReturn = 0;
   for (int i=0; i<item->m_nLength; i++)
      nReturn += getLocalScore(sentence, item, i);
   return nReturn;
}

/*---------------------------------------------------------------
 *
 * getLocalScore - get the local score for a word in sentence
 *
 * When bigram is needed from the beginning of sentence, the
 * -BEGIN- tag and the empty word are used. 
 *
 * This implies that empty words should not be used in other 
 * situations. 
 *
 *--------------------------------------------------------------*/

SCORE_TYPE CTagger::getLocalScore( CStringVector * sentence, CStateItem * item , unsigned int index ) {
   const CWord &word = m_Cache[index]; 
   const CWord &prev_word = index>0 ? m_Cache[index-1] : g_emptyWord; 
   const CWord &second_prev_word = index>1 ? m_Cache[index-2] : g_emptyWord;
   const CWord &next_word = index<m_Cache.size()-1 ? m_Cache[index+1] : g_emptyWord;
   const CWord &second_next_word = index<m_Cache.size()-2 ? m_Cache[index+2] : g_emptyWord;
   unsigned tag = item->getTag(index);
   unsigned prev_tag = index>0 ? item->getTag(index-1) : PENN_TAG_BEGIN;
   unsigned second_prev_tag = index>1 ? item->getTag(index-2) : PENN_TAG_BEGIN;
   unsigned prev_two_tag = joinTwoTags( second_prev_tag, prev_tag );

   static int i;
   static int word_size;
   static char letter;
   static bool bContainHyphen;
   static bool bContainNumber;
   static bool bContainCapitalLetter;
   static string prefix, suffix;

   static SCORE_TYPE nReturn;

   nReturn = m_weights->m_mapCurrentTag.find( make_pair(word, tag) , g_zeroScore ).score(m_nScoreIndex) ; 
   nReturn += m_weights->m_mapLastTagByTag.find( make_pair(prev_tag, tag), g_zeroScore ).score(m_nScoreIndex) ; 
   nReturn += m_weights->m_mapLastTwoTagsByTag.find( make_pair(prev_two_tag, tag), g_zeroScore ).score(m_nScoreIndex) ; 

   if (index>0) nReturn += m_weights->m_mapTagByPrevWord.find( make_pair(prev_word, tag) , g_zeroScore ).score(m_nScoreIndex) ; 
   if (index<m_Cache.size()-1) nReturn += m_weights->m_mapTagByNextWord.find( make_pair(next_word, tag) , g_zeroScore ).score(m_nScoreIndex) ; 
   if (index>1) nReturn += m_weights->m_mapTagBySecondPrevWord.find( make_pair(second_prev_word, tag) , g_zeroScore ).score(m_nScoreIndex) ; 
   if (index<m_Cache.size()-2) nReturn += m_weights->m_mapTagBySecondNextWord.find( make_pair(second_next_word, tag) , g_zeroScore ).score(m_nScoreIndex) ; 

   bContainHyphen = false;
   bContainNumber = false;
   bContainCapitalLetter = false;
   word_size = sentence->at(index).size();
   for ( i=0; i<word_size; i++ ) {
      letter = sentence->at(index)[i];
      if ( letter == '-' ) bContainHyphen = true;
      if ( letter >= '0' && letter <= '9' ) bContainNumber = true;
      if ( letter >= 'A' && letter <= 'Z' ) bContainCapitalLetter = true;
   }
   if (bContainHyphen) nReturn += m_weights->m_mapContainHyphen.find( tag, g_zeroScore ).score(m_nScoreIndex);
   if (bContainNumber) nReturn += m_weights->m_mapContainNumber.find( tag, g_zeroScore ).score(m_nScoreIndex);
   if (bContainCapitalLetter) nReturn += m_weights->m_mapContainCapitalLetter.find( tag, g_zeroScore ).score(m_nScoreIndex);

   prefix.clear();
   prefix += sentence->at(index)[0]; nReturn += m_weights->m_mapTagByPrefix.find( make_pair( prefix, tag ), g_zeroScore ).score(m_nScoreIndex);
   if ( word_size>1 ) prefix += sentence->at(index)[1]; nReturn += m_weights->m_mapTagByPrefix.find( make_pair( prefix, tag ), g_zeroScore ).score(m_nScoreIndex);
   if ( word_size>2 ) prefix += sentence->at(index)[2]; nReturn += m_weights->m_mapTagByPrefix.find( make_pair( prefix, tag ), g_zeroScore ).score(m_nScoreIndex);
   if ( word_size>3 ) prefix += sentence->at(index)[3]; nReturn += m_weights->m_mapTagByPrefix.find( make_pair( prefix, tag ), g_zeroScore ).score(m_nScoreIndex);

   // the processing of suffix is tricky - we are storing the revert of suffix!
   suffix.clear();
   suffix += sentence->at(index)[word_size-1]; nReturn += m_weights->m_mapTagBySuffix.find( make_pair( suffix, tag ), g_zeroScore ).score(m_nScoreIndex);
   if ( word_size>1 ) suffix += sentence->at(index)[word_size-2]; nReturn += m_weights->m_mapTagBySuffix.find( make_pair( suffix, tag ), g_zeroScore ).score(m_nScoreIndex);
   if ( word_size>2 ) suffix += sentence->at(index)[word_size-3]; nReturn += m_weights->m_mapTagBySuffix.find( make_pair( suffix, tag ), g_zeroScore ).score(m_nScoreIndex);
   if ( word_size>3 ) suffix += sentence->at(index)[word_size-4]; nReturn += m_weights->m_mapTagBySuffix.find( make_pair( suffix, tag ), g_zeroScore ).score(m_nScoreIndex);

   return nReturn;
}

/*---------------------------------------------------------------
 *
 * updateScoreVector - update the score vector by input
 *                     this is used in training to adjust params
 *
 * Inputs: the tagged and the correct example
 *
 * Affects: m_bScoreModified, which leads to saveScores on destructor
 *
 *--------------------------------------------------------------*/

void CTagger::updateScoreVector(CTwoStringVector* tagged, CTwoStringVector* correct, int round) {

   static int i;
   static unsigned long int possible_tags, current_tag;

   if ( *tagged != *correct ) {
      for (i=0; i<tagged->size(); ++i) 
         updateLocalFeatureVector(eSubtract, tagged, i, round);
      for (i=0; i<correct->size(); ++i)
         updateLocalFeatureVector(eAdd, correct, i, round);
      for (i=0; i<correct->size(); ++i) {
         possible_tags = m_TagDict.lookup(m_Cache[i]);
         if (possible_tags==0) possible_tags = static_cast<unsigned long>(static_cast<long int>(-1));
         possible_tags |= getPossibleTagsBySuffix( correct->at(i).first );
         possible_tags |= PENN_TAG_MUST_SEE ;
         current_tag = (static_cast<unsigned long>(1)<<CTag(correct->at(i).second).code()) ;
         if ( ( possible_tags & current_tag ) == 0 ) {
            cout << "Warning: knowledge contradicts with annotation for the word " << correct->at(i).first << " with tag " << correct->at(i).second << endl;
            m_TagDict.add(m_Cache[i], CTag(correct->at(i).second).code());
         }
      }
      m_bScoreModified = true;
   }

   if (round>m_nTrainingRound) {
      //
      // Updates that are common for all example
      //
      m_nTrainingRound = round;
   }
}

/*---------------------------------------------------------------
 *
 * updateLocalFeatureVector - update the given feature vector with
 *                            the local feature vector for a given
 *                            sentence. This is a private member only 
 *                            used by updateGlobalFeatureVector and is
 *                            only used for training. 
 *
 *--------------------------------------------------------------*/

void CTagger :: updateLocalFeatureVector( SCORE_UPDATE method , CTwoStringVector * sentence , int index , int round ) { 
   const CWord &word = m_Cache[index]; 
   const CWord &prev_word = index>0 ? m_Cache[index-1] : g_emptyWord; 
   const CWord &second_prev_word = index>1 ? m_Cache[index-2] : g_emptyWord;
   const CWord &next_word = index<m_Cache.size()-1 ? m_Cache[index+1] : g_emptyWord;
   const CWord &second_next_word = index<m_Cache.size()-2 ? m_Cache[index+2] : g_emptyWord;
   unsigned tag = CTag(sentence->at(index).second).code();
   unsigned prev_tag = index>0 ? CTag(sentence->at(index-1).second).code() : PENN_TAG_BEGIN;
   unsigned second_prev_tag = index>1 ? CTag(sentence->at(index-2).second).code() : PENN_TAG_BEGIN;
   unsigned prev_two_tag = joinTwoTags( second_prev_tag, prev_tag );

   static int i;
   static int word_size;
   static char letter;
   static bool bContainHyphen;
   static bool bContainNumber;
   static bool bContainCapitalLetter;
   static string prefix, suffix;

   SCORE_TYPE amount = method==eAdd ? 1 : -1;

   m_weights->m_mapCurrentTag[ make_pair(word, tag) ].updateCurrent( amount , round ) ;
   m_weights->m_mapLastTagByTag[ make_pair(prev_tag, tag) ].updateCurrent( amount , round ) ;
   m_weights->m_mapLastTwoTagsByTag[ make_pair(prev_two_tag, tag) ].updateCurrent( amount , round ) ;

   if (index>0) m_weights->m_mapTagByPrevWord[ make_pair(prev_word, tag) ].updateCurrent( amount , round ) ;
   if (index<m_Cache.size()-1) m_weights->m_mapTagByNextWord[ make_pair(next_word, tag) ].updateCurrent( amount , round ) ;
   if (index>1) m_weights->m_mapTagBySecondPrevWord[ make_pair(second_prev_word, tag) ].updateCurrent( amount , round ) ;
   if (index<m_Cache.size()-2) m_weights->m_mapTagBySecondNextWord[ make_pair(second_next_word, tag) ].updateCurrent( amount , round ) ;

   bContainHyphen = false;
   bContainNumber = false;
   bContainCapitalLetter = false;
   word_size = sentence->at(index).first.size();
   for ( i=0; i<word_size; i++ ) {
      letter = sentence->at(index).first[i];
      if ( letter == '-' ) bContainHyphen = true;
      if ( letter >= '0' && letter <= '9' ) bContainNumber = true;
      if ( letter >= 'A' && letter <= 'Z' ) bContainCapitalLetter = true;
   }
   if (bContainHyphen) m_weights->m_mapContainHyphen[ tag ].updateCurrent( amount , round );
   if (bContainNumber) m_weights->m_mapContainNumber[ tag ].updateCurrent( amount , round );
   if (bContainCapitalLetter) m_weights->m_mapContainCapitalLetter[ tag ].updateCurrent( amount , round );

   prefix.clear();
   prefix += sentence->at(index).first[0]; m_weights->m_mapTagByPrefix[ make_pair(prefix, tag) ].updateCurrent( amount , round );
   if ( word_size>1 ) prefix += sentence->at(index).first[1]; m_weights->m_mapTagByPrefix[ make_pair(prefix, tag) ].updateCurrent( amount , round );
   if ( word_size>2 ) prefix += sentence->at(index).first[2]; m_weights->m_mapTagByPrefix[ make_pair(prefix, tag) ].updateCurrent( amount , round );
   if ( word_size>3 ) prefix += sentence->at(index).first[3]; m_weights->m_mapTagByPrefix[ make_pair(prefix, tag) ].updateCurrent( amount , round );

   // the processing of suffix is tricky - we are storing the revert of suffix!
   suffix.clear();
   suffix += sentence->at(index).first[word_size-1]; m_weights->m_mapTagBySuffix[ make_pair(suffix, tag) ].updateCurrent( amount , round );
   if ( word_size>1 ) suffix += sentence->at(index).first[word_size-2]; m_weights->m_mapTagBySuffix[ make_pair(suffix, tag) ].updateCurrent( amount , round );
   if ( word_size>2 ) suffix += sentence->at(index).first[word_size-3]; m_weights->m_mapTagBySuffix[ make_pair(suffix, tag) ].updateCurrent( amount , round );
   if ( word_size>3 ) suffix += sentence->at(index).first[word_size-4]; m_weights->m_mapTagBySuffix[ make_pair(suffix, tag) ].updateCurrent( amount , round );

}

/*---------------------------------------------------------------
 *
 * loadScores - load scores from the file specified at constructor
 *              currently this uses database, but it can be modified
 *         
 * Affects: m_bScoreModified, clearing it. 
 *
 *--------------------------------------------------------------*/

void CTagger::loadScores() {
   m_weights->loadScores();
   m_bScoreModified = false;
}

/*---------------------------------------------------------------
 *
 * saveScores - save scores back to database
 *
 * This method is called by the destructor is m_bScoreModified
 * is true.
 *
 *--------------------------------------------------------------*/

void CTagger::saveScores() {
   m_weights->saveScores();
   m_bScoreModified = false;
}

/*---------------------------------------------------------------
 *
 * finishTraining - compute averaged scores and finish training
 *
 *--------------------------------------------------------------*/

void CTagger::finishTraining(int round) {
   m_weights->computeAverageFeatureWeights(round);
   saveScores();
   if ( m_sTagDictPath!="" ) m_TagDict.save(m_sTagDictPath);
}

/*===============================================================
 *
 * CTagger - the tagger for Chinese 
 *
 *==============================================================*/

/*---------------------------------------------------------------
 *
 * train - train the module auto
 *
 * Since we rely on external trainer, this method is empty
 *
 *--------------------------------------------------------------*/

void CTagger::train( const CStringVector * sentence , const CTwoStringVector * correct , int round ) {
   cerr << "Not implemented" << endl;
   assert( 0 == 1 );
}

/*---------------------------------------------------------------
 *
 * generate - helper function that generates tagged output
 *
 *--------------------------------------------------------------*/

void generate(const CStateItem *stateItem, CStringVector *sentence, CTagger *tagger, CTwoStringVector *vReturn) {
   for (int j=0; j<stateItem->m_nLength; j++) { 
      vReturn->push_back(make_pair(sentence->at(j), CTag(stateItem->getTag(j)).str()));
   }
}

/*---------------------------------------------------------------
 *
 * tag - assign POS tags to a sentence
 *
 * Returns: makes a new instance of CTwoStringVector 
 *
 *--------------------------------------------------------------*/

void CTagger::tag( CStringVector * sentence , CTwoStringVector * vReturn , int nBest , SCORE_TYPE * out_scores ) {
   clock_t total_start_time = clock();;
   const int length = sentence->size() ;
   static int index, temp_index, j;
   static int tag, last_tag;
   tagger::CStateItem *pGenerator;
   tagger::CStateItem *pCandidate;
   static CStateItem best_bigram[PENN_TAG_COUNT][PENN_TAG_COUNT];
   static int done_bigram[PENN_TAG_COUNT][PENN_TAG_COUNT];
   static SCORE_TYPE current_score;
   static unsigned long int possible_tags; // possible tags for a word

   assert(length<MAX_SENTENCE_SIZE);
   assert(vReturn!=NULL); 

   // initialise the return value, the agenda and cache
   TRACE("Initialising the tagging process...");
   vReturn->clear();

   m_Agenda->clear();
   pCandidate = m_Agenda->candidateItem();      // make the first item
   pCandidate->clear();                         // restore state using clean
   m_Agenda->pushCandidate();                   // and push it back
   m_Agenda->nextRound();                       // as the generator item

   m_Cache.clear();
   for ( index=0; index<length; index++ )
      m_Cache.push_back(CWord(sentence->at(index)));

   for ( tag=0; tag<PENN_TAG_COUNT; tag++ )
      for ( last_tag=0; last_tag<PENN_TAG_COUNT; last_tag++ )
         done_bigram[last_tag][tag] = -1;

   // start tag
   TRACE("Tagging started"); 

   for ( index=0; index<length; index++ ) {

      // generate new state itmes for each character
      pGenerator = m_Agenda->generatorStart();

      for ( j=0; j<m_Agenda->generatorSize(); j+=1 ) {

         last_tag = index>0 ? pGenerator->getTag(index-1) : PENN_TAG_BEGIN ;

         // lookup dictionary
         possible_tags = m_TagDict.lookup(m_Cache[index]);
         if (possible_tags==0) possible_tags = static_cast<unsigned long int>(static_cast<long int>(-1));
         possible_tags |= getPossibleTagsBySuffix( sentence->at(index) );
         possible_tags |= PENN_TAG_MUST_SEE ;
         //possible_tags |= getPossibleTagsByMorphology( m_TagDict, sentence->at(index) );

         assert(possible_tags!=0);

         bool bDone = false;
         for ( tag=2; tag<PENN_TAG_COUNT; ++tag ) {
            if ( possible_tags & (static_cast<unsigned long int>(1)<<tag) ) {
               bDone = true;
               pGenerator->setTag( index, tag );
               current_score = pGenerator->score() + getLocalScore(sentence, pGenerator, index); 
               if ( done_bigram[last_tag][tag] != index || current_score > best_bigram[last_tag][tag].score() ) {
                  done_bigram[last_tag][tag] = index;
                  best_bigram[last_tag][tag].copy( pGenerator );
                  best_bigram[last_tag][tag].append( tag );
                  best_bigram[last_tag][tag].score() = current_score;
               }
            }
         }assert(bDone);

         pGenerator = m_Agenda->generatorNext();  // next generator
      }

      for ( tag=2; tag<PENN_TAG_COUNT; ++tag )
         for ( last_tag=1; last_tag<PENN_TAG_COUNT; ++last_tag )
            if ( done_bigram[last_tag][tag]==index ) {
               pCandidate = m_Agenda->candidateItem();
               pCandidate->copy( &(best_bigram[last_tag][tag]) );
               m_Agenda->pushCandidate();
            }

      m_Agenda->nextRound(); // move round
   }

   // output 
   TRACE("Outputing sentence");
   m_Agenda->sortGenerators();
   for ( temp_index = 0 ; temp_index < nBest ; ++ temp_index ) {
      vReturn[temp_index].clear(); 
      generate(m_Agenda->generator(temp_index), sentence, this, &(vReturn[temp_index])); 
   }
   if (out_scores)
      out_scores[0] = m_Agenda->generator(0)->score();
   TRACE("Done, the highest score is: " << m_Agenda->generator(0)->score());
   TRACE("The total time spent: " << double(clock() - total_start_time)/CLOCKS_PER_SEC);
}


