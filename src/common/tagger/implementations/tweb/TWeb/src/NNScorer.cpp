#include "NNScorer.h"
#include "GlobalMap.h"

inline double sigmoid(double x)
{
  return 1.0 / (1.0 + exp(-x));
}

inline double uniform(double min, double max)
{
	return rand() / (RAND_MAX + 1.0) * (max - min) + min;
}

SWRRBM::SWRRBM()
{
	m_nGram = m_wDim = m_hNum = m_vcbSize = m_nBurnIn = 0;
	m_pHB = m_pVB = m_D = NULL;
	m_verbose = false;
}


bool SWRRBM::
Load(FILE *fp)
{
	fread(&m_nLeft,   sizeof(m_nLeft), 	 1, fp);
	fread(&m_nRight,  sizeof(m_nRight),  1, fp);
	fread(&m_nGram, 	sizeof(m_nGram), 	 1, fp);
	fread(&m_hNum, 		sizeof(m_hNum), 	 1, fp);
	fread(&m_wDim, 		sizeof(m_wDim), 	 1, fp);
	fread(&m_vcbSize, sizeof(m_vcbSize), 1, fp);
	fread(&m_nBurnIn, sizeof(m_nBurnIn), 1, fp);
//	fprintf(stderr, "Loading WRRBM left %d, right %d, ngram %d, hNum %d, wDim %d, vcbSize %d, burnIn %d\n",
//					m_nLeft, m_nRight, m_nGram, m_hNum, m_wDim, m_vcbSize, m_nBurnIn);


	m_pVB = (double*) m_pool.Allocate(sizeof(double) * m_vcbSize);
	m_pHB = (double*) m_pool.Allocate(sizeof(double) * m_hNum);
	m_D  =  (double*) m_pool.Allocate(sizeof(double) * m_vcbSize * m_wDim);
	
	fread(m_pVB, sizeof(double), m_vcbSize, fp);
	fread(m_pHB, sizeof(double), m_hNum, fp);
	fread(m_D, sizeof(double), m_vcbSize * m_wDim, fp);

	for (int i = 0; i < m_nGram; ++i)
	{
		double *pW = (double*)m_pool.Allocate(sizeof(double) * m_hNum * m_wDim);
		fread(pW, sizeof(double), m_hNum * m_wDim, fp);
		m_vW.push_back(pW);
	}

	return true;
}

SWRRBM * SWRRBM:: RBMFromFile(const string & strPrefix)
{
	WRRBMDictLoader(strPrefix + ".dict");
	SWRRBM *pRBM = new SWRRBM();
	if (pRBM->Load(strPrefix + ".model") == false)
		fprintf(stderr, "loading model failed\n");

	return pRBM;
}

//-----------------------------------------------------------------------------









CRBMNN::
CRBMNN(SWRRBM *pRBM, bool verbose):
	m_verbose(verbose)
{
	fprintf(stderr, "\n\n--------------WRRBM module---------------\n");
	if (pRBM == nullptr)
	{
		m_nLeft = m_nRight = m_nWindow = m_nEbdSize = 0;
		m_iDim = m_oDim = 0;
	}
	else
	{
		m_nLeft	 		= pRBM->m_nLeft;
		m_nRight 		= pRBM->m_nRight;
		m_nWindow	  = pRBM->m_nGram;
		m_nEbdSize	= pRBM->m_wDim;
		m_iDim = m_nEbdSize * m_nWindow;
		m_oDim = pRBM->m_hNum;
		copyWRRBMParam(pRBM);
	}
	fprintf(stderr, "ngram:            %d\n", m_nWindow);
	fprintf(stderr, "embedding size:   %d\n", m_nEbdSize);
	fprintf(stderr, "hidden size:      %d\n", m_oDim);
	fprintf(stderr, "-----------------------------------------\n\n");
}

CRBMNN::~CRBMNN()
{
}

void CRBMNN::
copyWRRBMParam(SWRRBM *pRBM)
{
	// copy embeddings
	m_vEbd.resize(pRBM->m_vcbSize, nullptr);
	//fprintf(stderr, "Init embedding\n");
	for (int i = 0; i < pRBM->m_vcbSize; ++i)
		m_vEbd[i] = &pRBM->m_D[i * m_nEbdSize];
	
	//fprintf(stderr, "Init weight matrix\n");
	m_pB = pRBM->m_pHB;
	m_pW = (double *)m_pool.Allocate(sizeof(double) * m_iDim * m_oDim);

	for (int i = 0; i < m_nWindow; ++i)
	{
		double *pWRBM = pRBM->m_vW[i];
		double *pHBase = m_pW + i * m_nEbdSize * m_oDim;
		for (int h = 0; h < pRBM->m_hNum; ++h)
			for (int d = 0; d < pRBM->m_wDim; ++d)
				pHBase[d * pRBM->m_hNum + h] = pWRBM[h * pRBM->m_wDim + d];
	}
}

