// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                              *
 * table2.h - a table with two dimensions specific classes      *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 ****************************************************************/

#ifndef _TABLE2_H
#define _TABLE2_H

#include "hash.h"

template<typename K1, typename K2, typename V>
class CTable2 {
protected:
   unsigned size1;
   unsigned size2;
   CHashMap<K1, unsigned> ind1;
   CHashMap<K2, unsigned> ind2;
   V* table;

public:
   CTable2() : size1(0), size2(0), ind1(1, false), ind2(1, false), table(0) {}

public:
   const V& lookup(const K1&k1, const K2&k2) {
      ASSERT(table, "The table is not initialized when used.");
      const unsigned &index1 = ind1.find(k1, size1-1);
      const unsigned &index2 = ind2.find(k2, size2-1);
      return table[index1*size2+index2];
   }

   friend std::ostream &operator << (std::ostream &os, CTable2 &j) {
      std::vector<K1> keys1;
      std::vector<K2> keys2;
      keys1.resize(j.size1);
      typename CHashMap<K1, unsigned>::iterator it1;
      for (it1 = j.ind1.begin(); it1 != j.ind1.end(); ++it1)
         keys1[it1.second()] = it1.first();
      keys2.resize(j.size2);
      typename CHashMap<K2, unsigned>::iterator it2;
      for (it2 = j.ind2.begin(); it2 != j.ind2.end(); ++it2)
         keys2[it2.second()] = it2.first();
      unsigned index1;
      for (index1=0; index1<keys1.size(); ++index1)
         os << keys1[index1] << " ";
      os << std::endl;
      unsigned index2;
      for (index2 = 0; index2 < keys2.size(); ++index2)
         os << keys2[index2] << " ";
      os << std::endl;
      for (index2 = 0; index2 < j.size2;  ++index2) {
         for (index1 = 0; index1 < j.size1; ++index1) {
            std::cerr << j.table[index1*j.size2+index2] << " ";
         }
         std::cerr << std::endl;
      }
      return os;
   }
   friend std::istream &operator >> (std::istream &is, CTable2 &j) {
      ASSERT(is, "The input file is not ready");
      std::vector<K1> keys1;
      std::vector<K2> keys2;
      std::string line;
      K1 key1;
      K2 key2;
      getline(is, line);
      std::istringstream is1(line);
      while (is1>>key1) {
         keys1.push_back(key1);
      }
      getline(is, line);
      std::istringstream is2(line);
      while(is2>>key2) {
         keys2.push_back(key2);
      }
      j.size1 = keys1.size();
      j.size2 = keys2.size();
      j.ind1.resize(j.size1);
      j.ind2.resize(j.size2);
      j.ind1.init();
      j.ind2.init();
      unsigned index1;
      unsigned index2;
      for (index1=0; index1<j.size1; ++index1)
         j.ind1[keys1[index1]] = index1;
      for (index2=0; index2<j.size2; ++index2)
         j.ind2[keys2[index2]] = index2;
      j.table = new V[j.size1*j.size2];
      for (index2=0; index2<j.size2; ++index2) {
//         ASSERT(is, "The file ends before its data retrieved.");
         getline(is, line);
         std::istringstream iss(line);
         for (index1=0; index1<j.size1; ++index1) {
//            ASSERT(iss, "The file ends before its data retrieved.");
            iss >> j.table[index1*j.size2+index2];
         }
      }
      return is;
   }
};

#endif
