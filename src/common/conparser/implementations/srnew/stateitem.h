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
   CStateNode *parent;
   NODE_TYPE type;
   bool temp;
   // fields for constituents
   CConstituent constituent;
   CStateNode* left_child;
   CStateNode* right_child;
   // fields for tokens and constituents
   int lexical_head;
//   int lexical_start;
//   int lexical_end;

public:
   inline bool head_left() const { return type==HEAD_LEFT; }
   inline bool single_child() const { return type==SINGLE_CHILD; }
   inline bool is_constituent() const { return type!=LEAF; }

public:
   CStateNode(const int &id, CStateNode* &parent, const NODE_TYPE &type, const bool &temp, const unsigned long &constituent, CStateNode *left_child, CStateNode *right_child, const int &lexical_head) : id(id), parent(parent), type(type), temp(temp), constituent(constituent), left_child(left_child), right_child(right_child), lexical_head(lexical_head) {}
   virtual ~CStateNode() {}
public:
   bool valid() { return id!=-1; }
   void clear() { 
      this->id = -1;
      this->parent = 0; 
      this->type = 0; 
      this->temp = 0; 
      this->constituent.clear(); 
      this->left_child = 0; 
      this->right_child = 0; 
      this->lexical_head = 0; 
   }
   void set(const int &id, CStateNode *parent, const NODE_TYPE &type, const bool &temp, const unsigned long &constituent, CStateNode *left_child, CStateNode *right_child, const int &lexical_head) { 
      this->id = id;
      this->parent = parent; 
      this->type = type; 
      this->temp = temp; 
      this->constituent = constituent; 
      this->left_child = left_child; 
      this->right_child = right_child; 
      this->lexical_head = lexical_head; 
   }//{}
   bool operator == (const CStateNode &nd) const {
      return id == nd.id &&
             parent == nd.parent && 
             type == nd.type && 
             temp == nd.temp && 
             constituent == nd.constituent && 
             left_child == nd.left_child && 
             right_child == nd.right_child &&
             lexical_head == nd.lexical_head;
   }
   void operator = (const CStateNode &nd) {
      id = nd.id;
      parent = nd.parent;
      type = nd.type;
      temp = nd.temp;
      constituent.copy(nd.constituent);
      left_child = nd.left_child;
      right_child = nd.right_child;
      lexical_head = nd.lexical_head;
   }
