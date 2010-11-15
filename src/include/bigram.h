// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                              *
 * bigram.h - the definitions for bigrams                       *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2007.10                        *
 *                                                              *
 ****************************************************************/

#ifndef _BIGRAM_H
#define _BIGRAM_H

/*===============================================================
 *
 * Bigram
 *
 *==============================================================*/

template<class CUnigram>
class CBigram {

protected:
   unsigned long m_nHash;
   const CUnigram *m_unigram1;
   const CUnigram *m_unigram2;
   bool m_bAllocated; // whether this is a reference or has allocated memory

public:
   // constructors
   CBigram() : m_nHash(0), m_unigram1(0), m_unigram2(0), m_bAllocated(false) {}
   // if an allocated bigram is copied to another, both will be allocated. 
   // if the source bigram is unallocated, neither will the target be allocated. 
   CBigram(const CBigram &w) { *this = w;}
   virtual ~CBigram() { if (m_bAllocated) { delete m_unigram1; delete m_unigram2;} }

public:
   // initialiser
   // wether take use of allocate or refer, do not use both!
   // allocate makes new memory allocations, refer only refers to existing words
   void allocate(const CUnigram &w1, const CUnigram &w2) { 
      clear(); 
      m_unigram1 = new CUnigram(w1); 
      m_unigram2 = new CUnigram(w2); 
      computehash(); 
      m_bAllocated = true; 
   }
   void refer(const CUnigram *s1, const CUnigram *s2) { 
      m_unigram1 = s1; 
      m_unigram2 = s2; 
      computehash(); 
      m_bAllocated = false; 
   }

   void clear() { 
      if (m_bAllocated) {
         delete m_unigram1; 
         delete m_unigram2; 
         m_bAllocated=false;
      } 
      m_unigram1 = 0; 
      m_unigram2 = 0; 
      m_nHash = 0; 
   }

public:
   // getting members
   const CUnigram *first() const { return m_unigram1; };
   const CUnigram *second() const { return m_unigram2; }

   // getting hash
   inline unsigned long int hash() const { return m_nHash; }

   // comparison methods
   bool operator == (const CBigram &w) const { return *m_unigram1 == *(w.m_unigram1) && *m_unigram2 == *(w.m_unigram2); }
   bool operator < (const CBigram &w) const { return *m_unigram1<*(w.m_unigram1) || (*m_unigram1==*(w.m_unigram1) && *m_unigram2<*(w.m_unigram2)); }

   // assignment
   void operator = (const CBigram &w) {
      if (w.m_bAllocated) { 
         m_unigram1 = new CUnigram(*(w.m_unigram1)); 
         m_unigram2 = new CUnigram(*(w.m_unigram2)); 
         m_nHash = w.m_nHash;
         m_bAllocated = true; 
      } 
      else { 
         m_unigram1 = w.m_unigram1;
         m_unigram2 = w.m_unigram2;
         m_nHash = w.m_nHash;
         m_bAllocated = false;
      } 
   }

protected:
   inline void computehash() { m_nHash = m_unigram1->hash() * 31 + m_unigram2->hash(); }
};

//===============================================================

template<class CUnigram>
inline unsigned long int hash(const CBigram<CUnigram> &w) { return w.hash(); }

//===============================================================

template <class CUnigram>
std::istream & operator >> (std::istream &is, CBigram<CUnigram> &bigram) {
   char c ;
   CUnigram unigram1; 
   CUnigram unigram2;
   is >> unigram1 >> c >> unigram2 ;
   bigram.allocate(unigram1, unigram2) ;
   assert( c == ',' ) ;
   return is ;
}

template <class CUnigram>
std::ostream & operator << (std::ostream &os, const CBigram<CUnigram> &bigram) {
   os << *(bigram.first()) << " , " << *(bigram.second()) ;
   return os ;
}

#endif

