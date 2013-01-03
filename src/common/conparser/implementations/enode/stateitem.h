// Copyright (C) University of Oxford 2010
#ifndef _COMMON_CON_PARSER_STATEITEM
#define _COMMON_CON_PARSER_STATEITEM

#include "action.h"

/*===============================================================
 *
 * CStateNode - tree nodes 
 *
 *==============================================================*/

class CStateNode {
public:
   enum NODE_TYPE {LEAF=0, SINGLE_CHILD, HEAD_LEFT, HEAD_RIGHT};
public:
   int id;
   NODE_TYPE type;
   bool temp;
   // fields for constituents
   CConstituent constituent;
   const CStateNode* left_child;
   const CStateNode* right_child;
   const CTaggedWord<CTag, TAG_SEPARATOR>* lexical_head;
   unsigned int lexical_head_index;
   const CStateNode * word_prev;
   const CStateNode * word_last;


public:
   inline bool head_left() const { return type==HEAD_LEFT; }
   inline bool single_child() const { return type==SINGLE_CHILD; }
   inline bool is_constituent() const { return type!=LEAF; }

public:
   CStateNode(const int &id, const NODE_TYPE &type, const bool &temp, const unsigned long &constituent, CStateNode *left_child, CStateNode *right_child, const CTaggedWord<CTag, TAG_SEPARATOR>* lexical_head, const int lexical_head_index, const CStateNode* word_prev, const CStateNode* word_last) : id(id), type(type), temp(temp), constituent(constituent), left_child(left_child), right_child(right_child), lexical_head(lexical_head), lexical_head_index(lexical_head_index), word_prev(word_prev), word_last(word_last) {}
   CStateNode() : id(-1), type(), temp(0), constituent(), left_child(0), right_child(0), lexical_head(0), lexical_head_index(-1), word_prev(0), word_last(this) {}
   virtual ~CStateNode() {}
public:
   bool valid() const { return id!=-1; }
   void clear() { 
      this->id = -1;
      this->type = static_cast<NODE_TYPE>(0); 
      this->temp = 0; 
      this->constituent.clear(); 
      this->left_child = 0; 
      this->right_child = 0; 
      this->lexical_head = 0; 
      this->lexical_head_index = -1;
      this->word_prev = 0;
      this->word_last = this;
   }
   void set(const int &id, const NODE_TYPE &type, const bool &temp, const unsigned long &constituent, const CStateNode *left_child, const CStateNode *right_child, const CTaggedWord<CTag, TAG_SEPARATOR>* lexical_head, const int lexical_head_index, const CStateNode *word_prev, const CStateNode * word_last) { 
      this->id = id;
      this->type = type; 
      this->temp = temp; 
      this->constituent = constituent; 
      this->left_child = left_child; 
      this->right_child = right_child; 
      this->lexical_head = lexical_head; 
      this->lexical_head_index = lexical_head_index;
      this->word_prev = word_prev;
      this->word_last = word_last;
   }//{}
   bool operator == (const CStateNode &nd) const {
      return id == nd.id &&
             type == nd.type && 
             temp == nd.temp && 
             constituent == nd.constituent && 
             left_child == nd.left_child && 
             right_child == nd.right_child &&
             lexical_head == nd.lexical_head &&
             lexical_head_index == nd.lexical_head_index && 
             word_prev == nd.word_prev &&
             word_last == nd.word_last;
   }
   void operator = (const CStateNode &nd) {
      id = nd.id;
      type = nd.type;
      temp = nd.temp;
      constituent.copy(nd.constituent);
      left_child = nd.left_child;
      right_child = nd.right_child;
      lexical_head = nd.lexical_head;
      lexical_head_index = nd.lexical_head_index;
      word_prev = nd.word_prev;
      word_last = nd.word_last;
   }
public:
   void toCCFGTreeNode(CCFGTreeNode &node) const {
//      node.parent = parent->id;
      node.is_constituent = is_constituent();
      node.temp = temp;
#ifdef NO_TEMP_CONSTITUENT
      ASSERT(!node.temp, "Internal error: this version does not temporary constituents but state items have them.")
      node.constituent = constituent.code();
#else
      node.constituent = constituent.extractConstituentCode();
#endif
      node.single_child = single_child();
      node.head_left = head_left();
      node.left_child = left_child ? left_child->id : -1;
      node.right_child = right_child ? right_child->id : -1;
      node.token = lexical_head_index;
   }
};

