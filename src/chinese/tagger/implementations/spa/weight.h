// Copyright (C) University of Oxford 2010
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

#define ID(x) x
#define ID_LRB (
#define ID_RRB )
#define ID_COMMA ,
#define ID_POINTER ->
//
// TYPE DEFINITIONS
//
typedef CScoreMap< CWord, SCORE_TYPE > CWordMap;
typedef CScoreMap< CTwoWords, SCORE_TYPE > CTwoWordsMap;
typedef CScoreMap< std::pair<CWord, int>, SCORE_TYPE > CWordIntMap;
typedef CScoreMap< std::pair<CWord, CTag>, SCORE_TYPE > CWordTagMap;
//typedef CScoreMap< long int, SCORE_TYPE > CIntMap;
typedef CScoreMap< CTwoTaggedWords, SCORE_TYPE > CTwoTaggedWordsMap;
typedef CScoreMap< std::pair<unsigned long long, CTag>, SCORE_TYPE > CIntTagMap;
typedef CScoreMap< CTagSet<CTag, 2>, SCORE_TYPE > CTagSet2Map;
typedef CScoreMap< CTagSet<CTag, 3>, SCORE_TYPE > CTagSet3Map;
typedef CScoreMap< std::pair<CWord, CTagSet<CTag, 2> >, SCORE_TYPE > CWordTagSet2Map;
typedef CScoreMap< std::pair<CWord, CTagSet<CTag, 3> >, SCORE_TYPE > CWordTagSet3Map;
typedef CHashMap< CWord, int > CWordToIntMap;
typedef CScoreMap< std::pair<CTagSet<CTag, 2>, int>, SCORE_TYPE > CTagSet2IntMap;
typedef CScoreMap< std::pair<CTagSet<CTag, 3>, int>, SCORE_TYPE > CTagSet3IntMap;
//typedef CHashMap< std::pair<CWord, int>, int > CWordIntToIntMap;
//typedef CScoreMap< std::pair<long int, long int>, SCORE_TYPE > CIntPairMap;

/*===============================================================
 *
 * CWeight - the definition of weights, in feature
 *
 *==============================================================*/

class CWeight : public CWeightBase {

public:
   unsigned long m_maxLengthByTag[CTag::COUNT+1];
   CCharCatDictionary *m_Knowledge;
   unsigned long m_nMaxWordFrequency;
   CRule m_rules;

public:
   unsigned long getMaxWordLength() const {return m_maxLengthByTag[CTag::COUNT];}
   void setMaxLengthByTag(unsigned long tag, unsigned long length) {
      if (length<=m_maxLengthByTag[tag])
         return;
      m_maxLengthByTag[tag]=length;
      if (length>m_maxLengthByTag[CTag::COUNT])
         m_maxLengthByTag[CTag::COUNT]=length;
   }

   // feature templates about characters
   CWordIntMap m_mapCharUnigram;
   CWordIntMap m_mapCharBigram;
   CWordIntMap m_mapCharTrigram;

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
   CWordTagMap m_mapCurrentTag;
   CTagSet2Map m_mapLastTagByTag;
   CTagSet3Map m_mapLastTwoTagsByTag;
   CWordTagMap m_mapTagByLastWord;
   CWordTagMap m_mapLastTagByWord;
   CWordTagMap m_mapTagByFirstChar;
   CWordTagMap m_mapTagByLastChar;
   CWordTagMap m_mapTagByChar;
   CWordTagMap m_mapTagOfOneCharWord;
   CWordTagMap m_mapRepeatedCharByTag;
   CWordTagMap m_mapTagByWordAndPrevChar;
   CWordTagMap m_mapTagByWordAndNextChar;
   CTwoTaggedWordsMap m_mapTaggedCharByFirstChar;
   CTwoTaggedWordsMap m_mapTaggedCharByLastChar;

   // extra features
   CTwoTaggedWordsMap m_mapTaggedSeparateChars;
   CWordTagMap m_mapTaggedConsecutiveChars;

   CWordTagSet2Map m_mapWordTagTag;
   CWordTagSet2Map m_mapTagWordTag;
   CWordTagSet2Map m_mapFirstCharBy2Tags;
   CWordTagSet3Map m_mapFirstCharBy3Tags;
   CTwoWordsMap m_mapFirstCharAndChar;

   CWordMap m_mapSepCharAndNextChar;
   CWordMap m_mapAppCharAndNextChar;

   CWordMap m_mapPartialWord;
   CWordIntMap m_mapPartialLengthByFirstChar;
   CWordIntMap m_mapLengthByTagAndFirstChar;
   CWordIntMap m_mapLengthByTagAndLastChar;

   CTagSet2IntMap m_mapTag0Tag1Size1;
   CTagSet2IntMap m_mapTag1Tag2Size1;
   CTagSet3IntMap m_mapTag0Tag1Tag2Size1;

   // feature templates knowledge
   CIntTagMap m_mapTagByFirstCharCat;
   CIntTagMap m_mapTagByLastCharCat;

   CIntTagMap m_mapSeparateCharCat;
   CIntTagMap m_mapConsecutiveCharCat;

