// Copyright (C) University of Oxford 2010
#ifndef DEPPARSER_ARC_STANDARD_STATE_H
#define DEPPARSER_ARC_STANDARD_STATE_H

class CStateItem {
protected:
  std::vector<int> m_Stack;
  // stack of words that are currently processed

  int m_nNextWord;
  // index for the next word

  int m_lHeads[MAX_SENTENCE_SIZE];
  // the lexical head for each word

  int m_lDepsL[MAX_SENTENCE_SIZE];
  // the leftmost dependency for each word (just for cache, temporary info)

  int m_lDepsR[MAX_SENTENCE_SIZE];
  // the rightmost dependency for each word (just for cache, temporary info)

  int m_lDepsL2[MAX_SENTENCE_SIZE];
  // the second-leftmost dependency for each word

  int m_lDepsR2[MAX_SENTENCE_SIZE];
  // the second-rightmost dependency for each word

  int m_lDepNumL[MAX_SENTENCE_SIZE];
  // the number of left dependencies

  int m_lDepNumR[MAX_SENTENCE_SIZE];
  // the number of right dependencies

  CSetOfTags<CDependencyLabel> m_lDepTagL[MAX_SENTENCE_SIZE];
  // the set of left tags

  CSetOfTags<CDependencyLabel> m_lDepTagR[MAX_SENTENCE_SIZE];
  // the set of right tags

#ifdef LABELED
  unsigned long m_lLabels[MAX_SENTENCE_SIZE];
  unsigned long m_lConstituents[MAX_SENTENCE_SIZE];   // the highest constituents at each node
  // the label of each dependency link
#endif

public:
  const std::vector < CTaggedWord<CTag, TAG_SEPARATOR> >* m_lCache;

public:
  SCORE_TYPE score;
  // score of stack - predicting how potentially this is the correct one

  int len_;
  // the length of the sentence, it's set manually.

  const CStateItem * previous_;
  // Previous state of the current state

  unsigned long last_action;
  // the last stack action

public:
  // constructors and destructor
  CStateItem() {
    clear();
  }

  ~CStateItem() { }
public:
  // comparison
  inline bool operator < (const CStateItem &item) const {
    return score < item.score;
  }

  inline bool operator > (const CStateItem &item) const {
    return score > item.score;
  }

  inline bool operator == (const CStateItem &item) const {
    THROW("equal operator should not be used");
  }

  inline bool operator != (const CStateItem &item) const {
    return ! ((*this)==item);
  }

  // propty
  inline int stacksize() const {
    return m_Stack.size();
  }

  inline bool stackempty() const {
    return m_Stack.empty();
  }

  inline int stacktop() const {
    if (m_Stack.empty()) { return -1; }
    return m_Stack.back();
  }

  inline int stack2top() const {
    if (m_Stack.size() < 2) { return -1; }
    return m_Stack[m_Stack.size() - 2];
  }

  inline int stack3top() const {
    if (m_Stack.size() < 3) { return -1; }
    return m_Stack[m_Stack.size() - 3];
  }

  inline int stack4top() const {
    if (m_Stack.size() < 4) { return -1; }
    return m_Stack[m_Stack.size() - 4];
  }

  inline int stackbottom() const {
    assert(!m_Stack.empty());
    return m_Stack.front();
  }

  inline int stackitem(const int & id) const {
    assert (id < m_Stack.size());
    return m_Stack[id];
  }

  inline int head(const int & id) const {
    assert (id < m_nNextWord);
    return m_lHeads[id];
  }

  inline int leftdep(const int & id) const {
    assert(id < m_nNextWord);
    return m_lDepsL[id];
  }

  inline int rightdep(const int & id) const {
    assert(id < m_nNextWord);
    return m_lDepsR[id];
  }

  inline int left2dep(const int & id) const {
    assert(id < m_nNextWord);
    return m_lDepsL2[id];
  }

  inline int right2dep(const int & id) const {
    assert(id < m_nNextWord);
    return m_lDepsR2[id];
  }

  inline int size() const {
    return m_nNextWord;
  }

  inline bool terminated() const {
    return (last_action == action::kPopRoot
            && m_Stack.empty()
            && m_nNextWord == len_);
  }

  inline bool complete() const {
    return (m_Stack.size() == 1
            && m_nNextWord == len_);
  }
#ifdef LABELED
  inline int label(const int & id) const {
    assert(id < m_nNextWord);
    return m_lLabels[id];
  }  
  inline const unsigned long &constituent( const int &index ) const { assert(index<=m_nNextWord); return m_lConstituents[index]; }
#endif

  inline int leftarity(const int & id) const {
    assert(id < m_nNextWord);
    return m_lDepNumL[id];
  }

  inline int rightarity(const int & id) const {
    assert(id < m_nNextWord);
    return m_lDepNumR[id];
  }

  inline const CSetOfTags<CDependencyLabel> &lefttagset(const int &id) const {
    assert(id < m_nNextWord);
    return m_lDepTagL[id];
  }

