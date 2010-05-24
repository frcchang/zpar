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

class CSentenceMultiCon {

public:
   CTwoStringVector words, 
   vector< vector<CConstituent> > constituents;

public:
   void clear() {
      words.clear();
      constituents.clear();
   }

};

//==============================================================

inline std::istream & operator >> (std::istream &is, CSentenceMultiCon &sent) {
   sent.clear();
   string line;
   string word;
   string pos;
   string constituent;
//   while(is && line.empty())
//      getline(is, line);
   getline(is, line);

   while(is && !line.empty())
   {
      istringstream iss(line);
      iss >> word >> pos >> constituent;
      ASSERT(!pos.empty() && !constituent.empty(), "A line must contain at least one POS and one constituent");
      sent.words.push_back( make_pair(word, pos) );
      sent.constituents.push_back(vector<CConstituent>());
      sent.constituents.back().push_back( constituent );
      while ( iss >> constituent )
         sent.constituents.back().push_back( constituent );
      getline(is, line);
   }
   return is ;
}

inline std::ostream & operator << (std::ostream &os, const CSentenceMultiCon &sent) {
   ASSERT(sent.words.size()==sent.constituents.size(), "Internal error: sentence multicon contains different words and constituents");
   for (int i=0; i<sent.words.size(); ++i) {
      os << sent.words.at(i).first << " " << sent.words.at(i).second;
      for (int j=0; j<sent.constituents.size(); ++j)
         os << " " << sent.constituents.at(i).at(j);
      os << endl ;
   }
   os << endl ;
   return os ;
}

#endif
