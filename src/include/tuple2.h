// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                              *
 * tuple2.h - the definitions for 2tuples                       *
 *                                                              *
 * Author: Yue Zhang                                            *
 * Date: 2010.3                                                 *
 *                                                              *
 ****************************************************************/

#ifndef _TUPLE2_H
#define _TUPLE2_H

/*===============================================================
 *
 * Tuple2
 *
 *==============================================================*/

template<class CClass1, class CClass2>
class CTuple2 {

protected:
   unsigned long int m_nHash;
   const CClass1 *m_object1;
   const CClass2 *m_object2;
   bool m_bAllocated; // whether this is a reference or has allocated memory

public:
   // constructors
   CTuple2() : m_nHash(0), m_object1(0), m_object2(0), m_bAllocated(false) {}
   // if an allocated tuple2 is copied to another, both will be allocated. 
   // if the source tuple2 is unallocated, neither will the target be allocated. 
   CTuple2(const CTuple2 &w) { 
      *this = w;
   }
   virtual ~CTuple2() { 
      if (m_bAllocated) { 
         delete m_object1; 
         delete m_object2;
      } 
   }

public:
   // initialiser
   // wether take use of allocate or refer, do not use both!
   // allocate makes new memory allocations, refer only refers to existing words
   void allocate(const CClass1 *w1, const CClass2 *w2) { 
      clear();
      m_object1 = new CClass1(*w1); 
      m_object2 = new CClass2(*w2); 
      computehash(); 
      m_bAllocated = true; 
   }
   void refer(const CClass1 *s1, const CClass2 *s2) { 
      if (m_bAllocated) {
         delete m_object1; 
         delete m_object2; 
      } 
      m_object1 = s1; 
      m_object2 = s2; 
      computehash(); 
      m_bAllocated = false; 
   }

   void clear() { 
      if (m_bAllocated) {
         delete m_object1; 
         delete m_object2; 
         m_bAllocated=false;
      } 
      m_object1 = 0; 
      m_object2 = 0; 
      m_nHash = 0; 
   }

public:
   // getting members
   const CClass1 *first() const { return m_object1; };
   const CClass2 *second() const { return m_object2; }

   // getting hash
   inline const unsigned long &hash() const { return m_nHash; }

   // comparison methods
   bool operator == (const CTuple2 &w) const { 
      return *m_object1 == *(w.m_object1) && 
             *m_object2 == *(w.m_object2);
   }
   bool operator != (const CTuple2 &w) const {
      return ! ((*this) == w);
   }
   bool operator < (const CTuple2 &w) const { 
      return *m_object1<*(w.m_object1) || (*m_object1==*(w.m_object1) && *m_object2<*(w.m_object2)); 
   }

   void operator = (const CTuple2 &w) {
      if (w.m_bAllocated) { 
         m_object1 = new CClass1(*(w.m_object1)); 
         m_object2 = new CClass2(*(w.m_object2)); 
         m_bAllocated = true; 
      } 
      else { 
         m_object1 = w.m_object1;
         m_object2 = w.m_object2;
         m_bAllocated = false;
      } 
      m_nHash = w.m_nHash;
   }

protected:
   inline void computehash() { 
      m_nHash = ( ::hash(*m_object1) * 31 + ::hash(*m_object2) ); 
   }
};

//===============================================================


template <class CClass1, class CClass2>
std::istream & operator >> (std::istream &is, CTuple2<CClass1, CClass2> &tuple2) {
   char c;
   CClass1 object1; 
   CClass2 object2;
   is >> object1;
   is >> c; 
   is >> object2;
   tuple2.allocate(&object1, &object2) ;
   assert( c == ',' ) ;
   return is ;
}

template <class CClass1, class CClass2>
std::ostream & operator << (std::ostream &os, const CTuple2<CClass1, CClass2> &tuple2) {
   os << *(tuple2.first()) << " , " << *(tuple2.second());
   return os ;
}

#endif