  inline const CSetOfTags<CDependencyLabel> &righttagset(const int &id) const {
    assert(id < m_nNextWord);
    return m_lDepTagR[id];
  }

  void clear() {
    m_nNextWord = 0;
    m_Stack.clear();
    score = 0;
    previous_ = 0;
    last_action = action::kNoAction;
    ClearNext();
  }

  void operator = (const CStateItem &item) {
    m_Stack = item.m_Stack;
    m_nNextWord = item.m_nNextWord;
    
    m_lCache = item.m_lCache;

    last_action = item.last_action;
    score       = item.score;
    len_        = item.len_;
    previous_   = item.previous_;

    for (int i = 0; i <= m_nNextWord; ++ i) { // only copy active word (including m_nNext)
      m_lHeads[i] = item.m_lHeads[i];
      m_lDepsL[i] = item.m_lDepsL[i];
      m_lDepsR[i] = item.m_lDepsR[i];
      m_lDepsL2[i] = item.m_lDepsL2[i];
      m_lDepsR2[i] = item.m_lDepsR2[i];
      m_lDepNumL[i] = item.m_lDepNumL[i];
      m_lDepNumR[i] = item.m_lDepNumR[i];
#ifdef LABELED
      m_lLabels[i] = item.m_lLabels[i];
      m_lConstituents[i] = item.m_lConstituents[i];
      m_lDepTagL[i] = item.m_lDepTagL[i];
      m_lDepTagR[i] = item.m_lDepTagR[i];
#endif
    }
  }

//-----------------------------------------------------------------------------
public:
  // Perform Arc-Left operation in the arc-standard algorithm
  void ArcLeft(
#ifdef LABELED
      unsigned long lab
#endif
      ) {
    // At least, there must be two elements in the stack.
    assert(m_Stack.size() > 1);
    assert(m_lHeads[m_Stack.back()] == DEPENDENCY_LINK_NO_HEAD);

    int stack_size = m_Stack.size();
    int top0 = m_Stack[stack_size - 1];
    int top1 = m_Stack[stack_size - 2];

    m_Stack.pop_back();
    m_Stack.back() = top0;

    m_lHeads[top1] = top0;
    m_lDepNumL[top0] ++;


#ifdef LABELED
    m_lLabels[top1] = lab;
    transfer((*m_lCache)[top0].tag.code(), (*m_lCache)[top1].tag.code(), lab,m_lConstituents[top0], false, m_lConstituents[top0]);
    m_lDepTagL[top0].add(lab);
#endif

    if (m_lDepsL[top0] == DEPENDENCY_LINK_NO_HEAD) {
      m_lDepsL[top0] = top1;
    } else if (top1 < m_lDepsL[top0]) {
      m_lDepsL2[top0] = m_lDepsL[top0];
      m_lDepsL[top0] = top1;
    } else if (top1 < m_lDepsL2[top0]) {
      m_lDepsL2[top0] = top1;
    }

    last_action = action::EncodeAction(action::kArcLeft
#ifdef LABELED
        , lab
#endif
        );
  }

  // Perform the arc-right operation in arc-standard
  void ArcRight(
#ifdef LABELED
      unsigned long lab
#endif
      ) {
    assert(m_Stack.size() > 1);

    int stack_size = m_Stack.size();
    int top0 = m_Stack[stack_size - 1];
    int top1 = m_Stack[stack_size - 2];

    m_Stack.pop_back();
    m_lHeads[top0] = top1;
    m_lDepNumR[top1] ++;

#ifdef LABELED
    m_lLabels[top0] = lab;
    transfer((*m_lCache)[top1].tag.code(), (*m_lCache)[top0].tag.code(), lab,m_lConstituents[top1], false, m_lConstituents[top1]);
    m_lDepTagR[top1].add(lab);
#endif

    if (m_lDepsR[top1] == DEPENDENCY_LINK_NO_HEAD) {
      m_lDepsR[top1] = top0;
    } else if (m_lDepsR[top1] < top0) {
      m_lDepsR2[top1] = m_lDepsR[top1];
      m_lDepsR[top1] = top0;
    } else if (m_lDepsR2[top1] < top0) {
      m_lDepsR2[top1] = top0;
    }

    last_action = action::EncodeAction(action::kArcRight
#ifdef LABELED
        , lab
#endif
        );
  }

  // the shift action does pushing
  void Shift() {      
    m_Stack.push_back(m_nNextWord);
#ifdef LABELED
      m_lConstituents[m_nNextWord] = (((*m_lCache)[m_nNextWord].tag.code()) | (1<<std::max(PENN_TAG_COUNT_BITS, PENN_CON_COUNT_BITS)));
#endif
    m_nNextWord ++;
    ClearNext();
    last_action = action::EncodeAction(action::kShift);
  }

