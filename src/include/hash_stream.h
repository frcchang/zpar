#ifndef _HASH_STREAM_DEF_H
#define _HASH_STREAM_DEF_H

template <typename K, typename V>
inline
istream & operator >> (istream &is, CHashMap<K, V> &table) {
   K key ;
   V value ;
   char c ;
   string s ; 
   getline(is, s);
   while (is && !(s.empty())) {
      istringstream iss(s) ; 
      iss >> key >> c >> value ;
      assert( c == ':' ) ;
      table.insert(key,value);
      getline(is, s);
   }
   return is;
}

template <typename K, typename V>
inline
ostream & operator << (ostream &os, CHashMap<K, V> &table) {
   typename CHashMap<K, V>::iterator it = table.begin() ;
   while (it != table.end()) {
      os << it.first() << "\t:\t" << it.second() << endl ;
      ++ it;
   }
   os << endl ;
   return os ;
}


#endif
