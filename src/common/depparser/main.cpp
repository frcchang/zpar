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

void process(const string sInputFile, const string sOutputFile, const string sFeatureFile, int nBest, const bool bScores) {

   cout << "Parsing started" << endl;

   int time_start = clock();

   CDepParser parser(sFeatureFile, false) ;
   CSuperTagReader input_reader(sInputFile);
   ofstream os(sOutputFile.c_str());
   ofstream *os_scores=0;
   depparser::SCORE_TYPE *scores=0;
   assert(os.is_open());
   CSentenceTagged input_sent;
   CSentenceParsed *output_sent; 

   int nCount=0;
   bool bReadSuccessful;

   if (bScores) {
      scores = new depparser::SCORE_TYPE[nBest];
      os_scores = new ofstream(string(sOutputFile+".scores").c_str());
   }

   output_sent = new CSentenceParsed[nBest];
 
   // Read the next example
   bReadSuccessful = input_reader.readTaggedSentence(&input_sent, false, CTag::SEPARATOR);
   while( bReadSuccessful ) {

      TRACE("Sentence " << nCount);
      ++ nCount;

      // Find decoder output
      parser.parse( input_sent , output_sent , nBest , scores ) ;
      
      // Ouptut sent
      for (int i=0; i<nBest; i++) {
         os << output_sent[i] ;
         if (bScores) *os_scores << scores[i] << endl;
      }
      
      // Read the next example
      bReadSuccessful = input_reader.readTaggedSentence(&input_sent, false, CTag::SEPARATOR);
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
   const string hint = " input_file output_file feature_file [-s] [-nN]\n\
Arguments:\n\
-n N-best output\n\n\
-s output scores to the file output_file.scores";

   if (argc < 4) {
      cout << "\nUsage: " << argv[0] << hint << endl;
      return 1;
   }

   int nBest = 1;
   bool bScores = false;

   if (argc>4) {
      for (int i=4; i<argc; i++) {
         if (argv[i][0]!='-') { cout << "\nUsage: " << argv[0] << hint << endl ; return 1; }
         switch (argv[i][1]) {
            case 'n':
               nBest = atoi(string(argv[i]).substr(2).c_str());
               break;
            case 's':
               bScores = true;
               break;
            default:
               cout << "\nUsage: " << argv[0] << hint << endl ;
               return 1;
         }
      }
   }

   process(argv[1], argv[2], argv[3], nBest, bScores);
   return 0;
}

