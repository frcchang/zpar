// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                              *
 * depparser.cpp - the action based parser implementation       *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2007.12                        *
 *                                                              *
 ****************************************************************/

#include <functional>
#include "depparser.h"
#include "depparser_weight.h"

using namespace TARGET_LANGUAGE;
using namespace TARGET_LANGUAGE::depparser;

typedef CWord                             Word;
typedef CTag                              Tag;
typedef CTaggedWord<CTag, TAG_SEPARATOR>  TaggedWord;

const Word        EmptyWord("");
const Tag         NoneTag(CTag::NONE);
const TaggedWord  EmptyTaggedWord;

const int         kMaxSentenceSize  = MAX_SENTENCE_SIZE;
const int         kAgendaSize       = AGENDA_SIZE;

#define cast_weights static_cast<CWeight*>(m_weights)
#define refer_or_allocate_tuple2(x, o1, o2) do { \
  if (amount == 0) { \
    x.refer(o1, o2); \
  } else { \
    x.allocate(o1, o2); \
  } \
} while (0);
#define refer_or_allocate_tuple3(x, o1, o2, o3) do { \
  if (amount == 0) { \
    x.refer(o1, o2, o3); \
  } else { \
    x.allocate(o1, o2, o3); \
  } \
} while (0);

#define travel(x) do { \
  const CStateItem * __p = (x); \
  int __deb = 0; \
  while (__p) { \
    __deb ++; \
    __p = __p->previous_;\
  } \
  std::cout << __deb << std::endl; \
} while (0);

#define _conll_or_empty(x) (x == "_" ? "" : x)

// the constructor
CDepParser::CDepParser(
    const std::string & feature_database_path,
    bool                is_train,
    bool                is_conll)
  : CDepParserBase(feature_database_path, is_train, is_conll) {
  m_Beam    = new CAgendaSimple<depparser::action::CScoredAction>(AGENDA_SIZE);
  m_weights = new depparser::CWeight(feature_database_path, is_train);
  m_nTrainingRound  = 0;
  m_nTotalErrors    = 0;

  if (is_train) {
    m_nScoreIndex = CScore<depparser::SCORE_TYPE>::eNonAverage;
  } else {
    m_nScoreIndex = CScore<depparser::SCORE_TYPE>::eAverage;
  }
}

/*===============================================================
 *
 * CDepParser - the depparser for TARGET_LANGUAGE
 *
 *==============================================================*/

/*---------------------------------------------------------------
 *
 * getOrUpdateStackScore - manipulate the score from stack
 *
 *---------------------------------------------------------------*/
