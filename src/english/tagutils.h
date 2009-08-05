
#ifndef ENGLISH_TAG_UTILS
#define ENGLISH_TAG_UTILS

namespace english {

inline
unsigned int joinTwoTags(const CTag& tag1, const CTag& tag2) {
   return (tag1.code() << PENN_TAG_COUNT_BITS) + tag2.code();
}

inline
unsigned int stringToTags(const string& s) {
   string sTemp="";
   unsigned int nRetval = 0;
   for (int i=0; i<s.size(); i++) {
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
string tagsToString(const unsigned& tags, int len=2) {
   string sRetval;
   unsigned temp = tags;
   while (len-->0) {
      if (!sRetval.empty())
         sRetval = ' ' + sRetval;
      sRetval = CTag(temp&((1<<PENN_TAG_COUNT_BITS)-1)).str() + sRetval;
      temp = temp >> PENN_TAG_COUNT_BITS;
   }
   return sRetval;
}

inline
bool anyNoneTag(const unsigned &tags, int len=2) {
   int mask = (1<<PENN_TAG_COUNT_BITS)-1;
   while (len-->0) {
      if ((tags&mask)==PENN_TAG_NONE)
         return true;
      mask <<= PENN_TAG_COUNT_BITS;
   }
   return false;
}

};

#endif
