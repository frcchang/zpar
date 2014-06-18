#ifndef __N_N_SCORER_H__
#define __N_N_SCORER_H__
#include <vector>
#include <string>
#include <unordered_map>
#include "Pool.h"
using std::string;
using std::vector;

using std::unordered_map;
typedef unordered_map<string, int> DICT_TYPE;
struct SWRRBM
{
public:
	SWRRBM();
	static SWRRBM* RBMFromFile(const string &strPrefix);
	bool 	 Load(FILE *fp);	
	int  	 GetWDim()																	{return m_wDim;}
	bool Load(const string &strPath)
	{
		FILE *fp = fopen(strPath.c_str(), "rb");
		if (fp == nullptr) 
		{
			fprintf(stderr, "Error: File %s open failed\n", strPath.c_str());
			exit(0);
		}

		Load(fp);
		fclose(fp);
		return true;
	}

	// data members
	int				m_nLeft, m_nRight;
	int 		  m_nGram;					// number of ngrams
	int 			m_hNum;						// hidden layer size
	int				m_wDim;						// dimension of each word
	int 			m_nBurnIn;				// how many samples should be discarded	
//	CWordIDMap 	*m_pMap;				// vocab dictionary
//	CRandomGen 	*m_pGen;				// generator of the proposal distribution
	int 			m_vcbSize;
	double 	 *m_pHB;
	double   *m_pVB;						// all position share the same visuable bias
	double 	 *m_D; 							// n * d projection layer
	vector<double *> m_vW;			// m_pW[i] is a H*D matrix
	CPool   m_pool;							// no longer share with other SWRRBM object

	// always for debugging
	bool 		m_verbose;
};


class CRBMNN
{
public:
	CRBMNN(bool verbose = false):m_verbose(verbose)			{}
	CRBMNN(SWRRBM *pRBM, bool verbose = false);
	~CRBMNN();
	void  ComputeHidden(int *pNgram, int ngram, double *pHidden);
	bool  Load(FILE *fp);
	bool  Save(FILE *fp);
	int 	WindowSize()										{return m_nWindow;}
	int		HDim()													{return m_oDim;}
	int   Left()													{return m_nLeft;}
	int		Right()													{return m_nRight;}

private:
	void	copyWRRBMParam(SWRRBM *pRBM);

private:
	vector<double *> m_vEbd;
	double 		*m_pW;
	double 		*m_pB;
	int 			 m_iDim;
	int				 m_oDim;
	int				 m_nLeft;
	int				 m_nRight;
	int				 m_nWindow;
	int				 m_nEbdSize;
	CPool			 m_pool;
	bool			 m_verbose;
};



class CNNScorer
{
public:
	CNNScorer(CRBMNN *pRBMNN, int fEbdSize, int oDim);
	CNNScorer(CRBMNN *pRBMNN)			{m_pRBMNN = pRBMNN;}
	~CNNScorer();
	void InitRBMStates(int *pNgram, double *pRBMUnits);
	void BPError(double *pErr, int oID);
	void Update(int *pFIDs, int nFID, double *pHidden, 
							double *pError, double rate);
	void Scoring(int *pFID, int nFID, double *pHidden);
	void AverageParameter();
	void UnAverage();
	void Save(const string & strPath);
	void Load(const string & strPath);

	int HDim()									{return m_pRBMNN->HDim() + m_nfEbdSize + m_oDim;}
	int	ErrDim()								{return m_nfEbdSize + m_oDim;}
	int FEbdSize()							{return m_nfEbdSize;}
	int ODim()									{return m_oDim;}
	void SetIMode(bool mode)		{m_iMode = mode;}
	void IncCount()							{++m_nCount;}
	CRBMNN *GetRBMNN()					{return m_pRBMNN;}
private:
	vector<double *> 		m_vFEbd;		// m_vW[i][j]: weight from input unit i to output unit j
	vector<double *> 		m_vGFEbd;		// G for weight matrix
	vector<double *> 		m_vTFEbd;		// G for weight matrix
	vector<double *>    m_vAvgEbd;

	double 	*m_pWOut;					// output layer weight matrix
	double  *m_pGWOut;				// accumulated gradients for the second layer weight matrix
	double  *m_pTWOut;				// T: total, adding model average
	double  *m_pAvgWout;

	double  *m_pOBias;				// output layer bias
	double  *m_pGOBias;				// output layer bias
	double  *m_pTOBias;				// adding model average
	double  *m_pAvgOBias;

	bool 		m_iMode;
	bool    m_bAverage;
	int			m_nfEbdSize;			// size of the feature projection layer 
	int			m_oDim;						
	int     m_nCount;					// for model average
	CRBMNN  *m_pRBMNN;
	CPool 	m_pool;
};
#endif  /*__N_N_SCORER_H__*/
