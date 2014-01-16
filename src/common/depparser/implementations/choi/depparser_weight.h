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

typedef CPackedScoreMap<CWord, \
  SCORE_TYPE, action::kMax> CWordMap;
typedef CPackedScoreMap<CTag, \
  SCORE_TYPE, action::kMax> CTagMap;
typedef CPackedScoreMap<int, \
  SCORE_TYPE, action::kMax> CIntMap;
typedef CPackedScoreMap<CTagSet<CTag, 2>, \
  SCORE_TYPE, action::kMax> CTagSet2Map;
typedef CPackedScoreMap<CTagSet<CTag, 3>, \
  SCORE_TYPE, action::kMax> CTagSet3Map;
typedef CPackedScoreMap<CTagSet<CTag, 4>,  \
  SCORE_TYPE, action::kMax> CTagSet4Map;
typedef CPackedScoreMap<CTuple2<CWord, CTag>, \
  SCORE_TYPE, action::kMax> CWordTagMap;
typedef CPackedScoreMap<CTuple3<CWord, CTag, CTag>, \
  SCORE_TYPE, action::kMax> CWordTagTagMap;
typedef CPackedScoreMap<CTuple3<CWord, CWord, CTag>, \
  SCORE_TYPE, action::kMax> CWordWordTagMap;
typedef CPackedScoreMap<CTaggedWord<CTag, TAG_SEPARATOR>, \
  SCORE_TYPE, action::kMax> CTaggedWordMap;
typedef CPackedScoreMap<CTwoWords, \
  SCORE_TYPE, action::kMax> CTwoWordsMap;
typedef CPackedScoreMap<CTwoTaggedWords, \
  SCORE_TYPE, action::kMax> CTwoTaggedWordsMap;
typedef CPackedScoreMap<CTuple2<CWord, int>, \
  SCORE_TYPE, action::kMax> CWordIntMap;
typedef CPackedScoreMap<CTuple2<CTag, int>, \
  SCORE_TYPE, action::kMax> CTagIntMap;
typedef CPackedScoreMap<CTuple3<CTag, CTag, int>, \
  SCORE_TYPE, action::kMax> CTagTagIntMap;
typedef CPackedScoreMap<CTuple3<CWord, CWord, int>, \
  SCORE_TYPE, action::kMax> CWordWordIntMap;
typedef CPackedScoreMap<CTuple2<int, int>, \
  SCORE_TYPE, action::kMax> CIntIntMap;
typedef CPackedScoreMap<CTuple3<int, int, int>, \
  SCORE_TYPE, action::kMax> CIntIntIntMap;

typedef CPackedScoreMap<CTuple2<CCoNLLCPOS, CCoNLLCPOS>, \
  SCORE_TYPE, action::kMax> CCPosSet2Map;
typedef CPackedScoreMap<CTuple3<CCoNLLCPOS, CCoNLLCPOS, CCoNLLCPOS>, \
  SCORE_TYPE, action::kMax> CCPosSet3Map;

typedef CPackedScoreMap<CTuple2< CWord, CSetOfTags<CDependencyLabel> >,\
  SCORE_TYPE, action::kMax> CWordSetOfLabelsMap;
typedef CPackedScoreMap<CTuple2< CTag, CSetOfTags<CDependencyLabel> >, \
  SCORE_TYPE, action::kMax> CTagSetOfLabelsMap;

typedef CPackedScoreMap<CLemma,       SCORE_TYPE, action::kMax> CLemmaMap;
typedef CPackedScoreMap<CCoNLLCPOS,   SCORE_TYPE, action::kMax> CCoNLLCPOSMap;
typedef CPackedScoreMap<CCoNLLFeats,  SCORE_TYPE, action::kMax> CCoNLLFeatsMap;
typedef CPackedScoreMap<CTuple2<CCoNLLFeats, CCoNLLFeats>, \
  SCORE_TYPE, action::kMax> CCoNLLFeats2Map;

/*===============================================================
 *
 * CWeight - the definition of weights, in feature
 *
 *==============================================================*/
