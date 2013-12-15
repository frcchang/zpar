#include "morph.h"

namespace english
{


	/**
	 * Convert a Penn Treebank POS tag into one of our morphological analyzer's POS tags.
	 */
	int pennToMorphTag(const std::string &pennTag)
	{

		if ( pennTag.length() < 1 ) return MORPH_TAG_NONE;

		switch (pennTag[0])
		{
			case '$': return MORPH_TAG_DOLLAR;
			case '`': return MORPH_TAG_OPEN_QUOTE;
			case '\'': return MORPH_TAG_CLOSE_QUOTE;
			case ',': return MORPH_TAG_COMMA;
			case '.': return MORPH_TAG_PERIOD;
			case ':': return MORPH_TAG_COLON;
			case '#': return MORPH_TAG_HASH;

			case 'N': return MORPH_TAG_NOUN;
			case 'V': return MORPH_TAG_VERB;
			case 'M': return MORPH_TAG_MODAL_VERB;
			case 'J': return MORPH_TAG_ADJ;

			case 'U': return MORPH_TAG_INTERJ;
			case 'L': return MORPH_TAG_LIST;
			case 'S': return MORPH_TAG_SYM;
			case 'F': return MORPH_TAG_FOREIGN;
			case 'E': return MORPH_TAG_EX;
			case 'T': return MORPH_TAG_TO;
			case 'I': return MORPH_TAG_IN;
			case 'D': return MORPH_TAG_DET;

			//if it's none of these, we have to look at 2 characters
		}

		assert ( pennTag.length() >= 2 );

		if ( pennTag[0] == '-' )
		{
			switch (pennTag[1])
			{
				case 'N': return MORPH_TAG_NONE;
				case 'B': return MORPH_TAG_BEGIN;
				case 'E': return MORPH_TAG_END;
				case 'L': return MORPH_TAG_LRB;
				case 'R': return MORPH_TAG_RRB;
				default: return MORPH_TAG_NONE;
			}
		}

		if ( pennTag[0] == 'R' && pennTag[1] == 'B' ) return MORPH_TAG_ADV;
		if ( pennTag[0] == 'R' && pennTag[1] == 'P' ) return MORPH_TAG_PARTICLE;

		if ( pennTag[0] == 'W' && pennTag[1] == 'D' ) return MORPH_TAG_WH_DET;
		if ( pennTag[0] == 'W' && pennTag[1] == 'R' ) return MORPH_TAG_WH_ADV;
		if ( pennTag[0] == 'W' && pennTag[1] == 'P' )
			return pennTag[pennTag.length()-1] == '$' ? MORPH_TAG_WH_POSS_PRON : MORPH_TAG_WH_PRON;

		if ( pennTag[0] == 'P' && pennTag[1] == 'O' ) return MORPH_TAG_POS;
		if ( pennTag[0] == 'P' && pennTag[1] == 'D' ) return MORPH_TAG_PREDET;
		if ( pennTag[0] == 'P' && pennTag[1] == 'R' )
			return pennTag[pennTag.length()-1] == '$' ? MORPH_TAG_POSS_PRON : MORPH_TAG_PRON;

		if ( pennTag[0] == 'C' && pennTag[1] == 'C' ) return MORPH_TAG_CC;
		if ( pennTag[0] == 'C' && pennTag[1] == 'D' ) return MORPH_TAG_NUM;

		//default
		return MORPH_TAG_NONE;

	}

	//TODO unused function
	int multextToMorphtag(const std::string &multextTag) //several tags? e.g. there: EX, but also adverb
	{
		/*TODO: Pending. Need to generate a list of CMorph, no 1-1 tag correspondence.*/
		if ( multextTag.length() < 1 ) return MORPH_TAG_NONE;

			switch (multextTag[0])
			{

				case 'N': return MORPH_TAG_NOUN;
				case 'V': {
					assert ( multextTag.length() >= 2 );
					if ( multextTag[1] == 'm' )
						return MORPH_TAG_MODAL_VERB;
					else
						return MORPH_TAG_VERB;
				}
				case 'A': return MORPH_TAG_ADJ;
				case 'P': {
					//TODO pending
					break;
				}

				case '`': return MORPH_TAG_OPEN_QUOTE;
				case '\'': return MORPH_TAG_CLOSE_QUOTE;
				case ',': return MORPH_TAG_COMMA;
				case '.': return MORPH_TAG_PERIOD;
				case ':': return MORPH_TAG_COLON;
				case '#': return MORPH_TAG_HASH;

				case 'J': return MORPH_TAG_ADJ;

				case 'U': return MORPH_TAG_INTERJ;
				case 'L': return MORPH_TAG_LIST;
				case 'S': return MORPH_TAG_SYM;
				case 'F': return MORPH_TAG_FOREIGN;
				case 'E': return MORPH_TAG_EX;
				case 'T': return MORPH_TAG_TO;
				case 'I': return MORPH_TAG_IN;
				case 'D': return MORPH_TAG_DET;

				//if it's none of these, we have to look at 2 characters
			}


	}

