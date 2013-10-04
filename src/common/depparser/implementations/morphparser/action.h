#ifndef DEPPARSER_ACTION_H
#define DEPPARSER_ACTION_H

#include "morph/morph.h"

namespace action {

// SH RE AL AR PR [AL+LABEL] AR+LABEL
//enum STACK_ACTION { NO_ACTION=0, SHIFT, REDUCE, ARC_LEFT, ARC_RIGHT, POP_ROOT, LABEL_AL_FIRST, LABEL_AR_FIRST=LABEL_AL_FIRST+DEP_LABEL_COUNT-1, MAX=LABEL_AR_FIRST+DEP_LABEL_COUNT-1 };

//Action codes. Additionally, arc actions can have an associated label, and shift actions an associated morph.
enum STACK_ACTION { NO_ACTION = 0 , SHIFT , SHIFT_CACHE, REDUCE , ARC_LEFT , ARC_RIGHT , POP_ROOT };
const int ACTION_TYPE_SIZE = 3; //bits for the action codes

const int MAX = 1; //this constant is unused because we use hashmap_score_packed_map, which needs it to be there (at the moment) but doesn't really use it

enum ACTION_FIELDS {

	ACTION_TYPE = 0,
	LEFT_ACTION_LABEL = ACTION_TYPE + ACTION_TYPE_SIZE,
	RIGHT_ACTION_LABEL = LEFT_ACTION_LABEL + DEP_LABEL_SIZE,
	SHIFT_CACHE_ACTION_MORPH = RIGHT_ACTION_LABEL + DEP_LABEL_SIZE,
	ACTION_CODE_SIZE = SHIFT_CACHE_ACTION_MORPH + MORPH_BITS

};

//returns -1 for invalid fields
static unsigned getFieldSize ( ACTION_FIELDS field )
{
	if ( field == ACTION_TYPE ) return ACTION_TYPE_SIZE;
	else if ( field == LEFT_ACTION_LABEL || field == RIGHT_ACTION_LABEL ) return DEP_LABEL_SIZE;
	else if ( field == SHIFT_CACHE_ACTION_MORPH ) return MORPH_BITS;
	else return -1;
}

//get the value of an action field
static unsigned long getField ( unsigned long encodedAction , ACTION_FIELDS field )
{
	int fieldSize = getFieldSize(field);
	assert(fieldSize >= 0);
	unsigned long result = encodedAction >> field;
	unsigned long mask = ( (1ul << fieldSize) - 1);
	return result & mask;
}

//clear an action field to a blank (zero) value
static void clearField ( unsigned long & encodedAction , ACTION_FIELDS field )
{
	int fieldSize = getFieldSize(field);
	assert(fieldSize >= 0);
	unsigned long mask = ( (1ul << fieldSize) - 1) << field; //1's only in that field
	encodedAction = encodedAction & (~mask); //clear the field to zeros
}

//set the value in an action field
static void setField ( unsigned long & encodedAction , ACTION_FIELDS field , unsigned long value )
{
	unsigned fieldSize = getFieldSize(field);
	assert(fieldSize >= 0);
	clearField(encodedAction,field);
	encodedAction = encodedAction | ( value << field );
}


static unsigned long encodeAction(const STACK_ACTION &action, const unsigned long &labelOrMorph=0) {
	unsigned long result = 0;
	setField ( result , ACTION_TYPE , action );
#ifdef LABELED
	if (action==ARC_LEFT)
	{
		assert(labelOrMorph<DEP_LABEL_COUNT);
		setField ( result , LEFT_ACTION_LABEL , labelOrMorph );
	}
	if (action==ARC_RIGHT)
	{
		assert(labelOrMorph<DEP_LABEL_COUNT);
		setField ( result , RIGHT_ACTION_LABEL , labelOrMorph );
	}
#endif
	if (action==SHIFT_CACHE)
	{
//		std::cout << "Before encoding: " << labelOrMorph << "\n";
		setField ( result , SHIFT_CACHE_ACTION_MORPH , labelOrMorph );
//		std::cout << "Encoded: " << result << "\n";
	}
	return result;
}


static unsigned long getUnlabeledAction( const unsigned long &action ) {
	unsigned long result = action;
#ifdef LABELED
	clearField ( result , LEFT_ACTION_LABEL );
	clearField ( result , RIGHT_ACTION_LABEL );
#endif
	clearField ( result , SHIFT_CACHE_ACTION_MORPH );
	return result;
}


//in unlabelled parsing, this can still be used to obtain the morph info of a shift cache transition
static unsigned long getLabelOrMorph(const unsigned long &action) {
	int actionType = getField(action,ACTION_TYPE);
#ifdef LABELED
	if ( actionType == ARC_LEFT ) return getField(action,LEFT_ACTION_LABEL);
	if ( actionType == ARC_RIGHT ) return getField(action,RIGHT_ACTION_LABEL);
#endif
	if ( actionType == SHIFT_CACHE )
	{
		//std::cout << "Before decoding: " << action << "\n";
		//std::cout << "Decoded: " << getField(action,SHIFT_CACHE_ACTION_MORPH) << "\n";
		return getField(action,SHIFT_CACHE_ACTION_MORPH);
	}
	return 0;
}

struct CScoredAction {
	unsigned long action;
	SCORE_TYPE score;
public:
	bool operator < (const CScoredAction &a) const {
		return score < a.score;
	}
	bool operator > (const CScoredAction &a) const {
		return score > a.score;
	}
};

}//namespace
#endif
