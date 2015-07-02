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
\
   left(m_mapN2w)right\
   left(m_mapN2t)right\
   left(m_mapN2wt)right\
\
   left(m_mapN3w)right\
   left(m_mapN3t)right\
   left(m_mapN3wt)right\
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
   left(m_mapS0LLwc)right\
   left(m_mapS0LLwt)right\
   left(m_mapS0LLtc)right\
   left(m_mapS0LRwc)right\
   left(m_mapS0LRwt)right\
   left(m_mapS0LRtc)right\
   left(m_mapS0LUwc)right\
   left(m_mapS0LUwt)right\
   left(m_mapS0LUtc)right\
   left(m_mapS0RLwc)right\
   left(m_mapS0RLwt)right\
   left(m_mapS0RLtc)right\
   left(m_mapS0RRwc)right\
   left(m_mapS0RRwt)right\
   left(m_mapS0RRtc)right\
   left(m_mapS0RUwc)right\
   left(m_mapS0RUwt)right\
   left(m_mapS0RUtc)right\
   left(m_mapS0ULwc)right\
   left(m_mapS0ULwt)right\
   left(m_mapS0ULtc)right\
   left(m_mapS0URwc)right\
   left(m_mapS0URwt)right\
   left(m_mapS0URtc)right\
   left(m_mapS0UUwc)right\
   left(m_mapS0UUwt)right\
   left(m_mapS0UUtc)right\
   left(m_mapS1LLwc)right\
   left(m_mapS1LLwt)right\
   left(m_mapS1LLtc)right\
   left(m_mapS1LRwc)right\
   left(m_mapS1LRwt)right\
   left(m_mapS1LRtc)right\
   left(m_mapS1LUwc)right\
   left(m_mapS1LUwt)right\
   left(m_mapS1LUtc)right\
   left(m_mapS1RLwc)right\
   left(m_mapS1RLwt)right\
   left(m_mapS1RLtc)right\
   left(m_mapS1RRwc)right\
   left(m_mapS1RRwt)right\
   left(m_mapS1RRtc)right\
   left(m_mapS1RUwc)right\
   left(m_mapS1RUwt)right\
   left(m_mapS1RUtc)right\
   left(m_mapS1ULwc)right\
   left(m_mapS1ULwt)right\
   left(m_mapS1ULtc)right\
   left(m_mapS1URwc)right\
   left(m_mapS1URwt)right\
   left(m_mapS1URtc)right\
   left(m_mapS1UUwc)right\
   left(m_mapS1UUwt)right\
   left(m_mapS1UUtc)right\
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
   left(m_mapA1A2)right


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
\
   left m_mapN2w middle m_mapN2w right\
   left m_mapN2t middle m_mapN2t right\
   left m_mapN2wt middle m_mapN2wt right\
