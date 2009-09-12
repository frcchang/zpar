/****************************************************************
 *                                                              *
 * context.h - the definition of context                        *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2007.12                        *
 *                                                              *
 ****************************************************************/

class CStateNode; 
class CStateItem;

//===============================================================
//
// CContext 
//
//===============================================================

class CContext {
public:
   int s0, s1, s2, s3;
   int n0, n1, n2, n3;
   int s0l, s0r, s0u;
   int s1l, s1r, s1u;
   int s0ld, s0rd;
   int s1ld, s1rd;

   unsigned long s0c, s1c, s2c, s3c;
   unsigned long s0t, s1t, s2t, s3t;
   unsigned long n0t, n1t, n2t, n3t;
   unsigned long s0lc, s0rc, s0uc;
   unsigned long s1lc, s1rc, s1uc;
   unsigned long s0ldt, s0rdt;
   unsigned long s1ldt, s1rdt;
   const CWord *s0w, *s1w, *s2w, *s3w;
   const CWord *n0w, *n1w, *n2w, *n3w;
   const CWord *s0lw, *s0rw, *s0uw, *s1lw, *s1rw, *s1uw;
   const CWord *s0ldw, *s0rdw, *s1ldw, *s1rdw;

   unsigned long stacksize;



   unsigned long s0cs1c, s1cn0t, s0cn0t, n0tn1t;
   CTwoWords s0ws1w, s0wn0w, n0wn1w, s1wn0w;

   vector<unsigned long> s0_unbinarized;
   vector<unsigned long> s1_unbinarized;
   vector<unsigned long> s0_unbinarized_cs;
   vector<unsigned long> s1_unbinarized_cs;
   int s0_head_node; // the head node among all unexpanded nodes from s0
   int s1_head_node; // the head node among all unexpanded nodes from s1
   int s0_head_index;
   int s1_head_index;

   unsigned long open_bracket_match_type;
   unsigned long s0c_bracket, s1c_bracket, n0t_bracket, s0cs1c_bracket, s0cn0t_bracket;

   unsigned long s0ln, s0rn, s1ln, s1rn;
   unsigned long s0m, s1m, s2m, n0m;
   bool n0norv;

   vector<unsigned long> s0cs1c_separator; // one particular separator punctuation
   vector<unsigned long> s0c_separator;
   vector<unsigned long> s1c_separator;
   unsigned long s0cs1c_sepcount; // the count of separator pus between them
   unsigned long s0c_sepcount; // unigram backup
   unsigned long s1c_sepcount;

