/****************************************************************
 *                                                              *
 * bidirectional.cpp - based on segmented                       *
 *                                                              *
 * This is a reimplementation of Shen et al. 2007 guilded learn *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2007.10                        *
 *                                                              *
 ****************************************************************/

#include "tagger.h"

#include "sqlite3x.hpp"
using namespace sqlite3x;
using namespace chinese;
using namespace chinese::tagger;

/*===============================================================
 *
 * CFeatureHandle - handles the features for the tagger
 *
 *==============================================================*/

/*---------------------------------------------------------------
 *
 * getGlobalScore - get the global score for sentence
 * [NOT USED]
 *
 *--------------------------------------------------------------*/

SCORE_TYPE CFeatureHandle::getGlobalScore(const CStringVector* sentence, const CStateItem* item){
   assert(1==0); return 0;
}

/*---------------------------------------------------------------
 *
 * getLocalScore - get the local score for a word in sentence
 * [NOT USED]
 *
 *--------------------------------------------------------------*/

SCORE_TYPE CFeatureHandle::getLocalScore(const CStringVector* sentence, const CStateItem* item, int index){
   assert(1==0); return 0;
}

/*---------------------------------------------------------------
 *
 * loadScores - load scores from the file specified at constructor
 *              currently this uses database, but it can be modified
 *
 *--------------------------------------------------------------*/

void CFeatureHandle::loadScores() {
   loadScoreFor(m_sFeatureDB, 0);
   if (m_bTrain)
      loadScoreFor(m_sFeatureDB+".total", 1);
}

/*---------------------------------------------------------------
 *
 * saveScores - save scores back to database
 *
 *--------------------------------------------------------------*/

void CFeatureHandle::saveScores() {
   saveScoreFor(m_sFeatureDB, 0);
   if (m_bTrain)
      saveScoreFor(m_sFeatureDB+".total", 1);
}

/*---------------------------------------------------------------
 *
 * updateScoreVector - update the score vector by input
 *                     this is used in training to adjust params
 *
 * NOT USED
 *
 *--------------------------------------------------------------*/

void CFeatureHandle::updateScoreVector(CStringVector* output, CStringVector* correct, int round) {
   assert( 1 == 0 ) ;
}

/*---------------------------------------------------------------
 *
 * updateLocalFeatureVector - update the given feature vector with
 *                            the local feature vector for a given
 *                            sentence. This is a private member only 
 *                            used by updateGlobalFeatureVector and is
 *                            only used for training. 
 *
 * NOT USED
 *
 *--------------------------------------------------------------*/

void CFeatureHandle::updateLocalFeatureVector(SCORE_UPDATE method, CStringVector* output, int index, int round) { 
   assert( 1 == 0 ) ;
}

/*--------------------------------------------------------------
 *
 * loadScoreFor - load score from a certain file for a weight
 *                structure specifically.
 * 
 * Inputs: sFeatureDB - the feature database file
 *         which      - which vector current or total
 *
 *------------------------------------------------------------*/

