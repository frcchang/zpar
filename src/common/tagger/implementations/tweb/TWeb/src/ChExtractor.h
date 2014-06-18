#ifndef __CH_EXTRACTOR_H__
#define __CH_EXTRACTOR_H__

#include "BaseExtractor.h"

class CChFeatureExtractor:public CBaseExtractor
{
public:
	CChFeatureExtractor(CTemplateManager *pManager, 
                      CWordTagManager *pWTM,
//											CWordIDMap			*pWIDMap,
											int l, int r,
                      bool insertMode,    
                      bool verbose):
  CBaseExtractor(	pManager, 	pWTM, //pWIDMap,
									l,  r, 	insertMode, verbose){ }

	CChFeatureExtractor():CBaseExtractor(){}
	~CChFeatureExtractor()
	{m_pManager = NULL;}

	void  ExtractStrHardCodeFeature(int ind,  
																	SSentence &refSen,  
																	vector<std::pair<wstring, int> > &wstrVec);

	string GetName()
	{
		return "ChnExtractor";
	}
private:		
	void	extractHardCodeFeature(int ind,  SSentence &refSen);
	void	extractKernelsFeatures(int wInd, SSentence &refSen);
  void  saveLanguageDependent(FILE *fp);
  void  loadLanguageDependent(FILE *fp);

private:
	
  CHashMap<wstring, int, WStrHash>  m_bcNb;
	CHashMap<wstring, int, WStrHash>  m_ecNb;
	CHashMap<wstring, int, WStrHash>  m_tagSetCNb;
	CHashMap<wstring, int, WStrHash>  m_3chNb;
	CHashMap<wstring, int, WStrHash>  m_repeatNb;
};


#endif  /*__EN_EXTRACTOR_H__*/
