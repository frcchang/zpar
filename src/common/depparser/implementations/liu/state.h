// Copyright (C) University of Oxford 2010
#ifndef _ZPAR_DEPPARSER_LIU_STATEITEM_H_
#define _ZPAR_DEPPARSER_LIU_STATEITEM_H_

#include <vector>
#include "action.h"
#include "pool.h"

class CStateItem;


// The memory pool. If duplication occurs, get an StateItem from the
// memory pool.
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

class CStateItem {
  public:
    CStateItem(const std::vector< CTaggedWord<CTag, TAG_SEPARATOR> > * cache = 0)
      : m_lCache(cache) {
        Clear();
    }

    ~CStateItem() {
    }

  public:
    /*
     * access property in state */
    const int Size() const {
      return this->m_BufferFront_Word;
    }

    const SCORE_TYPE Score() const {
      return this->m_Score;
    }

    const int StackTopWord() const {
      return this->m_StackTop_Word;
    }

    const int StackTopLabel() const {
      return this->m_StackTop_Label;
    }

    const int StackTopParentWord() const {
      return this->m_StackTop_Parent_Word;
    }

    const int StackTopParentLabel() const {
      return this->m_StackTop_Parent_Label;
    }

    const int StackTopGrandWord() const {
      return this->m_StackTop_Grand_Word;
    }