inline
void CDepParser::GetOrUpdateStackScore(
    const CStateItem                           *  item,
    CPackedScoreType<SCORE_TYPE, action::kMax> &  retval,
    const unsigned                             &  action,
    const SCORE_TYPE                           &  amount,
    const int                                     round) {
  // Global judgement of having CoNLLCPOS in given sentence
  bool enable_cpos = false;
  //  (m_bCoNLL && (m_lCacheCoNLLCPOS[1] != CCoNLLCPOS()));
  bool enable_ext_dep = false;

  const std::vector<CCoNLLCPOS> & CPS 
    = m_lCacheCoNLLCPOS;
  const int len       = m_lCache.size();

  const int S0_id   = item->stacktop();
  const int N0_id   = item->bufferfront();
  const int Sm2_id  = item->stack3rdtop();
  const int Sm1_id  = item->stack2ndtop();
  const int S0m2_id = (S0_id - 2 >= 0 ? S0_id - 2 : -1);
  const int S0m1_id = (S0_id - 1 >= 0 ? S0_id - 1 : -1);
  const int S0p1_id = (S0_id + 1 < len? S0_id + 1 : -1);
  const int S0p2_id = (S0_id + 2 < len? S0_id + 2 : -1);
  const int N0m2_id = (N0_id - 2 >= 0 ? N0_id - 2 : -1);
  const int N0m1_id = (N0_id - 1 >= 0 ? N0_id - 1 : -1);
  const int N0p1_id = (N0_id + 1 < len? N0_id + 1 : -1);
  const int N0p2_id = (N0_id + 2 < len? N0_id + 2 : -1);
  const int N0p3_id = (N0_id + 3 < len? N0_id + 3 : -1);

  const TaggedWord & S0   = (-1 == S0_id ?   EmptyTaggedWord : m_lCache[S0_id]);
  const TaggedWord & N0   = (-1 == N0_id ?   EmptyTaggedWord : m_lCache[N0_id]);
  const TaggedWord & Sm2  = (-1 == Sm2_id ?  EmptyTaggedWord : m_lCache[Sm2_id]);
  const TaggedWord & Sm1  = (-1 == Sm1_id ?  EmptyTaggedWord : m_lCache[Sm1_id]);
  const TaggedWord & S0m2 = (-1 == S0m2_id ? EmptyTaggedWord : m_lCache[S0m2_id]);
  const TaggedWord & S0m1 = (-1 == S0m1_id ? EmptyTaggedWord : m_lCache[S0m1_id]);
  const TaggedWord & S0p1 = (-1 == S0p1_id ? EmptyTaggedWord : m_lCache[S0p1_id]);
  const TaggedWord & S0p2 = (-1 == S0p2_id ? EmptyTaggedWord : m_lCache[S0p2_id]);
  const TaggedWord & N0m2 = (-1 == N0m2_id ? EmptyTaggedWord : m_lCache[N0m2_id]);
  const TaggedWord & N0m1 = (-1 == N0m1_id ? EmptyTaggedWord : m_lCache[N0m1_id]);
  const TaggedWord & N0p1 = (-1 == N0p1_id ? EmptyTaggedWord : m_lCache[N0p1_id]);
  const TaggedWord & N0p2 = (-1 == N0p2_id ? EmptyTaggedWord : m_lCache[N0p2_id]);
  const TaggedWord & N0p3 = (-1 == N0p3_id ? EmptyTaggedWord : m_lCache[N0p3_id]);

  const int S0h_id    = (-1 == S0_id ? -1 : item->head(S0_id));
  const int S0lm_id   = (-1 == S0_id ? -1 : item->leftdep(S0_id));
  const int S0rm_id   = (-1 == S0_id ? -1 : item->rightdep(S0_id));
  const int N0h_id    = (-1 == N0_id ? -1 : item->head(N0_id));
  const int N0lm_id   = (-1 == N0_id ? -1 : item->leftdep(N0_id));
  const int S0h2_id   = (-1 == S0h_id? -1 : item->head(S0h_id));
  const int S0lm2_id  = (-1 == S0_id ? -1 : item->left2dep(S0_id));
  const int S0rm2_id  = (-1 == S0_id ? -1 : item->right2dep(S0_id));
  const int N0h2_id   = (-1 == N0h_id? -1 : item->head(N0h_id));
  const int N0lm2_id  = (-1 == N0_id ? -1 : item->left2dep(N0_id));

  const TaggedWord & S0h  = (-1 == S0h_id  ? EmptyTaggedWord : m_lCache[S0h_id]);
  const TaggedWord & S0lm = (-1 == S0lm_id ? EmptyTaggedWord : m_lCache[S0lm_id]);
  const TaggedWord & S0rm = (-1 == S0rm_id ? EmptyTaggedWord : m_lCache[S0rm_id]);
  const TaggedWord & N0h  = (-1 == N0h_id  ? EmptyTaggedWord : m_lCache[N0h_id]);
  const TaggedWord & N0lm = (-1 == N0lm_id ? EmptyTaggedWord : m_lCache[N0lm_id]);
  const TaggedWord & S0h2 = (-1 == S0h2_id ? EmptyTaggedWord : m_lCache[S0h2_id]);
  const TaggedWord & S0lm2= (-1 == S0lm2_id? EmptyTaggedWord : m_lCache[S0lm2_id]);
  const TaggedWord & S0rm2= (-1 == S0rm2_id? EmptyTaggedWord : m_lCache[S0rm2_id]);
  const TaggedWord & N0h2 = (-1 == N0h2_id ? EmptyTaggedWord : m_lCache[N0h2_id]);
  const TaggedWord & N0lm2= (-1 == N0lm2_id? EmptyTaggedWord : m_lCache[N0lm2_id]);

  const int S0h_d  = (-1 == S0_id ?   CDependencyLabel::NONE : item->label(S0_id));
  const int S0lm_d = (-1 == S0lm_id ? CDependencyLabel::NONE : item->label(S0lm_id));
  const int S0rm_d = (-1 == S0rm_id ? CDependencyLabel::NONE : item->label(S0rm_id));
  const int N0h_d  = (-1 == N0_id ?   CDependencyLabel::NONE : item->label(N0_id));
  const int N0lm_d = (-1 == N0lm_id ? CDependencyLabel::NONE : item->label(N0lm_id));
  const int S0h2_d = (-1 == S0h_id ?  CDependencyLabel::NONE : item->label(S0h_id));
  const int S0lm2_d= (-1 == S0lm2_id? CDependencyLabel::NONE : item->label(S0lm2_id));
  const int S0rm2_d= (-1 == S0rm2_id? CDependencyLabel::NONE : item->label(S0rm2_id));
  const int N0h2_d = (-1 == N0h_id ?  CDependencyLabel::NONE : item->label(N0h_id));
  const int N0lm2_d= (-1 == N0lm2_id? CDependencyLabel::NONE : item->label(N0lm2_id));

  const int S0_ra = (S0_id == -1 ? 0 : item->rightarity(S0_id));
  const int S0_la = (S0_id == -1 ? 0 : item->leftarity(S0_id));
  const int N0_la = (N0_id == -1 ? 0 : item->leftarity(N0_id));

#define __GET_OR_UPDATE_SCORE(temp, feature) do { \
  cast_weights->temp.getOrUpdateScore(retval, feature, \
      action, m_nScoreIndex, amount, round); \
} while (0);

  // basic feature
  if (-1 != S0_id) {
    __GET_OR_UPDATE_SCORE(S0_w,       S0.word);         // 0
    __GET_OR_UPDATE_SCORE(S0_p,       S0.tag);          // 1
    __GET_OR_UPDATE_SCORE(S0_w_S0_p,  S0);              // 4

    if (enable_cpos) {
      __GET_OR_UPDATE_SCORE(S0_c, CPS[S0_id]); //  108
    }
  }

  if (-1 != N0_id) {
    __GET_OR_UPDATE_SCORE(N0_w,       N0.word);         // 2
    __GET_OR_UPDATE_SCORE(N0_p,       N0.tag);          // 3
    __GET_OR_UPDATE_SCORE(N0_w_N0_p,  N0);              // 5

    if (enable_cpos) {
      __GET_OR_UPDATE_SCORE(N0_c, CPS[N0_id]); //  109
    }
  }

  if (-1 != S0_id && -1 != N0_id) {
    CTagSet<CTag, 2> ts = CTagSet<CTag, 2>(encodeTags(S0.tag, N0.tag));
    CTwoWords ww;
    if (0 == amount)  { ww.refer(&S0.word, &N0.word);  }
    else              { ww.allocate(S0.word, N0.word); }

    __GET_OR_UPDATE_SCORE(S0_p_N0_p,  ts);              // 6
    __GET_OR_UPDATE_SCORE(S0_w_N0_w,  ww);              // 7
  }

  // unigram features
  if (-1 != Sm1_id) {
    __GET_OR_UPDATE_SCORE(Sm1_w,      Sm1.word);        // 10
  }

  if (-1 != S0m2_id) {
    __GET_OR_UPDATE_SCORE(S0m2_p,     S0m2.tag);        // 17

    if (enable_cpos) {
      __GET_OR_UPDATE_SCORE(S0m2_c,   CPS[S0m2_id]);
    }
  }

  if (-1 != S0m1_id) {
    __GET_OR_UPDATE_SCORE(S0m1_w,     S0m1.word);       // 11
    __GET_OR_UPDATE_SCORE(S0m1_p,     S0m1.tag);        // 18

    if (enable_cpos) {
      __GET_OR_UPDATE_SCORE(S0m1_c,   CPS[S0m1_id]);
    }
  }

  if (-1 != S0p1_id) {
    __GET_OR_UPDATE_SCORE(S0p1_w,     S0p1.word);       // 12
    __GET_OR_UPDATE_SCORE(S0p1_p,     S0p1.tag);        // 19

    if (enable_cpos) {
      __GET_OR_UPDATE_SCORE(S0p1_c,   CPS[S0p1_id]);
    }
  }

  if (-1 != S0p2_id) {
    __GET_OR_UPDATE_SCORE(S0p2_p,     S0p2.tag);        // 20

    if (enable_cpos) {
      __GET_OR_UPDATE_SCORE(S0p2_c,   CPS[S0p2_id]);
    }
  }

  if (-1 != N0m2_id) {
    __GET_OR_UPDATE_SCORE(N0m2_w,     N0m2.word);       // 13
  }

  if (-1 != N0m1_id) {
    __GET_OR_UPDATE_SCORE(N0m1_w,     N0m1.word);       // 14
    __GET_OR_UPDATE_SCORE(N0m1_p,     N0m1.tag);        // 21

    if (enable_cpos) {
      __GET_OR_UPDATE_SCORE(N0m1_c,   CPS[N0m1_id]);
    }
  }

  if (-1 != N0p1_id) {
    __GET_OR_UPDATE_SCORE(N0p1_w,     N0p1.word);       // 15
    __GET_OR_UPDATE_SCORE(N0p1_p,     N0p1.tag);        // 22

    if (enable_cpos) {
      __GET_OR_UPDATE_SCORE(N0p1_c,   CPS[N0p1_id]);
    }
  }

  if (-1 != N0p2_id) {
    __GET_OR_UPDATE_SCORE(N0p2_w,     N0p2.word);       // 16
  }

  if (-1 != N0p1_id && -1 != S0_id) {
    CTagSet<CTag, 2> ts(encodeTags(N0p1.tag, S0.tag));
    __GET_OR_UPDATE_SCORE(N0p1_p_S0_p,ts);              // 23

    if (enable_cpos) {
      CTuple2<CCoNLLCPOS, CCoNLLCPOS> ts;
      refer_or_allocate_tuple2(ts, &CPS[N0p1_id], &CPS[S0_id]);
      __GET_OR_UPDATE_SCORE(N0p1_c_S0_c, ts);
    }
  }

  if (-1 != Sm1_id && -1 != S0_id) {
    CTagSet<CTag, 2> ts(encodeTags(Sm1.tag, S0.tag));
    __GET_OR_UPDATE_SCORE(Sm1_p_S0_p, ts);              // 24

    if (enable_cpos) {
      CTuple2<CCoNLLCPOS, CCoNLLCPOS> ts;
      refer_or_allocate_tuple2(ts, &CPS[Sm1_id], &CPS[S0_id]);
      __GET_OR_UPDATE_SCORE(Sm1_c_S0_c, ts);
    }
  }

  if (-1 != Sm1_id && -1 != N0_id) {
    CTagSet<CTag, 2> ts(encodeTags(Sm1.tag, N0.tag));
    __GET_OR_UPDATE_SCORE(Sm1_p_N0_p, ts);              // 25

    if (enable_cpos) {
      CTuple2<CCoNLLCPOS, CCoNLLCPOS> ts;
      refer_or_allocate_tuple2(ts, &CPS[Sm1_id], &CPS[N0_id]);
      __GET_OR_UPDATE_SCORE(Sm1_c_N0_c, ts);
    }
  }

  if (-1 != N0m1_id && -1 != S0_id) {
    TaggedWord tw(S0.word, N0m1.tag);
    __GET_OR_UPDATE_SCORE(N0m1_p_S0_w, tw);             // 26
  }

  if (-1 != N0p1_id && -1 != S0_id) {
    TaggedWord tw(S0.word, N0p1.tag);
    __GET_OR_UPDATE_SCORE(N0p1_p_S0_w, tw);             // 27
  }

  if (-1 != N0p1_id && -1 != N0_id) {
    TaggedWord tw(N0.word, N0p1.tag);
    __GET_OR_UPDATE_SCORE(N0p1_p_N0_w, tw);             // 28
  }

  if (-1 != N0p1_id && -1 != S0_id) {
    TaggedWord tw(N0p1.word, S0.tag);
    __GET_OR_UPDATE_SCORE(N0p1_m_S0_p, tw);             // 29
  }

  if (-1 != N0p1_id && -1 != N0_id) {
    TaggedWord tw(N0p1.word, N0.tag);
    __GET_OR_UPDATE_SCORE(N0p1_m_N0_p, tw);             // 30
  }

  if (-1 != S0p1_id && -1 != S0_id) {
    CTwoWords ww;
    if (0 == amount)  { ww.refer(&S0p1.word, &S0.word);  }
    else              { ww.allocate(S0p1.word, S0.word); }
    __GET_OR_UPDATE_SCORE(S0p1_w_S0_w, ww);             // 31
  }

  if (-1 != S0p1_id && -1 != N0_id) {
    CTwoWords ww;
    if (0 == amount)  { ww.refer(&S0p1.word, &N0.word);  }
    else              { ww.allocate(S0p1.word, N0.word); }
    __GET_OR_UPDATE_SCORE(S0p1_w_N0_w, ww);             // 32
  }

  if (-1 != S0m2_id && -1 != S0m1_id && -1 != S0_id) {
    CTagSet<CTag, 3> ts(encodeTags(S0m2.tag, S0m1.tag, S0.tag));
    __GET_OR_UPDATE_SCORE(S0m2_p_S0m1_p_S0_p, ts);      // 33

    if (enable_cpos) {
      CTuple3<CCoNLLCPOS, CCoNLLCPOS, CCoNLLCPOS> ts;
      refer_or_allocate_tuple3(ts, &CPS[S0m2_id], &CPS[S0m1_id], &CPS[S0_id]);
      __GET_OR_UPDATE_SCORE(S0m2_c_S0m1_c_S0_c, ts);
    }
  }

  if (-1 != S0m1_id && -1 != S0p1_id && -1 != S0_id) {
    CTagSet<CTag, 3> ts(encodeTags(S0m1.tag, S0p1.tag, S0.tag));
    __GET_OR_UPDATE_SCORE(S0m1_p_S0p1_p_S0_p, ts);      // 34

    if (enable_cpos) {
      CTuple3<CCoNLLCPOS, CCoNLLCPOS, CCoNLLCPOS> ts;
      refer_or_allocate_tuple3(ts, &CPS[S0m1_id], &CPS[S0p1_id], &CPS[S0_id]);
      __GET_OR_UPDATE_SCORE(S0m1_c_S0p1_c_S0_c, ts);
    }
  }

  if (-1 != S0p1_id && -1 != S0p2_id && -1 != S0_id) {
    CTagSet<CTag, 3> ts(encodeTags(S0p1.tag, S0p2.tag, S0.tag));
    __GET_OR_UPDATE_SCORE(S0p1_p_S0p2_p_S0_p, ts);     // 35

    if (enable_cpos) {
      CTuple3<CCoNLLCPOS, CCoNLLCPOS, CCoNLLCPOS> ts;
      refer_or_allocate_tuple3(ts, &CPS[S0p1_id], &CPS[S0p2_id], &CPS[S0_id]);
      __GET_OR_UPDATE_SCORE(S0p1_c_S0p2_c_S0_c, ts);
    }
  }

  if (-1 != N0m1_id && -1 != N0p1_id && -1 != N0_id) {
    CTagSet<CTag, 3> ts(encodeTags(N0m1.tag, N0p1.tag, N0.tag));
    __GET_OR_UPDATE_SCORE(N0m1_p_N0p1_p_N0_p, ts);      // 36

    if (enable_cpos) {
      CTuple3<CCoNLLCPOS, CCoNLLCPOS, CCoNLLCPOS> ts;
      refer_or_allocate_tuple3(ts, &CPS[N0m1_id], &CPS[N0p1_id], &CPS[N0_id]);
      __GET_OR_UPDATE_SCORE(N0m1_c_N0p1_c_N0_c, ts);
    }
  }

  if (-1 != N0p1_id && -1 != N0p2_id && -1 != N0_id) {
    CTagSet<CTag, 3> ts(encodeTags(N0p1.tag, N0p2.tag, N0.tag));
    __GET_OR_UPDATE_SCORE(N0p1_p_N0p2_p_N0_p, ts);      // 37

    if (enable_cpos) {
      CTuple3<CCoNLLCPOS, CCoNLLCPOS, CCoNLLCPOS> ts;
      refer_or_allocate_tuple3(ts, &CPS[N0p1_id], &CPS[N0p2_id], &CPS[N0_id]);
      __GET_OR_UPDATE_SCORE(N0p1_c_N0p2_c_N0_c, ts);
    }
  }

  if (-1 != Sm2_id && -1 != Sm1_id && -1 != S0_id) {
    CTagSet<CTag, 3> ts(encodeTags(Sm2.tag, Sm1.tag, S0.tag));
    __GET_OR_UPDATE_SCORE(Sm2_p_Sm1_p_S0_p, ts);        // 38

    if (enable_cpos) {
      CTuple3<CCoNLLCPOS, CCoNLLCPOS, CCoNLLCPOS> ts;
      refer_or_allocate_tuple3(ts, &CPS[Sm2_id], &CPS[Sm1_id], &CPS[S0_id]);
      __GET_OR_UPDATE_SCORE(Sm2_c_Sm1_c_S0_c, ts);
    }
  }

  if (-1 != S0_id && -1 != N0_id) {
    if (-1 != Sm2_id) {
      CTagSet<CTag, 3> ts(encodeTags(Sm2.tag, S0.tag, N0.tag));
      __GET_OR_UPDATE_SCORE(Sm2_p_S0_p_N0_p, ts);         // 39

      if (enable_cpos) {
        CTuple3<CCoNLLCPOS, CCoNLLCPOS, CCoNLLCPOS> ts;
        refer_or_allocate_tuple3(ts, &CPS[Sm2_id], &CPS[S0_id], &CPS[N0_id]);
        __GET_OR_UPDATE_SCORE(Sm2_c_S0_c_N0_c, ts);
      }
    }

    if (-1 != S0m1_id) {
      CTagSet<CTag, 3> ts(encodeTags(S0m1.tag, S0.tag, N0.tag));
      __GET_OR_UPDATE_SCORE(S0m1_p_S0_p_N0_p, ts);      // 40

      if (enable_cpos) {
        CTuple3<CCoNLLCPOS, CCoNLLCPOS, CCoNLLCPOS> ts;
        refer_or_allocate_tuple3(ts, &CPS[S0m1_id], &CPS[S0_id], &CPS[N0_id]);
        __GET_OR_UPDATE_SCORE(S0m1_c_S0_c_N0_c, ts);
      }
    }

    if (-1 != S0p1_id) {
      CTagSet<CTag, 3> ts(encodeTags(S0p1.tag, S0.tag, N0.tag));
      __GET_OR_UPDATE_SCORE(S0p1_p_S0_p_N0_p, ts);      // 41

      if (enable_cpos) {
        CTuple3<CCoNLLCPOS, CCoNLLCPOS, CCoNLLCPOS> ts;
        refer_or_allocate_tuple3(ts, &CPS[S0p1_id], &CPS[S0_id], &CPS[N0_id]);
        __GET_OR_UPDATE_SCORE(S0p1_c_S0_c_N0_c, ts);
      }
    }

    if (-1 != N0m2_id) {
      CTagSet<CTag, 3> ts(encodeTags(N0m2.tag, S0.tag, N0.tag));
      __GET_OR_UPDATE_SCORE(N0m2_p_S0_p_N0_p, ts);      // 42

      if (enable_cpos) {
        CTuple3<CCoNLLCPOS, CCoNLLCPOS, CCoNLLCPOS> ts;
        refer_or_allocate_tuple3(ts, &CPS[N0m2_id], &CPS[S0_id], &CPS[N0_id]);
        __GET_OR_UPDATE_SCORE(N0m2_c_S0_c_N0_c, ts);
      }
    }

    if (-1 != N0m1_id) {
      CTagSet<CTag, 3> ts(encodeTags(N0m1.tag, S0.tag, N0.tag));
      __GET_OR_UPDATE_SCORE(N0m1_p_S0_p_N0_p, ts);      // 43

      if (enable_cpos) {
        CTuple3<CCoNLLCPOS, CCoNLLCPOS, CCoNLLCPOS> ts;
        refer_or_allocate_tuple3(ts, &CPS[N0m1_id], &CPS[S0_id], &CPS[N0_id]);
        __GET_OR_UPDATE_SCORE(N0m1_c_S0_c_N0_c, ts);
      }
    }

    if (-1 != N0p1_id) {
      CTagSet<CTag, 3> ts(encodeTags(N0p1.tag, S0.tag, N0.tag));
      __GET_OR_UPDATE_SCORE(N0p1_p_S0_p_N0_p, ts);      // 44

      if (enable_cpos) {
        CTuple3<CCoNLLCPOS, CCoNLLCPOS, CCoNLLCPOS> ts;
        refer_or_allocate_tuple3(ts, &CPS[N0p1_id], &CPS[S0_id], &CPS[N0_id]);
        __GET_OR_UPDATE_SCORE(N0p1_c_S0_c_N0_c, ts);
      }
    }

    if (-1 != N0p2_id) {
      CTagSet<CTag, 3> ts(encodeTags(N0p2.tag, S0.tag, N0.tag));
      __GET_OR_UPDATE_SCORE(N0p2_p_S0_p_N0_p, ts);      // 45

      if (enable_cpos) {
        CTuple3<CCoNLLCPOS, CCoNLLCPOS, CCoNLLCPOS> ts;
        refer_or_allocate_tuple3(ts, &CPS[N0p2_id], &CPS[S0_id], &CPS[N0_id]);
        __GET_OR_UPDATE_SCORE(N0p2_c_S0_c_N0_c, ts);
      }
    }

    if (-1 != N0p3_id) {
      CTagSet<CTag, 3> ts(encodeTags(N0p3.tag, S0.tag, N0.tag));
      __GET_OR_UPDATE_SCORE(N0p3_p_S0_p_N0_p, ts);      // 47

      if (enable_cpos) {
        CTuple3<CCoNLLCPOS, CCoNLLCPOS, CCoNLLCPOS> ts;
        refer_or_allocate_tuple3(ts, &CPS[N0p3_id], &CPS[S0_id], &CPS[N0_id]);
        __GET_OR_UPDATE_SCORE(N0p3_c_S0_c_N0_c, ts);
      }
    }
  }

  if (-1 != S0h_id) {
    __GET_OR_UPDATE_SCORE(S0h_w, S0h.word);   // 48
    __GET_OR_UPDATE_SCORE(S0h_p, S0h.tag);    // 53
    __GET_OR_UPDATE_SCORE(S0h_d, S0h_d);      // 58
  }

  if (-1 != S0lm_id) {
    __GET_OR_UPDATE_SCORE(S0lm_w, S0lm.word); // 49
    __GET_OR_UPDATE_SCORE(S0lm_p, S0lm.tag);  // 54
    __GET_OR_UPDATE_SCORE(S0lm_d, S0lm_d);    // 59

  }

  if (-1 != S0rm_id) {
    __GET_OR_UPDATE_SCORE(S0rm_w, S0rm.word); // 50
    __GET_OR_UPDATE_SCORE(S0rm_p, S0rm.tag);  // 55
    __GET_OR_UPDATE_SCORE(S0rm_d, S0rm_d);    // 60
  }

  if (-1 != N0h_id) {
    __GET_OR_UPDATE_SCORE(N0h_w,  N0h.word);  // 51
    __GET_OR_UPDATE_SCORE(N0h_p,  N0h.tag);   // 56
    __GET_OR_UPDATE_SCORE(N0h_d,  N0h_d);     // 61
  }

  if (-1 != N0lm_id) {
    __GET_OR_UPDATE_SCORE(N0lm_w, N0lm.word); // 52
    __GET_OR_UPDATE_SCORE(N0lm_p, N0lm.tag);  // 57
    __GET_OR_UPDATE_SCORE(N0lm_d, N0lm_d);    // 62
  }

  if (-1 != S0_id && -1 != N0_id) {
    if (-1 != S0lm_id) {
      CTagSet<CTag, 3> ts(encodeTags(S0lm.tag, S0.tag, N0.tag));
      __GET_OR_UPDATE_SCORE(S0lm_p_S0_p_N0_p, ts);

      if (enable_cpos) {
        CTuple3<CCoNLLCPOS, CCoNLLCPOS, CCoNLLCPOS> ts;
        refer_or_allocate_tuple3(ts, &CPS[S0lm_id], &CPS[S0_id], &CPS[N0_id]);
        // __GET_OR_UPDATE_SCORE(S0lm_c_S0_c_N0_c, ts);
      }
    }

    if (-1 != S0rm_id) {
      CTagSet<CTag, 3> ts(encodeTags(S0rm.tag, S0.tag, N0.tag));
      __GET_OR_UPDATE_SCORE(S0rm_p_S0_p_N0_p, ts);

      if (enable_cpos) {
        CTuple3<CCoNLLCPOS, CCoNLLCPOS, CCoNLLCPOS> ts;
        refer_or_allocate_tuple3(ts, &CPS[S0rm_id], &CPS[S0_id], &CPS[N0_id]);
        // __GET_OR_UPDATE_SCORE(S0rm_c_S0_c_N0_c, ts);
      }
    }

    if (-1 != S0h_id) {
      CTagSet<CTag, 3> ts(encodeTags(S0h.tag, S0.tag, N0.tag));
      __GET_OR_UPDATE_SCORE(S0h_p_S0_p_N0_p, ts);

      if (enable_cpos) {
        CTuple3<CCoNLLCPOS, CCoNLLCPOS, CCoNLLCPOS> ts;
        refer_or_allocate_tuple3(ts, &CPS[S0h_id], &CPS[S0_id], &CPS[N0_id]);
        // __GET_OR_UPDATE_SCORE(S0h_c_S0_c_N0_c, ts);
      }
    }

    if (-1 != N0lm_id) {
      CTagSet<CTag, 3> ts(encodeTags(N0lm.tag, S0.tag, N0.tag));
      __GET_OR_UPDATE_SCORE(N0lm_p_S0_p_N0_p, ts);

      if (enable_cpos) {
        CTuple3<CCoNLLCPOS, CCoNLLCPOS, CCoNLLCPOS> ts;
        refer_or_allocate_tuple3(ts, &CPS[N0lm_id], &CPS[S0_id], &CPS[N0_id]);
        // __GET_OR_UPDATE_SCORE(N0lm_c_S0_c_N0_c, ts);
      }
    }

    if (-1 != N0h_id) {
      CTagSet<CTag, 3> ts(encodeTags(N0h.tag, S0.tag, N0.tag));
      __GET_OR_UPDATE_SCORE(N0h_p_S0_p_N0_p, ts);

      if (enable_cpos) {
        CTuple3<CCoNLLCPOS, CCoNLLCPOS, CCoNLLCPOS> ts;
        refer_or_allocate_tuple3(ts, &CPS[N0h_id], &CPS[S0_id], &CPS[N0_id]);
        // __GET_OR_UPDATE_SCORE(N0h_c_S0_c_N0_c, ts);
      }
    }
  }

  // 3rd-order features
  if (-1 != S0h2_id) {
    __GET_OR_UPDATE_SCORE(S0h2_w, S0h2.word);   // 68
    __GET_OR_UPDATE_SCORE(S0h2_p, S0h2.tag);    // 73
    __GET_OR_UPDATE_SCORE(S0h2_d, S0h2_d);      // 78
  }

  if (-1 != S0lm2_id) {
    __GET_OR_UPDATE_SCORE(S0lm2_w, S0lm2.word); // 69
    __GET_OR_UPDATE_SCORE(S0lm2_p, S0lm2.tag);  // 74
    __GET_OR_UPDATE_SCORE(S0lm2_d, S0lm2_d);    // 79
  }

  if (-1 != S0rm2_id) {
    __GET_OR_UPDATE_SCORE(S0rm2_w, S0rm2.word); // 70
    __GET_OR_UPDATE_SCORE(S0rm2_p, S0rm2.tag);  // 75
    __GET_OR_UPDATE_SCORE(S0rm2_d, S0rm2_d);    // 80
  }

  if (-1 != N0h2_id) {
    __GET_OR_UPDATE_SCORE(N0h2_w, N0h2.word);   // 71
    __GET_OR_UPDATE_SCORE(N0h2_p, N0h2.tag);    // 76
    __GET_OR_UPDATE_SCORE(N0h2_d, N0h2_d);      // 81
  }

  if (-1 != N0lm2_id) {
    __GET_OR_UPDATE_SCORE(N0lm2_w, N0lm2.word); // 72
    __GET_OR_UPDATE_SCORE(N0lm2_p, N0lm2.tag);  // 77
    __GET_OR_UPDATE_SCORE(N0lm2_d, N0lm2_d);    // 82
  }

  if (-1 != S0lm2_id && -1 != S0lm_id && -1 != S0_id) {
    CTagSet<CTag, 3> ts(encodeTags(S0lm2.tag, S0lm.tag, S0.tag));
    __GET_OR_UPDATE_SCORE(S0lm2_p_S0lm_p_S0_p, ts);
  }

  if (-1 != S0rm2_id && -1 != S0rm_id && -1 != S0_id) {
    CTagSet<CTag, 3> ts(encodeTags(S0rm2.tag, S0rm.tag, S0.tag));
    __GET_OR_UPDATE_SCORE(S0rm2_p_S0rm_p_S0_p, ts);
  }

  if (-1 != N0lm2_id && -1 != N0lm_id && -1 != N0_id) {
    CTagSet<CTag, 3> ts(encodeTags(N0lm2.tag, N0lm.tag, N0.tag));
    __GET_OR_UPDATE_SCORE(N0lm2_p_N0lm_p_N0_p, ts);
  }

  if (-1 != S0h2_id && -1 != S0h_id && -1 != S0_id) {
    CTagSet<CTag, 3> ts(encodeTags(S0h2.tag, S0h.tag, S0.tag));
    __GET_OR_UPDATE_SCORE(S0h2_p_S0h_p_S0_p, ts);
  }

  if (-1 != N0h2_id && -1 != N0h_id && -1 != N0_id) {
    CTagSet<CTag, 3> ts(encodeTags(N0h2.tag, N0h.tag, N0.tag));
    __GET_OR_UPDATE_SCORE(N0h2_p_N0h_p_N0_p, ts);
  }

  if (-1 != S0_id) {
    CTuple2<CWord, int> wa;
    CTuple2<CTag,  int> ta;

    refer_or_allocate_tuple2(wa, &S0.word, &S0_ra);
    __GET_OR_UPDATE_SCORE(S0_w_S0_ra, wa);
    refer_or_allocate_tuple2(ta, &S0.tag,  &S0_ra);
    __GET_OR_UPDATE_SCORE(S0_p_S0_ra, ta);
    refer_or_allocate_tuple2(wa, &S0.word, &S0_la);
    __GET_OR_UPDATE_SCORE(S0_w_S0_la, wa);
    refer_or_allocate_tuple2(ta, &S0.tag,  &S0_la);
    __GET_OR_UPDATE_SCORE(S0_p_S0_la, ta);
  }

  if (enable_ext_dep) {
    CTuple2<int, int> dd;

    if (-1 != S0lm_id && -1 != S0_id) {
      dd.allocate(&S0lm_d, &S0h_d);
      __GET_OR_UPDATE_SCORE(S0lm_d_S0_d, dd); // 200
    }

    if (-1 != S0lm2_id && -1 != S0_id) {
      dd.allocate(&S0lm2_d, &S0h_d);
      __GET_OR_UPDATE_SCORE(S0lm2_d_S0_d, dd);  // 201
    }

    if (-1 != S0rm_id && -1 != S0_id) {
      dd.allocate(&S0rm_d, &S0h_d);
      __GET_OR_UPDATE_SCORE(S0rm_d_S0_d, dd); // 202
    }

    if (-1 != S0rm2_id && -1 != S0_id) {
      dd.allocate(&S0rm2_d, &S0h_d);
      __GET_OR_UPDATE_SCORE(S0rm2_d_S0_d, dd);  // 203
    }

    if (-1 != S0h_id && -1 != S0_id) {
      dd.allocate(&S0h2_d, &S0h_d);
      __GET_OR_UPDATE_SCORE(S0h_d_S0_d, dd);  //  204
    }

    if (-1 != N0lm_id && -1 != N0_id) {
      dd.allocate(&N0lm_d, &N0h_d);
      __GET_OR_UPDATE_SCORE(N0lm_d_N0_d, dd); // 205
    }

    if (-1 != N0lm2_id && -1 != N0_id) {
      dd.allocate(&N0lm2_d, &N0h_d);
      __GET_OR_UPDATE_SCORE(N0lm2_d_N0_d, dd);  // 206
    }

    if (-1 != N0h_id && -1 != N0_id) {
      dd.allocate(&N0h_d, &N0h_d);
      __GET_OR_UPDATE_SCORE(N0h_d_N0_d, dd);  //  207 
    }
  }

  // n0 arity
  if (-1 != N0_id) {
    CTuple2<CWord, int> wa;
    CTuple2<CTag,  int> ta;

    refer_or_allocate_tuple2(wa, &N0.word, &N0_la);
    __GET_OR_UPDATE_SCORE(N0_w_N0_la, wa);            // 95
    refer_or_allocate_tuple2(ta, &N0.tag,  &N0_la);
    __GET_OR_UPDATE_SCORE(N0_p_N0_la, ta);            // 101
  }

  // st labelset
  if (-1 != S0_id){
    const CSetOfTags<CDependencyLabel> & S0_rset = item->righttagset(S0_id);
    const CSetOfTags<CDependencyLabel> & S0_lset = item->lefttagset(S0_id);

    CTuple2<CWord, CSetOfTags<CDependencyLabel> > wtset;
    CTuple2<CTag, CSetOfTags<CDependencyLabel> >  ttset;

    refer_or_allocate_tuple2(wtset, &S0.word, &S0_rset);
    __GET_OR_UPDATE_SCORE(S0_w_S0_rset, wtset);
    refer_or_allocate_tuple2(ttset, &S0.tag,  &S0_rset);
    __GET_OR_UPDATE_SCORE(S0_p_S0_rset, ttset);
    refer_or_allocate_tuple2(wtset, &S0.word, &S0_lset);
    __GET_OR_UPDATE_SCORE(S0_w_S0_lset, wtset);
    refer_or_allocate_tuple2(ttset, &S0.tag,  &S0_lset);
    __GET_OR_UPDATE_SCORE(S0_p_S0_lset, ttset);
  }

  // n0 labelset
  if (-1 != N0_id){
    const CSetOfTags<CDependencyLabel> & N0_lset = item->lefttagset(N0_id);
    CTuple2<CWord, CSetOfTags<CDependencyLabel> > wtset;
    CTuple2<CTag, CSetOfTags<CDependencyLabel> >  ttset;

    refer_or_allocate_tuple2(wtset, &N0.word, &N0_lset);
    __GET_OR_UPDATE_SCORE(N0_w_N0_lset, wtset);
    refer_or_allocate_tuple2(ttset, &N0.tag,  &N0_lset);
    __GET_OR_UPDATE_SCORE(N0_p_N0_lset, ttset);
  }

  if (m_bCoNLL) {
    if (-1 != S0_id) {
      if (!m_lCacheCoNLLLemma[S0_id].empty()) {
        __GET_OR_UPDATE_SCORE(S0conll_l, m_lCacheCoNLLLemma[S0_id]);
      }
      if (CCoNLLCPOS() != m_lCacheCoNLLCPOS[S0_id]) {
        __GET_OR_UPDATE_SCORE(S0conll_c, m_lCacheCoNLLCPOS[S0_id]);
      }
      for (int i = 0; i < m_lCacheCoNLLFeats[S0_id].size(); ++ i) {
        __GET_OR_UPDATE_SCORE(S0conll_f, m_lCacheCoNLLFeats[S0_id][i]);
      }
    }

    if (-1 != N0_id) {
      if (!m_lCacheCoNLLLemma[N0_id].empty()) {
        __GET_OR_UPDATE_SCORE(N0conll_l, m_lCacheCoNLLLemma[N0_id]);
      }
      if (CCoNLLCPOS() != m_lCacheCoNLLCPOS[N0_id]) {
        __GET_OR_UPDATE_SCORE(N0conll_c, m_lCacheCoNLLCPOS[N0_id]);
      }
      for (int i = 0; i < m_lCacheCoNLLFeats[N0_id].size(); ++ i) {
        __GET_OR_UPDATE_SCORE(N0conll_f, m_lCacheCoNLLFeats[N0_id][i]);
      }
    }
  }
