
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <assert.h>
#include "morph.cpp"

//just for tests
int main ( void )
{
	//english::CMorph morph = english::pennToMorph("VBZ");
	//std::cout << "{" << morph.str() << "}";

	for ( ;; )
	{
		std::string inputWord;
		std::string inputTag;
		std::cout << "Input a Penn word and tag: ";
		std::cin >> inputWord;
		std::cin >> inputTag;
		english::CMorph morph = english::conllToMorph(inputWord,inputTag,"");
		std::cout << "{" << morph.str() << "}\n";
	}

}
