#include <cstdlib>
#include "Config.h"
#include "util.h"
using std::pair;
/* static members of class config */
int		CConfig::nRound				= 26;
int		CConfig::nRareWord		= -1;
int		CConfig::nCutoff			= 6;
int		CConfig::nBS					= 1;			// by default, deterministic tagger
int		CConfig::nRetry				= 3;
int		CConfig::nFordJK			= 10;
int		CConfig::nFixLen			= 4;			// by default 
int		CConfig::nGoldBS			= 1;
int		CConfig::nLowCount		= -1;			// by default, no low frequency feature
int		CConfig::nRandFactor	= 2;			// default
int		CConfig::nEbdCutoff		= 0;
int		CConfig::nLeft				=	0;
int		CConfig::nRight				= 0;
int		CConfig::nHLevel			=	0;
int		CConfig::nCodeBookLen	= 0;
int		CConfig::nDBN_HLayer	= 0;
int		CConfig::nDBN_HFeed		= 0;

bool  CConfig::bEarlyUpdate = false;
bool	CConfig::bRareFlag = false;		
bool	CConfig::bDisHis	 = false;		// for debuging purpose
bool	CConfig::bJK			 = false;
bool  CConfig::bEnTagger = true;     // by default, English POS tagger
bool	CConfig::bLen1WordEndWord	= true;			
bool	CConfig::bTagVerbose = false;
bool	CConfig::bCharFFromWord = false;

double CConfig::fRate 	= 0.1;
double CConfig::fMargin = 0.1;


string	CConfig:: strNull				= "null";
string	CConfig:: strLogDir			= strNull;
string	CConfig:: strTrainPath	= strNull;
string	CConfig:: strDevPath		= strNull;
string	CConfig:: strTestPath		= strNull;
string	CConfig:: strTempPath		= strNull;
string	CConfig:: strModelPath	= strNull;
string	CConfig:: strPrefix			= strNull;
string  CConfig:: strJKPath			= strNull;
string	CConfig:: strRBMPrefix	= strNull;
string	CConfig:: strTagLabelPath	= strNull;
string  CConfig:: strOutLayerType = strNull;
string	CConfig:: strDBNPrefix		= strNull;

string  CConfig:: strRegSamplePath 	= strNull;
string	CConfig:: strRegMarginalPath = strNull;
CConfig::UPDATE_STRATEGY CConfig::us		= CConfig::ALL;
CConfig::DY_ORACLE_NEXT_BEAM CConfig::dyNextBeam	= CConfig::GOLD_TOP;

vector<int> 		CConfig::vEHSize;
vector<int> 		CConfig::vIHSize;
vector<int> 		CConfig::vCHSize;

vector<string> 	CConfig::vDevPaths;
vector<string> 	CConfig::vTestPaths;
vector<string> 	CConfig::vEHType;
vector<string> 	CConfig::vIHType;
vector<string> 	CConfig::vCHType;


bool CConfig::
SaveConfig(const string & strPath)
{
	FILE *fp = fopen(strPath.c_str(), "w");
	CHK_OPEN_FAILED(fp, strPath.c_str());

	fprintf(fp, "%d %d %d %d %d", nLowCount,  nBS,  
					nRareWord,  nCutoff, bEnTagger);

	fclose(fp);
	return true;
}


bool CConfig::
LoadConfig(const string & strPath)
{
	FILE *fp = fopen(strPath.c_str(), "r");
	CHK_OPEN_FAILED(fp, strPath.c_str());

	int i = 0;
	fscanf(fp, "%d %d %d %d %d", &nLowCount,  &nBS,  
					&nRareWord,  &nCutoff,  &i);
	bEnTagger = i != 0;

	fprintf(stderr, "configs: %d %d %d %d %d\n", nLowCount,  nBS,  nRareWord,  nCutoff, bEnTagger);
	fclose(fp);
	return true;
}


