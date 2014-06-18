#ifndef _SSentenceTENCE_H__
#define _SSentenceTENCE_H__
#include <cassert>
#include "util.h"
#include "GlobalMap.h"

struct SSenNode
{
	SSenNode()								{memset(this, 0, sizeof(*this)); ClearID();}
	int * GetTemplateFids()		{return m_pFIDs + m_nHDs + m_nEbds;}
	int * GetEbdFeatures()		{return m_pFIDs;}
	void ClearTag()						{m_tID = -1, m_isTagged	= false;}
	void ClearID()						{m_tID = m_wID = m_gID = -1;}
	void SetWord(int wID)			{m_wID = wID;}
	void SetGenForm(int gID)	{m_gID = gID;}
	void SetTag(int tID)			{m_isTagged = true,	m_tID = tID;}
	
	static SSenNode * MakeNode(CPool &rPool)
	{
		SSenNode *pNode = (SSenNode *)rPool.Allocate(sizeof(SSenNode));
		memset(pNode, 0, sizeof(SSenNode));
		pNode->ClearID();
		return pNode;
	}
		
	void CopyNonPointerMember(SSenNode *p)
	{
		m_isTagged = p->m_isTagged;
		m_nDKernel = p->m_nDKernel;
		m_nFIDs 	 = p->m_nFIDs;
		m_nKernel	 = p->m_nKernel;
		m_nHDs		 = p->m_nHDs;
		m_nEbds		 = p->m_nEbds;
		m_gID      = p->m_gID;
		m_tID			 = p->m_tID;
		m_wID			 = p->m_wID;
	}

	void ClearFeatures()
	{
		m_pKernel	= NULL;			// dynamic kernels
		m_pFIDs		= NULL;			// dynamic features
		m_pOutScores = NULL;
	
		//  the following two members might be useless
		m_nDKernel	= 0;
		m_nFIDs		= 0;
		m_nKernel	= 0;
		m_nHDs		= 0;
		m_nEbds		= 0;// hard code features
	}

	int		*	m_pKernel;					// dynamic kernels
	int		*	m_pFIDs;						// dynamic features
	
	int			m_nDKernel;
	int			m_nFIDs;
	int			m_nKernel;
	int			m_nEbds;						// ngrams
	int			m_nHDs;							// hard code features

	int     m_wID;
	int			m_tID;
	int			m_gID;							// generalized form 
	bool		m_isTagged;
	double *m_pOutScores;
	wchar_t * m_pwzWord;				// for extracting character based features
	wchar_t * m_pwzGForm;				// for extracting hard code features
//	wchar_t * m_pwzTag;
};
//---------------------------------------------------------------------------


struct SSentence
{	
	SSentence()				{m_pNodes = NULL,m_len = 0;}
	// POS are not copied.
	static vector<SSentence *> CopySentences(vector<SSentence *> &rSens, CPool &rPool);
	static SSentence *CreateSentence(CPool &rPool, std::vector<wstring> lines, bool copyTag);
	static SSentence *CreateSentence(CPool &rPool, wchar_t *pwzline);
	
	void ClearFeatures();
	void Display(FILE *fp = stderr, bool wordOnly = false);
//	void CopyFeature(SSentence  &rSen, CPool &rPool);		used for beam search

	int Length()											{return m_len;}
	void Check(int i)									{assert(m_pNodes != NULL && i >= 0 && i < m_len);}
	bool Tagged(int i)								{Check(i);	return m_pNodes[i]->m_isTagged;}
	int TID(int i)										{Check(i);	return m_pNodes[i]->m_tID;}
	int GID(int i)										{Check(i);	return m_pNodes[i]->m_gID;}
	int WID(int i)										{Check(i);	return m_pNodes[i]->m_wID;}
	int GetHDNum(int i)								{Check(i);	return m_pNodes[i]->m_nHDs;}
	int GetFNum(int i)								{Check(i);  return m_pNodes[i]->m_nFIDs;}
	int *GetTemplateFids(int i)				{Check(i);	return m_pNodes[i]->GetTemplateFids();}
	int *GetEbdFeatures(int i)				{Check(i);	return m_pNodes[i]->GetEbdFeatures();}
	double *GetHScores(int i)					{Check(i);	return m_pNodes[i]->m_pOutScores;}
	int *GetKernels(int i)						{Check(i);	return m_pNodes[i]->m_pKernel;}
	int *GetFeatures(int i)						{Check(i);	return m_pNodes[i]->m_pFIDs;}
	wchar_t * Word(int i)							{Check(i);	return m_pNodes[i]->m_pwzWord;}//CGlobalMap::s_WIDMap.Str(WID(i)).c_str();}
	wchar_t * GForm(int i)						{Check(i);	return m_pNodes[i]->m_pwzGForm;}//CGlobalMap::s_GIDMap.Str(GID(i)).c_str();}
	const wchar_t * Tag(int i)				{Check(i);	return CGlobalMap::s_POSIDMap.Str(TID(i)).c_str();}				
	
	void SetGID(int i, int gID)				{Check(i); m_pNodes[i]->m_gID = gID;}
	void SetTag(int i, int tID)				{Check(i); m_pNodes[i]->SetTag(tID);}
	void SetWID(int i, int wID)				{Check(i); m_pNodes[i]->m_wID = wID;}
	void ClearKernel(int i)						{Check(i); m_pNodes[i]->m_pKernel = NULL;}
	void ClearTag(int i)							{Check(i); m_pNodes[i]->ClearTag();}
	void SetFIDs(int i, int* pFIDs)		{Check(i); m_pNodes[i]->m_pFIDs = pFIDs;}
	void SetHScores(int i , double *pOutput)		{Check(i); m_pNodes[i]->m_pOutScores = pOutput;}
	void SetKernel(int i, int *pKernels)				{Check(i); m_pNodes[i]->m_pKernel = pKernels;}

	void ClearKernels()
	{
		for (int i = 0; i < m_len; ++i)
			ClearKernel(i);
	}

	void ClearTags()
	{
		for (int i = 0; i < m_len; ++i)
			ClearTag(i);
	}

	void GetNgramFeatures(int i,  int **pNgrams, int &nGram)
	{
		Check(i);
		*pNgrams = m_pNodes[i]->m_pFIDs;
		nGram = m_pNodes[i]->m_nEbds; 
	}

	void GetNonNgramFeatures(int i, int **pFids, int &nFnum)
	{
		Check(i);
		*pFids = m_pNodes[i]->m_pFIDs + m_pNodes[i]->m_nEbds;
		nFnum = m_pNodes[i]->m_nFIDs - m_pNodes[i]->m_nEbds;
	}

	~SSentence()
	{
		if (m_pNodes != NULL)
		{
			for (int i = 0; i < m_len; ++i)
				m_pNodes[i] = NULL;

			m_pNodes = NULL, m_len = 0;
		}
	}
	
	SSenNode **m_pNodes;
	int		 m_len;
};
typedef SSentence _SEN;


class CReader
{
public:
	//  used for pos tagger related stuff
	static	vector<_SEN *> ReadSentences(const string &strPath, CPool &rPool, 
																			 bool updateMap = false);
	static  vector<_SEN *> ReadSenPerLine(const string &strPath, CPool &rPool);
	static const int BUF_LEN;	// hard code input buffer length
};


class CWriter
{
public:
	static  bool WriteSentence(vector<_SEN *> &senVec, const char *pszPath);
  static  bool WriteSentencePerLine(vector<_SEN *> &senVec, const char * pszPath);
};
#endif  /*_SSentenceTENCE_H__*/