\
   left m_mapN3w middle m_mapN3w right\
   left m_mapN3t middle m_mapN3t right\
   left m_mapN3wt middle m_mapN3wt right\
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
   left m_mapS0LLwc middle m_mapS0LLwc right\
   left m_mapS0LLwt middle m_mapS0LLwt right\
   left m_mapS0LLtc middle m_mapS0LLtc right\
   left m_mapS0LRwc middle m_mapS0LRwc right\
   left m_mapS0LRwt middle m_mapS0LRwt right\
   left m_mapS0LRtc middle m_mapS0LRtc right\
   left m_mapS0LUwc middle m_mapS0LUwc right\
   left m_mapS0LUwt middle m_mapS0LUwt right\
   left m_mapS0LUtc middle m_mapS0LUtc right\
   left m_mapS0RLwc middle m_mapS0RLwc right\
   left m_mapS0RLwt middle m_mapS0RLwt right\
   left m_mapS0RLtc middle m_mapS0RLtc right\
   left m_mapS0RRwc middle m_mapS0RRwc right\
   left m_mapS0RRwt middle m_mapS0RRwt right\
   left m_mapS0RRtc middle m_mapS0RRtc right\
   left m_mapS0RUwc middle m_mapS0RUwc right\
   left m_mapS0RUwt middle m_mapS0RUwt right\
   left m_mapS0RUtc middle m_mapS0RUtc right\
   left m_mapS0ULwc middle m_mapS0ULwc right\
   left m_mapS0ULwt middle m_mapS0ULwt right\
   left m_mapS0ULtc middle m_mapS0ULtc right\
   left m_mapS0URwc middle m_mapS0URwc right\
   left m_mapS0URwt middle m_mapS0URwt right\
   left m_mapS0URtc middle m_mapS0URtc right\
   left m_mapS0UUwc middle m_mapS0UUwc right\
   left m_mapS0UUwt middle m_mapS0UUwt right\
   left m_mapS0UUtc middle m_mapS0UUtc right\
   left m_mapS1LLwc middle m_mapS1LLwc right\
   left m_mapS1LLwt middle m_mapS1LLwt right\
   left m_mapS1LLtc middle m_mapS1LLtc right\
   left m_mapS1LRwc middle m_mapS1LRwc right\
   left m_mapS1LRwt middle m_mapS1LRwt right\
   left m_mapS1LRtc middle m_mapS1LRtc right\
   left m_mapS1LUwc middle m_mapS1LUwc right\
   left m_mapS1LUwt middle m_mapS1LUwt right\
   left m_mapS1LUtc middle m_mapS1LUtc right\
   left m_mapS1RLwc middle m_mapS1RLwc right\
   left m_mapS1RLwt middle m_mapS1RLwt right\
   left m_mapS1RLtc middle m_mapS1RLtc right\
   left m_mapS1RRwc middle m_mapS1RRwc right\
   left m_mapS1RRwt middle m_mapS1RRwt right\
   left m_mapS1RRtc middle m_mapS1RRtc right\
   left m_mapS1RUwc middle m_mapS1RUwc right\
   left m_mapS1RUwt middle m_mapS1RUwt right\
   left m_mapS1RUtc middle m_mapS1RUtc right\
   left m_mapS1ULwc middle m_mapS1ULwc right\
   left m_mapS1ULwt middle m_mapS1ULwt right\
   left m_mapS1ULtc middle m_mapS1ULtc right\
   left m_mapS1URwc middle m_mapS1URwc right\
   left m_mapS1URwt middle m_mapS1URwt right\
   left m_mapS1URtc middle m_mapS1URtc right\
   left m_mapS1UUwc middle m_mapS1UUwc right\
   left m_mapS1UUwt middle m_mapS1UUwt right\
   left m_mapS1UUtc middle m_mapS1UUtc right\
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
   left m_mapA1A2 middle m_mapA1A2 right

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
typedef CScoreMapType<CTuple2<CWord, CConstituent>, SCORE_TYPE, CAction::MAX>
CWordConstituentMap;
typedef CScoreMapType<CTuple2<CTag, CConstituent>, SCORE_TYPE, CAction::MAX> CTagConstituentMap;
typedef CScoreMapType<CTuple3<CTag, CTag, unsigned long>, SCORE_TYPE, CAction::MAX> CWordTagIntMap;
typedef CScoreMapType<CConstituent, SCORE_TYPE, CAction::MAX> CConstituentMap;
typedef CScoreMapType<CAction, SCORE_TYPE, CAction::MAX> CActionMap;
typedef CScoreMapType<CTuple2<CAction, CAction>, SCORE_TYPE, CAction::MAX> CAction2Map;

typedef CHashMap<CWord, unsigned long> CWordToIntMap;

