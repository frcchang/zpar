#include "Config.h"
#include "BaseExtractor.h"
#include "util.h"
using std::pair;

const wchar_t * CBaseExtractor::s_pwzKerUnk = L"unk";
const wchar_t * CBaseExtractor::s_pwzTBeg = L"tagBeg";
const wchar_t * CBaseExtractor::s_pwzTEnd = L"tagBnd";
const wchar_t * CBaseExtractor::s_pwzWBeg = L"wordBeg";
const wchar_t * CBaseExtractor::s_pwzWEnd = L"wordEnd";
const int CBaseExtractor::s_MaxKernel = 20;	// maximum number of kernels in each template
const int CBaseExtractor::s_kernelUnkId = -1;
const int CBaseExtractor::s_fUnkID = -1;


bool CBaseExtractor::
Save(const string & strPath)
{
	FILE *fp = fopen(strPath.c_str(), "w");
	CHK_OPEN_FAILED(fp, strPath.c_str());

	fprintf(fp, "%s\n", GetName().c_str());
	fprintf(fp, "%d %d %d %d\n", m_nLeft, m_nRight, 
					m_nFID, 	m_k2IDMap.size());

	// 1. save kernel feature Ids
	fprintf(stderr, "Saving kernel features ...\n");
	for (int i = 0; i < m_k2IDMap.size(); ++i)
		fprintf(fp, "%s\n", wstr2utf8(*m_k2IDMap.GetKey(i)).c_str());

  // 2. save language dependent
	saveLanguageDependent(fp);

	// 3. save templates 
	fprintf(stderr, "Saving templates ...\n");
	m_pManager->Save(fp);

	fclose(fp);
	return true;
}


bool CBaseExtractor::
Load(FILE *fp)
{
	char buf[65536];
	if (fgets(buf, 65535, fp) == NULL)
	{
		fprintf(stderr, "Error: empty model file\n");
		exit(0);
	}
	int totalKernel = 0;
	sscanf(buf, "%d%d%d%d",&m_nLeft, &m_nRight, &m_nFID, &totalKernel);
	
	fprintf(stderr, "Loading kernel features ...\n");
	for (int i = 0; i < totalKernel; ++i)
	{
		if (fgets(buf, 65535, fp) == NULL)
		{
			fprintf(stderr, "Error: Loading unfinished\n");
			exit(0);
		}
		removeNewLine(buf);
		m_k2IDMap.Insert(utf82wstr(buf));
	}


	fprintf(stderr, "Loading language dependent features ...\n");
	loadLanguageDependent(fp);

	fprintf(stderr, "Loading templates ...\n");
	bool success = m_pManager->Load(fp);
	return success;
}


void PrintFIDs(int *pfID, int nFID)
{
	for (int i = 0; i < nFID; ++i)
		if (pfID[i] != -1)
			fprintf(stderr,"[%d %d] ",i, pfID[i]);
	fprintf(stderr, "\n");
}


void CBaseExtractor::
updateDKernels(int wInd, _SEN &rSen)
{
	int nDyKernel	= m_pManager->GetDKernelNum();
	const vector<wstring> &kVec	= m_pManager->GetKernelVec();

	// note: when calling this function, kernel pointers should never be NULL
	int *pKernels		= rSen.GetKernels(wInd);
	assert(pKernels != NULL);

	// reset dynamic kernels 
	for (int i = 0;  i < nDyKernel;  ++i)
	{
		if (kVec[i] == L"t")
			pKernels[i] = rSen.Tagged(wInd) ? rSen.TID(wInd):s_kernelUnkId;
		else if (kVec[i] == L"tagged")
		{
			const wchar_t *tagFlag = rSen.Tagged(wInd) ? L"<tagged>" : L"<untagged>";
			pKernels[i] = m_k2IDMap.GetId(tagFlag, m_iMode);
		}
		else  
		{
			fprintf(stderr, "\nUnknown kernel: %s\n", wstr2utf8(kVec[i]).c_str());
			exit(0);
		}
	}
}



