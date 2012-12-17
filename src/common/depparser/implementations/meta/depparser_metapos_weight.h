// Copyright (C) University of Oxford 2010
/***************************************************************
 *
 * meta_weight.h - the dependency parser meta weights 
 *
 * Yue Zhang, 2007.8 - 2008.1
 *
 ***************************************************************/

#ifndef _DEPPARSER_METAPOS_WEIGHTS_H
#define _DEPPARSER_METAPOS_WEIGHTS_H

namespace TARGET_LANGUAGE {

namespace depparser {

const static int METAPOS_TABLE_SIZE = 53;

typedef CScoreMap<CTuple2<CTag, long>, SCORE_TYPE> CMetaPOSMap;

/*===============================================================
 *
 * CMetaPOSWeight - the definition of meta weights, in feature
 *
 *==============================================================*/

struct CMetaPOSWeight {

   CMetaPOSMap m_mapSTw;
   CMetaPOSMap m_mapSTt;
   CMetaPOSMap m_mapSTwt;

   CMetaPOSMap m_mapN0w;
   CMetaPOSMap m_mapN0t;
   CMetaPOSMap m_mapN0wt;

   CMetaPOSMap m_mapN1w;
   CMetaPOSMap m_mapN1t;
   CMetaPOSMap m_mapN1wt;

   CMetaPOSMap m_mapN2w;
   CMetaPOSMap m_mapN2t;
   CMetaPOSMap m_mapN2wt;

   CMetaPOSMap m_mapSTHw;
   CMetaPOSMap m_mapSTHt;
   CMetaPOSMap m_mapSTi;

   CMetaPOSMap m_mapSTHHw;
   CMetaPOSMap m_mapSTHHt;
   CMetaPOSMap m_mapSTHi;

   CMetaPOSMap m_mapSTLDw;
   CMetaPOSMap m_mapSTLDt;
   CMetaPOSMap m_mapSTLDi;

   CMetaPOSMap m_mapSTRDw;
   CMetaPOSMap m_mapSTRDt;
   CMetaPOSMap m_mapSTRDi;

   CMetaPOSMap m_mapN0LDw;
   CMetaPOSMap m_mapN0LDt;
   CMetaPOSMap m_mapN0LDi;

   CMetaPOSMap m_mapSTL2Dw;
   CMetaPOSMap m_mapSTL2Dt;
   CMetaPOSMap m_mapSTL2Di;

   CMetaPOSMap m_mapSTR2Dw;
   CMetaPOSMap m_mapSTR2Dt;
   CMetaPOSMap m_mapSTR2Di;

   CMetaPOSMap m_mapN0L2Dw;
   CMetaPOSMap m_mapN0L2Dt;
   CMetaPOSMap m_mapN0L2Di;

   CMetaPOSMap m_mapHTw;
   CMetaPOSMap m_mapHTt;
   CMetaPOSMap m_mapHTwt;

   CMetaPOSMap m_mapSTwtN0wt;
   CMetaPOSMap m_mapSTwtN0w;
   CMetaPOSMap m_mapSTwN0wt;
   CMetaPOSMap m_mapSTtN0wt;
   CMetaPOSMap m_mapSTwtN0t;
   CMetaPOSMap m_mapSTwN0w;
   CMetaPOSMap m_mapSTtN0t;

   CMetaPOSMap m_mapN0tN1t;
   CMetaPOSMap m_mapN0tN1tN2t;
   CMetaPOSMap m_mapSTtN0tN1t;
   CMetaPOSMap m_mapSTtN0tN0LDt;
   CMetaPOSMap m_mapN0tN0LDtN0L2Dt;
   CMetaPOSMap m_mapSTHtSTtN0t;
   CMetaPOSMap m_mapHTtHT2tN0t;
   CMetaPOSMap m_mapSTHHtSTHtSTt;
   CMetaPOSMap m_mapSTtSTLDtN0t;
   CMetaPOSMap m_mapSTtSTLDtSTL2Dt;
   CMetaPOSMap m_mapSTtSTRDtN0t;
   CMetaPOSMap m_mapSTtSTRDtSTR2Dt;

