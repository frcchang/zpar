// Copyright (C) University of Oxford 2010
/***************************************************************
 *
 * weight.h - the constituent parser weights 
 *
 * Yue Zhang, 2007.8 - 2008.1
 *
 ***************************************************************/

#ifndef _CONPARSER_WEIGHTS_H
#define _CONPARSER_WEIGHTS_H

#include "weight_base.h"

const static unsigned DEFAULT_TABLE_SIZE = 1<<17;

#define ID(x) x
#define ID_LRB (
#define ID_RRB )
#define ID_COMMA ,
#define ID_POINTER ->

#define iterate_templates(left, right) \
   left(m_mapS0c)right\
   left(m_mapS0w)right\
   left(m_mapS0tc)right\
   left(m_mapS0wc)right\
\
   left(m_mapS1c)right\
   left(m_mapS1w)right\
   left(m_mapS1tc)right\
   left(m_mapS1wc)right\
\
   left(m_mapS2tc)right\
   left(m_mapS2wc)right\
\
   left(m_mapS3tc)right\
   left(m_mapS3wc)right\
\
   left(m_mapN0w)right\
   left(m_mapN0t)right\
   left(m_mapN0wt)right\
\
   left(m_mapN1w)right\
   left(m_mapN1t)right\
   left(m_mapN1wt)right\
   left(m_mapN01w)right\
\
   left(m_mapN2w)right\
   left(m_mapN2t)right\
   left(m_mapN2wt)right\
   left(m_mapN12w)right\
   left(m_mapN012w)right\
\
   left(m_mapN3w)right\
   left(m_mapN3t)right\
   left(m_mapN3wt)right\
   left(m_mapN23w)right\
   left(m_mapN123w)right\
\
   left(m_mapS0Lwc)right\
   left(m_mapS0Ltc)right\
   left(m_mapS0Rwc)right\
   left(m_mapS0Rtc)right\
   left(m_mapS0Uwc)right\
   left(m_mapS0Utc)right\
   left(m_mapS1Lwc)right\
   left(m_mapS1Ltc)right\
   left(m_mapS1Rwc)right\
   left(m_mapS1Rtc)right\
   left(m_mapS1Uwc)right\
   left(m_mapS1Utc)right\
\
   left(m_mapS0wcS1wc)right\
   left(m_mapS0wS1c)right\
   left(m_mapS0cS1w)right\
   left(m_mapS0cS1c)right\
   left(m_mapBetweenTags)right\
\
   left(m_mapS0wN0w)right\
   left(m_mapS0cN0w)right\
   left(m_mapS0wN0t)right\
   left(m_mapS0cN0t)right\
   left(m_mapS0cmN0tm)right\
\
   left(m_mapS1wN0w)right\
   left(m_mapS1cN0w)right\
   left(m_mapS1wN0t)right\
   left(m_mapS1cN0t)right\
\
   left(m_mapN0wN1w)right\
   left(m_mapN0tN1w)right\
   left(m_mapN0wN1t)right\
   left(m_mapN0tN1t)right\
\
   left(m_mapS0wS1cN0t)right\
   left(m_mapS0cS1wN0t)right\
   left(m_mapS0cS1cN0w)right\
   left(m_mapS0cS1cN0t)right\
   left(m_mapS0tS1tN0t)right\
   left(m_mapS0jS1jN0t)right\
\
   left(m_mapS0cN1w)right\
   left(m_mapS0wN0tN1t)right\
   left(m_mapS0cN0wN1t)right\
   left(m_mapS0cN0tN1w)right\
   left(m_mapS0cN0tN1t)right\
   left(m_mapS0tN0tN1t)right\
   left(m_mapS0jN0tN1t)right\
\
   left(m_mapS0wS1cS2c)right\
   left(m_mapS0cS1wS2c)right\
   left(m_mapS0cS1cS2w)right\
   left(m_mapS0cS1cS2c)right\
   left(m_mapS0tS1tS2t)right\
\
   left(m_mapS1cS2c)right\
   left(m_mapS2cS3c)right\
   left(m_mapN1tN2t)right\
   left(m_mapN2tN3t)right\
\
   left(m_mapS0cS2c)right\
   left(m_mapS1cS3c)right\
   left(m_mapS0cN1t)right\
   left(m_mapN0tN2t)right\
   left(m_mapN1tN3t)right\
\
   left(m_mapS0cS0LcN0t)right\
   left(m_mapS0wS0LcN0t)right\
   left(m_mapS0cS0LcN0w)right\
   left(m_mapS0cS0RcN0t)right\
   left(m_mapS0cS0RjN0t)right\
   left(m_mapS0wS0RcN0t)right\
   left(m_mapS0cS0RcN0w)right\
   left(m_mapS0cS0UcN0t)right\
   left(m_mapS0wS0UcN0t)right\
   left(m_mapS0cS0UcN0w)right\
\
   left(m_mapS0cS0LcS1c)right\
   left(m_mapS0cS0LjS1j)right\
   left(m_mapS0wS0LcS1c)right\
   left(m_mapS0cS0LcS1w)right\
   left(m_mapS0cS0RcS1c)right\
   left(m_mapS0wS0RcS1c)right\
   left(m_mapS0cS0RcS1w)right\
   left(m_mapS0cS0UcS1c)right\
   left(m_mapS0wS0UcS1c)right\
   left(m_mapS0cS0UcS1w)right\
\
   left(m_mapN0tN1tN2t)right\
   left(m_mapS0cS1cS1Lc)right\
   left(m_mapS0wS1cS1Lc)right\
   left(m_mapS0cS1wS1Lc)right\
   left(m_mapS0cS1cS1Rc)right\
   left(m_mapS0jS1cS1Rj)right\
   left(m_mapS0wS1cS1Rc)right\
   left(m_mapS0cS1wS1Rc)right\
   left(m_mapS0cS1cS1Uc)right\
   left(m_mapS0wS1cS1Uc)right\
   left(m_mapS0cS1wS1Uc)right\
\
   left(m_mapA1)right\
   left(m_mapA1A2)right\
\
	left(m_mapWSS0w)right\
   left(m_mapWSS1w)right\
   left(m_mapWSS0wS1w)right\
   left(m_mapWSS0wTag)right\
   left(m_mapWSS1wTag)right\
   left(m_mapWSS0wS1wTag)right\
   left(m_mapWSS0wS1z)right\
   left(m_mapWSS0zS1w)right\
   left(m_mapWSS0zS1z)right\
   left(m_mapWSS0zS1zTag)right\
\
	left(m_mapCharUnigram)right \
	left(m_mapCharBigram)right \
	left(m_mapCharTrigram)right \
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
	left(m_mapTaggedSeparateChars)right \
	left(m_mapTaggedConsecutiveChars)right\
	left(m_mapWordTagTag)right\
	left(m_mapTagWordTag)right\
	left(m_mapFirstCharBy2Tags)right\
	left(m_mapFirstCharBy3Tags)right\
	left(m_mapFirstCharAndChar)right\
	left(m_mapSepCharAndNextChar)right\
	left(m_mapAppCharAndNextChar)right\
	left(m_mapPartialWord)right\
	left(m_mapPartialLengthByFirstChar)right \
	left(m_mapLengthByTagAndFirstChar)right \
	left(m_mapLengthByTagAndLastChar)right \
	left(m_mapTag0Tag1Size1)right\
	left(m_mapTag1Tag2Size1)right\
	left(m_mapTag0Tag1Tag2Size1)right





