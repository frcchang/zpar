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

void recordSegmentation(const CStringVector *raw, const CTwoStringVector* tagged, CBitArray &retval) {
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

void train(const string &sOutputFile, const string &sFeatureFile, const unsigned long &nBest, const unsigned long &nMaxSentSize, const bool &bEarlyUpdate, const bool &bSegmented, const string &sKnowledgePath, const bool &bDontJoinFWCD) {
   CTagger decoder(sFeatureFile, true, nMaxSentSize, sKnowledgePath, !bDontJoinFWCD);
   CSentenceReader output_reader(sOutputFile);
#ifdef DEBUG
   CSentenceWriter output_writer("");
   CSentenceWriter tagged_writer("");
#endif
   CStringVector *input_sent = new CStringVector;
   CTwoStringVector *tagged_sent = new CTwoStringVector[nBest];
   CTwoStringVector *output_sent = new CTwoStringVector; 

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
            for (int i=0; i<nBest; ++i) if (*(tagged_sent+i)!=*output_sent)
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
         for (int i=0; i<nBest; ++i) 
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
   try {
      COptions options(argc, argv);
      CConfigurations configurations;
      configurations.defineConfiguration("k", "Path", "use knowledge from the given path", "");
      configurations.defineConfiguration("m", "M", "maximum sentence size", "512");
      configurations.defineConfiguration("n", "N", "N best list train", "1");
      configurations.defineConfiguration("u", "", "early update", "");
      configurations.defineConfiguration("r", "", "use rules to segment numbers and letters", "");

      if (options.args.size() != 4) {
         cout << "\nUsage: " << argv[0] << " training_data model num_iterations" << endl ;
         cout << configurations.message() << endl;
         return 1;
      } 
      string warning = configurations.loadConfigurations(options.opts);
      if (!warning.empty()) {
         cout << "Warning: " << warning << endl;
      }

      unsigned long nBest, nMaxSentSize;
      if (!fromString(nMaxSentSize, configurations.getConfiguration("m"))) {
         cerr<<"Error: the max size of sentence is not integer." << endl; return 1;
      }
      if (!fromString(nBest, configurations.getConfiguration("n"))) {
         cerr<<"Error: the number of N best is not integer." << endl; return 1;
      }  
      bool bEarlyUpdate = configurations.getConfiguration("u").empty() ? false : true;
      bool bDontJoinFWCD = configurations.getConfiguration("r").empty() ? false : true;
      string sKnowledgePath = configurations.getConfiguration("k");
      bool bSegmented = false;
#ifdef SEGMENTED
      bSegmented = true; // compile option
#endif

      unsigned long training_rounds;
      if (!fromString(training_rounds, options.args[3])) {
         cerr << "Error: the number of training iterations must be an integer." << endl;
         return 1;
      }
      cout << "Training started." << endl;
      unsigned time_start = clock();
      for (unsigned i=0; i<training_rounds; ++i)
         train(argv[1], argv[2], nBest, nMaxSentSize, bEarlyUpdate, bSegmented, sKnowledgePath, bDontJoinFWCD);
      cout << "Training has finished successfully. Total time taken is: " << double(clock()-time_start)/CLOCKS_PER_SEC << endl;
      return 0;
   } catch (const string &e) {
      cerr << "Error: " << e << endl;
      return 1;
   }
}

