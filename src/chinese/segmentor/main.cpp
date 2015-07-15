// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                              *
 * main.cpp - main app of the Chinese segmentor.                *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2006.11                        *
 *                                                              *
 ****************************************************************/

#include "definitions.h"
#include "segmentor.h"
#include "reader.h"
#include "writer.h"
#include "stdlib.h"
#include "options.h"

using namespace chinese ;

/*===============================================================
 *
 * main
 *
 *==============================================================*/

void process(const std::string &sInputFile, const std::string &sOutputFile, const std::string &sFeatureFile, const int &nBest, const std::string &sOutputScores) {
   std::cerr << "Segmenting started"<<std::endl;
   int time_start = clock();
   CSegmentor *segmentor ;
   segmentor = new CSegmentor(sFeatureFile);
   CSentenceReader input_reader(sInputFile);
   CSentenceWriter output_writer(sOutputFile);
   CStringVector *input_sent = new CStringVector;
   CStringVector *output_sent = new CStringVector[nBest];
   double *scores = NULL;
   std::ofstream fileScores;

   int nCount=0;
   if (!sOutputScores.empty()) {
      scores = new double[nBest];
      fileScores.open(sOutputScores.c_str());
   }

   while( input_reader.readRawSentence(input_sent, false) ) {
      TRACE("Sentence " << nCount);
      ++nCount;
      segmentor->segment(input_sent, output_sent, scores, nBest);
      for (int i=0; i<nBest; ++i) {
         output_writer.writeSentence(output_sent+i);
         if (!sOutputScores.empty())
            fileScores << *(scores+i) << std::endl;
      }
   }
   delete input_sent;
   delete [] output_sent;

   if (!sOutputScores.empty()) {
      delete [] scores;
      fileScores.close();
   }

   delete segmentor ;

   std::cerr << "Segmenting has finished successfully. Total time taken is: " << double(clock()-time_start)/CLOCKS_PER_SEC<<std::endl;
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
      configurations.defineConfiguration("n", "N", "N best list output", "1");
      configurations.defineConfiguration("d", "Path", "save scores to Path", "");
      // check arguments
      if (options.args.size() < 2 || options.args.size() > 4) {
         std::cout << "Usage: " << argv[0] << " model_file [input_file [output_file]]" << std::endl;
         std::cout << configurations.message() << std::endl;
         return 1;
      }
      std::string warning = configurations.loadConfigurations(options.opts);
      if (!warning.empty()) {
         std::cout << "Warning: " << warning << std::endl;
      }

      // check options
      int nBest;
      if (!fromString(nBest, configurations.getConfiguration("n"))) {
         std::cout << "The N best specification must be an integer." << std::endl;
         return 1;
      }
      std::string sOutputScores = configurations.getConfiguration("d");

      // main
      std::string sInputFile = options.args.size() > 2 ? options.args[2] : "";
      std::string sOutputFile = options.args.size() > 3 ? options.args[3] : "";
      process(sInputFile, sOutputFile, options.args[1], nBest, sOutputScores);

      // return normal
      return 0;
   }
   catch(const std::string &e) {
      std::cerr << "Error: " << e << " Stop." << std::endl;
      return 1;
   }
}

