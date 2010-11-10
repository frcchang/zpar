// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                              *
 * train.cpp - the training of the general constituent parser.  *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2007.8                         *
 *                                                              *
 ****************************************************************/

#include "definitions.h"
#include "conparser.h"
#include "reader.h"
#include "writer.h"

using namespace TARGET_LANGUAGE;

/*===============================================================
 *
 * auto_train - train by the parser itself, black-box training
 *
 *===============================================================*/

void auto_train(const std::string &sOutputFile, const std::string &sFeatureFile, const std::string &sBinaryRulePath, const std::string &sUnaryRulePath, const std::string &sConInputPath) {

   std::cout << "Training iteration is started ... " << std::endl ; std::cout.flush();

   CConParser parser(sFeatureFile, true);
   if (!sBinaryRulePath.empty()) 
      parser.LoadBinaryRules(sBinaryRulePath);
   if (!sUnaryRulePath.empty())
      parser.LoadUnaryRules(sUnaryRulePath);

   std::ifstream is(sOutputFile.c_str());
   ASSERT(is.is_open(), "The training file is unaccessible.");

   std::ifstream *cis;
   if (!sConInputPath.empty()) cis=new std::ifstream(sConInputPath.c_str());

   static CSentenceMultiCon<CConstituent> con_input;
   static CSentenceParsed ref_sent; 

   int nCount=0;
   
   is >> ref_sent;
   while( ! ref_sent.empty() ) {
      std::cout << "Sentence " << nCount << " ... ";
      nCount ++;
      if (!sConInputPath.empty()) {
         ASSERT((*cis) >> con_input, "No input provided for the sentence, though the input data is provided.");
         parser.train( con_input, ref_sent, nCount );
      }
      else {
         parser.train( ref_sent, nCount );
      }
      std::cout << "done." << std::endl;
      is >> ref_sent;
   }

   parser.finishtraining();

   std::cout << "Done. " << std::endl;

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
      configurations.defineConfiguration("b", "Path", "use only the binary rules from the given path", "");
      configurations.defineConfiguration("u", "Path", "use only the unary rules from the given path", "");
      configurations.defineConfiguration("c", "Path", "input with multiple constituents for terminal tokens", "");
      if (options.args.size() != 4) {
         std::cout << "\nUsage: " << argv[0] << " training_data model num_iterations" << std::endl ;
         std::cout << configurations.message() << std::endl;
         return 1;
      } 
   
      int training_rounds;
      if (!fromString(training_rounds, options.args[3])) {
         std::cerr << "Error: the number of training iterations must be an integer." << std::endl;
         return 1;
      }
      std::string warning = configurations.loadConfigurations(options.opts);
      if (!warning.empty()) {
         std::cout << "Warning: " << warning << std::endl;
      }

      std::string sBinaryRulePath = configurations.getConfiguration("b");
      std::string sUnaryRulePath = configurations.getConfiguration("u");
      std::string sConInputPath = configurations.getConfiguration("c");
   
      std::cout << "Training started." << std::endl;
      int time_start = clock();
      for (int i=0; i<training_rounds; ++i) {
         auto_train(argv[1], argv[2], sBinaryRulePath, sUnaryRulePath, sConInputPath); // set update tag dict false now
         if (i==0) { // do not apply rules in next iterations
            sBinaryRulePath.clear();
            sUnaryRulePath.clear();
         }
      }
      std::cout << "Training has finished successfully. Total time taken is: " << double(clock()-time_start)/CLOCKS_PER_SEC << std::endl;
   
   } catch (const std::string &e) {
      std::cerr << "Error: " << e << std::endl;
      exit(1);
   }

   return 0;

}

