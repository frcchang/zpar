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

#include "weight.h"

using namespace TARGET_LANGUAGE;
using namespace TARGET_LANGUAGE::depparser;

const CScore<SCORE_TYPE> g_zeroScore;

/*
   left(m_mapHeadWord)right \
   left(m_mapDepWord)right \
   left(m_mapHeadWordTag)right \
   left(m_mapDepWordTag)right \
   left(m_mapHeadTag)right \
   left(m_mapDepTag)right \
   left(m_mapHeadWordTagDepWordTag)right \
   left(m_mapHeadWordTagDepWord)right \
   left(m_mapHeadWordTagDepTag)right \
   left(m_mapHeadWordDepWordTag)right \
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
   left(m_mapTwoSiblingTags)right \
   left(m_mapTwoSiblingAndParentTags)right \
   left(m_mapGrandChildTags)right \
   left(m_mapHeadWordTagArity)right \
   left(m_mapHeadTagArity)right 
*/

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
   iterate_label_templates(left, right)

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
   ifstream file ; 
   file.open(m_sRecordPath.c_str()) ;

   iterate_templates(file >>,;);
/*
   file >> m_mapHeadWord ; TRACE(1);
   file >> m_mapDepWord ; TRACE(2);
   file >> m_mapHeadWordTag ; TRACE(3);
   file >> m_mapDepWordTag ; TRACE(4);
   file >> m_mapHeadTag ; TRACE(5);
   file >> m_mapDepTag ; TRACE(6);
   file >> m_mapHeadWordTagDepWordTag ; TRACE(7);
   file >> m_mapHeadWordTagDepWord ; TRACE(8);
   file >> m_mapHeadWordTagDepTag ; TRACE(9);
   file >> m_mapHeadWordDepWordTag ; TRACE(10);
   file >> m_mapHeadTagDepWordTag ; TRACE(11);
   file >> m_mapHeadWordDepWord ; TRACE(12);
   file >> m_mapHeadTagDepTag ; TRACE(13);
   file >> m_mapBetweenTags ; TRACE(14);
   file >> m_mapSurroundingTagsLL ; TRACE(15);
   file >> m_mapSurroundingTagsLR ; TRACE(16);
   file >> m_mapSurroundingTagsRL ; TRACE(17);
   file >> m_mapSurroundingTagsRR ; TRACE(18);
   file >> m_mapSiblingWords ; TRACE(19);
   file >> m_mapSiblingWordTag ; TRACE(20);
   file >> m_mapSiblingTagWord ; TRACE(21);
   file >> m_mapSiblingTags ; TRACE(22);
   file >> m_mapSiblingAndParentTags ; TRACE(23);
   file >> m_mapTwoSiblingTags ; TRACE(24);
   file >> m_mapTwoSiblingAndParentTags ; TRACE(25);
   file >> m_mapLocalHeadAndNextTwoTags ; TRACE(26);
   file >> m_mapTwoLocalHeadAndNextTags ; TRACE(27);
   file >> m_mapShiftedWordTag ; TRACE(28);
   file >> m_mapShiftedWordTagNextTag ; TRACE(29);
   file >> m_mapShiftedWordTagNextTwoTags ; TRACE(30);
   file >> m_mapRootWordTag ; TRACE(31);
   file >> m_mapGrandChildTags ; TRACE(32);
   file >> m_mapHeadWordTagArity ; TRACE(33);
   file >> m_mapHeadTagArity ; TRACE(34);
*/

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
   ofstream file ;
   file.open(m_sRecordPath.c_str()) ;

   iterate_templates(file<<,;)
/*
   file << m_mapHeadWord ;
   file << m_mapDepWord ;
   file << m_mapHeadWordTag ;
   file << m_mapDepWordTag ;
   file << m_mapHeadTag ;
   file << m_mapDepTag ;
   file << m_mapHeadWordTagDepWordTag ;
   file << m_mapHeadWordTagDepWord ;
   file << m_mapHeadWordTagDepTag ;
   file << m_mapHeadWordDepWordTag ;
   file << m_mapHeadTagDepWordTag ;
   file << m_mapHeadWordDepWord ;
   file << m_mapHeadTagDepTag ;
   file << m_mapBetweenTags ;
   file << m_mapSurroundingTagsLL ;
   file << m_mapSurroundingTagsLR ;
   file << m_mapSurroundingTagsRL ;
   file << m_mapSurroundingTagsRR ;
   file << m_mapSiblingWords ;
   file << m_mapSiblingWordTag ;
   file << m_mapSiblingTagWord ;
   file << m_mapSiblingTags ;
   file << m_mapSiblingAndParentTags ;
   file << m_mapTwoSiblingTags ;
   file << m_mapTwoSiblingAndParentTags ;
   file << m_mapLocalHeadAndNextTwoTags ;
   file << m_mapTwoLocalHeadAndNextTags ;
   file << m_mapShiftedWordTag ;
   file << m_mapShiftedWordTagNextTag ;
   file << m_mapShiftedWordTagNextTwoTags ;
   file << m_mapRootWordTag ;
   file << m_mapGrandChildTags ;
   file << m_mapHeadWordTagArity ;
   file << m_mapHeadTagArity ;
*/

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
   
//   totalScore( m_mapHeadWord , round ) ;
//   totalScore( m_mapDepWord , round ) ;
//   totalScore( m_mapHeadWordTag , round ) ;
//   totalScore( m_mapDepWordTag , round ) ;
//   totalScore( m_mapHeadTag , round ) ;
//   totalScore( m_mapDepTag , round ) ;
//   totalScore( m_mapHeadWordTagDepWordTag , round ) ;
//   totalScore( m_mapHeadWordTagDepWord , round ) ;
//   totalScore( m_mapHeadWordTagDepTag , round ) ;
//   totalScore( m_mapHeadWordDepWordTag , round ) ;
//   totalScore( m_mapHeadTagDepWordTag , round ) ;
//   totalScore( m_mapHeadWordDepWord , round ) ;
//   totalScore( m_mapHeadTagDepTag , round ) ;
//   totalScore( m_mapBetweenTags , round ) ;
//   totalScore( m_mapSurroundingTagsLL , round ) ;
//   totalScore( m_mapSurroundingTagsLR , round ) ;
//   totalScore( m_mapSurroundingTagsRL , round ) ;
//   totalScore( m_mapSurroundingTagsRR , round ) ;
//   totalScore( m_mapSiblingWords , round ) ;
//   totalScore( m_mapSiblingWordTag , round ) ;
//   totalScore( m_mapSiblingTagWord , round ) ;
//   totalScore( m_mapSiblingTags , round ) ;
//   totalScore( m_mapSiblingAndParentTags , round ) ;
//   totalScore( m_mapTwoSiblingTags , round ) ;
//   totalScore( m_mapTwoSiblingAndParentTags , round ) ;
//   totalScore( m_mapLocalHeadAndNextTwoTags , round ) ;
//   totalScore( m_mapTwoLocalHeadAndNextTags , round ) ;
//   totalScore( m_mapShiftedWordTag , round ) ;
//   totalScore( m_mapShiftedWordTagNextTag , round ) ;
//   totalScore( m_mapShiftedWordTagNextTwoTags , round ) ;
//   totalScore( m_mapRootWordTag , round ) ;
//   totalScore( m_mapGrandChildTags , round ) ;
//   totalScore( m_mapHeadWordTagArity , round ) ;
//   totalScore( m_mapHeadTagArity , round ) ;

   TRACE("Done");
}

