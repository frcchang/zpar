// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                              *
 * zpar.cpp - main app; a convenience wrapper for all algorithms*
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2010.03                        *
 *                                                              *
 ****************************************************************/

#include "definitions.h"
#include "tagger.h"
#include "conparser.h"
#include "depparser.h"
#include "deplabeler.h"
#include "reader.h"
#include "writer.h"
#include "stdlib.h"

using namespace generic;

#define MAX_SENTENCE_SIZE 512

/*===============================================================
 *
 * tag
 *
 *==============================================================*/

void tag(const std::string sInputFile, const std::string sOutputFile, const std::string sFeaturePath) {
   std::cout << "Tagging started" << std::endl;
   int time_start = clock();
   CTagger tagger(sFeaturePath, false);
   CSentenceReader input_reader(sInputFile);
   CSentenceWriter outout_writer(sOutputFile);
   CStringVector *input_sent = new CStringVector;
   CTwoStringVector *outout_sent; 

   int nCount=0;
   
   const unsigned nBest = 1;
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
 * parse
 *
 *==============================================================*/

void parse(const std::string sInputFile, const std::string sOutputFile, const std::string sFeaturePath) {
   std::cout << "Parsing started" << std::endl;
   int time_start = clock();
   std::ostream *outs; if (sOutputFile=="") outs=&std::cout; else outs = new std::ofstream(sOutputFile.c_str()); 
   std::string sTaggerFeatureFile = sFeaturePath + "/tagger";
   std::string sParserFeatureFile = sFeaturePath + "/conparser";
   if (!FileExists(sTaggerFeatureFile))
      THROW("Tagger model does not exists. It should be put at model_path/tagger");
   if (!FileExists(sParserFeatureFile))
      THROW("Parser model does not exists. It should be put at model_path/conparser");
   CTagger tagger(sTaggerFeatureFile, false);
   CConParser conparser(sParserFeatureFile, false);
   CSentenceReader input_reader(sInputFile);
   CStringVector *input_sent = new CStringVector;
   CTwoStringVector *tagged_sent = new CTwoStringVector; 
   generic::CCFGTree *outout_sent = new generic::CCFGTree; 

   unsigned nCount=0;
   
   CBitArray prunes(MAX_SENTENCE_SIZE);

   // If we read segmented sentence, we will ignore spaces from input. 
   while( input_reader.readSegmentedSentence(input_sent) ) {
      TRACE("Sentence " << nCount);
      ++ nCount;
      if ( input_sent->back()=="\n" ) {
         input_sent->pop_back();
      }
      tagger.tag(input_sent, tagged_sent, 1, NULL);
      conparser.parse(*tagged_sent, outout_sent);
      // Ouptut sent
      (*outs) << outout_sent->str_unbinarized() << std::endl;
   }
   delete input_sent;
   delete tagged_sent;
   delete outout_sent;

   if (sOutputFile!="") delete outs;
   std::cout << "Parsing has finished successfully. Total time taken is: " << double(clock()-time_start)/CLOCKS_PER_SEC << std::endl;
}

/*===============================================================
 *
 * depparse
 *
 *==============================================================*/

void depparse(const std::string sInputFile, const std::string sOutputFile, const std::string sFeaturePath) {
   std::cout << "Parsing started" << std::endl;
   int time_start = clock();
   std::ostream *outs; if (sOutputFile=="") outs=&std::cout; else outs = new std::ofstream(sOutputFile.c_str()); 
   std::string sTaggerFeatureFile = sFeaturePath + "/tagger";
   std::string sParserFeatureFile = sFeaturePath + "/depparser";
   std::string sLabelerFeatureFile = sFeaturePath + "/deplabeler";
   if (!FileExists(sTaggerFeatureFile))
      THROW("Tagger model does not exists. It should be put at model_path/tagger");
   if (!FileExists(sParserFeatureFile))
      THROW("Parser model does not exists. It should be put at model_path/depparser");
//   if (bLabeled && !FileExists(sLabelerFeatureFile))
//      THROW("Labeler model does not exists. It should be put at model_path/deplabeler");
   std::cout << "[POS tagging module] "; std::cout.flush();
   CTagger tagger(sTaggerFeatureFile, false);
   std::cout << "[Parsing module] "; std::cout.flush();
   CDepParser depparser(sParserFeatureFile, false);
   CDepLabeler *deplabeler = 0;
//   if (bLabeled) {
//      std::cout << "[Labeling module] "; std::cout.flush();
//      deplabeler = new CDepLabeler(sLabelerFeatureFile, false);
//   }
   CSentenceReader input_reader(sInputFile);
   CStringVector input_sent;
   CTwoStringVector tagged_sent; 
   CDependencyTree parsed_sent; 
   CLabeledDependencyTree labeled_sent;

   unsigned nCount=0;
   
   CBitArray prunes(MAX_SENTENCE_SIZE);

   // If we read segmented sentence, we will ignore spaces from input. 
   while( input_reader.readSegmentedSentence(&input_sent) ) {
      TRACE("Sentence " << nCount);
      ++ nCount;
      if ( input_sent.back()=="\n" ) {
         input_sent.pop_back();
      }
      tagger.tag(&input_sent, &tagged_sent, 1, NULL);
      depparser.parse(tagged_sent, &labeled_sent, 1, NULL);
      (*outs) << labeled_sent;
//      if (bLabeled) {
//         deplabeler->label(parsed_sent, &labeled_sent);
         // Ouptut sent
//         (*outs) << labeled_sent;
//      }
//      else {
//         (*outs) << parsed_sent;
//      }
   }

//   if (bLabeled) delete deplabeler;

   if (sOutputFile!="") delete outs;
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
      configurations.defineConfiguration("o", "{t|d|c}", "outout format; 't' pos-tagged format in sentences, 'd' refers to labeled dependency tree format, and 'c' refers to constituent parse tree format", "d");

      if (options.args.size() < 2 || options.args.size() > 4) {
         std::cout << "\nUsage: " << argv[0] << " feature_path [input_file [outout_file]]" << std::endl;
         std::cout << configurations.message() << std::endl;
         return 1;
      }
      std::string warning = configurations.loadConfigurations(options.opts);
      if (!warning.empty()) {
         std::cout << "Warning: " << warning << std::endl;
      }

      std::string sInputFile = options.args.size() > 2 ? options.args[2] : "";
      std::string sToFile = options.args.size() > 3 ? options.args[3] : "";
      std::string sOutFormat = configurations.getConfiguration("o");

      if (sOutFormat == "t")
          tag(sInputFile, sToFile, options.args[1]);
      if (sOutFormat == "c" )
          parse(sInputFile, sToFile, options.args[1]);
      if (sOutFormat == "d" )
          depparse(sInputFile, sToFile, options.args[1]);
      return 0;
   } catch(const std::string&e) {std::cerr<<"Error: "<<e<<std::endl;return 1;}
}

