// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                              *
 * zpar.cpp - main app; a convenience wrapper for all algorithms*
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2006.11                        *
 *                                                              *
 ****************************************************************/

#include "definitions.h"
#include "tagger.h"
#include "depparser.h"
#include "doc2snt.h"
#include "reader.h"
#include "writer.h"
#include "stdlib.h"

using namespace chinese;

#define MAX_SENTENCE_SIZE 512

/*===============================================================
 *
 * tag
 *
 *==============================================================*/

void tag(const std::string sInputFile, const std::string sOutputFile, const std::string sFeaturePath, bool bOutDoc) {
   std::cerr << "Tagging started" << std::endl;
   int time_start = clock();
   std::string sFeatureFile = sFeaturePath + "/tagger";
   if (!FileExists(sFeatureFile))
      THROW("Tagger model does not exists. It should be put at model_path/tagger");
   CTagger tagger(sFeatureFile, false, MAX_SENTENCE_SIZE, false);
   CDoc2Snt doc2snt(sInputFile, MAX_SENTENCE_SIZE);
   CSentenceReader input_reader(sInputFile);
   CSentenceWriter output_writer(sOutputFile);
   CStringVector *input_sent = new CStringVector;
   CTwoStringVector *output_sent = new CTwoStringVector;

   unsigned nCount=0;
   bool bNewLine = bOutDoc ? false : true;
   bool bDocPar = false;

   CBitArray prunes(MAX_SENTENCE_SIZE);

   //bool bReadSuccessful;
   //bReadSuccessful = input_reader.readRawSentence(input_sent, true, false);

   // If we read segmented sentence, we will ignore spaces from input.
   while( doc2snt.getSentence( *input_sent ) ) {
      TRACE("Sentence " << nCount);
      ++ nCount;
      bDocPar = false;
      if ( input_sent->back()=="\n" ) {
         bDocPar = true;
         input_sent->pop_back();
      }
      tagger.tag(input_sent, output_sent, NULL, 1);
      // Ouptut sent
      output_writer.writeSentence(output_sent, '_', bNewLine);
      input_sent->clear();
   }
   delete input_sent;
   delete output_sent;

   std::cerr << "Tagging has finished successfully. Total time taken is: " << double(clock()-time_start)/CLOCKS_PER_SEC << std::endl;
}

/*===============================================================
 *
 * parse
 *
 *==============================================================*/

//void parse(const std::string sInputFile, const std::string sOutputFile, const std::string sFeaturePath) {
//   std::cerr << "Initializing ZPar..." << std::endl;
//   int time_start = clock();
//   std::ostream *outs; if (sOutputFile=="") outs=&std::cout; else outs = new std::ofstream(sOutputFile.c_str());
//   std::string sTaggerFeatureFile = sFeaturePath + "/tagger";
//   std::string sParserFeatureFile = sFeaturePath + "/conparser";
//   if (!FileExists(sTaggerFeatureFile))
//      THROW("Tagger model does not exists. It should be put at model_path/tagger");
//   if (!FileExists(sParserFeatureFile))
//      THROW("Parser model does not exists. It should be put at model_path/conparser");
//   std::cerr << "[The segmentation and tagging model] "; std::cerr.flush();
//   CTagger tagger(sTaggerFeatureFile, false, MAX_SENTENCE_SIZE, false);
//   std::cerr << "[The parsing model] "; std::cerr.flush();
//   CConParser conparser(sParserFeatureFile, false);
//   CDoc2Snt doc2snt(sInputFile, MAX_SENTENCE_SIZE);
//   CSentenceReader input_reader(sInputFile);
//   CStringVector *input_sent = new CStringVector;
//   CTwoStringVector *tagged_sent = new CTwoStringVector;
//   chinese::CCFGTree *output_sent = new chinese::CCFGTree;
//   std::cerr << "ZPar initialized." << std::endl; std::cerr.flush();
//
//   unsigned nCount=0;
//
//   CBitArray prunes(MAX_SENTENCE_SIZE);
//
//   // If we read segmented sentence, we will ignore spaces from input.
//   while( doc2snt.getSentence( *input_sent ) ) {
//      TRACE("Sentence " << nCount);
//      ++ nCount;
//      if ( input_sent->back()=="\n" ) {
//         input_sent->pop_back();
//      }
//      tagger.tag(input_sent, tagged_sent, NULL, 1);
//      conparser.parse(*tagged_sent, output_sent);
//      // Ouptut sent
//      (*outs) << output_sent->str_unbinarized() << std::endl;
//      input_sent->clear();
//   }
//   delete input_sent;
//   delete tagged_sent;
//   delete output_sent;
//
//   if (sOutputFile!="") delete outs;
//   std::cerr << "Parsing has finished successfully. Total time taken is: " << double(clock()-time_start)/CLOCKS_PER_SEC << std::endl;
//}