//! added by zhumuhua
typedef CScoreMapType<CTuple2<CWord, CTag>, SCORE_TYPE, CAction::MAX> CWordTagMap;

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
   CIntMap m_mapN0t;
   CTaggedWordMap m_mapN0wt;

   // N1
   CWordMap m_mapN1w;
   CIntMap m_mapN1t;
   CTaggedWordMap m_mapN1wt;

   // N2
   CWordMap m_mapN2w;
   CIntMap m_mapN2t;
   CTaggedWordMap m_mapN2wt;

   // N3
   CWordMap m_mapN3w;
   CIntMap m_mapN3t;
   CTaggedWordMap m_mapN3wt;

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

   //! added by zhumuhua
   //! S0LL
   CWordConstituentMap m_mapS0LLwc;
   CWordTagMap m_mapS0LLwt;
   CTagConstituentMap m_mapS0LLtc;
   //! S0LR
   CWordConstituentMap m_mapS0LRwc;
   CWordTagMap m_mapS0LRwt;
   CTagConstituentMap m_mapS0LRtc;
   //! S0LU
   CWordConstituentMap m_mapS0LUwc;
   CWordTagMap m_mapS0LUwt;
   CTagConstituentMap m_mapS0LUtc;
   //! S0RL
   CWordConstituentMap m_mapS0RLwc;
   CWordTagMap m_mapS0RLwt;
   CTagConstituentMap m_mapS0RLtc;

   //! S0RR
   CWordConstituentMap m_mapS0RRwc;
   CWordTagMap m_mapS0RRwt;
   CTagConstituentMap m_mapS0RRtc;

   //! S0RU
   CWordConstituentMap m_mapS0RUwc;
   CWordTagMap m_mapS0RUwt;
   CTagConstituentMap m_mapS0RUtc;

   //! S0UL
   CWordConstituentMap m_mapS0ULwc;
   CWordTagMap m_mapS0ULwt;
   CTagConstituentMap m_mapS0ULtc;

   //! S0UR
   CWordConstituentMap m_mapS0URwc;
   CWordTagMap m_mapS0URwt;
   CTagConstituentMap m_mapS0URtc;

   //! S0UU
   CWordConstituentMap m_mapS0UUwc;
   CWordTagMap m_mapS0UUwt;
   CTagConstituentMap m_mapS0UUtc;

   
   //! S1LL
   CWordConstituentMap m_mapS1LLwc;
   CWordTagMap m_mapS1LLwt;
   CTagConstituentMap m_mapS1LLtc;
   //! S1LR
   CWordConstituentMap m_mapS1LRwc;
   CWordTagMap m_mapS1LRwt;
   CTagConstituentMap m_mapS1LRtc;
   //! S1LU
   CWordConstituentMap m_mapS1LUwc;
   CWordTagMap m_mapS1LUwt;
   CTagConstituentMap m_mapS1LUtc;

   //! S1RL
   CWordConstituentMap m_mapS1RLwc;
   CWordTagMap m_mapS1RLwt;
   CTagConstituentMap m_mapS1RLtc;
   
   //! S1RR
   CWordConstituentMap m_mapS1RRwc;
   CWordTagMap m_mapS1RRwt;
   CTagConstituentMap m_mapS1RRtc;

   //! S1RU
   CWordConstituentMap m_mapS1RUwc;
   CWordTagMap m_mapS1RUwt;
   CTagConstituentMap m_mapS1RUtc;

   //! S1UL
   CWordConstituentMap m_mapS1ULwc;
   CWordTagMap m_mapS1ULwt;
   CTagConstituentMap m_mapS1ULtc;
   
   //! S1UR
   CWordConstituentMap m_mapS1URwc;
   CWordTagMap m_mapS1URwt;
   CTagConstituentMap m_mapS1URtc;

   //! S1UU
   CWordConstituentMap m_mapS1UUwc;
   CWordTagMap m_mapS1UUwt;
   CTagConstituentMap m_mapS1UUtc;
   

   // S0 S1
   CTwoWordsCFGSetMap m_mapS0wcS1wc; // the original 
   CWordCFGSetMap m_mapS0wS1c;
   CWordCFGSetMap m_mapS0cS1w;
   CCFGSetMap m_mapS0cS1c;

   CCFGSetMap m_mapBetweenTags;

   // S0 N0
   CTwoWordsCFGSetMap m_mapS0wN0w;
   CWordCFGSetMap m_mapS0cN0w;
   CWordCFGSetMap m_mapS0wN0t;
   CCFGSetMap m_mapS0cN0t;
   CTwoIntMap m_mapS0cmN0tm;

   // S1 N0
   CTwoWordsCFGSetMap m_mapS1wN0w;
   CWordCFGSetMap m_mapS1cN0w;
   CWordCFGSetMap m_mapS1wN0t;
   CCFGSetMap m_mapS1cN0t;

   // N0 N1
   CTwoWordsCFGSetMap m_mapN0wN1w;
   CWordCFGSetMap m_mapN0tN1w;
   CWordCFGSetMap m_mapN0wN1t;
   CCFGSetMap m_mapN0tN1t;

   // S0 S1N0
   CWordCFGSetMap m_mapS0wS1cN0t;
   CWordCFGSetMap m_mapS0cS1wN0t;
   CWordCFGSetMap m_mapS0cS1cN0w;
   CCFGSetMap m_mapS0cS1cN0t;
   CCFGSetMap m_mapS0tS1tN0t;
   CCFGSetMap m_mapS0jS1jN0t;

   // S0 N0N1
   CWordCFGSetMap m_mapS0wN0tN1t;
   CWordCFGSetMap m_mapS0cN0wN1t;
   CWordCFGSetMap m_mapS0cN0tN1w;
   CCFGSetMap m_mapS0cN0tN1t;
   CCFGSetMap m_mapS0tN0tN1t;
   CCFGSetMap m_mapS0jN0tN1t;

   // S0 S1S2
   CWordCFGSetMap m_mapS0wS1cS2c;
   CWordCFGSetMap m_mapS0cS1wS2c;
   CWordCFGSetMap m_mapS0cS1cS2w;
   CCFGSetMap m_mapS0cS1cS2c;
   CCFGSetMap m_mapS0tS1tS2t;

   // tag bigram
   CIntMap m_mapS1cS2c;
   CIntMap m_mapS2cS3c;
   CIntMap m_mapN1tN2t;
   CIntMap m_mapN2tN3t;
   
   CIntMap m_mapS0cS2c;
   CIntMap m_mapS1cS3c;
   CIntMap m_mapS0cN1t;
   CIntMap m_mapN0tN2t;
   CIntMap m_mapN1tN3t;

   CIntMap m_mapN0tN1tN2t;

   // S0 S0LRUN0
   CCFGSetMap m_mapS0cS0RcN0t;
   CCFGSetMap m_mapS0cS0RjN0t;
   CWordCFGSetMap m_mapS0cS0RcN0w;

   CCFGSetMap m_mapS0cS0LcN0t;
   CWordCFGSetMap m_mapS0wS0LcN0t;
   CWordCFGSetMap m_mapS0cS0LcN0w;
   CWordCFGSetMap m_mapS0wS0RcN0t;
   CCFGSetMap m_mapS0cS0UcN0t;
   CWordCFGSetMap m_mapS0wS0UcN0t;
   CWordCFGSetMap m_mapS0cS0UcN0w;

   // S0 S0LRUS1
   CCFGSetMap m_mapS0cS0LcS1c;
   CCFGSetMap m_mapS0cS0LjS1j;
   CWordCFGSetMap m_mapS0cS0LcS1w;

   CWordCFGSetMap m_mapS0wS0LcS1c;
   CCFGSetMap m_mapS0cS0RcS1c;
   CWordCFGSetMap m_mapS0wS0RcS1c;
   CWordCFGSetMap m_mapS0cS0RcS1w;
   CCFGSetMap m_mapS0cS0UcS1c;
   CWordCFGSetMap m_mapS0wS0UcS1c;
   CWordCFGSetMap m_mapS0cS0UcS1w;

   // S0 S1LRUS1
   CCFGSetMap m_mapS0cS1cS1Rc;
   CCFGSetMap m_mapS0jS1cS1Rj;
   CWordCFGSetMap m_mapS0wS1cS1Rc;

   CCFGSetMap m_mapS0cS1cS1Lc;
   CWordCFGSetMap m_mapS0wS1cS1Lc;
   CWordCFGSetMap m_mapS0cS1wS1Lc;
   CWordCFGSetMap m_mapS0cS1wS1Rc;
   CCFGSetMap m_mapS0cS1cS1Uc;
   CWordCFGSetMap m_mapS0wS1cS1Uc;
   CWordCFGSetMap m_mapS0cS1wS1Uc;

   // past action
   CActionMap m_mapA1;
   CAction2Map m_mapA1A2;

   // Rule dictionary
   unsigned long m_nMaxWordFrequency;
   CWordToIntMap m_mapWordFrequency;

