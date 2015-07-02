#include <ctime>
#include <sstream>
#include <unordered_map>
#include "EasyFirstPOS.h"
#include "Config.h"
using std::unordered_map;
using std::pair;
#define _MAX_FIX_LEN 50
#if 0
void CTagger::
DisplayDerivation(SAnalysis *pAna, FILE *fp)
{
	int len = pAna->m_pSen->Length();
	fprintf(fp, "Derivation:\n");
	for (int i = 0; i < len; ++i)
	{
		int index = pAna->m_actFired[i].m_index;
		if (index != -1)
			fprintf(fp, "(%d %s_%s) ", index, 
				wstr2utf8(pAna->m_pSen->Word(index)).c_str(),  
				wstr2utf8(pAna->m_pSen->Tag(index)).c_str());
	}
	fprintf(fp, "\n");
}
#endif

double * CTagger::
ScoringNN(SAnalysis *pAna, int k)
{
	_SEN &rSen = *pAna->m_pSen;
	double *pScores = rSen.GetHScores(k);
	if (pScores == NULL)
	{
		pScores = (double *)m_pool.Allocate(m_pNNScorer->HDim() * sizeof(double));
		rSen.SetHScores(k, pScores);
		int *pNgrams, ngram;
		rSen.GetNgramFeatures(k, &pNgrams, ngram);
		if (m_verbose == true)
		{
			fprintf(stderr, "\n\nword %s\n", wstr2utf8(rSen.Word(k)).c_str());
			for (int i = 0; i < ngram; ++i)
				fprintf(stderr, "gid %d ", pNgrams[i]);
			fprintf(stderr, "\n");
		}

		if (ngram != 0)
			m_pNNScorer->InitRBMStates(pNgrams, pScores);
	}

	int *pFIDs, fNum;
	rSen.GetNonNgramFeatures(k, &pFIDs, fNum);
	m_pNNScorer->Scoring(pFIDs, fNum, pScores);
	if (m_verbose == true)
	{
		fprintf(stderr, "\n\nword %s\n", wstr2utf8(rSen.Word(k)).c_str());
		for (int k = 0; k < m_pNNScorer->ODim(); ++k)
			fprintf(stderr, "%s %.2f  ", wstr2utf8(CGlobalMap::s_POSIDMap.Str(k)).c_str(), 
							pScores[m_pNNScorer->HDim() - m_pNNScorer->ODim() + k]);
		fprintf(stderr, "\n-----------------------------------\n");
	}
	return pScores + m_pNNScorer->HDim() - m_pNNScorer->ODim();
}

inline void SortTwoAction(_ACT *pActs)
{
	if (pActs[0].m_score < pActs[1].m_score)
	{
		std::swap(pActs[0].m_score, pActs[1].m_score);
		std::swap(pActs[0].m_TID, pActs[1].m_TID);
	}
}

std::pair<_ACT, _ACT> 
CTagger::TopActionPair(SAnalysis *pAna, vector<int> &goldTags)
{
	std::pair<_ACT, _ACT> actPair;
	actPair.first.m_score = actPair.second.m_score = -1.7e300;
	_SEN *pSen = pAna->m_pSen;
	int offset = m_pNNScorer->HDim() - m_pNNScorer->ODim();
	for (int i = 0; i < pSen->Length(); ++i)
	{
		if (pSen->Tagged(i) == false)
		{
			// update top gold Action
			int goldTag = goldTags[i];
			double goldScore = pSen->GetHScores(i)[offset + goldTag];
			if (actPair.first.m_score < goldScore)
				actPair.first.Init(i, goldTag, goldScore);

			// update top non-gold action
			_ACT *pActs = pAna->m_pActArys[i]->m_pAry;
			for (int k = 0; k < pAna->m_pActArys[i]->m_len; ++k)
				if (pActs[k].m_TID != goldTags[i] && actPair.second.m_score < pActs[k].m_score)
					actPair.second.Init(i, pActs[k].m_TID, pActs[k].m_score);
		}
	}
	
	return actPair;
}


