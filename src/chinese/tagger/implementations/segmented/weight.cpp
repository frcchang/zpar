// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                              *
 * weight.cpp - the impl of tagger weights                      *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2007.5                         *
 *                                                              *
 ****************************************************************/

#include "weight.h"

using namespace chinese;
using namespace chinese::tagger;

#define iterate_templates(left,right) \
   left(m_mapLastTagByTag)right \
   left(m_mapLastTwoTagsByTag)right \
   left(m_mapCurrentTag)right \
   left(m_mapTagByLastWord)right \
   left(m_mapLastTagByWord)right \
   left(m_mapTagByFirstChar)right \
   left(m_mapTagByLastChar)right \
   left(m_mapTagByPrevChar)right \
   left(m_mapTagByNextChar)right \
   left(m_mapTagByPrevTwoChar)right \
   left(m_mapTagByNextTwoChar)right \
   left(m_mapTagOfOneCharWord)right \
   left(m_mapTagByChar)right \
   left(m_mapRepeatedCharByTag)right \
   left(m_mapTagByWordAndPrevChar)right \
   left(m_mapTagByWordAndNextChar)right \
   left(m_mapTagBySeparateChars)right \
   left(m_mapTaggedCharByFirstChar)right \
   left(m_mapTaggedCharByLastChar)right \
   left(m_mapTagByFirstCharCat)right

/*---------------------------------------------------------------
 *
 * loadScores - load scores from the file specified at constructor
 *              currently this uses database, but it can be modified
 *
 *--------------------------------------------------------------*/

void CWeight::loadScores() {
   std::cerr << "Loading scores...";
   clock_t time_start = clock();
   std::string st;
   std::ifstream is(m_sFeatureDB.c_str());
   std::istringstream iss;

   if (!is.is_open()) {
      std::cerr << " empty." << std::endl; return;
   }
   // load feature weights
   iterate_templates(is>>,;);

   // load tag dictionary
   getline(is, st);
   assert(st=="Tag dictionary");
   is >> m_mapTagDictionary;
   getline(is, st);
   assert(st=="Char tag dictionary");
   is >> m_mapCharTagDictionary;
   getline(is, st);
   assert(st=="Word frequency");
   is >> m_mapWordFrequency;
   getline(is, st);
   assert(st=="Maximum frequency");
   getline(is, st);
   iss.str(st);
   iss >> m_nMaxWordFrequency;

   std::cerr << " done. (" << double(clock()-time_start)/CLOCKS_PER_SEC << "s)" << std::endl;
}

/*---------------------------------------------------------------
 *
 * saveScores - save scores back to database
 *
 *--------------------------------------------------------------*/

void CWeight::saveScores() {
   std::cerr << "Saving scores...";
   std::ofstream os(m_sFeatureDB.c_str());
   assert(os.is_open());
   iterate_templates(os<<,;);
   os << "Tag dictionary" << std::endl;
   os << m_mapTagDictionary;
   os << "Char tag dictionary" << std::endl;
   os << m_mapCharTagDictionary;
   os << "Word frequency" << std::endl;
   os << m_mapWordFrequency;
   os << "Maximum frequency" << std::endl;
   os << m_nMaxWordFrequency << std::endl;
   std::cerr << " done." << std::endl;
}

/*--------------------------------------------------------------
 *
 * computeAverageFeatureWeights - compute average feature weights
 *
 *-------------------------------------------------------------*/

void CWeight::computeAverageFeatureWeights(const unsigned long round) {
   std::cerr << ("Adding total feature vector... ");
   iterate_templates(,.computeAverage(round););
   std::cerr <<("Done") << std::endl;
}


