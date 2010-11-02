// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                              *
 * main.cpp - main app of the general dependency labeler.       *
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
#include "stdlib.h"

using namespace TARGET_LANGUAGE;

/*===============================================================
 *
 * decode
 *
 *==============================================================*/

void process(const string sInputFile, const string sOutputFile, const string sFeatureFile, bool bCoNLL) {

   cout << "Labeling started" << endl;

   clock_t time_start = clock();

   CDepLabeler labeler(sFeatureFile, false) ;
   ifstream is(sInputFile.c_str());
   assert(is.is_open());
   ofstream os(sOutputFile.c_str());
   assert(os.is_open());
   CDependencyTree input_sent;
   CLabeledDependencyTree output_sent; 
   CCoNLLOutput input_conll;
   CCoNLLOutput output_conll;

   int nCount=0;

   // Read the next example
   while( (bCoNLL ? (is >> input_conll) : (is >> input_sent)) ) {

      TRACE("Sentence " << nCount);
      ++ nCount;

      // Find decoder output
      if (bCoNLL) {
         labeler.label_conll( input_conll , &output_conll ) ;
         os << output_conll; 
      }
      else {
         labeler.label( input_sent , &output_sent ) ;
	 os << output_sent ;
      }
      
      // Ouptut sent
   }

   is.close();
   os.close();

   cout << "Labeling has finished successfully. Total time taken is: " << double(clock()-time_start)/CLOCKS_PER_SEC << endl;
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
      // check arguments
      if (options.args.size() != 4) {
         cout << "Usage: " << argv[0] << " input_file output_file model_file" << endl;
         cout << configurations.message() << endl;
         return 1;
      }
      configurations.loadConfigurations(options.opts);

      bool bCoNLL = configurations.getConfiguration("c").empty() ? false : true;
   
      process(options.args[1], options.args[2], options.args[3], bCoNLL);
      return 0;
   } catch (const string &e) {
      cerr << "Error: " << e << endl;
      return 1;
   }
}

