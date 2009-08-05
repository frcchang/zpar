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

namespace depparser {

//
// TYPE DEFINITIONS
//
typedef CScoreMap< CWord, SCORE_TYPE > CWordMap;
typedef CScoreMap<CTaggedWord<CTag>, SCORE_TYPE> CTaggedWordMap;
typedef CScoreMap< CTwoWords, SCORE_TYPE > CTwoWordsMap;
typedef CScoreMap< pair<CWord, int>, SCORE_TYPE > CWordIntMap;
typedef CScoreMap<pair<CTaggedWord<CTag>, int>,  SCORE_TYPE> CTaggedWordIntMap;
typedef CHashMap< CWord, int > CWordToIntMap;
typedef CScoreMap<pair<CTwoWords, int>, SCORE_TYPE> CTwoWordsIntMap;
typedef CHashMap< pair<CWord, int>, int > CWordIntToIntMap;
typedef CScoreMap< long int, SCORE_TYPE > CIntMap;
typedef CScoreMap< pair<long int, long int>, SCORE_TYPE > CIntPairMap;
typedef CScoreMap<CTwoTaggedWords, SCORE_TYPE > CTwoTaggedWordsMap;
typedef CScoreMap<pair<CTwoTaggedWords, int>, SCORE_TYPE> CTwoTaggedWordsIntMap;

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

   // feature templates from mcdonald
   CWordIntMap m_mapHeadWord;
   CWordIntMap m_mapDepWord;
   CIntPairMap m_mapHeadTag;
   CIntPairMap m_mapDepTag;
   CTaggedWordIntMap m_mapHeadWordTag;
   CTaggedWordIntMap m_mapDepWordTag;

   CTwoTaggedWordsIntMap m_mapHeadWordTagDepWordTag;
   CTwoTaggedWordsIntMap m_mapHeadWordTagDepWord;
   CTwoTaggedWordsIntMap m_mapHeadWordDepWordTag;
   CTwoTaggedWordsIntMap m_mapHeadTagDepWordTag;
   CTwoTaggedWordsIntMap m_mapHeadWordTagDepTag;
   CTwoWordsIntMap m_mapHeadWordDepWord;
   CIntPairMap m_mapHeadTagDepTag;

   CIntPairMap m_mapBetweenTags;
   CIntPairMap m_mapSurroundingTagsLL;
   CIntPairMap m_mapSurroundingTagsLR;
   CIntPairMap m_mapSurroundingTagsRL;
   CIntPairMap m_mapSurroundingTagsRR;

   // second order feature templates from mcdonald
   CTwoWordsIntMap m_mapSiblingWords;
   CTaggedWordIntMap m_mapSiblingWordTag;
   CTaggedWordIntMap m_mapSiblingTagWord;
   CIntPairMap m_mapSiblingTags;
   CIntPairMap m_mapSiblingAndParentTags;

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
                                                    m_mapCharTagDictionary(PENN_TAG_COUNT),

                                                    m_mapHeadWord("HeadWord", 122651),
                                                    m_mapDepWord("DepWord", 122651),
                                                    m_mapHeadTag("HeadTag", 122651),
                                                    m_mapDepTag("DepTag", 122651),
                                                    m_mapHeadWordTag("HeadWordTag", 122651),
                                                    m_mapDepWordTag("DepWordTag", 122651),

                                                    m_mapHeadWordTagDepWordTag("HeadWordTagDepWordTag", 122651),
                                                    m_mapHeadWordTagDepWord("HeadWordTagDepWord", 122651),
                                                    m_mapHeadWordDepWordTag("HeadWordDepWordTag", 122651),
                                                    m_mapHeadTagDepWordTag("HeadTagDepWordTag", 122651),
                                                    m_mapHeadWordTagDepTag("HeadWordTagDepTag", 122651),
                                                    m_mapHeadWordDepWord("HeadWordDepWord", 122651),
                                                    m_mapHeadTagDepTag("HeadTagDepTag", 122651),

                                                    m_mapBetweenTags("BetweenTags", 122651),
                                                    m_mapSurroundingTagsLL("SurroundingTagsLL", 122651),
                                                    m_mapSurroundingTagsLR("SurroundingTagsLR", 122651),
                                                    m_mapSurroundingTagsRL("SurroundingTagsRL", 122651),
                                                    m_mapSurroundingTagsRR("SurroundingTagsRR", 122651),

                                                    m_mapSiblingWords("SiblingWords", 122651),
                                                    m_mapSiblingWordTag("SiblingWordTag", 122651),
                                                    m_mapSiblingTagWord("SiblingTagWord", 122651),
                                                    m_mapSiblingTags("SiblingTags", 122651),
                                                    m_mapSiblingAndParentTags("SiblingAndParentTags", 122651)

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
