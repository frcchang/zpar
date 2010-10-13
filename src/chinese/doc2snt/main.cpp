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

void process(const string &sInputFile, const string &sOutputFile, unsigned long nMaxSentSize) {
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
         cout << "Usage: " << argv[0] << " input_file output_file" << endl;
         cout << configurations.message();
         return 1;
      }
      configurations.loadConfigurations(options.opts);
   
      unsigned long nMaxSentSize;
      string s = configurations.getConfiguration("m");
      if (!fromString(nMaxSentSize, s)) {
         cerr << "Error: the maximum sentence size must be an integer." << endl;
         exit(1);
      }
      string sInputFile = options.args.size()>1 ? options.args[1] : "";
      string sOutputFile = options.args.size()>2 ? options.args[2] : "";
      // main
      process(sInputFile, sOutputFile, nMaxSentSize);

      // return normal
      return 0;
   }
   catch(const string &e) {
      cerr << "Error: " << e << " Stop." << endl;
      return 1;
   }
}

