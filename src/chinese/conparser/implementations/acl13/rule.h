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

class CRuleBase {
protected:
   unsigned char m_chars[MAX_SENTENCE_SIZE];

public:
   enum PRUNE { eNoAction = 0, eAppend, eSeparate };
   CRuleBase() { reset(); }
   virtual ~CRuleBase() { }

public:
   void setSeparate(unsigned long index, bool separate) {
      m_chars[index] = ( separate ? eSeparate : eAppend ) ;
   }
   bool canSeparate(unsigned long index) {
      return m_chars[index] == eNoAction || m_chars[index] == eSeparate;
   }
   bool canAppend(unsigned long index) {
      return m_chars[index] == eNoAction || m_chars[index] == eAppend;
   }
   bool mustSeparate(unsigned long index) {
      return m_chars[index] == eSeparate;
   }
   bool mustAppend(unsigned long index) {
      return m_chars[index] == eAppend;
   }

   void reset() { memset(m_chars, 0, MAX_SENTENCE_SIZE*sizeof(char)); }//for (unsigned i=0; i<MAX_SENTENCE_SIZE; ++i) m_chars[i] = 0; }
};


class CRule  : public CRuleBase {

protected:
   const unsigned long *m_maxlengthbytag;
   const unsigned long *m_maxwordfreq;
   const CStringVector *m_sent;
   const CTagDict<CWord, CTag> *m_tagdict;
   const CHashMap<CWord, unsigned long> *m_wordfreq;
   const CHashMap<CWord, unsigned long> *m_partwordfreq;
   const CWordDictionary *m_mapwordheaddict;
   const CTagDict<CWord, CTag> *m_canstartword;
   CWordCache *m_WordCache;
   const CCharCatDictionary *m_char_categories; // use rules to segment foreign words?

public:
   CRule(const unsigned long *maxlengthbytag, const unsigned  long *maxwordfreq, const CTagDict<CWord, CTag> *tagdict, const CHashMap<CWord, unsigned long> *wordfreq, const CTagDict<CWord, CTag> *canstartword,
   		CWordCache *WordCache, const CCharCatDictionary* char_categories, const CHashMap<CWord, unsigned long> *partwordfreq, const CWordDictionary *wordheaddict) :
   	m_maxlengthbytag(maxlengthbytag), m_maxwordfreq(maxwordfreq), m_tagdict(tagdict), m_wordfreq(wordfreq), m_canstartword(canstartword)
      , m_WordCache(WordCache), m_char_categories(char_categories), m_partwordfreq(partwordfreq),  m_mapwordheaddict(wordheaddict){}
   virtual ~CRule() {
   }

public:
   void getActions(const CStateItem &item, const CStringVector *sent, std::vector<CAction> &actions, const CStringVector *charcandpos) {
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
             getShiftRules(item, actions, charcandpos);
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

   void setsegboundary(const CStringVector *words, const CStringVector *sentence_raw)
   {
   	static CStringVector chars;
   	reset();

   	for( int index = 0; index < sentence_raw->size(); index++)
   	{
   		setSeparate(index, false);
   	}
   	int length = 0;
   	for( int index = 0; index < words->size(); index++)
   	{
   		setSeparate(length, true);
   		chars.clear();
         getCharactersFromUTF8String(words->at(index), &chars);
   		length = length + chars.size();
   	}
   }




   void segment(const CStringVector *sentence_raw, CStringVector *sentence) {

      // clear output
      sentence->clear();
      reset();

      int index_out = 0;
      for ( int index_raw = 0; index_raw < sentence_raw->size(); index_raw++ ) {
         const std::string& current_char = sentence_raw->at(index_raw) ;
         const std::string& last_char = index_raw>0 ? sentence_raw->at(index_raw-1) : "" ;
         if (current_char!=" ") {
            sentence->push_back(current_char);
            // if input map for character types available then use
            if ( index_raw > 0 && m_char_categories != 0 &&
                 ( m_char_categories->isFWorCD( last_char ) &&
                 m_char_categories->isFWorCD( current_char ) )
               )
//            if ( index_raw > 0 && m_char_categories != 0 &&
//                 ( ( m_char_categories->isFW( last_char ) &&
//                 m_char_categories->isFW( current_char ) ) ||
//                 ( m_char_categories->isCD( last_char ) &&
//                 m_char_categories->isCD( current_char ) ) )
//               )
               setSeparate(index_out, false);
            // always process space
            if ( index_raw > 0 && last_char == " ")
               setSeparate(index_out, true);
            ++index_out ;
         }
      }
   }

   void record(const CTwoStringVector *sent, CStringVector *retval) {
      assert(retval != 0);
      retval->clear();
      if (sent == 0)
         return;
      reset();
      std::string temp;
      CTwoStringVector::const_iterator it;
      unsigned size = retval->size();
      for (it=sent->begin(); it!=sent->end(); ++it) {
         getCharactersFromUTF8String(it->first, retval);
         assert(retval->size() > size); // the new word must has characters
         if (size>0 && m_char_categories && m_char_categories->isFWorCD(retval->at(size)) && m_char_categories->isFWorCD(retval->at(size-1)))
            setSeparate(size, true);
         for (int index=size; index<retval->size()-1; ++index) {
            if (m_char_categories && m_char_categories->isFWorCD(retval->at(index)) && m_char_categories->isFWorCD(retval->at(index+1)))
               setSeparate(index+1, false);
         }
         size = retval->size();
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
         if ( m_canstartword->lookup( m_WordCache->find( index, index, m_sent ), tag ) == false)
            return false;
         // if it matches, search from the next characters
         if ( tag == PENN_TAG_CD ) return true; // don't search for CD assume correct
         for (tmp_i=0; tmp_i<m_maxlengthbytag[tag]; ++tmp_i) {
            if ( m_tagdict->lookup( m_WordCache->find( index, std::min(index+tmp_i, static_cast<unsigned long>(m_sent->size())-1), m_sent ), tag ) )
               return true;
         }
         return false;
      }
      return true;
   }

   inline bool canAssignHead(const CWord &word, const unsigned long &head) {
   	if(m_partwordfreq->find(word, 0) < *m_maxwordfreq/5000+5) return true;

   	if( ((*m_mapwordheaddict)[word] | head) == (*m_mapwordheaddict)[word])
   	{
   		return true;
   	}

   	return false;
    }


   void getShiftRules(const CStateItem &item, std::vector<CAction> &actions, const CStringVector *charcandpos) {
      static CAction action;
      static CTag tmptag;

      if(item.stacksize() > 0 && item.node.is_partial())
      {
      	if(canAppend(item.current_word))
      	{
				unsigned long last_tag = item.node.pos;
				if(last_tag == PENN_TAG_CD || (item.node.end_c - item.node.begin_c +1 < m_maxlengthbytag[last_tag]) || mustAppend(item.current_word))
				{
					action.encodeShiftA();
					actions.push_back(action);
				}
      	}
      }
      else if(item.stacksize() == 0 || !item.node.is_partial())
      {
      	if(canSeparate(item.current_word))
      	{
      		int wordsize = 0;
				for(int idx = 0; idx < item.current_word; idx++)
				{
					if(canSeparate(idx))wordsize++;
				}
            bool fixedpos = false;
            if(charcandpos != NULL)
            {
            	fixedpos = tmptag.loadcheck((*charcandpos)[wordsize]);
            }

				for(unsigned long tag = CTag::FIRST; tag < CTag::COUNT; ++tag)
				{
					if(fixedpos && tag != tmptag.code())continue;
					if(canStartWord(tag, item.current_word) || fixedpos)
					{
						action.encodeShiftS(tag);
						actions.push_back(action);
					}
				}
      	}
      }

   }

   void getWordXYZRules(const CStateItem &item, std::vector<CAction> &actions)
   {
   	static CAction action;
   	if(item.stacksize() > 1 && item.node.is_partial() && item.stackPtr->node.is_partial())
   	{
   		int iCount = 0;
   		if(canAssignHead(m_WordCache->find(item.stackPtr->node.begin_c, item.node.end_c, m_sent), 1))
   		{
   			action.encodeWORDXYZ('x');
   			actions.push_back(action);
   			iCount++;
   		}
   		if(canAssignHead(m_WordCache->find(item.stackPtr->node.begin_c, item.node.end_c, m_sent), 2))
   		{
				action.encodeWORDXYZ('y');
				actions.push_back(action);
				iCount++;
   		}
   		if(canAssignHead(m_WordCache->find(item.stackPtr->node.begin_c, item.node.end_c, m_sent), 4))
   		{
				action.encodeWORDXYZ('z');
				actions.push_back(action);
				iCount++;
   		}
   	}
   }

   void getWordTRules(const CStateItem &item, std::vector<CAction> &actions)
	{
   	static CAction action;

   	if(mustAppend(item.current_word)) return;

   	if(item.stacksize() > 0 && item.node.is_partial()
   			&& (!item.stackPtr->node.valid() || !item.stackPtr->node.is_partial()))
   	{
   		if(canAssignTag(m_WordCache->find(item.node.begin_c, item.node.end_c, m_sent), item.node.pos)
   				|| item.current_word ==  m_sent->size() || mustSeparate(item.current_word)
   				|| (!(item.node.pos == PENN_TAG_CD) && (item.node.end_c - item.node.begin_c +1 >= m_maxlengthbytag[item.node.pos])) )
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
      if(right.is_partial() || left.is_partial()) return;
      // the normal method
#ifdef NO_TEMP_CONSTITUENT
      const bool temporary = false;
#else
      const bool prev_temp = stack_size>2 ? item.stackPtr->stackPtr->node.temp:false;
      //TRACE("prev_temp" << prev_temp);
#endif
      for (unsigned long constituent=CConstituent::FIRST; constituent<CConstituent::COUNT; ++constituent) {
         for (unsigned i=0; i<=1; ++i) {
	    const bool &head_left = static_cast<bool>(i);
            //const CWord &head_wd = m_sent->at((head_left?left:right).lexical_head).word;
            const CWord &head_wd = m_WordCache->find( (head_left?left:right).word_head->begin_c, (head_left?left:right).word_head->end_c, m_sent );
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
      if(child.is_partial())return;
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

};

} // namespace conparser

}

#endif