#undef __GET_OR_UPDATE_SCORE
}

/*---------------------------------------------------------------
 *
 * getGlobalScore - get the score of a parse tree
 *
 * Inputs: parse graph
 *
 *---------------------------------------------------------------*/
SCORE_TYPE
CDepParser::getGlobalScore(const CDependencyParse &parsed) {
  THROW("depparser.cpp: getGlobalScore not implemented");
}

/*---------------------------------------------------------------
 *
 * updateScores - update the score std::vector
 *
 * This method is different from updateScoreVector in that
 * 1. It is for external call
 * 2. The tagging sequences for parsed and correct may differ
 *
 * Inputs: the parsed and the correct example
 *
 *---------------------------------------------------------------*/
void
CDepParser::updateScores(
    const CDependencyParse & parsed,
    const CDependencyParse & correct,
    int round ) {
  THROW("depparser.cpp: updateScores not implemented");
}

/*---------------------------------------------------------------
 *
 * updateScoresForStates - update scores for states
 *
 *--------------------------------------------------------------*/
void CDepParser::UpdateScoresForStates(
    const CStateItem *  predicated_state,
    const CStateItem *  correct_state,
    const SCORE_TYPE    amount_add,
    const SCORE_TYPE    amount_subtract) {

  static CPackedScoreType<SCORE_TYPE, action::kMax> empty;

  // do not update those steps where they are correct
  const CStateItem * predicated_state_chain[kMaxSentenceSize * kMaxSentenceSize];
  const CStateItem * correct_state_chain[kMaxSentenceSize * kMaxSentenceSize];

  int num_predicated_states = 0;
  int num_correct_states    = 0;

  for (const CStateItem * p = predicated_state; p; p = p->previous_) {
    predicated_state_chain[num_predicated_states] = p;
    ++ num_predicated_states;
  }

  for (const CStateItem * p = correct_state; p; p = p->previous_) {
    correct_state_chain[num_correct_states] = p;
    ++ num_correct_states;
  }

  ASSERT(num_correct_states == num_predicated_states,
      "Number of predicated action don't equals the correct one");

  int i;
  for (i = num_correct_states - 1; i >= 0; -- i) {
    // if the action is different, do the update
    unsigned predicated_action  = predicated_state_chain[i]->last_action;
    unsigned correct_action     = correct_state_chain[i]->last_action;
    if (predicated_action != correct_action) {
      break;
    }
  }

  // for (int j = i + 1; j > 0; -- j) { std::cout << (*predicated_state_chain[j]); }
  // for (int j = i + 1; j > 0; -- j) { std::cout << (*correct_state_chain[j]);    }

  for (i = i + 1; i > 0; -- i) {
    unsigned predicated_action  = predicated_state_chain[i - 1]->last_action;
    unsigned correct_action     = correct_state_chain[i - 1]->last_action;

    GetOrUpdateStackScore(predicated_state_chain[i],
        empty, predicated_action, amount_subtract, m_nTrainingRound);
    GetOrUpdateStackScore(correct_state_chain[i],
        empty, correct_action,    amount_add,      m_nTrainingRound);
  }

  m_nTotalErrors++;
}

