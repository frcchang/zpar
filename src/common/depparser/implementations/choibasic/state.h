// Copyright (C) University of Oxford 2010
#ifndef COMMON_DEPPARSER_IMPLEMENTATIONS_CHOIBASIC_STATE_H
#define COMMON_DEPPARSER_IMPLEMENTATIONS_CHOIBASIC_STATE_H

const int kMaxSentenceSize = MAX_SENTENCE_SIZE;
const int kMaxSteps = kMaxSentenceSize* kMaxSentenceSize / 2;
const int kAgendaSize = AGENDA_SIZE;

class CStateItem {
protected:
  //! the stack
  std::vector<int> stack_;
  //! the deque
  std::vector<int> deque_;
  //! index for the next word
  int next_word;
  //! the lexical head for each word
  int heads[kMaxSentenceSize];
  //! the leftmost dependency for each word (just for cache, temporary info)
  int left_most_child[kMaxSentenceSize];
  //! the second leftmost dependency
  int left_2most_child[kMaxSentenceSize];
  //! the rightmost dependency for each word (just for cache, temporary info)
  int right_most_child[kMaxSentenceSize];
  //! the second rightmost dependency
  int right_2most_child[kMaxSentenceSize];
  //! the number of left dependencies
  int num_left_children[kMaxSentenceSize];
  //! the number of right dependencies
  int num_right_children[kMaxSentenceSize];

#ifdef LABELED
  //! the set of left tags
  CSetOfTags<CDependencyLabel> left_dep_tagset[kMaxSentenceSize];
  //! the set of right tags
  CSetOfTags<CDependencyLabel> right_dep_tagset[kMaxSentenceSize];
  //! the label of each dependency link
  unsigned long labels[kMaxSentenceSize];
#endif

public:
  //! score of state - predicting how potentially this is the correct one
  SCORE_TYPE score;
  //! the link to the previous state.
  const CStateItem * previous_;
  //! the last stack action
  unsigned long last_action;
  //! the length of the input data.
  int len_;

public:
  //! constructors and destructor
  CStateItem() : len_(-1) {
    clear();
  }

  ~CStateItem() {}

public:
  // comparison
  inline bool operator < (const CStateItem &item) const {
    return score < item.score;
  }

  inline bool operator > (const CStateItem &item) const {
    return score > item.score;
  }

  //! judging if the two state item equal
  inline bool operator == (const CStateItem &item) const;

  inline bool operator != (const CStateItem &item) const;

  // propty
  inline int stacksize() const {
    return stack_.size();
  }

  inline bool stackempty() const {
    return stack_.empty();
  }

  inline int stacktop() const {
    if (stack_.empty()) {
      return -1;
    }
    return stack_.back();
  }

  inline int stack2ndtop() const {
    if (stack_.size() < 2) {
      return -1;
    }
    return stack_[stack_.size() - 2];
  }

  inline int stack3rdtop() const {
    if (stack_.size() < 3) {
      return -1;
    }
    return stack_[stack_.size() - 3];
  }

  inline int stackbottom() const {
    assert(!stack_.empty());
    return stack_.front();
  }

  inline int bufferfront() const {
    if (next_word == len_) {
      return -1;
    }
    return next_word;
  }

  inline int stackitem(const unsigned &index) const {
    assert(index < stack_.size());
    return stack_[index];
  }

  inline bool afterreduce() const {
    return action::DecodeUnlabeledAction(last_action) == action::kReduce;
  }

  inline int head(const int &index) const {
    assert(index <= next_word);
    return heads[index];
  }

  inline bool is_descendant(const int & from, const int & to) const {
    assert(from <= next_word);
    assert(to <= next_word);
    int current = heads[from];
    while (current != DEPENDENCY_LINK_NO_HEAD) {
      if (current == to) return true;
      current = heads[current];
    }
    return false;
  }

  inline bool hashead(const int & index) const {
    assert(index <= next_word);
    return heads[index] != DEPENDENCY_LINK_NO_HEAD;
  }

  inline int leftdep(const int &index) const {
    assert(index <= next_word);
    return left_most_child[index];
  }

  inline int left2dep(const int & index) const {
    assert(index <= next_word);
    return left_2most_child[index];
  }

