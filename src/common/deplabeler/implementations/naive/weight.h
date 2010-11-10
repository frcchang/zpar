// Copyright (C) University of Oxford 2010
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

//#include "macros.h"

//
// TYPE DEFINITIONS
//
typedef CScoreMap<std::pair<CDependencyLabel, int>, SCORE_TYPE> CLabelIntMap;
typedef CScoreMap<CTuple3<CWord, CDependencyLabel, int>,  SCORE_TYPE> CWordLabelIntMap;
typedef CScoreMap<CTuple3<CTaggedWord<CTag, TAG_SEPARATOR>, CDependencyLabel, int>,  SCORE_TYPE> CTaggedWordLabelIntMap;
typedef CScoreMap<CTuple3<CTag, CDependencyLabel, int>, SCORE_TYPE> CTagLabelIntMap;
typedef CScoreMap<CTuple3<CTagSet<CTag,3>, CDependencyLabel, int>, SCORE_TYPE> CTagSet3LabelIntMap;

/*===============================================================
 *
 * CWeight - the definition of weights, in feature
 *
 *==============================================================*/

class CWeight : public CWeightBase {

public:

   CLabelIntMap m_mapLabel;
   CWordLabelIntMap m_mapHeadWordLabel;
   CWordLabelIntMap m_mapDepWordLabel;
   CTaggedWordLabelIntMap m_mapHeadWordTagLabel;
   CTaggedWordLabelIntMap m_mapDepWordTagLabel;
   CTagLabelIntMap m_mapHeadTagLabel;
   CTagLabelIntMap m_mapDepTagLabel;
   CTagSet3LabelIntMap m_mapHeadSurroundingTagsLabel;
   CTagSet3LabelIntMap m_mapDepSurroundingTagsLabel;

//Int
public:

   CWeight(const std::string &sPath, bool bTrain) : CWeightBase(sPath, bTrain) ,

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
