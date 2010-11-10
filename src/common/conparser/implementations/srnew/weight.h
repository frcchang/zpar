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

//#define TABLE_SIZE 122651
const static unsigned TABLE_SIZE = 131072;

namespace TARGET_LANGUAGE {

namespace conparser {

//#include "conparser_macros.h"

//
// TYPE DEFINITIONS
//
typedef CScoreMapType<CWord, SCORE_TYPE> CWordMap;
typedef CScoreMapType<CTaggedWord<CTag, TAG_SEPARATOR>, SCORE_TYPE> CTaggedWordMap;
typedef CScoreMapType<std::pair<CWord, unsigned long>,  SCORE_TYPE> CWordIntMap;
typedef CScoreMapType<CTuple3<CWord, unsigned long, CAction>,  SCORE_TYPE> CWordIntActionMap;
typedef CScoreMapType<std::pair<CWord, CAction>,  SCORE_TYPE> CWordActionMap;
typedef CScoreMapType<std::pair<CTaggedWord<CTag, TAG_SEPARATOR>, CAction>, SCORE_TYPE> CTaggedWordActionMap;
typedef CScoreMapType<unsigned long, SCORE_TYPE> CIntMap;
typedef CScoreMapType<std::pair<unsigned long, CAction>, SCORE_TYPE> CIntActionMap;
typedef CScoreMapType<std::pair<unsigned long, unsigned long>, SCORE_TYPE> CTwoIntMap;
typedef CScoreMapType<CTwoWords, SCORE_TYPE> CTwoWordsMap;
typedef CScoreMapType<std::pair<CTwoWords, CAction>, SCORE_TYPE> CTwoWordsActionMap;
typedef CScoreMapType<std::pair<CTwoWords, CActionType>, SCORE_TYPE> CTwoWordsActionTypeMap;
typedef CScoreMapType<CTuple3<CTwoWords, CTag, CAction>, SCORE_TYPE> CTwoWordsTagActionMap;
typedef CScoreMapType<CTuple3<CTwoWords, CTag, CActionType>, SCORE_TYPE> CTwoWordsTagActionTypeMap;
typedef CScoreMapType<CTuple3<CTwoWords, CCFGSet, CAction>, SCORE_TYPE>
CTwoWordsCFGSetActionMap;
typedef CScoreMapType<CTwoTaggedWords, SCORE_TYPE> CTwoTaggedWordsMap;
typedef CScoreMapType<std::pair<CTwoTaggedWords, CAction>, SCORE_TYPE> CTwoTaggedWordsActionMap;
typedef CScoreMapType<std::pair<CTwoTaggedWords, CActionType>, SCORE_TYPE> CTwoTaggedWordsActionTypeMap;
typedef CScoreMapType<CCFGSet, SCORE_TYPE> CCFGSetMap;
typedef CScoreMapType<std::pair<CCFGSet, unsigned long>, SCORE_TYPE> CCFGSetIntMap;
typedef CScoreMapType<std::pair<CCFGSet, CAction>, SCORE_TYPE> CCFGSetActionMap;
typedef CScoreMapType<CTuple3<CWord, CCFGSet, CAction>, SCORE_TYPE> CWordCFGSetActionMap;
typedef CScoreMapType<CTuple3<CWord, CCFGSet, CActionType>, SCORE_TYPE> CWordCFGSetActionTypeMap;
typedef CScoreMapType<std::pair<CWord, CCFGSet>, SCORE_TYPE> CWordCFGRuleMap;
typedef CScoreMapType<std::pair<CTwoWords, CCFGSet>, SCORE_TYPE> CTwoWordsCFGRuleMap;
typedef CScoreMapType<std::pair<CCFGSet, unsigned long>, SCORE_TYPE> CCFGSetIntMap;
typedef CScoreMapType<std::pair<CCFGSet, CAction>, SCORE_TYPE> CCFGSetActionMap;
typedef CScoreMapType<std::pair<CCFGSet, CActionType>, SCORE_TYPE> CCFGSetActionTypeMap;
typedef CScoreMapType<CTuple3<CWord, CConstituent, CAction>, SCORE_TYPE>
CWordConstituentActionMap;
typedef CScoreMapType<CTuple3<CTag, CConstituent, CAction>, SCORE_TYPE> CTagConstituentActionMap;
typedef CScoreMapType<CTuple3<CTag, CTag, unsigned long>, SCORE_TYPE> CWordTagIntMap;
typedef CScoreMapType<std::pair<CConstituent, CAction>, SCORE_TYPE> CConstituentActionMap;

typedef CHashMap<CWord, unsigned long> CWordToIntMap;

/*===============================================================
 *
 * CWeight - the definition of weights, in feature
 *
 *==============================================================*/

class CWeight : public CWeightBase {

public:

