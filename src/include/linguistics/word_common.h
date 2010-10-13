// Copyright (C) University of Oxford 2010
// word manipulation methods
inline
const string &wordToString(const CWord &w) {
   return w.str();
}
inline CWord stringToWord(const string &s) { // this method is unecessary, for word can be assigned by a string
   return CWord(s);
};

// bigram word definition
typedef CBigram<CWord> CTwoWords;

// io methods
inline istream & operator >> (istream &is, CWord &w) {
   string s ;
   is >> s ;
   assert( s[0]=='[' && s[s.size()-1] == ']') ;
   w = s.substr(1,s.size()-2) ;
   return is ;
}

inline ostream & operator << (ostream &os, const CWord &w) {
   os << '[' << w.str() << ']' ;
   return os ;
}
