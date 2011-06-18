// Copyright (C) University of Oxford 2010
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

/*---------------------------------------------------------------
 *
 * loadScores - load scores from the file specified at constructor
 *              currently this uses database, but it can be modified
 *         
 * Affects: m_bScoreModified, clearing it. 
 *
 *--------------------------------------------------------------*/

void TARGET_LANGUAGE::conparser::CWeight::loadScores(std::ifstream &file) {
   clock_t time_start = clock();
   std::cout<<"Loading scores..."; std::cout.flush();
//   std::ifstream file ; 
//   file.open(m_sRecordPath.c_str()) ;

   if (!file.is_open()) {
      std::cout << " empty." << std::endl; return;
   }
   m_bEmpty = false;

   static std::string s;
   getline(file, s);
   ASSERT(s=="Categories:", "Category symbols not found in model file") ;
   getline(file, s);
   std::istringstream iss_c(s);
   CConstituent c;
   while(iss_c >> c);
   getline(file, s);
   ASSERT(s=="", "No empty line after the category symbols") ;

   iterate_templates(file >>,;);

   getline(file, s);
   ASSERT(s=="Word frequency", "Word frequency not found from model.");
   file >> m_mapWordFrequency;

   getline(file, s);
   ASSERT(s=="Maximum frequency", "Maximum frequency not found from model.");
   getline(file, s);
   std::istringstream iss_f(s);
//   iss.str(s);
   iss_f >> m_nMaxWordFrequency;

//   file.close() ;
   std::cout << " done. (" << double(clock()-time_start)/CLOCKS_PER_SEC << "s)" << std::endl;
}

/*---------------------------------------------------------------
 *
 * saveScores - save scores back to database
 *
 * This method is called by the destructor is m_bScoreModified
 * is true.
 *
 *--------------------------------------------------------------*/

void TARGET_LANGUAGE::conparser::CWeight::saveScores(std::ofstream &file) {
   std::cout<<"Saving scores..."; std::cout.flush();
//   std::ofstream file ;
//   file.open(m_sRecordPath.c_str()) ;

   file << "Categories:" << std::endl;
   file << CConstituent(CConstituent::FIRST);
   for (unsigned c=CConstituent::FIRST+1; c<CConstituent::COUNT; ++c)
      file << ' ' << CConstituent(c);
   file << std::endl << std::endl;

   iterate_templates(file<<,;)

   file << "Word frequency" << std::endl; 
   file << m_mapWordFrequency;
   file << "Maximum frequency" << std::endl; 
   file << m_nMaxWordFrequency << std::endl;

//   file.close();
   std::cout<<" done."<<std::endl;
}

/*--------------------------------------------------------------
 *
 * computeAverageFeatureWeights - compute average feature weights
 *
 *-------------------------------------------------------------*/

void TARGET_LANGUAGE::conparser::CWeight::computeAverageFeatureWeights(int round) {
   std::cout<<"Computing averaged (total) feature vector..."; std::cout.flush();
   
   iterate_templates(,.computeAverage(round);) ;
   
   std::cout<<"done."<<std::endl;
}