void CFeatureHandle::loadScoreFor(string sFeatureDB, int which) {
   TRACE("Loading scores...");
   try {
      sqlite3_connection con(sFeatureDB);
      string s1, s2;
      int i1, i2;
      SCORE_TYPE score;
      {// word
         sqlite3_command cmd(con, "SELECT sWord, nScore FROM WordDictionary");
         sqlite3_reader reader = cmd.executereader();
         while (reader.read()) {
            s1 = reader.getstring(0);
            score = reader.getdouble(1);
            m_weights.m_mapSeenWords[s1][which] = score;
         }
      }
      TRACE("Loaded known words information");
      {// separatechars
         sqlite3_command cmd(con, "SELECT sTwoChar, nScore FROM SeparateChars");
         sqlite3_reader reader = cmd.executereader();
         while (reader.read()) {
            s1 = reader.getstring(0);
            score = reader.getdouble(1);
            if (score!=0){
               m_weights.m_mapSeparateChars[s1][which] = score; 
            }
         }
      }
      TRACE("Loaded separate char information");
      {// firstandlastchars
         sqlite3_command cmd(con, "SELECT sChar1, sChar2, nScore FROM FirstAndLastChars");
         CTwoWords s1_s2;
         sqlite3_reader reader = cmd.executereader();
         while (reader.read()) {
            s1 = reader.getstring(0);
            s2 = reader.getstring(1);
            score = reader.getdouble(2);
            if (score!=0){
               s1_s2.allocate(s1, s2);
               m_weights.m_mapFirstAndLastChars[s1_s2][which] = score; 
               s1_s2.clear();
            }
         }
      }
      TRACE("Loaded first and last chars information");
      {// firstandcurrentchars
         sqlite3_command cmd(con, "SELECT sChar1, sChar2, nScore FROM FirstAndCurrentChars");
         CTwoWords s1_s2;
         sqlite3_reader reader = cmd.executereader();
         while (reader.read()) {
            s1 = reader.getstring(0);
            s2 = reader.getstring(1);
            score = reader.getdouble(2);
            if (score!=0){
               s1_s2.allocate(s1, s2);
               m_weights.m_mapFirstAndCurrentChars[s1_s2][which] = score; 
               s1_s2.clear();
            }
         }
      }
      TRACE("Loaded first and current chars information");
      {// firsttwochar
         sqlite3_command cmd(con, "SELECT sTwoChar, nScore FROM ConsecutiveChars");
         sqlite3_reader reader = cmd.executereader();
         while (reader.read()) {
            s1 = reader.getstring(0);
            score = reader.getdouble(1);
            if (score!=0){
               m_weights.m_mapConsecutiveChars[s1][which] = score; 
            }
         }
      }
      TRACE("Loaded first two chars information");
      {// firstcharlastwordbyword
         sqlite3_command cmd(con, "SELECT sFirstCharLastWord, sWord, nScore FROM FirstCharLastWordByWord");
         CTwoWords s1_s2;
         sqlite3_reader reader = cmd.executereader();
         while (reader.read()) {
            s1 = reader.getstring(0);
            s2 = reader.getstring(1);
            score = reader.getdouble(2);
            if (score!=0){
               s1_s2.allocate(s1, s2);
               m_weights.m_mapFirstCharLastWordByWord[s1_s2][which] = score; 
               s1_s2.clear();
            }
         }
      }
      TRACE("Loaded first char last word and current word information");
      {// lastwordbylastchar
         sqlite3_command cmd(con, "SELECT sLastWord, sLastChar, nScore FROM LastWordByLastChar");
         CTwoWords s1_s2;
         sqlite3_reader reader = cmd.executereader();
         while (reader.read()) {
            s1 = reader.getstring(0);
            s2 = reader.getstring(1);
            score = reader.getdouble(2);
            if (score!=0){
               s1_s2.allocate(s1, s2);
               m_weights.m_mapLastWordByLastChar[s1_s2][which] = score; 
               s1_s2.clear();
            }
         }
      }
      TRACE("Loaded last word and last char information");
      {// word
         sqlite3_command cmd(con, "SELECT sChar, nScore FROM OneCharWord");
         sqlite3_reader reader = cmd.executereader();
         while (reader.read()) {
            s1 = reader.getstring(0);
            score = reader.getdouble(1);
            if (score!=0)
               m_weights.m_mapOneCharWord[s1][which] = score;
         }
      }
      TRACE("Loaded the length one words information");
      {// lastwordfirstchar
         sqlite3_command cmd(con, "SELECT sLastWord, sFirstChar, nScore FROM LastWordFirstChar");
         CTwoWords s1_s2;
         sqlite3_reader reader = cmd.executereader();
         while (reader.read()) {
            s1 = reader.getstring(0);
            s2 = reader.getstring(1);
            score = reader.getdouble(2);

            if (score!=0){
               s1_s2.allocate(s1, s2);
               m_weights.m_mapLastWordFirstChar[s1_s2][which] = score; 
               s1_s2.clear();
            }
         }
      }
      TRACE("Loaded last word first char information");
      {// lastwordfirstchar
         sqlite3_command cmd(con, "SELECT sCurrentWord, sLastChar, nScore FROM CurrentWordLastChar");
         CTwoWords s1_s2;
         sqlite3_reader reader = cmd.executereader();
         while (reader.read()) {
            s1 = reader.getstring(0);
            s2 = reader.getstring(1);
            score = reader.getdouble(2);

            if (score!=0){
               s1_s2.allocate(s1, s2);
               m_weights.m_mapCurrentWordLastChar[s1_s2][which] = score; 
               s1_s2.clear();
            }
         }
      }
      TRACE("Loaded current word last char information");
      {// lengthbyfirstchar
         sqlite3_command cmd(con, "SELECT sFirstChar, nLength, nScore FROM LengthByFirstChar");
         sqlite3_reader reader = cmd.executereader();
         while (reader.read()) {
            s1 = reader.getstring(0); 
            i2 = reader.getint(1);
            score = reader.getdouble(2);
            if(score!=0)                        // ignore empty score
               m_weights.m_mapLengthByFirstChar[i2][s1][which] = score;
         }
      }
      TRACE("Loaded first char information");
      {// lengthbylastchar
         sqlite3_command cmd(con, "SELECT sLastChar, nLength, nScore FROM LengthByLastChar");
         sqlite3_reader reader = cmd.executereader();
         while (reader.read()) {
            s1 = reader.getstring(0); 
            i2 = reader.getint(1);
            score = reader.getdouble(2);
            if(score!=0)                        // ignore empty score
               m_weights.m_mapLengthByLastChar[i2][s1][which] = score;
         }
      }
      TRACE("Loaded last char information");
      {// lengthbychar
         sqlite3_command cmd(con, "SELECT sChar, nLength, nScore FROM LengthByChar");
         sqlite3_reader reader = cmd.executereader();
         while (reader.read()) {
            s1 = reader.getstring(0); 
            i2 = reader.getint(1);
            score = reader.getdouble(2);
            if(score!=0)                        // ignore empty score
               m_weights.m_mapLengthByChar[i2][s1][which] = score;
         }
      }
      TRACE("Loaded length by any char information");
      {// lengthbylastword
         sqlite3_command cmd(con, "SELECT sLastWord, nLength, nScore FROM LengthByLastWord");
         sqlite3_reader reader = cmd.executereader();
         while (reader.read()) {
            s1 = reader.getstring(0); 
            i2 = reader.getint(1);
            score = reader.getdouble(2);
            if(score!=0)                        // ignore empty score
               m_weights.m_mapLengthByLastWord[i2][s1][which] = score;
         }
      }
      {// lastlengthbyword
         sqlite3_command cmd(con, "SELECT sWord, nLastLength, nScore FROM LastLengthByWord");
         sqlite3_reader reader = cmd.executereader();
         while (reader.read()) {
            s1 = reader.getstring(0); 
            i2 = reader.getint(1);
            score = reader.getdouble(2);
            if(score!=0)                        // ignore empty score
               m_weights.m_mapLastLengthByWord[i2][s1][which] = score;
         }
      }
      TRACE("Loaded last length by word information");
      {// lastwordbyword
         sqlite3_command cmd(con, "SELECT sWord, sLastWord, nScore FROM LastWordByWord");
         //pair<CTwoWords, int> par;
         CTwoWords s1_s2;
         sqlite3_reader reader = cmd.executereader();
         while (reader.read()) {
            s1 = reader.getstring(0);
            s2 = reader.getstring(1);
            score = reader.getdouble(2);

            if (score!=0){
               s1_s2.allocate(s1, s2);
               m_weights.m_mapLastWordByWord[s1_s2][which] = score; 
               s1_s2.clear();
            }
         }
      }
      TRACE("Loaded last word by word information");
      {// lasttwocharscurrentword
         sqlite3_command cmd(con, "SELECT sWord, sLastTwoChar, nScore FROM LastTwoCharsByWord");
         sqlite3_reader reader = cmd.executereader();
         while (reader.read()) {
            s1 = reader.getstring(0);
            s2 = reader.getstring(1);
            score = reader.getdouble(2);
            if (score!=0)
               m_weights.m_mapWordByPrevTwoChars[s2][which] = score; 
         }
      }
      TRACE("Loaded last two chars and current word information");
      {// nexttwocharcurrentword
         sqlite3_command cmd(con, "SELECT sWord, sNextTwoChar, nScore FROM NextTwoCharsByWord");
         sqlite3_reader reader = cmd.executereader();
         while (reader.read()) {
            s1 = reader.getstring(0);
            s2 = reader.getstring(1);
            score = reader.getdouble(2);
            if (score!=0)
               m_weights.m_mapWordByNextTwoChars[s2][which] = score; 
         }
      }
      TRACE("Loaded next two chars and current word information");
      {// separatechars1
         sqlite3_command cmd(con, "SELECT sTwoChar, nScore FROM SeparateChars1");
         sqlite3_reader reader = cmd.executereader();
         while (reader.read()) {
            s1 = reader.getstring(0);
            score = reader.getdouble(1);
            if (score!=0)
               m_weights.m_mapSeparateChars1[s1][which] = score;
         }
      }
      TRACE("Loaded separate chars from the other side information");
      {// threechar
         sqlite3_command cmd(con, "SELECT sThreeChar, nScore FROM ThreeChars");
         sqlite3_reader reader = cmd.executereader();
         while (reader.read()) {
            s1 = reader.getstring(0);
            score = reader.getdouble(1);
            if (score!=0)
               m_weights.m_mapThreeChars[s1][which] = score;
         }
      }
      TRACE("Loaded the three char window around one char word information");
      {// word ln
         for (i1 = 0; i1 < LENGTH_MAX; i1 ++) m_weights.m_mapWordLength[i1][which] = 0 ; 
         sqlite3_command cmd(con, "SELECT nLength, nScore FROM WordLength");
         sqlite3_reader reader = cmd.executereader();
         while (reader.read()) {
            i1 = reader.getint(0);
            score = reader.getdouble(1);
            if (score!=0)
               m_weights.m_mapWordLength[i1][which] = score;
         }
      }
      TRACE("Loaded len of word information");


      m_weights.m_maxLength = 1;
      {// max word 
         sqlite3_command cmd(con, "SELECT nLength FROM LengthByTag WHERE sTag = \"-NONE-\"");
         sqlite3_reader reader = cmd.executereader();
         while (reader.read()) {
            i1 = reader.getint(0);
            if (i1>1)
               m_weights.m_maxLength = i1;
         }
      }
      TRACE("Loaded the maximum word length information");

      con.close();
   }
   catch(exception &e) {
      REPORT(e.what());
   }
   TRACE("Done");
}

