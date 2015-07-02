#include <cassert>
#include <queue>
#include <vector>
#include <set>
#include "Pool.h"
#include "util.h"
#include "GlobalMap.h"
#include "Sentence.h"

#ifndef __GUARD__DS__POS__EF
#define __GUARD__DS__POS__EF


struct STaggingAction
{
	int		m_idx;			//word index;
	int		m_TID;
	double	m_score; 
	bool	m_valid;
	
	STaggingAction()
	{
		memset(this, 0, sizeof(*this));
		m_valid = true;
	}
	
	void Init (int wIdx, int TID, double fScore)
	{
		m_idx = wIdx; m_TID = TID; m_score = fScore;
	}
	STaggingAction(int wid, int TID, double fScore)
	{
		m_idx		= wid;
		m_TID			= TID;
		m_score		= fScore;
		m_valid		= true;
	}


	bool operator() (STaggingAction *pl, STaggingAction *pr)
	{
		return pl->m_score > pr->m_score;
	}

	bool operator() (STaggingAction pl, STaggingAction pr)
	{
		return pl.m_score > pr.m_score;
	}
	
	struct greater
	{
		bool operator()(STaggingAction *pl, STaggingAction *pr)
		{
			return pl->m_score > pr->m_score;
		}
	};

	struct less
	{
		bool operator()(STaggingAction *pl, STaggingAction *pr)
		{
			return pl->m_score < pr->m_score;
		}
	};
};
typedef STaggingAction _ACT;


struct SActAry
{
	STaggingAction *m_pAry;
	int	m_len;
};

struct SAnalysis
{
	int				m_nSenLen;
	int				m_iUp;				// indexed updated
	int				m_nTagged;
	double			m_score;
	_SEN	 *	m_pSen;
	SActAry		**	m_pActArys;


	int Length() {return m_pSen->Length();}
	_ACT GetTopAction();

	SAnalysis()
	{
		m_pSen	  = NULL;
		m_pActArys  = NULL;
		m_nSenLen	= 0;
		m_iUp		= -1;
	}
	
	void Display(_ACT &act)
	{
		fprintf(stderr, "[%d:%s] tag %s score %.2e\n",
						act.m_idx, 	
						wstr2utf8(m_pSen->Word(act.m_idx)).c_str(),
						wstr2utf8(CGlobalMap::s_POSIDMap.Str(act.m_TID)).c_str(),
						act.m_score);
	}
};


















#if 0
	//	SActionLogger*	m_actFired;			// signature used to merge two equivalent analysis
//	int			 *  m_sig;
//	int				m_checkSum;			// to improve performance
//	int				m_cost;				// for searn based training
//	int				m_tStamp;			// time stamp
//	static const int s_sigNULL = -1;
	struct greater
	{
		bool operator() (SAnalysis *p1, SAnalysis *p2)
		{
			return p1->m_score > p2->m_score;
		}
	};
	struct less
	{
		bool operator() (SAnalysis *p1, SAnalysis *p2)
		{
			return p1->m_score < p2->m_score;
		}
	};
	struct sigCompare
	{
		bool operator ()(SAnalysis *p1, SAnalysis *p2)
		{
				return memcmp(p1->m_sig, p2->m_sig, sizeof(*p1->m_sig) * p1->m_nSenLen) < 0;
		}
	};
