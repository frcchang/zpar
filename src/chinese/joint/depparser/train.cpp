// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                              *
 * train.cpp - the training for the joint depparser.            *
 *                                                              *
 * The module does decoding via combining the best output for a *
 * tagger with a depparser.                                     *
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
 * train - the training process
 *
 *---------------------------------------------------------------*/

void train(std::string sInputFile, std::string sReferenceFile, std::string sFeatureDB) {
   TRACE("Training started");
   int time_start = clock();
   CReranker reranker(sFeatureDB, true);
   std::ifstream input_file(sInputFile.c_str());
   std::ifstream reference_file(sReferenceFile.c_str());
   std::ifstream tagging_score_file(std::string(sInputFile+".scores.tagging").c_str());
   std::ifstream parsing_score_file(std::string(sInputFile+".scores.parsing").c_str());

   int nTagAll, nParseAll;
   int nTagNeeded, nParseNeeded;

   CSentenceParsed *nbest;
   CSentenceParsed correct;

   double *prior_scores;

   input_file >> nTagAll >> nParseAll;
   input_file >> nTagNeeded >> nParseNeeded;
   std::string line;
   getline(input_file, line);
   TRACE("Reranking "<<nTagNeeded<<"/"<<nParseNeeded<<" from "<<nTagAll<<"/"<<nParseAll);

   int nCount;

   CSentenceParsed tmp_sent;
   double tmp_priors[2];

   int nBest = nTagNeeded*nParseNeeded;

   nbest = new CSentenceParsed[nBest];
   prior_scores = new double[nBest*2];

   nCount = 0;
   reference_file >> correct;
   while( !(correct.empty()) ) {
      std::cerr << "Sentence " << ++nCount << std::endl;
      int index=0;
      for (int i=0; i<nTagAll; ++i) {
         for (int j=0; j<nParseAll; j++) {
            input_file >> tmp_sent;
            tagging_score_file >> tmp_priors[0];
            parsing_score_file >> tmp_priors[1];
            if (i<nTagNeeded && j<nParseNeeded) {
               nbest[index] = tmp_sent;
               prior_scores[index*2] = tmp_priors[0] / (8003*9) / 1000;
               prior_scores[index*2+1] = tmp_priors[1] / (8003*15) / 1000;
               //std::cout << prior_scores[index*2] << "\t" << prior_scores[index*2+1] << std::endl;
               index++;
            }
         }
      }
      reranker.train(nbest, &correct, nBest, prior_scores);
      reference_file >> correct;
   }
   reranker.finishTraining();
   delete[] nbest;
   delete[] prior_scores;
   input_file.close();
   tagging_score_file.close();
   parsing_score_file.close();
   TRACE("Training has finished successfully. Total time taken is: " << double(clock()-time_start)/CLOCKS_PER_SEC);
}

/*===============================================================
 *
 * main
 *
 *==============================================================*/

int main(int argc, char* argv[]) {
   const std::string hint = " n_best_file reference_file feature_file numer_of_iterations\n\n\
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
   if (argc < 5) {
      std::cout << "Usage: " << argv[0] << hint << std::endl;
      return 1;
   }

   for (int i=0; i<atoi(argv[4]); ++i)
      train(argv[1], argv[2], argv[3]);
   return 0;
}

