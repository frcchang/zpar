// Copyright (C) University of Oxford 2010
#include "depparser.h"
#include "depparser_weight.h"

using namespace TARGET_LANGUAGE;
using namespace TARGET_LANGUAGE::depparser;

const CWord g_emptyWord("");
const CTaggedWord<CTag, TAG_SEPARATOR> g_emptyTaggedWord;
const CTag g_noneTag = CTag::NONE;

const int kMaxSentenceSize = MAX_SENTENCE_SIZE;
const int kAgendaSize = AGENDA_SIZE;

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

#define _conll_or_empty(x) (x == "_" ? "" : x)

CDepParser::CDepParser(const std::string &sFeatureDBPath,
                       bool bTrain,
                       bool bCoNLL)
  : CDepParserBase(sFeatureDBPath, bTrain, bCoNLL),
  lattice_(0),
  m_kBestTransitions(0),
  max_beam_size_(0),
  max_lattice_size_(0) {

  m_weights = new depparser :: CWeight(sFeatureDBPath, bTrain);
  m_kBestTransitions = new depparser::CScoredTransition[AGENDA_SIZE];
  m_nTrainingRound = 0;

  m_nTotalErrors = 0;

  max_beam_size_ = AGENDA_SIZE;
  if (bTrain) {
    m_nScoreIndex = CScore<depparser::SCORE_TYPE>::eNonAverage;
  } else {
    m_nScoreIndex = CScore<depparser::SCORE_TYPE>::eAverage;
  }
}

CDepParser::~CDepParser() {
  // delete m_Beam;
  delete m_weights;

  if (m_kBestTransitions) {
    delete[] m_kBestTransitions;
  }

  if (lattice_) {
    delete[] lattice_;
  }
}

/*---------------------------------------------------------------
 *
 * getOrUpdateStackScore - manipulate the score from stack
 *
 *---------------------------------------------------------------*/
