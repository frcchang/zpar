// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                              *
 * sentence_multicon.h - a sentence with multiple constituents  *
 *                       for each word.                         *
 *                                                              *
 * Yue Zhang                                                    *
 *                                                              *
 * Oxford University Computing Laboratory. 2008.3.              *
 *                                                              *
 ****************************************************************/

#ifndef _SENTENCE_MULTICON_H
#define _SENTENCE_MULTICON_H
#include "sentence_string.h"

/*==============================================================
 *
 * CSentenceMultiCon
 *
 *==============================================================*/

template<typename CConstituent>
class CSentenceMultiCon {

public:
   CTwoStringVector words; 
   std::vector< std::vector<CConstituent> > constituents;

public:
   void clear() {
      words.clear();
      constituents.clear();
   }

};

//==============================================================

template<typename CConstituent>
inline std::istream & operator >> (std::istream &is, CSentenceMultiCon<CConstituent> &sent) {
   sent.clear();
   std::string line;
   std::string word;
   std::string pos;
   std::string constituent;
//   while(is && line.empty())
//      getline(is, line);
   getline(is, line);

   while(is && !line.empty())
   {
      std::istringstream iss(line);
      iss >> word >> pos >> constituent;
      ASSERT(!pos.empty() && !constituent.empty(), "A line must contain at least one POS and one constituent");
      sent.words.push_back( std::make_pair(word, pos) );
      sent.constituents.push_back(std::vector<CConstituent>());
      sent.constituents.back().push_back( constituent );
      while ( iss >> constituent )
         sent.constituents.back().push_back( constituent );
      getline(is, line);
   }
   return is ;
}

template<typename CConstituent>
inline std::ostream & operator << (std::ostream &os, const CSentenceMultiCon<CConstituent> &sent) {
   ASSERT(sent.words.size()==sent.constituents.size(), "Internal error: sentence multicon contains different words and constituents");
   for (int i=0; i<sent.words.size(); ++i) {
      os << sent.words.at(i).first << " " << sent.words.at(i).second;
      for (int j=0; j<sent.constituents.at(i).size(); ++j)
         os << " " << sent.constituents.at(i).at(j);
      os << std::endl ;
   }
   os << std::endl ;
   return os ;
}

#endif
