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
   left(m_mapSYNS0w)right\
   left(m_mapSYNS0c)right\
   left(m_mapSYNS0tc)right\
   left(m_mapSYNS0wc)right\
   left(m_mapSYNS1w)right\
   left(m_mapSYNS1c)right\
   left(m_mapSYNS1tc)right\
   left(m_mapSYNS1wc)right\
   left(m_mapSYNS2tc)right\
   left(m_mapSYNS2wc)right\
   left(m_mapSYNS3tc)right\
   left(m_mapSYNS3wc)right\
   left(m_mapSYNN0w)right\
   left(m_mapSYNN1w)right\
   left(m_mapSYNN01w)right\
   left(m_mapSYNN2w)right\
   left(m_mapSYNN12w)right\
   left(m_mapSYNN012w)right\
   left(m_mapSYNN3w)right\
   left(m_mapSYNN23w)right\
   left(m_mapSYNN123w)right\
   left(m_mapSYNS0Ltc)right\
   left(m_mapSYNS0Lwc)right\
   left(m_mapSYNS0Rtc)right\
   left(m_mapSYNS0Rwc)right\
   left(m_mapSYNS0Utc)right\
   left(m_mapSYNS0Uwc)right\
   left(m_mapSYNS1Ltc)right\
   left(m_mapSYNS1Lwc)right\
   left(m_mapSYNS1Rtc)right\
   left(m_mapSYNS1Rwc)right\
   left(m_mapSYNS1Utc)right\
   left(m_mapSYNS1Uwc)right\
   left(m_mapSYNS0wcS1wc)right\
   left(m_mapSYNS0cS1w)right\
   left(m_mapSYNS0wS1c)right\
   left(m_mapSYNS0cS1c)right\
   left(m_mapSYNS0wcN0w)right\
   left(m_mapSYNS0cN0w)right\
   left(m_mapSYNS1wcN0w)right\
   left(m_mapSYNS1cN0w)right\
   left(m_mapSYNS0cS1cN0w)right\
   left(m_mapSYNS0wS1cS2c)right\
   left(m_mapSYNS0cS1wS2c)right\
   left(m_mapSYNS0cS1cS2w)right\
   left(m_mapSYNS0cS1cS2c)right\
   left(m_mapSYNS0tS1tS2t)right\
   left(m_mapSYNS0cS0RcN0w)right\
   left(m_mapSYNS0cS0LcS1c)right\
   left(m_mapSYNS0cS0LjS1j)right\
   left(m_mapSYNS0cS0LcS1w)right\
   left(m_mapSYNS0cS1cS1Rc)right\
   left(m_mapSYNS0jS1cS1Rj)right\
   left(m_mapSYNS0wS1cS1Rc)right\
   left(m_mapSYNS0z)right\
   left(m_mapSYNS0zc)right\
   left(m_mapSYNS0CharTagCont)right\
   left(m_mapSYNS1z)right\
   left(m_mapSYNS1zc)right\
   left(m_mapSYNS1CharTagCont)right\
   left(m_mapSYNS2CharTag)right\
   left(m_mapSYNS2zc)right\
   left(m_mapSYNS2CharTagCont)right\
   left(m_mapSYNS3CharTag)right\
   left(m_mapSYNS3zc)right\
   left(m_mapSYNS3CharTagCont)right\
   left(m_mapSYNS0ztS1zt)right\
   left(m_mapSYNS0zS1z)right\
   left(m_mapSYNS0zcS1zc)right\
   left(m_mapSYNS0cS1zc)right\
   left(m_mapSYNS0zcS1c)right\
   left(m_mapSYNS0zS1zS2z)right\
   left(m_mapSYNS0zS1cS2c)right\
   left(m_mapSYNS0cS1zS2c)right\
   left(m_mapSYNS0cS1cS2z)right\
   left(m_mapSYNS1zS2zS3z)right\
   left(m_mapSYNS0zS1zS2zS3z)right\
   left(m_mapSYNS0zN0z)right\
   left(m_mapSYNS0zcN0z)right\
   left(m_mapSYNS0zN01z)right\
   left(m_mapSYNS0zN012z)right\
   left(m_mapSYNS1zN0z)right\
   left(m_mapSYNS1zcN0z)right\
   left(m_mapSYNS1zN01z)right\
   left(m_mapSYNS1zN012z)right\
   left(m_mapSYNS1zS0zN0z)right\
   left(m_mapSYNS1zS0zN01z)right\
   left(m_mapPSYNS0w)right\
   left(m_mapPSYNS0c)right\
   left(m_mapPSYNS0tc)right\
   left(m_mapPSYNS0wc)right\
   left(m_mapPSYNS1w)right\
   left(m_mapPSYNS1c)right\
   left(m_mapPSYNS1tc)right\
   left(m_mapPSYNS1wc)right\
   left(m_mapPSYNS2tc)right\
   left(m_mapPSYNS2wc)right\
   left(m_mapPSYNS3tc)right\
   left(m_mapPSYNS3wc)right\
   left(m_mapPSYNS0Ltc)right\
   left(m_mapPSYNS0Lwc)right\
   left(m_mapPSYNS0Rtc)right\
   left(m_mapPSYNS0Rwc)right\
   left(m_mapPSYNS0Utc)right\
   left(m_mapPSYNS0Uwc)right\
   left(m_mapPSYNS1Ltc)right\
   left(m_mapPSYNS1Lwc)right\
   left(m_mapPSYNS1Rtc)right\
   left(m_mapPSYNS1Rwc)right\
   left(m_mapPSYNS1Utc)right\
   left(m_mapPSYNS1Uwc)right\
   left(m_mapPPSYNS0wcS1wc)right\
   left(m_mapPPSYNS0cS1wc)right\
   left(m_mapPPSYNS0wcS1c)right\
   left(m_mapPPSYNS0cS1c)right\
   left(m_mapPSYNS0wcS1wc)right\
   left(m_mapPSYNS0cS1wc)right\
   left(m_mapPSYNS0wcS1c)right\
   left(m_mapPSYNS0cS1c)right\
   left(m_mapPSYNS0wcN0w)right\
   left(m_mapPSYNS0cN0w)right\
   left(m_mapPSYNS0wN0t)right\
   left(m_mapPSYNS0cN0t)right\
   left(m_mapPSYNS1wcN0w)right\
   left(m_mapPSYNS1cN0w)right\
   left(m_mapPSYNS1wN0t)right\
   left(m_mapPSYNS1cN0t)right\
   left(m_mapSYNN0t)right\
   left(m_mapSYNN1t)right\
   left(m_mapSYNN2t)right\
   left(m_mapSYNN3t)right\
   left(m_mapSYNS0wN0t)right\
   left(m_mapSYNS0cN0t)right\
   left(m_mapSYNS1wN0t)right\
   left(m_mapSYNS1cN0t)right\
   left(m_mapSYNN0tN1w)right\
   left(m_mapSYNN0wN1t)right\
   left(m_mapSYNN0tN1t)right\
   left(m_mapSYNS0wS1cN0t)right\
   left(m_mapSYNS0cS1wN0t)right\
   left(m_mapSYNS0cS1cN0t)right\
   left(m_mapSYNS0cS1tN0t)right\
   left(m_mapSYNS0wcN0tN1t)right\
   left(m_mapSYNS0cN0wtN1t)right\
   left(m_mapSYNS0cN0tN1wt)right\
   left(m_mapSYNS0cN0tN1t)right\
   left(m_mapSYNS0tN0tN1t)right\
   left(m_mapPSYNS0z)right\
   left(m_mapPSYNS0zc)right\
   left(m_mapPSYNS0CharTagCont)right\
   left(m_mapPSYNS1z)right\
   left(m_mapPSYNS1zc)right\
   left(m_mapPSYNS1CharTagCont)right\
   left(m_mapPSYNS2zc)right\
   left(m_mapPSYNS2CharTagCont)right\
   left(m_mapPSYNS3zc)right\
   left(m_mapPSYNS3CharTagCont)right\
   left(m_mapPPSYNS0ztS1zt)right\
   left(m_mapPPSYNS0zS1z)right\
   left(m_mapPPSYNS0zcS1zc)right\
   left(m_mapPPSYNS0cS1zc)right\
   left(m_mapPPSYNS0zcS1c)right\
   left(m_mapPSYNS0ztS1zt)right\
   left(m_mapPSYNS0zS1z)right\
   left(m_mapPSYNS0zcS1zc)right\
   left(m_mapPSYNS0cS1zc)right\
   left(m_mapPSYNS0zcS1c)right\
   left(m_mapPSYNS0zN0z)right\
   left(m_mapPSYNS0zcN0z)right\
   left(m_mapPSYNS0zN01z)right\
   left(m_mapPSYNS0zN012z)right\
   left(m_mapPSYNS1zN0z)right\
   left(m_mapPSYNS1zcN0z)right\
   left(m_mapPSYNS1zN01z)right\
   left(m_mapPSYNS1zN012z)right\
   left(m_mapPPSYNS1zS0zN0z)right\
   left(m_mapPPSYNS1zS0zN01z)right\
   left(m_mapPSYNS1zS0zN0z)right\
   left(m_mapPSYNS1zS0zN01z)right\
	left(m_mapWSLastWordByLastChar)right\
	left(m_mapWSLengthByLastWord)right\
	left(m_mapWSLastLengthByWord)right\
	left(m_mapWSSeparateChars)right\
	left(m_mapWSFirstCharLastWordByWord)right\
   left(m_mapLastTagByTag)right\
   left(m_mapTag0Tag1Size1)right\
   left(m_mapTag0Tag1Tag2Size1)right\
   left(m_mapTagByLastWord)right\
   left(m_mapLastTwoTagsByTag)right\
   left(m_mapTagByFirstChar)right\
   left(m_mapTagByFirstCharCat)right\
   left(m_mapFirstCharBy2Tags)right\
   left(m_mapFirstCharBy3Tags)right\
   left(m_mapTagByChar)right\
   left(m_mapTaggedSeparateChars)right\
   left(m_mapTagWordTag)right\
   left(m_mapWordTagTag)right\
   left(m_mapSeenWords)right\
   left(m_mapLastWordByWord)right\
   left(m_mapOneCharWord)right\
   left(m_mapFirstAndLastChars)right\
   left(m_mapLengthByFirstChar)right\
   left(m_mapLengthByLastChar)right\
   left(m_mapLeftPartWordTag)right\
   left(m_mapRightPartWordTag)right\
   left(m_mapCurrentWordLastChar)right\
   left(m_mapLastWordByLastChar)right\
   left(m_mapLengthByLastWord)right\
   left(m_mapLastLengthByWord)right\
   left(m_mapCurrentTag)right\
   left(m_mapLastTagByWord)right\
   left(m_mapTagByWordAndPrevChar)right\
   left(m_mapTagOfOneCharWord)right\
   left(m_mapTagByLastChar)right\
   left(m_mapTagByLastCharCat)right\
   left(m_mapTaggedCharByLastChar)right\
   left(m_mapTag1Tag2Size1)right\
   left(m_mapSeparateChars)right\
   left(m_mapLastWordFirstChar)right\
   left(m_mapFirstCharLastWordByWord)right\
   left(m_mapTagByWordAndNextChar)right\
   left(m_mapAPPTagByChar)right\
   left(m_mapTaggedCharByFirstChar)right\
   left(m_mapConsecutiveChars)right\
   left(m_mapTaggedConsecutiveChars)right\
   left(m_mapSSYNTaggedSeparateChars)right\
   left(m_mapSSYNS0fzcurz)right\
   left(m_mapSSYNS0lzcurz)right\
   left(m_mapSSYNS1fzcurw)right\
   left(m_mapSSYNS1lzcurw)right\
   left(m_mapSSYNS1lzcurwlz)right\
   left(m_mapSSYNS1fzcurwt)right\
   left(m_mapSSYNS1lzcurwt)right\
   left(m_mapSSYNS1lzcurwlzt)right\
   left(m_mapPSYNS1lzcurwlz)right\
   left(m_mapInSubWordDictionary)right\
	left(m_mapInSubWordTagDictionary)right\
	left(m_mapInWordDictionary)right\
	left(m_mapInWordTagDictionary)right\
	left(m_mapPSYNS2z)right\
	left(m_mapSYNS2z)right\
	left(m_mapPSYNS0zS1zS2z)right\
	left(m_mapPPSYNS0zS1zS2z)right\
	left(m_mapPPPSYNS0zS1zS2z)right\
	left(m_mapPSYNS0zS1cS2c)right\
	left(m_mapPSYNS0cS1zS2c)right\
	left(m_mapPSYNS0cS1cS2z)right\
	left(m_mapPPSYNS0zS1cS2c)right\
	left(m_mapPPSYNS0cS1zS2c)right\
	left(m_mapPPSYNS0cS1cS2z)right\
	left(m_mapPPPSYNS0zS1cS2c)right\
	left(m_mapPPPSYNS0cS1zS2c)right\
	left(m_mapPPPSYNS0cS1cS2z)right\
	left(m_mapSubWordHeadCharType)right\
	left(m_mapSubWordHeadCharTypeTag)right\
	left(m_mapSubWordHeadCharEqual)right\
	left(m_mapSubWordHeadCharEqualTag)right\
	left(m_mapInSubWordDictionaryTag)right\
	left(m_mapInSubWordTagDictionaryTag)right\
	left(m_mapSYNRightMostc)right\
	left(m_mapSYNRightMostcWord)right



