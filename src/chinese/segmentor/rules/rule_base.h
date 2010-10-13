// Copyright (C) University of Oxford 2010

/*===============================================================
 *
 * CRuleBase 
 *
 *==============================================================*/

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

