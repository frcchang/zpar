// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                              *
 * train.cpp - the training of the Chinese tagger.              *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2006.10                        *
 *                                                              *
 ****************************************************************/

#include "definitions.h"
#include "tagger.h"
#include "reader.h"
#include "writer.h"
#include "charcat.h"

using namespace chinese;

/*----------------------------------------------------------------
 *
 * recordSegmentation - record a segmented sentence with bitarr.
 *
 * Input raw is the raw format of input tagged, it is provided
 * here because the caller already did UntagAndDesegmentSent
 *
 *---------------------------------------------------------------*/

void recordSegmentation(const CStringVector *raw, const CTwoStringVector* tagged, CBitArray &retval) {
   std::vector<unsigned> indice;
   indice.clear();
   for (unsigned i=0; i<raw->size(); ++i) {
      for (unsigned j=0; j<raw->at(i).size(); ++j)
         indice.push_back(i);
   }
   retval.clear();
   unsigned start=0;
   for (unsigned i=0; i<tagged->size(); ++i) {
      unsigned word_start = indice[start];
      unsigned word_end = indice[start+tagged->at(i).first.size()-1];
      start += tagged->at(i).first.size();
      retval.set(word_end);
   }
}

/*===============================================================
 *
 * auto_train
 *
 *==============================================================*/

void auto_train(const std::string &sOutputFile, const std::string &sFeatureFile, const unsigned long &nBest, const unsigned long &nMaxSentSize, const std::string &sKnowledgePath, const bool &bFWCDRule) {
   static CCharCatDictionary charcat ; // don't know why there is a segmentation fault when this is put as a global variable. The error happens when charcat.h CCharcat() is called and in particular when (*this)[CWord(letters[i])] = eFW is executed (if an empty CWord(letters[i]) line is put before this line then everything is okay. Is it static initialization fiasco? Not sure really.

   CTagger decoder(sFeatureFile, true, nMaxSentSize, bFWCDRule);
   if (!sKnowledgePath.empty()) decoder.loadKnowledge(sKnowledgePath);
   CSentenceReader output_reader(sOutputFile);
   CStringVector *input_sent = new CStringVector;
   CTwoStringVector *output_sent = new CTwoStringVector;

   unsigned nCount=0;
   unsigned nErrorCount=0;

   CBitArray word_ends(tagger::MAX_SENTENCE_SIZE);

#ifdef DEBUG
   CSentenceWriter output_writer("");
#endif
   //
   // Read the next sentence
   //
   while( output_reader.readTaggedSentence(output_sent) ) {
      if (bFWCDRule)
         UntagAndDesegmentSentence(output_sent, input_sent, charcat);
      else
         UntagAndDesegmentSentence(output_sent, input_sent);
      TRACE("Sentence " << nCount);
#ifdef DEBUG
      output_writer.writeSentence(input_sent);
#endif
      ++nCount;
      //
      // Find the decoder output
      //
      if (decoder.train(input_sent, output_sent)) ++nErrorCount;
   }
   delete input_sent;
   delete output_sent;
   std::cerr << "Completing the training process." << std::endl;
   decoder.finishTraining(nCount);
   std::cerr << "Done. Total errors: " << nErrorCount << std::endl;
}

/*===============================================================
 *
 * dumpfeatures
 *
 *==============================================================*/

void dumpfeatures(const std::string &sOutputFile, const std::string &sFeatureFile, const unsigned long &nMaxSentSize, const std::string &sKnowledgePath, const bool &bFWCDRule) {
   static CCharCatDictionary charcat ; // don't know why there is a segmentation fault when this is put as a global variable. The error happens when charcat.h CCharcat() is called and in particular when (*this)[CWord(letters[i])] = eFW is executed (if an empty CWord(letters[i]) line is put before this line then everything is okay. Is it static initialization fiasco? Not sure really.

   CTagger decoder(sFeatureFile, true, nMaxSentSize, bFWCDRule);
   if (!sKnowledgePath.empty()) decoder.loadKnowledge(sKnowledgePath);
   CSentenceReader output_reader(sOutputFile);
   CStringVector *input_sent = new CStringVector;
   CTwoStringVector *output_sent = new CTwoStringVector;

   unsigned nCount=0;
   unsigned nErrorCount=0;

   //
   // Read the next sentence
   //
   while( output_reader.readTaggedSentence(output_sent) ) {
      if (bFWCDRule)
         UntagAndDesegmentSentence(output_sent, input_sent, charcat);
      else
         UntagAndDesegmentSentence(output_sent, input_sent);
      TRACE("Sentence " << nCount);
      ++nCount;
      //
      // Dump features now
      //
      decoder.dumpfeatures(input_sent, output_sent);
   }
   delete input_sent;
   decoder.finishTraining(nCount);
   std::cerr << "Done. " << std::endl;
}

