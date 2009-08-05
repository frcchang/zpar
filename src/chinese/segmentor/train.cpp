/****************************************************************
 *                                                              *
 * train.cpp - the training of the Chinese segmentor.           *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2006.10                        *
 *                                                              *
 ****************************************************************/

#include "segmentor.h"
#include "reader.h"
#include "writer.h"
#include "file_utils.h"

#include <cstring>

using namespace chinese;

const int TRAINING_ROUND = 1;

#ifdef NO_NEG_FEATURE

/*===============================================================
 *
 * extract_features - extract features from a training file. used only when no negative feat
 *
 *===============================================================*/

void extract_features(const string &sTextFile, const string &sFeatureFile) {
   if (FileExists(sFeatureFile))
      return;
   cout << "Extracting features...";
   CSegmentor *segmentor;
   segmentor = new CSegmentor(sFeatureFile, true);
   CSentenceRaw *ref_sent = new CSentenceRaw; 
   CSentenceReader ref_reader(sTextFile);
   
   while( ref_reader.readSegmentedSentence(ref_sent) ) {
      segmentor->extractPosFeatures(ref_sent);
   }
   segmentor->finishTraining(0);
   delete segmentor;
   delete ref_sent;
   cout << "done" << endl;
}

#endif

/*===============================================================
 *
 * auto_train - train by the segmentor itself, black-box training
 *
 *===============================================================*/

void auto_train(const string &sOutputFile, const string &sFeatureFile, const bool &bNoFWAndCD, const string &sCharCatFile, const string &sLexiconDict) {
   CSegmentor *segmentor;
   segmentor = new CSegmentor(sFeatureFile, true);
   if (!sCharCatFile.empty()) segmentor->loadCharCat(sCharCatFile);
   if (!sLexiconDict.empty()) segmentor->loadLexiconDict(sLexiconDict);
   CSentenceReader ref_reader(sOutputFile);
   CSentenceRaw *input_sent = new CSentenceRaw;
   CSentenceRaw *ref_sent = new CSentenceRaw; 
   CCharCatDictionary char_information;

   int nCount=0;
   
   while( ref_reader.readSegmentedSentence(ref_sent) ) {
      if ( ref_sent->empty() ) continue;
      if (bNoFWAndCD)
         DesegmentSentence(ref_sent, input_sent);
      else
         DesegmentSentence(ref_sent, input_sent, char_information);
      TRACE("Sentence " << nCount);
      ++nCount;
      segmentor->train(input_sent, ref_sent, nCount);
   }

   segmentor->finishTraining(nCount);
   delete input_sent;
   delete ref_sent;
   cout << "Done. " << endl;
}

/*===============================================================
 *
 * train
 *
 *==============================================================*/

void train(const string &sOutputFile, const string &sFeatureFile, const bool &bAggressive, const bool &bNoFWAndCD, const string &sCharCatFile, const string &sLexiconDict) {
   CSegmentor *segmentor ; 
   segmentor = new CSegmentor(sFeatureFile, true);
   if (!sCharCatFile.empty()) segmentor->loadCharCat(sCharCatFile);
   if (!sLexiconDict.empty()) segmentor->loadLexiconDict(sLexiconDict);
   CSentenceReader ref_reader(sOutputFile);
#ifdef DEBUG
   CSentenceWriter ref_writer("");
   CSentenceWriter output_writer("");
#endif
   CSentenceRaw *input_sent = new CSentenceRaw;
   CSentenceRaw *ref_sent = new CSentenceRaw; 
   CSentenceRaw *output_sent = new CSentenceRaw;
   CCharCatDictionary char_information;

   int nCount=0;
   int nErrorCount=0;
   
   while( ref_reader.readSegmentedSentence(ref_sent) ) {
      if ( ref_sent->empty() ) continue ;
      if ( bNoFWAndCD )
         DesegmentSentence( ref_sent, input_sent );
      else
         DesegmentSentence( ref_sent, input_sent, char_information );
      TRACE("Sentence " << nCount);
      ++nCount;
      segmentor->segment( input_sent, output_sent );
      segmentor->updateScores( output_sent, ref_sent, nCount );
      if ( *output_sent != *ref_sent ) {
         ++nErrorCount;
#ifdef DEBUG
         ref_writer.writeSentence(ref_sent);
         output_writer.writeSentence(output_sent);
#endif
         if (bAggressive) {
            while (true) {
               segmentor->segment(input_sent, output_sent);
               if (*output_sent != *ref_sent) {
                  segmentor->updateScores(output_sent, ref_sent, nCount); continue;
               }
               break;
            }
         }
      }
   }
   delete input_sent;
   delete ref_sent;
   delete output_sent;

   segmentor->finishTraining(nCount);
   cout << "Done. Total errors: " << nErrorCount << endl;

   delete segmentor;
}

/*===============================================================
 *
 * main
 *
 *==============================================================*/

int main(int argc, char* argv[]) {
   const string hint = " training_input_file feature_file number_of_iterations [-a] [-cFile] [-r] [-s] [-wFile]\n\n\
Options:\n\
-a agressive learning: make sure one example is segmented correctly before moving on to the next example\n\
-s implementation specific training by the segmentor, instead of the standard perceptron updating\n\
-c provide character type information in File\n\
-w privide word list in File\n\
-r use rules to segment English letters and Arabic numbers\n\
";
   if (argc < 4) {
      cout << "\nUsage: " << argv[0] << hint << endl;
      return 1;
   }
   int training_rounds = atoi(argv[3]);
   if (training_rounds < 0)
      training_rounds = TRAINING_ROUND;

   bool bAggressive = false;
   bool bAutomatic = false;
   string sCharCatFile = "";
   string sLexiconDict = "";
   bool bNoFWAndCD = true;
   if (argc>4) {
      for (int i=4; i<argc; i++) {
         if (argv[i][0]!='-') { cout << "\nUsage: " << argv[0] << hint << endl ; return 1; }
         switch (argv[i][1]) {
            case 'a':
               bAggressive = true;
               break;
            case 'r':
               bNoFWAndCD = false;
               break;
            case 's':
               bAutomatic = true;
               break;
            case 'c':
               if (strlen(argv[i])<3) { cout << "\nUsage: " << argv[0] << hint << endl ; return 1; }
               sCharCatFile = string(&(argv[i][2]));
               break;
            case 'w':
               if (strlen(argv[i])<3) { cout << "\nUsage: " << argv[0] << hint << endl ; return 1; }
               sLexiconDict = string(&(argv[i][2]));
               break;
            default:
               cout << "\nUsage: " << argv[0] << hint << endl ;
               return 1;
         }
      }
   }

   if (bAutomatic)
      if (bAggressive||bNoFWAndCD)
         cout << "Warning: all other configurations will be ignored since automatic training is chosen." << endl;

   cout << "Training started" << endl;
   int time_start = clock();

#ifdef NO_NEG_FEATURE
   extract_features(argv[1], argv[2]);
#endif

   for (int i=0; i<training_rounds; ++i)
      if (bAutomatic)
         auto_train(argv[1], argv[2], bNoFWAndCD, sCharCatFile, sLexiconDict);
      else
         train(argv[1], argv[2], bAggressive, bNoFWAndCD, sCharCatFile, sLexiconDict);
   cout << "Training has finished successfully. Total time taken is: " << double(clock()-time_start)/CLOCKS_PER_SEC << endl;
   return 0;
}