/*--------------------------------------------------------------
 *
 * saveScoreFor - save score from specific weight structure
 *                to some specific file. 
 *
 * Inputs: sFeatureDB - file for the data
 *         which      - data source, total or the current
 *
 *-------------------------------------------------------------*/

void CFeatureHandle::saveScoreFor(string sFeatureDB, int which) {
   TRACE("saving scores.");
   try {
      sqlite3_connection con(sFeatureDB);
      CWeight::CTwoWordsMap::iterator it_feature;
      CWeight::CWordMap::iterator it; 
      int length;
      SCORE_TYPE nScore;

      {
         sqlite3_transaction trans(con);
         sqlite3_command cmd(con, "DELETE FROM WordDictionary");
         cmd.executenonquery();
         for (it = m_weights.m_mapSeenWords.begin(); it != m_weights.m_mapSeenWords.end(); ++it) {
            nScore = it.second().score(which);
            sqlite3_command cmd(con, "INSERT INTO WordDictionary (nScore, sWord) VALUES (?, ?)");
            cmd.bind(1, nScore);
            cmd.bind(2, wordToString(it.first()));
            cmd.executenonquery();
         }
         trans.commit();
      }
 
      {
         sqlite3_transaction trans(con);
         sqlite3_command cmd(con, "DELETE FROM LastWordByWord");
         cmd.executenonquery();
         for (it_feature = m_weights.m_mapLastWordByWord.begin(); it_feature != m_weights.m_mapLastWordByWord.end(); ++it_feature) {
            nScore = it_feature.second().score(which);
            if (nScore!=0) {
               sqlite3_command cmd(con, "INSERT INTO LastWordByWord (nScore, sWord, sLastWord) VALUES (?,?,?)");
               cmd.bind(1, nScore);
               cmd.bind(2, it_feature.first().first()->str());
               cmd.bind(3, it_feature.first().second()->str());
               cmd.executenonquery();
            }
         }
         trans.commit();
      }
 
      {
         sqlite3_transaction trans(con);
         sqlite3_command cmd(con, "DELETE FROM OneCharWord");
         cmd.executenonquery();
         for (it = m_weights.m_mapOneCharWord.begin(); it != m_weights.m_mapOneCharWord.end(); ++it) {
            nScore = it.second().score(which);
            if (nScore!=0) {
               sqlite3_command cmd(con, "INSERT INTO OneCharWord (nScore, sChar) VALUES (?, ?)");
               cmd.bind(1, nScore);
               cmd.bind(2, wordToString(it.first()));
               cmd.executenonquery();
            }
         }
         trans.commit();
      }
 
      {
         sqlite3_transaction trans(con);
         sqlite3_command cmd(con, "DELETE FROM LastWordFirstChar");
         cmd.executenonquery();
         for (it_feature = m_weights.m_mapLastWordFirstChar.begin(); it_feature != m_weights.m_mapLastWordFirstChar.end(); ++it_feature) {
            nScore = it_feature.second().score(which);
            if (nScore!=0) {
               sqlite3_command cmd(con, "INSERT INTO LastWordFirstChar (nScore, sLastWord, sFirstChar) VALUES (?,?,?)");
               cmd.bind(1, nScore);
               cmd.bind(2, it_feature.first().first()->str());
               cmd.bind(3, it_feature.first().second()->str());
               cmd.executenonquery();
            }
         }
         trans.commit();
      }
 
      {
         sqlite3_transaction trans(con);
         sqlite3_command cmd(con, "DELETE FROM CurrentWordLastChar");
         cmd.executenonquery();
         for (it_feature = m_weights.m_mapCurrentWordLastChar.begin(); it_feature != m_weights.m_mapCurrentWordLastChar.end(); ++it_feature) {
            nScore = it_feature.second().score(which);
            if (nScore!=0) {
               sqlite3_command cmd(con, "INSERT INTO CurrentWordLastChar (nScore, sCurrentWord, sLastChar) VALUES (?,?,?)");
               cmd.bind(1, nScore);
               cmd.bind(2, it_feature.first().first()->str());
               cmd.bind(3, it_feature.first().second()->str());
               cmd.executenonquery();
            }
         }
         trans.commit();
      }

      {
         sqlite3_transaction trans(con);
         sqlite3_command cmd(con, "DELETE FROM SeparateChars");
         cmd.executenonquery();
         for (it = m_weights.m_mapSeparateChars.begin(); it != m_weights.m_mapSeparateChars.end(); ++it) {
            nScore = it.second().score(which);
            if (nScore!=0) {
               sqlite3_command cmd(con, "INSERT INTO SeparateChars (nScore, sTwoChar) VALUES (?,?)");
               cmd.bind(1, nScore);
               cmd.bind(2, it.first().str());
               cmd.executenonquery();
            }
         }
         trans.commit();
      }

      {
         sqlite3_transaction trans(con);
         sqlite3_command cmd(con, "DELETE FROM FirstAndLastChars");
         cmd.executenonquery();
         for (it_feature = m_weights.m_mapFirstAndLastChars.begin(); it_feature != m_weights.m_mapFirstAndLastChars.end(); ++it_feature) {
            nScore = it_feature.second().score(which);
            if (nScore!=0) {
               sqlite3_command cmd(con, "INSERT INTO FirstAndLastChars (nScore, sChar1, sChar2) VALUES (?,?,?)");
               cmd.bind(1, nScore);
               cmd.bind(2, it_feature.first().first()->str());
               cmd.bind(3, it_feature.first().second()->str());
               cmd.executenonquery();
            }
         }
         trans.commit();
      }

      {
         sqlite3_transaction trans(con);
         sqlite3_command cmd(con, "DELETE FROM FirstAndCurrentChars");
         cmd.executenonquery();
         for (it_feature = m_weights.m_mapFirstAndCurrentChars.begin(); it_feature != m_weights.m_mapFirstAndCurrentChars.end(); ++it_feature) {
            nScore = it_feature.second().score(which);
            if (nScore!=0) {
               sqlite3_command cmd(con, "INSERT INTO FirstAndCurrentChars (nScore, sChar1, sChar2) VALUES (?,?,?)");
               cmd.bind(1, nScore);
               cmd.bind(2, it_feature.first().first()->str());
               cmd.bind(3, it_feature.first().second()->str());
               cmd.executenonquery();
            }
         }
         trans.commit();
      }

      {
         sqlite3_transaction trans(con);
         sqlite3_command cmd(con, "DELETE FROM ConsecutiveChars");
         cmd.executenonquery();
         for (it = m_weights.m_mapConsecutiveChars.begin(); it != m_weights.m_mapConsecutiveChars.end(); ++it) {
            nScore = it.second().score(which);
            if (nScore!=0) {
               sqlite3_command cmd(con, "INSERT INTO ConsecutiveChars (nScore, sTwoChar) VALUES (?,?)");
               cmd.bind(1, nScore);
               cmd.bind(2, it.first().str());
               cmd.executenonquery();
            }
         }
         trans.commit();
      }

      {
         sqlite3_transaction trans(con);
         sqlite3_command cmd(con, "DELETE FROM FirstCharLastWordByWord");
         cmd.executenonquery();
         for (it_feature = m_weights.m_mapFirstCharLastWordByWord.begin(); it_feature != m_weights.m_mapFirstCharLastWordByWord.end(); ++it_feature) {
            nScore = it_feature.second().score(which);
            if (nScore!=0) {
               sqlite3_command cmd(con, "INSERT INTO FirstCharLastWordByWord (nScore, sFirstCharLastWord, sWord) VALUES (?,?,?)");
               cmd.bind(1, nScore);
               cmd.bind(2, it_feature.first().first()->str());
               cmd.bind(3, it_feature.first().second()->str());
               cmd.executenonquery();
            }
         }
         trans.commit();
      }

      {
         sqlite3_transaction trans(con);
         sqlite3_command cmd(con, "DELETE FROM LastWordByLastChar");
         cmd.executenonquery();
         for (it_feature = m_weights.m_mapLastWordByLastChar.begin(); it_feature != m_weights.m_mapLastWordByLastChar.end(); ++it_feature) {
            nScore = it_feature.second().score(which);
            if (nScore!=0) {
               sqlite3_command cmd(con, "INSERT INTO LastWordByLastChar (nScore, sLastWord, sLastChar) VALUES (?,?,?)");
               cmd.bind(1, nScore);
               cmd.bind(2, it_feature.first().first()->str());
               cmd.bind(3, it_feature.first().second()->str());
               cmd.executenonquery();
            }
         }
         trans.commit();
      }

      {
         sqlite3_transaction trans(con);
         sqlite3_command cmd(con, "DELETE FROM LengthByFirstChar");
         cmd.executenonquery();
         for (length=0; length<LENGTH_MAX; length++) {
            for (it = m_weights.m_mapLengthByFirstChar[length].begin(); it != m_weights.m_mapLengthByFirstChar[length].end(); ++it) {
               nScore = it.second().score(which);
               if(nScore!=0) {
                  sqlite3_command cmd(con, "INSERT INTO LengthByFirstChar (nScore, nLength, sFirstChar) VALUES (?, ?, ?)");
                  cmd.bind(1, nScore);
                  cmd.bind(2, length);
                  cmd.bind(3, wordToString(it.first()));
                  cmd.executenonquery();
               }
            }
         }
         trans.commit();
      }

      {
         sqlite3_transaction trans(con);
         sqlite3_command cmd(con, "DELETE FROM LengthByLastChar");
         cmd.executenonquery();
         for (length=0; length<LENGTH_MAX; length++) {
            for (it = m_weights.m_mapLengthByLastChar[length].begin(); it != m_weights.m_mapLengthByLastChar[length].end(); ++it) {
               nScore = it.second().score(which);
               if(nScore!=0) {
                  sqlite3_command cmd(con, "INSERT INTO LengthByLastChar (nScore, nLength, sLastChar) VALUES (?, ?, ?)");
                  cmd.bind(1, nScore);
                  cmd.bind(2, length);
                  cmd.bind(3, wordToString(it.first()));
                  cmd.executenonquery();
               }
            }
         }
         trans.commit();
      }
 
      {
         sqlite3_transaction trans(con);
         sqlite3_command cmd(con, "DELETE FROM LengthByChar");
         cmd.executenonquery();
         for (length=0; length<LENGTH_MAX; length++) {
            for (it = m_weights.m_mapLengthByChar[length].begin(); it != m_weights.m_mapLengthByChar[length].end(); ++it) {
               nScore = it.second().score(which);
               if(nScore!=0) {
                  sqlite3_command cmd(con, "INSERT INTO LengthByChar (nScore, nLength, sChar) VALUES (?, ?, ?)");
                  cmd.bind(1, nScore);
                  cmd.bind(2, length);
                  cmd.bind(3, wordToString(it.first()));
                  cmd.executenonquery();
               }
            }
         }
         trans.commit();
      }
 
      {
         sqlite3_transaction trans(con);
         sqlite3_command cmd(con, "DELETE FROM LengthByLastWord");
         cmd.executenonquery();
         for (length=0; length<LENGTH_MAX; length++) {
            for (it = m_weights.m_mapLengthByLastWord[length].begin(); it != m_weights.m_mapLengthByLastWord[length].end(); ++it) {
               nScore = it.second().score(which);
               if(nScore!=0) {
                  sqlite3_command cmd(con, "INSERT INTO LengthByLastWord (nScore, nLength, sLastWord) VALUES (?, ?, ?)");
                  cmd.bind(1, nScore);
                  cmd.bind(2, length);
                  cmd.bind(3, wordToString(it.first()));
                  cmd.executenonquery();
               }
            }
         }
         trans.commit();
      }

      {
         sqlite3_transaction trans(con);
         sqlite3_command cmd(con, "DELETE FROM LastLengthByWord");
         cmd.executenonquery();
         for (length=0; length<LENGTH_MAX; length++) {
            for (it = m_weights.m_mapLastLengthByWord[length].begin(); it != m_weights.m_mapLastLengthByWord[length].end(); ++it) {
               nScore = it.second().score(which);
               if(nScore!=0) {
                  sqlite3_command cmd(con, "INSERT INTO LastLengthByWord (nScore, nLastLength, sWord) VALUES (?, ?, ?)");
                  cmd.bind(1, nScore);
                  cmd.bind(2, length);
                  cmd.bind(3, wordToString(it.first()));
                  cmd.executenonquery();
               }
            }
         }
         trans.commit();
      }

      {
         sqlite3_transaction trans(con);
         sqlite3_command cmd(con, "DELETE FROM LastTwoCharsByWord");
         cmd.executenonquery();
         for (it = m_weights.m_mapWordByPrevTwoChars.begin(); it != m_weights.m_mapWordByPrevTwoChars.end(); ++it) {
            nScore = it.second().score(which);
            if (nScore!=0) {
               sqlite3_command cmd(con, "INSERT INTO LastTwoCharsByWord (nScore, sWord, sLastTwoChar) VALUES (?,?,?)");
               cmd.bind(1, nScore);
               cmd.bind(2, "");
               cmd.bind(3, it.first().str());
               cmd.executenonquery();
            }
         }
         trans.commit();
      }

      {
         sqlite3_transaction trans(con);
         sqlite3_command cmd(con, "DELETE FROM NextTwoCharsByWord");
         cmd.executenonquery();
         for (it = m_weights.m_mapWordByNextTwoChars.begin(); it != m_weights.m_mapWordByNextTwoChars.end(); ++it) {
            nScore = it.second().score(which);
            if (nScore!=0) {
               sqlite3_command cmd(con, "INSERT INTO NextTwoCharsByWord (nScore, sWord, sNextTwoChar) VALUES (?,?,?)");
               cmd.bind(1, nScore);
               cmd.bind(2, "");
               cmd.bind(3, it.first().str());
               cmd.executenonquery();
            }
         }
         trans.commit();
      }

      {
         sqlite3_transaction trans(con);
         sqlite3_command cmd(con, "DELETE FROM SeparateChars1");
         cmd.executenonquery();
         for (it = m_weights.m_mapSeparateChars1.begin(); it != m_weights.m_mapSeparateChars1.end(); ++it) {
            nScore = it.second().score(which);
            if (nScore!=0) {
               sqlite3_command cmd(con, "INSERT INTO SeparateChars1 (nScore, sTwoChar) VALUES (?,?)");
               cmd.bind(1, nScore);
               cmd.bind(2, it.first().str());
               cmd.executenonquery();
            }
         }
         trans.commit();
      }

      {
         sqlite3_transaction trans(con);
         sqlite3_command cmd(con, "DELETE FROM ThreeChars");
         cmd.executenonquery();
         for (it = m_weights.m_mapThreeChars.begin(); it != m_weights.m_mapThreeChars.end(); ++it) {
            nScore = it.second().score(which);
            if (nScore!=0) {
               sqlite3_command cmd(con, "INSERT INTO ThreeChars (nScore, sThreeChar) VALUES (?,?)");
               cmd.bind(1, nScore);
               cmd.bind(2, it.first().str());
               cmd.executenonquery();
            }
         }
         trans.commit();
      }

      {
         sqlite3_transaction trans(con);
         sqlite3_command cmd(con, "DELETE FROM WordLength");
         cmd.executenonquery();
         for (int len=0; len<LENGTH_MAX; len ++) {
            nScore = m_weights.m_mapWordLength[len].score(which);
            if (nScore!=0) {
               sqlite3_command cmd(con, "INSERT INTO WordLength (nScore, nLength) VALUES (?,?)");
               cmd.bind(1, nScore);
               cmd.bind(2, len);
               cmd.executenonquery();
            }
         }
         trans.commit();
      }


      {
         sqlite3_transaction trans(con);
         sqlite3_command cmd0(con, "DELETE FROM LengthByTag");
         cmd0.executenonquery();
         sqlite3_command cmd(con, "INSERT INTO LengthByTag (nScore, nLength, sTag) VALUES (?, ?, ?)");
         cmd.bind(1, 0);
         cmd.bind(2, m_weights.m_maxLength);
         cmd.bind(3, "-NONE-");
         cmd.executenonquery();
         trans.commit();
      }

      con.close();
   }
   catch (exception &e) {
      REPORT(e.what());
   }
   TRACE("Done");
}