#define iterate_templates(left, right) do {\
  left(S0_w)right;            /* 0 */ \
  left(S0_p)right;            /* 1 */     left(S0_c)right;  /* 108 */ \
  left(N0_w)right;            /* 2 */ \
  left(N0_p)right;            /* 3 */     left(N0_c)right;  /* 109 */ \
  left(S0_w_S0_p)right;       /* 4 */ \
  left(N0_w_N0_p)right;       /* 5 */ \
  left(S0_p_N0_p)right;       /* 6 */ \
  left(S0_p_N0_w)right;       /* 7 */ \
  left(S0_w_N0_p)right;       /* 8 */ \
  left(S0_w_N0_w)right;       /* 9 */ \
  left(Sm1_w)right;           /* 10 */ \
  left(S0m1_w)right;          /* 11 */ \
  left(S0p1_w)right;          /* 12 */ \
  left(N0m2_w)right;          /* 13 */ \
  left(N0m1_w)right;          /* 14 */ \
  left(N0p1_w)right;          /* 15 */ \
  left(N0p2_w)right;          /* 16 */ \
  left(S0m2_p)right;          /* 17 */    left(S0m2_c)right;      /* 110 */ \
  left(S0m1_p)right;          /* 18 */    left(S0m1_c)right;      /* 111 */ \
  left(S0p1_p)right;          /* 19 */    left(S0p1_c)right;      /* 112 */ \
  left(S0p2_p)right;          /* 20 */    left(S0p2_c)right;      /* 113 */ \
  left(N0m1_p)right;          /* 21 */    left(N0m1_c)right;      /* 114 */ \
  left(N0p1_p)right;          /* 22 */    left(N0p1_c)right;      /* 115 */ \
  left(N0p1_p_S0_p)right;     /* 23 */    left(N0p1_c_S0_c)right; /* 116 */ \
  left(Sm1_p_S0_p)right;      /* 24 */    left(Sm1_c_S0_c)right;  /* 117 */ \
  left(Sm1_p_N0_p)right;      /* 25 */    left(Sm1_c_N0_c)right;  /* 118 */ \
  left(N0m1_p_S0_w)right;     /* 26 */ \
  left(N0p1_p_S0_w)right;     /* 27 */ \
  left(N0p1_p_N0_w)right;     /* 28 */ \
  left(N0p1_m_S0_p)right;     /* 29 */ \
  left(N0p1_m_N0_p)right;     /* 30 */ \
  left(S0p1_w_S0_w)right;     /* 31 */ \
  left(S0p1_w_N0_w)right;     /* 32 */ \
  left(S0m2_p_S0m1_p_S0_p)right;  /* 33 */ left(S0m2_c_S0m1_c_S0_c)right; /* 119 */ \
  left(S0m1_p_S0p1_p_S0_p)right;  /* 34 */ left(S0m1_c_S0p1_c_S0_c)right; /* 120 */ \
  left(S0p1_p_S0p2_p_S0_p)right;  /* 35 */ left(S0p1_c_S0p2_c_S0_c)right; /* 121 */ \
  left(N0m1_p_N0p1_p_N0_p)right;  /* 36 */ left(N0m1_c_N0p1_c_N0_c)right; /* 122 */ \
  left(N0p1_p_N0p2_p_N0_p)right;  /* 37 */ left(N0p1_c_N0p2_c_N0_c)right; /* 123 */ \
  left(Sm2_p_Sm1_p_S0_p)right;    /* 38 */ left(Sm2_c_Sm1_c_S0_c)right;   /* 124 */ \
  left(Sm2_p_S0_p_N0_p)right;     /* 39 */ left(Sm2_c_S0_c_N0_c)right;    /* 125 */ \
  left(S0m1_p_S0_p_N0_p)right;    /* 40 */ left(S0m1_c_S0_c_N0_c)right;   /* 126 */ \
  left(S0p1_p_S0_p_N0_p)right;    /* 41 */ left(S0p1_c_S0_c_N0_c)right;   /* 127 */ \
  left(N0m2_p_S0_p_N0_p)right;    /* 42 */ left(N0m2_c_S0_c_N0_c)right;   /* 128 */ \
  left(N0m1_p_S0_p_N0_p)right;    /* 43 */ left(N0m1_c_S0_c_N0_c)right;   /* 129 */ \
  left(N0p1_p_S0_p_N0_p)right;    /* 44 */ left(N0p1_c_S0_c_N0_c)right;   /* 130 */ \
  left(N0p2_p_S0_p_N0_p)right;    /* 45 */ left(N0p2_c_S0_c_N0_c)right;   /* 131 */ \
  left(N0p3_p_S0_p_N0_p)right;    /* 46 */ left(N0p3_c_S0_c_N0_c)right;   /* 132 */ \
  left(Sm3_p_S0_p_N0_p)right;     /* 47 */ left(Sm3_c_S0_c_N0_c)right;    /* 133 */ \
  left(S0h_w)right;               /* 48 */ \
  left(S0lm_w)right;              /* 49 */ \
  left(S0rm_w)right;              /* 50 */ \
  left(N0h_w)right;               /* 51 */ \
  left(N0lm_w)right;              /* 52 */ \
  left(S0h_p)right;               /* 53 */ left(S0h_c)right;              /* 134 */ \
  left(S0lm_p)right;              /* 54 */ left(S0lm_c)right;             /* 135 */ \
  left(S0rm_p)right;              /* 55 */ left(S0rm_c)right;             /* 136 */ \
  left(N0h_p)right;               /* 56 */ left(N0h_c)right;              /* 137 */ \
  left(N0lm_p)right;              /* 57 */ left(N0lm_c)right;             /* 138 */ \
  left(S0h_d)right;               /* 58 */ \
  left(S0lm_d)right;              /* 59 */ \
  left(S0rm_d)right;              /* 60 */ \
  left(N0h_d)right;               /* 61 */ \
  left(N0lm_d)right;              /* 62 */ \
  left(S0lm_p_S0_p_N0_p)right;    /* 63 */ left(S0lm_c_S0_c_N0_c)right;    /* 139 */ \
  left(S0rm_p_S0_p_N0_p)right;    /* 64 */ left(S0rm_c_S0_c_N0_c)right;    /* 140 */ \
  left(S0h_p_S0_p_N0_p)right;     /* 65 */ left(S0h_c_S0_c_N0_c)right;     /* 141 */ \
  left(N0lm_p_S0_p_N0_p)right;    /* 66 */ left(N0lm_c_S0_c_N0_c)right;    /* 142 */ \
  left(N0h_p_S0_p_N0_p)right;     /* 67 */ left(N0h_c_S0_c_N0_c)right;     /* 143 */ \
  left(S0h2_w)right;              /* 68 */ \
  left(S0lm2_w)right;             /* 69 */ \
  left(S0rm2_w)right;             /* 70 */ \
  left(N0h2_w)right;              /* 71 */ \
  left(N0lm2_w)right;             /* 72 */ \
  left(S0h2_p)right;              /* 73 */ left(S0h2_c)right;             /* 144 */ \
  left(S0lm2_p)right;             /* 74 */ left(S0lm2_c)right;            /* 145 */ \
  left(S0rm2_p)right;             /* 75 */ left(S0rm2_c)right;            /* 146 */ \
  left(N0h2_p)right;              /* 76 */ left(N0h2_c)right;             /* 147 */ \
  left(N0lm2_p)right;             /* 77 */ left(N0lm2_c)right;            /* 148 */ \
  left(S0h2_d)right;              /* 78 */ \
  left(S0lm2_d)right;             /* 79 */ \
  left(S0rm2_d)right;             /* 80 */ \
  left(N0h2_d)right;              /* 81 */ \
  left(N0lm2_d)right;             /* 82 */ \
  left(S0lm2_p_S0lm_p_S0_p)right; /* 83 */ left(S0lm2_c_S0lm_c_S0_c)right;/* 149 */ \
  left(S0rm2_p_S0rm_p_S0_p)right; /* 84 */ left(S0rm2_c_S0rm_c_S0_c)right;/* 150 */ \
  left(N0lm2_p_N0lm_p_N0_p)right; /* 85 */ left(N0lm2_c_N0lm_c_N0_c)right;/* 151 */ \
  left(S0h2_p_S0h_p_S0_p)right;   /* 86 */ left(S0h2_c_S0h_c_S0_c)right;  /* 152 */ \
  left(N0h2_p_N0h_p_N0_p)right;   /* 87 */ left(N0h2_c_N0h_c_N0_c)right;  /* 153 */ \
  left(S0lm2_p_S0_p_N0_p)right;   /* 88 */ left(S0lm2_c_S0_c_N0_c)right;  /* 154 */ \
  left(S0rm2_p_S0_p_N0_p)right;   /* 89 */ left(S0rm2_c_S0_c_N0_c)right;  /* 155 */ \
  left(S0_w_S0_ra)right;          /* 90 */ \
  left(S0_w_S0_la)right;          /* 91 */ \
  left(N0_w_N0_la)right;          /* 92 */ \
  left(S0_p_S0_ra)right;          /* 93 */ \
  left(S0_p_S0_la)right;          /* 94 */ \
  left(N0_p_N0_la)right;          /* 95 */ \
  left(S0_w_S0_lset)right;        /* 96 */ \
  left(S0_w_S0_rset)right;        /* 97 */ \
  left(N0_w_N0_lset)right;        /* 98 */ \
  left(S0_p_S0_lset)right;        /* 99 */ \
  left(S0_p_S0_rset)right;        /* 100 */ \
  left(N0_p_N0_lset)right;        /* 101 */ \
  left(S0conll_l)right;           /* 102 */ \
  left(S0conll_c)right;           /* 103 */ \
  left(S0conll_f)right;           /* 104 */ \
  left(N0conll_l)right;           /* 105 */ \
  left(N0conll_c)right;           /* 106 */ \
  left(N0conll_f)right;           /* 107 */ \
  left(S0conll_f_N0conll_f)right; /* 107.5 */ \
  left(S0lm_d_S0_d)right;         /* 200 */ \
  left(S0lm2_d_S0_d)right;        /* 201 */ \
  left(S0rm_d_S0_d)right;         /* 202 */ \
  left(S0rm2_d_S0_d)right;        /* 203 */ \
  left(S0h_d_S0_d)right;          /* 204 */ \
  left(N0lm_d_N0_d)right;         /* 205 */ \
  left(N0lm2_d_N0_d)right;        /* 206 */ \
  left(N0h_d_N0_d)right;          /* 207 */ \
} while (0);

