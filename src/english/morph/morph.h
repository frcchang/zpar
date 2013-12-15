/*
 * morph.h
 *
 *  Created on: Sep 9, 2013
 *      Author: carlos
 */

#ifndef MORPH_H_
#define MORPH_H_

#include <assert.h>

#include "definitions.h"
#include "pos/penn.h"
#include "linguistics/word_tokenized.h"
#include "linguistics/taggedword.h"
#include "linguistics/dependency.h"
#include "linguistics/conll.h"
#include "linguistics/tagset.h"
#include "tags.h"

#include "morph_pos.h"


namespace english
{


enum MORPH_FIELDS {

	//the numeric identifier of a field is also its offset in bits.
	//these fields are also the feature types.
	//for example a feature of type MORPH_VERB_TENSE with value MORPH_VERB_PRESENT can be extracted from the MORPH_VERB_TENSE bit field.

	MORPH_POSTAG = 0,
	MORPH_NOUN_TYPE = 6,
	MORPH_NUMBER = 8,
	MORPH_GENDER = 10,
	MORPH_VERB_TYPE = 12,
	MORPH_VERB_FORM = 14,
	MORPH_VERB_TENSE = 17,
	MORPH_PERSON = 19,
	MORPH_DEGREE = 22,
	MORPH_IS_POSS = 24,
	MORPH_IS_WH = 26

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

const int LAST_MORPH_FIELD = MORPH_IS_WH;
const int MORPH_BITS = LAST_MORPH_FIELD+2; //can be -2 instead of +2 for optimization, as actually last 4 bits (is-possessive and wh) are unused at the moment.
//If instead of 2 I type MORPH_FIELD_SIZE[LAST_MORPH_FIELD] I get error in action.h: "MORPH_BITS cannot appear in a constant expression". Not sure why, as all of that is const.

const std::string MORPH_FIELD_STRINGS [] = {

	//MORPH_FIELD_SIZE[i] = name of the field that ends at bit i counting from the right hand side
	//empty strings are as in MORPH_FIELD_SIZE

	"POSTAG", "","","","","",
	"NOUN TYPE", "",
	"NUMBER", "",
	"GENDER", "",
	"VERB TYPE", "",
	"VERB FORM", "","",
	"VERB TENSE", "",
	"PERSON", "","",
	"DEGREE", "",
	"IS POSS", "",
	"IS WH", ""

};

enum MORPH_NOUN_TYPES {

	MORPH_NOUN_TYPE_COMMON = 1,
	MORPH_NOUN_TYPE_PROPER,
	MORPH_NOUN_TYPE_COUNT

};

const std::string MORPH_NOUN_TYPE_STRINGS[] =
{
		"Unspecified",
		"Common",
		"Proper"
};

enum MORPH_NUMBERS {

	MORPH_NUMBER_SINGULAR = 1,
	MORPH_NUMBER_PLURAL,
	MORPH_NUMBER_COUNT

};

const std::string MORPH_NUMBER_STRINGS[] =
{
		"Unspecified",
		"Singular",
		"Plural"
};

enum MORPH_GENDERS {

	MORPH_GENDER_MASCULINE = 1,
	MORPH_GENDER_FEMININE,
	MORPH_GENDER_NEUTRAL,
	MORPH_GENDER_COUNT

};

const std::string MORPH_GENDER_STRINGS[] =
{
		"Unspecified",
		"Masculine",
		"Feminine",
		"Neutral"
};

enum MORPH_VERB_TYPES {

	MORPH_VERB_MAIN = 1,
	MORPH_VERB_MODAL,
	MORPH_VERB_TYPE_COUNT

};

const std::string MORPH_VERB_TYPE_STRINGS[] =
{
		"Unspecified",
		"Main",
		"Modal"
};

enum MORPH_VERB_FORMS {