   // Gr
   CWordIntMap m_mapHw;
   CTwoIntMap m_mapHt;
   CCFGSetMap m_mapHc;
   CWordCFGRuleMap m_mapHwc;

   CCFGSetMap m_mapHcMc;
   CWordCFGRuleMap m_mapHwMc;
   CWordCFGRuleMap m_mapHcMw;
   CTwoWordsCFGRuleMap m_mapHwMw;

   CCFGSetMap m_mapBigramsL;
   CCFGSetMap m_mapBigramsR;
   CWordCFGRuleMap m_mapBigramsLexA;
   CWordCFGRuleMap m_mapBigramsLexB;

   CCFGSetMap m_mapRules;
   CCFGSetIntMap m_mapRulesWithSizes;

   CIntMap m_mapBracket;

   CCFGSetMap m_mapHtMt;
   CWordCFGRuleMap m_mapHtMw;
   CWordCFGRuleMap m_mapHwMt;

   CIntMap m_mapSiblingTags;
   CWordIntMap m_mapSiblingWordTag;
   CWordIntMap m_mapSiblingTagWord;

   CTwoIntMap m_mapHtArityL;
   CTwoIntMap m_mapHtArityR;
   CWordIntMap m_mapHwArityL;
   CWordIntMap m_mapHwArityR;
   
   // S0
   CConstituentActionMap m_mapS0c;
   CTaggedWordActionMap m_mapS0w;
   CTagConstituentActionMap m_mapS0tc;
   CWordConstituentActionMap m_mapS0wc;

   // S1
   CConstituentActionMap m_mapS1c;
   CTaggedWordActionMap m_mapS1w;
   CTagConstituentActionMap m_mapS1tc;
   CWordConstituentActionMap m_mapS1wc;

   // S2
   CTagConstituentActionMap m_mapS2tc;
   CWordConstituentActionMap m_mapS2wc;

   // S3
   CTagConstituentActionMap m_mapS3tc;
   CWordConstituentActionMap m_mapS3wc;

   // N0
   CWordActionMap m_mapN0w;
   CIntMap m_mapN0t;
   CTaggedWordActionMap m_mapN0wt;

   // N1
   CWordActionMap m_mapN1w;
   CIntMap m_mapN1t;
   CTaggedWordActionMap m_mapN1wt;

   // N2
   CWordActionMap m_mapN2w;
   CIntMap m_mapN2t;
   CTaggedWordActionMap m_mapN2wt;

   // N3
   CWordActionMap m_mapN3w;
   CIntMap m_mapN3t;
   CTaggedWordActionMap m_mapN3wt;

   // S0L
   CWordConstituentActionMap m_mapS0Lwc;
   CTagConstituentActionMap m_mapS0Ltc;
   CCFGSetActionMap m_mapS0cS0HcS0L2c;

   // S0R
   CWordConstituentActionMap m_mapS0Rwc;
   CTagConstituentActionMap m_mapS0Rtc;
   CCFGSetActionMap m_mapS0cS0HcS0R2c;

   // S0U
   CWordConstituentActionMap m_mapS0Uwc;
   CTagConstituentActionMap m_mapS0Utc;

   // S1L
   CWordConstituentActionMap m_mapS1Lwc;
   CTagConstituentActionMap m_mapS1Ltc;

   // S1R
   CWordConstituentActionMap m_mapS1Rwc;
   CTagConstituentActionMap m_mapS1Rtc;
   CCFGSetActionMap m_mapS1cS1HcS1R2c;