#define iterate_double_templates(left, middle, right)\
   left m_mapSYNS0w middle m_mapSYNS0w right\
   left m_mapSYNS0c middle m_mapSYNS0c right\
   left m_mapSYNS0tc middle m_mapSYNS0tc right\
   left m_mapSYNS0wc middle m_mapSYNS0wc right\
   left m_mapSYNS1w middle m_mapSYNS1w right\
   left m_mapSYNS1c middle m_mapSYNS1c right\
   left m_mapSYNS1tc middle m_mapSYNS1tc right\
   left m_mapSYNS1wc middle m_mapSYNS1wc right\
   left m_mapSYNS2tc middle m_mapSYNS2tc right\
   left m_mapSYNS2wc middle m_mapSYNS2wc right\
   left m_mapSYNS3tc middle m_mapSYNS3tc right\
   left m_mapSYNS3wc middle m_mapSYNS3wc right\
   left m_mapSYNN0w middle m_mapSYNN0w right\
   left m_mapSYNN1w middle m_mapSYNN1w right\
   left m_mapSYNN01w middle m_mapSYNN01w right\
   left m_mapSYNN2w middle m_mapSYNN2w right\
   left m_mapSYNN12w middle m_mapSYNN12w right\
   left m_mapSYNN012w middle m_mapSYNN012w right\
   left m_mapSYNN3w middle m_mapSYNN3w right\
   left m_mapSYNN23w middle m_mapSYNN23w right\
   left m_mapSYNN123w middle m_mapSYNN123w right\
   left m_mapSYNS0Ltc middle m_mapSYNS0Ltc right\
   left m_mapSYNS0Lwc middle m_mapSYNS0Lwc right\
   left m_mapSYNS0Rtc middle m_mapSYNS0Rtc right\
   left m_mapSYNS0Rwc middle m_mapSYNS0Rwc right\
   left m_mapSYNS0Utc middle m_mapSYNS0Utc right\
   left m_mapSYNS0Uwc middle m_mapSYNS0Uwc right\
   left m_mapSYNS1Ltc middle m_mapSYNS1Ltc right\
   left m_mapSYNS1Lwc middle m_mapSYNS1Lwc right\
   left m_mapSYNS1Rtc middle m_mapSYNS1Rtc right\
   left m_mapSYNS1Rwc middle m_mapSYNS1Rwc right\
   left m_mapSYNS1Utc middle m_mapSYNS1Utc right\
   left m_mapSYNS1Uwc middle m_mapSYNS1Uwc right\
   left m_mapSYNS0wcS1wc middle m_mapSYNS0wcS1wc right\
   left m_mapSYNS0cS1w middle m_mapSYNS0cS1w right\
   left m_mapSYNS0wS1c middle m_mapSYNS0wS1c right\
   left m_mapSYNS0cS1c middle m_mapSYNS0cS1c right\
   left m_mapSYNS0wcN0w middle m_mapSYNS0wcN0w right\
   left m_mapSYNS0cN0w middle m_mapSYNS0cN0w right\
   left m_mapSYNS1wcN0w middle m_mapSYNS1wcN0w right\
   left m_mapSYNS1cN0w middle m_mapSYNS1cN0w right\
   left m_mapSYNS0cS1cN0w middle m_mapSYNS0cS1cN0w right\
   left m_mapSYNS0wS1cS2c middle m_mapSYNS0wS1cS2c right\
   left m_mapSYNS0cS1wS2c middle m_mapSYNS0cS1wS2c right\
   left m_mapSYNS0cS1cS2w middle m_mapSYNS0cS1cS2w right\
   left m_mapSYNS0cS1cS2c middle m_mapSYNS0cS1cS2c right\
   left m_mapSYNS0tS1tS2t middle m_mapSYNS0tS1tS2t right\
   left m_mapSYNS0cS0RcN0w middle m_mapSYNS0cS0RcN0w right\
   left m_mapSYNS0cS0LcS1c middle m_mapSYNS0cS0LcS1c right\
   left m_mapSYNS0cS0LjS1j middle m_mapSYNS0cS0LjS1j right\
   left m_mapSYNS0cS0LcS1w middle m_mapSYNS0cS0LcS1w right\
   left m_mapSYNS0cS1cS1Rc middle m_mapSYNS0cS1cS1Rc right\
   left m_mapSYNS0jS1cS1Rj middle m_mapSYNS0jS1cS1Rj right\
   left m_mapSYNS0wS1cS1Rc middle m_mapSYNS0wS1cS1Rc right\
   left m_mapSYNS0z middle m_mapSYNS0z right\
   left m_mapSYNS0zc middle m_mapSYNS0zc right\
   left m_mapSYNS0CharTagCont middle m_mapSYNS0CharTagCont right\
   left m_mapSYNS1z middle m_mapSYNS1z right\
   left m_mapSYNS1zc middle m_mapSYNS1zc right\
   left m_mapSYNS1CharTagCont middle m_mapSYNS1CharTagCont right\
   left m_mapSYNS2CharTag middle m_mapSYNS2CharTag right\
   left m_mapSYNS2zc middle m_mapSYNS2zc right\
   left m_mapSYNS2CharTagCont middle m_mapSYNS2CharTagCont right\
   left m_mapSYNS3CharTag middle m_mapSYNS3CharTag right\
   left m_mapSYNS3zc middle m_mapSYNS3zc right\
   left m_mapSYNS3CharTagCont middle m_mapSYNS3CharTagCont right\
   left m_mapSYNS0ztS1zt middle m_mapSYNS0ztS1zt right\
   left m_mapSYNS0zS1z middle m_mapSYNS0zS1z right\
   left m_mapSYNS0zcS1zc middle m_mapSYNS0zcS1zc right\
   left m_mapSYNS0cS1zc middle m_mapSYNS0cS1zc right\
   left m_mapSYNS0zcS1c middle m_mapSYNS0zcS1c right\
   left m_mapSYNS0zS1zS2z middle m_mapSYNS0zS1zS2z right\
   left m_mapSYNS0zS1cS2c middle m_mapSYNS0zS1cS2c right\
   left m_mapSYNS0cS1zS2c middle m_mapSYNS0cS1zS2c right\
   left m_mapSYNS0cS1cS2z middle m_mapSYNS0cS1cS2z right\
   left m_mapSYNS1zS2zS3z middle m_mapSYNS1zS2zS3z right\
   left m_mapSYNS0zS1zS2zS3z middle m_mapSYNS0zS1zS2zS3z right\
   left m_mapSYNS0zN0z middle m_mapSYNS0zN0z right\
   left m_mapSYNS0zcN0z middle m_mapSYNS0zcN0z right\
   left m_mapSYNS0zN01z middle m_mapSYNS0zN01z right\
   left m_mapSYNS0zN012z middle m_mapSYNS0zN012z right\
   left m_mapSYNS1zN0z middle m_mapSYNS1zN0z right\
   left m_mapSYNS1zcN0z middle m_mapSYNS1zcN0z right\
   left m_mapSYNS1zN01z middle m_mapSYNS1zN01z right\
   left m_mapSYNS1zN012z middle m_mapSYNS1zN012z right\
   left m_mapSYNS1zS0zN0z middle m_mapSYNS1zS0zN0z right\
   left m_mapSYNS1zS0zN01z middle m_mapSYNS1zS0zN01z right\
   left m_mapPSYNS0w middle m_mapPSYNS0w right\
   left m_mapPSYNS0c middle m_mapPSYNS0c right\
   left m_mapPSYNS0tc middle m_mapPSYNS0tc right\
   left m_mapPSYNS0wc middle m_mapPSYNS0wc right\
   left m_mapPSYNS1w middle m_mapPSYNS1w right\
   left m_mapPSYNS1c middle m_mapPSYNS1c right\
   left m_mapPSYNS1tc middle m_mapPSYNS1tc right\
   left m_mapPSYNS1wc middle m_mapPSYNS1wc right\
   left m_mapPSYNS2tc middle m_mapPSYNS2tc right\
   left m_mapPSYNS2wc middle m_mapPSYNS2wc right\
   left m_mapPSYNS3tc middle m_mapPSYNS3tc right\
   left m_mapPSYNS3wc middle m_mapPSYNS3wc right\
   left m_mapPSYNS0Ltc middle m_mapPSYNS0Ltc right\
   left m_mapPSYNS0Lwc middle m_mapPSYNS0Lwc right\
   left m_mapPSYNS0Rtc middle m_mapPSYNS0Rtc right\
   left m_mapPSYNS0Rwc middle m_mapPSYNS0Rwc right\
   left m_mapPSYNS0Utc middle m_mapPSYNS0Utc right\
   left m_mapPSYNS0Uwc middle m_mapPSYNS0Uwc right\
   left m_mapPSYNS1Ltc middle m_mapPSYNS1Ltc right\
   left m_mapPSYNS1Lwc middle m_mapPSYNS1Lwc right\
   left m_mapPSYNS1Rtc middle m_mapPSYNS1Rtc right\
   left m_mapPSYNS1Rwc middle m_mapPSYNS1Rwc right\
   left m_mapPSYNS1Utc middle m_mapPSYNS1Utc right\
   left m_mapPSYNS1Uwc middle m_mapPSYNS1Uwc right\
   left m_mapPPSYNS0wcS1wc middle m_mapPPSYNS0wcS1wc right\
   left m_mapPPSYNS0cS1wc middle m_mapPPSYNS0cS1wc right\
   left m_mapPPSYNS0wcS1c middle m_mapPPSYNS0wcS1c right\
   left m_mapPPSYNS0cS1c middle m_mapPPSYNS0cS1c right\
   left m_mapPSYNS0wcS1wc middle m_mapPSYNS0wcS1wc right\
   left m_mapPSYNS0cS1wc middle m_mapPSYNS0cS1wc right\
   left m_mapPSYNS0wcS1c middle m_mapPSYNS0wcS1c right\
   left m_mapPSYNS0cS1c middle m_mapPSYNS0cS1c right\
   left m_mapPSYNS0wcN0w middle m_mapPSYNS0wcN0w right\
   left m_mapPSYNS0cN0w middle m_mapPSYNS0cN0w right\
   left m_mapPSYNS0wN0t middle m_mapPSYNS0wN0t right\
   left m_mapPSYNS0cN0t middle m_mapPSYNS0cN0t right\
   left m_mapPSYNS1wcN0w middle m_mapPSYNS1wcN0w right\
   left m_mapPSYNS1cN0w middle m_mapPSYNS1cN0w right\
   left m_mapPSYNS1wN0t middle m_mapPSYNS1wN0t right\
   left m_mapPSYNS1cN0t middle m_mapPSYNS1cN0t right\
   left m_mapSYNN0t middle m_mapSYNN0t right\
   left m_mapSYNN1t middle m_mapSYNN1t right\
   left m_mapSYNN2t middle m_mapSYNN2t right\
   left m_mapSYNN3t middle m_mapSYNN3t right\
   left m_mapSYNS0wN0t middle m_mapSYNS0wN0t right\
   left m_mapSYNS0cN0t middle m_mapSYNS0cN0t right\
   left m_mapSYNS1wN0t middle m_mapSYNS1wN0t right\
   left m_mapSYNS1cN0t middle m_mapSYNS1cN0t right\
   left m_mapSYNN0tN1w middle m_mapSYNN0tN1w right\
   left m_mapSYNN0wN1t middle m_mapSYNN0wN1t right\
   left m_mapSYNN0tN1t middle m_mapSYNN0tN1t right\
   left m_mapSYNS0wS1cN0t middle m_mapSYNS0wS1cN0t right\
   left m_mapSYNS0cS1wN0t middle m_mapSYNS0cS1wN0t right\
   left m_mapSYNS0cS1cN0t middle m_mapSYNS0cS1cN0t right\
   left m_mapSYNS0cS1tN0t middle m_mapSYNS0cS1tN0t right\
   left m_mapSYNS0wcN0tN1t middle m_mapSYNS0wcN0tN1t right\
   left m_mapSYNS0cN0wtN1t middle m_mapSYNS0cN0wtN1t right\
   left m_mapSYNS0cN0tN1wt middle m_mapSYNS0cN0tN1wt right\
   left m_mapSYNS0cN0tN1t middle m_mapSYNS0cN0tN1t right\
   left m_mapSYNS0tN0tN1t middle m_mapSYNS0tN0tN1t right\
   left m_mapPSYNS0z middle m_mapPSYNS0z right\
   left m_mapPSYNS0zc middle m_mapPSYNS0zc right\
   left m_mapPSYNS0CharTagCont middle m_mapPSYNS0CharTagCont right\
   left m_mapPSYNS1z middle m_mapPSYNS1z right\
   left m_mapPSYNS1zc middle m_mapPSYNS1zc right\
   left m_mapPSYNS1CharTagCont middle m_mapPSYNS1CharTagCont right\
   left m_mapPSYNS2zc middle m_mapPSYNS2zc right\
   left m_mapPSYNS2CharTagCont middle m_mapPSYNS2CharTagCont right\
   left m_mapPSYNS3zc middle m_mapPSYNS3zc right\
   left m_mapPSYNS3CharTagCont middle m_mapPSYNS3CharTagCont right\
   left m_mapPPSYNS0ztS1zt middle m_mapPPSYNS0ztS1zt right\
   left m_mapPPSYNS0zS1z middle m_mapPPSYNS0zS1z right\
   left m_mapPPSYNS0zcS1zc middle m_mapPPSYNS0zcS1zc right\
   left m_mapPPSYNS0cS1zc middle m_mapPPSYNS0cS1zc right\
   left m_mapPPSYNS0zcS1c middle m_mapPPSYNS0zcS1c right\
   left m_mapPSYNS0ztS1zt middle m_mapPSYNS0ztS1zt right\
   left m_mapPSYNS0zS1z middle m_mapPSYNS0zS1z right\
   left m_mapPSYNS0zcS1zc middle m_mapPSYNS0zcS1zc right\
   left m_mapPSYNS0cS1zc middle m_mapPSYNS0cS1zc right\
   left m_mapPSYNS0zcS1c middle m_mapPSYNS0zcS1c right\
   left m_mapPSYNS0zN0z middle m_mapPSYNS0zN0z right\
   left m_mapPSYNS0zcN0z middle m_mapPSYNS0zcN0z right\
   left m_mapPSYNS0zN01z middle m_mapPSYNS0zN01z right\
   left m_mapPSYNS0zN012z middle m_mapPSYNS0zN012z right\
   left m_mapPSYNS1zN0z middle m_mapPSYNS1zN0z right\
   left m_mapPSYNS1zcN0z middle m_mapPSYNS1zcN0z right\
   left m_mapPSYNS1zN01z middle m_mapPSYNS1zN01z right\
   left m_mapPSYNS1zN012z middle m_mapPSYNS1zN012z right\
   left m_mapPPSYNS1zS0zN0z middle m_mapPPSYNS1zS0zN0z right\
   left m_mapPPSYNS1zS0zN01z middle m_mapPPSYNS1zS0zN01z right\
   left m_mapPSYNS1zS0zN0z middle m_mapPSYNS1zS0zN0z right\
   left m_mapPSYNS1zS0zN01z middle m_mapPSYNS1zS0zN01z right\ 
	left m_mapWSLengthByLastWord middle m_mapWSLengthByLastWord right\
	left m_mapWSLastLengthByWord middle m_mapWSLastLengthByWord right\
	left m_mapWSSeparateChars middle m_mapWSSeparateChars right\
	left m_mapWSFirstCharLastWordByWord middle m_mapWSFirstCharLastWordByWord right\
	left m_mapWSLastWordByLastChar middle m_mapWSLastWordByLastChar right\
   left m_mapLastTagByTag middle m_mapLastTagByTag right\
   left m_mapTag0Tag1Size1 middle m_mapTag0Tag1Size1 right\
   left m_mapTag0Tag1Tag2Size1 middle m_mapTag0Tag1Tag2Size1 right\
   left m_mapTagByLastWord middle m_mapTagByLastWord right\
   left m_mapLastTwoTagsByTag middle m_mapLastTwoTagsByTag right\
   left m_mapTagByFirstChar middle m_mapTagByFirstChar right\
   left m_mapTagByFirstCharCat middle m_mapTagByFirstCharCat right\
   left m_mapFirstCharBy2Tags middle m_mapFirstCharBy2Tags right\
   left m_mapFirstCharBy3Tags middle m_mapFirstCharBy3Tags right\
   left m_mapTagByChar middle m_mapTagByChar right\
   left m_mapTaggedSeparateChars middle m_mapTaggedSeparateChars right\
   left m_mapTagWordTag middle m_mapTagWordTag right\
   left m_mapWordTagTag middle m_mapWordTagTag right\
   left m_mapSeenWords middle m_mapSeenWords right\
   left m_mapLastWordByWord middle m_mapLastWordByWord right\
   left m_mapOneCharWord middle m_mapOneCharWord right\
   left m_mapFirstAndLastChars middle m_mapFirstAndLastChars right\
   left m_mapLengthByFirstChar middle m_mapLengthByFirstChar right\
   left m_mapLengthByLastChar middle m_mapLengthByLastChar right\
   left m_mapLeftPartWordTag middle m_mapLeftPartWordTag right\
   left m_mapRightPartWordTag middle m_mapRightPartWordTag right\
   left m_mapCurrentWordLastChar middle m_mapCurrentWordLastChar right\
   left m_mapLastWordByLastChar middle m_mapLastWordByLastChar right\
   left m_mapLengthByLastWord middle m_mapLengthByLastWord right\
   left m_mapLastLengthByWord middle m_mapLastLengthByWord right\
   left m_mapCurrentTag middle m_mapCurrentTag right\
   left m_mapLastTagByWord middle m_mapLastTagByWord right\
   left m_mapTagByWordAndPrevChar middle m_mapTagByWordAndPrevChar right\
   left m_mapTagOfOneCharWord middle m_mapTagOfOneCharWord right\
   left m_mapTagByLastChar middle m_mapTagByLastChar right\
   left m_mapTagByLastCharCat middle m_mapTagByLastCharCat right\
   left m_mapTaggedCharByLastChar middle m_mapTaggedCharByLastChar right\
   left m_mapTag1Tag2Size1 middle m_mapTag1Tag2Size1 right\
   left m_mapSeparateChars middle m_mapSeparateChars right\
   left m_mapLastWordFirstChar middle m_mapLastWordFirstChar right\
   left m_mapFirstCharLastWordByWord middle m_mapFirstCharLastWordByWord right\
   left m_mapTagByWordAndNextChar middle m_mapTagByWordAndNextChar right\
   left m_mapAPPTagByChar middle m_mapAPPTagByChar right\
   left m_mapTaggedCharByFirstChar middle m_mapTaggedCharByFirstChar right\
   left m_mapConsecutiveChars middle m_mapConsecutiveChars right\
   left m_mapTaggedConsecutiveChars middle m_mapTaggedConsecutiveChars right\
   left m_mapSSYNTaggedSeparateChars middle m_mapSSYNTaggedSeparateChars right\
   left m_mapSSYNS0fzcurz middle m_mapSSYNS0fzcurz right\
   left m_mapSSYNS0lzcurz middle m_mapSSYNS0lzcurz right\
   left m_mapSSYNS1fzcurw middle m_mapSSYNS1fzcurw right\
   left m_mapSSYNS1lzcurw middle m_mapSSYNS1lzcurw right\
   left m_mapSSYNS1lzcurwlz middle m_mapSSYNS1lzcurwlz right\
   left m_mapSSYNS1fzcurwt middle m_mapSSYNS1fzcurwt right\
   left m_mapSSYNS1lzcurwt middle m_mapSSYNS1lzcurwt right\
   left m_mapSSYNS1lzcurwlzt middle m_mapSSYNS1lzcurwlzt right\
   left m_mapPSYNS1lzcurwlz middle m_mapPSYNS1lzcurwlz right\
   left m_mapInSubWordDictionary middle m_mapInSubWordDictionary right\
	left m_mapInSubWordTagDictionary middle m_mapInSubWordTagDictionary right\
	left m_mapInWordDictionary middle m_mapInWordDictionary right\
	left m_mapInWordTagDictionary middle m_mapInWordTagDictionary right\
	left m_mapPSYNS2z middle m_mapPSYNS2z right\
	left m_mapSYNS2z middle m_mapSYNS2z right\
	left m_mapPSYNS0zS1zS2z middle m_mapPSYNS0zS1zS2z right\
	left m_mapPPSYNS0zS1zS2z middle m_mapPPSYNS0zS1zS2z right\
	left m_mapPPPSYNS0zS1zS2z middle m_mapPPPSYNS0zS1zS2z right\
	left m_mapPSYNS0zS1cS2c middle m_mapPSYNS0zS1cS2c right\
	left m_mapPSYNS0cS1zS2c middle m_mapPSYNS0cS1zS2c right\
	left m_mapPSYNS0cS1cS2z middle m_mapPSYNS0cS1cS2z right\
	left m_mapPPSYNS0zS1cS2c middle m_mapPPSYNS0zS1cS2c right\
	left m_mapPPSYNS0cS1zS2c middle m_mapPPSYNS0cS1zS2c right\
	left m_mapPPSYNS0cS1cS2z middle m_mapPPSYNS0cS1cS2z right\
	left m_mapPPPSYNS0zS1cS2c middle m_mapPPPSYNS0zS1cS2c right\
	left m_mapPPPSYNS0cS1zS2c middle m_mapPPPSYNS0cS1zS2c right\
	left m_mapPPPSYNS0cS1cS2z middle m_mapPPPSYNS0cS1cS2z right\
	left m_mapSubWordHeadCharType middle m_mapSubWordHeadCharType right\
	left m_mapSubWordHeadCharTypeTag middle m_mapSubWordHeadCharTypeTag right\
	left m_mapSubWordHeadCharEqual middle m_mapSubWordHeadCharEqual right\
	left m_mapSubWordHeadCharEqualTag middle m_mapSubWordHeadCharEqualTag right\
	left m_mapInSubWordDictionaryTag middle m_mapInSubWordDictionaryTag right\
	left m_mapInSubWordTagDictionaryTag middle m_mapInSubWordTagDictionaryTag right\
	left m_mapSYNRightMostc middle m_mapSYNRightMostc right\
	left m_mapSYNRightMostcWord middle m_mapSYNRightMostcWord right
   