   // statistical information
   CWordToIntMap m_mapWordFrequency;
   CTagDict<CWord, CTag> m_mapTagDictionary;
   CTagDict<CWord, CTag> m_mapCharTagDictionary;
   CTagDict<CWord, CTag> m_mapCanStart;

public:
   // note that m_bSegmentation rules will be covered by load()
   // if there is a model file to load
   // therefore this argument only used first time training
   CWeight(const std::string &sFeatureDB, bool bTrain, bool bSegmentationRules, unsigned long size) :
            CWeightBase(sFeatureDB, bTrain) ,
            m_Knowledge(0) ,
            m_mapCharUnigram("CharacterUnigram", size) ,
            m_mapCharBigram("CharacterBigram", size) ,
            m_mapCharTrigram("CharacterTrigram", size) ,
            m_mapSeenWords("WordDictionary", size) ,
            m_mapLastWordByWord("LastWordByWord", size) ,
            m_mapCurrentWordLastChar("CurrentWordLastChar", size) ,
            m_mapLastWordFirstChar("LastWordFirstChar", size) ,
            m_mapFirstCharLastWordByWord("FirstCharLastWordByWord", size) ,
            m_mapLastWordByLastChar("LastWordByLastChar", size) ,
            m_mapSeparateChars("SeparateChars", size) ,
            m_mapConsecutiveChars("ConsecutiveChars", size) ,
            m_mapFirstAndLastChars("FirstAndLastChars", size) ,
            m_mapOneCharWord("OneCharWord", size) ,
            m_mapLengthByFirstChar("LengthByFirstChar", size) ,
            m_mapLengthByLastChar("LengthByLastChar", size) ,
            m_mapLengthByLastWord("LengthByLastWord", size) ,
            m_mapLastLengthByWord("LastLengthByWord", size) ,

            m_mapLastTagByTag("LastTagByTag", size) ,
            m_mapLastTwoTagsByTag("LastTwoTagsByTag", size) ,
            m_mapCurrentTag("CurrentTag", size) ,
            m_mapTagByLastWord("TagByLastWord", size) ,
            m_mapLastTagByWord("LastTagByWord", size) ,
            m_mapTagByFirstChar("TagByFirstChar", size) ,
            m_mapTagByLastChar("TagByLastChar", size) ,
            m_mapTagByChar("TagByChar", size) ,
            m_mapTagOfOneCharWord("TagOfOneCharWord", size) ,
            m_mapRepeatedCharByTag("RepeatedCharByTag", size) ,
            m_mapTagByWordAndPrevChar("TagByWordAndPrevChar", size) ,
            m_mapTagByWordAndNextChar("TagByWordAndNextChar", size) ,
            m_mapTaggedCharByFirstChar("TaggedCharByFirstChar", size) ,
            m_mapTaggedCharByLastChar("TaggedCharByLastChar", size) ,
            m_mapTagByFirstCharCat("TagByFirstCharCat", size) ,
            m_mapTagByLastCharCat("TagByLastCharCat", size) ,

            m_mapSeparateCharCat("SeparateCharCat", size) ,
            m_mapConsecutiveCharCat("ConsecutiveCharCat", size) ,

            m_mapTagDictionary(CTag::COUNT),
            m_mapCharTagDictionary(CTag::COUNT),
            m_mapCanStart(CTag::COUNT),
            m_mapWordFrequency(65537),

            m_mapTaggedSeparateChars("TaggedSeparateChars", size) ,
            m_mapTaggedConsecutiveChars("TaggedConsecutiveChars", size),

            m_mapWordTagTag("WordByTheNextTwoTag", size),
            m_mapTagWordTag("TagByNextWordSecondNextTag", size),
            m_mapFirstCharBy2Tags("FirstCharByTagAndLastTag", size),
            m_mapFirstCharBy3Tags("FirstCharByTagLastTagAndSecondLastTag", size),
            m_mapFirstCharAndChar("FirstCharAndChar", size),
            m_mapSepCharAndNextChar("SeparatedCharAndNextChar", size),
            m_mapAppCharAndNextChar("AppendedCharAndNextChar", size),
            m_mapPartialWord("PartialWord", size),
            m_mapPartialLengthByFirstChar("PartialLengthByFirstChar", size),
            m_mapLengthByTagAndFirstChar("LengthByTagAndFirstChar", size) ,
            m_mapLengthByTagAndLastChar("LengthByTagAndLastChar", size) ,
            m_mapTag0Tag1Size1("Tag0Tag1Size1", size),
            m_mapTag1Tag2Size1("Tag1Tag2Size1", size),
            m_mapTag0Tag1Tag2Size1("Tag0Tag1Tag2Size1", size)
   {
      for (unsigned i=0; i<=CTag::COUNT; ++i) m_maxLengthByTag[i] = 1;
      m_nMaxWordFrequency=0;
      if(bSegmentationRules)newKnowledge();
      loadScores();
   }

   virtual ~CWeight() { if (m_Knowledge) delete m_Knowledge; }

public:

   void loadScores();
   void saveScores();
   void computeAverageFeatureWeights(unsigned long round);

   void addCurrent(CWeight *w, const int &round);
   void scaleCurrent(const SCORE_TYPE& scale, const int &round);
   SCORE_TYPE norm2();

   void clear();

   void newKnowledge() {
      std::cerr << "set character knowledge... " << std::endl;
      ASSERT(m_Knowledge==0, "CTagger::loadKnowledge: knowledge already loaded.");
      m_Knowledge = new CCharCatDictionary();
      m_rules.setKnowledge(m_Knowledge);
   }
};

} // namespace tagger
} // namespace chinese

#endif
