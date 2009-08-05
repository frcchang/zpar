#ifndef _DEPPARSER_INCLUDE_H
#define _DEPPARSER_INCLUDE_H

#include "tags.h"
#include "hash.h"
#include "linguistics/word_tokenized.h"
#include "learning/perceptron/score.h"
#include "learning/perceptron/hashmap_score.h"
#include "pair_stream.h"
#include "linguistics/taggedword.h"

typedef CDependencyTree CSentenceParsed;

namespace TARGET_LANGUAGE { 

typedef CBigram< CTaggedWord<CTag> > CTwoTaggedWords ;

namespace depparser{

#include "macros.h"

}
}

#endif
