/***************************************************************
 *
 * The weights for the English POS tagging model
 *
 * Yue Zhang, 2007.10
 *
 ***************************************************************/

#ifndef ENGLISH_TAGGER_WEIGHTS_H
#define ENGLISH_TAGGER_WEIGHTS_H 1

#include "tagger_include.h"

#include "weight_base.h"

namespace english {
namespace tagger {

#include "tagger_impl.h"
#include "stateitem.h"

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
   left(m_mapTagBySuffix)right

//
// TYPE DEFINITIONS
//
typedef CScoreMap< CWord, SCORE_TYPE > CWordMap;
typedef CScoreMap< CTwoWords, SCORE_TYPE > CTwoWordsMap;
typedef CScoreMap< unsigned long int, SCORE_TYPE > CIntMap;
typedef CScoreMap< pair<unsigned long int, unsigned long int>, SCORE_TYPE > CTwoIntMap;
typedef CScoreMap< pair<CWord, unsigned long int>, SCORE_TYPE > CWordIntMap;

//typedef CHashMap< CWord, int > CWordIntMap;

/*===============================================================
 *
 * CWeight - the definition of weights, in feature
 *
 *==============================================================*/

struct CWeight : public CWeightBase {

   // FEATURE TEMPLATES 
   CTwoIntMap m_mapLastTagByTag;
   CTwoIntMap m_mapLastTwoTagsByTag;

   CWordIntMap m_mapCurrentTag;
   CWordIntMap m_mapTagByPrevWord;
   CWordIntMap m_mapTagByNextWord;
   CWordIntMap m_mapTagBySecondPrevWord;
   CWordIntMap m_mapTagBySecondNextWord;

   CIntMap m_mapContainHyphen;
   CIntMap m_mapContainNumber;
   CIntMap m_mapContainCapitalLetter;
   CWordIntMap m_mapTagByPrefix;
   CWordIntMap m_mapTagBySuffix;

   // CONSTRUCTOR 
   CWeight(const string &sFeatureDB, bool bTrain) : CWeightBase(sFeatureDB, bTrain) , 
                                                    m_mapLastTagByTag("TagBigram", 65537),
                                                    m_mapLastTwoTagsByTag("TagTrigram", 65537),
                                                    m_mapCurrentTag("TagUnigram", 65537),
                                                    m_mapTagByPrevWord("Tag and previous word", 65537),
                                                    m_mapTagByNextWord("Tag and next word", 65537),
                                                    m_mapTagBySecondPrevWord("Tag and second previous", 65537),
                                                    m_mapTagBySecondNextWord("Tag and second next", 65537),
                                                    m_mapContainHyphen("Contains hyphen", 65537),
                                                    m_mapContainNumber("Contains number", 65537),
                                                    m_mapContainCapitalLetter("ontains capitalized letter", 65537),
                                                    m_mapTagByPrefix("Tag by prefix", 65537),
                                                    m_mapTagBySuffix("Tag by suffix", 65537) {}

   // MEHTODS
   void loadScores(); 
   void saveScores(); 
   void computeAverageFeatureWeights(int round);
 
};

};
};

#endif
