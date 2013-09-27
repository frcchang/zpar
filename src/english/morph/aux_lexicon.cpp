/*
 * aux_lexicon.cpp
 *
 *  Created on: Sep 27, 2013
 *      Author: carlos
 *
 * Use an auxiliary lexicon in Multext format for lemmatization.
 * The Penn treebank for English is not lemmatized, so we need this resource for lemmas.
 */

#include "definitions.h"
#include "english/pos/penn.h"
#include "writer.h"
#include "reader.h"
#include "linguistics/word_tokenized.h"
#include "linguistics/taggedword.h"
#include "linguistics/dependency.h"
#include "linguistics/conll.h"
#include "linguistics/tagset.h"
#include "tags.h"
#include "morph.h"
#include "penn_lexicon.cpp"

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
				ASSERT( !lexiconTag.length() >= 2 , "Bad format for verb lexicon tag" );
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

			iss = std::istringstream(rstrip(line));
			getline(iss, curToken, '\t');
			ASSERT(is && !curToken.empty(), "Not well formatted lexicon data (lemma not found)");
			lemma = curToken;

			iss = std::istringstream(rstrip(line));
			getline(iss, curToken, '\t');
			ASSERT(is && !curToken.empty(), "Not well formatted lexicon data (tag not found)");
			tag = curToken;

			//add to the word to lemma map
			wordToLemma.insert(form,lemma);

			CMorphTag morphTag = lexiconTagToMorphTag(tag);
			std::pair<std::string,CMorphTag> wordMorphPair = std::pair<std::string,CMorphTag>(form,morphTag);
			wordAndTagToLemma.insert( wordMorphPair , lemma );

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



}



/**
 * Unit test
 */
//TODO Complete this.
int main( void )
{
	std::string filename;
	std::cout << "Enter filename for Penn treebank in CoNLL format: ";
	std::cin >> filename;
	bool bSuccess = english::initLexicon(filename,true);
	std::cout << "Successfully loaded the lexicon? " << bSuccess << "\n";
	std::cout << "Enter filename for aux lexicon in Multext format: ";
	std::cin >> filename;
	bSuccess = english::initLemmaLexicon(filename);
	std::cout << "Successfully loaded the lexicon? " << bSuccess << "\n";
	for(;;)
	{
		std::string word;
		std::cout << "Enter a word: ";
		std::cin >> word;
		std::cout << "That word is " << (english::isKnown(word)?"known":"unknown") << ".\n";
		std::cout << "It can have the following Penn tags: \n";
		std::set<english::CTag> setOfTags = english::getPossibleTags(word);
		for ( std::set<english::CTag>::iterator it = setOfTags.begin() ; it != setOfTags.end() ; ++it )
		{
			std::cout << " " << *it;
		}
		std::cout << "\n";
		std::cout << "And the following morphological analyses: \n";
		std::set<english::CMorph> setOfMorphs = english::getPossibleMorph(word);
		for ( std::set<english::CMorph>::iterator it = setOfMorphs.begin() ; it != setOfMorphs.end() ; ++it )
		{
			CMorph morph = *it;
			std::cout << " " << (*it).str();
			std::cout << english::getLemma(word,(*it).getField(MORPH_POSTAG)) << "\n";
		}
		std::cout << "\n";
	}
}
