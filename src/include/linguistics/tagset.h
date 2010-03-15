/****************************************************************
 *                                                              *
 * tagset.h - the definitions for general tag set.              *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2009.8                         *
 *                                                              *
 ****************************************************************/

#ifndef _TAGSET_H
#define _TAGSET_H

/*===============================================================
 *
 * definitions 
 *
 *==============================================================*/

template<typename CTag, unsigned long size>
class CTagSet {

protected:
   unsigned long m_nHash;

public:
   CTagSet() : m_nHash(0) { }
   CTagSet(const unsigned long hash) : m_nHash(hash) { assert(hash>>(CTag::SIZE*size)==0); }
   virtual ~CTagSet() {}

private:
   void operator += (const CTag &i) { 
      m_nHash = (m_nHash<<CTag::SIZE) | i.code();
   }
   unsigned long getTag(const unsigned long &tc) const {
      return tc & ((1<<(CTag::SIZE-1))-1);
   }

public:
   virtual const unsigned long &hash() const { return m_nHash; }
   virtual bool operator == (const CTagSet &set) const { 
      return m_nHash == set.m_nHash; 
   }
   virtual bool operator != (const CTagSet &set) const { 
      return ! ((*this) == set); 
   }
   virtual bool operator < (const CTagSet &set) const { 
      return m_nHash < set.m_nHash; 
   }
   void operator = (const CTagSet &set) { 
      m_nHash = set.m_nHash; 
   }
   const string str() const { 
      string retval = "";
      unsigned long hs = m_nHash;
      unsigned long tc;
      for (unsigned long i=0; i<size; ++i) {
         if (retval.empty()==false) retval = " " + retval;
         tc = hs & ((1 << CTag::SIZE) -1);
         retval = CTag(tc).str() + retval;
         hs >>= CTag::SIZE;
      }
      return retval; 
   }
   void load(const string &s) {
      clear();
      istringstream iss(s);
      static string t;
      iss >> t;
      for (unsigned long i=0; i<size; ++i) {
         assert(iss.good());
         *this += CTag(t).code();
         iss >> t;
      }
   }
   bool clear() { m_nHash = 0; }
};

//===============================================================

template<typename CTag, unsigned long size>
inline const unsigned long &hash(const CTagSet<CTag, size> &set) {return set.hash();}

//===============================================================

template<typename CTag, unsigned long size>
inline istream & operator >> (istream &is, CTagSet<CTag, size> &c) {

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

template<typename CTag, unsigned long size>
inline ostream & operator << (ostream &os, const CTagSet<CTag, size> &c) {

   os << "[ ";
   os << c.str();
   os << " ]";

   return os;
}

#endif

