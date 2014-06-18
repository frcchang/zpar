#ifndef __EN_EXTRACTOR_H__
#define __EN_EXTRACTOR_H__
#include "BaseExtractor.h"

class CEnFeatureExtractor:public CBaseExtractor
{
public:
	CEnFeatureExtractor(CTemplateManager *pManager, 
                      CWordTagManager *pWTM,
											int l, int r,
                      bool iMode,  
                      bool v):
  CBaseExtractor(pManager, 	pWTM, l,	r, iMode, v)		{}
	CEnFeatureExtractor():CBaseExtractor()							{}
	~CEnFeatureExtractor()															{m_pManager = NULL;}
	string GetName()																		{return "EngExtractor";}
	void  ExtractStrHardCodeFeature(int ind,  _SEN &refSen,  
																	vector<std::pair<wstring, int> > &wstrVec);
private:		
	void	extractHardCodeFeature(int ind,  SSentence &refSen);
	void	extractKernelsFeatures(int wInd, SSentence &refSen);
  void  saveLanguageDependent(FILE *fp);
  void  loadLanguageDependent(FILE *fp);

private:
	CHashMap<wstring, int, WStrHash>  m_fixNb;  // why I use so wired name
};


#endif  /*__EN_EXTRACTOR_H__*/
