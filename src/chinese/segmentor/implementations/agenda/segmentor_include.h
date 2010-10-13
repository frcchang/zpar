// Copyright (C) University of Oxford 2010
#ifdef NO_NEG_FEATURE
#define CScoreMapType CScoreMapNoNegFeatures
#else
#define CScoreMapType CScoreMap
#endif

#define SELF_TRAIN
#define SIMPLE_HASH

#include "agenda.h"
#include "pair_stream.h"
#include "linguistics/word_tokenized.h"
#include "learning/perceptron/hashmap_score_nnf.h"
#include "pool.h"


typedef double SCORE_TYPE;

/*===============================================================
 *
 * Configuration constants
 *
 *==============================================================*/
namespace chinese {

namespace segmentor {
//
// Beam segmentor specific item
//
const int BEAM_SIZE = 16;
//
// All segmentor must define the following
//
const int MAX_SENTENCE_SIZE = 1024; // restrict size of sentences 

//const int LENGTH_MAX_BITS = 3;
const int LENGTH_MAX_BITS = 4;
const int LENGTH_MAX = 1<<LENGTH_MAX_BITS;

};
};

