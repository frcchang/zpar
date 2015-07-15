/*
 * Variant of ZPar for Spanish
 * Carlos Gomez-Rodriguez, Yue Zhang
 */

#define SIMPLE_HASH

#include "definitions.h"
#include "options.h"
#include "tagger.h"
#include "depparser.h"
#include "reader.h"
#include "writer.h"
#include "stdlib.h"

using namespace spanish;

#define MAX_SENTENCE_SIZE 512

/*===============================================================
 *
 * tag
 *
 *==============================================================*/

void tag(const std::string sInputFile, const std::string sOutputFile, const std::string sFeaturePath) {
   std::cerr << "Tagging started" << std::endl;
   int time_start = clock();
   std::string sTaggerFeatureFile = sFeaturePath + "/tagger";
   CTagger *tagger;
   tagger = new CTagger(sTaggerFeatureFile, false);
//   if (sKnowledgeBase.size())
//      tagger.loadTagDictionary(sKnowledgeBase);
   CSentenceReader input_reader(sInputFile);
   CSentenceWriter output_writer(sOutputFile);
   CStringVector *input_sent = new CStringVector;
   CTwoStringVector *output_sent;

   int nCount=0;

   const unsigned nBest = 1;
   output_sent = new CTwoStringVector[nBest];

   while( input_reader.readSegmentedSentenceAndTokenize(input_sent) ) {
      TRACE("Sentence " << nCount);
      ++ nCount;
      //
      // Find decoder output
      //
      tagger->tag(input_sent, output_sent, nBest, NULL);
      //
      // Ouptut sent
      //
      for (int i=0; i<nBest; ++i)
         output_writer.writeSentence(output_sent+i, '/');
   }
   delete input_sent;
   delete [] output_sent;

   delete tagger;

   std::cerr << "Tagging has finished successfully. Total time taken is: " << double(clock()-time_start)/CLOCKS_PER_SEC << std::endl;
}

/*===============================================================
 *
 * depparse
 *
 *==============================================================*/

void depparse(const std::string sInputFile, const std::string sOutputFile, const std::string sFeaturePath) {
   std::cerr << "Parsing started" << std::endl;
   int time_start = clock();
   int time_one;
   std::ostream *outs; if (sOutputFile=="") outs=&std::cout; else outs = new std::ofstream(sOutputFile.c_str());
   std::string sTaggerFeatureFile = sFeaturePath + "/tagger";
   std::string sParserFeatureFile = sFeaturePath + "/depparser";
   if (!FileExists(sTaggerFeatureFile))
      THROW("Tagger model does not exists. It should be put at model_path/tagger");
   if (!FileExists(sParserFeatureFile))
      THROW("Parser model does not exists. It should be put at model_path/depparser");
   std::cerr << "[tagger] ";
   CTagger tagger(sTaggerFeatureFile, false);
   std::cerr << "[parser] ";
   CDepParser depparser(sParserFeatureFile, false);
   CSentenceReader input_reader(sInputFile);
   CStringVector *input_sent = new CStringVector;
   CTwoStringVector *tagged_sent = new CTwoStringVector;
   CDependencyParse *parsed_sent = new CLabeledDependencyTree;
   CLabeledDependencyTree *labeled_sent = 0;

   unsigned nCount=0;

   CBitArray prunes(MAX_SENTENCE_SIZE);

   // If we read segmented sentence, we will ignore spaces from input.
   while( input_reader.readSegmentedSentenceAndTokenize(input_sent) ) {
//      TRACE("Sentence " << nCount);
      ++ nCount;
      time_one = clock();
      if ( input_sent->empty() )
         continue;
      else if ( input_sent->back()=="\n" ) {
         input_sent->pop_back();
      }
      tagger.tag(input_sent, tagged_sent, 1, NULL);
      depparser.parse(*tagged_sent, parsed_sent, 1, NULL);
      (*outs) << *parsed_sent;
      std::cerr << "Sentence " << nCount << " processed in " << double(clock()-time_one)/CLOCKS_PER_SEC << " sec." << std::endl;
   }
   delete input_sent;
   delete tagged_sent;
   delete parsed_sent;

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
      configurations.defineConfiguration("o", "{t|d}", "output format; 't' pos-tagged format in sentences, and 'd' refers to dependency parse tree format", "d");

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
      if (sOutFormat == "t")
         tag(sInputFile, sToFile, options.args[1]);//
      else if (sOutFormat == "d" )
         depparse(sInputFile, sToFile, options.args[1]);//
      return 0;
   } catch(const std::string&e) {
      std::cerr<<"Error: "<<e;
      std::cerr<<std::endl;
      return 1;
   }
}

