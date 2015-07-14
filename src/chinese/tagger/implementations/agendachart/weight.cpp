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
   left(m_mapSeenWords)right \
   left(m_mapLastWordByWord)right \
   left(m_mapCurrentWordLastChar)right \
   left(m_mapLastWordFirstChar)right \
   left(m_mapFirstCharLastWordByWord)right \
   left(m_mapLastWordByLastChar)right \
   left(m_mapSeparateChars)right \
   left(m_mapConsecutiveChars)right \
   left(m_mapFirstAndLastChars)right \
   left(m_mapOneCharWord)right \
   left(m_mapLengthByFirstChar)right \
   left(m_mapLengthByLastChar)right \
   left(m_mapLengthByLastWord)right \
   left(m_mapLastLengthByWord)right \
   left(m_mapLastTagByTag)right \
   left(m_mapLastTwoTagsByTag)right \
   left(m_mapCurrentTag)right \
   left(m_mapTagByLastWord)right \
   left(m_mapLastTagByWord)right \
   left(m_mapTagByFirstChar)right \
   left(m_mapTagByLastChar)right \
   left(m_mapTagByChar)right \
   left(m_mapTagOfOneCharWord)right \
   left(m_mapRepeatedCharByTag)right \
   left(m_mapTagByWordAndPrevChar)right \
   left(m_mapTagByWordAndNextChar)right \
   left(m_mapTaggedCharByFirstChar)right \
   left(m_mapTaggedCharByLastChar)right \
   left(m_mapTagByFirstCharCat)right \
   left(m_mapTagByLastCharCat)right

/*---------------------------------------------------------------
 *
 * loadScores - load scores from the file specified at constructor
 *              currently this uses database, but it can be modified
 *
 *--------------------------------------------------------------*/

void CWeight::loadScores() {
   std::cerr << "Loading scores ..."; std::cerr.flush();
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
   if (st=="Segmentation rules") {
      m_bSegmentationRules = true;
   }
   else {
      ASSERT(st=="Segmentation rules=0", "Segmentation rules on/off switch not found from model.");
      m_bSegmentationRules = false;
   }
   getline(is, st);
   if (st=="Knowledge") {
      ASSERT(m_Knowledge==0, "Model loading knowledge but it already exists.")
      is >> (*m_Knowledge);
   }
   else {
      ASSERT(st=="Knowledge=0", "Knowledge not found from model.");
   }
   getline(is, st);
   ASSERT(st=="Tag dictionary", "Tag dictionary not found from model.");
   is >> m_mapTagDictionary;
   getline(is, st);
   ASSERT(st=="Char tag dictionary", "Char tag dictionary not found from model.");
   is >> m_mapCharTagDictionary;
   getline(is, st);
   ASSERT(st=="Word frequency", "Word frequency not found from model.");
   is >> m_mapWordFrequency;
   getline(is, st);
   ASSERT(st=="Maximum frequency", "Maximum frequency not found from model.");
   getline(is, st);
   iss.str(st);
   iss >> m_nMaxWordFrequency;
   getline(is, st);
   getline(is, st);
   ASSERT(st=="Maximum wordlen by tag", "Maximum word length by tag not found from model.");
   std::string ts;
   for (int i=0; i<CTag::COUNT; ++i) {
      std::istringstream iss;
      getline(is, st);
      iss.str(st);
      int j;
      iss >> ts >> j;
      m_maxLengthByTag[i] = j;
      ASSERT(CTag(ts).code()==i, "Maximum word size record loading failed.");
   }
   {
      std::istringstream iss;
      getline(is, st);
      iss.str(st);
      iss >> ts >> m_maxLengthByTag[CTag::COUNT];
      ASSERT(ts=="All", "Maximum word size record failed loading.");
   }

   std::cerr << " done. (" << double(clock()-time_start)/CLOCKS_PER_SEC << "s)" << std::endl;
}

/*---------------------------------------------------------------
 *
 * saveScores - save scores back to database
 *
 *--------------------------------------------------------------*/

void CWeight::saveScores() {
   std::cerr << "Saving scores ..."; std::cerr.flush();

   std::ofstream os(m_sFeatureDB.c_str());
   ASSERT(os.is_open(), "Can't open "<<m_sFeatureDB<<" for saving model.");
   iterate_templates(os<<,;);

   if (m_bSegmentationRules) os << "Segmentation rules" << std::endl;
   else os << "Segmentation rules=0" << std::endl;

   if (m_Knowledge==0) {
      os << "Knowledge=0" << std::endl;
   }
   else {
      os << "Knowledge" << std::endl;
      os << (*m_Knowledge);
   }

   os << "Tag dictionary" << std::endl;
   os << m_mapTagDictionary;
   os << "Char tag dictionary" << std::endl;
   os << m_mapCharTagDictionary;
   os << "Word frequency" << std::endl;
   os << m_mapWordFrequency;
   os << "Maximum frequency" << std::endl;
   os << m_nMaxWordFrequency << std::endl;
   os << std::endl << "Maximum wordlen by tag" << std::endl;
   for (int i=0; i<CTag::COUNT; ++i)
      os << CTag(i).str() << "\t" << m_maxLengthByTag[i] << std::endl;
   os << "All\t" << m_maxLengthByTag[CTag::COUNT] << std::endl;
   std::cerr << " done." << std::endl;
}

/*--------------------------------------------------------------
 *
 * computeAverageFeatureWeights - compute average feature weights
 *
 *-------------------------------------------------------------*/

void CWeight::computeAverageFeatureWeights(unsigned long round) {
   std::cerr << "Comuting averaged feature vector ..."; std::cerr.flush();
   iterate_templates(,.computeAverage(round););
   std::cerr << " done." << std::endl;
}


