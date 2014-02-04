// Copyright (C) University of Oxford 2010
#define SIMPLE_HASH
#include "definitions.h"
#include "utils.h"
#include "linguistics/word_tokenized.h"
#include "linguistics/taggedword.h"
#include "linguistics/dependency.h"
#ifdef LABELED
#include "linguistics/dependencylabel.h"
#endif

namespace TARGET_LANGUAGE { 
namespace depparser { 
#include "depparser_macros.h" 
}
}

#include "tags.h"
#include "dep.h"
#include "depparser_impl_inc.h"

#ifdef JOINT_MORPH
#ifdef LABELED
   typedef CExtendedLabeledDependencyTree CDependencyParse;
#else
   typedef CExtendedDependencyTree CDependencyParse;
#endif
#else
#ifdef LABELED
   typedef CLabeledDependencyTree CDependencyParse;
#else
   typedef CDependencyTree CDependencyParse;
#endif
#endif

#include "linguistics/conll.h"
#include "linguistics/tagset.h"
#include "options.h"

#include "supertag.h"