/*===============================================================
 *
 * train
 *
 *==============================================================*/

void train(const std::string &sOutputFile, const std::string &sFeatureFile, const unsigned long &nBest, const unsigned long &nMaxSentSize, const bool &bEarlyUpdate, const bool &bSegmented, const std::string &sKnowledgePath, const bool &bFWCDRule) {
   static CCharCatDictionary charcat ; // don't know why there is a segmentation fault when this is put as a global variable. The error happens when charcat.h CCharcat() is called and in particular when (*this)[CWord(letters[i])] = eFW is executed (if an empty CWord(letters[i]) line is put before this line then everything is okay. Is it static initialization fiasco? Not sure really.

   CTagger decoder(sFeatureFile, true, nMaxSentSize, bFWCDRule);
   CSentenceReader output_reader(sOutputFile);
#ifdef DEBUG
   CSentenceWriter output_writer("");
   CSentenceWriter tagged_writer("");
#endif
   CStringVector *input_sent = new CStringVector;
   CTwoStringVector *tagged_sent = new CTwoStringVector[nBest];
   CTwoStringVector *output_sent = new CTwoStringVector;

   unsigned nCount=0;
   unsigned nErrorCount=0;
   unsigned nEarlyUpdateRepeat=0;

   CBitArray word_ends(tagger::MAX_SENTENCE_SIZE);

   //
   // Read the next sentence
   //
   while( output_reader.readTaggedSentence(output_sent) ) {
      if (bFWCDRule && !bSegmented)
         UntagAndDesegmentSentence(output_sent, input_sent, charcat);
      else
         UntagAndDesegmentSentence(output_sent, input_sent);
      TRACE("Sentence " << nCount);
      ++nCount;
      //
      // Find the decoder output
      //
      if (bSegmented) {
         recordSegmentation( input_sent, output_sent, word_ends );
         decoder.tag(input_sent, tagged_sent, NULL, nBest, &word_ends);
      }
      else {
         decoder.tag(input_sent, tagged_sent, NULL, nBest);
      }
      if (bEarlyUpdate) {
      //------------------------------------------------
      //
      // Early update
      //
      //------------------------------------------------
         if ( *tagged_sent != *output_sent )
            ++nErrorCount;

         nEarlyUpdateRepeat = 0;
         while ( *tagged_sent != *output_sent && nEarlyUpdateRepeat < 200 ) {
            //
            // Debug output
            //
#ifdef DEBUG
            output_writer.writeSentence(output_sent);
#endif
            TRACE("------");
            for (unsigned i=0; i<nBest; ++i) if (*(tagged_sent+i)!=*output_sent)
            {
#ifdef DEBUG
               tagged_writer.writeSentence(tagged_sent+i);
#endif
               decoder.updateScores(tagged_sent+i, output_sent, nCount);
            }
            //
            // Find the decoder output
            //
            if (bSegmented) {
               recordSegmentation( input_sent, output_sent, word_ends );
               decoder.tag(input_sent, tagged_sent, NULL, nBest, &word_ends);
            }
            else
               decoder.tag(input_sent, tagged_sent, NULL, nBest);

            nEarlyUpdateRepeat ++ ;
         } // while the sentence wrong
      }
      else {
      //------------------------------------------------
      //
      // Normal update
      //
      //------------------------------------------------
         if (*tagged_sent!=*output_sent) {
#ifdef DEBUG
            output_writer.writeSentence(input_sent);
            output_writer.writeSentence(output_sent);
#endif
            TRACE("------");
            ++nErrorCount;
         }
         for (unsigned i=0; i<nBest; ++i)
         {
#ifdef DEBUG
            if (*(tagged_sent+i)!=*output_sent) tagged_writer.writeSentence(tagged_sent+i);
#endif
            decoder.updateScores(tagged_sent+i, output_sent, nCount);
         }
      }
   }
   delete input_sent;
   delete output_sent;
   delete [] tagged_sent;
   std::cerr << "Completing the training process." << std::endl;
   decoder.finishTraining(nCount);
   std::cerr << "Done. Total errors: " << nErrorCount << std::endl;
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
      configurations.defineConfiguration("k", "Path", "use knowledge from the given path", "");
      std::ostringstream out; out << tagger::MAX_SENTENCE_SIZE;
      configurations.defineConfiguration("m", "M", "maximum sentence size", out.str());
      configurations.defineConfiguration("n", "N", "N best list train", "1");
      configurations.defineConfiguration("u", "", "early update", "");
      configurations.defineConfiguration("r", "", "do not use rules to segment numbers and letters", "");
      configurations.defineConfiguration("f", "", "dump features", "");

      if (options.args.size() != 4) {
         std::cout << "\nUsage: " << argv[0] << " training_data model num_iterations" << std::endl ;
         std::cout << configurations.message() << std::endl;
         return 1;
      }
      std::string warning = configurations.loadConfigurations(options.opts);
      if (!warning.empty()) {
         std::cout << "Warning: " << warning << std::endl;
      }

      unsigned long nBest, nMaxSentSize;
      if (!fromString(nMaxSentSize, configurations.getConfiguration("m"))) {
         std::cout <<"Error: the max size of sentence is not integer." << std::endl; return 1;
      }
      if (!fromString(nBest, configurations.getConfiguration("n"))) {
         std::cout <<"Error: the number of N best is not integer." << std::endl; return 1;
      }
      bool bEarlyUpdate = configurations.getConfiguration("u").empty() ? false : true;
      bool bFWCDRule = configurations.getConfiguration("r").empty() ? true : false;
      std::string sKnowledgePath = configurations.getConfiguration("k");
      bool bDumpFeatures = configurations.getConfiguration("f").empty() ? false : true;
      bool bSegmented = false;
#ifdef SEGMENTED
      bSegmented = true; // compile option
#endif

      unsigned training_rounds;
      if (!fromString(training_rounds, options.args[3])) {
         std::cout << "Error: the number of training iterations must be an integer." << std::endl;
         return 1;
      }
      std::cerr << "Training started." << std::endl;
      unsigned time_start = clock();
   if (bDumpFeatures) {
         // one single iteration: load knowledge
      std::cerr << "Dumping features; only one iteration; no early update." << std::endl;
         dumpfeatures(argv[1], argv[2], nMaxSentSize, sKnowledgePath, bFWCDRule);
   }
      else if (bSegmented) {
         // the first iteration: load knowledge
         train(argv[1], argv[2], nBest, nMaxSentSize, bEarlyUpdate, bSegmented, sKnowledgePath, bFWCDRule);
         // from the next iteration knowledge will be loaded from the model
         // and therefore sKnowledgePath is set to "". Thus separate 'for'
         for (unsigned i=1; i<training_rounds; ++i)
            train(argv[1], argv[2], nBest, nMaxSentSize, bEarlyUpdate, bSegmented, "", bFWCDRule);
      }
      else {
         // the first iteration: load knowledge
         auto_train(argv[1], argv[2], nBest, nMaxSentSize, sKnowledgePath, bFWCDRule);
         // from the next iteration knowledge will be loaded from the model
         // and therefore sKnowledgePath is set to "". Thus separate 'for'
         for (unsigned i=1; i<training_rounds; ++i)
            auto_train(argv[1], argv[2], nBest, nMaxSentSize, "", bFWCDRule);
      }
      std::cerr << "Training has finished successfully. Total time taken is: " << double(clock()-time_start)/CLOCKS_PER_SEC << std::endl;
      return 0;
   } catch (const std::string &e) {
      std::cerr << "Error: " << e << std::endl;
      return 1;
   }
}