void CRBMNN::
ComputeHidden(int *pNgram, int ngram, double *pHidden)
{
	assert(pNgram != NULL && ngram > 0);
	memcpy(pHidden, m_pB, sizeof(double) * m_oDim);
	for (int i = 0; i < ngram; ++i)
	{
		double *pEbd = m_vEbd[CGlobalMap::s_GIDMap.UnkID()];
		if (pNgram[i] >= 0 && pNgram[i] < (int)m_vEbd.size())
			pEbd = m_vEbd[pNgram[i]];
		
		assert(pEbd != NULL);
		double *pW = m_pW + i * m_nEbdSize * m_oDim;
		for (int k = 0; k < m_nEbdSize; ++k)
			for (int o = 0; o < m_oDim; ++o)
				pHidden[o] += pEbd[k] * pW[k * m_oDim + o];
	}

	for (int o = 0; o < m_oDim; ++o)
		pHidden[o] = sigmoid(pHidden[o]);
}


bool CRBMNN::
//Save(const string & strPath)
Save(FILE *fp)
{
//	FILE *fp = fopen(strPath.c_str(), "wb");
	assert(fp);
	fwrite(&m_nLeft,  sizeof(m_nLeft),  1, fp);
	fwrite(&m_nRight, sizeof(m_nRight), 1, fp);
	fwrite(&m_nEbdSize, sizeof(m_nEbdSize), 1, fp);
	fwrite(&m_oDim,   sizeof(m_oDim),   1, fp);

	// weight and bias
	fwrite(m_pW, sizeof(double), m_iDim * m_oDim, fp);
	fwrite(m_pB, sizeof(double), m_oDim, fp);

	// embeddings
	int nEbd = (int)m_vEbd.size();
	fwrite(&nEbd, sizeof(int), 1, fp);
//	int unkID = CGlobalMap::s_GIDMap.UnkID();
	for (int i = 0; i < (int)m_vEbd.size(); ++i)
	{
//		if (i != unkID && m_vEbd[i] == m_vEbd[unkID])
//			continue;
//		fwrite(&i, sizeof(int), 1, fp);
		fwrite(m_vEbd[i], sizeof(double), m_nEbdSize, fp);
	}
//	fclose(fp);
	return true;
}

bool CRBMNN::
Load(FILE *fp)
{
	fprintf(stderr, "------------Loading RBM NN---------------\n");
	assert(fp);
	fread(&m_nLeft,  sizeof(m_nLeft),  1, fp);
	fread(&m_nRight, sizeof(m_nRight), 1, fp);
	fread(&m_nEbdSize, sizeof(m_nEbdSize), 1, fp);
	fread(&m_oDim,   sizeof(m_oDim),   1, fp);
	m_nWindow = m_nLeft + m_nRight;
	m_iDim = m_nEbdSize * m_nWindow;

	// load weight and bias
	m_pW = (double *)m_pool.Allocate(sizeof(double) * m_iDim * m_oDim);
	fread(m_pW, sizeof(double), m_iDim * m_oDim, fp);
	m_pB = (double *)m_pool.Allocate(sizeof(double) * m_oDim);
	fread(m_pB, sizeof(double), m_oDim, fp);

	// load embedding
	int nEmbedding = 0;
	fread(&nEmbedding, sizeof(nEmbedding), 1, fp);
	double *EbdBuf = (double *)m_pool.Allocate(sizeof(double) * m_nEbdSize * nEmbedding);
	fread(EbdBuf,  sizeof(double),  m_nEbdSize * nEmbedding,  fp);

	m_vEbd.resize(nEmbedding, NULL);
	for (int i = 0; i < nEmbedding; ++i)
		m_vEbd[i] = EbdBuf + i * m_nEbdSize;

	fprintf(stderr, "l %d, r %d, embedding size %d, hidden size %d\n",
					m_nLeft, m_nRight, m_nEbdSize, m_oDim);
	fprintf(stderr, "-----------------------------------------\n\n");
	return true;
}
//------------------------------------------------------------------------------