class CWeight : public CWeightBase {
public:
  // basic features
  CWordMap        S0_w;             //  0 - S0.w
  CTagMap         S0_p;             //  1 - S0.p
  CWordMap        N0_w;             //  2 - N0.w
  CTagMap         N0_p;             //  3 - N0.p
  CTaggedWordMap  S0_w_S0_p;        //  4 - S0.w + S0.p
  CTaggedWordMap  N0_w_N0_p;        //  5 - N0.w + N0.p
  CTagSet2Map     S0_p_N0_p;        //  6 - S0.p + N0.p
  CTaggedWordMap  S0_p_N0_w;        //  7 - S0.p + N0.w
  CTaggedWordMap  S0_w_N0_p;        //  8 - S0.w + N0.p
  CTwoWordsMap    S0_w_N0_w;        //  9 - S0.w + N0.w
  // unigram features
  CWordMap        Sm1_w;            //  10 - S-1.w
  CWordMap        S0m1_w;           //  11 - S0-1.w
  CWordMap        S0p1_w;           //  12 - S0+1.w
  CWordMap        N0m2_w;           //  13 - N0-2.w
  CWordMap        N0m1_w;           //  14 - N0-1.w
  CWordMap        N0p1_w;           //  15 - N0+1.w
  CWordMap        N0p2_w;           //  16 - N0+2.w
  //
  CTagMap         S0m2_p;           //  17 - S0-2.p
  CTagMap         S0m1_p;           //  18 - S0-1.p
  CTagMap         S0p1_p;           //  19 - S0+1.p
  CTagMap         S0p2_p;           //  20 - S0+2.p
  CTagMap         N0m1_p;           //  21 - N0-1.p
  CTagMap         N0p1_p;           //  22 - N0+1.p
  // bigram features
  CTagSet2Map     N0p1_p_S0_p;      //  23 - N0+1.p + S0.p
  CTagSet2Map     Sm1_p_S0_p;       //  24 - S-1.p + S0.p
  CTagSet2Map     Sm1_p_N0_p;       //  25 - S-1.p + N0.p
  CTaggedWordMap  N0m1_p_S0_w;      //  26 - N0-1.p + S0.w
  CTaggedWordMap  N0p1_p_S0_w;      //  27 - N0+1.p + S0.w
  CTaggedWordMap  N0p1_p_N0_w;      //  28 - N0+1.p + N0.w
  //
  CTaggedWordMap  N0p1_m_S0_p;      //  29 - N0+1.w + S0.p
  CTaggedWordMap  N0p1_m_N0_p;      //  30 - N0+1.w + N0.p
  //
  CTwoWordsMap    S0p1_w_S0_w;      //  31 - S0+1.w + S0.w
  CTwoWordsMap    S0p1_w_N0_w;      //  32 - S0+1.w + N0.w

