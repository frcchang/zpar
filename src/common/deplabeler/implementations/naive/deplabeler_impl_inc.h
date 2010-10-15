// Copyright (C) University of Oxford 2010
#ifndef _DEPLABELER_IMPL_INCLUDE_H
#define _DEPLABELER_IMPL_INCLUDE_H

#include "tags.h"
#include "bigram.h"
#include "linguistics/word_tokenized.h"
#include "tuple3.h"
#include "tuple4.h"
#include "linguistics/taggedword.h"
#include "dep.h"
#include "agenda.h"
#include "pair_stream.h"

#include "learning/perceptron/score.h"
#include "learning/perceptron/hashmap_score.h"

typedef CBigram< CTaggedWord<TARGET_LANGUAGE::CTag, TARGET_LANGUAGE::TAG_SEPARATOR> > CTwoTaggedWords; 

#endif
