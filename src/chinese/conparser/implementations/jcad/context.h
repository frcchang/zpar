// Copyright (C) University of Oxford 2010
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

const CConstituent g_noneConstituent(CConstituent::NONE);
const CConstituent g_beginConstituent(CConstituent::SENTENCE_BEGIN);
const CTag g_noneTag(CTag::NONE);
const CWord g_noneWord(CWord::UNKNOWN);

//#define constituent_or_none(x) ((x).is_constituent() ? (x).constituent.code() : CConstituent::NONE)
#define constituent_or_none(x) ((x).constituent.code())
#define _encode_three_constituents(x, y, z) ((x<<CConstituent::SIZE<<CConstituent::SIZE)|(y<<CConstituent::SIZE)|z)
#define _encode_two_constituents(x, y) ((x<<CConstituent::SIZE)|y)
#define _load_char(x) modify ? wrds.replace(x->head_c, x->head_c, &sentence) : wrds.find(x->head_c, x->head_c, &sentence)
#define _load_char_bypos(x) modify ? wrds.replace(x, x, &sentence) : wrds.find(x, x, &sentence)
#define _load_word(x) x->is_partial() ? (modify ? wrds.replace(x->begin_c, x->end_c, &sentence) : wrds.find(x->begin_c, x->end_c, &sentence)) : (modify ? wrds.replace(x->word_head->begin_c, x->word_head->end_c, &sentence) : wrds.find(x->word_head->head_c, x->word_head->end_c, &sentence))
//#define _load_word(x) modify ? wrds.replace(x->word_head->begin_c, x->word_head->end_c, &sentence) : wrds.find(x->word_head->head_c, x->word_head->end_c, &sentence)

//===============================================================
//
// CContext 
//
//===============================================================

class CContext {
public:
   const CStateNode *s0, *s1, *s2, *s3;
   const CStateNode *s0l, *s0r, *s0u, *s0h;
   const CStateNode *s1l, *s1r, *s1u, *s1h;
   int p1, p2, n0, n1, n2, n3;
   int s0ld, s0rd;
   int s1ld, s1rd;

   CWord s0w, s1w, s2w, s3w;
   CConstituent s0c, s1c, s2c, s3c;
   CTag s0t, s1t, s2t, s3t;
   CWord s0z, s1z, s2z, s3z;

   //CWord n0w, n1w, n2w, n3w;
   //CTag n0t, n1t, n2t, n3t;
   CWord p1z, p2z, n0z, n1z, n2z, n3z; //addzms
   CWord rightmost1w, rightmost2w; //addzms
   CTag rightmonst1t, rightmost2t; //addzms


   CWord s0lw, s0rw, s0uw, s1lw, s1rw, s1uw;
   CConstituent s0lc, s0rc, s0uc;
   CTag s0lt, s0rt, s0ut;
   CConstituent s1lc, s1rc, s1uc;
   CTag s1lt, s1rt, s1ut;

   unsigned long s0ldt, s0rdt;
   unsigned long s1ldt, s1rdt;
   CTaggedWord<CTag, TAG_SEPARATOR> s0wt, s1wt, n0wt, n1wt, n2wt, n3wt;
   CWord s0ldw, s0rdw, s1ldw, s1rdw;

   unsigned long stacksize;


   CCFGSet s0cs1c, s0ts1t, s0cn0t, s1cn0t, n0tn1t, s0tn0t, 
           s0cs1cn0t, s0cn0tn1t, s0cs1cs2c, 
           s0ts1tn0t, s0tn0tn1t, s0ts1ts2t,
           s0js1jn0t, s0jn0tn1t, s0js1js2j;
   // here j means the combination of c and t -- t with non-constituent
   CCFGSet s0cs0lc, s0cs0rc, s1cs1rc, 
           s0cs0rcn0t, s0cs0rjn0t, s0cs0lcs1c, s0cs0ljs1j, s0cs1cs1rc, s0js1cs1rj;
   CTwoWords s0ws1w, s0wn0w, n0wn1w, s1wn0w;
   CTwoTaggedWords s0wts1wt;

   std::vector<CTag> between_tag;
   CCFGSet s0ts1tbt;

