// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                              *
 * reranker.cpp - the reranker for tagged and depparsed outputs *
 *                                                              *
 * It updates the tagger and depparser in a unified manner      *
 *                                                              *
 * Yue Zhang                                                    *
 * 2008                                                         *
 *                                                              *
 ****************************************************************/

#include "reranker.h"
#include "writer.h"

#include "linguistics/sentence_string.h"

using namespace chinese;

/*----------------------------------------------------------------
 *
 * RecordSegmentation - record a segmented sentence with bitarr.
 *
 * Input raw is the raw format of input parsed, it is provided
 * here because the caller already did UnparseAndDesegment
 *
 *---------------------------------------------------------------*/

void RecordSegmentation(const CStringVector *raw, const CSentenceParsed* parsed, int *retval) {
   std::vector<int> indice;
   indice.clear();
   for (int i=0; i<raw->size(); ++i) {
      for (int j=0; j<raw->at(i).size(); ++j)
         indice.push_back(i);
   }
   int start=0;
   for (int i=0; i<parsed->size(); ++i) {
      int word_start = indice[start];
      int word_end = indice[start+parsed->at(i).word.size()-1];
      start += parsed->at(i).word.size();
      retval[i] = word_end;
   }
}

/*----------------------------------------------------------------
 *
 * getFScore - get link f score
 *
 *----------------------------------------------------------------*/

