// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                              *
 * weight.cpp - the weight definition method                    *
 *                                                              *
 * Part of the dependency labeler of english                     *
 *                                                              *
 * Yue Zhang                                                    *
 * 2007.8                                                       *
 *                                                              *
 ****************************************************************/

#include "weight.h"

using namespace TARGET_LANGUAGE;
using namespace TARGET_LANGUAGE::deplabeler;

#define iterate_templates(left, right)\
   left(m_mapLabel)right\
   left(m_mapHeadWordLabel)right\
   left(m_mapDepWordLabel)right\
   left(m_mapHeadWordTagLabel)right\
   left(m_mapDepWordTagLabel)right\
   left(m_mapHeadTagLabel)right\
   left(m_mapDepTagLabel)right\
   left(m_mapHeadSurroundingTagsLabel)right\
   left(m_mapDepSurroundingTagsLabel)right

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
   std::cout<<"Loading scores..."; std::cout.flush();
   std::ifstream file ; 
   file.open(m_sRecordPath.c_str()) ;

   if (!file.is_open()) {
      std::cout << " empty." << std::endl; return;
   }

   iterate_templates(file >>,;);

   file.close() ;
   std::cout << " done. (" << double(clock()-time_start)/CLOCKS_PER_SEC << "s)" << std::endl;
}

/*---------------------------------------------------------------
 *
 * saveScores - save scores back to database
 *
 * This method is called by the destructor is m_bScoreModified
 * is true.
   std::cout<<"done."<<std::endl;
 *
 *--------------------------------------------------------------*/

void CWeight::saveScores() {
   std::cout<<"Saving scores..."; std::cout.flush();
   std::ofstream file ;
   file.open(m_sRecordPath.c_str()) ;

   iterate_templates(file<<,;)

   file.close();
   std::cout<<" done."<<std::endl;
}

/*--------------------------------------------------------------
 *
 * computeAverageFeatureWeights - compute average feature weights
 *
 *-------------------------------------------------------------*/

void CWeight::computeAverageFeatureWeights(int round) {
   std::cout<<"Computing averaged (total) feature vector..."; std::cout.flush();
   iterate_templates(,.computeAverage(round);) ;

   std::cout<<"done."<<std::endl;
}

