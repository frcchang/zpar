#ifndef _DEPPARSER_IMPL_INCLUDE_H
#define _DEPPARSER_IMPL_INCLUDE_H

#include "tags.h"
namespace TARGET_LANGUAGE { 
namespace depparser { 
#include "macros.h" 
}
}

#include "bigram.h"
#include "linguistics/word_tokenized.h"
#include "tuple3.h"
#include "tuple4.h"
#include "linguistics/taggedword.h"
#include "agenda.h"
#include "pair_stream.h"

#include "learning/perceptron/score.h"
#include "learning/perceptron/hashmap_score.h"

typedef CBigram< CTaggedWord<TARGET_LANGUAGE::CTag, TARGET_LANGUAGE::TAG_SEPARATOR> > CTwoTaggedWords; 

#ifdef LABELED
   typedef CLabeledDependencyTree CDependencyParse;
#else
   typedef CDependencyTree CDependencyParse;
#endif



#endif
