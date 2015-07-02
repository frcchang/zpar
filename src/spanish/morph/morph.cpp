#include "morph.h"

namespace spanish
{


	/**
	 * Convert a string representing a POS tag read from the CoNLL file into one of our morphological analyzer's POS tags.
	 */
	int conllToMorphTag(const std::string &conllTag)
	{

		if ( conllTag.length() < 1 ) return ES09_TAG_NONE;

		switch (conllTag[0])
		{
			case 'a': return ES09_TAG_ADJ;
			case 'c': return ES09_TAG_CONJ;
			case 'd': return ES09_TAG_DET;
			case 'f': return ES09_TAG_PUNCT;
			case 'i': return ES09_TAG_INTERJ;
			case 'n': return ES09_TAG_NOUN;
			case 'p': return ES09_TAG_PRON;
			case 'r': return ES09_TAG_ADV;
			case 's': return ES09_TAG_PREP;
			case 'v': return ES09_TAG_VERB;
			case 'w': return ES09_TAG_DATE;
			case 'z': return ES09_TAG_NUMBER;
		}

		//default
		return ES09_TAG_NONE;

		//TODO tags begin? end? where?

	}

	//TODO unused function
	/* not implemented for Spanish at the moment
	int multextToMorphtag(const std::string &multextTag) //several tags? e.g. there: EX, but also adverb
	{

	}
	*/

	/**
	 * Convert morphological information back into a Penn Treebank POS tag.
	 * Unneeded for Spanish.
	 */
	/*
	CTag morphToPenn(CMorph morph)
	{

	}
	*/

