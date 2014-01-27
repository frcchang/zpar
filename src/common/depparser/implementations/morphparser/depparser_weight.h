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
   left(m_mapSTm)right\
   left(m_mapSTwm)right\
   left(m_mapST1w)right\
   left(m_mapST1m)right\
   left(m_mapST1wm)right\
   left(m_mapN0w)right\
   left(m_mapN0m)right\
   left(m_mapN0wm)right\
   left(m_mapN1w)right\
   left(m_mapN1m)right\
   left(m_mapN1wm)right\
   left(m_mapN2w)right\
   left(m_mapN2m)right\
   left(m_mapN2wm)right\
   left(m_mapSTHw)right\
   left(m_mapSTHm)right\
   left(m_mapSTi)right\
   left(m_mapSTHHw)right\
   left(m_mapSTHHm)right\
   left(m_mapSTHi)right\
   left(m_mapSTLDw)right\
   left(m_mapSTLDm)right\
   left(m_mapSTLDi)right\
   left(m_mapSTRDw)right\
   left(m_mapSTRDm)right\
   left(m_mapSTRDi)right\
   left(m_mapN0LDw)right\
   left(m_mapN0LDm)right\
   left(m_mapN0LDi)right\
   left(m_mapSTL2Dw)right\
   left(m_mapSTL2Dm)right\
   left(m_mapSTL2Di)right\
   left(m_mapSTR2Dw)right\
   left(m_mapSTR2Dm)right\
   left(m_mapSTR2Di)right\
   left(m_mapN0L2Dw)right\
   left(m_mapN0L2Dm)right\
   left(m_mapN0L2Di)right\
   left(m_mapHTw)right\
   left(m_mapHTt)right\
   left(m_mapHTwt)right\
\
   left(m_mapBM1w)right\
   left(m_mapBM1m)right\
   left(m_mapBM1wm)right\
   left(m_mapBM2w)right\
   left(m_mapBM2m)right\
   left(m_mapBM2wm)right\
   left(m_mapBM3w)right\
   left(m_mapBM3m)right\
   left(m_mapBM3wm)right\
   left(m_mapBM2Hw)right\
   left(m_mapBM2Hm)right\
   left(m_mapBM2Hwm)right\
   left(m_mapBM3Hw)right\
   left(m_mapBM3Hm)right\
   left(m_mapBM3Hwm)right\
\
   left(m_mapNM1w)right\
   left(m_mapNM1m)right\
   left(m_mapNM1wm)right\
   left(m_mapNM2w)right\
   left(m_mapNM2m)right\
   left(m_mapNM2wm)right\
   left(m_mapNM3w)right\
   left(m_mapNM3m)right\
   left(m_mapNM3wm)right\
\
   left(m_mapB0w)right\
   left(m_mapB1w)right\
   left(m_mapB2w)right\
\
   left(m_mapB0p1)right\
   left(m_mapB0p2)right\
   left(m_mapB0p3)right\
   left(m_mapB0p4)right\
   left(m_mapB0s1)right\
   left(m_mapB0s2)right\
   left(m_mapB0s3)right\
   left(m_mapB0s4)right\
   left(m_mapB0hd)right\
   left(m_mapB0hu)right\
   left(m_mapB0hh)right\
\
   left(m_mapB1p1)right\
   left(m_mapB1p2)right\
   left(m_mapB1p3)right\
   left(m_mapB1p4)right\
   left(m_mapB1s1)right\
   left(m_mapB1s2)right\
   left(m_mapB1s3)right\
   left(m_mapB1s4)right\
\
   left(m_mapSTmp)right\
   left(m_mapSTnum)right\
   left(m_mapSTper)right\
   left(m_mapN0mp)right\
   left(m_mapN0num)right\
   left(m_mapN0per)right\
   left(m_mapSTnumN0num)right\
   left(m_mapSTperN0per)right\
   left(m_mapSTvtyN0vty)right\
   left(m_mapSTvfoN0vfo)right\
   left(m_mapST1numN0num)right\
   left(m_mapST1perN0per)right\
   left(m_mapSTnumN1num)right\
   left(m_mapSTperN1per)right\