  inline int rightdep(const int &index) const {
    assert(index <= next_word);
    return right_most_child[index];
  }

  inline int right2dep(const int & index) const {
    assert(index <= next_word);
    return right_2most_child[index];
  }

  inline int size() const {
    return next_word;
  }

  inline bool bufferempty() const {
    return (next_word == len_);
  }

  inline bool complete() const {
    return next_word == len_;
    /*return (deque_.empty()
            && next_word == len_
            && stack_.size() == 1
            && DEPENDENCY_LINK_NO_HEAD == heads[stack_.back()]);*/
  }

  inline bool terminated() const {
    return (last_action == action::kPopRoot || last_action == action::kIdle);
  }

#ifdef LABELED
  inline int label(const int &index) const {
    assert(index <= next_word);
    return labels[index];
  }
#endif

  inline int leftarity(const int &index) const {
    assert(index <= next_word);
    return num_left_children[index];
  }

  inline int rightarity(const int &index) const {
    assert(index <= next_word);
    return num_right_children[index];
  }

  inline const CSetOfTags<CDependencyLabel> &lefttagset(const int &index) const {
    assert(index <= next_word);
    return left_dep_tagset[index];
  }

  inline const CSetOfTags<CDependencyLabel> &righttagset(const int &index) const {
    assert(index <= next_word);
    return right_dep_tagset[index];
  }

  void clear() {
    score     = 0;
    previous_ = 0;
    next_word = 0;
    stack_.clear();
    deque_.clear();
    last_action = action::kNoAction;
    ClearNext();
  }

  CStateItem & operator = (const CStateItem &item) {
    stack_        = item.stack_;
    deque_        = item.deque_;
    next_word     = item.next_word;
    last_action   = item.last_action;
    len_          = item.len_;
    score         = item.score;
    previous_     = item.previous_;

    memcpy(heads,             item.heads,             sizeof(int)*(next_word + 1));
    memcpy(left_most_child,   item.left_most_child,   sizeof(int)*(next_word + 1));
    memcpy(left_2most_child,  item.left_2most_child,  sizeof(int)*(next_word + 1));
    memcpy(right_most_child,  item.right_most_child,  sizeof(int)*(next_word + 1));
    memcpy(right_2most_child, item.right_2most_child, sizeof(int)*(next_word + 1));
    memcpy(num_left_children, item.num_left_children, sizeof(int)*(next_word + 1));
    memcpy(num_right_children,item.num_right_children,sizeof(int)*(next_word + 1));

#ifdef LABELED
    memcpy(labels, item.labels, sizeof(unsigned long)*(next_word + 1));
    for (int i = 0; i <= next_word; ++ i) {
      // only copy active word (including m_nNext)
      left_dep_tagset[i]    = item.left_dep_tagset[i];
      right_dep_tagset[i]   = item.right_dep_tagset[i];
    }
#endif
  }

//-----------------------------------------------------------------------------

public:
  // perform idle action
  void Idle() {
    last_action = action::kIdle;
  }

  /**
   * Performing the shift action, the shift action does pushing. the algorithm
   * can be described as below.
   *
   * 1.loop over the deque from tail to head
   *  - 1.1 push the element into the stack
   * 2. push the next word into the deque
   */
  void Shift() {
    while (!deque_.empty()) {
      int word = deque_.back();
      deque_.pop_back();
      stack_.push_back(word);
    }

    stack_.push_back(next_word);
    next_word ++;
    ClearNext();
    last_action = action::EncodeAction(action::kShift);
  }

  // the reduce action does popping, the algorithm can be described as below
  //
  //  1. pop the stack
  void Reduce() {
    assert(false == stack_.empty());
    assert(heads[stack_.back()] != DEPENDENCY_LINK_NO_HEAD);
    stack_.pop_back();
    last_action = action::EncodeAction(action::kReduce);
  }

  // this is used for the convenience of scoring and updating
  void PopRoot() {
#ifdef LABELED
    labels[stack_.back()] = CDependencyLabel::ROOT;
#endif  //  end for LABELED

    last_action = action::EncodeAction(action::kPopRoot);
    // stack_.pop_back();  // should not pop it, extract features for idle
  }

