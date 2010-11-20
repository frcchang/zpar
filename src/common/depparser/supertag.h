// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                              *
 * supertag.h - the supertags for dependency parser in general  *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 ****************************************************************/

#ifndef _DEPPARSER_SUPERTAG_H
#define _DEPPARSER_SUPERTAG_H 

#ifdef SR_SUPERTAG

namespace TARGET_LANGUAGE {

namespace depparser {

class CSuperTag {

public:
   enum type {HL=0, HR, DL, DR, RT};

protected:
   unsigned long m_tags[MAX_SENTENCE_SIZE];
   unsigned long m_size;

public:
   bool canShift(const unsigned long &dep) const {
      return ( m_tags[dep] & (1<<HR) ) ;
   }
   bool canArcLeft(const unsigned long &head, const unsigned long &dep) const {
      return ( m_tags[head] & (1<<DL) ) && ( m_tags[dep] & (1<<HR) ) ;
   }
   bool canArcRight(const unsigned long &head, const unsigned long &dep) const {
      return (m_tags[head] & (1<<DR) ) && (m_tags[dep] & (1<<HL) ) ;
   }
   void setSentenceSize(const unsigned long &size) { m_size = size; }
   const unsigned long &getSentenceSize() const { return m_size; }

public:
   friend std::istream & operator >> (std::istream &is, CSuperTag &p);
   friend std::ostream & operator << (std::ostream &os, const CSuperTag &p);

};

//==============================================================================

inline std::istream & operator >> (std::istream &is, CSuperTag &p) {
   unsigned long index ;
   std::string s ; 
   getline(is, s);
   if (is && !(s.empty())) {
      std::istringstream iss(s) ; 
      index = 0;
      while ( iss >> p.m_tags[index] ) {
         ++index;
      }
      ASSERT(p.m_size==index, "The size of the supertag sentence from the input does not match the size given by caller");
   }
   else {
      THROW("No supertags read.");
   }
   return is;
}

//std::ostream & operator << (std::ostream &os, const TARGET_LANGUAGE::depparser::CSuperTag &p) {
inline std::ostream & operator << (std::ostream &os, const CSuperTag &p) {
   for (unsigned long i=0; i<p.m_size; ++i) {
      os << p.m_tags[i] << ' ';
   }
   os << std::endl;
   return os ;
}

} // namespace ddepparser
} // namespace TARGET_LANGUAGE

#else

namespace TARGET_LANGUAGE {

namespace depparser {

class CSuperTag {

protected:
   unsigned long m_tags[MAX_SENTENCE_SIZE*MAX_SENTENCE_SIZE];
   unsigned long m_size;

public:
   const unsigned long &getSuperTag(const unsigned long &head, const unsigned long &dep) const {
      return m_tags[head*m_size+dep];
   }
   void setSentenceSize(const unsigned long &size) { m_size = size; }
   const unsigned long &getSentenceSize() const { return m_size; }

public:
   friend std::istream & operator >> (std::istream &is, CSuperTag &p);
   friend std::ostream & operator << (std::ostream &os, const CSuperTag &p);

};

//==============================================================================

inline std::istream & operator >> (std::istream &is, CSuperTag &p) {
   unsigned long index ;
   std::string s ; 
   getline(is, s);
   if (is && !(s.empty())) {
      std::istringstream iss(s) ; 
      index = 0;
      while ( iss >> p.m_tags[index] ) {
         ++index;
         ASSERT(index <MAX_SENTENCE_SIZE*MAX_SENTENCE_SIZE,"The input supertag sequence does not match the sentence size.");
      }
      ASSERT(index==p.m_size*p.m_size,"The input supertag sequence does not match the sentence size.");
   }
   else {
      THROW("No supertags read.");
   }
   return is;
}

//std::ostream & operator << (std::ostream &os, const TARGET_LANGUAGE::depparser::CSuperTag &p) {
inline std::ostream & operator << (std::ostream &os, const CSuperTag &p) {
   for (unsigned long i=0; i<p.m_size*p.m_size; ++i) {
      os << p.m_tags[i] << ' ';
   }
   os << std::endl;
   return os ;
}

} // namespace ddepparser
} // namespace TARGET_LANGUAGE

#endif

#endif