void TARGET_LANGUAGE::conparser::CWeight::addCurrent(CWeight*w, int round) {
   iterate_double_templates(, .addCurrent ID_LRB w-> , ID_COMMA round ID_RRB ;);
/*   m_mapS0c.addCurrent(w->m_mapS0c, round);
   m_mapS0w.addCurrent(w->m_mapS0w, round);
   m_mapS0tc.addCurrent(w->m_mapS0tc, round);
   m_mapS0wc.addCurrent(w->m_mapS0wc, round);

   m_mapS1c.addCurrent(w->m_mapS1c, round);
   m_mapS1w.addCurrent(w->m_mapS1w, round);
   m_mapS1tc.addCurrent(w->m_mapS1tc, round);
   m_mapS1wc.addCurrent(w->m_mapS1wc, round);

   m_mapS2tc.addCurrent(w->m_mapS2tc, round);
   m_mapS2wc.addCurrent(w->m_mapS2wc, round);

   m_mapS3tc.addCurrent(w->m_mapS3tc, round);
   m_mapS3wc.addCurrent(w->m_mapS3wc, round);

   m_mapN0w.addCurrent(w->m_mapN0w, round);
   m_mapN0t.addCurrent(w->m_mapN0t, round);
   m_mapN0wt.addCurrent(w->m_mapN0wt, round);

   m_mapN1w.addCurrent(w->m_mapN1w, round);
   m_mapN1t.addCurrent(w->m_mapN1t, round);
   m_mapN1wt.addCurrent(w->m_mapN1wt, round);

   m_mapN2w.addCurrent(w->m_mapN2w, round);
   m_mapN2t.addCurrent(w->m_mapN2t, round);
   m_mapN2wt.addCurrent(w->m_mapN2wt, round);

   m_mapN3w.addCurrent(w->m_mapN3w, round);
   m_mapN3t.addCurrent(w->m_mapN3t, round);
   m_mapN3wt.addCurrent(w->m_mapN3wt, round);

   m_mapS0Lwc.addCurrent(w->m_mapS0Lwc, round);
   m_mapS0Ltc.addCurrent(w->m_mapS0Ltc, round);
   m_mapS0Rwc.addCurrent(w->m_mapS0Rwc, round);
   m_mapS0Rtc.addCurrent(w->m_mapS0Rtc, round);
   m_mapS0Uwc.addCurrent(w->m_mapS0Uwc, round);
   m_mapS0Utc.addCurrent(w->m_mapS0Utc, round);
   m_mapS1Lwc.addCurrent(w->m_mapS1Lwc, round);
   m_mapS1Ltc.addCurrent(w->m_mapS1Ltc, round);
   m_mapS1Rwc.addCurrent(w->m_mapS1Rwc, round);
   m_mapS1Rtc.addCurrent(w->m_mapS1Rtc, round);
   m_mapS1Uwc.addCurrent(w->m_mapS1Uwc, round);
   m_mapS1Utc.addCurrent(w->m_mapS1Utc, round);

   m_mapS0wcS1wc.addCurrent(w->m_mapS0wcS1wc, round);
   m_mapS0wS1c.addCurrent(w->m_mapS0wS1c, round);
   m_mapS0cS1w.addCurrent(w->m_mapS0cS1w, round);
   m_mapS0cS1c.addCurrent(w->m_mapS0cS1c, round);
   m_mapBetweenTags.addCurrent(w->m_mapBetweenTags, round);

   m_mapS0wN0w.addCurrent(w->m_mapS0wN0w, round);
   m_mapS0cN0w.addCurrent(w->m_mapS0cN0w, round);
   m_mapS0wN0t.addCurrent(w->m_mapS0wN0t, round);
   m_mapS0cN0t.addCurrent(w->m_mapS0cN0t, round);
   m_mapS0cmN0tm.addCurrent(w->m_mapS0cmN0tm, round);

   m_mapS1wN0w.addCurrent(w->m_mapS1wN0w, round);
   m_mapS1cN0w.addCurrent(w->m_mapS1cN0w, round);
   m_mapS1wN0t.addCurrent(w->m_mapS1wN0t, round);
   m_mapS1cN0t.addCurrent(w->m_mapS1cN0t, round);

   m_mapN0wN1w.addCurrent(w->m_mapN0wN1w, round);
   m_mapN0tN1w.addCurrent(w->m_mapN0tN1w, round);
   m_mapN0wN1t.addCurrent(w->m_mapN0wN1t, round);
   m_mapN0tN1t.addCurrent(w->m_mapN0tN1t, round);

   m_mapS0wS1cN0t.addCurrent(w->m_mapS0wS1cN0t, round);
   m_mapS0cS1wN0t.addCurrent(w->m_mapS0cS1wN0t, round);
   m_mapS0cS1cN0w.addCurrent(w->m_mapS0cS1cN0w, round);
   m_mapS0cS1cN0t.addCurrent(w->m_mapS0cS1cN0t, round);
   m_mapS0tS1tN0t.addCurrent(w->m_mapS0tS1tN0t, round);
   m_mapS0jS1jN0t.addCurrent(w->m_mapS0jS1jN0t, round);

   m_mapS0wN0tN1t.addCurrent(w->m_mapS0wN0tN1t, round);
   m_mapS0cN0wN1t.addCurrent(w->m_mapS0cN0wN1t, round);
   m_mapS0cN0tN1w.addCurrent(w->m_mapS0cN0tN1w, round);
   m_mapS0cN0tN1t.addCurrent(w->m_mapS0cN0tN1t, round);
   m_mapS0tN0tN1t.addCurrent(w->m_mapS0tN0tN1t, round);
   m_mapS0jN0tN1t.addCurrent(w->m_mapS0jN0tN1t, round);

   m_mapS0wS1cS2c.addCurrent(w->m_mapS0wS1cS2c, round);
   m_mapS0cS1wS2c.addCurrent(w->m_mapS0cS1wS2c, round);
   m_mapS0cS1cS2w.addCurrent(w->m_mapS0cS1cS2w, round);
   m_mapS0cS1cS2c.addCurrent(w->m_mapS0cS1cS2c, round);
   m_mapS0tS1tS2t.addCurrent(w->m_mapS0tS1tS2t, round);

   m_mapS1cS2c.addCurrent(w->m_mapS1cS2c, round);
   m_mapS2cS3c.addCurrent(w->m_mapS2cS3c, round);
   m_mapN1tN2t.addCurrent(w->m_mapN1tN2t, round);
   m_mapN2tN3t.addCurrent(w->m_mapN2tN3t, round);

   m_mapS0cS2c.addCurrent(w->m_mapS0cS2c, round);
   m_mapS1cS3c.addCurrent(w->m_mapS1cS3c, round);
   m_mapS0cN1t.addCurrent(w->m_mapS0cN1t, round);
   m_mapN0tN2t.addCurrent(w->m_mapN0tN2t, round);
   m_mapN1tN3t.addCurrent(w->m_mapN1tN3t, round);

   m_mapS0cS0LcN0t.addCurrent(w->m_mapS0cS0LcN0t, round);
   m_mapS0wS0LcN0t.addCurrent(w->m_mapS0wS0LcN0t, round);
   m_mapS0cS0LcN0w.addCurrent(w->m_mapS0cS0LcN0w, round);
   m_mapS0cS0RcN0t.addCurrent(w->m_mapS0cS0RcN0t, round);
   m_mapS0cS0RjN0t.addCurrent(w->m_mapS0cS0RjN0t, round);
   m_mapS0wS0RcN0t.addCurrent(w->m_mapS0wS0RcN0t, round);
   m_mapS0cS0RcN0w.addCurrent(w->m_mapS0cS0RcN0w, round);
   m_mapS0cS0UcN0t.addCurrent(w->m_mapS0cS0UcN0t, round);
   m_mapS0wS0UcN0t.addCurrent(w->m_mapS0wS0UcN0t, round);
   m_mapS0cS0UcN0w.addCurrent(w->m_mapS0cS0UcN0w, round);

   m_mapS0cS0LcS1c.addCurrent(w->m_mapS0cS0LcS1c, round);
   m_mapS0cS0LjS1j.addCurrent(w->m_mapS0cS0LjS1j, round);
   m_mapS0wS0LcS1c.addCurrent(w->m_mapS0wS0LcS1c, round);
   m_mapS0cS0LcS1w.addCurrent(w->m_mapS0cS0LcS1w, round);
   m_mapS0cS0RcS1c.addCurrent(w->m_mapS0cS0RcS1c, round);
   m_mapS0wS0RcS1c.addCurrent(w->m_mapS0wS0RcS1c, round);
   m_mapS0cS0RcS1w.addCurrent(w->m_mapS0cS0RcS1w, round);
   m_mapS0cS0UcS1c.addCurrent(w->m_mapS0cS0UcS1c, round);
   m_mapS0wS0UcS1c.addCurrent(w->m_mapS0wS0UcS1c, round);
   m_mapS0cS0UcS1w.addCurrent(w->m_mapS0cS0UcS1w, round);

   m_mapN0tN1tN2t.addCurrent(w->m_mapN0tN1tN2t, round);
   m_mapS0cS1cS1Lc.addCurrent(w->m_mapS0cS1cS1Lc, round);
   m_mapS0wS1cS1Lc.addCurrent(w->m_mapS0wS1cS1Lc, round);
   m_mapS0cS1wS1Lc.addCurrent(w->m_mapS0cS1wS1Lc, round);
   m_mapS0cS1cS1Rc.addCurrent(w->m_mapS0cS1cS1Rc, round);
   m_mapS0jS1cS1Rj.addCurrent(w->m_mapS0jS1cS1Rj, round);
   m_mapS0wS1cS1Rc.addCurrent(w->m_mapS0wS1cS1Rc, round);
   m_mapS0cS1wS1Rc.addCurrent(w->m_mapS0cS1wS1Rc, round);
   m_mapS0cS1cS1Uc.addCurrent(w->m_mapS0cS1cS1Uc, round);
   m_mapS0wS1cS1Uc.addCurrent(w->m_mapS0wS1cS1Uc, round);
   m_mapS0cS1wS1Uc.addCurrent(w->m_mapS0cS1wS1Uc, round);
*/}

#ifdef NO_NEG_FEATURE
void TARGET_LANGUAGE::conparser::CWeight::setPositiveFeature(const CWeight*w) {
   iterate_double_templates(,.setPositiveFeature ID_LRB w-> , ID_RRB ; );
}
#endif