void CBaseExtractor::
GetStrFeatureVec(SAnalysis *pAna, int index, 
									vector<std::pair<wstring, int> > &wstrVec)
{
//	wstrVec.clear();
	bool dyOnly = false;
	SSentence &rSen = *pAna->m_pSen;

	// so here we do not do any modification
	const int *pFIDs = rSen.GetFeatures(index);
	const int *pFBase	= pFIDs + rSen.GetHDNum(index);

	vector<CTemplateManager::STemplate *> *pTempVec;
	m_pManager->GetTemplates(&pTempVec);
	
	
	// extracting features
	for (size_t i = 0; i < pTempVec->size(); ++i)
	{
		CTemplateManager::STemplate *pTemp = (*pTempVec)[i];
		if (dyOnly == true && pTemp->IsDynamicTemplate() == false)
			continue;
		
		int kFeatureIDs[s_MaxKernel];
		int kFeatureIndex	= 0;
		bool outOfBound		= false;
		
		wstring strFeat = pTemp->name.substr(0, pTemp->name.find(' ')) + (pTemp->IsDynamicTemplate() ? wstring(L".dy_"):wstring(L"_"));
		
		
		// collect each kernel features needed for the current template
		// kernel features are determined by 1 pendingindex,  2 kernel index
		CTemplateManager::PEND_KERNEL_ITER  iterPendKernel = pTemp->m_vOfts.begin();
		bool	containUnk	 = false;
		for (;   iterPendKernel != pTemp->m_vOfts.end();   ++ iterPendKernel)
		{
			int offset = iterPendKernel->wOft;
			int kInd = iterPendKernel->kIdx, kfId = 0;
			
			if (index + offset >= 0 && index + offset < rSen.Length())
			{
				int *pPOSFids = rSen.GetKernels(index + offset);
				kfId = pPOSFids[kInd];
			}
			else
				outOfBound = true;
			
			if (kfId == s_kernelUnkId)
			{
				containUnk = true;
				break;
			}
			else
			{
				kFeatureIDs[kFeatureIndex++] = kfId;
				strFeat += *GetKernelByID(kfId) + L"_";
			}
		}

		if (containUnk == true || outOfBound == true)
			continue;
	

		int gfId = pTemp->GetGlobalId(kFeatureIDs, kFeatureIndex);
		
		/* fillin current pending's global id vector */
		if (pFBase[i] != gfId)
		{
			fprintf(stderr, "Error: inconsistent feature extracted\n");
			system("pause");
		}

		if (gfId != -1)
			wstrVec.push_back(pair<wstring, int>(strFeat, gfId));
	}
}


//  if dyOnly is true, then only dynamic features are updated
void CBaseExtractor::
constructFeatureIDs(int index, SSentence &rSen, bool dyOnly)
{
	// get templates vector
	vector<CTemplateManager::STemplate *> *pTempVec;
	m_pManager->GetTemplates(&pTempVec);
	
	// reset non-hardcode features 
	int *pFBase = rSen.GetTemplateFids(index);
	int nGFeatures	= m_pManager->GetTemplateNum();
	for (int i = 0; i <nGFeatures ; ++i)
		if (dyOnly == false || (*pTempVec)[i]->IsDynamicTemplate() == true)
			pFBase[i] = s_fUnkID;
	
	
	// extracting features
	for (size_t i = 0; i < pTempVec->size(); ++i)
	{
		CTemplateManager::STemplate *pTemp = (*pTempVec)[i];
		if (dyOnly == true && pTemp->IsDynamicTemplate() == false)
			continue;
		
		int kFeatureIDs[s_MaxKernel];
		int kFeatureIndex	= 0;
		bool outOfBound		= false;
		
		if (m_verbose == true)
		{
			fprintf(stderr, "\ntemplate [%-4lu %s]->",  
							i, wstr2utf8(pTemp->name).c_str());
			pTemp->Display(m_pManager->GetKernelMap(), m_pManager->GetKernelVec());
		}
		
		
		// collect each kernel features needed for the current template
		// kernel features are determined by 1 pendingindex,  2 kernel index
		CTemplateManager::PEND_KERNEL_ITER  iter = pTemp->m_vOfts.begin();
		bool	containUnk	 = false;
		for (; iter != pTemp->m_vOfts.end(); ++ iter)
		{
			int offset = iter->wOft;
			int kInd 	 = iter->kIdx;
			outOfBound = (index + offset < 0 || index + offset >= rSen.Length());
			if (outOfBound == true)
				break;
			
			kFeatureIDs[kFeatureIndex] = rSen.GetKernels(index + offset)[kInd];
			containUnk = kFeatureIDs[kFeatureIndex++] == s_kernelUnkId;
			if (containUnk == true)
				break;
		}

		if (containUnk == true || outOfBound == true)
			continue;
	
		int fID = pTemp->GetGlobalId(kFeatureIDs, kFeatureIndex);
		if (fID == -1)
		{
			if (m_iMode == true)
			{
				fID = m_nFID;
				pTemp->Insert(kFeatureIDs, kFeatureIndex, m_nFID ++);
			}
			else
				continue;
		}
		
		// fillin current pending's global id vector 
		pFBase[i] = fID;
	}

	if (m_verbose == true)
	{
		fprintf(stderr, "\nGlobalFeatureVector: \n");
		int *pFIDs = rSen.GetFeatures(index);
		for (int id = 0; id < rSen.GetFNum(index); ++id)
			if (pFIDs[id] != s_fUnkID)
				fprintf(stderr, "[%d %d] ", id, pFIDs[id]);
		fprintf(stderr, "\n");
	}
}