   // S1U
   CWordConstituentActionMap m_mapS1Uwc;
   CTagConstituentActionMap m_mapS1Utc;

   // S0LD
   CWordActionMap m_mapS0LDw;
   CIntMap m_mapS0LDt;

   // S0RD
   CWordActionMap m_mapS0RDw;
   CIntMap m_mapS0RDt;

   // S1LD
   CWordActionMap m_mapS1LDw;
   CIntMap m_mapS1LDt;

   // S1RD
   CWordActionMap m_mapS1RDw;
   CIntMap m_mapS1RDt;

   // S0 S1
   CTwoWordsCFGSetActionMap m_mapS0wcS1wc; // the original 
   CWordCFGSetActionMap m_mapS0wS1c;
   CWordCFGSetActionMap m_mapS0cS1w;
   CCFGSetActionMap m_mapS0cS1c;

   CTwoTaggedWordsActionTypeMap m_mapS0wtS1wt;
   CTwoWordsTagActionTypeMap m_mapS0wtS1w;
   CTwoWordsTagActionTypeMap m_mapS0wS1wt;
   CTwoWordsActionTypeMap m_mapS0wS1w;
   CWordCFGSetActionTypeMap m_mapS0wtS1t;
   CWordCFGSetActionTypeMap m_mapS0tS1wt;
   CCFGSetActionTypeMap m_mapS0tS1t;

   CCFGSetActionMap m_mapBetweenTags;

   // S0 N0
   CTwoWordsCFGSetActionMap m_mapS0wN0w;
   CWordCFGSetActionMap m_mapS0cN0w;
   CWordCFGSetActionMap m_mapS0wN0t;
   CCFGSetActionMap m_mapS0cN0t;
   CTwoIntMap m_mapS0cmN0tm;

   // S1 N0
   CTwoWordsCFGSetActionMap m_mapS1wN0w;
   CWordCFGSetActionMap m_mapS1cN0w;
   CWordCFGSetActionMap m_mapS1wN0t;
   CCFGSetActionMap m_mapS1cN0t;

   // N0 N1
   CTwoWordsCFGSetActionMap m_mapN0wN1w;
   CWordCFGSetActionMap m_mapN0tN1w;
   CWordCFGSetActionMap m_mapN0wN1t;
   CCFGSetActionMap m_mapN0tN1t;

   // Bracket
#ifdef _CHINESE_CFG_H
   CIntMap m_mapBracketS0c;
   CWordIntMap m_mapBracketS0w;
   CWordIntMap m_mapBracketS0wS1c;
   CWordIntMap m_mapBracketS0cS1w;
   CIntMap m_mapBracketS0cS1c;
   CWordIntMap m_mapBracketS0cN0w;
   CWordIntMap m_mapBracketS0wN0t;
   CIntMap m_mapBracketS0cN0t;

   // Separator
   CIntActionMap m_mapS0cSeparator;
   CWordIntActionMap m_mapS0wcSeparator;
   CIntActionMap m_mapS0cSepCount;
   CWordIntActionMap m_mapS0wcSepCount;
   CIntActionMap m_mapS1cSeparator;
   CWordIntActionMap m_mapS1wcSeparator;
   CIntActionMap m_mapS1cSepCount;
   CWordIntActionMap m_mapS1wcSepCount;
   CIntActionMap m_mapS0cS1cSeparator;
   CIntActionMap m_mapS0cS1cSepCount;
#endif

   // Distance
   CWordIntMap m_mapS0wS1Dist;
   CIntMap m_mapS0cS1Dist;
   CWordIntMap m_mapS0S1wDist;
   CIntMap m_mapS0S1cDist;
   CTwoWordsActionMap m_mapS0wS1wDist;
   CWordIntMap m_mapS0wS1cDist;
   CWordIntMap m_mapS0cS1wDist;
   CIntMap m_mapS0cS1cDist;

