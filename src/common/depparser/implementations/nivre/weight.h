/***************************************************************
 *
 * weight.h - the dependency parser weights 
 *
 * Yue Zhang, 2007.8 - 2008.1
 *
 ***************************************************************/

#ifndef _DEPPARSER_WEIGHTS_H
#define _DEPPARSER_WEIGHTS_H

#include "weight_base.h"

namespace TARGET_LANGUAGE {

namespace depparser {

#include "macros.h"

//
// TYPE DEFINITIONS
//
typedef CScoreMap<CWord, SCORE_TYPE> CWordMap;
typedef CScoreMap<CTaggedWord<CTag>, SCORE_TYPE> CTaggedWordMap;
typedef CScoreMap<pair<CWord, int>,  SCORE_TYPE> CWordIntMap;
typedef CScoreMap<pair<CTaggedWord<CTag>, int>,  SCORE_TYPE> CTaggedWordIntMap;
typedef CScoreMap<int, SCORE_TYPE> CIntMap;
typedef CScoreMap<pair<int, int>, SCORE_TYPE> CTwoIntMap;
typedef CScoreMap<CTwoWords, SCORE_TYPE> CTwoWordsMap;
typedef CScoreMap<pair<CTwoWords, int>, SCORE_TYPE> CTwoWordsIntMap;
typedef CScoreMap<CTwoTaggedWords, SCORE_TYPE> CTwoTaggedWordsMap;
typedef CScoreMap<pair<CTwoTaggedWords, int>, SCORE_TYPE> CTwoTaggedWordsIntMap;

/*===============================================================
 *
 * CWeight - the definition of weights, in feature
 *
 *==============================================================*/

class CWeight : public CWeightBase {

public:

   // feature templates from mcdonald
   CWordIntMap m_mapHeadWord;
   CWordIntMap m_mapDepWord;
   CTwoIntMap m_mapHeadTag;
   CTwoIntMap m_mapDepTag;
   CTaggedWordIntMap m_mapHeadWordTag;
   CTaggedWordIntMap m_mapDepWordTag;

   CTwoTaggedWordsIntMap m_mapHeadWordTagDepWordTag;
   CTwoTaggedWordsIntMap m_mapHeadWordTagDepWord;
   CTwoTaggedWordsIntMap m_mapHeadWordDepWordTag;
   CTwoTaggedWordsIntMap m_mapHeadTagDepWordTag;
   CTwoTaggedWordsIntMap m_mapHeadWordTagDepTag;
   CTwoWordsIntMap m_mapHeadWordDepWord;
   CTwoIntMap m_mapHeadTagDepTag;

   CTwoIntMap m_mapBetweenTags;
   CTwoIntMap m_mapSurroundingTagsLL;
   CTwoIntMap m_mapSurroundingTagsLR;
   CTwoIntMap m_mapSurroundingTagsRL;
   CTwoIntMap m_mapSurroundingTagsRR;

   // second order feature templates from mcdonald
   CTwoWordsIntMap m_mapSiblingWords;
   CTaggedWordIntMap m_mapSiblingWordTag;
   CTaggedWordIntMap m_mapSiblingTagWord;
   CTwoIntMap m_mapSiblingTags;
   CTwoIntMap m_mapSiblingAndParentTags;

   // new features
   CTwoIntMap m_mapGrandChildTags; 
   CTaggedWordIntMap m_mapHeadWordTagArity;
   CTwoIntMap m_mapHeadTagArity;
   CTwoIntMap m_mapTwoSiblingTags;
   CTwoIntMap m_mapTwoSiblingAndParentTags;

   // feature templates from mcdonald
   CWordIntMap m_mapSTw;
   CTwoIntMap m_mapSTt;
   CTaggedWordIntMap m_mapSTwt;
   CWordIntMap m_mapN0w;
   CTwoIntMap m_mapN0t;
   CTaggedWordIntMap m_mapN0wt;
   CWordIntMap m_mapN1w;
   CTwoIntMap m_mapN1t;
   CTaggedWordIntMap m_mapN1wt;

   CTwoTaggedWordsIntMap m_mapSTwtN0wt;
   CTwoTaggedWordsIntMap m_mapSTwtN0w;
   CTwoTaggedWordsIntMap m_mapSTwN0wt;
   CTwoTaggedWordsIntMap m_mapSTtN0wt;
   CTwoTaggedWordsIntMap m_mapSTwtN0t;
   CTwoWordsIntMap m_mapSTwN0w;
   CTwoIntMap m_mapSTtN0t;

   CTwoIntMap m_mapN0tN1t;
   CTwoIntMap m_mapN0tN1tN2t;
   CTwoIntMap m_mapSTtN0tN1t;
   CTwoIntMap m_mapSTtN0tN0LDt;
   CTwoIntMap m_mapSTHtSTtN0t;
   CTwoIntMap m_mapSTtSTLDtN0t;
   CTwoIntMap m_mapSTtSTRDtN0t;

   CWordIntMap m_mapN0wN1t;
   CWordIntMap m_mapN0wN1tN2t;
   CWordIntMap m_mapSTtN0wN1t;
   CWordIntMap m_mapSTtN0wN0LDt;
   CWordIntMap m_mapSTHtSTtN0w;
   CWordIntMap m_mapSTtSTLDtN0w;
   CWordIntMap m_mapSTtSTRDtN0w;

#ifdef LABELED
   CIntMap m_mapLabel;
   CWordIntMap m_mapHeadWordLabel;
   CWordIntMap m_mapDepWordLabel;
   CTaggedWordIntMap m_mapHeadWordTagLabel;
   CTaggedWordIntMap m_mapDepWordTagLabel;
   CTwoIntMap m_mapHeadTagLabel;
   CTwoIntMap m_mapDepTagLabel;
   CTwoIntMap m_mapHeadSurroundingTagsLabel;
   CTwoIntMap m_mapDepSurroundingTagsLabel;
#endif

public:

