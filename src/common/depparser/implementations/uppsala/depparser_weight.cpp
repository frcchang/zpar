// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                              *
 * weight.cpp - the weight definition method                    *
 *                                                              *
 * Part of the dependency parser of english                     *
 *                                                              *
 * Yue Zhang                                                    *
 * 2007.8                                                       *
 *                                                              *
 ****************************************************************/

#include "depparser_weight.h"

using namespace TARGET_LANGUAGE;
using namespace TARGET_LANGUAGE::depparser;

//const CScore<SCORE_TYPE> g_zeroScore;

#ifdef LABELED
   #define iterate_label_templates(left, right)\
      left(m_mapLabel)right\
      left(m_mapHeadWordLabel)right\
      left(m_mapDepWordLabel)right\
      left(m_mapHeadWordTagLabel)right\
      left(m_mapDepWordTagLabel)right\
      left(m_mapHeadTagLabel)right\
      left(m_mapDepTagLabel)right\
      left(m_mapHeadSurroundingTagsLabel)right\
      left(m_mapDepSurroundingTagsLabel)right
#else
   #define iterate_label_templates(left, right)\

#endif

#define iterate_templates(left, right) \
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
   left(m_mapSiblingAndParentTags)right \
\
   left(m_mapTwoSiblingTags)right \
   left(m_mapTwoSiblingAndParentTags)right \
   left(m_mapGrandChildTags)right \
   left(m_mapHeadWordTagArity)right \
   left(m_mapHeadTagArity)right \
\
   left(m_mapSTwt)right\
   left(m_mapSTw)right\
   left(m_mapSTt)right\
   left(m_mapN0wt)right\
   left(m_mapN0w)right\
   left(m_mapN0t)right\
   left(m_mapN1wt)right\
   left(m_mapN1w)right\
   left(m_mapN1t)right\
\
   left(m_mapSTwtN0wt)right\
   left(m_mapSTwtN0w)right\
   left(m_mapSTwN0wt)right\
   left(m_mapSTtN0wt)right\
   left(m_mapSTwtN0t)right\
   left(m_mapSTwN0w)right\
   left(m_mapSTtN0t)right\
\
   left(m_mapN0tN1t)right\
   left(m_mapN0tN1tN2t)right\
   left(m_mapSTtN0tN1t)right\
   left(m_mapSTtN0tN0LDt)right\
   left(m_mapSTHtSTtN0t)right\
   left(m_mapSTtSTLDtN0t)right\
   left(m_mapSTtSTRDtN0t)right\
\
   left(m_mapN0wN1t)right\
   left(m_mapN0wN1tN2t)right\
   left(m_mapSTtN0wN1t)right\
   left(m_mapSTtN0wN0LDt)right\
   left(m_mapSTHtSTtN0w)right\
   left(m_mapSTtSTLDtN0w)right\
   left(m_mapSTtSTRDtN0w)right\
\
   left(m_mapSTl)right\
   left(m_mapSTc)right\
   left(m_mapSTf)right\
\
   left(m_mapN0l)right\
   left(m_mapN0c)right\
   left(m_mapN0f)right\
\
   left(m_mapN1l)right\
   left(m_mapN1c)right\
   left(m_mapN1f)right\
\
   iterate_label_templates(left, right)

/*---------------------------------------------------------------
 *
 * loadScores - load scores from the file specified at constructor
 *              currently this uses database, but it can be modified
 *
 * Affects: m_bScoreModified, clearing it.
 *
 *--------------------------------------------------------------*/

void TARGET_LANGUAGE::depparser::CWeight::loadScores() {
   clock_t time_start = clock();
   std::cerr<<"Loading scores..."; std::cerr.flush();
   std::ifstream file ;
   file.open(m_sRecordPath.c_str()) ;

   if (!file.is_open()) {
      std::cerr << " empty." << std::endl; return;
   }

   iterate_templates(file >>,;);

   std::string s;
   getline(file, s);
   if (s=="Rules=1") {
      setRules(true);
   }
   else {
      ASSERT(s=="Rules=0", "Rules flag not found from the model file");
      setRules(false);
   }

   file.close() ;
   std::cerr << " done. (" << double(clock()-time_start)/CLOCKS_PER_SEC << "s)" << std::endl;
}

/*---------------------------------------------------------------
 *
 * saveScores - save scores back to database
 *
 * This method is called by the destructor is m_bScoreModified
 * is true.
   std::cerr<<"done."<<std::endl;
 *
 *--------------------------------------------------------------*/

void TARGET_LANGUAGE::depparser::CWeight::saveScores() {
   std::cerr<<"Saving scores..."; std::cerr.flush();
   std::ofstream file ;
   file.open(m_sRecordPath.c_str()) ;

   iterate_templates(file<<,;)
#ifdef DEBUG
   iterate_templates(,.trace(););
#endif
   if (m_bRules) file << "Rules=1" << std::endl;
   else file << "Rules=0" << std::endl;

   file.close();
   std::cerr<<" done."<<std::endl;
}

/*--------------------------------------------------------------
 *
 * computeAverageFeatureWeights - compute average feature weights
 *
 *-------------------------------------------------------------*/

void TARGET_LANGUAGE::depparser::CWeight::computeAverageFeatureWeights(int round) {
   std::cerr<<"Computing averaged (total) feature vector..."; std::cerr.flush();
   iterate_templates(,.computeAverage(round);) ;

   std::cerr<<"done."<<std::endl;
}

