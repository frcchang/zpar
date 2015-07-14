// Copyright (C) University of Oxford 2010

#include "rule_base.h"

/*===============================================================
 *
 * CRule - the pruning structure for the segmentor
 *
 *==============================================================*/

class CRule : public CRuleBase {
private:
   const CCharCatDictionary *m_char_categories; // use rules to segment foreign words?
public:
   CRule(const CCharCatDictionary *d=0) : CRuleBase(), m_char_categories(d) {
   }
   virtual ~CRule() {
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
    *           that they will be joined in the output
    *           sentence by using the rules.
    *
    * parameters: sentence - output sentence without spaces
    *
    *----------------------------------------------------------------*/
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

public:
   void setKnowledge(const CCharCatDictionary *d=0) {
      m_char_categories = d;
   }
};

