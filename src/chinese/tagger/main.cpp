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

//
// The following definitions must be consistent with the combined segmentor and tagger
//
#define N 64
#define MAX_SENTENCE_SIZE 512

static string sKnowledgePath;

/*----------------------------------------------------------------
 *
 * recordSegmentation - record a segmented sentence with bitarr.
 * 
 * Input raw is the raw format of input tagged, it is provided
 * here because the caller already did UntagAndDesegmentSent
 * 
 *---------------------------------------------------------------*/

void recordSegmentation(const CSentenceRaw *raw, const CSentenceRaw* segmented, CBitArray &retval) {
   vector<int> indice;
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

void process(const string sInputFile, const string sOutputFile, const string sFeatureFile, const unsigned nBest, const unsigned nMaxSentSize, const bool bSegmented, bool bKnowledge, bool bScores) {
   cout << "Tagging started" << endl;
   int time_start = clock();
   CTagger tagger(sFeatureFile, false, nMaxSentSize);
   if (bKnowledge) tagger.loadKnowledge(sKnowledgePath);
   CSentenceReader input_reader(sInputFile);
   CSentenceWriter output_writer(sOutputFile);
   ofstream *score_writer;
   CSentenceRaw *input_sent = new CSentenceRaw;
   CSentenceTagged *output_sent; 
   tagger::SCORE_TYPE *output_scores;

   int nCount=0;
   
   output_sent = new CSentenceTagged[nBest];
   if (bScores) {
      output_scores = new tagger::SCORE_TYPE[nBest];
      score_writer = new ofstream(string(sOutputFile+".scores").c_str());
   }
   else
      output_scores = NULL;

   CBitArray prunes(MAX_SENTENCE_SIZE);
   CSentenceRaw *unsegmented_sent;
   if (bSegmented) unsegmented_sent = new CSentenceRaw;

   bool bReadSuccessful; 
   if (bSegmented)
      bReadSuccessful = input_reader.readSegmentedSentence(input_sent);
   else
      bReadSuccessful = input_reader.readRawSentence(input_sent, true, false);

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
         tagger.tag(unsegmented_sent, output_sent, output_scores, nBest, &prunes);
      }
      else {
         tagger.tag(input_sent, output_sent, output_scores, nBest);
      }
      //
      // Ouptut sent
      //
      for (int i=0; i<nBest; ++i) {
         output_writer.writeSentence(output_sent+i); 
         if (bScores) (*score_writer)<<output_scores[i]<<endl;
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

   cout << "Tagging has finished successfully. Total time taken is: " << double(clock()-time_start)/CLOCKS_PER_SEC << endl;
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
      configurations.defineConfiguration("k", "Path", "use knowledge from Path", "");
      configurations.defineConfiguration("m", "M", "maximum sentence size", "512");
      configurations.defineConfiguration("n", "N", "N best list output", "1");
      configurations.defineConfiguration("s", "", "output scores", "");

      if (options.args.size() != 4) {
         cout << "\nUsage: " << argv[0] << " input_file output_file feature_file " << endl;
         cout << configurations.message() << endl;
         return 1;
      }
      configurations.loadConfigurations(options.opts);
   
      unsigned nBest, nMaxSentSize;
      if (!fromString(nMaxSentSize, configurations.getConfiguration("m"))) {
         cerr<<"Error: the size of largest sentence is not integer." << endl; return 1;
      }  
      if (!fromString(nBest, configurations.getConfiguration("n"))) {
         cerr<<"Error: the number of N best is not integer." << endl; return 1;
      }  
      bool bKnowledge = configurations.getConfiguration("k").empty() ? false : true;
      bool bScores = configurations.getConfiguration("s").empty() ? false : true;
#ifdef SEGMENTED
      bool bSegmented = true;
#else
      bool bSegmented = false;
#endif
   
      process(options.args[1], options.args[2], options.args[3], nBest, nMaxSentSize, bSegmented, bKnowledge, bScores);
      return 0;
   } catch(const string&e) {cerr<<"Error: "<<e<<endl;return 1;}
}

