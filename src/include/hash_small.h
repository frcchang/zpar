// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                              *
 * hash.h - the hash table                                      *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2006.11                        *
 *                                                              *
 ****************************************************************/

#ifndef _HASH_SMALL_H
#define _HASH_SMALL_H

#include "pool.h"

/*===============================================================
 *
 * Hash table
 *
 *==============================================================*/

template <typename K, typename V, unsigned TABLE_SIZE>
class CSmallHashMap {

protected:
   static const unsigned long POOL_BLOCK_SIZE=(1<<16);

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
      unsigned long int m_nBucket;
      CSmallHashMap<K, V, TABLE_SIZE> *m_parent;
      CEntry *m_entry;

   private:
      void validate() {
         // when the next item is at the end of the bucket, move on
         assert(m_nBucket < TABLE_SIZE);
         while (m_entry == 0) {
            if (m_nBucket == TABLE_SIZE-1)
               return;
            else {
               m_entry = m_parent->m_buckets[++m_nBucket];
               continue;
            }
         }
      }

   public:
      iterator() {}
      iterator(CSmallHashMap<K, V, TABLE_SIZE> *parent, int bucket, CEntry *entry) : m_nBucket(bucket), m_parent(parent), m_entry(entry) {
         validate();
      }
      iterator(const iterator &it) { m_parent = it.m_parent; m_nBucket = it.m_nBucket; m_entry = it.m_entry; }
      bool operator != (const iterator &it) const { return !((*this)==it);}
      bool operator == (const iterator &it) const { return m_parent == it.m_parent && m_nBucket == it.m_nBucket && m_entry == it.m_entry; }
      // move to next places
      void operator ++ () {
         assert(m_entry != 0);
         m_entry=m_entry->m_next ;
         validate();
      }
      bool valid() const { if (m_nBucket < 0 || m_nBucket > TABLE_SIZE-1 || m_entry == 0) return false; return true; }

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
      unsigned long int m_nBucket;
      const CSmallHashMap<K, V, TABLE_SIZE> *m_parent;
      const CEntry *m_entry;

   private:
      void validate() {
         // when the next item is at the end of the bucket, move on
         assert(m_nBucket < TABLE_SIZE);
         while (m_entry == 0) {
            if (m_nBucket == TABLE_SIZE-1)
               return;
            else {
               m_entry = m_parent->m_buckets[++m_nBucket];
               continue;
            }
         }
      }

   public:
      const_iterator() {}
      const_iterator(const CSmallHashMap<K, V, TABLE_SIZE> *parent, int bucket, CEntry *entry) : m_nBucket(bucket), m_parent(parent), m_entry(entry) { validate(); }
      const_iterator(const const_iterator &it) { m_parent = it.m_parent; m_nBucket = it.m_nBucket; m_entry = it.m_entry; }
      bool operator != (const const_iterator &it) const { return !((*this)==it);}
      bool operator == (const const_iterator &it) const { return m_parent == it.m_parent && m_nBucket == it.m_nBucket && m_entry == it.m_entry; }
      // move to next places
      void operator ++ () {
         assert(m_entry != 0);
         m_entry=m_entry->m_next ;
         validate();
      }
      bool valid() const { if (m_nBucket < 0 || m_nBucket > TABLE_SIZE-1 || m_entry == 0) return false; return true; }

      const K &first() { return m_entry->m_key; }
      const V &second() { return m_entry->m_value; }
   };

   //===============================================================

protected:
   CEntry* m_buckets[TABLE_SIZE];
public:
   CSmallHashMap() { memset(m_buckets, 0, TABLE_SIZE*sizeof(CEntry*)); }

   CSmallHashMap(const CSmallHashMap& wordmap) {
      THROW("CSmallHashMap does not support copy constructor!");
   }
   virtual ~CSmallHashMap() { }

protected:
   CEntry *&getEntry(const K &key) { return m_buckets[hash(key)%TABLE_SIZE]; }
   CEntry * const &getEntry(const K &key) const { return m_buckets[hash(key)%TABLE_SIZE]; }

   static CMemoryPool<CEntry> &getPool() { static CMemoryPool<CEntry> pool(POOL_BLOCK_SIZE); return pool; }