#define iterate_double_templates(left, middle, right)\
   left m_mapS0c middle m_mapS0c right\
   left m_mapS0w middle m_mapS0w right\
   left m_mapS0tc middle m_mapS0tc right\
   left m_mapS0wc middle m_mapS0wc right\
\
   left m_mapS1c middle m_mapS1c right\
   left m_mapS1w middle m_mapS1w right\
   left m_mapS1tc middle m_mapS1tc right\
   left m_mapS1wc middle m_mapS1wc right\
\
   left m_mapS2tc middle m_mapS2tc right\
   left m_mapS2wc middle m_mapS2wc right\
\
   left m_mapS3tc middle m_mapS3tc right\
   left m_mapS3wc middle m_mapS3wc right\
\
   left m_mapN0w middle m_mapN0w right\
   left m_mapN0t middle m_mapN0t right\
   left m_mapN0wt middle m_mapN0wt right\
\
   left m_mapN1w middle m_mapN1w right\
   left m_mapN1t middle m_mapN1t right\
   left m_mapN1wt middle m_mapN1wt right\
   left m_mapN01w middle m_mapN01w right\
\
   left m_mapN2w middle m_mapN2w right\
   left m_mapN2t middle m_mapN2t right\
   left m_mapN2wt middle m_mapN2wt right\
   left m_mapN12w middle m_mapN12w right\
   left m_mapN012w middle m_mapN012w right\
\
   left m_mapN3w middle m_mapN3w right\
   left m_mapN3t middle m_mapN3t right\
   left m_mapN3wt middle m_mapN3wt right\
   left m_mapN23w middle m_mapN23w right\
   left m_mapN123w middle m_mapN123w right\
\
   left m_mapS0Lwc middle m_mapS0Lwc right\
   left m_mapS0Ltc middle m_mapS0Ltc right\
   left m_mapS0Rwc middle m_mapS0Rwc right\
   left m_mapS0Rtc middle m_mapS0Rtc right\
   left m_mapS0Uwc middle m_mapS0Uwc right\
   left m_mapS0Utc middle m_mapS0Utc right\
   left m_mapS1Lwc middle m_mapS1Lwc right\
   left m_mapS1Ltc middle m_mapS1Ltc right\
   left m_mapS1Rwc middle m_mapS1Rwc right\
   left m_mapS1Rtc middle m_mapS1Rtc right\
   left m_mapS1Uwc middle m_mapS1Uwc right\
   left m_mapS1Utc middle m_mapS1Utc right\
\
   left m_mapS0wcS1wc middle m_mapS0wcS1wc right\
   left m_mapS0wS1c middle m_mapS0wS1c right\
   left m_mapS0cS1w middle m_mapS0cS1w right\
   left m_mapS0cS1c middle m_mapS0cS1c right\
   left m_mapBetweenTags middle m_mapBetweenTags right\
\
   left m_mapS0wN0w middle m_mapS0wN0w right\
   left m_mapS0cN0w middle m_mapS0cN0w right\
   left m_mapS0wN0t middle m_mapS0wN0t right\
   left m_mapS0cN0t middle m_mapS0cN0t right\
   left m_mapS0cmN0tm middle m_mapS0cmN0tm right\
\
   left m_mapS1wN0w middle m_mapS1wN0w right\
   left m_mapS1cN0w middle m_mapS1cN0w right\
   left m_mapS1wN0t middle m_mapS1wN0t right\
   left m_mapS1cN0t middle m_mapS1cN0t right\
\
   left m_mapN0wN1w middle m_mapN0wN1w right\
   left m_mapN0tN1w middle m_mapN0tN1w right\
   left m_mapN0wN1t middle m_mapN0wN1t right\
   left m_mapN0tN1t middle m_mapN0tN1t right\
\
   left m_mapS0wS1cN0t middle m_mapS0wS1cN0t right\
   left m_mapS0cS1wN0t middle m_mapS0cS1wN0t right\
   left m_mapS0cS1cN0w middle m_mapS0cS1cN0w right\
   left m_mapS0cS1cN0t middle m_mapS0cS1cN0t right\
   left m_mapS0tS1tN0t middle m_mapS0tS1tN0t right\
   left m_mapS0jS1jN0t middle m_mapS0jS1jN0t right\
\
	left m_mapS0cN1w middle m_mapS0cN1w right\
   left m_mapS0wN0tN1t middle m_mapS0wN0tN1t right\
   left m_mapS0cN0wN1t middle m_mapS0cN0wN1t right\
   left m_mapS0cN0tN1w middle m_mapS0cN0tN1w right\
   left m_mapS0cN0tN1t middle m_mapS0cN0tN1t right\
   left m_mapS0tN0tN1t middle m_mapS0tN0tN1t right\
   left m_mapS0jN0tN1t middle m_mapS0jN0tN1t right\
\
   left m_mapS0wS1cS2c middle m_mapS0wS1cS2c right\
   left m_mapS0cS1wS2c middle m_mapS0cS1wS2c right\
   left m_mapS0cS1cS2w middle m_mapS0cS1cS2w right\
   left m_mapS0cS1cS2c middle m_mapS0cS1cS2c right\
   left m_mapS0tS1tS2t middle m_mapS0tS1tS2t right\
\
   left m_mapS1cS2c middle m_mapS1cS2c right\
   left m_mapS2cS3c middle m_mapS2cS3c right\
   left m_mapN1tN2t middle m_mapN1tN2t right\
   left m_mapN2tN3t middle m_mapN2tN3t right\
\
   left m_mapS0cS2c middle m_mapS0cS2c right\
   left m_mapS1cS3c middle m_mapS1cS3c right\
   left m_mapS0cN1t middle m_mapS0cN1t right\
   left m_mapN0tN2t middle m_mapN0tN2t right\
   left m_mapN1tN3t middle m_mapN1tN3t right\
\
   left m_mapS0cS0LcN0t middle m_mapS0cS0LcN0t right\
   left m_mapS0wS0LcN0t middle m_mapS0wS0LcN0t right\
   left m_mapS0cS0LcN0w middle m_mapS0cS0LcN0w right\
   left m_mapS0cS0RcN0t middle m_mapS0cS0RcN0t right\
   left m_mapS0cS0RjN0t middle m_mapS0cS0RjN0t right\
   left m_mapS0wS0RcN0t middle m_mapS0wS0RcN0t right\
   left m_mapS0cS0RcN0w middle m_mapS0cS0RcN0w right\
   left m_mapS0cS0UcN0t middle m_mapS0cS0UcN0t right\
   left m_mapS0wS0UcN0t middle m_mapS0wS0UcN0t right\
   left m_mapS0cS0UcN0w middle m_mapS0cS0UcN0w right\
\
   left m_mapS0cS0LcS1c middle m_mapS0cS0LcS1c right\
   left m_mapS0cS0LjS1j middle m_mapS0cS0LjS1j right\
   left m_mapS0wS0LcS1c middle m_mapS0wS0LcS1c right\
   left m_mapS0cS0LcS1w middle m_mapS0cS0LcS1w right\
   left m_mapS0cS0RcS1c middle m_mapS0cS0RcS1c right\
   left m_mapS0wS0RcS1c middle m_mapS0wS0RcS1c right\
   left m_mapS0cS0RcS1w middle m_mapS0cS0RcS1w right\
   left m_mapS0cS0UcS1c middle m_mapS0cS0UcS1c right\
   left m_mapS0wS0UcS1c middle m_mapS0wS0UcS1c right\
   left m_mapS0cS0UcS1w middle m_mapS0cS0UcS1w right\