  // trigram featrues;
  CTagSet3Map     S0m2_p_S0m1_p_S0_p; // 33 - S0-2.p + S0-1.p + S0.p
  CTagSet3Map     S0m1_p_S0p1_p_S0_p; // 34 - S0-1.p + S0+1.p + S0.p
  CTagSet3Map     S0p1_p_S0p2_p_S0_p; // 35 - S0+1.p + S0+2.p + S0.p
  CTagSet3Map     N0m1_p_N0p1_p_N0_p; // 36 - N0-1.p + N0+1.p + N0.p
  CTagSet3Map     N0p1_p_N0p2_p_N0_p; // 37 - N0+1.p + N0+2.p + N0.p
  CTagSet3Map     Sm2_p_Sm1_p_S0_p;   // 38 - S0-2.p + S0-1.p + S0.p
  CTagSet3Map     Sm2_p_S0_p_N0_p;    // 39 - S0-2.p + S0.p + N0.p
  CTagSet3Map     S0m1_p_S0_p_N0_p;   // 40 - S0-1.p + S0.p + N0.p
  CTagSet3Map     S0p1_p_S0_p_N0_p;   // 41 - S0+1.p + S0.p + N0.p
  CTagSet3Map     N0m2_p_S0_p_N0_p;   // 42 - N0-2.p + S0.p + N0.p
  CTagSet3Map     N0m1_p_S0_p_N0_p;   // 43 - N0-1.p + S0.p + N0.p
  CTagSet3Map     N0p1_p_S0_p_N0_p;   // 44 - N0+1.p + S0.p + N0.p
  CTagSet3Map     N0p2_p_S0_p_N0_p;   // 45 - N0+2.p + S0.p + N0.p
  CTagSet3Map     N0p3_p_S0_p_N0_p;   // 46 - N0+3.p + S0.p + N0.p
  CTagSet3Map     Sm3_p_S0_p_N0_p;    // 47 - S-3.p + S0.p + N0.p
  // tree structure features
  CWordMap        S0h_w;              // 48 - S0>h.w
  CWordMap        S0lm_w;             // 49 - S0>lm.w
  CWordMap        S0rm_w;             // 50 - S0>rm.w
  CWordMap        N0h_w;              // 51 - N0>h.w
  CWordMap        N0lm_w;             // 52 - N0>lm.w
  CTagMap         S0h_p;              // 53 - S0>h.p
  CTagMap         S0lm_p;             // 54 - S0>lm.p
  CTagMap         S0rm_p;             // 55 - S0>rm.p
  CTagMap         N0h_p;              // 56 - N0>h.p
  CTagMap         N0lm_p;             // 57 - N0>lm.p
  CIntMap         S0h_d;              // 58 - S0>h.d
  CIntMap         S0lm_d;             // 59 - S0>lm.d
  CIntMap         S0rm_d;             // 60 - S0>rm.d
  CIntMap         N0h_d;              // 61 - N0>h.d
  CIntMap         N0lm_d;             // 62 - N0>lm.d

  CIntIntMap      S0lm_d_S0_d;        // 200 - S0>lm.d + S0.d
  CIntIntMap      S0lm2_d_S0_d;       // 201 - S0>lm2.d + S0.d
  CIntIntMap      S0rm_d_S0_d;        // 202 - S0>rm.d + S0.d
  CIntIntMap      S0rm2_d_S0_d;       // 203 - S0>rm2.d + S0.d
  CIntIntMap      S0h_d_S0_d;         // 204 - S0>h.d + S0.d
  CIntIntMap      N0lm_d_N0_d;        // 205 - N0>lm.d + N0.d
  CIntIntMap      N0lm2_d_N0_d;       // 206 - N0>lm2.d + N0.d
  CIntIntMap      N0h_d_N0_d;         // 207 - N0>h.d + N0.d

  CTagSet3Map     S0lm_p_S0_p_N0_p;   // 63 - S0>lm.p + S0.p + N0.p
  CTagSet3Map     S0rm_p_S0_p_N0_p;   // 64 - S0>rm.p + S0.p + N0.p
  CTagSet3Map     S0h_p_S0_p_N0_p;    // 65 - S0>h.p + S0.p + N0.p
  CTagSet3Map     N0lm_p_S0_p_N0_p;   // 66 - N0>lm.p + S0.p + N0.p
  CTagSet3Map     N0h_p_S0_p_N0_p;    // 67 - N0>h.p + S0.p + N0.p
  // 3rd-order features
  CWordMap        S0h2_w;             // 68 - S0>h>h.w
  CWordMap        S0lm2_w;            // 69 - S0>lm2.w
  CWordMap        S0rm2_w;            // 70 - S0>rm2.w
  CWordMap        N0h2_w;             // 71 - N0>h>h.w
  CWordMap        N0lm2_w;            // 72 - N0>lm2.w
  CTagMap         S0h2_p;             // 73 - S0>h>h.p
  CTagMap         S0lm2_p;            // 74 - S0>lm2.p
  CTagMap         S0rm2_p;            // 75 - S0>rm2.p
  CTagMap         N0h2_p;             // 76 - N0>h>h.p
  CTagMap         N0lm2_p;            // 77 - N0>lm2.p
  CIntMap         S0h2_d;             // 78 - S0>h>h.d
  CIntMap         S0lm2_d;            // 79 - S0>lm2.d
  CIntMap         S0rm2_d;            // 80 - S0>rm2.d
  CIntMap         N0h2_d;             // 81 - N0>h>h.d
  CIntMap         N0lm2_d;            // 82 - N0>lm2.d

  /*CIntIntIntMap     S0lm2_d_S0lm_d_S0_d;  //
  CIntIntIntMap     S0rm2_d_S0rm_d_S0_d;  //
  CIntIntIntMap     N0lm2_d_N0lm_d_S0_d;  //*/

  CTagSet3Map     S0lm2_p_S0lm_p_S0_p;  // 83 - S0>lm2.p + S0>lm.p + S0.p
  CTagSet3Map     S0rm2_p_S0rm_p_S0_p;  // 84 - S0>rm2.p + S0>rm.p + S0.p
  CTagSet3Map     N0lm2_p_N0lm_p_N0_p;  // 85 - N0>lm2.p + N0>lm.p + N0.p
  CTagSet3Map     S0h2_p_S0h_p_S0_p;    // 86 - S0>h2.p + S0>h.p + S0.p
  CTagSet3Map     N0h2_p_N0h_p_N0_p;    // 87 - N0>h2.p + N0>h.p + N0.p
  CTagSet3Map     S0lm2_p_S0_p_N0_p;    // 88 - S0>lm2.p + S0.p + N0.p
  CTagSet3Map     S0rm2_p_S0_p_N0_p;    // 89 - S0>rm2.p + S0.p + N0.p

  CWordIntMap     S0_w_S0_ra;           // 90 - S0.w + S0.ra
  CWordIntMap     S0_w_S0_la;           // 91 - S0.w + S0.la
  CWordIntMap     N0_w_N0_la;           // 92 - N0.w + N0.la
  CTagIntMap      S0_p_S0_ra;           // 93 - S0.p + S0.ra
  CTagIntMap      S0_p_S0_la;           // 94 - S0.p + S0.la
  CTagIntMap      N0_p_N0_la;           // 95 - N0.p + N0.la

  CWordSetOfLabelsMap S0_w_S0_lset;     // 96 - S0.w + S0.lset
  CWordSetOfLabelsMap S0_w_S0_rset;     // 97 - S0.w + S0.rset
  CWordSetOfLabelsMap N0_w_N0_lset;     // 98 - N0.w + N0.lset
  CTagSetOfLabelsMap  S0_p_S0_lset;     // 99 - S0.p + S0.lset
  CTagSetOfLabelsMap  S0_p_S0_rset;     // 100 - S0.p + S0.rset
  CTagSetOfLabelsMap  N0_p_N0_lset;     // 101 - N0.p + N0.lset

