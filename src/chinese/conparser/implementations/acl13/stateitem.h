// Copyright (C) University of Oxford 2010
#ifndef _CHINESE_CON_PARSER_STATEITEM
#define _CHINESE_CON_PARSER_STATEITEM

#include "action.h"

/*===============================================================
 *
 * CStateNode - tree nodes
 *
 *==============================================================*/

class CStateNode {
public:
   enum NODE_TYPE {LEAF=0, SINGLE_CHILD, HEAD_LEFT, HEAD_RIGHT, PARTIAL_X, PARTIAL_Y, PARTIAL_Z, CHAR_B, CHAR_I};
public:
   int id;
   NODE_TYPE type;
   bool temp;
   // fields for constituents
   CConstituent constituent;
   unsigned pos;
   const CStateNode* left_child;
   const CStateNode* right_child;

   const CStateNode * word_prev;
   const CStateNode * word_last;
   const CStateNode * word_head;
   unsigned begin_c, end_c, head_c;


public:
   inline bool head_left() const { return type==HEAD_LEFT || type==PARTIAL_X || type==PARTIAL_Z; }
   inline bool single_child() const { return type==SINGLE_CHILD || type==LEAF; }
   inline bool is_constituent() const { return type==SINGLE_CHILD||type==HEAD_LEFT||type==HEAD_RIGHT; }
   inline bool is_partial() const { return !is_constituent() && !is_leaf(); }
   inline bool is_leaf() const { return type==LEAF; }
   //inline unsigned long nodetype() const{ return  type + 1;  }

public:
   CStateNode(const int &id, const NODE_TYPE &type, const bool &temp, const unsigned long &constituent, const unsigned &pos, const CStateNode *left_child, const CStateNode *right_child, const CStateNode *word_prev, const CStateNode *word_last, const CStateNode *word_head, const unsigned &begin_c, const unsigned &end_c, const unsigned &head_c) : id(id), type(type), temp(temp), constituent(constituent), pos(pos), left_child(left_child), right_child(right_child), word_prev(word_prev), word_last(word_last), word_head(word_head) {}
   CStateNode() : id(-1), type(CHAR_B), temp(0), constituent(CConstituent::NONE), pos(CTag::NONE), left_child(0), right_child(0), word_prev(0), word_last(0), word_head(0), begin_c(0), end_c(0), head_c(0) {}
   virtual ~CStateNode() {}
public:
   bool valid() const { return id!=-1; }
   void clear() {
      this->id = -1;
      this->type = CHAR_B;
      this->temp = 0;
      this->constituent.clear();
      this->pos = CTag::NONE;
      this->left_child = 0;
      this->right_child = 0;
      this->word_head = 0;
      this->word_prev = 0;
      this->word_last = 0;
      this->begin_c = -1;
      this->end_c = -1;
      this->head_c = -1;
   }
   void set(const int &id, const NODE_TYPE &type, const bool &temp, const unsigned long &constituent, const unsigned &pos, const CStateNode *left_child, const CStateNode *right_child, const CStateNode *word_prev, const CStateNode *word_last, const CStateNode *word_head, const unsigned &begin_c, const unsigned &end_c, const unsigned &head_c) {
      this->id = id;
      this->type = type;
      this->temp = temp;
      this->constituent = constituent;
      this->left_child = left_child;
      this->right_child = right_child;
      this->pos = pos;
      this->word_head = word_head;
      this->word_prev = word_prev;
      this->word_last = word_last;
      this->begin_c = begin_c;
      this->end_c = end_c;
      this->head_c = head_c;
   }//{}
   bool operator == (const CStateNode &nd) const {
      return id == nd.id &&
             type == nd.type &&
             temp == nd.temp &&
             constituent == nd.constituent &&
             pos == nd.pos &&
             left_child == nd.left_child &&
             right_child == nd.right_child &&
             word_head == nd.word_head &&
             word_prev == nd.word_prev &&
             word_last == nd.word_last &&
             begin_c == nd.begin_c &&
             end_c == nd.end_c &&
             head_c == nd.head_c ;
   }
   void operator = (const CStateNode &nd) {
      id = nd.id;
      type = nd.type;
      temp = nd.temp;
      constituent.copy(nd.constituent);
      pos = nd.pos;
      left_child = nd.left_child;
      right_child = nd.right_child;
      word_head = nd.word_head;
      word_prev = nd.word_prev;
      word_last = nd.word_last;
      begin_c = nd.begin_c;
      end_c = nd.end_c;
      head_c = nd.head_c;
   }
public:
   void toCJointTreeNode(CJointTreeNode &node) const {
//      node.parent = parent->id;
      node.temp = temp;

      if(is_constituent())
      {
#ifdef NO_TEMP_CONSTITUENT
      ASSERT(!node.temp, "Internal error: this version does not temporary constituents but state items have them.")
      node.label = CConstituent(constituent.code()).str();
#else
      node.label = CConstituent(constituent.extractConstituentCode()).str();
#endif
      }
      else
      {
      	node.label = CTag(pos).str();
      }
      switch(type)
      {
      case LEAF :
      	node.type = 't';
      	break;
      case HEAD_LEFT :
      	node.type = 'l';
      	break;
      case HEAD_RIGHT :
      	node.type = 'r';
         break;
      case SINGLE_CHILD :
      	node.type = 's';
         break;
      case PARTIAL_X :
      	node.type = 'x';
         break;
      case PARTIAL_Y :
      	node.type = 'y';
         break;
      case PARTIAL_Z :
      	node.type = 'z';
         break;
      case CHAR_B :
      	node.type = 'b';
         break;
      case CHAR_I :
      	node.type = 'i';
         break;
      default:
      	THROW("unknown statenode type");
      }
      node.left_child = left_child ? left_child->id : -1;
      node.right_child = right_child ? right_child->id : -1;
      node.token = head_c;
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
   CStateItem() : current_word(0), score(0), action(), stackPtr(0), statePtr(0), node() LOSS_CON SCALE_CON {}
   virtual ~CStateItem() {}
public:
   void clear() {
      statePtr = 0;
      stackPtr = 0;
      current_word = 0;
      node.clear();
      score = 0;
      action.clear();
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
   void shiftS(CStateItem *retval, const unsigned long &pos) const {
      //TRACE("shift");
      assert(!IsTerminated());
      assert(stacksize() == 0 || !node.is_partial());
      retval->node.set(node.id+1, CStateNode::CHAR_B, false, CConstituent::NONE, pos, 0, 0, ( (stackPtr != 0 && stackPtr->node.valid())?stackPtr->node.word_last:0), &(retval->node), &(retval->node), current_word, current_word, current_word);
      retval->current_word = current_word+1;
      retval->stackPtr = this; ///
      assert(!retval->IsTerminated());

   }
   void shiftA(CStateItem *retval) const {
      //TRACE("shift_empty");
      assert(!IsTerminated());
      assert(node.is_partial());
      retval->node.set(node.id+1, CStateNode::CHAR_I, false, CConstituent::NONE, node.pos, 0, 0, (stackPtr->node.valid()?stackPtr->node.word_last:0), &(retval->node), &(retval->node), current_word, current_word, current_word);
      retval->current_word = current_word+1;
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
         assert(!l->is_partial());
         retval->node.set(node.id+1, CStateNode::SINGLE_CHILD, false, constituent, l->pos, l, 0, l->word_prev, l->word_last, l->word_head, l->begin_c, l->end_c, l->head_c);
         retval->stackPtr = stackPtr;
      }
      else {
         static unsigned long fullconst;
         assert(stacksize()>=2);
         r = &node;
         l = &(stackPtr->node);
         assert(!l->is_partial() && !r->is_partial());
#ifdef NO_TEMP_CONSTITUENT
         fullconst = constituent;
#else
         fullconst = CConstituent::encodeTmp(constituent, temporary);
#endif
         retval->node.set(node.id+1, (head_left?CStateNode::HEAD_LEFT:CStateNode::HEAD_RIGHT), temporary, fullconst, (head_left?l->pos:r->pos), l, r, (head_left?l->word_prev: r->word_prev), r->word_last, (head_left?l->word_head: r->word_head), l->begin_c, r->end_c, (head_left?l->head_c: r->head_c));
         retval->stackPtr = stackPtr->stackPtr;
      }
      retval->current_word = current_word;
      assert(!IsTerminated());
   }

   void wordXYZ(CStateItem *retval, const bool &no_head, const bool &head_left) const {
   	assert(!IsTerminated());
   	const static CStateNode *l, *r;
   	assert(stackPtr!=0);
   	assert(stacksize()>=2);
		r = &node;
		l = &(stackPtr->node);
		assert(l->is_partial() && r->is_partial());
		assert(l->pos == r->pos);
		retval->node.set(node.id+1, (no_head?CStateNode::PARTIAL_X:(head_left?CStateNode::PARTIAL_Z:CStateNode::PARTIAL_Y)), false, CConstituent::NONE, l->pos, l, r, (stackPtr->node.valid()?stackPtr->node.word_last:0), &(retval->node), &(retval->node), l->begin_c, r->end_c, (no_head?l->head_c:(head_left?l->head_c: r->head_c)));
		retval->stackPtr = stackPtr->stackPtr;
		retval->current_word = current_word;

   	assert(!IsTerminated());
   }

   void wordT(CStateItem *retval) const {
   	assert(!IsTerminated());
   	assert(stackPtr!=0);
   	assert(!stackPtr->node.valid() || !stackPtr->node.is_partial());
   	assert(node.is_partial());
		retval->node.set(node.id+1, CStateNode::LEAF, false, CConstituent::NONE, node.pos, &node, 0, (stackPtr->node.valid()?stackPtr->node.word_last:0), &(retval->node), &(retval->node), node.begin_c, node.end_c, node.head_c);
   	retval->stackPtr = stackPtr;
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

   void NextMove(const CJointTree &snt, CAction &retval) const {
      int s = node.id;
      const CJointTreeNode &nd = snt.nodes[s];
      const CJointTreeNode &hd = snt.nodes[snt.parent(s)];
      //assert(!hd.constituent.empty()); // so that reduce and reduce_root are not same
      bool single_child;
      bool head_left;
      bool temporary;
      // stack top single child ? reduce unary
      if(hd.is_constituent())
      {
			if (hd.single_child()) {
				single_child = true;
				head_left = false; assert(hd.head_left()==false);
				temporary = false; assert(hd.temp==false);
			}
			else {
				// stack top left child ? shift
				if (s == hd.left_child) {
					retval.encodeShiftS(CTag(snt.nodes[newNodeIndex()].label).code()); return;
				}
				// stack top right child ? reduce bin
				assert(s==hd.right_child);
				single_child = false;
				head_left = hd.head_left();
				temporary = hd.temp;
#ifdef NO_TEMP_CONSTITUENT
			ASSERT(!temporary, "This version does not accept temporary constituents, but the training data give them.");
#endif
			}
			retval.encodeReduce(CConstituent(hd.label).code(), single_child, head_left, temporary);
      }
      else
      {
      	if (hd.type == 't') {
      		retval.encodeWORDT();
			}
      	else
      	{
      		assert(hd.type == 'x' || hd.type == 'y' || hd.type == 'z');
      		if (s == hd.left_child) {
					retval.encodeShiftA();
				}
      		else
      		{
      			retval.encodeWORDXYZ(hd.type);
      		}
      	}
      }

   }

   void StandardMove(const CJointTree &tr, CAction &retval) const {
      if (IsTerminated()) {
         retval.encodeIdle();
         return;
      }
      // stack empty?shift
      if (stacksize()==0) {
         retval.encodeShiftS(CTag(tr.nodes[newNodeIndex()].label).code()); return;
      }
      if (tr.parent(node.id) == -1) {
         //assert(IsComplete(m_lCache->size()));
         retval.encodeReduceRoot();
         return;
      }
      NextMove(tr, retval);
   }

   void Move(CStateItem *retval, const CAction &action) const {
      retval->action = action; // this makes it necessary for the actions to
      retval->statePtr = this; // be called by Move

      if (action.isIdle()) {
         noact(retval);
#ifdef SCALE
         retval->size = this->size;
#endif
      }
      else {
         if (action.isShiftS())
         {
         	shiftS(retval, action.getPOS());
         }
         else if(action.isShiftA())
         {
         	shiftA(retval);
         }
         else if(action.isWordT())
         {
         	wordT(retval);
         }
         else if(action.isWordXYZ())
         {
         	wordXYZ(retval, action.noHead(), action.headLeft());
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

   void GenerateTree(const CStringVector &sent, CSentenceParsed &out) const {
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

      for(i=0; i<sent.size(); ++i)
      {
          out.newChar(sent[i]);
      }
      // second constituents
      static const CStateNode* nodes[MAX_SENTENCE_SIZE*(2+UNARY_MOVES)+2];
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
         nodes[i]->toCJointTreeNode(out.nodes[j]);
      }
      out.root = nodes[0]->id;
   }

   //===============================================================================

   void trace(const CStringVector *s=0) const {
      static const CStateItem* states[MAX_SENTENCE_SIZE*(2+UNARY_MOVES)+2];
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
      //TRACE("State item size == " << size);
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


   void debugtrace(const CStringVector *s=0) const {
      static const CStateItem* states[MAX_SENTENCE_SIZE*(2+UNARY_MOVES)+2];
      static int count;
      const static CStateItem *current;
      count = 0;
      current = this;
      while (current->statePtr) {
         states[count] = current;
         ++count ; //updating
         current = current->statePtr;
      }
      std::cerr << "State item score == " << score << std::endl;
      //TRACE("State item size == " << size);
      --count;
      while (count>=0) {
         //if (s) {
         //   //TRACE(states[count]->action.str()<<" ["<<(states[count]->stacksize()>0?s->at(states[count]->node.lexical_head).first:"")<<"]");
         //}
         //else {
            std::cerr << states[count]->action.str() <<" " << states[count]->score << std::endl;
         //}
         --count;
      }
      //TRACE("");
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
