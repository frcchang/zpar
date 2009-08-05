#ifndef _CHINESE_TAGGER_IMPLINCLUDE_H
#define _CHINESE_TAGGER_IMPLINCLUDE_H

#include "agenda.h"
#include "linguistics/word_tokenized.h"
#include "linguistics/taggedword.h"
#include "learning/perceptron/score.h"
#include "learning/perceptron/hashmap_score.h"
#include "knowledge/tagdict.h"
#include "pair_stream.h"

#define SEGMENTED 1

namespace chinese {

typedef CBigram< CTaggedWord<CTag> > CTwoTaggedWords ; 

namespace tagger {

typedef double SCORE_TYPE ;

//
// Specific global
//
const int AGENDA_SIZE = 16;
//
// General definitions for any method tagger.
//
const int MAX_SENTENCE_SIZE = 512; // restrict size of sentences 


}} // namespaces
#endif
