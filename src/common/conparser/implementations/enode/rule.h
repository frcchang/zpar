// Copyright (C) University of Oxford 2010
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
   CHashMap< CTuple2<CConstituent, CConstituent>, std::vector<CAction> > *m_mapBinaryRules;
   CHashMap< CConstituent, std::vector<CAction> > *m_mapUnaryRules;
   const std::vector< std::vector<CConstituent> > *m_LexConstituents;
   const std::vector< CTaggedWord<CTag, TAG_SEPARATOR> > *m_sent;

public:
   CRule(const std::vector< CTaggedWord<CTag, TAG_SEPARATOR> >*sent) : m_mapBinaryRules(0), m_mapUnaryRules(0), m_LexConstituents(0), m_sent(sent) {}
   virtual ~CRule() {
      if (m_mapBinaryRules)
         delete m_mapBinaryRules;
      if (m_mapUnaryRules)
         delete m_mapUnaryRules;
   }

public:
   void getActions(const CStateItem &item, std::vector<CAction> &actions) {
      actions.clear();

      static CAction action;
      const unsigned stack_size = item.stacksize();
      const unsigned &length = m_sent->size();

      // finish
      if (item.IsTerminated()) {
         action.encodeIdle();
         actions.push_back(action);
         return;
      }
      // finish parsing
      if (item.IsComplete(length)) {
         action.encodeReduceRoot();
         actions.push_back(action);
      }
      // shift
      if ( item.current_word < length ) {
         // do not shift for head right tmp item
#ifndef NO_TEMP_CONSTITUENT
         if (stack_size>0 && item.node.temp && item.node.head_left()==false) {
         }
         else {
#endif // NO_TEMP_CONSTITUENT
             getShiftRules(item, actions);
#ifndef NO_TEMP_CONSTITUENT
         }
#endif // NO_TEMP_CONSTITUENT
      }
      if(item.current_word < length)
      if(item.empty_shifts() < EMPTY_SHIFT_MOVES)
      {
          getShiftEmptyRules(item, actions);
      }
      // reduce bin
      if ( stack_size > 1 ) {
         getBinaryRules(item, actions);
      }
      // reduce unary
      if ( stack_size && item.unaryreduces()<UNARY_MOVES
#ifndef NO_TEMP_CONSTITUENT
           && !item.node.temp
#endif
      ) {
         getUnaryRules(item, actions);
      }
   }

protected:
   void getShiftRules(const CStateItem &item, std::vector<CAction> &actions) {
      static CAction action;
      // the rules onto lexical item constituents
      if (m_LexConstituents) {
         ASSERT(m_LexConstituents->at(item.current_word).size()>0, "no lexical constituents for word "<<item.current_word<<" ("<<m_sent->at(item.current_word)<<") is provided.");
         for (int i=0; i<m_LexConstituents->at(item.current_word).size(); ++i) {
//            TRACE("shift "<<m_LexConstituents->at(item.current_word).at(i).str());
            action.encodeShift(m_LexConstituents->at(item.current_word).at(i).code());
            actions.push_back(action);
         }
         return;
      }
      // the default rules
      action.encodeShift();
      actions.push_back(action);
   }
   void getBinaryRules(const CStateItem &item,  std::vector<CAction> &actions) {
      static CAction action;
      const unsigned stack_size = item.stacksize();
      ASSERT(stack_size>0, "Binary reduce required for stack containing one node");
      const CStateNode &right = item.node;
      const CStateNode  &left = item.stackPtr->node;
      // specified rules
      if (m_mapBinaryRules) {
         static CTuple2<CConstituent, CConstituent> tuple2;
         tuple2.refer(&(left.constituent), &(right.constituent));
         const std::vector<CAction> &result = m_mapBinaryRules->find(tuple2, std::vector<CAction>());
         actions.insert(actions.end(), result.begin(), result.end());
         return;
      }
      // the normal method
#ifdef NO_TEMP_CONSTITUENT
      const bool temporary = false;
#else
      const bool prev_temp = stack_size>2 ? item.stackPtr->stackPtr->node.temp:false;
#endif
      for (unsigned long constituent=CConstituent::FIRST; constituent<CConstituent::COUNT; ++constituent) {
         for (unsigned i=0; i<=1; ++i) {
	    const bool &head_left = static_cast<bool>(i);
            const CWord &head_wd = ((head_left?left:right).lexical_head)->word;
#ifndef NO_TEMP_CONSTITUENT
            for (unsigned j=0; j<=1; ++j) {
               const bool &temporary = static_cast<bool>(j);
               if ( ( !left.temp || !right.temp ) &&
                     ( !(stack_size==2 && item.current_word==m_sent->size()) || !temporary ) &&
                     ( !(stack_size==2) || (!temporary||head_left) ) &&
                     ( !(prev_temp && item.current_word==m_sent->size()) || !temporary ) &&
                     ( !(prev_temp) || (!temporary||head_left) ) &&
                     ( !left.temp || (head_left&&constituent==left.constituent.extractConstituentCode()) ) &&
                     ( !right.temp || (!head_left&&constituent==right.constituent.extractConstituentCode()) ) //&&
//                     ( !temporary || CConstituent::canBeTemporary(constituent) )
                 ) {
#endif
                        action.encodeReduce(constituent, false, head_left, temporary);
                        actions.push_back(action);
#ifndef NO_TEMP_CONSTITUENT
                  }
               } // for j
#endif
            } // for i
         } // for constituent
   }

   void getUnaryRules(const CStateItem &item,  std::vector<CAction> &actions) {
      const CStateNode &child = item.node;
      // input rule list
      m_mapUnaryRules = false;
      if (m_mapUnaryRules) {
         const std::vector<CAction> &result = m_mapUnaryRules->find(child.constituent, std::vector<CAction>());
         for (int i=0; i<result.size(); ++i) {
            if (result.at(i).getConstituent() != child.constituent.code()) {
               actions.push_back( result.at(i) );
            }
         }
         return;
      }
      // the normal rules
      static CAction action;
      const unsigned stack_size = item.stacksize();
      for (unsigned long constituent=CConstituent::FIRST; constituent<CConstituent::COUNT; ++constituent){
         if (constituent != child.constituent.code()) {
            action.encodeReduce(constituent, true, false, false);
            actions.push_back(action);
         }
      } // for constituent
   }

   void getShiftEmptyRules(const CStateItem &item, std::vector<CAction> &actions)
   {
       static CAction action;
       for(int i=0; i<PENN_EMPTY_TAG_COUNT; ++i)
       {
           action.encodeShiftEmpty(PENN_EMPTY_TAG_FIRST + i);
           actions.push_back(action);
       }
   }