   // S0 S1N0
   CWordCFGSetActionMap m_mapS0wS1cN0t;
   CWordCFGSetActionMap m_mapS0cS1wN0t;
   CWordCFGSetActionMap m_mapS0cS1cN0w;
   CCFGSetActionMap m_mapS0cS1cN0t;
   CCFGSetActionMap m_mapS0tS1tN0t;
   CCFGSetActionMap m_mapS0jS1jN0t;

   // S0 N0N1
   CWordCFGSetActionMap m_mapS0wN0tN1t;
   CWordCFGSetActionMap m_mapS0cN0wN1t;
   CWordCFGSetActionMap m_mapS0cN0tN1w;
   CCFGSetActionMap m_mapS0cN0tN1t;
   CCFGSetActionMap m_mapS0tN0tN1t;
   CCFGSetActionMap m_mapS0jN0tN1t;

   // S0 S1S2
   CWordCFGSetActionMap m_mapS0wS1cS2c;
   CWordCFGSetActionMap m_mapS0cS1wS2c;
   CWordCFGSetActionMap m_mapS0cS1cS2w;
   CCFGSetActionMap m_mapS0cS1cS2c;
   CCFGSetActionMap m_mapS0tS1tS2t;
   CCFGSetActionMap m_mapS0jS1jS2j;

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
   CCFGSetActionMap m_mapS0cS0RcN0t;
   CCFGSetActionMap m_mapS0cS0RjN0t;
   CWordCFGSetActionMap m_mapS0cS0RcN0w;

   CCFGSetActionMap m_mapS0cS0LcN0t;
   CWordCFGSetActionMap m_mapS0wS0LcN0t;
   CWordCFGSetActionMap m_mapS0cS0LcN0w;
   CWordCFGSetActionMap m_mapS0wS0RcN0t;
   CCFGSetActionMap m_mapS0cS0UcN0t;
   CWordCFGSetActionMap m_mapS0wS0UcN0t;
   CWordCFGSetActionMap m_mapS0cS0UcN0w;

   // S0 S0LRUS1
   CCFGSetActionMap m_mapS0cS0LcS1c;
   CCFGSetActionMap m_mapS0cS0LjS1j;
   CWordCFGSetActionMap m_mapS0cS0LcS1w;

   CWordCFGSetActionMap m_mapS0wS0LcS1c;
   CCFGSetActionMap m_mapS0cS0RcS1c;
   CWordCFGSetActionMap m_mapS0wS0RcS1c;
   CWordCFGSetActionMap m_mapS0cS0RcS1w;
   CCFGSetActionMap m_mapS0cS0UcS1c;
   CWordCFGSetActionMap m_mapS0wS0UcS1c;
   CWordCFGSetActionMap m_mapS0cS0UcS1w;

   // S0 S1LRUS1
   CCFGSetActionMap m_mapS0cS1cS1Rc;
   CCFGSetActionMap m_mapS0jS1cS1Rj;
   CWordCFGSetActionMap m_mapS0wS1cS1Rc;

   CCFGSetActionMap m_mapS0cS1cS1Lc;
   CWordCFGSetActionMap m_mapS0wS1cS1Lc;
   CWordCFGSetActionMap m_mapS0cS1wS1Lc;
   CWordCFGSetActionMap m_mapS0cS1wS1Rc;
   CCFGSetActionMap m_mapS0cS1cS1Uc;
   CWordCFGSetActionMap m_mapS0wS1cS1Uc;
   CWordCFGSetActionMap m_mapS0cS1wS1Uc;

   // Rule dictionary
   unsigned long m_nMaxWordFrequency;
   CWordToIntMap m_mapWordFrequency;

public:

   CWeight(bool bTrain) : CWeightBase(bTrain) ,

                          m_nMaxWordFrequency(0) ,
                          m_mapWordFrequency(TABLE_SIZE),

                          m_mapHw("HeadWord", TABLE_SIZE),
                          m_mapHt("HeadTag", TABLE_SIZE),
                          m_mapHc("HeadConstituents", TABLE_SIZE),
                          m_mapHwc("HeadWord", TABLE_SIZE),

                          m_mapHcMc("HeadModifierConstituents", TABLE_SIZE),
                          m_mapHwMc("HeadModifierWordConstituent", TABLE_SIZE),
                          m_mapHcMw("HeadModifierConstituentW", TABLE_SIZE),
                          m_mapHwMw("HeadModifierWrds", TABLE_SIZE),

