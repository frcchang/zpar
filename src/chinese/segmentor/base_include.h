#include "definitions.h"
#include "utils.h"
#include "file_utils.h"
#include "segmentor_include.h" // global includes must be defined here instead of in segmentor_impl.h, for namesapce
#include "stateitem.h"

#include "learning/perceptron/score.h"

#include "charcat.h"
#include "knowledge/lexicon.h"

namespace chinese {
namespace segmentor {
#include "prune.h"
#include "segmentor_impl.h" // defines implementation - SCORE_TYPE etc

}
}