    const int StackTopFurthestGrandWord() const {
      return this->m_StackTop_Furthest_Grand_Word;
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

    const int BufferFrontWord() const {
      return this->m_BufferFront_Word;
    }

    const int BufferFrontParentWord() const  {
      TRACE("buffer front should not have parent");
      return -1;
    }

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

    const int BufferFrontRightMostWord() const {
      TRACE("buffer front should not have right child");
      return -1;
    }

    const int BufferFrontRightMostLabel() const {
      TRACE("buffer front should not have right child");
      return -1;
    }

    const int BufferFrontRight2ndMostWord() const {
      TRACE("buffer front should not have right child");
      return -1;
    }

    const int BufferFrontRight2ndMostLabel() const {
      TRACE("buffer front should not have right child");
      return -1;
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
    const int StackSize() const {
      return this->m_StackSize;
    }

    const int StackTopLeftArity() const {
      return this->m_StackTop_Number_LeftChildren;
    }

    const int StackTopRightArity() const {
      return this->m_StackTop_Number_RightChildren;
    }

    const int BufferFrontLeftArity() const {
      return this->m_BufferFront_Number_LeftChildren;
    }

    const bool AfterPopRoot() const {
      return this->m_LastAction == action::POP_ROOT;
    }

    const bool AfterReduce() const {
      return this->m_LastAction == action::REDUCE; 
    }

    const bool AfterArcLeft() const {
#ifdef LABELED
      return action::getUnlabeledAction(this->m_LastAction) == action::ARC_LEFT;
#else
      return this->m_LastAction == action::ARC_LEFT;
#endif  //  end for LABELED
    }

    const bool AfterArcRight() const {
#ifdef LABELED
      return action::getUnlabeledAction(this->m_LastAction) == action::ARC_RIGHT;
#else
      return this->m_LastAction == action::ARC_RIGHT;
#endif  //  end for LABELED
    }

    const bool StackEmpty() const {
      return this->m_StackSize == 0;
    }

    const bool HeadStackEmpty() const {
      return NULL == this->m_HeadStack;
    }

    const int HeadStackTopWord() const {
      return this->m_HeadStackTop_Word;
    }

    const int HeadStack2ndTopWord() const { 
      if (NULL == this->m_HeadStack) {
        return -1;
      }
      return this->m_HeadStack->m_HeadStackTop_Word;
    }

    const int HeadStackSize() const {
      return this->m_HeadStackSize;
    }

  public:
    inline bool operator < (const CStateItem & o) const {
      return m_Score < o.m_Score;
    }

    inline bool operator > (const CStateItem & o) const {
      return m_Score > o.m_Score;
    }

    inline bool operator == (const CStateItem & o) const {
      THROW("equal operation is not supported.");
      return true;
    }

    inline bool operator != (const CStateItem & o) const {
      THROW("unequal operation is not supported.");
      return false;
    }

  public:
    /* set this state as initial state
    */
    void Clear() {
      m_Stack                         = NULL;
      m_HeadStack                     = NULL;
      m_Previous                      = NULL;
      m_Next                          = NULL;

      m_StackSize                     = 0;
      m_HeadStackSize                 = 0;
      m_StackTop_Word                 = -1;
      m_BufferFront_Word              = 0;

      m_StackTop_Parent_Word                  = DEPENDENCY_LINK_NO_HEAD;
      m_StackTop_Grand_Word                   = DEPENDENCY_LINK_NO_HEAD;
      m_StackTop_Furthest_Grand_Word          = DEPENDENCY_LINK_NO_HEAD;
      m_StackTop_LeftMostChild_Word           = DEPENDENCY_LINK_NO_HEAD;
      m_StackTop_Left2ndMostChild_Word        = DEPENDENCY_LINK_NO_HEAD;
      m_StackTop_RightMostChild_Word          = DEPENDENCY_LINK_NO_HEAD;
      m_StackTop_Right2ndMostChild_Word       = DEPENDENCY_LINK_NO_HEAD;
      m_BufferFront_LeftMostChild_Word        = DEPENDENCY_LINK_NO_HEAD;
      m_BufferFront_Left2ndMostChild_Word     = DEPENDENCY_LINK_NO_HEAD;

      m_StackTop_Number_LeftChildren          = 0;
      m_StackTop_Number_RightChildren         = 0;
      m_BufferFront_Number_LeftChildren       = 0;

      m_HeadStackTop_Word                     = -1;

#ifdef LABELED
      m_StackTop_Label                        = CDependencyLabel::NONE;
      m_StackTop_LeftMostChild_Label          = CDependencyLabel::NONE;
      m_StackTop_Left2ndMostChild_Label       = CDependencyLabel::NONE;
      m_StackTop_RightMostChild_Label         = CDependencyLabel::NONE;
      m_StackTop_Right2ndMostChild_Label      = CDependencyLabel::NONE;
      m_BufferFront_LeftMostChild_Label       = CDependencyLabel::NONE;
      m_BufferFront_Left2ndMostChild_Label    = CDependencyLabel::NONE;

      m_StackTop_LeftTags.clear();
      m_StackTop_RightTags.clear();
      m_BufferFront_LeftTags.clear();
#endif  //  end for LABELED

      m_LastAction                            = action::NO_ACTION;
      m_Score = 0.;
    }

    /*
     * copy the current state item to other state item
     *
     *  @parma[out]     other       the other state item 
     */
    void Copy(CStateItem * o) {
      o->m_Stack                      = this->m_Stack;
      o->m_HeadStack                  = this->m_HeadStack;
      o->m_Previous                   = this->m_Previous;
      o->m_Next                       = this->m_Next;

      o->m_StackSize                  = this->m_StackSize;
      o->m_HeadStackSize              = this->m_HeadStackSize;
      o->m_StackTop_Word              = this->m_StackTop_Word;
      o->m_BufferFront_Word           = this->m_BufferFront_Word;

      o->m_StackTop_Parent_Word           = this->m_StackTop_Parent_Word;
      o->m_StackTop_Grand_Word            = this->m_StackTop_Grand_Word;
      o->m_StackTop_Furthest_Grand_Word   = this->m_StackTop_Furthest_Grand_Word;
      o->m_StackTop_Parent_Label          = this->m_StackTop_Parent_Label;
      o->m_StackTop_LeftMostChild_Word    = this->m_StackTop_LeftMostChild_Word;
      o->m_StackTop_Left2ndMostChild_Word = this->m_StackTop_Left2ndMostChild_Word;
      o->m_StackTop_RightMostChild_Word   = this->m_StackTop_RightMostChild_Word;
      o->m_StackTop_Right2ndMostChild_Word= this->m_StackTop_Right2ndMostChild_Word;
      o->m_BufferFront_LeftMostChild_Word     = this->m_BufferFront_LeftMostChild_Word;
      o->m_BufferFront_Left2ndMostChild_Word  = this->m_BufferFront_Left2ndMostChild_Word;

      o->m_StackTop_Number_LeftChildren   = this->m_StackTop_Number_LeftChildren;
      o->m_StackTop_Number_RightChildren  = this->m_StackTop_Number_RightChildren;
      o->m_BufferFront_Number_LeftChildren= this->m_BufferFront_Number_LeftChildren;

      o->m_HeadStackTop_Word              = this->m_HeadStackTop_Word;

#ifdef LABELED
      o->m_StackTop_Label                 = this->m_StackTop_Label;
      o->m_StackTop_Parent_Label          = this->m_StackTop_Parent_Label;
      o->m_StackTop_LeftMostChild_Label   = this->m_StackTop_LeftMostChild_Label;
      o->m_StackTop_Left2ndMostChild_Label= this->m_StackTop_Left2ndMostChild_Label;
      o->m_StackTop_RightMostChild_Label  = this->m_StackTop_RightMostChild_Label;
      o->m_StackTop_Right2ndMostChild_Label=this->m_StackTop_Right2ndMostChild_Label;
      o->m_BufferFront_LeftMostChild_Label    = this->m_BufferFront_LeftMostChild_Label;
      o->m_BufferFront_Left2ndMostChild_Label = this->m_BufferFront_Left2ndMostChild_Label;

      o->m_StackTop_LeftTags      = this->m_StackTop_LeftTags;
      o->m_StackTop_RightTags     = this->m_StackTop_RightTags;
      o->m_BufferFront_LeftTags   = this->m_BufferFront_LeftTags;
#endif  //  end for LABELED

      o->m_LastAction                     = this->m_LastAction;
      o->m_Score                          = this->m_Score;
    }   //  end for Copy

    /*
     * perform the reduce operation
     */
    void Reduce(CStateItem * next) {
      next->m_Stack                       = this->m_Stack->m_Stack;
      next->m_HeadStack                   = this->m_HeadStack;
      next->m_Previous                    = this;
      next->m_Next                        = NULL;
      next->m_StackSize                   = this->m_StackSize - 1;
      next->m_HeadStackSize               = this->m_HeadStackSize;

#define NON_NULL(x) ((this->m_Stack) ? (this->m_Stack->x):DEPENDENCY_LINK_NO_HEAD);
      next->m_StackTop_Word                   = NON_NULL(m_StackTop_Word);
      next->m_StackTop_Parent_Word            = NON_NULL(m_StackTop_Parent_Word);
      next->m_StackTop_Grand_Word             = NON_NULL(m_StackTop_Grand_Word);
      next->m_StackTop_Furthest_Grand_Word    = NON_NULL(m_StackTop_Furthest_Grand_Word);
      next->m_StackTop_LeftMostChild_Word     = NON_NULL(m_StackTop_LeftMostChild_Word);
      next->m_StackTop_Left2ndMostChild_Word  = NON_NULL(m_StackTop_Left2ndMostChild_Word);
      next->m_StackTop_RightMostChild_Word    = NON_NULL(m_StackTop_RightMostChild_Word);
      next->m_StackTop_Right2ndMostChild_Word = NON_NULL(m_StackTop_Right2ndMostChild_Word);
      next->m_BufferFront_Word                = this->m_BufferFront_Word;
      next->m_BufferFront_LeftMostChild_Word    = this->m_BufferFront_LeftMostChild_Word;
      next->m_BufferFront_Left2ndMostChild_Word = this->m_BufferFront_Left2ndMostChild_Word;
#undef NON_NULL

#define NON_NULL(x) ((this->m_Stack) ? (this->m_Stack->x):0);
      next->m_StackTop_Number_LeftChildren    = NON_NULL(m_StackTop_Number_LeftChildren);
      next->m_StackTop_Number_RightChildren   = NON_NULL(m_StackTop_Number_RightChildren);
      next->m_BufferFront_Number_LeftChildren = this->m_BufferFront_Number_LeftChildren;
#undef NON_NULL

#define NON_NULL ((this->m_HeadStack)\
    ?(this->m_HeadStack->m_HeadStackTop_Word):DEPENDENCY_LINK_NO_HEAD);
      next->m_HeadStackTop_Word               = NON_NULL;
#undef NON_NULL

#ifdef LABELED
#define NON_NULL(x) ((this->m_Stack) ? (this->m_Stack->x) : CDependencyLabel::NONE);
      next->m_StackTop_Label                   = NON_NULL(m_StackTop_Label);
      next->m_StackTop_Parent_Label            = NON_NULL(m_StackTop_Parent_Label);
      next->m_StackTop_LeftMostChild_Label     = NON_NULL(m_StackTop_LeftMostChild_Label);
      next->m_StackTop_Left2ndMostChild_Label  = NON_NULL(m_StackTop_Left2ndMostChild_Label);
      next->m_StackTop_RightMostChild_Label    = NON_NULL(m_StackTop_RightMostChild_Label);
      next->m_StackTop_Right2ndMostChild_Label = NON_NULL(m_StackTop_Right2ndMostChild_Label);
      next->m_BufferFront_LeftMostChild_Label    = this->m_BufferFront_LeftMostChild_Label;
      next->m_BufferFront_Left2ndMostChild_Label = this->m_BufferFront_Left2ndMostChild_Label;
#undef NON_NULL // cancel the non_null

      if (this->m_Stack) { next->m_StackTop_LeftTags  = m_Stack->m_StackTop_LeftTags; }
      if (this->m_Stack) { next->m_StackTop_RightTags = m_Stack->m_StackTop_RightTags; }
      next->m_BufferFront_LeftTags = this->m_BufferFront_LeftTags;
#endif  //  end for LABELED

      next->m_LastAction                  = action::REDUCE;
    }   //  end for Reduce

    /*
     * perform the pop root operation
     *
     *  @param[out]
     */
    void PopRoot(CStateItem * next) {
      Reduce(next);
#ifdef LABEL
      next->m_StackTop_Label  = CDependencyLabel::ROOT;
#endif  //  end for LABEL
      next->m_LastAction      = action::encodeAction(action::POP_ROOT);
    }   //  end for PopRoot

    /*
     * perform the shift operation
     *
     *  @param[out]     next        the next state
     */
    void Shift(CStateItem * next) {
      next->m_Stack                           = this;
      next->m_HeadStack                       = this;
      next->m_Previous                        = this;
      next->m_Next                            = this;
      next->m_StackSize                       = this->m_StackSize + 1;
      next->m_HeadStackSize                   = this->m_HeadStackSize + 1;

      next->m_StackTop_Word                   = this->m_BufferFront_Word;
      next->m_BufferFront_Word                = this->m_BufferFront_Word + 1;
      next->m_StackTop_Parent_Word            = DEPENDENCY_LINK_NO_HEAD;
      next->m_StackTop_Grand_Word             = DEPENDENCY_LINK_NO_HEAD;
      next->m_StackTop_Furthest_Grand_Word    = DEPENDENCY_LINK_NO_HEAD;
      next->m_StackTop_LeftMostChild_Word     = this->m_BufferFront_LeftMostChild_Word;
      next->m_StackTop_Left2ndMostChild_Word  = this->m_BufferFront_Left2ndMostChild_Word;
      next->m_StackTop_RightMostChild_Word    = DEPENDENCY_LINK_NO_HEAD;
      next->m_StackTop_Right2ndMostChild_Word = DEPENDENCY_LINK_NO_HEAD;
      next->m_BufferFront_LeftMostChild_Word  = DEPENDENCY_LINK_NO_HEAD;
      next->m_BufferFront_Left2ndMostChild_Word=DEPENDENCY_LINK_NO_HEAD;

      next->m_StackTop_Number_LeftChildren    = this->m_BufferFront_Number_LeftChildren;
      next->m_StackTop_Number_RightChildren   = 0;
      next->m_BufferFront_Number_LeftChildren = 0;

      next->m_HeadStackTop_Word               = this->m_BufferFront_Word;

#ifdef LABELED
      next->m_StackTop_Label                  = CDependencyLabel::NONE;
      next->m_StackTop_Parent_Label           = CDependencyLabel::NONE;
      next->m_StackTop_LeftMostChild_Label    = this->m_BufferFront_LeftMostChild_Label;
      next->m_StackTop_Left2ndMostChild_Label = this->m_BufferFront_Left2ndMostChild_Label;
      next->m_StackTop_RightMostChild_Label   = CDependencyLabel::NONE;
      next->m_StackTop_Right2ndMostChild_Label= CDependencyLabel::NONE;
      next->m_BufferFront_LeftMostChild_Label = CDependencyLabel::NONE;
      next->m_BufferFront_Left2ndMostChild_Label=CDependencyLabel::NONE;

      next->m_StackTop_LeftTags               = this->m_BufferFront_LeftTags;
      next->m_StackTop_RightTags.clear();
      next->m_BufferFront_LeftTags.clear();
#endif  //  end for LABELED

      next->m_LastAction                  = action::SHIFT;
    }

    /*
     * perform the arcright operation
     *
     *  @param[out]         next    the next state tranfered from current state
     *  @param[optional]    label   the label of this transsion
     */
    void ArcRight(CStateItem * next
#ifdef LABELED
        , int label
#endif  //  end for LABELED
        ) {
      // next->m
      CStateItem * prev = CGlobalStateMemoryPool<void>::GetPool()->allocate();
      // CStateItem * prev = new CStateItem;
      Copy(prev);
      CStateItem * p = NULL;
      for (p = this; p->m_Next; p = p->m_Next);
      p->m_Next                           = prev;
      prev->m_Next                        = NULL;

      next->m_Stack                       = prev;
      next->m_HeadStack                   = prev->m_HeadStack;
      next->m_Previous                    = this;
      next->m_Next                        = NULL;

      next->m_StackSize                   = prev->m_StackSize + 1;
      next->m_HeadStackSize               = prev->m_HeadStackSize;
      next->m_StackTop_Word               = prev->m_BufferFront_Word;
      next->m_BufferFront_Word            = prev->m_BufferFront_Word + 1;

      prev->m_StackTop_Number_RightChildren = prev->m_StackTop_Number_RightChildren+1;
      prev->m_StackTop_Right2ndMostChild_Word = prev->m_StackTop_RightMostChild_Word;
      prev->m_StackTop_RightMostChild_Word = prev->m_BufferFront_Word;

#ifdef LABELED
      prev->m_StackTop_Right2ndMostChild_Label = prev->m_StackTop_RightMostChild_Label;
      prev->m_StackTop_RightMostChild_Label = label;
      prev->m_StackTop_RightTags.add(label);
#endif  //  end for LABELED

      next->m_StackTop_Parent_Word            = prev->m_StackTop_Word;
      next->m_StackTop_Grand_Word             = prev->m_StackTop_Parent_Word;

      if (DEPENDENCY_LINK_NO_HEAD == prev->m_StackTop_Furthest_Grand_Word) {
        next->m_StackTop_Furthest_Grand_Word= (
            (DEPENDENCY_LINK_NO_HEAD != prev->m_StackTop_Parent_Word) ?
            prev->m_StackTop_Parent_Word : 
            prev->m_StackTop_Word);
        // furthest is not found.
      } else {
        next->m_StackTop_Furthest_Grand_Word= prev->m_StackTop_Furthest_Grand_Word;
      }

      next->m_StackTop_LeftMostChild_Word     = prev->m_BufferFront_LeftMostChild_Word;
      next->m_StackTop_Left2ndMostChild_Word  = prev->m_BufferFront_Left2ndMostChild_Word;
      next->m_StackTop_RightMostChild_Word    = DEPENDENCY_LINK_NO_HEAD;
      next->m_StackTop_Right2ndMostChild_Word = DEPENDENCY_LINK_NO_HEAD;
      next->m_BufferFront_LeftMostChild_Word    = DEPENDENCY_LINK_NO_HEAD;
      next->m_BufferFront_Left2ndMostChild_Word = DEPENDENCY_LINK_NO_HEAD;

      next->m_StackTop_Number_LeftChildren    = prev->m_BufferFront_Number_LeftChildren;
      next->m_StackTop_Number_RightChildren   = 0;
      next->m_BufferFront_Number_LeftChildren = 0;

      next->m_HeadStackTop_Word               = prev->m_HeadStackTop_Word;

#ifdef LABELED
      next->m_StackTop_Label                  = label;
      next->m_StackTop_Parent_Label           = prev->m_StackTop_Label;
      next->m_StackTop_LeftMostChild_Label    = prev->m_BufferFront_LeftMostChild_Label;
      next->m_StackTop_Left2ndMostChild_Label = prev->m_BufferFront_Left2ndMostChild_Label;
      next->m_StackTop_RightMostChild_Label   = CDependencyLabel::NONE;
      next->m_StackTop_Right2ndMostChild_Label= CDependencyLabel::NONE;
      next->m_BufferFront_LeftMostChild_Label    = CDependencyLabel::NONE;
      next->m_BufferFront_Left2ndMostChild_Label = CDependencyLabel::NONE;

      next->m_StackTop_LeftTags = prev->m_BufferFront_LeftTags;
      next->m_StackTop_RightTags.clear();
      next->m_BufferFront_LeftTags.clear();
#endif  //  end for LABELED

      next->m_LastAction = action::encodeAction(action::ARC_RIGHT
#ifdef LABELED
          , label
#endif  //  end for LABELED
          );
    }

    /*
     * perform of arcleft operation
     *
     *  @param[out]         next    the next state transfer from current state
     *  @param[optional]    label   the label of resulting from this transsision
     */
    void ArcLeft(CStateItem * next
#ifdef LABELED
        , int label
#endif  //  end for LABELED
        ) {
      next->m_Stack                       = this->m_Stack->m_Stack;
      next->m_HeadStack                   = this->m_HeadStack->m_HeadStack;
      next->m_Previous                    = this;
      next->m_StackSize                   = this->m_StackSize - 1;
      next->m_HeadStackSize               = this->m_HeadStackSize - 1;

#define NON_NULL(x) ((this->m_Stack) ? (this->m_Stack->x):DEPENDENCY_LINK_NO_HEAD);
      next->m_StackTop_Word                   = NON_NULL(m_StackTop_Word);
      next->m_StackTop_Parent_Word            = NON_NULL(m_StackTop_Parent_Word);
      next->m_StackTop_Grand_Word             = NON_NULL(m_StackTop_Grand_Word);
      next->m_StackTop_Furthest_Grand_Word    = NON_NULL(m_StackTop_Furthest_Grand_Word);
      next->m_StackTop_LeftMostChild_Word     = NON_NULL(m_StackTop_LeftMostChild_Word);
      next->m_StackTop_Left2ndMostChild_Word  = NON_NULL(m_StackTop_Left2ndMostChild_Word);
      next->m_StackTop_RightMostChild_Word    = NON_NULL(m_StackTop_RightMostChild_Word);
      next->m_StackTop_Right2ndMostChild_Word = NON_NULL(m_StackTop_Right2ndMostChild_Word);
      next->m_BufferFront_Word                 = this->m_BufferFront_Word;
      next->m_BufferFront_Left2ndMostChild_Word= this->m_BufferFront_LeftMostChild_Word;
      next->m_BufferFront_LeftMostChild_Word   = this->m_StackTop_Word;
#undef NON_NULL

#define NON_NULL(x) ((this->m_Stack) ? (this->m_Stack->x):0);
      next->m_StackTop_Number_LeftChildren    = NON_NULL(m_StackTop_Number_LeftChildren);
      next->m_StackTop_Number_RightChildren   = NON_NULL(m_StackTop_Number_RightChildren);
      next->m_BufferFront_Number_LeftChildren = this->m_BufferFront_Number_LeftChildren+1;
#undef NON_NULL

      next->m_HeadStackTop_Word               = m_HeadStackTop_Word;

#ifdef LABELED
#define NON_NULL(x) ((this->m_Stack) ? (this->m_Stack->x):CDependencyLabel::NONE);
      next->m_StackTop_Label                  = NON_NULL(m_StackTop_Label);
      next->m_StackTop_Parent_Label           = NON_NULL(m_StackTop_Parent_Label);
      next->m_StackTop_Left2ndMostChild_Label = NON_NULL(m_StackTop_Left2ndMostChild_Label);
      next->m_StackTop_RightMostChild_Label   = NON_NULL(m_StackTop_RightMostChild_Label);
      next->m_StackTop_LeftMostChild_Label    = NON_NULL(m_StackTop_LeftMostChild_Label);
      next->m_StackTop_Right2ndMostChild_Label= NON_NULL(m_StackTop_Right2ndMostChild_Label);
      next->m_BufferFront_Left2ndMostChild_Label = this->m_BufferFront_LeftMostChild_Label;
      next->m_BufferFront_LeftMostChild_Label    = label;
#undef  NON_NULL

      if (this->m_Stack) { next->m_StackTop_LeftTags  = m_Stack->m_StackTop_LeftTags; }
      if (this->m_Stack) { next->m_StackTop_RightTags = m_Stack->m_StackTop_RightTags; }
      next->m_BufferFront_LeftTags = this->m_BufferFront_LeftTags;
      next->m_BufferFront_LeftTags.add(label);
#endif

      next->m_LastAction                  = action::encodeAction(action::ARC_LEFT
#ifdef LABELED
          , label
#endif  //  end for LABELED
          );

    }

    /*
     * perform the ac trans operation
     *
     *  @param[out] next    the next state
     *  @param[in]  ac      the action command
     */
    void Move(CStateItem * next, const unsigned long & ac) {
      switch (
#ifdef LABELED
          action::getUnlabeledAction(ac)
#else
          ac
#endif
          ) {
        case action::NO_ACTION: {               return; }
        case action::SHIFT:     { Shift(next);  return; }
        case action::REDUCE:    { Reduce(next); return; }
        case action::ARC_LEFT:  { ArcLeft(next
#ifdef LABELED
              , action::getLabel(ac)
#endif  //  end for LABELED
              );
          return;
        }
        case action::ARC_RIGHT: { ArcRight(next
#ifdef LABELED
              , action::getLabel(ac)
#endif  //  end for LABELED
              );
          return;
        }
        case action::POP_ROOT: { PopRoot(next); return; }
        default:
          THROW("unknown action: " << ac << ".");
      }
    }

    unsigned StandardMoveAction(const CDependencyParse & tree
#ifdef LABELED
                                , const std::vector<CDependencyLabel> & m_lCacheLabel
#endif  //  end for LABELED
        ) {
      if (this->BufferFrontWord() == static_cast<int>(tree.size())) {
        if (this->StackSize() > 1) {
          return action::encodeAction(action::REDUCE);
        } else {
          return action::encodeAction(action::POP_ROOT);
        }
      }

      if (this->StackSize() > 0) {
        int furthestGrandWord = (
            DEPENDENCY_LINK_NO_HEAD == this->StackTopFurthestGrandWord() ? 
            this->StackTopWord() :
            this->StackTopFurthestGrandWord());
        if (tree[furthestGrandWord].head == this->BufferFrontWord()) {
          if (furthestGrandWord == this->StackTopWord()) {
            return action::encodeAction(action::ARC_LEFT
#ifdef LABELED
                , m_lCacheLabel[furthestGrandWord].code()
#endif  //  end for LABELED
                );
          } else {
            return action::encodeAction(action::REDUCE);
          }
        }
      }

      if (tree[this->BufferFrontWord()].head == DEPENDENCY_LINK_NO_HEAD ||
          tree[this->BufferFrontWord()].head > this->BufferFrontWord()) {
        return action::encodeAction(action::SHIFT);
      }
      else {
        if (tree[this->BufferFrontWord()].head == this->StackTopWord()) {
          return action::encodeAction(action::ARC_RIGHT
#ifdef LABELED
              , m_lCacheLabel[this->BufferFrontWord()].code()
#endif  //  end for LABELED
              );
        } else {
          return action::encodeAction(action::REDUCE);
        }
      }

      return action::encodeAction(action::NO_ACTION);
    }

    /*
     * calculate next state of current state given the dependency parsing
     * tree
     *
     *  @param[out]     next    the next state
     *  @param[in]      tree    the input dependency tree
     */
    bool StandardMoveStep(CStateItem * next, 
        const CDependencyParse & tree
#ifdef LABELED
        , const std::vector<CDependencyLabel> & m_lCacheLabel
#endif  //  end for LABELED
        ) {
      if (this->BufferFrontWord() == static_cast<int>(tree.size())) {
        assert(this->StackSize() > 0);

        if (this->StackSize() > 1) {
          this->Reduce(next);
          return false;
        } else {
          this->PopRoot(next);
          return false;
        }
      }   // if 

      if (this->StackSize() > 0) {
        int furthestGrandWord = (
            DEPENDENCY_LINK_NO_HEAD == this->StackTopFurthestGrandWord() ? 
            this->StackTopWord() :
            this->StackTopFurthestGrandWord());
        if (tree[furthestGrandWord].head == this->BufferFrontWord()) {
          if (furthestGrandWord == this->StackTopWord()) {
            this->ArcLeft(next
#ifdef LABELED
                , m_lCacheLabel[furthestGrandWord].code()
#endif  //  end for LABELED
                );
            return false;
          } else {
            this->Reduce(next);
            return false;
          }
        }
      }

      if (tree[this->BufferFrontWord()].head == DEPENDENCY_LINK_NO_HEAD ||
          tree[this->BufferFrontWord()].head > this->BufferFrontWord()) {
        this->Shift(next);
        return true;
      }
      else {
        if (tree[this->BufferFrontWord()].head == this->StackTopWord()) {
          this->ArcRight(next
#ifdef LABELED
              , m_lCacheLabel[this->BufferFrontWord()].code()
#endif  //  end for LABELED
              );
          return true;
        } else {
          this->Reduce(next);
          return false;
        }
      }
    }

    void GenerateTree(const CTwoStringVector & input,
        CDependencyParse & output) const {
      int heads[MAX_SENTENCE_SIZE];
      int deprels[MAX_SENTENCE_SIZE];

      RecoverTree(this->Size(), heads
#ifdef LABELED
          , deprels
#endif  //  end for LABELED
      );

      for (int i = 0; i < this->Size(); ++ i) {
#ifdef LABELED
        output.push_back(CLabeledDependencyTreeNode(input.at(i).first,
                                                    input.at(i).second,
                                                    heads[i],
                                                    CDependencyLabel(deprels[i]).str()));
#else
        output.push_back(CDependencyTreeNode(input.at(i).first,
                                             input.at(i).second,
                                             heads[i]);
#endif
      }
    }


    void RecoverTree(int len, int * heads
#ifdef LABELED
              , int * deprels
#endif  //  end for LABELED
              ) const {
      for (int i = 0; i < len; ++ i) {
        heads[i] = DEPENDENCY_LINK_NO_HEAD;
#ifdef LABELED
        deprels[i] = CDependencyLabel::NONE;
#endif  //  end for LABELED
      }

      const CStateItem * p = this;
      while (p) {
        // if last action is al
        if (p->AfterArcLeft()) {
          heads[p->m_Previous->StackTopWord()] = p->m_Previous->BufferFrontWord();
#ifdef LABELED
          deprels[p->m_Previous->StackTopWord()] = action::getLabel(p->m_LastAction);
#endif  //  end for LABELED
        }
        else if (p->AfterArcRight()) {
          heads[p->m_Previous->BufferFrontWord()] = p->m_Previous->StackTopWord();
#ifdef LABELED
          deprels[p->m_Previous->BufferFrontWord()] =action::getLabel(p->m_LastAction);
#endif  //  end for LABELED
        }
        else if (p->AfterPopRoot()) {
#ifdef LABELED
          deprels[p->m_Previous->StackTopWord()] = CDependencyLabel::ROOT;
#endif  //  end for LABELED
        }

        p = p->m_Previous;
      }
    }

    friend std::ostream & operator >> (std::ostream & out, const CStateItem & item) {
      out << &item 
          << "::"         <<  item.Score()
          << " <- "       <<  item.m_Previous
          << " ["         <<  item.m_LastAction
          << "], stack:"  <<  item.m_Stack
          << "("          <<  item.StackSize() << ")"
          << ", top:"     <<  item.StackTopWord()
#ifdef LABELED
          << "/"          <<  item.StackTopLabel()
#endif  //  end for LABELED
          << ", buffer:"  <<  item.BufferFrontWord()
          << "\n\theadstack:"<< item.m_HeadStack
          << "("          <<  item.HeadStackSize() << ")"
          << ", headstacktop:" << item.HeadStackTopWord()
          << "\n\t"
          << "(parent:"   <<  item.StackTopParentWord()
#ifdef LABELED
          << "/"          <<  item.StackTopParentLabel()
#endif  //  end for LABELED
          << ", grand:"   <<  item.StackTopGrandWord()
          << ", futhest:" <<  item.StackTopFurthestGrandWord()
          << ")\n\t(lm:"  <<  item.StackTopLeftMostWord()
#ifdef LABELED
          << "/"          <<  item.StackTopLeftMostLabel()
#endif  //  end for LABELED
          << " ,l2m:"     <<  item.StackTopLeft2ndMostWord()
#ifdef LABELED
          << "/"          <<  item.StackTopLeft2ndMostLabel()
#endif  //  end for LABELED
          << " ,r2m:"     <<  item.StackTopRight2ndMostWord()
#ifdef LABELED
          << "/"          <<  item.StackTopRight2ndMostLabel()
#endif  //  end for LABELED
          << " ,rm:"      <<  item.StackTopRightMostWord()
#ifdef LABELED
          << "/"          <<  item.StackTopRightMostLabel()
#endif  //  end for LABELED
          << ") ("
          << "lm:"        <<  item.BufferFrontLeftMostWord()
#ifdef LABELED
          << "/"          <<  item.BufferFrontLeftMostLabel()
#endif  //  end for LABELED
          << " ,l2m:"     <<  item.BufferFrontLeft2ndMostWord()
#ifdef LABELED
          << "/"          <<  item.BufferFrontLeft2ndMostLabel()
#endif  //  end for LABELED
          << ")"          <<  std::endl;
      return out;
    }

  public:
    CStateItem *    m_Stack;
    CStateItem *    m_HeadStack;
    CStateItem *    m_Previous;
    CStateItem *    m_Next;
    int             m_StackSize;
    int             m_HeadStackSize;
    int             m_LastAction;

    // information of the word in this state
    int             m_StackTop_Word;
    int             m_StackTop_Label;
    int             m_StackTop_Parent_Word;
    int             m_StackTop_Parent_Label;
    int             m_StackTop_Grand_Word;
    int             m_StackTop_Furthest_Grand_Word;
    int             m_StackTop_Number_LeftChildren;
    int             m_StackTop_LeftMostChild_Word;
    int             m_StackTop_LeftMostChild_Label;
    int             m_StackTop_Left2ndMostChild_Word;
    int             m_StackTop_Left2ndMostChild_Label;
    int             m_StackTop_Number_RightChildren;
    int             m_StackTop_RightMostChild_Word;
    int             m_StackTop_RightMostChild_Label;
    int             m_StackTop_Right2ndMostChild_Word;
    int             m_StackTop_Right2ndMostChild_Label;
    int             m_BufferFront_Word;
    int             m_BufferFront_Label;
    int             m_BufferFront_Number_LeftChildren;
    int             m_BufferFront_LeftMostChild_Word;
    int             m_BufferFront_LeftMostChild_Label;
    int             m_BufferFront_Left2ndMostChild_Word;
    int             m_BufferFront_Left2ndMostChild_Label;
    int             m_HeadStackTop_Word;

    SCORE_TYPE      m_Score;

    CSetOfTags<CDependencyLabel> m_StackTop_LeftTags;
    CSetOfTags<CDependencyLabel> m_StackTop_RightTags;
    CSetOfTags<CDependencyLabel> m_BufferFront_LeftTags;

    const std::vector < CTaggedWord<CTag, TAG_SEPARATOR> >* m_lCache;
};

#endif  //  end for define _ZPAR_DEPENDENCY_PARSER_LIU_STATEITEM_H_
