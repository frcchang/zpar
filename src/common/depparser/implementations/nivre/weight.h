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

const static unsigned TABLE_SIZE = 65536;//1000121;

namespace TARGET_LANGUAGE {

namespace depparser {

#include "macros.h"

//
// TYPE DEFINITIONS
//
typedef CScoreMap<CWord, SCORE_TYPE> CWordMap;
typedef CScoreMap<CTaggedWord<CTag, TAG_SEPARATOR>, SCORE_TYPE> CTaggedWordMap;
typedef CScoreMap<pair<CWord, int>,  SCORE_TYPE> CWordIntMap;
typedef CScoreMap<pair<CTag, int>,  SCORE_TYPE> CTagIntMap;
typedef CScoreMap<pair<CTagSet<CTag, 2>, int>,  SCORE_TYPE> CTagSet2IntMap;
typedef CScoreMap<pair<CTagSet<CTag, 3>, int>,  SCORE_TYPE> CTagSet3IntMap;
typedef CScoreMap<pair<CTagSet<CTag, 4>, int>,  SCORE_TYPE> CTagSet4IntMap;
typedef CScoreMap<CTuple3<CWord, CTag, unsigned long>, SCORE_TYPE> CWordTagIntMap;
typedef CScoreMap<CTuple4<CWord, CTag, CTag, unsigned long>, SCORE_TYPE> CWordTagTagIntMap;
typedef CScoreMap<pair<CTaggedWord<CTag, TAG_SEPARATOR>, int>,  SCORE_TYPE> CTaggedWordIntMap;
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
   CTagIntMap m_mapHeadTag;
   CTagIntMap m_mapDepTag;
   CTaggedWordIntMap m_mapHeadWordTag;
   CTaggedWordIntMap m_mapDepWordTag;

   CTwoTaggedWordsIntMap m_mapHeadWordTagDepWordTag;
   CTwoTaggedWordsIntMap m_mapHeadWordTagDepWord;
   CTwoTaggedWordsIntMap m_mapHeadWordDepWordTag;
   CTwoTaggedWordsIntMap m_mapHeadTagDepWordTag;
   CTwoTaggedWordsIntMap m_mapHeadWordTagDepTag;
   CTwoWordsIntMap m_mapHeadWordDepWord;
   CTagSet2IntMap m_mapHeadTagDepTag;

   CTagSet3IntMap m_mapBetweenTags;
   CTagSet4IntMap m_mapSurroundingTagsLL;
   CTagSet4IntMap m_mapSurroundingTagsLR;
   CTagSet4IntMap m_mapSurroundingTagsRL;
   CTagSet4IntMap m_mapSurroundingTagsRR;

   // second order feature templates from mcdonald
   CTwoWordsIntMap m_mapSiblingWords;
   CTaggedWordIntMap m_mapSiblingWordTag;
   CTaggedWordIntMap m_mapSiblingTagWord;
   CTagSet2IntMap m_mapSiblingTags;
   CTagSet3IntMap m_mapSiblingAndParentTags;

   // new features
   CTagSet3IntMap m_mapGrandChildTags; 
   CTaggedWordIntMap m_mapHeadWordTagArity;
   CTagIntMap m_mapHeadTagArity;
   CTagSet3IntMap m_mapTwoSiblingTags;
   CTagSet4IntMap m_mapTwoSiblingAndParentTags;

   // feature templates from mcdonald
   CWordIntMap m_mapSTw;
   CTagIntMap m_mapSTt;
   CTaggedWordIntMap m_mapSTwt;
   CWordIntMap m_mapN0w;
   CTagIntMap m_mapN0t;
   CTaggedWordIntMap m_mapN0wt;
   CWordIntMap m_mapN1w;
   CTagIntMap m_mapN1t;
   CTaggedWordIntMap m_mapN1wt;

   CTwoTaggedWordsIntMap m_mapSTwtN0wt;
   CTwoTaggedWordsIntMap m_mapSTwtN0w;
   CTwoTaggedWordsIntMap m_mapSTwN0wt;
   CTwoTaggedWordsIntMap m_mapSTtN0wt;
   CTwoTaggedWordsIntMap m_mapSTwtN0t;
   CTwoWordsIntMap m_mapSTwN0w;
   CTagSet2IntMap m_mapSTtN0t;

   CTagSet2IntMap m_mapN0tN1t;
   CTagSet3IntMap m_mapN0tN1tN2t;
   CTagSet3IntMap m_mapSTtN0tN1t;
   CTagSet3IntMap m_mapSTtN0tN0LDt;
   CTagSet3IntMap m_mapSTHtSTtN0t;
   CTagSet3IntMap m_mapSTtSTLDtN0t;
   CTagSet3IntMap m_mapSTtSTRDtN0t;

   CWordTagIntMap m_mapN0wN1t;
   CWordTagTagIntMap m_mapN0wN1tN2t;
   CWordTagTagIntMap m_mapSTtN0wN1t;
   CWordTagTagIntMap m_mapSTtN0wN0LDt;
   CWordTagTagIntMap m_mapSTHtSTtN0w;
   CWordTagTagIntMap m_mapSTtSTLDtN0w;
   CWordTagTagIntMap m_mapSTtSTRDtN0w;

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

                                               m_mapHeadWord("HeadWord", TABLE_SIZE),
                                               m_mapDepWord("DepWord", TABLE_SIZE),
                                               m_mapHeadTag("HeadTag", TABLE_SIZE),
                                               m_mapDepTag("DepTag", TABLE_SIZE),
                                               m_mapHeadWordTag("HeadWordTag", TABLE_SIZE),
                                               m_mapDepWordTag("DepWordTag", TABLE_SIZE),