inline void
CDepParser::GetOrUpdateStackScore(const CStateItem * item,
                                  CPackedScore& retval,
                                  const unsigned & action,
                                  SCORE_TYPE amount,
                                  int round) {

  const int & S0id = item->stacktop();
  const int & S0l1did = (S0id == -1 ? -1 : item->leftdep(S0id));
  const int & S0r1did = (S0id == -1 ? -1 : item->rightdep(S0id));
  const int & S0l2did = (S0id == -1 ? -1 : item->left2dep(S0id));
  const int & S0r2did = (S0id == -1 ? -1 : item->right2dep(S0id));
  const int & S1id = item->stack2top();
  const int & S1l1did = (S1id == -1 ? -1 : item->leftdep(S1id));
  const int & S1r1did = (S1id == -1 ? -1 : item->rightdep(S1id));
  const int & S1l2did = (S1id == -1 ? -1 : item->left2dep(S1id));
  const int & S1r2did = (S1id == -1 ? -1 : item->right2dep(S1id));
  const int & N0id = item->size() >= m_lCache.size() ? -1 : item->size();
  const int & N1id = item->size() + 1 >= m_lCache.size() ? -1 : item->size() + 1;

#define REF(prefix) const CTaggedWord<CTag, TAG_SEPARATOR> & prefix##wt = (\
    prefix##id == -1 ? g_emptyTaggedWord : m_lCache[prefix##id]);
#define REFw(prefix) const CWord & prefix##w = prefix##wt.word;
#define REFt(prefix) const CTag & prefix##t = prefix##wt.tag;
#define REFd(prefix) const int & prefix##d = (\
    prefix##id == -1 ? CDependencyLabel::NONE : item->label(prefix##id));

  REF(S0); REF(S0l1d); REF(S0r1d); REF(S0l2d); REF(S0r2d);
  REF(S1); REF(S1l1d); REF(S1r1d); REF(S1l2d); REF(S1r2d);
  REF(N0); REF(N1);

  REFw(S0); REFw(S0l1d); REFw(S0r1d); REFw(S0l2d); REFw(S0r2d);
  REFw(S1); REFw(S1l1d); REFw(S1r1d); REFw(S1l2d); REFw(S1r2d);
  REFw(N0); REFw(N1);

  REFt(S0); REFt(S0l1d); REFt(S0r1d); REFt(S0l2d); REFt(S0r2d);
  REFt(S1); REFt(S1l1d); REFt(S1r1d); REFt(S1l2d); REFt(S1r2d);
  REFt(N0); REFt(N1);

  REFd(S0l1d); REFd(S0r1d); REFd(S0l2d); REFd(S0r2d);
  REFd(S1l1d); REFd(S1r1d); REFd(S1l2d); REFd(S1r2d);

  const int S0ra = (S0id == -1 ? 0 : item->rightarity(S0id));
  const int S0la = (S0id == -1 ? 0 : item->leftarity(S0id));
  const int S1ra = (S1id == -1 ? 0 : item->rightarity(S1id));
  const int S1la = (S1id == -1 ? 0 : item->leftarity(S1id));

  const CSetOfTags<CDependencyLabel> &S0rset = (
      S0id == -1 ? CSetOfTags<CDependencyLabel>() : item->righttagset(S0id));
  const CSetOfTags<CDependencyLabel> &S0lset = (
      S0id == -1 ? CSetOfTags<CDependencyLabel>() : item->lefttagset(S0id));
  const CSetOfTags<CDependencyLabel> &S1rset = (
      S1id == -1 ? CSetOfTags<CDependencyLabel>() : item->righttagset(S1id));
  const CSetOfTags<CDependencyLabel> &S1lset = (
      S1id == -1 ? CSetOfTags<CDependencyLabel>() : item->lefttagset(S1id));

#define __GET_OR_UPDATE_SCORE(temp, feature) \
  cast_weights->temp.getOrUpdateScore(retval, feature, \
      action, m_nScoreIndex, amount, round);

  CTuple2<CWord, CWord>     ww;
  CTuple2<CWord, CTag>      wt;
  CTuple2<CTag, CTag>       tt;
  CTuple3<CTag, CTag, CTag> ttt;
  CTuple2<CWord, int>       wi;
  CTuple2<CTag, int>        ti;
  CTuple2<CWord, CSetOfTags<CDependencyLabel> > wset;
  CTuple2<CTag,  CSetOfTags<CDependencyLabel> > tset;

  // single
  if (S0id != -1) {
    __GET_OR_UPDATE_SCORE(S0w,    S0wt.word); //  0
    __GET_OR_UPDATE_SCORE(S0t,    S0wt.tag);  //  1
    __GET_OR_UPDATE_SCORE(S0wS0t, S0wt);      //  2
  }

  if (S1id != -1) {
    __GET_OR_UPDATE_SCORE(S1w,    S1wt.word); //  3
    __GET_OR_UPDATE_SCORE(S1t,    S1wt.tag);  //  4
    __GET_OR_UPDATE_SCORE(S1wS1t, S1wt);      //  5
  }

  if (N0id != -1) {
    __GET_OR_UPDATE_SCORE(N0w,    N0wt.word); //  6
    __GET_OR_UPDATE_SCORE(N0t,    N0wt.tag);  //  7
    __GET_OR_UPDATE_SCORE(N0wN0t, N0wt);      //  8
  }

  if (N1id != -1) {
    __GET_OR_UPDATE_SCORE(N1w,    N1wt.word); //  9
    __GET_OR_UPDATE_SCORE(N1t,    N1wt.tag);  //  10
    __GET_OR_UPDATE_SCORE(N1wN1t, N1wt);      //  11
  }

  if (-1 != S0id && -1 != S1id) {
    refer_or_allocate_tuple2(ww, &S0wt.word, &S1wt.word);
    __GET_OR_UPDATE_SCORE(S0wS1w, ww);        //  12

    refer_or_allocate_tuple2(wt, &S0wt.word, &S1wt.tag);
    __GET_OR_UPDATE_SCORE(S0wS1t, wt);        //  13

    refer_or_allocate_tuple2(wt, &S1wt.word, &S0wt.tag);
    __GET_OR_UPDATE_SCORE(S0tS1w, wt);        //  14

    refer_or_allocate_tuple2(tt, &S0wt.tag,  &S1wt.tag);
    __GET_OR_UPDATE_SCORE(S0tS1t, tt);        //  15
  }

  if (-1 != S0id && -1 != N0id) {
    refer_or_allocate_tuple2(ww, &S0wt.word, &N0wt.word);
    __GET_OR_UPDATE_SCORE(S0wN0w, ww);        //  16

    refer_or_allocate_tuple2(wt, &S0wt.word, &N0wt.tag);
    __GET_OR_UPDATE_SCORE(S0wN0t, wt);        //  17

    refer_or_allocate_tuple2(wt, &N0wt.word, &S0wt.tag);
    __GET_OR_UPDATE_SCORE(S0tN0w, wt);        //  18

    refer_or_allocate_tuple2(tt, &S0wt.tag,  &N0wt.tag);
    __GET_OR_UPDATE_SCORE(S0tN0t, tt);        //  19
  }

  if (-1 != S0l1did) {
    __GET_OR_UPDATE_SCORE(S0l1dw,       S0l1dwt.word);  //  20
    __GET_OR_UPDATE_SCORE(S0l1dt,       S0l1dwt.tag);   //  21
    __GET_OR_UPDATE_SCORE(S0l1dwS0l1dt, S0l1dwt);       //  22
    __GET_OR_UPDATE_SCORE(S0l1dd,       S0l1dd);        //  23
  }

  if (-1 != S0r1did) {
    __GET_OR_UPDATE_SCORE(S0r1dw,       S0r1dwt.word);  //  24
    __GET_OR_UPDATE_SCORE(S0r1dt,       S0r1dwt.tag);   //  25
    __GET_OR_UPDATE_SCORE(S0r1dwS0r1dt, S0r1dwt);       //  26
    __GET_OR_UPDATE_SCORE(S0r1dd,       S0r1dd);        //  27
  }

  if (-1 != S1l1did) {
    __GET_OR_UPDATE_SCORE(S1l1dw,       S1l1dwt.word);  //  28
    __GET_OR_UPDATE_SCORE(S1l1dt,       S1l1dwt.tag);   //  29
    __GET_OR_UPDATE_SCORE(S1l1dwS1l1dt, S1l1dwt);       //  30
    __GET_OR_UPDATE_SCORE(S1l1dd,       S1l1dd);        //  31
  }

  if (-1 != S1r1did) {
    __GET_OR_UPDATE_SCORE(S1r1dw,       S1r1dwt.word);  //  32
    __GET_OR_UPDATE_SCORE(S1r1dt,       S1r1dwt.tag);   //  33
    __GET_OR_UPDATE_SCORE(S1r1dwS1r1dt, S1r1dwt);       //  34
    __GET_OR_UPDATE_SCORE(S1r1dd,       S1r1dd);        //  35
  }

  if (-1 != S0l2did) {
    __GET_OR_UPDATE_SCORE(S0l2dw,       S0l2dwt.word);  //  36
    __GET_OR_UPDATE_SCORE(S0l2dt,       S0l2dwt.tag);   //  37
    __GET_OR_UPDATE_SCORE(S0l2dwS0l2dt, S0l2dwt);       //  38
    __GET_OR_UPDATE_SCORE(S0l2dd,       S0l2dd);        //  39

    refer_or_allocate_tuple3(ttt, &S0wt.tag, &S0l1dwt.tag, &S0l2dwt.tag);
    __GET_OR_UPDATE_SCORE(S0tS0l1dtS0l2dt, ttt);
  }

  if (-1 != S0r2did) {
    __GET_OR_UPDATE_SCORE(S0r2dw,       S0r2dwt.word);  //  40
    __GET_OR_UPDATE_SCORE(S0r2dt,       S0r2dwt.tag);   //  41
    __GET_OR_UPDATE_SCORE(S0r2dwS0r2dt, S0r2dwt);       //  42
    __GET_OR_UPDATE_SCORE(S0r2dd,       S0r2dd);        //  43

    refer_or_allocate_tuple3(ttt, &S0wt.tag, &S0r1dwt.tag, &S0r2dwt.tag);
    __GET_OR_UPDATE_SCORE(S0tS0r1dtS0r2dt, ttt);
  }

  if (-1 != S1l2did) {
    __GET_OR_UPDATE_SCORE(S1l2dw,       S1l2dwt.word);  //  44
    __GET_OR_UPDATE_SCORE(S1l2dt,       S1l2dwt.tag);   //  45
    __GET_OR_UPDATE_SCORE(S1l2dwS1l2dt, S1l2dwt);       //  46
    __GET_OR_UPDATE_SCORE(S1l2dd,       S1l2dd);        //  47

    refer_or_allocate_tuple3(ttt, &S1wt.tag, &S1l1dwt.tag, &S1l2dwt.tag);
    __GET_OR_UPDATE_SCORE(S1tS1l1dtS1l2dt, ttt);
  }

  if (-1 != S1r2did) {
    __GET_OR_UPDATE_SCORE(S1r2dw,       S1r2dwt.word);  //  48
    __GET_OR_UPDATE_SCORE(S1r2dt,       S1r2dwt.tag);   //  49
    __GET_OR_UPDATE_SCORE(S1r2dwS1r2dt, S1r2dwt);       //  50
    __GET_OR_UPDATE_SCORE(S1r2dd,       S1r2dd);        //  51

    refer_or_allocate_tuple3(ttt, &S1wt.tag, &S1r1dwt.tag, &S1r2dwt.tag);
    __GET_OR_UPDATE_SCORE(S1tS1r1dtS1r2dt, ttt);
  }

  if (-1 != S0id && -1 != S1id) {
    if (-1 != S0l1did) {
      refer_or_allocate_tuple3(ttt, &S0wt.tag, &S1wt.tag, &S0l1dwt.tag);
      __GET_OR_UPDATE_SCORE(S0tS1tS0l1dt, ttt);         //  52
    }

    if (-1 != S0l2did) {
       refer_or_allocate_tuple3(ttt, &S0wt.tag, &S1wt.tag, &S0l2dwt.tag);
       __GET_OR_UPDATE_SCORE(S0tS1tS0l2dt, ttt);         //  82
    }

    if (-1 != S0r1did) {
      refer_or_allocate_tuple3(ttt, &S0wt.tag, &S1wt.tag, &S0r1dwt.tag);
      __GET_OR_UPDATE_SCORE(S0tS1tS0r1dt, ttt);          //  53
    }

    if (-1 != S0r2did) {
      refer_or_allocate_tuple3(ttt, &S0wt.tag, &S1wt.tag, &S0r2dwt.tag);
      __GET_OR_UPDATE_SCORE(S0tS1tS0r2dt, ttt);          //  83
    }

    if (-1 != S1l1did) {
      refer_or_allocate_tuple3(ttt, &S0wt.tag, &S1wt.tag, &S1l1dwt.tag);
      __GET_OR_UPDATE_SCORE(S0tS1tS1l1dt, ttt);          //  54
    }

    if (-1 != S1l2did) {
      refer_or_allocate_tuple3(ttt, &S0wt.tag, &S1wt.tag, &S1l2dwt.tag);
      __GET_OR_UPDATE_SCORE(S0tS1tS1l2dt, ttt);          //  84
    }

    if (-1 != S1r1did) {
      refer_or_allocate_tuple3(ttt, &S0wt.tag, &S1wt.tag, &S1r1dwt.tag);
      __GET_OR_UPDATE_SCORE(S0tS1tS1r1dt, ttt);          //  55
    }

    if (-1 != S1r2did) {
      refer_or_allocate_tuple3(ttt, &S0wt.tag, &S1wt.tag, &S1r2dwt.tag);
      __GET_OR_UPDATE_SCORE(S0tS1tS1r2dt, ttt);          //  85
    }
  }

  if (-1 != S0id) {
    refer_or_allocate_tuple2(wi, &S0wt.word, &S0ra);
    __GET_OR_UPDATE_SCORE(S0wS0ra, wi);

    refer_or_allocate_tuple2(ti, &S0wt.tag, &S0ra);
    __GET_OR_UPDATE_SCORE(S0tS0ra, ti);

    refer_or_allocate_tuple2(wi, &S0wt.word, &S0la);
    __GET_OR_UPDATE_SCORE(S0wS0la, wi);

    refer_or_allocate_tuple2(ti, &S0wt.tag, &S0la);
    __GET_OR_UPDATE_SCORE(S0tS0la, ti);
  }

  if (-1 != S1id) {
    refer_or_allocate_tuple2(wi, &S1wt.word, &S1ra);
    __GET_OR_UPDATE_SCORE(S1wS1ra, wi);

    refer_or_allocate_tuple2(ti, &S1wt.tag, &S1ra);
    __GET_OR_UPDATE_SCORE(S1tS1ra, ti);

    refer_or_allocate_tuple2(wi, &S1wt.word, &S1la);
    __GET_OR_UPDATE_SCORE(S1wS1la, wi);

    refer_or_allocate_tuple2(ti, &S1wt.tag, &S1la);
    __GET_OR_UPDATE_SCORE(S1tS1la, ti);
  }

  if (-1 != S0id) {
    refer_or_allocate_tuple2(wi, &S0wt.word, &S0ra);
    __GET_OR_UPDATE_SCORE(S0wS0ra, wi);

    refer_or_allocate_tuple2(ti, &S0wt.tag, &S0ra);
    __GET_OR_UPDATE_SCORE(S0tS0ra, ti);

    refer_or_allocate_tuple2(wi, &S0wt.word, &S0la);
    __GET_OR_UPDATE_SCORE(S0wS0la, wi);

    refer_or_allocate_tuple2(ti, &S0wt.tag, &S0la);
    __GET_OR_UPDATE_SCORE(S0tS0la, ti);
  }

  if (-1 != S1id) {
    refer_or_allocate_tuple2(wi, &S1wt.word, &S1ra);
    __GET_OR_UPDATE_SCORE(S1wS1ra, wi);

    refer_or_allocate_tuple2(ti, &S1wt.tag, &S1ra);
    __GET_OR_UPDATE_SCORE(S1tS1ra, ti);

    refer_or_allocate_tuple2(wi, &S1wt.word, &S1la);
    __GET_OR_UPDATE_SCORE(S1wS1la, wi);

    refer_or_allocate_tuple2(ti, &S1wt.tag, &S1la);
    __GET_OR_UPDATE_SCORE(S1tS1la, ti);
  }

  if (-1 != S0id) {
    refer_or_allocate_tuple2(wset, &S0wt.word, &S0lset);
    __GET_OR_UPDATE_SCORE(S0wS0lset, wset);

    refer_or_allocate_tuple2(tset, &S0wt.tag, &S0lset);
    __GET_OR_UPDATE_SCORE(S0tS0lset, tset);

    refer_or_allocate_tuple2(wset, &S0wt.word, &S0rset);
    __GET_OR_UPDATE_SCORE(S0wS0rset, wset);

    refer_or_allocate_tuple2(tset, &S0wt.tag, &S0rset);
    __GET_OR_UPDATE_SCORE(S0tS0rset, tset);
  }

  if (-1 != S1id) {
    refer_or_allocate_tuple2(wset, &S1wt.word, &S1lset);
    __GET_OR_UPDATE_SCORE(S1wS1lset, wset);

    refer_or_allocate_tuple2(tset, &S1wt.tag, &S1lset);
    __GET_OR_UPDATE_SCORE(S1tS1lset, tset);

    refer_or_allocate_tuple2(wset, &S1wt.word, &S1rset);
    __GET_OR_UPDATE_SCORE(S1wS1rset, wset);

    refer_or_allocate_tuple2(tset, &S1wt.tag, &S1rset);
    __GET_OR_UPDATE_SCORE(S1tS1rset, tset);
  }


  if (-1 != S0id && -1 != S1id) {
    int dist = encodeLinkDistance(S1id, S0id);
    refer_or_allocate_tuple2(wi, &S0wt.word, &dist);
    __GET_OR_UPDATE_SCORE(S0wDist, wi);

    refer_or_allocate_tuple2(ti, &S0wt.tag, &dist);
    __GET_OR_UPDATE_SCORE(S0tDist, ti);

    refer_or_allocate_tuple2(wi, &S1wt.word, &dist);
    __GET_OR_UPDATE_SCORE(S1wDist, wi);

    refer_or_allocate_tuple2(ti, &S1wt.tag, &dist);
    __GET_OR_UPDATE_SCORE(S1tDist, ti);
  }
}

