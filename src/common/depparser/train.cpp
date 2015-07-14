// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                              *
 * train.cpp - the training of the general dependency parser.   *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2007.8                         *
 *                                                              *
 ****************************************************************/

#include "definitions.h"
#include "depparser.h"
#include "reader.h"
#include "writer.h"

using namespace TARGET_LANGUAGE;

/*===============================================================
 *
 * auto_train - train by the parser itself, black-box training
 *
 *===============================================================*/

void auto_train(const std::string &sOutputFile, const std::string &sFeatureFile, const bool &bRules, const std::string &sSuperPath, const bool &bCoNLL, const bool &bExtract, const std::string &sMetaPath) {

   std::cerr << "Training iteration is started..." << std::endl ; std::cerr.flush();

   CDepParser parser(sFeatureFile, true, bCoNLL);
   parser.setRules(bRules);
#ifdef SUPPORT_META_FEATURE_DEFINITION
   if (!sMetaPath.empty() )
      parser.loadMeta(sMetaPath);
#endif

   std::ifstream is(sOutputFile.c_str());
   assert(is.is_open());

   CDependencyParse ref_sent;
   CCoNLLOutput ref_conll;

   depparser::CSuperTag *supertags;
   std::ifstream *is_supertags=0;

   supertags = 0;
   if (!sSuperPath.empty()) {
      supertags = new depparser::CSuperTag();
      is_supertags = new std::ifstream(sSuperPath.c_str());
      parser.setSuperTags(supertags);
   }

   int nCount=0;

   while( bCoNLL ? is>>ref_conll : is>>ref_sent ) {

      TRACE("Sentence " << nCount);
      ++ nCount ;

      if (supertags) {
         supertags->setSentenceSize( bCoNLL ? ref_conll.size() : ref_sent.size() );
         (*is_supertags) >> *supertags;
      }

      // example
      if (bExtract) {
#ifdef SUPPORT_FEATURE_EXTRACTION
         if (bCoNLL)
            parser.extract_features_conll( ref_conll );
         else
            parser.extract_features( ref_sent );
#else
         ASSERT(false, "Internal error: feature extract not allowed but option set.");
#endif
      }
      else {
       if ( (bCoNLL && ref_conll.size() > depparser::MAX_SENTENCE_SIZE) ||
            (!bCoNLL && ref_sent.size() > depparser::MAX_SENTENCE_SIZE) ) {
          WARNING("The sentence is longer than system limitation, skipping it.");
        } else {
          if (bCoNLL) {
            parser.train_conll( ref_conll, nCount );
          } else {
            parser.train( ref_sent, nCount );
          }
        }
      }
   }

   parser.finishtraining();

   if (supertags) {
      delete supertags;
      is_supertags->close();
      delete is_supertags;
   }

   std::cerr << "Done. " << std::endl;

}

/*===============================================================
 *
 * main
 *
 *==============================================================*/

int main(int argc, char* argv[]) {

//TODO This is temporary! Allow user to enter their own lexicons.
#ifdef JOINT_MORPH
	std::cerr << "Loading lexicons...\n";
	bool bSuccess = TARGET_LANGUAGE::initLexicon("/home/cgomezr/en/train.conll",true);
	std::cerr << "Successfully loaded primary lexicon? " << bSuccess << "\n";
	bSuccess = english::initLemmaLexicon("/home/cgomezr/multext-lexicons/en/wordform-improved.txt");
	std::cerr << "Successfully loaded the lemma lexicon? " << bSuccess << "\n";
#endif

   try {
      COptions options(argc, argv);
      CConfigurations configurations;
      configurations.defineConfiguration("c", "", "process CoNLL format", "");
      configurations.defineConfiguration("p", "path", "supertags", "");
      configurations.defineConfiguration("r", "", "use rules", "");
#ifdef SUPPORT_FEATURE_EXTRACTION
      configurations.defineConfiguration("f", "", "extract features only: weights will be counts", "");
#endif
#ifdef SUPPORT_META_FEATURE_DEFINITION
      configurations.defineConfiguration("t", "path", "meta feature types", "");
#endif
      if (options.args.size() != 4) {
         std::cout << "\nUsage: " << argv[0] << " training_data model num_iterations" << std::endl ;
         std::cout << configurations.message() << std::endl;
         return 1;
      }
      configurations.loadConfigurations(options.opts);

      int training_rounds;
      if (!fromString(training_rounds, options.args[3])) {
         std::cout << "Error: the number of training iterations must be an integer." << std::endl;
         return 1;
      }

      bool bCoNLL = configurations.getConfiguration("c").empty() ? false : true;
      std::string sSuperPath = configurations.getConfiguration("p");
      bool bRules = configurations.getConfiguration("r").empty() ? false : true;
      bool bExtract = false;
#ifdef SUPPORT_FEATURE_EXTRACTION
      bExtract = configurations.getConfiguration("f").empty() ? false : true;
#endif
   std::string sMetaPath;
#ifdef SUPPORT_META_FEATURE_DEFINITION
      sMetaPath = configurations.getConfiguration("t");
#endif

      std::cout << "Training started" << std::endl;
      int time_start = clock();
      for (int i=0; i<training_rounds; ++i)
         auto_train(options.args[1], options.args[2], bRules, sSuperPath, bCoNLL, bExtract, sMetaPath);
      std::cout << "Training has finished successfully. Total time taken is: " << double(clock()-time_start)/CLOCKS_PER_SEC << std::endl;

      return 0;
   } catch (const std::string &e) {
      std::cerr << std::endl << "Error: " << e << std::endl;
      return 1;
   }

}

