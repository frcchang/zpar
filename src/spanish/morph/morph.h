/*
 * morph.h
 *
 *  Created on: Dec 15, 2013
 *      Author: carlos
 */

//SPANISH_MORPH_H?
#ifndef MORPH_H_
#define MORPH_H_

#include <assert.h>

#include "definitions.h"
#include "pos/es09.h"
#include "linguistics/word_tokenized.h"
#include "linguistics/taggedword.h"
#include "linguistics/dependency.h"
#include "linguistics/conll.h"
#include "linguistics/tagset.h"
#include "tags.h"


namespace spanish
{


enum MORPH_FIELDS {

	//the numeric identifier of a field is also its offset in bits.
	//these fields are also the feature types.
	//for example a feature of type MORPH_VERB_TENSE with value MORPH_VERB_PRESENT can be extracted from the MORPH_VERB_TENSE bit field.

	MORPH_POSTAG = 0,
	MORPH_NOUN_TYPE = 4,
	MORPH_NUMBER = 6,
	MORPH_GENDER = 8,
	MORPH_VERB_TYPE = 10,
	MORPH_VERB_MOOD = 12,
	MORPH_VERB_TENSE = 15,
	MORPH_PERSON = 18,
//	MORPH_DEGREE = 20,
	MORPH_PRON_TYPE = 20,
	MORPH_CONTRACTED = 23,
	MORPH_DET_TYPE = 25

};

const int MORPH_FIELD_SIZE [] = {

	//MORPH_FIELD_SIZE[i] = length of the field that ends at bit i counting from the right hand side
	//zeros are for bits that don't correspond to the end of a field (thus, n-1 zeros after each n)

	4, 0,0,0, //postag
	2, 0, //noun type
	2, 0, //number
	2, 0, //gender
	2, 0, //verb type
	3, 0,0, //verb mood
	3, 0,0, //verb tense
	2, 0, //person
//	2, 0, //degree: not in spanish
	3, 0,0, //pronoun type (demostrative, indefinite, relative, personal, possessive)
	2 ,0, //contracted?
	3, 0,0 //determiner type
};

const int LAST_MORPH_FIELD = MORPH_DET_TYPE;
const int MORPH_BITS = LAST_MORPH_FIELD+MORPH_FIELD_SIZE[LAST_MORPH_FIELD]; //currently 28 bits.

const std::string MORPH_FIELD_STRINGS [] = {

	//MORPH_FIELD_SIZE[i] = name of the field that ends at bit i counting from the right hand side
	//empty strings are as in MORPH_FIELD_SIZE

	"POSTAG", "","","",
	"NOUN TYPE", "",
	"NUMBER", "",
	"GENDER", "",
	"VERB TYPE", "",
	"VERB MOOD", "","",
	"VERB TENSE", "","",
	"PERSON", "","",
	//"DEGREE", "",
	"PRON TYPE", "","",
	"CONTRACTED", "",
	"DET TYPE","",""

};

enum MORPH_NOUN_TYPES {

	MORPH_NOUN_TYPE_COMMON = 1,
	MORPH_NOUN_TYPE_PROPER,
	MORPH_NOUN_TYPE_COUNT

};

const std::string MORPH_NOUN_TYPE_STRINGS[] =
{
		"unspecified",
		"common",
		"proper"
};

enum MORPH_NUMBERS {

	MORPH_NUMBER_SINGULAR = 1,
	MORPH_NUMBER_PLURAL,
	MORPH_NUMBER_COUNT

};

const std::string MORPH_NUMBER_STRINGS[] =
{
		"unspecified",
		"singular",
		"plural"
};

enum MORPH_GENDERS {

	MORPH_GENDER_MASCULINE = 1,
	MORPH_GENDER_FEMININE,
	MORPH_GENDER_NEUTRAL,
	MORPH_GENDER_COUNT

};

const std::string MORPH_GENDER_STRINGS[] =
{
		"unspecified",
		"masculine",
		"feminine",
		"neutral"
};

enum MORPH_VERB_TYPES {

