// Copyright (C) University of Oxford 2010
#ifndef _DEPPARSER_IMPL_INCLUDE_H
#define _DEPPARSER_IMPL_INCLUDE_H

#include "agenda.h"
#include "pair_stream.h"

#include "learning/perceptron/score.h"
#include "learning/perceptron/hashmap_score.h"

#include "bigram.h"
#include "tuple3.h"
#include "tuple4.h"

#include "tags.h"
#include "linguistics/word_tokenized.h"
#include "linguistics/taggedword.h"
#include "dep.h"

#ifdef PUNCT
#include "charcat.h"
#endif

typedef CBigram< CTaggedWord<TARGET_LANGUAGE::CTag, TARGET_LANGUAGE::TAG_SEPARATOR> > CTwoTaggedWords; 

#ifdef LABELED
   typedef CLabeledDependencyTree CDependencyParse;
#else
   typedef CDependencyTree CDependencyParse;
#endif



#endif
