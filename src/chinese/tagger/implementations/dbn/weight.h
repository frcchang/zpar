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

//
// TYPE DEFINITIONS
//
typedef CScoreMap< CWord, SCORE_TYPE > CWordMap;
typedef CScoreMap< CTwoWords, SCORE_TYPE > CTwoWordsMap;
typedef CScoreMap< std::pair<CWord, int>, SCORE_TYPE > CWordIntMap;
typedef CScoreMap< std::pair<int, int>, SCORE_TYPE > CIntIntMap;
typedef CScoreMap< CCopyBitArray, SCORE_TYPE > CBitArrayMap;
typedef CScoreMap< std::pair<CWord, CTag>, SCORE_TYPE > CWordTagMap;
typedef CScoreMap< CTwoTaggedWords, SCORE_TYPE > CTwoTaggedWordsMap;
typedef CScoreMap< std::pair<unsigned long long, CTag>, SCORE_TYPE > CIntTagMap;
typedef CScoreMap< CTagSet<CTag, 2>, SCORE_TYPE > CTagSet2Map;
typedef CScoreMap< CTagSet<CTag, 3>, SCORE_TYPE > CTagSet3Map;
typedef CScoreMap< std::pair<CWord, CTagSet<CTag, 2> >, SCORE_TYPE > CWordTagSet2Map;
typedef CScoreMap< std::pair<CWord, CTagSet<CTag, 3> >, SCORE_TYPE > CWordTagSet3Map;
typedef CScoreMap< std::pair<CTagSet<CTag, 2>, int>, SCORE_TYPE > CTagSet2IntMap;
typedef CScoreMap< std::pair<CTagSet<CTag, 3>, int>, SCORE_TYPE > CTagSet3IntMap;
typedef CHashMap< CWord, int > CWordToIntMap;
typedef CHashMap< CWord, CBitArray > CWordToBitArrayMap;

/*===============================================================
 *
 * CWeight - the definition of weights, in feature
 *
 *==============================================================*/

class CWeight : public CWeightBase {

public:
   unsigned long m_maxLengthByTag[CTag::COUNT+1];
   CWordToBitArrayMap *m_Knowledge;
   std::ofstream *m_dump;
   unsigned long m_nMaxWordFrequency;
   CRule m_rules;
   CBitArray m_bitnone;
   CBitArray m_bitunknown;
   CWord m_emptyWord;
   DBN *m_dbn;
   bool m_bKnowledge;
   bool m_bFWOrCD;

public:

   unsigned long getMaxWordLength() const {return m_maxLengthByTag[CTag::COUNT];}

   void setMaxLengthByTag(unsigned long tag, unsigned long length) {
      if (length<=m_maxLengthByTag[tag])
         return;
      m_maxLengthByTag[tag]=length;
      if (length>m_maxLengthByTag[CTag::COUNT])
         m_maxLengthByTag[CTag::COUNT]=length;
   }

   // feature template about on-linear feature instances
   CIntIntMap m_mapNonLinear;

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
   CWeight(const std::string &sFeatureDB, bool bTrain, bool bFWorCD, int hash_table_size) :
            CWeightBase(sFeatureDB, bTrain) ,
            m_dump(0) ,
            m_Knowledge(0) ,
            m_rules(bFWorCD),
            m_bitnone(BIT_CHAR_COUNT),
            m_bitunknown(BIT_CHAR_COUNT) ,
            m_emptyWord("") ,
            m_dbn(0), // DBN
            m_bKnowledge(false),
            m_bFWOrCD(bFWorCD),

            m_mapCharUnigram("CharacterUnigram", 65537) ,
            m_mapCharBigram("CharacterBigram", 65537) ,
            m_mapCharTrigram("CharacterTrigram", 65537) ,
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

            m_mapTagDictionary(CTag::COUNT),
            m_mapCharTagDictionary(CTag::COUNT),
            m_mapCanStart(CTag::COUNT),
            m_mapWordFrequency(65537),

            m_mapTaggedSeparateChars("TaggedSeparateChars", 65537) ,
            m_mapTaggedConsecutiveChars("TaggedConsecutiveChars", 65537),

            m_mapWordTagTag("WordByTheNextTwoTag", 65537),
            m_mapTagWordTag("TagByNextWordSecondNextTag", 65537),
            m_mapFirstCharBy2Tags("FirstCharByTagAndLastTag", 65537),
            m_mapFirstCharBy3Tags("FirstCharByTagLastTagAndSecondLastTag", 65537),
            m_mapFirstCharAndChar("FirstCharAndChar", 65537),
            m_mapSepCharAndNextChar("SeparatedCharAndNextChar", 65537),
            m_mapAppCharAndNextChar("AppendedCharAndNextChar", 65537),
            m_mapPartialWord("PartialWord", 65537),
            m_mapPartialLengthByFirstChar("PartialLengthByFirstChar", 65537),
            m_mapLengthByTagAndFirstChar("LengthByTagAndFirstChar", 65537) ,
            m_mapLengthByTagAndLastChar("LengthByTagAndLastChar", 65537) ,
            m_mapTag0Tag1Size1("Tag0Tag1Size1", 65537),
            m_mapTag1Tag2Size1("Tag1Tag2Size1", 65537),
            m_mapTag0Tag1Tag2Size1("Tag0Tag1Tag2Size1", 65537),
            m_mapNonLinear("NoneLinear", 65537)
   {
      for (unsigned i=0; i<=CTag::COUNT; ++i) m_maxLengthByTag[i] = 1;
      m_nMaxWordFrequency=0;
      m_bitnone.setsize(BIT_CHAR_COUNT);
      m_bitunknown.setsize(BIT_CHAR_COUNT);
      m_bitunknown.set(0);
      newKnowledge();
      loadScores();
   }

   virtual ~CWeight() {
      if (m_Knowledge)
         delete m_Knowledge;
      if (m_dbn)
         delete m_dbn;
   }

public:

   void loadScores();
   void saveScores();
   void computeAverageFeatureWeights(unsigned long round);
   void dumpFeature(const std::string &s);

   void newKnowledge() {
      std::cerr << "set character knowledge... " << std::endl;
      ASSERT(m_Knowledge==0, "CTagger::loadKnowledge: knowledge already loaded.");
      m_Knowledge = new CWordToBitArrayMap(65537);
      (*m_Knowledge)[m_emptyWord] = m_bitnone;
      m_dbn = new DBN();
   }

   const CBitArray &getBitArray(const CWord &w) const {
      assert(m_Knowledge!=0);
      return m_Knowledge->find(w, m_bitunknown);
   }
};

} // namespace tagger
} // namespace chinese

#endif
