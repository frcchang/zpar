// Copyright (C) University of Oxford 2010
#ifndef _ZPAR_DEPPARSER_CHOI_STATEITEM_H_
#define _ZPAR_DEPPARSER_CHOI_STATEITEM_H_

#include <vector>
#include "action.h"
#include "pool.h"

class CStateItem;

// The memory pool type, a truth is known that given a sentence of n words total
// number of extra memory needed by Right-Shift operation is n \times beam size
// concerning the implementation, to make it head-only library, we need to
// implement it as a template class and instance a void type, which is a dirty
// trick of C++.
template<typename T>
class CGlobalStateMemoryPool {
public:
  static CMemoryPool<CStateItem> * GetPool() {
    if (NULL == pool) {
      pool = new CMemoryPool<CStateItem>(256);
    }

    return pool;
  };

  static void FreePool() {
    if (NULL != pool) {
      delete pool;
    }

    pool = NULL;
  }

private:
  CGlobalStateMemoryPool() {}
  static CMemoryPool<CStateItem> * pool;
};

template<typename T> CMemoryPool<CStateItem> * CGlobalStateMemoryPool<T>::pool = 0;

void ClearStackTop(CStateItem * item);
void ClearDequeTail(CStateItem * item);
void ClearBufferFront(CStateItem * item);

void CopyStackTop(const CStateItem * from,    CStateItem * to);
void CopyDequeTail(const CStateItem * from,   CStateItem * to);
void CopyBufferFront(const CStateItem * from, CStateItem * to);

void InheritStackTop(const CStateItem * from, CStateItem * to);

void CopyFromStackTopToDequeTail(const CStateItem * from, CStateItem * to);
void CopyFromDequeTailToStackTop(const CStateItem * from, CStateItem * to);
void CopyFromBufferFrontToStackTop(const CStateItem * from, CStateItem * to);

// Class for the state item
class CStateItem {
public:
  CStateItem() : len_(-1) {
    Clear();
  }

  ~CStateItem() {
  }

public:
  // access property in state
  const int Size()                const { return this->m_BufferFront_Word;    }
  const SCORE_TYPE Score()        const { return this->m_Score;               }
  const int StackTopWord()        const { return this->m_StackTop_Word;       }
  const int StackTopLabel()       const { return this->m_StackTop_Label;      }
  const int StackTopParentWord()  const { return this->m_StackTop_Parent_Word;}
  const int StackTopParentLabel() const { return this->m_StackTop_Parent_Label;}
  const int StackTopGrandWord()   const { return this->m_StackTop_Grand_Word; }

  const int Stack2ndTopWord() const {
    if (!this->m_Stack) { return -1; }
    return this->m_Stack->m_StackTop_Word;
  }

  const int Stack3rdTopWord() const {
    if (!this->m_Stack) { return -1; }
    if (!this->m_Stack->m_Stack) { return -1; }
    return this->m_Stack->m_Stack->m_StackTop_Word;
  }

  const int StackTopFurthestGrandWord() const {
    return 0;
    // return this->m_StackTop_Furthest_Grand_Word;
  }

  const int StackTopLeftMostWord() const {
    return this->m_StackTop_LeftMostChild_Word;
  }

  const int StackTopLeftMostLabel() const {
    return this->m_StackTop_LeftMostChild_Label;
  }

  const int StackTopLeft2ndMostWord() const {
    return this->m_StackTop_Left2ndMostChild_Word;
  }

  const int StackTopLeft2ndMostLabel() const {
    return this->m_StackTop_Left2ndMostChild_Label;
  }

  const int StackTopRightMostWord() const {
    return this->m_StackTop_RightMostChild_Word;
  }

  const int StackTopRightMostLabel() const {
    return this->m_StackTop_RightMostChild_Label;
  }

  const int StackTopRight2ndMostWord() const {
    return this->m_StackTop_Right2ndMostChild_Word;
  }

  const int StackTopRight2ndMostLabel() const {
    return this->m_StackTop_Right2ndMostChild_Label;
  }

  const int BufferFrontWord() const         {
    if (this->m_BufferFront_Word >= len_) { return -1; }
    return this->m_BufferFront_Word;
  }
  const int BufferFrontLabel() const        { return this->m_BufferFront_Label; }
  const int BufferFrontParentWord() const   { return this->m_BufferFront_Parent_Word; }
  const int BufferFrontParentLabel() const  { return this->m_BufferFront_Parent_Label; }
  const int BufferFrontGrandWord() const    { return this->m_BufferFront_Grand_Word; }

  const int BufferFrontLeftMostWord() const {
    return this->m_BufferFront_LeftMostChild_Word;
  }

  const int BufferFrontLeftMostLabel() const {
    return this->m_BufferFront_LeftMostChild_Label;
  }

  const int BufferFrontLeft2ndMostWord() const {
    return this->m_BufferFront_Left2ndMostChild_Word;
  }

  const int BufferFrontLeft2ndMostLabel() const {
    return this->m_BufferFront_Left2ndMostChild_Label;
  }

  const CSetOfTags<CDependencyLabel> & StackTopLeftTagset() const {
    return m_StackTop_LeftTags;
  }

  const CSetOfTags<CDependencyLabel> & StackTopRightTagset() const {
    return m_StackTop_RightTags;
  }

  const CSetOfTags<CDependencyLabel> & BufferFrontLeftTagset() const {
    return m_BufferFront_LeftTags;
  }

  const int StackSize()     const { return this->m_StackSize; }
  const bool StackEmpty()   const { return this->m_StackSize == 0; }
  const bool BufferEmpty()  const { return this->m_BufferFront_Word >= this->len_; }

  const int StackTopLeftArity()    const { return this->m_StackTop_Number_LeftChildren; }
  const int StackTopRightArity()   const { return this->m_StackTop_Number_RightChildren; }
  const int BufferFrontLeftArity() const { return this->m_BufferFront_Number_LeftChildren; }

  const bool AfterPopRoot() const {
    return this->m_LastAction == action::kPopRoot;
  }

  const bool AfterLeftReduce() const {
#ifdef LABELED
    return action::DecodeUnlabeledAction(this->m_LastAction) == action::kLeftReduce;
#else
    return this->m_LastAction == action::kLeftReduce;
#endif // end for LABELED
  }

  const bool AfterLeftPass() const {
#ifdef LABELED
    return action::DecodeUnlabeledAction(this->m_LastAction) == action::kLeftPass;
#else
    return this->m_LastAction = action::kLeftPass;
#endif  //  end for LABELED
  }