/*--------------------------------------------------------------
 *
 * computeAverageFeatureWeights - compute average feature weights
 *
 *-------------------------------------------------------------*/

void CFeatureHandle::computeAverageFeatureWeights(int round) {
   TRACE("adding total feature vector.");
   try {
      CWeight::CTwoWordsMap::iterator it_feature;
      CWeight::CWordMap::iterator it;
      int length;

      for (it = m_weights.m_mapSeenWords.begin(); it != m_weights.m_mapSeenWords.end(); ++it) 
         it.second().updateAverage(round);
 
      for (it_feature = m_weights.m_mapLastWordByWord.begin(); it_feature != m_weights.m_mapLastWordByWord.end(); ++it_feature) 
         it_feature.second().updateAverage(round);
 
      for (it = m_weights.m_mapOneCharWord.begin(); it != m_weights.m_mapOneCharWord.end(); ++it) 
         it.second().updateAverage(round);

      for (it_feature = m_weights.m_mapLastWordFirstChar.begin(); it_feature != m_weights.m_mapLastWordFirstChar.end(); ++it_feature) 
         it_feature.second().updateAverage(round);
 
      for (it_feature = m_weights.m_mapCurrentWordLastChar.begin(); it_feature != m_weights.m_mapCurrentWordLastChar.end(); ++it_feature) 
         it_feature.second().updateAverage(round);

      for (it = m_weights.m_mapSeparateChars.begin(); it != m_weights.m_mapSeparateChars.end(); ++it) 
         it.second().updateAverage(round);

      for (it = m_weights.m_mapConsecutiveChars.begin(); it != m_weights.m_mapConsecutiveChars.end(); ++it)
         it.second().updateAverage(round);

      for (it_feature = m_weights.m_mapFirstCharLastWordByWord.begin(); it_feature != m_weights.m_mapFirstCharLastWordByWord.end(); ++it_feature) 
         it_feature.second().updateAverage(round);

      for (it_feature = m_weights.m_mapLastWordByLastChar.begin(); it_feature != m_weights.m_mapLastWordByLastChar.end(); ++it_feature) 
         it_feature.second().updateAverage(round);

      for (it_feature = m_weights.m_mapFirstAndLastChars.begin(); it_feature != m_weights.m_mapFirstAndLastChars.end(); ++it_feature) 
         it_feature.second().updateAverage(round);

      for (it_feature = m_weights.m_mapFirstAndCurrentChars.begin(); it_feature != m_weights.m_mapFirstAndCurrentChars.end(); ++it_feature) 
         it_feature.second().updateAverage(round);

      for (length=0; length<LENGTH_MAX; length++) 
         for (it = m_weights.m_mapLengthByFirstChar[length].begin(); it != m_weights.m_mapLengthByFirstChar[length].end(); ++it) 
            it.second().updateAverage(round);

      for (length=0; length<LENGTH_MAX; length++) 
         for (it = m_weights.m_mapLengthByLastChar[length].begin(); it != m_weights.m_mapLengthByLastChar[length].end(); ++it) 
            it.second().updateAverage(round);
 
      for (length=0; length<LENGTH_MAX; length++) 
         for (it = m_weights.m_mapLengthByChar[length].begin(); it != m_weights.m_mapLengthByChar[length].end(); ++it) 
            it.second().updateAverage(round);

      for (length=0; length<LENGTH_MAX; length++) 
         for (it = m_weights.m_mapLengthByLastWord[length].begin(); it != m_weights.m_mapLengthByLastWord[length].end(); ++it) 
            it.second().updateAverage(round);
 
      for (length=0; length<LENGTH_MAX; length++) 
         for (it = m_weights.m_mapLastLengthByWord[length].begin(); it != m_weights.m_mapLastLengthByWord[length].end(); ++it) 
            it.second().updateAverage(round);

      for (it = m_weights.m_mapWordByPrevTwoChars.begin(); it != m_weights.m_mapWordByPrevTwoChars.end(); ++it) 
         it.second().updateAverage(round);

      for (it = m_weights.m_mapWordByNextTwoChars.begin(); it != m_weights.m_mapWordByNextTwoChars.end(); ++it) 
         it.second().updateAverage(round);

      for (it = m_weights.m_mapSeparateChars1.begin(); it != m_weights.m_mapSeparateChars1.end(); ++it)
         it.second().updateAverage(round);

      for (it = m_weights.m_mapThreeChars.begin(); it != m_weights.m_mapThreeChars.end(); ++it)
         it.second().updateAverage(round);

      for (length=0; length<LENGTH_MAX; length++) 
         m_weights.m_mapWordLength[length].updateAverage(round);

   }
   catch (exception &e) {
      REPORT(e.what());
   }
   TRACE("Done");
}

