// Copyright (C) University of Oxford 2010
/***************************************************************
 *
 * weight.h - the dependency parser weights
 *
 * Yue Zhang, 2007.8 - 2008.1
 *
 ***************************************************************/

#ifndef DEPPARSER_ARC_STANDARD_WEIGHT_H
#define DEPPARSER_ARC_STANDARD_WEIGHT_H

#include "depparser_weight_base.h"

#define iterate_templates(left, right) do { \
  left(S0w)right; /*  0 - S0.w  */  \
  left(S0t)right; /*  1 - S0.t  */  \
  left(S0wS0t)right; /*  2 - S0.w + S0.t  */  \
  left(S1w)right; /*  3 - S1.w  */  \
  left(S1t)right; /*  4 - S1.t  */  \
  left(S1wS1t)right; /*  5 - S1.w + S1.t  */  \  
  left(S2w)right; /*  86 - S2.w  */  \
  left(S2t)right; /*  87 - S2.t  */  \
  left(S2wS2t)right; /*  88 - S2.w + S2.t  */  \
  left(S3w)right; /*  89 - S3.w  */  \
  left(S3t)right; /*  90 - S3.t  */  \
  left(S3wS3t)right; /*  91 - S3.w + S3.t  */  \
  left(N0w)right; /*  6 - N0.w  */  \
  left(N0t)right; /*  7 - N0.t  */  \
  left(N0wN0t)right; /*  8 - N0.w + N0.t  */  \
  left(N1w)right; /*  9 - N1.w  */  \
  left(N1t)right; /*  10 - N1.t  */  \
  left(N1wN1t)right; /*  11 - N1.w + N1.t  */  \
  left(N2w)right; /*  92 - N2.w  */  \
  left(N2t)right; /*  93 - N2.t  */  \
  left(N2wN2t)right; /*  94 - N2.w + N2.t  */  \
  left(N3w)right; /*  95 - N3.w  */  \
  left(N3t)right; /*  96 - N3.t  */  \
  left(N3wN3t)right; /*  97 - N3.w + N3.t  */  \
  left(S0wS1w)right; /*  12 - S0.w + S1.w  */  \
  left(S0wS1t)right; /*  13 - S0.w + S1.t  */  \
  left(S0tS1w)right; /*  14 - S0.t + S1.w  */  \
  left(S0tS1t)right; /*  15 - S0.t + S1.t  */  \
  left(S0wN0w)right; /*  16 - S0.w + N0.w  */  \
  left(S0wN0t)right; /*  17 - S0.w + N0.t  */  \
  left(S0tN0w)right; /*  18 - S0.t + N0.w  */  \
  left(S0tN0t)right; /*  19 - S0.t + N0.t  */  \  
  left(S0wtS1t)right; /*  111 - S0.wt + S1.t  */  \
  left(S0wtS1w)right; /*  112 - S0.tt + S1.w  */  \
  left(S1wtS0t)right; /*  113 - S0.t + S1.wt  */  \
  left(S1wtS0w)right; /*  114 - S0.w + S1.wt  */  \ 
  left(S0wtN0t)right; /*  116 - S0.wt + NO.t  */  \
  left(S0wtN0w)right; /*  117 - S0.tw + NO.w  */  \
  left(N0wtS0t)right; /*  118 - S0.t + NO.wt  */  \
  left(N0wtS0w)right; /*  119 - S0.w + NO.wt  */  \  
  left(S0wtS1wt)right;/*  110 - S0.wt + S1.wt  */ \
  left(S0wtN0wt)right;/*  115 - S0.wt + NO.wt  */ \
  left(S0wN0w)right; /*  16 - S0.w + N0.w  */  \
  left(S0wN0t)right; /*  17 - S0.w + N0.t  */  \
  left(S0tN0w)right; /*  18 - S0.t + N0.w  */  \
  left(S0tN0t)right; /*  19 - S0.t + N0.t  */  \
  left(S0l1dw)right; /*  20 - S0.ld.w  */  \
  left(S0l1dt)right; /*  21 - S0.ld.t  */  \
  left(S0l1dwS0l1dt)right; /*  22 - S0.ld.w + S0.ld.t  */  \
  left(S0l1dd)right; /*  23 - S0.ld.d  */  \
  left(S0r1dw)right; /*  24 - S0.rd.w  */  \
  left(S0r1dt)right; /*  25 - S0.rd.t  */  \
  left(S0r1dwS0r1dt)right; /*  26 - S0.rd.w + S0.rd.t  */  \
  left(S0r1dd)right; /*  27 - S0.rd.d  */  \
  left(S1l1dw)right; /*  28 - S1.ld.w  */  \
  left(S1l1dt)right; /*  29 - S1.ld.t  */  \
  left(S1l1dwS1l1dt)right; /*  30 - S1.ld.w + S1.ld.t  */  \
  left(S1l1dd)right; /*  31 - S1.ld.d  */  \
  left(S1r1dw)right; /*  32 - S1.rd.w  */  \
  left(S1r1dt)right; /*  33 - S1.rd.t  */  \
  left(S1r1dwS1r1dt)right; /*  34 - S1.rd.w + S1.rd.t  */  \
  left(S1r1dd)right; /*  35 - S1.rd.d  */  \
  left(S0l2dw)right; /*  36 - S0.l2d.w  */  \
  left(S0l2dt)right; /*  37 - S0.l2d.t  */  \
  left(S0l2dwS0l2dt)right; /*  38 - S0.l2d.w + S0.l2d.t  */  \
  left(S0l2dd)right; /*  39 - S0.l2d.d  */  \
  left(S0r2dw)right; /*  40 - S0.r2d.w  */  \
  left(S0r2dt)right; /*  41 - S0.r2d.t  */  \
  left(S0r2dwS0r2dt)right; /*  42 - S0.r2d.w + S0.r2d.t  */  \
  left(S0r2dd)right; /*  43 - S0.r2d.d  */  \
  left(S1l2dw)right; /*  44 - S1.l2d.w  */  \
  left(S1l2dt)right; /*  45 - S1.l2d.t  */  \
  left(S1l2dwS1l2dt)right; /*  46 - S1.l2d.w + S1.l2d.t  */  \
  left(S1l2dd)right; /*  47 - S1.l2d.d  */  \
  left(S1r2dw)right; /*  48 - S1.r2d.w  */  \
  left(S1r2dt)right; /*  49 - S1.r2d.t  */  \
  left(S1r2dwS1r2dt)right; /*  50 - S1.r2d.w + S1.r2d.t  */  \
  left(S1r2dd)right; /*  51 - S1.r2d.d  */  \
  left(S0tS1tS1r1dt)right; /*  52 - S1.t + S1.rd.t + S0.t  */  \
  left(S0tS1tS0l1dt)right; /*  53 - S0.t + S0.ld.t + S1.t  */  \
  left(S0tS1tS1l1dt)right; /*  54 - S1.t + S1.ld.t + S0.t  */  \
  left(S0tS1tS0r1dt)right; /*  55 - S0.t + S0.rd.t + S1.t  */  \
  left(S0wS0ra)right; /*  56 - S0.w + S0.ra  */  \
  left(S0tS0ra)right; /*  57 - S0.t + S0.ra  */  \
  left(S0wS0la)right; /*  58 - S0.w + S0.la  */  \
  left(S0tS0la)right; /*  59 - S0.t + S0.la  */  \
  left(S1wS1ra)right; /*  60 - S1.w + S1.ra  */  \
  left(S1tS1ra)right; /*  61 - S1.t + S1.ra  */  \
  left(S1wS1la)right; /*  62 - S1.w + S1.la  */  \
  left(S1tS1la)right; /*  63 - S1.t + S1.la  */  \
  left(S0wS0lset)right; /*  64 - S0.w + S0.lset  */  \
  left(S0tS0lset)right; /*  65 - S0.t + S0.lset  */  \
  left(S0wS0rset)right; /*  66 - S0.w + S0.rset  */  \
  left(S0tS0rset)right; /*  67 - S0.t + S0.rset  */  \
  left(S1wS1lset)right; /*  68 - S1.w + S1.lset  */  \
  left(S1tS1lset)right; /*  69 - S1.t + S1.lset  */  \
  left(S1wS1rset)right; /*  70 - S1.w + S1.rset  */  \
  left(S1tS1rset)right; /*  71 - S1.t + S1.rset  */  \
  left(S0wDist)right; /*  72 - S0.w + Dist(S0, S1)  */  \
  left(S0tDist)right; /*  73 - S0.t + Dist(S0, S1)  */  \
  left(S1wDist)right; /*  74 - S0.w + Dist(S0, S1)  */  \
  left(S1tDist)right; /*  75 - S0.t + Dist(S0, S1)  */  \
  left(S0wS1wDist)right; /*  76 - S0.w + S1.w + Dist(S0, S1)  */  \
  left(S0tS1tDist)right; /*  77 - S0.t + S1.t + Dist(S0, S1)  */  \
  left(S0tS0l1dtS0l2dt)right; /* 78 */ \
  left(S0tS0r1dtS0r2dt)right; /* 79 */ \
  left(S1tS1l1dtS1l2dt)right; /* 80 */ \
  left(S1tS1r1dtS1r2dt)right; /* 81 */  \
  left(S0tS1tS1r2dt)right;    /* 82 */  \
  left(S0tS1tS0l2dt)right;    /* 83 */  \
  left(S0tS1tS1l2dt)right;    /* 84 */  \
  left(S0tS1tS0r2dt)right;    /* 85 */  \
  left(S0wS0l1dtS0l2dt)right; /* 98 */ \
  left(S0wS0r1dtS0r2dt)right; /* 99 */ \
  left(S1wS1l1dtS1l2dt)right; /* 100 */ \
  left(S1wS1r1dtS1r2dt)right; /* 101 */  \
  left(S0wS1tS1r1dt)right;    /* 102 */  \
  left(S0wS1tS0l1dt)right;    /* 103 */  \
  left(S0wS1tS1l1dt)right;    /* 104 */  \
  left(S0wS1tS0r1dt)right;    /* 105 */  \
  left(S1wS0tS1r1dt)right;    /* 106 */  \
  left(S1wS0tS0l1dt)right;    /* 107 */  \
  left(S1wS0tS1l1dt)right;    /* 108 */  \
  left(S1wS0tS0r1dt)right;    /* 109 */  \
  left(N0tN1t)right;/* 120 - No.t + N1.t */ \
  left(N0tN1tN2t)right;/* 121 - No.t + N1.t + N2.t */ \
  left(S0tN0tN1t)right;/* 122 - No.t + N1.t */ \
  left(S0tS1tS2t)right;/* 123 - S0.t + S1.t + S2.t */ \
  left(S0tS1tN0t)right;/* 124 - S0.t + S1.t + N0.t */ \
  left(S0wS1tN0t)right;/* 126 - S0.w + S1.t + N0.t */ \
  left(S0wS1wN0t)right;/* 127 - S0.w + S1.w + N0.t */ \
  left(S1wS0tN0t)right;/* 128 - S0.w + S1.t + N0.t */ \
  left(S0wN0tN1t)right;/* 129 - S0.w + N0.t + N1.t */ \ 
  left(S0wN0wN1t)right;/* 130 - S0.w + N0.w + N1.t */ \ 
  left(N0wS0tN1t)right;/* 131 - S0.t + N0.w + N1.t */ \ 
  left(S0wS1tS2t)right;/* 132 - S0.w + S1.t + S2.t */ \ 
  left(S0wS1wS2t)right;/* 133 - S0.w + S1.w + S2.t */ \ 
  left(S1wS0tS2t)right;/* 134 - S0.t + S1.w + S2.t */ \ 
  left(S0wS0raS0la)right;/* 135 - S0.w + S0.ra + S0.la */ \
  left(S0tS0raS0la)right;/* 136 - S0.t + S0.ra + S0.la */ \
  left(S1wS1raS1la)right;/* 137 - S1.w + S1.ra + S1.la */ \
  left(S1tS1raS1la)right;/* 138 - S1.t + S1.ra + S1.la */ \
  left(S0wS0lsetS0rset)right; /* 139 - S0.w + S0.lset + S0.rset */ \
  left(S0tS0lsetS0rset)right;/* 140 - S0.t + S0.lset + S0.rset */ \
  left(S1wS1lsetS1rset)right; /* 141 - S1.w + S1.lset + S1.rset */ \
  left(S1tS1lsetS1rset)right;/* 142 - S1.t + S1.lset + S1.rset */ \  
  left(S0wS1tDist)right; /*143-S0.w+S1.t+Dist(S0,S1) */ \
  left(S1wS0tDist)right;/* 144-S1.w+S0.t+Dist(S0,S1) */ \
  left(m_mapCFG)right;/**/\
  left(S0C)right;/*146 S0Constituent*/\
  left(S1C)right;/*147 S1Constituent*/\
  left(S0CS1C)right;/*148 S0C+S1C*/\
  left(S0wS1C)right;/*149*/\
  left(S0tS1C)right;/*150*/\
  left(S0wtS1C)right;/*151*/\
  left(S0wS1dS1C)right;/*152*/\
  left(S1wS0C)right;/*153*/\
  left(S1tS0C)right;/*154*/\
  left(S1wtS0C)right;/*155*/\
  left(S1wS0dS0C)right;/*156*/\
  left(S1S0PCFG)right;/*157*/\
  left(S1wS0tPCFG)right;/*158*/\
  left(S0wS1tPCFG)right;/*159*/\
} while (0);

