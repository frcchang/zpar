/***************************************************************
 *
 * The tagger's chart implementation 
 *
 * Yue Zhang, 2007
 *
 ***************************************************************/

#ifndef TAGGER_SEGMENTED_WEIGHT_IMPL_H
#define TAGGER_SEGMENTED_WEIGHT_IMPL_H

#include "weight_base.h"

namespace chinese {

namespace tagger {

//
// TYPE DEFINITIONS
//
typedef CScoreMap< CWord, SCORE_TYPE > CWordMap;
typedef CScoreMap< CTwoWords, SCORE_TYPE > CTwoWordsMap;
typedef CHashMap< CWord, int > CWordIntMap;
typedef CScoreMap< int, SCORE_TYPE > CIntMap;
typedef CScoreMap< pair<CWord, int>, SCORE_TYPE > CWordIntPairMap;
typedef CScoreMap< pair<CWord, CTag>, SCORE_TYPE > CWordTagPairMap;
typedef CScoreMap< pair<int, int>, SCORE_TYPE > CIntPairMap;
typedef CScoreMap< CTwoTaggedWords, SCORE_TYPE > CTwoTaggedWordsMap;
typedef CScoreMap< CTagSet<CTag, 2>, SCORE_TYPE > CTagSet2Map;
typedef CScoreMap< CTagSet<CTag, 3>, SCORE_TYPE > CTagSet3Map;
typedef CScoreMap< pair<long int, CTag>, SCORE_TYPE > CIntTagPairMap;

/*===============================================================
 *
 * CWeight - the definition of weights, in feature
 *
 *==============================================================*/

class CWeight : public CWeightBase {

public: 

   //
   // FEATURE TEMPLATES FOR TAG
   //
   CTagSet2Map m_mapLastTagByTag;
   CTagSet3Map m_mapLastTwoTagsByTag;

   CWordTagPairMap m_mapCurrentTag;
   CWordTagPairMap m_mapTagByLastWord;
   CWordTagPairMap m_mapLastTagByWord;
   CWordTagPairMap m_mapTagByFirstChar;
   CWordTagPairMap m_mapTagByLastChar;
   CWordTagPairMap m_mapTagByPrevChar;
   CWordTagPairMap m_mapTagByNextChar;
   CWordTagPairMap m_mapTagByPrevTwoChar;
   CWordTagPairMap m_mapTagByNextTwoChar;
   CWordTagPairMap m_mapTagOfOneCharWord;
   CWordTagPairMap m_mapTagByChar;
   CWordTagPairMap m_mapRepeatedCharByTag;
   CWordTagPairMap m_mapTagByWordAndPrevChar;
   CWordTagPairMap m_mapTagByWordAndNextChar;
   CWordIntPairMap m_mapTagBySeparateChars;

   CTwoTaggedWordsMap m_mapTaggedCharByFirstChar;
   CTwoTaggedWordsMap m_mapTaggedCharByLastChar;

   CIntTagPairMap m_mapTagByFirstCharCat;
   CIntTagPairMap m_mapTagByLastCharCat;

   CWordIntMap m_mapWordFrequency;
   CTagDict<CWord, CTag> m_mapTagDictionary;
   CTagDict<CWord, CTag> m_mapCharTagDictionary;
   int m_nMaxWordFrequency;

public:

   // note that bSegmentationRules is ignored because segmented
   CWeight(const string &sFeatureDB, bool bTrain, bool bSegmentationRules) : 
            CWeightBase(sFeatureDB, bTrain) , 
            m_mapLastTagByTag("LastTagByTag", 1627) ,
            m_mapLastTwoTagsByTag("LastTwoTagsByTag", 16381) ,
            m_mapCurrentTag("CurrentTag", 65537) ,
            m_mapTagByLastWord("TagByLastWord", 65537) ,
            m_mapLastTagByWord("LastTagByWord", 65537) ,
            m_mapTagByFirstChar("TagByFirstChar", 65537) ,
            m_mapTagByLastChar("TagByLastChar", 65537) ,
            m_mapTagByPrevChar("TagByPrevChar", 65537) , 
            m_mapTagByNextChar("TagByNextChar", 65537) ,
            m_mapTagByPrevTwoChar("TagByPrevTwoChar", 65537) , 
            m_mapTagByNextTwoChar("TagByNextTwoChar", 65537) ,
            m_mapTagOfOneCharWord("TagOfOneCharWord", 65537) ,
            m_mapTagByChar("TagByChar", 65537) ,
            m_mapRepeatedCharByTag("RepeatedCharByTag", 65537) ,
            m_mapTagByWordAndPrevChar("TagByWordAndPrevChar", 65537) ,
            m_mapTagByWordAndNextChar("TagByWordAndNextChar", 65537) ,
            m_mapTagBySeparateChars("TagBySeparateChars", 65537) ,
            m_mapTaggedCharByFirstChar("TaggedCharByFirstChar", 65537) ,
            m_mapTaggedCharByLastChar("TaggedCharByLastChar", 65537) ,
            m_mapTagByFirstCharCat("TagByFirstCharCat", 65537) ,
            m_mapTagByLastCharCat("TagByLastCharCat", 65537) , 
            m_mapWordFrequency(65537),
            m_mapTagDictionary(PENN_TAG_COUNT) ,
            m_mapCharTagDictionary(PENN_TAG_COUNT)
   { 
      m_nMaxWordFrequency=0; loadScores();
   }

public:
  
   void loadScores();
   void saveScores();
   void computeAverageFeatureWeights(const unsigned long round);

};

} // namespace tagger
} // namespace chinese

#endif
