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

#include "reranker.h"

using namespace chinese;

/*----------------------------------------------------------------
 *
 * process - decode
 * 
 * First segment second tag. 
 * 
 *---------------------------------------------------------------*/

void process(string sInputFile, string sOutputFile, string sFeatureDb, int nBest) {
   TRACE("Tagging started");
   int time_start = clock();
   CReranker reranker(sFeatureDb, nBest, false);
   CSentenceReader input_reader(sInputFile);
   CSentenceWriter output_writer(sOutputFile);
   CSentenceRaw input_sent;
   CSentenceTagged output_sent; 

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
   const string hint = " input_file output_file feature_file [-nN]\n\n\
Options:\n\
-n N best segmentation output; default 10\n\
";
   if (argc < 4) {
      cout << "Usage: " << argv[0] << hint << endl;
      return 1;
   }

   int nBest = 10;

   if (argc>4) {
      for (int i=4; i<argc; i++) {
         if (argv[i][0]!='-') { cout << "\nUsage: " << argv[0] << hint << endl ; return 1; }
         switch (argv[i][1]) {
            case 'n':
               if (strlen(argv[i])<3) { cout << "\nUsage: " << argv[0] << hint << endl ; return 1; }
               nBest = atoi(string(argv[i]+2).c_str());
               break;
            default:
               cout << "\nUsage: " << argv[0] << hint << endl ;
               return 1;
         }
      }
   }

   process(argv[1], argv[2], argv[3], nBest);
   return 0;
}