  const bool AfterRightShift() const {
#ifdef LABELED
    return action::DecodeUnlabeledAction(this->m_LastAction) == action::kRightShift;
#else
    return this->m_LastAction == action::kRightShift;
#endif // end for LABELED
  }

  const bool AfterRightPass() const {
#ifdef LABELED
    return action::DecodeUnlabeledAction(this->m_LastAction) == action::kRightPass;
#else
    return this->m_LastAction == action::kRightPass;
#endif  //  end for LABELED
  }

  const bool IsTerminated() const {
    return (m_LastAction == action::kPopRoot
            || m_LastAction == action::kIdle);
  }

  const bool AfterNoShift() const {
    return action::kNoShift == this->m_LastAction;
  }

  const bool IsCompleted() const {
    return (this->m_Deque == NULL
            && m_BufferFront_Word == len_
            && m_StackSize == 1
            && m_StackTop_Parent_Word == DEPENDENCY_LINK_NO_HEAD);
  }

  const bool StackTopHasHead() const {
    return (this->m_StackTop_Parent_Word != DEPENDENCY_LINK_NO_HEAD);
  }

  const bool BufferFrontHasHead() const {
    return (this->m_BufferFront_Parent_Word != DEPENDENCY_LINK_NO_HEAD);
  }
public:
  inline bool operator < (const CStateItem & o) const {
    return m_Score < o.m_Score;
  }

  inline bool operator > (const CStateItem & o) const {
    return m_Score > o.m_Score;
  }

  inline bool operator == (const CStateItem & o) const {
    THROW("equal is not implemented");
  }

  inline bool operator != (const CStateItem & o) const {
    THROW("unequal is not implemented");
  }

public:
  // set this state as initial state
  void Clear() {
    m_Stack             = NULL;
    m_Deque             = NULL;
    m_Previous          = NULL;
    m_Next              = NULL;

    m_StackSize         = 0;
    m_StackTop_Word     = -1;
    m_BufferFront_Word  = 0;
    m_DequeTail_Word    = -1;

    ClearStackTop(this);
    ClearDequeTail(this);
    ClearBufferFront(this);

    m_LastAction = action::kNoAction;
    m_Score = 0.;
  }

  // copy the current state item to other state item
  void Copy(CStateItem * o) {
    o->m_Stack     = this->m_Stack;
    o->m_Deque     = this->m_Deque;
    o->m_Previous  = this->m_Previous;
    o->m_Next      = this->m_Next;

    o->m_StackSize = this->m_StackSize;

    CopyStackTop(this, o);
    CopyDequeTail(this, o);
    CopyBufferFront(this, o);

    o->m_LastAction = this->m_LastAction;
    o->m_Score      = this->m_Score;

    o->len_ = this->len_;
  }   //  end for Copy

  // perform No-Reduce transition, includes
  //  1) pop a word from the stack
  //  2) keep the deque unchanged
  //  3) keep the buffer unchanged
  void NoReduce(CStateItem * next) {
    // maintaining the structure between the reduce operation
    next->m_Stack      = this->m_Stack->m_Stack;
    next->m_Deque      = this->m_Deque;
    next->m_Previous   = this;
    next->m_Next       = NULL;
    next->m_StackSize  = this->m_StackSize - 1;

    InheritStackTop(this, next);
    CopyDequeTail(this, next);
    CopyBufferFront(this, next);

    next->m_LastAction = action::kNoReduce;
    next->len_ = this->len_;
  }   //  end for Reduce

  // perform Pop-Root transition, actually it invoke the NoReduce transition
  // add Root link and keep the action
  void PopRoot(CStateItem * next) {
    // maintaining the structure between the reduce operation
    next->m_Stack      = this->m_Stack;
    next->m_Deque      = this->m_Deque;
    next->m_Previous   = this;
    next->m_Next       = NULL;
    next->m_StackSize  = this->m_StackSize;

    CopyStackTop(this, next);
    CopyDequeTail(this, next);
    CopyBufferFront(this, next);

#ifdef LABEL
    next->m_StackTop_Label = CDependencyLabel::ROOT;
#endif  //  end for LABEL
    next->m_LastAction     = action::EncodeAction(action::kPopRoot);
    next->len_ = this->len_;
  }   //  end for PopRoot

  void Idle(CStateItem * next) {
    next->m_Previous   = this;
    next->m_Stack      = this->m_Stack;
    next->m_Deque      = this->m_Deque;
    next->m_Next       = NULL;
    next->m_StackSize  = this->m_StackSize;

    CopyStackTop(this, next);
    CopyDequeTail(this, next);
    CopyBufferFront(this, next);

    next->m_LastAction = action::kIdle;
    next->len_ = this->len_;
  }

  // perform No-Pass transition, includes
  //  1) pop a word from the stack
  //  2) push the word into deque
  //  3) keep the buffer unchanged
  void NoPass(CStateItem * next) {
    next->m_Stack     = this->m_Stack->m_Stack;
    next->m_Deque     = this;
    next->m_Previous  = this;
    next->m_Next      = NULL;
    next->m_StackSize = this->m_StackSize -1;

    InheritStackTop(this, next);
    CopyFromStackTopToDequeTail(this, next);
    CopyBufferFront(this, next);

    next->m_LastAction = action::kNoPass;
    next->len_ = this->len_;
  }

