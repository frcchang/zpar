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

//static copulateVerbs({"be", "being", "been", "am", "are", "r", "is", "ai", "was", "were", "'m", "'re", "'s", "s", "seem", "seems", "seemed", "appear", "appears", "appeared", "stay", "stays", "stayed", "remain", "remains", "remained", "resemble", "resembles", "resembled", "become", "becomes", "became"};

static CWord g_word_mondays("Mondays");
static CWord g_word_monday("Monday");
static CWord g_word_tuesdays("Tuesdays");
static CWord g_word_tuesday("Tuesday");
static CWord g_word_wednesdays("Wednesdays");
static CWord g_word_wednesday("Wednesday");
static CWord g_word_thursdays("Thursdays");
static CWord g_word_thursday("Thursday");
static CWord g_word_fridays("Fridays");
static CWord g_word_friday("Friday");
static CWord g_word_saturdays("Saturdays");
static CWord g_word_saturday("Saturday");
static CWord g_word_sundays("Sundays");
static CWord g_word_sunday("Sunday");
static CWord g_word_years("years");
static CWord g_word_year("year");
static CWord g_word_months("months");
static CWord g_word_month("month");
static CWord g_word_weeks("weeks");
static CWord g_word_week("week");
static CWord g_word_days("days");
static CWord g_word_day("day");
static CWord g_word_mornings("mornings");
static CWord g_word_morning("morning");
static CWord g_word_evenings("evenings");
static CWord g_word_evening("evening");
static CWord g_word_nights("nights");
static CWord g_word_night("night");
static CWord g_word_january("January");
static CWord g_word_jan("Jan."); //...Jan+something
static CWord g_word_february("February");
static CWord g_word_feb("Feb.");//... Feb+something
static CWord g_word_march("March");
static CWord g_word_mar("Mar.");//... Mar+something
static CWord g_word_april("April");
static CWord g_word_apr("Apr.");//... Apr+something
static CWord g_word_may("May");
static CWord g_word_june("June");
static CWord g_word_july("July");
static CWord g_word_august("August");
static CWord g_word_aug("Aug.");//... Apr+something
static CWord g_word_september("September");
static CWord g_word_sept("Sept.");//... Apr+something
static CWord g_word_october("October");
static CWord g_word_oct("Oct.");//... Apr+something
static CWord g_word_november("November");
static CWord g_word_nov("Nov.");//... Apr+something
static CWord g_word_december("December");
static CWord g_word_dec("Dec.");//... Apr+something
static CWord g_word_today("today");
static CWord g_word_yesterday("yesteday");
static CWord g_word_tomorrow("tomorrow");
static CWord g_word_spring("spring");
static CWord g_word_summer("summer");
static CWord g_word_fall("fall");
static CWord g_word_autumn("autumn");
static CWord g_word_winter("winter");

static CWord g_word_underunder("__");
//static CWord g_word_lot("lot"); //?





#include "action.h"
#include "linkclass.h"
class CStateNode;

/*===============================================================
 *
 * CStateNodeList - tree nodes in a list 
 *
 *==============================================================*/
class CStateNodeList {
	public:
		const CStateNode* node;
		CStateNodeList* next;
		CStateNodeList* previous;
	
		CStateNodeList() : node(0), next(0), previous(0) {}
		virtual ~CStateNodeList() {}
		void clear() {node=0; next=0; previous=0;}
		
		void add(const CStateNode* new_node) {
			if (node==0){
				this->node=new_node;
				this->next=0;
				this->previous=0;
			}
			else {
				CStateNodeList* temp=this;
				while(temp->next!=0) {
					temp=temp->next;  //we iterate just until the very last node.
				}
				CStateNodeList* list_new=new CStateNodeList();
				list_new->node=node;
				temp->next=list_new;
				list_new->previous=temp;
			}
		}
		
		void add(CStateNodeList* list) {
			CStateNodeList* temp=this;
			while(temp->next!=0) {
				temp=temp->next;  //we iterate just until the very last node.
			}
			temp->next=list;
			list->previous=temp;
		}
		
		bool empty(){
			return (node==0 && next==0 && previous==0);
		}
};



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
   
   
   CStateNodeList* m_umbinarizedSubNodes; //list of subnodes of first level //Miguel
   //CHashMap<unsigned long, int>* m_subnodes; //list of subnodes //Miguel
   
   CStateNodeList* danglingSubNodes;
   
   
   // fields for tokens and constituents
   int lexical_head;
   int lexical_start;
   int lexical_end; 
 
   CLink* stfLinksCollapsed; //miguel
   CLink* stfLinks; //miguel
   
   //const CHeadFinder* headFinder;

public:
   inline bool head_left() const { return type==HEAD_LEFT; }
   inline bool single_child() const { return type==SINGLE_CHILD; }
   inline bool is_constituent() const { return type!=LEAF; }

