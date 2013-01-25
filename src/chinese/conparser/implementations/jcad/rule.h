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
   const unsigned long *m_maxlengthbytag;
   const unsigned *m_maxwordfreq;
   const CStringVector *m_sent;
   const CTagDict<CWord, CTag> *m_tagdict;
   const CHashMap<CWord, unsigned long> *m_wordfreq;
   const CTagDict<CWord, CTag> *m_canstartword;
   CWordCache m_WordCache;

public:
   CRule(const unsigned long *maxlengthbytag, const unsigned  *maxwordfreq, const CTagDict<CWord, CTag> *tagdict, const CHashMap<CWord, unsigned long> *wordfreq, const CTagDict<CWord, CTag> *canstartword, CWordCache WordCache) :
   	m_maxlengthbytag(maxlengthbytag), m_maxwordfreq(maxwordfreq), m_tagdict(tagdict), m_wordfreq(wordfreq), m_canstartword(canstartword), m_WordCache(WordCache) {}
   virtual ~CRule() {
   }

public:
   void getActions(const CStateItem &item, const CStringVector *sent, std::vector<CAction> &actions) {
      actions.clear();

      static CAction action;
      const unsigned stack_size = item.stacksize();
      m_sent = sent;
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
      // reduce bin
      if ( stack_size > 1 ) {
         getBinaryRules(item, actions);
         getWordXYZRules(item, actions);
      }
      if (stack_size)
      {
      	getWordTRules(item, actions);
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

   inline bool canAssignTag(const CWord &word, const unsigned long &tag) {
      return ( m_wordfreq->find( word, 0 ) <
                  *m_maxwordfreq/5000+5 &&
               PENN_TAG_CLOSED[ tag ] == false  ) ||
             m_tagdict->lookup( word, tag );
   }

   inline bool canStartWord(const unsigned long &tag, const unsigned long &index) {
      if (PENN_TAG_CLOSED[ tag ] || tag == PENN_TAG_CD ) {
         static int tmp_i;
         // if the first character doesn't match, don't search
         if ( m_canstartword->lookup( m_WordCache.find( index, index, m_sent ), tag ) == false)
            return false;
         // if it matches, search from the next characters
         if ( tag == PENN_TAG_CD ) return true; // don't search for CD assume correct
         for (tmp_i=0; tmp_i<m_maxlengthbytag[tag]; ++tmp_i) {
            if ( m_tagdict->lookup( m_WordCache.find( index, std::min(index+tmp_i, static_cast<unsigned long>(m_sent->size())-1), m_sent ), tag ) )
               return true;
         }
         return false;
      }
      return true;
   }


   void getShiftRules(const CStateItem &item, std::vector<CAction> &actions) {
      static CAction action;

      if(item.stacksize() > 0 && item.node.is_partial())
      {
      	unsigned long last_tag = item.node.pos;
      	if(last_tag == PENN_TAG_CD || (item.node.end_c - item.node.begin_c +1 < m_maxlengthbytag[last_tag]))
      	{
      		action.encodeShiftA();
      		actions.push_back(action);
      	}
      }
      else if(item.stacksize() == 0 || !item.node.is_partial())
      {
      	for(unsigned long tag = CTag::FIRST; tag < CTag::COUNT; ++tag)
      	{
      		if(canStartWord(tag, item.current_word))
      		{
      			action.encodeShiftS(tag);
      			actions.push_back(action);
      		}
      	}
      }

   }

   void getWordXYZRules(const CStateItem &item, std::vector<CAction> &actions)
   {
   	static CAction action;
   	if(item.stacksize() > 1 && item.node.is_partial() && item.stackPtr->node.is_partial())
   	{
   		action.encodeWORDXYZ('x');
   		actions.push_back(action);
   		action.encodeWORDXYZ('y');
   		actions.push_back(action);
   		action.encodeWORDXYZ('z');
   		actions.push_back(action);
   	}
   }

   void getWordTRules(const CStateItem &item, std::vector<CAction> &actions)
	{
   	static CAction action;

   	if(item.stacksize() > 0 && item.node.is_partial()
   			&& (!item.stackPtr || !item.stackPtr->node.is_partial()))
   	{
   		if(canAssignTag(m_WordCache.find(item.node.begin_c, item.node.end_c, m_sent), item.node.pos))
   		{
   			action.encodeWORDT();
   			actions.push_back(action);
   		}
   	}

	}

   void getBinaryRules(const CStateItem &item,  std::vector<CAction> &actions) {
      static CAction action;
      const unsigned stack_size = item.stacksize();
      ASSERT(stack_size>0, "Binary reduce required for stack containing one node");
      const CStateNode &right = item.node;
      const CStateNode  &left = item.stackPtr->node;
      // the normal method
#ifdef NO_TEMP_CONSTITUENT
      const bool temporary = false;
#else
      const bool prev_temp = stack_size>2 ? item.stackPtr->stackPtr->node.temp:false;
#endif
      for (unsigned long constituent=CConstituent::FIRST; constituent<CConstituent::COUNT; ++constituent) {
         for (unsigned i=0; i<=1; ++i) {
	    const bool &head_left = static_cast<bool>(i);
            //const CWord &head_wd = m_sent->at((head_left?left:right).lexical_head).word;
            const CWord &head_wd = m_WordCache.find( (head_left?left:right).word_head->begin_c, (head_left?left:right).word_head->end_c, m_sent );
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

public:
   void loadRules(std::ifstream &is) {
   }

   void saveRules(std::ofstream &os) {
   }

};

} // namespace conparser

}

#endif