  //
  void NoShift(CStateItem * next) {
    int heads[MAX_SENTENCE_SIZE];
    int deprels[MAX_SENTENCE_SIZE];

    for (int i = 0; i < len_; ++ i) {
      heads[i] = DEPENDENCY_LINK_NO_HEAD;
      deprels[i] = CDependencyLabel::NONE;
    }

    for (CStateItem * current = this;
         (current && !(current->AfterRightShift() || current->AfterNoShift()));
         current = current->m_Previous) {
      PackedToTree(current, heads, deprels);
    }

    CStateItem * chain = this;
    CStateItem * prev = CGlobalStateMemoryPool<void>::GetPool()->allocate();
    Copy(prev);
    CStateItem * p = NULL;
    for (p = this; p->m_Next; p = p->m_Next);
    p->m_Next     = prev;
    prev->m_Deque = NULL;
    prev->m_Next  = NULL;
    ClearDequeTail(prev);
    prev->m_DequeTail_Word = -1;

    // loop over the deque and shift
    // shift all the stuff in deque into the stack
    while (chain->m_Deque) {
      CStateItem * n2 = CGlobalStateMemoryPool<void>::GetPool()->allocate();
      n2->len_ = this->len_;
      n2->m_Stack     = prev;
      n2->m_Deque     = NULL;
      n2->m_Next      = NULL;
      n2->m_Previous  = prev;

      n2->m_StackSize = prev->m_StackSize + 1;
      // tree states of the new state should be maintained.

      CopyFromDequeTailToStackTop(chain, n2);
      ClearDequeTail(n2);
      n2->m_DequeTail_Word = -1;
      CopyBufferFront(prev, n2);

      int head = -1;
      head = n2->m_StackTop_Parent_Word;
      if (head >= 0) {
        n2->m_StackTop_Grand_Word   = heads[head];
        n2->m_StackTop_Parent_Label = deprels[head];
      }

      head = n2->m_BufferFront_Parent_Word;
      if (head >= 0) {
        n2->m_BufferFront_Grand_Word   = heads[head];
        n2->m_BufferFront_Parent_Label = deprels[head];
      }

      chain = chain->m_Deque;
      prev  = n2;
    }

    next->m_Stack     = prev;
    next->m_Deque     = NULL;
    next->m_Next      = NULL;
    next->m_Previous  = this;
    next->m_StackSize = prev->m_StackSize + 1;

    next->m_StackTop_Word  = prev->m_BufferFront_Word;
    CopyFromBufferFrontToStackTop(prev, next);
    ClearDequeTail(next);
    next->m_DequeTail_Word = -1;
    ClearBufferFront(next);
    next->m_BufferFront_Word = prev->m_BufferFront_Word + 1;

    next->m_LastAction = action::kNoShift;
    next->len_ = this->len_;
  }

  // perform the Left-Reduce transition, includes
  //  1) pop a word from stack
  //  2) keep the deque
  //  3) add an {i<-j} to buffer
  void * LeftReduce(CStateItem * next
#ifdef LABELED
      , int label
#endif  //  end for LABELED
      ) {
    next->m_Stack     = this->m_Stack->m_Stack;
    next->m_Deque     = this->m_Deque;
    next->m_Previous  = this;
    next->m_Next      = NULL;
    next->m_StackSize = this->m_StackSize - 1;

    InheritStackTop(this, next);
    CopyDequeTail(this, next);
    CopyBufferFront(this, next);

    next->m_BufferFront_Left2ndMostChild_Word = this->m_BufferFront_LeftMostChild_Word;
    next->m_BufferFront_LeftMostChild_Word    = this->m_StackTop_Word;
    next->m_BufferFront_Number_LeftChildren   = this->m_BufferFront_Number_LeftChildren+1;

#ifdef LABELED
    next->m_BufferFront_Left2ndMostChild_Label = this->m_BufferFront_LeftMostChild_Label;
    next->m_BufferFront_LeftMostChild_Label    = label;
    next->m_BufferFront_LeftTags.add(label);
#endif

    next->m_LastAction = action::EncodeAction(action::kLeftReduce
#ifdef LABELED
        , label
#endif  //  end for LABELED
        );
    next->len_ = this->len_;
  }

  // perform the Left-Pass transition, includes
  //  1) pop a word from stack
  //  2) push the word to deque
  //  3) add an {i<-j} to buffer
  void * LeftPass(CStateItem * next
#ifdef LABELED
      , unsigned label
#endif  //  end for LABELED
      ) {
    next->m_Stack     = this->m_Stack->m_Stack;
    next->m_Deque     = this;
    next->m_Previous  = this;
    next->m_Next      = NULL;
    next->m_StackSize = this->m_StackSize -1;

    InheritStackTop(this, next);
    CopyFromStackTopToDequeTail(this, next);
    CopyBufferFront(this, next);

    next->m_DequeTail_Parent_Word             = next->m_BufferFront_Word;
    next->m_DequeTail_Grand_Word              = next->m_BufferFront_Parent_Word;
    next->m_BufferFront_Left2ndMostChild_Word = next->m_BufferFront_LeftMostChild_Word;
    next->m_BufferFront_LeftMostChild_Word    = this->m_StackTop_Word;
    next->m_BufferFront_LeftTags.add(label);

#ifdef LABELED
    next->m_DequeTail_Label                   = label;
    next->m_DequeTail_Parent_Label            = next->m_BufferFront_Label;
    next->m_BufferFront_Left2ndMostChild_Label= next->m_BufferFront_LeftMostChild_Label;
    next->m_BufferFront_LeftMostChild_Label   = label;
    next->m_BufferFront_LeftTags.add(label);
#endif  //  end for LABELED

    next->m_LastAction = action::EncodeAction(action::kLeftPass
#ifdef LABELED
        , label
#endif  //  end for LABELED
        );
    next->len_ = this->len_;
  }

  // perform RightPass transition, includes
  //  1) pop a word from stack
  //  2) push the word to deque
  //  3) add an {i->j} to buffer
  void * RightPass(CStateItem * next
#ifdef LABELED
      , unsigned label
#endif  //  end for LABELED
      ) {
    next->m_Stack     = this->m_Stack->m_Stack;
    next->m_Deque     = this;
    next->m_Previous  = this;
    next->m_Next      = NULL;
    next->m_StackSize = this->m_StackSize - 1;

    InheritStackTop(this, next);
    CopyFromStackTopToDequeTail(this, next);
    CopyBufferFront(this, next);

    next->m_BufferFront_Parent_Word           = this->m_StackTop_Word;
    next->m_BufferFront_Grand_Word            = this->m_StackTop_Parent_Word;
    next->m_DequeTail_Right2ndMostChild_Word  = next->m_DequeTail_RightMostChild_Word;
    next->m_DequeTail_RightMostChild_Word     = this->m_BufferFront_Word;
    next->m_DequeTail_Number_RightChildren    = next->m_DequeTail_Number_RightChildren + 1;

#ifdef LABELED
    next->m_BufferFront_Label                 = label;
    next->m_BufferFront_Parent_Label          = this->m_StackTop_Label;
    next->m_DequeTail_Right2ndMostChild_Label = next->m_DequeTail_RightMostChild_Label;
    next->m_DequeTail_RightMostChild_Label    = label;
    next->m_DequeTail_RightTags.add(label);
#endif  //  end for LABELED

    next->m_LastAction = action::EncodeAction(action::kRightPass
#ifdef LABELED
        , label
#endif  //  end for LABELED
        );

    next->len_ = this->len_;
  }