  // perform the no-pass action, this action pop a word from the stack, push
  // it into the tail of the deque, the algorithm can be described as below:
  //
  //  initialize top = stack.top
  //  1. pop stack
  //  2. push top into tail of deque
  void NoPass() {
    int left = stack_.back(); stack_.pop_back();
    deque_.push_back(left);
    last_action = action::kNoPass;
  }

  // the arc left action links the current stack top to the next word
  // with pop out the stack top word. the algorithm can be described
  // as below:
  //
  //  1. link (stack.top <- buffer.front)
  //    1.1 heads[stack.top]        = buffer.front
  //    1.2 left2most[buffer.front] = leftmost[buffer.front]
  //    1.3 leftmost[buffer.front]  = stack.top
  //    1.4 numleftdep[buffer.front] += 1
  //  2. pop stack
  //  3. set last action
  void ArcLeft(
#ifdef LABELED
      unsigned long lab
#endif  //  end for LABELED
      ) {
    assert(stack_.size() > 0);
    assert(heads[stack_.back()] == DEPENDENCY_LINK_NO_HEAD);

    int left = stack_.back();
    stack_.pop_back();
    heads[left] = next_word;

    // update the tree structure
    if (DEPENDENCY_LINK_NO_HEAD == left_most_child[next_word] ||
        left < left_most_child[next_word]) {
      left_2most_child[next_word] = left_most_child[next_word];
      left_most_child[next_word]  = left;
    } else if (DEPENDENCY_LINK_NO_HEAD == left_2most_child[next_word] ||
        left < left_2most_child[next_word]) {
      left_2most_child[next_word] = left;
    }
    num_left_children[next_word] ++;

#ifdef LABELED
    labels[left] = lab;
    left_dep_tagset[next_word].add(lab);
#endif  //  end for LABELED

    last_action = action::EncodeAction(action::kArcLeft
#ifdef LABELED
        , lab
#endif  //  end for LABELED
        );
  }

  // the arc right action links the next word to the current stack top with
  // pushing. the algorithm can be described as below
  //
  //  1. link an arc (stack.top -> buffer.front)
  //  2. loop over element in the deque
  //    2.1 push it into the stack
  void ArcRight(
#ifdef LABELED
      unsigned long lab
#endif  //  end for LABELED
      ) {
    assert(stack_.size() > 0);
    int left = stack_.back();
    // push everything in deque into the stack
    while (!deque_.empty()) {
      int word = deque_.back();
      deque_.pop_back();
      stack_.push_back(word);
    }
    stack_.push_back(next_word);

    heads[next_word] = left;
    // update the tree structure
    if (DEPENDENCY_LINK_NO_HEAD == right_most_child[left] ||
        next_word > right_most_child[left]) {
      right_2most_child[left] = right_most_child[left];
      right_most_child[left]  = next_word;
    } else if (DEPENDENCY_LINK_NO_HEAD == right_2most_child[left] ||
        next_word > right_2most_child[left]) {
      right_2most_child[left] = next_word;
    }
    num_right_children[left] ++;

#ifdef LABELED
    labels[next_word] = lab;
    right_dep_tagset[left].add(lab);
#endif  //  end for LABELED

    next_word ++;
    ClearNext();

    last_action = action::EncodeAction(action::kArcRight
#ifdef LABELED
        , lab
#endif  //  end for LABELED
        );
  }

  // perform left pass operation,
  //
  void LeftPass(
#ifdef LABELED
      unsigned long lab
#endif  //  end for LABELED
      ) {
    assert(stack_.size() > 0);
    assert(heads[stack_.back()] == DEPENDENCY_LINK_NO_HEAD);

    int left = stack_.back();
    stack_.pop_back();
    deque_.push_back(left);
    heads[left] = next_word;

    // update the tree structure
    if (DEPENDENCY_LINK_NO_HEAD == left_most_child[next_word] ||
        left < left_most_child[next_word]) {
      left_2most_child[next_word] = left_most_child[next_word];
      left_most_child[next_word]  = left;
    } else if (DEPENDENCY_LINK_NO_HEAD == left_2most_child[next_word] ||
        left < left_2most_child[next_word]) {
      left_2most_child[next_word] = left;
    }
    num_left_children[next_word] ++;

#ifdef LABELED
    labels[left] = lab;
    left_dep_tagset[next_word].add(lab);
#endif  //  end for LABELED

    last_action = action::EncodeAction(action::kLeftPass
#ifdef LABELED
        , lab
#endif  //  end for LABELED
        );
  }

