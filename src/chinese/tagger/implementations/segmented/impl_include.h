// Copyright (C) University of Oxford 2010
#ifndef _CHINESE_TAGGER_IMPLINCLUDE_H
#define _CHINESE_TAGGER_IMPLINCLUDE_H

#include "agenda.h"
#include "linguistics/word_tokenized.h"
#include "linguistics/taggedword.h"
#include "learning/perceptron/score.h"
#include "learning/perceptron/hashmap_score.h"
#include "knowledge/tagdict.h"
#include "pair_stream.h"

namespace chinese {

typedef CBigram< CTaggedWord<CTag, TAG_SEPARATOR> > CTwoTaggedWords ; 

} // namespaces
#endif