	MORPH_VERB_PERSONAL = 1, //indicative/subjunctive: VBP, VBZ, VBD
	MORPH_VERB_INFINITIVE, //base form (VB)
	MORPH_VERB_GERUND, //VBG
	MORPH_VERB_PARTICIPLE, //VBN
	MORPH_VERB_FORM_COUNT

};

const std::string MORPH_VERB_FORM_STRINGS[] =
{
		"Unspecified",
		"Personal Form",
		"Infinitive",
		"Gerund",
		"Participle"
};

enum MORPH_VERB_TENSES {

	MORPH_VERB_PRESENT = 1,
	MORPH_VERB_PAST,
	MORPH_VERB_TENSE_COUNT

};

const std::string MORPH_VERB_TENSE_STRINGS[] =
{
		"Unspecified",
		"Present",
		"Past"
};

enum MORPH_PERSONS {

	MORPH_PERSON_FIRST = 1,
	MORPH_PERSON_SECOND,
	MORPH_PERSON_THIRD,
	MORPH_PERSON_NOT_THIRD, //VBP
	MORPH_PERSON_COUNT
};

const std::string MORPH_PERSON_STRINGS[] =
{
		"Unspecified",
		"First",
		"Second",
		"Third",
		"Not Third"
};

enum MORPH_DEGREES {

	MORPH_DEGREE_POSITIVE = 1,
	MORPH_DEGREE_SUPERLATIVE,
	MORPH_DEGREE_COMPARATIVE,
	MORPH_DEGREE_COUNT

};

const std::string MORPH_DEGREE_STRINGS[] =
{
		"Unspecified",
		"Positive",
		"Superlative",
		"Comparative"
};

enum MORPH_IS_POSS_VALUES { //unused at the moment. Give distinct values if we want to use it.

	MORPH_NON_POSS = 0,
	MORPH_POSS = 0,
	MORPH_IS_POSS_VALUE_COUNT

};

const std::string MORPH_IS_POSS_VALUE_STRINGS[] =
{
		"Unspecified",
		"Non possesive",
		"Possesive"
};

enum MORPH_IS_WH_VALUES { //unused at the moment. Give distinct values if we want to use it.

	MORPH_NON_WH = 0,
	MORPH_WH = 0,
	MORPH_IS_WH_VALUE_COUNT

};

const std::string MORPH_IS_WH_VALUE_STRINGS[] =
{
		"Unspecified",
		"Non WH"
		"WH"
};



class CMorphFeat
{

private:

	//create a vector of vectors for field value names.
	static bool fieldValueNamesInitted;
	static std::vector< std::vector<std::string> > fieldValueNames;

	static void initFieldValueNames ();

public:

	static std::string getFieldValueName ( const MORPH_FIELDS field , const int value );

	int featId;
	unsigned long featValue;

public:

	CMorphFeat() : featId(0), featValue(0) {}
	CMorphFeat( int featId , unsigned long featValue ) : featId(featId) , featValue(featValue) {}

	virtual ~CMorphFeat() {}

	unsigned long hash() const { return ((unsigned long)featId)*37 + featValue; }

	void load(const int &id , const unsigned long & value) { featId = id; featValue = value; }

public:

   bool operator == (const CMorphFeat &f1) const { return (featId == f1.featId && featValue == f1.featValue); }
   bool operator != (const CMorphFeat &f1) const { return (featId != f1.featId || featValue != f1.featValue); }
   bool operator < (const CMorphFeat &f1) const { return ( featId < f1.featId ) || ( featId == f1.featId && featValue < f1.featValue ); }
   bool operator > (const CMorphFeat &f1) const { return ( featId > f1.featId ) || ( featId == f1.featId && featValue > f1.featValue ); }
   bool operator <= (const CMorphFeat &f1) const { return ( *this == f1 || *this < f1 ); }
   bool operator >= (const CMorphFeat &f1) const { return ( *this == f1 || *this > f1 ); }

