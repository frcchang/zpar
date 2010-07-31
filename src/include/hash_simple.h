/****************************************************************
 *                                                              *
 * hash.h - the hash table                                      *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2006.11                        *
 *                                                              *
 ****************************************************************/

#ifndef _HASH_H
#define _HASH_H

#include "pool.h"

static const unsigned long DEFAULT_SIZE

static const unsigned long POOL_BLOCK_SIZE=1<<16;

/*===============================================================
 *
 * Hash table
 *
 *==============================================================*/

template <typename K, typename V>
class CHashMap {

protected:
   const unsigned long int m_nTableSize;

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
      CEntry(const K &key) : m_key(key), m_next(0), {}
      CEntry(const K &key, const V &value) : m_key(key), m_value(value), m_next(0){}
      virtual ~CEntry() {if(m_next) delete m_next;}
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
      CHashMap<K, V> *m_parent;
      CEntry *m_entry;

   public:
      iterator() {}
      iterator(CHashMap<K, V> *parent, int bucket, CEntry *entry) {
         m_parent = parent; 
         m_nBucket = bucket; 
         m_entry = entry; 
      }
      iterator(const iterator &it) { m_parent = it.m_parent; m_nBucket = it.m_nBucket; m_entry = it.m_entry; }
      bool operator != (const iterator &it) const { return !((*this)==it);}
      bool operator == (const iterator &it) const { return m_parent == it.m_parent && m_nBucket == it.m_nBucket && m_entry == it.m_entry; }
      // move to next places
      void operator ++ () { 
         if (m_entry == 0) { 
            if (m_nBucket >= m_parent->m_nTableSize-1) 
               return; 
            else 
               m_entry = m_parent->m_buckets[++m_nBucket]; 
         }
         else {
            m_entry=m_entry->m_next ;  
         }
         // when the next item is at the end of the bucket, move on
         while (m_entry == 0) { 
            if (m_nBucket >= m_parent->m_nTableSize-1) 
               return; 
            else { 
               m_entry = m_parent->m_buckets[++m_nBucket]; 
               continue; 
            } 
         } 
      }
      bool valid() const { if (m_nBucket < 0 || m_nBucket > m_parent->m_nTableSize-1 || m_entry == 0) return false; return true; }

      const K &first() { return m_entry->m_key; }
      V &second() { return m_entry->m_value; }
   }; 

   //===============================================================

protected:
   CEntry **m_buckets;
   CMemoryPool &getPool() const {static CMemoryPool<CEntry> pool(POOL_BLOCK_SIZE); return pool;}
public:
   CHashMap(unsigned long TABLE_SIZE=DEFAULT_SIZE) : m_nTableSize(TABLE_SIZE) { 
      m_buckets = new CEntry*[TABLE_SIZE] ;
      for (int i=0; i<TABLE_SIZE; ++i) 
         m_buckets[i]=0;
   }
   CHashMap(const CHashMap<K, V>& wordmap) { 
      cerr << "CHashMap does not support copy constructor!"; 
      assert(1==0);
   }
   virtual ~CHashMap() { 
      for (int i=0; i<m_nTableSize; ++i) if (m_buckets[i])delete m_buckets[i]; 
      delete [] m_buckets;
   }

protected:
   CEntry *&getEntry(const K &key) { return m_buckets[hash(key)%m_nTableSize]; }

public:
   V &operator[] (const K &key) { 
      CEntry* entry = getEntry(key); 
      if (entry==0) {
         entry = getEntry(key) = getPool().allocate(); 
         entry->m_key = key;
         return entry->m_value;
      }
      return entry->find(key, true);
   }
   void insert (const K &key, const V &val) { (*this)[key] = val; }
   const V &find (const K &key, const V &val) const { 
      CEntry*entry=getEntry(key); 
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
      iterator it = iterator(this, 0, m_buckets[0]); 
      if (!it.valid()) 
         ++it;  
      return it; 
   }
   iterator end() { 
      return iterator(this, m_nTableSize-1, 0); 
   }

public:
#ifdef DEBUG 
   void trace() { for (int i=0; i<m_nTableSize; ++i) cout<<m_buckets[i].size()<<' ';}
#endif

   friend istream & operator >> <> (istream &is, CHashMap &table) ;
   friend ostream & operator << <> (ostream &os, CHashMap &table) ;

};


#endif
