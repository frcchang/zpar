#include "penn_lexicon.h"

//do it in makefile
//#include "reader.cpp"

//this should be done in makefile:
//#include "morph.cpp"

/*
 * 2013-09-24, Carlos Gomez Rodriguez
 *
 * Compile a map from words to lists of tags from the training treebank, which will be used as gold standard data
 * for the morphological analysis
 */

namespace english
{

//the tag dictionary will only contain words that appear more than this amount of times. The rest of the words will be considered unknown.
#define RARE_WORD_CUTOFF 2

//map from words to tags, extracted from the training set
std::map< std::string , std::set<CTag> > trainingSetLexicon;

//tags that will be assigned to unknown words
std::set<CTag> tagsForUnknownWords;

bool initLexicon ( const std::string sInputFile , bool bCoNLL )
{

	std::map< std::string , int > wordFrequencies; //to cutoff rare words from the tag dictionary

	CSentenceReader *input_reader;
	std::ifstream *is;

	CTwoStringVector input_sent;
	CCoNLLInput input_conll;

	is = 0;
	input_reader = 0;
	if (bCoNLL)
		is = new std::ifstream(sInputFile.c_str());
	else
		input_reader = new CSentenceReader(sInputFile);

	if ( is->fail() ) return false;

	bool bReadSuccessful;

	// Read the next example
	if (bCoNLL)
		bReadSuccessful = ( (*is) >> input_conll );
	else
		bReadSuccessful = input_reader->readTaggedSentence(&input_sent, false, TAG_SEPARATOR);

	while( bReadSuccessful )
	{
		//Store the word/tag pairs that have been read
		if (bCoNLL)
		{
			for ( int i = 0 ; i < input_conll.size() ; i++ )
			{
				//add tag to the set of tags associated with that word
				std::set<CTag> & entry = trainingSetLexicon[input_conll[i].word];
				entry.insert(input_conll[i].tag);
				//increase word frequency
				wordFrequencies[input_conll[i].word]++;
			}
		}
		else
		{
			for ( int i = 0 ; i < input_sent.size() ; i++ )
			{
				//add tag to the set of tags associated with that word
				std::set<CTag> & entry = trainingSetLexicon[input_sent[i].first];
				entry.insert(input_sent[i].second);
				//increase word frequency
				wordFrequencies[input_sent[i].first]++;
			}
		}

		// Read the next example
		if (bCoNLL)
			bReadSuccessful = ( (*is) >> input_conll );
		else
			bReadSuccessful = input_reader->readTaggedSentence(&input_sent, false, TAG_SEPARATOR);
	}

	is->close();
	if (bCoNLL)
	   delete is;
	else
	   delete input_reader;

	//remove from the map the words appearing less than RARE_WORD_CUTOFF times
	for ( std::map< std::string , int >::iterator it = wordFrequencies.begin() ; it != wordFrequencies.end() ; it++ )
	{
		if ( it->second < RARE_WORD_CUTOFF )
		{
			trainingSetLexicon.erase(it->first);
		}
	}

	//now, initialize the tags for unknown words
    for (int i=1; i<PENN_TAG_COUNT; i++)
       if ( !PENN_TAG_CLOSED[i] )
          tagsForUnknownWords.insert(CTag(i));

	//And we are done.
	return true;

}

bool isKnown ( const std::string & word )
{
	//std::set<english::CTag> & setOfTags = trainingSetLexicon[word];
	//return !setOfTags.empty();
	//should be faster:
	return trainingSetLexicon.find(word) != trainingSetLexicon.end();
}


std::set<CTag> getTagsForUnknownWord ( const std::string & word )
{
	//return tagsForUnknownWords;

	//copy the generic set of tags:
	std::set<CTag> result = std::set<CTag>(tagsForUnknownWords);

	//superlatives must end with st
	if ( word.length() < 2 || word[word.length()-2] != 's' || word[word.length()-1] != 't' )
	{
		result.erase(CTag(PENN_TAG_ADJECTIVE_SUPERLATIVE));
		result.erase(CTag(PENN_TAG_ADVERB_SUPERLATIVE));
	}

	//vbz must end with s
	if ( word.length() < 2 || word[word.length()-1] != 's' )
	{
		result.erase(CTag(PENN_TAG_VERB_THIRD_SINGLE));
	}

	return result;

}

std::set<CTag> getTagsForNoneWord ( )
{

	std::set<CTag> result;
	result.insert(CTag(MORPH_TAG_BEGIN));
	return result;

}

std::set<CTag> getPossibleTags ( const std::string & word )
{
	if ( isKnown(word) )
	{
		return english::trainingSetLexicon[word];
	}
	else if ( word == "-NONE-" )
		return getTagsForNoneWord ( );
	else
		return getTagsForUnknownWord ( word );
}

//get the possible morphological analyses for a given word
std::set<CMorph> getPossibleMorph ( const std::string & word )
{
	std::set<CTag> tags = getPossibleTags ( word );
	std::set<CMorph> result;
	for ( std::set<english::CTag>::iterator it = tags.begin() ; it != tags.end() ; ++it )
	{
		CMorph morph = conllToMorph ( word , it->str() , "" );
		result.insert(morph);
	}
	return result;
}





} //namespace english




