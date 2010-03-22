/****************************************************************
 *                                                              *
 * weight.cpp - the weight definition method                    *
 *                                                              *
 * Part of the constituent parser of english                    *
 *                                                              *
 * Yue Zhang                                                    *
 * 2007.8                                                       *
 *                                                              *
 ****************************************************************/

#include "weight.h"

using namespace TARGET_LANGUAGE;
using namespace TARGET_LANGUAGE::conparser;

const CScore<SCORE_TYPE> g_zeroScore;

#ifdef _CHINESE_CFG_H
#define _BRACKET_AND_SEPARATOR_FEATURES(left,right) \
   left(m_mapBracketS0c)right\
   left(m_mapBracketS0w)right\
   left(m_mapBracketS0wS1c)right\
   left(m_mapBracketS0cS1w)right\
   left(m_mapBracketS0cS1c)right\
   left(m_mapBracketS0cN0w)right\
   left(m_mapBracketS0wN0t)right\
   left(m_mapBracketS0cN0t)right\
\
   left(m_mapS0cSeparator)right\
   left(m_mapS0wcSeparator)right\
   left(m_mapS0cSepCount)right\
   left(m_mapS0wcSepCount)right\
   left(m_mapS1cSeparator)right\
   left(m_mapS1wcSeparator)right\
   left(m_mapS1cSepCount)right\
   left(m_mapS1wcSepCount)right\
   left(m_mapS0cS1cSeparator)right\
   left(m_mapS0cS1cSepCount)right
#else
#define _BRACKET_AND_SEPARATOR_FEATURES(left,right)
#endif

#define iterate_templates(left, right) \
   left(m_mapHw)right\
   left(m_mapHt)right\
   left(m_mapHc)right\
   left(m_mapHwc)right\
   left(m_mapHcMc)right\
   left(m_mapHcMw)right\
   left(m_mapHwMc)right\
   left(m_mapHwMw)right\
   left(m_mapBigramsL)right\
   left(m_mapBigramsR)right\
   left(m_mapBigramsLexA)right\
   left(m_mapBigramsLexB)right\
   left(m_mapRules)right\
   left(m_mapRulesWithSizes)right\
   left(m_mapHtMt)right\
   left(m_mapHtMw)right\
   left(m_mapHwMt)right\
   left(m_mapSiblingTags)right\
   left(m_mapSiblingWordTag)right\
   left(m_mapSiblingTagWord)right\
   left(m_mapBracket)right\
   left(m_mapHtArityL)right\
   left(m_mapHtArityR)right\
   left(m_mapHwArityL)right\
   left(m_mapHwArityR)right\
\
   left(m_mapS0c)right\
   left(m_mapS0cm)right\
   left(m_mapS0w)right\
   left(m_mapS0tc)right\
   left(m_mapS0wc)right\
\
   left(m_mapS1c)right\
   left(m_mapS1cm)right\
   left(m_mapS1w)right\
   left(m_mapS1tc)right\
   left(m_mapS1wc)right\
\
   left(m_mapS2c)right\
   left(m_mapS2w)right\
   left(m_mapS2tc)right\
   left(m_mapS2wc)right\
\
   left(m_mapS3c)right\
   left(m_mapS3w)right\
   left(m_mapS3tc)right\
   left(m_mapS3wc)right\
\
   left(m_mapN0w)right\
   left(m_mapN0t)right\
   left(m_mapN0wt)right\
\
   left(m_mapN1w)right\
   left(m_mapN1t)right\
   left(m_mapN1wt)right\
\
   left(m_mapN2w)right\
   left(m_mapN2t)right\
   left(m_mapN2wt)right\
\
   left(m_mapN3w)right\
   left(m_mapN3t)right\
   left(m_mapN3wt)right\
\
   left(m_mapS0Lwc)right\
   left(m_mapS0Ltc)right\
   left(m_mapS0Rwc)right\
   left(m_mapS0Rtc)right\
   left(m_mapS0Uwc)right\
   left(m_mapS0Utc)right\
   left(m_mapS1Lwc)right\
   left(m_mapS1Ltc)right\
   left(m_mapS1Rwc)right\
   left(m_mapS1Rtc)right\
   left(m_mapS1Uwc)right\
   left(m_mapS1Utc)right\
   left(m_mapS0LDw)right\
   left(m_mapS0LDt)right\
   left(m_mapS0RDw)right\
   left(m_mapS0RDt)right\
   left(m_mapS1LDw)right\
   left(m_mapS1LDt)right\
   left(m_mapS1RDw)right\
   left(m_mapS1RDt)right\
\
   left(m_mapS0wS1w)right\
   left(m_mapS0wS1c)right\
   left(m_mapS0cS1w)right\
   left(m_mapS0tS1c)right\
   left(m_mapS0cS1t)right\
   left(m_mapS0cS1c)right\
   left(m_mapS0cmS1cm)right\
\
   left(m_mapS0wN0w)right\
   left(m_mapS0cN0w)right\
   left(m_mapS0wN0t)right\
   left(m_mapS0cN0t)right\
   left(m_mapS0cmN0tm)right\
\
   left(m_mapS1wN0w)right\
   left(m_mapS1cN0w)right\
   left(m_mapS1wN0t)right\
   left(m_mapS1cN0t)right\
