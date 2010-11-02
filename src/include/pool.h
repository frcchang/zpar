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
   CMemoryPool(): nItem(0), current(0) {
      newblock();
      assert(current!=0);
   }
   virtual ~CMemoryPool() {
      CMemoryPoolEntry<T, BLOCK_SIZE> *iter = current;
      while (iter) {
         CMemoryPoolEntry<T, BLOCK_SIZE> *prev = iter->prev;
//         if (BLOCK_INIT) delete iter; else std::free(iter);
         delete iter;
         iter = prev;
      }
   }
protected:
   void newblock() {
      CMemoryPoolEntry<T, BLOCK_SIZE> *iter = current;
//      if (BLOCK_INIT) {
         current = new CMemoryPoolEntry<T, BLOCK_SIZE>();
//      }
//      else {
//         current = static_cast<CMemoryPoolEntry<T, BLOCK_SIZE>*>(std::malloc(sizeof(CMemoryPoolEntry<T, BLOCK_SIZE>)));
//      }
      assert(current!=0);
      current->prev = iter;
   }
public:
   T* allocate() { 
      assert(current!=0);
      if (nItem==BLOCK_SIZE) {
         newblock();
         nItem = 0;
      }
      return &(current->blocks[nItem++]) ;
   }
};

#endif
