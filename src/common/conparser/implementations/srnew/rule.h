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
                     if (binaryRuleAllow(action, left.constituent, right.constituent)) {
                        actions.push_back(action);
                     } // if rules allow
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
               if (unaryRuleAllow(action, child.constituent)) {
                  actions.push_back(action);
               }
            }
         } // for constituent
      }
   }

public:
   bool binaryRuleAllow(const CAction &action, const CConstituent &c1, const CConstituent &c2) {
      if (m_mapBinaryRules==0) return true;
      static CTuple3<CAction, CConstituent, CConstituent> tuple3;
      tuple3.refer(&action, &c1, &c2);
      return m_mapBinaryRules->find(tuple3, 0);
   }
   bool unaryRuleAllow(const CAction &action, const CConstituent &c) {
      if (m_mapUnaryRules==0) return true;
      static CTuple2<CAction, CConstituent> tuple2;
      tuple2.refer(&action, &c);
      return m_mapUnaryRules->find(tuple2, 0);
   }

public:
   void loadRules(ifstream &is) {
      // initialization
      if (!is.is_open()) {
         return;
      }
      static string s;
      getline(is, s);
      // binary rules
      ASSERT(s=="Binary rules" or s=="Free binary rules", "Binary rules not found from model.");
      if (s=="Free binary rules")
         return;
      ASSERT(!m_mapBinaryRules, "Binary rules already loaded");
      m_mapBinaryRules = new CHashMap< CTuple3<CAction, CConstituent, CConstituent>, unsigned >;
      is >> (*m_mapBinaryRules);
      // unary rules
      getline(is, s);
      ASSERT(s=="Unary rules" or s=="Free unary rules", "Unary rules not found from model.");
      if (s=="Free unary rules")
         return;
      ASSERT(!m_mapUnaryRules, "Unary rules already loaded");
      m_mapUnaryRules = new CHashMap< CTuple2<CAction, CConstituent>, unsigned >;
      is >> (*m_mapUnaryRules);
   }

   void saveRules(ofstream &os) {
      ASSERT(os.is_open(), "Cannot save rules possibly because the output file is not accessible.");
      if (m_mapBinaryRules) {
         os << "Binary rules" << endl;
         os << (*m_mapBinaryRules);
      }
      else {
         os << "Free binary rules" << endl;
       }
      if (m_mapUnaryRules) {
         os << "Unary rules" << endl;
         os << (*m_mapUnaryRules);
      }
      else { os << "Free unary rules" << endl; }
      os << endl;
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
