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
typedef CScoreMap< pair<int, int>, SCORE_TYPE > CIntPairMap;
typedef CScoreMap< CTwoTaggedWords, SCORE_TYPE > CTwoTaggedWordsMap;

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
   CIntPairMap m_mapLastTagByTag;
   CIntPairMap m_mapLastTwoTagsByTag;

   CWordIntPairMap m_mapCurrentTag;
   CWordIntPairMap m_mapTagByLastWord;
   CWordIntPairMap m_mapLastTagByWord;
   CWordIntPairMap m_mapTagByFirstChar;
   CWordIntPairMap m_mapTagByLastChar;
   CWordIntPairMap m_mapTagByPrevChar;
   CWordIntPairMap m_mapTagByNextChar;
   CWordIntPairMap m_mapTagByPrevTwoChar;
   CWordIntPairMap m_mapTagByNextTwoChar;
   CWordIntPairMap m_mapTagOfOneCharWord;
   CWordIntPairMap m_mapTagByChar;
   CWordIntPairMap m_mapRepeatedCharByTag;
   CWordIntPairMap m_mapTagByWordAndPrevChar;
   CWordIntPairMap m_mapTagByWordAndNextChar;
   CWordIntPairMap m_mapTagBySeparateChars;

   CTwoTaggedWordsMap m_mapTaggedCharByFirstChar;
   CTwoTaggedWordsMap m_mapTaggedCharByLastChar;

   CIntPairMap m_mapTagByFirstCharCat;
   CIntPairMap m_mapTagByLastCharCat;

   CWordIntMap m_mapWordFrequency;
   CTagDict<CTag> m_mapTagDictionary;
   CTagDict<CTag> m_mapCharTagDictionary;
   int m_nMaxWordFrequency;

public:

   CWeight(const string &sFeatureDB, bool bTrain) : CWeightBase(sFeatureDB, bTrain) , 
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
                                                    m_mapTagByFirstCharCat("TagByFirstCharCat", 65531) ,
                                                    m_mapTagByLastCharCat("TagByLastCharCat", 65531) , 
                                                    m_mapTagDictionary(PENN_TAG_COUNT) ,
                                                    m_mapCharTagDictionary(PENN_TAG_COUNT)
   { 
      m_nMaxWordFrequency=0; loadScores();
   }

public:
  
   void loadScores();
   void saveScores();
   void computeAverageFeatureWeights(int round);

};

} // namespace tagger
} // namespace chinese

#endif