public:
   void loadRules(std::ifstream &is) {
      // initialization
      if (!is.is_open()) {
         return;
      }
      static std::string s;
      getline(is, s);
      // binary rules
      ASSERT(s=="Binary rules" or s=="Free binary rules", "Binary rules not found from model.");
      if (s=="Free binary rules")
         return;
      ASSERT(!m_mapBinaryRules, "Binary rules already loaded");
      m_mapBinaryRules = new CHashMap< CTuple2<CConstituent, CConstituent>, std::vector<CAction> >(257);
      is >> (*m_mapBinaryRules);
      // unary rules
      getline(is, s);
      ASSERT(s=="Unary rules" or s=="Free unary rules", "Unary rules not found from model.");
      if (s=="Free unary rules")
         return;
      ASSERT(!m_mapUnaryRules, "Unary rules already loaded");
      m_mapUnaryRules = new CHashMap< CConstituent, std::vector<CAction> >(257);
      is >> (*m_mapUnaryRules);
   }

   void saveRules(std::ofstream &os) {
      ASSERT(os.is_open(), "Cannot save rules possibly because the output file is not accessible.");
      if (m_mapBinaryRules) {
         os << "Binary rules" << std::endl;
         os << (*m_mapBinaryRules);
      }
      else {
         os << "Free binary rules" << std::endl;
       }
      if (m_mapUnaryRules) {
         os << "Unary rules" << std::endl;
         os << (*m_mapUnaryRules);
      }
      else { os << "Free unary rules" << std::endl; }
      os << std::endl;
   }

   void LoadBinaryRules(std::ifstream &is) {
      if (!is.is_open()) {
         THROW("Loading binary rules failed possibly becaues file not exists.");
      }
      m_mapBinaryRules = new CHashMap< CTuple2<CConstituent, CConstituent>, std::vector<CAction> >(257);
      is >> (*m_mapBinaryRules);
   }

   void LoadUnaryRules(std::ifstream &is) {
      if (!is.is_open()) {
         THROW("Loading unary rules failed possibly becaues file not exists.");
      }
      m_mapUnaryRules = new CHashMap< CConstituent, std::vector<CAction> >(257);
      is >> (*m_mapUnaryRules);
   }

   void SetLexConstituents(const std::vector<std::vector<CConstituent> > &con_input) {
      m_LexConstituents = &con_input;
   }
   void UnsetLexConstituents() {
      m_LexConstituents=0;
   }

};

} // namespace conparser

}

#endif