namespace TARGET_LANGUAGE {

namespace depparser {

const static unsigned DEP_TABLE_SIZE = (1<<17);//1000121;

//
// TYPE DEFINITIONS
//
typedef CPackedScoreMap<CWord, SCORE_TYPE, action::kMax> CWordMap;
typedef CPackedScoreMap<CTag, SCORE_TYPE, action::kMax> CTagMap;
typedef CPackedScoreMap<int, SCORE_TYPE, action::kMax> CIntMap;
typedef CPackedScoreMap<CTuple2<CTag, CTag>, SCORE_TYPE, action::kMax> \
  CTagTagMap;
typedef CPackedScoreMap<CTuple3<CTag, CTag, CTag>, SCORE_TYPE, action::kMax> \
  CTagTagTagMap;
typedef CPackedScoreMap<CTagSet<CTag, 2>, SCORE_TYPE, action::kMax> \
  CTagSet2Map;
typedef CPackedScoreMap<CTagSet<CTag, 3>, SCORE_TYPE, action::kMax> \
  CTagSet3Map;
typedef CPackedScoreMap<CTagSet<CTag, 4>, SCORE_TYPE, action::kMax> \
  CTagSet4Map;
typedef CPackedScoreMap<CTuple2<CWord, CTag>, SCORE_TYPE, action::kMax> \
  CWordTagMap;
typedef CPackedScoreMap<CTuple2<CTaggedWord<CTag, TAG_SEPARATOR>, CTag>, SCORE_TYPE, action::kMax> \
  CTaggedWordTagMap;
typedef CPackedScoreMap<CTuple2<CTaggedWord<CTag, TAG_SEPARATOR>, CWord>, SCORE_TYPE, action::kMax> \
  CTaggedWordWordMap;
typedef CPackedScoreMap<CTuple3<CWord, CTag, CTag>, SCORE_TYPE, action::kMax> \
  CWordTagTagMap;
typedef CPackedScoreMap<CTuple3<CWord, CWord, CTag>, SCORE_TYPE, action::kMax> \
  CWordWordTagMap;
typedef CPackedScoreMap<CTaggedWord<CTag, TAG_SEPARATOR>, SCORE_TYPE, action::kMax> \
  CTaggedWordMap;
typedef CPackedScoreMap<CTuple2<CWord, CWord>, SCORE_TYPE, action::kMax> \
  CWordWordMap;
typedef CPackedScoreMap<CTwoWords, SCORE_TYPE, action::kMax> \
  CTwoWordsMap;
typedef CPackedScoreMap<CTuple2< CTaggedWord<CTag, TAG_SEPARATOR>,CTaggedWord<CTag, TAG_SEPARATOR> >, SCORE_TYPE, action::kMax> \
  CTwoTaggedWordsMap;
typedef CPackedScoreMap<CTuple2<CWord, int>, SCORE_TYPE, action::kMax> \
  CWordIntMap;
typedef CPackedScoreMap<CTuple2<CTag, int>, SCORE_TYPE, action::kMax> \
  CTagIntMap;
typedef CPackedScoreMap<CTuple3<CTag, CTag, int>, SCORE_TYPE, action::kMax> \
  CTagTagIntMap;
typedef CPackedScoreMap<CTuple3<CWord, CWord, int>, SCORE_TYPE, action::kMax> \
  CWordWordIntMap;
typedef CPackedScoreMap<CTuple3<CTag, int, int>, SCORE_TYPE, action::kMax> \
  CTagIntIntMap;
typedef CPackedScoreMap<CTuple3<CWord, int, int>, SCORE_TYPE, action::kMax> \
  CWordIntIntMap;
typedef CPackedScoreMap<CTuple3<CWord, CTag, int>, SCORE_TYPE, action::kMax> \
  CWordTagIntMap;
typedef CPackedScoreMap<CTuple2< CWord, CSetOfTags<CDependencyLabel> >, \
  SCORE_TYPE, action::kMax> CWordSetOfLabelsMap;
typedef CPackedScoreMap<CTuple2< CTag, CSetOfTags<CDependencyLabel> >, \
  SCORE_TYPE, action::kMax> CTagSetOfLabelsMap;
typedef CPackedScoreMap<CTuple3< CWord, CSetOfTags<CDependencyLabel>, CSetOfTags<CDependencyLabel> >, \
  SCORE_TYPE, action::kMax> CWordTwoSetsOfLabelsMap;
typedef CPackedScoreMap<CTuple3< CTag, CSetOfTags<CDependencyLabel>,CSetOfTags<CDependencyLabel> >, \
  SCORE_TYPE, action::kMax> CTagTwoSetsOfLabelsMap;

typedef CPackedScoreMap<CLemma, SCORE_TYPE, action::kMax> CLemmaMap;
typedef CPackedScoreMap<CCoNLLCPOS, SCORE_TYPE, action::kMax> CCoNLLCPOSMap;
typedef CPackedScoreMap<CCoNLLFeats, SCORE_TYPE, action::kMax> CCoNLLFeatsMap;
typedef CPackedScoreMap<CTuple3<unsigned long, unsigned long, unsigned long>, SCORE_TYPE, action::kMax> CIntSet3Map;
typedef CPackedScoreMap<CTuple2<unsigned long, unsigned long>, SCORE_TYPE, action::kMax> CIntSet2Map;
typedef CPackedScoreMap<CTuple2<CTaggedWord<CTag, TAG_SEPARATOR>, int>, SCORE_TYPE, action::kMax> \
  CTaggedWordIntMap;
typedef CPackedScoreMap<CTuple2<CWord, unsigned long>, SCORE_TYPE, action::kMax> \
  CWordULongMap;
typedef CPackedScoreMap<CTuple2<CTag, unsigned long>, SCORE_TYPE, action::kMax> \
  CTagULongMap;
typedef CPackedScoreMap<CTuple2<CTaggedWord<CTag, TAG_SEPARATOR>, unsigned long>, SCORE_TYPE, action::kMax> \
  CTaggedWordULongMap;
typedef CPackedScoreMap<CTuple3<CWord, unsigned long, unsigned long>, SCORE_TYPE, action::kMax> \
  CWordULongULongMap;
typedef CPackedScoreMap<CTuple3<CWord, unsigned, unsigned long>, SCORE_TYPE, action::kMax> \
  CWordUULongMap;
typedef CPackedScoreMap<CTuple3<CWord, CWord, unsigned long>, SCORE_TYPE, action::kMax> \
  CWordWordULongMap;
typedef CPackedScoreMap<CTuple3<CWord, CTag, unsigned long>, SCORE_TYPE, action::kMax> \
  CWordTagULongMap;



/*===============================================================
 *
 * CWeight - the definition of weights, in feature
 *
 *==============================================================*/

class CWeight : public CWeightBase {
public:
  // Basic Features
  CWordMap        S0w;          /* 0 - S0.w */
  CTagMap         S0t;          /* 1 - S0.t */
  CTaggedWordMap  S0wS0t;       /* 2 - S0.w + S0.t */

