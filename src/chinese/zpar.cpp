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

#include "zpar.h"

using namespace chinese;

#define JOINT_CONPARSER

int main(int argc, char* argv[]) {
   try {
      COptions options(argc, argv);
      CConfigurations configurations;
      configurations.defineConfiguration("o", "{s|t|d|c|z}", "outout format; 's' segmented format, 't' pos-tagged format in sentences, 'd' refers to dependency parse tree format, 'c' refers to constituent parse tree format, and 'z' refers to constituent parse tree format with character tags", "c");

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

      bool bOutDoc = (sOutFormat=="td");
      bool bSegmented = (sOutFormat=="s");
      bool bTagged = (sOutFormat=="t") || (sOutFormat=="td");
  
      ZPAR * zpar = zpar_open_chinese(options.args[1].c_str(), sOutFormat.c_str());
      std::string line;
      while ( std::getline(std::cin, line)) {
        size_t sz = line.size();
        char * output = zpar_process_chinese(zpar, line.c_str(), &sz);
        std::cout << output << std::endl;
        int ret = zpar_release_chinese(output);
      }
      zpar_close_chinese(zpar);
      return 0;
   } catch(const std::string&e) {std::cerr<<"Error: "<<e<<std::endl;return 1;}
}
