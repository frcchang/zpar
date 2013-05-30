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
      CBitArray(unsigned long int capacity = 0) : m_slots(0), m_size(0), m_array(0) {
         if (capacity) init(capacity);
      }
      CBitArray(const CBitArray &a) : m_slots(0), m_size(0), m_array(0) {
         setsize(a.m_size);
         if (m_array && a.m_array)
            std::memcpy(m_array, a.m_array, m_slots);
      }
      virtual ~CBitArray() {
         if (m_array) delete [] m_array;
      }
      void clear() {
         std::memset(m_array, 0, m_slots);
         m_size=0;
      }
      void set(const unsigned long int &index) {
         assert(index<m_size);
         int slot = index / 8;
         int bit_in_slot = index % 8;
         m_array[slot] |= (1 << bit_in_slot);
      }
      void unset(const unsigned long int &index) {
         assert(index<m_size);
         int slot = index / 8;
         int bit_in_slot = index % 8;
         m_array[slot] &= ~(static_cast<char>(1 << bit_in_slot));
      }
      void flip(const unsigned long int &index) {
         assert(index<m_size);
         int slot = index / 8;
         int bit_in_slot = index % 8;
         m_array[slot] ^= (1 << bit_in_slot);
      }
      bool isset(const unsigned long int &index) const {
         assert(index<m_size);
         int slot = index / 8;
         int bit_in_slot = index % 8;
         return (m_array[slot] & (1 << bit_in_slot)) != 0;
      }
      const unsigned long int &size() const {
         return m_size;
      }
      void init(const unsigned long int &capacity) {
         if (m_array)  {
            delete [] m_array;
            m_array = 0;
         }
         m_slots = capacity / 8;
         m_slots += capacity%8==0 ? 0 : 1;
         if (capacity)
            m_array = new char[m_slots];
         clear();
      }
      void setsize(const unsigned long int &size) {
         if (size > m_slots*8) {
            char *tmp = m_array;
            int oldslots = m_slots;
            m_array = 0;
            init(size);
            if (tmp) {
               std::memcpy(m_array, tmp, oldslots);
               delete [] tmp;
            }
         }
         else if (size>m_size) {
            std::memset(m_array, 0, size-m_size);
         }
         m_size = size;
      }
      void add(const bool &s) {
         if (m_size == m_slots * 8 - 1) {
            char *tmp = m_array;
            int oldsize = m_size;
            m_array = 0;
            init(m_slots*8*2);
            std::memcpy(m_array, tmp, m_slots*8);
            m_size = oldsize;
            delete [] tmp;
         }
         if (s)
            set(m_size++);
         else
            unset(m_size++);
         assert(m_size/8 <= m_slots);
      }
      void add(const CBitArray &a) {
         for (unsigned long int i = 0; i<a.m_size; ++i)
            add(a.isset(i));
      }
      void add(int n, const int &size) {
         assert(n < (1<<size));
         while (n) {
            add(n%2);
            n >>= 1;
         }
      }
      CBitArray &operator = (const CBitArray &a) {
         setsize(a.m_size);
         if (m_array && a.m_array)
            std::memcpy(m_array, a.m_array, m_slots);
      }
      operator std::string () const {
         std::string retval = "";
         for (unsigned long int i=0; i<m_size; ++i)
            //os << (ba.isset(i)?'1':'0');
            retval += isset(i)  ? '1' : '0';
         return retval;
      }
};

inline
std::istream & operator >> (std::istream &is, CBitArray &ba) {
   std::string s;
   is >> s;
   ba.init(s.size());
   ba.setsize(0);
   for (unsigned long int i=0; i<s.size(); ++i)
      ba.add(s[i]=='1');
   return is;
}

inline
std::ostream & operator << (std::ostream &os, CBitArray &ba) {
   os << static_cast<std::string>(ba);
   return os;
}

#endif