  // perform Right Shift
  void * RightShift(CStateItem * next
#ifdef LABELED
      , int label
#endif  //  end for LABELED
      ) {

    // pack the tree
    int heads[MAX_SENTENCE_SIZE];
    int deprels[MAX_SENTENCE_SIZE];

    for (int i = 0; i < len_; ++ i) {
      heads[i] = DEPENDENCY_LINK_NO_HEAD;
      deprels[i] = CDependencyLabel::NONE;
    }

    for (CStateItem * current = this;
         (current && !(current->AfterRightShift() || current->AfterNoShift()));
         current = current->m_Previous) {
      PackedToTree(current, heads, deprels);
    }

    // Copy the Prev state,
    CStateItem * chain = this;
    CStateItem * prev = CGlobalStateMemoryPool<void>::GetPool()->allocate();
    Copy(prev);
    CStateItem * p = NULL;
    for (p = this; p->m_Next; p = p->m_Next);
    p->m_Next     = prev;
    prev->m_Deque = NULL;
    prev->m_Next  = NULL;

    prev->m_StackTop_Right2ndMostChild_Word = prev->m_StackTop_RightMostChild_Word;
    prev->m_StackTop_RightMostChild_Word    = prev->m_BufferFront_Word;
    prev->m_StackTop_Number_RightChildren   = prev->m_StackTop_Number_RightChildren+1;

#ifdef LABELED
    prev->m_StackTop_Right2ndMostChild_Label = prev->m_StackTop_RightMostChild_Label;
    prev->m_StackTop_RightMostChild_Label = label;
    prev->m_StackTop_RightTags.add(label);
#endif  //  end for LABELED

    CStateItem * prevv = prev;

    while (chain->m_Deque) {
      CStateItem * n2 = CGlobalStateMemoryPool<void>::GetPool()->allocate();
      n2->len_        = len_;
      n2->m_Stack     = prev;
      n2->m_Deque     = NULL;
      n2->m_Next      = NULL;
      n2->m_Previous  = prev;

      n2->m_StackSize = prev->m_StackSize + 1;
      // tree states of the new state should be maintained.

      CopyFromDequeTailToStackTop(chain, n2);
      ClearDequeTail(n2);
      n2->m_DequeTail_Word = -1;
      CopyBufferFront(prev, n2);

      int head = -1;
      head = n2->m_StackTop_Parent_Word;
      if (head >= 0) {
        n2->m_StackTop_Grand_Word   = heads[head];
        n2->m_StackTop_Parent_Label = deprels[head];
      }

      head = n2->m_BufferFront_Parent_Word;
      if (head >= 0) {
        n2->m_BufferFront_Grand_Word   = heads[head];
        n2->m_BufferFront_Parent_Label = deprels[head];
      }

      chain = chain->m_Deque;
      prev  = n2;
    }

    next->m_Stack     = prev;
    next->m_Previous  = this;
    next->m_Deque     = NULL;
    next->m_Next      = NULL;

    next->m_StackSize        = prev->m_StackSize + 1;
    CopyFromBufferFrontToStackTop(prevv, next);
    next->m_StackTop_Parent_Word  = prevv->m_StackTop_Word;
    next->m_StackTop_Grand_Word   = prevv->m_StackTop_Parent_Word;

#ifdef LABELED
    next->m_StackTop_Label = label;
    next->m_StackTop_Parent_Label = prevv->m_StackTop_Label;
#endif  //  endfor LABELED

    ClearDequeTail(next);
    next->m_DequeTail_Word = -1;
    ClearBufferFront(next);

    next->m_BufferFront_Word = prevv->m_BufferFront_Word + 1;
    next->m_LastAction = action::EncodeAction(action::kRightShift
#ifdef LABELED
        , label
#endif  //  end for LABELED
        );

    next->len_ = this->len_;
  }

  //
  void Move(CStateItem * next, const unsigned long & ac) {
    switch (
#ifdef LABELED
        action::DecodeUnlabeledAction(ac)
#else
        ac
#endif  // end for LABELED
        ) {
      case action::kNoAction:   {                   return; }
      case action::kIdle:       { Idle(next);       return; }
      case action::kNoShift:    { NoShift(next);    return; }
      case action::kNoReduce:   { NoReduce(next);   return; }
      case action::kNoPass:     { NoPass(next);     return; }
      case action::kPopRoot:    { PopRoot(next);    return; }
      case action::kLeftPass:   { LeftPass(next
#ifdef LABELED
                                           , action::DecodeLabel(ac)
#endif  //  end for LABELED
                                           );   return; }
      case action::kLeftReduce: { LeftReduce(next
#ifdef LABELED
                                             , action::DecodeLabel(ac)
#endif  //  end for LABELED
                                            ); return; }
      case action::kRightPass:  { RightPass(next
#ifdef LABELED
                                            , action::DecodeLabel(ac)
#endif  //  end for LABELED
                                            );  return; }
      case action::kRightShift: { RightShift(next
#ifdef LABELED
                                            , action::DecodeLabel(ac)
#endif  //  end for LABELED
                                            ); return; }
      default: {
                 std::cerr << ac << std::endl;
                 std::cerr << action::DecodeUnlabeledAction(ac) << std::endl;
                 assert(0==1);
               }
    }
  }

  //
  unsigned StandardTransition(const CDependencyParse & oracle_tree
#ifdef LABELED
                              , const std::vector<CDependencyLabel> & oracle_labels
#endif  //  end for LABELED
                       ) const {
    if (m_StackSize == 0) {
      return action::kNoShift;
    }

    int dir = -1;
    int len = oracle_tree.size();
    const int stack_top    = m_StackTop_Word;
    const int buffer_front = m_BufferFront_Word;

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

    for (const CStateItem * p = this->m_Stack; p; p = p->m_Stack) {
      if (oracle_tree[p->m_StackTop_Word].head == buffer_front) {
        is_gold_shift  = false; break; }
    }

    if (m_BufferFront_Word == len) {
      if (m_StackSize == 1)     { return action::kPopRoot; }
      else if (is_gold_reduce)  { return action::kNoReduce; }
      else {                      return action::kNoPass; }
    }

    if (0 == dir) {
      if (is_gold_reduce) {
        return action::EncodeAction(action::kLeftReduce
#ifdef LABELED
                                    , oracle_labels[stack_top].code()
#endif  //  end for LABELED
            );
      } else {
        return action::EncodeAction(action::kLeftPass
#ifdef LABELED
                                    , oracle_labels[stack_top].code()
#endif  //  end for LABELED
            );
      }
    } else if (1 == dir) {
      if (is_gold_shift)  {
        return action::EncodeAction(action::kRightShift
#ifdef LABELED
                                    , oracle_labels[buffer_front].code()
#endif  //  end for LABELED
            );
      } else {
        return action::EncodeAction(action::kRightPass
#ifdef LABELED
                                    , oracle_labels[buffer_front].code()
#endif  //  end for LABELED
            );
      }
    } else {
      if (is_gold_shift)  {
        return action::kNoShift;
      } else if ((DEPENDENCY_LINK_NO_HEAD != m_StackTop_Parent_Word)
          && (is_gold_reduce)) {
        return action::kNoReduce;
      } else {
        return action::kNoPass;
      }
    }
  }

