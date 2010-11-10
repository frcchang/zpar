// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                              *
 * utf.h - the utilities for unicode characters.                *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2007.6                        *
 *                                                              *
 ****************************************************************/

#ifndef _UTILITY_UTF_H
#define _UTILITY_UTF_H

/*===============================================================
 *
 * Unicode std::string and character utils
 *
 *==============================================================*/

/*---------------------------------------------------------------
 *
 * getUTF8StringLength - get how many characters are in a UTF8 std::string
 *
 *--------------------------------------------------------------*/

inline
unsigned long int getUTF8StringLength(const std::string &s) {
   unsigned long int retval=0;
   unsigned long int idx=0;
   while (idx<s.length()) {
      if ((s[idx]&0x80)==0) {
         ++idx;
         ++retval;
      }
      else if ((s[idx]&0xE0)==0xC0) {
         idx += 2;
         ++retval;
      }
      else if ((s[idx]&0xF0)==0xE0) {
         idx += 3;
         ++retval;
      }
      else {
         WARNING("in utf.h getUTF8StringLength: std::string '" << s << "' not encoded in unicode utf-8"); 
         return retval;
      }
   }
   if (idx != s.length()) {
      WARNING("in utf.h getUTF8StringLength: std::string '" << s << "' not encoded in unicode utf-8"); 
      return retval;
   }
   return retval;
}

/*----------------------------------------------------------------
 *
 * getCharactersFromUTF8String - get the characters from 
 *                               utf std::string. The characters from 
 *                               this std::string are appended
 *                               to a given sentence. 
 *
 *----------------------------------------------------------------*/

template<class CSentence>
inline int getCharactersFromUTF8String(const std::string &s, CSentence *sentence) {
   if (sentence==NULL)
      return 0;
   unsigned long int idx=0;
   unsigned long int len=0;
   while (idx<s.length()) {
      if ((s[idx]&0x80)==0) {
         sentence->push_back(s.substr(idx, 1));
         ++len;
         ++idx;
      }
      else if ((s[idx]&0xE0)==0xC0) {
         sentence->push_back(s.substr(idx, 2));
         ++len;
         idx += 2;
      }
      else if ((s[idx]&0xF0)==0xE0) {
         sentence->push_back(s.substr(idx, 3));
         ++len;
         idx += 3;
      }
      else {
         WARNING("in utf.h getCharactersFromUTF8String: std::string '" << s << "' not encoded in unicode utf-8"); 
         sentence->push_back("?");
         ++len;
         ++idx;
      }
   }
   if (idx != s.length()) {
      WARNING("in utf.h getCharactersFromUTF8String: std::string '" << s << "' not encoded in utf-8"); 
      return len;
   }

   return len;
}

/*----------------------------------------------------------------
 *
 * getFirstCharFromUTF8String - get the first character from 
 *                              utf std::string. 
 *
 *----------------------------------------------------------------*/

inline std::string getFirstCharFromUTF8String(const std::string &s) {
   if (s=="")
      return "";
   if ((s[0]&0x80)==0) {
      return s.substr(0, 1);
   }
   else if ((s[0]&0xE0)==0xC0) {
      assert(s.length()>=2);
      return s.substr(0, 2);
   }
   else if ((s[0]&0xF0)==0xE0) {
      assert(s.length()>=3);
      return s.substr(0, 3);
   }
   else {
      WARNING("in utf.h getFirstCharFromUTF8String: std::string '" << s << "' not encoded in unicode utf-8"); 
      return "?";
   }
}

/*----------------------------------------------------------------
 *
 * getLastCharFromUTF8String - get the last character from 
 *                             utf std::string.  
 *
 *----------------------------------------------------------------*/

inline std::string getLastCharFromUTF8String(const std::string &s) {
   if (s=="")
      return "";
   unsigned long int idx=0;
   std::string retval;
   while (idx<s.length()) {
      if ((s[idx]&0x80)==0) {
         retval = s.substr(idx, 1);
         ++idx;
      }
      else if ((s[idx]&0xE0)==0xC0) {
         retval = s.substr(idx, 2);
         idx += 2;
      }
      else if ((s[idx]&0xF0)==0xE0) {
         retval = s.substr(idx, 3);
         idx += 3;
      }
      else {
         WARNING("in utf.h getLastCharFromUTF8String: std::string '" << s << "' not encoded in unicode utf-8"); 
         return "?";
      }
   }
   if (idx != s.length()) {
      WARNING("in utf.h getLastCharFromUTF8String: std::string '" << s << "' not encoded in unicode utf-8"); 
      return "?";
   }
   return retval;
}

/*----------------------------------------------------------------
 *
 * isOneUTF8Character - whether a std::string is one utf8 character
 *
 *----------------------------------------------------------------*/

inline bool isOneUTF8Character(const std::string &s) {
   if (s=="") return false; // is no utf character
   if (s.size()>3) return false; // is more than one utf character
   if ((s[0]&0x80)==0) {
      return s.size() == 1;
   }
   else if ((s[0]&0xE0)==0xC0) {
      return s.size() == 2;
   }
   else if ((s[0]&0xF0)==0xE0) {
      return s.size() == 3;
   }
}


#endif
