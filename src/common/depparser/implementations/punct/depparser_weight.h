// Copyright (C) University of Oxford 2010
/***************************************************************
 *
 * weight.h - the dependency parser weights 
 *
 * Yue Zhang, 2007.8 - 2008.1
 *
 ***************************************************************/

#ifndef _DEPPARSER_WEIGHTS_H
#define _DEPPARSER_WEIGHTS_H

#include "depparser_weight_base.h"

namespace TARGET_LANGUAGE {

namespace depparser {

const static unsigned DEP_TABLE_SIZE = (1<<17);//1000121;

//
// TYPE DEFINITIONS
//
typedef CScoreMap<CWord, SCORE_TYPE> CWordMap;
typedef CScoreMap<CTaggedWord<CTag, TAG_SEPARATOR>, SCORE_TYPE> CTaggedWordMap;
typedef CScoreMap<std::pair<CWord, int>,  SCORE_TYPE> CWordIntMap;
typedef CScoreMap<std::pair<CTag, int>,  SCORE_TYPE> CTagIntMap;
typedef CScoreMap<std::pair<CTagSet<CTag, 2>, int>,  SCORE_TYPE> CTagSet2IntMap;
typedef CScoreMap<std::pair<CTagSet<CTag, 3>, int>,  SCORE_TYPE> CTagSet3IntMap;
typedef CScoreMap<std::pair<CTagSet<CTag, 4>, int>,  SCORE_TYPE> CTagSet4IntMap;
typedef CScoreMap<CTuple3<CWord, CTag, unsigned long>, SCORE_TYPE> CWordTagIntMap;
typedef CScoreMap<CTuple4<CWord, CTag, CTag, unsigned long>, SCORE_TYPE> CWordTagTagIntMap;
typedef CScoreMap<std::pair<CTaggedWord<CTag, TAG_SEPARATOR>, int>,  SCORE_TYPE> CTaggedWordIntMap;
typedef CScoreMap<int, SCORE_TYPE> CIntMap;
typedef CScoreMap<std::pair<int, int>, SCORE_TYPE> CTwoIntMap;
typedef CScoreMap<CTwoWords, SCORE_TYPE> CTwoWordsMap;
typedef CScoreMap<std::pair<CTwoWords, int>, SCORE_TYPE> CTwoWordsIntMap;
typedef CScoreMap<CTwoTaggedWords, SCORE_TYPE> CTwoTaggedWordsMap;
typedef CScoreMap<std::pair<CTwoTaggedWords, int>, SCORE_TYPE> CTwoTaggedWordsIntMap;

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

   // punct
   CWordIntMap m_mapSTwp;
   CTagIntMap m_mapSTtp;
   CTaggedWordIntMap m_mapSTwtp;

   CWordIntMap m_mapN0wp;
   CTagIntMap m_mapN0tp;
   CTaggedWordIntMap m_mapN0wtp;

   CWordIntMap m_mapSTwln;
   CTagIntMap m_mapSTtln;
   CTaggedWordIntMap m_mapSTwtln;

   CWordIntMap m_mapSTwrn;
   CTagIntMap m_mapSTtrn;
   CTaggedWordIntMap m_mapSTwtrn;

   CWordIntMap m_mapN0wln;
   CTagIntMap m_mapN0tln;
   CTaggedWordIntMap m_mapN0wtln;

   CWordIntMap m_mapSTwlp;
   CTagIntMap m_mapSTtlp;
   CTaggedWordIntMap m_mapSTwtlp;
   CWordIntMap m_mapN0wrp;
   CTagIntMap m_mapN0trp;
   CTaggedWordIntMap m_mapN0wtrp;

   CTwoTaggedWordsIntMap m_mapSTwtN0wtp;
   CTwoTaggedWordsIntMap m_mapSTwtN0wp;
   CTwoTaggedWordsIntMap m_mapSTwN0wtp;
   CTwoTaggedWordsIntMap m_mapSTtN0wtp;
   CTwoTaggedWordsIntMap m_mapSTwtN0tp;
   CTwoWordsIntMap m_mapSTwN0wp;
   CTagSet2IntMap m_mapSTtN0tp;

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

   CWeight(const std::string &sPath, bool bTrain) : CWeightBase(sPath, bTrain) ,

                                               m_mapHeadWord("HeadWord", DEP_TABLE_SIZE),
                                               m_mapDepWord("DepWord", DEP_TABLE_SIZE),
                                               m_mapHeadTag("HeadTag", DEP_TABLE_SIZE),
                                               m_mapDepTag("DepTag", DEP_TABLE_SIZE),
                                               m_mapHeadWordTag("HeadWordTag", DEP_TABLE_SIZE),
                                               m_mapDepWordTag("DepWordTag", DEP_TABLE_SIZE),

