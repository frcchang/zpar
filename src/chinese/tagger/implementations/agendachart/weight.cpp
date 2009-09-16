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
   left(m_mapTagByLastCharCat)right \
   left(m_mapSeparateCharCat)right \
   left(m_mapConsecutiveCharCat)right

/*---------------------------------------------------------------
 *
 * loadScores - load scores from the file specified at constructor
 *              currently this uses database, but it can be modified
 *
 *--------------------------------------------------------------*/

void CWeight::loadScores() {
   cout << "Loading scores ..."; cout.flush();
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
   getline(is, st);
   getline(is, st);
   assert(st=="Maximum wordlen by tag");
   string ts;
   for (int i=0; i<CTag::COUNT; ++i) {
      istringstream iss;
      getline(is, st);
      iss.str(st);
      int j;
      iss >> ts >> j;
      m_maxLengthByTag[i] = j;
      assert(CTag(ts).code()==i);
   }
   {
      istringstream iss;
      getline(is, st);
      iss.str(st);
      iss >> ts >> m_maxLengthByTag[CTag::COUNT];
      assert(ts=="All");
   }

   cout << " done." << endl;
}

/*---------------------------------------------------------------
 *
 * saveScores - save scores back to database
 *
 *--------------------------------------------------------------*/

void CWeight::saveScores() {
   cout << "Saving scores ..."; cout.flush();
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
   os << endl << "Maximum wordlen by tag" << endl;
   for (int i=0; i<CTag::COUNT; ++i)
      os << CTag(i).str() << "\t" << m_maxLengthByTag[i] << endl;
   os << "All\t" << m_maxLengthByTag[CTag::COUNT] << endl;
   cout << " done." << endl;
}

/*--------------------------------------------------------------
 *
 * computeAverageFeatureWeights - compute average feature weights
 *
 *-------------------------------------------------------------*/

void CWeight::computeAverageFeatureWeights(int round) {
   cout << "Comuting averaged feature vector ..."; cout.flush();
   iterate_templates(,.computeAverage(round););
   cout << " done." << endl;
}


