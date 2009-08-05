/***************************************************************
 *
 * The tagger's chart implementation 
 *
 * Yue Zhang, 2007
 *
 ***************************************************************/

#ifndef TAGGER_AGENDACHART_WEIGHT_IMPL_H
#define TAGGER_AGENDACHART_WEIGHT_IMPL_H

//
// TYPE DEFINITIONS
//
typedef CHashMap<CWord, CScore, 32771> CWordMap;
typedef CHashMap<CTwoWords, CScore, 65537> CTwoWordsMap;
typedef CHashMap<CWord, int, 65537> CWordIntMap;
typedef CHashMap<int, CScore, 16381> CIntMap;
typedef CHashMap<pair<int, int>, CScore, 16381> CIntPairMap;

/*===============================================================
 *
 * CWeight - the definition of weights, in feature
 *
 *==============================================================*/

struct CWeight {
   int m_maxLengthByTag[PENN_TAG_COUNT+1];
   int getMaxWordLength() const {return m_maxLengthByTag[PENN_TAG_COUNT];}
   void setMaxLengthByTag(int tag, int length) {
      if (length<=m_maxLengthByTag[tag])
         return;  
      m_maxLengthByTag[tag]=length;
      if (length>m_maxLengthByTag[PENN_TAG_COUNT])
         m_maxLengthByTag[PENN_TAG_COUNT]=length;
   }
   //
   // FEATURE TEMPLATES FOR WORD
   //
   CWordMap m_mapSeenWords;
   CTwoWordsMap m_mapLastWordByWord;
   CTwoWordsMap m_mapAroundChars;
   CWordMap m_mapCurrentWordLastChar;
   CWordMap m_mapLastWordFirstChar;
   CWordMap m_mapWordByPrevTwoChars;
   CWordMap m_mapWordByNextTwoChars;
   CTwoWordsMap m_mapFirstCharLastWordByWord;
   CTwoWordsMap m_mapLastWordByLastChar;
   CWordMap m_mapSeparateChars;
   CWordMap m_mapConsecutiveChars;
   CTwoWordsMap m_mapFirstAndLastChars;
   CWordMap m_mapOneCharWord;
   CWordMap m_mapLengthByFirstChar[LENGTH_MAX];
   CWordMap m_mapLengthByLastChar[LENGTH_MAX];
   CWordMap m_mapLengthByLastWord[LENGTH_MAX];
   CWordMap m_mapLastLengthByWord[LENGTH_MAX];
   //
   // FEATURE TEMPLATES FOR TAG
   //
   CScore m_mapLastTagByTag[PENN_TAG_COUNT][PENN_TAG_COUNT];
   CScore m_mapLastTwoTagsByTag[PENN_TAG_COUNT][1<<PENN_TAG_COUNT_BITS*2];

   CWordMap m_mapCurrentTag[PENN_TAG_COUNT];
   CWordMap m_mapTagByLastWord[PENN_TAG_COUNT];
   CWordMap m_mapLastTagByWord[PENN_TAG_COUNT];
   CWordMap m_mapTagByFirstChar[PENN_TAG_COUNT];
   CWordMap m_mapTagByLastChar[PENN_TAG_COUNT];
   CWordMap m_mapTagByPrevChar[PENN_TAG_COUNT];
   CWordMap m_mapTagByNextChar[PENN_TAG_COUNT];
   CWordMap m_mapTagByPrevTwoChar[PENN_TAG_COUNT];
   CWordMap m_mapTagByNextTwoChar[PENN_TAG_COUNT];
   CWordMap m_mapTagOfOneCharWord[PENN_TAG_COUNT];
   CWordMap m_mapTagByChar[PENN_TAG_COUNT];
   CWordMap m_mapRepeatedCharByTag[PENN_TAG_COUNT];
   CWordMap m_mapTagByWordAndPrevChar[PENN_TAG_COUNT];
   CWordMap m_mapTagByWordAndNextChar[PENN_TAG_COUNT];
   CWordMap m_mapTagBySeparateChars[PENN_TAG_COUNT];

   CTwoWordsMap m_mapTagByWordAndLastWord;
   CTwoWordsMap m_mapLastTagByWordAndLastWord;
   CTwoWordsMap m_mapTagByWordAndLastTag;
   CTwoWordsMap m_mapLastTagByLastWordAndTag;
   CTwoWordsMap m_mapTaggedCharByLastWord;
   CTwoWordsMap m_mapLastTaggedCharByWord;
   CTwoWordsMap m_mapTaggedCharByFirstChar;
   CTwoWordsMap m_mapTaggedCharByLastChar;
   CTwoWordsMap m_mapTaggedCharByLastTag;

   CIntMap m_mapTagByFirstCharCat[PENN_TAG_COUNT];
   CIntMap m_mapTagByLastCharCat[PENN_TAG_COUNT];

   CIntPairMap m_mapSeparateCharCat;
   CIntPairMap m_mapConsecutiveCharCat;
   //CIntPairMap m_mapFirstCharCatTwoWords;
   //CIntPairMap m_mapLastCharCatTwoWords;

   // 
   // CONSTRUCTOR 
   //
   CWeight() { for (int i=0; i<=PENN_TAG_COUNT; ++i) m_maxLengthByTag[i] = 1; }
   CWordIntMap m_mapWordFrequency;
   CWordIntMap m_mapTagDictionary[PENN_TAG_COUNT];
   CWordIntMap m_mapCharTagDictionary;
   int m_nMaxWordFrequency;
};

inline long int hash(const int &i) {return i;}
inline long int hash(const pair<int, int> &i) {return i.first * 31 + i.second;}



#endif
