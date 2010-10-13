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

#include <cstring>

using namespace TARGET_LANGUAGE;

/*===============================================================
 *
 * auto_train - train by the labeler itself, black-box training
 *
 *===============================================================*/

void auto_train(const string &sOutputFile, const string &sFeatureFile) {

   cout << "Training iteration is started..." << endl ; cout.flush();

   CDepLabeler labeler(sFeatureFile, true);

   ifstream is(sOutputFile.c_str());
   assert(is.is_open());

   CLabeledDependencyTree ref_sent; 

   unsigned long nCount=0;
   
   is >> ref_sent;
   while( ! ref_sent.empty() ) {
      TRACE("Sentence " << nCount);
      ++ nCount ; 
      labeler.train( ref_sent );
      is >> ref_sent;
   }

   labeler.finishtraining();

   cout << "Done. " << endl;

}

/*===============================================================
 *
 * main
 *
 *==============================================================*/

int main(int argc, char* argv[]) {

   try {
      COptions options(argc, argv);
      if (options.args.size() != 4) {
         cout << "\nUsage: " << argv[0] << " training_data model num_iterations" << endl ;
         return 1;
      } 
   
      unsigned long training_rounds;
      if (!fromString(training_rounds, options.args[3])) {
         cerr << "Error: the number of training iterations must be an integer." << endl;
         return 1;
      }
   
      cout << "Training started" << endl;
      int time_start = clock();
      for (int i=0; i<training_rounds; ++i) 
         auto_train(argv[1], argv[2]);
      cout << "Training has finished successfully. Total time taken is: " << double(clock()-time_start)/CLOCKS_PER_SEC << endl;
   
      return 0;
   } catch (const string &e) {
      cerr << "Error: " << e << endl;
      return 1;
   }

}