/*---------------------------------------------------------------
 *
 * updateScoresForStates - update scores for states
 *
 *--------------------------------------------------------------*/
void
CDepParser::UpdateScoresForStates(const CStateItem* predicated_state,
    const CStateItem* correct_state,
    SCORE_TYPE amount_add,
    SCORE_TYPE amount_subtract) {
  static CPackedScore empty;
  // do not update those steps where they are correct

  const CStateItem * predicated_state_chain[kMaxSentenceSize * 2];
  const CStateItem * correct_state_chain[kMaxSentenceSize * 2];

  int num_predicated_states = 0;
  int num_correct_states = 0;
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
    unsigned predicated_action = predicated_state_chain[i]->last_action;
    unsigned correct_action = correct_state_chain[i]->last_action;
    // std::cout << correct_action << " " << predicated_action << std::endl;
    if (predicated_action != correct_action) {
      break;
    }
  }

  // for (int j = i + 1; j > 0; -- j) { std::cout << (*predicated_state_chain[j]); }
  // for (int j = i + 1; j > 0; -- j) { std::cout << (*correct_state_chain[j]); }
  for (i = i + 1; i > 0; -- i) {
    unsigned predicated_action = predicated_state_chain[i - 1]->last_action;
    unsigned correct_action = correct_state_chain[i - 1]->last_action;
    GetOrUpdateStackScore(predicated_state_chain[i],
        empty, predicated_action, amount_subtract, m_nTrainingRound);
    GetOrUpdateStackScore(correct_state_chain[i],
        empty, correct_action, amount_add, m_nTrainingRound);
  }
  m_nTotalErrors++;
}

