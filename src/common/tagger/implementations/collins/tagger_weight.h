// Copyright (C) University of Oxford 2010
/***************************************************************
 *
 * The weights for the General POS tagging model
 *
 * Yue Zhang, 2007.10
 *
 ***************************************************************/

#ifndef _COMMON_TAGGER_WEIGHTS_H
#define _COMMON_TAGGER_WEIGHTS_H

#include "tagger_include.h"

#include "tagger_weight_base.h"

namespace TARGET_LANGUAGE {
namespace tagger {

#include "macros.h"

#define iterate_templates(left, right)\
   left(m_mapLastTagByTag)right\
   left(m_mapLastTwoTagsByTag)right\
\
   left(m_mapCurrentTag)right\
   left(m_mapTagByPrevWord)right\
   left(m_mapTagByNextWord)right\
   left(m_mapTagBySecondPrevWord)right\
   left(m_mapTagBySecondNextWord)right\
\
   left(m_mapContainHyphen)right\
   left(m_mapContainNumber)right\
   left(m_mapContainCapitalLetter)right\
   left(m_mapTagByPrefix)right\
   left(m_mapTagBySuffix)right\
\
   left(m_mapTagByTopTag0)right\
   left(m_mapTagByTopTagR1)right\
   left(m_mapTagByTopTag0L1)right\
   left(m_mapTagByTopTag0L1L2)right\
   left(m_mapTagByTopTag0R1)right\
   left(m_mapTagByTopTag0R1R2)right\
   left(m_mapTagByTopTag0L1R1)right

//
// TYPE DEFINITIONS
//
typedef CPackedScoreMap< CWord, SCORE_TYPE, CTag::MAX_COUNT > CWordMap;
typedef CPackedScoreMap< CTagSet<CTag, 2>, SCORE_TYPE, CTag::MAX_COUNT > CTagSet2Map;
typedef CPackedScoreMap< CTagSet<CTag, 3>, SCORE_TYPE, CTag::MAX_COUNT > CTagSet3Map;
typedef CPackedScoreMap< CTag, SCORE_TYPE, CTag::MAX_COUNT > CTagMap;
typedef CPackedScoreMap< unsigned, SCORE_TYPE, CTag::MAX_COUNT > CUnsignedMap;

/*===============================================================
 *
 * CWeight - the definition of weights, in feature
 *
 *==============================================================*/

class CWeight : public CWeightBase {

public:
   // FEATURE TEMPLATES 
   CTagMap m_mapLastTagByTag;
   CTagSet2Map m_mapLastTwoTagsByTag;

   CWordMap m_mapCurrentTag;
   CWordMap m_mapTagByPrevWord;
   CWordMap m_mapTagByNextWord;
   CWordMap m_mapTagBySecondPrevWord;
   CWordMap m_mapTagBySecondNextWord;

   CUnsignedMap m_mapContainHyphen;
   CUnsignedMap m_mapContainNumber;
   CUnsignedMap m_mapContainCapitalLetter;
   CWordMap m_mapTagByPrefix;
   CWordMap m_mapTagBySuffix;

   CTagMap m_mapTagByTopTag0;
   CTagMap m_mapTagByTopTagR1;
   CTagSet2Map m_mapTagByTopTag0L1;
   CTagSet3Map m_mapTagByTopTag0L1L2;
   CTagSet2Map m_mapTagByTopTag0R1;
   CTagSet3Map m_mapTagByTopTag0R1R2;
   CTagSet3Map m_mapTagByTopTag0L1R1;

   // CONSTRUCTOR 
   CWeight(const std::string &sFeatureDB, bool bTrain) : CWeightBase(sFeatureDB, bTrain) , 
                                                    m_mapLastTagByTag("TagBigram", 128),
                                                    m_mapLastTwoTagsByTag("TagTrigram", 65536),
                                                    m_mapCurrentTag("TagUnigram", 65536),
                                                    m_mapTagByPrevWord("Tag and previous word", 65536),
                                                    m_mapTagByNextWord("Tag and next word", 65536),
                                                    m_mapTagBySecondPrevWord("Tag and second previous", 65536),
                                                    m_mapTagBySecondNextWord("Tag and second next", 65536),
                                                    m_mapContainHyphen("Contains hyphen", 2),
                                                    m_mapContainNumber("Contains number", 2),
                                                    m_mapContainCapitalLetter("ontains capitalized letter", 2),
                                                    m_mapTagByPrefix("Tag by prefix", 65536),
                                                    m_mapTagBySuffix("Tag by suffix", 65536), 

                                                    m_mapTagByTopTag0("Tag by toptag0", 65536),
                                                    m_mapTagByTopTagR1("Tag by toptagr1", 65536),
                                                    m_mapTagByTopTag0L1("Tag by toptag0, l1", 65536),
                                                    m_mapTagByTopTag0L1L2("Tag by toptag0, l1, l2", 65536),
                                                    m_mapTagByTopTag0R1("Tag by toptag0, r1", 65536),
                                                    m_mapTagByTopTag0R1R2("Tag by toptag0, r1, r2", 65536),
                                                    m_mapTagByTopTag0L1R1("Tag by toptag0, l1, r1", 65536)
   { /* do nothing */ }
   virtual ~CWeight() {
//      iterate_templates(,.freePoolMemory(););
//      CPackedScore<SCORE_TYPE, CTag::MAX_COUNT>::freePoolMemory();
   }

   // MEHTODS
   void loadScores(); 
   void saveScores(); 
   void computeAverageFeatureWeights(int round);
 
};

};
};

#endif
