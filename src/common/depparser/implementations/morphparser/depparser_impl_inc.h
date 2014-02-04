// Copyright (C) University of Oxford 2010
#ifndef _DEPPARSER_IMPL_INCLUDE_H
#define _DEPPARSER_IMPL_INCLUDE_H

#include "agenda.h"
#include "pair_stream.h"

#include "learning/perceptron/hashmap_score_packed_map.h"

#include "bigram.h"
#include "tuple2.h"
#include "tuple3.h"
#include "tuple4.h"

#include "linguistics/lemma.h"

#include "morph/morph.h"
#include "morph/penn_lexicon.h"
#include "morph/aux_lexicon.h"

#ifdef PUNCT
#include "charcat.h"
#endif

#include "morph/morph.h"
#include "morph/penn_lexicon.h"
#include "morph/aux_lexicon.h"

namespace TARGET_LANGUAGE {
namespace depparser {
//label
#ifdef LABELED
const unsigned DEP_LABEL_COUNT=CDependencyLabel::MAX_COUNT;
const unsigned DEP_LABEL_SIZE=CDependencyLabel::SIZE;
#else
const unsigned DEP_LABEL_COUNT = 1;
const unsigned DEP_LABEL_SIZE=0;
#endif
#include "action.h"
}
}

typedef CBigram< CTaggedWord<TARGET_LANGUAGE::CTag, TARGET_LANGUAGE::TAG_SEPARATOR> > CTwoTaggedWords; 




#endif
