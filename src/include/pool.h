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
   vector<const T*> blocks;
   unsigned nItem;
   unsigned block_size;
public:
   CMemoryPool(const unsigned &block_size): block_size(block_size), nItem(0) {
      T* block = new T[block_size];
      blocks.push_back(block);
   }
   virtual ~CMemoryPool() {
      for (unsigned i=0; i<blocks.size(); ++i)
         delete blocks[i];
      }
   }
public:
   T* allocate() { 
      if (nItem==block_size) {
         T* block = new T[block_size];
         blocks.push_back(block);
         nItem = 0;
      }
      return blocks.back()[nItem++];
   }
};

#endif