  CWordMap        S1w;          /* 3 - S1.w */
  CTagMap         S1t;          /* 4 - S1.t */
  CTaggedWordMap  S1wS1t;       /* 5 - S1.w + S1.t */

  CWordMap        S2w;          /* 86 - S2.w */
  CTagMap         S2t;          /* 87 - S2.t */
  CTaggedWordMap  S2wS2t;       /* 88 - S2.w + S2.t */

  CWordMap        S3w;          /* 89 - S3.w */
  CTagMap         S3t;          /* 90 - S3.t */
  CTaggedWordMap  S3wS3t;       /* 91 - S3.w + S3.t */

  CWordMap        N0w;          /* 6 - N0.w */
  CTagMap         N0t;          /* 7 - N0.t */
  CTaggedWordMap  N0wN0t;       /* 8 - N0.w + N0.t */

  CWordMap        N1w;          /* 9 - N1.w */
  CTagMap         N1t;          /* 10 - N1.t */
  CTaggedWordMap  N1wN1t;       /* 11 - N1.w + N1.t */

  CWordMap        N2w;          /* 92 - N2.w */
  CTagMap         N2t;          /* 93 - N2.t */
  CTaggedWordMap  N2wN2t;       /* 94 - N2.w + N2.t */

  CWordMap        N3w;          /* 95 - N3.w */
  CTagMap         N3t;          /* 96 - N3.t */
  CTaggedWordMap  N3wN3t;       /* 97 - N3.w + N3.t */