\
   left(m_mapSTwmN0wm)right\
   left(m_mapSTwmN0w)right\
   left(m_mapSTwN0wm)right\
   left(m_mapSTmN0wm)right\
   left(m_mapSTwmN0m)right\
   left(m_mapSTwN0w)right\
   left(m_mapSTmN0m)right\
\
   left(m_mapBM1mBM2m)right\
   left(m_mapBM1mBM2mBM3m)right\
\
   left(m_mapN0mN1m)right\
   left(m_mapN0mN1mN2m)right\
   left(m_mapSTmN0mN1m)right\
   left(m_mapSTmN0mN0LDm)right\
   left(m_mapN0mN0LDmN0L2Dm)right\
   left(m_mapSTHmSTmN0m)right\
   left(m_mapHTmHT2mN0m)right\
   left(m_mapSTHHmSTHmSTm)right\
   left(m_mapSTmSTLDmN0m)right\
   left(m_mapSTmSTLDmSTL2Dm)right\
   left(m_mapSTmSTRDmN0m)right\
   left(m_mapSTmSTRDmSTR2Dm)right\
\
   left(m_mapN0wN1w)right\
   left(m_mapN1wN2w)right\
   left(m_mapN0wN1wN1m)right\
\
   left(m_mapSTwd)right\
   left(m_mapSTmd)right\
   left(m_mapN0wd)right\
   left(m_mapN0md)right\
   left(m_mapSTwN0wd)right\
   left(m_mapSTmN0md)right\
\
   left(m_mapSTwra)right\
   left(m_mapSTmra)right\
   left(m_mapSTwla)right\
   left(m_mapSTmla)right\
   left(m_mapN0wla)right\
   left(m_mapN0mla)right\
