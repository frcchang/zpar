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

#define iterate_templates(left, right) \
   left(m_mapSTw)right\
   left(m_mapSTt)right\
   left(m_mapSTwt)right\
   left(m_mapN0w)right\
   left(m_mapN0t)right\
   left(m_mapN0wt)right\
   left(m_mapN1w)right\
   left(m_mapN1t)right\
   left(m_mapN1wt)right\
   left(m_mapN2w)right\
   left(m_mapN2t)right\
   left(m_mapN2wt)right\
   left(m_mapSTHw)right\
   left(m_mapSTHt)right\
   left(m_mapSTi)right\
   left(m_mapSTHHw)right\
   left(m_mapSTHHt)right\
   left(m_mapSTHi)right\
   left(m_mapSTLDw)right\
   left(m_mapSTLDt)right\
   left(m_mapSTLDi)right\
   left(m_mapSTRDw)right\
   left(m_mapSTRDt)right\
   left(m_mapSTRDi)right\
   left(m_mapN0LDw)right\
   left(m_mapN0LDt)right\
   left(m_mapN0LDi)right\
   left(m_mapSTL2Dw)right\
   left(m_mapSTL2Dt)right\
   left(m_mapSTL2Di)right\
   left(m_mapSTR2Dw)right\
   left(m_mapSTR2Dt)right\
   left(m_mapSTR2Di)right\
   left(m_mapN0L2Dw)right\
   left(m_mapN0L2Dt)right\
   left(m_mapN0L2Di)right\
   left(m_mapHTw)right\
   left(m_mapHTt)right\
   left(m_mapHTwt)right\
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
   left(m_mapN0tN0LDtN0L2Dt)right\
   left(m_mapSTHtSTtN0t)right\
   left(m_mapHTtHT2tN0t)right\
   left(m_mapSTHHtSTHtSTt)right\
   left(m_mapSTtSTLDtN0t)right\
   left(m_mapSTtSTLDtSTL2Dt)right\
   left(m_mapSTtSTRDtN0t)right\
   left(m_mapSTtSTRDtSTR2Dt)right\
\
   left(m_mapSTwd)right\
   left(m_mapSTtd)right\
   left(m_mapN0wd)right\
   left(m_mapN0td)right\
   left(m_mapSTwN0wd)right\
   left(m_mapSTtN0td)right\
\
   left(m_mapSTwra)right\
   left(m_mapSTtra)right\
   left(m_mapSTwla)right\
   left(m_mapSTtla)right\
   left(m_mapN0wla)right\
   left(m_mapN0tla)right\
\
   left(m_mapSTwrp)right\
   left(m_mapSTtrp)right\
   left(m_mapSTwlp)right\
   left(m_mapSTtlp)right\
   left(m_mapN0wlp)right\
   left(m_mapN0tlp)right\
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
   left(m_mapN1f)right

namespace TARGET_LANGUAGE {

namespace depparser {

const static unsigned DEP_TABLE_SIZE = (1<<17);//1000121;

//
// TYPE DEFINITIONS
//
typedef CPackedScoreMap<CWord, SCORE_TYPE, action::MAX> CWordMap;
typedef CPackedScoreMap<CTag, SCORE_TYPE, action::MAX> CTagMap;
typedef CPackedScoreMap<int, SCORE_TYPE, action::MAX> CIntMap;
typedef CPackedScoreMap<CTagSet<CTag, 2>,  SCORE_TYPE, action::MAX> CTagSet2Map;
typedef CPackedScoreMap<CTagSet<CTag, 3>, SCORE_TYPE, action::MAX> CTagSet3Map;
typedef CPackedScoreMap<CTagSet<CTag, 4>, SCORE_TYPE, action::MAX> CTagSet4Map;
typedef CPackedScoreMap<CTuple2<CWord, CTag>, SCORE_TYPE, action::MAX> CWordTagMap;
typedef CPackedScoreMap<CTuple3<CWord, CTag, CTag>, SCORE_TYPE, action::MAX> CWordTagTagMap;
typedef CPackedScoreMap<CTuple3<CWord, CWord, CTag>, SCORE_TYPE, action::MAX> CWordWordTagMap;
typedef CPackedScoreMap<CTaggedWord<CTag, TAG_SEPARATOR>, SCORE_TYPE, action::MAX> CTaggedWordMap;
typedef CPackedScoreMap<CTwoWords, SCORE_TYPE, action::MAX> CTwoWordsMap;
typedef CPackedScoreMap<CTwoTaggedWords, SCORE_TYPE, action::MAX> CTwoTaggedWordsMap;
typedef CPackedScoreMap<CTuple2<CWord, int>, SCORE_TYPE, action::MAX> CWordIntMap;
typedef CPackedScoreMap<CTuple2<CTag, int>, SCORE_TYPE, action::MAX> CTagIntMap;
typedef CPackedScoreMap<CTuple3<CTag, CTag, int>, SCORE_TYPE, action::MAX> CTagTagIntMap;
typedef CPackedScoreMap<CTuple3<CWord, CWord, int>, SCORE_TYPE, action::MAX> CWordWordIntMap;
typedef CPackedScoreMap<CTuple2< CWord, CSetOfTags<CDependencyLabel> >, SCORE_TYPE, action::MAX> CWordSetOfLabelsMap;
typedef CPackedScoreMap<CTuple2< CTag, CSetOfTags<CDependencyLabel> >, SCORE_TYPE, action::MAX> CTagSetOfLabelsMap;

typedef CPackedScoreMap<CLemma, SCORE_TYPE, action::MAX> CLemmaMap;
typedef CPackedScoreMap<CCoNLLCPOS, SCORE_TYPE, action::MAX> CCoNLLCPOSMap;
typedef CPackedScoreMap<CCoNLLFeats, SCORE_TYPE, action::MAX> CCoNLLFeatsMap;

/*===============================================================
 *
 * CWeight - the definition of weights, in feature
 *
 *==============================================================*/

class CWeight : public CWeightBase {

public:

