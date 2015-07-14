// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                              *
 * hash_combine.h - the hash table                              *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2006.11                        *
 *                                                              *
 ****************************************************************/

#ifndef _HASH_H
#define _HASH_H

static const unsigned long DEFAULT_SIZE = 65537;

template <typename K, typename V>
class CHashMap {

protected:
   unsigned long int m_nTableSize ;

public:

   //===============================================================
   //
   // Hash table iterator class
   //
   //===============================================================

   class iterator {

   private:
      unsigned long m_nBucket;
      CHashMap<K, V> *m_parent;
      typename map<K, V>::iterator m_it;

   public:
      iterator() {}
      iterator(CHashMap<K, V> *parent, const unsigned long &bucket, const typename std::map<K, V>::iterator &it) {
         m_parent = parent;
         m_nBucket = bucket;
         m_it=it;
      }
      iterator(const iterator &it) { m_parent = it.m_parent; m_nBucket = it.m_nBucket; m_it = it.m_it; }

      bool operator != (const iterator &it) const { return !((*this)==it);}
      bool operator == (const iterator &it) const {
         return m_parent == it.m_parent && m_nBucket == it.m_nBucket && m_it == it.m_it;
      }

      // move to next places
      void operator ++ () {
         // move on
         // if at the end of a bucket
         if (m_it == m_parent->m_mapBuckets[m_nBucket].end()) {
            // if the end of the last bucket, don't move
            if (m_nBucket >= m_parent->m_nTableSize-1)
               return;
            else
               m_it = m_parent->m_mapBuckets[++m_nBucket].begin();
         }
         else {
            ++m_it ;
         }
         // when the next item is at the end of the bucket, move on
         while (m_it == m_parent->m_mapBuckets[m_nBucket].end()) {
            if (m_nBucket >= m_parent->m_nTableSize-1)
               return;
            else {
               m_it = m_parent->m_mapBuckets[++m_nBucket].begin();
               continue;
            }
         }
      }

      bool valid() const {
         if (m_nBucket < 0 || m_nBucket > m_parent->m_nTableSize-1 || m_it == m_parent->m_mapBuckets[m_nBucket].end())
            return false;
         return true;
      }

      const K &first() { return m_it->first; }
      V &second() { return m_it->second; }
   };

   //===============================================================

protected:
   std::map<K, V> *m_mapBuckets;

   //===============================================================

public:
   CHashMap(unsigned long TABLE_SIZE = DEFAULT_SIZE, bool bInit=true) : m_nTableSize(TABLE_SIZE), m_mapBuckets(0) {
      if (bInit) init();
   }
   CHashMap(const CHashMap<K, V>& wordmap) : m_nTableSize(0) {
      std::cerr << "CHashMap does not support copy constructor!";
      assert(1==0);
   }
   virtual ~CHashMap() { delete[] m_mapBuckets;}

   void resize(const unsigned &size) {
      ASSERT(m_mapBuckets==0, "Cannot resize table after init");
      m_nTableSize = size;
   }

   void init() {
      ASSERT(m_mapBuckets==0, "Cannot reinit table");
      m_mapBuckets = new std::map<K, V>[m_nTableSize];
   }

   V &operator[] (const K &key) { return m_mapBuckets[hash(key)%m_nTableSize][key]; }

   void insert (const K &key, const V &val) { m_mapBuckets[hash(key)%m_nTableSize].insert(std::make_pair(key, val)); }
   void insert (const std::pair<K,V> &par) { m_mapBuckets[hash(par.first)%m_nTableSize].insert(par); }

   const V &find (const K &key, const V &val) const {
      std::map<K, V>&m=m_mapBuckets[hash(key)%m_nTableSize];
      typename std::map<K,V>::iterator it=m.find(key);
      if (it != m.end())
         return it->second;
      else return val;
   }

   bool findorinsert (const K &key, const V &val, V &retvalue) {
      std::map<K, V>&m=m_mapBuckets[hash(key)%m_nTableSize];
      typename std::map<K,V>::iterator it=m.find(key);
      if (it != m.end()) {
         retvalue = it->second;
         return false;
      } else {
         m.insert(it, std::make_pair(key, val));
         retvalue = val;
         return true;
      }
   }

   bool element (const K &key) const {
      std::map<K, V>&m=m_mapBuckets[hash(key)%m_nTableSize];
      typename std::map<K,V>::iterator it=m.find(key);
      if(it == m.end())
         return false;
      return true;
   }

   void erase (iterator &it) {
      m_mapBuckets[it.m_nBucket].erase(it.m_it);
   }

   iterator begin() { iterator it = iterator(this, 0, m_mapBuckets[0].begin()); if (!it.valid()) ++it;  return it; }
   iterator end() { return iterator(this, m_nTableSize-1, m_mapBuckets[m_nTableSize-1].end()); }

   //===============================================================

public:

#ifdef DEBUG
   void trace() { for (unsigned i=0; i<m_nTableSize; ++i) std::cerr<<m_mapBuckets[i].size()<<' ';}
#endif

};

#endif

