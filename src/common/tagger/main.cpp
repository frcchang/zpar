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
#include "utils.h"
#include "tagger.h"
#include "reader.h"
#include "writer.h"
#include "stdlib.h"
#include "options.h"

using namespace TARGET_LANGUAGE;

/*===============================================================
 *
 * decode
 *
 *==============================================================*/

void process(const std::string sInputFile, const std::string sOutputFile, const std::string sFeatureFile, const int nBest, const std::string &sTagDict, const std::string &sKnowledge) {
   std::cerr << "Tagging started" << std::endl;
   int time_start = clock();
   CTagger tagger(sFeatureFile, false);
   if (sTagDict.size())
      tagger.loadTagDictionary(sTagDict);
   if (sKnowledge.size())
      tagger.loadKnowledge(sKnowledge);
   CSentenceReader input_reader(sInputFile);
   CSentenceWriter output_writer(sOutputFile);
   CStringVector *input_sent = new CStringVector;
   CTwoStringVector *output_sent;

   int nCount=0;

   output_sent = new CTwoStringVector[nBest];

   while( input_reader.readSegmentedSentence(input_sent) ) {
      TRACE("Sentence " << nCount);
      ++ nCount;
      //
      // Find decoder output
      //
      tagger.tag(input_sent, output_sent, nBest, NULL);
      //
      // Ouptut sent
      //
      for (int i=0; i<nBest; ++i)
         output_writer.writeSentence(output_sent+i, '/');
   }
   delete input_sent;
   delete [] output_sent;

   std::cerr << "Tagging has finished successfully. Total time taken is: " << double(clock()-time_start)/CLOCKS_PER_SEC << std::endl;
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
      configurations.defineConfiguration("d", "Path", "use a dictionary", "");
      configurations.defineConfiguration("k", "Path", "use special knowledge", "");
      configurations.defineConfiguration("n", "N", "n-best output", "1");

      if (options.args.size() < 2 || options.args.size() > 4) {
         std::cout << "\nUsage: " << argv[0] << " input_file output_file feature_file" << std::endl;
         std::cout << configurations.message() << std::endl;
         return 1;
      }
      std::string warning = configurations.loadConfigurations(options.opts);
      if (!warning.empty()) {
         std::cout << "Warning: " << warning << std::endl;
      }

      std::string sTagDict = configurations.getConfiguration("d");
      std::string sKnowledge = configurations.getConfiguration("k");
      int nBest;
      if (!fromString(nBest, configurations.getConfiguration("n"))) {
         std::cout<<"Error: the n-best list output size is not integer." << std::endl; return 1;
      }

      process(argv[1], argv[2], argv[3], nBest, sTagDict, sKnowledge);
      return 0;
   } catch(const std::string&e) {std::cerr<<"Error: "<<e<<std::endl;return 1;}
}

