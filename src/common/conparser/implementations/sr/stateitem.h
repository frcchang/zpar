#ifndef _COMMON_CON_PARSER_STATEITEM
#define _COMMON_CON_PARSER_STATEITEM

/*===============================================================
 *
 * Reduce actions
 *
 *==============================================================*/

inline unsigned getConstituent(const unsigned &action) {
   return action & ((1<<PENN_CON_COUNT_BITS)-1);
}

inline bool singleChild(const unsigned &action) {
   return action & (1<<(PENN_CON_COUNT_BITS+1));
}

inline bool headLeft(const unsigned &action) {
   return action & (1<<PENN_CON_COUNT_BITS);
}

inline bool isTemporary(const unsigned &action) {
   return action & (1<<(PENN_CON_COUNT_BITS+2));
}

inline unsigned long encodeAction(const unsigned &action, const unsigned long &num) {
   assert(action>>(PENN_CON_COUNT_BITS+4)==0);
   return action | ( num << (PENN_CON_COUNT_BITS+4) ); // action takes 4 extra bits plus consti (REDUCE, SINGLE_C, HEAD_L, TEMP)
}

inline unsigned encodeReduce(const unsigned &constituent, bool single_child, bool head_left, bool temporary) {
   assert(!single_child || (!head_left&&!temporary));
   return (1<<(PENN_CON_COUNT_BITS+3)) | 
          ((temporary?1:0) << (PENN_CON_COUNT_BITS+2)) | 
          ((single_child?1:0) << (PENN_CON_COUNT_BITS+1)) | 
          ((head_left?1:0) << PENN_CON_COUNT_BITS) | 
          constituent ;
}

inline unsigned encodeShift() {
   return 0;
}

inline unsigned encodeReduceRoot() {
   return encodeReduce(PENN_CON_NONE, false, false, false);
}

inline bool isShift(const unsigned &action) { return action == 0; }
inline bool isReduce(const unsigned &action) { return action & (1<<(PENN_CON_COUNT_BITS+3)); }
inline bool isReduceRoot(const unsigned &action) { return action == 1<<(PENN_CON_COUNT_BITS+3); }

inline string printAction(const unsigned &action) {
   if (isShift(action)) return "SHIFT";
   assert(isReduce(action));
   string retval = "REDUCE";
   if (isReduceRoot(action)) { retval += " ROOT"; return retval; }
   else if (singleChild(action)) retval += " UNARY";
   else {
      retval += " BINARY";
      retval += (headLeft(action)) ? " LEFT" : " RIGHT";
      if (isTemporary(action)) retval += " TMP";
   }
   retval += " "; retval += CConstituent(getConstituent(action)).str();
   return retval;
}

/*===============================================================
 *
 * CStateNode - tree nodes 
 *
 *==============================================================*/

