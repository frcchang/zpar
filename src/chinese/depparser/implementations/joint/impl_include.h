#ifndef _CHINESE_TAGGER_IMPLINCLUDE_H
#define _CHINESE_TAGGER_IMPLINCLUDE_H

#include "agendachart.h"
#include "linguistics/word_tokenized.h"
#include "linguistics/taggedword.h"
#include "learning/perceptron/score.h"
#include "learning/perceptron/hashmap_score.h"
#include "knowledge/tagdict.h"
#include "pair_stream.h"

namespace chinese { namespace depparser { 
#include "macros.h"
}}

namespace chinese {

typedef CBigram< CTaggedWord<CTag> > CTwoTaggedWords;
#include "charcat.h"

} // namespaces
#endif
