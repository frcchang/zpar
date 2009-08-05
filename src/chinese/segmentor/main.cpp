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

using namespace chinese ;

/*===============================================================
 *
 * main
 *
 *==============================================================*/

void process(const string &sInputFile, const string &sOutputFile, const string &sFeatureFile, const int &nBest, const string &sOutputScores, const bool &bNoFWAndCD, const string &sCharCatFile, const string &sLexiconDict) {
   cout<<"Segmenting started"<<endl;
   int time_start = clock();
   CSegmentor *segmentor ;
   segmentor = new CSegmentor(sFeatureFile, false);
   if (!sCharCatFile.empty()) segmentor->loadCharCat(sCharCatFile);
   if (!sLexiconDict.empty()) segmentor->loadLexiconDict(sLexiconDict);
   CSentenceReader input_reader(sInputFile);
   CSentenceWriter output_writer(sOutputFile);
   CSentenceRaw *input_sent = new CSentenceRaw;
   CSentenceRaw *output_sent = new CSentenceRaw[nBest]; 
   double *scores = NULL;
   ofstream fileScores;

   int nCount=0;
   if (!sOutputScores.empty()) {
      scores = new double[nBest];
      fileScores.open(sOutputScores.c_str());
   }
   
   while( input_reader.readRawSentence(input_sent, false, bNoFWAndCD) ) {
      TRACE("Sentence " << nCount);
      ++nCount;
      segmentor->segment(input_sent, output_sent, scores, nBest);
      for (int i=0; i<nBest; ++i) {
         output_writer.writeSentence(output_sent+i);
         if (!sOutputScores.empty())
            fileScores << *(scores+i) << endl;
      }
   }
   delete input_sent;
   delete [] output_sent;

   if (!sOutputScores.empty()) {
      delete [] scores;
      fileScores.close();
   }

   delete segmentor ; 

   cout<<"Segmenting has finished successfully. Total time taken is: " << double(clock()-time_start)/CLOCKS_PER_SEC<<endl;
}

/*===============================================================
 *
 * main
 *
 *==============================================================*/

int main(int argc, char* argv[]) {
   const string hint = " input_file output_file feature_file [-cFile] [-dFile] [-nN] [-r] [-wFile]\n\n\
Options:\n\
-n n best list output\n\
-d output scores to File\n\
-c make use of character type information from File\n\
-w make use of word list from File\n\
-r use rule to segment English letters and Arabic numbers\n\
Note that -c -w -r should be set in consistence with the training method\n\
";
   // check arguments
   if (argc < 4) {
      cout << "\nUsage: " << argv[0] << hint << endl;
      return 1;
   }
   // check options
   int nBest = 1;
   string sOutputScores = ""; // the file of scores
   string sCharCatFile = "";
   string sLexiconDict = "";
   bool bNoFWAndCD = false;
   if (argc>4) {
      for (int i=4; i<argc; i++) {
         if (argv[i][0]!='-') { cout << "\nUsage: " << argv[0] << hint << endl ; return 1; }
         switch (argv[i][1]) {
            case 'n':
               nBest = atoi(string(argv[i]).substr(2).c_str());
               break;
            case 'd':
               if (string(argv[i]).size()<3) { cout << "\nUsage: " << argv[0] << hint << endl ; return 1; }
               sOutputScores = string(&(argv[i][2]));
               break;
            case 'r':
               bNoFWAndCD = true;
               break;
            case 'c':
               if (string(argv[i]).size()<3) { cout << "\nUsage: " << argv[0] << hint << endl ; return 1; }
               sCharCatFile = string(&(argv[i][2]));
               break;
            case 'w':
               if (string(argv[i]).size()<3) { cout << "\nUsage: " << argv[0] << hint << endl ; return 1; }
               sLexiconDict = string(&(argv[i][2]));
               break;
            default:
               cout << "\nUsage: " << argv[0] << hint << endl ;
               return 1;
         }
      }
   }
   // go on processing item
   process(argv[1], argv[2], argv[3], nBest, sOutputScores, bNoFWAndCD, sCharCatFile, sLexiconDict);
   // return normal
   return 0;
}