/*---------------------------------------------------------------
 *
 * arcleft - helping function
 *
 *--------------------------------------------------------------*/
inline void
CDepParser::arcleft(const CStateItem *item,
                    const CPackedScore& scores) {
  static CScoredTransition transition;
  static unsigned label;

#ifdef LABELED
  for (label=CDependencyLabel::FIRST; label<CDependencyLabel::COUNT; ++label) {
    transition.source = item;
    transition.action = action::EncodeAction(action::kArcLeft, label);
    transition.score = item->score + scores[transition.action];
    InsertIntoBeam(transition);
  }
#else
  transition.source = item;
  transition.action = action::kArcLeft;
  transition.score = item->score + scores[transition.action];
  InsertIntoBeam(transition);
#endif
}

/*---------------------------------------------------------------
 *
 * arcright - helping function
 *
 *--------------------------------------------------------------*/
inline void
CDepParser::arcright(const CStateItem * item,
                     const CPackedScore& scores) {
  static CScoredTransition transition;
  static unsigned label;

#ifdef LABELED
  for (label=CDependencyLabel::FIRST; label<CDependencyLabel::COUNT; ++label) {
    transition.source = item;
    transition.action = action::EncodeAction(action::kArcRight, label);
    transition.score  = item->score + scores[transition.action];
    InsertIntoBeam(transition);
  }
#else
  transition.source = item;
  transition.action = action::kArcRight;
  transition.score = item->score + scores[transition.action];
  InsertIntoBeam(transition);
#endif
}

