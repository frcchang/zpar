/****************************************************************
 *                                                              *
 * main.cpp - main app of the Chinese parser.                   *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2006.11                        *
 *                                                              *
 ****************************************************************/

#include "definitions.h"
#include "depparser.h"
#include "reader.h"
#include "writer.h"
#include "stdlib.h"

using namespace chinese;

static string sKnowledgePath;

/*===============================================================
 *
 * processor
 *
 *==============================================================*/

void process(const string sInputFile, const string sOutputFile, const string sFeatureFile, bool bKnowledge) {
   cout << "Parsing started" << endl;
   int time_start = clock();
   CDepParser parser(sFeatureFile, false) ;
//   if (bKnowledge) parser.loadKnowledge(sKnowledgePath);
   CSentenceReader input_reader(sInputFile);
   ofstream os(sOutputFile.c_str());
   assert(os.is_open());
   CStringVector input_sent;
   CDependencyTree output_sent; 

   int nCount=0;
   
   bool bReadSuccessful; 
   bReadSuccessful = input_reader.readRawSentence(&input_sent, true, false);

   // If we read segmented sentence, we will ignore spaces from input. 
   while( bReadSuccessful ) {
      TRACE("Sentence " << nCount);
      ++ nCount;
      //
      // Find decoder output
      //
      parser.parse(input_sent, output_sent);
      //
      // Ouptut sent
      //
      os << output_sent ;
      bReadSuccessful = input_reader.readRawSentence(&input_sent, true, false);
   }

   cout << "Tagging has finished successfully. Total time taken is: " << double(clock()-time_start)/CLOCKS_PER_SEC << endl;
}

/*===============================================================
 *
 * main
 *
 *==============================================================*/

int main(int argc, char* argv[]) {
   const string hint = " input_file output_file feature_file [-kPATH]\n\n\
Options:\n\
-k use knowledge form the given path\n\
";

   if (argc < 4) {
      cout << "\nUsage: " << argv[0] << hint << endl;
      return 1;
   }

   bool bKnowledge = false;

   if (argc>4) {
      for (int i=4; i<argc; i++) {
         if (argv[i][0]!='-') {
            cout << "\nUsage: " << argv[0] << hint << endl ;
            return 1;
         }
         switch (argv[i][1]) {
            case 'k':
               bKnowledge = true;
               sKnowledgePath = string(argv[i]).substr(2);
               break;
            default:
               cout << "\nUsage: " << argv[0] << hint << endl ;
               return 1;
         }
      }
   }

   process(argv[1], argv[2], argv[3], bKnowledge);
   return 0;
}

