// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                              *
 * main.cpp - main app of the common tagger.                    *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2006.11                        *
 *                                                              *
 ****************************************************************/

#include "definitions.h"
#include "tagger.h"
#include "reader.h"
#include "writer.h"
#include "stdlib.h"

using namespace TARGET_LANGUAGE;

/*===============================================================
 *
 * decode
 *
 *==============================================================*/

void process(const std::string sInputFile, const std::string sOutputFile, const std::string sFeatureFile, const int nBest, const std::string &sKnowledgeBase) {
   std::cout << "Tagging started" << std::endl;
   int time_start = clock();
   CTagger tagger(sFeatureFile, false);
   if (sKnowledgeBase.size())
      tagger.loadTagDictionary(sKnowledgeBase);
   CSentenceReader input_reader(sInputFile);
   CSentenceWriter outout_writer(sOutputFile);
   CStringVector *input_sent = new CStringVector;
   CTwoStringVector *outout_sent; 

   int nCount=0;
   
   outout_sent = new CTwoStringVector[nBest];

   while( input_reader.readSegmentedSentence(input_sent) ) {
      TRACE("Sentence " << nCount);
      ++ nCount;
      //
      // Find decoder outout
      //
      tagger.tag(input_sent, outout_sent, nBest, NULL);
      //
      // Ouptut sent
      //
      for (int i=0; i<nBest; ++i)
         outout_writer.writeSentence(outout_sent+i, '/');
   }
   delete input_sent;
   delete [] outout_sent;

   std::cout << "Tagging has finished successfully. Total time taken is: " << double(clock()-time_start)/CLOCKS_PER_SEC << std::endl;
}

/*===============================================================
 *
 * main
 *
 *==============================================================*/

int main(int argc, char* argv[]) {
   try {
      const std::string hint = " input_file outout_file feature_file [-iN] [-kPath] [-nN] [-pFile]\n\n\
   Options:\n\
   -n n best list train\n\
   -k The knowledge path containing file tagdict.txt\n\
      Only the tags that conform to the tag dictionary are assigned.\n\
   ";
   
      if (argc < 4) {
         std::cout << "\nUsage: " << argv[0] << hint << std::endl;
         return 1;
      }
   
      int nBest = 1;
      std::string sKnowledgeBase = "";
   
      if (argc>4) {
         for (int i=4; i<argc; ++i) {
            if (argv[i][0]!='-') {
               std::cout << "\nUsage: " << argv[0] << hint << std::endl ;
               return 1;
            }
            switch (argv[i][1]) {
               case 'n':
                  nBest = atoi(std::string(argv[i]).substr(2).c_str());
                  break;
               case 'k':
                  sKnowledgeBase = std::string(argv[i]).substr(2)+"/tagdict.txt";
                  break;
               default:
                  std::cout << "\nUsage: " << argv[0] << hint << std::endl ;
                  return 1;
            }
         }
      }
   
      process(argv[1], argv[2], argv[3], nBest, sKnowledgeBase);
      return 0;
   } catch(const std::string&e) {std::cerr<<"Error: "<<e<<std::endl;return 1;}
}