/*---------------------------------------------------------------
 *
 * getUnigramScore - get the unigram score for a word in sentence
 *
 *--------------------------------------------------------------*/

SCORE_TYPE getUnigramScore( CTagger * tagger , const CStringVector * sentence , int start , int length ) {

   CWeight & weights = tagger->getFeatureHandle()->getWeights() ; 
   static SCORE_TYPE nReturn ; 
   static int end ;
   end = start+length-1 ;

   // about the words
   const static CWord emptyWord( "" ) ;
   const static CScore zeroScore ;
   const CWord & word = tagger->findWordFromCache( start , length , sentence ) ;
   const CWord & first_char = tagger->findWordFromCache( start , 1 , sentence ) ;
   const CWord & last_char = tagger->findWordFromCache( end , 1 , sentence ) ;
   const CWord & prev_char = start > 0 ? tagger->findWordFromCache( start - 1 , 1 , sentence ) : emptyWord ;
   const CWord & next_char = end < sentence->size() - 1 ? tagger->findWordFromCache( end + 1 , 1 , sentence ) : emptyWord;
   const CWord & word_prev_twochar = start > 1 ? tagger->findWordFromCache( start - 2 , length + 2 , sentence ) : emptyWord ;
   const CWord & word_next_twochar = end < sentence->size() - 2 ? tagger->findWordFromCache( start , length + 2 , sentence ) : emptyWord;
   static CTwoWords first_and_last_char , currentword_lastchar , lastword_lastchar , lastword_firstchar , firstcharlastword_word , first_and_current_char;
   first_and_last_char.refer( &first_char , &last_char ) ;
   const CWord & two_char = start > 0 ? tagger->findWordFromCache( start - 1 , 2 , sentence ) : emptyWord ;
   const CWord & two_char1 = end < sentence->size() - 1 ? tagger->findWordFromCache( end , 2 , sentence ) : emptyWord ;
   const CWord & three_char = ( length == 1 && start > 0 && end < sentence->size() - 1 ) ? tagger->findWordFromCache( start - 1 , 3 , sentence ) : emptyWord ;
   if ( start > 0 ) {
      currentword_lastchar.refer( &word , &prev_char ) ;
      lastword_lastchar.refer( &prev_char , &last_char ) ;
   }
   if ( end < sentence->size() - 1 ) {
      lastword_firstchar.refer( &word , &next_char ) ;
      firstcharlastword_word.refer( &first_char , &next_char ) ;
   }

   // about the length
   if ( length > LENGTH_MAX-1 ) length = LENGTH_MAX-1 ;

   // adding scores with features
   nReturn = weights.m_mapSeenWords.find( word , zeroScore ).score() ; 

   if (length==1) {
      nReturn += weights.m_mapOneCharWord.find( word , zeroScore ).score() ;

      if ( start > 0 && end < sentence->size() - 1 )
         nReturn += weights.m_mapThreeChars.find( three_char , zeroScore ).score() ;
   }
   else {
      nReturn += weights.m_mapFirstAndLastChars.find( first_and_last_char , zeroScore ).score() ;

      nReturn += weights.m_mapLengthByFirstChar[ length ].find( first_char , zeroScore ).score() ;
      nReturn += weights.m_mapLengthByLastChar[ length ].find( last_char , zeroScore ).score() ;
   }
   if ( start > 0 ) {
      nReturn += weights.m_mapSeparateChars.find( two_char , zeroScore ).score() ; 
      
      nReturn += weights.m_mapCurrentWordLastChar.find( currentword_lastchar , zeroScore ).score() ;
      nReturn += weights.m_mapLastWordByLastChar.find( lastword_lastchar , zeroScore ).score() ;

      //if ( start > 1 )
      //   nReturn += weights.m_mapWordByPrevTwoChars.find( word_prev_twochar , zeroScore ).score() ;
   }
   if ( end < sentence->size() - 1 ) {
      nReturn += weights.m_mapSeparateChars1.find( two_char1 , zeroScore ).score() ; 

      nReturn += weights.m_mapLastWordFirstChar.find( lastword_firstchar , zeroScore ).score() ;
      nReturn += weights.m_mapFirstCharLastWordByWord.find( firstcharlastword_word , zeroScore ).score() ;

      //if ( end < sentence->size() - 2 )
      //   nReturn += weights.m_mapWordByNextTwoChars.find( word_next_twochar , zeroScore ).score();
   }
   for ( int j = 0 ; j <= end - start ; j ++ ) {
      if ( j < end - start ) {
         nReturn += weights.m_mapConsecutiveChars.find( tagger->findWordFromCache( start + j , 2 , sentence ) , zeroScore ).score() ;
         if ( j > 0 ) 
            nReturn += weights.m_mapLengthByChar[length].find( tagger->findWordFromCache( start + j , 1 , sentence ) , zeroScore ).score() ;
      }
   }
   //nReturn += weights.m_mapWordLength[ length ].score() ; this does not help
 
   return nReturn;
}

