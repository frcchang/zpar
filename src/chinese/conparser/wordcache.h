// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                              *
 * wordcache.h - the word cache for the joint depparser.        *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2006.10                        *
 *                                                              *
 ****************************************************************/

#ifndef _CHINESE_TAGGER_WORDCACHE_H
#define _CHINESE_TAGGER_WORDCACHE_H

/*===============================================================
 *
 * CWordCache - the definition .
 *
 * Given an input sentence, m_lWordCache caches the std::string words
 * in certain positions. 
 * For example: suppose that a sentence is
 *              A B C D E F
 *              m_lWordCache[0][0] = A
 *              m_lWordCache[0][4] = A B C D E
 *              m_lWordCache[1][5] = B C D E F
 * Note: there are space wastes, but this seem to be the most time
 *       efficient. 
 * 
 *===============================================================*/

class CWordCache {

public:

   unsigned *m_lWordCache;
   int m_nMaxSentenceSize;

public:

   CWordCache(int nSentenceSize) : m_nMaxSentenceSize(nSentenceSize) { TRACE(nSentenceSize);m_lWordCache = new unsigned[nSentenceSize*nSentenceSize]; }
   virtual ~CWordCache() { delete [] m_lWordCache; }

public:

   // find a word from the cache
   const unsigned& find(const int &start, const int &end, const CStringVector* sentence) {
      static std::string temp;
      if (m_lWordCache[start*m_nMaxSentenceSize+end] == ~0 ) { // empty std::string
         temp.clear();
         for (int i=start; i<=end; ++i) // append the corresponding characters
            temp += sentence->at(i);
            m_lWordCache[start*m_nMaxSentenceSize+end] = CWord(temp,false).code();

      }
      return m_lWordCache[start*m_nMaxSentenceSize+end];
   }

   // find a word from the cache, if it is unknown then add into dictionary (tokenized word)
   const unsigned& replace(const int &start, const int &end, const CStringVector* sentence) {
      static std::string temp;
      if (m_lWordCache[start*m_nMaxSentenceSize+end] == ~0 ||
          m_lWordCache[start*m_nMaxSentenceSize+end] == CWord::UNKNOWN ) { // empty std::string
         temp.clear();
         for (int i=start; i<=end; ++i) // append the corresponding characters
            temp += sentence->at(i);
         m_lWordCache[start*m_nMaxSentenceSize+end] = CWord(temp, true).code();
      }
      return m_lWordCache[start*m_nMaxSentenceSize+end];
   }

   // clear the word cache
   void clear() {
//      for (int i=0; i<m_nMaxSentenceSize; ++i)
//         for (int j=0; j<m_nMaxSentenceSize; j++)
//            m_lWordCache[i*m_nMaxSentenceSize+j].clear();
      memset(m_lWordCache, 255, sizeof(m_lWordCache[0])*m_nMaxSentenceSize*m_nMaxSentenceSize);
   }

};

#endif