class CStateNode : public CCFGTreeNode {
public:
   int lexical_head;
   int lexical_start;
   int lexical_end;
   bool norv; // pure noun or verb
public:
   CStateNode() : CCFGTreeNode() { lexical_head=-1; lexical_start=-1; lexical_end = -1; norv = false; }
   virtual ~CStateNode() {}
public:
   bool operator == (const CStateNode &nd) const {
      return lexical_head == nd.lexical_head &&
      lexical_start == nd.lexical_start &&
      lexical_end == nd.lexical_end &&
      is_constituent == nd.is_constituent &&
      parent == nd.parent &&
      constituent == nd.constituent &&
      single_child == nd.single_child &&// single or double
      head_left == nd.head_left && // double children 
      left_child == nd.left_child &&
      right_child == nd.right_child &&
      temp == nd.temp &&
      token == nd.token ;
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
   const vector< CTaggedWord<CTag> > *sent;
   
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
   int newNode() { nodes.push_back(CStateNode()); return nodes.size()-1; }

public:
   void shift() {
      //TRACE("shift");
      assert(!IsTerminated());
      int t = newNode();
      nodes[t].is_constituent = false;
      nodes[t].single_child = false;
      nodes[t].head_left = false;
      nodes[t].temp = false;
      nodes[t].constituent = PENN_CON_NONE;
      nodes[t].token = current_word;
      nodes[t].norv = (sent->at(current_word).tag == PENN_TAG_NN || 
                        sent->at(current_word).tag == PENN_TAG_VV);
      nodes[t].lexical_start = current_word;
      nodes[t].lexical_end = current_word;
      nodes[t].lexical_head = current_word ++;
      unary_reduce = 0;
      stack.push_back(t);
      assert(!IsTerminated());
   }
   void reduce(const unsigned &constituent, const bool &single_child, const bool &head_left, const bool &temporary) {
      //TRACE("reduce");
      assert(!IsTerminated());
      int c = newNode();         // the new node
      assert(stack.size()>=0);
      nodes[c].is_constituent = true;
      nodes[c].constituent = constituent;
      nodes[c].single_child = single_child;
      nodes[c].head_left = head_left;
      nodes[c].temp = temporary;//
      const bool norv = (constituent == PENN_CON_NP || constituent == PENN_CON_VP);
      if (single_child) {
         assert(head_left == false);
         assert(temporary == false);
         int l = stack.back();
         stack.pop_back();
         nodes[c].left_child = l;
         nodes[c].right_child = -1;
         nodes[l].parent = c;
         nodes[c].lexical_start = nodes[l].lexical_start;
         nodes[c].lexical_end = nodes[l].lexical_end;
         nodes[c].lexical_head = nodes[l].lexical_head;
         nodes[c].norv = (nodes[l].norv && norv);
         stack.push_back(c);
         unary_reduce ++ ;
      }
      else {
         assert(stack.size()>=2);
         int r = stack.back();
         stack.pop_back();
         int l = stack.back();
         stack.pop_back();
         nodes[c].left_child = l;
         nodes[c].right_child = r;
         nodes[l].parent = c;
         nodes[r].parent = c;
         nodes[c].lexical_start = nodes[l].lexical_start;
         nodes[c].lexical_end = nodes[r].lexical_end;
         if (head_left) 
            nodes[c].lexical_head = nodes[l].lexical_head;
         else
            nodes[c].lexical_head = nodes[r].lexical_head;
         nodes[c].norv = (nodes[l].norv && nodes[r].norv && norv);
         stack.push_back(c);
         unary_reduce = 0;
      }
      assert(!IsTerminated());
   }
   void unshift(const SCORE_TYPE &original_score, const int &original_unary) {
      //TRACE("unshift");
      assert(!IsTerminated());
      assert(!nodes[stack.back()].is_constituent);
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
      assert(nodes[s].is_constituent);
      if (nodes[s].single_child) {
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

   //
   // This method applies to both full parse [CCFGTree] and partial parse [CStateItem]
   // The first case for standard move and the second for follow move in updating scores.
   //
   template<class CPartialParseTree> unsigned NextMove(const CPartialParseTree &snt) const {
      int s = stack.back();
      const CCFGTreeNode &nd = snt.nodes[s];
      const CCFGTreeNode &hd = snt.nodes[nd.parent];
      assert(hd.constituent!=PENN_CON_NONE); // so that reduce and reduce_root are not same
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
         if (s == hd.left_child) 
            return encodeShift();
         // stack top right child ? reduce bin
         assert(s==hd.right_child);
         single_child = false;
         head_left = hd.head_left;
         temporary = hd.temp;
      }
      return encodeReduce(hd.constituent, single_child, head_left, temporary);
   }

   unsigned FollowMove(const CStateItem &st) const {
      // don't follow move when the states euqal
      assert(*this!=st);

      // stack empty?shift
      if (stack.empty())
         return encodeShift();
      int s = stack.back();
      if (st.nodes[s].parent == -1) { // not finished
         assert(st.stack[stack.size()-1]==s); // also on stack with guider state
         if (st.stack.size()==1) {
            assert(st.IsTerminated());
            return encodeReduceRoot();
         }
         else {
            return encodeShift();
         }
      }
      else {
         return NextMove(st);
      }
   }

   unsigned StandardMove(const CCFGTree &tr) const {
      assert(!IsTerminated());
      assert(tr.words.size() == sent->size());
      // stack empty?shift
      if (stack.empty())
         return encodeShift();
      int s = stack.back();
      if (tr.nodes[s].parent == -1) {
         assert(IsComplete());
         return encodeReduceRoot();
      }
      return NextMove(tr);
   }

   void Move(const unsigned &action) {
      if (isShift(action))
         shift();
      else if (isReduceRoot(action))
         { assert(IsComplete()); terminate(); }
      else
         reduce(getConstituent(action), singleChild(action), headLeft(action), isTemporary(action));
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
      for (i=0; i<tagged.size(); i++) 
         out.newWord(tagged[i].first, tagged[i].second);
      for (i=0; i<nodes.size(); i++) {
         j = out.newNode();
         out.nodes[j] = static_cast<CCFGTreeNode>(nodes[j]);
      }
      out.root = stack.back();
   }

   //===============================================================================

   void trace(const CTwoStringVector *s=0) const {
      CStateItem st; 
      st.sent = sent;
      unsigned action;
      //TRACE("Nodes"); for (unsigned i=0; i<nodes.size(); ++i) TRACE(nodes[i].str());
      //TRACE("Stack: " << toString(stack));
      TRACE("State item score == " << score);
      while (st!=*this) {
         action = st.FollowMove(*this);
         st.Move(action);
         if (s) {
            TRACE(printAction(action)<<" ["<<(st.stack.size()>0?s->at(st.nodes[st.stack.back()].lexical_head).first:"")<<"]"); 
         }
         else {
            TRACE(printAction(action));
         }
      }
      TRACE("");
   }
};

#endif
