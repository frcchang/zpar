// Copyright (C) University of Oxford 2010
#ifndef _DEPPARSER_INCLUDE_H
#define _DEPPARSER_INCLUDE_H

#include "bigram.h"
#include "linguistics/word_tokenized.h"
#include "tags.h"
#include "linguistics/taggedword.h"
#include "agenda.h"
#include "pair_stream.h"

#include "learning/perceptron/score.h"
#include "learning/perceptron/hashmap_score.h"

typedef CBigram< CTaggedWord<TARGET_LANGUAGE::CTag, TARGET_LANGUAGE::TAG_SEPARATOR> > CTwoTaggedWords; 

typedef CDependencyTree CDependencyParse;

#endif
