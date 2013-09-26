#include "definitions.h"
#include "english/pos/penn.h"
#include "reader.cpp"
#include "writer.h"
//#include "common/depparser/depparser_base.h" //for conll, etc. Not really necessary to include the full file, probably

#include "linguistics/word_tokenized.h"
#include "linguistics/taggedword.h"
#include "linguistics/dependency.h"
#include "linguistics/conll.h"
#include "linguistics/tagset.h"

#include "tags.h"


/*
 * 2013-09-24, Carlos Gomez Rodriguez
 *
 * Compile a map from words to lists of tags from the training treebank, which will be used as gold standard data
 * for the morphological analysis
 */

namespace english
{

std::map< std::string , std::set<CTag> > trainingSetLexicon;

bool initLexicon ( const std::string sInputFile , bool bCoNLL )
{
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
			}
		}
		else
		{
			for ( int i = 0 ; i < input_sent.size() ; i++ )
			{
				//add tag to the set of tags associated with that word
				std::set<CTag> & entry = trainingSetLexicon[input_sent[i].first];
				entry.insert(input_sent[i].second);
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

	//And we are done.
	return true;

}


} //namespace english


/**
 * Unit test
 */
int main( void )
{
	std::string filename;
	std::cout << "Enter filename for Penn treebank in CoNLL format: ";
	std::cin >> filename;
	bool bSuccess = english::initLexicon(filename,true);
	std::cout << "Successfully loaded the lexicon? " << bSuccess << "\n";
	for(;;)
	{
		std::string word;
		std::cout << "Enter a word: ";
		std::cin >> word;
		std::cout << "That word can be: ";
		std::set<english::CTag> & setOfTags = english::trainingSetLexicon[word];
		for ( std::set<english::CTag>::iterator it = setOfTags.begin() ; it != setOfTags.end() ; ++it )
		{
			std::cout << " " << *it;
		}
		std::cout << "\n";
	}
}

