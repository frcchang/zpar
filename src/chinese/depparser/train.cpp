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

using namespace chinese;

static string sKnowledgePath;

/*===============================================================
 *
 * auto_train - train by the parser itself, black-box training
 *
 *===============================================================*/

void auto_train(const string &sOutputFile, const string &sFeatureFile, const bool bKnowledge) {

   cout << "Training iteration is started..." << endl ; cout.flush();

   CDepParser parser(sFeatureFile, true);

   ifstream is(sOutputFile.c_str());
   assert(is.is_open());

   CDependencyTree ref_sent; 

   int nCount=0;
   
   is >> ref_sent;
   while( ! ref_sent.empty() ) {
      TRACE("Sentence " << nCount);
      ++ nCount ; 
      parser.train( ref_sent, nCount );
      is >> ref_sent;
   }

   parser.finishtraining();

   cout << "Done. " << endl;

}

/*===============================================================
 *
 * main
 *
 *==============================================================*/

int main(int argc, char* argv[]) {

   const string hint = " training_input_file feature_file number_of_iterations [-kPATH]\n\
Arguments:\n\
-k use knowledge form the given path\n\
";

   if (argc < 4) {
      cout << "\nUsage: " << argv[0] << hint << endl;
      return 1;
   }

   bool bKnowledge = false;

   if (argc>4) {
      for (int i=4; i<argc; ++i) {
         if (argv[i][0]!='-') { cout << "\nUsage: " << argv[0] << hint << endl ; return 1; }
         switch (argv[i][1]) {
            case 'k':
               bKnowledge = true;
               sKnowledgePath = string(argv[i]).substr(2);
               break;
            default:
               cout << "\nUsage: " << argv[0] << hint << endl ;
               return 1;
         }
      }
   }

   int training_rounds = atoi(argv[3]);
   if (training_rounds < 1) {
      cout << "\nThe number of training iterations is incorrect. " << endl;
      return 1;
   }

   cout << "Training started" << endl;
   int time_start = clock();
   for (int i=0; i<training_rounds; ++i) 
      auto_train(argv[1], argv[2], bKnowledge);
   cout << "Training has finished successfully. Total time taken is: " << double(clock()-time_start)/CLOCKS_PER_SEC << endl;

   return 0;

}

