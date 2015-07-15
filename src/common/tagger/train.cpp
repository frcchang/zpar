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
#include "utils.h"
#include "tagger.h"
#include "reader.h"
#include "writer.h"
#include "options.h"

using namespace TARGET_LANGUAGE;

const int TRAINING_ROUND = 1;

/*===============================================================
 *
 * auto_train
 *
 *==============================================================*/

void auto_train(std::string sOutputFile, std::string sFeatureFile, const std::string &sTagDict, const std::string &sKnowledge) {
   CTagger decoder(sFeatureFile, true);
   if (sTagDict.size())
      decoder.loadTagDictionary(sTagDict);
   if (sKnowledge.size())
      decoder.loadKnowledge(sKnowledge);
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
   std::cerr << "Completing the training process" << std::endl;
   decoder.finishTraining();
   std::cerr << "Done. Total errors: " << nErrorCount << std::endl;
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
      configurations.defineConfiguration("d", "Path", "use dictionary from the given path", "");
      configurations.defineConfiguration("k", "Path", "use special knowledge from the given path", "");

      if (options.args.size() != 4) {
         std::cout << "\nUsage: " << argv[0] << " training_data model num_iterations" << std::endl ;
         std::cout << configurations.message() << std::endl;
         return 1;
      }
      std::string warning = configurations.loadConfigurations(options.opts);
      if (!warning.empty()) {
         std::cout << "Warning: " << warning << std::endl;
      }

      std::string sTagDict = configurations.getConfiguration("d");
      std::string sKnowledge = configurations.getConfiguration("k");

      unsigned training_rounds;
      if (!fromString(training_rounds, options.args[3])) {
         std::cout << "Error: the number of training iterations must be an integer." << std::endl;
         return 1;
      }
      std::cout << "Training started" << std::endl;
      int time_start = clock();
      for (unsigned i=0; i<training_rounds; ++i)
         auto_train(argv[1], argv[2], sTagDict, sKnowledge);
      std::cout << "Training has finished successfully. Total time taken is: " << double(clock()-time_start)/CLOCKS_PER_SEC << std::endl;
      return 0;
   } catch (const std::string &e) {
      std::cerr << "Error: " << e << std::endl;
      return 1;
   }
}

