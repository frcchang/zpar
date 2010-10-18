// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                              *
 * bitarray.h - a bit array class.                              *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2007.5                         *
 *                                                              *
 ****************************************************************/

#ifndef _BITARRAY_H
#define _BITARRAY_H

#include "assert.h"

/*===============================================================
 *
 * CBitArray - the bit array mapping index into bit
 *
 *==============================================================*/

class CBitArray {
   protected:
      char *m_array;
      unsigned long int m_size;
      int m_slots; // simply for convenience
   public:
      CBitArray(unsigned long int size) : m_size(size) {
         m_slots = size / 8;
         m_slots += size%8==0 ? 0 : 1;
         m_array = new char[m_slots];
         clear();
      }
      virtual ~CBitArray() {
         delete [] m_array;
      }
      void clear() {
         for (int i=0; i<m_slots; ++i)
            m_array[i] = 0;
      }
      void set(unsigned long int index) {
         assert(index<m_size);
         int slot = index / 8;
         int bit_in_slot = index % 8;
         m_array[slot] |= (1 << bit_in_slot);
      }
      void unset(unsigned long int index) {
         assert(index<m_size);
         int slot = index / 8;
         int bit_in_slot = index % 8;
         m_array[slot] &= ~(static_cast<char>(1 << bit_in_slot));
      }
      void flip(unsigned long int index) {
         assert(index<m_size);
         int slot = index / 8;
         int bit_in_slot = index % 8;
         m_array[slot] ^= (1 << bit_in_slot);
      }
      bool isset(unsigned long int index) const {
         assert(index<m_size);
         int slot = index / 8;
         int bit_in_slot = index % 8;
         return (m_array[slot] & (1 << bit_in_slot)) != 0;
      }
};

#endif