   unsigned long s0s1_dist, s0cs1_dist, s0s1c_dist, s0cs1c_dist;
protected:
   // protected because they're transient
   const CStateNode *s0node, *s1node, *s2node, *s3node;
public:
   CContext() {}
   ~CContext() {}
public:
   unsigned long unbinarize(const vector<CStateNode> &nodes, const unsigned long &curr, vector<unsigned long> &rval) {
      unsigned long head;
      head = curr;
      if (nodes[curr].temp) {
         assert(nodes[curr].is_constituent&&!nodes[curr].single_child);
         if (nodes[curr].head_left) {
            head = unbinarize(nodes, nodes[curr].left_child, rval);
            assert(!nodes[nodes[curr].right_child].temp);
            rval.push_back(nodes[curr].right_child);
         }
         else {
            rval.push_back(nodes[curr].left_child);
            assert(!nodes[nodes[curr].left_child].temp);
            head = unbinarize(nodes, nodes[curr].right_child, rval);
         }
      }
      else {
         rval.push_back(curr);
      }
      return head;
   }
   void countleftdependents(const vector<CStateNode> &nodes, const unsigned long &i, int &last, unsigned long &count) {
      if (!nodes[i].is_constituent) {
         last = -1;
         count = 0;
      }
      else {
         if (nodes[i].single_child) {
            countleftdependents(nodes, nodes[i].left_child, last, count);
         }
         else {
            if (nodes[i].head_left) {
               countleftdependents(nodes, nodes[i].left_child, last, count);
            }
            else {
               countleftdependents(nodes, nodes[i].right_child, last, count) ;
               count ++;
               last = nodes[i].left_child;
            }
         }
      }
   }
   void countrightdependents(const vector<CStateNode> &nodes, const unsigned long &i, int &last, unsigned long &count) {
      if (!nodes[i].is_constituent) {
         last = -1;
         count = 0;
      }
      else {
         if (nodes[i].single_child) {
            countrightdependents(nodes, nodes[i].left_child, last, count);
         }
         else {
            if (nodes[i].head_left) {
               countrightdependents(nodes, nodes[i].left_child, last, count);
               count ++;
               last = nodes[i].right_child;
            }
            else {
               countrightdependents(nodes, nodes[i].right_child, last, count) ;
            }
         }
      }
   }
   void load(const CStateItem *item, const vector<CTaggedWord<CTag> > &wrds, const vector<unsigned long> &wordlen, const bool &modify) {
      static CStateNode emptyItem;
      stacksize = item->stack.size();
      if (stacksize==0) return; // must shift; no feature updates, no comparisons for different actions
      static unsigned long tmp;
      static int i, j;
      s0 = item->stack.back();
      s1 = stacksize<2 ? -1 : item->stack[stacksize-2];
      s2 = stacksize<3 ? -1 : item->stack[stacksize-3];
      s3 = stacksize<4 ? -1 : item->stack[stacksize-4];
      n0 = item->current_word >= wrds.size() ? -1 : item->current_word;
      n1 = item->current_word+1 >= wrds.size() ? -1 : item->current_word+1;
      n2 = item->current_word+2 >= wrds.size() ? -1 : item->current_word+2;
      n3 = item->current_word+3 >= wrds.size() ? -1 : item->current_word+3;
   
      s0node = &(item->nodes[s0]);
      s1node = s1==-1 ? 0 : &(item->nodes[s1]);
      s2node = s2==-1 ? 0 : &(item->nodes[s2]);
      s3node = s3==-1 ? 0 : &(item->nodes[s3]);
  
      s0l = s0node->is_constituent ? (s0node->single_child ? -1 : s0node->left_child) : -1;
      s0r = s0node->is_constituent ? (s0node->single_child ? -1 : s0node->right_child) : -1;
      s0u = s0node->is_constituent ? (s0node->single_child ? s0node->left_child : -1) : -1;

      s1l = s1==-1 ? -1 : ( s1node->is_constituent ? (s1node->single_child ? -1 : s1node->left_child) : -1 );
      s1r = s1==-1 ? -1 : ( s1node->is_constituent ? (s1node->single_child ? -1 : s1node->right_child) : -1 );
      s1u = s1==-1 ? -1 : ( s1node->is_constituent ? (s1node->single_child ? s1node->left_child : -1) : -1 );
   
      s0c = encodeC(*s0node, wrds); assert(s0==-1||s0c!=0);
      s0w = &(wrds[s0node->lexical_head]);
      s0t = encodeT(wrds[s0node->lexical_head]);
   
      s1c = s1==-1 ? 0 : encodeC(*s1node, wrds); assert(s1==-1||s1c!=0);
      s1w = s1 == -1 ? 0 : &(wrds[s1node->lexical_head]);
      s1t = s1 == -1 ? 0 : encodeT(wrds[s1node->lexical_head]);
   
      s2c = s2==-1 ? 0 : encodeC(*s2node, wrds); assert(s2==-1||s2c!=0);
      s2w = s2 == -1 ? 0 : &(wrds[s2node->lexical_head]);
      s2t = s2 == -1 ? 0 : encodeT(wrds[s2node->lexical_head]);
   
      s3c = s3==-1 ? 0 : encodeC(*s3node, wrds); assert(s3==-1||s3c!=0);
      s3w = s3 == -1 ? 0 : &(wrds[s3node->lexical_head]);
      s3t = s3 == -1 ? 0 : encodeT(wrds[s3node->lexical_head]);
   
      s0lc = s0l==-1 ? 0 : encodeC(item->nodes[s0l], wrds); assert(s0l==-1||s0lc!=0);
      s0lw = s0l==-1 ? 0 : &(wrds[item->nodes[s0l].lexical_head]);
      s0rc = s0r==-1 ? 0 : encodeC(item->nodes[s0r], wrds); assert(s0r==-1||s0rc);
      s0rw = s0r==-1 ? 0 : &(wrds[item->nodes[s0r].lexical_head]);
      s0uc = s0u==-1 ? 0 : encodeC(item->nodes[s0u], wrds); assert(s0u==-1||s0uc);
      s0uw = s0u==-1 ? 0 : &(wrds[item->nodes[s0u].lexical_head]);
   
      s1lc = s1l==-1 ? 0 : encodeC(item->nodes[s1l], wrds); assert(s1l==-1||s1lc!=0);
      s1lw = s1l==-1 ? 0 : &(wrds[item->nodes[s1l].lexical_head]);
      s1rc = s1r==-1 ? 0 : encodeC(item->nodes[s1r], wrds); assert(s1r==-1||s1rc);
      s1rw = s1r==-1 ? 0 : &(wrds[item->nodes[s1r].lexical_head]);
      s1uc = s1u==-1 ? 0 : encodeC(item->nodes[s1u], wrds); assert(s1u==-1||s1uc);
      s1uw = s1u==-1 ? 0 : &(wrds[item->nodes[s1u].lexical_head]);

      n0t = n0==-1 ? 0 : encodeT(wrds[n0]);
      n0w = n0 == -1 ? 0 : &(wrds[n0]);
   
      n1t = n1==-1 ? 0 : encodeT(wrds[n1]);
      n1w = n1 == -1 ? 0 : &(wrds[n1]);
   
      n2t = n2==-1 ? 0 : encodeT(wrds[n2]);
      n2w = n2 == -1 ? 0 : &(wrds[n2]);
 
      n3t = n3==-1 ? 0 : encodeT(wrds[n3]);
      n3w = n3 == -1 ? 0 : &(wrds[n3]);

      if (modify==false) {
         if (s1!=-1) s0ws1w.refer(s0w, s1w); 
         if (n0!=-1) s0wn0w.refer(s0w, n0w); 
         if (n1!=-1) n0wn1w.refer(n0w, n1w);
         if (s1!=-1&&n0!=-1) s1wn0w.refer(s1w, n0w);
      }
      else {
         if (s1!=-1) s0ws1w.allocate(*s0w, *s1w);
         if (n0!=-1) s0wn0w.allocate(*s0w, *n0w);
         if (n1!=-1) n0wn1w.allocate(*n0w, *n1w);
         if (s1!=-1&&n0!=-1) s1wn0w.allocate(*s1w, *n0w);
      }
      s0cs1c = encodeTorCs(s0c, s1c);
      s0cn0t = encodeTorCs(s0c, n0t);
      n0tn1t = encodeTorCs(n0t, n1t);
      s1cn0t = encodeTorCs(s1c, n0t);

      // unexpand s0 and s1
      s0_unbinarized.clear();
      s0_unbinarized_cs.clear();
      s0_head_node = unbinarize(item->nodes, s0, s0_unbinarized);
      s0_head_index = -1;
      for (tmp=0; tmp<s0_unbinarized.size(); tmp++) {
         s0_unbinarized_cs.push_back(encodeC(item->nodes[s0_unbinarized[tmp]], wrds));
         if (s0_unbinarized[tmp]==s0_head_node) s0_head_index=tmp;
      }
      assert(s0_head_index!=-1);
      s1_unbinarized.clear();
      s1_unbinarized_cs.clear();
      s1_head_index=-1;
      if (s1!=-1) s1_head_node = unbinarize(item->nodes, s1, s1_unbinarized);
      else s1_head_node = -1;
      if (s1!=-1)  {
         for(tmp=0; tmp<s1_unbinarized.size(); tmp++) {
            s1_unbinarized_cs.push_back(encodeC(item->nodes[s1_unbinarized[tmp]], wrds));
            if (s1_unbinarized[tmp]==s1_head_node) s1_head_index=tmp;
         }
      }
      assert(s1==-1||s1_head_index!=-1);

      // bracket
      static int last_stack_starting_bracket;
      last_stack_starting_bracket = -1;
      open_bracket_match_type = 0; // null
      for (i=stacksize-1; i>=0; i--) {
         const CStateNode &node = item->nodes[item->stack[i]];
         last_stack_starting_bracket = node.is_constituent?-1:getStartingBracket(wrds[node.lexical_head]);
         if (last_stack_starting_bracket!=-1) { // find the first bracket
            open_bracket_match_type = 1; // open
            const int &rt = item->nodes[s0_unbinarized.back()].lexical_head;
            if (getEndingBracket(wrds[rt])==last_stack_starting_bracket) // match
               open_bracket_match_type = 2; // matched bracket
            else {
               for (j=node.lexical_head; j<rt; j++)
                  if (getEndingBracket(wrds[j])==last_stack_starting_bracket)
                     open_bracket_match_type = 3; // missed
            }
            break;
         }
      }
      s0c_bracket = encodeTorCs(open_bracket_match_type, s0c);   
      s1c_bracket = encodeTorCs(open_bracket_match_type, s1c);
      n0t_bracket = encodeTorCs(open_bracket_match_type, n0t);
      s0cs1c_bracket = encodeTorCs(open_bracket_match_type, s0c, s1c);
      s0cn0t_bracket = encodeTorCs(open_bracket_match_type, s0c, n0t);

      // comma, full stop, dun hao, semi colon;
      static int last_separator;
      static unsigned long sepcount;
      static unsigned long s0_sepset, s1_sepset, s0s1_sepset;
      s0cs1c_separator.clear();
      s0c_separator.clear();
      s1c_separator.clear();
      s0_sepset=0; s1_sepset=0; s0s1_sepset=0;
      sepcount = 0;
      if (s1!=-1) {
         for (i=s1_head_index+1; i<s0_head_index; ++i) {
            last_separator = getSeparatingPunctuation(wrds[item->nodes[i].lexical_head]);
            if (last_separator!=-1) {
               if ((s0s1_sepset&(1<<last_separator))==0) {
                  s0s1_sepset|=(1<<last_separator);
                  s0cs1c_separator.push_back(encodeTorCs(s0cs1c, last_separator));
               }
               if ((s0_sepset&(1<<last_separator))==0) {
                  s0_sepset |= (1<<last_separator);
                  s0c_separator.push_back(encodeTorCs(s0c, last_separator));
               }
               if ((s1_sepset & (1<<last_separator)) == 0) {
                  s1_sepset |= (1<<last_separator);
                  s1c_separator.push_back(encodeTorCs(s1c, last_separator));
               }
               sepcount++ ;
            }
         }
      }
      s0cs1c_sepcount=encodeTorCs(s0cs1c,sepcount);
      s0c_sepcount=encodeTorCs(s0c,sepcount);
      s1c_sepcount=encodeTorCs(s1c,sepcount);

      // S{0/1}{LD/RD}
      countleftdependents(item->nodes, s0, s0ld, s0ln);
      countrightdependents(item->nodes, s0, s0rd, s0rn);
      if (s1==-1) {
         s1ld = -1;
         s1rd = -1;
         s1ln = 0;
         s1rn = 0;
      }
      else {
         countleftdependents(item->nodes, s1, s1ld, s1ln);
         countrightdependents(item->nodes, s1, s1rd, s1rn);
      }
      s0ldt = s0ld==-1 ? 0 : encodeT(wrds[item->nodes[s0ld].lexical_head]);
      s0ldw = s0ld == -1 ? 0 : &(wrds[item->nodes[s0ld].lexical_head]);
      s0rdt = s0rd==-1 ? 0 : encodeT(wrds[item->nodes[s0rd].lexical_head]);
      s0rdw = s0rd == -1 ? 0 : &(wrds[item->nodes[s0rd].lexical_head]);
      s1ldt = s1ld==-1 ? 0 : encodeT(wrds[item->nodes[s1ld].lexical_head]);
      s1ldw = s1ld == -1 ? 0 : &(wrds[item->nodes[s1ld].lexical_head]);
      s1rdt = s1rd==-1 ? 0 : encodeT(wrds[item->nodes[s1rd].lexical_head]);
      s1rdw = s1rd == -1 ? 0 : &(wrds[item->nodes[s1rd].lexical_head]);

      // Rhythm
      if (s0node->norv) s0m = encodeRhythm(*s0node, wordlen);
      if (s1!=-1&&s1node->norv) s1m = encodeRhythm(*s1node, wordlen);
      if (s2!=-1&&s2node->norv) s2m = encodeRhythm(*s2node, wordlen);
      n0norv = (n0!=-1&&(n0t==PENN_TAG_NN||n0t==PENN_TAG_VV));
      if (n0norv) n0m = wordlen[n0]; else n0m = 0;

      if (s1!=-1) {
//         s0s1_dist = encodeLinkSize(s0node->lexical_head, s1node->lexical_head);
//         s0cs1_dist = encodeTorCs(s0c, s0s1_dist);
//         s0s1c_dist = encodeTorCs(s1c, s0s1_dist);
         s0cs1c_dist = encodeTorCs(s0cs1c, s0s1_dist);
      }

      return;
   }
};
                           
