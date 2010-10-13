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
#include "conparser.h"
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

void tag(const string sInputFile, const string sOutputFile, const string sFeaturePath, bool bOutDoc) {
   cout << "Tagging started" << endl;
   int time_start = clock();
   string sFeatureFile = sFeaturePath + "/tagger";
   if (!FileExists(sFeatureFile))
      THROW("Tagger model does not exists. It should be put at model_path/tagger");
   CTagger tagger(sFeatureFile, false, MAX_SENTENCE_SIZE, "", false);
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

   cout << "Tagging has finished successfully. Total time taken is: " << double(clock()-time_start)/CLOCKS_PER_SEC << endl;
}

/*===============================================================
 *
 * parse
 *
 *==============================================================*/

void parse(const string sInputFile, const string sOutputFile, const string sFeaturePath) {
   cout << "Initializing ZPar..." << endl;
   int time_start = clock();
   ostream *outs; if (sOutputFile=="") outs=&cout; else outs = new ofstream(sOutputFile.c_str()); 
   string sTaggerFeatureFile = sFeaturePath + "/tagger";
   string sParserFeatureFile = sFeaturePath + "/conparser";
   if (!FileExists(sTaggerFeatureFile))
      THROW("Tagger model does not exists. It should be put at model_path/tagger");
   if (!FileExists(sParserFeatureFile))
      THROW("Parser model does not exists. It should be put at model_path/conparser");
   cout << "[The segmentation and tagging model] "; cout.flush();
   CTagger tagger(sTaggerFeatureFile, false, MAX_SENTENCE_SIZE, "", false);
   cout << "[The parsing model] "; cout.flush();
   CConParser conparser(sParserFeatureFile, false);
   CDoc2Snt doc2snt(sInputFile, MAX_SENTENCE_SIZE);
   CSentenceReader input_reader(sInputFile);
   CStringVector *input_sent = new CStringVector;
   CTwoStringVector *tagged_sent = new CTwoStringVector; 
   chinese::CCFGTree *output_sent = new chinese::CCFGTree; 
   cout << "ZPar initialized." << endl; cout.flush();

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
      conparser.parse(*tagged_sent, output_sent);
      // Ouptut sent
      (*outs) << output_sent->str_unbinarized() << endl;
      input_sent->clear();
   }
   delete input_sent;
   delete tagged_sent;
   delete output_sent;

   if (sOutputFile!="") delete outs;
   cout << "Parsing has finished successfully. Total time taken is: " << double(clock()-time_start)/CLOCKS_PER_SEC << endl;
}

/*===============================================================
 *
 * depparse
 *
 *==============================================================*/

void depparse(const string sInputFile, const string sOutputFile, const string sFeaturePath) {
   cout << "Initializing ZPar..." << endl;
   int time_start = clock();
   ostream *outs; if (sOutputFile=="") outs=&cout; else outs = new ofstream(sOutputFile.c_str()); 
   string sTaggerFeatureFile = sFeaturePath + "/tagger";
   string sParserFeatureFile = sFeaturePath + "/depparser";
   if (!FileExists(sTaggerFeatureFile))
      THROW("Tagger model does not exists. It should be put at model_path/tagger");
   if (!FileExists(sParserFeatureFile))
      THROW("Parser model does not exists. It should be put at model_path/depparser");
   cout << "[The segmentation and tagging model] "; cout.flush();
   CTagger tagger(sTaggerFeatureFile, false, MAX_SENTENCE_SIZE, "", false);
   cout << "[The parsing model] "; cout.flush();
   CDepParser depparser(sParserFeatureFile, false);
   CDoc2Snt doc2snt(sInputFile, MAX_SENTENCE_SIZE);
   CSentenceReader input_reader(sInputFile);
   CStringVector *input_sent = new CStringVector;
   CTwoStringVector *tagged_sent = new CTwoStringVector; 
   CDependencyTree *output_sent = new CDependencyTree; 
   cout << "ZPar initialized." << endl; cout.flush();

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
      configurations.defineConfiguration("o", "{s|t[d]|d|c}", "output format; 's' segmented format, 't' pos-tagged format in sentences, 'td' pos-tagged format in documents without sentence boundary delimination, 'd' refers to dependency parse tree format, and 'c' refers to constituent parse tree format", "c");

      if (options.args.size() < 2 || options.args.size() > 4) {
         cout << "\nUsage: " << argv[0] << " feature_path [input_file [output_file]]" << endl;
         cout << configurations.message() << endl;
         return 1;
      }
      string warning = configurations.loadConfigurations(options.opts);
      if (!warning.empty()) {
         cout << "Warning: " << warning << endl;
      }

      string sInputFile = options.args.size() > 2 ? options.args[2] : "";
      string sToFile = options.args.size() > 3 ? options.args[3] : "";
      string sOutFormat = configurations.getConfiguration("o");

      bool bOutDoc = (sOutFormat=="td");
  
      if (sOutFormat == "t" || sOutFormat == "td")
          tag(sInputFile, sToFile, options.args[1], bOutDoc);
      if (sOutFormat == "c" )
          parse(sInputFile, sToFile, options.args[1]);
      if (sOutFormat == "d" )
          depparse(sInputFile, sToFile, options.args[1]);
      return 0;
   } catch(const string&e) {cerr<<"Error: "<<e<<endl;return 1;}
}

