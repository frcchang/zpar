#ifndef _CHINESE_DEPPAR_STATEITEM_H
#define _CHINESE_DEPPAR_STATEITEM_H

#include "utf.h"

/*===============================================================
 *
 * CStateItem - the search state item, representing a tagged sentence 
 *              or a part of a complete tagged sentence. 
 *
 * m_lWords stores the ending positions of each word. 
 *
 * For example: suppose that a sentence 
 *              A B C D E F
 *              is segmented into
 *              A B C | D E | F
 *              it's corresponding m_lWords is
 *              {2,4,5}
 *
 * Dependency links are added to all words except the last, 
 * because we need to consider surrounding tags. When all 
 * the char in the input sentence are processed, we add link
 * to the last item in the stateitem. Therefore, when adding
 * links, it is always safe to use index+1 unless the end of sentence
 * is reached. 
 *
 *==============================================================*/

class CStateItem {

protected:
   // words and tags
   unsigned *m_lWords;                      // words storing word end char index
   unsigned *m_lTags;                       // tags for each word in corresponding index
   unsigned m_nLength;                      // the number of words it contains

   // dependency links, the stack and action information
   int *m_lHeads;                           // the lexical head for each word
   int *m_lDepsL;                           // the leftmost dependency for each word (just for cache, temporary info)
   int *m_lDepsR;                           // the rightmost dependency for each word (just for cache, temporary info)
   int *m_lSibling;                         // the sibling towards head

public:

   // item score
   SCORE_TYPE score;

//-----------------------------------------------------------------------------

public:

   // constructor and destructor

   CStateItem() { 
      m_lWords   = new unsigned[MAX_SENTENCE_SIZE]; 
      m_lTags    = new unsigned[MAX_SENTENCE_SIZE]; 
      m_lHeads   = new int[MAX_SENTENCE_SIZE]; 
      m_lDepsL   = new int[MAX_SENTENCE_SIZE]; 
      m_lDepsR   = new int[MAX_SENTENCE_SIZE]; 
      m_lSibling = new int[MAX_SENTENCE_SIZE]; 
      clear(); 
   }

   ~CStateItem() { 
      delete[] m_lWords; 
      delete[] m_lTags; 
      delete[] m_lHeads; 
      delete[] m_lDepsL; 
      delete[] m_lDepsR; 
      delete[] m_lSibling; 
   }

   CStateItem(CStateItem& item) { cerr<<"CStateItem does not support copy constructor!"; cerr.flush(); assert(1==0); }

//-----------------------------------------------------------------------------

public:

   // comparison and assignment

   inline bool operator < (const CStateItem &item) const { 
      return this->score < item.score ? true : false ; 
   }

   inline bool operator > (const CStateItem &item) const { 
      return this->score > item.score ? true : false ; 
   }

   bool operator == (const CStateItem &item) const {
      // compare words and tags but no dep first
      if ( m_nLength != item.m_nLength ) return false;
      for ( int i=0; i<m_nLength; i++ ) {
         if ( m_lWords[i] != item.m_lWords[i] ) return false;
         if ( m_lTags[i] != item.m_lTags[i] ) return false;
      }
      // compare dep
      for ( int i=0; i<m_nLength; i++ ) {
         if ( m_lHeads[i] != item.m_lHeads[i] )
            return false;
      }
      return true;
   }

   bool operator != (const CStateItem &item) const {
      return !((*this)==item);
   }

   void copy(const CStateItem *from) {
      // copy length and score
      m_nLength = from->m_nLength;
      score = from->score;
      // copy words and tag information from item
      for (int i=0; i<m_nLength; i++) {
         m_lWords[i] = from->m_lWords[i];
         m_lTags[i] = from->m_lTags[i];
      }
      // copy dependency link and parsing stack
      for ( int i=0; i<m_nLength; i++ ){ // only copy active word (including m_nNext)
         m_lHeads[i] = from->m_lHeads[i];  
         m_lDepsL[i] = from->m_lDepsL[i]; 
         m_lDepsR[i] = from->m_lDepsR[i];
         m_lSibling[i] = from->m_lSibling[i];
      }
   }

   void operator = (const CStateItem &from) {
      this->copy(&from);
   }

   inline void clear() { 
      // clear score and length
      score = 0; 
      m_nLength = 0; 
   }

//-----------------------------------------------------------------------------

public:

   // information about segmentation

   inline unsigned size() const { return m_nLength; }
   inline unsigned charactersize() const { return m_nLength==0 ? 0 : m_lWords[m_nLength-1]+1; }

   inline unsigned getWordStart(const unsigned &i) const { return i>0?m_lWords[i-1]+1:0; }
   inline unsigned getWordEnd(const unsigned &i) const { return m_lWords[i]; }
   inline unsigned getWordLength(const unsigned &i) const { return i>0?m_lWords[i]-m_lWords[i-1]:m_lWords[0]+1;}

   inline int head( const unsigned int &index ) const { assert(index<m_nLength); return m_lHeads[index]; }
   inline int leftmostdep( const int &index ) const { assert(index<m_nLength); return m_lDepsL[index]; }
   inline int rightmostdep( const int &index ) const { assert(index<m_nLength); return m_lDepsR[index]; }
   inline int sibling( const unsigned int &index ) const { assert(index<m_nLength); return m_lSibling[index]; }