/* --------------------------------------------------------------
 *
 *
 * ------------------------------------------------------------- */
inline
void CDepParser::Idle(
    const CStateItem * item,
    const CPackedScoreType<SCORE_TYPE, action::kMax> & scores) {
  static action::CScoredAction scored_action;
  scored_action.action = action::kIdle;
  scored_action.score = item->score + scores[scored_action.action];
  m_Beam->insertItem(&scored_action);
}

/*---------------------------------------------------------------
 *
 * reduce - helper function
 *
 *--------------------------------------------------------------*/
inline
void CDepParser::Reduce(
    const CStateItem * item,
    const CPackedScoreType<SCORE_TYPE, action::kMax> & scores) {
  static action::CScoredAction scored_action;
  // update stack score
  scored_action.action = action::kReduce;
  scored_action.score = item->score + scores[scored_action.action];
  m_Beam->insertItem(&scored_action);
}

/*---------------------------------------------------------------
 *
 * arcleft - helping function
 *
 *--------------------------------------------------------------*/
inline
void CDepParser::ArcLeft(
    const CStateItem * item,
    const CPackedScoreType<SCORE_TYPE, action::kMax> & scores) {
  static action::CScoredAction scored_action;
#ifdef LABELED
  for (unsigned label = CDependencyLabel::FIRST;
      label < CDependencyLabel::COUNT; ++ label) {
    scored_action.action = action::EncodeAction(action::kArcLeft, label);
    scored_action.score = item->score + scores[scored_action.action];
    m_Beam->insertItem(&scored_action);
  }
#else
  scored_action.action = action::kArcLeft;
  scored_action.score = item->score + scores[scored_action.action];
  m_Beam->insertItem(&scored_action);
#endif
}