\
   left(m_mapN0wN1w)right\
   left(m_mapN0tN1w)right\
   left(m_mapN0wN1t)right\
   left(m_mapN0tN1t)right\
\
   _BRACKET_AND_SEPARATOR_FEATURES(left,right)\
\
   left(m_mapS0wS1Dist)right\
   left(m_mapS0cS1Dist)right\
   left(m_mapS0S1wDist)right\
   left(m_mapS0S1cDist)right\
   left(m_mapS0wS1wDist)right\
   left(m_mapS0wS1cDist)right\
   left(m_mapS0cS1wDist)right\
   left(m_mapS0cS1cDist)right\
\
   left(m_mapS0wS1cN0t)right\
   left(m_mapS0cS1wN0t)right\
   left(m_mapS0cS1cN0w)right\
   left(m_mapS0cS1cN0t)right\
   left(m_mapS0cmS1cmN0tm)right\
\
   left(m_mapS0wN0tN1t)right\
   left(m_mapS0cN0wN1t)right\
   left(m_mapS0cN0tN1w)right\
   left(m_mapS0cN0tN1t)right\
\
   left(m_mapS0wS1cS2c)right\
   left(m_mapS0cS1wS2c)right\
   left(m_mapS0cS1cS2w)right\
   left(m_mapS0cS1cS2c)right\
   left(m_mapS0cmS1cmS2cm)right\
\
   left(m_mapS1cS2c)right\
   left(m_mapS2cS3c)right\
   left(m_mapN1tN2t)right\
   left(m_mapN2tN3t)right\
\
   left(m_mapS0cS2c)right\
   left(m_mapS1cS3c)right\
   left(m_mapS0cN1t)right\
   left(m_mapN0tN2t)right\
   left(m_mapN1tN3t)right\
\
   left(m_mapS0cS0LcN0t)right\
   left(m_mapS0wS0LcN0t)right\
   left(m_mapS0cS0LcN0w)right\
   left(m_mapS0cS0RcN0t)right\
   left(m_mapS0wS0RcN0t)right\
   left(m_mapS0cS0RcN0w)right\
   left(m_mapS0cS0UcN0t)right\
   left(m_mapS0wS0UcN0t)right\
   left(m_mapS0cS0UcN0w)right\
\
   left(m_mapS0cS0LcS1c)right\
   left(m_mapS0wS0LcS1c)right\
   left(m_mapS0cS0LcS1w)right\
   left(m_mapS0cS0RcS1c)right\
   left(m_mapS0wS0RcS1c)right\
   left(m_mapS0cS0RcS1w)right\
   left(m_mapS0cS0UcS1c)right\
   left(m_mapS0wS0UcS1c)right\
   left(m_mapS0cS0UcS1w)right\
\
   left(m_mapN0tN1tN2t)right\
   left(m_mapS0cS1cS1Lc)right\
   left(m_mapS0wS1cS1Lc)right\
   left(m_mapS0cS1wS1Lc)right\
   left(m_mapS0cS1cS1Rc)right\
   left(m_mapS0wS1cS1Rc)right\
   left(m_mapS0cS1wS1Rc)right\
   left(m_mapS0cS1cS1Uc)right\
   left(m_mapS0wS1cS1Uc)right\
   left(m_mapS0cS1wS1Uc)right

/*---------------------------------------------------------------
 *
 * loadScores - load scores from the file specified at constructor
 *              currently this uses database, but it can be modified
 *         
 * Affects: m_bScoreModified, clearing it. 
 *
 *--------------------------------------------------------------*/

void CWeight::loadScores() {
   clock_t time_start = clock();
   cout<<"Loading scores..."; cout.flush();
   ifstream file ; 
   file.open(m_sRecordPath.c_str()) ;

   if (!file.is_open()) {
      cout << " empty." << endl; return;
   }

   iterate_templates(file >>,;);

   static string s;
   getline(file, s);
   ASSERT(s=="Word frequency", "Word frequency not found from model.");
   file >> m_mapWordFrequency;

   static istringstream iss;
   getline(file, s);
   ASSERT(s=="Maximum frequency", "Maximum frequency not found from model.");
   getline(file, s);
   iss.str(s);
   iss >> m_nMaxWordFrequency;

   file.close() ;
   cout << " done. (" << double(clock()-time_start)/CLOCKS_PER_SEC << "s)" << endl;
}

/*---------------------------------------------------------------
 *
 * saveScores - save scores back to database
 *
 * This method is called by the destructor is m_bScoreModified
 * is true.
 *
 *--------------------------------------------------------------*/

void CWeight::saveScores() {
   cout<<"Saving scores..."; cout.flush();
   ofstream file ;
   file.open(m_sRecordPath.c_str()) ;

   iterate_templates(file<<,;)

   file << "Word frequency" << endl; 
   file << m_mapWordFrequency;
   file << "Maximum frequency" << endl; 
   file << m_nMaxWordFrequency << endl;

   file.close();
   cout<<" done."<<endl;
}

/*--------------------------------------------------------------
 *
 * computeAverageFeatureWeights - compute average feature weights
 *
 *-------------------------------------------------------------*/

void CWeight::computeAverageFeatureWeights(int round) {
   cout<<"Computing averaged (total) feature vector..."; cout.flush();
   
   iterate_templates(,.computeAverage(round);) ;
   
   cout<<"done."<<endl;
}