/*---------------------------------------------------------------
 *
 * shift - help function
 *
 *--------------------------------------------------------------*/
inline void
CDepParser::shift(const CStateItem * item,
                  const CPackedScore& scores) {
  static CScoredTransition transition;
  // update stack score
  transition.source = item;
  transition.action = action::kShift;
  transition.score = item->score + scores[transition.action];
  InsertIntoBeam(transition);
}

/*---------------------------------------------------------------
 *
 * poproot - help function
 *
 *--------------------------------------------------------------*/

inline void
CDepParser::poproot(const CStateItem *item,
                    const CPackedScore& scores) {
  static CScoredTransition transition;
  // update stack score
  transition.source = item;
  transition.action = action::kPopRoot;
  transition.score = item->score + scores[transition.action];
  InsertIntoBeam(transition);
}

bool
ScoredTransitionMore(const CScoredTransition& x, const CScoredTransition& y) {
  return x.score > y.score;
}

bool
StateMore(const CStateItem& x, const CStateItem& y) {
  return x.score > y.score;
}

int
CDepParser::InsertIntoBeam(const CScoredTransition & transition) {
  if (current_beam_size_ == max_beam_size_) {
    if (transition.score > m_kBestTransitions[0].score) {
      std::pop_heap(m_kBestTransitions, m_kBestTransitions + max_beam_size_,
          ScoredTransitionMore);
      m_kBestTransitions[max_beam_size_- 1] = transition;
      std::push_heap(m_kBestTransitions, m_kBestTransitions+ max_beam_size_,
          ScoredTransitionMore);
    }
    return 0;
  }

  m_kBestTransitions[current_beam_size_] = transition;
  std::push_heap(m_kBestTransitions, m_kBestTransitions+ current_beam_size_+ 1,
      ScoredTransitionMore);
  ++ current_beam_size_;
  return 1;
}

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

  for (int i = 0; i < max_lattice_size; ++ i) { lattice_[i].clear(); }
  return lattice_;
}