public:

   CWeight(bool bTrain, unsigned TABLE_SIZE = DEFAULT_TABLE_SIZE) : 
                          CWeightBase(bTrain) ,

                          m_nMaxWordFrequency(0) ,
                          m_mapWordFrequency(TABLE_SIZE),

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

                          m_mapN2w("Next2Word", TABLE_SIZE),
                          m_mapN2t("Next2Tag", TABLE_SIZE),
                          m_mapN2wt("Next2WordTag", TABLE_SIZE),

                          m_mapN3w("Next3Word", TABLE_SIZE),
                          m_mapN3t("Next3Tag", TABLE_SIZE),
                          m_mapN3wt("Next3WordTag", TABLE_SIZE),

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

                          //! added by zhumuhua
                          m_mapS0LLwc("S0LLWordConstituent", TABLE_SIZE),
                          m_mapS0LLwt("S0LLWordTag", TABLE_SIZE),
                          m_mapS0LLtc("S0LLTagConstituent", TABLE_SIZE),
                          m_mapS0LRwc("S0LRWordConstituent", TABLE_SIZE),
                          m_mapS0LRwt("S0LRWordTag", TABLE_SIZE),
                          m_mapS0LRtc("S0LRTagConstituent", TABLE_SIZE),
                          m_mapS0LUwc("S0LUWordConstituent", TABLE_SIZE),
                          m_mapS0LUwt("S0LUWordTag", TABLE_SIZE),
                          m_mapS0LUtc("S0LUTagConstituent", TABLE_SIZE),

                          m_mapS0RLwc("S0RLWordConstituent", TABLE_SIZE),
                          m_mapS0RLwt("S0RLWordTag", TABLE_SIZE),
                          m_mapS0RLtc("S0RLTagConstituent", TABLE_SIZE),
                          m_mapS0RRwc("S0RRWordConstituent", TABLE_SIZE),
                          m_mapS0RRwt("S0RRWordTag", TABLE_SIZE),
                          m_mapS0RRtc("S0RRTagConstituent", TABLE_SIZE),
                          m_mapS0RUwc("S0RUWordConstituent", TABLE_SIZE),
                          m_mapS0RUwt("S0RUWordTag", TABLE_SIZE),
                          m_mapS0RUtc("S0RUTagConstituent", TABLE_SIZE),

                          m_mapS0ULwc("S0ULWordConstituent", TABLE_SIZE),
                          m_mapS0ULwt("S0ULWordTag", TABLE_SIZE),
                          m_mapS0ULtc("S0ULTagConstituent", TABLE_SIZE),
                          m_mapS0URwc("S0URWordConstituent", TABLE_SIZE),
                          m_mapS0URwt("S0URWordTag", TABLE_SIZE),
                          m_mapS0URtc("S0URTagConstituent", TABLE_SIZE),
                          m_mapS0UUwc("S0UUWordConstituent", TABLE_SIZE),
                          m_mapS0UUwt("S0UUWordTag", TABLE_SIZE),
                          m_mapS0UUtc("S0UUTagConstituent", TABLE_SIZE),

                          m_mapS1LLwc("S1LLWordConstituent", TABLE_SIZE),
                          m_mapS1LLwt("S1LLWordTag", TABLE_SIZE),
                          m_mapS1LLtc("S1LLTagConstituent", TABLE_SIZE),
                          m_mapS1LRwc("S1LRWordConstituent", TABLE_SIZE),
                          m_mapS1LRwt("S1LRWordTag", TABLE_SIZE),
                          m_mapS1LRtc("S1LRTagConstituent", TABLE_SIZE),
                          m_mapS1LUwc("S1LUWordConstituent", TABLE_SIZE),
                          m_mapS1LUwt("S1LUWordTag", TABLE_SIZE),
                          m_mapS1LUtc("S1LUTagConstituent", TABLE_SIZE),

                          m_mapS1RLwc("S1RLWordConstituent", TABLE_SIZE),
                          m_mapS1RLwt("S1RLWordTag", TABLE_SIZE),
                          m_mapS1RLtc("S1RLTagConstituent", TABLE_SIZE),
                          m_mapS1RRwc("S1RRWordConstituent", TABLE_SIZE),
                          m_mapS1RRwt("S1RRWordTag", TABLE_SIZE),
                          m_mapS1RRtc("S1RRTagConstituent", TABLE_SIZE),
                          m_mapS1RUwc("S1RUWordConstituent", TABLE_SIZE),
                          m_mapS1RUwt("S1RUWordTag", TABLE_SIZE),
                          m_mapS1RUtc("S1RUTagConstituent", TABLE_SIZE),

                          m_mapS1ULwc("S1ULWordConstituent", TABLE_SIZE),
                          m_mapS1ULwt("S1ULWordTag", TABLE_SIZE),
                          m_mapS1ULtc("S1ULTagConstituent", TABLE_SIZE),
                          m_mapS1URwc("S1URWordConstituent", TABLE_SIZE),
                          m_mapS1URwt("S1URWordTag", TABLE_SIZE),
                          m_mapS1URtc("S1URTagConstituent", TABLE_SIZE),
                          m_mapS1UUwc("S1UUWordConstituent", TABLE_SIZE),
                          m_mapS1UUwt("S1UUWordTag", TABLE_SIZE),
                          m_mapS1UUtc("S1UUTagConstituent", TABLE_SIZE),

                          m_mapS0wcS1wc("Stack0WordStack1Word", TABLE_SIZE),
                          m_mapS0wS1c("Stack0WordStack1Constituent", TABLE_SIZE),
                          m_mapS0cS1w("Stack0ConstituentStack1Word", TABLE_SIZE),
                          m_mapS0cS1c("Stack0ConstituentStack1Constituent",
TABLE_SIZE),
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
                          m_mapA1A2("PreviousActionBigram", TABLE_SIZE)
   { }
   ~CWeight() {
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
