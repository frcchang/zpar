// Copyright (C) University of Oxford 2010
#ifndef _CONPARSER_IMPL_INCLUDE_H
#define _CONPARSER_IMPL_INCLUDE_H

#define SIMPLE_HASH

namespace TARGET_LANGUAGE {
#ifdef NO_TEMP_CONSTITUENTT
typedef CConstituentLabel CConstituent;
#else
#include "linguistics/cfgtemp.h"
typedef CTemporary<CConstituentLabel> CConstituent;
#endif
};

#include "hash.h"
#include "bigram.h"
#include "tuple3.h"
#include "tuple4.h"
#include "vector_stream.h"
#include "linguistics/word_tokenized.h"
#include "linguistics/taggedword.h"
#include "linguistics/sentence_multicon.h"
#include "agenda.h"
#include "pair_stream.h"
#ifdef _CHINESE_CFG_H
#include "charcat.h"
#endif
#include "knowledge/tagdict.h"

#include "learning/perceptron/score.h"
#include "learning/perceptron/hashmap_score_packed.h"

#define CScoreMapType CPackedScoreMap

typedef CBigram< CTaggedWord<TARGET_LANGUAGE::CTag, TARGET_LANGUAGE::TAG_SEPARATOR> > CTwoTaggedWords; 

typedef TARGET_LANGUAGE::CJointTree CSentenceParsed;

#include "labeledbracket.h"
#include "stack.h"

namespace TARGET_LANGUAGE { 

#include "linguistics/cfgset.h"
#include "wordcache.h"

namespace conparser { 

typedef long SCORE_TYPE ;

// labeled brackets
typedef CTuple3<int, int, CConstituent> CBracketTuple;
typedef CHashMap<CBracketTuple, unsigned> CBracketTupleMap;

#include "stateitem.h" 
#include "context.h"

}

}

#include "rule.h"

#endif