namespace TARGET_LANGUAGE {

namespace conparser {

//#include "conparser_macros.h"

//
// TYPE DEFINITIONS
//
typedef CScoreMapType<CWord, SCORE_TYPE, CAction::MAX> CWordMap;
typedef CScoreMapType<CTaggedWord<CTag, TAG_SEPARATOR>, SCORE_TYPE, CAction::MAX> CTaggedWordMap;
typedef CScoreMapType<std::pair<CWord, unsigned long>,  SCORE_TYPE, CAction::MAX> CWordIntMap;
typedef CScoreMapType<std::pair<CTag, unsigned long>,  SCORE_TYPE, CAction::MAX> CTagIntPairMap;
typedef CScoreMapType<std::pair<CConstituent, unsigned long>,  SCORE_TYPE, CAction::MAX> CConstituentIntMap;
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
//typedef CScoreMapType<std::pair<CCFGSet, unsigned long>, SCORE_TYPE, CAction::MAX> CCFGSetIntMap;
typedef CScoreMapType<std::pair<CCFGSet, CActionType>, SCORE_TYPE, CAction::MAX> CCFGSetActionTypeMap;
typedef CScoreMapType<CTuple2<CWord, CConstituent>, SCORE_TYPE, CAction::MAX> CWordConstituentMap;
typedef CScoreMapType<CTuple2<CTag, CConstituent>, SCORE_TYPE, CAction::MAX> CTagConstituentMap;
typedef CScoreMapType<CTuple3<CWord, CTag, unsigned long long>, SCORE_TYPE, CAction::MAX> CWordTagIntMap;
typedef CScoreMapType<CConstituent, SCORE_TYPE, CAction::MAX> CConstituentMap;
typedef CScoreMapType<CAction, SCORE_TYPE, CAction::MAX> CActionMap;
typedef CScoreMapType<CTuple2<CAction, CAction>, SCORE_TYPE, CAction::MAX> CAction2Map;

typedef CScoreMapType<CTuple2<CWord, CWord>, SCORE_TYPE, CAction::MAX> CBiWordMap;
typedef CScoreMapType<CTuple3<CWord, CWord, CWord>, SCORE_TYPE, CAction::MAX> CTriWordMap;
typedef CScoreMapType<CTuple4<CWord, CWord, CWord, CWord>, SCORE_TYPE, CAction::MAX> CFourWordMap;
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
typedef CScoreMapType< CTuple2<CWord, unsigned long long>, SCORE_TYPE , CAction::MAX> CWord2IntMap;



//typedef CScoreMap< long int, SCORE_TYPE > CIntMap;

typedef CScoreMapType< std::pair<CWord, CTag>, SCORE_TYPE , CAction::MAX > CWordTagPairMap;
typedef CScoreMapType< std::pair<unsigned long long, CTag>, SCORE_TYPE , CAction::MAX> CIntTagMap;
typedef CScoreMapType< std::pair<unsigned long long, unsigned long long>, SCORE_TYPE , CAction::MAX> CIntIntPairMap;
typedef CScoreMapType< CTagSet<CTag, 2>, SCORE_TYPE , CAction::MAX> CTagSet2Map;
typedef CScoreMapType< CTagSet<CTag, 3>, SCORE_TYPE , CAction::MAX> CTagSet3Map;
typedef CScoreMapType< std::pair<CWord, CTagSet<CTag, 2> >, SCORE_TYPE , CAction::MAX> CWordTagSet2Map;
typedef CScoreMapType< std::pair<CWord, CTagSet<CTag, 3> >, SCORE_TYPE , CAction::MAX> CWordTagSet3Map;
typedef CScoreMapType< std::pair<CTagSet<CTag, 2>, int>, SCORE_TYPE , CAction::MAX> CTagSet2IntMap;
typedef CScoreMapType< std::pair<CTagSet<CTag, 3>, int>, SCORE_TYPE , CAction::MAX> CTagSet3IntMap;
//typedef CScoreMapType< std::pair<CWord, CTagSet<CTag, 3> >, SCORE_TYPE , CAction::MAX> CWordTagSet3Map;

typedef CScoreMapType<CTuple3<CWord, CTag, CConstituent>, SCORE_TYPE, CAction::MAX> CWordTagConstituentMap;
typedef CScoreMapType<CTuple4<CWord, CWord, CTag, CConstituent>, SCORE_TYPE, CAction::MAX> CWordWordTagConstituentMap;
typedef CScoreMapType<CTuple4<CWord, CWord, CWord, CTag>, SCORE_TYPE, CAction::MAX> CTriWordTagMap;
typedef CScoreMapType<CTuple4<CWord, CWord, CWord, CConstituent>, SCORE_TYPE, CAction::MAX> CTriWordConstituentMap;

typedef CScoreMapType<CTuple4<CWord, CWord, unsigned long long, unsigned long long>, SCORE_TYPE, CAction::MAX> CTwoWordsTwoIntsMap;
typedef CScoreMapType<CTuple4<CWord, CWord, CConstituent, unsigned long long>, SCORE_TYPE, CAction::MAX> CWordWordConstituentIntMap;
typedef CScoreMapType<CTuple3<CWord, CConstituent, unsigned long long>, SCORE_TYPE, CAction::MAX> CWordConstituentIntMap;
typedef CScoreMapType<CTuple2<CConstituent, unsigned long long>, SCORE_TYPE, CAction::MAX> CConstituentIntTupleMap;
typedef CScoreMapType<CTuple3<CWord, CWord, unsigned long long>, SCORE_TYPE, CAction::MAX> CTwoWordsIntMap;

typedef CScoreMapType<CTuple2<CCFGSet, unsigned long long>, SCORE_TYPE, CAction::MAX> CCFGIntMap;
typedef CScoreMapType<CTuple3<CTag, unsigned long long, unsigned long long>, SCORE_TYPE, CAction::MAX> CTagTwoIntsMap;
typedef CScoreMapType<CTuple3<CConstituent, unsigned long long, unsigned long long>, SCORE_TYPE, CAction::MAX> CConstituentTwoIntsMap;
typedef CScoreMapType<CTuple4<CWord, CConstituent, unsigned long long, unsigned long long>, SCORE_TYPE, CAction::MAX> CWordConstituentTwoIntsMap;
typedef CScoreMapType<CTuple3<CWord, CCFGSet, unsigned long long>, SCORE_TYPE, CAction::MAX> CWordCfgIntMap;
typedef CScoreMapType<CTuple3<CConstituent, CTag, unsigned long long>, SCORE_TYPE, CAction::MAX> CConstituentTagIntMap;
typedef CScoreMapType<CTuple4<CWord, CWord, unsigned long long, unsigned long long>, SCORE_TYPE, CAction::MAX> CWordWordTwoIntsMap;
typedef CScoreMapType<CTuple3<CWord, unsigned long long, unsigned long long>, SCORE_TYPE, CAction::MAX> CWordTwoIntsMap;
typedef CScoreMapType<CTuple4<CTag, CTag, unsigned long long, unsigned long long>, SCORE_TYPE, CAction::MAX> CTagTagTwoIntsMap;
typedef CScoreMapType<CTuple2<unsigned long long, unsigned long long>, SCORE_TYPE, CAction::MAX> CTwoIntsMap;


/*===============================================================
 *
 * CWeight - the definition of weights, in feature
 *
 *==============================================================*/

class CWeight : public CWeightBase {

public:
   CTaggedWordMap m_mapSYNS0w;
   CConstituentMap m_mapSYNS0c;
   CTagConstituentMap m_mapSYNS0tc;
   CWordConstituentMap m_mapSYNS0wc;
   CTaggedWordMap m_mapSYNS1w;
   CConstituentMap m_mapSYNS1c;
   CTagConstituentMap m_mapSYNS1tc;
   CWordConstituentMap m_mapSYNS1wc;
   CTagConstituentMap m_mapSYNS2tc;
   CWordConstituentMap m_mapSYNS2wc;
   CTagConstituentMap m_mapSYNS3tc;
   CWordConstituentMap m_mapSYNS3wc;
   CWordMap m_mapSYNN0w;
   CWordMap m_mapSYNN1w;
   CBiWordMap m_mapSYNN01w;
   CWordMap m_mapSYNN2w;
   CBiWordMap m_mapSYNN12w;
   CTriWordMap m_mapSYNN012w;
   CWordMap m_mapSYNN3w;
   CBiWordMap m_mapSYNN23w;
   CTriWordMap m_mapSYNN123w;
   CTagConstituentMap m_mapSYNS0Ltc;
   CWordConstituentMap m_mapSYNS0Lwc;
   CTagConstituentMap m_mapSYNS0Rtc;
   CWordConstituentMap m_mapSYNS0Rwc;
   CTagConstituentMap m_mapSYNS0Utc;
   CWordConstituentMap m_mapSYNS0Uwc;
   CTagConstituentMap m_mapSYNS1Ltc;
   CWordConstituentMap m_mapSYNS1Lwc;
   CTagConstituentMap m_mapSYNS1Rtc;
   CWordConstituentMap m_mapSYNS1Rwc;
   CTagConstituentMap m_mapSYNS1Utc;
   CWordConstituentMap m_mapSYNS1Uwc;
   CTwoWordsCFGSetMap m_mapSYNS0wcS1wc;
   CWordCFGSetMap m_mapSYNS0cS1w;
   CWordCFGSetMap m_mapSYNS0wS1c;
   CCFGSetMap m_mapSYNS0cS1c;
   CWordWordConstituentMap m_mapSYNS0wcN0w;
   CWordConstituentMap m_mapSYNS0cN0w;
   CWordWordConstituentMap m_mapSYNS1wcN0w;
   CWordConstituentMap m_mapSYNS1cN0w;
   CWordCFGSetMap m_mapSYNS0cS1cN0w;
   CWordCFGSetMap m_mapSYNS0wS1cS2c;
   CWordCFGSetMap m_mapSYNS0cS1wS2c;
   CWordCFGSetMap m_mapSYNS0cS1cS2w;
   CCFGSetMap m_mapSYNS0cS1cS2c;
   CCFGSetMap m_mapSYNS0tS1tS2t;
   CWordCFGSetMap m_mapSYNS0cS0RcN0w;
   CCFGSetMap m_mapSYNS0cS0LcS1c;
   CCFGSetMap m_mapSYNS0cS0LjS1j;
   CWordCFGSetMap m_mapSYNS0cS0LcS1w;
   CCFGSetMap m_mapSYNS0cS1cS1Rc;
   CCFGSetMap m_mapSYNS0jS1cS1Rj;
   CWordCFGSetMap m_mapSYNS0wS1cS1Rc;
   CTaggedWordMap m_mapSYNS0z;
   CWordConstituentMap m_mapSYNS0zc;
   CWordTagConstituentMap m_mapSYNS0CharTagCont;
   CTaggedWordMap m_mapSYNS1z;
   CWordConstituentMap m_mapSYNS1zc;
   CWordTagConstituentMap m_mapSYNS1CharTagCont;
   CTaggedWordMap m_mapSYNS2CharTag;
   CWordConstituentMap m_mapSYNS2zc;
   CWordTagConstituentMap m_mapSYNS2CharTagCont;
   CTaggedWordMap m_mapSYNS3CharTag;
   CWordConstituentMap m_mapSYNS3zc;
   CWordTagConstituentMap m_mapSYNS3CharTagCont;
   CTwoTaggedWordsMap m_mapSYNS0ztS1zt;
   CBiWordMap m_mapSYNS0zS1z;
   CTwoWordsCFGSetMap m_mapSYNS0zcS1zc;
   CWordCFGSetMap m_mapSYNS0cS1zc;
   CWordCFGSetMap m_mapSYNS0zcS1c;
   CTriWordMap m_mapSYNS0zS1zS2z;
   CWordCFGSetMap m_mapSYNS0zS1cS2c;
   CWordCFGSetMap m_mapSYNS0cS1zS2c;
   CWordCFGSetMap m_mapSYNS0cS1cS2z;
   CTriWordMap m_mapSYNS1zS2zS3z;
   CFourWordMap m_mapSYNS0zS1zS2zS3z;
   CBiWordMap m_mapSYNS0zN0z;
   CWordWordConstituentMap m_mapSYNS0zcN0z;
   CTriWordMap m_mapSYNS0zN01z;
   CFourWordMap m_mapSYNS0zN012z;
   CBiWordMap m_mapSYNS1zN0z;
   CWordWordConstituentMap m_mapSYNS1zcN0z;
   CTriWordMap m_mapSYNS1zN01z;
   CFourWordMap m_mapSYNS1zN012z;
   CTriWordMap m_mapSYNS1zS0zN0z;
   CFourWordMap m_mapSYNS1zS0zN01z;
   CTaggedWordMap m_mapPSYNS0w;
   CIntMap m_mapPSYNS0c;
   CTagIntPairMap m_mapPSYNS0tc;
   CWordIntMap m_mapPSYNS0wc;
   CTaggedWordMap m_mapPSYNS1w;
   CIntMap m_mapPSYNS1c;
   CTagIntPairMap m_mapPSYNS1tc;
   CWordIntMap m_mapPSYNS1wc;
   CTagIntPairMap m_mapPSYNS2tc;
   CWordIntMap m_mapPSYNS2wc;
   CTagIntPairMap m_mapPSYNS3tc;
   CWordIntMap m_mapPSYNS3wc;
   CTagIntPairMap m_mapPSYNS0Ltc;
   CWordIntMap m_mapPSYNS0Lwc;
   CTagIntPairMap m_mapPSYNS0Rtc;
   CWordIntMap m_mapPSYNS0Rwc;
   CTagIntPairMap m_mapPSYNS0Utc;
   CWordIntMap m_mapPSYNS0Uwc;
   CTagIntPairMap m_mapPSYNS1Ltc;
   CWordIntMap m_mapPSYNS1Lwc;
   CTagIntPairMap m_mapPSYNS1Rtc;
   CWordIntMap m_mapPSYNS1Rwc;
   CTagIntPairMap m_mapPSYNS1Utc;
   CWordIntMap m_mapPSYNS1Uwc;
   CWordWordTwoIntsMap m_mapPPSYNS0wcS1wc;
   CWordTwoIntsMap m_mapPPSYNS0cS1wc;
   CWordTwoIntsMap m_mapPPSYNS0wcS1c;
   CTwoIntsMap m_mapPPSYNS0cS1c;
   CWordWordConstituentIntMap m_mapPSYNS0wcS1wc;
   CWordConstituentIntMap m_mapPSYNS0cS1wc;
   CWordConstituentIntMap m_mapPSYNS0wcS1c;
   CConstituentIntTupleMap m_mapPSYNS0cS1c;
   CTwoWordsIntMap m_mapPSYNS0wcN0w;
   CWordIntMap m_mapPSYNS0cN0w;
   CWordIntMap m_mapPSYNS0wN0t;
   CIntIntPairMap m_mapPSYNS0cN0t;
   CTwoWordsIntMap m_mapPSYNS1wcN0w;
   CWordIntMap m_mapPSYNS1cN0w;
   CWordIntMap m_mapPSYNS1wN0t;
   CIntIntPairMap m_mapPSYNS1cN0t;
   CIntMap m_mapSYNN0t;
   CIntMap m_mapSYNN1t;
   CIntMap m_mapSYNN2t;
   CIntMap m_mapSYNN3t;
   CWordIntMap m_mapSYNS0wN0t;
   CConstituentIntMap m_mapSYNS0cN0t;
   CWordIntMap m_mapSYNS1wN0t;
   CConstituentIntMap m_mapSYNS1cN0t;
   CWordIntMap m_mapSYNN0tN1w;
   CWordIntMap m_mapSYNN0wN1t;
   CTwoIntMap m_mapSYNN0tN1t;
   CWordCfgIntMap m_mapSYNS0wS1cN0t;
   CWordCfgIntMap m_mapSYNS0cS1wN0t;
   CCFGIntMap m_mapSYNS0cS1cN0t;
   CConstituentTagIntMap m_mapSYNS0cS1tN0t;
   CWordConstituentTwoIntsMap m_mapSYNS0wcN0tN1t;
   CWordConstituentTwoIntsMap m_mapSYNS0cN0wtN1t;
   CWordConstituentTwoIntsMap m_mapSYNS0cN0tN1wt;
   CConstituentTwoIntsMap m_mapSYNS0cN0tN1t;
   CTagTwoIntsMap m_mapSYNS0tN0tN1t;
   CTaggedWordMap m_mapPSYNS0z;
   CWordIntMap m_mapPSYNS0zc;
   CWordTagIntMap m_mapPSYNS0CharTagCont;
   CTaggedWordMap m_mapPSYNS1z;
   CWordIntMap m_mapPSYNS1zc;
   CWordTagIntMap m_mapPSYNS1CharTagCont;
   CWordIntMap m_mapPSYNS2zc;
   CWordTagIntMap m_mapPSYNS2CharTagCont;
   CWordIntMap m_mapPSYNS3zc;
   CWordTagIntMap m_mapPSYNS3CharTagCont;
   CTwoTaggedWordsMap m_mapPPSYNS0ztS1zt;
   CBiWordMap m_mapPPSYNS0zS1z;
   CWordWordTwoIntsMap m_mapPPSYNS0zcS1zc;
   CWordTwoIntsMap m_mapPPSYNS0cS1zc;
   CWordTwoIntsMap m_mapPPSYNS0zcS1c;
   CTwoTaggedWordsMap m_mapPSYNS0ztS1zt;
   CBiWordMap m_mapPSYNS0zS1z;
   CWordWordConstituentIntMap m_mapPSYNS0zcS1zc;
   CWordConstituentIntMap m_mapPSYNS0cS1zc;
   CWordConstituentIntMap m_mapPSYNS0zcS1c;
   CBiWordMap m_mapPSYNS0zN0z;
   CTwoWordsIntMap m_mapPSYNS0zcN0z;
   CTriWordMap m_mapPSYNS0zN01z;
   CFourWordMap m_mapPSYNS0zN012z;
   CBiWordMap m_mapPSYNS1zN0z;
   CTwoWordsIntMap m_mapPSYNS1zcN0z;
   CTriWordMap m_mapPSYNS1zN01z;
   CFourWordMap m_mapPSYNS1zN012z;
   CTriWordMap m_mapPPSYNS1zS0zN0z;
   CFourWordMap m_mapPPSYNS1zS0zN01z;
   CTriWordMap m_mapPSYNS1zS0zN0z;
   CFourWordMap m_mapPSYNS1zS0zN01z;
   CTwoWordsMap m_mapWSFirstCharLastWordByWord;
   CTwoWordsMap m_mapWSLastWordByLastChar;
   CWordIntMap m_mapWSLengthByLastWord;
   CWordIntMap m_mapWSLastLengthByWord;
   CWordMap m_mapWSSeparateChars;
   CBiTagMap m_mapLastTagByTag;
   CBiTagIntMap m_mapTag0Tag1Size1;
   CTriTagIntMap m_mapTag0Tag1Tag2Size1;
   CWordTagMap m_mapTagByLastWord;
   CTriTagMap m_mapLastTwoTagsByTag;
   CWordTagMap m_mapTagByFirstChar;
   CTagIntMap m_mapTagByFirstCharCat;
   CWordTagTagMap m_mapFirstCharBy2Tags;
   CWordTriTagMap m_mapFirstCharBy3Tags;
   CWordTagMap m_mapTagByChar;
   CTwoTaggedWordsMap m_mapTaggedSeparateChars;
   CWordTagTagMap m_mapTagWordTag;
   CWordTagTagMap m_mapWordTagTag;
   CWordMap m_mapSeenWords;
   CTwoWordsMap m_mapLastWordByWord;
   CWordMap m_mapOneCharWord;
   CTwoWordsMap m_mapFirstAndLastChars;
   CWordIntMap m_mapLengthByFirstChar;
   CWordIntMap m_mapLengthByLastChar;
   CWordTagPairMap m_mapLeftPartWordTag;
   CWordTagPairMap m_mapRightPartWordTag;
   CWordMap m_mapCurrentWordLastChar;
   CTwoWordsMap m_mapLastWordByLastChar;
   CWordIntMap m_mapLengthByLastWord;
   CWordIntMap m_mapLastLengthByWord;
   CWordTagPairMap m_mapCurrentTag;
   CWordTagPairMap m_mapLastTagByWord;
   CWordTagPairMap m_mapTagByWordAndPrevChar;
   CWordTagPairMap m_mapTagOfOneCharWord;
   CWordTagPairMap m_mapTagByLastChar;
   CIntTagMap m_mapTagByLastCharCat;
   CWordWordTagMap m_mapTaggedCharByLastChar;
   CTagSet2IntMap m_mapTag1Tag2Size1;
   CWordMap m_mapSeparateChars;
   CWordMap m_mapLastWordFirstChar;
   CTwoWordsMap m_mapFirstCharLastWordByWord;
   CWordTagPairMap m_mapTagByWordAndNextChar;
   CWordTagMap m_mapAPPTagByChar;
   CWordWordTagMap m_mapTaggedCharByFirstChar;
   CWordMap m_mapConsecutiveChars;
   CWordTagPairMap m_mapTaggedConsecutiveChars;