   std::string str() const
   {
	   std::stringstream st;
	   st << MORPH_FIELD_STRINGS[featId] << " = " << getFieldValueName((MORPH_FIELDS)featId,featValue);
	   return st.str();
   }

};




class CMorph
{

	unsigned long morphInfo;

public:

	enum {NONE=0};

	CMorph() : morphInfo(0) {}
	CMorph(unsigned long morphInfo) : morphInfo(morphInfo) { }

	virtual ~CMorph() {}

	unsigned long code() const { return morphInfo; }
	unsigned long hash() const { return morphInfo; }
	void copy(const CMorph &m) { morphInfo = m.morphInfo; }

	void load(const unsigned long &i) { morphInfo = i; }

public:

   bool operator == (const CMorph &m1) const { return morphInfo == m1.morphInfo; }
   bool operator != (const CMorph &m1) const { return morphInfo != m1.morphInfo; }
   bool operator < (const CMorph &m1) const { return morphInfo < m1.morphInfo; }
   bool operator > (const CMorph &m1) const { return morphInfo > m1.morphInfo; }
   bool operator <= (const CMorph &m1) const { return morphInfo <= m1.morphInfo; }
   bool operator >= (const CMorph &m1) const { return morphInfo >= m1.morphInfo; }


public:

   //get the value of a morphological information field
   unsigned long getField ( MORPH_FIELDS field ) const
   {
	   assert(MORPH_FIELD_SIZE[field] > 0);
	   unsigned long result = morphInfo >> field;
	   unsigned long mask = ( (1ul << MORPH_FIELD_SIZE[field]) - 1);
	   return result & mask;
   }

   //clear a morphological information field to a blank (zero) value
   void clearField ( MORPH_FIELDS field )
   {
	   assert(MORPH_FIELD_SIZE[field] > 0);
	   unsigned long mask = ( (1ul << MORPH_FIELD_SIZE[field]) - 1) << field; //1's only in that field
	   morphInfo = morphInfo & (~mask); //clear the field to zeros
   }

   //set the value in a morphological information field
   void setField ( MORPH_FIELDS field , unsigned long value )
   {
	   assert(MORPH_FIELD_SIZE[field] > 0);
	   clearField(field);
	   morphInfo = morphInfo | ( value << field );
   }

   std::vector<CMorphFeat> getFeatureVector() const
   {
	   std::vector<CMorphFeat> result;
	   for ( int field = MORPH_POSTAG ; field <= LAST_MORPH_FIELD ; field++ )
	   {
		   if ( MORPH_FIELD_SIZE[field] > 0 )
		   {
			   unsigned long val = getField ( (MORPH_FIELDS)field );
			   if ( val != 0ul ) result.push_back( CMorphFeat(field,val) );
		   }
	   }
	   return result;
   }

   std::string str() const
   {
	   std::vector<CMorphFeat> featVector = getFeatureVector();
	   std::stringstream stream;
	   for ( int i = 0 ; i < featVector.size() ; i++ )
	   {
		   stream << featVector[i].str() << ", ";
	   }
	   return stream.str();
   }

};

inline std::istream & operator >> (std::istream &is, CMorph &m) {
	  unsigned long code;
	  is >> code;
	  m.load(code);
   return is ;
}

inline std::ostream & operator << (std::ostream &os, const CMorph &m) {
   os << m.code();
   return os ;
}


/**
 * Convert a Penn Treebank word + POS tag into its morphological information.
 * The word is used only for some known words.
 * Use an empty word
 * Pass the empty string as the word if only tag information is to be used for morphological analysis.
 * The feats parameter is ignored since the English Penn treebank has no feats field, but it is included because it is used in other languages.
 */
CMorph conllToMorph(const std::string &word , const std::string &pennTag, const std::string &feats);

/**
 * Convert morphological information back into a Penn Treebank POS tag.
 * Useful to evaluate tagging accuracy.
 */
CTag morphToPenn(CMorph morph);


} //namespace english

#endif /* MORPH_H_ */
