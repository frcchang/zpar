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

//===============================================================

#define iterate_templates(left, right) \
   left(m_mapHeadWord)right \
   left(m_mapDepWord)right \
   left(m_mapHeadWordTag)right \
   left(m_mapDepWordTag)right \
   left(m_mapHeadTag)right \
   left(m_mapDepTag)right \
   left(m_mapHeadWordTagDepWordTag)right \
   left(m_mapHeadWordTagDepWord)right \
   left(m_mapHeadWordDepWordTag)right \
   left(m_mapHeadWordTagDepTag)right \
   left(m_mapHeadTagDepWordTag)right \
   left(m_mapHeadWordDepWord)right \
   left(m_mapHeadTagDepTag)right \
   left(m_mapBetweenTags)right \
   left(m_mapSurroundingTagsLL)right \
   left(m_mapSurroundingTagsLR)right \
   left(m_mapSurroundingTagsRL)right \
   left(m_mapSurroundingTagsRR)right \
   left(m_mapSiblingWords)right \
   left(m_mapSiblingWordTag)right \
   left(m_mapSiblingTagWord)right \
   left(m_mapSiblingTags)right \
   left(m_mapSiblingAndParentTags)right \
   left(m_mapGrandChildTags)right \
   left(m_mapHeadWordTagArity)right \
   left(m_mapHeadTagArity)right \
   left(m_mapHeadWordTagArityByTag)right \
   left(m_mapHeadTagArityByTag)right \
   left(m_mapTwoSiblingTags)right \
   left(m_mapTwoSiblingAndParentTags)right 

/*---------------------------------------------------------------
 *
 * loadScores - load scores from the file specified at constructor
 *              currently this uses database, but it can be modified
 *         
 * Affects: m_bScoreModified, clearing it. 
 *
 *--------------------------------------------------------------*/

void CWeight::loadScores() {
   TRACE("Loading scores...");
   std::ifstream file ; 
   file.open(m_sRecordPath.c_str()) ;

   iterate_templates(file >>,;);

   file.close() ;
   TRACE("Done");
}

/*---------------------------------------------------------------
 *
 * saveScores - save scores back to database
 *
 * This method is called by the destructor is m_bScoreModified
 * is true.
 *
 *--------------------------------------------------------------*/

void CWeight::saveScores() {
   TRACE("saving scores.");
   std::ofstream file ;
   file.open(m_sRecordPath.c_str()) ;

   iterate_templates(file<<,;)

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
   iterate_templates(,.computeAverage(round);) ;
   
   TRACE("Done");
}