public:
   void toCCFGTreeNode(CCFGTreeNode &node) const {
      node.parent = parent->id;
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
      node.left_child = left_child->id;
      node.right_child = right_child->id;
      node.token = lexical_head;
   }
   void fromCCFGTreeNode(const CCFGTreeNode &node) {
      parent = node.parent;
      temp = node.temp;
      constituent.load(node.constituent);
      left_child = node.left_child;
      right_child = node.right_child;
      lexical_head = node.token;
      if (!node.is_constituent) {
         type = LEAF;
      }
      else {
         if (node.single_child) {
            type = SINGLE_CHILD;
         }
         else {
            if (node.head_left) {
               type = HEAD_LEFT;
            }
            else {
               type = HEAD_RIGHT;
            }
         }
      }
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
   CStateItem *stackPtr;
   int current_word;
   CAction action;
   const CContext *context;
   const vector< CTaggedWord<CTag, TAG_SEPARATOR> > *sent;
   
public:
   CStateItem() : current_word(0), score(0), action(), context(0), sent(0), stackPtr(0), node() {}
   virtual ~CStateItem() {}
public:
   void clear() {
      stackPtr = 0;
      current_word = 0;
      node.clear();
      score = 0;
      action = 0;
      context = 0;
      sent = 0;
   }
   bool empty() const {
      if (current_word==0) {
         assert(stackPtr == 0 && action==0 && score==0);
         return true;
      }
      return false;
   }
   unsigned stacksize() const {
      unsigned retval = 0;
      CStateItem *current = this;
      while (current) {
         if (current->node.valid()) ++retval;// no node -> start/fini
         current = current->stackPtr;
      }
      return retval;
   }
   unsigned unaryreduces() const {
      unsigned retval = 0;
      CStateItem *current = this;
      while (current) {
         if (current.action == CActionType::REDUCE_UNARY)
            ++retval;
         else
            return retval;
         current = current->stackPtr;
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
   void shift(CStateItem *retval, const unsigned long &constituent = CConstituent::NONE) {
      //TRACE("shift");
      assert(!IsTerminated());
      static int t;
      retval->node.set(node.id+1, 0, CStateNode::LEAF, false, constituent, 0, 0, current_word);
      retval->current_word = current_word+1;
      return->stackPtr = this; ///  
      assert(!retval->IsTerminated());
   }
   void reduce(CStateItem *retval, const unsigned long &constituent, const bool &single_child, const bool &head_left, const bool &temporary) {
      //TRACE("reduce");
      assert(!IsTerminated());
      static int c;         // the new node
      static CStateNode *l, *r;
      assert(stackPtr!=0);
      if (single_child) {
         assert(head_left == false);
         assert(temporary == false);
         l = &node;
         retval->node->set(node.id+1, -1, CStateNode::SINGLE_CHILD, false, constituent, l, 0, l->lexical_head);
         l->parent = &retval0>node;
         retval->stackPtr = stackPtr;
      }
      else {
         assert(stacksize()>=2);
         r = &node;
         l = &(stackPtr->node);
#ifdef NO_TEMP_CONSTITUENT
         retval->set(node.id+1, -1, (head_left?CStateNode::HEAD_LEFT:CStateNode::HEAD_RIGHT), temporary, constituent, l, r, (head_left?l->lexical_head:r->lexical_head));
#else
         retval->set(node.id+1, -1, (head_left?CStateNode::HEAD_LEFT:CStateNode::HEAD_RIGHT), temporary, CConstituent::encodeTmp(constituent, temporary), l, r, (head_left?l->lexical_head:r->lexical_head));
#endif
         l->parent = &(retval->node);
         r->parent = &(retval->node);
         retval->stackPtr = stackPtr->stackPtr;
      }
      assert(!IsTerminated());
   }
   void terminate(CStateItem *retval) {
      //TRACE("terminate");
      assert(IsComplete());
      assert(!IsTerminated());
      retval->clear();
      retval->stackPtr=this;
      assert(retval->IsTerminated());
   }

public:

   void NextMove(const CCFGTree &snt, CAction &retval) const {
      int s = stack.back();
      const CCFGTreeNode &nd = snt.nodes[s];
      const CCFGTreeNode &hd = snt.nodes[nd.parent];
      assert(hd.constituent!=CConstituent::NONE); // so that reduce and reduce_root are not same
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
            retval.encodeShift(snt.nodes[newNodeIndex()].constituent); return;
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
      retval.encodeReduce(hd.constituent, single_child, head_left, temporary);
   }
   void NextMove(const CStateItem &snt, CAction &retval) const {
      int s = stack.back();
      const CStateNode &nd = snt.nodes[s];
      const CStateNode &hd = snt.nodes[nd.parent];
      assert(hd.constituent.code()!=CConstituent::NONE); // so that reduce and reduce_root are not same
      bool single_child;
      bool head_left;
      bool temporary;
      // stack top single child ? reduce unary
      if (hd.single_child()) {
         single_child = true;
         head_left = false; assert(hd.head_left()==false);
         temporary = false; assert(hd.temp==false);
      }
      else {
         // stack top left child ? shift
         if (s == hd.left_child) {
            retval.encodeShift(snt.nodes[newNodeIndex()].constituent.code()); return;
         }
         // stack top right child ? reduce bin
         assert(s==hd.right_child);
         single_child = false;
         head_left = hd.head_left();
         temporary = hd.temp;
      }
      retval.encodeReduce(hd.constituent.code(), single_child, head_left, temporary);
   }

   void StandardMove(const CCFGTree &tr, CAction &retval) const {
      assert(!IsTerminated());
      assert(tr.words.size() == sent->size());
      // stack empty?shift
      if (stack.empty()) {
         retval.encodeShift(tr.nodes[newNodeIndex()].constituent);
         return;
      }
      int s = stack.back();
      if (tr.nodes[s].parent == -1) {
         assert(IsComplete());
         retval.encodeReduceRoot();
         return;
      }
      NextMove(tr, retval);
   }

   void Move(CStateItem *retval, const CAction &action) {
      retval->action = action;
      if (action.isShift())
         shift(retval, action.getConstituent());
      else if (action.isReduceRoot())
         { assert(IsComplete()); terminate(retval); }
      else
         reduce(retval, action.getConstituent(), action.singleChild(), action.headLeft(), action.isTemporary());
   }
   
   bool IsComplete() const {
#ifdef FRAGMENTED_TREE
      return current_word == sent->size(); // allow multiple-rt.
#else
      return current_word == sent->size() && stacksize() == 1;
#endif
   }

   bool IsTerminated() const {
      return action == CActionType::POP_ROOT; 
   }

   void GenerateTree(const CTwoStringVector &tagged, CSentenceParsed &out) const {
      // parsing done?
      //assert(IsComplete());
      assert(IsTerminated());
      assert(tagged.size()==sent->size());
      out.clear();
#ifdef FRAGMENTED_TREE
      if (stacksize()>1) {
         static CStateItem *item;
         item = stackPtr;
         static CStateItem *tmp;
         tmp = new CStateItem[stacksize()];
         static CStateItem *current;
         current = tmp;
         while (item->stacksize()>1) {
            // form NONE nodes
            item->reduce(current, CConstituent::NONE, false, false, false); 
            item = current;
            ++ current;
         }
         item->terminate(current);
         item = current;
         item->GenerateTree(tagged, out);
         delete tmp;
         return;
      }
#else
      if (stacksize()>1) { WARNING("Parser failed.");return; }
#endif
      // generate nodes for out
      static int i,j;
      // first words
      for (i=0; i<tagged.size(); ++i) 
         out.newWord(tagged[i].first, tagged[i].second);
      // second constituents
      static CStateNode* nodes[MAX_SENTENCE_SIZE*(2+UNARY_REDUCE)+2];
      static int count;
      count = 0;
      static CStateItem *current;
      current = this;
      while (current) {
         if (current->node.valid()) nodes[count] = &current->node;
         current = current->stackPtr;
         ++count; 
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
      static CStateItem* states[MAX_SENTENCE_SIZE*(2+UNARY_REDUCE)+2];
      static int count;
      static CStateItem *current;
      count = 0;
      current = this;
      while (current) {
         if (current) states[count] = current;
         ++count ; //updating
         current = current->stackPtr;
      }
      TRACE("State item score == " << score);
      --count;
      while (count>=0) {
         if (s) {
            TRACE(states[count]->action.str()<<" ["<<(states[count]->stacksize()>0?s->at(states[count]->node.lexical_head).first:"")<<"]"); 
         }
         else {
            TRACE(states[count]->action.str());
         }
      }
      TRACE("");
   }
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
   CScoredAction() : action(), score(0) {}
   void load(const CAction &action, const CStateItem *item, const SCORE_TYPE &score) {
      this->action = action; 
      this->item = item;
      this->score = score;
   }

public:
   bool operator < (const CScoredAction &a1) const { return score < a1.score; }
   bool operator > (const CScoredAction &a1) const { return score > a1.score; }
   bool operator <= (const CScoredAction &a1) const { return score <= a1.score; }
   bool operator >= (const CScoredAction &a1) const { return score >= a1.score; }

};

#endif