\
   left(m_mapSTwrp)right\
   left(m_mapSTmrp)right\
   left(m_mapSTwlp)right\
   left(m_mapSTmlp)right\
   left(m_mapN0wlp)right\
   left(m_mapN0mlp)right\
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
typedef CPackedScoreMap<bool, SCORE_TYPE, action::MAX> CBoolMap;
typedef CPackedScoreMap<CMorph, SCORE_TYPE, action::MAX> CMorphMap;
typedef CPackedScoreMap<CTagSet<CTag, 2>,  SCORE_TYPE, action::MAX> CTagSet2Map;
typedef CPackedScoreMap<CTagSet<CTag, 3>, SCORE_TYPE, action::MAX> CTagSet3Map;
typedef CPackedScoreMap<CTagSet<CTag, 4>, SCORE_TYPE, action::MAX> CTagSet4Map;
typedef CPackedScoreMap<CTuple2<CWord, CTag>, SCORE_TYPE, action::MAX> CWordTagMap;
typedef CPackedScoreMap<CTuple2<CWord, CMorph>, SCORE_TYPE, action::MAX> CWordMorphMap;
typedef CPackedScoreMap<CTuple2<CWord, CWord>, SCORE_TYPE, action::MAX> CWordWordMap;
typedef CPackedScoreMap<CTuple3<CWord, CTag, CTag>, SCORE_TYPE, action::MAX> CWordTagTagMap;
typedef CPackedScoreMap<CTuple3<CWord, CWord, CTag>, SCORE_TYPE, action::MAX> CWordWordTagMap;
typedef CPackedScoreMap<CTuple3<CWord, CMorph, CMorph>, SCORE_TYPE, action::MAX> CWordMorphMorphMap;
typedef CPackedScoreMap<CTuple3<CWord, CWord, CMorph>, SCORE_TYPE, action::MAX> CWordWordMorphMap;
typedef CPackedScoreMap<CTuple4<CWord, CWord, CMorph, CMorph>, SCORE_TYPE, action::MAX> CWordWordMorphMorphMap;
typedef CPackedScoreMap<CTuple2<CMorph, CMorph>, SCORE_TYPE, action::MAX> CMorphMorphMap;
typedef CPackedScoreMap<CTuple3<CMorph, CMorph, CMorph>, SCORE_TYPE, action::MAX> CMorphMorphMorphMap;
typedef CPackedScoreMap<CTaggedWord<CTag, TAG_SEPARATOR>, SCORE_TYPE, action::MAX> CTaggedWordMap;
typedef CPackedScoreMap<CTwoWords, SCORE_TYPE, action::MAX> CTwoWordsMap;
typedef CPackedScoreMap<CTwoTaggedWords, SCORE_TYPE, action::MAX> CTwoTaggedWordsMap;
typedef CPackedScoreMap<CTuple2<CWord, int>, SCORE_TYPE, action::MAX> CWordIntMap;
typedef CPackedScoreMap<CTuple2<CTag, int>, SCORE_TYPE, action::MAX> CTagIntMap;
typedef CPackedScoreMap<CTuple3<CTag, CTag, int>, SCORE_TYPE, action::MAX> CTagTagIntMap;
typedef CPackedScoreMap<CTuple2<CMorph, int>, SCORE_TYPE, action::MAX> CMorphIntMap;
typedef CPackedScoreMap<CTuple3<CMorph, CMorph, int>, SCORE_TYPE, action::MAX> CMorphMorphIntMap;
typedef CPackedScoreMap<CTuple3<CWord, CWord, int>, SCORE_TYPE, action::MAX> CWordWordIntMap;
typedef CPackedScoreMap<CTuple2<int, int>, SCORE_TYPE, action::MAX> CIntIntMap;
typedef CPackedScoreMap<CTuple2< CWord, CSetOfTags<CDependencyLabel> >, SCORE_TYPE, action::MAX> CWordSetOfLabelsMap;
typedef CPackedScoreMap<CTuple2< CTag, CSetOfTags<CDependencyLabel> >, SCORE_TYPE, action::MAX> CTagSetOfLabelsMap;
typedef CPackedScoreMap<CTuple2< CMorph, CSetOfTags<CDependencyLabel> >, SCORE_TYPE, action::MAX> CMorphSetOfLabelsMap;

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
   CMorphMap m_mapSTm;
   CWordMorphMap m_mapSTwm;

   CWordMap m_mapST1w;
   CMorphMap m_mapST1m;
   CWordMorphMap m_mapST1wm;

   CWordMap m_mapN0w;
   CMorphMap m_mapN0m;
   CWordMorphMap m_mapN0wm;

   CWordMap m_mapN1w;
   CMorphMap m_mapN1m;
   CWordMorphMap m_mapN1wm;

   CWordMap m_mapN2w;
   CMorphMap m_mapN2m;
   CWordMorphMap m_mapN2wm;

   CWordMap m_mapSTHw;
   CMorphMap m_mapSTHm;
   CIntMap m_mapSTi;

   CWordMap m_mapSTHHw;
   CMorphMap m_mapSTHHm;
   CIntMap m_mapSTHi;

   CWordMap m_mapSTLDw;
   CMorphMap m_mapSTLDm;
   CIntMap m_mapSTLDi;

   CWordMap m_mapSTRDw;
   CMorphMap m_mapSTRDm;
   CIntMap m_mapSTRDi;

   CWordMap m_mapN0LDw;
   CMorphMap m_mapN0LDm;
   CIntMap m_mapN0LDi;

   CWordMap m_mapSTL2Dw;
   CMorphMap m_mapSTL2Dm;
   CIntMap m_mapSTL2Di;

   CWordMap m_mapSTR2Dw;
   CMorphMap m_mapSTR2Dm;
   CIntMap m_mapSTR2Di;

   CWordMap m_mapN0L2Dw;
   CMorphMap m_mapN0L2Dm;
   CIntMap m_mapN0L2Di;

   CWordMap m_mapHTw;
   CTagMap m_mapHTt;
   CTaggedWordMap m_mapHTwt;

   CWordMap m_mapBM1w;
   CMorphMap m_mapBM1m;
   CWordMorphMap m_mapBM1wm;
   CWordMap m_mapBM2w;
   CMorphMap m_mapBM2m;
   CWordMorphMap m_mapBM2wm;
   CWordMap m_mapBM3w;
   CMorphMap m_mapBM3m;
   CWordMorphMap m_mapBM3wm;
   CWordMap m_mapBM2Hw;
   CMorphMap m_mapBM2Hm;
   CWordMorphMap m_mapBM2Hwm;
   CWordMap m_mapBM3Hw;
   CMorphMap m_mapBM3Hm;
   CWordMorphMap m_mapBM3Hwm;

   CWordMap m_mapNM1w;
   CMorphMap m_mapNM1m;
   CWordMorphMap m_mapNM1wm;
   CWordMap m_mapNM2w;
   CMorphMap m_mapNM2m;
   CWordMorphMap m_mapNM2wm;
   CWordMap m_mapNM3w;
   CMorphMap m_mapNM3m;
   CWordMorphMap m_mapNM3wm;

   CWordMap m_mapB0w;
   CWordMap m_mapB1w;
   CWordMap m_mapB2w;

   CWordMap m_mapB0p1;
   CWordMap m_mapB0p2;
   CWordMap m_mapB0p3;
   CWordMap m_mapB0p4;
   CWordMap m_mapB0s1;
   CWordMap m_mapB0s2;
   CWordMap m_mapB0s3;
   CWordMap m_mapB0s4;
   CBoolMap m_mapB0hd;
   CBoolMap m_mapB0hu;
   CBoolMap m_mapB0hh;

   CWordMap m_mapB1p1;
   CWordMap m_mapB1p2;
   CWordMap m_mapB1p3;
   CWordMap m_mapB1p4;
   CWordMap m_mapB1s1;
   CWordMap m_mapB1s2;
   CWordMap m_mapB1s3;
   CWordMap m_mapB1s4;

   CIntMap m_mapSTmp;
   CIntMap m_mapSTnum;
   CIntMap m_mapSTper;
   CIntMap m_mapN0mp;
   CIntMap m_mapN0num;
   CIntMap m_mapN0per;

   CIntIntMap m_mapSTnumN0num;
   CIntIntMap m_mapSTperN0per;
   CIntIntMap m_mapSTvtyN0vty;
   CIntIntMap m_mapSTvfoN0vfo;
   CIntIntMap m_mapST1numN0num;
   CIntIntMap m_mapST1perN0per;
   CIntIntMap m_mapSTnumN1num;
   CIntIntMap m_mapSTperN1per;

   CWordWordMorphMorphMap m_mapSTwmN0wm;
   CWordWordMorphMap m_mapSTwmN0w;
   CWordWordMorphMap m_mapSTwN0wm;
   CWordMorphMorphMap m_mapSTmN0wm;
   CWordMorphMorphMap m_mapSTwmN0m;
   CTwoWordsMap m_mapSTwN0w;
   CMorphMorphMap m_mapSTmN0m;

   CMorphMorphMap m_mapBM1mBM2m;
   CMorphMorphMorphMap m_mapBM1mBM2mBM3m;

   CMorphMorphMap m_mapN0mN1m;
   CMorphMorphMorphMap m_mapN0mN1mN2m;
   CMorphMorphMorphMap m_mapSTmN0mN1m;
   CMorphMorphMorphMap m_mapSTmN0mN0LDm;
   CMorphMorphMorphMap m_mapN0mN0LDmN0L2Dm;
   CMorphMorphMorphMap m_mapSTHmSTmN0m;
   CMorphMorphMorphMap m_mapHTmHT2mN0m;
   CMorphMorphMorphMap m_mapSTHHmSTHmSTm;
   CMorphMorphMorphMap m_mapSTmSTLDmN0m;
   CMorphMorphMorphMap m_mapSTmSTLDmSTL2Dm;
   CMorphMorphMorphMap m_mapSTmSTRDmN0m;
   CMorphMorphMorphMap m_mapSTmSTRDmSTR2Dm;

   CWordWordMap m_mapN0wN1w;
   CWordWordMap m_mapN1wN2w;
   CWordWordMorphMap m_mapN0wN1wN1m;

   CWordIntMap m_mapSTwd;
   CMorphIntMap m_mapSTmd;
   CWordIntMap m_mapN0wd;
   CMorphIntMap m_mapN0md;
   CWordWordIntMap m_mapSTwN0wd;
   CMorphMorphIntMap m_mapSTmN0md;

   CWordIntMap m_mapSTwra;
   CMorphIntMap m_mapSTmra;
   CWordIntMap m_mapSTwla;
   CMorphIntMap m_mapSTmla;
   CWordIntMap m_mapN0wla;
   CMorphIntMap m_mapN0mla;

   CWordSetOfLabelsMap m_mapSTwrp;
   CMorphSetOfLabelsMap m_mapSTmrp;
   CWordSetOfLabelsMap m_mapSTwlp;
   CMorphSetOfLabelsMap m_mapSTmlp;
   CWordSetOfLabelsMap m_mapN0wlp;
   CMorphSetOfLabelsMap m_mapN0mlp;

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

   CWeight(const std::string &sPath, bool bTrain) : CWeightBase(sPath, bTrain)  ,

                                               m_mapSTw("StackWord", DEP_TABLE_SIZE),
                                               m_mapSTm("StackMorph", DEP_TABLE_SIZE),
                                               m_mapSTwm("StackWordMorph", DEP_TABLE_SIZE),

                                               m_mapST1w("Stack1Word", DEP_TABLE_SIZE),
                                               m_mapST1m("Stack1Morph", DEP_TABLE_SIZE),
                                               m_mapST1wm("Stack1WordMorph", DEP_TABLE_SIZE),

                                               m_mapN0w("NextWord", DEP_TABLE_SIZE),
                                               m_mapN0m("NextMorph", DEP_TABLE_SIZE),
                                               m_mapN0wm("NextWordMorph", DEP_TABLE_SIZE),

                                               m_mapN1w("Next+1Word", DEP_TABLE_SIZE),
                                               m_mapN1m("Next+1Morph", DEP_TABLE_SIZE),
                                               m_mapN1wm("Next+1WordMorph", DEP_TABLE_SIZE),

                                               m_mapN2w("Next+2Word", DEP_TABLE_SIZE),
                                               m_mapN2m("Next+2Morph", DEP_TABLE_SIZE),
                                               m_mapN2wm("Next+2WordMorph", DEP_TABLE_SIZE),

                                               m_mapSTHw("StackHeadWord", DEP_TABLE_SIZE),
                                               m_mapSTHm("StackHeadMorph", DEP_TABLE_SIZE),
                                               m_mapSTi("StackLabel", DEP_TABLE_SIZE),

                                               m_mapSTHHw("StackHeadHeadWord", DEP_TABLE_SIZE),
                                               m_mapSTHHm("StackHeadHeadMorph", DEP_TABLE_SIZE),
                                               m_mapSTHi("StackLabel", DEP_TABLE_SIZE),

                                               m_mapSTLDw("StackLDWord", DEP_TABLE_SIZE),
                                               m_mapSTLDm("StackLDMorph", DEP_TABLE_SIZE),
                                               m_mapSTLDi("StackLDLabel", DEP_TABLE_SIZE),

                                               m_mapSTRDw("StackRDWord", DEP_TABLE_SIZE),
                                               m_mapSTRDm("StackRDMorph", DEP_TABLE_SIZE),
                                               m_mapSTRDi("StackRDLabel", DEP_TABLE_SIZE),

                                               m_mapN0LDw("NextLDWord", DEP_TABLE_SIZE),
                                               m_mapN0LDm("NextLDMorph", DEP_TABLE_SIZE),
                                               m_mapN0LDi("NextLDLabel", DEP_TABLE_SIZE),

                                               m_mapSTL2Dw("StackL2DWord", DEP_TABLE_SIZE),
                                               m_mapSTL2Dm("StackL2DMorph", DEP_TABLE_SIZE),
                                               m_mapSTL2Di("StackL2DLabel", DEP_TABLE_SIZE),

                                               m_mapSTR2Dw("StackR2DWord", DEP_TABLE_SIZE),
                                               m_mapSTR2Dm("StackR2DMorph", DEP_TABLE_SIZE),
                                               m_mapSTR2Di("StackR2DLabel", DEP_TABLE_SIZE),

                                               m_mapN0L2Dw("NextL2DWord", DEP_TABLE_SIZE),
                                               m_mapN0L2Dm("NextL2DMorph", DEP_TABLE_SIZE),
                                               m_mapN0L2Di("NextL2DLabel", DEP_TABLE_SIZE),

                                               m_mapHTw("HeadStackWord", DEP_TABLE_SIZE),
                                               m_mapHTt("HeadStackTag", DEP_TABLE_SIZE),
                                               m_mapHTwt("HeadStackWordTag", DEP_TABLE_SIZE),

                                               m_mapBM1w("Buffer-1Word", DEP_TABLE_SIZE),
                                               m_mapBM1m("Buffer-1Morph", DEP_TABLE_SIZE),
                                               m_mapBM1wm("Buffer-1WordMorph", DEP_TABLE_SIZE),
                                               m_mapBM2w("Buffer-2Word", DEP_TABLE_SIZE),
                                               m_mapBM2m("Buffer-2Morph", DEP_TABLE_SIZE),
                                               m_mapBM2wm("Buffer-2WordMorph", DEP_TABLE_SIZE),
                                               m_mapBM3w("Buffer-3Word", DEP_TABLE_SIZE),
                                               m_mapBM3m("Buffer-3Morph", DEP_TABLE_SIZE),
                                               m_mapBM3wm("Buffer-3WordMorph", DEP_TABLE_SIZE),
                                               m_mapBM2Hw("Buffer-2HeadWord", DEP_TABLE_SIZE),
                                               m_mapBM2Hm("Buffer-2HeadMorph", DEP_TABLE_SIZE),
                                               m_mapBM2Hwm("Buffer-2HeadWordMorph", DEP_TABLE_SIZE),
                                               m_mapBM3Hw("Buffer-3HeadWord", DEP_TABLE_SIZE),
                                               m_mapBM3Hm("Buffer-3HeadMorph", DEP_TABLE_SIZE),
                                               m_mapBM3Hwm("Buffer-3HeadWordMorph", DEP_TABLE_SIZE),

                                               m_mapNM1w("Cache-1Word", DEP_TABLE_SIZE),
                                               m_mapNM1m("Cache-1Morph", DEP_TABLE_SIZE),
                                               m_mapNM1wm("Cache-1WordMorph", DEP_TABLE_SIZE),
                                               m_mapNM2w("Cache-2Word", DEP_TABLE_SIZE),
                                               m_mapNM2m("Cache-2Morph", DEP_TABLE_SIZE),
                                               m_mapNM2wm("Cache-2WordMorph", DEP_TABLE_SIZE),
                                               m_mapNM3w("Cache-3Word", DEP_TABLE_SIZE),
                                               m_mapNM3m("Cache-3Morph", DEP_TABLE_SIZE),
                                               m_mapNM3wm("Cache-3WordMorph", DEP_TABLE_SIZE),

                                               m_mapB0w("NextUnanalyzedWord", DEP_TABLE_SIZE),
                                               m_mapB1w("Next+1UnanalyzedWord", DEP_TABLE_SIZE),
                                               m_mapB2w("Next+2UnanalyzedWord", DEP_TABLE_SIZE),

                                               m_mapB0p1("NextUnanalyzedPrefix1", DEP_TABLE_SIZE),
                                               m_mapB0p2("NextUnanalyzedPrefix2", DEP_TABLE_SIZE),
                                               m_mapB0p3("NextUnanalyzedPrefix3", DEP_TABLE_SIZE),
                                               m_mapB0p4("NextUnanalyzedPrefix4", DEP_TABLE_SIZE),
                                               m_mapB0s1("NextUnanalyzedSuffix1", DEP_TABLE_SIZE),
                                               m_mapB0s2("NextUnanalyzedSuffix2", DEP_TABLE_SIZE),
                                               m_mapB0s3("NextUnanalyzedSuffix3", DEP_TABLE_SIZE),
                                               m_mapB0s4("NextUnanalyzedSuffix4", DEP_TABLE_SIZE),
                                               m_mapB0hd("NextUnanalyzedSuffixHasDigit", DEP_TABLE_SIZE),
                                               m_mapB0hu("NextUnanalyzedSuffixHasUpper", DEP_TABLE_SIZE),
                                               m_mapB0hh("NextUnanalyzedSuffixHasHyphen", DEP_TABLE_SIZE),

                                               m_mapB1p1("Next+1UnanalyzedPrefix1", DEP_TABLE_SIZE),
                                               m_mapB1p2("Next+1UnanalyzedPrefix2", DEP_TABLE_SIZE),
                                               m_mapB1p3("Next+1UnanalyzedPrefix3", DEP_TABLE_SIZE),
                                               m_mapB1p4("Next+1UnanalyzedPrefix4", DEP_TABLE_SIZE),
                                               m_mapB1s1("Next+1UnanalyzedSuffix1", DEP_TABLE_SIZE),
                                               m_mapB1s2("Next+1UnanalyzedSuffix2", DEP_TABLE_SIZE),
                                               m_mapB1s3("Next+1UnanalyzedSuffix3", DEP_TABLE_SIZE),
                                               m_mapB1s4("Next+1UnanalyzedSuffix4", DEP_TABLE_SIZE),

                                               m_mapSTmp("StackMorphPostag", DEP_TABLE_SIZE),
   	   	   	   	   	   	   	   	   	   	   	   m_mapSTnum("StackNumber", DEP_TABLE_SIZE),
                                               m_mapSTper("StackPerson", DEP_TABLE_SIZE),
                                               m_mapN0mp("NextMorphPostag", DEP_TABLE_SIZE),
   	   	   	   	   	   	   	   	   	   	   	   m_mapN0num("NextNumber", DEP_TABLE_SIZE),
                                               m_mapN0per("NextPerson", DEP_TABLE_SIZE),
                                               m_mapSTnumN0num("StackNumberNextNumber", DEP_TABLE_SIZE),
                                               m_mapSTperN0per("StackPersonNextPerson", DEP_TABLE_SIZE),
                                               m_mapSTvtyN0vty("StackVTypeNextVType", DEP_TABLE_SIZE),
                                               m_mapSTvfoN0vfo("StackVFormNextVForm", DEP_TABLE_SIZE),
                                               m_mapST1numN0num("Stack1NumberNextNumber", DEP_TABLE_SIZE),
                                               m_mapST1perN0per("Stack1PersonNextPerson", DEP_TABLE_SIZE),
                                               m_mapSTnumN1num("StackNumberNext+1Number", DEP_TABLE_SIZE),
                                               m_mapSTperN1per("StackPersonNext+1Person", DEP_TABLE_SIZE),

                                               m_mapSTwmN0wm("StackWordMorphNextWordMorph", DEP_TABLE_SIZE),
                                               m_mapSTwmN0w("StackWordMorphNextWord", DEP_TABLE_SIZE),
                                               m_mapSTwN0wm("StackWordNextWordMorph", DEP_TABLE_SIZE),
                                               m_mapSTmN0wm("StackMorphNextWordMorph", DEP_TABLE_SIZE),
                                               m_mapSTwmN0m("StackWordMorphNextMorph", DEP_TABLE_SIZE),
                                               m_mapSTwN0w("StackWordNextWord", DEP_TABLE_SIZE),
                                               m_mapSTmN0m("StackMorphNextMorph", DEP_TABLE_SIZE),

                                               m_mapBM1mBM2m("Buffer-1-2Morph", DEP_TABLE_SIZE),
                                               m_mapBM1mBM2mBM3m("Buffer-1-2-3Morph", DEP_TABLE_SIZE),

                                               m_mapN0mN1m("NextMorphNext+1Morph", DEP_TABLE_SIZE),
                                               m_mapN0mN1mN2m("NextMorphTrigram", DEP_TABLE_SIZE),
                                               m_mapSTmN0mN1m("StackMorphNextMorphNext+1Morph", DEP_TABLE_SIZE),
                                               m_mapSTmN0mN0LDm("StackMorphNextMorphNextLDMorph", DEP_TABLE_SIZE),
                                               m_mapN0mN0LDmN0L2Dm("StackMorphNextMorphNextLDMorphNextMorphNextL2DMorph", DEP_TABLE_SIZE),
                                               m_mapSTHmSTmN0m("StackHeadMorphStackMorphNextMorph", DEP_TABLE_SIZE),
                                               m_mapHTmHT2mN0m("HeadStackMorphHeadStack2MorphNextMorph", DEP_TABLE_SIZE),
                                               m_mapSTHHmSTHmSTm("StackHeadHeadMorphStackHeadMorphStackMorph", DEP_TABLE_SIZE),
                                               m_mapSTmSTLDmN0m("StackMorphStackLDMorphNextMorph", DEP_TABLE_SIZE),
                                               m_mapSTmSTLDmSTL2Dm("StackMorphStackLDMorphStackL2DMorph", DEP_TABLE_SIZE),
                                               m_mapSTmSTRDmN0m("StackMorphStackRDMorphNextMorph", DEP_TABLE_SIZE),
                                               m_mapSTmSTRDmSTR2Dm("StackMorphStackRDMorphStackR2DMorph", DEP_TABLE_SIZE),

                                               m_mapN0wN1w("NextWordNext+1Word", DEP_TABLE_SIZE),
                                               m_mapN1wN2w("Next+1WordNext+2Word", DEP_TABLE_SIZE),
                                               m_mapN0wN1wN1m("NextWordNext+1WordNext+1Morph", DEP_TABLE_SIZE),

                                               m_mapSTwd("StackWordDist", DEP_TABLE_SIZE),
                                               m_mapSTmd("StackMorphDist", DEP_TABLE_SIZE),
                                               m_mapN0wd("NextWordDist", DEP_TABLE_SIZE),
                                               m_mapN0md("NextMorphDist", DEP_TABLE_SIZE),
                                               m_mapSTwN0wd("StackWordNextWordDist", DEP_TABLE_SIZE),
                                               m_mapSTmN0md("StackMorphNextMorphDist", DEP_TABLE_SIZE),

                                               m_mapSTwra("StackWordRightArity", DEP_TABLE_SIZE),
                                               m_mapSTmra("StackMorphRightArity", DEP_TABLE_SIZE),
                                               m_mapSTwla("StackWordLeftArity", DEP_TABLE_SIZE),
                                               m_mapSTmla("StackMorphLeftArity", DEP_TABLE_SIZE),
                                               m_mapN0wla("NextWordRightArity", DEP_TABLE_SIZE),
                                               m_mapN0mla("NextMorphRightArity", DEP_TABLE_SIZE),

                                               m_mapSTwrp("StackWordRightSetoftags", DEP_TABLE_SIZE),
                                               m_mapSTmrp("StackMorphRightSetoftags", DEP_TABLE_SIZE),
                                               m_mapSTwlp("StackWordLeftSetoftags", DEP_TABLE_SIZE),
                                               m_mapSTmlp("StackMorphLeftSetoftags", DEP_TABLE_SIZE),
                                               m_mapN0wlp("Next0WordLeftSetoftags", DEP_TABLE_SIZE),
                                               m_mapN0mlp("Next0MorphLeftSetoftags", DEP_TABLE_SIZE),

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
