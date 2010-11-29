// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                              *
 * tagdict.h - the tag dictionary implementation.               *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2007.10                        *
 *                                                              *
 ****************************************************************/

#ifndef _TAGDICT_H
#define _TAGDICT_H

#include "hash.h"

/*===============================================================
 *
 * CTagDict - the tagdict
 *
 *==============================================================*/

template <typename CWord, typename CTag>
class CTagDict {

protected:

   CHashMap<CWord, unsigned long long> m_mapItems;
   unsigned long m_nTotalNumberOfTags;

public:

   CTagDict(unsigned long nTotalNumberOfTags) : m_mapItems(65536) { 
      m_nTotalNumberOfTags = nTotalNumberOfTags ;
      ASSERT( nTotalNumberOfTags < (sizeof(unsigned long long)*8), "The number of tags is too much for the current tag dictionary to support." );   
   }

public:

   unsigned long long lookup(const CWord &word) const {
      return m_mapItems.find(word, 0);
   }

   // whether the word and tag std::pair is in the dictionary
   bool lookup(const CWord &word, const CTag &tag) const {
      return m_mapItems.find(word, 0) & (static_cast<unsigned long long>(1)<<tag.code()) ;
   }

   // add a word and tag std::pair into entries
   void add(const CWord &word, const CTag &tag) {
      m_mapItems[word] |= (static_cast<unsigned long long>(1)<<tag.code());
   }

public:

   // load tag dictionary from a stream
   void load(std::istream &is) {
      std::string s, word, tag ;
      unsigned long long i ;
      getline(is, s);
      while( is && !(s.empty()) ) {
         std::istringstream iss(s);
         iss >> word >> tag ;
         i = CTag(tag).code() ; 
         ASSERT(i, "Unrecognized tag in the input file for the dict: "<<tag);
         m_mapItems[word] |= (static_cast<unsigned long long>(1)<<i) ;
         getline(is, s);
      }
   }

   // load tag dictionary from a file
   void load(const std::string &sFileName) {
      std::ifstream ifs ;

      ifs.open( sFileName.c_str() ) ;
      assert( ifs.is_open() ) ;
      load(ifs) ;
      ifs.close() ;
   }

   // save tag dictionary to a stream
   void save(std::ostream &os) {
      typename CHashMap<CWord, unsigned long long>::iterator it;
      unsigned long long i, j ; 
      it = m_mapItems.begin() ;
      while (it != m_mapItems.end()) {
         j = it.second();
         for (i = 0; i < m_nTotalNumberOfTags; ++i) {
            if ( j & (static_cast<unsigned long long>(1)<<i) ) 
               os << it.first().str() << "\t" << CTag(i).str() << std::endl ;
         }
         ++it ; 
      }
      os << std::endl;
   }

   // save tag dictionary to a file
   void save(const std::string &sFileName) {
      std::ofstream ofs ;
      ofs.open( sFileName.c_str() ) ;
      assert( ofs.is_open() ) ;
      save(ofs);
      ofs.close() ;
   }

};

//================================================================

template<typename CWord, typename CTag>
std::istream & operator >> (std::istream &is, CTagDict<CWord, CTag> &dict) {
   dict.load(is);
   return is;
}

template<typename CWord, typename CTag>
std::ostream & operator << (std::ostream &os, CTagDict<CWord, CTag> &dict) {
   dict.save(os);
   return os;
}

#endif
