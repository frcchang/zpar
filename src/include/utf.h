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
 * Unicode string and character utils
 *
 *==============================================================*/

/*---------------------------------------------------------------
 *
 * getUTF8StringLength - get how many characters are in a UTF8 string
 *
 *--------------------------------------------------------------*/

inline
unsigned int getUTF8StringLength(const string &s) {
   unsigned int retval=0;
   unsigned int idx=0;
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
         THROW("string '" << s << "' not encoded in unicode utf-8"); 
      }
   }
   if (idx != s.length()) {
      THROW("string '" << s << "' not encoded in unicode utf-8"); 
   }
   return retval;
}

/*----------------------------------------------------------------
 *
 * getCharactersFromUTF8String - get the characters from 
 *                               utf string. The characters from 
 *                               this string are appended
 *                               to a given sentence. 
 *
 *----------------------------------------------------------------*/

template<class CSentence>
inline int getCharactersFromUTF8String(const string &s, CSentence *sentence) {
   if (sentence==NULL)
      return 0;
   unsigned int idx=0;
   unsigned int len=0;
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
         THROW("string '" << s << "' not encoded in unicode utf-8"); 
      }
   }
   if (idx != s.length()) {
      THROW("string '" << s << "' not encoded in unicode utf-8"); 
   }

   return len;
}

/*----------------------------------------------------------------
 *
 * getFirstCharFromUTF8String - get the first character from 
 *                              utf string. 
 *
 *----------------------------------------------------------------*/

inline string getFirstCharFromUTF8String(const string &s) {
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
      THROW("string '" << s << "' not encoded in unicode utf-8"); 
   }
}

/*----------------------------------------------------------------
 *
 * getLastCharFromUTF8String - get the last character from 
 *                             utf string.  
 *
 *----------------------------------------------------------------*/

inline string getLastCharFromUTF8String(const string &s) {
   if (s=="")
      return "";
   unsigned int idx=0;
   string retval;
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
         THROW("string '" << s << "' not encoded in unicode utf-8"); 
      }
   }
   if (idx != s.length()) {
      THROW("string '" << s << "' not encoded in unicode utf-8"); 
   }
   return retval;
}

/*----------------------------------------------------------------
 *
 * isOneUTF8Character - whether a string is one utf8 character
 *
 *----------------------------------------------------------------*/

inline bool isOneUTF8Character(const string &s) {
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