void CTagger::
EvalActions(SAnalysis *pAna, bool inside, bool evalVerbose)
{
	int beg = 0, end = 0;
	GetRange(pAna,  beg,  end);
	if (evalVerbose == true)
		fprintf(stderr, "iUp %d, beg %d, end %d\n", pAna->m_iUp, beg, end);

	// for each position, evaluate actions
	_SEN &rSen = *pAna->m_pSen;
	for (int k = 0; k < rSen.Length(); ++k)
	{
		if (rSen.Tagged(k) == true)
			continue;
		
		if (inside == true && (k < beg || k >= end))
			continue;

		if (inside == false && (k >= beg && k < end))
			continue;

		// allocate space for action array
		if (pAna->m_pActArys[k] == NULL)
		{
			pAna->m_pActArys[k] = (SActAry *)m_pool.Allocate(sizeof(SActAry));
			pAna->m_pActArys[k]->m_pAry = (_ACT*)m_pool.Allocate(sizeof(_ACT) * 2);
			pAna->m_pActArys[k]->m_len	= 2;		// 0 denotes no actions initialized
		}

		if (evalVerbose == true)
		{
			int *pFIDs = rSen.GetFeatures(k);
			int nFIDs  = rSen.GetFNum(k);
			fprintf(stderr, "\nFeatures:");
			for (int i = 0; i < nFIDs; ++i)
				fprintf(stderr, "%d_%d, ", i, pFIDs[i]);
			fprintf(stderr, "\n");
		}
	
		// caculate scores for each action
		double *pScores = ScoringNN(pAna, k);
		pAna->m_pActArys[k]->m_len = 2;
		_ACT *pActs = pAna->m_pActArys[k]->m_pAry;
		pActs[0].Init(k, 0, pScores[0]);
		pActs[1].Init(k, 1, pScores[1]);
		
		SortTwoAction(pActs);
		for (int TID = 2;  TID < (int)CGlobalMap::s_POSIDMap.Size();  ++TID)
			if (pScores[TID] > pActs[1].m_score)
			{
				pActs[1].m_score = pScores[TID];
				pActs[1].m_TID = TID;
				SortTwoAction(pActs);
			}
	}
}

void CTagger::
EvalActions(SAnalysis *pAna)
{
	int beg = 0, end = 0;
	GetRange(pAna,  beg,  end);

	// for each position, evaluate actions
	_SEN &rSen = *pAna->m_pSen;
	for (int k = beg; k < end; ++k)
	{
		if (rSen.Tagged(k) == true)
			continue;
		// allocate space for action array
		if (pAna->m_pActArys[k] == NULL)
		{
			pAna->m_pActArys[k] = (SActAry *)m_pool.Allocate(sizeof(SActAry));
			pAna->m_pActArys[k]->m_pAry = (_ACT*)m_pool.Allocate(sizeof(_ACT));
			pAna->m_pActArys[k]->m_len	= 1;		// 0 denotes no actions initialized
		}

		double *pScores = ScoringNN(pAna, k);
		const int nPOS = CGlobalMap::s_POSIDMap.Size();
		int BestID = 0;
		for (int TID = 1; TID < nPOS; ++TID)
			if (pScores[BestID] < pScores[TID])
				BestID = TID;

		pAna->m_pActArys[k]->m_pAry->Init(k, BestID, pScores[BestID]);
	}
}


_SEN *CTagger::
Tagging(_SEN *pSen)
{
	m_pool.Recycle(),		m_pExtractor->ResetMemory();
	SAnalysis *pAna = (SAnalysis*)m_pool.Allocate(sizeof(SAnalysis));
	InitAnalysis(pAna, pSen);
	
	for (int nTagged = 0; nTagged < pSen->Length(); ++nTagged)
	{
		if (nTagged == 0)
			m_pExtractor->InitFeatures(*pAna->m_pSen);
		else
			m_pExtractor->UpdateFeatureIDs(pAna);

		EvalActions(pAna);
		_ACT topAction = pAna->GetTopAction();
		AssignTag(pAna, topAction);
	}
	return pSen;
}


_SEN *CTagger::
TrainGreedy(_SEN *pSen, _SEN *pGold, 
						double fRate, double fMargin)
{
	if (m_verbose == true)
		pGold->Display(stderr);
	
	m_pool.Recycle(),		m_pExtractor->ResetMemory();
	SAnalysis *pAna = (SAnalysis*)m_pool.Allocate(sizeof(SAnalysis));
	InitAnalysis(pAna, pSen);

	// collect gold standard pos tags
	vector<int> vGoldTags(pSen->Length());
	for (int i = 0; i < pSen->Length(); ++i)
		vGoldTags[i] = pGold->TID(i);

	bool inside = true;
	for (int nTagged = 0; nTagged < pSen->Length(); ++nTagged)
	{
		if (nTagged == 0)
			m_pExtractor->InitFeatures(*pAna->m_pSen);
		else
			m_pExtractor->UpdateFeatureIDs(pAna);

		EvalActions(pAna, inside, m_verbose);
		double loss = MaxMarginUpdate(pAna, vGoldTags, fRate, fMargin);
		if (loss > 0 && CConfig::bEarlyUpdate)
			break;
	}
	m_pNNScorer->IncCount();
	return pSen;
}

