// Copyright (C) University of Oxford 2010
/***************************************************************
 *
 * meta_weight.h - the dependency parser meta weights 
 *
 * Yue Zhang, 2007.8 - 2008.1
 *
 ***************************************************************/

#ifndef _DEPPARSER_META_WEIGHTS_H
#define _DEPPARSER_META_WEIGHTS_H

namespace TARGET_LANGUAGE {

namespace depparser {

const static int META_TABLE_SIZE = 3;

typedef CScoreMap<int, SCORE_TYPE> CMetaMap;

/*===============================================================
 *
 * CMetaWeight - the definition of meta weights, in feature
 *
 *==============================================================*/

struct CMetaWeight {

   CMetaMap m_mapSTw;
   CMetaMap m_mapSTt;
   CMetaMap m_mapSTwt;

   CMetaMap m_mapN0w;
   CMetaMap m_mapN0t;
   CMetaMap m_mapN0wt;

   CMetaMap m_mapN1w;
   CMetaMap m_mapN1t;
   CMetaMap m_mapN1wt;

   CMetaMap m_mapN2w;
   CMetaMap m_mapN2t;
   CMetaMap m_mapN2wt;

   CMetaMap m_mapSTHw;
   CMetaMap m_mapSTHt;
   CMetaMap m_mapSTi;

   CMetaMap m_mapSTHHw;
   CMetaMap m_mapSTHHt;
   CMetaMap m_mapSTHi;

   CMetaMap m_mapSTLDw;
   CMetaMap m_mapSTLDt;
   CMetaMap m_mapSTLDi;

   CMetaMap m_mapSTRDw;
   CMetaMap m_mapSTRDt;
   CMetaMap m_mapSTRDi;

   CMetaMap m_mapN0LDw;
   CMetaMap m_mapN0LDt;
   CMetaMap m_mapN0LDi;

   CMetaMap m_mapSTL2Dw;
   CMetaMap m_mapSTL2Dt;
   CMetaMap m_mapSTL2Di;

   CMetaMap m_mapSTR2Dw;
   CMetaMap m_mapSTR2Dt;
   CMetaMap m_mapSTR2Di;

   CMetaMap m_mapN0L2Dw;
   CMetaMap m_mapN0L2Dt;
   CMetaMap m_mapN0L2Di;

   CMetaMap m_mapHTw;
   CMetaMap m_mapHTt;
   CMetaMap m_mapHTwt;

   CMetaMap m_mapSTwtN0wt;
   CMetaMap m_mapSTwtN0w;
   CMetaMap m_mapSTwN0wt;
   CMetaMap m_mapSTtN0wt;
   CMetaMap m_mapSTwtN0t;
   CMetaMap m_mapSTwN0w;
   CMetaMap m_mapSTtN0t;

   CMetaMap m_mapN0tN1t;
   CMetaMap m_mapN0tN1tN2t;
   CMetaMap m_mapSTtN0tN1t;
   CMetaMap m_mapSTtN0tN0LDt;
   CMetaMap m_mapN0tN0LDtN0L2Dt;
   CMetaMap m_mapSTHtSTtN0t;
   CMetaMap m_mapHTtHT2tN0t;
   CMetaMap m_mapSTHHtSTHtSTt;
   CMetaMap m_mapSTtSTLDtN0t;
   CMetaMap m_mapSTtSTLDtSTL2Dt;
   CMetaMap m_mapSTtSTRDtN0t;
   CMetaMap m_mapSTtSTRDtSTR2Dt;

   CMetaMap m_mapSTwd;
   CMetaMap m_mapSTtd;
   CMetaMap m_mapN0wd;
   CMetaMap m_mapN0td;
   CMetaMap m_mapSTwN0wd;
   CMetaMap m_mapSTtN0td;

   CMetaMap m_mapSTwra;
   CMetaMap m_mapSTtra;
   CMetaMap m_mapSTwla;
   CMetaMap m_mapSTtla;
   CMetaMap m_mapN0wla;
   CMetaMap m_mapN0tla;

   CMetaMap m_mapSTwrp;
   CMetaMap m_mapSTtrp;
   CMetaMap m_mapSTwlp;
   CMetaMap m_mapSTtlp;
   CMetaMap m_mapN0wlp;
   CMetaMap m_mapN0tlp;

   CMetaMap m_mapSTl;
   CMetaMap m_mapSTc;
   CMetaMap m_mapSTf;

   CMetaMap m_mapN0l;
   CMetaMap m_mapN0c;
   CMetaMap m_mapN0f;

   CMetaMap m_mapN1l;
   CMetaMap m_mapN1c;
   CMetaMap m_mapN1f;

   CMetaMap m_mapSTwN0wN1w;
   CMetaMap m_mapSTHwSTwN0w;
   CMetaMap m_mapSTwSTLDwN0w;
   CMetaMap m_mapSTwSTRDwN0w;
   CMetaMap m_mapSTwN0wN0LDw;
 