   CWeight(const string &sPath, bool bTrain) : CWeightBase(sPath, bTrain) ,

                                               m_mapHeadWord("HeadWord", 122651),
                                               m_mapDepWord("DepWord", 122651),
                                               m_mapHeadTag("HeadTag", 122651),
                                               m_mapDepTag("DepTag", 122651),
                                               m_mapHeadWordTag("HeadWordTag", 122651),
                                               m_mapDepWordTag("DepWordTag", 122651),

                                               m_mapHeadWordTagDepWordTag("HeadWordTagDepWordTag", 122651),
                                               m_mapHeadWordTagDepWord("HeadWordTagDepWord", 122651),
                                               m_mapHeadWordDepWordTag("HeadWordDepWordTag", 122651),
                                               m_mapHeadTagDepWordTag("HeadTagDepWordTag", 122651),
                                               m_mapHeadWordTagDepTag("HeadWordTagDepTag", 122651),
                                               m_mapHeadWordDepWord("HeadWordDepWord", 122651),
                                               m_mapHeadTagDepTag("HeadTagDepTag", 122651),

                                               m_mapBetweenTags("BetweenTags", 122651),
                                               m_mapSurroundingTagsLL("SurroundingTagsLL", 122651),
                                               m_mapSurroundingTagsLR("SurroundingTagsLR", 122651),
                                               m_mapSurroundingTagsRL("SurroundingTagsRL", 122651),
                                               m_mapSurroundingTagsRR("SurroundingTagsRR", 122651),

                                               m_mapSiblingWords("SiblingWords", 122651),
                                               m_mapSiblingWordTag("SiblingWordTag", 122651),
                                               m_mapSiblingTagWord("SiblingTagWord", 122651),
                                               m_mapSiblingTags("SiblingTags", 122651),
                                               m_mapSiblingAndParentTags("SiblingAndParentTags", 122651),

                                               m_mapTwoSiblingTags("TwoSiblingTags", 122651),
                                               m_mapTwoSiblingAndParentTags("TwoSiblingAndParentTags", 122651),
                                               m_mapGrandChildTags("GrandChildTags", 122651), 
                                               m_mapHeadWordTagArity("HeadWordTagArity", 122651),
                                               m_mapHeadTagArity("HeadTagArity", 122651),

#ifdef LABELED
                                               m_mapLabel("Label", 122651),
                                               m_mapHeadWordLabel("HeadWordLabel", 122651),
                                               m_mapDepWordLabel("DepWordLabel", 122651),
                                               m_mapHeadWordTagLabel("HeadWordTagLabel", 122651),
                                               m_mapDepWordTagLabel("DepWordTagLabel", 122651),
                                               m_mapHeadTagLabel("HeadTagLabel", 122651),
                                               m_mapDepTagLabel("DepTagLabel", 122651),
                                               m_mapHeadSurroundingTagsLabel("HeadSurroundingTagsLabel", 122651),
                                               m_mapDepSurroundingTagsLabel("DepSurroundingTagsLabel", 122651),
#endif
                                               m_mapSTwt("StackWordTag", 122651),
                                               m_mapSTw("StackWord", 122651),
                                               m_mapSTt("StackTag", 122651),

                                               m_mapN0wt("NextWordTag", 122651),
                                               m_mapN0w("NextWord", 122651),
                                               m_mapN0t("NextTag", 122651),

                                               m_mapN1wt("Next+1WordTag", 122651),
                                               m_mapN1w("Next+1Word", 122651),
                                               m_mapN1t("Next+1Tag", 122651),

                                               m_mapSTwtN0wt("StackWordTagNextWordTag", 122651),
                                               m_mapSTwtN0w("StackWordTagNextWord", 122651),
                                               m_mapSTwN0wt("StackWordNextWordTag", 122651),
                                               m_mapSTwtN0t("StackWordTagNextTag", 122651),
                                               m_mapSTtN0wt("StackTagNextWordTag", 122651),
                                               m_mapSTwN0w("StackWordNextWord", 122651),
                                               m_mapSTtN0t("StackTagNextTag", 122651),

                                               m_mapN0tN1t("NextTagNext+1Tag", 122651),
                                               m_mapN0tN1tN2t("NextTagTrigram", 122651),
                                               m_mapSTtN0tN1t("StackTagNextTagNext+1Tag", 122651),
                                               m_mapSTtN0tN0LDt("StackTagNextTagNextLDTag", 122651),
                                               m_mapSTHtSTtN0t("StackHeadTagStackTagNextTag", 122651),
                                               m_mapSTtSTLDtN0t("StackTagStackLDTagNextTag", 122651),
                                               m_mapSTtSTRDtN0t("StackTagStackRDTagNextTag", 122651),

                                               m_mapN0wN1t("NextWordNext+1Tag", 122651),
                                               m_mapN0wN1tN2t("NextWordNext+1TagNext+2Tag", 122651),
                                               m_mapSTtN0wN1t("StackTagNextWordNext+1Tag", 122651),
                                               m_mapSTtN0wN0LDt("StackTagNextWordNextLDTag", 122651),
                                               m_mapSTHtSTtN0w("StackHeadTagStackTagNextWord", 122651),
                                               m_mapSTtSTLDtN0w("StackTagStackLDTagNextWord", 122651),
                                               m_mapSTtSTRDtN0w("StackTagStackRDTagNextWord", 122651)
   { loadScores(); }


   // MEHTODS
   virtual void loadScores();
   virtual void saveScores();
   void computeAverageFeatureWeights(int round);
   SCORE_TYPE dotProduct(const CWeight &w);
 
};

};
};

#endif