double CTagger::
MaxMarginUpdate(SAnalysis *pAna, vector<int> &goldTags, 
								double fRate,    double fMargin)
{
	int nRetry = 0;
	double loss = 0;
	while (true)
	{
		std::pair<_ACT, _ACT> acts = TopActionPair(pAna, goldTags);
		_ACT &gold = acts.first, &pred = acts.second;
		
		loss = fMargin + pred.m_score - gold.m_score;
		if (m_verbose == true)
		{
			fprintf(stderr, "\n-----------------------\nGold Action:\n");
			pAna->Display(gold);
			fprintf(stderr, "Pred Action:\n");
			pAna->Display(pred);
			fprintf(stderr, "loss %.4e\n\n", loss);
		}
	
		if (nRetry >= CConfig::nRetry || loss <= 0)
		{
			AssignTag(pAna, gold);
			break;
		}

		++nRetry;
		UpdateNN(pAna, gold, pred, fRate);
		EvalActions(pAna, false, m_verbose);
		EvalActions(pAna, true,  m_verbose);
		if (CConfig::bEarlyUpdate)
			break;
	}
	m_statics.nRetry += nRetry;// > 1 ? nRetry - 1: 0;
	return loss;
}


void CTagger::
UpdateNN(SAnalysis *pAna,  _ACT &gold, 
				 _ACT &pred,	 double fRate)
{
	_SEN *pSen  = pAna->m_pSen;
	vector<double> vErrGold(m_pNNScorer->ErrDim(), 0);
	vector<double> vErrPred(m_pNNScorer->ErrDim(), 0);
		
	// update for gold
	int *fids, *ngram, nfNum, nNgram;
	pSen->GetNonNgramFeatures(gold.m_idx, &fids, nfNum);
	pSen->GetNgramFeatures(gold.m_idx, &ngram, nNgram);
	vErrGold[m_pNNScorer->FEbdSize() + gold.m_TID] = -1.0;
	m_pNNScorer->BPError(&vErrGold[0], gold.m_TID);


	// update for incorrect 
	int *fidsPred, *ngramPred, nfNumPred;
	pSen->GetNonNgramFeatures(pred.m_idx, &fidsPred, nfNumPred);
	pSen->GetNgramFeatures(pred.m_idx, &ngramPred, nNgram);
	vErrPred[m_pNNScorer->FEbdSize() + pred.m_TID] = 1.0;
	m_pNNScorer->BPError(&vErrPred[0], pred.m_TID);

	double *pHScores = pSen->GetHScores(gold.m_idx);
	double *pHScoresPred = pSen->GetHScores(pred.m_idx);
	m_pNNScorer->Update(fids, 	nfNum, 	pHScores, 	&vErrGold[0], 	fRate);
	m_pNNScorer->Update(fidsPred, 	nfNumPred, 	pHScoresPred, 
											&vErrPred[0], fRate);
}
	
	
void CTagger::
AssignTag(SAnalysis *pAna,  _ACT &rAction)//,  int nStep)
{
	pAna->m_pActArys[rAction.m_idx]	= NULL;
	pAna->m_nTagged ++;
	
	// update other members
	pAna->m_iUp   = rAction.m_idx;
	pAna->m_score += rAction.m_score;
	pAna->m_pSen->SetTag(rAction.m_idx, rAction.m_TID); 
}


void CTagger::
InitAnalysis(SAnalysis *pAna, _SEN *pSen)
{
	assert(pAna != NULL);
	_SEN &rSen = *(pAna->m_pSen = pSen);
	rSen.ClearTags(),	rSen.ClearFeatures();
	int senLen		= rSen.Length();

	// non-pointer members
	pAna->m_iUp			 = -1;
	pAna->m_nSenLen	 = senLen;
	pAna->m_nTagged	 = 0;

	// init aciton arrays
	pAna->m_pActArys = (SActAry **)m_pool.Allocate(sizeof(SActAry*) *senLen);
	memset(pAna->m_pActArys, 0, sizeof(SActAry *) * senLen);

	// init number feature stuff
	for (int i = 0;  i < senLen;  ++i)
	{
		rSen.m_pNodes[i]->m_nHDs = -1;// wLen > CConfig::nFixLen ? CConfig::nFixLen * 2: 2 * wLen;
		rSen.m_pNodes[i]->m_nFIDs   = -1;// nTemps + rSen.m_pNodes[i]->m_nHDs;
		rSen.m_pNodes[i]->m_nKernel = -1;// m_pExtractor->GetTempMgr()->GetKernelNum(); 
		rSen.m_pNodes[i]->m_nEbds	= -1;
	}
}

