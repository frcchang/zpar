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
   cout << "Loading model..."; cout.flush();
   ifstream is(m_sFeatureDB.c_str());
   iterate_templates(is>>,;);
   is.close();
   cout << " done." << endl ;
}

/*--------------------------------------------------------------
 *
 * saveScores - save score from specific weight structure
 *
 *-------------------------------------------------------------*/

void TARGET_LANGUAGE::tagger::CWeight::saveScores() {
   cout << "Saving model..."; cout.flush();
   ofstream os(m_sFeatureDB.c_str());
   assert(os.is_open());
   iterate_templates(os<<,;);
   os.close();
   cout << " done." << endl ;
}

/*--------------------------------------------------------------
 *
 * computeAverageFeatureWeights - compute average feature weights
 *
 *-------------------------------------------------------------*/

void TARGET_LANGUAGE::tagger::CWeight::computeAverageFeatureWeights(int round) {
   cout << "Computing averaged feature scores...";
   iterate_templates(,.computeAverage(round););
   cout << " Done" << endl;
}



