#ifndef COMMON_DEPPARSER_IMPLEMENTATIONS_CHOIBASIC_ACTION_H
#define COMMON_DEPPARSER_IMPLEMENTATIONS_CHOIBASIC_ACTION_H

namespace action {

const int kNumDependencyLabels = DEP_LABEL_COUNT;

enum ActionType {
  kNoAction = 0,
  kIdle,
  kShift,
  kReduce,
  kArcLeft,
  kArcRight,
  kPopRoot,
  kNoPass,
  kLeftPass,
  kRightPass,
  kLabeledALFirst,
  kLabeledARFirst = kLabeledALFirst + kNumDependencyLabels - 1,
  kLabeledLPFirst = kLabeledARFirst + kNumDependencyLabels - 1,
  kLabeledRPFirst = kLabeledLPFirst + kNumDependencyLabels - 1,
  kMax            = kLabeledRPFirst + kNumDependencyLabels - 1,
};

/**
 * use base action an label to generate global action value
 *
 *  @param[in]  action  the basic action
 *  @param[in]  label   the label, default equals 0
 *  @return     the encoded action
 */
static unsigned EncodeAction(
    const ActionType & action,
    const unsigned   & label = 0) {

  assert(label < kNumDependencyLabels);

  switch (action) {
    case kArcLeft: {
      return (0 == label) ? kArcLeft  : kLabeledALFirst + label - 1;
    }
    case kArcRight: {
      return (0 == label) ? kArcRight : kLabeledARFirst + label - 1;
    }
    case kLeftPass: {
      return (0 == label) ? kLeftPass : kLabeledLPFirst + label - 1;
    }
    case kRightPass: {
      return (0 == label) ? kRightPass: kLabeledRPFirst + label - 1;
    }
    default: {
      assert(label == 0);
    }
  }

  return action;
}

/**
 * Decode the action to get the unlabled action
 *
 *  @param[in]  action    the labeled/unlabeled action
 *  @return     the unlabeled action
 */
static unsigned DecodeUnlabeledAction(
    const unsigned & action) {
  assert(action < kMax);

  if (action < kLabeledALFirst)       { return action;      }
  else if (action < kLabeledARFirst)  { return kArcLeft;    }
  else if (action < kLabeledLPFirst)  { return kArcRight;   }
  else if (action < kLabeledRPFirst)  { return kLeftPass;   }
  else                                { return kRightPass;  }
}

/**
 * Decode the action to get the label
 *
 *  @param[in]  action    the labeled/unlabeled action
 *  @return     the label
 */
static unsigned DecodeLabel(
    const unsigned &action) {

  assert(action < kMax);

  if (action < kLabeledALFirst)      { return 0; }
  else if (action < kLabeledARFirst) { return action - kLabeledALFirst + 1;}
  else if (action < kLabeledLPFirst) { return action - kLabeledARFirst + 1;}
  else if (action < kLabeledRPFirst) { return action - kLabeledLPFirst + 1;}
  else                               { return action - kLabeledRPFirst + 1;}
}

}       //  end for namespace action

#endif  //  end for COMMON_DEPPARSER_IMPLEMENTATIONS_CHOIBASIC_ACTION_H
