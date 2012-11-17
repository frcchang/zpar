// Copyright (C) University of Oxford 2010
/***************************************************************
 *
 * meta_weight.h - the dependency parser meta weights 
 *
 * Yue Zhang, 2007.8 - 2008.1
 *
 ***************************************************************/

#ifndef _DEPPARSER_META_WEIGHTS_H
#define _DEPPARSER_META_WEIGHTS_H

namespace TARGET_LANGUAGE {

namespace depparser {

typedef CScoreMap<int, SCORE_TYPE> CMetaMap;

/*===============================================================
 *
 * CMetaWeight - the definition of meta weights, in feature
 *
 *==============================================================*/

class CMetaWeight {

public:

   CMetaMap m_mapSTw;
   CMetaMap m_mapSTt;
   CMetaMap m_mapSTwt;

   CMetaMap m_mapN0w;
   CMetaMap m_mapN0t;
   CMetaMap m_mapN0wt;

   CMetaMap m_mapN1w;
   CMetaMap m_mapN1t;
   CMetaMap m_mapN1wt;

   CMetaMap m_mapN2w;
   CMetaMap m_mapN2t;
   CMetaMap m_mapN2wt;

   CMetaMap m_mapSTHw;
   CMetaMap m_mapSTHt;
   CMetaMap m_mapSTi;

   CMetaMap m_mapSTHHw;
   CMetaMap m_mapSTHHt;
   CMetaMap m_mapSTHi;

   CMetaMap m_mapSTLDw;
   CMetaMap m_mapSTLDt;
   CMetaMap m_mapSTLDi;

   CMetaMap m_mapSTRDw;
   CMetaMap m_mapSTRDt;
   CMetaMap m_mapSTRDi;

   CMetaMap m_mapN0LDw;
   CMetaMap m_mapN0LDt;
   CMetaMap m_mapN0LDi;

   CMetaMap m_mapSTL2Dw;
   CMetaMap m_mapSTL2Dt;
   CMetaMap m_mapSTL2Di;

   CMetaMap m_mapSTR2Dw;
   CMetaMap m_mapSTR2Dt;
   CMetaMap m_mapSTR2Di;

   CMetaMap m_mapN0L2Dw;
   CMetaMap m_mapN0L2Dt;
   CMetaMap m_mapN0L2Di;

   CMetaMap m_mapHTw;
   CMetaMap m_mapHTt;
   CMetaMap m_mapHTwt;

   CMetaMap m_mapSTwtN0wt;
   CMetaMap m_mapSTwtN0w;
   CMetaMap m_mapSTwN0wt;
   CMetaMap m_mapSTtN0wt;
   CMetaMap m_mapSTwtN0t;
   CMetaMap m_mapSTwN0w;
   CMetaMap m_mapSTtN0t;

   CMetaMap m_mapN0tN1t;
   CTagSet3Map CMetaMap;
   CMetaMap m_mapSTtN0tN1t;
   CMetaMap m_mapSTtN0tN0LDt;
   CMetaMap m_mapN0tN0LDtN0L2Dt;
   CMetaMap m_mapSTHtSTtN0t;
   CMetaMap m_mapHTtHT2tN0t;
   CMetaMap m_mapSTHHtSTHtSTt;
   CMetaMap m_mapSTtSTLDtN0t;
   CMetaMap m_mapSTtSTLDtSTL2Dt;
   CMetaMap m_mapSTtSTRDtN0t;
   CMetaMap m_mapSTtSTRDtSTR2Dt;

   CMetaMap m_mapSTwd;
   CMetaMap m_mapSTtd;
   CMetaMap m_mapN0wd;
   CMetaMap m_mapN0td;
   CMetaMap m_mapSTwN0wd;
   CMetaMap m_mapSTtN0td;

   CMetaMap m_mapSTwra;
   CMetaMap m_mapSTtra;
   CMetaMap m_mapSTwla;
   CMetaMap m_mapSTtla;
   CMetaMap m_mapN0wla;
   CMetaMap m_mapN0tla;

   CMetaMap m_mapSTwrp;
   CMetaMap m_mapSTtrp;
   CMetaMap m_mapSTwlp;
   CMetaMap m_mapSTtlp;
   CMetaMap m_mapN0wlp;
   CMetaMap m_mapN0tlp;

   CMetaMap m_mapSTl;
   CMetaMap m_mapSTc;
   CMetaMap m_mapSTf;

   CMetaMap m_mapN0l;
   CMetaMap m_mapN0c;
   CMetaMap m_mapN0f;

   CMetaMap m_mapN1l;
   CMetaMap m_mapN1c;
   CMetaMap m_mapN1f;
 
};

};
};

#endif