/*---------------------------------------------------------------
 *
 * getBigramScore - get the local score for a word in sentence
 *
 *--------------------------------------------------------------*/

SCORE_TYPE getBigramScore( CTagger * Tagger , const CStringVector * sentence , int last_start , int last_length , int start , int length ) {
   static SCORE_TYPE nReturn ; 
   const static CScore zeroScore ;
   CWeight & weights = Tagger->getFeatureHandle()->getWeights() ; 
   // about the words
   const CWord & word = Tagger->findWordFromCache( start , length , sentence ) ;      
   const CWord & last_word = Tagger->findWordFromCache( last_start , last_length , sentence ) ; 
   static CTwoWords two_word ;
   two_word.refer( &word , &last_word ) ;
   // about the length
   if ( length > LENGTH_MAX-1 ) length = LENGTH_MAX-1 ;
   if( last_length > LENGTH_MAX-1 ) last_length = LENGTH_MAX-1;
   
   // add the scores up
   nReturn = weights.m_mapLastWordByWord.find( two_word , zeroScore ).score();
   nReturn += weights.m_mapLengthByLastWord[ length ].find( last_word , zeroScore ).score();
   nReturn += weights.m_mapLastLengthByWord[ last_length ].find( word , zeroScore ).score();

   return nReturn ;
}

/*---------------------------------------------------------------
 *
 * updateUnigramScore - update the unigram score for a word 
 *
 *--------------------------------------------------------------*/

