
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
   void setSeparate(unsigned long int index, bool separate) { 
      m_chars[index] = ( separate ? eSeparate : eAppend ) ; 
   }
   bool canSeparate(unsigned long int index) {
      return m_chars[index] == eNoAction || m_chars[index] == eSeparate;
   }
   bool canAppend(unsigned long int index) { 
      return m_chars[index] == eNoAction || m_chars[index] == eAppend; 
   }
   bool mustSeparate(unsigned long index) {
      return m_chars[index] == eSeparate; 
   }
   bool mustAppend(unsigned long index) {
      return m_chars[index] == eAppend;
   }

   void reset() { for (int i=0; i<MAX_SENTENCE_SIZE; ++i) m_chars[i] = 0; }
};