  // this is used for the convenience of scoring and updating
  void PopRoot() {
    assert(m_Stack.size() == 1
           && m_lHeads[m_Stack.back()] == DEPENDENCY_LINK_NO_HEAD);
    // make sure only one root item in stack

#ifdef LABELED
    m_lLabels[m_Stack.back()] = CDependencyLabel::ROOT;
#endif

    last_action = action::EncodeAction(action::kPopRoot);
    m_Stack.pop_back(); // pop it
  }

  // the clear next action is used to clear the next word, used 
  // with forwarding the next word index
  void ClearNext() {
    m_lHeads[m_nNextWord]   = DEPENDENCY_LINK_NO_HEAD;
    m_lDepsL[m_nNextWord]   = DEPENDENCY_LINK_NO_HEAD;
    m_lDepsL2[m_nNextWord]  = DEPENDENCY_LINK_NO_HEAD;
    m_lDepsR[m_nNextWord]   = DEPENDENCY_LINK_NO_HEAD;
    m_lDepsR2[m_nNextWord]  = DEPENDENCY_LINK_NO_HEAD;
    m_lDepNumL[m_nNextWord] = 0;
    m_lDepNumR[m_nNextWord] = 0;
    m_lDepTagL[m_nNextWord].clear();
    m_lDepTagR[m_nNextWord].clear();

#ifdef LABELED
    m_lLabels[m_nNextWord] = CDependencyLabel::NONE;
    m_lConstituents[m_nNextWord] = 0;
#endif
  }

  // the move action is a simple call to do action according to the action code
  void Move (const unsigned long &ac) {
    switch (action::DecodeUnlabeledAction(ac)) {
      case action::kNoAction: { return; }
      case action::kShift:    { Shift();  return; }
      case action::kArcLeft:  { ArcLeft(
#ifdef LABELED
                                  action::DecodeLabel(ac)
#endif
                                  );
                                return;
                              }
      case action::kArcRight: { ArcRight(
#ifdef LABELED
                                  action::DecodeLabel(ac)
#endif
                                  );
                                return;
                              }
      case action::kPopRoot:  { PopRoot(); return; }
      default:                { THROW("unknown action: " << ac << '.'); }
    }
  }

//-----------------------------------------------------------------------------

public:
  unsigned StandardMove(const CDependencyParse & tree,
                        const std::vector<CDependencyLabel> & m_lCacheLabel) {
    if (terminated()) {
      return action::EncodeAction(action::kNoAction);
    }

    int stack_size = m_Stack.size();
    if (0 == stack_size) {
      return action::EncodeAction(action::kShift);
    }
    else if (1 == stack_size) {
      if (m_nNextWord == static_cast<int>(tree.size())) {
        return action::EncodeAction(action::kPopRoot);
      } else {
        return action::EncodeAction(action::kShift);
      }
    }
    else {
      int top0 = m_Stack[stack_size - 1];
      int top1 = m_Stack[stack_size - 2];

      bool has_right_child = false;
      for (int i = m_nNextWord; i < tree.size(); ++ i) {
        if (tree[i].head == top0) { has_right_child = true; break; }
      }

      if (tree[top0].head == top1 && !has_right_child) {
        return action::EncodeAction(action::kArcRight
#ifdef LABELED
            , m_lCacheLabel[top0].code()
#endif
            );
      }
      else if (tree[top1].head == top0) {
        return action::EncodeAction(action::kArcLeft
#ifdef LABELED
            , m_lCacheLabel[top1].code()
#endif
            );
      }
      else {
        return action::EncodeAction(action::kShift);
      }
    }
  }

  void StandardMoveStep(const CDependencyParse & tree,
                        const std::vector<CDependencyLabel> & m_lCacheLabel) {
    unsigned action = StandardMove(tree, m_lCacheLabel);
    Move(action);
  }

  // we want to pop the root item after the whole tree done
  // on the one hand this seems more natural
  // on the other it is easier to score
  void StandardFinish() {
    assert(m_Stack.size() == 0);
  }

  void GenerateTree(const CTwoStringVector &input, CDependencyParse &output) const {
    output.clear();
    for (int i = 0; i < size(); ++ i) {
#ifdef LABELED
      output.push_back(CLabeledDependencyTreeNode(input.at(i).first,
                                                  input.at(i).second,
                                                  m_lHeads[i],
                                                  CDependencyLabel(m_lLabels[i]).str()));
#else
      output.push_back(CDependencyTreeNode(input.at(i).first,
                                           input.at(i).second,
                                           m_lHeads[i]));
#endif
    }
  }

  friend std::ostream & operator << (std::ostream& out, const CStateItem & item) {
    out << action::DecodeUnlabeledAction(item.last_action)
        << "-" << action::DecodeLabel(item.last_action)
        << " (" << item.score
        << "): (";
    for (int i = 0; i < item.m_Stack.size(); ++ i) {
      out << item.m_Stack[i];
      if (i + 1 < item.m_Stack.size()) { out << "|"; }
    }
    out << ") ";
    out << item.m_nNextWord << std::endl;
    return out;
  }
};

#endif  //  end for DEPPARSER_ARC_STANDARD_STATE_H