CNNScorer::
CNNScorer(CRBMNN *pRBMNN, int fEbdSize, int oDim)
{
	fprintf(stderr, "\n-------------Create NN scorer-------------\n");
	m_iMode = true;
	m_bAverage = false;
	m_nCount = 1;
	m_pRBMNN = pRBMNN;
	m_nfEbdSize = fEbdSize;
	m_oDim 	 = oDim;
	int iDim = pRBMNN->HDim() + fEbdSize;
	
	m_pWOut = (double *) m_pool.Allocate(sizeof(double) * iDim * oDim);
	for (int i = 0; i < iDim * oDim; ++i)
		m_pWOut[i] = uniform(-0.01, 0.01); 

	m_pTWOut= (double *) m_pool.Allocate(sizeof(double) * iDim * oDim);
	m_pOBias  = (double *)m_pool.Allocate(sizeof(double) * oDim);
	m_pTOBias = (double *)m_pool.Allocate(sizeof(double) * oDim);
	memset(m_pTWOut, 0, sizeof(double) * iDim * oDim);
	memset(m_pTOBias, 0, sizeof(double) * oDim);
	memset(m_pOBias,  0, sizeof(double) * oDim);
//	m_pGWOut= (double *) m_pool.Allocate(sizeof(double) * iDim * oDim);
//	m_pGOBias = (double *)m_pool.Allocate(sizeof(double) * oDim);
//	memset(m_pGWOut, 0, sizeof(double) * iDim * oDim);
//	memset(m_pGOBias, 0, sizeof(double) * oDim);

	fprintf(stderr, "rbm hidden states:            %d\n", m_pRBMNN->HDim());
	fprintf(stderr, "feature embedding dimension:  %d\n", m_nfEbdSize);
	fprintf(stderr, "output dimension:             %d\n", m_oDim);
	fprintf(stderr, "output weight matrix:         %d * %d\n", iDim, oDim);
	fprintf(stderr, "------------------------------------------\n");
}

CNNScorer::
~CNNScorer()
{
	m_pRBMNN = NULL;
}

void CNNScorer::
Save(const string & strPath)
{
	FILE *fp = fopen(strPath.c_str(), "wb");
	assert(fp);

	AverageParameter();				// only save averaged parameters
	m_pRBMNN->Save(fp);
	
	fwrite(&m_nfEbdSize, sizeof(m_nfEbdSize), 1, fp);
	fwrite(&m_oDim, sizeof(m_oDim), 1, fp);
	
	const int iDim = m_pRBMNN->HDim() + m_nfEbdSize;
	fwrite(m_pWOut, sizeof(double), iDim * m_oDim, fp);
	fwrite(m_pOBias, sizeof(double), m_oDim, fp);

	
	int nNonEmpty = 0, nEbd = (int) m_vFEbd.size();
	for (size_t i = 0; i < m_vFEbd.size(); ++i)
		nNonEmpty += m_vFEbd[i] != NULL;

	fwrite(&nEbd, sizeof(nEbd), 1, fp);
	fwrite(&nNonEmpty, sizeof(nNonEmpty), 1, fp);
	for (int i = 0; i < nEbd; ++i)
	{
		if (m_vFEbd[i] != NULL)
		{
			fwrite(&i, sizeof(i), 1, fp);
			fwrite(m_vFEbd[i], sizeof(double), m_nfEbdSize, fp);
		}
	}
	fclose(fp);
}

void CNNScorer::
Load(const string & strPath)
{
	fprintf(stderr, "\n------------Create NN scorer-----------\n");
	FILE *fp = fopen(strPath.c_str(), "rb");
	assert(fp);
	
	m_bAverage = true;				// we only save averaged parameters
	m_pRBMNN->Load(fp);
	fread(&m_nfEbdSize, sizeof(m_nfEbdSize), 1, fp);
	fread(&m_oDim, sizeof(m_oDim), 1, fp);

	// bias and weight
	const int iDim = m_pRBMNN->HDim() + m_nfEbdSize;
	m_pWOut  = (double *) m_pool.Allocate(sizeof(double) * iDim * m_oDim);
	m_pOBias = (double *) m_pool.Allocate(sizeof(double) * m_oDim);
	fread(m_pWOut, sizeof(double), iDim * m_oDim, fp);
	fread(m_pOBias, sizeof(double), m_oDim, fp);

	
	int nAry[2];
	fread(nAry, sizeof(int), 2, fp);
	m_vFEbd.resize(nAry[0], NULL);
	for (int i = 0; i < nAry[1]; ++i)
	{
		int idx = 0;
		fread(&idx, sizeof(idx), 1, fp);
		m_vFEbd[idx] = (double *) m_pool.Allocate(sizeof(double) * m_nfEbdSize);
		fread(m_vFEbd[idx], sizeof(double), m_nfEbdSize, fp);
	}
	fprintf(stderr, "# rbm hidden states:       %d\n", m_pRBMNN->HDim());
	fprintf(stderr, "features embedding size:   %d\n", m_nfEbdSize);
	fprintf(stderr, "output dimension:          %d\n", m_oDim);	
	fprintf(stderr, "nEmbedding:                %d\n", nAry[0]); 
	fprintf(stderr, "non-empty:                 %d\n", nAry[1]);
	fprintf(stderr, "----------------------------------\n\n");
	fclose(fp);
}