  CLemmaMap           S0conll_l;        // 102 - S0|conll.l
  CCoNLLCPOSMap       S0conll_c;        // 103 - S0|conll.c
  CCoNLLFeatsMap      S0conll_f;        // 104 - S0|conll.f
  CLemmaMap           N0conll_l;        // 105 - N0|conll.l
  CCoNLLCPOSMap       N0conll_c;        // 106 - N0|conll.c
  CCoNLLFeatsMap      N0conll_f;        // 107 - N0|conll.f
  CCoNLLFeats2Map     S0conll_f_N0conll_f;//  107.5 - S0|conll.f - N0|conll.f

  CCoNLLCPOSMap       S0_c;               //  108 - S0.c
  CCoNLLCPOSMap       N0_c;               //  109 - N0.c
  CCoNLLCPOSMap       S0m2_c;             //  110 - S0-2.c
  CCoNLLCPOSMap       S0m1_c;             //  111 - S0-1.c
  CCoNLLCPOSMap       S0p1_c;             //  112 - S0+1.c
  CCoNLLCPOSMap       S0p2_c;             //  113 - S0+2.c
  CCoNLLCPOSMap       N0m1_c;             //  114 - N0-1.c
  CCoNLLCPOSMap       N0p1_c;             //  115 - N0+1.c
  CCPosSet2Map        N0p1_c_S0_c;        //  116 - N0-1.c + S0.c
  CCPosSet2Map        Sm1_c_S0_c;         //  117 - S-1.c + S0.c
  CCPosSet2Map        Sm1_c_N0_c;         //  118 - S-1.c + N0.c
  CCPosSet3Map        S0m2_c_S0m1_c_S0_c; //  119 - S0-2.c + S0-1.c + S0.c
  CCPosSet3Map        S0m1_c_S0p1_c_S0_c; //  120 - S0-1.c + S0+1.c + S0.c
  CCPosSet3Map        S0p1_c_S0p2_c_S0_c; //  121 - S0+1.c + S0+2.c + S0.c
  CCPosSet3Map        N0m1_c_N0p1_c_N0_c; //  122 - N0-1.c + N0+1.c + N0.c
  CCPosSet3Map        N0p1_c_N0p2_c_N0_c; //  123 - N0+1.c + N0+2.c + N0.c
  CCPosSet3Map        Sm2_c_Sm1_c_S0_c;   //  124 - S-2.c + S-1.c + S0.c
  CCPosSet3Map        Sm2_c_S0_c_N0_c;    //  125 - S-2.c + S0.c + N0.c
  CCPosSet3Map        S0m1_c_S0_c_N0_c;   //  126 - S0-1.c + S0.c + N0.c
  CCPosSet3Map        S0p1_c_S0_c_N0_c;   //  127 - S0+1.c + S0.c + N0.c
  CCPosSet3Map        N0m2_c_S0_c_N0_c;   //  128 - N0-2.c + S0.c + N0.c
  CCPosSet3Map        N0m1_c_S0_c_N0_c;   //  129 - N0-1.c + S0.c + N0.c
  CCPosSet3Map        N0p1_c_S0_c_N0_c;   //  130 - N0+1.c + S0.c + N0.c
  CCPosSet3Map        N0p2_c_S0_c_N0_c;   //  131 - N0+2.c + S0.c + N0.c
  CCPosSet3Map        N0p3_c_S0_c_N0_c;   //  132 - N0+3.c + S0.c + N0.c
  CCPosSet3Map        Sm3_c_S0_c_N0_c;    //  133 - S-3.c + S0.c + N0.c

  CCoNLLCPOSMap       S0h_c;              //  134 - S0>h.c
  CCoNLLCPOSMap       S0lm_c;             //  135 - S0>lm.c
  CCoNLLCPOSMap       S0rm_c;             //  136 - S0>rm.c
  CCoNLLCPOSMap       N0h_c;              //  137 - N0>h.c
  CCoNLLCPOSMap       N0lm_c;             //  138 - N0>lm.c

  CCPosSet3Map        S0lm_c_S0_c_N0_c;   // 139 - S0>lm.p + S0.p + N0.p
  CCPosSet3Map        S0rm_c_S0_c_N0_c;   // 140 - S0>rm.p + S0.p + N0.p
  CCPosSet3Map        S0h_c_S0_c_N0_c;    // 141 - S0>h.p + S0.p + N0.p
  CCPosSet3Map        N0lm_c_S0_c_N0_c;   // 142 - N0>lm.p + S0.p + N0.p
  CCPosSet3Map        N0h_c_S0_c_N0_c;    // 143 - N0>h.p + S0.p + N0.p

  CCoNLLCPOSMap       S0h2_c;             // 144 - S0>h2.c
  CCoNLLCPOSMap       S0lm2_c;            // 145 - S0>lm2.c
  CCoNLLCPOSMap       S0rm2_c;            // 146 - S0>rm2.c
  CCoNLLCPOSMap       N0h2_c;             // 147 - N0>h2.c
  CCoNLLCPOSMap       N0lm2_c;            // 148 - N0>lm2.c

  CCPosSet3Map        S0lm2_c_S0lm_c_S0_c;  //  149 - S0>lm2.c + S0>lm.c + S0.c
  CCPosSet3Map        S0rm2_c_S0rm_c_S0_c;  //  150 - S0>rm2.c + S0>rm.c + S0.c
  CCPosSet3Map        N0lm2_c_N0lm_c_N0_c;  //  151 - N0>lm2.c + N0>lm.c + S0.c
  CCPosSet3Map        S0h2_c_S0h_c_S0_c;    //  152 - S0>h2.c + S0>h.c + S0.c
  CCPosSet3Map        N0h2_c_N0h_c_N0_c;    //  153 - N0>h2.c + N0>h.c + S0.c
  CCPosSet3Map        S0lm2_c_S0_c_N0_c;    //  154 - S0>lm2.c + S0.c + N0.c
  CCPosSet3Map        S0rm2_c_S0_c_N0_c;    //  155 - S0>rm2.c + S0.c + N0.c

