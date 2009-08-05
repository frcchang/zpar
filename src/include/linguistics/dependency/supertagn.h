/****************************************************************
 *                                                              *
 * supertag2dep.h - the definitions for dependency supertag     *
 *                   which encodes dep information.             *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2008.4                         *
 *                                                              *
 ****************************************************************/

#ifndef _DEPENDENCY_SUPERTAG_H
#define _DEPENDENCY_SUPERTAG_H 

//===============================================================

class CSuperTag {

protected:
   unsigned m_code;

public:
   const static unsigned m_size=2 ;
   const static unsigned m_mlh=2 ; // mask for left neighbour being the head word of current word
   const static unsigned m_mrh=1 ; 
   const static unsigned limit=4;

   const static unsigned begin=3;

public:
   CSuperTag(int code=0) { assert(valid(code)); m_code = code; }
   CSuperTag(string s) { load(s); }
   virtual ~CSuperTag() {}

public:
   unsigned int code() const { return m_code; }
   string str() const { 
      switch (m_code) {
         case 0:
            return "00";
         case 1:
            return "01";
         case 2:
            return "10";
         case 3:
            return "11";
         default:
            throw("invalid code for supertag");
      }
   }
   void load(const string &s) {
      assert(s.size()==2);
      m_code = 0 ;
      for (int i=0; i<2; i++) {
         assert( s[i]=='0' || s[i]=='1' );
         if (s[i]=='1') m_code |= (1<<(1-i));
      }
   }

public:
   static unsigned first() { return 0; }
   static unsigned next(const unsigned &i, const bool &be=false) { return i+1; }
   static bool valid(const unsigned &i, const bool &be=false) { if (be||!be&&i<4) return true; }
   static bool inrange(const unsigned &i) { return i<limit; }

public:
   bool inrange() const { return inrange(m_code); }
   void operator++() { m_code = next(m_code); }

public:
   bool leftNeighbourIsHead() const { return m_code == m_mlh; }
   bool rightNeighbourIsHead() const { return m_code == m_mrh; }
   void setLeftNeighbourIsHead() { m_code = m_mlh; }
   void setRightNeighbourIsHead() { m_code = m_mrh; }

public:
   bool operator == (const CSuperTag &t1) const { return m_code == t1.m_code; }
   bool operator < (const CSuperTag &t1) const { return m_code < t1.m_code; }
   bool operator > (const CSuperTag &t1) const { return m_code > t1.m_code; }
   bool operator <= (const CSuperTag &t1) const { return m_code <= t1.m_code; }
   bool operator >= (const CSuperTag &t1) const { return m_code >= t1.m_code; }
   void operator = (const CSuperTag &t1) { m_code = t1.m_code; }

};

//===============================================================

inline void getSuperTagsFromDependencyTree( const CDependencyTree &deptree, vector<CSuperTag> &supertags ) {
   int head;
   int i;
   supertags.clear();
   for (i=0; i<deptree.size(); i++)
      supertags.push_back( CSuperTag(0) ) ;
   for (i=0; i<deptree.size(); i++) {
      head = deptree[i].head ;
      assert( head != i );
      if ( head != -1 ) {
         if (head==i-1) {
            supertags[head].setLeftNeighbourIsHead();
         }
         else if (head==i+1) {
            supertags[head].setRightNeighbourIsHead();
         }
      }
   }
}

#endif

