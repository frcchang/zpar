/***************************************************************
 *
 * weight.h - the dependency labeler weights 
 *
 * Yue Zhang, 2007.8 - 2008.1
 *
 ***************************************************************/

#ifndef _DEPLABELER_WEIGHTS_H
#define _DEPLABELER_WEIGHTS_H

#include "weight_base.h"

namespace TARGET_LANGUAGE {

namespace deplabeler {

#include "macros.h"

//
// TYPE DEFINITIONS
//
typedef CScoreMap<CWord, SCORE_TYPE> CWordMap;
typedef CScoreMap<CTaggedWord<CTag, TAG_SEPARATOR>, SCORE_TYPE> CTaggedWordMap;
typedef CScoreMap<pair<CWord, int>,  SCORE_TYPE> CWordIntMap;
typedef CScoreMap<pair<CTag, int>,  SCORE_TYPE> CTagIntMap;
typedef CScoreMap<pair<CTagSet<CTag, 2>, int>,  SCORE_TYPE> CTagSet2IntMap;
typedef CScoreMap<pair<CTagSet<CTag, 3>, int>,  SCORE_TYPE> CTagSet3IntMap;
typedef CScoreMap<pair<CTagSet<CTag, 4>, int>,  SCORE_TYPE> CTagSet4IntMap;
typedef CScoreMap<CTuple3<CWord, CTag, unsigned long>, SCORE_TYPE> CWordTagIntMap;
typedef CScoreMap<CTuple4<CWord, CTag, CTag, unsigned long>, SCORE_TYPE> CWordTagTagIntMap;
typedef CScoreMap<pair<CTaggedWord<CTag, TAG_SEPARATOR>, int>,  SCORE_TYPE> CTaggedWordIntMap;
typedef CScoreMap<int, SCORE_TYPE> CIntMap;
typedef CScoreMap<pair<int, int>, SCORE_TYPE> CTwoIntMap;
typedef CScoreMap<CTwoWords, SCORE_TYPE> CTwoWordsMap;
typedef CScoreMap<pair<CTwoWords, int>, SCORE_TYPE> CTwoWordsIntMap;
typedef CScoreMap<CTwoTaggedWords, SCORE_TYPE> CTwoTaggedWordsMap;
typedef CScoreMap<pair<CTwoTaggedWords, int>, SCORE_TYPE> CTwoTaggedWordsIntMap;

/*===============================================================
 *
 * CWeight - the definition of weights, in feature
 *
 *==============================================================*/

class CWeight : public CWeightBase {

public:

   CIntMap m_mapLabel;
   CWordIntMap m_mapHeadWordLabel;
   CWordIntMap m_mapDepWordLabel;
   CTaggedWordIntMap m_mapHeadWordTagLabel;
   CTaggedWordIntMap m_mapDepWordTagLabel;
   CTwoIntMap m_mapHeadTagLabel;
   CTwoIntMap m_mapDepTagLabel;
   CTwoIntMap m_mapHeadSurroundingTagsLabel;
   CTwoIntMap m_mapDepSurroundingTagsLabel;

public:

   CWeight(const string &sPath, bool bTrain) : CWeightBase(sPath, bTrain) ,

                                               m_mapLabel("Label", 122651),
                                               m_mapHeadWordLabel("HeadWordLabel", 122651),
                                               m_mapDepWordLabel("DepWordLabel", 122651),
                                               m_mapHeadWordTagLabel("HeadWordTagLabel", 122651),
                                               m_mapDepWordTagLabel("DepWordTagLabel", 122651),
                                               m_mapHeadTagLabel("HeadTagLabel", 122651),
                                               m_mapDepTagLabel("DepTagLabel", 122651),
                                               m_mapHeadSurroundingTagsLabel("HeadSurroundingTagsLabel", 122651),
                                               m_mapDepSurroundingTagsLabel("DepSurroundingTagsLabel", 122651)
   { loadScores(); }


   // MEHTODS
   virtual void loadScores();
   virtual void saveScores();
   void computeAverageFeatureWeights(int round);
   SCORE_TYPE dotProduct(const CWeight &w);
 
};

};
};

#endif
