/****************************************************************
 *                                                              *
 * supertag_util.h - the utility for dependency supertag        *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2008.4                         *
 *                                                              *
 ****************************************************************/

#ifndef _DEPENDENCY_SUPERTAG_UTIL_H
#define _DEPENDENCY_SUPERTAG_UTIL_H 

#include "reader.h"
#include "writer.h"

//===============================================================

inline unsigned long hash(const CSuperTag &tag) { return tag.code();}

//===============================================================

inline istream & operator >> (istream &is, CSuperTag &tag) {
   string s;
   is >> s;
   tag.load(s);
   return is;
}

inline ostream & operator << (ostream &os, const CSuperTag &tag) {
   os << tag.str() ;
   return os;
}

//===============================================================

// independent of supertag implementation
class CSuperTagReader : public CSentenceReader {

public:
   CSuperTagReader(const string &file) : CSentenceReader(file) {}
   virtual ~CSuperTagReader() {}

public:
   bool readSuperTaggedSentence(CSentenceTagged *sent, vector<CSuperTag> *supertags) {
      // clear output
      sent->clear();
      supertags->clear();
   
      string word, tag, supertag, line;
   
      while((*m_iStream) && line.empty())
         getline((*m_iStream), line);
   
      if (!(*m_iStream)) 
         return false;

      while(*m_iStream && !line.empty())
      {
         istringstream iss(line);
         iss >> word >> tag >> supertag ;
         sent->push_back( make_pair( word, tag ) ) ;
         supertags->push_back( CSuperTag(supertag) ) ;
         getline(*m_iStream, line);
      }
      return true;
   }
};

//===============================================================

// independent of supertag implementation
class CSuperTagWriter : public CSentenceWriter {

public:
   CSuperTagWriter(const string &file) : CSentenceWriter(file) {}
   virtual ~CSuperTagWriter() {}

public:
   void writeSentence(CSentenceTagged *tagged, vector<CSuperTag> *supertags) {
      assert(tagged->size()==supertags->size());
      for (int i=0; i<supertags->size(); i++) {
         *m_oStream << tagged->at(i).first << "\t" << tagged->at(i).second << "\t";
         *m_oStream << supertags->at(i);
         *m_oStream << endl;
      }
      *m_oStream << endl;
   }
};

//===============================================================

#endif

