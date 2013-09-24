#include "morph.h"

namespace english
{

	bool CMorphFeat::fieldValueNamesInitted = false;
	std::vector< std::vector<std::string> > CMorphFeat::fieldValueNames = std::vector< std::vector<std::string> >( LAST_MORPH_FIELD+1 , std::vector<std::string>( 0 , "N/A ") );

	//initialize the names of field values that are used for pretty printing features
	/*static*/ void CMorphFeat::initFieldValueNames ()
	{

		fieldValueNames = std::vector< std::vector<std::string> >( LAST_MORPH_FIELD+1 , std::vector<std::string>( 0 , "N/A ") );

		//postag
		for ( int i = 0 ; i < MORPH_TAG_COUNT ; i++ )
		{
			fieldValueNames[MORPH_POSTAG].push_back(MORPH_TAG_STRINGS[i]);
		}

		for ( int i = 0 ; i < MORPH_NOUN_TYPE_COUNT ; i++ )
			fieldValueNames[MORPH_NOUN_TYPE].push_back(MORPH_NOUN_TYPE_STRINGS[i]);

		for ( int i = 0 ; i < MORPH_NUMBER_COUNT ; i++ )
					fieldValueNames[MORPH_NUMBER].push_back(MORPH_NUMBER_STRINGS[i]);

		for ( int i = 0 ; i < MORPH_GENDER_COUNT ; i++ )
					fieldValueNames[MORPH_GENDER].push_back(MORPH_GENDER_STRINGS[i]);

		for ( int i = 0 ; i < MORPH_VERB_TYPE_COUNT ; i++ )
					fieldValueNames[MORPH_VERB_TYPE].push_back(MORPH_VERB_TYPE_STRINGS[i]);

		for ( int i = 0 ; i < MORPH_VERB_FORM_COUNT ; i++ )
					fieldValueNames[MORPH_VERB_FORM].push_back(MORPH_VERB_FORM_STRINGS[i]);

		for ( int i = 0 ; i < MORPH_VERB_TENSE_COUNT ; i++ )
					fieldValueNames[MORPH_VERB_TENSE].push_back(MORPH_VERB_TENSE_STRINGS[i]);

		for ( int i = 0 ; i < MORPH_PERSON_COUNT ; i++ )
					fieldValueNames[MORPH_PERSON].push_back(MORPH_PERSON_STRINGS[i]);

		for ( int i = 0 ; i < MORPH_DEGREE_COUNT ; i++ )
					fieldValueNames[MORPH_DEGREE].push_back(MORPH_DEGREE_STRINGS[i]);

		for ( int i = 0 ; i < MORPH_IS_POSS_VALUE_COUNT ; i++ )
					fieldValueNames[MORPH_IS_POSS].push_back(MORPH_IS_POSS_VALUE_STRINGS[i]);

		for ( int i = 0 ; i < MORPH_IS_WH_VALUE_COUNT ; i++ )
					fieldValueNames[MORPH_IS_WH].push_back(MORPH_IS_WH_VALUE_STRINGS[i]);

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

