#include "definitions.h"
#include "utils.h"
namespace TARGET_LANGUAGE { 
namespace depparser { 
#include "depparser_macros.h" 
}
}

#include "linguistics/dependency.h"
#ifdef LABELED
#include "linguistics/dependencylabel.h"
#endif
#include "depparser_impl_inc.h"
#include "linguistics/tagset.h"
#include "options.h"

#include "supertag.h"
