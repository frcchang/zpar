#ifndef __BASE_EXTRACTOR_H__
#define __BASE_EXTRACTOR_H__

#include "Maps.h"
#include "Template.h"
#include "Lexicon.h"
#include "Analysis.h"
#include "util.h"
#include "Sentence.h"

class CBaseExtractor
{
public:
	typedef CTemplateManager::STemplate STemplate;

public:
	const static int s_MaxKernel;	// maximum number of kernels in each template
	const static int s_kernelUnkId;
	const static int s_fUnkID;
	
	const static wchar_t *s_pwzKerUnk;	// maybe we do not need it
	const static wchar_t *s_pwzWBeg;
	const static wchar_t *s_pwzWEnd;

	const static wchar_t *s_pwzTBeg;
	const static wchar_t *s_pwzTEnd;
public:
	CBaseExtractor(	CTemplateManager *pManager, 
									CWordTagManager *pWTM,
									int left, 		int right,
									bool iMode, 	bool v):
	m_pManager(pManager), m_pWTManager(pWTM), 
	m_nLeft(left),        m_nRight(right),
	m_iMode(iMode),	      m_verbose(v)
	{
	  m_nFID = 3;
	  m_k2IDMap.Insert(s_pwzKerUnk);
		fprintf(stderr, "\n-------------Feature Extractor------------\n");
		fprintf(stderr, "Global feature id: %d\n", m_nFID);
		fprintf(stderr, "left :             %d\n", m_nLeft);
		fprintf(stderr, "right:             %d\n", m_nRight);
		fprintf(stderr,  "------------------------------------------\n");
	}

	CBaseExtractor(CTemplateManager *pManager):
		m_pManager(pManager)
	{
		m_verbose = m_iMode = false;
		m_nLeft = m_nRight = -1;
	}

	CBaseExtractor()
	{
		m_verbose = m_iMode = false;
		m_nLeft = m_nRight = -1;
	}
	
  virtual ~CBaseExtractor()
	{
		m_pWTManager = NULL;
		m_pManager = NULL;
	}

  void ResetMemory()									{m_pool.Recycle();}
	void SetWindow(int l, int r)				{m_nLeft = l, m_nRight = r;}
	void SetVerbose(bool v)							{m_verbose = v;}
	void SetInsertMode(bool mode)				{m_iMode = mode;}
	void SetTempMgr(CTemplateManager *pMgr)	{m_pManager = pMgr;}
	CTemplateManager * GetTempMgr()			{return m_pManager;}
	size_t GetTemplateNum()							{return m_pManager->GetTemplateNum();}
  int GetGlobalFeatureNum()						{return m_nFID;}
	CWordTagManager * GetLexicon()			{return m_pWTManager;}
	virtual string GetName()						{return "BaseExtractor";}
	int GetKernelId(const wchar_t *pwzKernel)	{return m_k2IDMap.GetId(pwzKernel, false);}

	const wstring *GetKernelByID(int KID)
	{
		if (KID < 0 || KID > (int)m_k2IDMap.size())
			return NULL;
		return m_k2IDMap.GetKey(KID);
	}
	
	int GetWindowSize()
	{
		int left = 0, right = 0;
		m_pManager->GetWindowSize(left, right);
		return left > right ? left: right;
	}


public:
	/* for debuging */
	void  DisplayFeatures(_SEN *pSen,  int wind,  const char *pszPath = NULL);
	void  GetStrFeatureVec(SAnalysis *pAna, int wind,  vector<std::pair<wstring, int> > &wstrVec);
	void	UpdateFeatureIDs(SAnalysis *pAna);
	void	InitFeatures(_SEN &rSen);			
	
	bool	operator == (CBaseExtractor &refExtractor);
  bool Save(const string &strPath);
	bool Load(FILE *fp);
	virtual void  ExtractStrHardCodeFeature(int ind,  _SEN &refSen,  vector<std::pair<wstring, int> > &wstrVec) = 0;
private:
	virtual void  extractKernelsFeatures(int wInd, _SEN &refSen) = 0;
	virtual void	extractHardCodeFeature(int ind,  _SEN &refSen) = 0;
  virtual void  saveLanguageDependent(FILE * fp) = 0;
  virtual void  loadLanguageDependent(FILE * fp) = 0;

protected:		
	void	updateDKernels(int wInd, _SEN &rSen);				// update dynamic kernels
	void	constructFeatureIDs(int wInd, _SEN &refSen,  bool dyOnly);
	void 	extractDeepFeatures(int wInd, _SEN &rSen);
	void  extractHiddenFeature(_SEN &rSen);

protected:
  void saveHardCodeFeatures(FILE *fp, CHashMap<wstring, int, WStrHash> &rMap)
	{
		fprintf(fp, "%d\n",(int)rMap.size());
		CHashMap<wstring, int, WStrHash>::iterator iter = rMap.begin();
		for (; iter != rMap.end(); ++iter)
			fprintf(fp, "%s %d\n", wstr2utf8(iter->first).c_str(), iter->second);
	}

	void loadHDFeatures(FILE *fp, CHashMap<wstring, int, WStrHash> &rMap)
	{
		const int BUF_LEN = 65535;
		const int MAX_KEY_LEN = 50;
		static char buf[BUF_LEN];

		if (fgets(buf, BUF_LEN, fp) == NULL)
		{
			fprintf(stderr, "Error: loading parameter failed!");
			exit(0);
		}

		
		int nParam =0, nLoad = 0;
		if (sscanf(buf, "%d", &nParam) != 1)
		{
			fprintf(stderr, "Error: loading hd feature failed %s\n", buf);
			exit(0);
		}

		char pKey[MAX_KEY_LEN];
		int val;
		while (nLoad < nParam && fgets(buf, BUF_LEN, fp) != NULL)
		{
			nLoad++;
			if (sscanf(buf, "%s%d", pKey, &val) != 2)
			{
				fprintf(stderr, "Error: loading hardcode featrue %s\n", buf);
				fprintf(stderr, "buf: %s\n", buf);
				exit(0);
			}
			rMap.Insert(utf82wstr(pKey), val);
		}

		if (nLoad != nParam)
		{
			fprintf(stderr, "Loading HD unfinished\n");
			exit(0);
		}
	}


	int getHardCodeFID(	CHashMap<wstring, int, WStrHash> &rMap, 
											const wchar_t *pwzKey)
	{
		CHashMap<wstring, int, WStrHash>::iterator iter = rMap.Find(pwzKey);
		if (iter == rMap.end())
		{
			if (m_iMode == true)
			{
				rMap.Insert(pwzKey, m_nFID);
				return m_nFID ++;
			}
			else
				return -1;
		}

		return iter->second;
	}


protected:
	typedef CNumberer<wstring,  WStrHash>  WCS2ID_MAP;
	CTemplateManager*	m_pManager;
	CWordTagManager	*	m_pWTManager;		// used for tag set features
	WCS2ID_MAP				m_k2IDMap;

	// dbn features
	int 		m_nLeft;
	int			m_nRight;
	bool		m_iMode;
	bool		m_verbose;
	int			m_nFID;
	CPool		m_pool;
};


bool LoadExtractor(const string &strPath);

#endif  /*__BASE_EXTRACTOR_H__*/
