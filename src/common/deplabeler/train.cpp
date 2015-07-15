// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                              *
 * train.cpp - the training of the general dependency labeler.  *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2007.8                         *
 *                                                              *
 ****************************************************************/

#include "definitions.h"
#include "deplabeler.h"
#include "reader.h"
#include "writer.h"

using namespace TARGET_LANGUAGE;

/*===============================================================
 *
 * auto_train - train by the labeler itself, black-box training
 *
 *===============================================================*/

void auto_train(const std::string &sOutputFile, const std::string &sFeatureFile, const bool bCoNLL) {

   std::cerr << "Training iteration is started..." << std::endl ; std::cerr.flush();

   CDepLabeler labeler(sFeatureFile, true);

   std::ifstream is(sOutputFile.c_str());
   assert(is.is_open());

   CLabeledDependencyTree ref_sent;
   CCoNLLOutput ref_conll;

   unsigned long nCount=0;

//   if (bCoNLL) is >> ref_conll; else is >> ref_sent;
   while( (bCoNLL?(is>>ref_conll):(is>>ref_sent)) ) {
      TRACE("Sentence " << nCount);
      ++ nCount ;
      if (bCoNLL)
         labeler.train_conll( ref_conll );
      else
         labeler.train( ref_sent );
//      if (bCoNLL) is >> ref_conll; else is >> ref_sent;
   }

   labeler.finishtraining();

   std::cerr << "Done. " << std::endl;

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
      configurations.defineConfiguration("c", "", "process CoNLL format", "");
      if (options.args.size() != 4) {
         std::cout << "\nUsage: " << argv[0] << " training_data model num_iterations" << std::endl ;
         std::cout << configurations.message() << std::endl;
         return 1;
      }
      configurations.loadConfigurations(options.opts);

      unsigned long training_rounds;
      if (!fromString(training_rounds, options.args[3])) {
         std::cout << "Error: the number of training iterations must be an integer." << std::endl;
         return 1;
      }

      bool bCoNLL = configurations.getConfiguration("c").empty() ? false : true;

      std::cerr << "Training started" << std::endl;
      int time_start = clock();
      for (int i=0; i<training_rounds; ++i)
         auto_train(options.args[1], options.args[2], bCoNLL);
      std::cerr << "Training has finished successfully. Total time taken is: " << double(clock()-time_start)/CLOCKS_PER_SEC << std::endl;

      return 0;
   } catch (const std::string &e) {
      std::cerr << "Error: " << e << std::endl;
      return 1;
   }

}

