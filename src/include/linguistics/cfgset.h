/****************************************************************
 *                                                              *
 * cfgset.h - the definitions for Chinese cfg constituent-set.  *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2008.12                        *
 *                                                              *
 ****************************************************************/

#ifndef _CFGSET_H
#define _CFGSET_H

#define CFGSET_SIZE 9

#include "hash.h"

const unsigned long PACKED_CON_OR_TAG_SIZE = std::max(static_cast<unsigned long>(CConstituent::SIZE), static_cast<unsigned long>(CTag::SIZE)) + 1;
// the first bit being one for pos

//=============================================================

inline unsigned long encodeTorC(const unsigned long &tagorconstituent, const bool &tag) {
   assert((tagorconstituent&(1<<(PACKED_CON_OR_TAG_SIZE-1)))==0);
   return tagorconstituent | (tag?(1<<(PACKED_CON_OR_TAG_SIZE-1)):0);
}
inline unsigned long encodeTorCs(const unsigned long &toc1, const unsigned long &toc2) {
   assert((toc2>>PACKED_CON_OR_TAG_SIZE)==0);
   return (toc1<<PACKED_CON_OR_TAG_SIZE) | toc2;
}

inline unsigned long encodeTorCs(const unsigned long &toc1, const unsigned long &toc2, const unsigned long &toc3) {
   assert((toc2>>PACKED_CON_OR_TAG_SIZE)==0);
   assert((toc3>>PACKED_CON_OR_TAG_SIZE)==0);
   return (toc1<<(PACKED_CON_OR_TAG_SIZE*2)) | (toc2<<PACKED_CON_OR_TAG_SIZE) | toc3;
}

/*===============================================================
 *
 * definitions about constituent set 
 *
 *==============================================================*/

class CCFGSet {

protected:
   unsigned long long m_nHash;

public:
   CCFGSet() : m_nHash(0) { }
   CCFGSet(const unsigned long long &hash) : m_nHash(hash) { }
//   virtual ~CCFGSet() {}

public:
   void operator += (const unsigned long long &i) { 
      assert(i>>PACKED_CON_OR_TAG_SIZE==0);
      assert(m_nHash>>(PACKED_CON_OR_TAG_SIZE*(CFGSET_SIZE-1))==0);
      m_nHash = (m_nHash<<PACKED_CON_OR_TAG_SIZE)|i;
   }

private:
   bool isTag(const unsigned long long &tc) const {
      return tc & (1<<(PACKED_CON_OR_TAG_SIZE-1));
   }
   unsigned long getTag(const unsigned long long &tc) const {
      return tc & ((1<<(PACKED_CON_OR_TAG_SIZE-1))-1);
   }

public:
   virtual unsigned long long hash() const { return m_nHash; }
   virtual bool operator == (const CCFGSet &set) const { return m_nHash == set.m_nHash; }
   virtual bool operator != (const CCFGSet &set) const { return m_nHash != set.m_nHash; }
   virtual bool operator < (const CCFGSet &set) const { return m_nHash < set.m_nHash; }
   void operator = (const CCFGSet &set) { m_nHash = set.m_nHash; }
   void operator = (const unsigned long long &uint) { m_nHash = uint; }
   const string str() const { 
      string retval = "";
      unsigned long long hs = m_nHash;
      unsigned long tc;
      while (hs) {
         if (retval.empty()==false) retval = " " + retval;
         tc = hs & ((1 << PACKED_CON_OR_TAG_SIZE) -1);
         if (isTag(tc))
            retval = "."+CTag(getTag(tc)).str()+retval;
         else
            retval = CConstituent(tc).str() + retval;
         hs >>= PACKED_CON_OR_TAG_SIZE;
      }
      return retval; 
   }
   void load(const string &s) {
      m_nHash = 0;
      istringstream iss(s);
      static string t;
      iss >> t;
      while (iss.good()) {
         if (t[0]=='.') 
            *this += CTag(t.substr(1)).code()|(1<<(PACKED_CON_OR_TAG_SIZE-1));
         else
            *this += CConstituent(t).code();
         iss >> t;
      }
   }
   bool empty() { return m_nHash==0 ; }
   bool clear() { m_nHash = 0; }
};

//===============================================================

inline istream & operator >> (istream &is, CCFGSet &c) {

   string s;
   string t;
   is >> t;
   assert(t=="[");

   is >> t;
   s = "";
   while (t!="]") {
     s+=t;
     s+=" ";
     is >> t;
   }

   c.load(s);

   return is;
}

inline ostream & operator << (ostream &os, const CCFGSet &c) {

   os << "[ ";
   os << c.str();
   os << " ]";

   return os;
}

inline unsigned long long hash(const CCFGSet &set) {return set.hash();}

#endif

