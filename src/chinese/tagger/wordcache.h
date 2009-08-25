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
 * Given an input sentence, m_lWordCache caches the string words
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

protected:

   CWord *m_lWordCache;
   int m_nMaxSentenceSize;

public:

   CWordCache(int nSentenceSize) : m_nMaxSentenceSize(nSentenceSize) { m_lWordCache = new CWord[nSentenceSize*nSentenceSize]; }
   virtual ~CWordCache() { delete [] m_lWordCache; }

public:

   // find a word from the cache
   const CWord& find(const int &start, const int &end, const CStringVector* sentence) {
      static string temp;
      if (m_lWordCache[start*m_nMaxSentenceSize+end].empty()) { // empty string
         temp.clear();
         for (int i=start; i<=end; i++) // append the corresponding characters
            temp += sentence->at(i);
         m_lWordCache[start*m_nMaxSentenceSize+end].setString(temp);
      }
      return m_lWordCache[start*m_nMaxSentenceSize+end];
   }

   // find a word from the cache, if it is unknown then add into dictionary (tokenized word)
   const CWord& replace(const int &start, const int &end, const CStringVector* sentence) {
      static string temp;
      if (m_lWordCache[start*m_nMaxSentenceSize+end].empty() ||
          m_lWordCache[start*m_nMaxSentenceSize+end].unknown()) { // empty string
         temp.clear();
         for (int i=start; i<=end; i++) // append the corresponding characters
            temp += sentence->at(i);
         m_lWordCache[start*m_nMaxSentenceSize+end] = (temp);
      }
      return m_lWordCache[start*m_nMaxSentenceSize+end];
   }

   // clear the word cache
   void clear() {
      for (int i=0; i<m_nMaxSentenceSize; i++)
         for (int j=0; j<m_nMaxSentenceSize; j++)
            m_lWordCache[i*m_nMaxSentenceSize+j].clear();
   }

};

#endif
