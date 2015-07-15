// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                              *
 * tagger_weight.cpp - the weight definition method             *
 *                                                              *
 * Part of the part-of-speech tagger of common                  *
 *                                                              *
 * Yue Zhang                                                    *
 * 2007.10                                                      *
 *                                                              *
 ****************************************************************/

#include "tagger_weight.h"

using namespace TARGET_LANGUAGE;
using namespace TARGET_LANGUAGE::tagger;

/*--------------------------------------------------------------
 *
 * loadScores - load score from a certain file for a weight
 *
 *------------------------------------------------------------*/

void TARGET_LANGUAGE::tagger::CWeight::loadScores() {
   std::cerr << "Loading model..."; std::cerr.flush();
   std::ifstream is(m_sFeatureDB.c_str());
   iterate_templates(is>>,;);
#ifdef DEBUG
   iterate_templates(,.trace(););
#endif
   is.close();
   std::cerr << " done." << std::endl ;
}

/*--------------------------------------------------------------
 *
 * saveScores - save score from specific weight structure
 *
 *-------------------------------------------------------------*/

void TARGET_LANGUAGE::tagger::CWeight::saveScores() {
   std::cerr << "Saving model..."; std::cerr.flush();
   std::ofstream os(m_sFeatureDB.c_str());
   assert(os.is_open());
   iterate_templates(os<<,;);
   os.close();
   std::cerr << " done." << std::endl ;
}

/*--------------------------------------------------------------
 *
 * computeAverageFeatureWeights - compute average feature weights
 *
 *-------------------------------------------------------------*/

void TARGET_LANGUAGE::tagger::CWeight::computeAverageFeatureWeights(int round) {
   std::cerr << "Computing averaged feature scores...";
   iterate_templates(,.computeAverage(round););
   std::cerr << " Done" << std::endl;
}



