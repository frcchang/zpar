// The action defination

#ifndef DEPPARSER_ARC_STANDARD_ACTION_H
#define DEPPARSER_ARC_STANDARD_ACTION_H

namespace action {

// SH RE AL AR PR [AL+LABEL] AR+LABEL
enum StackActions {
  kNoAction = 0,
  kShift,
  kArcLeft,
  kArcRight,
  kPopRoot,
  kLabeledALFirst,
  kLabeledARFirst = kLabeledALFirst + DEP_LABEL_COUNT - 1,
  kMax = kLabeledARFirst + DEP_LABEL_COUNT - 1,
};

static unsigned
EncodeAction(const StackActions & action,
             const unsigned & label = 0) {
  assert(label < DEP_LABEL_COUNT);

  if (action == kArcLeft) {
    if (label == 0) {
      return kArcLeft;
    } else {
      return kLabeledALFirst + label - 1;
    }
  }
  else if (action == kArcRight) {
    if (label == 0) {
      return kArcRight;
    } else {
      return kLabeledARFirst + label - 1;
    }
  }
  else {
    assert(label==0);
  }

  return action;
}

static unsigned
DecodeUnlabeledAction(const unsigned & action) {
  assert(action < kMax);

  if (action < kLabeledALFirst) {
    return action;
  }
  else if (action<kLabeledARFirst) {
    return kArcLeft;
  } 
  else {
    return kArcRight;
  }
}

static unsigned
DecodeLabel(const unsigned & action) {
  assert(action < kMax);

  if (action < kLabeledALFirst) {
    return 0;
  }
  else if (action < kLabeledARFirst) {
    return action - kLabeledALFirst + 1;
  }
  else {
    return action - kLabeledARFirst + 1;
  }
}

struct CScoredAction {
  unsigned action;
  SCORE_TYPE score;
public:
  bool operator < (const CScoredAction &a) const {
    return score < a.score;
  }

  bool operator > (const CScoredAction &a) const {
    return score > a.score;
  }
};

}//namespace

#endif  //  end for DEPPARSER_ARC_STANDARD_ACTION_H
