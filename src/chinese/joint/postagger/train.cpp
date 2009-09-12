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
   CTwoStringVector correct_sent; 

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
   try {
      COptions options(argc, argv);
      CConfigurations configurations;
      configurations.defineConfiguration("n", "N", "N best list rerank", "10");
      configurations.defineConfiguration("s", "", "train segmentor and postagger independently", "");
      // check arguments
      if (options.args.size() != 4) {
         cout << "Usage: " << argv[0] << " input_file output_file model_file" << endl;
         cout << configurations.message() << endl;
         return 1;
      }
      configurations.loadConfigurations(options.opts);
   
      int nBest;
      if (!fromString(nBest, configurations.getConfiguration("n"))) {
         cerr<<"Error: N must be integer."<<endl; return 1;
      }
      bool bSeparateTrain = configurations.getConfiguration("s").empty() ? false: true;
   
      for (int i=0; i<atoi(argv[3]); ++i)
         train(argv[1], argv[2], nBest, bSeparateTrain);
      return 0;
   } catch (const string &e) {
      cerr << e << endl;
      return 1;
   }
}

