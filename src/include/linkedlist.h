// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                              *
 * linkedlist.h - the linked list                               *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 ****************************************************************/

#ifndef _LINKED_LIST_H
#define _LINKED_LIST_H

#include "pool.h"

/*===============================================================
 *
 * Hash table
 *
 *==============================================================*/

template <typename K, typename V>
class CLinkedList {

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
      K m_key;
      V m_value;
      CEntry *m_next;

   public:
      CEntry() : m_key(), m_value(), m_next(0) {}
      CEntry(const K &key) : m_key(key), m_value(), m_next(0) {}
      CEntry(const K &key, const V &value) : m_key(key), m_value(value), m_next(0){}
   };

public:

   //===============================================================
   //
   // Hash table iterator class
   //
   //===============================================================

   class iterator {

   private:
      CLinkedList<K, V> *m_parent;
      CEntry *m_entry;

   public:
      iterator() {}
      iterator(CLinkedList<K, V> *parent, CEntry *entry) : m_parent(parent), m_entry(entry) {}
      iterator(const iterator &it) : m_parent(it.m_parent), m_entry(it.m_entry) { }
      bool operator != (const iterator &it) const { return !((*this)==it);}
      bool operator == (const iterator &it) const { return m_parent == it.m_parent && m_entry == it.m_entry; }
      // move to next places
      void operator ++ () { 
         if (m_entry) m_entry=m_entry->m_next ;  
      }

      const K &first() { return m_entry->m_key; }
      V &second() { return m_entry->m_value; }
   }; 

   //===============================================================
   //
   // Hash table iterator class
   //
   //===============================================================

   class const_iterator {

   private:
      const CLinkedList<K, V> *m_parent;
      const CEntry *m_entry;

   public:
      const_iterator() {}
      const_iterator(const CLinkedList<K, V> *parent, const CEntry *entry) : m_parent(parent), m_entry(entry) {}
      const_iterator(const const_iterator &it) : m_parent(it.m_parent), m_entry(it.m_entry) { }
      bool operator != (const const_iterator &it) const { return !((*this)==it);}
      bool operator == (const const_iterator &it) const { return m_parent == it.m_parent && m_entry == it.m_entry; }
      // move to next places
      void operator ++ () { 
         if(m_entry == 0) return;
         m_entry=m_entry->m_next ;  
      }

      const K &first() { return m_entry->m_key; }
      const V &second() { return m_entry->m_value; }
   }; 

   //===============================================================

protected:
   CEntry* m_buckets;
public:
   CLinkedList() : m_buckets(0) { 
      getPool(); // ensure that the pool is constructed.
   }
      
   CLinkedList(const CLinkedList& o) { 
      ASSERT(o.m_buckets==0, "CLinkedList does not support copy constructor unless copying from an empty one.");
      getPool(); // ensure that the pool is constructed.
      clear(); 
   }
   virtual ~CLinkedList() { 
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
   V &operator[] (const K &key) { 
      CEntry* entry = m_buckets; 
      if (entry==0) {
         entry = m_buckets = allocate(); 
         entry->m_key = key;
         return entry->m_value;
      }
      while (true) {
         if (entry->m_key==key)
            return entry->m_value;
         else {
            if (entry->m_next==0)
               break;
            else
               entry = entry->m_next;
         }
      }
      entry->m_next = allocate();
      entry->m_next->m_key = key;   
      return entry->m_next->m_value;
   }
   void insert (const K &key, const V &val) { (*this)[key] = val; }
   const V &find (const K &key, const V &val) const { 
      const CEntry*entry=m_buckets; 
      while (entry) {
         if (entry->m_key == key)
            return entry->m_value;
         else
            entry = entry->m_next;
      }
      return val;
   }
   bool findorinsert (const K &key, const V &val, V &retval) { 
      CEntry*entry=m_buckets; 
      if (entry == 0) { 
         retval = val; 
         entry= m_buckets =allocate(); 
         entry->m_key = key;
         entry->m_value = val; 
         return true; 
       } 
       while (true) {
          assert (entry);
          if (entry->m_key == key) {
             retval = entry->m_value;
             return false;
          }
          else if (entry->m_next==0) 
             break;
          else
             entry = entry->m_next;
       }
       assert(entry);
       entry->m_next = allocate();
       entry->m_next->m_key = key;
       entry->m_next->m_value = val;
       retval = val;
       return true;
   }
   bool element (const K &key) const { 
      CEntry*entry=m_buckets; 
      while (entry) {
         if (entry->m_key == key)
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
   void operator = (const CLinkedList& o) { 
      ASSERT(o.m_buckets==0, "CLinkedList does not support copy constructor unless copying from an empty one.");
      clear(); 
   }

public:
   bool empty() const { return m_buckets==0; }

//public:
//   static void freePoolMemory() { // call after all instances clean!
//      getPool().reset();
//      getFreeMemory() = 0;
//   }

};

template <typename K, typename V>
std::istream & operator >> (std::istream &is, CLinkedList<K, V> &score_map) {
   if (!is) return is ;
   static std::string s ;
   static K key;
//   static V value;
//   assert(score_map.empty());
   is >> s;
   ASSERT(s=="{"||s=="{}", "The small hashmap does not start with {");
   if (s=="{}")
      return is;
   while (true) {
      is >> key;
      is >> s;
      ASSERT(s==":", "The small hashmap does not have : after key: "<<key);
//      is >> value;
//      score_map[key] = value;
      is >> score_map[key];
      is >> s;
      ASSERT(s==","||s=="}", "The small hashmap does not have a , or } after value: "<<score_map[key]);
      if (s=="}")
         return is;
   }
   THROW("hashmap_small.h: the program should not have reached here.");
   return is ;
}

template <typename K, typename V>
std::ostream & operator << (std::ostream &os, const CLinkedList<K, V> &score_map) {
   os << "{";
   typename CLinkedList<K, V>::const_iterator it = score_map.begin();
   if (it==score_map.end()) {
      os << "}"; // empty {}
      return os;
   }
   else
      os << " "; // non-empty { a , b , c }
   while (it!=score_map.end()) {
      if (it!=score_map.begin()) 
         os << " , ";
      os << it.first() << " : " << it.second();
      ++it;
   }
   os << " }";
   return os;
}

#endif