                                               m_mapHeadWordTagDepWordTag("HeadWordTagDepWordTag", DEP_TABLE_SIZE),
                                               m_mapHeadWordTagDepWord("HeadWordTagDepWord", DEP_TABLE_SIZE),
                                               m_mapHeadWordDepWordTag("HeadWordDepWordTag", DEP_TABLE_SIZE),
                                               m_mapHeadTagDepWordTag("HeadTagDepWordTag", DEP_TABLE_SIZE),
                                               m_mapHeadWordTagDepTag("HeadWordTagDepTag", DEP_TABLE_SIZE),
                                               m_mapHeadWordDepWord("HeadWordDepWord", DEP_TABLE_SIZE),
                                               m_mapHeadTagDepTag("HeadTagDepTag", DEP_TABLE_SIZE),

                                               m_mapBetweenTags("BetweenTags", DEP_TABLE_SIZE),
                                               m_mapSurroundingTagsLL("SurroundingTagsLL", DEP_TABLE_SIZE),
                                               m_mapSurroundingTagsLR("SurroundingTagsLR", DEP_TABLE_SIZE),
                                               m_mapSurroundingTagsRL("SurroundingTagsRL", DEP_TABLE_SIZE),
                                               m_mapSurroundingTagsRR("SurroundingTagsRR", DEP_TABLE_SIZE),

                                               m_mapSiblingWords("SiblingWords", DEP_TABLE_SIZE),
                                               m_mapSiblingWordTag("SiblingWordTag", DEP_TABLE_SIZE),
                                               m_mapSiblingTagWord("SiblingTagWord", DEP_TABLE_SIZE),
                                               m_mapSiblingTags("SiblingTags", DEP_TABLE_SIZE),
                                               m_mapSiblingAndParentTags("SiblingAndParentTags", DEP_TABLE_SIZE),

                                               m_mapTwoSiblingTags("TwoSiblingTags", DEP_TABLE_SIZE),
                                               m_mapTwoSiblingAndParentTags("TwoSiblingAndParentTags", DEP_TABLE_SIZE),
                                               m_mapGrandChildTags("GrandChildTags", DEP_TABLE_SIZE), 
                                               m_mapHeadWordTagArity("HeadWordTagArity", DEP_TABLE_SIZE),
                                               m_mapHeadTagArity("HeadTagArity", DEP_TABLE_SIZE),

#ifdef LABELED
                                               m_mapLabel("Label", DEP_TABLE_SIZE),
                                               m_mapHeadWordLabel("HeadWordLabel", DEP_TABLE_SIZE),
                                               m_mapDepWordLabel("DepWordLabel", DEP_TABLE_SIZE),
                                               m_mapHeadWordTagLabel("HeadWordTagLabel", DEP_TABLE_SIZE),
                                               m_mapDepWordTagLabel("DepWordTagLabel", DEP_TABLE_SIZE),
                                               m_mapHeadTagLabel("HeadTagLabel", DEP_TABLE_SIZE),
                                               m_mapDepTagLabel("DepTagLabel", DEP_TABLE_SIZE),
                                               m_mapHeadSurroundingTagsLabel("HeadSurroundingTagsLabel", DEP_TABLE_SIZE),
                                               m_mapDepSurroundingTagsLabel("DepSurroundingTagsLabel", DEP_TABLE_SIZE),
#endif
                                               m_mapSTwt("StackWordTag", DEP_TABLE_SIZE),
                                               m_mapSTw("StackWord", DEP_TABLE_SIZE),
                                               m_mapSTt("StackTag", DEP_TABLE_SIZE),

                                               m_mapN0wt("NextWordTag", DEP_TABLE_SIZE),
                                               m_mapN0w("NextWord", DEP_TABLE_SIZE),
                                               m_mapN0t("NextTag", DEP_TABLE_SIZE),

                                               m_mapN1wt("Next+1WordTag", DEP_TABLE_SIZE),
                                               m_mapN1w("Next+1Word", DEP_TABLE_SIZE),
                                               m_mapN1t("Next+1Tag", DEP_TABLE_SIZE),

                                               m_mapSTwtN0wt("StackWordTagNextWordTag", DEP_TABLE_SIZE),
                                               m_mapSTwtN0w("StackWordTagNextWord", DEP_TABLE_SIZE),
                                               m_mapSTwN0wt("StackWordNextWordTag", DEP_TABLE_SIZE),
                                               m_mapSTwtN0t("StackWordTagNextTag", DEP_TABLE_SIZE),
                                               m_mapSTtN0wt("StackTagNextWordTag", DEP_TABLE_SIZE),
                                               m_mapSTwN0w("StackWordNextWord", DEP_TABLE_SIZE),
                                               m_mapSTtN0t("StackTagNextTag", DEP_TABLE_SIZE),