  CWordWordMap    S0wS1w;       /* 12 - S0.w + S1.w */
  CWordTagMap     S0wS1t;       /* 13 - S0.w + S1.t */
  CWordTagMap     S0tS1w;       /* 14 - S0.t + S1.w */
  CTagTagMap      S0tS1t;       /* 15 - S0.t + S1.t */

  CWordWordMap    S0wN0w;       /* 16 - S0.w + N0.w */
  CWordTagMap     S0wN0t;       /* 17 - S0.w + N0.t */
  CWordTagMap     S0tN0w;       /* 18 - S0.t + N0.w */
  CTagTagMap      S0tN0t;       /* 19 - S0.t + N0.t */

  // Second-order features
  CWordMap        S0l1dw;       /* 20 - S0.ld.w */
  CTagMap         S0l1dt;       /* 21 - S0.ld.t */
  CTaggedWordMap  S0l1dwS0l1dt; /* 22 - S0.ld.w + S0.ld.t */
  CIntMap         S0l1dd;       /* 23 - S0.ld.d */

  CWordMap        S0r1dw;       /* 24 - S0.rd.w */
  CTagMap         S0r1dt;       /* 25 - S0.rd.t */
  CTaggedWordMap  S0r1dwS0r1dt; /* 26 - S0.rd.w + S0.rd.t */
  CIntMap         S0r1dd;       /* 27 - S0.rd.d */

  CWordMap        S1l1dw;       /* 28 - S1.ld.w */
  CTagMap         S1l1dt;       /* 29 - S1.ld.t */
  CTaggedWordMap  S1l1dwS1l1dt; /* 30 - S1.ld.w + S1.ld.t */
  CIntMap         S1l1dd;       /* 31 - S1.ld.d */

  CWordMap        S1r1dw;       /* 32 - S1.rd.w */
  CTagMap         S1r1dt;       /* 33 - S1.rd.t */
  CTaggedWordMap  S1r1dwS1r1dt; /* 34 - S1.rd.w + S1.rd.t */
  CIntMap         S1r1dd;       /* 35 - S1.rd.d */

  // Third-order features
  CWordMap        S0l2dw;       /* 36 - S0.l2d.w */
  CTagMap         S0l2dt;       /* 37 - S0.l2d.t */
  CTaggedWordMap  S0l2dwS0l2dt; /* 38 - S0.l2d.w + S0.l2d.t */
  CIntMap         S0l2dd;       /* 39 - S0.l2d.d */

  CWordMap        S0r2dw;       /* 40 - S0.r2d.w */
  CTagMap         S0r2dt;       /* 41 - S0.r2d.t */
  CTaggedWordMap  S0r2dwS0r2dt; /* 42 - S0.r2d.w + S0.r2d.t */
  CIntMap         S0r2dd;       /* 43 - S0.r2d.d */

  CWordMap        S1l2dw;       /* 44 - S1.l2d.w */
  CTagMap         S1l2dt;       /* 45 - S1.l2d.t */
  CTaggedWordMap  S1l2dwS1l2dt; /* 46 - S1.l2d.w + S1.l2d.t */
  CIntMap         S1l2dd;       /* 47 - S1.l2d.d */

  CWordMap        S1r2dw;       /* 48 - S1.r2d.w */
  CTagMap         S1r2dt;       /* 49 - S1.r2d.t */
  CTaggedWordMap  S1r2dwS1r2dt; /* 50 - S1.r2d.w + S1.r2d.t */
  CIntMap         S1r2dd;       /* 51 - S1.r2d.d */

  // Potential third-order features
  CTagTagTagMap   S0tS1tS1r1dt;   /* 52 - S1.t + S1.rd.t + S0.t */
  CTagTagTagMap   S0tS1tS0l1dt;   /* 53 - S0.t + S0.ld.t + S1.t */
  CTagTagTagMap   S0tS1tS1l1dt;   /* 54 - S1.t + S1.ld.t + S0.t */
  CTagTagTagMap   S0tS1tS0r1dt;   /* 55 - S0.t + S0.rd.t + S1.t */