  //
  //
  //
  void GenerateTree(const CTwoStringVector & input,
                    CDependencyParse & output) const {

    int heads[MAX_SENTENCE_SIZE];
    int deprels[MAX_SENTENCE_SIZE];
    RecoverTree(this->Size(), heads, deprels);

    for (int i = 0; i < this->Size(); ++ i) {
#ifdef LABELED
      output.push_back(CLabeledDependencyTreeNode(
                       input.at(i).first,
                       input.at(i).second,
                       heads[i],
                       CDependencyLabel(deprels[i]).str()));
#else
      output.push_back(CDependencyTreeNode(
                       input.at(i).first,
                       input.at(i).second,
                       heads[i]);
#endif  //  end for LABELED
    }
  }

  void RecoverTree(int len, int * heads, int * deprels) const {
    // init
    for (int i = 0; i < len; ++ i) {
      heads[i] = DEPENDENCY_LINK_NO_HEAD;
#ifdef LABELED
      deprels[i] = CDependencyLabel::NONE;
#endif  //  end for LABELED
    }

    const CStateItem * p = this;
    while (p) {
      // if last action is al or
      PackedToTree(p, heads, deprels);
      p = p->m_Previous;
    }
  }

  void PackedToTree(const CStateItem * p, int * heads, int * deprels) const {
    if (p->AfterLeftReduce() || p->AfterLeftPass()) {
      heads[p->m_Previous->StackTopWord()] = p->m_Previous->BufferFrontWord();
#ifdef LABELED
      deprels[p->m_Previous->StackTopWord()] = action::DecodeLabel(p->m_LastAction);
#endif  //  end for LABELED
    }
    else if (p->AfterRightShift() || p->AfterRightPass()) {
      heads[p->m_Previous->BufferFrontWord()] = p->m_Previous->StackTopWord();
#ifdef LABELED
      deprels[p->m_Previous->BufferFrontWord()] =action::DecodeLabel(p->m_LastAction);
#endif  //  end for LABELED
    }
    else if (p->AfterPopRoot()) {
#ifdef LABELED
      deprels[p->m_Previous->StackTopWord()] = CDependencyLabel::ROOT;
#endif  //  end for LABELED
    }
  }

  friend std::ostream & operator << (std::ostream & out, const CStateItem & x) {
    out << x.m_Previous << "->" << &x << "("
        << action::DecodeUnlabeledAction(x.m_LastAction)
        << "-" << action::DecodeLabel(x.m_LastAction) << ")"
        << std::endl
        << "  * (" << x.m_Stack                 << ") "
        << x.m_StackTop_Word                    << " : ("
        << x.m_StackTop_Parent_Word             << "|"
        << x.m_StackTop_Label                   << ", "
        << x.m_StackTop_Grand_Word              << "|"
        << x.m_StackTop_Parent_Label            << ", "
        << x.m_StackTop_LeftMostChild_Word      << "|"
        << x.m_StackTop_LeftMostChild_Label     << ", "
        << x.m_StackTop_Left2ndMostChild_Word   << "|"
        << x.m_StackTop_Left2ndMostChild_Label  << ", "
        << x.m_StackTop_Right2ndMostChild_Word  << "|"
        << x.m_StackTop_Right2ndMostChild_Label << ", "
        << x.m_StackTop_RightMostChild_Word     << "|"
        << x.m_StackTop_RightMostChild_Label    <<  ")"
        << std::endl
        << "  * (" << x.m_Deque                 << ") "
        << x.m_DequeTail_Word                   << " : ("
        << x.m_DequeTail_Parent_Word            << "|"
        << x.m_DequeTail_Label                  << ", "
        << x.m_DequeTail_Grand_Word             << "|"
        << x.m_DequeTail_Parent_Label           << ", "
        << x.m_DequeTail_LeftMostChild_Word     << "|"
        << x.m_DequeTail_LeftMostChild_Label    << ", "
        << x.m_DequeTail_Left2ndMostChild_Word  << "|"
        << x.m_DequeTail_Left2ndMostChild_Label << ", "
        << x.m_DequeTail_Right2ndMostChild_Word << "|"
        << x.m_DequeTail_Right2ndMostChild_Label<< ", "
        << x.m_DequeTail_RightMostChild_Word    << "|"
        << x.m_DequeTail_RightMostChild_Label   <<  ")"
        << std::endl
        << "  * " << x.m_BufferFront_Word         << " : ("
        << x.m_BufferFront_Parent_Word            << "|"
        << x.m_BufferFront_Label                  << ", "
        << x.m_BufferFront_Grand_Word             << "|"
        << x.m_BufferFront_Parent_Label           << ", "
        << x.m_BufferFront_LeftMostChild_Word     << "|"
        << x.m_BufferFront_LeftMostChild_Label    << ", "
        << x.m_BufferFront_Left2ndMostChild_Word  << "|"
        << x.m_BufferFront_Left2ndMostChild_Label << ")"
        << std::endl;

    return out;
  }

public:
  CStateItem *  m_Stack;
  CStateItem *  m_Deque;
  CStateItem *  m_Previous;
  CStateItem *  m_Next;

  int           m_StackSize;
  int           m_LastAction;

  // information of the stack top word
  int           m_StackTop_Word;
  int           m_StackTop_Label;
  int           m_StackTop_Parent_Word;
  int           m_StackTop_Parent_Label;
  int           m_StackTop_Grand_Word;
  int           m_StackTop_Furthest_Grand_Word;
  int           m_StackTop_Number_LeftChildren;
  int           m_StackTop_LeftMostChild_Word;
  int           m_StackTop_LeftMostChild_Label;
  int           m_StackTop_Left2ndMostChild_Word;
  int           m_StackTop_Left2ndMostChild_Label;
  int           m_StackTop_Number_RightChildren;
  int           m_StackTop_RightMostChild_Word;
  int           m_StackTop_RightMostChild_Label;
  int           m_StackTop_Right2ndMostChild_Word;
  int           m_StackTop_Right2ndMostChild_Label;

  // information of the deque tail word
  int           m_DequeTail_Word;
  int           m_DequeTail_Label;
  int           m_DequeTail_Parent_Word;
  int           m_DequeTail_Parent_Label;
  int           m_DequeTail_Grand_Word;
  int           m_DequeTail_Furthest_Grand_Word;
  int           m_DequeTail_Number_LeftChildren;
  int           m_DequeTail_LeftMostChild_Word;
  int           m_DequeTail_LeftMostChild_Label;
  int           m_DequeTail_Left2ndMostChild_Word;
  int           m_DequeTail_Left2ndMostChild_Label;
  int           m_DequeTail_Number_RightChildren;
  int           m_DequeTail_RightMostChild_Word;
  int           m_DequeTail_RightMostChild_Label;
  int           m_DequeTail_Right2ndMostChild_Word;
  int           m_DequeTail_Right2ndMostChild_Label;

  //  information of the buffer front word
  int           m_BufferFront_Word;
  int           m_BufferFront_Label;
  int           m_BufferFront_Parent_Word;
  int           m_BufferFront_Parent_Label;
  int           m_BufferFront_Grand_Word;
  int           m_BufferFront_Furthest_Grand_Word;
  int           m_BufferFront_Number_LeftChildren;
  int           m_BufferFront_LeftMostChild_Word;
  int           m_BufferFront_LeftMostChild_Label;
  int           m_BufferFront_Left2ndMostChild_Word;
  int           m_BufferFront_Left2ndMostChild_Label;

  SCORE_TYPE    m_Score;

  CSetOfTags<CDependencyLabel> m_StackTop_LeftTags;
  CSetOfTags<CDependencyLabel> m_StackTop_RightTags;
  CSetOfTags<CDependencyLabel> m_DequeTail_LeftTags;
  CSetOfTags<CDependencyLabel> m_DequeTail_RightTags;
  CSetOfTags<CDependencyLabel> m_BufferFront_LeftTags;

  int len_;
};

inline
void ClearStackTop(CStateItem * item) {
  // the links
  item->m_StackTop_Parent_Word              = DEPENDENCY_LINK_NO_HEAD;
  item->m_StackTop_Grand_Word               = DEPENDENCY_LINK_NO_HEAD;
  item->m_StackTop_Furthest_Grand_Word      = DEPENDENCY_LINK_NO_HEAD;
  item->m_StackTop_LeftMostChild_Word       = DEPENDENCY_LINK_NO_HEAD;
  item->m_StackTop_Left2ndMostChild_Word    = DEPENDENCY_LINK_NO_HEAD;
  item->m_StackTop_RightMostChild_Word      = DEPENDENCY_LINK_NO_HEAD;
  item->m_StackTop_Right2ndMostChild_Word   = DEPENDENCY_LINK_NO_HEAD;

  // the valancy
  item->m_StackTop_Number_LeftChildren      = 0;
  item->m_StackTop_Number_RightChildren     = 0;

#ifdef LABELED
  // the label
  item->m_StackTop_Label                    = CDependencyLabel::NONE;
  item->m_StackTop_Parent_Label             = CDependencyLabel::NONE;
  item->m_StackTop_LeftMostChild_Label      = CDependencyLabel::NONE;
  item->m_StackTop_Left2ndMostChild_Label   = CDependencyLabel::NONE;
  item->m_StackTop_RightMostChild_Label     = CDependencyLabel::NONE;
  item->m_StackTop_Right2ndMostChild_Label  = CDependencyLabel::NONE;

  // the label set
  item->m_StackTop_LeftTags.clear();
  item->m_StackTop_RightTags.clear();
#endif
}

inline
void ClearDequeTail(CStateItem * item) {
  // the links
  item->m_DequeTail_Parent_Word             = DEPENDENCY_LINK_NO_HEAD;
  item->m_DequeTail_Grand_Word              = DEPENDENCY_LINK_NO_HEAD;
  item->m_DequeTail_Furthest_Grand_Word     = DEPENDENCY_LINK_NO_HEAD;
  item->m_DequeTail_LeftMostChild_Word      = DEPENDENCY_LINK_NO_HEAD;
  item->m_DequeTail_Left2ndMostChild_Word   = DEPENDENCY_LINK_NO_HEAD;
  item->m_DequeTail_RightMostChild_Word     = DEPENDENCY_LINK_NO_HEAD;
  item->m_DequeTail_Right2ndMostChild_Word  = DEPENDENCY_LINK_NO_HEAD;

  // the valancy
  item->m_DequeTail_Number_LeftChildren   = 0;
  item->m_DequeTail_Number_RightChildren  = 0;

#ifdef LABELED
  // the labels
  item->m_DequeTail_Label                   = CDependencyLabel::NONE;
  item->m_DequeTail_Parent_Label            = CDependencyLabel::NONE;
  item->m_DequeTail_LeftMostChild_Label     = CDependencyLabel::NONE;
  item->m_DequeTail_Left2ndMostChild_Label  = CDependencyLabel::NONE;
  item->m_DequeTail_RightMostChild_Label    = CDependencyLabel::NONE;
  item->m_DequeTail_Right2ndMostChild_Label = CDependencyLabel::NONE;

  // the label set
  item->m_DequeTail_LeftTags.clear();
  item->m_DequeTail_RightTags.clear();
#endif  //  end for LABELED
}

inline
void ClearBufferFront(CStateItem * item) {
  // the links
  item->m_BufferFront_Parent_Word           = DEPENDENCY_LINK_NO_HEAD;
  item->m_BufferFront_Grand_Word            = DEPENDENCY_LINK_NO_HEAD;
  item->m_BufferFront_Furthest_Grand_Word   = DEPENDENCY_LINK_NO_HEAD;
  item->m_BufferFront_LeftMostChild_Word    = DEPENDENCY_LINK_NO_HEAD;
  item->m_BufferFront_Left2ndMostChild_Word = DEPENDENCY_LINK_NO_HEAD;

  // the valancy
  item->m_BufferFront_Number_LeftChildren   = 0;
#ifdef LABELED
  // the labels
  item->m_BufferFront_Label                  = CDependencyLabel::NONE;
  item->m_BufferFront_Parent_Label           = CDependencyLabel::NONE;
  item->m_BufferFront_LeftMostChild_Label    = CDependencyLabel::NONE;
  item->m_BufferFront_Left2ndMostChild_Label = CDependencyLabel::NONE;

  // the label set
  item->m_BufferFront_LeftTags.clear();
#endif  //  end for LABELED
}

inline
void CopyStackTop(const CStateItem * from, CStateItem * to) {
  // locality
  to->m_StackTop_Word                  = from->m_StackTop_Word;
  // links
  to->m_StackTop_Parent_Word           = from->m_StackTop_Parent_Word;
  to->m_StackTop_Grand_Word            = from->m_StackTop_Grand_Word;
  to->m_StackTop_LeftMostChild_Word    = from->m_StackTop_LeftMostChild_Word;
  to->m_StackTop_Left2ndMostChild_Word = from->m_StackTop_Left2ndMostChild_Word;
  to->m_StackTop_RightMostChild_Word   = from->m_StackTop_RightMostChild_Word;
  to->m_StackTop_Right2ndMostChild_Word= from->m_StackTop_Right2ndMostChild_Word;
  // valancy
  to->m_StackTop_Number_LeftChildren   = from->m_StackTop_Number_LeftChildren;
  to->m_StackTop_Number_RightChildren  = from->m_StackTop_Number_RightChildren;

#ifdef LABELED
  // label
  to->m_StackTop_Label                 = from->m_StackTop_Label;
  to->m_StackTop_Parent_Label          = from->m_StackTop_Parent_Label;
  to->m_StackTop_LeftMostChild_Label   = from->m_StackTop_LeftMostChild_Label;
  to->m_StackTop_Left2ndMostChild_Label= from->m_StackTop_Left2ndMostChild_Label;
  to->m_StackTop_RightMostChild_Label  = from->m_StackTop_RightMostChild_Label;
  to->m_StackTop_Right2ndMostChild_Label=from->m_StackTop_Right2ndMostChild_Label;

  to->m_StackTop_LeftTags  = from->m_StackTop_LeftTags;
  to->m_StackTop_RightTags = from->m_StackTop_RightTags;

#endif  //  end for LABELED
}

inline
void CopyDequeTail(const CStateItem * from, CStateItem * to) {
  // locality
  to->m_DequeTail_Word                 = from->m_DequeTail_Word;
  // links
  to->m_DequeTail_Parent_Word            = from->m_DequeTail_Parent_Word;
  to->m_DequeTail_Grand_Word             = from->m_DequeTail_Grand_Word;
  to->m_DequeTail_LeftMostChild_Word     = from->m_DequeTail_LeftMostChild_Word;
  to->m_DequeTail_Left2ndMostChild_Word  = from->m_DequeTail_Left2ndMostChild_Word;
  to->m_DequeTail_RightMostChild_Word    = from->m_DequeTail_RightMostChild_Word;
  to->m_DequeTail_Right2ndMostChild_Word = from->m_DequeTail_Right2ndMostChild_Word;
  // valancy
  to->m_DequeTail_Number_LeftChildren  = from->m_DequeTail_Number_LeftChildren;
  to->m_DequeTail_Number_RightChildren = from->m_DequeTail_Number_RightChildren;

#ifdef LABELED
  // labels
  to->m_DequeTail_Label                = from->m_DequeTail_Label;
  to->m_DequeTail_Parent_Label         = from->m_DequeTail_Parent_Label;
  to->m_DequeTail_LeftMostChild_Label  = from->m_DequeTail_LeftMostChild_Label;
  to->m_DequeTail_Left2ndMostChild_Label = from->m_DequeTail_Left2ndMostChild_Label;
  to->m_DequeTail_RightMostChild_Label = from->m_DequeTail_RightMostChild_Label;
  to->m_DequeTail_Right2ndMostChild_Label= from->m_DequeTail_Right2ndMostChild_Label;
  // tag set
  to->m_DequeTail_LeftTags  = from->m_DequeTail_LeftTags;
  to->m_DequeTail_RightTags = from->m_DequeTail_RightTags;
#endif  //  end for LABELED
}

inline
void CopyBufferFront(const CStateItem * from, CStateItem * to) {
  // locality
  to->m_BufferFront_Word                  = from->m_BufferFront_Word;
  // links
  to->m_BufferFront_Parent_Word           = from->m_BufferFront_Parent_Word;
  to->m_BufferFront_Grand_Word            = from->m_BufferFront_Grand_Word;
  to->m_BufferFront_LeftMostChild_Word    = from->m_BufferFront_LeftMostChild_Word;
  to->m_BufferFront_Left2ndMostChild_Word = from->m_BufferFront_Left2ndMostChild_Word;
  // valancy
  to->m_BufferFront_Number_LeftChildren   = from->m_BufferFront_Number_LeftChildren;

#ifdef LABELED
  // label
  to->m_BufferFront_Label                  = from->m_BufferFront_Label;
  to->m_BufferFront_Parent_Label           = from->m_BufferFront_Parent_Label;
  to->m_BufferFront_LeftMostChild_Label    = from->m_BufferFront_LeftMostChild_Label;
  to->m_BufferFront_Left2ndMostChild_Label = from->m_BufferFront_Left2ndMostChild_Label;
  // tag sets
  to->m_BufferFront_LeftTags = from->m_BufferFront_LeftTags;
#endif  //  end for LABELED
}

inline
void InheritStackTop(const CStateItem * from, CStateItem * to) {
#define NON_NULL(x) ((from->m_Stack) ? (from->m_Stack->x):DEPENDENCY_LINK_NO_HEAD);
  to->m_StackTop_Word                   = NON_NULL(m_StackTop_Word);
  to->m_StackTop_Parent_Word            = NON_NULL(m_StackTop_Parent_Word);
  to->m_StackTop_Grand_Word             = NON_NULL(m_StackTop_Grand_Word);
  to->m_StackTop_Furthest_Grand_Word    = NON_NULL(m_StackTop_Furthest_Grand_Word);
  to->m_StackTop_LeftMostChild_Word     = NON_NULL(m_StackTop_LeftMostChild_Word);
  to->m_StackTop_Left2ndMostChild_Word  = NON_NULL(m_StackTop_Left2ndMostChild_Word);
  to->m_StackTop_RightMostChild_Word    = NON_NULL(m_StackTop_RightMostChild_Word);
  to->m_StackTop_Right2ndMostChild_Word = NON_NULL(m_StackTop_Right2ndMostChild_Word);
#undef NON_NULL

#define NON_NULL(x) ((from->m_Stack) ? (from->m_Stack->x):0);
  to->m_StackTop_Number_LeftChildren    = NON_NULL(m_StackTop_Number_LeftChildren);
  to->m_StackTop_Number_RightChildren   = NON_NULL(m_StackTop_Number_RightChildren);
#undef NON_NULL

#ifdef LABELED
#define NON_NULL(x) ((from->m_Stack) ? (from->m_Stack->x) : CDependencyLabel::NONE);
  to->m_StackTop_Label                    = NON_NULL(m_StackTop_Label);
  to->m_StackTop_Parent_Label             = NON_NULL(m_StackTop_Parent_Label);
  to->m_StackTop_LeftMostChild_Label      = NON_NULL(m_StackTop_LeftMostChild_Label);
  to->m_StackTop_Left2ndMostChild_Label   = NON_NULL(m_StackTop_Left2ndMostChild_Label);
  to->m_StackTop_RightMostChild_Label     = NON_NULL(m_StackTop_RightMostChild_Label);
  to->m_StackTop_Right2ndMostChild_Label  = NON_NULL(m_StackTop_Right2ndMostChild_Label);
#undef NON_NULL // cancel the non_null

  if (from->m_Stack) { to->m_StackTop_LeftTags  = from->m_Stack->m_StackTop_LeftTags; }
  if (from->m_Stack) { to->m_StackTop_RightTags = from->m_Stack->m_StackTop_RightTags; }
#endif  //  end for LABELED
}

inline
void CopyFromStackTopToDequeTail(const CStateItem * from, CStateItem * to) {
  to->m_DequeTail_Word                  = from->m_StackTop_Word;
  to->m_DequeTail_Parent_Word           = from->m_StackTop_Parent_Word;
  to->m_DequeTail_Grand_Word            = from->m_StackTop_Grand_Word;
  to->m_DequeTail_LeftMostChild_Word    = from->m_StackTop_LeftMostChild_Word;
  to->m_DequeTail_Left2ndMostChild_Word = from->m_StackTop_Left2ndMostChild_Word;
  to->m_DequeTail_RightMostChild_Word   = from->m_StackTop_RightMostChild_Word;
  to->m_DequeTail_Right2ndMostChild_Word= from->m_StackTop_Right2ndMostChild_Word;

  to->m_DequeTail_Number_LeftChildren   = from->m_StackTop_Number_LeftChildren;
  to->m_DequeTail_Number_RightChildren  = from->m_StackTop_Number_RightChildren;

#ifdef LABELED
  to->m_DequeTail_Label                  = from->m_StackTop_Label;
  to->m_DequeTail_Parent_Label           = from->m_StackTop_Parent_Label;
  to->m_DequeTail_LeftMostChild_Label    = from->m_StackTop_LeftMostChild_Label;
  to->m_DequeTail_Left2ndMostChild_Label = from->m_StackTop_Left2ndMostChild_Label;
  to->m_DequeTail_RightMostChild_Label   = from->m_StackTop_RightMostChild_Label;
  to->m_DequeTail_Right2ndMostChild_Label= from->m_StackTop_Right2ndMostChild_Label;

  to->m_DequeTail_LeftTags  = from->m_StackTop_LeftTags;
  to->m_DequeTail_RightTags = from->m_StackTop_RightTags;
#endif  // end for LABELED
}

inline
void CopyFromDequeTailToStackTop(const CStateItem * from, CStateItem * to) {
  to->m_StackTop_Word                 = from->m_DequeTail_Word;
  to->m_StackTop_Parent_Word          = from->m_DequeTail_Parent_Word;
  to->m_StackTop_Grand_Word           = from->m_DequeTail_Grand_Word;
  to->m_StackTop_LeftMostChild_Word   = from->m_DequeTail_LeftMostChild_Word;
  to->m_StackTop_Left2ndMostChild_Word= from->m_DequeTail_Left2ndMostChild_Word;
  to->m_StackTop_RightMostChild_Word  = from->m_DequeTail_RightMostChild_Word;
  to->m_StackTop_Right2ndMostChild_Word=from->m_DequeTail_Right2ndMostChild_Word;

  to->m_StackTop_Number_LeftChildren  = from->m_DequeTail_Number_LeftChildren;
  to->m_StackTop_Number_RightChildren = from->m_DequeTail_Number_RightChildren;

#ifdef LABELED
  to->m_StackTop_Label                = from->m_DequeTail_Label;
  to->m_StackTop_Parent_Label         = from->m_DequeTail_Parent_Label;
  to->m_StackTop_LeftMostChild_Label  = from->m_DequeTail_LeftMostChild_Label;
  to->m_StackTop_Left2ndMostChild_Label=from->m_DequeTail_Left2ndMostChild_Label;
  to->m_StackTop_RightMostChild_Label = from->m_DequeTail_Right2ndMostChild_Label;

  to->m_StackTop_LeftTags   = from->m_DequeTail_LeftTags;
  to->m_StackTop_RightTags  = from->m_DequeTail_RightTags;
#endif  //  end for LABELED
}

inline
void CopyFromBufferFrontToStackTop(const CStateItem * from, CStateItem * to) {
  to->m_StackTop_Word                   = from->m_BufferFront_Word;
  to->m_StackTop_Parent_Word            = from->m_BufferFront_Parent_Word;
  to->m_StackTop_Grand_Word             = from->m_BufferFront_Grand_Word;
  to->m_StackTop_LeftMostChild_Word     = from->m_BufferFront_LeftMostChild_Word;
  to->m_StackTop_Left2ndMostChild_Word  = from->m_BufferFront_Left2ndMostChild_Word;
  to->m_StackTop_RightMostChild_Word    = DEPENDENCY_LINK_NO_HEAD;
  to->m_StackTop_Right2ndMostChild_Word = DEPENDENCY_LINK_NO_HEAD;

  to->m_StackTop_Number_LeftChildren    = from->m_BufferFront_Number_LeftChildren;
  to->m_StackTop_Number_RightChildren   = 0;

#ifdef LABELED
  to->m_StackTop_Label                  = from->m_BufferFront_Label;
  to->m_StackTop_Parent_Label           = from->m_BufferFront_Parent_Label;
  to->m_StackTop_LeftMostChild_Label    = from->m_BufferFront_LeftMostChild_Label;
  to->m_StackTop_Left2ndMostChild_Label = from->m_BufferFront_Left2ndMostChild_Label;
  to->m_StackTop_RightMostChild_Label   = CDependencyLabel::NONE;
  to->m_StackTop_Right2ndMostChild_Label= CDependencyLabel::NONE;

  to->m_StackTop_LeftTags               = from->m_BufferFront_LeftTags;
  to->m_StackTop_RightTags.clear();
#endif  //  end for LABELED
}

#endif  //  end for define _ZPAR_DEPPARSER_CHOI_STATEITEM_H_