  void RightPass(
#ifdef LABELED
      unsigned long lab
#endif  //  end for LABELED
      ) {
    int left = stack_.back();
    stack_.pop_back();
    deque_.push_back(left);
    heads[next_word]          = left;

    // update the tree structure
    if (DEPENDENCY_LINK_NO_HEAD == right_most_child[left] ||
        next_word > right_most_child[left]) {
      right_2most_child[left] = right_most_child[left];
      right_most_child[left]  = next_word;
    } else if (DEPENDENCY_LINK_NO_HEAD == right_2most_child[left] ||
        next_word > right_2most_child[left]) {
      right_2most_child[left] = next_word;
    }
    num_right_children[left] ++;

#ifdef LABELED
    labels[next_word] = lab;
    right_dep_tagset[left].add(lab);
#endif  //  end for LABELED

    last_action = action::EncodeAction(action::kRightPass
#ifdef LABELED
        , lab
#endif  //  end for LABELED
        );
  }

  // the clear next action is used to clear the next word, used with
  // forwarding the next word index
  void ClearNext() {
    heads[next_word]              = DEPENDENCY_LINK_NO_HEAD;
    left_most_child[next_word]    = DEPENDENCY_LINK_NO_HEAD;
    left_2most_child[next_word]   = DEPENDENCY_LINK_NO_HEAD;
    right_most_child[next_word]   = DEPENDENCY_LINK_NO_HEAD;
    right_2most_child[next_word]  = DEPENDENCY_LINK_NO_HEAD;

    num_left_children[next_word]  = 0;
    num_right_children[next_word] = 0;
#ifdef LABELED
    left_dep_tagset[next_word].clear();
    right_dep_tagset[next_word].clear();
    labels[next_word] = CDependencyLabel::NONE;
#endif
  }

  // the move action is a simple call to do action according to the action code
  void Move(const unsigned long &ac) {
    switch (action::DecodeUnlabeledAction(ac)) {
      case action::kNoAction: {             return; }
      case action::kIdle:     {             return; }
      case action::kShift:    { Shift();    return; }
      case action::kReduce:   { Reduce();   return; }
      case action::kPopRoot:  { PopRoot();  return; }
      case action::kNoPass:   { NoPass();   return; }
      case action::kArcLeft:  { ArcLeft(
#ifdef LABELED
                                  action::DecodeLabel(ac)
#endif  //  end for LABELED
                                  );        return;
      }
      case action::kArcRight: { ArcRight(
#ifdef LABELED
                                  action::DecodeLabel(ac)
#endif  //  end for LABELED
                                  );        return;
      }
      case action::kLeftPass: { LeftPass(
#ifdef LABELED
                                  action::DecodeLabel(ac)
#endif  //  end for LABELED
                                  );        return;
      }
      case action::kRightPass:{ RightPass(
#ifdef LABELED
                                   action::DecodeLabel(ac)
#endif  //  end for LABELED
                                   );       return;
      }
      default: {
        THROW("unknown action: " << ac << '.');
      }
    }
  }

//-----------------------------------------------------------------------------

public:
  // perform standard move with the oracle given, this method is oritented from
  // choi's code. the algorithm can be described below
  //
  //  1. if gold_heads[stack.top] = buffer.front:
  //        dir = left;
  //     elif gold_heads[buffer.front] = stack.top:
  //        dir = right;
  //     else:
  //        dir = no_dir
  //  2.
  //
  void StandardMoveStep(
      const CDependencyParse & oracle_tree
#ifdef LABELED
      , const std::vector<CDependencyLabel> & oracle_labels
#endif  //  end for LABELED
      ) {

    // if the stack is empty, only shift operation can be applied
    if (stack_.empty()) {
      Shift();
      return;
    }

    int dir = -1;
    int len = oracle_tree.size();
    const int stack_top    = stack_.back();
    const int buffer_front = next_word;

    // there should be a left arc adding operation
    if (oracle_tree[stack_top].head == buffer_front) {
      dir = 0;
    } else if (oracle_tree[buffer_front].head == stack_top) {
      dir = 1;
    }

    // std::cout << dir << std::endl;
    bool is_gold_reduce = true;
    bool is_gold_shift  = (oracle_tree[buffer_front].head >= stack_top);

    for (int i = buffer_front + 1; i < len; ++ i) {
      if (oracle_tree[i].head == stack_top)            { is_gold_reduce = false; break; }
    }

    for (int i = 0; i < stack_.size() - 1; ++ i) {
      if (oracle_tree[stack_[i]].head == buffer_front) { is_gold_shift  = false; break; }
    }

    if (next_word == len) {
      PopRoot();
      return;
    }

    if (0 == dir) {
      if (is_gold_reduce) {
        ArcLeft(
#ifdef LABELED
            oracle_labels[stack_top].code()
#endif  //  end for LABELED
            );
      } else {
        LeftPass(
#ifdef LABELED
            oracle_labels[stack_top].code()
#endif  //  end for LABELED
            );
      }
    } else if (1 == dir) {
      if (is_gold_shift)  {
        ArcRight(
#ifdef LABELED
            oracle_labels[buffer_front].code()
#endif  //  end for LABELED
            );
      } else {
        RightPass(
#ifdef LABELED
            oracle_labels[buffer_front].code()
#endif  //  end for LABELED
            );
      }
    } else {
      if (is_gold_shift)  {
        Shift();
      } else if ((DEPENDENCY_LINK_NO_HEAD != heads[stack_top])
          && (is_gold_reduce)) {
        Reduce();
      } else {
        NoPass();
      }
    }
  }