  CWordIntMap   S0wS0ra;    /* 56 - S0.w + S0.ra */
  CTagIntMap    S0tS0ra;    /* 57 - S0.t + S0.ra */
  CWordIntMap   S0wS0la;    /* 58 - S0.w + S0.la */
  CTagIntMap    S0tS0la;    /* 59 - S0.t + S0.la */

  CWordIntMap   S1wS1ra;    /* 60 - S1.w + S1.ra */
  CTagIntMap    S1tS1ra;    /* 61 - S1.t + S1.ra */
  CWordIntMap   S1wS1la;    /* 62 - S1.w + S1.la */
  CTagIntMap    S1tS1la;    /* 63 - S1.t + S1.la */

  CWordSetOfLabelsMap   S0wS0lset;  /* 64 - S0.w + S0.lset */
  CTagSetOfLabelsMap    S0tS0lset;  /* 65 - S0.t + S0.lset */
  CWordSetOfLabelsMap   S0wS0rset;  /* 66 - S0.w + S0.rset */
  CTagSetOfLabelsMap    S0tS0rset;  /* 67 - S0.t + S0.rset */
  CWordSetOfLabelsMap   S1wS1lset;  /* 68 - S1.w + S1.lset */
  CTagSetOfLabelsMap    S1tS1lset;  /* 69 - S1.t + S1.lset */
  CWordSetOfLabelsMap   S1wS1rset;  /* 70 - S1.w + S1.rset */
  CTagSetOfLabelsMap    S1tS1rset;  /* 71 - S1.t + S1.rset */

  CWordIntMap           S0wDist;    /* 72 - S0.w + Dist(S0, S1) */
  CTagIntMap            S0tDist;    /* 73 - S0.t + Dist(S0, S1) */
  CWordIntMap           S1wDist;    /* 74 - S0.w + Dist(S0, S1) */
  CTagIntMap            S1tDist;    /* 75 - S0.t + Dist(S0, S1) */
  CWordWordIntMap       S0wS1wDist; /* 76 - S0.w + S1.w + Dist(S0, S1) */
  CTagTagIntMap         S0tS1tDist; /* 77 - S0.t + S1.t + Dist(S0, S1) */

  CTagTagTagMap   S0tS0l1dtS0l2dt;   /* 78 - S0.t + S0.ld.t + S0.l2d.t */
  CTagTagTagMap   S0tS0r1dtS0r2dt;   /* 79 - S0.t + S0.rd.t + S0.r2d.t */
  CTagTagTagMap   S1tS1l1dtS1l2dt;   /* 80 - S1.t + S1.ld.t + S1.l2d.t */
  CTagTagTagMap   S1tS1r1dtS1r2dt;   /* 81 - S1.t + S1.rd.t + S1.r2d.t */

  CTagTagTagMap   S0tS1tS1r2dt;   /* 82 - S1.t + S1.rd.t + S0.t */
  CTagTagTagMap   S0tS1tS0l2dt;   /* 83 - S0.t + S0.ld.t + S1.t */
  CTagTagTagMap   S0tS1tS1l2dt;   /* 84 - S1.t + S1.ld.t + S0.t */
  CTagTagTagMap   S0tS1tS0r2dt;   /* 85 - S0.t + S0.rd.t + S1.t */

  CWordTagTagMap   S0wS0l1dtS0l2dt;   /* 98 - S0.w + S0.ld.t + S0.l2d.t */
  CWordTagTagMap   S0wS0r1dtS0r2dt;   /* 99 - S0.w + S0.rd.t + S0.r2d.t */
  CWordTagTagMap   S1wS1l1dtS1l2dt;   /* 100 - S1.w + S1.ld.t + S1.l2d.t */
  CWordTagTagMap   S1wS1r1dtS1r2dt;   /* 101 - S1.w + S1.rd.t + S1.r2d.t */

  CWordTagTagMap   S0wS1tS1r1dt;   /* 102 - S1.t + S1.rd.t + S0.w */
  CWordTagTagMap   S0wS1tS0l1dt;   /* 103 - S0.w + S0.ld.t + S1.t */
  CWordTagTagMap   S0wS1tS1l1dt;   /* 104 - S1.t + S1.ld.t + S0.w */
  CWordTagTagMap   S0wS1tS0r1dt;   /* 105 - S0.w + S0.rd.t + S1.t */

  CWordTagTagMap   S1wS0tS1r1dt;   /* 106 - S1.w + S1.rd.t + S0.t */
  CWordTagTagMap   S1wS0tS0l1dt;   /* 107 - S0.t + S0.ld.t + S1.w */
  CWordTagTagMap   S1wS0tS1l1dt;   /* 108 - S1.w + S1.ld.t + S0.t */
  CWordTagTagMap   S1wS0tS0r1dt;   /* 109 - S0.t + S0.rd.t + S1.w */

  CTwoTaggedWordsMap S0wtS1wt;/*  110 - S0.wt + S1.wt  */
  CTaggedWordTagMap S0wtS1t;/*  111 - S0.wt + S1.t  */
  CTaggedWordWordMap S0wtS1w;/*  112 - S0.tt + S1.w  */
  CTaggedWordTagMap S1wtS0t;/*  113 - S0.t + S1.wt  */
  CTaggedWordWordMap S1wtS0w;/*  114 - S0.w + S1.wt  */
  CTwoTaggedWordsMap S0wtN0wt;/*  115 - S0.wt + NO.wt  */ 
  CTaggedWordTagMap S0wtN0t;/*  116 - S0.wt + NO.t  */ 
  CTaggedWordWordMap S0wtN0w;/*  117 - S0.tw + NO.w  */
  CTaggedWordTagMap N0wtS0t;/*  118 - S0.t + NO.wt  */ 
  CTaggedWordWordMap N0wtS0w;/*  119 - S0.w + NO.wt  */ 
  CTagTagMap N0tN1t;/* 120 - No.t + N1.t */ 
  CTagTagTagMap N0tN1tN2t;/* 121 - No.t + N1.t + N2.t */
  CTagTagTagMap S0tN0tN1t;/* 122 - No.t + N1.t */ 
  CTagTagTagMap S0tS1tS2t;/* 123 - S0.t + S1.t + S2.t */

