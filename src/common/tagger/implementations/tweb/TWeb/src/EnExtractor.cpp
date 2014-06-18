#include "Config.h"
#include "EnExtractor.h"
#include "util.h"
void CEnFeatureExtractor::
saveLanguageDependent(FILE *fp)
{
  saveHardCodeFeatures(fp, m_fixNb);
}

void CEnFeatureExtractor::
loadLanguageDependent(FILE *fp)
{
  loadHDFeatures(fp, m_fixNb);
}


void CEnFeatureExtractor::
extractKernelsFeatures(int wind, SSentence &rSen)
{
	map<wstring, int> kernelMap = m_pManager->GetKernelMap();
	map<wstring, int>::iterator iter = kernelMap.begin(), end = kernelMap.end(), findIter;
	const static int nKernels = kernelMap.size();

	assert(wind >=0 && wind < rSen.Length());
	const	wchar_t *pwzWord = rSen.Word(wind);

	int *pKernels = NULL;
	if (rSen.GetKernels(wind) == NULL)
	{
		pKernels = (int*)m_pool.Allocate(sizeof(int) * nKernels);
		rSen.SetKernel(wind, pKernels);
	}
	else
		pKernels = rSen.GetKernels(wind);


	// dumb features are denoted by -1 
	std::fill(pKernels,  pKernels + nKernels, s_kernelUnkId);
	for (iter = kernelMap.begin(); iter != end; ++iter)
	{
		const wchar_t *pwzName = iter->first.c_str();
		int nameLen = iter->first.size();
		int kIdx = iter->second;
		int kID = s_kernelUnkId;	
		
		// fillin kernel ids by kernel names 
		switch (nameLen)
		{
		case 1:	
			if(*pwzName == 't')
				kID = rSen.Tagged(wind) ? rSen.TID(wind): -1;
			else if (*pwzName == 'w')
				kID = rSen.WID(wind);
			else 
				goto UNSUPPORT_KERNEL;
			
			pKernels[kIdx] = kID == -1? s_kernelUnkId: kID;
			break;
		case 2:
			{
				const wchar_t *p = pwzWord;
				if (pwzName[0] == 'g' && pwzName[1] == 'f')	
					kID = rSen.GID(wind);
				else if (*pwzName == 'h')//wcscmp(pwzName, L"hp") == 0)
				{
					while (*p && *p != L'-')
						++p;
					
					if (*p != 0)
						kID = m_k2IDMap.GetId(L"<Hp>", m_iMode);
				}
				else if (*pwzName == 'u')//wcscmp(pwzName, L"uc") == 0)
				{
					while (*p && !(*p >= L'A' && *p <= L'Z'))
						++p;

					if (*p != 0)
						kID = m_k2IDMap.GetId(L"<Uc>", m_iMode);
				}
				else
					goto UNSUPPORT_KERNEL;
			}

			pKernels[kIdx] = kID == -1? s_kernelUnkId: kID;
			break;
		case 3:
			if (*pwzName == 'n')
			{
				const wchar_t *p = pwzWord;
				while (*p && (*p < '0' || *p > '9'))
					++p;
					
				if (*p != 0)
					kID = m_k2IDMap.GetId(L"<num>", m_iMode);
			}
			else
				goto UNSUPPORT_KERNEL;

			pKernels[kIdx] = kID == -1? s_kernelUnkId: kID;
			break;
		default:
			goto UNSUPPORT_KERNEL;
		}
	}

	return;
UNSUPPORT_KERNEL:
	fprintf(stderr, "\nUnknown kernel: %s\n", wstr2utf8(iter->first).c_str());	
	exit(0);
}


void CEnFeatureExtractor::
ExtractStrHardCodeFeature(int wInd,  SSentence &refSen,  vector<std::pair<wstring, int> > &wstrVec)
{
	fprintf(stderr, "warning unfinished\n");
	return;
}


void CEnFeatureExtractor::
extractHardCodeFeature(int wInd,  SSentence &refSen)
{
//	const wchar_t *pWord = refSen.GForm(wInd);
	const wchar_t *pWord = refSen.Word(wInd);
	int *pBase = refSen.GetFeatures(wInd);		// newly modified
	int wLen = wcslen(pWord);	
	int minLen = std::min(wLen, CConfig::nFixLen);
	if (pBase == NULL)
	{
		// bcs: n-1, ecs: n - 1, rep : n-1, inner: n-2
		SSenNode *pNode = refSen.m_pNodes[wInd];
		pNode->m_nHDs  = 2 * minLen;
		pNode->m_nEbds = m_nLeft + m_nRight;
		pNode->m_nFIDs = pNode->m_nHDs + pNode->m_nEbds + 
										 m_pManager->GetTemplateNum();
																		 
		pBase = (int *)m_pool.Allocate(sizeof(int) * pNode->m_nFIDs);
		memset(pBase,  -1,  sizeof(int) * pNode->m_nFIDs);
		refSen.SetFIDs(wInd, pBase);
	}
	pBase += m_nLeft + m_nRight;

  const int _MAX_FIX_LEN = 64;
	wchar_t fixBuf[_MAX_FIX_LEN] = L"pre_", *pBuf = NULL;
	const wchar_t *pIter = pWord;
	int i = 0;
	for (pBuf = fixBuf + 4; i < minLen; ++i)
	{
		*pBuf++ = *pIter++;
		*pBase++ = getHardCodeFID(m_fixNb, fixBuf);
		if (m_verbose == true)
			fprintf(stderr, "[%s %d] ", 	wstr2utf8(fixBuf).c_str(), *(pBase - 1));
	}
	
	memset(fixBuf, 0, sizeof(fixBuf));
	wcscpy(fixBuf, L"sur_");
	
	pIter = pWord + wLen - 1;
	for (pBuf = fixBuf + 4, i = 0; i < minLen; ++i)
	{
		*pBuf++ = *pIter--;
		*pBase++ = getHardCodeFID(m_fixNb, fixBuf);
		if (m_verbose == true)
			fprintf(stderr, "[%s %d] ", wstr2utf8(fixBuf).c_str(), *(pBase - 1));
	}

	if (m_verbose)
		fprintf(stderr, "\n");
}

