/**************************************************************** 
 *                                                              * 
 * tagger.cpp - the combined method for segmentor and a tagger. * 
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
#include "options.h"

#include "reranker.h"

using namespace chinese;

/*----------------------------------------------------------------
 *
 * process - decode
 * 
 * First segment second tag. 
 * 
 *---------------------------------------------------------------*/

void process(string sInputFile, string sOutputFile, string sFeatureDb, int nBest, bool bRankingIncludeSeg) {
   TRACE("Tagging started");
   int time_start = clock();
   CReranker reranker(sFeatureDb, nBest, false, bRankingIncludeSeg);
   CSentenceReader input_reader(sInputFile);
   CSentenceWriter output_writer(sOutputFile);
   CStringVector input_sent;
   CTwoStringVector output_sent; 

   int nCount=0;
   
   input_reader.readRawSentence(&input_sent);
   while( !(input_sent.empty()) ) {
      cout << "Sentence " << ++nCount << endl;
      reranker.decode(input_sent, output_sent);
      output_writer.writeSentence(&output_sent);
      input_reader.readRawSentence(&input_sent);
   }
   TRACE("Tagging has finished successfully. Total time taken is: " << double(clock()-time_start)/CLOCKS_PER_SEC);
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
      configurations.defineConfiguration("s", "", "Use segmentor scores in ranking", "");
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
      bool bRankingIncludeSeg = !configurations.getConfiguration("s").empty() ? true : false;
   
      process(argv[1], argv[2], argv[3], nBest, bRankingIncludeSeg);
      return 0;
   } catch (const string &e) { cerr<<"Error: "<<e<<endl; return 1;
   }
}

