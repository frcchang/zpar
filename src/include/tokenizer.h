// Copyright (C) University of Oxford 2010
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

//static const unsigned TOKENIZER_SIZE = 65537 ;

/*===============================================================
 *
 * Tokenizer
 *
 *==============================================================*/

template <typename K, unsigned TOKENIZER_SIZE>
class CTokenizer {
   protected:
      CHashMap<K, unsigned long> m_mapTokens;
      std::vector<K> m_vecKeys;
      unsigned long m_nWaterMark;
      unsigned long m_nStartingToken;
   public:
      CTokenizer(unsigned nTokenStartsFrom=0) : m_mapTokens(TOKENIZER_SIZE), m_nWaterMark(nTokenStartsFrom), m_nStartingToken(nTokenStartsFrom) {}
      virtual ~CTokenizer() {}
      unsigned long lookup(const K &key) {
         unsigned long retval; 
         bool bNew = m_mapTokens.findorinsert(key, m_nWaterMark, retval); 
         if (bNew) { 
            ++m_nWaterMark; 
            assert(m_nWaterMark!=0); // there is no overflow on the number of tokens!
            m_vecKeys.push_back(key);
         } return retval;
      }
      unsigned long find(const K &key, const unsigned long &val) const {return m_mapTokens.find(key, val);}
      const K &key(const unsigned long &token) const {assert( token < m_vecKeys.size()+m_nStartingToken ); return m_vecKeys[token-m_nStartingToken];}
      const unsigned long &count() const {return m_nWaterMark;}
};

#endif
