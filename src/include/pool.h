// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                              *
 * pool.h - the pool                                            *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 ****************************************************************/

#ifndef _MEMORY_POOL_H
#define _MEMORY_POOL_H

/*===============================================================
 *
 * Memory pool entry
 *
 *==============================================================*/

template <typename T, unsigned BLOCK_SIZE>
class CMemoryPoolEntry {
public:
   T blocks[BLOCK_SIZE];
   CMemoryPoolEntry *prev;
};
/*===============================================================
 *
 * Memory pool
 *
 *==============================================================*/

template <typename T, unsigned BLOCK_SIZE>
class CMemoryPool {
protected:
   CMemoryPoolEntry<T, BLOCK_SIZE> *current;
   unsigned long nItem;
public:
   CMemoryPool(): current(0), nItem(0) {
   }
   virtual ~CMemoryPool() {
      reset();
   }
protected:
   void newblock() {
      CMemoryPoolEntry<T, BLOCK_SIZE> *iter = current;
      current = new CMemoryPoolEntry<T, BLOCK_SIZE>();
      assert(current!=0);
      current->prev = iter;
      nItem = 0;
   }
public:
   T* allocate() { 
      if (current==0 || nItem==BLOCK_SIZE) { // if start or block full
         newblock();
      }
      return &(current->blocks[nItem++]) ;
   }
   void reset() {
      CMemoryPoolEntry<T, BLOCK_SIZE> *iter = current;
      while (iter) {
         CMemoryPoolEntry<T, BLOCK_SIZE> *prev = iter->prev;
         delete iter;
         iter = prev;
      }
      current=0;
   }
};

#endif