/*---------------------------------------------------------------
 *
 * arcright - helping function
 *
 *--------------------------------------------------------------*/
inline
void CDepParser::ArcRight(
    const CStateItem *item,
    const CPackedScoreType<SCORE_TYPE, action::kMax> & scores) {
  static action::CScoredAction scored_action;
#ifdef LABELED
  if (0 == item->stacktop()) {
    scored_action.action = action::EncodeAction(action::kArcRight, 
                                                CDependencyLabel::ROOT);
    scored_action.score = item->score + scores[scored_action.action];
    m_Beam->insertItem(&scored_action);
  } else {
    for (unsigned label = CDependencyLabel::FIRST + 1;
        label < CDependencyLabel::COUNT; ++ label) {
      scored_action.action = action::EncodeAction(action::kArcRight, label);
      scored_action.score = item->score + scores[scored_action.action];
      m_Beam->insertItem(&scored_action);
    }
  }
#else
  scored_action.action = action::kArcRight;
  scored_action.score = item->score + scores[scored_action.action];
  m_Beam->insertItem(&scored_action);
#endif
}

/*---------------------------------------------------------------
 *
 * shift - help function
 *
 *--------------------------------------------------------------*/

inline
void CDepParser::Shift(
    const CStateItem *item,
    const CPackedScoreType<SCORE_TYPE, action::kMax> & scores) {
  static action::CScoredAction scored_action;
  scored_action.action = action::kShift;
  scored_action.score  = item->score + scores[scored_action.action];
  m_Beam->insertItem(&scored_action);
}

