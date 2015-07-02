#ifndef __GUARD__POS__EF
#define __GUARD__POS__EF

#include <vector>
#include <algorithm>
#include <cassert>
#include <string>

#include "Pool.h"
#include "util.h"
#include "Analysis.h"
#include "Maps.h"
#include "Lexicon.h"
#include "BaseExtractor.h"
#include "NNScorer.h"

	
#define MAX_BUF_SIZE  65535

class CTagger
{
public:
	struct SStatics
	{
		int		nInvalid;
		int		nNoValid;		// the case where we gold does not survive
								// however, top guess beam has lower score than 
								// top gold beam !
		int		nTraceBack;
		int		nResetBeam;
		int		nRetry;
	};

public:
	CTagger(bool verbose, bool intNN) 
	{
		m_printHis = false;
		m_pExtractor = NULL;
		m_pWTm 	= NULL;
		m_pNNScorer = NULL;
		m_verbose = verbose;
	}

	void AssignTag(SAnalysis *pAna, _ACT &rAction);
	void InitAnalysis(SAnalysis * pAna, _SEN *pSen);
	void EvalActions(SAnalysis *pAna, bool inside, bool evalVerbose = false);
	void EvalActions(SAnalysis *pAna);
	std::pair<_ACT,_ACT> TopActionPair(SAnalysis *pAna, vector<int> &goldTags);
	_SEN * Tagging(_SEN *pSen);
	_SEN * TrainGreedy(_SEN *pSen, 	_SEN *pGold,	
													double fRate, 	double fMargin);
	double MaxMarginUpdate(SAnalysis *pAna, 	vector<int> & goldTagIds,
													double fRate,  double fMargin);

	// here we try to achieve the upper bound
	typedef vector<int> _WRONG_RECORDER;
	_SEN * TaggingGoldHis(	_SEN *pSen,  _SEN *pGoldSen,  
															_WRONG_RECORDER & wrongs);

	double *ScoringNN(SAnalysis *pAna, int k);
	void UpdateNN(SAnalysis *pAna, _ACT &goldAct,  _ACT &predAct,   double fRate);

	void GetRange(SAnalysis *pAna,  int &rBeg,  int &rEnd)
	{
		int windowSize = m_pExtractor->GetWindowSize();
		// determine the range
		if (pAna->m_iUp == -1)
			rEnd = pAna->m_nSenLen;
		else
		{
			rBeg = std::max<int>(pAna->m_iUp - windowSize,  0);
			rEnd = std::min<int>(pAna->m_iUp + windowSize + 1,  pAna->m_nSenLen);
		}
	}

	void SetIMode(bool m)
	{
		m_insertMode = m;
		m_pExtractor->SetInsertMode(m);
		m_pNNScorer->SetIMode(m);
	}
	
	void Init(CBaseExtractor *pExtractor, 
						CWordTagManager *pWTm,  
						CNNScorer *pNNScorer,
						int nBs)
	{
		m_pExtractor = pExtractor;
		m_pWTm		 	 = pWTm;
		m_pNNScorer  = pNNScorer;
	}


	~CTagger() 
	{
		m_pExtractor = NULL;
		m_pWTm		 = NULL;
	}


	void ResetStatis()								{memset(&m_statics, 0, sizeof(m_statics));}
	SStatics GetStatics()							{return m_statics;}
	void SetVerbose(bool verbose)			{m_verbose = verbose;}
	CWordTagManager *GetLexicon()			{return m_pWTm;}
	CNNScorer *GetNNScorer()					{return m_pNNScorer;}
	CBaseExtractor *GetExtractor()		{return m_pExtractor;}
	bool Updated()										{return m_updated;}

private:
	CBaseExtractor  	*m_pExtractor;
	CWordTagManager   *m_pWTm;
	CPool						m_pool;
	bool						m_verbose;
	bool						m_insertMode;
	bool						m_printHis;
	bool						m_updated;
	SStatics				m_statics;
	CNNScorer 			*m_pNNScorer;
};
#endif