/*===============================================================
 *
 * CStateItem - the search state item, representing a partial
 *              candidate with shift reduce. 
 *
 *==============================================================*/

class CContext;

class CStateItem {
public:
   SCORE_TYPE score;
   CStateNode node;
   const CStateItem *statePtr;
   const CStateItem *stackPtr;
   int current_word;
   CAction action;
   const std::vector<CTaggedWord<CTag, TAG_SEPARATOR> > *m_lCache; 
   const CTaggedWord<CTag, TAG_SEPARATOR> *m_lEmptyWords; 
#ifdef SCALE
   unsigned size;
#endif
   
public:
#define LOSS_CON 
#ifdef SCALE
#define SCALE_CON , size(0)
#else
#define SCALE_CON 
#endif
   CStateItem() : current_word(0), score(0), action(), stackPtr(0), statePtr(0), node(), m_lCache(0), m_lEmptyWords(0) LOSS_CON SCALE_CON {}
   virtual ~CStateItem() {}
public:
   void clear() {
      statePtr = 0;
      stackPtr = 0;
      current_word = 0;
      node.clear();
      score = 0;
      action.clear();
      m_lCache = 0;
      m_lEmptyWords = 0;
#ifdef SCALE
      size = 0;
#endif
   }
   bool empty() const {
      if (current_word==0) {
         assert(stackPtr == 0 && statePtr == 0 && score==0);
         return true;
      }
      return false;
   }
   unsigned stacksize() const {
      unsigned retval = 0;
      const CStateItem *current = this;
      while (current) {
         if (current->node.valid()) ++retval;// no node -> start/fini
         current = current->stackPtr;
      }
      return retval;
   }
   unsigned unaryreduces() const {
      unsigned retval = 0;
      const CStateItem *current = this;
      while (current) {
         if (current->action.type() == CActionType::REDUCE_UNARY)
            ++retval;
         else
            return retval;
         current = current->statePtr;
      }
      return retval;
   }

   unsigned empty_shifts() const {
      unsigned retval = 0;
      const CStateItem *current = this;
      while (current) {
         if (current->action.type() == CActionType::SHIFT_EMPTY)
            ++retval;
         else
            return retval;
         current = current->statePtr;
      }
      return retval;
   }

   int newNodeIndex() const { return node.id+1; }

public:
   bool operator < (const CStateItem &st) const { return score < st.score; }
   bool operator > (const CStateItem &st) const { return score > st.score; }
   bool operator == (const CStateItem &st) const {
      THROW("State items are not comparable with each other");
   }
   bool operator != (const CStateItem &st) const { return !( (*this) == st ); }

protected:
   // now this actions MUST BE called by Move
   void shift(CStateItem *retval, const unsigned long &constituent = CConstituent::NONE) const {
      //TRACE("shift");
      assert(!IsTerminated());
      retval->node.set(node.id+1, CStateNode::LEAF, false, constituent, 0, 0, &((*m_lCache)[current_word]), ((this->node.word_last))->lexical_head_index+1, (this->node).word_last, &(retval->node));
      retval->current_word = current_word+1;
      retval->stackPtr = this; ///  
      assert(!retval->IsTerminated());

   }
   void shift_empty(CStateItem *retval, const unsigned long &index) const {
      //TRACE("shift_empty");
      assert(!IsTerminated());
      retval->node.set(node.id+1, CStateNode::LEAF, false, CConstituent::NONE, 0, 0, m_lEmptyWords+index, ((this->node).word_last)->lexical_head_index+1, (this->node).word_last, &(retval->node));
      retval->current_word = current_word;
      retval->stackPtr = this; ///  
      assert(!retval->IsTerminated());
   }
   void reduce(CStateItem *retval, const unsigned long &constituent, const bool &single_child, const bool &head_left, const bool &temporary) const {
      //TRACE("reduce");
      assert(!IsTerminated());
      const static CStateNode *l, *r;
      assert(stackPtr!=0);
      if (single_child) {
         assert(head_left == false);
         assert(temporary == false);
         l = &node;
         retval->node.set(node.id+1, CStateNode::SINGLE_CHILD, false, constituent, l, 0, l->lexical_head, l->lexical_head_index, l->word_prev, l->word_last);
         retval->stackPtr = stackPtr;
      }
      else {
         static unsigned long fullconst; 
         assert(stacksize()>=2);
         r = &node;
         l = &(stackPtr->node);
#ifdef NO_TEMP_CONSTITUENT
         fullconst = constituent;
#else
         fullconst = CConstituent::encodeTmp(constituent, temporary);
#endif
         retval->node.set(node.id+1, (head_left?CStateNode::HEAD_LEFT:CStateNode::HEAD_RIGHT), temporary, fullconst, l, r, (head_left?l->lexical_head:r->lexical_head), (head_left?l->lexical_head_index:r->lexical_head_index), (head_left?l->word_prev: r->word_prev), r->word_last);
         retval->stackPtr = stackPtr->stackPtr;
      }
      retval->current_word = current_word;
      assert(!IsTerminated());
   }
   void terminate(CStateItem *retval) const {
      //TRACE("terminate");
//      assert(IsComplete());
      assert(!IsTerminated());
      retval->node = this->node;
      retval->stackPtr=this->stackPtr;
      retval->current_word = current_word;
      // compute loss
      assert(retval->IsTerminated());
   }
   void noact(CStateItem *retval) const {
      //TRACE("noact");
      assert(IsTerminated());
      retval->node = this->node;
      retval->stackPtr=this->stackPtr;
      retval->current_word = current_word;
      // compute loss
   }

protected:

public:

   void NextMove(const CCFGTree &snt, CAction &retval) const {
      int s = node.id;
      const CCFGTreeNode &nd = snt.nodes[s];
      const CCFGTreeNode &hd = snt.nodes[snt.parent(s)];
      assert(!hd.constituent.empty()); // so that reduce and reduce_root are not same
      bool single_child;
      bool head_left;
      bool temporary;
      // stack top single child ? reduce unary
      if (hd.single_child) {
         single_child = true;
         head_left = false; assert(hd.head_left==false);
         temporary = false; assert(hd.temp==false);
      }
      else {
         // stack top left child ? shift
         if (s == hd.left_child) {
             if(snt.words[snt.nodes[newNodeIndex()].token].first == "-NONE-")
             {
                 retval.encodeShiftEmpty(CTag(snt.words[snt.nodes[newNodeIndex()].token].second).code()); return;
             }
             else
             {
                retval.encodeShift(snt.nodes[newNodeIndex()].constituent.code()); return;
             }
         }
         // stack top right child ? reduce bin
         assert(s==hd.right_child);
         single_child = false;
         head_left = hd.head_left;
         temporary = hd.temp;
#ifdef NO_TEMP_CONSTITUENT
         ASSERT(!temporary, "This version does not accept temporary constituents, but the training data give them.");
#endif
      }
      retval.encodeReduce(hd.constituent.code(), single_child, head_left, temporary);
   }

   void StandardMove(const CCFGTree &tr, CAction &retval) const {
      if (IsTerminated()) {
         retval.encodeIdle();
         return;
      }
      // stack empty?shift
      if (stacksize()==0) {
          if(tr.words[tr.nodes[newNodeIndex()].token].first == "-NONE-")
          {
              retval.encodeShiftEmpty(CTag(tr.words[tr.nodes[newNodeIndex()].token].second).code()); return;
          }
          else
          {
             retval.encodeShift(tr.nodes[newNodeIndex()].constituent.code()); return;
          }
      }
      if (tr.parent(node.id) == -1) {
         assert(IsComplete(m_lCache->size()));
         retval.encodeReduceRoot();
         return;
      }
      NextMove(tr, retval);
   }

   void Move(CStateItem *retval, const CAction &action) const {
      retval->action = action; // this makes it necessary for the actions to 
      retval->statePtr = this; // be called by Move

      retval->m_lCache = m_lCache;
      retval->m_lEmptyWords = m_lEmptyWords;

      if (action.isIdle()) {
         noact(retval);
#ifdef SCALE
         retval->size = this->size;
#endif
      }
      else {
         if (action.isShift())
            {shift(retval, action.getConstituent()); }
         else if(action.isShiftEmpty())
         {
             shift_empty(retval, action.getConstituent()-PENN_EMPTY_TAG_FIRST);
         }
         else if (action.isReduceRoot())
            { terminate(retval); }
         else
            { reduce(retval, action.getConstituent(), action.singleChild(), action.headLeft(), action.isTemporary()); }
#ifdef SCALE
         retval->size = this->size + 1;
#endif
      }
   }
   
   bool IsComplete(const int &nWords) const {
#ifdef FRAGMENTED_TREE
      return current_word == nWords; // allow multiple-rt.
#else
      return current_word == nWords && stacksize() == 1;
#endif
   }