public:
   V &operator[] (const K &key) {
      CEntry* entry = getEntry(key);
      if (entry==0) {
         entry = getEntry(key) = getPool().allocate();
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
      entry->m_next = getPool().allocate();
      entry->m_next->m_key = key;
      return entry->m_next->m_value;
   }
   void insert (const K &key, const V &val) { (*this)[key] = val; }
   const V &find (const K &key, const V &val) const {
      const CEntry*entry=getEntry(key);
      while (entry) {
         if (entry->m_key == key)
            return entry->m_value;
         else
            entry = entry->m_next;
      }
      return val;
   }
   bool findorinsert (const K &key, const V &val, V &retval) {
      CEntry*entry=getEntry(key);
      if (entry == 0) {
         retval = val;
         entry= getEntry(key) =getPool().allocate();
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
       entry->m_next = getPool().allocate();
       entry->m_next->m_key = key;
       entry->m_next->m_value = val;
       retval = val;
       return true;
   }
   bool element (const K &key) const {
      CEntry*entry=getEntry(key);
      while (entry) {
         if (entry->m_key == key)
            return true;
         else
            entry = entry->m_next;
      }
      return false;
   }

public:
   iterator begin() {
      return iterator(this, 0, m_buckets[0]);
   }
   iterator end() {
      return iterator(this, TABLE_SIZE-1, 0);
   }
   const_iterator begin() const {
      return const_iterator(this, 0, m_buckets[0]);
   }
   const_iterator end() const {
      return const_iterator(this, TABLE_SIZE-1, 0);
   }

public:
   void operator = (const CSmallHashMap& wordmap) {
      THROW("CSmallHashMap does not support copy constructor!");
   }

public:
   bool empty() const { for (unsigned i=0; i<TABLE_SIZE; ++i) if (getEntry(i)) return false; return true;}
#ifdef DEBUG
   void trace() {
      std::cerr << "tracing size:amount" << std::endl;
      std::map<unsigned, unsigned> statistic;
      for (unsigned i=0; i<TABLE_SIZE; ++i) {
         unsigned size = 0;
         CEntry* entry = m_buckets[i];
         while (entry) {
            ++size;
            entry=entry->m_next;
         }
         ++statistic[size];
      }
      std::map<unsigned, unsigned>::iterator it;
      for (it=statistic.begin(); it!=statistic.end(); ++it)
         if (it->second != 0)
            std::cerr << it->first << ':' << it->second << " (" << float(it->second)/TABLE_SIZE << ")" << std::endl;
      std::cerr << "done" << std::endl;
   }
#endif

public:
   static void freePoolMemory() { // call after all instances clean!
      getPool().reset();
   }

};

template <typename K, typename V, unsigned TABLE_SIZE>
std::istream & operator >> (std::istream &is, CSmallHashMap<K, V, TABLE_SIZE> &score_map) {
   if (!is) return is ;
   static std::string s ;
   static K key;
   static V value;
   assert(score_map.empty());
   is >> s;
   ASSERT(s=="{"||s=="{}", "The small hashmap does not start with {");
   if (s=="{}")
      return is;
   while (true) {
      is >> key;
      is >> s;
      ASSERT(s==":", "The small hashmap does not have : after key: "<<key);
      is >> value;
      score_map[key] = value;
      is >> s;
      ASSERT(s==","||s=="}", "The small hashmap does not have a , or } after value: "<<value);
      if (s=="}")
         return is;
   }
   THROW("hashmap_small.h: the program should not have reached here.");
   return is ;
}

template <typename K, typename V, unsigned TABLE_SIZE>
std::ostream & operator << (std::ostream &os, const CSmallHashMap<K, V, TABLE_SIZE> &score_map) {
   os << "{";
   typename CSmallHashMap<K, V, TABLE_SIZE>::const_iterator it = score_map.begin();
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
