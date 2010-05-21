#ifndef _COMMON_CON_PARSER_RULE
#define _COMMON_CON_PARSER_RULE

/*===============================================================
 *
 * Rules
 *
 *==============================================================*/

#include "tuple2.h"

namespace TARGET_LANGUAGE {

namespace conparser {

class CRule {

protected:
   CHashMap< CTuple3<CAction, CConstituent, CConstituent>, unsigned > *m_mapBinaryRules;
   CHashMap< CTuple2<CAction, CConstituent>, unsigned > *m_mapUnaryRules;

public:
   CRule() : m_mapBinaryRules(0), m_mapUnaryRules(0) {}
   virtual ~CRule() {
      if (m_mapBinaryRules) 
         delete m_mapBinaryRules; 
      if (m_mapUnaryRules) 
         delete m_mapUnaryRules;
   }

public:
   void getActions(const CStateItem &item, vector<CAction> &actions) {
      assert(!item.IsTerminated());
      actions.clear();

      static CAction action;
      const unsigned &stack_size = item.stack.size();
      const unsigned &length = item.sent->size();

      // finish parsing
      if (item.IsComplete()) { 
         action.encodeReduceRoot();
         actions.push_back(action);
      }
      // shift
      if ( item.current_word < length ) { 
         // do not shift for head right tmp item
         if (stack_size>0 && 
             item.nodes[item.stack.back()].temp &&
             item.nodes[item.stack.back()].head_left()==false) {
         }
         else {
            action.encodeShift();
            actions.push_back(action);
         }
      }
      // reduce bin
      if ( stack_size > 1 ) {
         const bool prev_temp = stack_size>2 ? item.nodes[item.stack[stack_size-3]].temp:false;
	 for (unsigned long constituent=CConstituent::FIRST; constituent<CConstituent::COUNT; ++constituent) {
	    for (unsigned i=0; i<=1; ++i) {
	       for (unsigned j=0; j<=1; ++j) {
		  const bool &head_left = static_cast<bool>(i);
		  const bool &temporary = static_cast<bool>(j);
		  const CStateNode &right = item.nodes[item.stack.back()];
		  const CStateNode  &left = item.nodes[item.stack[stack_size-2]];
		  assert( item.stack.back() == item.context->s0 );
		  assert( item.stack[stack_size-2] == item.context ->s1 );
		  const CWord &head_wd = item.sent->at( (head_left?left:right).lexical_head );
		  if ( ( !left.temp || !right.temp ) &&
		       ( !(stack_size==2 && item.current_word==item.sent->size()) || !temporary ) &&
		       ( !(stack_size==2) || (!temporary||head_left) ) &&
		       ( !(prev_temp && item.current_word==item.sent->size()) || !temporary ) &&
		       ( !(prev_temp) || (!temporary||head_left) ) &&
		       ( !left.temp || (head_left&&constituent==left.constituent.code()) ) &&
		       ( !right.temp || (!head_left&&constituent==right.constituent.code()) ) //&&
      //                 ( !temporary || CConstituent::canBeTemporary(constituent) ) 
		     ) {
		     action.encodeReduce(constituent, false, head_left, temporary);
                     actions.push_back(action);
		  }
               } // for j
            } // for i
         } // for constituent
      }
      // reduce unary
      if ( stack_size && item.unary_reduce<UNARY_MOVES &&
           !item.nodes[item.stack.back()].temp ) {
         for (unsigned long constituent=CConstituent::FIRST; constituent<CConstituent::COUNT; ++constituent){
            const CStateNode &child = item.nodes[item.stack.back()];
            assert(item.context->s0==item.stack.back());
            if (constituent != child.constituent.code()) { 
               action.encodeReduce(constituent, true, false, false);
               actions.push_back(action);
            }
         } // for constituent
      }
   }

public:
   bool binaryRuleAllow() {
      if (m_mapBinaryRules==0) return true;
      // TODO
   }

public:
   void loadRules(ifstream &is) {
      if (!is.is_open()) {
         return;
      }
      // TODO
   }

   void saveRules(ofstream &os) {
      ASSERT(os.is_open(), "Cannot save rules possibly because the output file is not accessible.");
      // TODO
   }

   void LoadBinaryRules(ifstream &is) {
      if (!is.is_open()) {
         THROW("Loading binary rules failed possibly becaues file not exists.");
      }
      m_mapBinaryRules = new CHashMap< CTuple3<CAction, CConstituent, CConstituent>, unsigned >;
      is >> (*m_mapBinaryRules);
   }

   void LoadUnaryRules(ifstream &is) {
      if (!is.is_open()) {
         THROW("Loading unary rules failed possibly becaues file not exists.");
      }
      m_mapUnaryRules = new CHashMap< CTuple2<CAction, CConstituent>, unsigned >;
      is >> (*m_mapUnaryRules);
   }

};

} // namespace conparser

}

#endif