   CMetaPOSMap m_mapSTwd;
   CMetaPOSMap m_mapSTtd;
   CMetaPOSMap m_mapN0wd;
   CMetaPOSMap m_mapN0td;
   CMetaPOSMap m_mapSTwN0wd;
   CMetaPOSMap m_mapSTtN0td;

   CMetaPOSMap m_mapSTwra;
   CMetaPOSMap m_mapSTtra;
   CMetaPOSMap m_mapSTwla;
   CMetaPOSMap m_mapSTtla;
   CMetaPOSMap m_mapN0wla;
   CMetaPOSMap m_mapN0tla;

   CMetaPOSMap m_mapSTwrp;
   CMetaPOSMap m_mapSTtrp;
   CMetaPOSMap m_mapSTwlp;
   CMetaPOSMap m_mapSTtlp;
   CMetaPOSMap m_mapN0wlp;
   CMetaPOSMap m_mapN0tlp;

   CMetaPOSMap m_mapSTl;
   CMetaPOSMap m_mapSTc;
   CMetaPOSMap m_mapSTf;

   CMetaPOSMap m_mapN0l;
   CMetaPOSMap m_mapN0c;
   CMetaPOSMap m_mapN0f;

   CMetaPOSMap m_mapN1l;
   CMetaPOSMap m_mapN1c;
   CMetaPOSMap m_mapN1f;

   CMetaPOSMap m_mapSTwN0wN1w;
   CMetaPOSMap m_mapSTHwSTwN0w;
   CMetaPOSMap m_mapSTwSTLDwN0w;
   CMetaPOSMap m_mapSTwSTRDwN0w;
   CMetaPOSMap m_mapSTwN0wN0LDw;
 
   CMetaPOSMap m_mapSTwN0wN1t;
   CMetaPOSMap m_mapSTHtSTwN0w;
   CMetaPOSMap m_mapSTwSTLDtN0w;
   CMetaPOSMap m_mapSTwSTRDtN0w;
   CMetaPOSMap m_mapSTwN0wN0LDt;
 
   CMetaPOSWeight() : m_mapSTw("MetaStackWord", METAPOS_TABLE_SIZE),
                                               m_mapSTt("MetaStackTag", METAPOS_TABLE_SIZE),
                                               m_mapSTwt("MetaStackWordTag", METAPOS_TABLE_SIZE),

                                               m_mapN0w("MetaNextWord", METAPOS_TABLE_SIZE),
                                               m_mapN0t("MetaNextTag", METAPOS_TABLE_SIZE),
                                               m_mapN0wt("MetaNextWordTag", METAPOS_TABLE_SIZE),

                                               m_mapN1w("MetaNext+1Word", METAPOS_TABLE_SIZE),
                                               m_mapN1t("MetaNext+1Tag", METAPOS_TABLE_SIZE),
                                               m_mapN1wt("MetaNext+1WordTag", METAPOS_TABLE_SIZE),

                                               m_mapN2w("MetaNext+2Word", METAPOS_TABLE_SIZE),
                                               m_mapN2t("MetaNext+2Tag", METAPOS_TABLE_SIZE),
                                               m_mapN2wt("MetaNext+2WordTag", METAPOS_TABLE_SIZE),

                                               m_mapSTHw("MetaStackHeadWord", METAPOS_TABLE_SIZE),
                                               m_mapSTHt("MetaStackHeadTag", METAPOS_TABLE_SIZE),
                                               m_mapSTi("MetaStackLabel", METAPOS_TABLE_SIZE),

                                               m_mapSTHHw("MetaStackHeadHeadWord", METAPOS_TABLE_SIZE),
                                               m_mapSTHHt("MetaStackHeadHeadTag", METAPOS_TABLE_SIZE),
                                               m_mapSTHi("MetaStackLabel", METAPOS_TABLE_SIZE),

                                               m_mapSTLDw("MetaStackLDWord", METAPOS_TABLE_SIZE),
                                               m_mapSTLDt("MetaStackLDTag", METAPOS_TABLE_SIZE),
                                               m_mapSTLDi("MetaStackLDLabel", METAPOS_TABLE_SIZE),

