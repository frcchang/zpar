#include "training_lexicon.h"

//do it in makefile
//#include "reader.cpp"

//this should be done in makefile:
//#include "morph.cpp"

/*
 * 2014-01-07, Carlos Gomez Rodriguez
 *
 * Compile a map from words to lists of tags from the training treebank, which will be used as gold standard data
 * for the morphological analysis
 *
 * Lemmas are handled by a different file even though taken from the same source in the case of English
 *
 * TODO Need to store full morphs (tags+feats), as feats are also needed to generate all possible morphs.
 * In English, tags were sufficient to generate all candidate morphs for a given word, in Spanish that isn't true.
 * So this lexicon either go from word to set of tag+feats pair, or directly from word to set of cmorph (rather than from word to set of ctag).
 * And morphs for unknown words would be the pairs or the morphs containing an open tag.
 * The closest match to the way it's done in English would be with the pairs (so we could still call conllToMorph inside getPossibleMorph) but the other should also
 * be valid, without calling conllToMorph, and more efficient (no need to tokenize the features each time we call getPossibleMorph, which would be very inefficient!)
 * Note that getPossibleMorph (here) is what should interface with the parser in training mode, and conllToMorph (in morph.cpp) is what should interface in parsing mode.
 * As long as those two work, the rest is implementation details.
 */

namespace spanish
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
    for (int i=1; i<ES09_TAG_COUNT; i++)
       if ( !ES09_TAG_CLOSED[i] )
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

	//here, we could add rules that allow us to discard tags (like superlatives having to end with st in English), but I can't think of similar restrictions for Spanish at the moment

	return result;

}

std::set<CTag> getTagsForNoneWord ( )
{

	std::set<CTag> result;
	result.insert(CTag(ES09_TAG_BEGIN));
	return result;

}

std::set<CTag> getPossibleTags ( const std::string & word )
{
	if ( isKnown(word) )
	{
		return spanish::trainingSetLexicon[word];
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
	//TODO Iterating over possible tags here is not enough. Have to iterate over possible feats as well! (or over all the seen full morph tags)
	for ( std::set<spanish::CTag>::iterator it = tags.begin() ; it != tags.end() ; ++it )
	{
		CMorph morph = conllToMorph ( word , it->str() , "" );
		result.insert(morph);
	}
	return result;
}





} //namespace spanish




