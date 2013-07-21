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

#include <algorithm>
#include "assert.h"
#include <cstring>

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
      bool m_allocated;
   public:
      CBitArray(unsigned long int capacity = 0) : m_slots(0), m_size(0), m_array(0), m_allocated(false) {
         // If you want to allocate bitarray, specify non zero!
         if (capacity) init(capacity);
      }
      CBitArray(const CBitArray &a) : m_slots(0), m_size(0), m_array(0), m_allocated(false) {
         (*this) = a;
      }
      virtual ~CBitArray() {
         if (m_array && m_allocated) delete [] m_array;
      }
      void clear() {
         assert(m_allocated);
         std::memset(m_array, 0, m_slots);
         m_size=0;
      }
      
      void clearandsize(const unsigned long int& size) {
    	  assert(m_allocated);
          std::memset(m_array, 0, m_slots);
          m_size=size;
      }
      
      void set(const unsigned long int &index) {
         assert(m_allocated);
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
//         TRACE("bitarray.h: Allocating memory.");
         if (m_array&&m_allocated)  {
            delete [] m_array;
            m_array = 0;
         }
         m_slots = capacity / 8;
         m_slots += capacity%8==0 ? 0 : 1;
         if (capacity) {
            m_array = new char[m_slots];
            m_allocated = true;
         }
         clear();
      }
      void setsize(const unsigned long int &size) {
         assert(m_allocated);
         if (size > m_slots*8) {
            expand(size);
         }
         else if (size>m_size) {
            int slot = m_size/8;
            int bit_in_slot = m_size % 8;
            int newslot = size/8;
            std::memset(m_array+slot+1, 0, newslot-slot);
            for (int i=bit_in_slot+1; i<8; ++i)
               m_array[slot] &= ~(static_cast<char>(1 << i));
         }
         m_size = size;
      }
      void expand(const unsigned long int &size) {
         assert(m_allocated);
         char *tmp = m_array;
         int oldslots = m_slots;
         int oldsize = m_size;
         m_array = 0;
         init(size);
         if (tmp) {
            std::memcpy(m_array, tmp, oldslots);
            delete [] tmp;
         }
         m_size = oldsize;
      }
      void add(const bool &s) {
         assert(m_allocated);
         if (m_size == m_slots * 8 - 1) {
            expand(m_slots*8*2);
         }
         if (s)
            set(m_size++);
         else
            unset(m_size++);
         assert(m_size/8 <= m_slots);
      }
      void add(const CBitArray &a) {
         assert(m_allocated);
         if (a.m_size < 32) {
            for (unsigned long int i = 0; i<a.m_size; ++i)
               add(a.isset(i));
         }
         else {
            // for efficiency reasons, copy the the next available char slot and leave some empty space
            int slot = m_size / 8;
            if (slot + a.m_slots >= m_slots)
               expand(slot*8*2);
            std::memcpy(m_array+slot+1, a.m_array, a.m_slots);
            m_size = (slot + 1) * 8 + a.m_size;
         }
      }
      void add(unsigned long int n, unsigned long int size) {
         assert(m_allocated);
         assert(n < (1<<size));
         while (size) {
            add(n%2);
            n >>= 1;
            size -= 1;
         }
      }
      CBitArray &operator = (const CBitArray &a) {
         if (m_allocated&&m_array) {
            delete[]m_array;
         }
         m_allocated = false;
         m_size = a.m_size;
         m_slots = a.m_slots;
         m_array = a.m_array;
      }
      void copy(const CBitArray &a) {
         m_allocated = true;
         setsize(a.m_size);
         if (m_array && a.m_array)
            std::memcpy(m_array, a.m_array, m_slots);
      }
      bool operator == (const CBitArray &a) const {
         if (m_array == a.m_array) {
            assert(m_slots == a.m_slots);
            assert(m_size == a.m_size);
            return true;
         }
         if (m_size != a.m_size) return false;
         if (m_slots != a.m_slots) return false;
         return std::memcmp(m_array, a.m_array, m_slots);
      }
      bool operator != (const CBitArray &a) const {
         return !((*this) == a);
      }
      operator std::string () const {
         std::string retval = "";
         for (unsigned long int i=0; i<m_size; ++i)
            //os << (ba.isset(i)?'1':'0');
            retval += isset(i)  ? '1' : '0';
         return retval;
      }
      unsigned long hash() const{ 
         unsigned long retval = 0;
         std::memcpy(&retval, m_array, std::min(static_cast<unsigned long>(m_slots), static_cast<unsigned long>(sizeof(unsigned long)))); // use the first few bits as the hash value
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
std::ostream & operator << (std::ostream &os, const CBitArray &ba) {
   os << static_cast<std::string>(ba);
   return os;
}


/*===============================================================
 *
 * CCopyBitArray - the bit array class which by default copies 
 *
 *==============================================================*/
 
class CCopyBitArray : public CBitArray {
   public:
      CCopyBitArray() : CBitArray() {
      }
      CCopyBitArray(const CBitArray &a) : CBitArray() {
         copy(a);
      }
      CCopyBitArray(const CCopyBitArray &a) : CBitArray() {
         copy(a);
      }
   public:
      CCopyBitArray &operator = (const CBitArray &a) {
         copy(a);
      }
      CCopyBitArray &operator = (const CCopyBitArray &a) {
         copy(a);
      }
      bool operator == (const CBitArray &a) const {
         return static_cast<CBitArray>(*this) == a;
      }
      bool operator != (const CBitArray &a) const {
         return !((*this) == a);
      }
};

#endif
