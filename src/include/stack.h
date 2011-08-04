// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                              *
 * stack.h - the stack                                          *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 ****************************************************************/

#ifndef _STACK_H
#define _STACK_H

#include "pool.h"

/*===============================================================
 *
 * Hash table
 *
 *==============================================================*/

template <typename V>
class CStack {

protected:
   enum{POOL_BLOCK_SIZE=(1<<16)};

protected:

   //===============================================================
   //
   // Hash table entry
   //
   //===============================================================

   class CEntry {
   public:
      V m_value;
      CEntry *m_next;

   public:
      CEntry() : m_value(), m_next(0) {}
      CEntry(&key, const V &value) : m_value(value), m_next(0){}
   };

public:

   //===============================================================
   //
   // Hash table iterator class
   //
   //===============================================================

   class iterator {

   private:
      CStack<V> *m_parent;
      CEntry *m_entry;

   public:
      iterator() {}
      iterator(CStack<V> *parent, CEntry *entry) : m_parent(parent), m_entry(entry) {}
      iterator(const iterator &it) : m_parent(it.m_parent), m_entry(it.m_entry) { }
      bool operator != (const iterator &it) const { return !((*this)==it);}
      bool operator == (const iterator &it) const { return m_parent == it.m_parent && m_entry == it.m_entry; }
      // move to next places
      void operator ++ () { 
         if (m_entry) m_entry=m_entry->m_next ;  
      }

      V &operator *() { return m_entry->m_value; }
   }; 

   //===============================================================
   //
   // Hash table iterator class
   //
   //===============================================================

   class const_iterator {

   private:
      const CStack<V> *m_parent;
      const CEntry *m_entry;

   public:
      const_iterator() {}
      const_iterator(const CStack<V> *parent, const CEntry *entry) : m_parent(parent), m_entry(entry) {}
      const_iterator(const const_iterator &it) : m_parent(it.m_parent), m_entry(it.m_entry) { }
      bool operator != (const const_iterator &it) const { return !((*this)==it);}
      bool operator == (const const_iterator &it) const { return m_parent == it.m_parent && m_entry == it.m_entry; }
      // move to next places
      void operator ++ () { 
         if(m_entry == 0) return;
         m_entry=m_entry->m_next ;  
      }

      const V &operator *() { return m_entry->m_value; }
   }; 

   //===============================================================

protected:
   CEntry* m_buckets;
public:
   CStack() : m_buckets(0) { }
      
   CStack(const CStack& o) { 
      (*this) = o;
   }
   virtual ~CStack() { 
      clear();
   }

protected:
   static CMemoryPool<CEntry> &getPool() { static CMemoryPool<CEntry> pool(POOL_BLOCK_SIZE); return pool; }
   static CEntry* &getFreeMemory() { static CEntry* c_free = 0; return c_free; }

public:
   CEntry *allocate() {
      static CEntry *retval;
      CEntry * &c_free = getFreeMemory();
      if (c_free) {
         retval = c_free;
         c_free = c_free->m_next;
         retval->m_next = 0;
         return retval;
      }
      else {
         return getPool().allocate();
      }
   }

public:
   void push(const V&val) {
      CEntry *entry = allocate();
      entry->m_value = val;
      entry->m_next = m_buckets;
      m_buckets = entry;
   }
   void pop(V&ret) const {
      static V empty;
      ret = m_buckets->m_value;
      CEntry *&c_free = getFreeMemory();
      CEntry *entry = m_buckets->m_next;
      m_buckets->m_next = c_free;
      m_buckets->m_value = empty;
      c_free = m_buckets;
      m_buckets = entry;
   }
   bool element (const V &value) const { 
      CEntry*entry=m_buckets; 
      while (entry) {
         if (entry->m_value == value)
            return true;
         else
            entry = entry->m_next;
      }
      return false;
   }
   void clear() {
      if (!m_buckets) return;
      CEntry *tail = m_buckets;
      static V empty;
      while (tail->m_next) {
         tail->m_value = empty;
         tail = tail->m_next;
      }
      tail->m_value = empty;
      CEntry* &c_free = getFreeMemory();
      tail->m_next = c_free;
      c_free = m_buckets;
      m_buckets = 0;
   }

public:
   iterator begin() { 
      return iterator(this, m_buckets); 
   }
   iterator end() { 
      return iterator(this, 0); 
   }
   const_iterator begin() const { 
      return const_iterator(this, m_buckets); 
   }
   const_iterator end() const { 
      return const_iterator(this, 0); 
   }

public:
   void operator = (const CStack& o) { 
      clear(); 
      CEntry *entry = m_buckets;
      assert(entry==0);
      const_iterator it = o.begin();
      while (it != o.end()) {
         if (m_buckets==0) {
            m_buckets = allocate();
            m_buckets->m_value = *it; 
            entry = m_buckets;
         }
         else {
            entry->m_next = allocate();
            entry->m_next->m_value = *it; 
            entry = entry->m_next;
         }
         ++it;
      }
   }

public:
   bool empty() const { return m_buckets==0; }

};

template <typename V>
std::istream & operator >> (std::istream &is, CStack<V> &score_map) {
   if (!is) return is ;
   static std::string s ;
   static V value;
   is >> s;
   ASSERT(s=="{"||s=="{}", "The stack does not start with {");
   if (s=="{}")
      return is;
   while (true) {
      is >> value;
      score_map.push(value);
      is >> s;
      ASSERT(s==","||s=="}", "The stack does not have a , or } after value: "<<score_map[key]);
      if (s=="}")
         return is;
   }
   THROW("stack.h the program should not have reached here.");
   return is ;
}

template <typename V>
std::ostream & operator << (std::ostream &os, const CStack<V> &score_map) {
   static CStack<V> buffer;
   os << "{";
   if (score_map.empty()) {
      os << "}"; // empty {}
      return os;
   }
   os << " "; // non-empty { a , b , c }
   buffer.clear();
   typename CStack<V>::const_iterator it = score_map.begin();
   while (it!=score_map.end()) {
      buffer.push(*it);
      ++it;
   }
   it = buffer.begin();
   while (it != buffer.end()) {
      if (it!=buffer.begin()) 
         os << " , ";
      os << *it;
      ++it;
   }
   os << " }";
   return os;
}

#endif
