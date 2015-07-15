// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                              *
 * main.cpp - the decoder for the joint depparser.              *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2008.10                        *
 *                                                              *
 ****************************************************************/

#include "reader.h"
#include "writer.h"

#include "reranker.h"

using namespace chinese;

/*----------------------------------------------------------------
 *
 * process - the training process
 *
 *---------------------------------------------------------------*/

void process(std::string sInputFile, std::string sOutputFile, std::string sFeatureDB) {
   TRACE("Decoding started");
   int time_start = clock();
   CReranker reranker(sFeatureDB, false);
   std::ifstream input_file(sInputFile.c_str());
   std::ofstream output_file(sOutputFile.c_str());
   std::ifstream tagging_prior_file(std::string(sInputFile+".scores.tagging").c_str());
   std::ifstream parsing_prior_file(std::string(sInputFile+".scores.parsing").c_str());

   int nTagAll, nParseAll;
   int nTagNeeded, nParseNeeded;

   CSentenceParsed *nbest;
   CSentenceParsed output;

   double *prior_scores;

   input_file >> nTagAll >> nParseAll;
   input_file >> nTagNeeded >> nParseNeeded;
   std::string line; getline(input_file, line);
   TRACE("Reranking "<<nTagNeeded<<"/"<<nParseNeeded<<" from "<<nTagAll<<"/"<<nParseAll);

   int nCount;
   int nBest = nTagNeeded*nParseNeeded;

   nbest = new CSentenceParsed[nBest];
   prior_scores = new double[nBest*2];

   CSentenceParsed tmp_sent;
   double tmp_score[2];

   nCount = 0;
   while( input_file ) {
      std::cerr << "Sentence " << ++nCount << std::endl;
      int index=0;
      for (int i=0; i<nTagAll; ++i) {
         for (int j=0; j<nParseAll; j++) {
            input_file >> tmp_sent;
            tagging_prior_file >> tmp_score[0];
//std::cout << "re" << tmp_score[0] << std::endl;
            parsing_prior_file >> tmp_score[1];
            if (i<nTagNeeded && j<nParseNeeded) {
               nbest[index] = tmp_sent;
               prior_scores[index*2] = tmp_score[0]/(8892*9)/1000;
               prior_scores[index*2+1] = tmp_score[1]/(8892*15)/1000;
               ++index;
            }
         }
      }
      reranker.rerank(nbest, &output, nBest, prior_scores);
      output_file << output;
   }
   delete[] nbest;
   delete[] prior_scores;
   input_file.close();
   output_file.close();
   tagging_prior_file.close();
   parsing_prior_file.close();
   TRACE("Decoding has finished successfully. Total time taken is: " << double(clock()-time_start)/CLOCKS_PER_SEC);
}

/*===============================================================
 *
 * main
 *
 *==============================================================*/

int main(int argc, char* argv[]) {
   const std::string hint = " n_best_file output_file feature_file\n\n\
The N-best file must contain two header lines, the first specifying how\n\
many tagging and parsing outputs are contained in the file, and the \n\
second specifying how many tagging and parsing outputs are used in the \n\
reranking system. \n\
For example, the file might start with the heading\n\
10 10\n\
5 5\n\
which means that there are 100 outputs per input, organised by 10 tag \n\
outputs, each with 10 parses. We use the best 5 tag 5 parse for reranking\n\
";
   if (argc < 4) {
      std::cout << "Usage: " << argv[0] << hint << std::endl;
      return 1;
   }

   process(argv[1], argv[2], argv[3]);
   return 0;
}

