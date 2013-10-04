/*
 * penn_lexicon.h
 *
 *  Created on: Sep 30, 2013
 *      Author: carlos
 */

#ifndef PENN_LEXICON_H_
#define PENN_LEXICON_H_

#include "definitions.h"
#include "pos/penn.h"
#include "reader.h"
#include "writer.h"
#include "linguistics/word_tokenized.h"
#include "linguistics/taggedword.h"
#include "linguistics/dependency.h"
#include "linguistics/conll.h"
#include "linguistics/tagset.h"

#include "morph.h"

#include "tags.h"

namespace english
{

bool initLexicon ( const std::string sInputFile , bool bCoNLL );
bool isKnown ( const std::string & word );
std::set<CTag> getTagsForUnknownWord ( const std::string & word );
std::set<CTag> getTagsForNoneWord ( );
std::set<CTag> getPossibleTags ( const std::string & word );
std::set<CMorph> getPossibleMorph ( const std::string & word );

}


#endif /* PENN_LEXICON_H_ */
