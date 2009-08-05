/****************************************************************
 *                                                              *
 * train.cpp - the training of the Chinese tagger.              *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2006.10                        *
 *                                                              *
 ****************************************************************/

#include "definitions.h"
#include "tagger.h"
#include "reader.h"
#include "writer.h"
#include "charcat.h"

#include <cstring>

using namespace chinese;

const int TRAINING_ROUND = 1;
//
// The following definitions must be consistent with the combined segmentor and tagger
//
#define N 64
#define MAX_SENTENCE_SIZE 512

static CCharCatDictionary g_CharCat ;

/*----------------------------------------------------------------
 *
 * recordSegmentation - record a segmented sentence with bitarr.
 * 
 * Input raw is the raw format of input tagged, it is provided
 * here because the caller already did UntagAndDesegmentSent
 * 
 *---------------------------------------------------------------*/

void recordSegmentation(const CSentenceRaw *raw, const CSentenceTagged* tagged, CBitArray &retval) {
   vector<int> indice;
   indice.clear(); 
   for (int i=0; i<raw->size(); ++i) {
      for (int j=0; j<raw->at(i).size(); ++j)
         indice.push_back(i);
   }
   retval.clear();
   int start=0;
   for (int i=0; i<tagged->size(); ++i) {
      int word_start = indice[start];
      int word_end = indice[start+tagged->at(i).first.size()-1];
      start += tagged->at(i).first.size();
      retval.set(word_end);
   }
}

/*===============================================================
 *
 * train
 *
 *==============================================================*/

void train(const string &sOutputFile, const string &sFeatureFile, const int &nBest, const bool &bEarlyUpdate, const bool &bSegmented, const string &sKnowledgePath, const bool &bDontJoinFWCD) {
   CTagger decoder(sFeatureFile, true);
   if (!sKnowledgePath.empty()) decoder.loadKnowledge(sKnowledgePath);
   CSentenceReader output_reader(sOutputFile);
#ifdef DEBUG
   CSentenceWriter output_writer("");
   CSentenceWriter tagged_writer("");
#endif
   CSentenceRaw *input_sent = new CSentenceRaw;
   CSentenceTagged *tagged_sent = new CSentenceTagged[nBest];
   CSentenceTagged *output_sent = new CSentenceTagged; 

   int nCount=0;
   int nErrorCount=0;
   int nEarlyUpdateRepeat=0;

   CBitArray word_ends(MAX_SENTENCE_SIZE);

   //
   // Read the next sentence
   //
   while( output_reader.readTaggedSentence(output_sent) ) {
      if (bDontJoinFWCD)
         UntagAndDesegmentSentence(output_sent, input_sent, g_CharCat);
      else
         UntagAndDesegmentSentence(output_sent, input_sent);
      TRACE("Sentence " << nCount);
      ++nCount;
      //
      // Find the decoder output
      //
      if (bSegmented) {
         recordSegmentation( input_sent, output_sent, word_ends );
         decoder.tag(input_sent, tagged_sent, NULL, nBest, &word_ends);
      }
      else {
         decoder.tag(input_sent, tagged_sent, NULL, nBest);
      }
      if (bEarlyUpdate) {
      //------------------------------------------------
      //
      // Early update
      //
      //------------------------------------------------
         if ( *tagged_sent != *output_sent ) 
            ++nErrorCount;

         nEarlyUpdateRepeat = 0;
         while ( *tagged_sent != *output_sent && nEarlyUpdateRepeat < 200 ) {
            //
            // Debug output
            //
#ifdef DEBUG
            output_writer.writeSentence(output_sent);
#endif
            TRACE("------");
            for (int i=0; i<nBest; i++) if (*(tagged_sent+i)!=*output_sent)
            {
#ifdef DEBUG
               tagged_writer.writeSentence(tagged_sent+i);
#endif
               decoder.updateScores(tagged_sent+i, output_sent, nCount);
            }
            //
            // Find the decoder output
            //
            if (bSegmented) {
               recordSegmentation( input_sent, output_sent, word_ends );
               decoder.tag(input_sent, tagged_sent, NULL, nBest, &word_ends);
            }
            else 
               decoder.tag(input_sent, tagged_sent, NULL, nBest);

            nEarlyUpdateRepeat ++ ;
         } // while the sentence wrong
      }
      else {
      //------------------------------------------------
      //
      // Normal update
      //
      //------------------------------------------------
         if (*tagged_sent!=*output_sent) {
#ifdef DEBUG
            output_writer.writeSentence(input_sent);
            output_writer.writeSentence(output_sent);
#endif
            TRACE("------");  
            ++nErrorCount;
         }
         for (int i=0; i<nBest; i++) 
         {
#ifdef DEBUG
            if (*(tagged_sent+i)!=*output_sent) tagged_writer.writeSentence(tagged_sent+i);
#endif
            decoder.updateScores(tagged_sent+i, output_sent, nCount);
         }
      }
   }
   delete input_sent;
   delete output_sent;
   delete [] tagged_sent;
   cout << "Completing the training process." << endl;
   decoder.finishTraining(nCount);
   cout << "Done. Total errors: " << nErrorCount << endl;
}

/*===============================================================
 *
 * main
 *
 *==============================================================*/

int main(int argc, char* argv[]) {
   const string hint = " training_input_file feature_file number_of_iterations [-j] [-kPATH] [-nN] [-u]\n\n\
Options:\n\
-j separate numbers and letters when preparing input data from training examples\n\
-k use knowledge by the given path\n\
-n n best list train\n\
-u early update\n\
";
   if (argc < 4) {
      cout << "\nUsage: " << argv[0] << hint << endl ;
      return 1;
   } 
   int nBest = 1;
   bool bEarlyUpdate = false;
   bool bSegmented = false;
   bool bDontJoinFWCD = false;
   string sKnowledgePath = "";
#ifdef SEGMENTED
   bSegmented = true; // compile option
#endif
   int training_rounds = atoi(argv[3]);
   if (training_rounds < 0)
      training_rounds = TRAINING_ROUND;
   if (argc>4) {
      for (int i=4; i<argc; i++) {
         if (argv[i][0]!='-') { cout << "\nUsage: " << argv[0] << hint << endl ; return 1; }
         switch (argv[i][1]) {
            case 'j':
               bDontJoinFWCD = true;
               break;
            case 'k':
               if (strlen(argv[i])<3) { cout << "\nUsage: " << argv[0] << hint << endl ; return 1; }
               sKnowledgePath = string(argv[i]).substr(2);
               break;
            case 'n':
               nBest = atoi(string(argv[i]).substr(2).c_str());
               break;
            case 'u':
               bEarlyUpdate = true;
               break;
            default:
               cout << "\nUsage: " << argv[0] << hint << endl ;
               return 1;
         }
      }
   }
   cout << "Training started" << endl;
   int time_start = clock();
   for (int i=0; i<training_rounds; ++i)
      train(argv[1], argv[2], nBest, bEarlyUpdate, bSegmented, sKnowledgePath, bDontJoinFWCD);
   cout << "Training has finished successfully. Total time taken is: " << double(clock()-time_start)/CLOCKS_PER_SEC << endl;
   return 0;
}