void CNNScorer::
InitRBMStates(int *pNgram, double *pRBMUnits)
{
	m_pRBMNN->ComputeHidden(pNgram, m_pRBMNN->WindowSize(), pRBMUnits);
}


void CNNScorer::
Scoring(int *pFID, int nFID, double *pHidden)
{
	double *pRBMUnits = pHidden;
	double *pHLinear = pRBMUnits + m_pRBMNN->HDim();
	double *pOut  = pHLinear + m_nfEbdSize;
	if (m_iMode == true)
	{
		int maxID = -1;
		for (int *p = pFID; p < pFID + nFID;  ++p)
			maxID = std::max(maxID, *p);

		if (maxID >= (int)m_vFEbd.size() && m_iMode)
		{
			m_vFEbd.resize(maxID * 1.2, nullptr);
//			m_vGFEbd.resize(maxID * 1.2, nullptr);
			m_vTFEbd.resize(maxID * 1.2, nullptr);
		}
	}

	// linear layer of sparse feature module
	memset(pHLinear, 0, sizeof(double) * m_nfEbdSize);
	for (int *p = pFID;  p < pFID + nFID;  ++p)
	{
		if (*p == -1 || *p >= (int)m_vFEbd.size())
			continue;

		double *pEbd = m_vFEbd[*p];
		double *pTEbd = m_vTFEbd[*p];
		if (pEbd == nullptr)
		{
			if (m_iMode == false)
				continue;

			pEbd = m_vFEbd[*p] = (double *)m_pool.Allocate(sizeof(double) * m_nfEbdSize);
			memset(pEbd, 0, sizeof(double) * m_nfEbdSize);

//			m_vGFEbd[*p] = (double *)m_pool.Allocate(sizeof(double) * m_nfEbdSize);
//			memset(m_vGFEbd[*p], 0, sizeof(double) * m_nfEbdSize);
			
			pTEbd = m_vTFEbd[*p] = (double *)m_pool.Allocate(sizeof(double) * m_nfEbdSize);
			memset(pTEbd, 0, sizeof(double) * m_nfEbdSize);
		}
		
		for (int h = 0; h < m_nfEbdSize; ++h)
			pHLinear[h] += pEbd[h];
	}

	// compute the output layer
	memcpy(pOut, m_pOBias, sizeof(double) * m_oDim);

	const int iDim = m_nfEbdSize + m_pRBMNN->HDim(); 
	for (int i = 0; i < iDim; ++i)
	{
		double *pW  = m_pWOut + i * m_oDim;
		for (int o = 0 ; o < m_oDim; ++o)
			pOut[o] += pHidden[i] * pW[o]; 
	}

//	int maxID = 0;
//	for (int o = 1; o < m_oDim; ++o)
//		if (pOut[maxID] < pOut[o])
//			maxID = o;
//	return maxID;
}

// only need to compute the linear projection layer
void CNNScorer::
BPError(double *pError, int nonZeroIdx)
{
	double *pErrLinear = pError;
	double *pErrOut = pErrLinear + m_nfEbdSize;

	memset(pErrLinear, 0, sizeof(double) * m_nfEbdSize);
	for (int i = 0; i < m_nfEbdSize; ++i, ++pErrLinear)
	{
		double *pW = m_pWOut + (i + m_pRBMNN->HDim()) * m_oDim;
		*pErrLinear += pErrOut[nonZeroIdx] * pW[nonZeroIdx];
	}
}