                                               m_mapSTRDw("MetaStackRDWord", METAPOS_TABLE_SIZE),
                                               m_mapSTRDt("MetaStackRDTag", METAPOS_TABLE_SIZE),
                                               m_mapSTRDi("MetaStackRDLabel", METAPOS_TABLE_SIZE),

                                               m_mapN0LDw("MetaNextLDWord", METAPOS_TABLE_SIZE),
                                               m_mapN0LDt("MetaNextLDTag", METAPOS_TABLE_SIZE),
                                               m_mapN0LDi("MetaNextLDLabel", METAPOS_TABLE_SIZE),

                                               m_mapSTL2Dw("MetaStackL2DWord", METAPOS_TABLE_SIZE),
                                               m_mapSTL2Dt("MetaStackL2DTag", METAPOS_TABLE_SIZE),
                                               m_mapSTL2Di("MetaStackL2DLabel", METAPOS_TABLE_SIZE),

                                               m_mapSTR2Dw("MetaStackR2DWord", METAPOS_TABLE_SIZE),
                                               m_mapSTR2Dt("MetaStackR2DTag", METAPOS_TABLE_SIZE),
                                               m_mapSTR2Di("MetaStackR2DLabel", METAPOS_TABLE_SIZE),

                                               m_mapN0L2Dw("MetaNextL2DWord", METAPOS_TABLE_SIZE),
                                               m_mapN0L2Dt("MetaNextL2DTag", METAPOS_TABLE_SIZE),
                                               m_mapN0L2Di("MetaNextL2DLabel", METAPOS_TABLE_SIZE),

                                               m_mapHTw("MetaHeadStackWord", METAPOS_TABLE_SIZE),
                                               m_mapHTt("MetaHeadStackTag", METAPOS_TABLE_SIZE),
                                               m_mapHTwt("MetaHeadStackWordTag", METAPOS_TABLE_SIZE),

                                               m_mapSTwtN0wt("MetaStackWordTagNextWordTag", METAPOS_TABLE_SIZE),
                                               m_mapSTwtN0w("MetaStackWordTagNextWord", METAPOS_TABLE_SIZE),
                                               m_mapSTwN0wt("MetaStackWordNextWordTag", METAPOS_TABLE_SIZE),
                                               m_mapSTtN0wt("MetaStackTagNextWordTag", METAPOS_TABLE_SIZE),
                                               m_mapSTwtN0t("MetaStackWordTagNextTag", METAPOS_TABLE_SIZE),
                                               m_mapSTwN0w("MetaStackWordNextWord", METAPOS_TABLE_SIZE),
                                               m_mapSTtN0t("MetaStackTagNextTag", METAPOS_TABLE_SIZE),

                                               m_mapN0tN1t("MetaNextTagNext+1Tag", METAPOS_TABLE_SIZE),
                                               m_mapN0tN1tN2t("MetaNextTagTrigram", METAPOS_TABLE_SIZE),
                                               m_mapSTtN0tN1t("MetaStackTagNextTagNext+1Tag", METAPOS_TABLE_SIZE),
                                               m_mapSTtN0tN0LDt("MetaStackTagNextTagNextLDTag", METAPOS_TABLE_SIZE),
                                               m_mapN0tN0LDtN0L2Dt("MetaStackTagNextTagNextLDTagNextTagNextL2DTag", METAPOS_TABLE_SIZE),
                                               m_mapSTHtSTtN0t("MetaStackHeadTagStackTagNextTag", METAPOS_TABLE_SIZE),
                                               m_mapHTtHT2tN0t("MetaHeadStackTagHeadStack2TagNextTag", METAPOS_TABLE_SIZE),
                                               m_mapSTHHtSTHtSTt("MetaStackHeadHeadTagStackHeadTagStackTag", METAPOS_TABLE_SIZE),
                                               m_mapSTtSTLDtN0t("MetaStackTagStackLDTagNextTag", METAPOS_TABLE_SIZE),
                                               m_mapSTtSTLDtSTL2Dt("MetaStackTagStackLDTagStackL2DTag", METAPOS_TABLE_SIZE),
                                               m_mapSTtSTRDtN0t("MetaStackTagStackRDTagNextTag", METAPOS_TABLE_SIZE),
                                               m_mapSTtSTRDtSTR2Dt("MetaStackTagStackRDTagStackR2DTag", METAPOS_TABLE_SIZE),