	/**
	 * Auxiliary function to split a string given a delimiter and an output vector
	 */
	std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems)
	{
	    std::stringstream ss(s);
	    std::string item;
	    while (std::getline(ss, item, delim)) {
	        elems.push_back(item);
	    }
	    return elems;
	}

	/**
	 * Auxiliary function to split a string given a delimiter
	 */
	std::vector<std::string> split(const std::string &s, char delim)
	{
	    std::vector<std::string> elems;
	    split(s, delim, elems);
	    return elems;
	}

	/**
	 * Convert a CoNLL word + POS tag + feats into its morphological information.
	 * The word is used only for some known words.
	 * Use an empty word
	 * Pass the empty string as the word if only tag information is to be used for morphological analysis.
	 * The feats parameter is ignored for English since the English Penn treebank has no feats field, but it is included for other languages.
	 */
	CMorph conllToMorph(const std::string &word , const std::string &conllTag, const std::string &feats)
	{

		CMorph morph;

		int postag = conllToMorphTag(conllTag);

		//set the POS tag
		morph.setField(MORPH_POSTAG,postag);

		//We are here.

		//split feats string into tokens using '|' character
		std::vector<std::string> tokens;
		split ( feats , '|' , tokens );

		for(std::vector<std::string>::iterator it = tokens.begin(); it != tokens.end(); ++it)
		{
			//split feat=value string into its feat and value
			std::string featequalvalue = *it;
			std::vector<std::string> featandvalue;
			split( featequalvalue , '=', featandvalue );
			std::string feat = featandvalue[0];
			std::string value = featandvalue[1];

			//noun type
			if ( !feat.compare("postype") && !value.compare("common") )
				morph.setField(MORPH_NOUN_TYPE,MORPH_NOUN_TYPE_COMMON);

			if ( !feat.compare("postype") && !value.compare("proper") )
				morph.setField(MORPH_NOUN_TYPE,MORPH_NOUN_TYPE_PROPER);

			//number
			if ( !feat.compare("num") && !value.compare("s") )
				morph.setField(MORPH_NUMBER,MORPH_NUMBER_SINGULAR);

			if ( !feat.compare("num") && !value.compare("p") )
				morph.setField(MORPH_NUMBER,MORPH_NUMBER_PLURAL);

			//gender
			if ( !feat.compare("gen") && !value.compare("m") )
				morph.setField(MORPH_GENDER,MORPH_GENDER_MASCULINE);

			if ( !feat.compare("gen") && !value.compare("f") )
				morph.setField(MORPH_GENDER,MORPH_GENDER_FEMININE);

			if ( !feat.compare("gen") && !value.compare("c") )
				morph.setField(MORPH_GENDER,MORPH_GENDER_NEUTRAL);

			//verb type
			if ( !feat.compare("postype") && !value.compare("main") )
				morph.setField(MORPH_VERB_TYPE,MORPH_VERB_MAIN);

			if ( !feat.compare("postype") && !value.compare("auxiliary") )
				morph.setField(MORPH_VERB_TYPE,MORPH_VERB_AUXILIARY);

			if ( !feat.compare("postype") && !value.compare("semiauxiliary") )
				morph.setField(MORPH_VERB_TYPE,MORPH_VERB_SEMIAUXILIARY);

			//verb mood
			if ( !feat.compare("mood") && !value.compare("indicative") )
				morph.setField(MORPH_VERB_MOOD,MORPH_VERB_INDICATIVE);

			if ( !feat.compare("mood") && !value.compare("subjunctive") )
				morph.setField(MORPH_VERB_MOOD,MORPH_VERB_SUBJUNCTIVE);

			if ( !feat.compare("mood") && !value.compare("participle") )
				morph.setField(MORPH_VERB_MOOD,MORPH_VERB_PASTPARTICIPLE);

			if ( !feat.compare("mood") && !value.compare("infinitive") )
				morph.setField(MORPH_VERB_MOOD,MORPH_VERB_INFINITIVE);

			if ( !feat.compare("mood") && !value.compare("gerund") )
				morph.setField(MORPH_VERB_MOOD,MORPH_VERB_GERUND);

			if ( !feat.compare("mood") && !value.compare("imperative") )
				morph.setField(MORPH_VERB_MOOD,MORPH_VERB_IMPERATIVE);

			//verb tense
			if ( !feat.compare("tense") && !value.compare("present") )
				morph.setField(MORPH_VERB_TENSE,MORPH_VERB_PRESENT);

			if ( !feat.compare("tense") && !value.compare("past") )
				morph.setField(MORPH_VERB_TENSE,MORPH_VERB_PAST);

			if ( !feat.compare("tense") && !value.compare("future") )
				morph.setField(MORPH_VERB_TENSE,MORPH_VERB_FUTURE);

			if ( !feat.compare("tense") && !value.compare("imperfect") )
				morph.setField(MORPH_VERB_TENSE,MORPH_VERB_IMPERFECT);

			if ( !feat.compare("tense") && !value.compare("conditional") )
				morph.setField(MORPH_VERB_TENSE,MORPH_VERB_CONDITIONAL);

			//person
			if ( !feat.compare("person") && !value.compare("1") )
				morph.setField(MORPH_PERSON,MORPH_PERSON_FIRST);

			if ( !feat.compare("person") && !value.compare("2") )
				morph.setField(MORPH_PERSON,MORPH_PERSON_SECOND);

			if ( !feat.compare("person") && !value.compare("3") )
				morph.setField(MORPH_PERSON,MORPH_PERSON_THIRD);

			//pronoun type
			if ( postag == ES09_TAG_PRON && !feat.compare("postype") && !value.compare("demonstrative") )
				morph.setField(MORPH_PRON_TYPE,MORPH_PRON_TYPE_DEMONSTRATIVE);

			if ( postag == ES09_TAG_PRON && !feat.compare("postype") && !value.compare("indefinite") )
				morph.setField(MORPH_PRON_TYPE,MORPH_PRON_TYPE_INDEFINITE);

			if ( postag == ES09_TAG_PRON && !feat.compare("postype") && !value.compare("relative") )
				morph.setField(MORPH_PRON_TYPE,MORPH_PRON_TYPE_RELATIVE);

			if ( postag == ES09_TAG_PRON && !feat.compare("postype") && !value.compare("personal") )
				morph.setField(MORPH_PRON_TYPE,MORPH_PRON_TYPE_PERSONAL);

			if ( postag == ES09_TAG_PRON && !feat.compare("postype") && !value.compare("possessive") )
				morph.setField(MORPH_PRON_TYPE,MORPH_PRON_TYPE_POSSESSIVE);

			//contracted?
			if ( !feat.compare("contracted") && !value.compare("yes") )
				morph.setField(MORPH_CONTRACTED,MORPH_CONTRACTED_YES);

			if ( !feat.compare("contracted") && !value.compare("no") ) //TODO I think the no value actually doesn't exist, they just leave unspecified for no
				morph.setField(MORPH_CONTRACTED,MORPH_CONTRACTED_NO);

			//determiner type
			if ( postag == ES09_TAG_DET && !feat.compare("postype") && !value.compare("article") )
					morph.setField(MORPH_DET_TYPE,MORPH_DET_TYPE_ARTICLE);

			if ( postag == ES09_TAG_DET && !feat.compare("postype") && !value.compare("demonstrative") )
					morph.setField(MORPH_DET_TYPE,MORPH_DET_TYPE_DEMONSTRATIVE);

			if ( postag == ES09_TAG_DET && !feat.compare("postype") && !value.compare("indefinite") )
					morph.setField(MORPH_DET_TYPE,MORPH_DET_TYPE_INDEFINITE);

			if ( postag == ES09_TAG_DET && !feat.compare("postype") && !value.compare("numeral") )
					morph.setField(MORPH_DET_TYPE,MORPH_DET_TYPE_NUMERAL);

			if ( postag == ES09_TAG_DET && !feat.compare("postype") && !value.compare("possessive") )
					morph.setField(MORPH_DET_TYPE,MORPH_DET_TYPE_POSSESSIVE);

		}

		return morph;

	}


	bool CMorphFeat::fieldValueNamesInitted = false;
	std::vector< std::vector<std::string> > CMorphFeat::fieldValueNames = std::vector< std::vector<std::string> >( LAST_MORPH_FIELD+1 , std::vector<std::string>( 0 , "N/A ") );

	//initialize the names of field values that are used for pretty printing features
	/*static*/ void CMorphFeat::initFieldValueNames ()
	{

		fieldValueNames = std::vector< std::vector<std::string> >( LAST_MORPH_FIELD+1 , std::vector<std::string>( 0 , "N/A ") );

		//postag
		for ( int i = 0 ; i < ES09_TAG_COUNT ; i++ )
		{
			fieldValueNames[MORPH_POSTAG].push_back(ES09_TAG_STRINGS[i]);
		}

		for ( int i = 0 ; i < MORPH_NOUN_TYPE_COUNT ; i++ )
			fieldValueNames[MORPH_NOUN_TYPE].push_back(MORPH_NOUN_TYPE_STRINGS[i]);

		for ( int i = 0 ; i < MORPH_NUMBER_COUNT ; i++ )
					fieldValueNames[MORPH_NUMBER].push_back(MORPH_NUMBER_STRINGS[i]);

		for ( int i = 0 ; i < MORPH_GENDER_COUNT ; i++ )
					fieldValueNames[MORPH_GENDER].push_back(MORPH_GENDER_STRINGS[i]);

		for ( int i = 0 ; i < MORPH_VERB_TYPE_COUNT ; i++ )
					fieldValueNames[MORPH_VERB_TYPE].push_back(MORPH_VERB_TYPE_STRINGS[i]);

		for ( int i = 0 ; i < MORPH_VERB_MOOD_COUNT ; i++ )
					fieldValueNames[MORPH_VERB_MOOD].push_back(MORPH_VERB_MOOD_STRINGS[i]);

		for ( int i = 0 ; i < MORPH_VERB_TENSE_COUNT ; i++ )
					fieldValueNames[MORPH_VERB_TENSE].push_back(MORPH_VERB_TENSE_STRINGS[i]);

		for ( int i = 0 ; i < MORPH_PERSON_COUNT ; i++ )
					fieldValueNames[MORPH_PERSON].push_back(MORPH_PERSON_STRINGS[i]);

		//for ( int i = 0 ; i < MORPH_DEGREE_COUNT ; i++ )
		//			fieldValueNames[MORPH_DEGREE].push_back(MORPH_DEGREE_STRINGS[i]);

		for ( int i = 0 ; i < MORPH_PRON_TYPE_COUNT ; i++ )
					fieldValueNames[MORPH_PRON_TYPE].push_back(MORPH_PRON_TYPE_STRINGS[i]);

		for ( int i = 0 ; i < MORPH_CONTRACTED_COUNT ; i++ )
					fieldValueNames[MORPH_CONTRACTED].push_back(MORPH_CONTRACTED_STRINGS[i]);

		for ( int i = 0 ; i < MORPH_DET_TYPE_COUNT ; i++ )
					fieldValueNames[MORPH_DET_TYPE].push_back(MORPH_DET_TYPE_STRINGS[i]);

		fieldValueNamesInitted = true;

	}

	/*static*/ std::string CMorphFeat::getFieldValueName ( const MORPH_FIELDS field , const int value )
	{
		if ( !fieldValueNamesInitted ) initFieldValueNames();
		std::vector<std::string> namesVector = fieldValueNames[field];
		assert ( namesVector.size() > value );
		return namesVector[value];
	}

}
