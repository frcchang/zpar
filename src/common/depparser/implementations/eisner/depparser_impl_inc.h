// Copyright (C) University of Oxford 2010
#ifndef _DEPPARSER_INCLUDE_H
#define _DEPPARSER_INCLUDE_H

#include "tags.h"
#include "hash.h"
#include "linguistics/word_tokenized.h"
#include "learning/perceptron/score.h"
#include "learning/perceptron/hashmap_score.h"
#include "pair_stream.h"
#include "linguistics/taggedword.h"

typedef CDependencyTree CDependencyParse;

namespace TARGET_LANGUAGE { 

typedef CBigram< CTaggedWord<CTag, TAG_SEPARATOR> > CTwoTaggedWords ;

}

#endif
