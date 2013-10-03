/*
 * aux_lexicon.h
 *
 *  Created on: Oct 1, 2013
 *      Author: carlos
 */

#ifndef AUX_LEXICON_H_
#define AUX_LEXICON_H_

#include "definitions.h"
#include "pos/penn.h"
#include "writer.h"
#include "reader.h"
#include "linguistics/word_tokenized.h"
#include "linguistics/taggedword.h"
#include "linguistics/dependency.h"
#include "linguistics/conll.h"
#include "linguistics/tagset.h"
#include "tags.h"
#include "morph.h"
#include "morph_pos.h"
#include "penn_lexicon.h"

namespace english
{
bool initLemmaLexicon ( const std::string sInputFile );
std::string getLemma ( const std::string & form , const CMorphTag & morphTag );
std::string getLemma ( const std::string & form , const CMorph & morph );
}


#endif /* AUX_LEXICON_H_ */