   CWordMap m_mapSTw;
   CTagMap m_mapSTt;
   CTaggedWordMap m_mapSTwt;

   CWordMap m_mapN0w;
   CTagMap m_mapN0t;
   CTaggedWordMap m_mapN0wt;

   CWordMap m_mapN1w;
   CTagMap m_mapN1t;
   CTaggedWordMap m_mapN1wt;

   CWordMap m_mapN2w;
   CTagMap m_mapN2t;
   CTaggedWordMap m_mapN2wt;

   CWordMap m_mapSTHw;
   CTagMap m_mapSTHt;
   CIntMap m_mapSTi;

   CWordMap m_mapSTHHw;
   CTagMap m_mapSTHHt;
   CIntMap m_mapSTHi;

   CWordMap m_mapSTLDw;
   CTagMap m_mapSTLDt;
   CIntMap m_mapSTLDi;

   CWordMap m_mapSTRDw;
   CTagMap m_mapSTRDt;
   CIntMap m_mapSTRDi;

   CWordMap m_mapN0LDw;
   CTagMap m_mapN0LDt;
   CIntMap m_mapN0LDi;

   CWordMap m_mapSTL2Dw;
   CTagMap m_mapSTL2Dt;
   CIntMap m_mapSTL2Di;

   CWordMap m_mapSTR2Dw;
   CTagMap m_mapSTR2Dt;
   CIntMap m_mapSTR2Di;

   CWordMap m_mapN0L2Dw;
   CTagMap m_mapN0L2Dt;
   CIntMap m_mapN0L2Di;

   CWordMap m_mapHTw;
   CTagMap m_mapHTt;
   CTaggedWordMap m_mapHTwt;

   CTwoTaggedWordsMap m_mapSTwtN0wt;
   CWordWordTagMap m_mapSTwtN0w;
   CWordWordTagMap m_mapSTwN0wt;
   CWordTagTagMap m_mapSTtN0wt;
   CWordTagTagMap m_mapSTwtN0t;
   CTwoWordsMap m_mapSTwN0w;
   CTagSet2Map m_mapSTtN0t;

   CTagSet2Map m_mapN0tN1t;
   CTagSet3Map m_mapN0tN1tN2t;
   CTagSet3Map m_mapSTtN0tN1t;
   CTagSet3Map m_mapSTtN0tN0LDt;
   CTagSet3Map m_mapN0tN0LDtN0L2Dt;
   CTagSet3Map m_mapSTHtSTtN0t;
   CTagSet3Map m_mapHTtHT2tN0t;
   CTagSet3Map m_mapSTHHtSTHtSTt;
   CTagSet3Map m_mapSTtSTLDtN0t;
   CTagSet3Map m_mapSTtSTLDtSTL2Dt;
   CTagSet3Map m_mapSTtSTRDtN0t;
   CTagSet3Map m_mapSTtSTRDtSTR2Dt;

   CWordIntMap m_mapSTwd;
   CTagIntMap m_mapSTtd;
   CWordIntMap m_mapN0wd;
   CTagIntMap m_mapN0td;
   CWordWordIntMap m_mapSTwN0wd;
   CTagTagIntMap m_mapSTtN0td;

   CWordIntMap m_mapSTwra;
   CTagIntMap m_mapSTtra;
   CWordIntMap m_mapSTwla;
   CTagIntMap m_mapSTtla;
   CWordIntMap m_mapN0wla;
   CTagIntMap m_mapN0tla;

