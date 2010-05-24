#ifndef _CONPARSER_IMPL_INCLUDE_H
#define _CONPARSER_IMPL_INCLUDE_H

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
#include "learning/perceptron/hashmap_score.h"

typedef CBigram< CTaggedWord<TARGET_LANGUAGE::CTag, TARGET_LANGUAGE::TAG_SEPARATOR> > CTwoTaggedWords; 

typedef TARGET_LANGUAGE::CCFGTree CSentenceParsed;

namespace TARGET_LANGUAGE { 

#include "linguistics/cfgset.h"

namespace conparser { 

typedef double SCORE_TYPE ;

#include "stateitem.h" 
#include "macros.h" 
#include "context.h"

}

}

#include "rule.h"

#endif
