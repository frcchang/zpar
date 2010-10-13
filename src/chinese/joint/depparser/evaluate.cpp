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

void process(string sOutputFile, string sReferenceFile) {
   ifstream output_file(sOutputFile.c_str());
   ifstream reference_file(sReferenceFile.c_str());

   double fSeg, fTag, fPar, fParUnlabeled, fParIncPunc;

   CSentenceParsed output;
   CSentenceParsed reference;
   
   int nCount;

   nCount = 0;
   while( output_file ) {
      cout << "Sentence " << ++ nCount << endl;
      output_file >> output;
      reference_file >> reference;
      getFScore(output, reference, fSeg, fTag, fPar, fParUnlabeled, fParIncPunc);
      cout << "Segmentation F-score: " << fSeg << endl;
      cout << "Tag F-score: " << fTag << endl;
      cout << "Parse F-score: " << fPar << endl;
      cout << "Prase F-score excl tag: " << fParUnlabeled << endl;
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
   const string hint = " output_file rererence_file\n\n\
";
   if (argc < 3) {
      cout << "Usage: " << argv[0] << hint << endl;
      return 1;
   }

   process(argv[1], argv[2]);
   return 0;
}

