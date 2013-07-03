// Copyright (C) University of Oxford 2010
/*===============================================================
 *
 * stanford dependencies in ZPar  
 * Authors: Miguel Ballesteros and Yue Zhang
 * UPF-Spain and SUTD-Singapore
 *
 *==============================================================*/

#ifndef _COMMON_CON_PARSER_STATEITEM
#define _COMMON_CON_PARSER_STATEITEM

#include "action.h"
#include "linkclass.h"


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
   // fields for tokens and constituents
   int lexical_head;
   int lexical_start;
   int lexical_end; 
 
   CLink* cLinkCollapsed; //miguel
   CLink* cLink; //miguel
   
   const CHeadFinder* headFinder;

public:
   inline bool head_left() const { return type==HEAD_LEFT; }
   inline bool single_child() const { return type==SINGLE_CHILD; }
   inline bool is_constituent() const { return type!=LEAF; }

public:
   CStateNode(const int &id, const NODE_TYPE &type, const bool &temp, const unsigned long &constituent, CStateNode *left_child, CStateNode *right_child, const int &lexical_head, const int &lexical_start, const int &lexical_end) : id(id), type(type), temp(temp), constituent(constituent), left_child(left_child), right_child(right_child), lexical_head(lexical_head), lexical_start(lexical_start), lexical_end(lexical_end), cLinkCollapsed(0), cLink(0) {}
   CStateNode() : id(-1), type(), temp(0), constituent(), left_child(0), right_child(0), lexical_head(0), lexical_start(0), lexical_end(0), cLinkCollapsed(0), cLink(0) {}
   virtual ~CStateNode() {
	   CLink* temp=cLink;
	   while (temp!=0) {
		   CLink* next=temp->next;
		   delete temp;
		   temp=next;
	   }
   }
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
      this->lexical_start = 0; 
      this->lexical_end = 0; 

      //this->cLink = 0; //miguel
      this->cLink = 0; //miguel
      //this->cLinkCollapsed =0; //miguel
      this->cLinkCollapsed = 0; //miguel
   }
   void set(const int &id, const NODE_TYPE &type, const bool &temp, const unsigned long &constituent, const CStateNode *left_child, const CStateNode *right_child, const int &lexical_head, const int &lexical_start, const int &lexical_end) { 
      this->id = id;
      this->type = type; 
      this->temp = temp; 
      this->constituent = constituent; 
      this->left_child = left_child; 
      this->right_child = right_child; 
      this->lexical_head = lexical_head; 
      this->lexical_start = lexical_start; 
      this->lexical_end = lexical_end;
      
      this->cLink=0; //Miguel
      
   }//{}

   bool operator == (const CStateNode &nd) const {
      return id == nd.id &&
             type == nd.type && 
             temp == nd.temp && 
             constituent == nd.constituent && 
             left_child == nd.left_child && 
             right_child == nd.right_child &&
             lexical_head == nd.lexical_head;
             lexical_start == nd.lexical_start;
             lexical_end == nd.lexical_end;
             
             //the stanford links are missing. Let's see whether they are necessary or not. (Miguel)
   }
   void operator = (const CStateNode &nd) {
      id = nd.id;
      type = nd.type;
      temp = nd.temp;
      constituent.copy(nd.constituent);
      left_child = nd.left_child;
      right_child = nd.right_child;
      lexical_head = nd.lexical_head;
      lexical_start = nd.lexical_start;
      lexical_end = nd.lexical_end;
      
      //the stanford links are missing. Let's see whether they are necessary or not. (Miguel)
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
      node.token = lexical_head;
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
	
   const std::vector<CTaggedWord<CTag, TAG_SEPARATOR> > *words; //Miguel
	
   SCORE_TYPE score;
   CStateNode node; //head of the stack (top)
   const CStateItem *statePtr; //points to the state item before the action is applied, (chain of actions)
   const CStateItem *stackPtr; //tail of the stack
   int current_word;
   CAction action;
#ifdef TRAIN_LOSS
   CStack<CLabeledBracket> gold_lb;
   unsigned correct_lb;
   unsigned plost_lb;
   unsigned rlost_lb;
   bool bTrain;
#endif
#ifdef SCALE
   unsigned size;
#endif
   
public:
#ifdef TRAIN_LOSS
#define LOSS_CON , correct_lb(0), plost_lb(0), rlost_lb(0), bTrain(false)
#else
#define LOSS_CON 
#endif
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
#ifdef TRAIN_LOSS
      gold_lb.clear();
      correct_lb=0;
      plost_lb=0;
      rlost_lb=0;
      bTrain = false;
#endif
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
   void shift(CStateItem *retval, const unsigned long &constituent = CConstituent::NONE) const {
      //TRACE("shift");
      assert(!IsTerminated());
      retval->node.set(node.id+1, CStateNode::LEAF, false, constituent, 0, 0, current_word, current_word, current_word);
      retval->current_word = current_word+1;
      retval->stackPtr = this; ///  
#ifdef TRAIN_LOSS
      retval->bTrain = this->bTrain;
      computeShiftLB(&(retval->gold_lb), retval->correct_lb, retval->plost_lb, retval->rlost_lb);
#endif
      assert(!retval->IsTerminated());
   }
   void reduce(CStateItem *retval, const unsigned long &constituent, const bool &single_child, const bool &head_left, const bool &temporary) const {
      //TRACE("reduce");
      assert(!IsTerminated());
      const static CStateNode *l, *r;
      assert(stackPtr!=0);
      retval->current_word = current_word; //modified by Miguel and Yue 3 jul 12.01 pm
      if (single_child) {
         assert(head_left == false);
         assert(temporary == false);
         l = &node;
         retval->node.set(node.id+1, CStateNode::SINGLE_CHILD, false, constituent, l, 0, l->lexical_head, l->lexical_start, l->lexical_end);
         retval->stackPtr = stackPtr;
#ifdef TRAIN_LOSS
         retval->bTrain = this->bTrain;
         computeReduceUnaryLB(&(retval->gold_lb), retval->correct_lb, retval->plost_lb, retval->rlost_lb, constituent);
#endif
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
         retval->node.set(node.id+1, (head_left?CStateNode::HEAD_LEFT:CStateNode::HEAD_RIGHT), temporary, fullconst, l, r, (head_left?l->lexical_head:r->lexical_head), l->lexical_start, r->lexical_end);
        

         retval->stackPtr = stackPtr->stackPtr;
#ifdef TRAIN_LOSS
         retval->bTrain = this->bTrain;
         computeReduceBinaryLB(&(retval->gold_lb), retval->correct_lb, retval->plost_lb, retval->rlost_lb, fullconst);
#endif
	//SOMEWHERE in this else block we MAKE THE stanford LINKS (Miguel)
	 //By using: R and L. Which are the right child and left child. 
	 //R and L are state nodes. So, the idea is to make links between them. And store them in CLink* stanfordLinks; //miguel
	 // Take into account that we process it in Inorder (or left order)
	
         //retval->node.generateStanford(); //here we call the method that generates the stanford dependencies which is in CStateNode
         
        	 //retval->generateStanford(head_left); //collapsed and then uncollapsed
         	retval->generateStanfordLinks(); //collapsed and then uncollapsed
        	 
      }

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
#ifdef TRAIN_LOSS
      retval->bTrain = this->bTrain;
      computeTerminateLoss(&(retval->gold_lb), retval->correct_lb, retval->plost_lb, retval->rlost_lb);
#endif
      assert(retval->IsTerminated());
   }
   void noact(CStateItem *retval) const {
      //TRACE("noact");
      assert(IsTerminated());
      retval->node = this->node;
      retval->stackPtr=this->stackPtr;
      retval->current_word = current_word;
      // compute loss
#ifdef TRAIN_LOSS
      retval->bTrain = this->bTrain;
      computeIdleLoss(&(retval->gold_lb), retval->correct_lb, retval->plost_lb, retval->rlost_lb);
#endif
   }

protected:

#ifdef TRAIN_LOSS
   void computeShiftLB( CStack<CLabeledBracket> *gold, unsigned &correct, unsigned &plost, unsigned &rlost) const {
      // compute shift
      if (gold) gold->clear();
      correct = correct_lb;
      plost = plost_lb;
      rlost = rlost_lb;
      if (!bTrain) return;
      static CStack< CLabeledBracket >::const_iterator it;
      it = gold_lb.begin();
      while ( it != gold_lb.end() ) {
         if ( node.valid() && (*it).end == node.lexical_end ) {
            ++ (rlost);
         }
         else {
            if (gold) gold->push( *it );
         }
         ++it;
      } // while
   }
   
   void computeReduceUnaryLB(CStack<CLabeledBracket> *gold, unsigned &correct, unsigned &plost, unsigned &rlost, const unsigned long &constituent) const {
      static CStack< CLabeledBracket >::const_iterator it;
      static bool bCorrect;
      if (gold) gold->clear();
      plost = plost_lb;
      rlost = rlost_lb;
      correct = correct_lb;
      if (!bTrain) return;
      // loop
      it = gold_lb.begin();
      bCorrect = false;
      while ( it != gold_lb.end() ) {
         if ( (*it).begin == node.lexical_start &&
              (*it).end == node.lexical_end &&
              (*it).constituent == constituent ) {
            bCorrect = true;
            ++correct;
         }
         else {
            if (gold) gold->push(*it);
         }
         ++it;
      } //while
      if (!bCorrect)
         ++(plost);
   }

   void computeReduceBinaryLB(CStack<CLabeledBracket> *gold, unsigned &correct, unsigned &plost, unsigned &rlost, const unsigned long &constituent) const {
      const static CStateNode *l, *r;
      static CStack< CLabeledBracket >::const_iterator it;
      static bool bCorrect;
      if (gold) gold->clear();
      correct = correct_lb;
      plost = plost_lb;
      rlost = rlost_lb;
      if (!bTrain) return;
      r = &node;
      l = &(stackPtr->node);
      // loop
      bCorrect = false;
      it=gold_lb.begin();
      while ( it != gold_lb.end() ) {
         if ( (*it).begin == l->lexical_start &&
              (*it).end == r->lexical_end &&
              (*it).constituent == constituent ) {
            bCorrect=true;
            ++correct;
         }
         else if ( (*it).begin == r->lexical_start ) {
            ++(rlost);
         }
         else {
            if (gold) gold->push(*it);
         }
         ++it;
      } // while
      if (!bCorrect)
         ++(plost);
   }
   void computeTerminateLoss(CStack<CLabeledBracket> *gold, unsigned &correct, unsigned &plost, unsigned &rlost) const {
      if (gold) gold->clear();
      correct = correct_lb;
      plost = plost_lb;
      rlost = rlost_lb;
      if (!bTrain) return;
      static CStack< CLabeledBracket >::const_iterator it;
      it = gold_lb.begin();
      while ( it != gold_lb.end() ) {
         assert( (*it).begin == node.lexical_start && (*it).end == node.lexical_end);
         ++(rlost);
         ++it;
      }//while
   }
   void computeIdleLoss(CStack<CLabeledBracket> *gold, unsigned &correct, unsigned &plost, unsigned &rlost) const {
      if (gold) gold->clear();
      correct = correct_lb;
      plost = plost_lb;
      rlost = rlost_lb;
      if (!bTrain) return;
   }
#endif

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
            retval.encodeShift(snt.nodes[newNodeIndex()].constituent.code()); return;
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
         retval.encodeShift(tr.nodes[newNodeIndex()].constituent.code());
         return;
      }
      if (tr.parent(node.id) == -1) {
         assert(IsComplete(tr.words.size()));
         retval.encodeReduceRoot();
         return;
      }
      NextMove(tr, retval);
   }

   void Move(CStateItem *retval, const CAction &action) const {
      retval->action = action; // this makes it necessary for the actions to 
      retval->statePtr = this; // be called by Move
      
      retval->words = this->words; // Miguel
      
      if (action.isIdle()) {
         noact(retval);
#ifdef SCALE
         retval->size = this->size;
#endif
      }
      else {
         if (action.isShift())
            { shift(retval, action.getConstituent()); }
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
      for (i=0; i<tagged.size(); ++i) 
         out.newWord(tagged[i].first, tagged[i].second);
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
         nodes[i]->toCCFGTreeNode(out.nodes[j]);
         //iterate through nodes[i] Clink
        //printing the stanford dependencies
         const CLink* temp=nodes[i]->cLink;
         while(temp!=0) {
        	 //std::cout<<"i="<<i<<": "<<temp->label<<"("<<tagged.at(nodes[i]->lexical_head).first<<", "<<tagged.at(temp->dependent).first<<")\n"; //miguel
        	 std::cout<<"i="<<i<<": "<<temp->label<<"("<<tagged.at(temp->head).first<<", "<<tagged.at(temp->dependent).first<<")\n"; //miguel
        	 temp=temp->next;
         }
      }
      out.root = nodes[0]->id;
   }


   //===============================================================================

   void trace(const CTwoStringVector *s=0) const {
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
      #ifdef SCALE
      TRACE("State item score == " << score);
      TRACE("State item size == " << size);
      #endif
#ifdef TRAIN_LOSS
      TRACE("cor = " << correct_lb << ", plo = " << plost_lb << ", rlo = " << rlost_lb << ", Loss = " << FLoss());
#endif
      --count;
      while (count>=0) {
         if (s) {
            TRACE(states[count]->action.str()<<" ["<<(states[count]->stacksize()>0?s->at(states[count]->node.lexical_head).first:"")<<"]"); 
         }
         else {
            TRACE(states[count]->action.str());
         }
         --count;
      }
      TRACE("");
   }
   
   /*void generateStanford(bool head_left) {
   	   
   	   //TODO MIGUEL
   	   //this method should include the stanford dependencies from the left child and the right child
   	   
   	   //1st, create grammaticalstructure.
   	   //2nd. use semantic head finder.
   	   //3rd. add transformations.
   	   //if a head is not included in the subtree just forget about it... so far, so good. Speak with yue about this.
   	   generateStanfordLinks();
	   
   	   /*if (head_left){ 
   		   //the head is the left, therefore the right_child is the dependent.
   		   //find dependency label for this thing. We already have the arcs.
   		   updateStanfordLink(node.right_child->lexical_head);
   	   }
   	   else { 
   		   //the head is the right, therefore the left_child is the dependent.
   		   //find dependency label for this thing. We already have the arcs.
   		   updateStanfordLink(node.left_child->lexical_head);
   	   }*/
   	   
   	   
      //}
   
   //Miguel
   //this method generates the stanford links that are available for the current node.
   void generateStanfordLinks() {
	   
	   
	   //nsubj
	   //S < (NP=target $+ NP|ADJP) > VP
	   buildNsubj1(&this->node);
	   //SQ|PRN < (NP=target !< EX $++ VP)
	   //if (buildNsubj2(this->node))  return;
	   //"S < ((NP|WHNP=target !< EX !<# (/^NN/ < (" + timeWordRegex + "))) $++ VP)"
	   //if (buildNsubj3(this->node))  return;
	   //"S < ( NP=target <# (/^NN/ < " + timeWordRegex + ") !$++ NP $++VP)",
	   //if (buildNsubj4(this->node))  return;
	   //if (buildNsubj5(this->node))  return;
	   //"SQ < ((NP=target !< EX) $- /^(?:VB|AUX)/ !$++ VP)",
	   //if (buildNsubj6(this->node))  return;
	   //"SQ < ((NP=target !< EX) $- (RB $- /^(?:VB|AUX)/) ![$++ VP])",
	   //if (buildNsubj7(this->node))  return;
	   //"SBARQ < WHNP=target < (SQ < (VP ![$-- NP]))",
	   //if (buildNsubj8(this->node))  return;
	   //"SBARQ < (SQ=target < /^(?:VB|AUX)/ !< VP)",
	   //if (buildNsubj9(this->node))  return;
	   //"SINV < (NP|WHNP=target [ $- VP|VBZ|VBD|VBP|VB|MD|AUX | $- (@RB|ADVP $- VP|VBZ|VBD|VBP|VB|MD|AUX) | !$- __ !$ @NP] )",
	   //if (buildNsubj10(this->node))  return;
	   //"SBAR < WHNP=target [ < (S < (VP !$-- NP) !< SBAR) | < (VP !$-- NP) !< S ]"
	   //if (buildNsubj11(this->node))  return;
	   //"SBAR !< WHNP < (S !< (NP $++ VP)) > (VP > (S $- WHNP=target))",
	   //if (buildNsubj12(this->node))  return;
	   //"SQ < ((NP < EX) $++ NP=target)",
	   //if (buildNsubj13(this->node))  return;
	   //"S < (NP < EX) <+(VP) (VP < NP=target)"
	   //if (buildNsubj14(this->node))  return;
	   
	   //Conj
	   //if (buildConj1(&this->node))  return;
	   //if (buildConj2(this->node))  return;
	   //if (buildConj3(this->node))  return;
	   //if (buildConj4(this->node))  return;
	   //if (buildConj5(this->node))  return;
	   
	   //Copula
	   //if (buildCopula1(&this->node))  return;
	   //if (buildCopula2(this->node))  return;
	   //if (buildCopula3(this->node))  return;
	   //if (buildCopula4(this->node))  return;
	   //if (buildCopula5(this->node))  return;
	   
	   //...
	   
   }
   
   
   
   /**
    * Miguel
    */
   /*void updateStanfordLink(const int dependent) {
	   //cl=new CDependencyLabel(0);
	   	   
	   //this->lexical_start (word).
	   //this->lexical_end (word)
	   	   
	   //start matching patterns. We know the dependent, and we can traverse it.
	   	   
	   CDependencyLabel* label;
	   if (true) {
		   //Nsubject
		   //S < (NP=target $+ NP|ADJP) > VP
		   //WE ALREADY HAVE THE DEPENDENT... so ??? what about pattern matching in which the target is the same one ...???
		   //words->at(node.lexical_head).word; this is the word
		   //words->at(node.lexical_head).tag; this is the pos
		   
		   label=new CDependencyLabel(STANFORD_DEP_NSUBJ);
	   }
	   //pattern matching
	   
	   CLink* newNode=new CLink(*label, dependent, 0);
	   	 	  
	   newNode->next=this->node.cLink;
	   node.cLink=newNode; //the new node (with the arc and label is added to the list)
	 
   }*/
   
   /**
    * Miguel  (checked)
    */
   const CStateNode* findInmediateChild(const CStateNode* node, CConstituent constituent) {
	   if (node->type==CStateNode::LEAF) {
		   return 0;
	   }
	   else if(node->type==CStateNode::SINGLE_CHILD) {
		   if (node->left_child->constituent==constituent) {
			   return node->left_child;
		   }
		   else {
			   return 0;
		   }
	   }
	   else if (node->type==CStateNode::HEAD_LEFT) {
		   if (node->right_child->constituent==constituent) {
			   return node->right_child;
		   }
		   else if (node->left_child->temp) {
			   return findInmediateChild(node->left_child,constituent); //we have to do this because we have binarize structures.
		   }
	   }
	   else if (node->left_child->constituent==constituent) {
		   return node->left_child;
	   }
	   else {
		   return 0;
	   }
   }
   
   /**
     * Miguel (unchecked, but seems fine)
     */
   const CStateNode* findChild(const CStateNode* node, CConstituent constituent) {
   	   if (node->type==CStateNode::LEAF) { //it can't have children!
   		   return 0;
   	   }
   	   else if(node->type==CStateNode::SINGLE_CHILD) {
   		   if (node->left_child->constituent==constituent) {
   			   return node->left_child;
   		   }
   		   else {
   			   const CStateNode* aux=findInmediateChild(node->left_child,constituent);
   			   if (aux!=0) return aux;
   		   }
   	   }
   	   else 
   		{
   		   if (node->right_child->constituent==constituent) {
   		   return node->right_child;
   		   }
   		   else if (node->left_child->constituent==constituent) {
   			   return node->left_child;
   		   }
   		   else {
   			   const CStateNode* aux=findInmediateChild(node->left_child,constituent);
   			   if (aux!=0) return aux;
   			   aux=findInmediateChild(node->right_child,constituent);
   			   if (aux!=0) return aux;			   
   		   }
   		}
   	   return 0;
   	}
   
   /*
    * Miguel 
    */
   const CStateNode* findInmediateLeftSister(const CStateNode* node, CConstituent constituent) {
   		   return 0;
   }
   
   /*
    * Miguel
    */
   bool buildNsubj1(const CStateNode* node) {
	   if (node->constituent==PENN_CON_VP) {
		   const CStateNode* s=findInmediateChild(node, PENN_CON_PENN_CON_S);
		   if (s!=0) {
			   const CStateNode* np=findInmediateChild(s, PENN_CON_NP);
			   if (np!=0) {
				   const CStateNode* npadjp = findInmediateLeftSister(np, PENN_CON_NP);
				   if (npadjp==0) npadjp=findInmediateLeftSister(np, PENN_CON_ADJP); 
				   if (npadjp!=0) {
					   CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_NSUBJ);
					   buildStanfordLink(label, np->lexical_head, s->lexical_head);
					   return true;
				   }
			   }
		   }
	   }
	   return false;
   }
   
   
   void buildStanfordLink(CDependencyLabel* label, int dependent, int head) {
   	   CLink* newNode=new CLink(*label, dependent, head, 0);
   	   newNode->next=this->node.cLink;
   	   node.cLink=newNode; //the new node (with the arc and label is added to the list)
      }

   //===============================================================================