void
CDepParser::Transit(const CStateItem * item,
                    const CPackedScore & scores) {
  int L = m_lCache.size();
  if (item->terminated()) {
    return;
  }

  if (item->stacksize() == 0 && item->size() < L) {
    shift(item, scores);
  } else if (item->stacksize() == 1) {
    if (item->size() == L) {
      poproot(item, scores);
    } else if (item->size() < L) {
      shift(item, scores);
    }
  } else {
    if (item->size() < L) { shift(item, scores); }
    arcleft(item, scores);
    arcright(item, scores);
  }
}

/*---------------------------------------------------------------
 *
 * work - the working process shared by training and parsing
 *
 * Returns: makes a new instance of CDependencyParse
 *
 *--------------------------------------------------------------*/
int
CDepParser::work(const bool is_train,
                 const CTwoStringVector & sentence,
                 CDependencyParse * retval,
                 const CDependencyParse & oracle_tree,
                 int nbest,
                 SCORE_TYPE *scores) {
#ifdef DEBUG
  clock_t total_start_time = clock();
#endif

  const int length = sentence.size();
  const int max_round = length * 2 + 1;
  const int max_lattice_size = (kAgendaSize + 1) * max_round;

  ASSERT(length < MAX_SENTENCE_SIZE,
         "The size of sentence is too long.");

  CStateItem * lattice = GetLattice(max_lattice_size);
  CStateItem * lattice_index[max_round];
  CStateItem * correct_state = lattice;

  for (int i = 0; i < max_lattice_size; ++ i) {
    lattice[i].len_ = length;
  }

  lattice[0].clear();
  correct_state = lattice;
  lattice_index[0] = lattice;
  lattice_index[1] = lattice_index[0] + 1;

  static CPackedScore packed_scores;
  TRACE("Initialising the decoding process ...");

  m_lCache.clear();
  for (int i = 0; i < length; ++ i) {
    m_lCache.push_back(CTaggedWord<CTag, TAG_SEPARATOR>(sentence[i].first,
                                                        sentence[i].second));
#ifdef LABELED
    if (is_train) {
      if (i == 0) { m_lCacheLabel.clear(); }
      m_lCacheLabel.push_back(CDependencyLabel(oracle_tree[i].label));
    }
#endif
  }

  int num_results = 0;
  int round = 0;
  bool is_correct; // used for training to specify correct state in lattice

  // loop with the next word to process in the sentence, 'round' represent the
  // generators, and the condidates should be inserted into the 'round + 1'
  for (round = 1; round < max_round; ++ round) {
    if (lattice_index[round - 1] == lattice_index[round]) {
      // There is nothing in generators, the proning has cut all legel
      // generator. Actually, in this kind of case, we should raise a
      // exception. However to achieve a parsing tree, an alternative
      // solution is go back to the previous round
      WARNING("Parsing Failed!");
      -- round;
      break;
    }

    current_beam_size_ = 0;
    // loop over the generator states
    // std::cout << "round : " << round << std::endl;
    for (CStateItem * q = lattice_index[round - 1]; q != lattice_index[round];
        ++ q) {
      const CStateItem * generator = q;
      packed_scores.reset();
      GetOrUpdateStackScore(generator, packed_scores, action::kNoAction);
      Transit(generator, packed_scores);
    }

    for (unsigned i = 0; i < current_beam_size_; ++ i) {
      const CScoredTransition& transition = m_kBestTransitions[i];
      CStateItem* target = lattice_index[round]+ i;
      (*target) = (*transition.source);
      // generate candidate state according to the states in beam
      target->Move(transition.action);
      target->score = transition.score;
      target->previous_ = transition.source;
    }

    lattice_index[round + 1] = lattice_index[round] + current_beam_size_;

    if (is_train) {
      CStateItem next_correct_state(*correct_state);
      next_correct_state.StandardMoveStep(oracle_tree
#ifdef LABELED
          , m_lCacheLabel
#endif // end for LABELED
          );

      next_correct_state.previous_ = correct_state;
      is_correct = false;

      for (CStateItem *p = lattice_index[round]; p != lattice_index[round + 1];
          ++ p) {
        if (next_correct_state.last_action == p->last_action
            && p->previous_ == correct_state) {
          correct_state = p;
          is_correct = true;
          break;
        }
      }

#ifdef EARLY_UPDATE
      if (!is_correct) {
        TRACE("ERROR at the " << next_correct_state.size() << "th word;"
              << " Total is " << oracle_tree.size());

        CStateItem * best_generator = lattice_index[round];
        for (CStateItem * p = lattice_index[round]; p != lattice_index[round + 1];
            ++ p) {
          if (best_generator->score < p->score) {
            best_generator = p;
          }
        }
        UpdateScoresForStates(best_generator, &next_correct_state, 1, -1);
        return -1;
      }
#endif // end for EARLY_UPDATE
    }
  }

  if (is_train) {
    CStateItem * best_generator = lattice_index[round-1];

    for (CStateItem * p = lattice_index[round-1]; p != lattice_index[round]; ++ p) {
      if (best_generator->score < p->score) {
        best_generator = p;
      }
    }
    if (best_generator != correct_state) {
      UpdateScoresForStates(best_generator, correct_state, 1, -1);
    }
    return -1;
  }

  if (!retval) {
    return -1;
  }

  TRACE("Output sentence");
  std::sort(lattice_index[round - 1], lattice_index[round], StateMore);
  num_results = lattice_index[round] - lattice_index[round - 1];

  for (int i = 0; i < std::min(num_results, nbest); ++ i) {
    assert( (lattice_index[round - 1] + i)->size() == m_lCache.size());
    (lattice_index[round - 1] + i)->GenerateTree(sentence, retval[i]);
    if (scores) {
      scores[i] = (lattice_index[round - 1] + i)->score;
    }
  }
  TRACE("Done, total time spent: " << double(clock() - total_start_time) / CLOCKS_PER_SEC);
  return num_results;
}