  CWeight(const std::string &sPath, bool bTrain) :
    CWeightBase(sPath, bTrain) ,
    // initialization for the
    S0_w("0_S0.w",            DEP_TABLE_SIZE, false),
    S0_p("1_S0.p",            DEP_TABLE_SIZE, false),
    N0_w("2_N0.w",            DEP_TABLE_SIZE, false),
    N0_p("3_N0.p",            DEP_TABLE_SIZE, false),
    S0_w_S0_p("4_S0.w_S0.p",  DEP_TABLE_SIZE, false),
    N0_w_N0_p("5_N0.w_N0.p",  DEP_TABLE_SIZE, false),
    S0_p_N0_p("6_S0.p_N0.p",  DEP_TABLE_SIZE, false),
    S0_p_N0_w("7_S0.p_N0.w",  DEP_TABLE_SIZE, false),
    S0_w_N0_p("8_S0.w_N0.p",  DEP_TABLE_SIZE, false),
    S0_w_N0_w("9_S0.w_N0.w",  DEP_TABLE_SIZE, false),
    // unigram features
    Sm1_w("10_S-1.w",         DEP_TABLE_SIZE, false),
    S0m1_w("11_S0-1.w",       DEP_TABLE_SIZE, false),
    S0p1_w("12_S0+1.w",       DEP_TABLE_SIZE, false),
    N0m2_w("13_N0-2.w",       DEP_TABLE_SIZE, false),
    N0m1_w("14_N0-1.w",       DEP_TABLE_SIZE, false),
    N0p1_w("15_N0+1.w",       DEP_TABLE_SIZE, false),
    N0p2_w("16_N0+2.w",       DEP_TABLE_SIZE, false),
    S0m2_p("17_S0-2.p",       DEP_TABLE_SIZE, false),
    S0m1_p("18_S0-1.p",       DEP_TABLE_SIZE, false),
    S0p1_p("19_S0+1.p",       DEP_TABLE_SIZE, false),
    S0p2_p("20_S0+2.p",       DEP_TABLE_SIZE, false),
    N0m1_p("21_N0-1.p",       DEP_TABLE_SIZE, false),
    N0p1_p("22_N0+1.p",       DEP_TABLE_SIZE, false),
    // bigram features
    N0p1_p_S0_p("23_N0+1.p_S0.p", DEP_TABLE_SIZE, false),
    Sm1_p_S0_p ("24_S-1.p_S0.p",  DEP_TABLE_SIZE, false),
    Sm1_p_N0_p ("25_S-1.p_N0.p",  DEP_TABLE_SIZE, false),
    N0m1_p_S0_w("26_N0-1.p_S0.w", DEP_TABLE_SIZE, false),
    N0p1_p_S0_w("27_N0+1.p_S0.w", DEP_TABLE_SIZE, false),
    N0p1_p_N0_w("28_N0+1.p_N0.w", DEP_TABLE_SIZE, false),
    N0p1_m_S0_p("29_N0+1.m_S0.p", DEP_TABLE_SIZE, false),
    N0p1_m_N0_p("30_N0+1.m_N0.p", DEP_TABLE_SIZE, false),
    S0p1_w_S0_w("31_S0+1.w_S0.w", DEP_TABLE_SIZE, false),
    S0p1_w_N0_w("32_S0+1.w_N0.w", DEP_TABLE_SIZE, false),
    // trigram features
    S0m2_p_S0m1_p_S0_p("33_S0-2.p_S0-1.p_S0.p", DEP_TABLE_SIZE, false),
    S0m1_p_S0p1_p_S0_p("34_S0-1.p_S0+1.p_S0.p", DEP_TABLE_SIZE, false),
    S0p1_p_S0p2_p_S0_p("35_S0+1.p_S0+2.p_S0.p", DEP_TABLE_SIZE, false),
    N0m1_p_N0p1_p_N0_p("36_N0-1.p_N0+1.p_N0.p", DEP_TABLE_SIZE, false),
    N0p1_p_N0p2_p_N0_p("37_N0+1.p_N0+2.p_N0.p", DEP_TABLE_SIZE, false),
    Sm2_p_Sm1_p_S0_p  ("38_S-2.p_S-1.p_S0.p",   DEP_TABLE_SIZE, false),
    Sm2_p_S0_p_N0_p   ("39_S-2.p_S0.p_N0.p",    DEP_TABLE_SIZE, false),
    S0m1_p_S0_p_N0_p  ("40_S0-1.p_S0.p_N0.p",   DEP_TABLE_SIZE, false),
    S0p1_p_S0_p_N0_p  ("41_S0+1.p_S0.p_N0.p",   DEP_TABLE_SIZE, false),
    N0m2_p_S0_p_N0_p  ("42_N0-2.p_S0.p_N0.p",   DEP_TABLE_SIZE, false),
    N0m1_p_S0_p_N0_p  ("43_N0-1.p_S0.p_N0.p",   DEP_TABLE_SIZE, false),
    N0p1_p_S0_p_N0_p  ("44_N0+1.p_S0.p_N0.p",   DEP_TABLE_SIZE, false),
    N0p2_p_S0_p_N0_p  ("45_N0+2.p_S0.p_N0.p",   DEP_TABLE_SIZE, false),
    N0p3_p_S0_p_N0_p  ("46_N0+3.p_S0.p_N0.p",   DEP_TABLE_SIZE, false),
    Sm3_p_S0_p_N0_p   ("47_S-3.p_S0.p_N0.p",    DEP_TABLE_SIZE, false),
    // 2nd-order features
    S0h_w ("48_S0>h.w",  DEP_TABLE_SIZE, false),
    S0lm_w("49_S0>lm.w", DEP_TABLE_SIZE, false),
    S0rm_w("50_S0>rm.w", DEP_TABLE_SIZE, false),
    N0h_w ("51_N0>h.w",  DEP_TABLE_SIZE, false),
    N0lm_w("52_N0>lm.w", DEP_TABLE_SIZE, false),
    S0h_p ("53_S0>h.p",  DEP_TABLE_SIZE, false),
    S0lm_p("54_S0>lm.p", DEP_TABLE_SIZE, false),
    S0rm_p("55_S0>rm.p", DEP_TABLE_SIZE, false),
    N0h_p ("56_N0>h.p",  DEP_TABLE_SIZE, false),
    N0lm_p("57_N0>lm.p", DEP_TABLE_SIZE, false),
    S0h_d ("58_S0>h.d",  DEP_TABLE_SIZE, false),
    S0lm_d("59_S0>lm.d", DEP_TABLE_SIZE, false),
    S0rm_d("60_S0>rm.d", DEP_TABLE_SIZE, false),
    N0h_d ("61_N0>h.d",  DEP_TABLE_SIZE, false),
    N0lm_d("62_N0>lm.d", DEP_TABLE_SIZE, false),
    // 
    S0lm_p_S0_p_N0_p("63_S0>lm.p_S0.p_N0.p", DEP_TABLE_SIZE, false),
    S0rm_p_S0_p_N0_p("64_S0>rm.p_S0.p_N0.p", DEP_TABLE_SIZE, false),
    S0h_p_S0_p_N0_p ("65_S0>h.p_S0.p_N0.p",  DEP_TABLE_SIZE, false),
    N0lm_p_S0_p_N0_p("66_N0>lm.p_S0.p_N0.p", DEP_TABLE_SIZE, false),
    N0h_p_S0_p_N0_p ("67_N0>h.p_S0.p_N0.p",  DEP_TABLE_SIZE, false),
    // 3rd-order 
    S0h2_w ("68_S0>h>h.w", DEP_TABLE_SIZE, false),
    S0lm2_w("69_S0>lm2.w", DEP_TABLE_SIZE, false),
    S0rm2_w("70_S0>rm2.w", DEP_TABLE_SIZE, false),
    N0h2_w ("71_N0>h>h.w", DEP_TABLE_SIZE, false),
    N0lm2_w("72_N0>lm2.w", DEP_TABLE_SIZE, false),
    S0h2_p ("73_S0>h>h.p", DEP_TABLE_SIZE, false),
    S0lm2_p("74_S0>lm2.p", DEP_TABLE_SIZE, false),
    S0rm2_p("75_S0>rm2.p", DEP_TABLE_SIZE, false),
    N0h2_p ("76_N0>h>h.p", DEP_TABLE_SIZE, false),
    N0lm2_p("77_N0>lm2.p", DEP_TABLE_SIZE, false),
    S0h2_d ("78_S0>h>h.d", DEP_TABLE_SIZE, false),
    S0lm2_d("79_S0>lm2.d", DEP_TABLE_SIZE, false),
    S0rm2_d("80_S0>rm2.d", DEP_TABLE_SIZE, false),
    N0h2_d ("81_N0>h>h.d", DEP_TABLE_SIZE, false),
    N0lm2_d("82_N0>lm2.d", DEP_TABLE_SIZE, false),
    // 
    S0lm2_p_S0lm_p_S0_p("83_S0>lm2.p_S0>lm.p_S0.p", DEP_TABLE_SIZE, false),
    S0rm2_p_S0rm_p_S0_p("84_S0>rm2.p_S0>rm.p_S0.p", DEP_TABLE_SIZE, false),
    N0lm2_p_N0lm_p_N0_p("85_N0>lm2.p_N0>lm.p_N0.p", DEP_TABLE_SIZE, false),
    S0h2_p_S0h_p_S0_p  ("86_S0>h2.p_S0>h.p_S0.p",   DEP_TABLE_SIZE, false),
    N0h2_p_N0h_p_N0_p  ("87_N0>h2.p_N0>h.p_N0.p",   DEP_TABLE_SIZE, false),
    S0lm2_p_S0_p_N0_p  ("88_S0>lm2.p_S0.p_N0.p",    DEP_TABLE_SIZE, false),
    S0rm2_p_S0_p_N0_p  ("89_S0>rm2.p_S0.p_N0.p",    DEP_TABLE_SIZE, false),
    // 
    S0_w_S0_la("90_S0.w_S0.la", DEP_TABLE_SIZE, false),
    S0_w_S0_ra("91_S0.w_S0.ra", DEP_TABLE_SIZE, false),
    N0_w_N0_la("92_N0.w_S0.la", DEP_TABLE_SIZE, false),
    S0_p_S0_ra("93_S0.p_S0.ra", DEP_TABLE_SIZE, false),
    S0_p_S0_la("94_S0.p_S0.la", DEP_TABLE_SIZE, false),
    N0_p_N0_la("95_N0.p_N0.la", DEP_TABLE_SIZE, false),
    //
    S0_w_S0_lset("96_S0.w_S0.lset", DEP_TABLE_SIZE, false),
    S0_w_S0_rset("97_S0.w_S0.rset", DEP_TABLE_SIZE, false),
    N0_w_N0_lset("98_N0.w_N0.lset", DEP_TABLE_SIZE, false),
    S0_p_S0_lset("99_S0.p_S0.lset", DEP_TABLE_SIZE, false),
    S0_p_S0_rset("100_S0.p_S0.rset",DEP_TABLE_SIZE, false),
    N0_p_N0_lset("101_N0.p_N0.lset",DEP_TABLE_SIZE, false),
    S0conll_l("102_S0|conll.l", DEP_TABLE_SIZE, false),
    S0conll_c("103_S0|conll.c", DEP_TABLE_SIZE, false),
    S0conll_f("104_S0|conll.f", DEP_TABLE_SIZE, false),
    N0conll_l("105_N0|conll.l", DEP_TABLE_SIZE, false),
    N0conll_c("106_N0|conll.c", DEP_TABLE_SIZE, false),
    N0conll_f("107_N0|conll.f", DEP_TABLE_SIZE, false),
    S0conll_f_N0conll_f("107.5_S0|conll.f_N0|conll.f", DEP_TABLE_SIZE, false),
    S0_c("108_S0.c",            DEP_TABLE_SIZE, false),
    N0_c("109_N0.c",            DEP_TABLE_SIZE, false),
    S0m2_c("110_S0-2.c",        DEP_TABLE_SIZE, false),
    S0m1_c("111_S0-1.c",        DEP_TABLE_SIZE, false),
    S0p1_c("112_S0+1.c",        DEP_TABLE_SIZE, false),
    S0p2_c("113_S0+2.c",        DEP_TABLE_SIZE, false),
    N0m1_c("114_N0-1.c",        DEP_TABLE_SIZE, false),
    N0p1_c("115_N0+1.c",        DEP_TABLE_SIZE, false),
    N0p1_c_S0_c("116_N0-1.c_S0.c", DEP_TABLE_SIZE, false),
    Sm1_c_S0_c("117_S-1.c_S0.c", DEP_TABLE_SIZE, false),
    Sm1_c_N0_c("118_S-1.c_N0.c", DEP_TABLE_SIZE, false),
    S0m2_c_S0m1_c_S0_c("119_S0-2.c_S0-1.c_S0.c", DEP_TABLE_SIZE, false),
    S0m1_c_S0p1_c_S0_c("120_S0-1.c_S0+1.c_S0.c", DEP_TABLE_SIZE, false),
    S0p1_c_S0p2_c_S0_c("121_S0+1.c_S0+2.c_S0.c", DEP_TABLE_SIZE, false),
    N0m1_c_N0p1_c_N0_c("122_N0-1.c_N0+1.c_N0.c", DEP_TABLE_SIZE, false),
    N0p1_c_N0p2_c_N0_c("123_N0+1.c_N0+2.c_N0.c", DEP_TABLE_SIZE, false),
    Sm2_c_Sm1_c_S0_c("124_S-2.c_S-1.c_S0.c",    DEP_TABLE_SIZE, false),
    Sm2_c_S0_c_N0_c("125_S-2.c_S0.c_N0.c",      DEP_TABLE_SIZE, false),
    S0m1_c_S0_c_N0_c("126_S0-1.c_S0.c_N0.c",    DEP_TABLE_SIZE, false),
    S0p1_c_S0_c_N0_c("127_S0+1.c_S0.c_N0.c",    DEP_TABLE_SIZE, false),
    N0m2_c_S0_c_N0_c("128_N0-2.c_S0.c_N0.c",    DEP_TABLE_SIZE, false),
    N0m1_c_S0_c_N0_c("129_N0-1.c_S0.c_N0.c",    DEP_TABLE_SIZE, false),
    N0p1_c_S0_c_N0_c("130_N0+1.c_S0.c_N0.c",    DEP_TABLE_SIZE, false),
    N0p2_c_S0_c_N0_c("131_N0+2.c_S0.c_N0.c",    DEP_TABLE_SIZE, false),
    N0p3_c_S0_c_N0_c("132_N0+3.c_S0.c_N0.c",    DEP_TABLE_SIZE, false),
    Sm3_c_S0_c_N0_c("133_S-3.c_S0.c_N0.c",      DEP_TABLE_SIZE, false),