SCORE_TYPE updateUnigramScore( CTagger * Tagger , const CStringVector * sentence , int start , int length , CFeatureHandle:: SCORE_UPDATE method , int round ) {

   CWeight & weights = Tagger->getFeatureHandle()->getWeights() ; 
   static SCORE_TYPE amount ; 
   amount = ( method == CFeatureHandle :: eAdd ) ? 1 : -1 ;
   static int end ;
   end = start + length - 1 ;

   // about the words
   const static CWord emptyWord("") ;
   assert( length != 0 ) ;
   const CWord & word = Tagger->findWordFromCache( start , length , sentence ) ;
   const CWord & first_char = Tagger->findWordFromCache( start , 1 , sentence ) ;
   const CWord & last_char = Tagger->findWordFromCache( end , 1 , sentence ) ;
   const CWord & prev_char = start > 0 ? Tagger->findWordFromCache( start - 1 , 1 , sentence ) : emptyWord ;
   const CWord & next_char = end < sentence->size() - 1 ? Tagger->findWordFromCache( end + 1 , 1 , sentence ) : emptyWord ;
   const CWord & word_prev_twochar = start > 1 ? Tagger->findWordFromCache( start - 2 , length + 2 , sentence ) : emptyWord ;
   const CWord & word_next_twochar = end < sentence->size() - 2 ? Tagger->findWordFromCache( start , length + 2 , sentence ) : emptyWord;
   static CTwoWords first_and_last_char , currentword_lastchar , lastword_lastchar , lastword_firstchar , firstcharlastword_word , first_and_current_char ;
   first_and_last_char.allocate( first_char , last_char ) ;
   const CWord & two_char = start > 0 ? Tagger->findWordFromCache( start - 1 , 2 , sentence ) : emptyWord ;
   const CWord & two_char1 = end < sentence->size() - 1 ? Tagger->findWordFromCache( end , 2 , sentence ) : emptyWord ;
   const CWord & three_char = ( length == 1 && start > 0 && end < sentence->size() - 1 ) ? Tagger->findWordFromCache( start - 1 , 3 , sentence ) : emptyWord ;
   if ( start > 0 ) {
      currentword_lastchar.allocate( word , prev_char ) ;
      lastword_lastchar.allocate( prev_char , last_char ) ;
   }
   if ( end < sentence->size() - 1 ) {
      lastword_firstchar.allocate( word , next_char ) ;
      firstcharlastword_word.allocate( first_char , next_char ) ;
   }

   // about the length
   if ( length > LENGTH_MAX-1 ) length = LENGTH_MAX-1 ;
   
   // adding scores with features
   weights.m_mapSeenWords[ word ].updateCurrent( amount , round ) ; 
   if ( length == 1 ) {
      weights.m_mapOneCharWord[ word ].updateCurrent( amount , round ) ;

      if ( start > 0 && end < sentence->size() - 1 )
         weights.m_mapThreeChars[ three_char ].updateCurrent( amount , round ) ;
   }
   else {
      weights.m_mapFirstAndLastChars[ first_and_last_char ].updateCurrent( amount , round ) ;

      weights.m_mapLengthByFirstChar[ length ][ first_char ].updateCurrent( amount , round ) ;
      weights.m_mapLengthByLastChar[ length ][ last_char ].updateCurrent( amount , round ) ;
   }
   if ( start > 0 ) {
      weights.m_mapSeparateChars[ two_char ].updateCurrent( amount , round ) ; 
      
      weights.m_mapCurrentWordLastChar[ currentword_lastchar ].updateCurrent( amount , round ) ;
      weights.m_mapLastWordByLastChar[ lastword_lastchar ].updateCurrent( amount , round ) ;

      if ( start > 1 )
         weights.m_mapWordByPrevTwoChars[ word_prev_twochar ].updateCurrent( amount , round ) ;
   }
   if ( end < sentence->size() - 1 ) {
      weights.m_mapSeparateChars1[ two_char1 ].updateCurrent( amount , round ) ; 

      weights.m_mapLastWordFirstChar[ lastword_firstchar ].updateCurrent( amount , round ) ;
      weights.m_mapFirstCharLastWordByWord[ firstcharlastword_word ].updateCurrent( amount , round ) ;

      if ( end < sentence->size() - 2 )
         weights.m_mapWordByNextTwoChars[ word_next_twochar ].updateCurrent( amount , round ) ;
   }

   for ( int j = 0 ; j <= end - start ; j ++ ) {
      if ( j < end - start ) {
         weights.m_mapConsecutiveChars[ tagger->findWordFromCache( start + j , 2 , sentence ) ].updateCurrent( amount , round ) ;
         if ( j > 0 )
            weights.m_mapLengthByChar[ length ][ tagger->findWordFromCache( start + j , 1 , sentence ) ].updateCurrent( amount , round ) ;
      }
   }
   weights.m_mapWordLength[ length ].updateCurrent( amount , round ); 

   // those information collected for the decoding algorithm
   if ( method == CFeatureHandle :: eAdd ) {
      if ( end - start + 1 > weights.m_maxLength )
         weights.m_maxLength = end - start + 1 ;
      if ( weights.m_maxLength > MAX_WORD_SIZE ) {
         TRACE("Warning: the maximum word size is exceeding the system limit!")
         weights.m_maxLength = MAX_WORD_SIZE ;
      }
   }

}

/*---------------------------------------------------------------
 *
 * updateBigramScore - update the local score for a word 
 *
 *--------------------------------------------------------------*/

SCORE_TYPE updateBigramScore( CTagger * tagger , const CStringVector * sentence , int last_start , int last_length , int start , int length , CFeatureHandle :: SCORE_UPDATE method , int round ) {

   static SCORE_TYPE amount ; 
   amount = ( method == CFeatureHandle :: eAdd ) ? 1 : -1 ;
   CWeight & weights = tagger->getFeatureHandle()->getWeights() ; 

   // about the words
   const CWord & word = tagger->findWordFromCache( start , length , sentence ) ; 
   const CWord &last_word = tagger->findWordFromCache( last_start , last_length , sentence ) ; 
   static CTwoWords two_word ;
   two_word.allocate( word , last_word ) ;

   // about the length
   if ( length > LENGTH_MAX-1 ) length = LENGTH_MAX-1 ;
   if ( last_length > LENGTH_MAX-1 ) last_length = LENGTH_MAX-1 ;

   // adding scores with features
   weights.m_mapLastWordByWord[ two_word ].updateCurrent( amount , round ) ;
   weights.m_mapLengthByLastWord[ length ][ last_word ].updateCurrent( amount , round ) ;
   weights.m_mapLastLengthByWord[ last_length ][ word ].updateCurrent( amount , round ) ;

}




const static SCORE_TYPE absolute_min_score = -1000000;

/*===============================================================
 *
 * CTagger 
 *
 *==============================================================*/

/*---------------------------------------------------------------
 *
 * train_loop - do a training loop
 *
 * Inputs: segmented_item - contains segmentation information but empty tags
 *         correct_item   - contains both segmentation and tag information ;
 *                          set this value to NULL when doing decoding
 *
 *--------------------------------------------------------------*/