   CWordSetOfLabelsMap m_mapSTwrp;
   CTagSetOfLabelsMap m_mapSTtrp;
   CWordSetOfLabelsMap m_mapSTwlp;
   CTagSetOfLabelsMap m_mapSTtlp;
   CWordSetOfLabelsMap m_mapN0wlp;
   CTagSetOfLabelsMap m_mapN0tlp;

   CLemmaMap m_mapSTl;
   CCoNLLCPOSMap m_mapSTc;
   CCoNLLFeatsMap m_mapSTf;

   CLemmaMap m_mapN0l;
   CCoNLLCPOSMap m_mapN0c;
   CCoNLLFeatsMap m_mapN0f;

   CLemmaMap m_mapN1l;
   CCoNLLCPOSMap m_mapN1c;
   CCoNLLFeatsMap m_mapN1f;

public:

   CWeight(const std::string &sPath, bool bTrain) : CWeightBase(sPath, bTrain) ,

                                               m_mapSTw("StackWord", DEP_TABLE_SIZE),
                                               m_mapSTt("StackTag", DEP_TABLE_SIZE),
                                               m_mapSTwt("StackWordTag", DEP_TABLE_SIZE),

                                               m_mapN0w("NextWord", DEP_TABLE_SIZE),
                                               m_mapN0t("NextTag", DEP_TABLE_SIZE),
                                               m_mapN0wt("NextWordTag", DEP_TABLE_SIZE),

                                               m_mapN1w("Next+1Word", DEP_TABLE_SIZE),
                                               m_mapN1t("Next+1Tag", DEP_TABLE_SIZE),
                                               m_mapN1wt("Next+1WordTag", DEP_TABLE_SIZE),

                                               m_mapN2w("Next+2Word", DEP_TABLE_SIZE),
                                               m_mapN2t("Next+2Tag", DEP_TABLE_SIZE),
                                               m_mapN2wt("Next+2WordTag", DEP_TABLE_SIZE),

                                               m_mapSTHw("StackHeadWord", DEP_TABLE_SIZE),
                                               m_mapSTHt("StackHeadTag", DEP_TABLE_SIZE),
                                               m_mapSTi("StackLabel", DEP_TABLE_SIZE),

                                               m_mapSTHHw("StackHeadHeadWord", DEP_TABLE_SIZE),
                                               m_mapSTHHt("StackHeadHeadTag", DEP_TABLE_SIZE),
                                               m_mapSTHi("StackLabel", DEP_TABLE_SIZE),

                                               m_mapSTLDw("StackLDWord", DEP_TABLE_SIZE),
                                               m_mapSTLDt("StackLDTag", DEP_TABLE_SIZE),
                                               m_mapSTLDi("StackLDLabel", DEP_TABLE_SIZE),

                                               m_mapSTRDw("StackRDWord", DEP_TABLE_SIZE),
                                               m_mapSTRDt("StackRDTag", DEP_TABLE_SIZE),
                                               m_mapSTRDi("StackRDLabel", DEP_TABLE_SIZE),

                                               m_mapN0LDw("NextLDWord", DEP_TABLE_SIZE),
                                               m_mapN0LDt("NextLDTag", DEP_TABLE_SIZE),
                                               m_mapN0LDi("NextLDLabel", DEP_TABLE_SIZE),

                                               m_mapSTL2Dw("StackL2DWord", DEP_TABLE_SIZE),
                                               m_mapSTL2Dt("StackL2DTag", DEP_TABLE_SIZE),
                                               m_mapSTL2Di("StackL2DLabel", DEP_TABLE_SIZE),

                                               m_mapSTR2Dw("StackR2DWord", DEP_TABLE_SIZE),
                                               m_mapSTR2Dt("StackR2DTag", DEP_TABLE_SIZE),
                                               m_mapSTR2Di("StackR2DLabel", DEP_TABLE_SIZE),

                                               m_mapN0L2Dw("NextL2DWord", DEP_TABLE_SIZE),
                                               m_mapN0L2Dt("NextL2DTag", DEP_TABLE_SIZE),
                                               m_mapN0L2Di("NextL2DLabel", DEP_TABLE_SIZE),

                                               m_mapHTw("HeadStackWord", DEP_TABLE_SIZE),
                                               m_mapHTt("HeadStackTag", DEP_TABLE_SIZE),
                                               m_mapHTwt("HeadStackWordTag", DEP_TABLE_SIZE),

                                               m_mapSTwtN0wt("StackWordTagNextWordTag", DEP_TABLE_SIZE),
                                               m_mapSTwtN0w("StackWordTagNextWord", DEP_TABLE_SIZE),
                                               m_mapSTwN0wt("StackWordNextWordTag", DEP_TABLE_SIZE),
                                               m_mapSTtN0wt("StackTagNextWordTag", DEP_TABLE_SIZE),
                                               m_mapSTwtN0t("StackWordTagNextTag", DEP_TABLE_SIZE),
                                               m_mapSTwN0w("StackWordNextWord", DEP_TABLE_SIZE),
                                               m_mapSTtN0t("StackTagNextTag", DEP_TABLE_SIZE),