   CTwoTaggedWordsMap m_mapSSYNTaggedSeparateChars;
   CBiWordMap m_mapSSYNS0fzcurz;
   CBiWordMap m_mapSSYNS0lzcurz;
   CBiWordMap m_mapSSYNS1fzcurw;
   CBiWordMap m_mapSSYNS1lzcurw;
   CBiWordMap m_mapSSYNS1lzcurwlz;
   CWordWordTagMap m_mapSSYNS1fzcurwt;
   CWordWordTagMap m_mapSSYNS1lzcurwt;
   CWordWordTagMap m_mapSSYNS1lzcurwlzt;
   CBiWordMap m_mapPSYNS1lzcurwlz;


   CIntMap m_mapInSubWordDictionary;
   CIntMap m_mapInSubWordTagDictionary;
   CIntMap m_mapInWordDictionary;
   CIntMap m_mapInWordTagDictionary;


   CTaggedWordMap m_mapPSYNS2z;
   CTaggedWordMap m_mapSYNS2z;
   CTriWordMap m_mapPSYNS0zS1zS2z;
   CTriWordMap m_mapPPSYNS0zS1zS2z;
   CTriWordMap m_mapPPPSYNS0zS1zS2z;
   CWordCfgIntMap m_mapPSYNS0zS1cS2c;
   CWordCfgIntMap m_mapPSYNS0cS1zS2c;
   CWordCfgIntMap m_mapPSYNS0cS1cS2z;
   CWordCfgIntMap m_mapPPSYNS0zS1cS2c;
   CWordCfgIntMap m_mapPPSYNS0cS1zS2c;
   CWordCfgIntMap m_mapPPSYNS0cS1cS2z;
   CWordCfgIntMap m_mapPPPSYNS0zS1cS2c;
   CWordCfgIntMap m_mapPPPSYNS0cS1zS2c;
   CWordCfgIntMap m_mapPPPSYNS0cS1cS2z;