	MORPH_VERB_MAIN = 1,
	MORPH_VERB_AUXILIARY,
	MORPH_VERB_SEMIAUXILIARY,
	MORPH_VERB_TYPE_COUNT

};

const std::string MORPH_VERB_TYPE_STRINGS[] =
{
		"unspecified",
		"main",
		"auxiliary",
		"semiauxiliary"
};

enum MORPH_VERB_MOODS {

	MORPH_VERB_INDICATIVE=1,
	MORPH_VERB_SUBJUNCTIVE,
	MORPH_VERB_PASTPARTICIPLE,
	MORPH_VERB_INFINITIVE,
	MORPH_VERB_GERUND,
	MORPH_VERB_IMPERATIVE,
	MORPH_VERB_MOOD_COUNT

};

const std::string MORPH_VERB_MOOD_STRINGS[] =
{
		"unspecified",
		"indicative",
		"subjunctive",
		"participle",
		"infinitive",
		"gerund",
		"imperative"
};

enum MORPH_VERB_TENSES {

	MORPH_VERB_PRESENT = 1,
	MORPH_VERB_PAST,
	MORPH_VERB_FUTURE,
	MORPH_VERB_IMPERFECT,
	MORPH_VERB_CONDITIONAL,
	MORPH_VERB_TENSE_COUNT

};

const std::string MORPH_VERB_TENSE_STRINGS[] =
{
		"unspecified",
		"present",
		"past",
		"future",
		"imperfect",
		"conditional"
};

enum MORPH_PERSONS {

	MORPH_PERSON_FIRST = 1,
	MORPH_PERSON_SECOND,
	MORPH_PERSON_THIRD,
	MORPH_PERSON_COUNT
};

const std::string MORPH_PERSON_STRINGS[] =
{
		"unspecified",
		"first",
		"second",
		"third",
};

/*
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
*/

enum MORPH_PRON_TYPES {

	MORPH_PRON_TYPE_DEMONSTRATIVE = 1,
	MORPH_PRON_TYPE_INDEFINITE,
	MORPH_PRON_TYPE_RELATIVE,
	MORPH_PRON_TYPE_PERSONAL,
	MORPH_PRON_TYPE_POSSESSIVE,
	MORPH_PRON_TYPE_COUNT

};

const std::string MORPH_PRON_TYPE_STRINGS[] =
{

		"unspecified",
		"demonstrative",
		"indefinite",
		"relative",
		"personal",
		"possessive",

};

enum MORPH_CONTRACTEDS {

	MORPH_CONTRACTED_YES = 1,
	MORPH_CONTRACTED_NO,
	MORPH_CONTRACTED_COUNT

};

const std::string MORPH_CONTRACTED_STRINGS[] =
{

		"unspecified",
		"yes",
		"no"

};

enum MORPH_DET_TYPES {

	MORPH_DET_TYPE_ARTICLE = 1,
	MORPH_DET_TYPE_DEMONSTRATIVE,
	MORPH_DET_TYPE_INDEFINITE,
	MORPH_DET_TYPE_NUMERAL,
	MORPH_DET_TYPE_POSSESSIVE,
	MORPH_DET_TYPE_COUNT

};

const std::string MORPH_DET_TYPE_STRINGS[] =
{

		"unspecified",
		"article",
		"demonstrative",
		"indefinite",
		"numeral",
		"possessive",

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
 * Convert an AnCoRa word + POS tag + feats field into its morphological information.
 * Equivalent to the old english::pennToMorph(word, tag) which should be renamed now.
 */
CMorph conllToMorph(const std::string &word , const std::string &pennTag, const std::string &feats);

/**
 * Convert morphological information back into a POS tag.
 * Useful to evaluate tagging accuracy.
 */
CTag morphToTag(CMorph morph);


} //namespace spanish

#endif /* MORPH_H_ */
