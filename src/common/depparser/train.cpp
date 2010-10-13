// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                              *
 * train.cpp - the training of the general dependency parser.   *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2007.8                         *
 *                                                              *
 ****************************************************************/

#include "definitions.h"
#include "depparser.h"
#include "reader.h"
#include "writer.h"

#include <cstring>

using namespace TARGET_LANGUAGE;

/*===============================================================
 *
 * auto_train - train by the parser itself, black-box training
 *
 *===============================================================*/

void auto_train(const string &sOutputFile, const string &sFeatureFile, const bool &bRules, const string &sSuperPath) {

   cout << "Training iteration is started..." << endl ; cout.flush();

   CDepParser parser(sFeatureFile, true);
   parser.setRules(bRules);

   ifstream is(sOutputFile.c_str());
   assert(is.is_open());

   CDependencyParse ref_sent; 

   depparser::CSuperTag *supertags;
   ifstream *is_supertags;

   supertags = 0;
   if (!sSuperPath.empty()) {
      supertags = new depparser::CSuperTag();
      is_supertags = new ifstream(sSuperPath.c_str());
      parser.setSuperTags(supertags);
   }

   int nCount=0;
   
   is >> ref_sent;
   while( ! ref_sent.empty() ) {
      TRACE("Sentence " << nCount);
      ++ nCount ; 

      if (supertags) {
         supertags->setSentenceSize( ref_sent.size() );
         (*is_supertags) >> *supertags;
      }

      parser.train( ref_sent, nCount );
      is >> ref_sent;
   }

   parser.finishtraining();

   if (supertags) {
      delete supertags;
      is_supertags->close();
      delete is_supertags;
   }

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
      CConfigurations configurations;
      configurations.defineConfiguration("p", "path", "supertags", "");
      configurations.defineConfiguration("r", "", "use rules", "");
      if (options.args.size() != 4) {
         cout << "\nUsage: " << argv[0] << " training_data model num_iterations" << endl ;
         return 1;
      } 
      configurations.loadConfigurations(options.opts);
   
      int training_rounds;
      if (!fromString(training_rounds, options.args[3])) {
         cerr << "Error: the number of training iterations must be an integer." << endl;
         return 1;
      }
   
      string sSuperPath = configurations.getConfiguration("p");
      bool bRules = configurations.getConfiguration("r").empty() ? false : true;

      cout << "Training started" << endl;
      int time_start = clock();
      for (int i=0; i<training_rounds; ++i) 
         auto_train(argv[1], argv[2], bRules, sSuperPath);
      cout << "Training has finished successfully. Total time taken is: " << double(clock()-time_start)/CLOCKS_PER_SEC << endl;
   
      return 0;
   } catch (const string &e) {
      cerr << "Error: " << e << endl;
      return 1;
   }

}