\
   left m_mapN0tN1tN2t middle m_mapN0tN1tN2t right\
   left m_mapS0cS1cS1Lc middle m_mapS0cS1cS1Lc right\
   left m_mapS0wS1cS1Lc middle m_mapS0wS1cS1Lc right\
   left m_mapS0cS1wS1Lc middle m_mapS0cS1wS1Lc right\
   left m_mapS0cS1cS1Rc middle m_mapS0cS1cS1Rc right\
   left m_mapS0jS1cS1Rj middle m_mapS0jS1cS1Rj right\
   left m_mapS0wS1cS1Rc middle m_mapS0wS1cS1Rc right\
   left m_mapS0cS1wS1Rc middle m_mapS0cS1wS1Rc right\
   left m_mapS0cS1cS1Uc middle m_mapS0cS1cS1Uc right\
   left m_mapS0wS1cS1Uc middle m_mapS0wS1cS1Uc right\
   left m_mapS0cS1wS1Uc middle m_mapS0cS1wS1Uc right\
\
   left m_mapA1 middle m_mapA1 right\
   left m_mapA1A2 middle m_mapA1A2 right\
\
	left m_mapWSS0w middle m_mapWSS0w right\
   left m_mapWSS1w middle m_mapWSS1w right\
   left m_mapWSS0wS1w middle m_mapWSS0wS1w right\
   left m_mapWSS0wTag middle m_mapWSS0wTag right\
   left m_mapWSS1wTag middle m_mapWSS1wTag right\
   left m_mapWSS0wS1wTag middle m_mapWSS0wS1wTag right\
   left m_mapWSS0wS1z middle m_mapWSS0wS1z right\
   left m_mapWSS0zS1w middle m_mapWSS0zS1w right\
   left m_mapWSS0zS1z middle m_mapWSS0zS1z right\
   left m_mapWSS0zS1zTag middle m_mapWSS0zS1zTag right\
\
	left m_mapCharUnigram middle m_mapCharUnigram right \
	left m_mapCharBigram middle m_mapCharBigram right \
	left m_mapCharTrigram middle m_mapCharTrigram right \
	left m_mapSeenWords middle m_mapSeenWords right \
	left m_mapLastWordByWord middle m_mapLastWordByWord right \
	left m_mapCurrentWordLastChar middle m_mapCurrentWordLastChar right \
	left m_mapLastWordFirstChar middle m_mapLastWordFirstChar right \
	left m_mapFirstCharLastWordByWord middle m_mapFirstCharLastWordByWord right \
	left m_mapLastWordByLastChar middle m_mapLastWordByLastChar right \
	left m_mapSeparateChars middle m_mapSeparateChars right \
	left m_mapConsecutiveChars middle m_mapConsecutiveChars right \
	left m_mapFirstAndLastChars middle m_mapFirstAndLastChars right \
	left m_mapOneCharWord middle m_mapOneCharWord right \
	left m_mapLengthByFirstChar middle m_mapLengthByFirstChar right \
	left m_mapLengthByLastChar middle m_mapLengthByLastChar right \
	left m_mapLengthByLastWord middle m_mapLengthByLastWord right \
	left m_mapLastLengthByWord middle m_mapLastLengthByWord right \
	left m_mapLastTagByTag middle m_mapLastTagByTag right \
	left m_mapLastTwoTagsByTag middle m_mapLastTwoTagsByTag right \
	left m_mapCurrentTag middle m_mapCurrentTag right \
	left m_mapTagByLastWord middle m_mapTagByLastWord right \
	left m_mapLastTagByWord middle m_mapLastTagByWord right \
	left m_mapTagByFirstChar middle m_mapTagByFirstChar right \
	left m_mapTagByLastChar middle m_mapTagByLastChar right \
	left m_mapTagByChar middle m_mapTagByChar right \
	left m_mapTagOfOneCharWord middle m_mapTagOfOneCharWord right \
	left m_mapRepeatedCharByTag middle m_mapRepeatedCharByTag right \
	left m_mapTagByWordAndPrevChar middle m_mapTagByWordAndPrevChar right \
	left m_mapTagByWordAndNextChar middle m_mapTagByWordAndNextChar right \
	left m_mapTaggedCharByFirstChar middle m_mapTaggedCharByFirstChar right \
	left m_mapTaggedCharByLastChar middle m_mapTaggedCharByLastChar right \
	left m_mapTagByFirstCharCat middle m_mapTagByFirstCharCat right \
	left m_mapTagByLastCharCat middle m_mapTagByLastCharCat right \
	left m_mapTaggedSeparateChars middle m_mapTaggedSeparateChars right \
	left m_mapTaggedConsecutiveChars middle m_mapTaggedConsecutiveChars right \
	left m_mapWordTagTag middle m_mapWordTagTag right \
	left m_mapTagWordTag middle m_mapTagWordTag right \
	left m_mapFirstCharBy2Tags middle m_mapFirstCharBy2Tags right \
	left m_mapFirstCharBy3Tags middle m_mapFirstCharBy3Tags right \
	left m_mapFirstCharAndChar middle m_mapFirstCharAndChar right \
	left m_mapSepCharAndNextChar middle m_mapSepCharAndNextChar right \
	left m_mapAppCharAndNextChar middle m_mapAppCharAndNextChar right \
	left m_mapPartialWord middle m_mapPartialWord right \
	left m_mapPartialLengthByFirstChar middle m_mapPartialLengthByFirstChar right \
	left m_mapLengthByTagAndFirstChar middle m_mapLengthByTagAndFirstChar right \
	left m_mapLengthByTagAndLastChar middle m_mapLengthByTagAndLastChar right \
	left m_mapTag0Tag1Size1 middle m_mapTag0Tag1Size1 right \
	left m_mapTag1Tag2Size1 middle m_mapTag1Tag2Size1 right \
	left m_mapTag0Tag1Tag2Size1 middle m_mapTag0Tag1Tag2Size1 right

