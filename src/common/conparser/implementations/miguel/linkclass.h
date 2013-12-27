
#ifndef _COMMON_CON_PARSER_LINK
#define _COMMON_CON_PARSER_LINK

//class CStateNode;

/*===============================================================
 *
 * CLINK - stanford dependencies in ZPar (links) 
 * Authors: Miguel Ballesteros and Yue Zhang
 * UPF-Spain and SUTD-Singapore
 *
 *==============================================================*/

class CLink {
public:
   unsigned long label; //src/english/dependency/label/stanford.h
   int dependent;
   int head; 
   CLink *next;   
   
   int nsubjRule;

public:
   CLink(const unsigned long label, const int dependent, const int head, CLink* next): label(label), dependent(dependent), head(head), next(next) {
	   this->nsubjRule=0;
   }   
   CLink(): label(0), dependent(-1), head(-1), next(0) {
	   this->nsubjRule=0;
   }	
   
   virtual ~CLink() {}

public:
   bool valid() const { return dependent!=-1 && head!=-1 && label!=0; }
   void clear() { 
      this->label = CDependencyLabel::NONE;
      this->dependent=-1;
      this->next=0;
      this->head=-1;
   }
   void set(const unsigned long label, const int dependent, CLink* next, const int head) { 
      this->label = label;
      this->dependent = dependent; 
      this->next = next;
      this->head=head;
   }
   
   
   bool operator == (const CLink &nd) const {
      return dependent == nd.dependent &&
             label == nd.label && 
             head == nd.head &&
             next == nd.next;
   }
   void operator = (const CLink &nd) {
	   dependent = nd.dependent;
       label = nd.label; 
       next = nd.next;   
       head = nd.head;
   }
    

/*public:
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
};*/



};

#endif