  CTagTagTagMap S0tS1tN0t;/* 124 - S0.t + S1.t + N0.t */
  CWordTagTagMap S0wS1tN0t;/* 126 - S0.w + S1.t + N0.t */
  CWordWordTagMap S0wS1wN0t;/* 127 - S0.w + S1.w + N0.t */
  CWordTagTagMap S1wS0tN0t;/* 128 - S0.w + S1.t + N0.t */
  CWordTagTagMap S0wN0tN1t;/* 129 - S0.w + N0.t + N1.t */
  CWordWordTagMap S0wN0wN1t;/* 130 - S0.w + N0.w + N1.t */
  CWordTagTagMap N0wS0tN1t;/* 131 - S0.t + N0.w + N1.t */
  CWordTagTagMap S0wS1tS2t;/* 132 - S0.w + S1.t + S2.t */
  CWordWordTagMap S0wS1wS2t;/* 133 - S0.w + S1.w + S2.t */
  CWordTagTagMap S1wS0tS2t;/* 134 - S1.t + S0.w + S2.t */
  
  CWordIntIntMap S0wS0raS0la;/* 135 - S0.w + S0.ra + S0.la */ \
  CTagIntIntMap S0tS0raS0la;/* 136 - S0.t + S0.ra + S0.la */ \
  CWordIntIntMap S1wS1raS1la;/* 137 - S1.w + S1.ra + S1.la */ \
  CTagIntIntMap S1tS1raS1la;/* 138 - S1.t + S1.ra + S1.la */ \
  CWordTwoSetsOfLabelsMap S0wS0lsetS0rset; /* 139 - S0.w + S0.lset + S0.rset */ \
  CTagTwoSetsOfLabelsMap S0tS0lsetS0rset;/* 140 - S0.t + S0.lset + S0.rset */ \
  CWordTwoSetsOfLabelsMap S1wS1lsetS1rset; /* 141 - S1.w + S1.lset + S1.rset */ \
  CTagTwoSetsOfLabelsMap S1tS1lsetS1rset;/* 142 - S1.t + S1.lset + S1.rset */ \  
  CWordTagIntMap S0wS1tDist; /*143-S0.w+S1.t+Dist(S0,S1) */ \
  CWordTagIntMap S1wS0tDist;/* 144-S1.w+S0.t+Dist(S0,S1) */ \
  CIntSet3Map m_mapCFG;/*145 CFG*/\
  CIntMap S0C;/*146 S0Constituent*/\
  CIntMap S1C;/*147 S1Constituent*/\
  CIntSet2Map S0CS1C;/*148 S0C+S1C*/\
  CWordULongMap S0wS1C;/*149*/\
  CTagULongMap S0tS1C;/*150*/\
  CTaggedWordULongMap S0wtS1C;/*151*/\
  CWordUULongMap S0wS1dS1C;/*152*/\
  CWordULongMap S1wS0C;/*153*/\
  CTagULongMap S1tS0C;/*154*/\
  CTaggedWordULongMap S1wtS0C;/*155*/\
  CWordUULongMap S1wS0dS0C;/*156*/\
  CWordWordULongMap S1S0PCFG;/*157*/\
  CWordTagULongMap S1wS0tPCFG;/*158*/\
  CWordTagULongMap S0wS1tPCFG;/*159*/\


public:
  CWeight(const std::string &sPath, bool bTrain)
    : CWeightBase(sPath, bTrain),
    S0w("0-S0.w", DEP_TABLE_SIZE),
    S0t("1-S0.t", DEP_TABLE_SIZE),
    S0wS0t("2-S0.w+S0.t", DEP_TABLE_SIZE),
    S1w("3-S1.w", DEP_TABLE_SIZE),
    S1t("4-S1.t", DEP_TABLE_SIZE),
    S1wS1t("5-S1.w+S1.t", DEP_TABLE_SIZE),
    N0w("6-N0.w", DEP_TABLE_SIZE),
    N0t("7-N0.t", DEP_TABLE_SIZE),
    N0wN0t("8-N0.w+N0.t", DEP_TABLE_SIZE),
    N1w("9-N1.w", DEP_TABLE_SIZE),
    N1t("10-N1.t", DEP_TABLE_SIZE),
    N1wN1t("11-N1.w+N1.t", DEP_TABLE_SIZE),
    S0wS1w("12-S0.w+S1.w", DEP_TABLE_SIZE),
    S0wS1t("13-S0.w+S1.t", DEP_TABLE_SIZE),
    S0tS1w("14-S0.t+S1.w", DEP_TABLE_SIZE),
    S0tS1t("15-S0.t+S1.t", DEP_TABLE_SIZE),
    S0wN0w("16-S0.w+N0.w", DEP_TABLE_SIZE),
    S0wN0t("17-S0.w+N0.t", DEP_TABLE_SIZE),
    S0tN0w("18-S0.t+N0.w", DEP_TABLE_SIZE),
    S0tN0t("19-S0.t+N0.t", DEP_TABLE_SIZE),
    S0l1dw("20-S0.ld.w", DEP_TABLE_SIZE),
    S0l1dt("21-S0.ld.t", DEP_TABLE_SIZE),
    S0l1dwS0l1dt("22-S0.ld.w+S0.ld.t", DEP_TABLE_SIZE),
    S0l1dd("23-S0.ld.d", DEP_TABLE_SIZE),
    S0r1dw("24-S0.rd.w", DEP_TABLE_SIZE),
    S0r1dt("25-S0.rd.t", DEP_TABLE_SIZE),
    S0r1dwS0r1dt("26-S0.rd.w+S0.rd.t", DEP_TABLE_SIZE),
    S0r1dd("27-S0.rd.d", DEP_TABLE_SIZE),
    S1l1dw("28-S1.ld.w", DEP_TABLE_SIZE),
    S1l1dt("29-S1.ld.t", DEP_TABLE_SIZE),
    S1l1dwS1l1dt("30-S1.ld.w+S1.ld.t", DEP_TABLE_SIZE),
    S1l1dd("31-S1.ld.d", DEP_TABLE_SIZE),
    S1r1dw("32-S1.rd.w", DEP_TABLE_SIZE),
    S1r1dt("33-S1.rd.t", DEP_TABLE_SIZE),
    S1r1dwS1r1dt("34-S1.rd.w+S1.rd.t", DEP_TABLE_SIZE),
    S1r1dd("35-S1.rd.d", DEP_TABLE_SIZE),
    S0l2dw("36-S0.l2d.w", DEP_TABLE_SIZE),
    S0l2dt("37-S0.l2d.t", DEP_TABLE_SIZE),
    S0l2dwS0l2dt("38-S0.l2d.w+S0.l2d.t", DEP_TABLE_SIZE),
    S0l2dd("39-S0.l2d.d", DEP_TABLE_SIZE),
    S0r2dw("40-S0.r2d.w", DEP_TABLE_SIZE),
    S0r2dt("41-S0.r2d.t", DEP_TABLE_SIZE),
    S0r2dwS0r2dt("42-S0.r2d.w+S0.r2d.t", DEP_TABLE_SIZE),
    S0r2dd("43-S0.r2d.d", DEP_TABLE_SIZE),
    S1l2dw("44-S1.l2d.w", DEP_TABLE_SIZE),
    S1l2dt("45-S1.l2d.t", DEP_TABLE_SIZE),
    S1l2dwS1l2dt("46-S1.l2d.w+S1.l2d.t", DEP_TABLE_SIZE),
    S1l2dd("47-S1.l2d.d", DEP_TABLE_SIZE),
    S1r2dw("48-S1.r2d.w", DEP_TABLE_SIZE),
    S1r2dt("49-S1.r2d.t", DEP_TABLE_SIZE),
    S1r2dwS1r2dt("50-S1.r2d.w+S1.r2d.t", DEP_TABLE_SIZE),
    S1r2dd("51-S1.r2d.d", DEP_TABLE_SIZE),
    S0tS1tS1r1dt("52-S1.t+S1.rd.t+S0.t", DEP_TABLE_SIZE),
    S0tS1tS0l1dt("53-S0.t+S0.ld.t+S1.t", DEP_TABLE_SIZE),
    S0tS1tS1l1dt("54-S1.t+S1.ld.t+S0.t", DEP_TABLE_SIZE),
    S0tS1tS0r1dt("55-S0.t+S0.rd.t+S1.t", DEP_TABLE_SIZE),
    S0wS0ra("56-S0.w+S0.ra", DEP_TABLE_SIZE),
    S0tS0ra("57-S0.t+S0.ra", DEP_TABLE_SIZE),
    S0wS0la("58-S0.w+S0.la", DEP_TABLE_SIZE),
    S0tS0la("59-S0.t+S0.la", DEP_TABLE_SIZE),
    S1wS1ra("60-S1.w+S1.ra", DEP_TABLE_SIZE),
    S1tS1ra("61-S1.t+S1.ra", DEP_TABLE_SIZE),
    S1wS1la("62-S1.w+S1.la", DEP_TABLE_SIZE),
    S1tS1la("63-S1.t+S1.la", DEP_TABLE_SIZE),
    S0wS0lset("64-S0.w+S0.lset", DEP_TABLE_SIZE),
    S0tS0lset("65-S0.t+S0.lset", DEP_TABLE_SIZE),
    S0wS0rset("66-S0.w+S0.rset", DEP_TABLE_SIZE),
    S0tS0rset("67-S0.t+S0.rset", DEP_TABLE_SIZE),
    S1wS1lset("68-S1.w+S1.lset", DEP_TABLE_SIZE),
    S1tS1lset("69-S1.t+S1.lset", DEP_TABLE_SIZE),
    S1wS1rset("70-S1.w+S1.rset", DEP_TABLE_SIZE),
    S1tS1rset("71-S1.t+S1.rset", DEP_TABLE_SIZE),
    S0wDist("72-S0.w+Dist(S0,S1)", DEP_TABLE_SIZE),
    S0tDist("73-S0.t+Dist(S0,S1)", DEP_TABLE_SIZE),
    S1wDist("74-S0.w+Dist(S0,S1)", DEP_TABLE_SIZE),
    S1tDist("75-S0.t+Dist(S0,S1)", DEP_TABLE_SIZE),
    S0wS1wDist("76-S0.w+S1.w+Dist(S0,S1)", DEP_TABLE_SIZE),
    S0tS1tDist("77-S0.t+S1.t+Dist(S0,S1)", DEP_TABLE_SIZE),
    S0tS0l1dtS0l2dt("78-S0.t+S0.ld.t+S0.l2d.t", DEP_TABLE_SIZE),
    S0tS0r1dtS0r2dt("79-S0.t+S0.rd.t+S0.r2d.t", DEP_TABLE_SIZE),
    S1tS1l1dtS1l2dt("80-S1.t+S1.ld.t+S1.l2d.t", DEP_TABLE_SIZE),
    S1tS1r1dtS1r2dt("81-S1.t+S1.rd.t+S1.r2d.t", DEP_TABLE_SIZE),
    S0tS1tS1r2dt("82-S0.t+S1.t+S1.r2d.t", DEP_TABLE_SIZE),
    S0tS1tS0l2dt("83-S0.t+S1.t+S0.l2d.t", DEP_TABLE_SIZE),
    S0tS1tS1l2dt("84-S1.t+S1.t+S1.l2d.t", DEP_TABLE_SIZE),
    S0tS1tS0r2dt("85-S0.t+S1.t+S0.r2d.t", DEP_TABLE_SIZE),