                          m_mapBigramsL("BigramsOnTheLeft", TABLE_SIZE),
                          m_mapBigramsR("BigramsOnTheRight", TABLE_SIZE),
                          m_mapBigramsLexA("LexicalizedBigramsWC", TABLE_SIZE),
                          m_mapBigramsLexB("LexicalizedBigramsCW", TABLE_SIZE),

                          m_mapRules("Rules", TABLE_SIZE),
                          m_mapRulesWithSizes("RulesSz", TABLE_SIZE),

                          m_mapBracket("BracketType", TABLE_SIZE),

                          m_mapHtMt("HeadTagModifierTag", TABLE_SIZE),
                          m_mapHtMw("HeadTagModifierWord", TABLE_SIZE),
                          m_mapHwMt("HeadWordModifierTag", TABLE_SIZE),
                          m_mapSiblingTags("SiblingTs", TABLE_SIZE),
                          m_mapSiblingWordTag("SiblingWt", TABLE_SIZE),
                          m_mapSiblingTagWord("SiblingTw", TABLE_SIZE),
                          m_mapHtArityL("HeadTagArityLeft", TABLE_SIZE),
                          m_mapHtArityR("HeadTagArityRight", TABLE_SIZE),
                          m_mapHwArityL("HeadWordArityLeft", TABLE_SIZE),
                          m_mapHwArityR("HeadWordArityRight", TABLE_SIZE),

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
                          m_mapS0cS0HcS0L2c("Stack0ConstituentStack0HeadSubConstituentS0Left2SubConstituents", TABLE_SIZE),

                          m_mapS0Rwc("Stack0RightWord", TABLE_SIZE),
                          m_mapS0Rtc("Stack0RightConstituent", TABLE_SIZE), 
                          m_mapS0cS0HcS0R2c("Stack0ConstituentStack0HeadSubConstituentS0Right2SubConstituents", TABLE_SIZE),

                          m_mapS0Uwc("Stack0UniWord", TABLE_SIZE),
                          m_mapS0Utc("Stack0UniConstituent", TABLE_SIZE), 

                          m_mapS1Lwc("Stack1LeftWord", TABLE_SIZE),
                          m_mapS1Ltc("Stack1LeftConstituent", TABLE_SIZE), 

                          m_mapS1Rwc("Stack1RightWord", TABLE_SIZE),
                          m_mapS1Rtc("Stack1RightConstituent", TABLE_SIZE), 
                          m_mapS1cS1HcS1R2c("Stack1ConstituentStack1HeadSubConstituentS1Right2SubConstituents", TABLE_SIZE),

                          m_mapS1Uwc("Stack1UniWord", TABLE_SIZE),
                          m_mapS1Utc("Stack1UniConstituent", TABLE_SIZE), 

                          m_mapS0LDw("Stack0LeftDepWord", TABLE_SIZE),
                          m_mapS0LDt("Stack0LeftDepTag", TABLE_SIZE), 

                          m_mapS0RDw("Stack0RightDepWord", TABLE_SIZE),
                          m_mapS0RDt("Stack0RightDepTag", TABLE_SIZE), 

                          m_mapS1LDw("Stack1LeftDepWord", TABLE_SIZE),
                          m_mapS1LDt("Stack1LeftDepTag", TABLE_SIZE), 

                          m_mapS1RDw("Stack1RightDepWord", TABLE_SIZE),
                          m_mapS1RDt("Stack1RightDepTag", TABLE_SIZE), 