namespace TARGET_LANGUAGE {

namespace conparser {

//#include "conparser_macros.h"

//
// TYPE DEFINITIONS
//
typedef CScoreMapType<CWord, SCORE_TYPE, CAction::MAX> CWordMap;
typedef CScoreMapType<CTaggedWord<CTag, TAG_SEPARATOR>, SCORE_TYPE, CAction::MAX> CTaggedWordMap;
typedef CScoreMapType<std::pair<CWord, unsigned long>,  SCORE_TYPE, CAction::MAX> CWordIntMap;
typedef CScoreMapType<unsigned long, SCORE_TYPE, CAction::MAX> CIntMap;
typedef CScoreMapType<std::pair<unsigned long, unsigned long>, SCORE_TYPE, CAction::MAX> CTwoIntMap;
typedef CScoreMapType<CTwoWords, SCORE_TYPE, CAction::MAX> CTwoWordsMap;
typedef CScoreMapType<std::pair<CTwoWords, CActionType>, SCORE_TYPE, CAction::MAX> CTwoWordsActionTypeMap;
typedef CScoreMapType<CTuple3<CTwoWords, CTag, CActionType>, SCORE_TYPE, CAction::MAX> CTwoWordsTagActionTypeMap;
typedef CScoreMapType<CTuple2<CTwoWords, CCFGSet>, SCORE_TYPE, CAction::MAX> CTwoWordsCFGSetMap;
typedef CScoreMapType<CTuple2<CTwoWords, CConstituent>, SCORE_TYPE, CAction::MAX> CTwoWordsConstituentMap;
typedef CScoreMapType<CTwoTaggedWords, SCORE_TYPE, CAction::MAX> CTwoTaggedWordsMap;
typedef CScoreMapType<std::pair<CTwoTaggedWords, CActionType>, SCORE_TYPE, CAction::MAX> CTwoTaggedWordsActionTypeMap;
typedef CScoreMapType<CCFGSet, SCORE_TYPE, CAction::MAX> CCFGSetMap;
typedef CScoreMapType<std::pair<CCFGSet, unsigned long>, SCORE_TYPE, CAction::MAX> CCFGSetIntMap;
typedef CScoreMapType<CTuple2<CWord, CCFGSet>, SCORE_TYPE, CAction::MAX> CWordCFGSetMap;
typedef CScoreMapType<CTuple3<CWord, CCFGSet, CActionType>, SCORE_TYPE, CAction::MAX> CWordCFGSetActionTypeMap;
typedef CScoreMapType<std::pair<CWord, CCFGSet>, SCORE_TYPE, CAction::MAX> CWordCFGRuleMap;
typedef CScoreMapType<std::pair<CTwoWords, CCFGSet>, SCORE_TYPE, CAction::MAX> CTwoWordsCFGRuleMap;
typedef CScoreMapType<std::pair<CCFGSet, unsigned long>, SCORE_TYPE, CAction::MAX> CCFGSetIntMap;
typedef CScoreMapType<std::pair<CCFGSet, CActionType>, SCORE_TYPE, CAction::MAX> CCFGSetActionTypeMap;
typedef CScoreMapType<CTuple2<CWord, CConstituent>, SCORE_TYPE, CAction::MAX> CWordConstituentMap;
typedef CScoreMapType<CTuple2<CTag, CConstituent>, SCORE_TYPE, CAction::MAX> CTagConstituentMap;
typedef CScoreMapType<CTuple3<CTag, CTag, unsigned long>, SCORE_TYPE, CAction::MAX> CWordTagIntMap;
typedef CScoreMapType<CConstituent, SCORE_TYPE, CAction::MAX> CConstituentMap;
typedef CScoreMapType<CAction, SCORE_TYPE, CAction::MAX> CActionMap;
typedef CScoreMapType<CTuple2<CAction, CAction>, SCORE_TYPE, CAction::MAX> CAction2Map;

typedef CScoreMapType<CTuple2<CWord, CWord>, SCORE_TYPE, CAction::MAX> CBiWordMap;
typedef CScoreMapType<CTuple3<CWord, CWord, CWord>, SCORE_TYPE, CAction::MAX> CTriWordMap;
typedef CScoreMapType<CTuple3<CWord, CWord, CTag>, SCORE_TYPE, CAction::MAX> CWordWordTagMap;
typedef CScoreMapType<CTuple2<CWord, CTag>, SCORE_TYPE, CAction::MAX> CWordTagMap;
typedef CScoreMapType<CTuple2<CTag, CTag>, SCORE_TYPE, CAction::MAX> CBiTagMap;
typedef CScoreMapType<CTuple3<CTag, CTag, CTag>, SCORE_TYPE, CAction::MAX> CTriTagMap;
typedef CScoreMapType<CTuple3<CWord, CTag, CTag>, SCORE_TYPE, CAction::MAX> CWordTagTagMap;
typedef CScoreMapType<CTuple3<CWord, CWord, CConstituent>, SCORE_TYPE, CAction::MAX> CWordWordConstituentMap;

typedef CHashMap<CWord, unsigned long> CWordToIntMap;

typedef CScoreMapType<CTuple3<CTag, CTag, int>, SCORE_TYPE, CAction::MAX> CBiTagIntMap;
typedef CScoreMapType<CTuple4<CTag, CTag, CTag, int>, SCORE_TYPE, CAction::MAX> CTriTagIntMap;
typedef CScoreMapType<CTuple4<CWord, CTag, CTag, CTag>, SCORE_TYPE, CAction::MAX> CWordTriTagMap;
typedef CScoreMapType< CTuple2<CTag, unsigned long long>, SCORE_TYPE , CAction::MAX> CTagIntMap;



//typedef CScoreMap< long int, SCORE_TYPE > CIntMap;

typedef CScoreMapType< std::pair<CWord, CTag>, SCORE_TYPE , CAction::MAX > CWordTagPairMap;
typedef CScoreMapType< std::pair<unsigned long long, CTag>, SCORE_TYPE , CAction::MAX> CIntTagMap;
typedef CScoreMapType< CTagSet<CTag, 2>, SCORE_TYPE , CAction::MAX> CTagSet2Map;
typedef CScoreMapType< CTagSet<CTag, 3>, SCORE_TYPE , CAction::MAX> CTagSet3Map;
typedef CScoreMapType< std::pair<CWord, CTagSet<CTag, 2> >, SCORE_TYPE , CAction::MAX> CWordTagSet2Map;
typedef CScoreMapType< std::pair<CWord, CTagSet<CTag, 3> >, SCORE_TYPE , CAction::MAX> CWordTagSet3Map;
typedef CScoreMapType< std::pair<CTagSet<CTag, 2>, int>, SCORE_TYPE , CAction::MAX> CTagSet2IntMap;
typedef CScoreMapType< std::pair<CTagSet<CTag, 3>, int>, SCORE_TYPE , CAction::MAX> CTagSet3IntMap;
//typedef CScoreMapType< std::pair<CWord, CTagSet<CTag, 3> >, SCORE_TYPE , CAction::MAX> CWordTagSet3Map;

/*===============================================================
 *
 * CWeight - the definition of weights, in feature
 *
 *==============================================================*/

class CWeight : public CWeightBase {

public:
   // S0
   CConstituentMap m_mapS0c;
   CTaggedWordMap m_mapS0w;
   CTagConstituentMap m_mapS0tc;
   CWordConstituentMap m_mapS0wc;

   // S1
   CConstituentMap m_mapS1c;
   CTaggedWordMap m_mapS1w;
   CTagConstituentMap m_mapS1tc;
   CWordConstituentMap m_mapS1wc;

   // S2
   CTagConstituentMap m_mapS2tc;
   CWordConstituentMap m_mapS2wc;

   // S3
   CTagConstituentMap m_mapS3tc;
   CWordConstituentMap m_mapS3wc;

   // N0
   CWordMap m_mapN0w;
   CIntMap m_mapN0t;  //not use
   CTaggedWordMap m_mapN0wt;  //not use

   // N1
   CWordMap m_mapN1w;
   CIntMap m_mapN1t;  //not use
   CTaggedWordMap m_mapN1wt;  //not use
   CBiWordMap m_mapN01w; 	//added

   // N2
   CWordMap m_mapN2w;
   CIntMap m_mapN2t;   //not use
   CTaggedWordMap m_mapN2wt;  //not use
   CBiWordMap m_mapN12w; 	//added
   CTriWordMap m_mapN012w; 	//added

   // N3
   CWordMap m_mapN3w;
   CIntMap m_mapN3t;  //not use
   CTaggedWordMap m_mapN3wt;  //not use
   CBiWordMap m_mapN23w; 	//added
   CTriWordMap m_mapN123w; 	//added

   // S0L
   CWordConstituentMap m_mapS0Lwc;
   CTagConstituentMap m_mapS0Ltc;

   // S0R
   CWordConstituentMap m_mapS0Rwc;
   CTagConstituentMap m_mapS0Rtc;

   // S0U
   CWordConstituentMap m_mapS0Uwc;
   CTagConstituentMap m_mapS0Utc;

