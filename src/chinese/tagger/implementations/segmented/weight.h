// Copyright (C) University of Oxford 2010
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
typedef CScoreMap< std::pair<CWord, int>, SCORE_TYPE > CWordIntPairMap;
typedef CScoreMap< std::pair<CWord, CTag>, SCORE_TYPE > CWordTagPairMap;
typedef CScoreMap< std::pair<int, int>, SCORE_TYPE > CIntPairMap;
typedef CScoreMap< CTwoTaggedWords, SCORE_TYPE > CTwoTaggedWordsMap;
typedef CScoreMap< CTagSet<CTag, 2>, SCORE_TYPE > CTagSet2Map;
typedef CScoreMap< CTagSet<CTag, 3>, SCORE_TYPE > CTagSet3Map;
typedef CScoreMap< std::pair<long int, CTag>, SCORE_TYPE > CIntTagPairMap;

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
   CWeight(const std::string &sFeatureDB, bool bTrain, bool bSegmentationRules, int size) :
            CWeightBase(sFeatureDB, bTrain) , 
            m_mapLastTagByTag("LastTagByTag", size) ,
            m_mapLastTwoTagsByTag("LastTwoTagsByTag", size) ,
            m_mapCurrentTag("CurrentTag", size) ,
            m_mapTagByLastWord("TagByLastWord", size) ,
            m_mapLastTagByWord("LastTagByWord", size) ,
            m_mapTagByFirstChar("TagByFirstChar", size) ,
            m_mapTagByLastChar("TagByLastChar", size) ,
            m_mapTagByPrevChar("TagByPrevChar", size) ,
            m_mapTagByNextChar("TagByNextChar", size) ,
            m_mapTagByPrevTwoChar("TagByPrevTwoChar", size) ,
            m_mapTagByNextTwoChar("TagByNextTwoChar", size) ,
            m_mapTagOfOneCharWord("TagOfOneCharWord", size) ,
            m_mapTagByChar("TagByChar", size) ,
            m_mapRepeatedCharByTag("RepeatedCharByTag", size) ,
            m_mapTagByWordAndPrevChar("TagByWordAndPrevChar", size) ,
            m_mapTagByWordAndNextChar("TagByWordAndNextChar", size) ,
            m_mapTagBySeparateChars("TagBySeparateChars", size) ,
            m_mapTaggedCharByFirstChar("TaggedCharByFirstChar", size) ,
            m_mapTaggedCharByLastChar("TaggedCharByLastChar", size) ,
            m_mapTagByFirstCharCat("TagByFirstCharCat", size) ,
            m_mapTagByLastCharCat("TagByLastCharCat", size) ,
            m_mapWordFrequency(size),
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
