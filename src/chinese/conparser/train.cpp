// Copyright (C) SUTD 2013
/******************************************************************
 *                                                                *
 * train.cpp - train app of the chinese joint constituent parser. *
 *                                                                *
 * Author: Yue Zhang, Meishan Zhang                               *
 *                                                                *
 * SUTD 2013.1                                                    *
 *                                                                *
 ******************************************************************/

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

void auto_train(const std::string &sOutputFile, const std::string &sFeatureFile) {

   std::cout << "Training iteration is started... " << std::endl ; std::cout.flush();

   CConParser parser(sFeatureFile, conparser::MAX_SENTENCE_SIZE, true);

   std::ifstream is(sOutputFile.c_str());
   ASSERT(is.is_open(), "The training file is unaccessible.");


   static CSentenceParsed ref_sent; 

   int nCount=0;
   
   is >> ref_sent;
   while( ! ref_sent.empty() ) {
//      TRACE_WORD("Sentence " << nCount << " ... ");
      nCount ++;
      parser.train( ref_sent, nCount );
      if(nCount%1000==0)
      {
      	std::cout << nCount << " ";
      	std::cout.flush();
      }
//      TRACE("done.");
      is >> ref_sent;
   }

   std::cout << std::endl;

   parser.finishtraining();

   is.close();

   TRACE("Done. ");

}

/*===============================================================
 *
 * extract_features - train by the parser itself, black-box training
 *
 *===============================================================*/

#ifdef NO_NEG_FEATURE
void extract_features(const std::string &sOutputFile, const std::string &sFeatureFile) {

   std::cout << "Extracting feature... "; std::cout.flush();

   CConParser parser(sFeatureFile,  conparser::MAX_SENTENCE_SIZE, true);

   std::ifstream is(sOutputFile.c_str());
   ASSERT(is.is_open(), "The training file is unaccessible.");

   static CSentenceParsed ref_sent; 

   int nCount=0;
   
   is >> ref_sent;
   while( ! ref_sent.empty() ) {
   	nCount ++;
      parser.getPositiveFeatures( ref_sent );
      is >> ref_sent;
   }

   parser.finishtraining();

   is.close();

   std::cout << "done. " << std::endl;

}
#endif

/*===============================================================
 *
 * main
 *
 *==============================================================*/

int main(int argc, char* argv[]) {

   try {
      COptions options(argc, argv);
      CConfigurations configurations;
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
   
      std::cout << "Training started." << std::endl;
      int time_start = clock();
#ifdef NO_NEG_FEATURE
      if (!FileExists(options.args[2]))
         extract_features(options.args[1], options.args[2]); 
#endif     
      for (int i=0; i<training_rounds; ++i) {
         auto_train(options.args[1], options.args[2]); // set update tag dict false now
      }
      std::cout << "Training has finished successfully. Total time taken is: " << double(clock()-time_start)/CLOCKS_PER_SEC << std::endl;
   
   } catch (const std::string &e) {
      std::cerr << "Error: " << e << std::endl;
      exit(1);
   }

   return 0;

}

int test_main(int argc, char* argv[]) {
	try {
	      COptions options(argc, argv);
	      CConfigurations configurations;
	      configurations.defineConfiguration("f", "Format", "output format: 0, binary; 1, unbinary, all; 2, unbinary, constituent", "0");
	      // check arguments
	      if (options.args.size() != 3) {
	         std::cout << "Usage: " << argv[0] << " input_file outout_file" << std::endl;
	         std::cout << configurations.message() << std::endl;
	         return 1;
	      }
	      configurations.loadConfigurations(options.opts);

	      unsigned long nformat = 0;
	      if (!fromString(nformat, configurations.getConfiguration("f"))) {
	    	  std::cout << "The output format specification must be an integer." << std::endl;
	    	  return 1;
	      }

	      if(nformat != 0 && nformat != 1 && nformat != 2)
	      {
	    	  std::cout << "The output format specification must be 0,1 or 2." << std::endl;
	    	  return 1;
	      }

	      std::ifstream is(options.args[1].c_str());
	      std::ofstream os(options.args[2].c_str());

	      static CJointTree ref_sent;
	      int nCount=0;

	      is >> ref_sent;
	      while( ! ref_sent.empty() ) {
	    	  TRACE_WORD("Sentence " << nCount << " ... ");
	    	  nCount ++;

	    	  TRACE("done.");
	    	  if(nformat == 0)
	    	  {
	    		  os << ref_sent.str() << std::endl;
	    	  }
	    	  else if(nformat == 1)
	    	  {
	    		  std::string outstr = ref_sent.str_unbinarizedall();
	    		  os << outstr << std::endl;
	    	  }
	    	  else if(nformat == 2)
			  {
	    		  std::string outstr = ref_sent.str_unbinarized();
	    		  os << outstr << std::endl;
			  }
	    	  else
	    	  {
	    		  assert(false);
	    	  }

	    	  is >> ref_sent;
	      }

	      is.close();
	      os.close();

	}
	catch (const std::string &e) {
		std::cerr << "Error: " << e << std::endl;
	    exit(1);
	}


	return 0;
}