#ifdef TRAIN_LOSS   
public:
   SCORE_TYPE actionLoss(const CAction &action, unsigned &correct, unsigned &plost, unsigned &rlost) const {
      static unsigned long constituent;
      if (action.isShift()) {
         computeShiftLB(0, correct, plost, rlost);
      }
      else if (action.isReduceRoot()) {
         computeTerminateLoss(0, correct, plost, rlost);
      }
      else if (action.isIdle()) {
         computeIdleLoss(0, correct, plost, rlost);
      }
      else if (action.singleChild()) {
         computeReduceUnaryLB(0, correct, plost, rlost, action.getConstituent());
      }
      else {
#ifdef NO_TEMP_CONSTITUENT
         constituent = action.getConstituent();
#else
         constituent = CConstituent::encodeTmp(action.getConstituent(), action.isTemporary());
#endif
         computeReduceBinaryLB(0, correct, plost, rlost, constituent);
      }
   }
   SCORE_TYPE actionFLoss(const CAction &action) const {
      static unsigned correct, plost, rlost;
      actionLoss(action, correct, plost, rlost);
      return FLoss(correct, plost, rlost);
   }
   SCORE_TYPE FLoss(const unsigned &correct, const unsigned &plost, const unsigned &rlost) const {
      static SCORE_TYPE p, r, f;
      if (correct == 0) {
         if (plost == 0 && rlost == 0) {
            return static_cast<SCORE_TYPE>(0);
         }
         else {
            return static_cast<SCORE_TYPE>(1);
         }
      }
      p = static_cast<SCORE_TYPE>(correct) / (correct + plost);
      r = static_cast<SCORE_TYPE>(correct) / (correct + rlost);
      f = 2*static_cast<SCORE_TYPE>(p)*r / (p + r);
      return 1-f;
   }
   SCORE_TYPE FLoss() const {
      return FLoss(correct_lb, plost_lb, rlost_lb);
   }
   SCORE_TYPE HammingLoss() const {
      return plost_lb + rlost_lb;
   }
   SCORE_TYPE stepHammingLoss() const {
      if (statePtr == 0)
         return 0;
      return plost_lb-statePtr->plost_lb + rlost_lb-statePtr->rlost_lb;
   }
   SCORE_TYPE actionHammingLoss(const CAction &action) const {
      static unsigned correct, plost, rlost;
      actionLoss(action, correct, plost, rlost);
      return plost + rlost;
   }
   SCORE_TYPE actionStepHammingLoss(const CAction &action) const {
      static unsigned correct, plost, rlost;
      actionLoss(action, correct, plost, rlost);
      return plost-plost_lb + rlost-rlost_lb;
   }
#endif
   
   
   
   
   
   
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
#ifdef TRAIN_LOSS
#define LOSS_ADD + item->actionStepHammingLoss(action)
//#define LOSS_ADD -std::sqrt(item->HammingLoss()) + std::sqrt(item->actionHammingLoss(action))
#else
#define LOSS_ADD 
#endif
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


