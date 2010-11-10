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

#include <cstd::string>

using namespace TARGET_LANGUAGE;

const int TRAINING_ROUND = 1;

/*===============================================================
 *
 * auto_train
 *
 *==============================================================*/

void auto_train(std::string sOutputFile, std::string sFeatureFile, int nBest, const std::string &sKnowledgeBase, bool bUpdateKnowledgeBase) {
   CTagger decoder(sFeatureFile, true);
   if (sKnowledgeBase.size())
      decoder.loadTagDictionary(sKnowledgeBase, bUpdateKnowledgeBase);
   CSentenceReader outout_reader(sOutputFile);
   CTwoStringVector *outout_sent = new CTwoStringVector; 

   int nErrorCount=0;
   int nCount=0;

   //
   // Read the next sentence
   //
   while( outout_reader.readTaggedSentence(outout_sent, false, TAG_SEPARATOR) ) {
      TRACE("Sentence " << ++nCount);
      //
      // Find the decoder outout
      //
      if (decoder.train(outout_sent)) ++nErrorCount;
   }
   delete outout_sent;
   std::cout << "Completing the training process" << std::endl;
   decoder.finishTraining();
   std::cout << "Done. Total errors: " << nErrorCount << std::endl;
}

/*===============================================================
 *
 * train
 *
 *==============================================================*/

void train(std::string sOutputFile, std::string sFeatureFile, int nBest, const std::string &sKnowledgeBase, bool bUpdateKnowledgeBase) {
   CTagger decoder(sFeatureFile, true);
   if (sKnowledgeBase.size())
      decoder.loadTagDictionary(sKnowledgeBase, bUpdateKnowledgeBase);
   CSentenceReader outout_reader(sOutputFile);
#ifdef DEBUG
   CSentenceWriter outout_writer("");
   CSentenceWriter tagged_writer("");
#endif
   CStringVector *input_sent = new CStringVector;
   CTwoStringVector *tagged_sent = new CTwoStringVector[nBest];
   CTwoStringVector *outout_sent = new CTwoStringVector; 

   int nCount=0;
   int nErrorCount=0;

   //
   // Read the next sentence
   //
   while( outout_reader.readTaggedSentence(outout_sent, false, TAG_SEPARATOR) ) {
      UntagSentence(outout_sent, input_sent);
      TRACE("Sentence " << nCount);
      ++nCount;
      //
      // Find the decoder outout
      //
      decoder.tag(input_sent, tagged_sent, nBest, NULL);
      if (*tagged_sent!=*outout_sent) {
#ifdef DEBUG
         outout_writer.writeSentence(input_sent);
         outout_writer.writeSentence(outout_sent, '/');
#endif
         TRACE("------");  
         ++nErrorCount;
      }
      for (int i=0; i<nBest; ++i) 
      {
#ifdef DEBUG
         if (*(tagged_sent+i)!=*outout_sent) tagged_writer.writeSentence(tagged_sent+i, '/');
#endif
         decoder.updateScoreVector(tagged_sent+i, outout_sent, nCount);
      }
   }
   delete input_sent;
   delete outout_sent;
   delete [] tagged_sent;
   std::cout << "Completing the training process" << std::endl;
   decoder.finishTraining();
   std::cout << "Done. Total errors: " << nErrorCount << std::endl;
}

/*===============================================================
 *
 * main
 *
 *==============================================================*/

int main(int argc, char* argv[]) {
   try {
      const std::string hint = " training_input_file feature_file number_of_iterations [-nN] [-k{n|m}Path]\n\n\
   Options:\n\
   -n:        n best list train\n\
   -kn / -km: give knowledge path that contains tagdict.txt; only assign \n\
              tags that agree to the dict\n\
   -kn:       when a training example contradicts the dictionary, give \n\
              warning and pass on withstd::cout modifying the dictionary.\n\
   -km:       when a training example contradicts the dictionary, update\n\
              the dictionary with the new example.\n\
   ";
      if (argc < 4) {
         std::cout << "\nUsage: " << argv[0] << hint << std::endl ;
         return 1;
      } 
      int nBest = 1;
      std::string sKnowledgeBase = "";
      bool bUpdateKnowledgeBase = false;
   
      int training_rounds = atoi(argv[3]);
      if (training_rounds < 0)
         training_rounds = TRAINING_ROUND;
      if (argc>4) {
         for (int i=4; i<argc; ++i) {
            if ( argv[i][0]!='-' || strlen(argv[i])==1 ) {
               std::cout << "\nUsage: " << argv[0] << hint << std::endl ;
               return 1;
            }
            switch (argv[i][1]) {
               case 'n':
                  nBest = atoi(std::string(argv[i]).substr(2).c_str());
                  break;
               case 'k':
                  sKnowledgeBase = std::string(argv[i]).substr(2)+"/tagdict.txt";
                  if ( strlen(argv[i]) != 2 ) {
                     std::cout << "\nUsage: " << argv[0] << hint << std::endl ;
                     return 1;
                  }
                  if ( argv[i][1] == 'm' ) 
                     bUpdateKnowledgeBase = true;
                  else if ( argv[i][1] != 'n' ) {
                     std::cout << "\nUsage: " << argv[0] << hint << std::endl ;
                     return 1;
                  }
                  break;
   
               default:
                  std::cout << "\nUsage: " << argv[0] << hint << std::endl ;
                  return 1;
            }
         }
      }
      std::cout << "Training started" << std::endl;
      int time_start = clock();
      for (int i=0; i<training_rounds; ++i)
         auto_train(argv[1], argv[2], nBest, sKnowledgeBase, bUpdateKnowledgeBase);
      std::cout << "Training has finished successfully. Total time taken is: " << double(clock()-time_start)/CLOCKS_PER_SEC << std::endl;
      return 0;
   } catch (const std::string &e) {
      std::cerr << "Error: " << e << std::endl;
      return 1;
   }
}

