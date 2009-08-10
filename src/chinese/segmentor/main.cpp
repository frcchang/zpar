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

void process(const string &sInputFile, const string &sOutputFile, const string &sFeatureFile, const int &nBest, const string &sOutputScores, const bool &bNoFWAndCD, const string &sLexiconDict) {
   cout<<"Segmenting started"<<endl;
   int time_start = clock();
   CSegmentor *segmentor ;
   segmentor = new CSegmentor(sFeatureFile, false, "", sLexiconDict);
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
   try {
      COptions options(argc, argv);
      CConfigurations configurations;
      configurations.defineConfiguration("n", "N", "N best list output", "1");
      configurations.defineConfiguration("d", "Path", "save scores to Path", "");
      configurations.defineConfiguration("w", "Path", "privide word list in Path", "");
      configurations.defineConfiguration("r", "", "use rules to segment English letters and Arabic numbers", "");
      // check arguments
      if (options.args.size() != 4) {
         cout << "Usage: " << argv[0] << " input_file output_file model_file" << endl;
         cout << configurations.message() << endl;
         return 1;
      }
      configurations.loadConfigurations(options.opts);
   
      // check options
      int nBest;
      if (!fromString(nBest, configurations.getConfiguration("n"))) {
         cout << "The N best specification must be an integer." << endl;
         return 1;
      }
      string sOutputScores = configurations.getConfiguration("d");
      string sLexiconDict = configurations.getConfiguration("w");
      bool bNoFWAndCD = configurations.getConfiguration("r").empty() ? true : false;

      // main
      process(argv[1], argv[2], argv[3], nBest, sOutputScores, bNoFWAndCD, sLexiconDict);

      // return normal
      return 0;
   }
   catch(const string &e) {
      cerr << "Error: " << e << " Stop." << endl;
      return 1;
   }
}