    S0h_c("134_S0>h.c", DEP_TABLE_SIZE, false),
    S0lm_c("135_S0>lm.c", DEP_TABLE_SIZE, false),
    S0rm_c("136_S0>rm.c", DEP_TABLE_SIZE, false),
    N0h_c("137_N0>h.c", DEP_TABLE_SIZE, false),
    N0lm_c("138_N0>lm.c", DEP_TABLE_SIZE, false),

    S0lm_c_S0_c_N0_c("139_S0>lm.p_S0.p_N0.p", DEP_TABLE_SIZE, false),
    S0rm_c_S0_c_N0_c("140_S0>rm.p_S0.p_N0.p", DEP_TABLE_SIZE, false),
    S0h_c_S0_c_N0_c("141_S0>h.p_S0.p_N0.p", DEP_TABLE_SIZE, false),
    N0lm_c_S0_c_N0_c("142_N0>lm.p_S0.p_N0.p", DEP_TABLE_SIZE, false),
    N0h_c_S0_c_N0_c("143_N0>h.p_S0.p_N0.p", DEP_TABLE_SIZE, false),

    S0h2_c("144_S0>h2.c", DEP_TABLE_SIZE, false),
    S0lm2_c("145_S0>lm2.c", DEP_TABLE_SIZE, false),
    S0rm2_c("146_S0>rm2.c", DEP_TABLE_SIZE, false),
    N0h2_c("147_N0>h2.c", DEP_TABLE_SIZE, false),
    N0lm2_c("148_N0>lm2.c", DEP_TABLE_SIZE, false),