    S0wS0l1dtS0l2dt("98-S0.w+S0.ld.t+S0.l2d.t", DEP_TABLE_SIZE),
    S0wS0r1dtS0r2dt("99-S0.w+S0.rd.t+S0.r2d.t", DEP_TABLE_SIZE),
    S1wS1l1dtS1l2dt("100-S1.w+S1.ld.t+S1.l2d.t", DEP_TABLE_SIZE),
    S1wS1r1dtS1r2dt("101-S1.w+S1.rd.t+S1.r2d.t", DEP_TABLE_SIZE),
    S0wS1tS1r1dt("102-S0.w+S1.t+S1.r1d.t", DEP_TABLE_SIZE),
    S0wS1tS0l1dt("103-S0.w+S1.t+S0.l1d.t", DEP_TABLE_SIZE),
    S0wS1tS1l1dt("104-S0.w+S1.t+S1.l1d.t", DEP_TABLE_SIZE),
    S0wS1tS0r1dt("105-S0.w+S1.t+S0.r1d.t", DEP_TABLE_SIZE),
    S1wS0tS1r1dt("106-S0.t+S1.w+S1.r1d.t", DEP_TABLE_SIZE),
    S1wS0tS0l1dt("107-S0.t+S1.w+S0.l1d.t", DEP_TABLE_SIZE),
    S1wS0tS1l1dt("108-S1.w+S0.t+S1.l1d.t", DEP_TABLE_SIZE),
    S1wS0tS0r1dt("109-S1.w+S0.t+S0.r1d.t", DEP_TABLE_SIZE),

