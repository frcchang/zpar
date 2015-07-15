// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                              *
 * main.cpp - main app of the Chinese tagger.                   *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2006.11                        *
 *                                                              *
 ****************************************************************/

#include "definitions.h"
#include "tagger.h"
#include "reader.h"
#include "writer.h"
#include "stdlib.h"

using namespace chinese;

/*----------------------------------------------------------------
 *
 * recordSegmentation - record a segmented sentence with bitarr.
 *
 * Input raw is the raw format of input tagged, it is provided
 * here because the caller already did UntagAndDesegmentSent
 *
 *---------------------------------------------------------------*/

void recordSegmentation(const CStringVector *raw, const CStringVector* segmented, CBitArray &retval) {
   std::vector<int> indice;
   indice.clear();
   for (int i=0; i<raw->size(); ++i) {
      for (int j=0; j<raw->at(i).size(); ++j)
         indice.push_back(i);
   }
   retval.clear();
   int start=0;
   for (int i=0; i<segmented->size(); ++i) {
      int word_start = indice[start];
      int word_end = indice[start+segmented->at(i).size()-1];
      start += segmented->at(i).size();
      retval.set(word_end);
   }
}

/*===============================================================
 *
 * decode
 *
 *==============================================================*/

void process(const std::string sInputFile, const std::string sOutputFile, const std::string sFeatureFile, const unsigned long nBest, const unsigned long nMaxSentSize, const bool bSegmented, bool bScores) {
   std::cerr << "Tagging started" << std::endl;
   int time_start = clock();
   CTagger tagger(sFeatureFile, false, nMaxSentSize, false);
   CSentenceReader input_reader(sInputFile);
   CSentenceWriter output_writer(sOutputFile);
   std::ofstream *score_writer;
   CStringVector *input_sent = new CStringVector;
   CTwoStringVector *output_sent;
   tagger::SCORE_TYPE *output_scores;

   int nCount=0;

   output_sent = new CTwoStringVector[nBest];
   if (bScores) {
      output_scores = new tagger::SCORE_TYPE[nBest];
      score_writer = new std::ofstream(std::string(sOutputFile+".scores").c_str());
   }
   else
      output_scores = NULL;

   CBitArray prunes(tagger::MAX_SENTENCE_SIZE);
   CStringVector *unsegmented_sent;
   if (bSegmented) unsegmented_sent = new CStringVector;

   static bool bReadSuccessful;
   if (bSegmented)
      bReadSuccessful = input_reader.readSegmentedSentence(input_sent);
   else
      bReadSuccessful = input_reader.readRawSentence(input_sent, true, false);

   static int i;

   // If we read segmented sentence, we will ignore spaces from input.
   while( bReadSuccessful ) {
      TRACE("Sentence " << nCount);
      ++ nCount;
      //
      // Find decoder output
      //
      if (bSegmented) {
         DesegmentSentence(input_sent, unsegmented_sent);
         recordSegmentation( unsegmented_sent, input_sent, prunes );
         // check sentence size
         if ( unsegmented_sent->size() >= tagger::MAX_SENTENCE_SIZE ) {
            std::cerr << "The size of the sentence is larger than the limit (" << tagger::MAX_SENTENCE_SIZE << "), skipping." << std::endl;
            for (i=0; i<nBest; ++i) {
               output_sent[i].clear();
               if (bScores) output_scores[i]=0;
            }
         }
         else
            tagger.tag(unsegmented_sent, output_sent, output_scores, nBest, &prunes);
      }
      else {
         if ( input_sent->size() >= tagger::MAX_SENTENCE_SIZE ) {
            std::cerr << "The size of the sentence is larger than the limit (" << tagger::MAX_SENTENCE_SIZE << "), skipping." << std::endl;
            for (i=0; i<nBest; ++i) {
               output_sent[i].clear();
               if (bScores) output_scores[i]=0;
            }
         }
         else
            tagger.tag(input_sent, output_sent, output_scores, nBest);
      }
      //
      // Ouptut sent
      //
      for (i=0; i<nBest; ++i) {
         output_writer.writeSentence(output_sent+i);
         if (bScores) (*score_writer)<<output_scores[i]<<std::endl;
      }
      if (bSegmented)
         bReadSuccessful = input_reader.readSegmentedSentence(input_sent);
      else
         bReadSuccessful = input_reader.readRawSentence(input_sent, true, false);
   }
   delete input_sent;
   delete [] output_sent;
   if (bScores) {
      delete [] output_scores;
      score_writer->close(); delete score_writer; // remove file
   }

   if (bSegmented) delete unsegmented_sent;

   std::cerr << "Tagging has finished successfully. Total time taken is: " << double(clock()-time_start)/CLOCKS_PER_SEC << std::endl;
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
      std::stringstream out; out << tagger::MAX_SENTENCE_SIZE;
//std::cout << out.str() << std::endl;
      configurations.defineConfiguration("m", "M", "maximum sentence size", out.str());
      configurations.defineConfiguration("n", "N", "N best list output", "1");
      configurations.defineConfiguration("s", "", "output scores", "");

      if (options.args.size() < 2 || options.args.size() > 4) {
         std::cout << "\nUsage: " << argv[0] << " feature_file [input_file [output_file]]" << std::endl;
         std::cout << configurations.message() << std::endl;
         return 1;
      }
      std::string warning = configurations.loadConfigurations(options.opts);
      if (!warning.empty()) {
         std::cout << "Warning: " << warning << std::endl;
      }

      unsigned long nBest, nMaxSentSize;
      if (!fromString(nMaxSentSize, configurations.getConfiguration("m"))) {
         std::cout <<"Error: the size of largest sentence is not integer." << std::endl; return 1;
      }
      if (!fromString(nBest, configurations.getConfiguration("n"))) {
         std::cout <<"Error: the number of N best is not integer." << std::endl; return 1;
      }
      bool bScores = configurations.getConfiguration("s").empty() ? false : true;
#ifdef SEGMENTED
      bool bSegmented = true;
#else
      bool bSegmented = false;
#endif

      std::string sInputFile = options.args.size() > 2 ? options.args[2] : "";
      std::string sToFile = options.args.size() > 3 ? options.args[3] : "";
      process(sInputFile, sToFile, options.args[1], nBest, nMaxSentSize, bSegmented, bScores);
      return 0;
   } catch(const std::string&e) { std::cerr << "Error: "<< e << std::endl; return 1;}
}

