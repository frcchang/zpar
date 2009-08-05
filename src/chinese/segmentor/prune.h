
/*===============================================================
 *
 * CPrune - the pruning structure for the segmentor
 *
 *==============================================================*/

class CPrune {
   private:
      unsigned char m_chars[MAX_SENTENCE_SIZE];
   public:
      enum PRUNE { eNoAction = 0, eAppend, eSeparate }; 
      CPrune() { reset(); }
      ~CPrune() {}
      void segment(unsigned int index, bool separate) { m_chars[index] = ( separate ? eSeparate : eAppend ) ; }
      bool canSeparate(unsigned int index) { return m_chars[index] == eNoAction || m_chars[index] == eSeparate; }
      bool canAppend(unsigned int index) { return m_chars[index] == eNoAction || m_chars[index] == eAppend; }
      void reset() { for (int i=0; i<MAX_SENTENCE_SIZE; i++) m_chars[i] = 0; }
};

/*----------------------------------------------------------------
 *
 * segmentationRules - find segmentation rules from raw input
 *                     sentence, by removing spaces from them
 *                     and add the corresponding separation
 *                     to rules. Because the input spaces are
 *                     normally inserted only between foreigh
 *                     letters and arabic numbers, the function
 *                     also takes an input word map to recognise
 *                     foreign letters and arabic numbers, so 
 *                     that they will be joined in the output
 *                     sentence by using the rules. 
 *                     
 *----------------------------------------------------------------*/

inline
void segmentationRules(const CSentenceRaw *sentence_raw, const CCharCatDictionary *char_categories, CSentenceRaw *sentence, segmentor::CPrune *rules) {

   // clear output
   sentence->clear();
   rules->reset();

   int index_out = 0;
   for ( int index_raw = 0; index_raw < sentence_raw->size(); index_raw++ ) {
      const string& current_char = sentence_raw->at(index_raw) ; 
      const string& last_char = index_raw>0 ? sentence_raw->at(index_raw-1) : "" ; 
      if (current_char!=" ") {
         sentence->push_back(current_char);
         // if input map for character types available then use
         if ( index_raw > 0 && char_categories != 0 &&
              char_categories->isFWorCD( last_char ) &&
              char_categories->isFWorCD( current_char )
            ) 
            rules->segment(index_out, false); 
         // always process space 
         if ( index_raw > 0 && last_char == " ")
            rules->segment(index_out, true);
         index_out++ ; 
      }
   }
}
