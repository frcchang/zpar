// Copyright (C) University of Oxford 2010
/***************************************************************
 *
 * weight.h - the dependency parser weights 
 *
 * Yue Zhang, 2007.8 - 2008.1
 *
 ***************************************************************/

#ifndef _DEPPARSER_IMPL_WEIGHTS_H
#define _DEPPARSER_IMPL_WEIGHTS_H

#include "depparser_weight_base.h"

namespace TARGET_LANGUAGE {

namespace depparser {

//
// TYPE DEFINITIONS
//
typedef CScoreMap< CWord, SCORE_TYPE > CWordMap;
typedef CScoreMap< CTaggedWord<CTag, TAG_SEPARATOR>, SCORE_TYPE > CTaggedWordMap;
typedef CScoreMap< std::pair<CWord, int>,  SCORE_TYPE > CWordIntMap;
typedef CScoreMap< std::pair<CTaggedWord<CTag, TAG_SEPARATOR>, int>,  SCORE_TYPE > CTaggedWordIntMap;
typedef CScoreMap< int, SCORE_TYPE > CIntMap;
typedef CScoreMap< std::pair<int, int>, SCORE_TYPE > CTwoIntMap;
typedef CScoreMap< CTwoWords, SCORE_TYPE > CTwoWordsMap;
typedef CScoreMap< std::pair<CTwoWords, int>, SCORE_TYPE > CTwoWordsIntMap;
typedef CScoreMap< CTwoTaggedWords, SCORE_TYPE > CTwoTaggedWordsMap;
typedef CScoreMap< std::pair<CTwoTaggedWords, int>, SCORE_TYPE > CTwoTaggedWordsIntMap;

/*===============================================================
 *
 * CWeight - the definition of weights, in feature
 *
 *==============================================================*/

class CWeight : public CWeightBase {

public:

   // feature templates from mcdonald
   CWordIntMap m_mapHeadWord;
   CWordIntMap m_mapDepWord;
   CTwoIntMap m_mapHeadTag;
   CTwoIntMap m_mapDepTag;
   CTaggedWordIntMap m_mapHeadWordTag;
   CTaggedWordIntMap m_mapDepWordTag;

   CTwoTaggedWordsIntMap m_mapHeadWordTagDepWordTag;
   CTwoTaggedWordsIntMap m_mapHeadWordTagDepWord;
   CTwoTaggedWordsIntMap m_mapHeadWordDepWordTag;
   CTwoTaggedWordsIntMap m_mapHeadTagDepWordTag;
   CTwoTaggedWordsIntMap m_mapHeadWordTagDepTag;
   CTwoWordsIntMap m_mapHeadWordDepWord;
   CTwoIntMap m_mapHeadTagDepTag;

   CTwoIntMap m_mapBetweenTags;
   CTwoIntMap m_mapSurroundingTagsLL;
   CTwoIntMap m_mapSurroundingTagsLR;
   CTwoIntMap m_mapSurroundingTagsRL;
   CTwoIntMap m_mapSurroundingTagsRR;

public:

   CWeight(const std::string &sPath, bool bTrain) : CWeightBase(sPath, bTrain) ,
                                               m_mapHeadWord("HeadWord", 122651) ,
                                               m_mapDepWord("DepWord", 122651) ,
                                               m_mapHeadTag("HeadTag", 122651) ,
                                               m_mapDepTag("DepTag", 122651) ,
                                               m_mapHeadWordTag("HeadWordTag", 122651) ,
                                               m_mapDepWordTag("DepWordTag", 122651) ,
                                               m_mapHeadWordTagDepWordTag("HeadWordTagDepWordTag", 122651) ,
                                               m_mapHeadWordTagDepWord("HeadWordTagDepWord", 122651) ,
                                               m_mapHeadWordTagDepTag("HeadWordTagDepTag", 122651) ,
                                               m_mapHeadWordDepWordTag("HeadWordDepWordTag", 122651) ,
                                               m_mapHeadTagDepWordTag("HeadTagDepWordTag", 122651) ,
                                               m_mapHeadWordDepWord("HeadWordDepWord", 122651) ,
                                               m_mapHeadTagDepTag("HeadTagDepTag", 122651) ,
                                               m_mapBetweenTags("BetweenTags", 122651) ,
                                               m_mapSurroundingTagsLL("SurroundingTagsLL", 122651) ,
                                               m_mapSurroundingTagsLR("SurroundingTagsLR", 122651) ,
                                               m_mapSurroundingTagsRL("SurroundingTagsRL", 122651) ,
                                               m_mapSurroundingTagsRR("SurroundingTagsRR", 122651)
   { 
      loadScores(); 
   }


   // MEHTODS
   virtual void loadScores(); 
   virtual void saveScores(); 
   void computeAverageFeatureWeights(int round);
 
};

inline long int hash(const int &i) {return i;}
inline long int hash(const std::pair<int, int> &pi) {return pi.first*31+pi.second;}
inline long int hash(const std::pair<CWord, int> &pwi) {return pwi.first.hash()+pwi.second*37;}
inline long int hash(const std::pair<CTwoWords, int> &ptw) {return ptw.first.hash()+ptw.second*37;}
inline long int hash(const std::pair<CTaggedWord<CTag, TAG_SEPARATOR>, int> &pwi) {return pwi.first.hash()+pwi.second*37;}
inline long int hash(const std::pair<CTwoTaggedWords, int> &ptwi) {return ptwi.first.hash()+ptwi.second*37;}

};
};

#endif
