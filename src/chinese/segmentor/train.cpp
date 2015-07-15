// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                              *
 * train.cpp - the training of the Chinese segmentor.           *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2006.10                        *
 *                                                              *
 ****************************************************************/

#include "segmentor.h"
#include "reader.h"
#include "writer.h"
#include "file_utils.h"
#include "options.h"

using namespace chinese;

const int TRAINING_ROUND = 1;

#ifdef NO_NEG_FEATURE

/*===============================================================
 *
 * extract_features - extract features from a training file. used only when no negative feat
 *
 *===============================================================*/

void extract_features(const std::string &sTextFile, const std::string &sFeatureFile) {
   if (FileExists(sFeatureFile))
      return;
   std::cerr << "Extracting features...";
   CSegmentor *segmentor;
   segmentor = new CSegmentor(sFeatureFile, true);
   CStringVector *ref_sent = new CStringVector;
   CSentenceReader ref_reader(sTextFile);

   while( ref_reader.readSegmentedSentence(ref_sent) ) {
      segmentor->extractPosFeatures(ref_sent);
   }
   segmentor->finishTraining(0);
   delete segmentor;
   delete ref_sent;
   std::cerr << "done" << std::endl;
}

#endif

/*===============================================================
 *
 * auto_train - train by the segmentor itself, black-box training
 *
 *===============================================================*/

void auto_train(const std::string &sOutputFile, const std::string &sFeatureFile, const bool &bNoFWAndCD, const std::string &sCharCatFile, const std::string &sLexiconDict) {
   CSegmentor *segmentor;
   segmentor = new CSegmentor(sFeatureFile, true, sCharCatFile, sLexiconDict, !bNoFWAndCD);
   CSentenceReader ref_reader(sOutputFile);
   CStringVector *input_sent = new CStringVector;
   CStringVector *ref_sent = new CStringVector;
   CCharCatDictionary char_information;

   int nCount=0;

   while( ref_reader.readSegmentedSentence(ref_sent) ) {
      if ( ref_sent->empty() ) continue;
      if (bNoFWAndCD)
         DesegmentSentence(ref_sent, input_sent);
      else
         DesegmentSentence(ref_sent, input_sent, char_information);
      TRACE("Sentence " << nCount);
      ++nCount;
      segmentor->train(input_sent, ref_sent, nCount);
   }

   segmentor->finishTraining(nCount);
   delete input_sent;
   delete ref_sent;
   delete segmentor;
   std::cerr << "Done. " << std::endl;
}

/*===============================================================
 *
 * train
 *
 *==============================================================*/

void train(const std::string &sOutputFile, const std::string &sFeatureFile, const bool &bAggressive, const bool &bNoFWAndCD, const std::string &sCharCatFile, const std::string &sLexiconDict) {
   CSegmentor *segmentor ;
   segmentor = new CSegmentor(sFeatureFile, true, sCharCatFile, sLexiconDict, !bNoFWAndCD);
   CSentenceReader ref_reader(sOutputFile);
#ifdef DEBUG
   CSentenceWriter ref_writer("");
   CSentenceWriter output_writer("");
#endif
   CStringVector *input_sent = new CStringVector;
   CStringVector *ref_sent = new CStringVector;
   CStringVector *output_sent = new CStringVector;
   CCharCatDictionary char_information;

   int nCount=0;
   int nErrorCount=0;

   while( ref_reader.readSegmentedSentence(ref_sent) ) {
      if ( ref_sent->empty() ) continue ;
      if ( bNoFWAndCD )
         DesegmentSentence( ref_sent, input_sent );
      else
         DesegmentSentence( ref_sent, input_sent, char_information );
      TRACE("Sentence " << nCount);
      ++nCount;
      segmentor->segment( input_sent, output_sent );
      if (output_sent->empty()) continue;
#ifdef DEBUG
      if ( *output_sent != *ref_sent ) {
         std::cerr << "correct" << std::endl;
         ref_writer.writeSentence(ref_sent);
         std::cerr << "output" << std::endl;
         output_writer.writeSentence(output_sent);
      }
#endif
      segmentor->updateScores( output_sent, ref_sent, nCount );
      if ( *output_sent != *ref_sent ) {
         ++nErrorCount;
         if (bAggressive) {
            while (true) {
               segmentor->segment(input_sent, output_sent);
               if (*output_sent != *ref_sent) {
                  segmentor->updateScores(output_sent, ref_sent, nCount); continue;
               }
               break;
            }
         }
      }
   }
   delete input_sent;
   delete ref_sent;
   delete output_sent;

   segmentor->finishTraining(nCount);
   std::cerr << "Done. Total errors: " << nErrorCount << std::endl;

   delete segmentor;
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
      configurations.defineConfiguration("c", "Path", "provide character type info in Path", "");
      configurations.defineConfiguration("w", "Path", "privide word list in Path", "");
      configurations.defineConfiguration("r", "", "use rules to segment English letters and Arabic numbers", "");
      if (options.args.size() != 4) {
         std::cout << "Usage: " << argv[0] << " training_data model_file iterations" << std::endl;
         std::cout << configurations.message() << std::endl;
         return 1;
      }
      unsigned long training_rounds;
      if (!fromString(training_rounds, options.args[3])) {
         std::cout << "The number of training iterations must be an integer." << std::endl;
         return 1;
      }

      std::string warning = configurations.loadConfigurations(options.opts);
      if (!warning.empty()) {
         std::cout << "Warning: " << warning << std::endl;
      }

      bool bNoFWAndCD = configurations.getConfiguration("r").empty() ? true : false;
      std::string sCharCatFile = configurations.getConfiguration("c");
      std::string sLexiconDict = configurations.getConfiguration("w");

      std::cerr << "Training started ..." << std::endl;
      int time_start = clock();

#ifdef NO_NEG_FEATURE
      extract_features(options.args[1], options.args[2]);
#endif

      for (int i=0; i<training_rounds; ++i) {
#ifdef SELF_TRAIN
            auto_train(options.args[1], options.args[2], bNoFWAndCD, sCharCatFile, sLexiconDict);
#else
            train(options.args[1], options.args[2], false, bNoFWAndCD, sCharCatFile, sLexiconDict);
#endif
         if (i==0) { // from the next iteration these will be loaded from the db
            sCharCatFile = "";
            sLexiconDict = "";
         }
      }
      std::cerr << "Training has finished successfully. Total time taken is: " << double(clock()-time_start)/CLOCKS_PER_SEC << std::endl;
      return 0;
   }
   catch(const std::string &e) {
      std::cerr << "Error: " << e << " Stop." << std::endl;
      exit(1);
   }
}

