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

static const unsigned long DEFAULT_SIZE

/*===============================================================
 *
 * Hash table
 *
 *==============================================================*/

template <typename K, typename V>
class CHashMap {

protected:
   const unsigned long int m_nTableSize;

public:

   class CEntryNotFoundException { string what() {return "Entry not found when looking up hash map.";} };

protected:

   //===============================================================
   //
   // Hash table entry
   //
   //===============================================================

   class CEntry {

   protected:
      K m_key;
      V m_value;
      CEntry *m_prev;
      CEntry *m_next;
      CEntryNotFoundException *CENF;

   public:
      CEntry(CEntry *prev, const K &key) : m_key(key) , CENF(new CEntryNotFoundException) {m_prev=prev; m_next=0;}
      CEntry(CEntry *prev, const K &key, const V &value) : m_key(key) , m_value(value) , CENF(new CEntryNotFoundException) {m_prev=prev; m_next=0;}
      void unlink() { if (m_prev) m_prev->m_next = m_next; if (m_next) m_next->m_prev = m_prev; m_prev = 0; m_next = 0; }
      virtual ~CEntry() {if(m_next) delete m_next;delete CENF;}

      V &find(const K &key, bool bInsert) {if (m_key==key) return m_value; if (m_next==0) { if (bInsert) return insert(key); else throw CENF; } return m_next->find(key, bInsert); }
      V &insert(const K &key) {CEntry *entry=new CEntry(this, key); m_next=entry; return entry->m_value;}
      bool tryinsert(const K &key, const V &val, V &retval) {if (m_key==key) {retval = m_value; return false;} if (m_next==0) { insert(key) = val; retval = val; return true; } return m_next->tryinsert(key, val, retval); }

      K &key() {return m_key;}
      V &value() {return m_value;}

      CEntry *next() {return m_next;}
   };
   //
   // Member variables
   //
   CEntry **m_buckets;

public:

   //===============================================================
   //
   // Hash table iterator class
   //
   //===============================================================

   class iterator {

   private:
      unsigned int m_nBucket;
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
            m_entry=m_entry->next() ;  
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
      void erase (iterator &it) {
         m_mapBuckets[it.m_nBucket].erase(it.m_it);
      }

      const K &first() { return m_entry->key(); }
      V &second() { return m_entry->value(); }
   }; 

   //===============================================================

public:
   const iterator m_end;
   CHashMap(unsigned long TABLE_SIZE=DEFAULT_SIZE) : m_nTableSize(TABLE_SIZE) , m_end(this, TABLE_SIZE-1, 0) { 
      m_buckets = new CEntry*[TABLE_SIZE] ;
      for (int i=0; i<TABLE_SIZE; i++) 
         m_buckets[i]=0;
   }
   CHashMap(const CHashMap<K, V>& wordmap) { 
      cerr << "CHashMap does not support copy constructor!"; 
      assert(1==0);
   }
   virtual ~CHashMap() { 
      for (int i=0; i<m_nTableSize; i++) if (m_buckets[i])delete m_buckets[i]; 
      delete [] m_buckets;
   }
   V &operator[] (const K &key) { 
      CEntry* entry = m_buckets[hash(key)%m_nTableSize]; 
      if (entry==0) {
         entry = m_buckets[hash(key)%m_nTableSize] = new CEntry(0, key); 
         return entry->value();
      }
      return entry->find(key, true);
   }
   void insert (const K &key, const V &val) { (*this)[key] = val; }
   const V &find (const K &key, const V &val) const { CEntry*entry=m_buckets[hash(key)%m_nTableSize]; try { if (entry == 0) return val; else return entry->find(key, false);} catch (CEntryNotFoundException *t) {return val;}; }
   bool findorinsert (const K &key, const V &val, V &retval) { CEntry*entry=m_buckets[hash(key)%m_nTableSize]; if (entry == 0) { retval = val; entry=m_buckets[hash(key)%m_nTableSize]=new CEntry(0, key); entry->value() = val; return true; } else return entry->tryinsert(key, val, retval); }
   bool element (const K &key) const { CEntry*entry=m_buckets[hash(key)%m_nTableSize]; if (entry == 0) return false; try {return entry->find(key, false);} catch (CEntryNotFoundException *t) {return false;}}
   iterator begin() { iterator it = iterator(this, 0, m_buckets[0]); if (!it.valid()) ++it;  return it; }
   iterator end() { return iterator(this, m_nTableSize-1, 0); }

   void erase(iterator &it) {
      it->m_entry->unlink();
      delete it->m_entry;
   }

public:
#ifdef DEBUG 
   void trace() { for (int i=0; i<m_nTableSize; i++) cout<<m_buckets[i].size()<<' ';}
#endif

   friend istream & operator >> <> (istream &is, CHashMap &table) ;
   friend ostream & operator << <> (ostream &os, CHashMap &table) ;

};


#endif
