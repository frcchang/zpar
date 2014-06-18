#include <cstdlib>
#include <cstdio>
#include "util.h"

void usage(const char *pName)
{
	fprintf(stderr, "usage: %s training   configFile\n", pName);
	fprintf(stderr, "     : %s tagging    modelPrefix   input   output\n", pName);
}

void Tagging(const string &strModelPrefix, 
						 const string &strIn, 
						 const string &strOut);
void Training(const string &strConfig);


int main(int argc, char**argv)
{
	if (argc == 3 && strcmp(argv[1], "training") == 0)
	{
		Training(argv[2]);
		return 0;
	}
	else if (argc == 5 && strcmp(argv[1], "tagging") == 0)
	{
		Tagging(argv[2], argv[3], argv[4]);
		return 0;
	}
	else
	{
		usage(*argv);
		return -1;
	}
}
