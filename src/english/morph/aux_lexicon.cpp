/*
 * aux_lexicon.cpp
 *
 *  Created on: Sep 27, 2013
 *      Author: carlos
 *
 * Use an auxiliary lexicon in Multext format for lemmatization.
 * The Penn treebank for English is not lemmatized, so we need this resource for lemmas.
 */

#include "aux_lexicon.h"
//#include "penn_lexicon.cpp"

namespace english
{

	//map from word+tag to lemma
	std::map<std::pair<std::string,CMorphTag>,std::string> wordAndTagToLemma;

	//map from word to lemma (in case there is no entry for the word+tag pair)
	std::map< std::string , std::string > wordToLemma;


	/**
	 * Converts a lexicon tag in the Multext format to one of our internal morph tags.
	 * Note that this is a partial mapping! We don't need a perfect mapping, since this is only used for lemmatization.
	 * It's enough if the mapping works for the tags where lemmatization is relevant (nouns, verbs, adjectives, adverbs).
	 * Were are not distinguishing things like wh-pronouns from normal pronouns at the moment, because that shouldn't be relevant.
	 */
	CMorphTag lexiconTagToMorphTag ( const std::string lexiconTag )
	{
		ASSERT( !lexiconTag.empty() , "Empty lexicon tag" );
		switch ( lexiconTag[0] )
		{
			case 'V':
			{
				ASSERT( lexiconTag.length() >= 2 , "Bad format for verb lexicon tag" );
				if ( lexiconTag[1] == 'm')
					return MORPH_TAG_MODAL_VERB;
				else
					return MORPH_TAG_VERB;
			}
			case 'N': return MORPH_TAG_NOUN;
			case 'A': return MORPH_TAG_ADJ;
			case 'R': return MORPH_TAG_ADV;
			case 'P': return MORPH_TAG_PRON;
			default: return MORPH_TAG_NONE;
		}
	}


	bool initLemmaLexicon ( const std::string sInputFile )
	{
		std::ifstream *is;

		is = new std::ifstream(sInputFile.c_str());
		if ( is->fail() ) return false;

		bool bReadSuccessful;
		std::string line;
		std::string curToken;

		getline(*is, line);

		while(is && !lstrip(line).empty())
		{
			std::string form;
			std::string lemma;
			std::string tag;

			std::istringstream iss(rstrip(line));
			getline(iss, curToken, '\t');
			ASSERT(is && !curToken.empty(), "Not well formatted lexicon data (form not found)");
			form = curToken;

			//iss = std::istringstream(rstrip(line));
			getline(iss, curToken, '\t');
			ASSERT(is && !curToken.empty(), "Not well formatted lexicon data (lemma not found)");
			lemma = curToken;
			if ( lemma == "=" ) lemma = form; //lexicon uses = to represent that lemma equals form

			//iss = std::istringstream(rstrip(line));
			getline(iss, curToken, '\t');
			ASSERT(is && !curToken.empty(), "Not well formatted lexicon data (tag not found)");
			tag = curToken;

			//add to the word to lemma map
			//wordToLemma.insert(form,lemma);
			wordToLemma[form]=lemma;

			CMorphTag morphTag = lexiconTagToMorphTag(tag);
			std::pair<std::string,CMorphTag> wordMorphPair = std::pair<std::string,CMorphTag>(form,morphTag);

			wordAndTagToLemma[wordMorphPair] = lemma;

		    getline(*is, line);
		}

		is->close();
		delete is;

		//And we are done.
		return true;

	}

	std::string getLemma ( const std::string & form , const CMorphTag & morphTag )
	{
		if ( wordAndTagToLemma.find(std::pair<std::string,CMorphTag>(form,morphTag)) != wordAndTagToLemma.end() )
			return wordAndTagToLemma[std::pair<std::string,CMorphTag>(form,morphTag)];
		else if ( wordToLemma.find(form) != wordToLemma.end() )
			return wordToLemma[form];
		else
		{
			std::string result = std::string(form);
			std::transform( result.begin() , result.end() , result.begin() , ::tolower );
			return result;
		}
	}

	std::string getLemma ( const std::string & form , const CMorph & morph )
	{
		CMorphTag morphTag = morph.getField(MORPH_POSTAG);
		return getLemma ( form , morphTag );
	}



}


