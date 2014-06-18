#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <string>
#include <vector>

using std::vector;
using std::string;

class CConfig
{
public:
	static int		nRound;
	static int		nRetry;		
	static int		nGoldBS;
	static int		nRandFactor;

	enum     UPDATE_STRATEGY{TOP_ONE, ALL};
	enum     DY_ORACLE_NEXT_BEAM{GOLD_TOP,  GOLD_SURVIVE,  NON_ZERO};

	static DY_ORACLE_NEXT_BEAM  dyNextBeam;  
	static UPDATE_STRATEGY us;
	
	static double 	fRate;	
	static double   fMargin;
	static bool			bRareFlag;
	static bool			bLen1WordEndWord;
	static bool			bDisHis;
	static bool			bJK;								// jacknifing
  static bool   	bEnTagger;
	static bool			bTagVerbose;
	static bool			bRBMNN;
	static bool			bDBNNN;
	static bool 		bEarlyUpdate;
	static bool     bCharFFromWord;

	static string	strNull;
	static string	strTrainPath;
	static string	strDevPath;
	static string	strTestPath;
	static string	strTempPath;
	static string	strModelPath;
	static string	strTagLabelPath;
	static string	strPrefix;
	static string	strJKPath;
	static string	strRBMPrefix;				// currently, wrrbm
	static string strOutLayerType;
	static string strLogDir;
	static string strDBNPrefix;				// dbn prefix
	static string strRegSamplePath;		// for regularizing
	static string strRegMarginalPath; // for regularizing

	static vector<int>	vEHSize;
	static vector<int>	vIHSize;
	static vector<int>	vCHSize;

	static vector<string> vEHType;
	static vector<string> vIHType;
	static vector<string> vCHType;
	static vector<string> vDevPaths;
	static vector<string> vTestPaths;

	// configuration for decoding
	static int    nDBN_HLayer;				// number of hidden layer used for ENN
	static int    nDBN_HFeed;				// number of hidden layer used for ENN
	static int		nFordJK;						// n-ford jacknifing
	static int		nLowCount;					// low count
	static int		nRareWord;					// rare word
	static int		nCutoff;						// cutoff for word embedding
	static int		nEbdCutoff;					// cutoff for word embedding
	static int		nBS;								// beam size
	static int		nFixLen;						// pre,surffix
	
	// deep belief net features
	static int		nLeft;
	static int		nRight;
	static int 		nHLevel;
	static int		nCodeBookLen;

public:
	static bool SaveConfig(const string & strPath);
	static bool LoadConfig(const string & strPath);
	static bool ReadConfig(const string & strPath);
	static string BuildPath();
};







#endif  /*__CONFIG_H__*/
