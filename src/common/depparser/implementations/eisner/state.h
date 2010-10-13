// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                              *
 * span.h - the definitions for the span in dependency trees.   *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2007.10                        *
 *                                                              *
 ****************************************************************/


/*===============================================================
 *
 * CSpan - span definition
 *
 *==============================================================*/

class CSpan {

public:
   enum SPAN_TYPE { LF=0 /*left free*/, RF=1 /*right free*/, BF=2 /*both free*/};

protected:
   int m_nLeft;                     // the left boundary
   int m_nRight;                    // the right boundary
   int m_lLinks[MAX_SENTENCE_SIZE]; // the dependency links
   SPAN_TYPE m_nType;               // the type of span, see SPAN_TYPE
   bool m_bMinimal;                 // minimal span
   bool m_bActive;                  // active means being useful
   SCORE_TYPE m_nScore;             // score
   int m_nLeftArity;                // the arity of the left word 
   int m_nRightArity;               // the arity of the right word

public:
   CSpan() { clear(); }
   virtual ~CSpan() {};

public:
   inline int getLeftBoundary() const { return m_nLeft; }
   inline int getRightBoundary() const { return m_nRight; }
   inline SPAN_TYPE getType()const  { return m_nType; }
   inline int getLink( int index ) const { return m_lLinks[index]; }
   inline int getHeadArity() const { assert(m_nType!=BF); return m_nType==LF ? m_nLeftArity : m_nRightArity; }
   inline bool isMinimal() const { return m_bMinimal; }
   inline bool isActive() const { return m_bActive; }
   // the the score which can be assigned
   inline SCORE_TYPE &score() { return m_nScore; }
   inline bool isRightLinkedTo() const { return m_nRightArity != 0; }

private:
   // this method simply combines two spans
   // it does not set the type of the new span
   // nor does it add any links on it
   // it is only called as private method
   void setConcatenateSpan(const CSpan &left, const CSpan &right) {

      static int j;
      m_nLeft = left.m_nLeft;
      m_nRight = right.m_nRight;
      for ( j=m_nLeft; j<left.m_nRight; j++ )
         m_lLinks[j] = left.m_lLinks[j];
      if ( left.m_lLinks[left.m_nRight] != DEPENDENCY_LINK_NO_HEAD ) {
         assert( right.m_lLinks[right.m_nLeft] == DEPENDENCY_LINK_NO_HEAD );
         m_lLinks[left.m_nRight] = left.m_lLinks[left.m_nRight];
      }
      else {
         assert( right.m_lLinks[right.m_nLeft] != DEPENDENCY_LINK_NO_HEAD );
         m_lLinks[right.m_nLeft] = right.m_lLinks[right.m_nLeft];
      }
      for ( j=right.m_nLeft+1; j<=m_nRight; j++ )
         m_lLinks[j] = right.m_lLinks[j];
      m_bActive = true;
      m_nLeftArity = left.m_nLeftArity;
      m_nRightArity = right.m_nRightArity;
   }
   // this method adds a covering link modifying minimal
   void addCoveringLink(SPAN_TYPE type) {
      if ( type == LF ) {      // add link to result in a new LF
         m_lLinks[m_nRight] = m_nLeft;
         m_bMinimal = true;
         m_nLeftArity ++;
      }
      else if ( type == RF ) { // add link to result in a new RF
         m_lLinks[m_nLeft] = m_nRight;
         m_bMinimal = true;
         m_nRightArity ++;
      }
      else {                   // do not add covering link
         m_bMinimal = false;
      }
      m_nType = type;          // according to desired type
   }

public:
   // make the span a basic binary span - this method does not assign scores!
   void setBinarySpan(int start, SPAN_TYPE type) { 
      m_nLeft = start; 
      m_nRight = start + 1;
      if ( type == LF ) {
         m_lLinks[m_nLeft] = DEPENDENCY_LINK_NO_HEAD; 
         m_lLinks[m_nRight] = m_nLeft; 
         m_nLeftArity = 1;
         m_nRightArity = 0;
      }
      else if ( type == RF ) {
         m_lLinks[m_nLeft] = m_nRight;
         m_lLinks[m_nRight] = DEPENDENCY_LINK_NO_HEAD;
         m_nLeftArity = 0;
         m_nRightArity = 1;
      }
      else {
         m_lLinks[m_nLeft] = DEPENDENCY_LINK_NO_HEAD;
         m_lLinks[m_nRight] = DEPENDENCY_LINK_NO_HEAD;
         m_nLeftArity = 0;
         m_nRightArity = 0;
      }
      m_nType = type;
      m_bMinimal = true;
      m_bActive = true;
   }
   // make the span a combined type - this method does not assign scores!
   void setCombinedSpan(const CSpan &left, const CSpan &right, SPAN_TYPE type) {
      assert( left.m_nRight == right.m_nLeft );
      assert( left.m_bActive && right.m_bActive );
      assert( left.m_bMinimal );
      static int j;
      // ====== Left is LF ======
      if ( left.m_nType == LF ) { 
         if ( right.m_nType == LF ) {     
            // LF + LF = LF
            setConcatenateSpan(left, right);
            m_nType = LF;
            m_bMinimal = false;
         }
         else if ( right.m_nType == RF ) { 
            // LF + RF = None
            clear();
         }
         else {                            
            // LF + BF = LF / RF / BF
            setConcatenateSpan(left, right);
            addCoveringLink(type);
         }
      }
      // ====== Left is BF ======
      else if ( left.m_nType == BF ) { 
         if ( right.m_nType == RF ) {
            // BF + RF = LF / RF / BF
            setConcatenateSpan(left, right);
            m_nType = type; // according to desired type
            addCoveringLink(type);
         }
         else{
            // BF + BF / LF = None
            clear();
         }
      }
      // ====== Left is RF ======
      else {
         if ( right.m_nType == RF ) {
            // RF + RF = RF
            setConcatenateSpan(left, right);
            m_nType = RF;
            m_bMinimal = false;
         }
         else {
            // RF + LF / BF = None
            clear();
         }
      }
   }
   // clear a span
   inline void clear() { 
      m_bActive = false ; 
   }
   inline void copy( const CSpan &span ) {
      m_nLeft = span.m_nLeft; 
      m_nRight = span.m_nRight;
      m_nType = span.m_nType;
      m_bMinimal = span.m_bMinimal;
      m_bActive = span.m_bActive;
      m_nLeftArity = span.m_nLeftArity;
      m_nRightArity = span.m_nRightArity;
      for ( int i=0; i<MAX_SENTENCE_SIZE; ++i )
         m_lLinks[i] = span.m_lLinks[i];
      m_nScore = span.m_nScore;
   }

public:
   inline bool operator > (const CSpan &span) const { return this->m_nScore > span.m_nScore; }
   inline bool operator < (const CSpan &span) const { return this->m_nScore < span.m_nScore; }
};