                                               m_mapN0tN1t("NextTagNext+1Tag", DEP_TABLE_SIZE),
                                               m_mapN0tN1tN2t("NextTagTrigram", DEP_TABLE_SIZE),
                                               m_mapSTtN0tN1t("StackTagNextTagNext+1Tag", DEP_TABLE_SIZE),
                                               m_mapSTtN0tN0LDt("StackTagNextTagNextLDTag", DEP_TABLE_SIZE),
                                               m_mapSTHtSTtN0t("StackHeadTagStackTagNextTag", DEP_TABLE_SIZE),
                                               m_mapSTtSTLDtN0t("StackTagStackLDTagNextTag", DEP_TABLE_SIZE),
                                               m_mapSTtSTRDtN0t("StackTagStackRDTagNextTag", DEP_TABLE_SIZE),

                                               m_mapN0wN1t("NextWordNext+1Tag", DEP_TABLE_SIZE),
                                               m_mapN0wN1tN2t("NextWordNext+1TagNext+2Tag", DEP_TABLE_SIZE),
                                               m_mapSTtN0wN1t("StackTagNextWordNext+1Tag", DEP_TABLE_SIZE),
                                               m_mapSTtN0wN0LDt("StackTagNextWordNextLDTag", DEP_TABLE_SIZE),
                                               m_mapSTHtSTtN0w("StackHeadTagStackTagNextWord", DEP_TABLE_SIZE),
                                               m_mapSTtSTLDtN0w("StackTagStackLDTagNextWord", DEP_TABLE_SIZE),
                                               m_mapSTtSTRDtN0w("StackTagStackRDTagNextWord", DEP_TABLE_SIZE), 

                                               m_mapSTwtp("StackWordTagPunct", DEP_TABLE_SIZE),
                                               m_mapSTwp("StackWordPunct", DEP_TABLE_SIZE),
                                               m_mapSTtp("StackTagPunct", DEP_TABLE_SIZE),

                                               m_mapN0wtp("NextWordTagPunct", DEP_TABLE_SIZE),
                                               m_mapN0wp("NextWordPunct", DEP_TABLE_SIZE),
                                               m_mapN0tp("NextTagPunct", DEP_TABLE_SIZE), 

                                               m_mapSTwtln("StackWordTagLeftInnerPunct", DEP_TABLE_SIZE),
                                               m_mapSTwln("StackWordLeftInnerPunct", DEP_TABLE_SIZE),
                                               m_mapSTtln("StackTagLeftInnerPunct", DEP_TABLE_SIZE),
                                               m_mapSTwtrn("StackWordTagRightInnerPunct", DEP_TABLE_SIZE),
                                               m_mapSTwrn("StackWordRightInnerPunct", DEP_TABLE_SIZE),
                                               m_mapSTtrn("StackTagRightInnerPunct", DEP_TABLE_SIZE),

                                               m_mapN0wtln("NextWordTagInnerPunct", DEP_TABLE_SIZE),
                                               m_mapN0wln("NextWordInnerPunct", DEP_TABLE_SIZE),
                                               m_mapN0tln("NextTagInnerPunct", DEP_TABLE_SIZE), 

                                               m_mapSTwtlp("StackWordTagLeftPunct", DEP_TABLE_SIZE),
                                               m_mapSTwlp("StackWordLeftPunct", DEP_TABLE_SIZE),
                                               m_mapSTtlp("StackTagLeftPunct", DEP_TABLE_SIZE),

                                               m_mapN0wtrp("NextWordTagRightPunct", DEP_TABLE_SIZE),
                                               m_mapN0wrp("NextWordRightPunct", DEP_TABLE_SIZE),
                                               m_mapN0trp("NextTagRightPunct", DEP_TABLE_SIZE), 

                                               m_mapSTwtN0wtp("StackWordTagNextWordTagPunct", DEP_TABLE_SIZE),
                                               m_mapSTwtN0wp("StackWordTagNextWordPunct", DEP_TABLE_SIZE),
                                               m_mapSTwN0wtp("StackWordNextWordTagPunct", DEP_TABLE_SIZE),
                                               m_mapSTwtN0tp("StackWordTagNextTagPunct", DEP_TABLE_SIZE),
                                               m_mapSTtN0wtp("StackTagNextWordTagPunct", DEP_TABLE_SIZE),
                                               m_mapSTwN0wp("StackWordNextWordPunct", DEP_TABLE_SIZE),
                                               m_mapSTtN0tp("StackTagNextTagPunct", DEP_TABLE_SIZE)


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