/*---------------------------------------------------------------
 *
 * poproot - help function
 *
 *--------------------------------------------------------------*/
inline
void CDepParser::PopRoot(
    const CStateItem *item,
    const CPackedScoreType<SCORE_TYPE, action::kMax> & scores) {
  static action::CScoredAction scored_action;
  scored_action.action = action::kPopRoot;
  scored_action.score  = item->score + scores[scored_action.action];
  m_Beam->insertItem(&scored_action);
}

/* --------------------------------------------------------------
 *
 * nopass - help function
 *
 * -------------------------------------------------------------- */
inline
void CDepParser::NoPass(
    const CStateItem * item,
    const CPackedScoreType<SCORE_TYPE, action::kMax> & scores) {
  static action::CScoredAction scored_action;
  scored_action.action = action::kNoPass;
  scored_action.score  = item->score + scores[scored_action.action];
  m_Beam->insertItem(&scored_action);
}

/* -------------------------------------------------------------
 *
 * left pass - help function
 *
 * ------------------------------------------------------------- */
inline
void CDepParser::LeftPass(
    const CStateItem * item,
    const CPackedScoreType<SCORE_TYPE, action::kMax> & scores) {
  static action::CScoredAction scored_action;
#ifdef LABELED
  for (unsigned label = CDependencyLabel::FIRST + 1;
      label < CDependencyLabel::COUNT; ++ label) {
    scored_action.action = action::EncodeAction(action::kLeftPass, label);
    scored_action.score  = item->score + scores[scored_action.action];
    m_Beam->insertItem(&scored_action);
  }
#else
  scored_action.action = action::kLeftPass;
  scored_action.score  = item->score + scores[scored_action.action];
  m_Beam->insertItem(&scored_action);
#endif  //  end for LABELED
}

/* -------------------------------------------------------------
 *
 * arcright - the helper function
 *
 * ------------------------------------------------------------- */
inline
void CDepParser::RightPass(
    const CStateItem * item,
    const CPackedScoreType<SCORE_TYPE, action::kMax> & scores) {
  static action::CScoredAction scored_action;
#ifdef LABELED
  if (0 == item->stacktop()) {
    scored_action.action = action::EncodeAction(action::kRightPass, 
                                                CDependencyLabel::ROOT);
    scored_action.score = item->score + scores[scored_action.action];
    m_Beam->insertItem(&scored_action);
  } else {
    for (unsigned label = CDependencyLabel::FIRST + 1;
        label < CDependencyLabel::COUNT; ++ label) {
      scored_action.action = action::EncodeAction(action::kRightPass, label);
      scored_action.score  = item->score + scores[scored_action.action];
      m_Beam->insertItem(&scored_action);
    }
  }
#else
  scored_action.action = action::kRightPass;
  scored_action.score  = item->score + scores[scored_action.action];
  m_Beam->insertItem(&scored_action);
#endif  //  end for LABELED
}

bool StateHeapMore(const CStateItem & x, const CStateItem & y) {
  return x.score > y.score;
}

