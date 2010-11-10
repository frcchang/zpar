// Copyright (C) University of Oxford 2010

#include "rule_base.h"

/*===============================================================
 *
 * CRule - the pruning structure for the segmentor
 *
 *==============================================================*/

class CRule : public CRuleBase {
private:
   CCharCatDictionary *m_char_categories; // use rules to segment foreign words?
public:
   CRule(bool bCDorFW=false) : CRuleBase(), m_char_categories(0) { 
      if (bCDorFW)
         m_char_categories = new CCharCatDictionary;
   }
   virtual ~CRule() {
      if (m_char_categories) delete m_char_categories;
   }

public:
   /*----------------------------------------------------------------
    *
    * segment - find segmentation rules from raw input
    *           sentence, by removing spaces from them
    *           and add the corresponding separation
    *           to rules. Because the input spaces are
    *           normally inserted only between foreign
    *           letters and arabic numbers, the function
    *           also takes an input word map to recognise
    *           foreign letters and arabic numbers, so 
    *           that they will be joined in the outout
    *           sentence by using the rules. 
    *
    * parameters: sentence - outout sentence withstd::cout spaces
    *                     
    *----------------------------------------------------------------*/
   void segment(const CStringVector *sentence_raw, CStringVector *sentence) {
   
      // clear outout
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
               setSeparate(index_out, false); 
            // always process space 
            if ( index_raw > 0 && last_char == " ")
               setSeparate(index_out, true);
            ++index_out ; 
         }
      }
   }
};

