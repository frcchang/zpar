/*
 * morph.h
 *
 *  Created on: Sep 9, 2013
 *      Author: carlos
 */

#ifndef MORPH_H_
#define MORPH_H_

#include <assert.h>


#include "morph_pos.h"


namespace english
{


enum MORPH_FIELDS {

	//the numeric identifier of a field is also its offset in bits

	MORPH_POSTAG = 0,
	MORPH_NOUN_TYPE = 6,
	MORPH_NUMBER = 8,
	MORPH_GENDER = 10,
	MORPH_VERB_TYPE = 12,
	MORPH_VERB_FORM = 14,
	MORPH_VERB_TENSE = 17,
	MORPH_PERSON = 19,
	MORPH_DEGREE = 22,
	MORPH_POSS = 24,
	MORPH_WH = 26

};

const int MORPH_FIELD_SIZE [] = {

	//MORPH_FIELD_SIZE[i] = length of the field that ends at bit i counting from the right hand side
	//zeros are for bits that don't correspond to the end of a field (thus, n-1 zeros after each n)

	6, 0,0,0,0,0,
	2, 0,
	2, 0,
	2, 0,
	2, 0,
	3, 0,0,
	2, 0,
	3, 0,0,
	2, 0,
	2, 0,
	2, 0
};

enum MORPH_NOUN_TYPE {

	MORPH_NOUN_TYPE_COMMON = 1,
	MORPH_NOUN_TYPE_PROPER

};

enum MORPH_NUMBER {

	MORPH_NUMBER_SINGULAR = 1,
	MORPH_NUMBER_PLURAL

};

enum MORPH_GENDER {

	MORPH_GENDER_MASCULINE = 1,
	MORPH_GENDER_FEMININE,
	MORPH_GENDER_NEUTRAL

};

//TODO continue


class CMorph
{

	long morphInfo;

};




} //namespace english

#endif /* MORPH_H_ */
