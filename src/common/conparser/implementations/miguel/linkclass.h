
#ifndef _COMMON_CON_PARSER_LINK
#define _COMMON_CON_PARSER_LINK


/*===============================================================
 *
 * CLINK - stanford dependencies in ZPar (links) 
 * Authors: Miguel Ballesteros and Yue Zhang
 * UPF-Spain and SUTD-Singapore
 *
 *==============================================================*/

class CLink {
public:
   const CDependencyLabel* label; //src/english/dependency/label/stanford.h
   int dependent;
   //int head; so far, so good.
   const CLink* next;   

public:
   CLink(const CDependencyLabel *label, const int dependent, const CLink* next): label(label), dependent(dependent), next(next) {}   
   CLink(): label(0), dependent(-1), next(0) {}	
   virtual ~CLink() {}

public:
   bool valid() const { return dependent!=-1 && label!=0; }
   void clear() { 
      this->label = 0;
      this->dependent=-1;
      this->next=0;
   }
   void set(const CDependencyLabel *label, const int dependent, const CLink* next) { 
      this->label = label;
      this->dependent = dependent; 
      this->next = next; 
   }
   bool operator == (const CLink &nd) const {
      return dependent == nd.dependent &&
             label == nd.label && 
             next == nd.next;
   }
   void operator = (const CLink &nd) {
	dependent = nd.dependent;
        label = nd.label; 
        next = nd.next;   
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