int CDepParser::InsertIntoBeam(
    CStateItem *       beam,
    const CStateItem * item,
    const int          current_beam_size,
    const int          max_beam_size) {
  if (current_beam_size == max_beam_size) {
    if (*item > *beam) {
      std::pop_heap(beam, beam + max_beam_size, StateHeapMore);
      *(beam + max_beam_size - 1) = *item;
      std::push_heap(beam, beam + max_beam_size, StateHeapMore);
    }
    return 0;
  }

  *(beam + current_beam_size) = *(item);
  std::push_heap(beam, beam + current_beam_size + 1, StateHeapMore);
  return 1;
}

// Enumerate all the possible transition from the given state, insert the legal
// actions into beam.
void CDepParser::Transit(
    const CStateItem * generator,
    const CPackedScoreType<SCORE_TYPE, action::kMax> & packed_scores) {

  // no possible transtion, return to guarentee
  if (generator->terminated()) {
    Idle(generator, packed_scores);
    return;
  }

  // precondition:
  //  - 1. stack is not empty;
  //  - 2. stack size equals 1
  if (generator->complete()) {
    PopRoot(generator, packed_scores);
    return;
  }

  // precondition:
  //  - 1. buffer is not empty
  if (!generator->bufferempty()) {
    Shift(generator, packed_scores);
  }

  // precondition:
  //  1. stack is not empty
  //  2. stack top has head.
  if ((!generator->stackempty()) &&
      (generator->hashead(generator->stacktop()))) {
    Reduce(generator, packed_scores);
  }

  bool left_linkable  = ((!generator->stackempty())
                        && (!generator->bufferempty())
                        && (!generator->hashead(generator->stacktop()))
                        && !generator->is_descendant(generator->bufferfront(),
                                                     generator->stacktop())
                        );
  bool right_linkable = ((!generator->stackempty())
                        && (!generator->bufferempty())
                        && !generator->hashead(generator->bufferfront()) 
                        && !generator->is_descendant(generator->stacktop(),
                                                     generator->bufferfront())
                        );
  // std::cout << left_linkable << " " << right_linkable << std::endl;
  // precondition:
  //  - 1. stack is not empty
  if (!generator->stackempty()) {
    NoPass(generator, packed_scores);
  }

  // link arc (left <- right)
  // precondition:
  //  - 1. stack is not empty
  //  - 2. stack top is not pseudo root 
  //  - 3. buffer is not empty
  if (left_linkable
      && (generator->stacktop())
      ) {
    ArcLeft(generator, packed_scores);
  }

  // link arc (left -> right)
  // precondition:
  //  - 1. stack is not empty
  //  - 2. buffer is not empty
  //  - 3. buffer has no head
  if (right_linkable) {
    ArcRight(generator, packed_scores);
  }

  // link arc (left <- right)
  // precondition:
  //  - 1. stack is not empty
  //  - 2. stack top is not pseudo root
  //  - 3. buffer is not empty
  if (left_linkable 
      && (generator->stacktop())) {
    LeftPass(generator, packed_scores);
  }

  // precondition
  //
  if (right_linkable) {
    RightPass(generator, packed_scores);
  }
}

CStateItem * CDepParser::lattice_ = 0;
int CDepParser::max_lattice_size_ = 0;

CStateItem *
CDepParser::GetLattice(int max_lattice_size) {
  if (0 == lattice_) {
    max_lattice_size_ = max_lattice_size;
    lattice_ = new CStateItem[max_lattice_size];
  } else if (max_lattice_size_ < max_lattice_size) {
    delete [] lattice_;
    max_lattice_size_ = max_lattice_size;
    lattice_ = new CStateItem[max_lattice_size];
  }

  return lattice_;
}

/*---------------------------------------------------------------
 *
 * work - the working process shared by training and parsing
 *
 * Returns: makes a new instance of CDependencyParse
 *
 *--------------------------------------------------------------*/
int CDepParser::Work(
    CDependencyParse       * retval,
    SCORE_TYPE             * scores,
    const CTwoStringVector & sentence,
    const CDependencyParse & oracle_tree,
    int                      nbest,
    bool                     is_train) {

#ifdef DEBUG
  clock_t total_start_time = clock();
#endif  // end for DEBUG

  const int length           = sentence.size();
  const int max_round        = (length + 1) * (length + 1);
  const int max_lattice_size = (kAgendaSize + 1) * max_round;

  ASSERT(length < kMaxSentenceSize,
      "The size of the sentence is larger than the system configuration.");

  CStateItem * lattice = GetLattice(max_lattice_size);
    //new CStateItem[max_lattice_size];     // allocate the memory pool
  CStateItem * lattice_index[max_round];  // specify states of certain round
  const CStateItem * correct_state;       // point to the correct state's
                                          // position in the lattice

  for (int i = 0; i < max_lattice_size; ++ i) {
    lattice[i].len_ = length;
  }

  // initialize first round lattice
  // clear the start state.
  lattice[0].clear();
  correct_state = lattice;
  lattice_index[0] = lattice;
  lattice_index[1] = lattice_index[0] + 1;

  static CPackedScoreType<SCORE_TYPE, action::kMax> packed_scores;

  // TRACE("Initialising the decoding process...");
  m_lCache.clear();
  for (int index = 0; index < length; ++ index) {
     m_lCache.push_back(TaggedWord(sentence[index].first , sentence[index].second));
  }

  if (m_supertags) {
    // verifying supertags
    ASSERT(m_supertags->getSentenceSize() == length,
        "Sentence size does not match supertags size");
  }

#ifdef LABELED
  m_lCacheLabel.clear();
  if (is_train) {
    for (int i = 0; i < length; ++ i) {
      m_lCacheLabel.push_back(CDependencyLabel(oracle_tree[i].label));
    }
  }
#endif

  // TRACE("Decoding started");
  int num_results = 0;
  int round = 0;
  bool is_correct;    // used for training to specify correct state in lattice

  // loop with the next word to process in the sentence,
  // `round` represent the generators, and the condidates should be inserted
  // into the `round + 1`
  for (round = 1; round < max_round; ++ round) {
    if (is_train) {
      is_correct = false;
    }

    if (lattice_index[round - 1] == lattice_index[round]) {
      // there is nothing in generators, the proning has cut all legel
      // generator. actually, in this kind of case, we should raise a 
      // exception. however to achieve a parsing tree, an alternative
      // solution is go back to the previous round
      WARNING("Parsing Failed!");
      -- round;
      break;
      // return -1;
    }

    int current_beam_size = 0;
    // loop over the generator states
    // std::cout << "round : " << round << std::endl;
    for (const CStateItem * generator = lattice_index[round - 1];
        generator != lattice_index[round];
        ++ generator) {

      m_Beam->clear();  packed_scores.reset();
      GetOrUpdateStackScore(generator, packed_scores, action::kNoAction);
      Transit(generator, packed_scores);

      for (unsigned i = 0; i < m_Beam->size(); ++ i) {
        CStateItem candidate; candidate = (*generator);
        // generate candidate state according to the states in beam
        candidate.score     = m_Beam->item(i)->score;
        candidate.previous_ = generator;
        candidate.Move(m_Beam->item(i)->action);

        current_beam_size += InsertIntoBeam(lattice_index[round],
                                            &candidate,
                                            current_beam_size,
                                            kAgendaSize);
      }
    }

    lattice_index[round + 1] = lattice_index[round] + current_beam_size;

    bool all_finished = (true && current_beam_size > 0);
    for (const CStateItem * p = lattice_index[round];
        p != lattice_index[round + 1]; ++ p) {
      if (false == p->terminated()) { all_finished = false; }
    }

    if (all_finished) {
      break;
    }

    if (is_train) {
      CStateItem next_correct_state;
      next_correct_state = (*correct_state);
      next_correct_state.StandardMoveStep(oracle_tree
#ifdef LABELED
          , m_lCacheLabel
#endif  //  end for LABELED
          );
      next_correct_state.previous_ = correct_state;

      // correct_state = NULL;
      is_correct = false;
      for (CStateItem * p = lattice_index[round];
          p != lattice_index[round + 1];
          ++ p) {
        if (next_correct_state.last_action == p->last_action &&
            p->previous_ == correct_state) {
          correct_state = p;
          is_correct = true;
        }
      }
#ifdef EARLY_UPDATE
      if (!is_correct) {
        TRACE("ERROR at the " << next_correct_state.size() << "th word;"
            << " Total is " << oracle_tree.size());
        CStateItem * best_generator = lattice_index[round];
        for (CStateItem * p = lattice_index[round];
            p != lattice_index[round + 1];
            ++ p) {
          if (best_generator->score < p->score) {
            best_generator = p;
          }
        }

        // std::cout << "predicated: " << (*best_generator);
        // std::cout << "correct:    " << (next_correct_state);
        UpdateScoresForStates(best_generator, &next_correct_state, 1, -1);
        // delete [] lattice;

        return -1;
      }
#endif  //  end for EARLY_UPDATE

      if (correct_state->terminated()) {
        break;
      }
    }
  }

  if (!retval) {
    // delete [] lattice;
    return -1;
  }

  TRACE("Output sentence");
  std::sort(lattice_index[round - 1], lattice_index[round], std::greater<CStateItem>());
  num_results = lattice_index[round] - lattice_index[round - 1];
  for (int i = 0; i < std::min(num_results, nbest); ++ i) {
    (lattice_index[round - 1] + 1)->GenerateTree(sentence, retval[i]);
    if (scores) { scores[i] = (lattice_index[round - 1] + i)->score; }
  }

  TRACE("Done, total time spent: " << double(clock() - total_start_time) / CLOCKS_PER_SEC);
  // delete [] lattice;
  return num_results;
}

