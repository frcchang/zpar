// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                              *
 * tuple4.h - the definitions for 4tuples                       *
 *                                                              *
 * Author: Yue Zhang                                            *
 * Date: 2010.3                                                 *
 *                                                              *
 ****************************************************************/

#ifndef _TUPLE4_H
#define _TUPLE4_H

/*===============================================================
 *
 * Tuple4
 *
 *==============================================================*/

template<class CClass1, class CClass2, class CClass3, class CClass4>
class CTuple4 {

protected:
   unsigned long int m_nHash;
   const CClass1 *m_object1;
   const CClass2 *m_object2;
   const CClass3 *m_object3;
   const CClass4 *m_object4;
   bool m_bAllocated; // whether this is a reference or has allocated memory

public:
   // constructors
   CTuple4() : m_object1(0), m_object2(0), m_object3(0), m_object4(0), m_bAllocated(false) {}
   // if an allocated tuple4 is copied to another, both will be allocated. 
   // if the source tuple4 is unallocated, neither will the target be allocated. 
   CTuple4(const CTuple4 &w) { 
      *this = w;
   }
   virtual ~CTuple4() { 
      if (m_bAllocated) { 
         delete m_object1; 
         delete m_object2;
         delete m_object3;
         delete m_object4;
      } 
   }

public:
   // initialiser
   // wether take use of allocate or refer, do not use both!
   // allocate makes new memory allocations, refer only refers to existing words
   void allocate(const CClass1 *w1, const CClass2 *w2, const CClass3 *w3, const CClass4 *w4) { 
      clear();
      m_object1 = new CClass1(*w1); 
      m_object2 = new CClass2(*w2); m_object3 = new CClass3(*w3); m_object4 = new CClass4(*w4); 
      computehash(); 
      m_bAllocated = true; 
   }
   void refer(const CClass1 *s1, const CClass2 *s2, const CClass3 *s3, const CClass4 *s4) { 
      if (m_bAllocated) {
         delete m_object1; 
         delete m_object2; 
         delete m_object3; 
         delete m_object4; 
      } 
      m_object1 = s1; 
      m_object2 = s2; 
      m_object3 = s3; 
      m_object4 = s4; 
      computehash(); 
      m_bAllocated = false; 
   }

   void clear() { 
      if (m_bAllocated) {
         delete m_object1; 
         delete m_object2; 
         delete m_object3; 
         delete m_object4; 
         m_bAllocated=false;
      } 
      m_object1 = 0; 
      m_object2 = 0; 
      m_object3 = 0; 
      m_object4 = 0; 
      m_nHash = 0; 
   }

public:
   // getting members
   const CClass1 *first() const { return m_object1; };
   const CClass2 *second() const { return m_object2; }
   const CClass3 *third() const { return m_object3; }
   const CClass4 *fourth() const { return m_object4; }

   // getting hash
   inline const unsigned long int &hash() const { return m_nHash; }

   // comparison methods
   bool operator == (const CTuple4 &w) const { 
      return *m_object1 == *(w.m_object1) && 
             *m_object2 == *(w.m_object2) &&
             *m_object3 == *(w.m_object3) &&
             *m_object4 == *(w.m_object4) ;
   }
   bool operator != (const CTuple4 &w) const {
      return ! ((*this) == w);
   }
   bool operator < (const CTuple4 &w) const { 
      return *m_object1<*(w.m_object1) || 
             (*m_object1==*(w.m_object1) && *m_object2<*(w.m_object2)) || 
             (*m_object1==*(w.m_object1) && *m_object2 == *(w.m_object2) && *m_object3<*(w.m_object3)) || 
             (*m_object1==*(w.m_object1) && *m_object2 == *(w.m_object2) && *m_object3==*(w.m_object3) && *m_object4<*(w.m_object4)); 
   }

   void operator = (const CTuple4 &w) {
      if (w.m_bAllocated) { 
         m_object1 = new CClass1(*(w.m_object1)); 
         m_object2 = new CClass2(*(w.m_object2)); 
         m_object3 = new CClass3(*(w.m_object3)); 
         m_object4 = new CClass4(*(w.m_object4)); 
         m_bAllocated = true; 
      } 
      else { 
         m_object1 = w.m_object1;
         m_object2 = w.m_object2;
         m_object3 = w.m_object3;
         m_object4 = w.m_object4;
         m_bAllocated = false;
      }
      m_nHash = w.m_nHash; 
   }

protected:
   inline void computehash() { 
      m_nHash = ( ( ::hash(*m_object1) * 31 + ::hash(*m_object2) ) * 29 + ::hash(*m_object3) ) * 23 + ::hash(*m_object4); 
   }
};

//===============================================================

//template<class CClass1, class CClass2, class CClass3, class CClass4>
//inline unsigned long int hash(const CTuple4<CClass1, CClass2, CClass3, CClass4> &w) { return w.hash(); }

//===============================================================

template <class CClass1, class CClass2, class CClass3, class CClass4>
std::istream & operator >> (std::istream &is, CTuple4<CClass1, CClass2, CClass3, CClass4> &tuple4) {
   char c, d, e ;
   CClass1 object1; 
   CClass2 object2;
   CClass3 object3;
   CClass4 object4;
   // one at a time because inlined (weird bug is they are separated sometimes)
   is >> object1; 
   is >> c; 
   is >> object2; 
   is >> d; 
   is >> object3; 
   is >> e; 
   is >> object4;
   tuple4.allocate(&object1, &object2, &object3, &object4) ;
   assert( c == ',' && d == ',' && e == ',') ;
   return is ;
}

template <class CClass1, class CClass2, class CClass3, class CClass4>
std::ostream & operator << (std::ostream &os, const CTuple4<CClass1, CClass2, CClass3, CClass4> &tuple4) {
   os << *(tuple4.first()) << " , " << *(tuple4.second()) << " , " << *(tuple4.third()) << " , " << *(tuple4.fourth());
   return os ;
}

#endif