/*===============================================================
 *
 * depparse
 *
 *==============================================================*/

void depparse(const std::string sInputFile, const std::string sOutputFile, const std::string sFeaturePath) {
   std::cerr << "Initializing ZPar..." << std::endl;
   int time_start = clock();
   std::ostream *outs; if (sOutputFile=="") outs=&std::cout; else outs = new std::ofstream(sOutputFile.c_str());
   std::string sTaggerFeatureFile = sFeaturePath + "/tagger";
   std::string sParserFeatureFile = sFeaturePath + "/depparser";
   if (!FileExists(sTaggerFeatureFile))
      THROW("Tagger model does not exists. It should be put at model_path/tagger");
   if (!FileExists(sParserFeatureFile))
      THROW("Parser model does not exists. It should be put at model_path/depparser");
   std::cerr << "[The segmentation and tagging model] "; std::cerr.flush();
   CTagger tagger(sTaggerFeatureFile, false, MAX_SENTENCE_SIZE, false);
   std::cerr << "[The parsing model] "; std::cerr.flush();
   CDepParser depparser(sParserFeatureFile, false);
   CDoc2Snt doc2snt(sInputFile, MAX_SENTENCE_SIZE);
   CSentenceReader input_reader(sInputFile);
   CStringVector *input_sent = new CStringVector;
   CTwoStringVector *tagged_sent = new CTwoStringVector;
   CLabeledDependencyTree *output_sent = new CLabeledDependencyTree;
   std::cerr << "ZPar initialized." << std::endl; std::cerr.flush();

   unsigned nCount=0;

   CBitArray prunes(MAX_SENTENCE_SIZE);

   // If we read segmented sentence, we will ignore spaces from input.
   while( doc2snt.getSentence( *input_sent ) ) {
      TRACE("Sentence " << nCount);
      ++ nCount;
      if ( input_sent->back()=="\n" ) {
         input_sent->pop_back();
      }
      tagger.tag(input_sent, tagged_sent, NULL, 1);
      depparser.parse(*tagged_sent, output_sent);
      // Ouptut sent
      (*outs) << *output_sent;
      input_sent->clear();
   }
   delete input_sent;
   delete tagged_sent;
   delete output_sent;

   if (sOutputFile!="") delete outs;
   std::cerr << "Parsing has finished successfully. Total time taken is: " << double(clock()-time_start)/CLOCKS_PER_SEC << std::endl;
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
      configurations.defineConfiguration("o", "{s|t[d]|d}", "output format; 's' segmented format, 't' pos-tagged format in sentences, 'td' pos-tagged format in documents without sentence boundary delimination, 'd' refers to dependency parse tree format", "d");

      if (options.args.size() < 2 || options.args.size() > 4) {
         std::cout << "\nUsage: " << argv[0] << " feature_path [input_file [output_file]]" << std::endl;
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

      bool bOutDoc = (sOutFormat=="td");

      if (sOutFormat == "t" || sOutFormat == "td")
          tag(sInputFile, sToFile, options.args[1], bOutDoc);
//      if (sOutFormat == "c" )
//          parse(sInputFile, sToFile, options.args[1]);
      if (sOutFormat == "d" )
          depparse(sInputFile, sToFile, options.args[1]);
      return 0;
   } catch(const std::string&e) {std::cerr<<"Error: "<<e<<std::endl;return 1;}
}

