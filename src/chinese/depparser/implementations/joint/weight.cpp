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
using namespace chinese::depparser;

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
   left(m_mapConsecutiveCharCat)right\
\
   left(m_mapHeadWord)right \
   left(m_mapDepWord)right \
   left(m_mapHeadWordTag)right \
   left(m_mapDepWordTag)right \
   left(m_mapHeadTag)right \
   left(m_mapDepTag)right \
\
   left(m_mapHeadWordTagDepWordTag)right \
   left(m_mapHeadWordTagDepWord)right \
   left(m_mapHeadWordTagDepTag)right \
   left(m_mapHeadWordDepWordTag)right \
   left(m_mapHeadTagDepWordTag)right \
   left(m_mapHeadWordDepWord)right \
   left(m_mapHeadTagDepTag)right \
\
   left(m_mapBetweenTags)right \
   left(m_mapSurroundingTagsLL)right \
   left(m_mapSurroundingTagsLR)right \
   left(m_mapSurroundingTagsRL)right \
   left(m_mapSurroundingTagsRR)right \
\
   left(m_mapSiblingWords)right \
   left(m_mapSiblingWordTag)right \
   left(m_mapSiblingTagWord)right \
   left(m_mapSiblingTags)right \
   left(m_mapSiblingAndParentTags)right 

/*---------------------------------------------------------------
 *
 * loadScores - load scores from the file specified at constructor
 *              currently this uses database, but it can be modified
 *
 *--------------------------------------------------------------*/

void CWeight::loadScores() {
   cout << "Loading scores...";
   string st;
   ifstream is(m_sRecordPath.c_str());
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
   for (int i=0; i<PENN_TAG_COUNT; ++i) {
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
      iss >> ts >> m_maxLengthByTag[PENN_TAG_COUNT];
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
   cout << "Saving scores...";
   ofstream os(m_sRecordPath.c_str());
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
   for (int i=0; i<PENN_TAG_COUNT; ++i)
      os << CTag(i).str() << "\t" << m_maxLengthByTag[i] << endl;
   os << "All\t" << m_maxLengthByTag[PENN_TAG_COUNT] << endl;
   cout << " done." << endl;
}

/*--------------------------------------------------------------
 *
 * computeAverageFeatureWeights - compute average feature weights
 *
 *-------------------------------------------------------------*/

void CWeight::computeAverageFeatureWeights(int round) {
   cout << "Comuting averaged feature vector... ";
   iterate_templates(,.computeAverage(round););
   cout << "done." << endl;
}