                                               m_mapN0tN1t("NextTagNext+1Tag", DEP_TABLE_SIZE),
                                               m_mapN0tN1tN2t("NextTagTrigram", DEP_TABLE_SIZE),
                                               m_mapSTtN0tN1t("StackTagNextTagNext+1Tag", DEP_TABLE_SIZE),
                                               m_mapSTtN0tN0LDt("StackTagNextTagNextLDTag", DEP_TABLE_SIZE),
                                               m_mapN0tN0LDtN0L2Dt("StackTagNextTagNextLDTagNextTagNextL2DTag", DEP_TABLE_SIZE),
                                               m_mapSTHtSTtN0t("StackHeadTagStackTagNextTag", DEP_TABLE_SIZE),
                                               m_mapHTtHT2tN0t("HeadStackTagHeadStack2TagNextTag", DEP_TABLE_SIZE),
                                               m_mapSTHHtSTHtSTt("StackHeadHeadTagStackHeadTagStackTag", DEP_TABLE_SIZE),
                                               m_mapSTtSTLDtN0t("StackTagStackLDTagNextTag", DEP_TABLE_SIZE),
                                               m_mapSTtSTLDtSTL2Dt("StackTagStackLDTagStackL2DTag", DEP_TABLE_SIZE),
                                               m_mapSTtSTRDtN0t("StackTagStackRDTagNextTag", DEP_TABLE_SIZE),
                                               m_mapSTtSTRDtSTR2Dt("StackTagStackRDTagStackR2DTag", DEP_TABLE_SIZE),

                                               m_mapSTwd("StackWordDist", DEP_TABLE_SIZE),
                                               m_mapSTtd("StackTagDist", DEP_TABLE_SIZE),
                                               m_mapN0wd("NextWordDist", DEP_TABLE_SIZE),
                                               m_mapN0td("NextTagDist", DEP_TABLE_SIZE),
                                               m_mapSTwN0wd("StackWordNextWordDist", DEP_TABLE_SIZE),
                                               m_mapSTtN0td("StackTagNextTagDist", DEP_TABLE_SIZE),

                                               m_mapSTwra("StackWordRightArity", DEP_TABLE_SIZE),
                                               m_mapSTtra("StackTagRightArity", DEP_TABLE_SIZE),
                                               m_mapSTwla("StackWordLeftArity", DEP_TABLE_SIZE),
                                               m_mapSTtla("StackTagLeftArity", DEP_TABLE_SIZE),
                                               m_mapN0wla("NextWordRightArity", DEP_TABLE_SIZE),
                                               m_mapN0tla("NextTagRightArity", DEP_TABLE_SIZE),

                                               m_mapSTwrp("StackWordRightSetoftags", DEP_TABLE_SIZE),
                                               m_mapSTtrp("StackTagRightSetoftags", DEP_TABLE_SIZE),
                                               m_mapSTwlp("StackWordLeftSetoftags", DEP_TABLE_SIZE),
                                               m_mapSTtlp("StackTagLeftSetoftags", DEP_TABLE_SIZE),
                                               m_mapN0wlp("Next0WordLeftSetoftags", DEP_TABLE_SIZE),
                                               m_mapN0tlp("Next0TagLeftSetoftags", DEP_TABLE_SIZE),

                                               m_mapSTl("StackLemma", DEP_TABLE_SIZE),
                                               m_mapSTc("StackCPOS", DEP_TABLE_SIZE),
                                               m_mapSTf("StackFeats", DEP_TABLE_SIZE),

                                               m_mapN0l("NextLemma", DEP_TABLE_SIZE),
                                               m_mapN0c("NextCPOS", DEP_TABLE_SIZE),
                                               m_mapN0f("NextFeats", DEP_TABLE_SIZE),

                                               m_mapN1l("Next+1Lemma", DEP_TABLE_SIZE),
                                               m_mapN1c("Next+1CPOS", DEP_TABLE_SIZE),
                                               m_mapN1f("Next+1Feats", DEP_TABLE_SIZE)
   { loadScores(); }

   virtual ~CWeight() {
//      iterate_templates(,.freePoolMemory(););
//      CPackedScore<SCORE_TYPE, action::MAX>::freePoolMemory();
   }

   // MEHTODS
   virtual void loadScores();
   virtual void saveScores();
   void computeAverageFeatureWeights(int round);
   SCORE_TYPE dotProduct(const CWeight &w);
 
};

};
};

#endif
