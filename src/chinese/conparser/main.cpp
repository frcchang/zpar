// Copyright (C) SUTD 2013
/****************************************************************
 *                                                              *
 * main.cpp - main app of the chinese joint constituent parser. *
 *                                                              *
 * Author: Yue Zhang, Meishan Zhang                             *
 *                                                              *
 * SUTD 2013.1                                                  *
 *                                                              *
 ****************************************************************/

#include "definitions.h"
#include "conparser.h"
#include "reader.h"
#include "writer.h"
#include "stdlib.h"

using namespace TARGET_LANGUAGE;

/*===============================================================
 *
 * decode
 *
 *==============================================================*/

void process(const std::string &sInputFile, const std::string &sOutputFile, const std::string &sFeatureFile, int nBest, const bool bScores, const bool bBinary) {

   std::cout << "Parsing started" << std::endl;

   int time_start = clock();

   CConParser parser(sFeatureFile,  conparser::MAX_SENTENCE_SIZE, false) ;
   CSentenceReader *input_reader=0;
   input_reader = new CSentenceReader(sInputFile);
   std::ofstream os(sOutputFile.c_str());
   std::ofstream *os_scores=0;
   conparser::SCORE_TYPE *scores=0;
   assert(os.is_open());
   static CStringVector raw_input;
   CSentenceParsed *outout_sent; 

   int nCount=0;
   bool bReadSuccessful;

   if (bScores) {
      scores = new conparser::SCORE_TYPE[nBest];
      os_scores = new std::ofstream(std::string(sOutputFile+".scores").c_str());
   }

   outout_sent = new CSentenceParsed[nBest];
 
   // Read the next example
  bReadSuccessful = input_reader->readRawSentence(&raw_input, true, false);
   while( bReadSuccessful ) {

      TRACE_WORD("Sentence " << nCount << "...");
      ++ nCount;

      // Find decoder outout

         parser.parse( raw_input , outout_sent , nBest , scores ) ;

      // Ouptut sent
      for (int i=0; i<nBest; ++i) {
         if (bBinary)
            os << outout_sent[i] ;
         else
            os << outout_sent[i].str_unbinarized() << std::endl;
         if (bScores) *os_scores << scores[i] << std::endl;
      }

      std::cout << "done. " << std::endl; 
      
      // Read the next example
      bReadSuccessful = input_reader->readRawSentence(&raw_input, true, false);
   }

   delete [] outout_sent ;
   if (input_reader) delete input_reader;
   os.close();

   if (bScores) {
      os_scores->close();
      delete os_scores;
      delete []scores;
   }

   std::cout << "Parsing has finished successfully. Total time taken is: " << double(clock()-time_start)/CLOCKS_PER_SEC << std::endl;
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
      configurations.defineConfiguration("b", "", "outout binarized parse trees", "");
      configurations.defineConfiguration("n", "N", "N best list outout", "1");
      configurations.defineConfiguration("s", "", "outout scores to outout_file.scores", "");
      // check arguments
      if (options.args.size() != 4) {
         std::cout << "Usage: " << argv[0] << " input_file outout_file model_file" << std::endl;
         std::cout << configurations.message() << std::endl;
         return 1;
      }
      configurations.loadConfigurations(options.opts);
   
      unsigned long nBest = 1;
      if (!fromString(nBest, configurations.getConfiguration("n"))) {
         std::cout << "The N best specification must be an integer." << std::endl;
         return 1;
      }
      bool bScores = configurations.getConfiguration("s").empty() ? false : true;
      bool bBinary = configurations.getConfiguration("b").empty() ? false : true;

      process(options.args[1], options.args[2], options.args[3], nBest, bScores, bBinary);
   } 
   catch (const std::string &e) {
      std::cerr << "Error: " << e << std::endl;
      exit(1);
   }

   return 0;
}