   CMetaMap m_mapSTwN0wN1t;
   CMetaMap m_mapSTHtSTwN0w;
   CMetaMap m_mapSTwSTLDtN0w;
   CMetaMap m_mapSTwSTRDtN0w;
   CMetaMap m_mapSTwN0wN0LDt;
 
   CMetaWeight() : m_mapSTw("MetaStackWord", META_TABLE_SIZE),
                                               m_mapSTt("MetaStackTag", META_TABLE_SIZE),
                                               m_mapSTwt("MetaStackWordTag", META_TABLE_SIZE),

                                               m_mapN0w("MetaNextWord", META_TABLE_SIZE),
                                               m_mapN0t("MetaNextTag", META_TABLE_SIZE),
                                               m_mapN0wt("MetaNextWordTag", META_TABLE_SIZE),

                                               m_mapN1w("MetaNext+1Word", META_TABLE_SIZE),
                                               m_mapN1t("MetaNext+1Tag", META_TABLE_SIZE),
                                               m_mapN1wt("MetaNext+1WordTag", META_TABLE_SIZE),

                                               m_mapN2w("MetaNext+2Word", META_TABLE_SIZE),
                                               m_mapN2t("MetaNext+2Tag", META_TABLE_SIZE),
                                               m_mapN2wt("MetaNext+2WordTag", META_TABLE_SIZE),

                                               m_mapSTHw("MetaStackHeadWord", META_TABLE_SIZE),
                                               m_mapSTHt("MetaStackHeadTag", META_TABLE_SIZE),
                                               m_mapSTi("MetaStackLabel", META_TABLE_SIZE),

                                               m_mapSTHHw("MetaStackHeadHeadWord", META_TABLE_SIZE),
                                               m_mapSTHHt("MetaStackHeadHeadTag", META_TABLE_SIZE),
                                               m_mapSTHi("MetaStackLabel", META_TABLE_SIZE),

                                               m_mapSTLDw("MetaStackLDWord", META_TABLE_SIZE),
                                               m_mapSTLDt("MetaStackLDTag", META_TABLE_SIZE),
                                               m_mapSTLDi("MetaStackLDLabel", META_TABLE_SIZE),

                                               m_mapSTRDw("MetaStackRDWord", META_TABLE_SIZE),
                                               m_mapSTRDt("MetaStackRDTag", META_TABLE_SIZE),
                                               m_mapSTRDi("MetaStackRDLabel", META_TABLE_SIZE),

                                               m_mapN0LDw("MetaNextLDWord", META_TABLE_SIZE),
                                               m_mapN0LDt("MetaNextLDTag", META_TABLE_SIZE),
                                               m_mapN0LDi("MetaNextLDLabel", META_TABLE_SIZE),

                                               m_mapSTL2Dw("MetaStackL2DWord", META_TABLE_SIZE),
                                               m_mapSTL2Dt("MetaStackL2DTag", META_TABLE_SIZE),
                                               m_mapSTL2Di("MetaStackL2DLabel", META_TABLE_SIZE),

                                               m_mapSTR2Dw("MetaStackR2DWord", META_TABLE_SIZE),
                                               m_mapSTR2Dt("MetaStackR2DTag", META_TABLE_SIZE),
                                               m_mapSTR2Di("MetaStackR2DLabel", META_TABLE_SIZE),

                                               m_mapN0L2Dw("MetaNextL2DWord", META_TABLE_SIZE),
                                               m_mapN0L2Dt("MetaNextL2DTag", META_TABLE_SIZE),
                                               m_mapN0L2Di("MetaNextL2DLabel", META_TABLE_SIZE),

                                               m_mapHTw("MetaHeadStackWord", META_TABLE_SIZE),
                                               m_mapHTt("MetaHeadStackTag", META_TABLE_SIZE),
                                               m_mapHTwt("MetaHeadStackWordTag", META_TABLE_SIZE),

                                               m_mapSTwtN0wt("MetaStackWordTagNextWordTag", META_TABLE_SIZE),
                                               m_mapSTwtN0w("MetaStackWordTagNextWord", META_TABLE_SIZE),
                                               m_mapSTwN0wt("MetaStackWordNextWordTag", META_TABLE_SIZE),
                                               m_mapSTtN0wt("MetaStackTagNextWordTag", META_TABLE_SIZE),
                                               m_mapSTwtN0t("MetaStackWordTagNextTag", META_TABLE_SIZE),
                                               m_mapSTwN0w("MetaStackWordNextWord", META_TABLE_SIZE),
                                               m_mapSTtN0t("MetaStackTagNextTag", META_TABLE_SIZE),

