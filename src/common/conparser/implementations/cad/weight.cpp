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
   std::cerr << "Loading scores..."; std::cerr.flush();
//   std::ifstream file ;
//   file.open(m_sRecordPath.c_str()) ;

   if (!file.is_open()) {
      std::cerr << " empty." << std::endl; return;
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
   std::cerr << " done. (" << double(clock()-time_start)/CLOCKS_PER_SEC << "s)" << std::endl;
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
   std::cerr << "Saving scores..."; std::cerr.flush();
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
   std::cerr<<" done."<<std::endl;
}

/*--------------------------------------------------------------
 *
 * computeAverageFeatureWeights - compute average feature weights
 *
 *-------------------------------------------------------------*/

void TARGET_LANGUAGE::conparser::CWeight::computeAverageFeatureWeights(int round) {
   std::cerr<<"Computing averaged (total) feature vector..."; std::cerr.flush();

   iterate_templates(,.computeAverage(round);) ;

   std::cerr<<"done."<<std::endl;
}

void TARGET_LANGUAGE::conparser::CWeight::addCurrent(CWeight*w, int round) {
   iterate_double_templates(, .addCurrent ID_LRB w-> , ID_COMMA round ID_RRB ;);
}

void TARGET_LANGUAGE::conparser::CWeight::subtractCurrent(CWeight*w, int round) {
   iterate_double_templates(, .subtractCurrent ID_LRB w-> , ID_COMMA round ID_RRB ;);
}

SCORE_TYPE TARGET_LANGUAGE::conparser::CWeight::dotProduct(CWeight&w) {
   iterate_double_templates(, .dotProduct ID_LRB w. , ID_RRB ;);
}

#ifdef NO_NEG_FEATURE
void TARGET_LANGUAGE::conparser::CWeight::setPositiveFeature(const CWeight*w) {
   iterate_double_templates(,.setPositiveFeature ID_LRB w-> , ID_RRB ; );
}
#endif

