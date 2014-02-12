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
  left(N0w)right; /*  6 - N0.w  */  \
  left(N0t)right; /*  7 - N0.t  */  \
  left(N0wN0t)right; /*  8 - N0.w + N0.t  */  \
  left(N1w)right; /*  9 - N1.w  */  \
  left(N1t)right; /*  10 - N1.t  */  \
  left(N1wN1t)right; /*  11 - N1.w + N1.t  */  \
  left(S0wS1w)right; /*  12 - S0.w + S1.w  */  \
  left(S0wS1t)right; /*  13 - S0.w + S1.t  */  \
  left(S0tS1w)right; /*  14 - S0.t + S1.w  */  \
  left(S0tS1t)right; /*  15 - S0.t + S1.t  */  \
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
typedef CPackedScoreMap<CTwoTaggedWords, SCORE_TYPE, action::kMax> \
  CTwoTaggedWordsMap;
typedef CPackedScoreMap<CTuple2<CWord, int>, SCORE_TYPE, action::kMax> \
  CWordIntMap;
typedef CPackedScoreMap<CTuple2<CTag, int>, SCORE_TYPE, action::kMax> \
  CTagIntMap;
typedef CPackedScoreMap<CTuple3<CTag, CTag, int>, SCORE_TYPE, action::kMax> \
  CTagTagIntMap;
typedef CPackedScoreMap<CTuple3<CWord, CWord, int>, SCORE_TYPE, action::kMax> \
  CWordWordIntMap;
typedef CPackedScoreMap<CTuple2< CWord, CSetOfTags<CDependencyLabel> >, \
  SCORE_TYPE, action::kMax> CWordSetOfLabelsMap;
typedef CPackedScoreMap<CTuple2< CTag, CSetOfTags<CDependencyLabel> >, \
  SCORE_TYPE, action::kMax> CTagSetOfLabelsMap;

typedef CPackedScoreMap<CLemma, SCORE_TYPE, action::kMax> CLemmaMap;
typedef CPackedScoreMap<CCoNLLCPOS, SCORE_TYPE, action::kMax> CCoNLLCPOSMap;
typedef CPackedScoreMap<CCoNLLFeats, SCORE_TYPE, action::kMax> CCoNLLFeatsMap;

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

  CWordMap        N0w;          /* 6 - N0.w */
  CTagMap         N0t;          /* 7 - N0.t */
  CTaggedWordMap  N0wN0t;       /* 8 - N0.w + N0.t */

  CWordMap        N1w;          /* 9 - N1.w */
  CTagMap         N1t;          /* 10 - N1.t */
  CTaggedWordMap  N1wN1t;       /* 11 - N1.w + N1.t */

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
    S0tS1tS0r2dt("85-S0.t+S1.t+S0.r2d.t", DEP_TABLE_SIZE) {
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