bool train_loop( CTagger * tagger , const CStringVector * sentence , const CStateItem * correct_item , CStateItem * segmented_item, int & round ) {
   const int length = sentence->size() ;

   int i , j , k ; // iterators 
   int length_left ;

   int most_confident_index ; 
   CTag most_confident_tag ; 
   SCORE_TYPE most_confident_score ; 

   assert( length < MAX_SENTENCE_SIZE ) ;

   //
   // Initialise candidates
   //
   tagger->clearWordCache() ;
   tagger->m_Agenda->clear() ; 
   length_left = length ;

   // initially no tags assigned - find the best item from unigram scores;
   most_confident_score = absolute_min_score ;
   for ( i=0 ; i < segmented_item->m_nLength ; ++i ) {
      partial_score = getUnigramScore( tagger , sentence , segmented_item ) ;
      candidates[ i ] = partial_score ;
      if ( partial_score > most_confident_score ) {
         most_confident_score = partial_score ;
         most_confident_index = i ;
      }
   }

   //
   // Loop to find candidates
   //
   TRACE("Tagging started ");
   while ( length_left > 0 ) {

      // if the tag of the current best word and the corresponding output mismatch
      if ( correct_item != NULL && // when we are training, we get the correct tag information
           correct_item->getTag(most_confident_index) != segmented_item->getTag(most_confident_index) ) {

         updateScores(sentence, correct_item, segmented_item, most_confident_index) ; 

         return false;
      }

      // exclude possible candidates from overlapping with the current one
      length_left-- ;                     

      for ( i = 0 ; i < segmented_item.m_nLength ; ++i )
         candidates[ i ] = absolute_min_score ; 

      // update scores
      for ( len = 1 ; len < max_word_length ; len ++ ) { // for words before current
         i = max_partial_start - len ;
         if ( i < 0 || chars[ i ] != 0 )
            break;
         candidates[ i * max_word_length + ( len - 1 ) ] += getBigramScore( tagger , sentence , i , len , max_partial_start , max_partial_length ) ;
      }

      i = max_partial_start + max_partial_length ; // for words after current 
      for ( len = 1 ; len < max_word_length ; len ++ ) { 
         if ( i + len > length || chars[ i + len - 1 ] != 0)
            break;
         candidates[ i * max_word_length + ( len - 1 ) ] += getBigramScore( tagger , sentence , max_partial_start , max_partial_length , i , len ) ;
      }

      // find the best again
      max_partial_score = absolute_min_score ;
      for ( i = 0 ; i < length * max_word_length ; ++i ) {
         if ( candidates[ i ] > max_partial_score ) {
            max_partial_score = candidates[ i ] ;
            max_partial_start = i / max_word_length ;
            max_partial_length = i % max_word_length + 1 ;
         }
      }
      assert( max_partial_score != absolute_min_score || length_left == 0 );
   }   
   return true ;
}

/*---------------------------------------------------------------
 *
 * train - do automatic training process
 *
 *--------------------------------------------------------------*/

void CTagger::train( const CStringVector * sentence, const CStringVector * correct, int & round ) {
   TRACE("start trainging a sentence...");
   clock_t total_start_time = clock() ;
   // get the correct output
   static vector<int> indice ;
   static vector<int> startindex ;
   static vector<int> endindex ;
   static unsigned long int i , j ;
   static unsigned long int word_start , word_end ;
   static int start ;

   indice.clear() ;
   for ( i = 0 ; i < sentence->size() ; ++ i ) {
      for ( j = 0 ; j < sentence->at(i).size() ; ++ j)
         indice.push_back( i ) ;
   }

   // record word index for the correcting
   startindex.clear() ;
   endindex.clear() ;
   start=0 ;
   for ( i = 0 ; i < correct->size() ; ++ i ) {
      word_start = indice[ start ] ;
      word_end = indice[ start + correct->at( i ).size() - 1 ] ;
      for ( j = word_start ; j <= word_end ; j++ ) {
         startindex.push_back( -1 ) ;
         endindex.push_back( -1 ) ;
      }
      startindex[ word_start ] = word_end ;
      endindex[ word_end ] = word_start ;
      start += correct->at( i ).size() ;
   }
   while ( train_loop( this , sentence , correct , startindex , endindex , round ) == false) ;
   TRACE("total time spent: " << double( clock() - total_start_time ) / CLOCKS_PER_SEC );
}

/*---------------------------------------------------------------
 *
 * tag - do tagging on  a sentence
 *
 *--------------------------------------------------------------*/

void CTagger::tag(const CStringVector* sentence, CStringVector *vReturn, double *out_scores, int nBest) {
   clock_t total_start_time = clock();;
   const int length = sentence->size();
   int i, j, k, len;                            // temporary index
   int max_word_length;

   static SCORE_TYPE candidates[MAX_SENTENCE_SIZE*MAX_WORD_SIZE];
   static int chars[MAX_SENTENCE_SIZE];
   SCORE_TYPE partial_score, max_partial_score;
   int max_partial_start, max_partial_length;
   int length_left;
   int current_index;

   assert(length<MAX_SENTENCE_SIZE);
   assert(vReturn!=NULL);
   assert(nBest == 1); // this is deterministic
   assert(m_Feature->m_bTrain == false);

   //
   // Initialise candidates
   //
   TRACE("Initialising the tagging process...");
   clearWordCache();
   max_word_length = m_Feature->getWeights().m_maxLength;
   max_partial_score = absolute_min_score;
   length_left = length;
   for (i=0; i<length; ++i)
      chars[i]=0;
   for ( i = 0 ; i < length ; ++i )
      for ( len = 1 ; len < max_word_length + 1 ; len ++ ) {
         if ( len > length - i ) {
            candidates[ i * max_word_length + ( len - 1 ) ] = absolute_min_score;
         }
         else {
            partial_score = getUnigramScore( this , sentence , i , len ) ;
            candidates[i*max_word_length+(len-1)] = partial_score;
            if (partial_score>max_partial_score) {
               max_partial_score = partial_score;
               max_partial_start = i;
               max_partial_length = len;
            }
         }
      }
   current_index = 1;

   //
   // Loop to find the best candidates
   //
   TRACE("Tagging started ");

   while (length_left>0) {

      // record 
      for ( i = max_partial_start ; i < max_partial_start + max_partial_length ; ++i )
         chars[i] = current_index;

      // exclude possible candidates from overlapping with the current one
      for ( i = max( 0, max_partial_start - max_word_length + 1) ; i < max_partial_start + max_partial_length; ++i )
         for ( len = max( 1, max_partial_start - i + 1 ) ; len < min( max_word_length , length - i ) + 1 ; len ++ )
            candidates[ i * max_word_length + ( len - 1 ) ] = absolute_min_score ; 

      current_index ++ ;
      length_left -= max_partial_length;

      // update the scores of neighbouring word
      for ( len = 1 ; len <= max_word_length ; len ++ ) { // the left
         i = max_partial_start - len;
         if ( i < 0 || chars[ i ] != 0) break;
         candidates[ i * max_word_length + ( len - 1 ) ] += getBigramScore( this , sentence , i , len , max_partial_start , max_partial_length ) ;
      }

      i = max_partial_start + max_partial_length ; // the right
      for ( len = 1 ; len < max_word_length ; len ++ ) {
         if ( i + len > length || chars[ i + len - 1 ] != 0 )
            break;
         candidates[ i * max_word_length + ( len - 1 ) ] += getBigramScore( this , sentence , max_partial_start , max_partial_length , i , len ) ;
      }

      // find the best again
      max_partial_score = absolute_min_score;
      for ( i = 0 ; i < length * max_word_length ; ++i )
         if ( candidates[ i ] > max_partial_score ) {
            max_partial_score = candidates[ i ] ;
            max_partial_start = i / max_word_length ;
            max_partial_length = i % max_word_length + 1 ;
         }
   }


   // now generate output sentence, the score is *not* computed
   TRACE("Outputing sentence");
   vReturn[0].clear();
   // assign retval
   k = -1;
   static string temp;
   for (j=0; j<length; j++) {
      if (chars[j] != k) {
         if (k != -1) vReturn[0].push_back(temp);
         temp = "";
         k = chars[j];
      }
      temp += sentence->at(j);
   }
   vReturn[0].push_back(temp);
   TRACE("total time spent: " << double(clock() - total_start_time)/CLOCKS_PER_SEC);
}


