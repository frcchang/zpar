/****************************************************************
 *                                                              *
 * main.cpp - main app of the general dependency parser.        *
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
#include "stdlib.h"

using namespace TARGET_LANGUAGE;

/*===============================================================
 *
 * decode
 *
 *==============================================================*/

void process(const string sInputFile, const string sOutputFile, const string sFeatureFile, unsigned long nBest, const bool bScores) {

   cout << "Parsing started" << endl;

   int time_start = clock();

   CDepParser parser(sFeatureFile, false) ;
   CSentenceReader input_reader(sInputFile);
   ofstream os(sOutputFile.c_str());
   ofstream *os_scores=0;
   depparser::SCORE_TYPE *scores=0;
   assert(os.is_open());
   CTwoStringVector input_sent;
   CDependencyParse *output_sent; 

   int nCount=0;
   bool bReadSuccessful;

   if (bScores) {
      scores = new depparser::SCORE_TYPE[nBest];
      os_scores = new ofstream(string(sOutputFile+".scores").c_str());
   }

   output_sent = new CDependencyParse[nBest];
 
   // Read the next example
   bReadSuccessful = input_reader.readTaggedSentence(&input_sent, false, TAG_SEPARATOR);
   while( bReadSuccessful ) {

      TRACE("Sentence " << nCount);
      ++ nCount;

      // Find decoder output
      parser.parse( input_sent , output_sent , nBest , scores ) ;
      
      // Ouptut sent
      for (int i=0; i<nBest; ++i) {
         os << output_sent[i] ;
         if (bScores) *os_scores << scores[i] << endl;
      }
      
      // Read the next example
      bReadSuccessful = input_reader.readTaggedSentence(&input_sent, false, TAG_SEPARATOR);
   }

   delete [] output_sent ;
   os.close();

   if (bScores) {
      os_scores->close();
      delete os_scores;
      delete []scores;
   }

   cout << "Parsing has finished successfully. Total time taken is: " << double(clock()-time_start)/CLOCKS_PER_SEC << endl;
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
      configurations.defineConfiguration("s", "", "output scores to output_file.scores", "");
      // check arguments
      if (options.args.size() != 4) {
         cout << "Usage: " << argv[0] << " input_file output_file model_file" << endl;
         cout << configurations.message() << endl;
         return 1;
      }
      configurations.loadConfigurations(options.opts);
   
      unsigned long nBest = 1;
      if (!fromString(nBest, configurations.getConfiguration("n"))) {
         cout << "The N best specification must be an integer." << endl;
         return 1;
      }
      bool bScores = configurations.getConfiguration("s").empty() ? false : true;
   
      process(argv[1], argv[2], argv[3], nBest, bScores);
      return 0;
   } catch (const string &e) {
      cerr << "Error: " << e << endl;
      return 1;
   }
}

