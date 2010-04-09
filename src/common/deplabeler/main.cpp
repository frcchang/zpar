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

void process(const string sInputFile, const string sOutputFile, const string sFeatureFile) {

   cout << "Labeling started" << endl;

   int time_start = clock();

   CDepLabeler labeler(sFeatureFile, false) ;
   ofstream is(sInputFile.c_str());
   assert(is.is_open());
   ofstream os(sOutputFile.c_str());
   assert(os.is_open());
   CDependencyTree input_sent;
   CLabeledDependencyTree output_sent; 

   int nCount=0;
   bool bReadSuccessful;

   // Read the next example
   while( is >> input_sent ) {

      TRACE("Sentence " << nCount);
      ++ nCount;

      // Find decoder output
      labeler.label( input_sent , &output_sent ) ;
      
      // Ouptut sent
      os << output_sent ;
      }
      
      // Read the next example
   }

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
      // check arguments
      if (options.args.size() != 4) {
         cout << "Usage: " << argv[0] << " input_file output_file model_file" << endl;
         cout << configurations.message() << endl;
         return 1;
      }
      configurations.loadConfigurations(options.opts);
   
      process(argv[1], argv[2], argv[3]);
      return 0;
   } catch (const string &e) {
      cerr << "Error: " << e << endl;
      return 1;
   }
}

