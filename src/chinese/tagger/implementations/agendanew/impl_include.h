// Copyright (C) University of Oxford 2010
#ifndef _CHINESE_TAGGER_IMPLINCLUDE_H
#define _CHINESE_TAGGER_IMPLINCLUDE_H

#include "agendachart.h"
#include "linguistics/word_tokenized.h"
#include "linguistics/taggedword.h"
#include "learning/perceptron/score.h"
#include "learning/perceptron/hashmap_score.h"
#include "knowledge/tagdict.h"
#include "pair_stream.h"

namespace chinese {

typedef CBigram< CTaggedWord<CTag, TAG_SEPARATOR> > CTwoTaggedWords;
#include "charcat.h"

namespace tagger {

typedef double SCORE_TYPE ;

const int AGENDA_SIZE = 32;

const int MAX_SENTENCE_SIZE = 512; // restrict size of sentences 

const int LENGTH_MAX_BITS = 4;
const int LENGTH_MAX = 1<<LENGTH_MAX_BITS;

#include "segmentor/rules/basic.h"
}} // namespaces
#endif
