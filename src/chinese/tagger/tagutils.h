// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                              *
 * tagutils.h - utilities for manipulating POS tags.            *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2006.10                        *
 *                                                              *
 ****************************************************************/


#ifndef _TAG_UTILS_H
#define _TAG_UTILS_H

inline
unsigned long joinTwoTags(const CTag& tag1, const CTag& tag2) {
   return (tag1.code() << PENN_TAG_COUNT_BITS) + tag2.code();
};

inline
unsigned long std::stringToTags(const std::string& s) {
   std::string sTemp="";
   unsigned long nRetval = 0;
   for (int i=0; i<s.size(); ++i) {
      if (s[i]==' ') {
         if (!sTemp.empty()) {
            nRetval <<= PENN_TAG_COUNT_BITS;
            nRetval += CTag(sTemp).code();
            sTemp="";
         }
      } // end if current ' '
      else {
         sTemp += s[i];
      }
   } // end loop
   if (!sTemp.empty()) {
      nRetval <<= PENN_TAG_COUNT_BITS;
      nRetval += CTag(sTemp).code();
   }
   return nRetval;
}

inline
std::string tagsToString(const unsigned long& tags, int len=2) {
   std::string sRetval;
   unsigned long temp = tags;
   while (len-->0) {
      if (!sRetval.empty())
         sRetval = ' ' + sRetval;
      sRetval = CTag(temp&(static_cast<unsigned long>((1<<PENN_TAG_COUNT_BITS)-1))).str() + sRetval;
      temp = temp >> PENN_TAG_COUNT_BITS;
   }
   return sRetval;
}

inline
bool anyNoneTag(const unsigned long &tags, int len=2) {
   int mask = (1<<PENN_TAG_COUNT_BITS)-1;
   while (len-->0) {
      if ((tags&mask)==PENN_TAG_NONE)
         return true;
      mask <<= PENN_TAG_COUNT_BITS;
   }
   return false;
};

#endif