   CIntMap m_mapSubWordHeadCharType;
   CTagIntMap m_mapSubWordHeadCharTypeTag;
   CIntMap m_mapSubWordHeadCharEqual;
   CTagIntMap m_mapSubWordHeadCharEqualTag;
   CTagIntMap m_mapInSubWordDictionaryTag;
   CTagIntMap m_mapInSubWordTagDictionaryTag;


   //new 2013.0706
   CConstituentMap m_mapSYNRightMostc;
   CWordConstituentMap m_mapSYNRightMostcWord;



   // Rule dictionary
   unsigned long m_nMaxWordFrequency;
   CWordToIntMap m_mapWordFrequency;
   CWordToIntMap m_mapPartWordFrequency;

   CTagDict<CWord, CTag> m_mapTagDictionary;
   CTagDict<CWord, CTag> m_mapCharTagDictionary;
   CTagDict<CWord, CTag> m_mapCanStart;
   unsigned long m_maxlengthByTag[CTag::COUNT+1];
   CCharCatDictionary *m_Knowledge;
   CWordDictionary m_mapWordHeadDictionary;
   CTagDict<CWord, CTag> m_mapSubTagDictionary;

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
                          m_mapPartWordFrequency(65537),
                          m_mapTagDictionary(CTag::COUNT),
                          m_mapSubTagDictionary(CTag::COUNT),
                          m_mapCharTagDictionary(CTag::COUNT),
                          m_mapCanStart(CTag::COUNT),
                          m_mapWordHeadDictionary(65537),

