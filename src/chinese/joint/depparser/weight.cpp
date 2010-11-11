// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                              *
 * weight.cpp - the weight definition method                    *
 *                                                              *
 * Part of the reranking joint parsing tagging                  *
 *                                                              *
 * Yue Zhang                                                    *
 * 2008.                                                       *
 *                                                              *
 ****************************************************************/

#include "weight.h"

using namespace chinese;
using namespace chinese::reranker;

#define iterate_templates(left, right) \
   left(m_mapPriors)right \
   

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