void getFScore(const CSentenceParsed &output, const CSentenceParsed &correct, double &fSeg, double &fTag, double &fPar, double &fParUnlabeled, double &fParIncPunc) {

   static CStringVector raw;

   int nCandidateWords = 0;
   int nReferenceWords = 0;
   int nCandidateWordsExcPunc = 0;
   int nReferenceWordsExcPunc = 0;

   int nCorrectWords = 0;
   int nCorrectTags = 0;
   int nCorrectLinks = 0;
   int nCorrectLinksUnlabeled = 0;
   int nCorrectLinksExcPunc = 0;

   int indexCandidateWord = 0;
   int indexReferenceWord = 0;

   static int lReferenceIndex[MAX_SENTENCE_SIZE+2];
   static int lCandidateIndex[MAX_SENTENCE_SIZE+2];

   static int lReferenceIndexForCandidate[MAX_SENTENCE_SIZE];
   static int lCandidateIndexForReference[MAX_SENTENCE_SIZE];

   lReferenceIndex[0] = -1;
   lCandidateIndex[0] = -1;
   int *lReferenceWords = lReferenceIndex + 1; // the end char of each word in the sentence
   int *lCandidateWords = lCandidateIndex + 1; // the end char of each word in the sentence

   UnparseAndDesegmentSentence(&correct, &raw);
   RecordSegmentation(&raw, &correct, lReferenceWords);
   RecordSegmentation(&raw, &output, lCandidateWords);

   // get the corresponding index for candidate and reference words
   // lCandidateIndexForReference[indexReferenceWord] {-1 no match}
   while (indexCandidateWord<output.size() && indexReferenceWord<correct.size()) {
      if (lCandidateWords[indexCandidateWord] == lReferenceWords[(indexReferenceWord)] &&
          lCandidateWords[indexCandidateWord-1] == lReferenceWords[(indexReferenceWord-1)] ) { // words right
         nCorrectWords++ ;
         if (output.at(indexCandidateWord).tag == correct.at(indexReferenceWord).tag)  //: # tags
            nCorrectTags++ ;
         lReferenceIndexForCandidate[indexCandidateWord] = (indexReferenceWord);
         lCandidateIndexForReference[indexReferenceWord] = indexCandidateWord;
         indexCandidateWord ++ ;
         indexReferenceWord ++ ;
      }
      else {
         if (lCandidateWords[indexCandidateWord] == lReferenceWords[indexReferenceWord]) {
            lReferenceIndexForCandidate[indexCandidateWord] = -1;
            lCandidateIndexForReference[indexReferenceWord] = -1;
            indexCandidateWord++ ;
            indexReferenceWord++ ;
         }
         else if (lCandidateWords[indexCandidateWord] < lReferenceWords[indexReferenceWord]) {
            lReferenceIndexForCandidate[indexCandidateWord] = -1;
            indexCandidateWord ++ ;
         }
         else if (lCandidateWords[indexCandidateWord] > lReferenceWords[indexReferenceWord]) {
            lCandidateIndexForReference[indexReferenceWord] = -1;
            indexReferenceWord ++ ;
         }
      }
   }

   while (indexCandidateWord<output.size()) {
      assert (indexReferenceWord==correct.size());
      lReferenceIndexForCandidate[indexCandidateWord] = -1;
      indexCandidateWord ++ ;
   }
   while (indexReferenceWord<correct.size()) {
      assert (indexCandidateWord==output.size());
      lCandidateIndexForReference[indexReferenceWord] = -1;
      indexReferenceWord ++ ;
   }

   // now compute precision recall
   int referenceHead;
   for (indexCandidateWord = 0; indexCandidateWord<output.size(); indexCandidateWord++) {
      indexReferenceWord = lReferenceIndexForCandidate[indexCandidateWord];
      nCandidateWords ++ ;
      if (output[indexCandidateWord].tag != "PU")
         nCandidateWordsExcPunc ++ ;
      if (indexReferenceWord != -1) {
         referenceHead = correct[indexReferenceWord].head;
         if (referenceHead != -1) referenceHead = lCandidateIndexForReference[referenceHead];
         if ( (output[indexCandidateWord].head == referenceHead) ) {
            if (correct[indexReferenceWord].tag != "PU")
               nCorrectLinksUnlabeled ++ ;
            if (output[indexCandidateWord].tag == correct[indexReferenceWord].tag)  {
               nCorrectLinks ++ ;
               if (correct[indexReferenceWord].tag != "PU")
                  nCorrectLinksExcPunc ++ ;
            }
         }
      }
   }

   for (indexReferenceWord = 0; indexReferenceWord<correct.size(); indexReferenceWord++) {
      nReferenceWords++ ;
      if (correct[indexReferenceWord].tag != "PU") //:
         nReferenceWordsExcPunc ++ ;
   }

   double precision = static_cast<double>(nCorrectLinks) / static_cast<double>(nCandidateWords);
   double recall = static_cast<double>(nCorrectLinks) / static_cast<double>(nReferenceWords);
   fParIncPunc = precision * recall * 2.0 / (precision + recall);

   precision = static_cast<double>(nCorrectLinksUnlabeled) / static_cast<double>(nCandidateWordsExcPunc);
   recall = static_cast<double>(nCorrectLinksUnlabeled) / static_cast<double>(nReferenceWordsExcPunc);
   fParUnlabeled = precision * recall * 2.0 / (precision + recall);

   precision = static_cast<double>(nCorrectLinksExcPunc) / static_cast<double>(nCandidateWordsExcPunc);
   recall = static_cast<double>(nCorrectLinksExcPunc) / static_cast<double>(nReferenceWordsExcPunc);
   fPar = (precision * recall * 2.0) / (precision + recall);

   precision = static_cast<double>(nCorrectWords) / nCandidateWords;
   recall = static_cast<double>(nCorrectWords) / nReferenceWords;
   fSeg = precision * recall * 2.0 / (precision + recall);

   precision = static_cast<double>(nCorrectTags) / nCandidateWords;
   recall = static_cast<double>(nCorrectTags) / nReferenceWords;
   fTag = precision * recall * 2.0 / (precision + recall);
}

/*----------------------------------------------------------------
 *
 * getOrUpdatePriorScores - get or addsub score
 *
 *----------------------------------------------------------------*/

double CReranker::getOrUpdatePriorScores(double *prior_scores, int amount) {
   double retval;
   retval = 0;
   retval += prior_scores[0] * m_weights->m_mapPriors.getOrUpdateScore(0, m_nScoreIndex, amount*prior_scores[0], m_nTrainingRound);
   retval += prior_scores[1] * m_weights->m_mapPriors.getOrUpdateScore(1, m_nScoreIndex, amount*prior_scores[1], m_nTrainingRound);
   return retval;
}

/*----------------------------------------------------------------
 *
 * findBest - find the best tagged and depparsed sequence index
 *
 *---------------------------------------------------------------*/

