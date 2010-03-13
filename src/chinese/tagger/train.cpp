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

//
// The following definitions must be consistent with the combined segmentor and tagger
//
#define MAX_SENTENCE_SIZE 512

/*----------------------------------------------------------------
 *
 * recordSegmentation - record a segmented sentence with bitarr.
 * 
 * Input raw is the raw format of input tagged, it is provided
 * here because the caller already did UntagAndDesegmentSent
 * 
 *---------------------------------------------------------------*/

void recordSegmentation(const CStringVector *raw, const CTwoStringVector* tagged, CBitArray &retval) {
   vector<unsigned> indice;
   indice.clear(); 
   for (unsigned i=0; i<raw->size(); ++i) {
      for (unsigned j=0; j<raw->at(i).size(); ++j)
         indice.push_back(i);
   }
   retval.clear();
   unsigned start=0;
   for (unsigned i=0; i<tagged->size(); ++i) {
      unsigned word_start = indice[start];
      unsigned word_end = indice[start+tagged->at(i).first.size()-1];
      start += tagged->at(i).first.size();
      retval.set(word_end);
   }
}

/*===============================================================
 *
 * auto_train
 *
 *==============================================================*/

void auto_train(const string &sOutputFile, const string &sFeatureFile, const unsigned long &nBest, const unsigned long &nMaxSentSize, const string &sKnowledgePath, const bool &bFWCDRule) {
   static CCharCatDictionary charcat ; // don't know why there is a segmentation fault when this is put as a global variable. The error happens when charcat.h CCharcat() is called and in particular when (*this)[CWord(letters[i])] = eFW is executed (if an empty CWord(letters[i]) line is put before this line then everything is okay. Is it static initialization fiasco? Not sure really.

   CTagger decoder(sFeatureFile, true, nMaxSentSize, sKnowledgePath, bFWCDRule);
   CSentenceReader output_reader(sOutputFile);
   CStringVector *input_sent = new CStringVector;
   CTwoStringVector *output_sent = new CTwoStringVector; 

   unsigned nCount=0;
   unsigned nErrorCount=0;

   CBitArray word_ends(MAX_SENTENCE_SIZE);

   //
   // Read the next sentence
   //
   while( output_reader.readTaggedSentence(output_sent) ) {
      if (bFWCDRule)
         UntagAndDesegmentSentence(output_sent, input_sent, charcat);
      else
         UntagAndDesegmentSentence(output_sent, input_sent);
      TRACE("Sentence " << nCount);
      ++nCount;
      //
      // Find the decoder output
      //
      if (decoder.train(input_sent, output_sent)) ++nErrorCount;
   }
   delete input_sent;
   delete output_sent;
   cout << "Completing the training process." << endl;
   decoder.finishTraining(nCount);
   cout << "Done. Total errors: " << nErrorCount << endl;
}

/*===============================================================
 *
 * train
 *
 *==============================================================*/

void train(const string &sOutputFile, const string &sFeatureFile, const unsigned long &nBest, const unsigned long &nMaxSentSize, const bool &bEarlyUpdate, const bool &bSegmented, const string &sKnowledgePath, const bool &bFWCDRule) {
   static CCharCatDictionary charcat ; // don't know why there is a segmentation fault when this is put as a global variable. The error happens when charcat.h CCharcat() is called and in particular when (*this)[CWord(letters[i])] = eFW is executed (if an empty CWord(letters[i]) line is put before this line then everything is okay. Is it static initialization fiasco? Not sure really.

   CTagger decoder(sFeatureFile, true, nMaxSentSize, sKnowledgePath, bFWCDRule);
   CSentenceReader output_reader(sOutputFile);
#ifdef DEBUG
   CSentenceWriter output_writer("");
   CSentenceWriter tagged_writer("");
#endif
   CStringVector *input_sent = new CStringVector;
   CTwoStringVector *tagged_sent = new CTwoStringVector[nBest];
   CTwoStringVector *output_sent = new CTwoStringVector; 

   unsigned nCount=0;
   unsigned nErrorCount=0;
   unsigned nEarlyUpdateRepeat=0;

   CBitArray word_ends(MAX_SENTENCE_SIZE);

   //
   // Read the next sentence
   //
   while( output_reader.readTaggedSentence(output_sent) ) {
      if (bFWCDRule)
         UntagAndDesegmentSentence(output_sent, input_sent, charcat);
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
            for (unsigned i=0; i<nBest; ++i) if (*(tagged_sent+i)!=*output_sent)
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
         for (unsigned i=0; i<nBest; ++i) 
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
      configurations.defineConfiguration("r", "", "do not use rules to segment numbers and letters", "");

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
      bool bFWCDRule = configurations.getConfiguration("r").empty() ? true : false;
      string sKnowledgePath = configurations.getConfiguration("k");
      bool bSegmented = false;
#ifdef SEGMENTED
      bSegmented = true; // compile option
#endif

      unsigned training_rounds;
      if (!fromString(training_rounds, options.args[3])) {
         cerr << "Error: the number of training iterations must be an integer." << endl;
         return 1;
      }
      cout << "Training started." << endl;
      unsigned time_start = clock();
      if (bSegmented) {
         // the first iteration: load knowledge
         train(argv[1], argv[2], nBest, nMaxSentSize, bEarlyUpdate, bSegmented, sKnowledgePath, bFWCDRule);
         // from the next iteration knowledge will be loaded from the model
         // and therefore sKnowledgePath is set to "". Thus separate 'for'
         for (unsigned i=1; i<training_rounds; ++i)
            train(argv[1], argv[2], nBest, nMaxSentSize, bEarlyUpdate, bSegmented, "", bFWCDRule);
      }
      else {
         // the first iteration: load knowledge
         auto_train(argv[1], argv[2], nBest, nMaxSentSize, sKnowledgePath, bFWCDRule);
         // from the next iteration knowledge will be loaded from the model
         // and therefore sKnowledgePath is set to "". Thus separate 'for'
         for (unsigned i=1; i<training_rounds; ++i)
            auto_train(argv[1], argv[2], nBest, nMaxSentSize, "", bFWCDRule);
      }
      cout << "Training has finished successfully. Total time taken is: " << double(clock()-time_start)/CLOCKS_PER_SEC << endl;
      return 0;
   } catch (const string &e) {
      cerr << "Error: " << e << endl;
      return 1;
   }
}