                                               m_mapN0tN1t("MetaNextTagNext+1Tag", META_TABLE_SIZE),
                                               m_mapN0tN1tN2t("MetaNextTagTrigram", META_TABLE_SIZE),
                                               m_mapSTtN0tN1t("MetaStackTagNextTagNext+1Tag", META_TABLE_SIZE),
                                               m_mapSTtN0tN0LDt("MetaStackTagNextTagNextLDTag", META_TABLE_SIZE),
                                               m_mapN0tN0LDtN0L2Dt("MetaStackTagNextTagNextLDTagNextTagNextL2DTag", META_TABLE_SIZE),
                                               m_mapSTHtSTtN0t("MetaStackHeadTagStackTagNextTag", META_TABLE_SIZE),
                                               m_mapHTtHT2tN0t("MetaHeadStackTagHeadStack2TagNextTag", META_TABLE_SIZE),
                                               m_mapSTHHtSTHtSTt("MetaStackHeadHeadTagStackHeadTagStackTag", META_TABLE_SIZE),
                                               m_mapSTtSTLDtN0t("MetaStackTagStackLDTagNextTag", META_TABLE_SIZE),
                                               m_mapSTtSTLDtSTL2Dt("MetaStackTagStackLDTagStackL2DTag", META_TABLE_SIZE),
                                               m_mapSTtSTRDtN0t("MetaStackTagStackRDTagNextTag", META_TABLE_SIZE),
                                               m_mapSTtSTRDtSTR2Dt("MetaStackTagStackRDTagStackR2DTag", META_TABLE_SIZE),

                                               m_mapSTwd("MetaStackWordDist", META_TABLE_SIZE),
                                               m_mapSTtd("MetaStackTagDist", META_TABLE_SIZE),
                                               m_mapN0wd("MetaNextWordDist", META_TABLE_SIZE),
                                               m_mapN0td("MetaNextTagDist", META_TABLE_SIZE),
                                               m_mapSTwN0wd("MetaStackWordNextWordDist", META_TABLE_SIZE),
                                               m_mapSTtN0td("MetaStackTagNextTagDist", META_TABLE_SIZE),

                                               m_mapSTwra("MetaStackWordRightArity", META_TABLE_SIZE),
                                               m_mapSTtra("MetaStackTagRightArity", META_TABLE_SIZE),
                                               m_mapSTwla("MetaStackWordLeftArity", META_TABLE_SIZE),
                                               m_mapSTtla("MetaStackTagLeftArity", META_TABLE_SIZE),
                                               m_mapN0wla("MetaNextWordRightArity", META_TABLE_SIZE),
                                               m_mapN0tla("MetaNextTagRightArity", META_TABLE_SIZE),

                                               m_mapSTwrp("MetaStackWordRightSetoftags", META_TABLE_SIZE),
                                               m_mapSTtrp("MetaStackTagRightSetoftags", META_TABLE_SIZE),
                                               m_mapSTwlp("MetaStackWordLeftSetoftags", META_TABLE_SIZE),
                                               m_mapSTtlp("MetaStackTagLeftSetoftags", META_TABLE_SIZE),
                                               m_mapN0wlp("MetaNext0WordLeftSetoftags", META_TABLE_SIZE),
                                               m_mapN0tlp("MetaNext0TagLeftSetoftags", META_TABLE_SIZE),

                                               m_mapSTl("MetaStackLemma", META_TABLE_SIZE),
                                               m_mapSTc("MetaStackCPOS", META_TABLE_SIZE),
                                               m_mapSTf("MetaStackFeats", META_TABLE_SIZE),

                                               m_mapN0l("MetaNextLemma", META_TABLE_SIZE),
                                               m_mapN0c("MetaNextCPOS", META_TABLE_SIZE),
                                               m_mapN0f("MetaNextFeats", META_TABLE_SIZE),

                                               m_mapN1l("MetaNext+1Lemma", META_TABLE_SIZE),
                                               m_mapN1c("MetaNext+1CPOS", META_TABLE_SIZE),
                                               m_mapN1f("MetaNext+1Feats", META_TABLE_SIZE),

                                               m_mapSTwN0wN1w("StackWordNextWordNext+1Word", META_TABLE_SIZE),
                                               m_mapSTHwSTwN0w("StackHeadWordStackWordNextWord", META_TABLE_SIZE),
                                               m_mapSTwSTLDwN0w("StackWordStackLDWordNextWord", META_TABLE_SIZE),
                                               m_mapSTwSTRDwN0w("StackWordStackRDWordNextWord", META_TABLE_SIZE),
                                               m_mapSTwN0wN0LDw("StackWordNextWordNextLDWord", META_TABLE_SIZE),

                                               m_mapSTwN0wN1t("StackWordNextWordNext+1Tag", META_TABLE_SIZE),
                                               m_mapSTHtSTwN0w("StackHeadTagStackWordNextWord", META_TABLE_SIZE),
                                               m_mapSTwSTLDtN0w("StackWordStackLDTagNextWord", META_TABLE_SIZE),
                                               m_mapSTwSTRDtN0w("StackWordStackRDTagNextWord", META_TABLE_SIZE),
                                               m_mapSTwN0wN0LDt("StackWordNextWordNextLDTag", META_TABLE_SIZE) {}


};

};
};

#endif
