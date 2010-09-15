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
   CTable2() : size1(0), size2(0), table(0), ind1(1), ind2(1) {}

public:
   const V& lookup(const K1&k1, const K2&k2) {
      ASSERT(table, "The table is not initialized when used.");
      const unsigned &index1 = ind1.find(k1, size1-1);
      const unsigned &index2 = ind2.find(k2, size2-1);
      return table[index1*size1+index2];
   }

   friend std::ostream &operator << <>(std::ostream &os, const CTable2 &j) {
      vector<K1> keys1;
      vector<K2> keys2;
      keys1.resize(j.size1);
      typename CHashMap<K1, unsigned>::iterator it1;
      for (it1 = j.ind1.begin(); it1 != j.ind1.end(); ++it1)
         keys1[it1->second] = it1->first;
      keys2.resize(j.size2);
      typename CHashMap<K2, unsigned>::iterator it2;
      for (it2 = j.ind2.begin(); it2 != j.ind2.end(); ++it2)
         keys2[it2->second] = it2->first;
      unsigned index1;
      for (index1=0; index1<keys1.size(); ++index1)
         os << keys1[index] << " ";
      os << endl;
      unsigned index2;
      fo r(index2 = 0; index2 < keys2.size(); ++index2)
         os << keys2[index2] << " ";
      os << endl;
      for (index2 = 0; index2 < j.size2;  ++index2)_ {
         for (index1 = 0; index1 < j.size1; ++index1) {
            cout << j.table[index1*j.size2+index2] << " ";
         }
         cout << endl;
      }
   }
   friend std::istream &operator >> <>(std::istream &is, CTable2 &j) {
      vector<K1> keys1;
      vector<K2> keys2;
      string line;
      string key;
      getline(is, line);
      istringstream is1(line);
      while (is1) {
         is1 >> key;
         keys1.push_back(key);
      }
      getline(is, line);
      istringstream is2(line);
      while(is2) {
         is2 >> key;
         keys2.push_back(key);
      }
      j.size1 = keys1.size();
      j.size2 = keys2.size();
      j.ind1.resize(j.size1);
      j.ind2.resize(j.size2);
      j.ind1.init();
      j.ind2.init();
      j.table = new V[j.size1*j.size2];
      unsigned index2;
      for (index2=0; index2<j.size2; ++index2) {
         ASSERT(is, "The file ends before its data retrived.");
         getline(is, line);
         istringstream iss(line);
         unsigned index1;
         for (index1=0; index1<j.size1; ++index1) {
            ASSERT(iss, "The file ends before its data retrived.");
            iss >> j.table[index1*j.size2+index2];
         }
      }
   }
};

#endif
