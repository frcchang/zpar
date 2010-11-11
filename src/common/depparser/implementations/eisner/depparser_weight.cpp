// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                              *
 * weight.cpp - the weight definition method                    *
 *                                                              *
 * Part of the dependency parser                                *
 *                                                              *
 * Yue Zhang                                                    *
 * 2007.8                                                       *
 *                                                              *
 ****************************************************************/

#include "depparser_weight.h"

using namespace TARGET_LANGUAGE;
using namespace TARGET_LANGUAGE::depparser;

#define iterate_templates(left, right) \
   left(m_mapHeadWord)right \
   left(m_mapDepWord)right \
   left(m_mapHeadWordTag)right \
   left(m_mapDepWordTag)right \
   left(m_mapHeadTag)right \
   left(m_mapDepTag)right \
   left(m_mapHeadWordTagDepWordTag)right \
   left(m_mapHeadWordTagDepWord)right \
   left(m_mapHeadWordTagDepTag)right \
   left(m_mapHeadWordDepWordTag)right \
   left(m_mapHeadTagDepWordTag)right \
   left(m_mapHeadWordDepWord)right \
   left(m_mapHeadTagDepTag)right \
   left(m_mapBetweenTags)right \
   left(m_mapSurroundingTagsLL)right \
   left(m_mapSurroundingTagsLR)right \
   left(m_mapSurroundingTagsRL)right \
   left(m_mapSurroundingTagsRR)right 

/*--------------------------------------------------------------
 *
 * loadScores - load score from a certain file for a weight
 *              structure specifically.
 * 
 * Inputs: sFeatureDB - the feature database file
 *
 *------------------------------------------------------------*/

void CWeight::loadScores() {
   TRACE("Loading scores...");
   std::ifstream file(m_sRecordPath.c_str());
   iterate_templates(file>>,;);
   file.close();
   TRACE("Done");
}

/*--------------------------------------------------------------
 *
 * saveScores - save score from specific weight structure
 *              to some specific file. 
 *
 * Inputs: sFeatureDB - file for the data
 *
 *-------------------------------------------------------------*/

void CWeight::saveScores() {
   TRACE("saving scores.");
   std::ofstream file(m_sRecordPath.c_str());
   iterate_templates(file<<,;);
   file.close();
   TRACE("Done");
}

/*--------------------------------------------------------------
 *
 * computeAverageFeatureWeights - compute average feature weights
 *
 *-------------------------------------------------------------*/

void CWeight::computeAverageFeatureWeights(int round) {
   TRACE("adding total feature vector.");
   iterate_templates(,.computeAverage(round););
   TRACE("Done");
}