/*---------------------------------------------------------------
 *
 * parse - do dependency parsing to a sentence
 *
 * Returns: makes a new instance of CDependencyParse
 *
 *--------------------------------------------------------------*/

void CDepParser::parse(
    const CTwoStringVector & sentence,
    CDependencyParse *       retval,
    int                      nbest,
    SCORE_TYPE *             scores) {
  THROW("depparser.cpp :: parse not implemented");
}

/*---------------------------------------------------------------
 *
 * train - train the models with an example
 *
 *---------------------------------------------------------------*/
void CDepParser::train(
    const CDependencyParse &  correct,
    int                       round) {
  THROW("depparser.cpp :: parse not implemented");
};

/*---------------------------------------------------------------
 *
 * extract_features - extract features from an example (counts
 * recorded to parser model as weights)
 *
 *---------------------------------------------------------------*/

void CDepParser::extract_features(const CDependencyParse & oracle_tree) {
  int max_steps = (oracle_tree.size() + 1) * (oracle_tree.size() + 1);
  CStateItem * state_chain = new CStateItem[max_steps];
  CPackedScoreType<SCORE_TYPE, action::kMax> empty;

  for (int i = 0; i < max_steps; ++ i) {
    state_chain[i].len_ = oracle_tree.size();
  }

  m_lCache.clear();
  for (int i = 0; i < oracle_tree.size(); ++ i) {
    m_lCache.push_back(TaggedWord(oracle_tree[i].word, oracle_tree[i].tag));
  }

#ifdef LABELED
  m_lCacheLabel.clear();
  for (int i = 0; i < oracle_tree.size(); ++ i) {
    m_lCacheLabel.push_back(CDependencyLabel(oracle_tree[i].label));
  }
#endif

  int round = 0;
  state_chain[round].clear();

  for (round = 1; ; ++ round) {
    state_chain[round] = state_chain[round - 1];
    state_chain[round].StandardMoveStep(oracle_tree
#ifdef LABELED
        , m_lCacheLabel
#endif
        );

    unsigned long action = state_chain[round].last_action;
    GetOrUpdateStackScore(state_chain + round - 1, empty, action, 1, 1);
    if (state_chain[round].terminated()) {
      break;
    }
  }

  assert(state_chain[round].len_ == oracle_tree.size());
  for (int i = 1; i < oracle_tree.size(); ++ i) {
    assert(state_chain[round].head(i) != DEPENDENCY_LINK_NO_HEAD);
  }
  delete [] state_chain;
}

/*---------------------------------------------------------------
 *
 * initCoNLLCache
 *
 *---------------------------------------------------------------*/

template<typename CCoNLLInputOrOutput>
void CDepParser::InitializeCoNLLCache(
    const CCoNLLInputOrOutput & conll_tree_with_pseudo_root) {
  int len = conll_tree_with_pseudo_root.size();
  m_lCacheCoNLLLemma.resize(len);
  m_lCacheCoNLLCPOS.resize (len);
  m_lCacheCoNLLFeats.resize(len);

  for (unsigned i = 0; i < conll_tree_with_pseudo_root.size(); ++ i) {
     m_lCacheCoNLLLemma[i].load(
         _conll_or_empty(conll_tree_with_pseudo_root.at(i).lemma));
     m_lCacheCoNLLCPOS[i].load(
         _conll_or_empty(conll_tree_with_pseudo_root.at(i).ctag));
     m_lCacheCoNLLFeats[i].clear();
     if (conll_tree_with_pseudo_root.at(i).feats != "_") {
        readCoNLLFeats(m_lCacheCoNLLFeats[i],
                       conll_tree_with_pseudo_root.at(i).feats);
     }
  }
}

void CDepParser::ConvertCoNLLToDependency(
    const CCoNLLOutput & conll_tree,
    CDependencyParse   & tree) {
  conll_tree.toDependencyTree(tree);
}

void CDepParser::ConvertCoNLLToTwoStringVector(
    const CCoNLLInput  & conll_tree,
    CTwoStringVector   & sentence) {
  conll_tree.toTwoStringVector(sentence);
}

/*---------------------------------------------------------------
 *
 * parse_conll - do dependency parsing to a sentence
 *
 * Returns: makes a new instance of CDependencyParse
 *
 *--------------------------------------------------------------*/

void CDepParser::parse_conll(
    const CCoNLLInput & conll_tree_with_pseudo_root,
    CCoNLLOutput *      retval,
    int                 nbest,
    SCORE_TYPE *        scores) {
  assert(m_bCoNLL);

  // std::cout << conll_tree_with_pseudo_root.size() << std::endl;
  CDependencyParse empty;
  CTwoStringVector sentence_with_pseudo_root;
  CDependencyParse outout[AGENDA_SIZE];

  InitializeCoNLLCache(conll_tree_with_pseudo_root);
  ConvertCoNLLToTwoStringVector(conll_tree_with_pseudo_root,
                                sentence_with_pseudo_root);

  for (int i = 0; i < nbest; ++ i) {
     // clear the outout sentences
     retval[i].clear();
     outout[i].clear();
     if (scores) scores[i] = 0; //pGenerator->score;
  }

  int num_results = Work(outout,
                         scores,
                         sentence_with_pseudo_root,
                         empty,
                         nbest,
                         false);

  for (int i = 0; i < std::min(nbest, num_results); ++ i) {
    // now make the conll format stype outout
    retval[i].fromCoNLLInput(conll_tree_with_pseudo_root);
    // std::cout << conll_tree.size() << " " << outout[i].size() << std::endl;
    retval[i].copyDependencyHeads(outout[i]);
  }
}

/*---------------------------------------------------------------
 *
 * train_conll - train the models with an example
 *
 *---------------------------------------------------------------*/
// perform training on conll data
void CDepParser::train_conll(
    const CCoNLLOutput & oracle_conll_tree_with_pseudo_root,
    int round) {
  assert(m_bCoNLL);

  CTwoStringVector sentence;
  CDependencyParse oracle_tree_with_pseudo_root;

  InitializeCoNLLCache(oracle_conll_tree_with_pseudo_root);
  ConvertCoNLLToDependency(oracle_conll_tree_with_pseudo_root,
                           oracle_tree_with_pseudo_root);
  UnparseSentence (&oracle_tree_with_pseudo_root, &sentence);

  // The following code does update for each processing stage
  m_nTrainingRound = round ;
  Work(
      NULL,
      NULL,
      sentence,
      oracle_tree_with_pseudo_root,
      1,
      true);
}

/*---------------------------------------------------------------
 *
 * extract_features_conll - extract features from an example
 *
 *---------------------------------------------------------------*/

void CDepParser::extract_features_conll(
    const CCoNLLOutput & oracle_conll_tree_with_pseudo_root) {
  assert(m_bCoNLL);

  CDependencyParse oracle_tree_with_pseudo_root;
  InitializeCoNLLCache(oracle_conll_tree_with_pseudo_root);
  ConvertCoNLLToDependency(oracle_conll_tree_with_pseudo_root,
                           oracle_tree_with_pseudo_root);
  extract_features(oracle_tree_with_pseudo_root);
}

void CDepParser::finishtraining() {
  static_cast<depparser::CWeight*>(m_weights)->computeAverageFeatureWeights(
      m_nTrainingRound);
  static_cast<depparser::CWeight*>(m_weights)->saveScores();
  std::cout << "Total number of training errors are: " << m_nTotalErrors << std::endl;
}