//	typedef std::priority_queue<STaggingAction *, vector<STaggingAction *>, STaggingAction::greater> _HEAP_TYPE;
//	bool GetTopKAction(int k, vector<STaggingAction *> &rActVec, bool verbose = false);
//	bool GetKActions(int k, vector<STaggingAction *> &rActVec, bool ascending);
	int LongestActAryLen()
	{
		int longest = 0;
		for (int i = 0; i < m_nSenLen; ++i)
		{
			if (m_pSen->Tagged(i) == false	&& 
				m_pActArys[i] != NULL		&&
				m_pActArys[i]->m_len > longest)
				longest = m_pActArys[i]->m_len;
		}

		return longest;
	}
	static SAnalysis * CopyAnalysis(SAnalysis *pTgt, CPool &rPool, bool noAction = false);
	bool operator ==(SAnalysis &r2);
	bool cmpActFired(SAnalysis &r2)//, int n)
	{
		for (int i = 0; i < r2.m_nTagged; ++i)
		{
			if (m_actFired[i].m_index != r2.m_actFired[i].m_index ||
				m_actFired[i].m_outcomeID != r2.m_actFired[i].m_outcomeID)
				return false;
		}
		return true;
	}
	static int TracingBackLatest(SAnalysis *pH, SAnalysis *pL)
	{
		SActionLogger *pHighLogger = pH->m_actFired;
		SActionLogger *pLowLogger = pL->m_actFired;

		if (pH->m_nTagged != pL->m_nTagged)
		{
			fprintf(stderr, "Error: number of tagged words are not identical\n");
			return -1;
		}

		if (pH->m_score < pL->m_score)
		{
			fprintf(stderr, "Error: incorrect call, pH should have higher score\n");
			return -1;
		}

		
		for (int i = pH->m_nTagged - 1; i >= 0; --i)
		{
			if (pLowLogger[i].m_accScore >= pHighLogger[i].m_accScore)
				return i;
		}

		// failed to find the step
		return -1;
	}
#endif

#if 0
struct SCostAnalysis
{
	SAnalysis	*m_pAnalysis;
	double		m_cost;
	int			m_nSPoint;				// starting point
};


#define _MAX_COST  1024 * 1024
class CBeam
{
public:
	CBeam():m_ordered(false), m_lowCost(_MAX_COST){}

	bool InsertToBeam(SAnalysis *pAna){
		m_AnaVec.push_back(pAna);
		if (pAna->m_cost < m_lowCost)
			m_lowCost = pAna->m_cost;

		return true;
	}
	void InsertToBeam(vector<SAnalysis *> pAnaVec)
	{
		for (SAnalysis *pAna:pAnaVec)
			InsertToBeam(pAna);
	}
	
	SAnalysis * FindAnalysis(SAnalysis *pLeft)
	{
		return FindAnalysisBySig(pLeft->m_sig, pLeft->m_nSenLen);
	}

	SAnalysis * FindAnalysisBySig(int *pSig, int len)
	{
		for (SAnalysis *pAna: m_AnaVec)
		{
			if (memcmp(pAna->m_sig, pSig, len) == 0)
				return pAna;
		}
		return NULL;
	}

	SAnalysis * GetTopCandidate()
	{
		std::sort(m_AnaVec.begin(), m_AnaVec.end(), SAnalysis::greater());
		return m_AnaVec[0];
	}

	void Replace(SAnalysis *pAna){fprintf(stderr, "Error: replace not supported\n");};
	void ReserveTopK(int k)
	{
		std::sort(m_AnaVec.begin(), m_AnaVec.end(), SAnalysis::greater());
		if (m_AnaVec.size() > (size_t)k)
		{
			m_AnaVec.resize(k);
			m_lowCost = _MAX_COST;
			for (SAnalysis *pAna: m_AnaVec)
				if (m_lowCost > pAna->m_cost)
					m_lowCost = pAna->m_cost;
		}
		return;
	};
	void GetCandidates(vector<SAnalysis *> &rResVec)			// after calling this, heap and set are cleared.
	{
		rResVec.clear();
		rResVec.assign(m_AnaVec.begin(), m_AnaVec.end());
	}
	void Clear()  {m_AnaVec.clear();	m_lowCost = _MAX_COST;	m_ordered = false;}
	int  GetLowCost(){return m_lowCost;}

private:
	vector<SAnalysis *> m_AnaVec;
	bool				m_ordered;
	int					m_lowCost;
};
#endif

#endif