void CNNScorer::
AverageParameter()
{
	if (m_bAverage == true)
		return;
	m_bAverage = true;
	m_vAvgEbd.resize(m_vFEbd.size(), NULL);
	for (size_t i = 0; i < m_vFEbd.size(); ++i)
	{
		if (m_vFEbd[i] != NULL)
		{
			m_vAvgEbd[i] = new double[m_nfEbdSize];
			assert(m_vAvgEbd[i]);
			for (int k = 0; k < m_nfEbdSize; ++k)
				m_vAvgEbd[i][k] = m_vFEbd[i][k] - m_vTFEbd[i][k]/m_nCount;

			std::swap(m_vAvgEbd[i], m_vFEbd[i]);
		}
	}
	
	const int iDim = m_pRBMNN->HDim() + m_nfEbdSize;
	m_pAvgOBias = new double[m_oDim];
	m_pAvgWout  = new double[iDim * m_oDim];
	assert(m_pAvgOBias);
	assert(m_pAvgWout);
	for (int o = 0; o < m_oDim; ++o)
	{
		m_pAvgOBias[o] = m_pOBias[o] - m_pTOBias[o]/m_nCount;
		for (int i = 0; i < iDim; ++i)
		{
			int idx = i * m_oDim + o;
			m_pAvgWout[idx] = m_pWOut[idx] - m_pTWOut[idx]/m_nCount;
		}
	}

	std::swap(m_pAvgOBias, m_pOBias);
	std::swap(m_pAvgWout, m_pWOut);
}

void CNNScorer::
UnAverage()
{
	if (m_bAverage == false)
		return;
	m_bAverage = false;
	for (size_t i = 0; i < m_vFEbd.size(); ++i)
		if (m_vFEbd[i])
		{
			std::swap(m_vAvgEbd[i], m_vFEbd[i]);
			delete [] m_vAvgEbd[i];
		}
	m_vAvgEbd.clear();
	std::swap(m_pAvgOBias, m_pOBias);
	std::swap(m_pAvgWout, m_pWOut);
	delete[] m_pAvgOBias;
	delete[] m_pAvgWout;
}


// standard sgd with average parameter, instead of adagrad
void CNNScorer::
Update(int *pFID, int nFID, double *pHidden, double *pError, double rate)
{
	double *pErrLinear = pError;
	double *pErrOut    = pErrLinear + m_nfEbdSize;
	
	// update output layer weight
	const int iDim = m_pRBMNN->HDim() + m_nfEbdSize;
	for (int i = 0; i < iDim; ++i)
	{
		if (pHidden[i] == 0)
			continue;

		int baseIdx = i * m_oDim;
		for (int o = 0; o < m_oDim; ++o)
		{
			if (pErrOut[o] == 0)
				continue;

			double grad = pErrOut[o] * pHidden[i];// pHLinear[i];
//			m_pGWOut[baseIdx + o] += grad * grad;
//			if (m_pGWOut[baseIdx + o] < 1.0e-200)
				m_pWOut[baseIdx + o] -= rate * grad;
//			else
//				m_pWOut[baseIdx + o] -= rate * grad / sqrt(m_pGWOut[baseIdx + o]);
			m_pTWOut[baseIdx + o] -= m_nCount * rate * grad;// / sqrt(m_pGWOut[baseIdx + o]);
		}
	}

	// update output layer bias
	for (int o = 0; o < m_oDim; ++o)
	{
		if (pErrOut[o] == 0)
			continue;

//		m_pGOBias[o] += pErrOut[o] * pErrOut[o];
//		if (m_pGOBias[o] < 1.0e-200)
			m_pOBias[o] -= rate *pErrOut[o];
//		else
//			m_pOBias[o] -= rate *pErrOut[o]/sqrt(m_pGOBias[o]);
		m_pTOBias[o] -= m_nCount * rate *pErrOut[o];// *pErrOut[o]/sqrt(m_pGOBias[o]);
	}


	// update web-feature module parameters
	for (int *p = pFID; p < pFID + nFID; ++p)
	{
		if (*p == -1 || *p >= (int)m_vFEbd.size() || 
				m_vFEbd[*p] == nullptr)
			continue;

		double *pW  = m_vFEbd[*p];
//		double *pGW = m_vGFEbd[*p];
		double *pTW = m_vTFEbd[*p];
		for (int o = 0; o < m_nfEbdSize; ++o)
		{
			if (pErrLinear[o] == 0.0)
				continue;
		
			double grad = pErrLinear[o];
//			pGW[o] += grad * grad; 
//			if (pGW[o] < 1.0e-200)
				pW[o] -= rate * grad;
//			else
//				pW[o] -= rate * grad/sqrt(pGW[o]);
			pTW[o] -= m_nCount * rate *grad;// * grad/sqrt(pGW[o]);
		}
	}
	return;
}