	/**
	 * Convert morphological information back into a Penn Treebank POS tag.
	 */
	CTag morphToPenn(CMorph morph)
	{

		int result;

		//first, morph tags that do not need the extra morphological information to convert them to penn treebank format, so each morph tag goes into a penn tag.
		//some do have it in the morph format (e.g. numerals, pronouns) but not in the penn treebank
		switch ( morph.getField(MORPH_POSTAG) )
		{
			case MORPH_TAG_DOLLAR: return CTag ( PENN_TAG_DOLLAR );
			case MORPH_TAG_OPEN_QUOTE: return CTag ( PENN_TAG_L_QUOTE );
			case MORPH_TAG_CLOSE_QUOTE: return CTag ( PENN_TAG_R_QUOTE );
			case MORPH_TAG_COMMA: return CTag ( PENN_TAG_COMMA );
			case MORPH_TAG_PERIOD: return CTag ( PENN_TAG_PERIOD );
			case MORPH_TAG_COLON: return CTag ( PENN_TAG_COLUM );
			case MORPH_TAG_HASH: return CTag ( PENN_TAG_SHART );

			case MORPH_TAG_INTERJ: return CTag ( PENN_TAG_UH );
			case MORPH_TAG_LIST: return CTag ( PENN_TAG_LS );
			case MORPH_TAG_SYM: return CTag ( PENN_TAG_SYM );
			case MORPH_TAG_FOREIGN: return CTag ( PENN_TAG_FW );
			case MORPH_TAG_EX: return CTag ( PENN_TAG_EX );
			case MORPH_TAG_TO: return CTag ( PENN_TAG_TO );
			case MORPH_TAG_IN: return CTag ( PENN_TAG_IN );
			case MORPH_TAG_DET: return CTag ( PENN_TAG_DT );

			case MORPH_TAG_NONE: return CTag ( PENN_TAG_NONE );
			case MORPH_TAG_BEGIN: return CTag ( PENN_TAG_BEGIN );
			case MORPH_TAG_END: return CTag ( PENN_TAG_END );
			case MORPH_TAG_LRB: return CTag ( PENN_TAG_L_BRACKET );
			case MORPH_TAG_RRB: return CTag ( PENN_TAG_R_BRACKET );

			case MORPH_TAG_PARTICLE: return CTag ( PENN_TAG_RP );

			case MORPH_TAG_PRON: return CTag( PENN_TAG_PRP );
			case MORPH_TAG_POSS_PRON: return CTag ( PENN_TAG_PRP_DOLLAR );
			case MORPH_TAG_WH_PRON: return CTag ( PENN_TAG_WP );
			case MORPH_TAG_WH_POSS_PRON: return CTag ( PENN_TAG_WP_DOLLAR );
			case MORPH_TAG_POS: return CTag( PENN_TAG_POS );
			case MORPH_TAG_PREDET: return CTag( PENN_TAG_PDT );
			case MORPH_TAG_CC: return CTag( PENN_TAG_CC );
			case MORPH_TAG_NUM: return CTag( PENN_TAG_CD );
			case MORPH_TAG_WH_DET: return CTag( PENN_TAG_WDT );
			case MORPH_TAG_WH_ADV: return CTag( PENN_TAG_WRB );

		}

		//now, morph tags for which we need to access other information to convert them to Penn format.

		if ( morph.getField(MORPH_NOUN_TYPE) == MORPH_NOUN_TYPE_COMMON )
		{
			if ( morph.getField(MORPH_NUMBER) == MORPH_NUMBER_SINGULAR )
				return PENN_TAG_NOUN;
			else
				return PENN_TAG_NOUN_PLURAL;
		}
		if ( morph.getField(MORPH_NOUN_TYPE) == MORPH_NOUN_TYPE_PROPER )
		{
			if ( morph.getField(MORPH_NUMBER) == MORPH_NUMBER_SINGULAR )
				return PENN_TAG_NOUN_PROPER;
			else
				return PENN_TAG_NOUN_PROPER_PLURAL;
		}

		if ( morph.getField(MORPH_POSTAG) == MORPH_TAG_ADJ )
		{
			switch ( morph.getField(MORPH_DEGREE) )
			{
				case MORPH_DEGREE_POSITIVE: return PENN_TAG_ADJECTIVE;
				case MORPH_DEGREE_COMPARATIVE: return PENN_TAG_ADJECTIVE_COMPARATIVE;
				case MORPH_DEGREE_SUPERLATIVE: return PENN_TAG_ADJECTIVE_SUPERLATIVE;
			}
		}

		if ( morph.getField(MORPH_POSTAG) == MORPH_TAG_ADV )
		{
			switch ( morph.getField(MORPH_DEGREE) )
			{
				case MORPH_DEGREE_POSITIVE: return PENN_TAG_ADVERB;
				case MORPH_DEGREE_COMPARATIVE: return PENN_TAG_ADVERB_COMPARATIVE;
				case MORPH_DEGREE_SUPERLATIVE: return PENN_TAG_ADVERB_SUPERLATIVE;
			}
		}

		if ( morph.getField(MORPH_VERB_TYPE) == MORPH_VERB_MODAL ) //or also, tag = MORPH_TAG_MODAL_VERB
			return PENN_TAG_MD;

		if ( morph.getField(MORPH_POSTAG) == MORPH_TAG_VERB )
		{
			switch ( morph.getField(MORPH_VERB_FORM) )
			{
				case MORPH_VERB_GERUND: return PENN_TAG_VERB_PROG;
				case MORPH_VERB_PARTICIPLE: return PENN_TAG_VERB_PAST_PARTICIPATE;
				case MORPH_VERB_INFINITIVE: return PENN_TAG_VERB;
			}
			assert (morph.getField(MORPH_VERB_FORM) == MORPH_VERB_PERSONAL );
			if ( morph.getField(MORPH_VERB_TENSE) == MORPH_VERB_PAST ) return PENN_TAG_VERB_PAST;
			assert (morph.getField(MORPH_VERB_TENSE) == MORPH_VERB_PRESENT );
			if ( morph.getField(MORPH_PERSON) == MORPH_PERSON_THIRD ) return PENN_TAG_VERB_THIRD_SINGLE;
			else return PENN_TAG_VERB_PRES;
		}

		assert(false); //we shouldn't reach this!
		return PENN_TAG_NONE;


	}


