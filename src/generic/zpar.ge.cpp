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

#include "zpar.h"
#include "libzpar.h"

using namespace generic;

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

      ZPAR * zpar = zpar_open_generic(options.args[1].c_str(), sOutFormat.c_str());
      std::string line;
      while ( std::getline(std::cin, line)) {
        size_t sz = line.size();
        char * output = zpar_process_generic(zpar, line.c_str(), &sz);
        std::cout << output << std::endl;
        int ret = zpar_release_generic(output);
      }
      zpar_close_generic(zpar);
      return 0;
   } catch(const std::string&e) {std::cerr<<"Error: "<<e<<std::endl;return 1;}
}