bool CConfig::
ReadConfig(const string & strPath)
{
	FILE *fpIn = fopen(strPath.c_str(), "r");
	CHK_OPEN_FAILED(fpIn, strPath.c_str());

	const int BUF_LEN = 65535;
	char buf[BUF_LEN];
	fprintf(stderr, "\n\n-------------Training config--------------\n");
	while (fgets(buf, BUF_LEN, fpIn) != NULL)
	{
		if (buf [0] == ':' && buf [1] == ':')
			continue;
		
		removeNewLine(buf);
		if (strlen(buf) == 0)
			continue;
		char *pKey = strtok(buf, " \r\t\n");
		if (pKey == NULL)
		{
			fprintf(stderr, "Error: config file invalid format %s\n", buf);
			return false;
		}
		
		char *pVal = strtok(NULL, " \r\t\n");
		
		if (pVal == NULL)
		{
			fprintf(stderr, "Error: config file invalid format %s\n", buf);
			return false;
		}

		if (strcmp(pKey, "trainPath") == 0)
			strTrainPath = pVal;
		else if (strcmp(pKey, "devPath") == 0)
		{
			vDevPaths.push_back(pVal);
			fprintf(stderr, "dev set %-2lu:       %s\n", 
					vDevPaths.size(), vDevPaths.back().c_str());
		}
		else if (strcmp(pKey, "testPath") == 0)
		{
			vTestPaths.push_back(pVal);
			fprintf(stderr, "test set %-2lu:      %s\n", 
					vTestPaths.size(), vTestPaths.back().c_str());
		}
		else if (strcmp(pKey, "bEarlyUpdate") == 0)
			bEarlyUpdate = strcmp(pVal, "true") == 0;

		else if (strcmp(pKey, "nFixLen") == 0)
		{
			nFixLen = atoi(pVal);
			fprintf(stderr, "|char feature|:   %d\n", nFixLen);
		}
		else if (strcmp(pKey, "strLogDir") == 0)
		{
			strLogDir	= pVal;
			fprintf(stderr, "log dir:          %s\n", strLogDir.c_str());
		}
    else if (strcmp(pKey, "bEnTagger") == 0)
    {
      bEnTagger = strcmp(pVal, "true") == 0;
      fprintf(stderr, "tagger type:      %s\n", bEnTagger ? "English":"Chinese");
    }
		else if (strcmp(pKey, "fMargin") == 0)
		{
			fMargin = atof(pVal);
			fprintf(stderr, "fMargin:          %.2f\n", fMargin);
		}
		else if (strcmp(pKey, "strModelPath") == 0)
		{
			strModelPath = pVal;
			fprintf(stderr, "model path:       %s\n", strModelPath.c_str());
		}
		
		else if (strcmp(pKey, "strRBMPrefix") == 0)
		{
			strRBMPrefix = pVal;
			fprintf(stderr, "WRRBM prefix:     %s\n", strRBMPrefix.c_str());
		}
		else if (strcmp(pKey, "strOutLayerType") == 0)
		{
			strOutLayerType = pVal;
			fprintf(stderr, "Out layer type:   %s\n", strOutLayerType.c_str());
		}
		else if (strcmp(pKey, "bCharacterFeatureFromWord") == 0)
		{
			bCharFFromWord = string(pVal) == "true";
			fprintf(stderr, "Char feature from Word: %s\n", 
							bCharFFromWord ? "true":"false");

		}
		else if (strcmp(pKey, "bTagVerbose") == 0)
		{
			bTagVerbose = strcmp(pVal, "true") == 0;
			fprintf(stderr, "Tagging verbose:  %d\n", bTagVerbose);
		}
		else if (strcmp(pKey, "lowFreqThres") == 0)
		{
			nLowCount	= atoi(pVal);
			fprintf(stderr, "%s %d\n", pKey, nLowCount);
		}
		else if (strcmp(pKey, "fRate") == 0)
		{
			fRate	= atof(pVal);
			fprintf(stderr, "fRate:            %f\n", fRate);
		}
		else if (strcmp(pKey, "prefix") == 0)
		{
			strPrefix	= pVal;
			fprintf(stderr, "prefix:           %s\n", pVal);
		}
		else if (strcmp(pKey, "nCutoff") == 0)
		{
			CConfig::nCutoff	= atoi(pVal);
			fprintf(stderr, "Lexicon cutoff:   %d\n", nCutoff);
		}
		else if (strcmp(pKey, "templatePath") == 0)
		{
			strTempPath	= pVal;
			fprintf(stderr, "template file:    %s\n", strTempPath.c_str());
		}
		else if (strcmp(pKey, "bJK") == 0)
			bJK		 = string(pVal) == "true";
		else if (strcmp(pKey, "nFord") == 0)
			nFordJK	 = atoi(pVal);
		else if (strcmp(pKey, "JKPath") == 0)
			strJKPath	 = pVal;
		else if (strcmp(pKey, "nRound") == 0)
			nRound		= atoi(pVal);
		else if (strcmp(pKey, "nRare") == 0)
			nRareWord  = atoi(pVal);
		else if (string(pKey).find("HSize") != string::npos)
		{
			char *pSizes = pVal; 
			vector<int> &vHiddenSize = pKey[1] == 'I' ? vIHSize:
																 pKey[1] == 'E' ? vEHSize:vCHSize;
			vHiddenSize.clear();
			while (pSizes != NULL)
			{
				vHiddenSize.push_back(atoi(pSizes));
				pSizes = strtok(NULL, " \t\r\n");
			}
		}
		else if (string(pKey).find("HType") != string::npos)
		{
			char *pType = pVal; 
			vector<string> &vHiddenType = pKey[1] == 'I' ? vIHType:
																 pKey[1] == 'E' ? vEHType:CConfig::vCHType;
			while (pType != NULL)
			{
				vHiddenType.push_back(pType);
				pType = strtok(NULL, " \t\r\n");
			}
		}
		else 
		{
			fprintf(stderr, "Error: unknown configure parameter %s \n", pKey);
			return false;
		}
	}

	if (CConfig::strTrainPath == CConfig::strNull)
	{
		fprintf(stderr, "Error: Training file un-specified\n");
		return false;
	}

	if (CConfig::strModelPath == CConfig::strNull)
	{
		fprintf(stderr, "Error: Model file un-specified\n");
		return false;
	}

	if (CConfig::strTempPath == CConfig::strNull)
	{
		fprintf(stderr, "Error: Template file un-specified\n");
		return false;
	}

	fprintf(stderr, "-----------------------------------------\n");
	fclose(fpIn);
	return true;
}


string CConfig:: 
BuildPath()
{
	string path = strPrefix + "_";
	char buf[100];
	path += bEarlyUpdate == true ? "earlyUp_":"";
	sprintf(buf, "nFDim_%d", vIHSize[0]);
	path += buf;
	sprintf(buf, "fMargin_%.4f_fRate_%.4f", fMargin, fRate);
	path += buf;
//	if (nLowCount > 0)
//		path += string("low_") + string(itoa(nLowCount, buf, 10));
//	if (nRareWord > 0)
//		path += string("_rare_") + itoa(nRareWord, buf, 10);
	return path;
}
