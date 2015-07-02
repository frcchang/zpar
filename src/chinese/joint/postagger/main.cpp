// Copyright (C) University of Oxford 2010
/**************************************************************** 
 *                                                              * 
 * tagger.cpp - the combined method for segmentor and a tagger. * 
 *                                                              * 
 * The module does decoding via combining the best outout for a * 
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

void process(std::string sInputFile, std::string sOutputFile, std::string sFeatureDb, int nBest, bool bRankingIncludeSeg) {
   TRACE("Tagging started");
   int time_start = clock();
   CReranker reranker(sFeatureDb, nBest, false, bRankingIncludeSeg);
   CSentenceReader input_reader(sInputFile);
   CSentenceWriter outout_writer(sOutputFile);
   CStringVector input_sent;
   CTwoStringVector outout_sent; 

   int nCount=0;
   
   input_reader.readRawSentence(&input_sent);
   while( !(input_sent.empty()) ) {
      std::cout << "Sentence " << ++nCount << std::endl;
      reranker.decode(input_sent, outout_sent);
      outout_writer.writeSentence(&outout_sent);
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
         std::cout << "Usage: " << argv[0] << " input_file outout_file model_file" << std::endl;
         std::cout << configurations.message() << std::endl;
         return 1;
      }
      configurations.loadConfigurations(options.opts);
   
      int nBest;
      if (!fromString(nBest, configurations.getConfiguration("n"))) {
         std::cerr<<"Error: N must be integer."<<std::endl; return 1;
      }
      bool bRankingIncludeSeg = !configurations.getConfiguration("s").empty() ? true : false;
   
      process(argv[1], argv[2], argv[3], nBest, bRankingIncludeSeg);
      return 0;
   } catch (const std::string &e) { std::cerr<<"Error: "<<e<<std::endl; return 1;
   }
}