                          m_mapS0wcS1wc("Stack0WordStack1Word", TABLE_SIZE),
                          m_mapS0wS1c("Stack0WordStack1Constituent", TABLE_SIZE),
                          m_mapS0cS1w("Stack0ConstituentStack1Word", TABLE_SIZE),
                          m_mapS0cS1c("Stack0ConstituentStack1Constituent",
TABLE_SIZE),
                          m_mapS0wtS1wt("Stack0WordTagStack1WordTag", TABLE_SIZE),
                          m_mapS0wtS1w("Stack0WordTagStack1Word", TABLE_SIZE),
                          m_mapS0wS1wt("Stack0WordStack1WordTag", TABLE_SIZE),
                          m_mapS0wS1w("Stack0WordStack1Word", TABLE_SIZE),
                          m_mapS0wtS1t("Stack0WordTagStack1Tag", TABLE_SIZE),
                          m_mapS0tS1wt("Stack0TagStack1WordTag", TABLE_SIZE),
                          m_mapS0tS1t("Stack0TagStack1Tag", TABLE_SIZE),
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
                          m_mapN0tN1t("Next0TagNext1WordTag", TABLE_SIZE),

#ifdef _CHINESE_CFG_H
                          m_mapBracketS0c("BracketStack0Constituent", TABLE_SIZE), 
                          m_mapBracketS0w("BracketStack0Word", TABLE_SIZE),
                          m_mapBracketS0wS1c("BracketStack0WordStack1Constituent", TABLE_SIZE),
                          m_mapBracketS0cS1w("BracketStack0ConstituentStack1Word", TABLE_SIZE),
                          m_mapBracketS0cS1c("BracketStack0ConsituentStack1Constituent", TABLE_SIZE),
                          m_mapBracketS0cN0w("BracketStack0ConstituentNext0Word", TABLE_SIZE),
                          m_mapBracketS0wN0t("BracketStack0WordNext0Tag", TABLE_SIZE),
                          m_mapBracketS0cN0t("BracketStack0ConstituentNext0Tag", TABLE_SIZE),

                          m_mapS0cSeparator("Stack0ConstituentSeparator", TABLE_SIZE),
                          m_mapS0wcSeparator("Stack0WordConstituentSeparator", TABLE_SIZE),
                          m_mapS0cSepCount("Stack0ConstituentSeparatorCount", TABLE_SIZE),
                          m_mapS0wcSepCount("Stack0WordConstituentSeparatorCount", TABLE_SIZE),
                          m_mapS1cSeparator("Stack1ConstituentSeparator", TABLE_SIZE),
                          m_mapS1wcSeparator("Stack1WordConstituentSeparator", TABLE_SIZE),
                          m_mapS1cSepCount("Stack1ConstituentSeparatorCount", TABLE_SIZE),
                          m_mapS1wcSepCount("Stack1WordConstituentSeparatorCount", TABLE_SIZE),
                          m_mapS0cS1cSeparator("Stack0ConstituentStack1ConstituentSeparator", TABLE_SIZE),
                          m_mapS0cS1cSepCount("Stack0ConstituentStack1ConstituentSeparatorCount", TABLE_SIZE),
#endif

                          m_mapS0wS1Dist("Stack0WordStack1Distance", TABLE_SIZE),
                          m_mapS0cS1Dist("Stack0ConstituentStack1Distance", TABLE_SIZE),
                          m_mapS0S1wDist("Stack0Stack1WordDistance", TABLE_SIZE),
                          m_mapS0S1cDist("Stack0Stack1ConstituentDistance", TABLE_SIZE),
                          m_mapS0wS1wDist("Stack0WordStack1WordDistance", TABLE_SIZE),
                          m_mapS0wS1cDist("Stack0WordStack1ConstituentDistance", TABLE_SIZE),
                          m_mapS0cS1wDist("Stack0ConstituentStack1WordDistance", TABLE_SIZE),
                          m_mapS0cS1cDist("Stack0ConstituentStack1ConstituentDistance",TABLE_SIZE),

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
                          m_mapS0jS1jS2j("Stack0TagOrConstituentStack1TagOrConstituentStack2TagOrConstituent", TABLE_SIZE),

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
                          m_mapS0cS1wS1Uc("Stack0ConstituentStack1WordStack1UConstituent", TABLE_SIZE)
   { }


   // MEHTODS
   virtual void loadScores(std::ifstream &is);
   virtual void saveScores(std::ofstream &os);
   void computeAverageFeatureWeights(int round);
   SCORE_TYPE dotProduct(const CWeight &w);
 
};

};
};

#endif