   // S1L
   CWordConstituentMap m_mapS1Lwc;
   CTagConstituentMap m_mapS1Ltc;

   // S1R
   CWordConstituentMap m_mapS1Rwc;
   CTagConstituentMap m_mapS1Rtc;

   // S1U
   CWordConstituentMap m_mapS1Uwc;
   CTagConstituentMap m_mapS1Utc;

   // S0 S1
   CTwoWordsCFGSetMap m_mapS0wcS1wc; // the original 
   CWordCFGSetMap m_mapS0wS1c;
   CWordCFGSetMap m_mapS0cS1w;
   CCFGSetMap m_mapS0cS1c;

   CCFGSetMap m_mapBetweenTags;

   // S0 N0
   CWordWordConstituentMap m_mapS0wN0w;
   CWordConstituentMap m_mapS0cN0w;
   CWordConstituentMap m_mapS0wN0t;
   CCFGSetMap m_mapS0cN0t;  //not use
   CTwoIntMap m_mapS0cmN0tm;  //not use

   // S1 N0
   CWordWordConstituentMap m_mapS1wN0w;
   CWordConstituentMap m_mapS1cN0w;
   CWordConstituentMap m_mapS1wN0t; //not use
   CCFGSetMap m_mapS1cN0t;  //not use

   // N0 N1
   CTwoWordsConstituentMap m_mapN0wN1w;  //not use
   CWordConstituentMap m_mapN0tN1w;  //not use
   CWordConstituentMap m_mapN0wN1t;  //not use
   CCFGSetMap m_mapN0tN1t;  //not use

   // S0 S1N0
   CWordConstituentMap m_mapS0wS1cN0t;  //not use
   CWordConstituentMap m_mapS0cS1wN0t;  //not use
   CWordCFGSetMap m_mapS0cS1cN0w;
   CCFGSetMap m_mapS0cS1cN0t;  //not use
   CCFGSetMap m_mapS0tS1tN0t;  //not use
   CCFGSetMap m_mapS0jS1jN0t;  //not use

   // S0 N0N1
   CWordConstituentMap m_mapS0cN1w; 	//added
   CWordCFGSetMap m_mapS0wN0tN1t;  //not use
   CWordCFGSetMap m_mapS0cN0wN1t;  //not use
   CWordWordConstituentMap m_mapS0cN0tN1w;
   CCFGSetMap m_mapS0cN0tN1t;  //not use
   CCFGSetMap m_mapS0tN0tN1t;  //not use
   CCFGSetMap m_mapS0jN0tN1t;  //not use

   // S0 S1S2
   CWordCFGSetMap m_mapS0wS1cS2c;
   CWordCFGSetMap m_mapS0cS1wS2c;
   CWordCFGSetMap m_mapS0cS1cS2w;
   CCFGSetMap m_mapS0cS1cS2c;
   CCFGSetMap m_mapS0tS1tS2t;

   // tag bigram
   CIntMap m_mapS1cS2c;   //not use
   CIntMap m_mapS2cS3c;   //not use
   CIntMap m_mapN1tN2t;   //not use
   CIntMap m_mapN2tN3t;   //not use
   
   CIntMap m_mapS0cS2c;   //not use
   CIntMap m_mapS1cS3c;   //not use
   CIntMap m_mapS0cN1t;   //not use
   CIntMap m_mapN0tN2t;   //not use
   CIntMap m_mapN1tN3t;   //not use

   CIntMap m_mapN0tN1tN2t;   //not use

   // S0 S0LRUN0
   CCFGSetMap m_mapS0cS0RcN0t;   //not use
   CCFGSetMap m_mapS0cS0RjN0t;  //not use
   CWordCFGSetMap m_mapS0cS0RcN0w;

   CCFGSetMap m_mapS0cS0LcN0t;   //not use
   CWordCFGSetMap m_mapS0wS0LcN0t;   //not use
   CWordCFGSetMap m_mapS0cS0LcN0w;   //not use
   CWordCFGSetMap m_mapS0wS0RcN0t;   //not use
   CCFGSetMap m_mapS0cS0UcN0t;   //not use
   CWordCFGSetMap m_mapS0wS0UcN0t;   //not use
   CWordCFGSetMap m_mapS0cS0UcN0w;   //not use

   // S0 S0LRUS1
   CCFGSetMap m_mapS0cS0LcS1c;
   CCFGSetMap m_mapS0cS0LjS1j;
   CWordCFGSetMap m_mapS0cS0LcS1w;

   CWordCFGSetMap m_mapS0wS0LcS1c;   //not use
   CCFGSetMap m_mapS0cS0RcS1c;   //not use
   CWordCFGSetMap m_mapS0wS0RcS1c;   //not use
   CWordCFGSetMap m_mapS0cS0RcS1w;   //not use
   CCFGSetMap m_mapS0cS0UcS1c;   //not use
   CWordCFGSetMap m_mapS0wS0UcS1c;   //not use
   CWordCFGSetMap m_mapS0cS0UcS1w;   //not use

   // S0 S1LRUS1
   CCFGSetMap m_mapS0cS1cS1Rc;
   CCFGSetMap m_mapS0jS1cS1Rj;
   CWordCFGSetMap m_mapS0wS1cS1Rc;

   CCFGSetMap m_mapS0cS1cS1Lc;   //not use
   CWordCFGSetMap m_mapS0wS1cS1Lc;   //not use
   CWordCFGSetMap m_mapS0cS1wS1Lc;   //not use
   CWordCFGSetMap m_mapS0cS1wS1Rc;   //not use
   CCFGSetMap m_mapS0cS1cS1Uc;   //not use
   CWordCFGSetMap m_mapS0wS1cS1Uc;   //not use
   CWordCFGSetMap m_mapS0cS1wS1Uc;   //not use

   // past action
   CActionMap m_mapA1;   //not use
   CAction2Map m_mapA1A2;   //not use

   //word structure feature
   CWordMap m_mapWSS0w; 	//added
   CWordMap m_mapWSS1w; 	//added
   CBiWordMap m_mapWSS0wS1w; 	//added
   CWordTagMap m_mapWSS0wTag; 	//added
   CWordTagMap m_mapWSS1wTag; 	//added
   CWordWordTagMap m_mapWSS0wS1wTag;   	//added

   CBiWordMap m_mapWSS0wS1z; 	//added
   CBiWordMap m_mapWSS0zS1w; 	//added
   CBiWordMap m_mapWSS0zS1z; 	//added
   CWordWordTagMap m_mapWSS0zS1zTag;   	//added

   //segmentation tagging features

   CWordIntMap m_mapCharUnigram;
   CWordIntMap m_mapCharBigram;
   CWordIntMap m_mapCharTrigram;

   // feature templates abstd::cout words
   CWordMap m_mapSeenWords;
   CTwoWordsMap m_mapLastWordByWord;
   CWordMap m_mapCurrentWordLastChar;
   CWordMap m_mapLastWordFirstChar;
   CTwoWordsMap m_mapFirstCharLastWordByWord;
   CTwoWordsMap m_mapLastWordByLastChar;
   CWordMap m_mapSeparateChars;
   CWordMap m_mapConsecutiveChars;
   CTwoWordsMap m_mapFirstAndLastChars;
   CWordMap m_mapOneCharWord;
   CWordIntMap m_mapLengthByFirstChar;
   CWordIntMap m_mapLengthByLastChar;
   CWordIntMap m_mapLengthByLastWord;
   CWordIntMap m_mapLastLengthByWord;

