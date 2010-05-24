/****************************************************************
 *                                                              *
 * main.cpp - main app of the general constituent parser.       *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2007.8                         *
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

void process(const string &sInputFile, const string &sOutputFile, const string &sFeatureFile, const char cInputFormat, int nBest, const bool bScores, const bool bBinary) {

   cout << "Parsing started" << endl;

   int time_start = clock();

   CConParser parser(sFeatureFile, false) ;
   CSentenceReader *input_reader=0;
   ifstream *is=0;
   if (cInputFormat=='c')
      is = new ifstream(sInputFile.c_str());
   else
      input_reader = new CSentenceReader(sInputFile);
   ofstream os(sOutputFile.c_str());
   ofstream *os_scores=0;
   conparser::SCORE_TYPE *scores=0;
   assert(os.is_open());
   static CTwoStringVector raw_input;
   static CSentenceMultiCon con_input;
   CSentenceParsed *output_sent; 

   int nCount=0;
   bool bReadSuccessful;

   if (bScores) {
      scores = new conparser::SCORE_TYPE[nBest];
      os_scores = new ofstream(string(sOutputFile+".scores").c_str());
   }

   output_sent = new CSentenceParsed[nBest];
 
   // Read the next example
   if (cInputFormat=='c')
      bReadSuccessful = ((*is)>>con_input);
   else
      bReadSuccessful = input_reader->readTaggedSentence(&raw_input, false, TAG_SEPARATOR);
   while( bReadSuccessful ) {

      cout << "Sentence " << nCount << "...";
      ++ nCount;

      // Find decoder output
      if (cInputFormat=='c')
         parser.parse( con_input , output_sent , nBest , scores ) ;
      else
         parser.parse( raw_input , output_sent , nBest , scores ) ;
      
      // Ouptut sent
      for (int i=0; i<nBest; ++i) {
         if (bBinary)
            os << output_sent[i] ;
         else
            os << output_sent[i].str_unbinarized() << endl;
         if (bScores) *os_scores << scores[i] << endl;
      }

      cout << "done. " << endl; 
      
      // Read the next example
      if (cInputFormat==;c;)
         bReadSuccessful = ((*is)>>con_input);
      else
         bReadSuccessful = input_reader->readTaggedSentence(&raw_input, false, TAG_SEPARATOR);
   }

   delete [] output_sent ;
   if (input_reader) delete input_reader;
   if (is) {is.close(); delete is;}
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
      configurations.defineConfiguration("i", "r/c", "input format: r - pos-tagged sentence; c - pos-tagged and a lit of constituents for each word", "r");
      configurations.defineConfiguration("b", "", "output binarized parse trees", "");
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
      bool bBinary = configurations.getConfiguration("b").empty() ? false : true;
      char cInputFormat = configurations.getConfiguration("i") == "c" ? 'c' : 'r';
      process(argv[1], argv[2], argv[3], cInputFormat, nBest, bScores, bBinary);
   } 
   catch (const string &e) {
      cerr << "Error: " << e << endl;
      exit(1);
   }

   return 0;
}

