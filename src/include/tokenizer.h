/****************************************************************
 *                                                              *
 * tokenizer.h - the tokenizer.                                 *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2007.5                         *
 *                                                              *
 ****************************************************************/

#ifndef _TOKENIZER_H
#define _TOKENIZER_H

#include "hash.h"

static const int TOKENIZER_SIZE = 65537 ;

/*===============================================================
 *
 * Tokenizer
 *
 *==============================================================*/

template <typename K>
class CTokenizer {
   protected:
      CHashMap<K, unsigned long int> m_mapTokens;
      vector<K> m_vecKeys;
      unsigned long int m_nWaterMark;
      unsigned long int m_nStartingToken;
   public:
      CTokenizer(int nTokenStartsFrom=0) : m_nStartingToken(nTokenStartsFrom), 
                                           m_nWaterMark(nTokenStartsFrom), 
                                           m_mapTokens(TOKENIZER_SIZE) { }
      virtual ~CTokenizer() { }
      unsigned long int lookup(const K &key) {
         unsigned long int retval; 
         bool bNew = m_mapTokens.findorinsert(key, m_nWaterMark, retval); 
         if (bNew) { 
            ++m_nWaterMark; 
            assert(m_nWaterMark!=0); // there is no overflow on the number of tokens!
            m_vecKeys.push_back(key);
         } return retval;
      }
      unsigned long int find(const K &key, unsigned long int val) {return m_mapTokens.find(key, val);}
      const K &key(unsigned long int token) {assert( token < m_vecKeys.size()+m_nStartingToken ); return m_vecKeys[token-m_nStartingToken];}
};

#endif