   // feature templates tag
   CWordTagPairMap m_mapCurrentTag;
   CBiTagMap m_mapLastTagByTag;
   CTriTagMap m_mapLastTwoTagsByTag;
   CWordTagMap m_mapTagByLastWord;
   CWordTagPairMap m_mapLastTagByWord;
   CWordTagMap m_mapTagByFirstChar;
   CWordTagPairMap m_mapTagByLastChar;
   CWordTagMap m_mapTagByChar;
   CWordTagPairMap m_mapTagOfOneCharWord;
   CWordTagPairMap m_mapRepeatedCharByTag;
   CWordTagPairMap m_mapTagByWordAndPrevChar;
   CWordTagPairMap m_mapTagByWordAndNextChar;
   CTwoTaggedWordsMap m_mapTaggedCharByFirstChar;
   CTwoTaggedWordsMap m_mapTaggedCharByLastChar;

   // extra features
   CTwoTaggedWordsMap m_mapTaggedSeparateChars;
   CWordTagPairMap m_mapTaggedConsecutiveChars;

   CWordTagTagMap m_mapWordTagTag;
   CWordTagTagMap m_mapTagWordTag;
   CWordTagTagMap m_mapFirstCharBy2Tags;
   CWordTriTagMap m_mapFirstCharBy3Tags;
   CTwoWordsMap m_mapFirstCharAndChar;

   CWordMap m_mapSepCharAndNextChar;
   CWordMap m_mapAppCharAndNextChar;

   CWordMap m_mapPartialWord;
   CWordIntMap m_mapPartialLengthByFirstChar;
   CWordIntMap m_mapLengthByTagAndFirstChar;
   CWordIntMap m_mapLengthByTagAndLastChar;

   CBiTagIntMap m_mapTag0Tag1Size1;
   CTagSet2IntMap m_mapTag1Tag2Size1;
   CTriTagIntMap m_mapTag0Tag1Tag2Size1;

   // feature templates knowledge
   CTagIntMap m_mapTagByFirstCharCat;
   CIntTagMap m_mapTagByLastCharCat;

   CIntTagMap m_mapSeparateCharCat;
   CIntTagMap m_mapConsecutiveCharCat;




   //character features in stack


   //combined features

   // Rule dictionary
   unsigned long m_nMaxWordFrequency;
   CWordToIntMap m_mapWordFrequency;

   CTagDict<CWord, CTag> m_mapTagDictionary;
   CTagDict<CWord, CTag> m_mapCharTagDictionary;
   CTagDict<CWord, CTag> m_mapCanStart;
   unsigned long m_maxlengthByTag[CTag::COUNT+1];
   CCharCatDictionary *m_Knowledge;

public:

   unsigned long getMaxWordLength() const {return m_maxlengthByTag[CTag::COUNT];}
	void setMaxLengthByTag(unsigned long tag, unsigned long length) {
		if (length<=m_maxlengthByTag[tag])
			return;
		m_maxlengthByTag[tag]=length;
		if (length>m_maxlengthByTag[CTag::COUNT])
			m_maxlengthByTag[CTag::COUNT]=length;
	}


   CWeight(bool bTrain, unsigned TABLE_SIZE = DEFAULT_TABLE_SIZE) : 
                          CWeightBase(bTrain) ,

                          m_nMaxWordFrequency(0) ,
                          m_Knowledge(new CCharCatDictionary()) ,
                          m_mapWordFrequency(65537),
                          m_mapTagDictionary(CTag::COUNT),
                          m_mapCharTagDictionary(CTag::COUNT),
                          m_mapCanStart(CTag::COUNT),

                          m_mapS0c("Stack0Constituent", TABLE_SIZE),
                          m_mapS0w("Stack0Word", TABLE_SIZE),
                          m_mapS0tc("Stack0Tag", TABLE_SIZE),
                          m_mapS0wc("Stack0WordConstituent", TABLE_SIZE),

                          m_mapS1c("Stack1Constituent", TABLE_SIZE),
                          m_mapS1w("Stack1Word", TABLE_SIZE),
                          m_mapS1tc("Stack1Tag", TABLE_SIZE),
                          m_mapS1wc("Stack1WordConstituent", TABLE_SIZE),

                          m_mapS2tc("Stack2Tag", TABLE_SIZE),
                          m_mapS2wc("Stack2WordConstituent", TABLE_SIZE),

                          m_mapS3tc("Stack3Tag", TABLE_SIZE),
                          m_mapS3wc("Stack3WordConstituent", TABLE_SIZE),

                          m_mapN0w("Next0Word", TABLE_SIZE),
                          m_mapN0t("Next0Tag", TABLE_SIZE),
                          m_mapN0wt("Next0WordTag", TABLE_SIZE),
                                            
                          m_mapN1w("Next1Word", TABLE_SIZE),
                          m_mapN1t("Next1Tag", TABLE_SIZE),
                          m_mapN1wt("Next1WordTag", TABLE_SIZE),
                          m_mapN01w("Next01Word", TABLE_SIZE),

                          m_mapN2w("Next2Word", TABLE_SIZE),
                          m_mapN2t("Next2Tag", TABLE_SIZE),
                          m_mapN2wt("Next2WordTag", TABLE_SIZE),
                          m_mapN12w("Next12Word", TABLE_SIZE),
                          m_mapN012w("Next012Word", TABLE_SIZE),

                          m_mapN3w("Next3Word", TABLE_SIZE),
                          m_mapN3t("Next3Tag", TABLE_SIZE),
                          m_mapN3wt("Next3WordTag", TABLE_SIZE),
                          m_mapN23w("Next23Word", TABLE_SIZE),
                          m_mapN123w("Next123Word", TABLE_SIZE),

                          m_mapS0Lwc("Stack0LeftWord", TABLE_SIZE),
                          m_mapS0Ltc("Stack0LeftConstituent", TABLE_SIZE), 

                          m_mapS0Rwc("Stack0RightWord", TABLE_SIZE),
                          m_mapS0Rtc("Stack0RightConstituent", TABLE_SIZE), 

                          m_mapS0Uwc("Stack0UniWord", TABLE_SIZE),
                          m_mapS0Utc("Stack0UniConstituent", TABLE_SIZE), 

                          m_mapS1Lwc("Stack1LeftWord", TABLE_SIZE),
                          m_mapS1Ltc("Stack1LeftConstituent", TABLE_SIZE), 

                          m_mapS1Rwc("Stack1RightWord", TABLE_SIZE),
                          m_mapS1Rtc("Stack1RightConstituent", TABLE_SIZE), 

                          m_mapS1Uwc("Stack1UniWord", TABLE_SIZE),
                          m_mapS1Utc("Stack1UniConstituent", TABLE_SIZE), 

                          m_mapS0wcS1wc("Stack0WordStack1Word", TABLE_SIZE),
                          m_mapS0wS1c("Stack0WordStack1Constituent", TABLE_SIZE),
                          m_mapS0cS1w("Stack0ConstituentStack1Word", TABLE_SIZE),
                          m_mapS0cS1c("Stack0ConstituentStack1Constituent", TABLE_SIZE),
                          m_mapBetweenTags("BetweenTags", TABLE_SIZE),
                                            
                          m_mapS0wN0w("Stack0WordNext0Word", TABLE_SIZE),
                          m_mapS0wN0t("Stack0WordNext0Tag", TABLE_SIZE),
                          m_mapS0cN0w("Stack0ConstituentNext0Word", TABLE_SIZE),
                          m_mapS0cN0t("Stack0ConstituentNext0Tag", TABLE_SIZE),
                          m_mapS0cmN0tm("Stack0ConstituentRhythmNext0TagRhythm", TABLE_SIZE),

