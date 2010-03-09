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
   cout << "Loading scores...";
   clock_t start_time = clock();
   string st;
   ifstream is(m_sFeatureDB.c_str());
   istringstream iss;

   if (!is.is_open()) {
      cout << " empty." << endl; return;
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

   cout << " done. (" << double(clock()-time_start)/CLOCKS_PER_SEC << "s)" << endl;
}

/*---------------------------------------------------------------
 *
 * saveScores - save scores back to database
 *
 *--------------------------------------------------------------*/

void CWeight::saveScores() {
   cout << "Saving scores...";
   ofstream os(m_sFeatureDB.c_str());
   assert(os.is_open());
   iterate_templates(os<<,;);
   os << "Tag dictionary" << endl;
   os << m_mapTagDictionary;
   os << "Char tag dictionary" << endl;
   os << m_mapCharTagDictionary;
   os << "Word frequency" << endl;
   os << m_mapWordFrequency;
   os << "Maximum frequency" << endl;
   os << m_nMaxWordFrequency << endl;
   cout << " done." << endl;
}

/*--------------------------------------------------------------
 *
 * computeAverageFeatureWeights - compute average feature weights
 *
 *-------------------------------------------------------------*/

void CWeight::computeAverageFeatureWeights(const unsigned long round) {
   cout << ("Adding total feature vector... ");
   iterate_templates(,.computeAverage(round););
   cout <<("Done") << endl;
}