/*---------------------------------------------------------------
 *
 * parse - do dependency parsing to a sentence
 *
 * Returns: makes a new instance of CDependencyParse
 *
 *--------------------------------------------------------------*/
void
CDepParser::parse(const CTwoStringVector & sentence,
                  CDependencyParse * retval,
                  int nBest,
                  SCORE_TYPE *scores) {
  static CDependencyParse empty;
  assert(!m_bCoNLL);

  for (int i=0; i<nBest; ++i) {
    // clear the output sentences
    retval[i].clear();
    if (scores) scores[i] = 0; //pGenerator->score;
  }

  work(false, sentence, retval, empty, nBest, scores);
}

/*---------------------------------------------------------------
 *
 * train - train the models with an example
 *
 *---------------------------------------------------------------*/
void
CDepParser::train(const CDependencyParse &correct , int round) {
  static CTwoStringVector sentence;
  static CDependencyParse output;

  assert(!m_bCoNLL);
  assert(IsProjectiveDependencyTree(correct));
  UnparseSentence(&correct, &sentence);
  // The following code does update for each processing stage
  ++m_nTrainingRound;
  ASSERT(m_nTrainingRound == round, "Training round error");
  work(true, sentence, NULL, correct , 1 , 0);
};

/*---------------------------------------------------------------
 *
 * extract_features - extract features from an example (counts
 * recorded to parser model as weights)
 *
 *---------------------------------------------------------------*/
