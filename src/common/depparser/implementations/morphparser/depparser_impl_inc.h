// Copyright (C) University of Oxford 2010
#ifndef _DEPPARSER_IMPL_INCLUDE_H
#define _DEPPARSER_IMPL_INCLUDE_H

#include "agenda.h"
#include "pair_stream.h"

#include "learning/perceptron/hashmap_score_packed.h"

#include "bigram.h"
#include "tuple2.h"
#include "tuple3.h"
#include "tuple4.h"

#include "linguistics/lemma.h"

#ifdef PUNCT
#include "charcat.h"
#endif

#include "morph/morph.h"
#include "morph/penn_lexicon.h"
#include "morph/aux_lexicon.h"


typedef CBigram< CTaggedWord<TARGET_LANGUAGE::CTag, TARGET_LANGUAGE::TAG_SEPARATOR> > CTwoTaggedWords; 




#endif