public:
   CStateNode(const int &id, const NODE_TYPE &type, const bool &temp, const unsigned long &constituent, CStateNode *left_child, CStateNode *right_child, const int &lexical_head, const int &lexical_start, const int &lexical_end,bool headFound) : id(id), type(type), temp(temp), constituent(constituent), left_child(left_child), right_child(right_child), lexical_head(lexical_head), lexical_start(lexical_start), lexical_end(lexical_end), stfLinksCollapsed(0), stfLinks(0), m_umbinarizedSubNodes(0),danglingSubNodes(0) {
	   m_umbinarizedSubNodes=new CStateNodeList();
	   danglingSubNodes=new CStateNodeList();
	   //std::cout<<"miguel";
   }
   CStateNode() : id(-1), type(), temp(0), constituent(), left_child(0), right_child(0), lexical_head(0), lexical_start(0), lexical_end(0), stfLinksCollapsed(0), stfLinks(0), m_umbinarizedSubNodes(0),danglingSubNodes(0) {}
   virtual ~CStateNode() {
	   CLink* temp=stfLinks;
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

      //this->stfLinks = 0; //miguel
      this->stfLinks = 0; //miguel
      //this->stfLinksCollapsed =0; //miguel
      this->stfLinksCollapsed = 0; //miguel
      
      m_umbinarizedSubNodes=0;
      //m_subnodes=0;
      danglingSubNodes=0;
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
      
      this->stfLinks=0; //Miguel
      this->m_umbinarizedSubNodes=0; //Miguel
      m_umbinarizedSubNodes=new CStateNodeList();
      //this->m_subnodes=0; //Miguel
      this->danglingSubNodes=0; //Miguel
      this->danglingSubNodes=new CStateNodeList();
      
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
      
      //retval->node.m_subnodes->clear(); //miguel
      assert(retval->node.m_umbinarizedSubNodes.empty()); //miguel
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
         retval->node.m_umbinarizedSubNodes->add(l);//miguel
         
         //addToHash(retval->node.l->m_umbinarizedSubNodes);//miguel
         /*CStateNodeList* aux=retval->node.left_child->m_umbinarizedSubNodes; //MIGUEL
         //unsigned long c=static_cast<unsigned long>(&node);
         while(aux!=0){
        	 retval->node.m_subnodes[reinterpret_cast<unsigned long> (aux->node)]=1; ///??? ASK YUE. Miguel
        	 //retval->node.m_subnodes[aux->node]=1;
        	 aux=aux->next;
         }*/
         
         
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
         if (l->temp) {
        	retval->node.m_umbinarizedSubNodes->add(l->m_umbinarizedSubNodes);
         }
         else {
        	 retval->node.m_umbinarizedSubNodes->add(l);//miguel
         }
         if (r->temp) {
        	 retval->node.m_umbinarizedSubNodes->add(r->m_umbinarizedSubNodes);
         }
         else {
        	 retval->node.m_umbinarizedSubNodes->add(r);//miguel 
         }
         //addToHash(node.m_umbinarizedSubNodes);
         //CStateNodeList* aux=retval->node.left_child->m_umbinarizedSubNodes; //MIGUEL
         //unsigned long c=static_cast<unsigned long>(&node);
         /*while(aux!=0){
        	 retval->node.m_subnodes[reinterpret_cast<unsigned long> (aux->node)]=1; ///??? ASK YUE
             //retval->node.m_subnodes[aux->node]=1;
             aux=aux->next;
         }
         aux=retval->node.right_child->m_umbinarizedSubNodes; //MIGUEL
         //unsigned long c=static_cast<unsigned long>(&node);
         while(aux!=0){
        	 retval->node.m_subnodes[reinterpret_cast<unsigned long> (aux->node)]=1; ///??? ASK YUE
             //retval->node.m_subnodes[aux->node]=1;
             aux=aux->next;
         }*/
         
         
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
         //if (ret_links) { // I suppose this is what Yue meant.
        	 const CLink* temp=nodes[i]->stfLinks;
        	 while(temp!=0) {
        		 //std::cout<<"i="<<i<<": "<<temp->label<<"("<<tagged.at(nodes[i]->lexical_head).first<<", "<<tagged.at(temp->dependent).first<<")\n"; //miguel
        		 std::cout<<"i="<<i<<": "<<temp->label<<"("<<tagged.at(temp->head).first<<", "<<tagged.at(temp->dependent).first<<")\n"; //miguel
        		 temp=temp->next;
        	 }
         //}
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
   
   
   //Miguel
   //this method generates the stanford links that are available for the current node.
   void generateStanfordLinks() {
	   
	   
	   
	   //nsubj
	   //S < (NP=target $+ NP|ADJP) > VP
	   std::cout<<"Rule 1 \n";
	   buildNsubj1();
	   //SQ|PRN < (NP=target !< EX $++ VP)
	   std::cout<<"Rule 2 \n";
	   buildNsubj2();
	   //"S < ((NP|WHNP=target !< EX !<# (/^NN/ < (" + timeWordRegex + "))) $++ VP)"
	   std::cout<<"Rule 3 \n";
	   buildNsubj3();
	   //"S < ( NP=target <# (/^NN/ < " + timeWordRegex + ") !$++ NP $++VP)",
	   std::cout<<"Rule 4 \n";
	   buildNsubj4();
	   //"S < (NP < EX) <+(VP) (VP < NP=target)"
	   std::cout<<"Rule 5 \n";
	   buildNsubj5();
	   //"SQ < ((NP=target !< EX) $- /^(?:VB|AUX)/ !$++ VP)",
	   std::cout<<"Rule 6 \n";
	   buildNsubj6();
	   //"SQ < ((NP=target !< EX) $- (RB $- /^(?:VB|AUX)/) ![$++ VP])",
	   std::cout<<"Rule 7 \n";
	   buildNsubj7();
	   //"SBARQ < WHNP=target < (SQ < (VP ![$-- NP]))",
	   std::cout<<"Rule 8 \n";
	   buildNsubj8();
	   //"SBARQ < (SQ=target < /^(?:VB|AUX)/ !< VP)",
	   std::cout<<"Rule 9 \n";
	   buildNsubj9();
	   //"SINV < (NP|WHNP=target [ $- VP|VBZ|VBD|VBP|VB|MD|AUX | $- (@RB|ADVP $- VP|VBZ|VBD|VBP|VB|MD|AUX) | !$- __ !$ @NP] )",
	   std::cout<<"Rule 10 \n";
	   buildNsubj10();
	   //"SBAR < WHNP=target [ < (S < (VP !$-- NP) !< SBAR) | < (VP !$-- NP) !< S ]"
	   std::cout<<"Rule 11 \n";
	   buildNsubj11(); //REVISE
	   //"SBAR !< WHNP < (S !< (NP $++ VP)) > (VP > (S $- WHNP=target))",
	   std::cout<<"Rule 12 \n";
	   buildNsubj12();
	   //"SQ < ((NP < EX) $++ NP=target)",
	   std::cout<<"Rule 13 \n";
	   buildNsubj13();
	   
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
   
   //==============================================================================
   //this->compareToTimeWordRegex((*words)[npLeftSister->lexical_head].word);
   bool compareWordToTimeWordRegex(CWord a) {
	   
	   if (a==g_word_mondays) return true;
	   if (a==g_word_monday) return true;
	   if (a==g_word_tuesdays) return true;
	   if (a==g_word_tuesday) return true;
	   if (a==g_word_wednesdays) return true;
	   if (a==g_word_wednesday) return true;
	   if (a==g_word_thursdays) return true;
	   if (a==g_word_thursday) return true;
	   if (a==g_word_friday) return true;
	   if (a==g_word_fridays) return true;
	   if (a==g_word_saturdays) return true;
	   if (a==g_word_saturday) return true;
	   if (a==g_word_sundays) return true;
	   if (a==g_word_sunday) return true;
	   if (a==g_word_days) return true;
	   if (a==g_word_day) return true;
	   if (a==g_word_morning) return true;
	   if (a==g_word_mornings) return true;
	   if (a==g_word_evenings) return true;
	   if (a==g_word_evening) return true;
	   if (a==g_word_nights) return true;
	   if (a==g_word_night) return true;
	   //...
	   if (a==g_word_january) return true;
	   if (a==g_word_jan) return true;
	   
	   if (a==g_word_february) return true;
	   if (a==g_word_feb) return true;
	   	   
	   if (a==g_word_march) return true;
	   if (a==g_word_mar) return true;
	   	
	   if (a==g_word_april) return true;
	   if (a==g_word_apr) return true;
	   
	   if (a==g_word_may) return true;
	   if (a==g_word_june) return true;
	   if (a==g_word_july) return true;
	   if (a==g_word_august) return true;
	   if (a==g_word_aug) return true;
	   if (a==g_word_september) return true;
	   if (a==g_word_sept) return true;
	   if (a==g_word_october) return true;
	   if (a==g_word_oct) return true;
	   if (a==g_word_november) return true;
	   if (a==g_word_nov) return true;
	   if (a==g_word_december) return true;
	   if (a==g_word_dec) return true;
	   if (a==g_word_today) return true;
	   if (a==g_word_yesterday) return true;
	   if (a==g_word_tomorrow) return true;
	   if (a==g_word_spring) return true;
	   if (a==g_word_summer) return true;
	   if (a==g_word_fall) return true;
	   if (a==g_word_autumn) return true;
	   if (a==g_word_winter) return true;
	   
	   //fix that....
	   
	   
	   return false;
   }
   //==============================================================================
  
   
   //S <+ (VP) (VP < .. sthing)
     //it returns the first node that "breaks the chain" of VPs or another thing. In this case:S <+ (VP) (VP < NP), it would return the last NP
     
     //The chain cannot be temporal (vp*) because we are using m_umbinarized_subnodes.
     void findChain(CConstituent via_category, CConstituent target_category, const CStateNode* head, CStateNodeList*& candidates) {
  	   CStateNodeList* headChilds=head->m_umbinarizedSubNodes;
  	   while(headChilds) {
  		   const CStateNode* node=headChilds->node;
  		   if (node->constituent==target_category) {
  			   candidates->add(node);
  		   }
  		   if (node->constituent==via_category) { 
  			   findChain(via_category,target_category,node,candidates);
  		   }
  		   headChilds=headChilds->next;
  	   }
     } 
     
   

   
   //==================================================================================================
   //LEGEND
       //A << B:  A dominates B  m_subnodes
   	   //A < B: A inmediately dominantes B  m_umbinarizedsubnodes.	
       //A $+ B: A is the inmediately left sister of B
       //A $++ B: A is a left sister of B
       //A $- B: A is the inmediately right sister of B.
       //A $-- B: A is the right sister of B
       //A <+(C) B: A dominates B via an unbroken chain of (zero or more) nodes matching description C
   //===================================================================================================
    
    
    bool isDangling(const CStateNode* head, const CStateNode* child){
    	CStateNodeList* danglings=head->danglingSubNodes;
    	while(danglings!=0){
    		if (danglings->node==child) return true;
    		danglings=danglings->next;
    	}
    	return false;
    }
    
    void addDangling(const CStateNode* head, const CStateNode* child){
        	CStateNodeList* danglings=head->danglingSubNodes;
        	danglings->add(child);
        }
    
    //===================================================================================================
    
   /*
    * Miguel: RULES FOR NSUBJ. There are 13
    * 
    * //S < (NP=target $+ NP|ADJP) > VP
    * 
    * Double-checked and seems fine. 11th July 13.
    */
   bool buildNsubj1() {
	   if (node.constituent==PENN_CON_VP) {
		   CStateNodeList* childsVP=node.m_umbinarizedSubNodes;
		   while(childsVP!=0) {
			   const CStateNode* sHead=childsVP->node;
			   if (sHead->constituent==PENN_CON_S) {
				   CStateNodeList* childsS=sHead->m_umbinarizedSubNodes;
				   while(childsS!=0){
					   const CStateNode* npTarg=childsS->node;
					   if (npTarg->constituent==PENN_CON_NP && !(isDangling(sHead,npTarg))) {
						   CStateNodeList* sistersRightNp=childsS;
						   if (sistersRightNp!=0) sistersRightNp=sistersRightNp->next;
						   if(sistersRightNp!=0) { //it is the immediately (just one)
							   const CStateNode* sister=sistersRightNp->node;
							   if (sister->constituent==PENN_CON_NP || sister->constituent==PENN_CON_ADJP) { //FIRE RULE
								   CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_NSUBJ);
								   if (buildStanfordLink(label, npTarg->lexical_head, sHead->lexical_head)){
									   std::cout<<"nSubj1"<<" (head: "<<sHead->lexical_head<<")"<<"(dependent: "<<npTarg->lexical_head<<")\n";
									   addDangling(sHead,npTarg);
									   return true;
								   }
							   }
						   }
					   }
					   childsS=childsS->next;
				   }
			   }
			   childsVP=childsVP->next;
		   }
	   }
	   return false;
	   
   }
   
   
   /*
   *SQ|PRN < (NP=target !< EX $++ VP)
   *
   * Double checked and seems fine.
   */
   bool buildNsubj2() {
	   if (node.constituent==PENN_CON_SQ || node.constituent==PENN_CON_PRN) {
		   CStateNodeList* childs=node.m_umbinarizedSubNodes;
		   while(childs!=0) {
			   const CStateNode* npTarg=childs->node;
			   if (npTarg->constituent==PENN_CON_NP && !(isDangling(&node,npTarg))) {
				   CStateNodeList* childsNp=npTarg->m_umbinarizedSubNodes;
				   bool noExChild=true;
				   while(childsNp!=0){
					   const CStateNode* noex=childsNp->node;
					   if ((*words)[noex->lexical_head].tag.code()==PENN_TAG_EX) {
						   noExChild=false;
					   }
					   childsNp=childsNp->next;
				   }
				   if (noExChild) {
					   CStateNodeList* sistersRightNp=childs;
					   if (sistersRightNp!=0) sistersRightNp=sistersRightNp->next;
					   while(sistersRightNp!=0) { //iterate through the righ sisters of NP
						   const CStateNode* sister=sistersRightNp->node;
						   if (sister->constituent==PENN_CON_VP) { //FIRE RULE
							   CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_NSUBJ);
							   if (buildStanfordLink(label, npTarg->lexical_head, node.lexical_head)) {
								   std::cout<<"nSubj2"<<" (head: "<<node.lexical_head<<")"<<"(dependent: "<<npTarg->lexical_head<<")\n";
								   addDangling(&node,npTarg);
								   return true;
							   }
						   }
						   sistersRightNp=sistersRightNp->next;
					   }
					  
				   }
			   }
			   childs=childs->next;
		   }
	   }
	   return false;  	  
    }
   
   /*
   *"S < ((NP|WHNP=target !< EX !<# (/^NN/ < (" + timeWordRegex + "))) $++ VP)"
   * 
   * Checked and seems fine.
   */
   bool buildNsubj3() {
	   if (node.constituent==PENN_CON_S) {
		   CStateNodeList* childsS=node.m_umbinarizedSubNodes;
		   while(childsS!=0){
			   const CStateNode* npwhnpTarg=childsS->node;
			   if ((npwhnpTarg->constituent==PENN_CON_NP ||npwhnpTarg->constituent==PENN_CON_WHNP)&& !(isDangling(&node, npwhnpTarg))){
				   bool vpSister=false;
				   CStateNodeList* rightSistersNp=childsS;
				   while(rightSistersNp!=0){
					   if (rightSistersNp->node->constituent==PENN_CON_VP){
						   vpSister=true;
				   		}
				   		rightSistersNp=rightSistersNp->next;
				   	}
				   if (vpSister) { //NP|WHNP must have a VP sister.
					   CStateNodeList* childsNpwhnp=npwhnpTarg->m_umbinarizedSubNodes;
					   bool noEx=true;
					   bool noNNTime=true;
					   //NP|WHNP=target !< EX !<# (/^NN/ < (" + timeWordRegex + ")))
					   while(childsNpwhnp!=0){
						   const CStateNode* nnChildNp=childsNpwhnp->node;
						   if ((*words)[nnChildNp->lexical_head].tag.code()==PENN_TAG_EX) {
							   noEx=false;
						   }
						   else if ((nnChildNp->type==CStateNode::LEAF)
								   && (nnChildNp->lexical_head==npwhnpTarg->lexical_head) //<#
								   && (compareWordToTimeWordRegex((*words)[nnChildNp->lexical_head].word))
								   && ((((*words)[nnChildNp->lexical_head].tag.code()==PENN_TAG_NOUN))
								   ||(((*words)[nnChildNp->lexical_head].tag.code()==PENN_TAG_NOUN_PROPER))
								   ||(((*words)[nnChildNp->lexical_head].tag.code()==PENN_TAG_NOUN_PLURAL))
								   ||(((*words)[nnChildNp->lexical_head].tag.code()==PENN_TAG_NOUN_PROPER_PLURAL)))) {
							   noNNTime=false;
						   }
						   childsNpwhnp=childsNpwhnp->next;
					   }
					   if (noEx && noNNTime){ //FIRE RULE
						   CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_NSUBJ);
						   if (buildStanfordLink(label, npwhnpTarg->lexical_head, node.lexical_head)) {
							   addDangling(&node,npwhnpTarg);
							   std::cout<<"nSubj3"<<" (head: "<<node.lexical_head<<")"<<"(dependent: "<<npwhnpTarg->lexical_head<<")\n";
						   	   return true;
						   	}     
					   }
				   }
			   }
			   childsS=childsS->next;
		   }
	   }
	   return false;
   }
   
  /*
   *"S < ( NP=target <# (/^NN/ < " + timeWordRegex + ") !$++ NP $++ VP)",
   * 
   * Checked and seems fine.
   */
   bool buildNsubj4() {
	   if (node.constituent==PENN_CON_S){
		   CStateNodeList* childsS=node.m_umbinarizedSubNodes;
		   while (childsS!=0){
			   const CStateNode* npTarg=childsS->node;
			   if (npTarg->constituent==PENN_CON_NP && !(isDangling(&node, npTarg))) {
				   //$++ and !$++
				   bool noNpSister=true;
				   bool vpSister=false;
				   CStateNodeList* rightSistersNp=childsS;
				   while(rightSistersNp!=0){
					   if (rightSistersNp->node->constituent==PENN_CON_NP){
						   noNpSister=false;
					   }
					   else if (rightSistersNp->node->constituent==PENN_CON_VP){
						   vpSister=true;
					   }
					   rightSistersNp=rightSistersNp->next;
				   }
				   
				   if (noNpSister && vpSister) { //the first NP must have a VP as right sister and it must not have a NP as right sister.
					   CStateNodeList* childsNp=npTarg->m_umbinarizedSubNodes;
					   while(childsNp!=0){
						   const CStateNode* nnChildNp=childsNp->node;
						   
						   if ((nnChildNp->type==CStateNode::LEAF)
							   && (nnChildNp->lexical_head==npTarg->lexical_head) //<#
							   && (compareWordToTimeWordRegex((*words)[nnChildNp->lexical_head].word))
							   && ((((*words)[nnChildNp->lexical_head].tag.code()==PENN_TAG_NOUN))
							   ||(((*words)[nnChildNp->lexical_head].tag.code()==PENN_TAG_NOUN_PROPER))
							   ||(((*words)[nnChildNp->lexical_head].tag.code()==PENN_TAG_NOUN_PLURAL))
							   ||(((*words)[nnChildNp->lexical_head].tag.code()==PENN_TAG_NOUN_PROPER_PLURAL)))) { //FIre rule
							   		CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_NSUBJ);
							   		if (buildStanfordLink(label, npTarg->lexical_head, node.lexical_head)) {
							   			addDangling(&node,npTarg);
							   			std::cout<<"nSubj4"<<" (head: "<<node.lexical_head<<")"<<"(dependent: "<<npTarg->lexical_head<<")\n";
							   			return true;
							  }   
						   }
						   childsNp=childsNp->next;
					   }
				   }
			   }
			   childsS=childsS->next;
		   }
	   }
	   return false;
   }
    
   /*
    *"S < (NP < EX) <+(VP) (VP < NP=target)"
    *
    * Checked and seems fine. 
    */
   bool buildNsubj5() {
	   if (node.constituent==PENN_CON_S) {
		   CStateNodeList* childsS=node.m_umbinarizedSubNodes;
		   while (childsS!=0) {
			   const CStateNode* npChilds=childsS->node;
			   if (npChilds->constituent==PENN_CON_NP) {
				   CStateNodeList* childsNp=npChilds->m_umbinarizedSubNodes;
				   std::cout<<"2\n";
				   bool thereIsEx=false;
      			   while(childsNp!=0){
      				   const CStateNode* exChildnp=childsNp->node;
      				   std::cout<<"before (lex_head):"<<exChildnp->lexical_head<<"\n";
      				   //std::cout<<"problem?:"<<(*words)[exChildnp->lexical_head]<<"\n";
      				   if ((*words)[exChildnp->lexical_head].tag.code()==PENN_TAG_EX) {
      					   	  std::cout<<"after (lex_head):"<<exChildnp->lexical_head<<"\n";
      					   	  thereIsEx=true;
      				   }
      				   childsNp=childsNp->next;
      			   }
      			   if (thereIsEx) {
      				   CStateNodeList* childsS2=node.m_umbinarizedSubNodes;
      				  //std::cout<<"3\n";
      				  while(childsS2!=0){
      					  const CStateNode* vpSonofNode=childsS2->node;
      					  if (vpSonofNode->constituent==PENN_CON_VP) {
      						  //<+ (VP) (VP ...)
      						  CStateNodeList* vpsChain=new CStateNodeList();
      						  std::cout<<"findingchain\n";
      						  findChain(PENN_CON_VP,PENN_CON_VP, vpSonofNode, vpsChain);
      						  //std::cout<<"4\n";
      						  while(vpsChain!=0){
      							  CStateNodeList* childsOfAVp=vpsChain->node->m_umbinarizedSubNodes;
      							  //std::cout<<"5\n";
      							  while(childsOfAVp!=0){
      								  const CStateNode* npTarg=childsOfAVp->node;
      								  if (npTarg->constituent==PENN_CON_NP && !(isDangling(vpsChain->node,npTarg))) {
      									  CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_NSUBJ);
      									  if (buildStanfordLink(label, npTarg->lexical_head, node.lexical_head)) {
      										  addDangling(vpsChain->node,npTarg);
      										  std::cout<<"nSubj5"<<" (head: "<<node.lexical_head<<")"<<"(dependent: "<<npTarg->lexical_head<<")\n";
      										  return true; 
      									  }
      								  }
      								  childsOfAVp=childsOfAVp->next;
      							  }
      							  vpsChain=vpsChain->next;
      							}
      					 }
      					 childsS2=childsS2->next;
      				  }
      			   } //if thereisex
      			} //if NP
      		 childsS=childsS->next;
      	   }//while childsS
      	}
      	return false;
    }
   
    /*
     *SQ < ((NP=target !< EX) $- /^(?:VB|AUX)/ !$++ VP)
     *
     *  Checked and seems fine.
     */
   	bool buildNsubj6() {
   		if (node.constituent==PENN_CON_SQ) {
   			CStateNodeList* childsSq=node.m_umbinarizedSubNodes;
   			while (childsSq!=0){
   				const CStateNode* npTarg=childsSq->node;
   				if (npTarg->constituent==PENN_CON_NP && !(isDangling(&node,npTarg))){
   					bool noEx=true;
   					CStateNodeList* childsNp=npTarg->m_umbinarizedSubNodes;
   					while(childsNp!=0) {
   						if ((*words)[childsNp->node->lexical_head].tag.code()==PENN_TAG_EX) {
   							noEx=false;
   						}
   						childsNp=childsNp->next;
   					}
   					if (noEx){
   						bool noVpRightSister=true;
   						CStateNodeList* sistersNp=childsSq;
   						while(sistersNp!=0){
   							if (sistersNp->node->constituent==PENN_CON_VP) {
   								noVpRightSister=false;
   							}
   							sistersNp=sistersNp->next;
   						}
   						if (noVpRightSister) {
   							sistersNp=childsSq;
   							if (sistersNp->previous!=0) {
   								const CStateNode* npLeftSister=sistersNp->previous->node;
   								if ((*words)[npLeftSister->lexical_head].tag.code()==PENN_TAG_VERB) { //for now, we leave AUX, as it is.
   									//std::cout<<(*words)[npLeftSister->lexical_head].word;
   									CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_NSUBJ);
   									if (buildStanfordLink(label, npTarg->lexical_head, node.lexical_head)) {
   										addDangling(&node,npTarg);
   										std::cout<<"nSubj6"<<" (head: "<<node.lexical_head<<")"<<"(dependent: "<<npTarg->lexical_head<<")\n";
   										return true;
   									}
   								}
   							}
   						}
   					}
   				}
   				childsSq=childsSq->next;
   			}
   		}
   		return false;
   	}
   	
   	
   	/*
   	*"SQ < ((NP=target !< EX) $- (RB $- /^(?:VB|AUX)/) ![$++ VP])",
    *"SQ < ((NP=target !< EX) $- (RB $- /^(?:VB|AUX)/) !$++ VP)", (they are equivalent)
    * 
    * Checked ans seems fine.
    */
 	bool buildNsubj7() {
 		if (node.constituent==PENN_CON_SQ){
 			CStateNodeList* childsSQ=node.m_umbinarizedSubNodes;
 			while(childsSQ!=0){
 				const CStateNode* npTarg=childsSQ->node;
 				if (npTarg->constituent==PENN_CON_NP && !(isDangling(&node,npTarg))){
 					bool noEx=true;
 					CStateNodeList* childsNp=npTarg->m_umbinarizedSubNodes;
 					while(childsNp!=0){
 						if ((*words)[childsNp->node->lexical_head].tag.code()==PENN_TAG_EX) {
 							noEx=false;
 						}
 						childsNp=childsNp->next;
 					}
 					if (noEx){ //then keep going
 						//NP $- ()
 						if (childsSQ->previous!=0){ //NP $- RB 
 							if ((*words)[childsSQ->previous->node->lexical_head].tag.code()==PENN_TAG_ADVERB) { //RB==ADVERB?
 								if (childsSQ->previous->previous!=0){//RB $- /^.../
 									if ((*words)[childsSQ->previous->previous->node->lexical_head].tag.code()==PENN_TAG_VERB) { //VB? YUE
 										CStateNodeList* rightSisters=childsSQ;
 										bool noVp=true;
 										while(rightSisters!=0){
 											if (rightSisters->node->constituent==PENN_CON_VP){
 												noVp=false;
 											}
 											rightSisters=rightSisters->next;
 										}
 										if (noVp) { //fire rule
 											CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_NSUBJ);
 											if (buildStanfordLink(label, npTarg->lexical_head, node.lexical_head)){
 												addDangling(&node,npTarg);
 												std::cout<<"nSubj7"<<" (head: "<<node.lexical_head<<")"<<"(dependent:"<<npTarg->lexical_head<<")\n";
 												return true;
 											}
 										}
 									}
 								}
 							}
 						}
					}
				}
				childsSQ=childsSQ->next;
			}
		}
		return false;
	}
   
   
       /* 	
   	   *"SBARQ < WHNP=target < (SQ < (VP ![$-- NP]))",
   	   *"SBARQ < WHNP=target < (SQ < (VP !$-- NP))", I did it, presuming that it is like this. According to John Bauer, it is like this.
   	   * 
   	   * Checked and seems fine.
   	   */
      bool buildNsubj8() {
    	  if (node.constituent==PENN_CON_SBARQ){
    		  CStateNodeList* childsSbarq=node.m_umbinarizedSubNodes;
    		  bool secondCondition=false;
    		  while(childsSbarq!=0){
    			  const CStateNode* sq=childsSbarq->node;
    			  if (sq->constituent==PENN_CON_SQ) {
    				  CStateNodeList* childsSq=sq->m_umbinarizedSubNodes;
    				  while(childsSq!=0){
    					  if (childsSq->node->constituent==PENN_CON_VP) {
    						  bool noSisterNp=true;
    						  CStateNodeList* leftSistersVp=childsSq;
    						  leftSistersVp=leftSistersVp->previous;
    						  while(leftSistersVp!=0){
    							  if (leftSistersVp->node->constituent==PENN_CON_NP){
    								  noSisterNp=false;
    							  }
    							  leftSistersVp=leftSistersVp->previous;
    						  }
    						  if (noSisterNp) secondCondition=true;
    					  }
    					  childsSq=childsSq->next;
    				  }
    			  }
    			  childsSbarq=childsSbarq->next;
    		  }
    		  if (secondCondition) {
    			  childsSbarq=node.m_umbinarizedSubNodes;
    			  while(childsSbarq!=0){
    				  const CStateNode* whnpTarg=childsSbarq->node;
    				  if (whnpTarg->constituent==PENN_CON_WHNP && !(isDangling(&node,whnpTarg))) {
    					  CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_NSUBJ);
    					   if (buildStanfordLink(label, whnpTarg->lexical_head, node.lexical_head)){
    						   addDangling(&node,whnpTarg);
    						   std::cout<<"nSubj8"<<" (head: "<<node.lexical_head<<")"<<"(dependent:"<<whnpTarg->lexical_head<<")\n";
    					   	   return true;
    					   }
    				  }
    				  childsSbarq=childsSbarq->next;
    			  }
    		  }
    	  }
    	  return false;
      }
      
       /*
        *"SBARQ < (SQ=target < /^(?:VB|AUX)/ !< VP)", 
        * 
        * Checked and seems fine.
   	    */
   	   bool buildNsubj9() {
   		   if (node.constituent==PENN_CON_SBARQ) {
   			   CStateNodeList* childsSbarq=node.m_umbinarizedSubNodes;
   			   while(childsSbarq!=0){
   				   const CStateNode* sqTarg=childsSbarq->node;
   				   if (sqTarg->constituent==PENN_CON_SQ && !(isDangling(&node,sqTarg))) {
   					   bool firstCondition=false;
   					   bool noVp=true;
   					   CStateNodeList* sonsOfSq=sqTarg->m_umbinarizedSubNodes;
   					   while(sonsOfSq!=0){
   						   if (sonsOfSq->node->constituent==PENN_CON_VP) {
   							   noVp=false;
   						   }
   						   if ((*words)[sonsOfSq->node->lexical_head].tag.code()==PENN_TAG_VERB) {
   						   // || sonsOfSq->node->constituent==PENN_CON_AUX) { WE DON¡T KNOW WHAT IS AUX yet
   							   firstCondition=true;
   							   
   						   	}
   						   sonsOfSq=sonsOfSq->next;
   					   }
   					   if (noVp && firstCondition) {
   						   CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_NSUBJ);
   						   if (buildStanfordLink(label, sqTarg->lexical_head, node.lexical_head)){
   							   addDangling(&node,sqTarg);
   							   std::cout<<"nSubj9"<<" (head: "<<node.lexical_head<<")"<<"(sqTarg->lexical_head<<")\n";
   							   return true;
   						   }
   						   
   					   }
   				   }
   				   childsSbarq=childsSbarq->next;
   			   }
   		   }
   		   return false;
   	   }
   	   
   	   
   	   //"SINV < (NP|WHNP=target [ $- VP|VBZ|VBD|VBP|VB|MD|AUX | $- (@RB|ADVP $- VP|VBZ|VBD|VBP|VB|MD|AUX) | !$- __ !$ @NP] )",
   	   
   	   //(NP|WHNP=target [ $- VP|VBZ|VBD|VBP|VB|MD|AUX | $- (@RB|ADVP $- VP|VBZ|VBD|VBP|VB|MD|AUX) | !$- __ !$ @NP] )
   	      //2.1 NP|WHNP $- VP... OR
   	      //2.2 NP|WHNP $- (@RB|ADVP $- VP|VBZ|VBD|VBP|VB|MD|AUX) OR
   	      //2.3 NP|WHNP !$- __ !$ @NP
   	   
   	   bool buildNsubj10() {
   		   if (node.constituent==PENN_CON_SINV) {
   			   CStateNodeList* childsSinv=node.m_umbinarizedSubNodes;
   			   while(childsSinv!=0){
   				   const CStateNode* npwhnpTarg=childsSinv->node;
   				   
   				   if ((npwhnpTarg->constituent==PENN_CON_NP || npwhnpTarg->constituent==PENN_CON_WHNP) && !isDangling(&node,npwhnpTarg)) {
   					   bool firstCondition=false;
   					   bool secondCondition=false;
   					   bool thirdCondition=false;
   					   
   					   if (childsSinv->previous!=0){
   						   const CStateNode* leftSisterNpWhnp=childsSinv->previous->node;
   						   if ((leftSisterNpWhnp->constituent==PENN_CON_VP) 
   							   || ((*words)[leftSisterNpWhnp->lexical_head].tag.code()==PENN_TAG_VERB_THIRD_SINGLE) 
   							   || ((*words)[leftSisterNpWhnp->lexical_head].tag.code()==PENN_TAG_VERB_PAST) 
   							   || ((*words)[leftSisterNpWhnp->lexical_head].tag.code()==PENN_TAG_VERB_PRES) 
   							   || ((*words)[leftSisterNpWhnp->lexical_head].tag.code()==PENN_TAG_VERB) 
   							   || ((*words)[leftSisterNpWhnp->lexical_head].tag.code()==PENN_TAG_MD)) {
   								
   								   firstCondition=true;
   							   }
   							
   							if ((leftSisterNpWhnp->constituent==PENN_CON_ADVP) 
   								|| ((*words)[leftSisterNpWhnp->lexical_head].tag.code()==PENN_TAG_ADVERB)
   							    || ((*words)[leftSisterNpWhnp->lexical_head].tag.code()==PENN_TAG_ADVERB_COMPARATIVE) 
   							    || ((*words)[leftSisterNpWhnp->lexical_head].tag.code()==PENN_TAG_ADVERB_SUPERLATIVE)) { 
   							    	
   							    	if (childsSinv->previous->previous!=0){
   							    		const CStateNode* leftSisterRbAdvp=childsSinv->previous->node;
   							    		if ((leftSisterRbAdvp->constituent==PENN_CON_VP)
   							    		    || ((*words)[leftSisterRbAdvp->lexical_head].tag.code()==PENN_TAG_VERB_THIRD_SINGLE)
   							    		    || ((*words)[leftSisterRbAdvp->lexical_head].tag.code()==PENN_TAG_VERB_PAST) 
   							    		    ||((*words)[leftSisterRbAdvp->lexical_head].tag.code()==PENN_TAG_VERB_PRES) 
   							    		   	|| ((*words)[leftSisterRbAdvp->lexical_head].tag.code()==PENN_TAG_VERB) 
   							    		    || ((*words)[leftSisterRbAdvp->lexical_head].tag.code()==PENN_TAG_MD)) {
   							    		   		 secondCondition=true;
   							    		   		 
   							    		   	 }
   							    	}
   							  }
   					   }
   					   //third condition
   					   //NP|WHNP !$- __ !$ @NP
   					   bool firstPart=true;
   					   bool secondPart=true;
   					   //first part
   					   if (childsSinv->previous!=0){
   							   firstPart=false;
   					   }
   					   //second part
   					   CStateNodeList* sisters=childsSinv;
   					   sisters=sisters->next;
   					   while(sisters!=0){
   						   const CStateNode* atnp=sisters->node;
   						   if (atnp->constituent==PENN_CON_NP){
   							   secondPart=false;
   						   }
   						   sisters=sisters->next;
   					   }
   					   sisters=childsSinv;
   					   sisters=sisters->previous;
   					   while(sisters!=0){
   						   const CStateNode* atnp=sisters->node;
   						   if (atnp->constituent==PENN_CON_NP){
   							   secondPart=false;
   						   }
   						   sisters=sisters->previous;
   					   }
   					   
   					   if (firstPart && secondPart) {
   						   thirdCondition=true;
   					   }
   					   
   					   if (firstCondition || secondCondition || thirdCondition) {
   						   CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_NSUBJ);
   						   if (buildStanfordLink(label, npwhnpTarg->lexical_head, node.lexical_head)){
   							   addDangling(&node,npwhnpTarg);
   							   std::cout<<"nSubj10"<<" (head: "<<node.lexical_head<<")"<<"(npwhnpTarg->lexical_head<<")\n";
   						   	   return true;
   						   }   
   					   }
   					   
   				   }
   				   childsSinv=childsSinv->next;
   			   }
   		   }
   		   return false;
   	   }
   
      
   	   //"SBAR < WHNP=target [ < (S < (VP !$-- NP) !< SBAR) | < (VP !$-- NP) !< S ]"
   	   //Let's try to put it into words
       //Relations can be grouped using brackets [ and ]. So the expression, NP [< NN | < NNS] & > S means that
       // (1) NP dominates either an NN or an NNS
       // (2) S dominates NP
   
       //So in our case of study.
       // 1. SBAR < WHNP=target
       // 2.1 SBAR < ((S < VP !$-- NP) !< SBAR)  OR
       // 2.2 SBAR < (VP !$-- NP) & !< S
      
       
   	   bool buildNsubj11() {
   		   if (node.constituent==PENN_CON_SBAR) {
   			   CStateNodeList* childsSbar=node.m_umbinarizedSubNodes;
   			   //std::cout<<"first while \n";
   			   const CStateNode* whnpTarg=0;
   			   bool thereIsWhnp=false;
   			   while(childsSbar!=0){
   				   const CStateNode* whnpTarg=childsSbar->node;
   				   if (whnpTarg->constituent==PENN_CON_WHNP && !(isDangling(&node, whnpTarg))) {
   					  thereIsWhnp=true;
   				   }
   				   if (thereIsWhnp) {
   					   CStateNodeList* childsSbar2=node.m_umbinarizedSubNodes;
   					   bool secondCondition=true;
   					   bool thereIsVp=false;
   					   //std::cout<<"second while \n";
   					   while(childsSbar2!=0) {
   						   const CStateNode* childSbar=childsSbar2->node;
   						   if (childSbar->constituent==PENN_CON_S){
   							   secondCondition=false;
   						   } //if childSbar
   						   else if (childSbar->constituent==PENN_CON_VP){
   							   thereIsVp=true;
   							   if (childsSbar2->previous!=0) {
   								   const CStateNode* immLeftSister=childsSbar2->previous->node;
   								   if (immLeftSister->constituent==PENN_CON_NP) {
   									   secondCondition=false;
   								   }
   							   } //if childsSbar2
   						   }//else if
   						   childsSbar2=childsSbar2->next;
   					   } //while childsSbar2
   					   if (thereIsVp && secondCondition) secondCondition=true;
   					   
   					   // 2.1 SBAR < ((S < VP !$-- NP) !< SBAR)  OR
   					   CStateNodeList* childsSbar3=node.m_umbinarizedSubNodes;
   					   bool thereIsVpAndS=false;
   					   bool firstCondition=true;
   					   //std::cout<<"third while \n";
   					   while(childsSbar3!=0) {
   						   const CStateNode* childSbar=childsSbar3->node;
   						   if (childSbar->constituent==PENN_CON_S){
   							   if (childsSbar3->previous!=0) {
   								   const CStateNode* immLeftSister=childsSbar3->previous->node;
   								   if (immLeftSister->constituent==PENN_CON_NP) {
   									   firstCondition=false;
   								   }//if inmLeftSister
   							   }//if (childsSbar3)
   							   CStateNodeList* childsS=childSbar->m_umbinarizedSubNodes;
   							   while(childsS!=0) {
   								   const CStateNode* childS=childsS->node; 
   								   if (childS->constituent==PENN_CON_VP) {
   									   thereIsVpAndS=true;
   								   }
   								   if (childS->constituent==PENN_CON_SBAR) {
   									   firstCondition=false;
   								   }
   								   childsS=childsS->next;
   							   }//while childsS
   						   }//if (penn_con_s)
   						   childsSbar3=childsSbar3->next;
   					   }//while childSbar3   
   					   if (firstCondition && thereIsVpAndS){
   						   firstCondition=true;
   					   }   
   					   if (secondCondition || firstCondition) {
   						   CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_NSUBJ);
   						   if (buildStanfordLink(label, whnpTarg->lexical_head, node.lexical_head)){
   							   addDangling(&node, whnpTarg);
   							   std::cout<<"nSubj11"<<" (head: "<<node.lexical_head<<")"<<"(whnpTarg->lexical_head<<")\n";
   							   return true;
   						   }
   					   }
   				   } //if whnp
   		   		   childsSbar=childsSbar->next;
   			   	} //while(childsSbar!=0)
   		   
   		   }//if node=sbar
   		   return false;	
   	   	}//method
       
   	
   
   		//"SBAR !< WHNP < (S !< (NP $++ VP)) > (VP > (S $- WHNP=target))",
   		//Simplified version
   		// SBAR !< WHNP & < (S !< (NP $++ VP)) & > (VP > (S $- WHNP=target))
        //which means that SBAR is not the immediate head of WHNP but it is the immediate head of (S !< (NP $++ VP)), and it also means that 
        //SBAR is the immediate child of the following structure (VP > (S $- WHNP=target)) which is headed by S.
        
   	   
   	   //ARE WE SURE ABOUT THIS ONE? I'm NOT...but I think is fine.
   	   
   	   //YUE:
   	   //little trick in this one. I have to start with the immediate head of the rightmost S (in the rule)... but I don't care about its constituent
   	   //S must have a head, because otherwise it cannot be the immediate right sister of WHNP
   	   
        bool buildNsubj12() {
        	CStateNodeList* childsNode=node.m_umbinarizedSubNodes;
        	while(childsNode!=0) {
        		const CStateNode* secondS=childsNode->node;
        		if (secondS->constituent==PENN_CON_S) {
        			CStateNodeList* leftSisterS=childsNode;
        			if (leftSisterS->previous!=0){
        				const CStateNode* whnpTarg=leftSisterS->node;
        				if (whnpTarg->constituent==PENN_CON_WHNP && !(isDangling(&node,whnpTarg))) {
        					CStateNodeList* childsS=secondS->m_umbinarizedSubNodes;
        					while(childsS!=0) {
        						const CStateNode* vp=childsS->node;
        						if (vp->constituent==PENN_CON_VP) {
        							CStateNodeList* childsVp=vp->m_umbinarizedSubNodes;
        							while(childsVp!=0){
        								const CStateNode* sbarHead=childsVp->node;
        								if (sbarHead->constituent==PENN_CON_SBAR){
        									CStateNodeList* childsSbar=sbarHead->m_umbinarizedSubNodes;
        									bool noWhnp=true;
        									while(childsSbar!=0){
        										if (childsSbar->node->constituent==PENN_CON_WHNP){
        											noWhnp=false;
        										}
        										//(S !< (NP $++ VP)
        										bool secondCondition=true;
        										const CStateNode* sSonOfSbar=childsSbar->node;
        										if (sSonOfSbar->constituent==PENN_CON_S){
        											CStateNodeList* childs2S=sSonOfSbar->m_umbinarizedSubNodes;
        											while(childs2S!=0){
        												const CStateNode* npSonofS=childs2S->node;
        												if (npSonofS->constituent==PENN_CON_NP) {
        													CStateNodeList* leftSistersNP=childs2S;
        													while(leftSistersNP!=0){
        														if (leftSistersNP->node->constituent==PENN_CON_VP) {
        															secondCondition=false;
        														}
        														leftSistersNP=leftSistersNP->next;
        													}
        												}
        												childs2S=childs2S->next;
        											}
        											if (noWhnp && secondCondition){
        												CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_NSUBJ);
        												if (buildStanfordLink(label, whnpTarg->lexical_head, sbarHead->lexical_head)){
        													addDangling(&node,whnpTarg);
        													std::cout<<"nSubj11"<<" (head: "<<sbarHead->lexical_head<<")"<<"(whnpTarg->lexical_head<<")\n";
        													return true;	
        												}
        											}
        										}
        										childsSbar=childsSbar->next;
        									}
        								}
        								childsVp=childsVp->next;
        							}
        						}
        						childsS=childsS->next;
        					}
        				}
        			}
        		}
        		childsNode=childsNode->next;
        	}
        	return false;
        }	
   
        //"SQ < ((NP < EX) $++ NP=target)",
      
      bool buildNsubj13() {
   	   	if (node.constituent==PENN_CON_SQ) {
   		   //const CStateNode* s=immediatelyDominates(node, PENN_CON_S);
   		   CStateNodeList* childs=node.m_umbinarizedSubNodes;
   		   while(childs!=0) {
   			   const CStateNode* np=childs->node;
   			   if (np->constituent==PENN_CON_NP) {
   				   CStateNodeList* sistersNp=childs;
   				   bool sisterIsNp=false;
   				   const CStateNode* npTarg;
   				   while(sistersNp!=0) {
   					   npTarg=sistersNp->node;
   					   if (npTarg->constituent==PENN_CON_NP && !(isDangling(&node,npTarg))) {
   						   sisterIsNp=true;
   					   }
   					   sistersNp=sistersNp->next;
   				   }
   				   CStateNodeList* childsNp=np->m_umbinarizedSubNodes;
   				   while(childsNp!=0){
   					   const CStateNode* ex=childsNp->node;
   					   //if ((*words)[noex->lexical_head].word==CWord("yue")) {}
   					   
   					   if ((*words)[ex->lexical_head].tag.code()==PENN_TAG_EX) {
   						   if (sisterIsNp) {
   							   CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_NSUBJ);
   							   if (buildStanfordLink(label, npTarg->lexical_head, node.lexical_head)){
   								   addDangling(&node,npTarg);
   								   std::cout<<"nSubj13"<<" (head: "<<node.lexical_head<<")"<<"(npTarg->lexical_head<<")\n";
   								   return true;
   							   }
   						   }
   					   }
   					   childsNp=childsNp->next;
   				   }
   			   }
   			   childs=childs->next;
   		   }
   	   }
   	   return false;
       }
      
     //===============================================================================
      
      //aux rules: there are 4
      
      //"VP < VP < /^(?:TO|MD|VB.*|AUXG?|POS)$/=target",
      //"SQ|SINV < (/^(?:VB|MD|AUX)/=target $++ /^(?:VP|ADJP)/)",
      //"CONJP < TO=target < VB", // (CONJP not to mention)
                // add handling of tricky VP fronting cases...
      //"SINV < (VP=target < (/^(?:VB|AUX|POS)/ < " + beAuxiliaryRegex + ") $-- (VP < VBG))",
      
      
      //"CONJP < TO=target < VB"
      
      //YUE: neither VB nor TO were in the penn.h. I added them. We are in 31 if I am not mistaken
      
      bool buildConj3() {
    	  /*if (node->constituent==PENN_CON_CONJP) {
    		  const CStateNode* to=immediatelyDominates(node, PENN_CON_TO); 
    		  if (to!=0) {
    			  const CStateNode* vb=immediatelyDominates(node, PENN_CON_VB);
    			  if (vb!=0){
    				  CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_AUX);
    				  buildStanfordLink(label, to->lexical_head, node->lexical_head);
    				  //std::cout<<"nSubj13"<<" (head: "<<node->lexical_head<<")"<<"(dependent: "<<np2->lexical_head<<")\n"; //miguel
    				  return true;
    			  }
    		  }
    	  }
    	  return false;*/
      }
            
      
   
    //===============================================================================
      
      //conj rules. There are 
      //(stf comment) this is more ugly, but the first 3 patterns are now duplicated and for clausal things, that daughter to the left of the CC/CONJP can't be a PP or RB or ADVP either
      //(stf comment) non-parenthetical or comma in suitable phrase with conjunction to left
      //"VP|S|SBAR|SBARQ|SINV|SQ < (CC|CONJP $-- !/^(?:``|-LRB-|PRN|PP|ADVP|RB)/ $+ !/^(?:PRN|``|''|-[LR]RB-|,|:|\\.)$/=target)",
      // non-parenthetical or comma in suitable phrase with conj then adverb to left
      //"VP|S|SBAR|SBARQ|SINV|SQ < (CC|CONJP $-- !/^(?:``|-LRB-|PRN|PP|ADVP|RB)/ $+ (ADVP $+ !/^(?:PRN|``|''|-[LR]RB-|,|:|\\.)$/=target))",
      // content phrase to the right of a comma or a parenthetical
      // The test at the end is to make sure that a conjunction or
      // comma etc actually show up between the target of the conj
      // dependency and the head of the phrase.  Otherwise, a
      // different relationship is probably more appropriate.
      //"VP|S|SBAR|SBARQ|SINV|SQ=root < (CC|CONJP $-- !/^(?:``|-LRB-|PRN|PP|ADVP|RB)/) < (/^(?:PRN|``|''|-[LR]RB-|,|:|\\.)$/ $+ (/^S|SINV$|^(?:A|N|V|PP|PRP|J|W|R)/=target $-- (/^CC|CONJP|:|,$/ $-- (__ ># =root))) )",
      // non-parenthetical or comma in suitable phrase with conjunction to left
      //"/^(?:ADJP|JJP|PP|QP|(?:WH)?NP(?:-TMP|-ADV)?|ADVP|UCP(?:-TMP|-ADV)?|NX|NML)$/ < (CC|CONJP $-- !/^(?:``|-LRB-|PRN)$/ $+ !/^(?:PRN|``|''|-[LR]RB-|,|:|\\.)$/=target)",
      // non-parenthetical or comma in suitable phrase with conj then adverb to left
      //"/^(?:ADJP|PP|(?:WH)?NP(?:-TMP|-ADV)?|ADVP|UCP(?:-TMP|-ADV)?|NX|NML)$/ < (CC|CONJP $-- !/^(?:``|-LRB-|PRN)$/ $+ (ADVP $+ !/^(?:PRN|``|''|-[LR]RB-|,|:|\\.)$/=target))",
      // content phrase to the right of a comma or a parenthetical
      //"/^(?:ADJP|PP|(?:WH)?NP(?:-TMP|-ADV)?|ADVP|UCP(?:-TMP|-ADV)?|NX|NML)$/ < (CC|CONJP $-- !/^(?:``|-LRB-|PRN)$/) < (/^(?:PRN|``|''|-[LR]RB-|,|:|\\.)$/ $+ /^S|SINV$|^(?:A|N|V|PP|PRP|J|W|R)/=target)",
                // content phrase to the left of a comma for at least NX
      //"NX|NML < (CC|CONJP $- __) < (/^,$/ $- /^(?:A|N|V|PP|PRP|J|W|R|S)/=target)",
                // to take the conjunct in a preconjunct structure "either X or Y"
      //"/^(?:VP|S|SBAR|SBARQ|SINV|ADJP|PP|QP|(?:WH)?NP(?:-TMP|-ADV)?|ADVP|UCP(?:-TMP|-ADV)?|NX|NML)$/ < (CC $++ (CC|CONJP $+ !/^(?:PRN|``|''|-[LR]RB-|,|:|\\.)$/=target))",
      
      
      
    //===============================================================================
      
      //cc coordination. THere is one rule.
      
      //"__ [ < (CC=target !< /^(?i:either|neither|both)$/ ) | < (CONJP=target !< (RB < /^(?i:not)$/ $+ (RB|JJ < /^(?i:only|just|merely)$/))) ]"
      
     //===============================================================================
      
      //punct punctuation. There are 2 rules.
      
      //"__ < /^(?:\\.|:|,|''|``|\\*|-LRB-|-RRB-|HYPH)$/=target",
      //"__ < (NFP=target !< /^(?:[<>]?[:;=8][\\-o\\*']?(?:-RRB-|-LRB-|[DPdpO\\/\\\\\\:}{@\\|\\[\\]])|(?:-RRB-|-LRB-|[DPdpO\\/\\\\\\:}{@\\|\\[\\]])[\\-o\\*']?[:;=8][<>]?)$/"
      //              + "!< /^(?:-LRB-)?[\\-\\^x=~<>'][_.]?[\\-\\^x=~<>'](?:-RRB-)?$/)"
      
      
      //===============================================================================
      
      //nsubjpass. THere is one rule.
      
      //"S|SQ < (WHNP|NP=target !< EX) < (VP < (/^(?:VB|AUX)/ < " + passiveAuxWordRegex + ")  < (VP < VBN|VBD))",
      //===============================================================================
      
      //csubj. Clausal subject. there is one rule.
      
      //"S < (SBAR|S=target !$+ /^,$/ $++ (VP !$-- NP))"
      
      //===============================================================================
      //csubjpass. There are 2 rules.
      
      //"S < (SBAR|S=target !$+ /^,$/ $++ (VP < (VP < VBN|VBD) < (/^(?:VB|AUXG?)/ < " + passiveAuxWordRegex + ") !$-- NP))",
      //"S < (SBAR|S=target !$+ /^,$/ $++ (VP <+(VP) (VP < VBN|VBD > (VP < (/^(?:VB|AUX)/ < " + passiveAuxWordRegex + "))) !$-- NP))"
      
      //===============================================================================
      //dobj, direct object. There are 8 rules.
      
      //"VP < (NP $+ (NP|WHNP=target !< (/^NN/ < " + timeWordLotRegex + "))) !<(/^(?:VB|AUX)/ < " + copularWordRegex + ")",  // this time one also included "lot"
      // match "give it next week"
      //"VP < (NP < (NP $+ (/^(NP|WHNP)$/=target !< (/^NN/ < " + timeWordLotRegex + "))))!< (/^(?:VB|AUX)/ < " + copularWordRegex + ")",  // this time one also included "lot"
      // basic direct object cases
      //"VP !<(/^(?:VB|AUX)/ < " + copularWordRegex + ") < (NP|WHNP=target [ [ !<# (/^NN/ < " + timeWordRegex + ") !$+ NP ] | $+ NP-TMP | $+ (NP <# (/^NN/ < " + timeWordRegex + ")) ] )",
      // cdm Jul 2010: No longer require WHNP as first child of SBARQ below: often not because of adverbials, quotes, etc., and removing restriction does no harm
      // this next pattern used to assume no empty NPs. Corrected.  If you adjust this pattern, also adjust the corresponding one for attr!
      //"SBARQ < (WHNP=target !< WRB !<# (/^NN/ < " + timeWordRegex + ")) <+(SQ|SINV|S|VP) (VP !< NP|TO !< (S < (VP < TO)) !< (/^(?:VB|AUX)/ < " + copularWordRegex + " $++ (VP < VBN|VBD)) !<- PRT !<- (PP <: IN) $-- (NP !< /^-NONE-$/))",
      // matches direct object in relative clauses "I saw the book that you bought"
      //"SBAR < (WHNP=target !< WRB) < (S < NP < (VP !< SBAR !<+(VP) (PP <- IN) !< (S < (VP < TO))))",
      // matches direct object in relative clauses "I saw the book that you said you bought"
      //"SBAR !< WHNP|WHADVP < (S < (@NP $++ (VP !$++ NP))) > (VP > (S < NP $- WHNP=target))",
      // matches direct object for long dependencies in relative clause without explicit relative pronouns
      //"SBAR !< (WHPP|WHNP|WHADVP) < (S < (@NP $+ (VP !< (/^(?:VB|AUX)/ < " + copularWordRegex + " !$+ VP)  !<+(VP) (/^(?:VB|AUX)/ < " + copularWordRegex + " $+ (VP < VBN|VBD)) !<+(VP) NP !< SBAR !<+(VP) (PP <- IN)))) !$-- CC $-- NP > NP=target",
      //"SBAR !< (WHPP|WHNP|WHADVP) < (S < (@NP $+ (ADVP $+ (VP !< (/^(?:VB|AUX)/ < " + copularWordRegex + " !$+ VP) !<+(VP) (/^(?:VB|AUX)/ < " + copularWordRegex + " $+ (VP < VBN|VBD)) !<+(VP) NP !< SBAR !<+(VP) (PP <- IN))))) !$-- CC $-- NP > NP=target"      
      //===============================================================================
      //iobj, indirect object. There are 2 rules.
      
      //"VP < (NP=target !< /\\$/ !<# (/^NN/ < " + timeWordRegex + ") $+ (NP !<# (/^NN/ < " + timeWordRegex + ")))",
      // this next one was meant to fix common mistakes of our parser, but is perhaps too dangerous to keep
      //"VP < (NP=target < (NP !< /\\$/ $++ (NP !< (/^NN/ < " + timeWordLotRegex + ")) !$ CC|CONJP !$ /^,$/ !$++ /^:$/))",
      
      //===============================================================================
      //pobj, prepositional object. There are 8 rules.
      
      //"/^(?:PP(?:-TMP)?|(?:WH)?(?:PP|ADVP))$/ < (IN|VBG|TO|FW|RB|RBR $++ (/^(?:WH)?(?:NP|ADJP)(?:-TMP|-ADV)?$/=target !$- @NP))",
      // We allow ADVP with NP objects for cases like (ADVP earlier this year)
      //"/^PP(?:-TMP)?$/ < (/^(?:IN|VBG|TO)$/ $+ (ADVP=target [ < (RB < /^(?i:here|there)$/) | < (ADVP < /^NP(?:-TMP)?$/) ] ))",
      // second disjunct is weird ADVP, only matches 1 tree in 2-21
      // to deal with preposition stranding in questions (e.g., "Which city do you live in?") -- the preposition is sometimes treated as a particle by the parser (works well but doesn't preserve the tree structure!)
      //"PRT >- (VP !< (S < (VP < TO)) >+(SQ|SINV|S|VP) (SBARQ <, (WHNP=target !< WRB)) $-- (NP !< /^-NONE-$/))",
      //"(PP <: IN|TO) >- (VP !< (S < (VP < TO)) >+(SQ|SINV|S|VP) (SBARQ <, (WHNP=target !< WRB)) $-- (NP !< /^-NONE-$/))",
      //"(PP <: IN|TO) $- (NP $-- (VBZ|VBD) !$++ VP) >+(SQ) (SBARQ <, (WHNP=target !< WRB)) $-- (NP !< /^-NONE-$/)",
      // to deal with preposition stranding in relative clause (works well but doesn't preserve the tree structure!)
      //"(PP <- IN|TO) >+(@VP|S|SINV|SBAR) (SBAR !< (WHPP|WHNP) < (S < (NP $+ (VP !<(/^(?:VB|AUX)/ < " + copularWordRegex + " !$+ VP) !<+(VP) NP !< SBAR ))) $-- NP > NP=target)",
      //"XS|ADVP < (IN < /^(?i:at)$/) < JJS|DT=target", // at least, at most, at best, at worst, at all
      //"PP < (CC < less) < NP"
      
      //===============================================================================
      //pcomp, prepositional complement. There are 3 rules.
      
      //"@PP|WHPP < (IN|VBG|VBN|TO $+ @SBAR|S|PP|ADVP=target)", // no intervening NP; VBN is for "compared with"
      //"@PP|WHPP < (RB $+ @SBAR|S=target)", // RB is for weird tagging like "after/RB adjusting for inflation"
      //"@PP|WHPP !< IN|TO < (SBAR=target <, (IN $+ S))",
      
      
      //===============================================================================
      //attr attributive. There are 4 rules.
      
      //"VP < NP=target <(/^(?:VB|AUX)/ < " + copularWordRegex + ") !$ (NP < EX)",
      // "What is that?"
      //"SBARQ < (WHNP|WHADJP=target $+ (SQ < (/^(?:VB|AUX)/ < " + copularWordRegex + " !$++ VP) !< (VP <- (PP <:IN)) !<- (PP <: IN)))",
      //"SBARQ <, (WHNP|WHADJP=target !< WRB) <+(SQ|SINV|S|VP) (VP !< (S < (VP < TO)) < (/^(?:VB|AUX)/ < " + copularWordRegex + " $++ (VP < VBN|VBD)) !<- PRT !<- (PP <: IN) $-- (NP !< /^-NONE-$/))",
      //"Is he the man?"
      //"SQ <, (/^(?:VB|AUX)/ < " + copularWordRegex + ") < (NP=target $-- (NP !< EX))"
      
      //===============================================================================
      //ccomp clausal complement
      
      //"VP < (S=target < (VP !<, TO|VBG|VBN) !$-- NP)",
      //"VP < (SBAR=target < (S <+(S) VP) <, (IN|DT < /^(?i:that|whether)$/))",
      //"VP < (SBAR=target < (SBAR < (S <+(S) VP) <, (IN|DT < /^(?i:that|whether)$/)) < CC|CONJP)",
      //"VP < (SBAR=target < (S < VP) !$-- NP !<, (IN|WHADVP))",
      //"VP < (SBAR=target < (S < VP) !$-- NP <, (WHADVP < (WRB < /^(?i:how)$/)))",
      //"VP < (/^VB/ < " + haveRegex + ") < (S=target < @NP < VP)",
      //// to find "...", he said or "...?" he asked.
      //"S|SINV < (S|SBARQ=target $+ /^(,|\\.|'')$/ !$- /^(?:CC|:)$/ !< (VP < TO|VBG|VBN))",
      //"ADVP < (SBAR=target < (IN < /^(?i:as|that)/) < (S < (VP !< TO)))", // ADVP is things like "As long as they spend ..."
      //"ADJP < (SBAR=target !< (IN < as) < S)", // ADJP is things like "sure (that) he'll lose" or for/to ones or object of comparison with than "than we were led to expect"; Leave aside as in "as clever as we thought.
      // That ... he know
      //"S <, (SBAR=target <, (IN < /^(?i:that|whether)$/) !$+ VP)",
      // JJ catches a couple of funny NPs with heads like "enough"
      // Note that we eliminate SBAR which also match an infmod pattern
      //"@NP < JJ|NN|NNS < (SBAR=target [ !<(S < (VP < TO )) | !$-- NP|NN|NNP|NNS ] )"
                
      //===============================================================================
      //xcomp xclausal complement
      
      //"VP < (S=target !$- (NN < order) < (VP < TO))",    // used to have !> (VP < (VB|AUX < be))
      //"ADJP < (S=target <, (VP <, TO))",
      //"VP < (S=target !$- (NN < order) < (NP $+ NP|ADJP))",
      // to find "help sustain ...
      //"VP < (/^(?:VB|AUX)/ $+ (VP=target < VB < NP))",
      //"VP < (SBAR=target < (S !$- (NN < order) < (VP < TO))) !> (VP < (VB|AUX < be)) ",
      //"VP < (S=target !$- (NN < order) <: NP) > VP",
      // stop eating
      // note that we eliminate parentheticals and clauses that could match a partmod
      //"(VP < (S=target < (VP < VBG ) !< NP !$- (/^,$/ [$- @NP  |$- (@PP $-- @NP ) |$- (@ADVP $-- @NP)]) !$-- /^:$/))",
      
      //===============================================================================
      //rel, relative
      
      //"SBAR <, WHNP|WHPP|WHADJP=target > /^NP/ [ !<, /^WHNP/ | < (S < (VP $-- (/^NP/ !< /^-NONE-$/)))]"
      
      //===============================================================================
      
      //expl, expletitive
      
      //"S|SQ|SINV < (NP=target <+(NP) EX)"      
      
      //===============================================================================
      
      //acomp adjectival complement
      
      //"VP [ < (ADJP=target !$-- NP) |  < (/^VB/ $+ (@S=target < (@ADJP < /^JJ/ ! $-- @NP|S))) ]",
      
      //===============================================================================
      
      //advcl, adverbial clause modifier
      
      //"VP < (@SBAR=target [ < (IN !< /^(?i:that|whether)$/) | <: (SINV <1 /^(?:VB|MD|AUX)/) | < (IN < that) < (RB|IN < so) ] )",
      //"S|SQ|SINV < (SBAR|SBAR-TMP=target <, (IN !< /^(?i:that|whether)$/ !$+ (NN < order)) !$-- /^(?!CC|CONJP|``|,|INTJ|PP(-.*)?).*$/ !$+ VP)",
      // to get "rather than"
      // "S|SQ|SINV < (SBAR|SBAR-TMP=target <2 (IN !< /^(?i:that|whether)$/ !$+ (NN < order)) !$-- /^(?!CC|CONJP|``|,|INTJ|PP(-.*)?$).*$/)",
      // "S|SQ|SINV < (SBAR|SBAR-TMP=target <, (IN !< /^(?i:that|whether)$/ !$+ (NN < order)) !$+ @VP $+ /^,$/ $++ @NP)", // this one might just be better, but at any rate license one with quotation marks or a conjunction beforehand
      // "SBARQ < (SBAR|SBAR-TMP|SBAR-ADV=target <, (IN !< /^(?i:that|whether)$/ !$+ (NN < order)) $+ /^,$/ $++ @SQ|S|SBARQ)", // the last part should probably only be @SQ, but this captures some strays at no cost
      // "VP < (SBAR|SBAR-TMP=target <, (WHADVP|WHNP < (WRB !< /^(?i:how)$/)) !< (S < (VP < TO)))", // added the (S < (VP <TO)) part so that "I tell them how to do so" doesn't get a wrong advcl
      // "S|SQ < (SBAR|SBAR-TMP=target <, (WHADVP|WHNP < (WRB !< /^(?i:how)$/)) !< (S < (VP < TO)))",
      
      //"@S < (@SBAR=target $++ @NP $++ @VP)",  // fronted adverbial clause
      //"@S < (@S=target < (VP < TO) $+ (/^,$/ $++ @NP))"
      
      
      
      
      //===============================================================================
      //"rcmod", "relative clause modifier",
      
      //"NP|WHNP|NML $++ (SBAR=target <+(SBAR) WHPP|WHNP) !$-- NP|WHNP|NML > @NP|WHNP",
      //"NP|WHNP|NML $++ (SBAR=target <: (S !<, (VP <, TO))) !$-- NP|WHNP|NLP > @NP|WHNP",
      // this next pattern is restricted to where and why because
      //"when" is usually incorrectly parsed: temporal clauses
      // are put inside the NP; 2nd is for case of relative
      // clauses with no relativizer (it doesn't distinguish
      // whether actually gapped).
      //"NP|NML $++ (SBAR=target < (WHADVP < (WRB </^(?i:where|why|when)/))) !$-- NP|NML > @NP",
      //"NP|WHNP|NML $++ RRC=target !$-- NP|WHNP|NML",
      //"@ADVP < (@ADVP < (RB < /where$/)) < @SBAR=target",
      
      //===============================================================================
      //mark marker.
      
      //"SBAR|SBAR-TMP < (IN|DT=target $++ S|FRAG)",
      //"SBAR < (IN|DT=target < that|whether) [ $-- /^(?:VB|AUX)/ | $- NP|NN|NNS | > ADJP|PP | > (@NP|UCP|SBAR < CC|CONJP $-- /^(?:VB|AUX)/) ]",
      
      //===============================================================================
      
      //amod, adjectival modifier
      
      // "/^(?:NP(?:-TMP|-ADV)?|NX|NML|NAC|WHNP)$/ < (ADJP|WHADJP|JJ|JJR|JJS|JJP|VBN|VBG|VBD|IN=target !< QP !$- CC)",
      // IN above is needed for "next" in "next week" etc., which is often tagged IN.
      //"ADJP !< CC|CONJP < (JJ|NNP $ JJ|NNP=target)",
      // Cover the case of "John, 34, works at Stanford" - similar to an expression for appos
      //"WHNP|WHNP-TMP|WHNP-ADV|NP|NP-TMP|NP-ADV < (NP=target <: CD $- /^,$/ $-- /^(?:WH)?NP/ !$ CC|CONJP)",
      //===============================================================================
      
      
      
      //===============================================================================
      
      
      
   bool buildStanfordLink(CDependencyLabel* label, int dependent, int head) {
	   if (head==dependent) return false;
	   
   	   CLink* newNode=new CLink(*label, dependent, head, 0);
   	   newNode->next=this->node.stfLinks;
   	   node.stfLinks=newNode; //the new node (with the arc and label is added to the list)
   	   return true;
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





