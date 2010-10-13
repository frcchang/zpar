// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                              *
 * train.cpp - the training of the common tagger.               *
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

#include <cstring>

using namespace TARGET_LANGUAGE;

const int TRAINING_ROUND = 1;

/*===============================================================
 *
 * auto_train
 *
 *==============================================================*/

void auto_train(string sOutputFile, string sFeatureFile, int nBest, const string &sKnowledgeBase, bool bUpdateKnowledgeBase) {
   CTagger decoder(sFeatureFile, true);
   if (sKnowledgeBase.size())
      decoder.loadTagDictionary(sKnowledgeBase, bUpdateKnowledgeBase);
   CSentenceReader output_reader(sOutputFile);
   CTwoStringVector *output_sent = new CTwoStringVector; 

   int nErrorCount=0;
   int nCount=0;

   //
   // Read the next sentence
   //
   while( output_reader.readTaggedSentence(output_sent, false, TAG_SEPARATOR) ) {
      TRACE("Sentence " << ++nCount);
      //
      // Find the decoder output
      //
      if (decoder.train(output_sent)) ++nErrorCount;
   }
   delete output_sent;
   cout << "Completing the training process" << endl;
   decoder.finishTraining();
   cout << "Done. Total errors: " << nErrorCount << endl;
}

/*===============================================================
 *
 * train
 *
 *==============================================================*/

void train(string sOutputFile, string sFeatureFile, int nBest, const string &sKnowledgeBase, bool bUpdateKnowledgeBase) {
   CTagger decoder(sFeatureFile, true);
   if (sKnowledgeBase.size())
      decoder.loadTagDictionary(sKnowledgeBase, bUpdateKnowledgeBase);
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

   //
   // Read the next sentence
   //
   while( output_reader.readTaggedSentence(output_sent, false, TAG_SEPARATOR) ) {
      UntagSentence(output_sent, input_sent);
      TRACE("Sentence " << nCount);
      ++nCount;
      //
      // Find the decoder output
      //
      decoder.tag(input_sent, tagged_sent, nBest, NULL);
      if (*tagged_sent!=*output_sent) {
#ifdef DEBUG
         output_writer.writeSentence(input_sent);
         output_writer.writeSentence(output_sent, '/');
#endif
         TRACE("------");  
         ++nErrorCount;
      }
      for (int i=0; i<nBest; ++i) 
      {
#ifdef DEBUG
         if (*(tagged_sent+i)!=*output_sent) tagged_writer.writeSentence(tagged_sent+i, '/');
#endif
         decoder.updateScoreVector(tagged_sent+i, output_sent, nCount);
      }
   }
   delete input_sent;
   delete output_sent;
   delete [] tagged_sent;
   cout << "Completing the training process" << endl;
   decoder.finishTraining();
   cout << "Done. Total errors: " << nErrorCount << endl;
}

/*===============================================================
 *
 * main
 *
 *==============================================================*/

int main(int argc, char* argv[]) {
   try {
      const string hint = " training_input_file feature_file number_of_iterations [-nN] [-k{n|m}Path]\n\n\
   Options:\n\
   -n:        n best list train\n\
   -kn / -km: give knowledge path that contains tagdict.txt; only assign \n\
              tags that agree to the dict\n\
   -kn:       when a training example contradicts the dictionary, give \n\
              warning and pass on without modifying the dictionary.\n\
   -km:       when a training example contradicts the dictionary, update\n\
              the dictionary with the new example.\n\
   ";
      if (argc < 4) {
         cout << "\nUsage: " << argv[0] << hint << endl ;
         return 1;
      } 
      int nBest = 1;
      string sKnowledgeBase = "";
      bool bUpdateKnowledgeBase = false;
   
      int training_rounds = atoi(argv[3]);
      if (training_rounds < 0)
         training_rounds = TRAINING_ROUND;
      if (argc>4) {
         for (int i=4; i<argc; ++i) {
            if ( argv[i][0]!='-' || strlen(argv[i])==1 ) {
               cout << "\nUsage: " << argv[0] << hint << endl ;
               return 1;
            }
            switch (argv[i][1]) {
               case 'n':
                  nBest = atoi(string(argv[i]).substr(2).c_str());
                  break;
               case 'k':
                  sKnowledgeBase = string(argv[i]).substr(2)+"/tagdict.txt";
                  if ( strlen(argv[i]) != 2 ) {
                     cout << "\nUsage: " << argv[0] << hint << endl ;
                     return 1;
                  }
                  if ( argv[i][1] == 'm' ) 
                     bUpdateKnowledgeBase = true;
                  else if ( argv[i][1] != 'n' ) {
                     cout << "\nUsage: " << argv[0] << hint << endl ;
                     return 1;
                  }
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
         auto_train(argv[1], argv[2], nBest, sKnowledgeBase, bUpdateKnowledgeBase);
      cout << "Training has finished successfully. Total time taken is: " << double(clock()-time_start)/CLOCKS_PER_SEC << endl;
      return 0;
   } catch (const string &e) {
      cerr << "Error: " << e << endl;
      return 1;
   }
}

