// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                              *
 * buffer.h - the generic buffer                                *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2009.09                        *
 *                                                              *
 ****************************************************************/

#ifndef _BUFFER_H
#define _BUFFER_H

template <typename K>
class CBufferCycle {

protected:
   K *m_buffer;
   unsigned long m_size;
   unsigned long m_flag; // the last free
   unsigned long m_end;

public:
   CBufferCycle(const unsigned long size) : m_size(size+1) {
      if (m_size<2) THROW("CBufferCycle cannot have zero or one size.");
      m_buffer = new K[m_size];
      m_end = 0;
      m_flag = m_size-1;
   }
   virtual ~CBufferCycle() {
      delete [] m_buffer;
   }

protected:
   unsigned long cycle_index(const unsigned long &index) const {
      return index >m_size-1?index- m_size:index;
   }

public:
   bool full() const {
      return m_end == m_flag;
   }
   bool empty() const {
      return size()==0;
   }
   unsigned long size() const {
      return m_end >= m_flag+1 ? m_end-m_flag-1 : m_end+m_size-m_flag-1;
   }
   const K &at(const unsigned long &i) const {
      assert(i<m_size-1); // m_size = required_size + 1
      return m_buffer[cycle_index(m_flag+i+1)];
   }
   void pushBack(const K &k) {
      if (full()) THROW("CBufferCycle full, but received a pushBack request.");
      m_buffer[m_end] = k;
      m_end = cycle_index(m_end+1);
   }
   void popFront(const unsigned long &n=1) {
      assert(cycle_index(m_flag+n)<m_end);
      m_flag = cycle_index(m_flag+n);
   }
   const K& operator [] (const unsigned long &i) const {
      return at(i);
   }
};

#endif