                                               m_mapSTwd("MetaStackWordDist", METAPOS_TABLE_SIZE),
                                               m_mapSTtd("MetaStackTagDist", METAPOS_TABLE_SIZE),
                                               m_mapN0wd("MetaNextWordDist", METAPOS_TABLE_SIZE),
                                               m_mapN0td("MetaNextTagDist", METAPOS_TABLE_SIZE),
                                               m_mapSTwN0wd("MetaStackWordNextWordDist", METAPOS_TABLE_SIZE),
                                               m_mapSTtN0td("MetaStackTagNextTagDist", METAPOS_TABLE_SIZE),

                                               m_mapSTwra("MetaStackWordRightArity", METAPOS_TABLE_SIZE),
                                               m_mapSTtra("MetaStackTagRightArity", METAPOS_TABLE_SIZE),
                                               m_mapSTwla("MetaStackWordLeftArity", METAPOS_TABLE_SIZE),
                                               m_mapSTtla("MetaStackTagLeftArity", METAPOS_TABLE_SIZE),
                                               m_mapN0wla("MetaNextWordRightArity", METAPOS_TABLE_SIZE),
                                               m_mapN0tla("MetaNextTagRightArity", METAPOS_TABLE_SIZE),

                                               m_mapSTwrp("MetaStackWordRightSetoftags", METAPOS_TABLE_SIZE),
                                               m_mapSTtrp("MetaStackTagRightSetoftags", METAPOS_TABLE_SIZE),
                                               m_mapSTwlp("MetaStackWordLeftSetoftags", METAPOS_TABLE_SIZE),
                                               m_mapSTtlp("MetaStackTagLeftSetoftags", METAPOS_TABLE_SIZE),
                                               m_mapN0wlp("MetaNext0WordLeftSetoftags", METAPOS_TABLE_SIZE),
                                               m_mapN0tlp("MetaNext0TagLeftSetoftags", METAPOS_TABLE_SIZE),

                                               m_mapSTl("MetaStackLemma", METAPOS_TABLE_SIZE),
                                               m_mapSTc("MetaStackCPOS", METAPOS_TABLE_SIZE),
                                               m_mapSTf("MetaStackFeats", METAPOS_TABLE_SIZE),

                                               m_mapN0l("MetaNextLemma", METAPOS_TABLE_SIZE),
                                               m_mapN0c("MetaNextCPOS", METAPOS_TABLE_SIZE),
                                               m_mapN0f("MetaNextFeats", METAPOS_TABLE_SIZE),

                                               m_mapN1l("MetaNext+1Lemma", METAPOS_TABLE_SIZE),
                                               m_mapN1c("MetaNext+1CPOS", METAPOS_TABLE_SIZE),
                                               m_mapN1f("MetaNext+1Feats", METAPOS_TABLE_SIZE),

                                               m_mapSTwN0wN1w("StackWordNextWordNext+1Word", METAPOS_TABLE_SIZE),
                                               m_mapSTHwSTwN0w("StackHeadWordStackWordNextWord", METAPOS_TABLE_SIZE),
                                               m_mapSTwSTLDwN0w("StackWordStackLDWordNextWord", METAPOS_TABLE_SIZE),
                                               m_mapSTwSTRDwN0w("StackWordStackRDWordNextWord", METAPOS_TABLE_SIZE),
                                               m_mapSTwN0wN0LDw("StackWordNextWordNextLDWord", METAPOS_TABLE_SIZE),

                                               m_mapSTwN0wN1t("StackWordNextWordNext+1Tag", METAPOS_TABLE_SIZE),
                                               m_mapSTHtSTwN0w("StackHeadTagStackWordNextWord", METAPOS_TABLE_SIZE),
                                               m_mapSTwSTLDtN0w("StackWordStackLDTagNextWord", METAPOS_TABLE_SIZE),
                                               m_mapSTwSTRDtN0w("StackWordStackRDTagNextWord", METAPOS_TABLE_SIZE),
                                               m_mapSTwN0wN0LDt("StackWordNextWordNextLDTag", METAPOS_TABLE_SIZE) {}


};

};
};

#endif