                          m_mapS1wN0w("Stack1WordNext0Word", TABLE_SIZE),
                          m_mapS1wN0t("Stack1WordNext0Tag", TABLE_SIZE),
                          m_mapS1cN0w("Stack1ConstituentNext0Word", TABLE_SIZE),
                          m_mapS1cN0t("Stack1ConstituentNext0Tag", TABLE_SIZE),

                          m_mapN0wN1w("Next0WordNext1Word", TABLE_SIZE),
                          m_mapN0tN1w("Next0TagNext1Word", TABLE_SIZE),
                          m_mapN0wN1t("Next0WordNext1Tag", TABLE_SIZE),
                          m_mapN0tN1t("Next0TagNext1Tag", TABLE_SIZE),

                          m_mapS0wS1cN0t("Stack0WordStack1ConstituentNext0Tag", TABLE_SIZE),
                          m_mapS0cS1wN0t("Stack0ConstituentStack1WordNext0Tag", TABLE_SIZE),
                          m_mapS0cS1cN0w("Stack0ConstituentStack1ConstituentNext0Word", TABLE_SIZE),
                          m_mapS0cS1cN0t("Stack0ConstituentStack1ConstituentNext0Tag", TABLE_SIZE),
                          m_mapS0tS1tN0t("Stack0TagStack1TagNext0Tag", TABLE_SIZE),
                          m_mapS0jS1jN0t("Stack0TagOrConstituentStack1TagOrConstituentNext0Tag", TABLE_SIZE),

                          m_mapS0cN1w("Stack0ConstituentNext0WordNext", TABLE_SIZE),
                          m_mapS0wN0tN1t("Stack0WordNext0TagNext", TABLE_SIZE),
                          m_mapS0cN0wN1t("Stack0ConstituentNext0WordNext1Tag", TABLE_SIZE),
                          m_mapS0cN0tN1w("Stack0ConstituentNext0TagNext1Word", TABLE_SIZE),
                          m_mapS0cN0tN1t("Stack0ConstituentNext0TagNext1Tag", TABLE_SIZE),
                          m_mapS0tN0tN1t("Stack0TagNext0TagNext1Tag", TABLE_SIZE),
                          m_mapS0jN0tN1t("Stack0TagOrConstituentNext0TagNext1Tag", TABLE_SIZE),

                          m_mapS0wS1cS2c("Stack0WordStack1ConstituentStack2Constituent", TABLE_SIZE),
                          m_mapS0cS1wS2c("Stack0ConstituentStack1WordStack2Constituent", TABLE_SIZE),
                          m_mapS0cS1cS2w("Stack0ConstituentStack1ConstituentStack2Word", TABLE_SIZE),
                          m_mapS0cS1cS2c("Stack0ConstituentStack1ConstituentStack2Constituent", TABLE_SIZE),
                          m_mapS0tS1tS2t("Stack0TagStack1TagStack2Tag", TABLE_SIZE),

                          m_mapS1cS2c("Stack1ConstituentStack2Constituent", TABLE_SIZE),
                          m_mapS2cS3c("Stack2ConstituentStack3Constituent", TABLE_SIZE),
                          m_mapN1tN2t("Next1TagNext2Tag", TABLE_SIZE),
                          m_mapN2tN3t("Next2TagNext3Tag", TABLE_SIZE),
                                               
                          m_mapS0cS2c("Stack0ConstituentStack2Constituent", TABLE_SIZE),
                          m_mapS1cS3c("Stack1ConstituentStack3Constituent", TABLE_SIZE),
                          m_mapS0cN1t("Stack0ConstituentNext1Tag", TABLE_SIZE),
                          m_mapN0tN2t("Next0TagNext2Tag", TABLE_SIZE),
                          m_mapN1tN3t("Next1TagNextTag", TABLE_SIZE),

                          m_mapS0cS0LcN0t("Stack0ConstituentStack0LConstituentNex0tTag", TABLE_SIZE),
                          m_mapS0wS0LcN0t("Stack0WordStack0LConstituentNex0tTag", TABLE_SIZE),
                          m_mapS0cS0LcN0w("Stack0ConstituentStack0LConstituentNex0tWord", TABLE_SIZE),
                          m_mapS0cS0RcN0t("Stack0ConstituentStack0RConstituentNex0tTag", TABLE_SIZE),
                          m_mapS0cS0RjN0t("Stack0ConstituentStack0RTagOrConstituentNex0tTag", TABLE_SIZE),
                          m_mapS0wS0RcN0t("Stack0WordStack0RConstituentNex0tTag", TABLE_SIZE),
                          m_mapS0cS0RcN0w("Stack0ConstituentStack0RConstituentNex0tWord", TABLE_SIZE),
                          m_mapS0cS0UcN0t("Stack0ConstituentStack0UConstituentNex0tTag", TABLE_SIZE),
                          m_mapS0wS0UcN0t("Stack0WordStack0UConstituentNex0tTag", TABLE_SIZE),
                          m_mapS0cS0UcN0w("Stack0ConstituentStack0UConstituentNex0tWord", TABLE_SIZE),

                          m_mapS0cS0LcS1c("Stack0ConstituentStack0LConstituentStack1Constituent", TABLE_SIZE),
                          m_mapS0cS0LjS1j("Stack0ConstituentStack0LTagOrConstituentStack1TagOrConstituent", TABLE_SIZE),
                          m_mapS0wS0LcS1c("Stack0WordStack0LConstituentStack1Constituent", TABLE_SIZE),
                          m_mapS0cS0LcS1w("Stack0ConstituentStack0LConstituentStack1Word", TABLE_SIZE),
                          m_mapS0cS0RcS1c("Stack0ConstituentStack0RConstituentStack1Constituent", TABLE_SIZE),
                          m_mapS0wS0RcS1c("Stack0WordStack0RConstituentStack1Constituent", TABLE_SIZE),
                          m_mapS0cS0RcS1w("Stack0ConstituentStack0RConstituentStack1Word", TABLE_SIZE),
                          m_mapS0cS0UcS1c("Stack0ConstituentStack0UConstituentStack1Constituent", TABLE_SIZE),
                          m_mapS0wS0UcS1c("Stack0WordStack0UConstituentStack1Constituent", TABLE_SIZE),
                          m_mapS0cS0UcS1w("Stack0ConstituentStack0UConstituentStack1Word", TABLE_SIZE),

                          m_mapN0tN1tN2t("NextTagTrigram", TABLE_SIZE),
                          m_mapS0cS1cS1Lc("Stack0ConstituentStack1ConstituentStack1LConstituent", TABLE_SIZE),
                          m_mapS0wS1cS1Lc("Stack0WordStack1ConstituentStack1LConstituent", TABLE_SIZE),
                          m_mapS0cS1wS1Lc("Stack0ConstituentStack1WordStack1LConstituent", TABLE_SIZE),
                          m_mapS0cS1cS1Rc("Stack0ConstituentStack1ConstituentStack1RConstituent", TABLE_SIZE),
                          m_mapS0jS1cS1Rj("Stack0TagOrConstituentStack1ConstituentStack1RTagOrConstituent", TABLE_SIZE),
                          m_mapS0wS1cS1Rc("Stack0WordStack1ConstituentStack1RConstituent", TABLE_SIZE),
                          m_mapS0cS1wS1Rc("Stack0ConstituentStack1WordStack1RConstituent", TABLE_SIZE),
                          m_mapS0cS1cS1Uc("Stack0ConstituentStack1ConstituentStack1UConstituent", TABLE_SIZE),
                          m_mapS0wS1cS1Uc("Stack0WordStack1ConstituentStack1UConstituent", TABLE_SIZE),
                          m_mapS0cS1wS1Uc("Stack0ConstituentStack1WordStack1UConstituent", TABLE_SIZE),