    S2w("86-S2.w", DEP_TABLE_SIZE),
    S2t("87-S2.t", DEP_TABLE_SIZE),
    S2wS2t("88-S2.w+S2.t", DEP_TABLE_SIZE),
    S3w("89-S3.w", DEP_TABLE_SIZE),
    S3t("90-S3.t", DEP_TABLE_SIZE),
    S3wS3t("91-S3.w+S3.t", DEP_TABLE_SIZE),
    N2w("92-N2.w", DEP_TABLE_SIZE),
    N2t("93-N2.t", DEP_TABLE_SIZE),
    N2wN2t("94-N2.w+N2.t", DEP_TABLE_SIZE),
    N3w("95-N3.w", DEP_TABLE_SIZE),
    N3t("96-N3.t", DEP_TABLE_SIZE),
    N3wN3t("97-N3.w+N3.t", DEP_TABLE_SIZE),

    S0wtS1wt("110 - S0.wt + S1.wt", DEP_TABLE_SIZE),
    S0wtS1t("111 - S0.wt + S1.t", DEP_TABLE_SIZE),
    S0wtS1w("112 - S0.wt + S1.w", DEP_TABLE_SIZE),
    S1wtS0t("113 - S1.wt + S0.t", DEP_TABLE_SIZE),
    S1wtS0w("114 - S1.wt + S0.w", DEP_TABLE_SIZE),
    S0wtN0wt("115 - S0.wt + N0.wt", DEP_TABLE_SIZE),
    S0wtN0t("116 - S0.wt + N0.t", DEP_TABLE_SIZE),
    S0wtN0w("117 - S0.wt + NO.w", DEP_TABLE_SIZE),
    N0wtS0t("118 - N0.wt + S0.t", DEP_TABLE_SIZE),
    N0wtS0w("119 - N0.wt + S0.w", DEP_TABLE_SIZE),
    N0tN1t("120 - N0.t + N1.t", DEP_TABLE_SIZE),
    N0tN1tN2t("121 - N0.t + N1.t + N2.t", DEP_TABLE_SIZE),
    S0tN0tN1t("122 - S0.t + N0.t + N1.t", DEP_TABLE_SIZE),
    S0tS1tS2t("123 - S0.t + S1.t + S2.t", DEP_TABLE_SIZE),
    S0tS1tN0t("124 - S0.t + S1.t + N0.t", DEP_TABLE_SIZE),
    S0wS1tN0t("126 - S0.w + S1.t + N0.t", DEP_TABLE_SIZE),
    S0wS1wN0t("127 - S0.w + S1.w + N0.t", DEP_TABLE_SIZE),
    S1wS0tN0t("128 - S1.w + S0.t + N0.t", DEP_TABLE_SIZE),
    S0wN0tN1t("129 - S0.w + N0.t + N1.t", DEP_TABLE_SIZE),
    S0wN0wN1t("130 - S0.w + N0.w + N1.t", DEP_TABLE_SIZE),
    N0wS0tN1t("131 - N0.w + S0.t + N1.t", DEP_TABLE_SIZE),
    S0wS1tS2t("132 - S0.w + S1.t + S2.t", DEP_TABLE_SIZE),
    S0wS1wS2t("133 - S0.w + S1.w + S2.t", DEP_TABLE_SIZE),
    S1wS0tS2t("134 - S1.w + S0.t + S2.t", DEP_TABLE_SIZE),
    S0wS0raS0la("135 - S0.w + S0.ra + S0.la", DEP_TABLE_SIZE),
    S0tS0raS0la("136 - S0.t + S0.ra + S0.la", DEP_TABLE_SIZE),
    S1wS1raS1la("137 - S1.w + S1.ra + S1.la", DEP_TABLE_SIZE),
    S1tS1raS1la("138 - S1.t + S1.ra + S1.la", DEP_TABLE_SIZE),
    S0wS0lsetS0rset("139 - S0.w + S0.lset + S0.rset", DEP_TABLE_SIZE),
    S0tS0lsetS0rset("140 - S0.t + S0.lset + S0.rset", DEP_TABLE_SIZE),
    S1wS1lsetS1rset("141 - S1.w + S1.lset + S1.rset", DEP_TABLE_SIZE),
    S1tS1lsetS1rset("142 - S1.t + S1.lset + S1.rset", DEP_TABLE_SIZE),
    S0wS1tDist("143-S0.w+S1.t+Dist(S0,S1)", DEP_TABLE_SIZE),
    S1wS0tDist("144-S1.w+S0.t+Dist(S0,S1)", DEP_TABLE_SIZE),
    m_mapCFG("145 - CFG", DEP_TABLE_SIZE),
    S0C("146 - S0Constituent", DEP_TABLE_SIZE),
    S1C("147 - S1Constituent", DEP_TABLE_SIZE),
    S0CS1C("148 - S0C + S1C", DEP_TABLE_SIZE),
    S0wS1C("149 - S0w + S1C", DEP_TABLE_SIZE),
    S0tS1C("150 - S0t + S1C", DEP_TABLE_SIZE),
    S0wtS1C("151 - S0wt + S1C", DEP_TABLE_SIZE),
    S0wS1dS1C("152 - S0w + S1d + S1C", DEP_TABLE_SIZE),
    S1wS0C("153 - S1w + S0c", DEP_TABLE_SIZE),
    S1tS0C("154 - S1t +S0c", DEP_TABLE_SIZE),
    S1wtS0C("155 - S1wt + S0c", DEP_TABLE_SIZE),
    S1wS0dS0C("156 - S1w +S0d +S0c", DEP_TABLE_SIZE),
    S1S0PCFG("157 - S1wS0wPCFG",DEP_TABLE_SIZE),
    S1wS0tPCFG("158 - S1wS0tPCFG",DEP_TABLE_SIZE),
    S0wS1tPCFG("159 - S0tS1wPCFG",DEP_TABLE_SIZE)
   {
      loadScores();
  }

  virtual ~CWeight() {
//      iterate_templates(,.freePoolMemory(););
//      CPackedScore<SCORE_TYPE, action::kMax>::freePoolMemory();
  }

  // MEHTODS
  virtual void loadScores();
  virtual void saveScores();
  void computeAverageFeatureWeights(int round);
  SCORE_TYPE dotProduct(const CWeight &w);

};

};
};

#endif  //  end for DEPPARSER_ARC_STANDARD_WEIGHT_H

