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
   int parent;
   NODE_TYPE type;
   bool temp;
   // fields for constituents
   CConstituent constituent;
   int left_child;
   int right_child;
   // fields for tokens and constituents
   int lexical_head;
//   int lexical_start;
//   int lexical_end;

public:
   inline bool head_left() const { return type==HEAD_LEFT; }
   inline bool single_child() const { return type==SINGLE_CHILD; }
   inline bool is_constituent() const { return type!=LEAF; }

public:
   CStateNode(const int &parent, const NODE_TYPE &type, const bool &temp, const unsigned long &constituent, const int &left_child, const int &right_child, const int &lexical_head) : parent(parent), type(type), temp(temp), constituent(constituent), left_child(left_child), right_child(right_child), lexical_head(lexical_head) {}
   virtual ~CStateNode() {}
public:
   bool operator == (const CStateNode &nd) const {
      return parent == nd.parent && 
             type == nd.type && 
             temp == nd.temp && 
             constituent == nd.constituent && 
             left_child == nd.left_child && 
             right_child == nd.right_child &&
             lexical_head == nd.lexical_head;
   }
   void operator = (const CStateNode &nd) {
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
      node.parent = parent;
      node.is_constituent = is_constituent();
      node.temp = temp;
      node.constituent = constituent.code();
      node.single_child = single_child();
      node.head_left = head_left();
      node.left_child = left_child;
      node.right_child = right_child;
      node.token = lexical_head;
#ifdef NO_TEMP_CONSTITUENT
      ASSERT(!node.temp, "Internal error: this version does not temporary constituents but state items have them.")
#endif
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
   vector<CStateNode> nodes;
   vector<int> stack;
   int current_word;
   int unary_reduce; // unary_reduce stores two things: non-negative value is the number of unary_reduces consecutively, -1 means that the state item is finished.
   const CContext *context;
   const vector< CTaggedWord<CTag, TAG_SEPARATOR> > *sent;
   
public:
   CStateItem() {clear();}
   virtual ~CStateItem() {}
public:
   void clear() {
      stack.clear();
      nodes.clear();
      current_word = 0;
      score = 0;
      unary_reduce = 0;
      context = 0;
      sent = 0;
   }
   int newNode(const int &parent, const CStateNode::NODE_TYPE &type, const bool &tmp, const unsigned long &constituent, const int &left_child, const int &right_child, const int &lexical_head) { nodes.push_back(CStateNode(parent, type, tmp, constituent, left_child, right_child, lexical_head)); return nodes.size()-1; }
   int newNodeIndex() const { return nodes.size(); }

public:
   void shift(const unsigned long &constituent = CConstituent::NONE) {
      //TRACE("shift");
      assert(!IsTerminated());
      static int t;
      t = newNode(-1, CStateNode::LEAF, false, constituent, -1, -1, current_word);
//      nodes[t].lexical_start = current_word;
//      nodes[t].lexical_end = current_word;
//      nodes[t].lexical_head = current_word ++;
      ++current_word;
      unary_reduce = 0;
      stack.push_back(t);
      assert(!IsTerminated());
   }
   void reduce(const unsigned long &constituent, const bool &single_child, const bool &head_left, const bool &temporary) {
      //TRACE("reduce");
      assert(!IsTerminated());
      static int c;         // the new node
      static int l, r;
      assert(stack.size()>=0);
      if (single_child) {
         assert(head_left == false);
         assert(temporary == false);
         l = stack.back();
         stack.pop_back();
         c = newNode(-1, CStateNode::SINGLE_CHILD, false, constituent, l, -1, nodes[l].lexical_head);
         nodes[l].parent = c;
//         nodes[c].lexical_start = nodes[l].lexical_start;
//         nodes[c].lexical_end = nodes[l].lexical_end;
         stack.push_back(c);
         ++ unary_reduce ;
      }
      else {
         assert(stack.size()>=2);
         r = stack.back();
         stack.pop_back();
         l = stack.back();
         stack.pop_back();
#ifdef NO_TEMP_CONSTITUENT
         c = newNode(-1, (head_left?CStateNode::HEAD_LEFT:CStateNode::HEAD_RIGHT), temporary, constituent, l, r, (head_left?nodes[l].lexical_head:nodes[r].lexical_head));
#else
         c = newNode(-1, (head_left?CStateNode::HEAD_LEFT:CStateNode::HEAD_RIGHT), temporary, CConstituent::encodeTmp(constituent, temporary), l, r, (head_left?nodes[l].lexical_head:nodes[r].lexical_head));
#endif
         nodes[l].parent = c;
         nodes[r].parent = c;
//         nodes[c].lexical_start = nodes[l].lexical_start;
//         nodes[c].lexical_end = nodes[r].lexical_end;
         stack.push_back(c);
         unary_reduce = 0;
      }
      assert(!IsTerminated());
   }
   void unshift(const SCORE_TYPE &original_score, const int &original_unary) {
      //TRACE("unshift");
      assert(!IsTerminated());
      assert(!nodes[stack.back()].is_constituent());
      assert(nodes.size()-1==stack.back());
      stack.pop_back();
      nodes.pop_back();
      score = original_score;
      current_word--;
      unary_reduce = original_unary;
      assert(!IsTerminated());
   }
   void unreduce(const SCORE_TYPE &original_score, const int &original_unary) {
      //TRACE("unreduce");
      assert(!IsTerminated());
      int s = stack.back();
      assert(nodes.size()-1==s);
      assert(nodes[s].is_constituent());
      if (nodes[s].single_child()) {
         stack.pop_back();
         stack.push_back(nodes[s].left_child);
         nodes[nodes[s].left_child].parent = -1;
         nodes.pop_back();
      }
      else {
         stack.pop_back();
         stack.push_back(nodes[s].left_child);
         stack.push_back(nodes[s].right_child);
         nodes[nodes[s].left_child].parent = -1;
         nodes[nodes[s].right_child].parent = -1;
         nodes.pop_back();
      }
      score = original_score;
      unary_reduce = original_unary;
      assert(!IsTerminated());
   }
   void terminate() {
      //TRACE("terminate");
      assert(IsComplete());
      assert(!IsTerminated());
      unary_reduce = -1;
      assert(IsTerminated());
   }
   void unterminate(const SCORE_TYPE &original_score, const int &original_unary) {
      //TRACE("unterminate");
      assert(IsTerminated());
      score = original_score;
      unary_reduce = original_unary;
      assert(!IsTerminated());
   }

public:
   bool operator < (const CStateItem &st) const { return score < st.score; }
   bool operator > (const CStateItem &st) const { return score > st.score; }
   bool operator == (const CStateItem &st) const {
      return current_word == st.current_word && unary_reduce == st.unary_reduce && nodes == st.nodes && stack == st.stack ;
   }
   bool operator != (const CStateItem &st) const { return !( (*this) == st ); }
   void operator = (const CStateItem &st) { current_word=st.current_word; unary_reduce=st.unary_reduce; nodes=st.nodes; stack=st.stack; score=st.score; sent = st.sent; }

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

   void FollowMove(const CStateItem &st, CAction &retval) const {
      // don't follow move when the states euqal
      assert(*this!=st);

      // stack empty?shift
      if (stack.empty()) {
         retval.encodeShift(st.nodes[newNodeIndex()].constituent.code());
         return;
      }
      int s = stack.back();
      if (st.nodes[s].parent == -1) { // not finished
         assert(st.stack[stack.size()-1]==s); // also on stack with guider state
         if (st.stack.size()==1) {
            assert(st.IsTerminated());
            retval.encodeReduceRoot();
         }
         else {
            retval.encodeShift(st.nodes[newNodeIndex()].constituent.code());
         }
      }
      else {
         NextMove(st, retval);
      }
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

   void Move(const CAction &action) {
      if (action.isShift())
         shift(action.getConstituent());
      else if (action.isReduceRoot())
         { assert(IsComplete()); terminate(); }
      else
         reduce(action.getConstituent(), action.singleChild(), action.headLeft(), action.isTemporary());
   }
   
   void UnMove(const CAction &action, const SCORE_TYPE &original_score, const int &original_unary) {
      if (action.isShift())
         unshift(original_score, original_unary);
      else if (action.isReduceRoot())
         { unterminate(original_score, original_unary); }
      else
         unreduce(original_score, original_unary);
   }

   bool IsComplete() const {
      return current_word == sent->size() && stack.size() == 1;
   }

   bool IsTerminated() const {
      assert( unary_reduce>=0 || IsComplete() );
      return unary_reduce == -1; 
   }

   void GenerateTree(const CTwoStringVector &tagged, CSentenceParsed &out) const {
      // parsing done?
      assert(IsComplete());
      assert(tagged.size()==sent->size());
      // generate nodes for out
      out.clear();
      int i,j;
      // first words
      for (i=0; i<tagged.size(); ++i) 
         out.newWord(tagged[i].first, tagged[i].second);
      // second constituents
      for (i=0; i<nodes.size(); ++i) {
         j = out.newNode();
         // copy node
         nodes[j].toCCFGTreeNode(out.nodes[j]);
         // update words ; use the constituent label for leaf nodes if any
//         if (!nodes[j].is_constituent() && nodes[j].constituent.code()!=CConstituent::NONE) {
//            out.words[nodes[j].lexical_head].second = nodes[j].constituent.str();
//         }
      }
      out.root = stack.back();
   }

   //===============================================================================

   void trace(const CTwoStringVector *s=0) const {
      CStateItem st; 
      st.sent = sent;
      CAction action;
      //TRACE("Nodes"); for (unsigned long i=0; i<nodes.size(); ++i) TRACE(nodes[i].str());
      //TRACE("Stack: " << toString(stack));
      TRACE("State item score == " << score);
      while (st!=*this) {
         st.FollowMove(*this, action);
         st.Move(action);
         if (s) {
            TRACE(action.str()<<" ["<<(st.stack.size()>0?s->at(st.nodes[st.stack.back()].lexical_head).first:"")<<"]"); 
         }
         else {
            TRACE(action.str());
         }
      }
      TRACE("");
   }
};

#endif
