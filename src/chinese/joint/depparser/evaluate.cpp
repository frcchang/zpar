// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                              *
 * evaluate.cpp - the evalfor the joint depparser.              *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2008.10                        *
 *                                                              *
 ****************************************************************/

#include "reader.h"
#include "writer.h"

#include "reranker.h"

using namespace chinese;

/*----------------------------------------------------------------
 *
 * process - the training process
 *
 *---------------------------------------------------------------*/

void process(std::string sOutputFile, std::string sReferenceFile) {
   std::ifstream output_file(sOutputFile.c_str());
   std::ifstream reference_file(sReferenceFile.c_str());

   double fSeg, fTag, fPar, fParUnlabeled, fParIncPunc;

   CSentenceParsed output;
   CSentenceParsed reference;

   int nCount;

   nCount = 0;
   while( output_file ) {
      std::cout << "Sentence " << ++ nCount << std::endl;
      output_file >> output;
      reference_file >> reference;
      getFScore(output, reference, fSeg, fTag, fPar, fParUnlabeled, fParIncPunc);
      std::cout << "Segmentation F-score: " << fSeg << std::endl;
      std::cout << "Tag F-score: " << fTag << std::endl;
      std::cout << "Parse F-score: " << fPar << std::endl;
      std::cout << "Prase F-score excl tag: " << fParUnlabeled << std::endl;
   }
   output_file.close();
   reference_file.close();
}

/*===============================================================
 *
 * main
 *
 *==============================================================*/

int main(int argc, char* argv[]) {
   const std::string hint = " output_file rererence_file\n\n\
";
   if (argc < 3) {
      std::cout << "Usage: " << argv[0] << hint << std::endl;
      return 1;
   }

   process(argv[1], argv[2]);
   return 0;
}