   bool IsTerminated() const {
      return action.type() == CActionType::POP_ROOT or action.type() == CActionType::IDLE; 
   }

   bool IsIdle() const {
      return action.type() == CActionType::IDLE; 
   }

   void GenerateTree(const CTwoStringVector &tagged, CSentenceParsed &out) const {
      // parsing done?
      assert(IsTerminated());
//      assert(tagged.size()==sent->size());
      out.clear();
#ifdef FRAGMENTED_TREE
      if (stacksize()>1) {
         static const CStateItem *item;
         item = statePtr;
         assert(item==stackPtr);
         static CStateItem *tmp;
         tmp = new CStateItem[stacksize()];
         static CStateItem *current;
         current = tmp;
         static CAction action;
         action.encodeReduce(CConstituent::NONE, false, false, false);
         while (item->stacksize()>1) {
            // form NONE nodes
            item->Move(current, action); 
            item = current;
            ++ current;
         }
         action.encodeReduceRoot();
         item->Move(current, action);
         item = current;
         item->GenerateTree(tagged, out);
         delete [] tmp;
         return;
      }
#else
      if (stacksize()>1) { WARNING("Parser failed.");return; }
#endif
      // generate nodes for out
      static int i,j;
      // first words
      std::vector<const CTaggedWord<CTag, TAG_SEPARATOR>* > tmp;
      const CStateNode* current_word;
      //current_word = this->node.word_last;
      current_word = node.right_child==0 ? node.left_child->word_last : node.right_child->word_last;
      while(current_word->valid())
      {
          tmp.push_back(current_word->lexical_head);
          current_word = current_word->word_prev;
      }

      for(i=tmp.size()-1; i>=0; --i)
      {
          out.newWord((tmp[i]->word).str(), (tmp[i]->tag).str());
      }
      // second constituents
      static const CStateNode* nodes[MAX_SENTENCE_SIZE*(1+EMPTY_SHIFT_MOVES)*(2+UNARY_MOVES)+2];
      static int count;
      count = 0;
      const static CStateItem *current;
      current = this;
      while (current) {
         if (!current->IsTerminated() && current->node.valid()) {
            nodes[count] = &current->node;
            ++count; 
         }
         current = current->statePtr;
      }

      for (i=count-1; i>=0; --i) {
         j = out.newNode();
         // copy node
         assert(j==nodes[i]->id);
         nodes[i]->toCCFGTreeNode(out.nodes[j]);
      }
      out.root = nodes[0]->id;
   }

   //===============================================================================

   void trace(const CTwoStringVector *s=0) const {
      static const CStateItem* states[MAX_SENTENCE_SIZE*(1+EMPTY_SHIFT_MOVES)*(2+UNARY_MOVES)+2];
      static int count;
      const static CStateItem *current;
      count = 0;
      current = this;
      while (current->statePtr) {
         states[count] = current;
         ++count ; //updating
         current = current->statePtr;
      }
      TRACE("State item score == " << score);
      TRACE("State item size == " << size);
      --count;
      while (count>=0) {
         if (s) {
            //TRACE(states[count]->action.str()<<" ["<<(states[count]->stacksize()>0?s->at(states[count]->node.lexical_head).first:"")<<"]"); 
         }
         else {
            TRACE(states[count]->action.str());
         }
         --count;
      }
      TRACE("");
   }

   //===============================================================================
};

/*===============================================================
 *
 * scored actions
 *
 *==============================================================*/

class CScoredStateAction {

public:
   CAction action;
   const CStateItem *item;
   SCORE_TYPE score;

public:
   CScoredStateAction() : item(0), action(), score(0) {}
   void load(const CAction &action, const CStateItem *item, const SCORE_TYPE &score) {
      SCORE_TYPE item_sc;
      this->action = action; 
      this->item = item;
      item_sc = item->score;
#ifdef SCALE
      item_sc *= item->size;
#endif
#define LOSS_ADD 
      this->score = item_sc + score LOSS_ADD;
#ifdef SCALE
      this->score /= (item->size + 1);
#endif
   }

public:
   bool operator < (const CScoredStateAction &a1) const { return score < a1.score; }
   bool operator > (const CScoredStateAction &a1) const { return score > a1.score; }
   bool operator <= (const CScoredStateAction &a1) const { return score <= a1.score; }
   bool operator >= (const CScoredStateAction &a1) const { return score >= a1.score; }

};

#endif
