// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                              *
 * tuple3.h - the definitions for 3tuples                       *
 *                                                              *
 * Author: Yue Zhang                                            *
 * Date: 2010.3                                                 *
 *                                                              *
 ****************************************************************/

#ifndef _TUPLE3_H
#define _TUPLE3_H

/*===============================================================
 *
 * Tuple3
 *
 *==============================================================*/

template<class CClass1, class CClass2, class CClass3>
class CTuple3 {

protected:
   unsigned long int m_nHash;
   const CClass1 *m_object1;
   const CClass2 *m_object2;
   const CClass3 *m_object3;
   bool m_bAllocated; // whether this is a reference or has allocated memory

public:
   // constructors
   CTuple3() : m_object1(0), m_object2(0), m_object3(0), m_bAllocated(false) {}
   // if an allocated tuple3 is copied to another, both will be allocated. 
   // if the source tuple3 is unallocated, neither will the target be allocated. 
   CTuple3(const CTuple3 &w) { 
      *this = w;
   }
   virtual ~CTuple3() { 
      if (m_bAllocated) { 
         delete m_object1; 
         delete m_object2;
         delete m_object3;
      } 
   }

public:
   // initialiser
   // wether take use of allocate or refer, do not use both!
   // allocate makes new memory allocations, refer only refers to existing words
   void allocate(const CClass1 *w1, const CClass2 *w2, const CClass3 *w3) { 
      clear();
      m_object1 = new CClass1(*w1); 
      m_object2 = new CClass2(*w2); m_object3 = new CClass3(*w3); 
      computehash(); 
      m_bAllocated = true; 
   }
   void refer(const CClass1 *s1, const CClass2 *s2, const CClass3 *s3) { 
      if (m_bAllocated) {
         delete m_object1; 
         delete m_object2; 
         delete m_object3; 
      } 
      m_object1 = s1; 
      m_object2 = s2; 
      m_object3 = s3; 
      computehash(); 
      m_bAllocated = false; 
   }

   void clear() { 
      if (m_bAllocated) {
         delete m_object1; 
         delete m_object2; 
         delete m_object3; 
         m_bAllocated=false;
      } 
      m_object1 = 0; 
      m_object2 = 0; 
      m_object3 = 0; 
      m_nHash = 0; 
   }

public:
   // getting members
   const CClass1 *first() const { return m_object1; };
   const CClass2 *second() const { return m_object2; }
   const CClass3 *third() const { return m_object3; }

   // getting hash
   inline const unsigned long int &hash() const { return m_nHash; }

   // comparison methods
   bool operator == (const CTuple3 &w) const { 
      return *m_object1 == *(w.m_object1) && 
             *m_object2 == *(w.m_object2) &&
             *m_object3 == *(w.m_object3); 
   }
   bool operator != (const CTuple3 &w) const {
      return ! ((*this) == w);
   }
   bool operator < (const CTuple3 &w) const { 
      return *m_object1<*(w.m_object1) || (*m_object1==*(w.m_object1) && *m_object2<*(w.m_object2)) || (*m_object1==*(w.m_object1) && *m_object2 == *(w.m_object2) && *m_object3<*(w.m_object3)); 
   }

   void operator = (const CTuple3 &w) {
      if (w.m_bAllocated) { 
         m_object1 = new CClass1(*(w.m_object1)); 
         m_object2 = new CClass2(*(w.m_object2)); 
         m_object3 = new CClass3(*(w.m_object3)); 
         m_bAllocated = true; 
      } 
      else { 
         m_object1 = w.m_object1;
         m_object2 = w.m_object2;
         m_object3 = w.m_object3;
         m_bAllocated = false;
      } 
      m_nHash = w.m_nHash;
   }

protected:
   inline void computehash() { 
      m_nHash = ( ::hash(*m_object1) * 31 + ::hash(*m_object2) ) * 29 + ::hash(*m_object3); 
   }
};

//===============================================================

//template<class CClass1, class CClass2, class CClass3>
//inline unsigned long int hash(const CTuple3<CClass1, CClass2, CClass3> &w) { return w.hash(); }

//===============================================================

template <class CClass1, class CClass2, class CClass3>
std::istream & operator >> (std::istream &is, CTuple3<CClass1, CClass2, CClass3> &tuple3) {
   char c, d ;
   CClass1 object1; 
   CClass2 object2;
   CClass3 object3;
   is >> object1; 
   is >> c;
   is >> object2; 
   is >> d; 
   is >> object3;
   tuple3.allocate(&object1, &object2, &object3) ;
   assert( c == ',' && d == ',' ) ;
   return is ;
}

template <class CClass1, class CClass2, class CClass3>
std::ostream & operator << (std::ostream &os, const CTuple3<CClass1, CClass2, CClass3> &tuple3) {
   os << *(tuple3.first()) << " , " << *(tuple3.second()) << " , " << *(tuple3.third());
   return os ;
}

#endif