                          m_mapA1("PreviousAction", TABLE_SIZE),
                          m_mapA1A2("PreviousActionBigram", TABLE_SIZE),

                          m_mapWSS0w("WORDXYZS0W", TABLE_SIZE), 	//added
                          m_mapWSS1w("WORDXYZS1W", TABLE_SIZE), 	//added
                          m_mapWSS0wS1w("WORDXYZS0WS1W", TABLE_SIZE), 	//added
                          m_mapWSS0wTag("WORDXYZS0WTag", TABLE_SIZE), 	//added
                          m_mapWSS1wTag("WORDXYZS0WTag", TABLE_SIZE), 	//added
                          m_mapWSS0wS1wTag("WORDXYZS0WTag", TABLE_SIZE),   	//added

                          m_mapWSS0wS1z("WORDXYZS0WS1Z", TABLE_SIZE), 	//added
                          m_mapWSS0zS1w("WORDXYZS1WS0Z", TABLE_SIZE), 	//added
                          m_mapWSS0zS1z("WORDXYZS0ZS1Z", TABLE_SIZE), 	//added
                          m_mapWSS0zS1zTag("WORDXYZS0ZS1ZTag", TABLE_SIZE), 	//added

								m_mapCharUnigram("CharacterUnigram", TABLE_SIZE),
								m_mapCharBigram("CharacterBigram", TABLE_SIZE),
								m_mapCharTrigram("CharacterTrigram", TABLE_SIZE),
								m_mapSeenWords("WordDictionary", TABLE_SIZE),
								m_mapLastWordByWord("LastWordByWord", TABLE_SIZE),
								m_mapCurrentWordLastChar("CurrentWordLastChar", TABLE_SIZE),
								m_mapLastWordFirstChar("LastWordFirstChar", TABLE_SIZE),
								m_mapFirstCharLastWordByWord("FirstCharLastWordByWord", TABLE_SIZE),
								m_mapLastWordByLastChar("LastWordByLastChar", TABLE_SIZE),
								m_mapSeparateChars("SeparateChars", TABLE_SIZE),
								m_mapConsecutiveChars("ConsecutiveChars", TABLE_SIZE),
								m_mapFirstAndLastChars("FirstAndLastChars", TABLE_SIZE),
								m_mapOneCharWord("OneCharWord", TABLE_SIZE),
								m_mapLengthByFirstChar("LengthByFirstChar", TABLE_SIZE),
								m_mapLengthByLastChar("LengthByLastChar", TABLE_SIZE),
								m_mapLengthByLastWord("LengthByLastWord", TABLE_SIZE),
								m_mapLastLengthByWord("LastLengthByWord", TABLE_SIZE),

								m_mapLastTagByTag("LastTagByTag", TABLE_SIZE),
								m_mapLastTwoTagsByTag("LastTwoTagsByTag", TABLE_SIZE),
								m_mapCurrentTag("CurrentTag", TABLE_SIZE),
								m_mapTagByLastWord("TagByLastWord", TABLE_SIZE),
								m_mapLastTagByWord("LastTagByWord", TABLE_SIZE),
								m_mapTagByFirstChar("TagByFirstChar", TABLE_SIZE),
								m_mapTagByLastChar("TagByLastChar", TABLE_SIZE),
								m_mapTagByChar("TagByChar", TABLE_SIZE),
								m_mapTagOfOneCharWord("TagOfOneCharWord", TABLE_SIZE),
								m_mapRepeatedCharByTag("RepeatedCharByTag", TABLE_SIZE),
								m_mapTagByWordAndPrevChar("TagByWordAndPrevChar", TABLE_SIZE),
								m_mapTagByWordAndNextChar("TagByWordAndNextChar", TABLE_SIZE),
								m_mapTaggedCharByFirstChar("TaggedCharByFirstChar", TABLE_SIZE),
								m_mapTaggedCharByLastChar("TaggedCharByLastChar", TABLE_SIZE),
								m_mapTagByFirstCharCat("TagByFirstCharCat", TABLE_SIZE),
								m_mapTagByLastCharCat("TagByLastCharCat", TABLE_SIZE),

								m_mapSeparateCharCat("SeparateCharCat", TABLE_SIZE),
								m_mapConsecutiveCharCat("ConsecutiveCharCat", TABLE_SIZE),
				            m_mapTaggedSeparateChars("TaggedSeparateChars", TABLE_SIZE),
				            m_mapTaggedConsecutiveChars("TaggedConsecutiveChars", TABLE_SIZE),

				            m_mapWordTagTag("WordByTheNextTwoTag", TABLE_SIZE),
				            m_mapTagWordTag("TagByNextWordSecondNextTag", TABLE_SIZE),
				            m_mapFirstCharBy2Tags("FirstCharByTagAndLastTag", TABLE_SIZE),
				            m_mapFirstCharBy3Tags("FirstCharByTagLastTagAndSecondLastTag", TABLE_SIZE),
				            m_mapFirstCharAndChar("FirstCharAndChar", TABLE_SIZE),
				            m_mapSepCharAndNextChar("SeparatedCharAndNextChar", TABLE_SIZE),
				            m_mapAppCharAndNextChar("AppendedCharAndNextChar", TABLE_SIZE),
				            m_mapPartialWord("PartialWord", TABLE_SIZE),
				            m_mapPartialLengthByFirstChar("PartialLengthByFirstChar", TABLE_SIZE),
				            m_mapLengthByTagAndFirstChar("LengthByTagAndFirstChar", TABLE_SIZE),
				            m_mapLengthByTagAndLastChar("LengthByTagAndLastChar", TABLE_SIZE),
				            m_mapTag0Tag1Size1("Tag0Tag1Size1", TABLE_SIZE),
				            m_mapTag1Tag2Size1("Tag1Tag2Size1", TABLE_SIZE),
				            m_mapTag0Tag1Tag2Size1("Tag0Tag1Tag2Size1", TABLE_SIZE)
   { }
   ~CWeight() {
   	if (m_Knowledge) delete m_Knowledge;
//      iterate_templates(,.freePoolMemory(););
//      CPackedScore<SCORE_TYPE, CAction::MAX>::freePoolMemory();
   }

   // MEHTODS
   virtual void loadScores(std::ifstream &is);
   virtual void saveScores(std::ofstream &os);
   void computeAverageFeatureWeights(int round);
   SCORE_TYPE dotProduct(CWeight &w);
   void clear() {
      iterate_templates(,.clear(););
   }
   void addCurrent(CWeight*w, int round);
   void subtractCurrent(CWeight*w, int round);
#ifdef NO_NEG_FEATURE
   void setPositiveFeature(const CWeight*w);
#endif
   void scaleCurrent(const SCORE_TYPE &scale, const int &round) {
      iterate_templates(,ID(.scaleCurrent(scale, round);));
   }
   SCORE_TYPE squareNorm() {
      static SCORE_TYPE retval;
      retval = 0;
      iterate_templates(retval+=,.squareNorm(););
      return retval;
   }
 
};

};
};

#endif
