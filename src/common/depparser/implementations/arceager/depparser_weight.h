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
typedef CPackedScoreMap<CWord, SCORE_TYPE, action::MAX> CWordMap;
typedef CPackedScoreMap<CTag, SCORE_TYPE, action::MAX> CTagMap;
typedef CPackedScoreMap<CTagSet<CTag, 2>,  SCORE_TYPE, action::MAX> CTagSet2Map;
typedef CPackedScoreMap<CTagSet<CTag, 3>, SCORE_TYPE, action::MAX> CTagSet3Map;
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
   CTagSet3Map m_mapSTHtSTtN0t;
   CTagSet3Map m_mapSTtSTLDtN0t;
   CTagSet3Map m_mapSTtSTRDtN0t;

   CWordTagMap m_mapN0wN1t;
   CWordTagTagMap m_mapN0wN1tN2t;
   CWordTagTagMap m_mapSTtN0wN1t;
   CWordTagTagMap m_mapSTtN0wN0LDt;
   CWordTagTagMap m_mapSTHtSTtN0w;
   CWordTagTagMap m_mapSTtSTLDtN0w;
   CWordTagTagMap m_mapSTtSTRDtN0w;

   CWordIntMap m_mapSTwd;
   CTagIntMap m_mapSTtd;
   CWordIntMap m_mapN0wd;
   CTagIntMap m_mapN0td;
   CWordWordIntMap m_mapSTwN0wd;
   CTagTagIntMap m_mapSTtN0td;

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

                                               m_mapSTwd("StackWordDist", DEP_TABLE_SIZE),
                                               m_mapSTtd("StackTagDist", DEP_TABLE_SIZE),
                                               m_mapN0wd("NextWordDist", DEP_TABLE_SIZE),
                                               m_mapN0td("NextTagDist", DEP_TABLE_SIZE),
                                               m_mapSTwN0wd("StackWordNextWordDist", DEP_TABLE_SIZE),
                                               m_mapSTtN0td("StackTagNextTagDist", DEP_TABLE_SIZE),

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


   // MEHTODS
   virtual void loadScores();
   virtual void saveScores();
   void computeAverageFeatureWeights(int round);
   SCORE_TYPE dotProduct(const CWeight &w);
 
};

};
};

#endif