void
CDepParser::extract_features(const CDependencyParse &input) {
  CStateItem item;
  unsigned action;
  CPackedScoreType<SCORE_TYPE, action::kMax> empty;

  // word and pos
  m_lCache.clear();
#ifdef LABELED
  m_lCacheLabel.clear();
#endif

  for (int i = 0; i < input.size(); ++ i) {
    m_lCache.push_back(CTaggedWord<CTag, TAG_SEPARATOR>(input[i].word,
                                                        input[i].tag));

#ifdef LABELED
    m_lCacheLabel.push_back(CDependencyLabel(input[i].label));
#endif
  }

  // make standard item
  item.clear(); item.len_ = input.size();
  for (int i = 0; i < input.size() * 2; ++ i) {
    unsigned action = item.StandardMove(input
#ifdef LABELED
        , m_lCacheLabel
#endif
        );

    GetOrUpdateStackScore(&item, empty, action, 1, 1);
    item.Move(action);
  }
}

/*---------------------------------------------------------------
 *
 * initCoNLLCache
 *
 *---------------------------------------------------------------*/
template<typename CCoNLLInputOrOutput>
void CDepParser::initCoNLLCache(const CCoNLLInputOrOutput &sentence) {
  m_lCacheCoNLLLemma.resize(sentence.size());
  m_lCacheCoNLLCPOS.resize(sentence.size());
  m_lCacheCoNLLFeats.resize(sentence.size());

  for (unsigned i = 0; i < sentence.size(); ++ i) {
    m_lCacheCoNLLLemma[i].load(_conll_or_empty(sentence.at(i).lemma));
    m_lCacheCoNLLCPOS[i].load(_conll_or_empty(sentence.at(i).ctag));
    m_lCacheCoNLLFeats[i].clear();
    if (sentence.at(i).feats != "_")
      readCoNLLFeats(m_lCacheCoNLLFeats[i], sentence.at(i).feats);
  }
}

/*---------------------------------------------------------------
 *
 * parse_conll - do dependency parsing to a sentence
 *
 * Returns: makes a new instance of CDependencyParse
 *
 *--------------------------------------------------------------*/
void
CDepParser::parse_conll(const CCoNLLInput & sentence,
                        CCoNLLOutput * retval,
                        int nBest,
                        SCORE_TYPE *scores) {

  static CDependencyParse empty;
  static CTwoStringVector input;
  static CDependencyParse output[AGENDA_SIZE];

  assert(m_bCoNLL);

  initCoNLLCache(sentence);
  sentence.toTwoStringVector(input);

  for (int i=0; i<nBest; ++i) {
    // clear the output sentences
    retval[i].clear();
    output[i].clear();
    if (scores) scores[i] = 0; //pGenerator->score;
  }

  int nr_results = work(false, input, output, empty, nBest, scores);

  for (int i=0; i<std::min(nBest, nr_results); ++i) {
    // now make the conll format stype output
    retval[i].fromCoNLLInput(sentence);
    retval[i].copyDependencyHeads(output[i]);
  }
}

/*---------------------------------------------------------------
 *
 * train_conll - train the models with an example
 *
 *---------------------------------------------------------------*/
void
CDepParser::train_conll(const CCoNLLOutput &correct,
                        int round) {

  static CTwoStringVector sentence;
  static CDependencyParse output;
  static CDependencyParse reference;

  assert(m_bCoNLL);

//todo: is this ifndef correct?
#ifndef FRAGMENTED_TREE
  assert(IsProjectiveDependencyTree(correct));
#endif

  initCoNLLCache(correct);

  correct.toDependencyTree(reference);
  UnparseSentence(&reference, &sentence);

  // The following code does update for each processing stage
  m_nTrainingRound = round;
  work(true , sentence , &output , reference , 1 , 0);
}

/*---------------------------------------------------------------
 *
 * extract_features_conll - extract features from an example
 *
 *---------------------------------------------------------------*/

void
CDepParser::extract_features_conll(const CCoNLLOutput &input) {
  CDependencyParse dep;
  assert(m_bCoNLL);
  initCoNLLCache(input);
  input.toDependencyTree(dep);
  extract_features(dep);
}
