// Copyright (C) University of Oxford 2010
#ifndef _DEPENDENCY_PARSER_STATEITEM
#define _DEPENDENCY_PARSER_STATEITEM

/*===============================================================
 *
 * CStateItem - the search state item
 *
 *==============================================================*/

class CStateItem {

protected:
   int m_nNextWord;                         // index for the next word
   int m_lHeads[MAX_SENTENCE_SIZE];         // the lexical head for each word
   int m_lDepsL[MAX_SENTENCE_SIZE];         // the leftmost dependency for each word (just for cache, temporary info)
   int m_lDepsR[MAX_SENTENCE_SIZE];         // the rightmost dependency for each word (just for cache, temporary info)
   int m_lSibling[MAX_SENTENCE_SIZE];       // the sibling towards head
   SCORE_TYPE m_nLinkScore;                 // score of item

public:
   // constructors and destructor
   CStateItem() { clear(); }
   virtual ~CStateItem() { }
   CStateItem(CStateItem& item) { THROW("depparser::CStateItem does not support copy constructor!"); }
   // assignment operator
   void operator = ( const CStateItem &item ) {
      m_nNextWord = item.m_nNextWord;
      m_nLinkScore = item.m_nLinkScore;
      for ( int i=0; i<=m_nNextWord; ++i ){ // only copy active word (including m_nNext)
         m_lHeads[i] = item.m_lHeads[i];
         m_lDepsL[i] = item.m_lDepsL[i];
         m_lDepsR[i] = item.m_lDepsR[i];
         m_lSibling[i] = item.m_lSibling[i];
      }
   }

public:
   // propty
   inline int head( const unsigned long int &index ) const { assert(index<=m_nNextWord); return m_lHeads[index]; }
   inline int leftmostdep( const int &index ) const { assert(index<=m_nNextWord); return m_lDepsL[index]; }
   inline int rightmostdep( const int &index ) const { assert(index<=m_nNextWord); return m_lDepsR[index]; }
   inline int sibling( const unsigned long int &index ) const { assert(index<=m_nNextWord); return m_lSibling[index]; }
   inline int size( ) const { return m_nNextWord ; }

   inline int leftarity( const int &index ) const {
      assert(index<=m_nNextWord);
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
      assert(index<=m_nNextWord);
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

   inline SCORE_TYPE &score() { return m_nLinkScore; }
   inline SCORE_TYPE const_score() const { return m_nLinkScore; }

public:
   void clear() {
      m_nNextWord = -1;
      Move();
      m_nLinkScore = 0;
   }

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

public:
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
      retval = 0;
      while ( m_lHeads[retval] != DEPENDENCY_LINK_NO_HEAD ) {
         retval = std::max( retval+1, std::max(m_lHeads[retval], m_lDepsR[retval]) );
         assert(retval <= m_nNextWord);
      }
      return retval;
   }

public:
   void Move() {
      m_nNextWord ++ ;
      m_lHeads[m_nNextWord] = DEPENDENCY_LINK_NO_HEAD;
      m_lDepsL[m_nNextWord] = DEPENDENCY_LINK_NO_HEAD;
      m_lDepsR[m_nNextWord] = DEPENDENCY_LINK_NO_HEAD;
      m_lSibling[m_nNextWord] = DEPENDENCY_LINK_NO_HEAD ;
   }

   void StandardMove( const CDependencyParse &tree ) {
#ifdef DEBUG
      static bool bLinked;
      bLinked=false;
#endif
      // first add standard links
      for (int i=m_nNextWord-1; i>-1; i--) {
         if (tree[i].head==m_nNextWord) {
            addLink(m_nNextWord, i);
            assert(!bLinked);
         }
         else if (tree[m_nNextWord].head==i) {
            addLink(i, m_nNextWord);
#ifdef DEBUG
            bLinked=true;
#endif
         }
      }
      // second move
      Move();
   }

public:
   // comparison
   inline bool operator < (const CStateItem &item) const { return const_score() < item.const_score(); }
   inline bool operator > (const CStateItem &item) const { return const_score() > item.const_score(); }
   inline bool operator == (const CStateItem &item) const {
      static int i;
      if ( m_nNextWord != item.m_nNextWord )
         return false;
      for ( i=0; i<m_nNextWord; ++i ) {
         if ( m_lHeads[i] != item.m_lHeads[i] )
            return false;
      }
//      for ( i=0; i<m_nNextWord; ++i ) {
//         assert( m_lDepsL[i] == item.m_lDepsL[i] );
//         assert( m_lDepsR[i] == item.m_lDepsR[i] );
//         assert( m_lSibling[i] == item.m_lSibling[i] );
//      }
      return true;
   }
   inline bool operator != (const CStateItem &item) const {
      return ! ((*this)==item);
   }

public:

   void GenerateTree( const CTwoStringVector &input, CDependencyParse &output ) const {
      output.clear();
      for ( int i=0; i<size(); ++i )
         output.push_back( CDependencyTreeNode( input.at(i).first , input.at(i).second , m_lHeads[i] ) ) ;
   }

};

inline std::ostream & operator << (std::ostream &os, const CStateItem &item) {
   os << "index\thead\tlmd\trmd\tla\tra" << std::endl;
   for (int i=0; i<item.size(); ++i) {
      os << i << "\t" << item.head(i) << "\t" << item.leftmostdep(i) << "\t" << item.rightmostdep(i) << "\t" << item.leftarity(i) << "\t" << item.rightarity(i) << std::endl;
   }
   os << std::endl;
   return os;
}

#endif
