/****************************************************************
 *                                                              *
 * supertag.h - the definitions for dependency supertag         *
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
   const static unsigned m_size=4 ;
   const static unsigned m_mlh=(1<<3) ; // mask for left head
   const static unsigned m_mrh=(1<<2) ; 
   const static unsigned m_mld=(1<<1) ;
   const static unsigned m_mrd=1 ;
   const static unsigned limit=1<<m_size;

   const static unsigned begin=0xc;
   const static unsigned end=0xd;

public:
   CSuperTag(int code=0) { assert(valid(code)); m_code = code; }
   CSuperTag(string s) { load(s); }
   virtual ~CSuperTag() {}

public:
   unsigned int code() const { return m_code; }
   string str() const { 
      string retval; 
      for (int i=m_size-1; i>=0; i--) 
         retval += '0'+((m_code&(1<<i))?1:0); 
      return retval; 
   }
   void load(const string &s) {
      assert(s.size()==4);
      m_code = 0 ;
      for (int i=0; i<m_size; i++) {
         assert( s[i]=='0' || s[i]=='1' );
         if (s[i]=='1') m_code |= (1<<(m_size-i-1));
      }
      assert(valid(m_code));
   }

public:
   //static unsigned begin() { return bg; }
   //static unsigned end() { return ed; }
   static unsigned first() { return 0; }
   static unsigned next(const unsigned &i, const bool &bIncludeBE=false) { 
      static unsigned retval;
      retval = i+1;
      while (!valid(retval,bIncludeBE)&&inrange(retval))
         retval++;
      return retval;
   }
   static bool valid(const unsigned &i, const bool &bIncludeBE=false) {
      if ( i >= (1<<m_size) ) return false;
      if ( (i&m_mlh) && (i&m_mrh) ) {
         if (bIncludeBE&&(i==begin||i==end))
            return true;
         else
            return false;
      }
      return true;
   }
   static bool inrange(const unsigned &i) { return i<limit; }

public:
   bool inrange() const { return inrange(m_code); }
   void operator++() { m_code = next(m_code); }

public:
   bool hasLeftHead() const { return m_code&m_mlh; }
   bool hasRightHead() const { return m_code&m_mrh; }
   bool hasLeftDep() const { return m_code&m_mld; }
   bool hasRightDep() const { return m_code&m_mrd; }
   void setHasLeftHead() { m_code |= m_mlh; }
   void setHasRightHead() { m_code |= m_mrh; }
   void setHasLeftDep() { m_code |= m_mld; }
   void setHasRightDep() { m_code |= m_mrd; }

public:
   bool operator == (const CSuperTag &t1) const { return m_code == t1.m_code; }
   bool operator < (const CSuperTag &t1) const { return m_code < t1.m_code; }
   bool operator > (const CSuperTag &t1) const { return m_code > t1.m_code; }
   bool operator <= (const CSuperTag &t1) const { return m_code <= t1.m_code; }
   bool operator >= (const CSuperTag &t1) const { return m_code >= t1.m_code; }
   void operator = (const CSuperTag &t1) { m_code = t1.m_code; }

public:
   static bool compatible( const int &head, const int &dep, const CSuperTag &head_tag, const CSuperTag &dep_tag ) {
      assert( head != dep );
      if ( head > dep ) 
         return head_tag.hasLeftDep() && dep_tag.hasRightHead() ;
      else
         return head_tag.hasRightDep() && dep_tag.hasLeftHead() ;
   }
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
         if (head<i) {
            supertags[i].setHasLeftHead();
            supertags[head].setHasRightDep();
         }
         else {
            supertags[i].setHasRightHead();
            supertags[head].setHasLeftDep();
         }
      }
   }
}


#endif