   inline int leftarity( const int &index ) const { 
      assert(index<m_nLength); 
      static int retval;
      retval = 0;
      static int dep;
      dep = leftmostdep( index );
      while (dep!=DEPENDENCY_LINK_NO_HEAD) {
         retval++;
         dep = sibling( dep );
      }
      return retval; 
   }
   inline int rightarity( const int &index ) const { 
      assert(index<m_nLength); 
      static int retval;
      retval = 0;
      static int dep;
      dep = rightmostdep( index );
      while (dep!=DEPENDENCY_LINK_NO_HEAD) {
         retval++;
         dep = sibling( dep );
      }
      return retval; 
   }

//-----------------------------------------------------------------------------

public:

   // the segmentation and tagging actions 

   void append(const unsigned &char_index, const unsigned &tag=PENN_TAG_NONE) {
      // append word and tag
      m_lWords[m_nLength] = char_index;
      m_lTags[m_nLength] = tag;
      // clear dependency link 
      m_lHeads[m_nLength] = DEPENDENCY_LINK_NO_HEAD; 
      m_lDepsL[m_nLength] = DEPENDENCY_LINK_NO_HEAD; 
      m_lDepsR[m_nLength] = DEPENDENCY_LINK_NO_HEAD; 
      m_lSibling[m_nLength] = DEPENDENCY_LINK_NO_HEAD ;
      // move
      m_nLength ++ ;
   }

   inline void replace(const unsigned &char_index, const unsigned &tag=PENN_TAG_NONE) {
      m_lWords[m_nLength-1] = char_index;
      m_lTags[m_nLength-1] = tag;
   }

   inline void setTag(const unsigned &index, const unsigned &tag) {
      m_lTags[index] = tag;
   }

   inline unsigned getTag(const unsigned &index) const {
      return m_lTags[index];
   }

//-----------------------------------------------------------------------------

public:

   // the parsing actions

   void addLink( const int &head, const int &dep ) {
      // head information
      m_lHeads[dep] = head;
      // dep information
      if ( head > dep ) {
         assert(m_lDepsL[head]==-1||m_lDepsL[head]>dep) ;
         m_lSibling[dep] = m_lDepsL[head] ;
         m_lDepsL[head] = dep ;
      }
      else {
         assert(m_lDepsR[head]<dep) ;
         m_lSibling[dep] = m_lDepsR[head];
         m_lDepsR[head] = dep;
      }
   }

   int findPreviousLinkPoint( const int &index ) {
      static int retval;
      retval = index;
      while (true) {
         // we never go across local head
         assert( m_lHeads[retval] != DEPENDENCY_LINK_NO_HEAD );
         // if head on the right of current
         if ( m_lHeads[retval]>retval ) {
            // if it is the leaf node then return its left
            if ( m_lDepsL[retval] == DEPENDENCY_LINK_NO_HEAD ) 
               return retval-1;
            // otherwise look for leaf node on the left
            assert(m_lDepsL[retval]<retval);
            retval = m_lDepsL[retval]; 
            assert(m_lHeads[retval]>retval);
            continue;
         }
         else { // head on the left
            // if there is left dependency, it can't be on the left of the head
            assert(m_lDepsL[retval]==DEPENDENCY_LINK_NO_HEAD||m_lHeads[retval]<m_lDepsL[retval]);
            return m_lHeads[retval];
         }
      } 
      THROW("depparser::CStateItem findPreviousLinkPoint: the code should not have reached here");
   }

   int findFirstHead() const {
      static int retval;
      if (m_nLength==0)
         return -1;
      retval = 0;
      while ( m_lHeads[retval] != DEPENDENCY_LINK_NO_HEAD ) {
         retval = max( retval+1, max(m_lHeads[retval], m_lDepsR[retval]) );
         assert(retval < m_nLength);
      }
      return retval;
   }

//-----------------------------------------------------------------------------

protected:

   void StandardLink( const CDependencyTree &tree, int index ) {
      for (int i=index-1; i>-1; i--) {
         if ( tree[i].head == index ) {
            addLink(index, i);
         }
         else if (tree[index].head==i) {
            addLink(i, index); 
         }
      }
   }

//-----------------------------------------------------------------------------

public:

   // returns whether the standard move is successful. 
   bool StandardMove( const CDependencyTree &tree, int char_index ) {
      static int len;

      // check if we are at the end of a word
      len = getUTF8StringLength(tree[m_nLength].word);
      if ((m_nLength==0?-1:static_cast<int>(m_lWords[m_nLength-1]))+len>char_index) // see if we are at the end of a word
         return false;                          // return unsuccessful if not

      // append the last word
      assert((m_nLength==0?-1:static_cast<int>(m_lWords[m_nLength-1]))+len==char_index);
      append( char_index, CTag(tree[m_nLength].tag).code() );

      // add standard links to words before nLength-1
      StandardLink( tree, m_nLength-2 );
      return true;
   }

   void StandardFinish( const CDependencyTree &tree ) {
      StandardLink( tree, m_nLength-1 );
   }

//-----------------------------------------------------------------------------

public:

   void GenerateTree( const CStringVector &input, CDependencyTree &output ) const {
      static string st;
      static int k;
      output.clear();
      k=0;
      for ( int i=0; i<size(); i++ ) {
         st.clear();
         while (k<=m_lWords[i]) {
            st += input.at(k);
            k++;
         }
         output.push_back( CDependencyTreeNode( st , CTag(m_lTags[i]).str() , m_lHeads[i] ) ) ;
      }
   }
};

#endif