  // we want to pop the root item after the whole tree done
  // on the one hand this seems more natural
  // on the other it is easier to score
  void StandardFinish() {
    assert( stack_.size() == 0 );
  }

  void GenerateTree(
      const CTwoStringVector & input,
      CDependencyParse       & output) const {

    output.clear();
    for (int i = 0; i < input.size(); ++ i) {
#ifdef LABELED
      output.push_back(CLabeledDependencyTreeNode(
            input.at(i).first,
            input.at(i).second,
            heads[i],
            CDependencyLabel(labels[i]).str()));
#else
      output.push_back(CDependencyTreeNode(
            input.at(i).first,
            input.at(i).second,
            heads[i])) ;
#endif  //  end if LABELED
    }
  }

  friend std::ostream & operator << (std::ostream& out, const CStateItem & item) {
    out << action::DecodeUnlabeledAction(item.last_action)
      << "-" << action::DecodeLabel(item.last_action)
      << " (" << item.score
      << "): (";
    for (int i = 0; i < item.stack_.size(); ++ i) {
      out << item.stack_[i];
      if (i + 1 < item.stack_.size()) { out << "|"; }
    }
    out << ") ";
    out << "[";
    for (int i = 0; i < item.deque_.size(); ++ i) {
      out << item.deque_[i];
      if (i + 1 < item.deque_.size()) { out << " "; }
    }
    out << "] ";
    out << item.next_word << std::endl;
    return out;
  }
};

struct CScoredTransition {
  //! The source state.
  const CStateItem* source;
  //! The action.
  unsigned long action;
  //! The score.
  SCORE_TYPE score;
};

struct CDecodeContext {
  //!
  CStateItem* lattice_index[kMaxSteps];
  //!
  unsigned lattice_size[kMaxSteps];

  CDecodeContext() {
    memset(lattice_index, 0, sizeof(lattice_index));
    memset(lattice_size, 0, sizeof(lattice_size));
  }

  ~CDecodeContext() {
    for (int i = 0; i < kMaxSteps; ++ i) {
      if (lattice_index[i]) {
        delete [](lattice_index[i]);
      }
    }
  }

  CStateItem* malloc(int id, unsigned size) {
    assert(id < kMaxSteps);
    assert(size <= kAgendaSize);

    if (lattice_index[id] == 0) {
      //! One for out-of-beam correct state.
      lattice_index[id] = new CStateItem[kAgendaSize];
    }

    lattice_size[id] = size;
    return lattice_index[id];
  }
};

#endif  // COMMON_DEPPARSER_IMPLEMENTATIONS_CHOIBASIC_STATE_H