	/**
	 * Convert a Penn Treebank word + POS tag into its morphological information.
	 * The word is used only for some known words.
	 * Use an empty word
	 * Pass the empty string as the word if only tag information is to be used for morphological analysis.
	 * The feats parameter is ignored since the English Penn treebank has no feats field, but it is included because it is used in other languages.
	 */
	CMorph conllToMorph(const std::string &word , const std::string &pennTag, const std::string &feats)
	{

		CMorph morph;

		//set the POS tag
		morph.setField(MORPH_POSTAG,pennToMorphTag(pennTag));

		//common nouns
		if ( !pennTag.compare("NN") || !pennTag.compare("NNS") )
			morph.setField(MORPH_NOUN_TYPE,MORPH_NOUN_TYPE_COMMON);

		//proper nouns
		if ( !pennTag.compare("NNP") || !pennTag.compare("NNPS") )
			morph.setField(MORPH_NOUN_TYPE,MORPH_NOUN_TYPE_PROPER);

		//plural
		if ( !pennTag.compare("NNS") || !pennTag.compare("NNPS") )
			morph.setField(MORPH_NUMBER,MORPH_NUMBER_PLURAL);

		if ( !pennTag.compare("PRP") )
		{
			if ( !word.compare("they") || !word.compare("them") || !word.compare("we") || !word.compare("ourselves") || !word.compare("themselves") )
				morph.setField(MORPH_NUMBER,MORPH_NUMBER_PLURAL);
		}

		//singular
		if ( !pennTag.compare("NN") || !pennTag.compare("NNP") )
			morph.setField(MORPH_NUMBER,MORPH_NUMBER_SINGULAR);

		if ( !pennTag.compare("PRP") )
		{
			if ( !word.compare("it") || !word.compare("he") || !word.compare("him") || !word.compare("she") || !word.compare("her")
					|| !word.compare("itself") || !word.compare("himself") || !word.compare("herself") )
				morph.setField(MORPH_NUMBER,MORPH_NUMBER_SINGULAR);
		}

		if ( !pennTag.compare("VBZ") )
			morph.setField(MORPH_NUMBER,MORPH_NUMBER_SINGULAR);

		//plural/singular for numerals
		if ( !pennTag.compare("CD") && word.length() > 0 )
		{
			if ( !word.compare("1") || !word.compare("one") )
				morph.setField(MORPH_NUMBER,MORPH_NUMBER_SINGULAR);
			else
				morph.setField(MORPH_NUMBER,MORPH_NUMBER_PLURAL);
		}

		//pronoun gender
		if ( !pennTag.compare("PRP") )
		{
			if ( !word.compare("he") || !word.compare("him")  )
				morph.setField(MORPH_GENDER, MORPH_GENDER_MASCULINE);
			if ( !word.compare("she") || !word.compare("her") )
				morph.setField(MORPH_GENDER, MORPH_GENDER_FEMININE);
		}
		if ( !pennTag.compare("PRP$") )
		{
			if ( !word.compare("his") )
				morph.setField(MORPH_GENDER, MORPH_GENDER_MASCULINE);
			if ( !word.compare("her") || !word.compare("hers") )
				morph.setField(MORPH_GENDER, MORPH_GENDER_FEMININE);
		}

		//verb type
		if ( !pennTag.compare("MD") )
			morph.setField(MORPH_VERB_TYPE,MORPH_VERB_MODAL);
		if ( pennTag.length() > 0 && pennTag[0] == 'V' )
			morph.setField(MORPH_VERB_TYPE,MORPH_VERB_MAIN);

		//verb form
		if ( !pennTag.compare("VBG") )
			morph.setField(MORPH_VERB_FORM,MORPH_VERB_GERUND);
		if ( !pennTag.compare("VBN") )
			morph.setField(MORPH_VERB_FORM,MORPH_VERB_PARTICIPLE);
		if ( !pennTag.compare("VB") )
			morph.setField(MORPH_VERB_FORM,MORPH_VERB_INFINITIVE);
		if ( !pennTag.compare("VBD") || !pennTag.compare("VBP") || !pennTag.compare("VBZ") )
			morph.setField(MORPH_VERB_FORM,MORPH_VERB_PERSONAL);

		//tense
		if ( !pennTag.compare("VBD") )
			morph.setField(MORPH_VERB_TENSE,MORPH_VERB_PAST);
		if ( !pennTag.compare("VBP") || !pennTag.compare("VBZ") )
			morph.setField(MORPH_VERB_TENSE,MORPH_VERB_PRESENT);

		//verb person
		if ( !pennTag.compare("VBZ") )
			morph.setField(MORPH_PERSON,MORPH_PERSON_THIRD);
		if ( !pennTag.compare("VBP") )
			morph.setField(MORPH_PERSON,MORPH_PERSON_NOT_THIRD);
		//we don't do anything for VBD here because it could be any person

		//pronoun person
		if ( !pennTag.compare("PRP") )
		{
			if ( !word.compare("I") || !word.compare("me") || !word.compare("we") || !word.compare("us") || !word.compare("myself")
					|| !word.compare("ourself") || !word.compare("ourselves") )
				morph.setField(MORPH_PERSON,MORPH_PERSON_FIRST);
			if ( !word.compare("you") || !word.compare("yourself") || word.compare("yourselves") )
					morph.setField(MORPH_PERSON,MORPH_PERSON_SECOND);
			if ( !word.compare("he") || !word.compare("she") || !word.compare("him") || !word.compare("her") || !word.compare("himself")
					|| !word.compare("herself") || !word.compare("they") || !word.compare("them") || !word.compare("themselves") )
				morph.setField(MORPH_PERSON,MORPH_PERSON_THIRD);
		}

		//adjective degree
		if ( !pennTag.compare("JJ") || !pennTag.compare("RB") )
			morph.setField(MORPH_DEGREE,MORPH_DEGREE_POSITIVE);
		if ( !pennTag.compare("JJR") || !pennTag.compare("RBR") )
			morph.setField(MORPH_DEGREE,MORPH_DEGREE_COMPARATIVE);
		if ( !pennTag.compare("JJS") || !pennTag.compare("RBS") )
			morph.setField(MORPH_DEGREE,MORPH_DEGREE_SUPERLATIVE);

		//pronoun type. Note that this is unused (all enum values are zero)
		if ( !pennTag.compare("PRP") || !pennTag.compare("WP") )
			morph.setField(MORPH_IS_POSS,MORPH_NON_POSS);
		if ( !pennTag.compare("PRP$") || !pennTag.compare("WP$") )
			morph.setField(MORPH_IS_POSS,MORPH_POSS);

		//pronoun whness. Note that this is unused (all enum values are zero)
		if ( !pennTag.compare("PRP") || !pennTag.compare("PRP$") || !pennTag.compare("DT") )
			morph.setField(MORPH_IS_WH,MORPH_NON_WH);
		if ( !pennTag.compare("WP") || !pennTag.compare("WP$") || !pennTag.compare("WDT") )
			morph.setField(MORPH_IS_WH,MORPH_WH);

		return morph;

	}


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

