// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                              *
 * main.cpp - main app of the Chinese sentence segmentor.       *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2006.11                        *
 *                                                              *
 ****************************************************************/

#include "definitions.h"
#include "doc2snt.h"
#include "writer.h"
#include "options.h"

using namespace chinese ;

/*===============================================================
 *
 * main
 *
 *==============================================================*/

void process(const std::string &sInputFile, const std::string &sOutputFile, unsigned long nMaxSentSize) {
   CDoc2Snt doc2snt(sInputFile, nMaxSentSize);
   CSentenceWriter writer(sOutputFile);
   CStringVector sent;
   while (doc2snt.getSentence(sent)) {
      if (sent.size()>0 && sent.back()=="\n")
         sent.pop_back();
      writer.writeSentence(&sent, "");
      sent.clear();
   }
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
      configurations.defineConfiguration("m", "M", "the maximum sentence size in character count", "512");

      // check arguments
      if (options.args.size() > 3) {
         std::cout << "Usage: " << argv[0] << " input_file output_file" << std::endl;
         std::cout << configurations.message();
         return 1;
      }
      configurations.loadConfigurations(options.opts);

      unsigned long nMaxSentSize;
      std::string s = configurations.getConfiguration("m");
      if (!fromString(nMaxSentSize, s)) {
         std::cout << "Error: the maximum sentence size must be an integer." << std::endl;
         exit(1);
      }
      std::string sInputFile = options.args.size()>1 ? options.args[1] : "";
      std::string sOutputFile = options.args.size()>2 ? options.args[2] : "";
      // main
      process(sInputFile, sOutputFile, nMaxSentSize);

      // return normal
      return 0;
   }
   catch(const std::string &e) {
      std::cerr << "Error: " << e << " Stop." << std::endl;
      return 1;
   }
}