int CReranker::findBest(const CSentenceParsed *nbest, int nBest, double *prior_scores) {
   int best_index=-1;
   double score, best_score;
   static CTwoStringVector tagged;
   for (int i=0; i<nBest; ++i) {
      if (nbest[i].empty()) {
         if (best_index==-1)
            best_index = i;
         continue;
      }

      UnparseSentence(nbest+i, &tagged);
      score = m_tagger->getGlobalScore(&tagged);
      score += m_depparser->getGlobalScore(nbest[i]);
      score += getOrUpdatePriorScores(prior_scores+(i*2), 0);//
      if (best_index==-1 || score>best_score) {
         best_index=i; best_score=score;
      }
   }
   assert (best_index!=-1);
   TRACE("Best score" << best_score);
   return best_index;
}

/*----------------------------------------------------------------
 *
 * rerank - decoding interface
 *
 *---------------------------------------------------------------*/

void CReranker::rerank(const CSentenceParsed *nbest, CSentenceParsed *std::cout, int nBest, double *prior_scores) {
   assert(!m_bTrain);
   int best_index = findBest(nbest, nBest, prior_scores);
   *std::cout = nbest[best_index];
}

/*----------------------------------------------------------------
 *
 * train - training interface
 *
 *---------------------------------------------------------------*/

void CReranker::train(const CSentenceParsed *nbest, const CSentenceParsed *correct, int nBest, double *prior_scores) {

   assert(m_bTrain);
   ++m_nTrainingRound;

   int best_idx;
   static double fSeg, fTag, fPar, fParUnlabeled, fParIncPunc;
   static double bestFPar, bestFParIncPunc, bestFTag, bestFSeg, bestFParUnlabeled;

   // find the most correct from nbest
   for (int i=0; i<nBest; ++i) {
      getFScore(nbest[i], *correct, fSeg, fTag, fPar, fParUnlabeled, fParIncPunc);
      if (i!=0&&fPar==bestFPar&&fParIncPunc==bestFParIncPunc&&fParUnlabeled==bestFParUnlabeled&&fTag==bestFTag&&fSeg==bestFSeg)
         TRACE("equal"<<std::endl<<nbest[best_idx]<<nbest[i]);
      if (i==0 || fPar>bestFPar ||
                 (fPar==bestFPar&&fParIncPunc>bestFParIncPunc) ||
                 (fPar==bestFPar&&fParIncPunc==bestFParIncPunc&&fParUnlabeled>bestFParUnlabeled) ||
                 (fPar==bestFPar&&fParIncPunc==bestFParIncPunc&&fParUnlabeled==bestFParUnlabeled&&fTag>bestFTag) ||
                 (fPar==bestFPar&&fParIncPunc==bestFParIncPunc&&fParUnlabeled==bestFParUnlabeled&&fTag==bestFTag&&fSeg>bestFSeg)
         ) {
         TRACE("refresh")
         best_idx = i; bestFPar = fPar; bestFParIncPunc = fParIncPunc; bestFTag = fTag; bestFSeg = fSeg; bestFParUnlabeled = fParUnlabeled;
      }
   }

   CTwoStringVector tagged, best_tagged;
   UnparseSentence(nbest+best_idx, &best_tagged);

   // find the reranking output
   int output_idx = findBest(nbest, nBest, prior_scores);
   assert(nbest[output_idx].empty()==false);
   UnparseSentence(nbest+output_idx, &tagged);

   // update scores
   m_tagger->updateScores(&tagged, &best_tagged, m_nTrainingRound);
   if ( nbest[output_idx] != nbest[best_idx] ) {
      TRACE(nbest[best_idx]);
      TRACE(nbest[output_idx])
      ++m_nErrors;
   }
   m_depparser->updateScores(nbest[output_idx], nbest[best_idx], m_nTrainingRound);

   // update prior
   if (output_idx!=best_idx) {
      getOrUpdatePriorScores(prior_scores + best_idx*2, 1.0);
      getOrUpdatePriorScores(prior_scores + output_idx*2, -1.0);
   }
}

/*----------------------------------------------------------------
 *
 * finishTraining - must_call
 *
 *---------------------------------------------------------------*/

void CReranker::finishTraining() {
   m_tagger->finishTraining(m_nTrainingRound);
   m_depparser->finishtraining();
   m_weights->computeAverageFeatureWeights(m_nTrainingRound);
   m_weights->saveScores();
   assert(m_bTrain);
   std::cerr << m_nTrainingRound << " training sentences in total have been processed; " << m_nErrors << " mistakes made."<< std::endl;
}

