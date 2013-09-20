
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <assert.h>
#include "morph_pos.h"
#include "morph.h"

//just for tests
int main ( void )
{
	//english::CMorph morph = english::pennToMorph("VBZ");
	//std::cout << "{" << morph.str() << "}";

	for ( ;; )
	{
		std::string input;
		std::cout << "Input a Penn tag: ";
		std::cin >> input;
		english::CMorph morph = english::pennToMorph(input);
		std::cout << "{" << morph.str() << "}\n";
	}

}