                                               m_mapHeadWordTagDepWordTag("HeadWordTagDepWordTag", TABLE_SIZE),
                                               m_mapHeadWordTagDepWord("HeadWordTagDepWord", TABLE_SIZE),
                                               m_mapHeadWordDepWordTag("HeadWordDepWordTag", TABLE_SIZE),
                                               m_mapHeadTagDepWordTag("HeadTagDepWordTag", TABLE_SIZE),
                                               m_mapHeadWordTagDepTag("HeadWordTagDepTag", TABLE_SIZE),
                                               m_mapHeadWordDepWord("HeadWordDepWord", TABLE_SIZE),
                                               m_mapHeadTagDepTag("HeadTagDepTag", TABLE_SIZE),

                                               m_mapBetweenTags("BetweenTags", TABLE_SIZE),
                                               m_mapSurroundingTagsLL("SurroundingTagsLL", TABLE_SIZE),
                                               m_mapSurroundingTagsLR("SurroundingTagsLR", TABLE_SIZE),
                                               m_mapSurroundingTagsRL("SurroundingTagsRL", TABLE_SIZE),
                                               m_mapSurroundingTagsRR("SurroundingTagsRR", TABLE_SIZE),

                                               m_mapSiblingWords("SiblingWords", TABLE_SIZE),
                                               m_mapSiblingWordTag("SiblingWordTag", TABLE_SIZE),
                                               m_mapSiblingTagWord("SiblingTagWord", TABLE_SIZE),
                                               m_mapSiblingTags("SiblingTags", TABLE_SIZE),
                                               m_mapSiblingAndParentTags("SiblingAndParentTags", TABLE_SIZE),

                                               m_mapTwoSiblingTags("TwoSiblingTags", TABLE_SIZE),
                                               m_mapTwoSiblingAndParentTags("TwoSiblingAndParentTags", TABLE_SIZE),
                                               m_mapGrandChildTags("GrandChildTags", TABLE_SIZE), 
                                               m_mapHeadWordTagArity("HeadWordTagArity", TABLE_SIZE),
                                               m_mapHeadTagArity("HeadTagArity", TABLE_SIZE),

#ifdef LABELED
                                               m_mapLabel("Label", TABLE_SIZE),
                                               m_mapHeadWordLabel("HeadWordLabel", TABLE_SIZE),
                                               m_mapDepWordLabel("DepWordLabel", TABLE_SIZE),
                                               m_mapHeadWordTagLabel("HeadWordTagLabel", TABLE_SIZE),
                                               m_mapDepWordTagLabel("DepWordTagLabel", TABLE_SIZE),
                                               m_mapHeadTagLabel("HeadTagLabel", TABLE_SIZE),
                                               m_mapDepTagLabel("DepTagLabel", TABLE_SIZE),
                                               m_mapHeadSurroundingTagsLabel("HeadSurroundingTagsLabel", TABLE_SIZE),
                                               m_mapDepSurroundingTagsLabel("DepSurroundingTagsLabel", TABLE_SIZE),
#endif
                                               m_mapSTwt("StackWordTag", TABLE_SIZE),
                                               m_mapSTw("StackWord", TABLE_SIZE),
                                               m_mapSTt("StackTag", TABLE_SIZE),

                                               m_mapN0wt("NextWordTag", TABLE_SIZE),
                                               m_mapN0w("NextWord", TABLE_SIZE),
                                               m_mapN0t("NextTag", TABLE_SIZE),

                                               m_mapN1wt("Next+1WordTag", TABLE_SIZE),
                                               m_mapN1w("Next+1Word", TABLE_SIZE),
                                               m_mapN1t("Next+1Tag", TABLE_SIZE),

                                               m_mapSTwtN0wt("StackWordTagNextWordTag", TABLE_SIZE),
                                               m_mapSTwtN0w("StackWordTagNextWord", TABLE_SIZE),
                                               m_mapSTwN0wt("StackWordNextWordTag", TABLE_SIZE),
                                               m_mapSTwtN0t("StackWordTagNextTag", TABLE_SIZE),
                                               m_mapSTtN0wt("StackTagNextWordTag", TABLE_SIZE),
                                               m_mapSTwN0w("StackWordNextWord", TABLE_SIZE),
                                               m_mapSTtN0t("StackTagNextTag", TABLE_SIZE),

                                               m_mapN0tN1t("NextTagNext+1Tag", TABLE_SIZE),
                                               m_mapN0tN1tN2t("NextTagTrigram", TABLE_SIZE),
                                               m_mapSTtN0tN1t("StackTagNextTagNext+1Tag", TABLE_SIZE),
                                               m_mapSTtN0tN0LDt("StackTagNextTagNextLDTag", TABLE_SIZE),
                                               m_mapSTHtSTtN0t("StackHeadTagStackTagNextTag", TABLE_SIZE),
                                               m_mapSTtSTLDtN0t("StackTagStackLDTagNextTag", TABLE_SIZE),
                                               m_mapSTtSTRDtN0t("StackTagStackRDTagNextTag", TABLE_SIZE),

                                               m_mapN0wN1t("NextWordNext+1Tag", TABLE_SIZE),
                                               m_mapN0wN1tN2t("NextWordNext+1TagNext+2Tag", TABLE_SIZE),
                                               m_mapSTtN0wN1t("StackTagNextWordNext+1Tag", TABLE_SIZE),
                                               m_mapSTtN0wN0LDt("StackTagNextWordNextLDTag", TABLE_SIZE),
                                               m_mapSTHtSTtN0w("StackHeadTagStackTagNextWord", TABLE_SIZE),
                                               m_mapSTtSTLDtN0w("StackTagStackLDTagNextWord", TABLE_SIZE),
                                               m_mapSTtSTRDtN0w("StackTagStackRDTagNextWord", TABLE_SIZE)
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