   // sub nodes unbinarized for the binarized 
//   std::vector<unsigned long> s0_unbinarized;
//   std::vector<unsigned long> s1_unbinarized;
//   std::vector<CConstituent> s0_unbinarized_cs;
//   std::vector<CConstituent> s1_unbinarized_cs;
   // the head child
   CConstituent s0hc, s1hc;
   // the constituent, its head child, and the left/right two ones
   // applied to any constituent no matter how many sub-nodes
   CCFGSet s0cs0hcs0r2c, s0cs0hcs0l2c, s1cs1hcs1r2c;
   // unpacked left/right nodes for those having >=2
   CCFGSet s0r6c, s0l6c, s1r6c;
//   int s0h_unbinarized; // the head node among all unexpanded nodes from s0
//   int s1h_unbinarized; // the head node among all unexpanded nodes from s1
//   int s0h_unbinarized_index;
//   int s1h_unbinarized_index;

   unsigned long open_bracket_match_type;
#ifdef _CHINESE_CFG_H
   unsigned long s0c_bracket, s1c_bracket, n0t_bracket, s0cs1c_bracket, s0cn0t_bracket;

   std::vector<unsigned long> s0cs1c_separator; // one particular separator punctuation
   std::vector<unsigned long> s0c_separator;
   std::vector<unsigned long> s1c_separator;
   unsigned long s0cs1c_sepcount; // the count of separator pus between them
   unsigned long s0c_sepcount; // unigram backup
   unsigned long s1c_sepcount;
#endif

   unsigned long s0ln, s0rn, s1ln, s1rn;
   unsigned long s0m, s1m, s2m, n0m;
   bool n0norv;

   unsigned long s0s1_dist, s0cs1_dist, s0s1c_dist, s0cs1c_dist;

//public:
//   CContext() {}
//   ~CContext() {}
public:

   void load(const CStateItem *item, CWordCache &wrds, const CStringVector &sentence, const bool &modify) {
      stacksize = item->stacksize();
      if (stacksize==0) return; // must shift; no feature updates, no comparisons for different actions
      static unsigned long tmp;
      static int tmp_i;
      static int i, j;
      p1 = item->current_word-1 > 0 ? -1 : item->current_word-1;
      p2 = item->current_word-2 > 0 ? -1 : item->current_word-2;
      n0 = item->current_word >= sentence.size() ? -1 : item->current_word;
      n1 = item->current_word+1 >= sentence.size() ? -1 : item->current_word+1;
      n2 = item->current_word+2 >= sentence.size() ? -1 : item->current_word+2;
      n3 = item->current_word+3 >= sentence.size() ? -1 : item->current_word+3;
      //n1 = -1; n2 = -1; n3 = -1;
   
      s0 = &(item->node);
      s1 = stacksize<2 ? 0 : &(item->stackPtr->node);
      s2 = stacksize<3 ? 0 : &(item->stackPtr->stackPtr->node);
      s3 = stacksize<4 ? 0 : &(item->stackPtr->stackPtr->stackPtr->node);
  
      //s0l = s0->is_constituent() ? (s0->single_child()||s0->head_left() ? 0 : s0->left_child) : 0;
      //s0r = s0->is_constituent() ? (s0->single_child()||!s0->head_left() ? 0 : s0->right_child) : 0;
      //s0u = s0->is_constituent() ? (s0->single_child() ? s0->left_child : 0) : 0;

      s0l = s0->single_child()||s0->head_left() ? 0 : s0->left_child;
      s0r = s0->single_child()||!s0->head_left() ? 0 : s0->right_child;
      s0u = s0->single_child() ? s0->left_child : 0;
//      s0h = s0->is_constituent() ? (s0->single_child()||s0->head_left() ? s0->left_child : s0->right_child) : 0;

      //s1l = s1==0 ? 0 : ( s1->is_constituent() ? (s1->single_child()||s1->head_left() ? 0 : s1->left_child) : 0 );
      //s1r = s1==0 ? 0 : ( s1->is_constituent() ? (s1->single_child()||!s1->head_left() ? 0 : s1->right_child) : 0 );
      //s1u = s1==0 ? 0 : ( s1->is_constituent() ? (s1->single_child() ? s1->left_child : 0) : 0 );
      s1l = s1==0 ? 0 : (s1->single_child()||s1->head_left() ? 0 : s1->left_child);
      s1r = s1==0 ? 0 : (s1->single_child()||!s1->head_left() ? 0 : s1->right_child);
      s1u = s1==0 ? 0 : (s1->single_child() ? s1->left_child : 0);
//      s1h = s1==0 ? 0 : ( s1->is_constituent() ? (s1->single_child()||s1->head_left() ? s1->left_child : s1->right_child) : 0 );
   
      s0c.load( constituent_or_none(*s0) );
      TRACE("x->begin_c:" << s0->begin_c);
      TRACE("x->end_c:" << s0->end_c);
      TRACE("x->head_c:" << s0->head_c);
      for(int i = 0; i < sentence.size(); i++)
      {
      	TRACE_WORD(i << "#" << sentence[i] << " ");
      }
      TRACE((wrds.m_nMaxSentenceSize));
      TRACE("x->is_partial()" << s0->is_partial());
      TRACE(modify);
      CJointTreeNode tempnode;
      s0->toCJointTreeNode(tempnode);
      TRACE(tempnode.str());
      unsigned ids0w = _load_word(s0);
      TRACE(ids0w);
      s0w.load(_load_word(s0));
      s0t.load(s0->pos);
      s0wt.load(s0w, s0t);
      s0z.load(_load_char(s0));

   
      s1c.load( s1==0 ? CConstituent::SENTENCE_BEGIN : constituent_or_none(*s1) );
      s1w.load(s1 == 0 ? g_noneWord.code() : _load_word(s1));
      s1t.load(s1 == 0 ? g_noneTag.code() : s1->pos);
      s1z.load(s1 == 0 ? g_noneWord.code() : _load_char(s1));
   
      s2c.load( s2==0 ? CConstituent::SENTENCE_BEGIN : constituent_or_none(*s2) );
      s2w.load(s2 == 0 ? g_noneWord.code() : _load_word(s2));
      s2t.load(s2 == 0 ? g_noneTag.code() : s2->pos);
      s2z.load(s2 == 0 ? g_noneWord.code() : _load_char(s2));
   
      s3c.load( s3==0 ? CConstituent::SENTENCE_BEGIN : constituent_or_none(*s3) );
      s3w.load(s3 == 0 ? g_noneWord.code() : _load_word(s3));
      s3t.load(s3 == 0 ? g_noneTag.code() : s3->pos);
      s3z.load(s3 == 0 ? g_noneWord.code() : _load_char(s3));

   
      s0lc.load( s0l==0 ? CConstituent::SENTENCE_BEGIN : constituent_or_none(*s0l) );
      s0lw.load(s0l == 0 ? g_noneWord.code() : _load_word(s0l));
      s0lt = s0l == 0 ? g_noneTag : CTag(s0l->pos);

      s0rc.load(s0r==0 ? CConstituent::SENTENCE_BEGIN : constituent_or_none(*s0r));
      s0rw.load(s0r == 0 ? g_noneWord.code() : _load_word(s0r));
      s0rt.load(s0r == 0 ? g_noneTag.code() : s0r->pos);

      s0uc.load(s0u==0 ? CConstituent::SENTENCE_BEGIN : constituent_or_none(*s0u));
      s0uw.load(s0u == 0 ? g_noneWord.code() : _load_word(s0u));
      s0ut = s0u == 0 ? g_noneTag : CTag(s0u->pos);

      s1lc.load( s1l==0 ? CConstituent::SENTENCE_BEGIN : constituent_or_none(*s1l) );
      s1lw.load(s1l == 0 ? g_noneWord.code() : _load_word(s1l));
      s1lt.load(s1l == 0 ? g_noneTag.code() : s1l->pos);

      s1rc.load(s1r==0 ? CConstituent::SENTENCE_BEGIN : constituent_or_none(*s1r));
      s1rw.load(s1r == 0 ? g_noneWord.code() : _load_word(s1r));
      s1rt.load(s1r == 0 ? g_noneTag.code() : s1r->pos);

      s1uc.load(s1u==0 ? CConstituent::SENTENCE_BEGIN : constituent_or_none(*s1u));
      s1uw.load(s1u == 0 ? g_noneWord.code() : _load_word(s1u));
      s1ut.load(s1u == 0 ? g_noneTag.code() : s1u->pos);

 /*
		n0w.load(_load_word(n0));
		n0t.load(n0->pos);
		n0wt.load(n0w, n0t);

		n1w.load(_load_word(n1));
		n1t.load(n1->pos);
		n1wt.load(n1w, n1t);

		n2w.load(_load_word(n2));
		n2t.load(n2->pos);
		n2wt.load(n2w, n2t);

		n3w.load(_load_word(n3));
		n3t.load(n3->pos);
		n3wt.load(n3w, n3t);
*/
      p2z = p2 == -1 ? g_noneWord : _load_char_bypos(p2);
      p1z = p1 == -1 ? g_noneWord : _load_char_bypos(p1);
      n0z = n0 == -1 ? g_noneWord : _load_char_bypos(n0);
      n1z = n1 == -1 ? g_noneWord : _load_char_bypos(n1);
      n2z = n2 == -1 ? g_noneWord : _load_char_bypos(n2);
      n3z = n3 == -1 ? g_noneWord : _load_char_bypos(n3);

      if (modify==false) {
         if (s1!=0) s0ws1w.refer(&s0w, &s1w);
         //if (n0!=-1) s0wn0w.refer(&s0w, &n0w);
         //if (n1!=-1) n0wn1w.refer(&n0w, &n1w);
         //if (s1!=0&&n0!=-1) s1wn0w.refer(&s1w, &n0w);
         if (s1!=0) s0wts1wt.refer(&s0wt, &s1wt);
      }
      else {
         if (s1!=0) s0ws1w.allocate(s0w, s1w);
         //if (n0!=-1) s0wn0w.allocate(s0w, n0w);
         //if (n1!=-1) n0wn1w.allocate(n0w, n1w);
         //if (s1!=0&&n0!=-1) s1wn0w.allocate(s1w, n0w);
         if (s1!=0) s0wts1wt.allocate(s0wt, s1wt);
      }
      s0cs1c.load(s0c, s1c);
      s0ts1t.load(s0t, s1t);
      //s0cn0t.load(s0c, n0t);
      //n0tn1t.load(n0t, n1t);
      //s1cn0t.load(s1c, n0t);
      //s0tn0t.load(s0t, n0t);
      //s0cs1cn0t.copy(s0cs1c); s0cs1cn0t.add(n0t);
      s0cs1cs2c.copy(s0cs1c); s0cs1cs2c.add(s2c);
      //s0cn0tn1t.copy(s0cn0t); s0cn0tn1t.add(n1t);
      //s0ts1tn0t.copy(s0ts1t); s0ts1tn0t.add(n0t);
      s0ts1ts2t.copy(s0ts1t); s0ts1ts2t.add(s2t);
      //s0tn0tn1t.copy(s0tn0t); s0tn0tn1t.add(n1t);

      //if (s0c.empty()) s0js1jn0t.load(s0t); else s0js1jn0t.load(s0c);
      //if (s1c.empty()) s0js1jn0t.add(s1t); else s0js1jn0t.add(s1c);
      //s0js1jn0t.add(n0t);

     //if (s0c.empty()) s0jn0tn1t.load(s0t); else s0jn0tn1t.load(s0c);
      //s0jn0tn1t.add(n0t);
      //s0jn0tn1t.add(n1t);

      if (s0c.empty()) s0js1js2j.load(s0t); else s0js1js2j.load(s0c);
      if (s1c.empty()) s0js1js2j.add(s1t); else s0js1js2j.add(s1c);
      if (s2c.empty()) s0js1js2j.add(s2t); else s0js1js2j.add(s2c);

      // s0 s0r and n0 -- by presuming that n0 exists!
      // s0rc is used when s0r exists, while s0rj is used all times
      // s0rc can be empty is s0r is not a constituent
      // s0rj is s0rc when s0rc not empty, and s0rt otherwise
      // s0rj empty only when s0r non existant
      s0cs0rc.load(s0c, s0rc);
     // s0cs0rcn0t.copy(s0cn0t); s0cs0rcn0t.add(s0rc);

     // s0cs0rjn0t.copy(s0cn0t);
      //if (s0rc.empty()) s0cs0rjn0t.add(s0rt); else s0cs0rjn0t.add(s0rc);

      // s0 s0l and s1 -- by presuming that s1 exists!
      // see comments above
      s0cs0lc.load(s0c, s0lc);
      s0cs0lcs1c.copy(s0cs1c);
      s0cs0lcs1c.add(s0lc);

      s0cs0ljs1j.load(s0c);
      if (s0lc.empty()) s0cs0ljs1j.add(s0lt); else s0cs0ljs1j.add(s0lc);
      if (s1c.empty()) s0cs0ljs1j.add(s1t); else s0cs0ljs1j.add(s1c);

      // s0 slr and s1 -- by presuming that s1 exists!
      s1cs1rc.load(s1c, s1rc);
      s0cs1cs1rc.copy(s0cs1c);
      s0cs1cs1rc.add(s1rc);

      if (s0c.empty()) s0js1cs1rj.load(s0t); else s0js1cs1rj.load(s0c);
      s0js1cs1rj.add(s1c);
      if (s1rc.empty()) s0js1cs1rj.add(s1rt); else s0js1cs1rj.add(s1rc);

      // a tag between s0 and s1
      s0ts1tbt = s0ts1t; s0ts1tbt.add(g_noneTag);
      //addedzms attention
     // between_tag.clear();
      //if (s1!=0)
     // for (i=s1->lexical_head+1; i<s0->lexical_head; ++i) {
     //    between_tag.push_back(wrds[i].tag);
     // }

      return;
   }
};
                           
