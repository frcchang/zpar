/**************************************************************** 
 *                                                              * 
 * tagger_train.cpp - the training for the joint tagger.        * 
 *                                                              * 
 * The module does decoding via combining the best output for a * 
 * segmentor with a tagger.                                     * 
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2006.10                        *
 *                                                              *
 ****************************************************************/

#include "reader.h"
#include "writer.h"

#include "reranker.h"

using namespace chinese;

/*----------------------------------------------------------------
 *
 * train - the training process 
 * 
 *---------------------------------------------------------------*/

void train(string sInputFile, string sFeatureDB, int nBest, bool bSeparateTrain) {
   TRACE("Training started");
   int time_start = clock();
   CReranker reranker(sFeatureDB, nBest, true);
   CSentenceReader input_reader(sInputFile);
   CSentenceTagged correct_sent; 

   int nCount=0;
   int nErrorCount=0;
   
   input_reader.readTaggedSentence(&correct_sent);
   while( !(correct_sent.empty()) ) {
      cout << "Sentence " << ++nCount << endl;
      if (bSeparateTrain)
         reranker.train_separate(correct_sent);
      else
         reranker.train(correct_sent);
      input_reader.readTaggedSentence(&correct_sent);
   }
   reranker.finishTraining();
   TRACE("Training has finished successfully. Total time taken is: " << double(clock()-time_start)/CLOCKS_PER_SEC);
}

/*===============================================================
 *
 * main
 *
 *==============================================================*/

int main(int argc, char* argv[]) {
   const string hint = " training_data feature_file numer_of_iterations [-nN] [-s]\n\n\
Options:\n\
-n N best segmentor output; default 10.\n\
-s train the segmentor and tagger separately.\n\
";
   if (argc < 4) {
      cout << "Usage: " << argv[0] << hint << endl;
      return 1;
   }
   int nBest = 10;
   bool bSeparateTrain = false;

   if (argc>4) {
      for (int i=4; i<argc; i++) {
         if (argv[i][0]!='-') { cout << "\nUsage: " << argv[0] << hint << endl ; return 1; }
         switch (argv[i][1]) {
            case 'n':
               if (strlen(argv[i])<3) { cout << "\nUsage: " << argv[0] << hint << endl ; return 1; }
               nBest = atoi(string(argv[i]+2).c_str());
               break;
            case 's':
               bSeparateTrain = true;
               break;
            default:
               cout << "\nUsage: " << argv[0] << hint << endl ;
               return 1;
         }
      }
   }
   for (int i=0; i<atoi(argv[3]); i++)
      train(argv[1], argv[2], nBest, bSeparateTrain);
   return 0;
}

