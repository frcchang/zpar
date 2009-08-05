/***************************************************************
 *
 * The tagger's chart implementation 
 *
 * Yue Zhang, 2007
 *
 ***************************************************************/

#ifndef TAGGER_AGENDACHART_WEIGHT_IMPL_H
#define TAGGER_AGENDACHART_WEIGHT_IMPL_H

#include "weight_base.h"

namespace chinese {

namespace tagger {

//
// TYPE DEFINITIONS
//
typedef CScoreMap< CWord, SCORE_TYPE > CWordMap;
typedef CScoreMap< CTwoWords, SCORE_TYPE > CTwoWordsMap;
typedef CScoreMap< pair<CWord, int>, SCORE_TYPE > CWordIntMap;
typedef CHashMap< CWord, int > CWordToIntMap;
typedef CHashMap< pair<CWord, int>, int > CWordIntToIntMap;
typedef CScoreMap< long int, SCORE_TYPE > CIntMap;
typedef CScoreMap< pair<long int, long int>, SCORE_TYPE > CIntPairMap;
typedef CScoreMap<CTwoTaggedWords, SCORE_TYPE > CTwoTaggedWordsMap;

/*===============================================================
 *
 * CWeight - the definition of weights, in feature
 *
 *==============================================================*/

class CWeight : public CWeightBase {

public: 

   int m_maxLengthByTag[PENN_TAG_COUNT+1];
   int getMaxWordLength() const {return m_maxLengthByTag[PENN_TAG_COUNT];}
   void setMaxLengthByTag(int tag, int length) {
      if (length<=m_maxLengthByTag[tag])
         return;
      m_maxLengthByTag[tag]=length;
      if (length>m_maxLengthByTag[PENN_TAG_COUNT])
         m_maxLengthByTag[PENN_TAG_COUNT]=length;
   }

   // feature templates about words
   CWordMap m_mapSeenWords;
   CTwoWordsMap m_mapLastWordByWord;
   CWordMap m_mapCurrentWordLastChar;
   CWordMap m_mapLastWordFirstChar;
   CTwoWordsMap m_mapFirstCharLastWordByWord;
   CTwoWordsMap m_mapLastWordByLastChar;
   CWordMap m_mapSeparateChars;
   CWordMap m_mapConsecutiveChars;
   CTwoWordsMap m_mapFirstAndLastChars;
   CWordMap m_mapOneCharWord;
   CWordIntMap m_mapLengthByFirstChar;
   CWordIntMap m_mapLengthByLastChar;
   CWordIntMap m_mapLengthByLastWord;
   CWordIntMap m_mapLastLengthByWord;
   
   // feature templates tag
   CWordIntMap m_mapCurrentTag;
   CIntMap m_mapLastTagByTag;
   CIntMap m_mapLastTwoTagsByTag;
   CWordIntMap m_mapTagByLastWord;
   CWordIntMap m_mapLastTagByWord;
   CWordIntMap m_mapTagByFirstChar;
   CWordIntMap m_mapTagByLastChar;
   CWordIntMap m_mapTagByChar;
   CWordIntMap m_mapTagOfOneCharWord;
   CWordIntMap m_mapRepeatedCharByTag;
   CWordIntMap m_mapTagByWordAndPrevChar;
   CWordIntMap m_mapTagByWordAndNextChar;
   CTwoTaggedWordsMap m_mapTaggedCharByFirstChar;
   CTwoTaggedWordsMap m_mapTaggedCharByLastChar;

   CIntPairMap m_mapTagByFirstCharCat;
   CIntPairMap m_mapTagByLastCharCat;

   // feature templates knowledge
   CIntPairMap m_mapSeparateCharCat;
   CIntPairMap m_mapConsecutiveCharCat;

   // statistical information
   CWordToIntMap m_mapWordFrequency;
   CTagDict<CTag> m_mapTagDictionary;
   CTagDict<CTag> m_mapCharTagDictionary;
   int m_nMaxWordFrequency;

public:

   CWeight(const string &sFeatureDB, bool bTrain) : CWeightBase(sFeatureDB, bTrain) ,
                                                    m_mapSeenWords("WordDictionary", 65537) ,
                                                    m_mapLastWordByWord("LastWordByWord", 65537) ,
                                                    m_mapCurrentWordLastChar("CurrentWordLastChar", 65537) , 
                                                    m_mapLastWordFirstChar("LastWordFirstChar", 65537) ,
                                                    m_mapFirstCharLastWordByWord("FirstCharLastWordByWord", 65537) ,
                                                    m_mapLastWordByLastChar("LastWordByLastChar", 65537) ,
                                                    m_mapSeparateChars("SeparateChars", 65537) , 
                                                    m_mapConsecutiveChars("ConsecutiveChars", 65537) , 
                                                    m_mapFirstAndLastChars("FirstAndLastChars", 65537) ,
                                                    m_mapOneCharWord("OneCharWord", 65537) ,
                                                    m_mapLengthByFirstChar("LengthByFirstChar", 65537) , 
                                                    m_mapLengthByLastChar("LengthByLastChar", 65537) , 
                                                    m_mapLengthByLastWord("LengthByLastWord", 65537) ,
                                                    m_mapLastLengthByWord("LastLengthByWord", 65537) ,

                                                    m_mapLastTagByTag("LastTagByTag", 1627) ,
                                                    m_mapLastTwoTagsByTag("LastTwoTagsByTag", 16381) ,
                                                    m_mapCurrentTag("CurrentTag", 65537) ,
                                                    m_mapTagByLastWord("TagByLastWord", 65537) ,
                                                    m_mapLastTagByWord("LastTagByWord", 65537) ,
                                                    m_mapTagByFirstChar("TagByFirstChar", 65537) ,
                                                    m_mapTagByLastChar("TagByLastChar", 65537) ,
                                                    m_mapTagByChar("TagByChar", 65537) ,
                                                    m_mapTagOfOneCharWord("TagOfOneCharWord", 65537) ,
                                                    m_mapRepeatedCharByTag("RepeatedCharByTag", 65537) ,
                                                    m_mapTagByWordAndPrevChar("TagByWordAndPrevChar", 65537) ,
                                                    m_mapTagByWordAndNextChar("TagByWordAndNextChar", 65537) ,
                                                    m_mapTaggedCharByFirstChar("TaggedCharByFirstChar", 65537) ,
                                                    m_mapTaggedCharByLastChar("TaggedCharByLastChar", 65537) ,
                                                    m_mapTagByFirstCharCat("TagByFirstCharCat", 65537) ,
                                                    m_mapTagByLastCharCat("TagByLastCharCat", 65537) , 

                                                    m_mapSeparateCharCat("SeparateCharCat", 16381) , 
                                                    m_mapConsecutiveCharCat("ConsecutiveCharCat", 16381) ,

                                                    m_mapTagDictionary(PENN_TAG_COUNT),
                                                    m_mapCharTagDictionary(PENN_TAG_COUNT) 

   { 
      for (int i=0; i<=PENN_TAG_COUNT; ++i) m_maxLengthByTag[i] = 1; 
      m_nMaxWordFrequency=0;
      loadScores();
   }

public:
  
   void loadScores();
   void saveScores();
   void computeAverageFeatureWeights(int round);

};

} // namespace tagger
} // namespace chinese

#endif