void CBaseExtractor::
InitFeatures(SSentence &rSen)
{
	vector<int> vGID(rSen.Length());
	for (int i = 0; i < rSen.Length(); ++i)
	{
		if (m_verbose == true)
			fprintf(stderr, "i:%d, w:%s\n",i,	wstr2utf8(rSen.Word(i)).c_str());
		
		extractKernelsFeatures(i, rSen);
		extractHardCodeFeature(i, rSen);
		vGID[i] = rSen.GID(i);
	
		if (m_verbose == true)
			DisplayFeatures(&rSen,  i);
	}

	// initNgrams ids
	int begID = CGlobalMap::s_GIDMap.ID(L"<s>");
	int endID = CGlobalMap::s_GIDMap.ID(L"</s>");
	for (int i = 0; i < rSen.Length(); ++i)
	{
		int *pFeature = rSen.GetFeatures(i);
		for (int idx = i - m_nLeft; idx < i + m_nRight; ++idx)
		{
			*pFeature = begID;
			if (idx >= rSen.Length())
				*pFeature = endID;
			else if (idx >= 0)
				*pFeature = vGID[idx];
			++pFeature;
		}
	}

	for (int i = 0; i < rSen.Length(); ++i)
		constructFeatureIDs(i,  rSen,  false);
}


/* for pending within the window
* 1. extract features (only one pending is involoved)
* 2. re-constructFeatureIds (pendings within the window) 
* 3. update time step (pendings within the window)
*/
void CBaseExtractor::
UpdateFeatureIDs(SAnalysis *pAna)
{
	// update kernels, dynamic kernels only
	SSentence &rSen = *pAna->m_pSen;
	updateDKernels(pAna->m_iUp, rSen);

	if (m_verbose == true)
		DisplayFeatures(&rSen,  pAna->m_iUp);

	
	// re-construct feature ids, dynamic features only 
	int windowSize = GetWindowSize();
	int low		= std::max<int>(0, pAna->m_iUp-windowSize);
	int high	= std::min<int>(rSen.Length(), pAna->m_iUp+windowSize + 1);
	for (int ind = low; ind < high; ++ind)
	{
		// note features of the tagged words should be kept unchanged
		if (rSen.Tagged(ind) == false)
			constructFeatureIDs(ind, rSen,  true);
	}
}



void CBaseExtractor::
DisplayFeatures(_SEN *pSen,  int wInd,  const char * pszPath)
{
	FILE *fp = pszPath == NULL ? stderr: fopen(pszPath, "w");
	if (pszPath != NULL)
		CHK_OPEN_FAILED(fp, pszPath);

	/* 1. display kernel features */
	fprintf(fp, "\n%s kernelFeatures:\n", wstr2utf8(pSen->Word(wInd)).c_str());
	const vector<wstring> &kernelTemplateVec = m_pManager->GetKernelVec();
	size_t nKernelTemps = kernelTemplateVec.size();

	int *pKernelIds = pSen->GetKernels(wInd);
	for (size_t i = 0; i < nKernelTemps; ++i)
	{
		const wstring &strKernel = kernelTemplateVec[i];
		int fkerId = pKernelIds[i];
		fprintf(fp, "%s: ",  wstr2utf8(strKernel).c_str());  
		
		if (fkerId == -1)
			fprintf(fp, "null  ");
		else
		{
			if (strKernel == L"gf")
				fprintf(fp, "%s  ", wstr2utf8(pSen->GForm(wInd)).c_str());
			else if (strKernel == L"t")
				fprintf(fp, "%s  ", fkerId == s_kernelUnkId ? "noTag":wstr2utf8(pSen->Tag(wInd)).c_str());
			else
				fprintf(fp, "%s  ", wstr2utf8(*m_k2IDMap.GetKey(fkerId)).c_str()); 
		}
	}
	fprintf(stderr, "\n-------------------\n");
}



