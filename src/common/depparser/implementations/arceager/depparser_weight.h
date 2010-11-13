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
typedef CPackedScoreMap<CWord, SCORE_TYPE, action::MAX> CWordIntMap;
typedef CPackedScoreMap<CTag, SCORE_TYPE, action::MAX> CTagIntMap;
typedef CPackedScoreMap<CTagSet<CTag, 2>,  SCORE_TYPE, action::MAX> CTagSet2IntMap;
typedef CPackedScoreMap<CTagSet<CTag, 3>, SCORE_TYPE, action::MAX> CTagSet3IntMap;
typedef CPackedScoreMap<CTuple2<CWord, CTag>, SCORE_TYPE, action::MAX> CWordTagIntMap;
typedef CPackedScoreMap<CTuple3<CWord, CTag, CTag>, SCORE_TYPE, action::MAX> CWordTagTagIntMap;
typedef CPackedScoreMap<CTaggedWord<CTag, TAG_SEPARATOR>, SCORE_TYPE, action::MAX> CTaggedWordIntMap;
typedef CPackedScoreMap<CTwoWords, SCORE_TYPE, action::MAX> CTwoWordsIntMap;
typedef CPackedScoreMap<CTwoTaggedWords, SCORE_TYPE, action::MAX> CTwoTaggedWordsIntMap;

typedef CPackedScoreMap<CLemma, SCORE_TYPE, action::MAX> CLemmaIntMap;
typedef CPackedScoreMap<CCoNLLCPOS, SCORE_TYPE, action::MAX> CCoNLLCPOSIntMap;
typedef CPackedScoreMap<CCoNLLFeats, SCORE_TYPE, action::MAX> CCoNLLFeatsIntMap;

/*===============================================================
 *
 * CWeight - the definition of weights, in feature
 *
 *==============================================================*/

class CWeight : public CWeightBase {

public:

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

   CLemmaIntMap m_mapSTl;
   CCoNLLCPOSIntMap m_mapSTc;
   CCoNLLFeatsIntMap m_mapSTf;

   CLemmaIntMap m_mapN0l;
   CCoNLLCPOSIntMap m_mapN0c;
   CCoNLLFeatsIntMap m_mapN0f;

   CLemmaIntMap m_mapN1l;
   CCoNLLCPOSIntMap m_mapN1c;
   CCoNLLFeatsIntMap m_mapN1f;

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