    S0lm2_c_S0lm_c_S0_c("149_S0>lm2.c_S0>lm.c_S0.c",  DEP_TABLE_SIZE, false),
    S0rm2_c_S0rm_c_S0_c("150_S0>rm2.c_S0>rm.c_S0.c",  DEP_TABLE_SIZE, false),
    N0lm2_c_N0lm_c_N0_c("151_N0>lm2.c_N0>lm.c_S0.c",  DEP_TABLE_SIZE, false),
    S0h2_c_S0h_c_S0_c("152_S0>h2.c_S0>h.c_S0.c",      DEP_TABLE_SIZE, false),
    N0h2_c_N0h_c_N0_c("153_N0>h2.c_N0>h.c_S0.c",      DEP_TABLE_SIZE, false),
    S0lm2_c_S0_c_N0_c("154_S0>lm2.c_S0.c_N0.c",       DEP_TABLE_SIZE, false),
    S0rm2_c_S0_c_N0_c("155_S0>rm2.c_S0.c_N0.c",       DEP_TABLE_SIZE, false),

    S0lm_d_S0_d("200_S0>lm.d_S0.d",   DEP_TABLE_SIZE, false),
    S0lm2_d_S0_d("201_S0>lm2.d_S0.d", DEP_TABLE_SIZE, false),
    S0rm_d_S0_d("202_S0>rm.d_S0.d",   DEP_TABLE_SIZE, false),
    S0rm2_d_S0_d("203_S0>rm2.d_S0.d", DEP_TABLE_SIZE, false),
    S0h_d_S0_d("204_S0>h.d_S0.d",     DEP_TABLE_SIZE, false),
    N0lm_d_N0_d("205_N0>lm.d_N0.d",   DEP_TABLE_SIZE, false),
    N0lm2_d_N0_d("206_N0>lm2.d_N0.d", DEP_TABLE_SIZE, false),
    N0h_d_N0_d("207_N0>h.d_N0.d",     DEP_TABLE_SIZE, false)
  {
    loadScores();
  }

  virtual ~CWeight() {
  }

  // MEHTODS
  virtual void  loadScores();
  virtual void  saveScores();
  void          computeAverageFeatureWeights(int round);
  SCORE_TYPE    dotProduct(const CWeight &w);
};

};
};


#endif    //
