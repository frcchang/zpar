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
 * Memory pool
 *
 *==============================================================*/

template <typename T>
class CMemoryPool {
protected:
   vector<T*> blocks;
   unsigned long nItem;
   unsigned long block_size;
public:
   CMemoryPool(const unsigned long &block_size): block_size(block_size), nItem(0) {
      newblock();
   }
   virtual ~CMemoryPool() {
      for (unsigned i=0; i<blocks.size(); ++i) { delete [] (blocks[i]); }
   }
protected:
   void newblock() {
      T* block = new T[block_size];
      blocks.push_back(block);
   }
public:
   T* allocate() { 
      if (nItem==block_size) {
         newblock();
         nItem = 0;
      }
      ++nItem;
      return blocks.back() + nItem - 1 ;
   }
};

#endif