                           m_mapSYNS0w("SYNS0w", TABLE_SIZE),	
                           m_mapSYNS0c("SYNS0c", TABLE_SIZE),	
                           m_mapSYNS0tc("SYNS0tc", TABLE_SIZE),	
                           m_mapSYNS0wc("SYNS0wc", TABLE_SIZE),	
                           m_mapSYNS1w("SYNS1w", TABLE_SIZE),	
                           m_mapSYNS1c("SYNS1c", TABLE_SIZE),	
                           m_mapSYNS1tc("SYNS1tc", TABLE_SIZE),	
                           m_mapSYNS1wc("SYNS1wc", TABLE_SIZE),	
                           m_mapSYNS2tc("SYNS2tc", TABLE_SIZE),	
                           m_mapSYNS2wc("SYNS2wc", TABLE_SIZE),	
                           m_mapSYNS3tc("SYNS3tc", TABLE_SIZE),	
                           m_mapSYNS3wc("SYNS3wc", TABLE_SIZE),	
                           m_mapSYNN0w("SYNN0w", TABLE_SIZE),	
                           m_mapSYNN1w("SYNN1w", TABLE_SIZE),	
                           m_mapSYNN01w("SYNN01w", TABLE_SIZE),	
                           m_mapSYNN2w("SYNN2w", TABLE_SIZE),	
                           m_mapSYNN12w("SYNN12w", TABLE_SIZE),	
                           m_mapSYNN012w("SYNN012w", TABLE_SIZE),	
                           m_mapSYNN3w("SYNN3w", TABLE_SIZE),	
                           m_mapSYNN23w("SYNN23w", TABLE_SIZE),	
                           m_mapSYNN123w("SYNN123w", TABLE_SIZE),	
                           m_mapSYNS0Ltc("SYNS0Ltc", TABLE_SIZE),	
                           m_mapSYNS0Lwc("SYNS0Lwc", TABLE_SIZE),	
                           m_mapSYNS0Rtc("SYNS0Rtc", TABLE_SIZE),	
                           m_mapSYNS0Rwc("SYNS0Rwc", TABLE_SIZE),	
                           m_mapSYNS0Utc("SYNS0Utc", TABLE_SIZE),	
                           m_mapSYNS0Uwc("SYNS0Uwc", TABLE_SIZE),	
                           m_mapSYNS1Ltc("SYNS1Ltc", TABLE_SIZE),	
                           m_mapSYNS1Lwc("SYNS1Lwc", TABLE_SIZE),	
                           m_mapSYNS1Rtc("SYNS1Rtc", TABLE_SIZE),	
                           m_mapSYNS1Rwc("SYNS1Rwc", TABLE_SIZE),	
                           m_mapSYNS1Utc("SYNS1Utc", TABLE_SIZE),	
                           m_mapSYNS1Uwc("SYNS1Uwc", TABLE_SIZE),	
                           m_mapSYNS0wcS1wc("SYNS0wcS1wc", TABLE_SIZE),	
                           m_mapSYNS0cS1w("SYNS0cS1w", TABLE_SIZE),	
                           m_mapSYNS0wS1c("SYNS0wS1c", TABLE_SIZE),	
                           m_mapSYNS0cS1c("SYNS0cS1c", TABLE_SIZE),	
                           m_mapSYNS0wcN0w("SYNS0wcN0w", TABLE_SIZE),	
                           m_mapSYNS0cN0w("SYNS0cN0w", TABLE_SIZE),	
                           m_mapSYNS1wcN0w("SYNS1wcN0w", TABLE_SIZE),	
                           m_mapSYNS1cN0w("SYNS1cN0w", TABLE_SIZE),	
                           m_mapSYNS0cS1cN0w("SYNS0cS1cN0w", TABLE_SIZE),	
                           m_mapSYNS0wS1cS2c("SYNS0wS1cS2c", TABLE_SIZE),	
                           m_mapSYNS0cS1wS2c("SYNS0cS1wS2c", TABLE_SIZE),	
                           m_mapSYNS0cS1cS2w("SYNS0cS1cS2w", TABLE_SIZE),	
                           m_mapSYNS0cS1cS2c("SYNS0cS1cS2c", TABLE_SIZE),	
                           m_mapSYNS0tS1tS2t("SYNS0tS1tS2t", TABLE_SIZE),	
                           m_mapSYNS0cS0RcN0w("SYNS0cS0RcN0w", TABLE_SIZE),	
                           m_mapSYNS0cS0LcS1c("SYNS0cS0LcS1c", TABLE_SIZE),	
                           m_mapSYNS0cS0LjS1j("SYNS0cS0LjS1j", TABLE_SIZE),	
                           m_mapSYNS0cS0LcS1w("SYNS0cS0LcS1w", TABLE_SIZE),	
                           m_mapSYNS0cS1cS1Rc("SYNS0cS1cS1Rc", TABLE_SIZE),	
                           m_mapSYNS0jS1cS1Rj("SYNS0jS1cS1Rj", TABLE_SIZE),	
                           m_mapSYNS0wS1cS1Rc("SYNS0wS1cS1Rc", TABLE_SIZE),	
                           m_mapSYNS0z("SYNS0z", TABLE_SIZE),	
                           m_mapSYNS0zc("SYNS0zc", TABLE_SIZE),	
                           m_mapSYNS0CharTagCont("SYNS0CharTagCont", TABLE_SIZE),	
                           m_mapSYNS1z("SYNS1z", TABLE_SIZE),	
                           m_mapSYNS1zc("SYNS1zc", TABLE_SIZE),	
                           m_mapSYNS1CharTagCont("SYNS1CharTagCont", TABLE_SIZE),	
                           m_mapSYNS2CharTag("SYNS2CharTag", TABLE_SIZE),	
                           m_mapSYNS2zc("SYNS2zc", TABLE_SIZE),	
                           m_mapSYNS2CharTagCont("SYNS2CharTagCont", TABLE_SIZE),	
                           m_mapSYNS3CharTag("SYNS3CharTag", TABLE_SIZE),	
                           m_mapSYNS3zc("SYNS3zc", TABLE_SIZE),	
                           m_mapSYNS3CharTagCont("SYNS3CharTagCont", TABLE_SIZE),	
                           m_mapSYNS0ztS1zt("SYNS0ztS1zt", TABLE_SIZE),	
                           m_mapSYNS0zS1z("SYNS0zS1z", TABLE_SIZE),	
                           m_mapSYNS0zcS1zc("SYNS0zcS1zc", TABLE_SIZE),	
                           m_mapSYNS0cS1zc("SYNS0cS1zc", TABLE_SIZE),	
                           m_mapSYNS0zcS1c("SYNS0zcS1c", TABLE_SIZE),	
                           m_mapSYNS0zS1zS2z("SYNS0zS1zS2z", TABLE_SIZE),	
                           m_mapSYNS0zS1cS2c("SYNS0zS1cS2c", TABLE_SIZE),	
                           m_mapSYNS0cS1zS2c("SYNS0cS1zS2c", TABLE_SIZE),	
                           m_mapSYNS0cS1cS2z("SYNS0cS1cS2z", TABLE_SIZE),	
                           m_mapSYNS1zS2zS3z("SYNS1zS2zS3z", TABLE_SIZE),	
                           m_mapSYNS0zS1zS2zS3z("SYNS0zS1zS2zS3z", TABLE_SIZE),	
                           m_mapSYNS0zN0z("SYNS0zN0z", TABLE_SIZE),	
                           m_mapSYNS0zcN0z("SYNS0zcN0z", TABLE_SIZE),	
                           m_mapSYNS0zN01z("SYNS0zN01z", TABLE_SIZE),	
                           m_mapSYNS0zN012z("SYNS0zN012z", TABLE_SIZE),	
                           m_mapSYNS1zN0z("SYNS1zN0z", TABLE_SIZE),	
                           m_mapSYNS1zcN0z("SYNS1zcN0z", TABLE_SIZE),
                           m_mapSYNS1zN01z("SYNS1zN01z", TABLE_SIZE),	
                           m_mapSYNS1zN012z("SYNS1zN012z", TABLE_SIZE),	
                           m_mapSYNS1zS0zN0z("SYNS1zS0zN0z", TABLE_SIZE),	
                           m_mapSYNS1zS0zN01z("SYNS1zS0zN01z", TABLE_SIZE),	
                           m_mapPSYNS0w("PSYNS0w", TABLE_SIZE),
                           m_mapPSYNS0c("PSYNS0c", TABLE_SIZE),
                           m_mapPSYNS0tc("PSYNS0tc", TABLE_SIZE),
                           m_mapPSYNS0wc("PSYNS0wc", TABLE_SIZE),
                           m_mapPSYNS1w("PSYNS1w", TABLE_SIZE),
                           m_mapPSYNS1c("PSYNS1c", TABLE_SIZE),
                           m_mapPSYNS1tc("PSYNS1tc", TABLE_SIZE),
                           m_mapPSYNS1wc("PSYNS1wc", TABLE_SIZE),
                           m_mapPSYNS2tc("PSYNS2tc", TABLE_SIZE),
                           m_mapPSYNS2wc("PSYNS2wc", TABLE_SIZE),
                           m_mapPSYNS3tc("PSYNS3tc", TABLE_SIZE),
                           m_mapPSYNS3wc("PSYNS3wc", TABLE_SIZE),
                           m_mapPSYNS0Ltc("PSYNS0Ltc", TABLE_SIZE),
                           m_mapPSYNS0Lwc("PSYNS0Lwc", TABLE_SIZE),
                           m_mapPSYNS0Rtc("PSYNS0Rtc", TABLE_SIZE),
                           m_mapPSYNS0Rwc("PSYNS0Rwc", TABLE_SIZE),
                           m_mapPSYNS0Utc("PSYNS0Utc", TABLE_SIZE),
                           m_mapPSYNS0Uwc("PSYNS0Uwc", TABLE_SIZE),
                           m_mapPSYNS1Ltc("PSYNS1Ltc", TABLE_SIZE),
                           m_mapPSYNS1Lwc("PSYNS1Lwc", TABLE_SIZE),
                           m_mapPSYNS1Rtc("PSYNS1Rtc", TABLE_SIZE),
                           m_mapPSYNS1Rwc("PSYNS1Rwc", TABLE_SIZE),
                           m_mapPSYNS1Utc("PSYNS1Utc", TABLE_SIZE),
                           m_mapPSYNS1Uwc("PSYNS1Uwc", TABLE_SIZE),
                           m_mapPPSYNS0wcS1wc("PPSYNS0wcS1wc", TABLE_SIZE),
                           m_mapPPSYNS0cS1wc("PPSYNS0cS1wc", TABLE_SIZE),
                           m_mapPPSYNS0wcS1c("PPSYNS0wcS1c", TABLE_SIZE),
                           m_mapPPSYNS0cS1c("PPSYNS0cS1c", TABLE_SIZE),
                           m_mapPSYNS0wcS1wc("PSYNS0wcS1wc", TABLE_SIZE),
                           m_mapPSYNS0cS1wc("PSYNS0cS1wc", TABLE_SIZE),
                           m_mapPSYNS0wcS1c("PSYNS0wcS1c", TABLE_SIZE),
                           m_mapPSYNS0cS1c("PSYNS0cS1c", TABLE_SIZE),
                           m_mapPSYNS0wcN0w("PSYNS0wcN0w", TABLE_SIZE),
                           m_mapPSYNS0cN0w("PSYNS0cN0w", TABLE_SIZE),
                           m_mapPSYNS0wN0t("PSYNS0wN0t", TABLE_SIZE),
                           m_mapPSYNS0cN0t("PSYNS0cN0t", TABLE_SIZE),
                           m_mapPSYNS1wcN0w("PSYNS1wcN0w", TABLE_SIZE),
                           m_mapPSYNS1cN0w("PSYNS1cN0w", TABLE_SIZE),
                           m_mapPSYNS1wN0t("PSYNS1wN0t", TABLE_SIZE),
                           m_mapPSYNS1cN0t("PSYNS1cN0t", TABLE_SIZE),
                           m_mapSYNN0t("SYNN0t", TABLE_SIZE),
                           m_mapSYNN1t("SYNN1t", TABLE_SIZE),
                           m_mapSYNN2t("SYNN2t", TABLE_SIZE),
                           m_mapSYNN3t("SYNN3t", TABLE_SIZE),
                           m_mapSYNS0wN0t("SYNS0wN0t", TABLE_SIZE),
                           m_mapSYNS0cN0t("SYNS0cN0t", TABLE_SIZE),
                           m_mapSYNS1wN0t("SYNS1wN0t", TABLE_SIZE),
                           m_mapSYNS1cN0t("SYNS1cN0t", TABLE_SIZE),
                           m_mapSYNN0tN1w("SYNN0tN1w", TABLE_SIZE),
                           m_mapSYNN0wN1t("SYNN0wN1t", TABLE_SIZE),
                           m_mapSYNN0tN1t("SYNN0tN1t", TABLE_SIZE),
                           m_mapSYNS0wS1cN0t("SYNS0wS1cN0t", TABLE_SIZE),
                           m_mapSYNS0cS1wN0t("SYNS0cS1wN0t", TABLE_SIZE),
                           m_mapSYNS0cS1cN0t("SYNS0cS1cN0t", TABLE_SIZE),
                           m_mapSYNS0cS1tN0t("SYNS0cS1tN0t", TABLE_SIZE),
                           m_mapSYNS0wcN0tN1t("SYNS0wcN0tN1t", TABLE_SIZE),
                           m_mapSYNS0cN0wtN1t("SYNS0cN0wtN1t", TABLE_SIZE),
                           m_mapSYNS0cN0tN1wt("SYNS0cN0tN1wt", TABLE_SIZE),
                           m_mapSYNS0cN0tN1t("SYNS0cN0tN1t", TABLE_SIZE),
                           m_mapSYNS0tN0tN1t("SYNS0tN0tN1t", TABLE_SIZE),
                           m_mapPSYNS0z("PSYNS0z", TABLE_SIZE),
                           m_mapPSYNS0zc("PSYNS0zc", TABLE_SIZE),
                           m_mapPSYNS0CharTagCont("PSYNS0CharTagCont", TABLE_SIZE),
                           m_mapPSYNS1z("PSYNS1z", TABLE_SIZE),
                           m_mapPSYNS1zc("PSYNS1zc", TABLE_SIZE),
                           m_mapPSYNS1CharTagCont("PSYNS1CharTagCont", TABLE_SIZE),
                           m_mapPSYNS2zc("PSYNS2zc", TABLE_SIZE),
                           m_mapPSYNS2CharTagCont("PSYNS2CharTagCont", TABLE_SIZE),
                           m_mapPSYNS3zc("PSYNS3zc", TABLE_SIZE),
                           m_mapPSYNS3CharTagCont("PSYNS3CharTagCont", TABLE_SIZE),
                           m_mapPPSYNS0ztS1zt("PPSYNS0ztS1zt", TABLE_SIZE),
                           m_mapPPSYNS0zS1z("PPSYNS0zS1z", TABLE_SIZE),
                           m_mapPPSYNS0zcS1zc("PPSYNS0zcS1zc", TABLE_SIZE),
                           m_mapPPSYNS0cS1zc("PPSYNS0cS1zc", TABLE_SIZE),
                           m_mapPPSYNS0zcS1c("PPSYNS0zcS1c", TABLE_SIZE),
                           m_mapPSYNS0ztS1zt("PSYNS0ztS1zt", TABLE_SIZE),
                           m_mapPSYNS0zS1z("PSYNS0zS1z", TABLE_SIZE),
                           m_mapPSYNS0zcS1zc("PSYNS0zcS1zc", TABLE_SIZE),
                           m_mapPSYNS0cS1zc("PSYNS0cS1zc", TABLE_SIZE),
                           m_mapPSYNS0zcS1c("PSYNS0zcS1c", TABLE_SIZE),
                           m_mapPSYNS0zN0z("PSYNS0zN0z", TABLE_SIZE),
                           m_mapPSYNS0zcN0z("PSYNS0zcN0z", TABLE_SIZE),
                           m_mapPSYNS0zN01z("PSYNS0zN01z", TABLE_SIZE),
                           m_mapPSYNS0zN012z("PSYNS0zN012z", TABLE_SIZE),
                           m_mapPSYNS1zN0z("PSYNS1zN0z", TABLE_SIZE),
                           m_mapPSYNS1zcN0z("PSYNS1zcN0z", TABLE_SIZE),
                           m_mapPSYNS1zN01z("PSYNS1zN01z", TABLE_SIZE),
                           m_mapPSYNS1zN012z("PSYNS1zN012z", TABLE_SIZE),
                           m_mapPPSYNS1zS0zN0z("PPSYNS1zS0zN0z", TABLE_SIZE),
                           m_mapPPSYNS1zS0zN01z("PPSYNS1zS0zN01z", TABLE_SIZE),
                           m_mapPSYNS1zS0zN0z("PSYNS1zS0zN0z", TABLE_SIZE),
                           m_mapPSYNS1zS0zN01z("PSYNS1zS0zN01z", TABLE_SIZE),
                           m_mapWSLastWordByLastChar("WSLastWordByLastChar", TABLE_SIZE),
                           m_mapWSLengthByLastWord("WSLengthByLastWord", TABLE_SIZE),
                           m_mapWSLastLengthByWord("WSLastLengthByWord", TABLE_SIZE),
                           m_mapWSSeparateChars("WSSeparateChars", TABLE_SIZE),
                           m_mapWSFirstCharLastWordByWord("WSFirstCharLastWordByWord", TABLE_SIZE),
                           m_mapLastTagByTag("LastTagByTag", TABLE_SIZE),	
                           m_mapTag0Tag1Size1("Tag0Tag1Size1", TABLE_SIZE),	
                           m_mapTag0Tag1Tag2Size1("Tag0Tag1Tag2Size1", TABLE_SIZE),	
                           m_mapTagByLastWord("TagByLastWord", TABLE_SIZE),	
                           m_mapLastTwoTagsByTag("LastTwoTagsByTag", TABLE_SIZE),	
                           m_mapTagByFirstChar("TagByFirstChar", TABLE_SIZE),	
                           m_mapTagByFirstCharCat("TagByFirstCharCat", TABLE_SIZE),	
                           m_mapFirstCharBy2Tags("FirstCharBy2Tags", TABLE_SIZE),	
                           m_mapFirstCharBy3Tags("FirstCharBy3Tags", TABLE_SIZE),	
                           m_mapTagByChar("TagByChar", TABLE_SIZE),	
                           m_mapTaggedSeparateChars("TaggedSeparateChars", TABLE_SIZE),	
                           m_mapTagWordTag("TagWordTag", TABLE_SIZE),	
                           m_mapWordTagTag("WordTagTag", TABLE_SIZE),	
                           m_mapSeenWords("SeenWords", TABLE_SIZE),	
                           m_mapLastWordByWord("LastWordByWord", TABLE_SIZE),	
                           m_mapOneCharWord("OneCharWord", TABLE_SIZE),	
                           m_mapFirstAndLastChars("FirstAndLastChars", TABLE_SIZE),	
                           m_mapLengthByFirstChar("LengthByFirstChar", TABLE_SIZE),	
                           m_mapLengthByLastChar("LengthByLastChar", TABLE_SIZE),	
                           m_mapLeftPartWordTag("LeftPartWordTag", TABLE_SIZE),	
                           m_mapRightPartWordTag("RightPartWordTag", TABLE_SIZE),	
                           m_mapCurrentWordLastChar("CurrentWordLastChar", TABLE_SIZE),	
                           m_mapLastWordByLastChar("LastWordByLastChar", TABLE_SIZE),	
                           m_mapLengthByLastWord("LengthByLastWord", TABLE_SIZE),	
                           m_mapLastLengthByWord("LastLengthByWord", TABLE_SIZE),	
                           m_mapCurrentTag("CurrentTag", TABLE_SIZE),	
                           m_mapLastTagByWord("LastTagByWord", TABLE_SIZE),	
                           m_mapTagByWordAndPrevChar("TagByWordAndPrevChar", TABLE_SIZE),	
                           m_mapTagOfOneCharWord("TagOfOneCharWord", TABLE_SIZE),	
                           m_mapTagByLastChar("TagByLastChar", TABLE_SIZE),	
                           m_mapTagByLastCharCat("TagByLastCharCat", TABLE_SIZE),	
                           m_mapTaggedCharByLastChar("TaggedCharByLastChar", TABLE_SIZE),	
                           m_mapTag1Tag2Size1("Tag1Tag2Size1", TABLE_SIZE),	
                           m_mapSeparateChars("SeparateChars", TABLE_SIZE),	
                           m_mapLastWordFirstChar("LastWordFirstChar", TABLE_SIZE),	
                           m_mapFirstCharLastWordByWord("FirstCharLastWordByWord", TABLE_SIZE),	
                           m_mapTagByWordAndNextChar("TagByWordAndNextChar", TABLE_SIZE),	
                           m_mapAPPTagByChar("APPTagByChar", TABLE_SIZE),	
                           m_mapTaggedCharByFirstChar("TaggedCharByFirstChar", TABLE_SIZE),	
                           m_mapConsecutiveChars("ConsecutiveChars", TABLE_SIZE),	
                           m_mapTaggedConsecutiveChars("TaggedConsecutiveChars", TABLE_SIZE),
                           m_mapSSYNTaggedSeparateChars("SSYNTaggedSeparateChars", TABLE_SIZE),
                           m_mapSSYNS0fzcurz("SSYNS0fzcurz", TABLE_SIZE),
                           m_mapSSYNS0lzcurz("SSYNS0lzcurz", TABLE_SIZE),
                           m_mapSSYNS1fzcurw("SSYNS1fzcurw", TABLE_SIZE),
                           m_mapSSYNS1lzcurw("SSYNS1lzcurw", TABLE_SIZE),
                           m_mapSSYNS1lzcurwlz("SSYNS1lzcurwlz", TABLE_SIZE),
                           m_mapSSYNS1fzcurwt("SSYNS1fzcurwt", TABLE_SIZE),
                           m_mapSSYNS1lzcurwt("SSYNS1lzcurwt", TABLE_SIZE),
                           m_mapSSYNS1lzcurwlzt("SSYNS1lzcurwlzt", TABLE_SIZE),
                           m_mapPSYNS1lzcurwlz("PSYNS1lzcurwlz", TABLE_SIZE),
                           m_mapInSubWordDictionary("InSubWordDictionary", TABLE_SIZE),
                           m_mapInSubWordTagDictionary("InSubWordTagDictionary", TABLE_SIZE),
                           m_mapInWordDictionary("InWordDictionary", TABLE_SIZE),
                           m_mapInWordTagDictionary("InWordTagDictionary", TABLE_SIZE),
                           m_mapPSYNS2z("PSYNS2z", TABLE_SIZE),
                           m_mapSYNS2z("SYNS2z", TABLE_SIZE),
                           m_mapPSYNS0zS1zS2z("PSYNS0zS1zS2z", TABLE_SIZE),
                           m_mapPPSYNS0zS1zS2z("PPSYNS0zS1zS2z", TABLE_SIZE),
                           m_mapPPPSYNS0zS1zS2z("PPPSYNS0zS1zS2z", TABLE_SIZE),
                           m_mapPSYNS0zS1cS2c("PSYNS0zS1cS2c", TABLE_SIZE),
                           m_mapPSYNS0cS1zS2c("PSYNS0cS1zS2c", TABLE_SIZE),
                           m_mapPSYNS0cS1cS2z("PSYNS0cS1cS2z", TABLE_SIZE),
                           m_mapPPSYNS0zS1cS2c("PPSYNS0zS1cS2c", TABLE_SIZE),
                           m_mapPPSYNS0cS1zS2c("PPSYNS0cS1zS2c", TABLE_SIZE),
                           m_mapPPSYNS0cS1cS2z("PPSYNS0cS1cS2z", TABLE_SIZE),
                           m_mapPPPSYNS0zS1cS2c("PPPSYNS0zS1cS2c", TABLE_SIZE),
                           m_mapPPPSYNS0cS1zS2c("PPPSYNS0cS1zS2c", TABLE_SIZE),
                           m_mapPPPSYNS0cS1cS2z("PPPSYNS0cS1cS2z", TABLE_SIZE),
                           m_mapSubWordHeadCharType("SubWordHeadCharType", TABLE_SIZE),
                           m_mapSubWordHeadCharTypeTag("SubWordHeadCharTypeTag", TABLE_SIZE),
                           m_mapSubWordHeadCharEqual("SubWordHeadCharEqual", TABLE_SIZE),
                           m_mapSubWordHeadCharEqualTag("SubWordHeadCharEqualTag", TABLE_SIZE),
                           m_mapInSubWordDictionaryTag("InSubWordDictionaryTag", TABLE_SIZE),
                           m_mapInSubWordTagDictionaryTag("InSubWordTagDictionaryTag", TABLE_SIZE),
                           m_mapSYNRightMostc("SYNRightMostc", TABLE_SIZE),
                           m_mapSYNRightMostcWord("SYNRightMostcWord", TABLE_SIZE)
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
