#include "ChExtractor.h"
#include "Config.h"
using std::pair;

void CChFeatureExtractor::
saveLanguageDependent(FILE *fp)
{
	saveHardCodeFeatures(fp, m_3chNb);
	saveHardCodeFeatures(fp, m_bcNb);
	saveHardCodeFeatures(fp, m_ecNb);
	saveHardCodeFeatures(fp, m_tagSetCNb);
	saveHardCodeFeatures(fp, m_repeatNb);
}

void CChFeatureExtractor::
loadLanguageDependent(FILE *fp)
{
	loadHDFeatures(fp, m_3chNb);
	loadHDFeatures(fp, m_bcNb);
	loadHDFeatures(fp, m_ecNb);
	loadHDFeatures(fp, m_tagSetCNb);
	loadHDFeatures(fp, m_repeatNb);
}

void CChFeatureExtractor::
extractKernelsFeatures(int wind, SSentence &rSen)
{
#if 0
	map<wstring, int> kernelMap = m_pManager->GetKernelMap();
	map<wstring, int>::iterator iter = kernelMap.begin(), end = kernelMap.end(), findIter;
	const static int nKernels = kernelMap.size();

	assert(wind >=0 && wind < rSen.Length());
	wchar_t *pwzWord = rSen.Word(wind);
	int wLen = wcslen(pwzWord);

	int *pKernels = NULL;
	if (rSen.GetKernels(wind) == NULL)
	{
		pKernels = (int*)m_pool.Allocate(sizeof(int) * nKernels);
		rSen.SetKernel(wind, pKernels);
		rSen.m_pNodes[wind]->m_nKernel = nKernels;
	}
	else
		pKernels = rSen.GetKernels(wind);


	/* dumb features are denoted by -1 */
	std::fill(pKernels,  pKernels + nKernels, s_kernelUnkId);
	for (iter = kernelMap.begin(); iter != end; ++iter)
	{
		const wchar_t *pwzName = iter->first.c_str();
		int nameLen = iter->first.size();
		
		int kernelIndex = iter->second;
		int kernelId = s_kernelUnkId;		// kernel feature id;
		
		/* fillin kernel ids by kernel names */
		switch (nameLen)
		{
		case 1:	// w, t
			if (*pwzName == 'w')
				kernelId = m_kernelIdNumberer.GetId(pwzWord, m_insertNewFeatureMode);
			else if(*pwzName == 't')
			{
				if (rSen.Tagged(wind) == true)
				{
					const wchar_t *pwzTag = rSen.Tag(wind);
					kernelId = m_kernelIdNumberer.GetId(pwzTag, m_insertNewFeatureMode);
				}
				else 
					kernelId = s_kernelUnkId;
			}
			else 
				goto UNSUPPORT_KERNEL;

			pKernels[kernelIndex] = kernelId == -1? s_kernelUnkId: kernelId;
			break;

		case 2:	// bw or ew or de
			{
				assert (*pwzName == 'b' || *pwzName == 'e' || *pwzName == 'd');
				if (wcscmp(L"de", pwzName) == 0 || wcscmp(L"di", pwzName) == 0)
				{
					kernelId = -1;
					pKernels[kernelIndex] = kernelId == -1? s_kernelUnkId: kernelId;
				}
				else
				{
					int wCount = m_pWTManager->GetWordCount(pwzWord);
					if (CConfig::bRareFlag == true && wCount >= CConfig::nRareWord)
						pKernels[kernelIndex] = s_kernelUnkId;
					else
					{
						wchar_t pwzBE[2] = {0};
						if (wLen == 1)
						{
							if (CConfig::bLen1WordEndWord == true && *pwzName == 'e')
							{
								pwzBE[0] = *(pwzWord + wLen - 1); 
								kernelId = m_kernelIdNumberer.GetId(pwzBE,  m_insertNewFeatureMode);
								pKernels[kernelIndex] = kernelId == -1? s_kernelUnkId: kernelId;
							}
							else
								pKernels[kernelIndex] = s_kernelUnkId;
						}
						else
						{
							wchar_t pwzBE[2] = {0};
							pwzBE[0] = *pwzName == 'b' ? *pwzWord: *(pwzWord + wLen - 1); 
							kernelId = m_kernelIdNumberer.GetId(pwzBE,  m_insertNewFeatureMode);
							pKernels[kernelIndex] = kernelId == -1? s_kernelUnkId: kernelId;
						}
					}
				}
				break;
			}
		case 3:
			{
				if (wcscmp(pwzName, L"low") != 0)
					goto UNSUPPORT_KERNEL;
				else
				{
					int freq = m_pWTManager->GetWordCount(pwzWord);
					if (freq < CConfig::nLowCount)
					{
						const wchar_t *lowFlag = L"LOW";
						kernelId = m_kernelIdNumberer.GetId(lowFlag, m_insertNewFeatureMode);
						pKernels[kernelIndex] = kernelId == -1? s_kernelUnkId: kernelId;
					}
					else
						pKernels[kernelIndex] = s_kernelUnkId;
				}
				break;
			}
		case 6:
			{
				if (wcscmp(pwzName, L"tagged") != 0)
					goto UNSUPPORT_KERNEL;
				else
				{
					const wchar_t *tagFlag = rSen.Tagged(wind) == true ? L"tagged" : L"untagged";
					kernelId = m_kernelIdNumberer.GetId(tagFlag, m_insertNewFeatureMode);
					pKernels[kernelIndex] = kernelId == -1? s_kernelUnkId: kernelId;
				}
				break;
			}
		case 8:	// tagsetbw, tagsetew
			{
				assert(wcscmp(pwzName, L"tagsetbw") == 0 || wcscmp(pwzName, L"tagsetew") == 0);
				if (wLen == 1)
					pKernels[kernelIndex] = s_kernelUnkId;
				else
				{
					wchar_t BEBuf[2] = {0};
					BEBuf[0] = pwzName[6] == 'b' ? pwzWord[0]:pwzWord[wLen - 1];

					const wchar_t *pTags = NULL;
					CWordTagManager::WORD_CH w_ch = pwzName[6] == 'b' ? CWordTagManager::B_CH:CWordTagManager::E_CH;
				
					if (m_pWTManager->GetTags(BEBuf, &pTags, w_ch) == true)
						kernelId = m_kernelIdNumberer.GetId(pTags, m_insertNewFeatureMode);
					else
						kernelId = m_kernelIdNumberer.GetId(L"emptyTagSet", m_insertNewFeatureMode);

					pKernels[kernelIndex] = kernelId == -1? s_kernelUnkId: kernelId;
				}
				break;
			}
		default:
			goto UNSUPPORT_KERNEL;
		}
	}

	return;
UNSUPPORT_KERNEL:
	fwprintf(stderr, L"Error: unsupport kernel type %ls\n", iter->first.c_str());	
	exit(0);
#endif
}


void CChFeatureExtractor::
extractHardCodeFeature(int wInd,  SSentence &refSen)
{
	const wchar_t *pWord = refSen.Word(wInd);
	int wLen = wcslen(pWord);
	
	//int *pBase = rNode.m_pFIds;
	int *pBase = refSen.GetFeatures(wInd);		// newly modified
	if (pBase == NULL)
	{
		// bcs: n-1, ecs: n - 1, rep : n-1, inner: n-2
		SSenNode *pNode = refSen.m_pNodes[wInd];
		pNode->m_nHDs  = wLen > 1 ? (4 * wLen - 5): 1;
		pNode->m_nEbds = m_nLeft + m_nRight;
		pNode->m_nFIDs = pNode->m_nHDs + pNode->m_nEbds + m_pManager->GetTemplateNum();
																		 
		pBase = (int *)m_pool.Allocate(sizeof(int) * pNode->m_nFIDs);
		memset(pBase,  -1,  sizeof(int) * pNode->m_nFIDs);
		refSen.SetFIDs(wInd, pBase);
	}
	pBase += m_nLeft + m_nRight;

	*pBase = -1;
	if (m_verbose == true)
	{
		fwprintf(stderr, L"word: %ls, len %d, nHDF %d:  ",
			pWord, wLen, refSen.GetHDNum(wInd));
	}

	
	if (wLen > 1)
	{
		wchar_t fc = *pWord, lc = pWord[wLen - 1];
		wchar_t bcs[3] = {0};
		wchar_t ecs[3] = {0};
		wchar_t ch[2] = {0};
		wchar_t rep[2] = {0};
		*bcs = fc;
		*ecs = lc;

//		if ((m_len2Limit && wLen > 2) || (m_len2Limit == false))
			for (int i = 0; i < wLen; ++i)
			{
				if (i != 0)
				{
					bcs[1] = pWord[i];
					*pBase ++ = getHardCodeFID(m_bcNb, bcs);

					if (m_verbose == true)
						fwprintf(stderr, L"<bcs: %ls, fid: %d> ", bcs, pBase[-1]);

					if (pWord[i] == pWord[i-1])
					{
						rep[0] = pWord[i];
						*pBase ++ = getHardCodeFID(m_repeatNb, rep);

						if (m_verbose == true)
							fwprintf(stderr, L"<rep: %ls, fid: %d> ", rep, pBase[-1]);
					}
					else
						*pBase ++ = -1;
				}
			}


//		if ((m_len2Limit && wLen > 2) || (m_len2Limit == false))
			for (int i = 0; i < wLen; ++i)
			{
				if (i != wLen - 1)
				{
					ecs[1] = pWord[i];
					*pBase ++ = getHardCodeFID(m_ecNb, ecs);

					if (m_verbose == true)
						fwprintf(stderr, L"<ecs: %ls, fid: %d> ", ecs, pBase[-1]);
				}
			}

		for (int i = 1; i < wLen - 1; ++i)
		{
			if (i != 0 && i != wLen - 1)
			{
				ch[0] = pWord[i];
				if (m_verbose == true)
					fwprintf(stderr, L"<innerchar: %ls, tags: ", ch);
				*pBase ++ = getHardCodeFID(m_tagSetCNb, ch);

				if (m_verbose == true)
					fwprintf(stderr, L"%ls, id: %d> ",ch, pBase[-1]);
			}
		}
	}
	else
	{
		if (wInd > 0 && wInd < refSen.Length() - 1)
		{
			wchar_t three_char[4] = {0};
			const wchar_t *plw = refSen.Word(wInd - 1);	//last
			const wchar_t *pnw = refSen.Word(wInd + 1);	//next

			int lastLen = wcslen(plw);
			three_char[0] = plw[lastLen - 1];
			three_char[1] = *pWord;
			three_char[2] = *pnw;
			*pBase = getHardCodeFID(m_3chNb, three_char);

			if (m_verbose)
				fwprintf(stderr, L"<3char: %ls, id %d>", three_char, *pBase);
		}
	}

	if (m_verbose)
		fwprintf(stderr, L"\n");
}


void CChFeatureExtractor::
ExtractStrHardCodeFeature(int wInd,  SSentence &refSen,  vector<std::pair<wstring, int> > &wstrVec)
{
	const wchar_t *pWord = refSen.Word(wInd);
	int wLen = wcslen(pWord);
	wstrVec.clear();
	
	//int *pBase = rNode.m_pFIds;
	const int *pBase = refSen.GetFeatures(wInd);		// newly modified

	if (m_verbose == true)
	{
		fwprintf(stderr, L"word: %ls, len %d, nHDF %d:  ",
			pWord, wLen, refSen.GetHDNum(wInd));
	}

	
	if (wLen > 1)
	{
		++pBase;
		wchar_t fc = *pWord, lc = pWord[wLen - 1];
		wchar_t bcs[3] = {0};
		wchar_t ecs[3] = {0};
		wchar_t ch[2] = {0};
		wchar_t rep[2] = {0};
		*bcs = fc;
		*ecs = lc;

//		if ((m_len2Limit && wLen > 2) || (m_len2Limit == false))
			for (int i = 0; i < wLen; ++i)
			{
				if (i != 0)
				{
					bcs[1] = pWord[i];
					int fid = getHardCodeFID(m_bcNb, bcs);
					wstrVec.push_back(pair<wstring, int>(wstring(L"bcs_")+bcs, fid));

					if (*pBase ++ != fid)
					{
						fprintf(stderr, "Error: inconsistent feature\n");
						system("pause");
					}
//					*pBase ++ = getHardCodeFID(m_bcNb, bcs);

					if (m_verbose == true)
						fwprintf(stderr, L"<bcs: %ls, fid: %d> ", bcs, pBase[-1]);

					if (pWord[i] == pWord[i-1])
					{
						rep[0] = pWord[i];
						int fid = getHardCodeFID(m_repeatNb, rep);
						wstrVec.push_back(pair<wstring ,int>(wstring(L"rep_")+rep, fid));
						if (*pBase ++ != fid)
						{
							fprintf(stderr, "Error: inconsistent feature\n");
							system("pause");
						}

						if (m_verbose == true)
							fwprintf(stderr, L"<rep: %ls, fid: %d> ", rep, pBase[-1]);
					}
					else
					{
						if (*pBase ++ != -1)
						{
							fprintf(stderr, "Error: inconsistent feature\n");
							system("pause");
						}
					}
				}
			}


//		if ((m_len2Limit && wLen > 2) || (m_len2Limit == false))
			for (int i = 0; i < wLen; ++i)
			{
				if (i != wLen - 1)
				{
					ecs[1] = pWord[i];
					int fid = getHardCodeFID(m_ecNb, ecs);
					wstrVec.push_back(pair<wstring, int>(wstring(L"ecs_")+ecs, fid));
					if (*pBase ++ != fid)
					{
						fprintf(stderr, "Error: inconsistent feature\n");
						system("pause");
					}

					if (m_verbose == true)
						fwprintf(stderr, L"<ecs: %ls, fid: %d> ", ecs, pBase[-1]);
				}
			}

		for (int i = 1; i < wLen - 1; ++i)
		{
			if (i != 0 && i != wLen - 1)
			{
				ch[0] = pWord[i];
				if (m_verbose == true)
					fwprintf(stderr, L"<innerchar: %ls, tags: ", ch);
				

				int fid = getHardCodeFID(m_tagSetCNb, ch);
				wstrVec.push_back(pair<wstring, int>(wstring(L"ch_")+ch, fid));
				if (*pBase ++ != fid)
				{
					fprintf(stderr, "Error: inconsistent feature\n");
					system("pause");
				}

				if (m_verbose == true)
					fwprintf(stderr, L"%ls, id: %d> ",ch, pBase[-1]);
			}
		}
	}
	else
	{
		if (wInd > 0 && wInd < refSen.Length() - 1)
		{
			wchar_t three_char[4] = {0};
			const wchar_t *plw = refSen.Word(wInd - 1);	//last
			const wchar_t *pnw = refSen.Word(wInd + 1);	//next

			int lastLen = wcslen(plw);
			three_char[0] = plw[lastLen - 1];
			three_char[1] = *pWord;
			three_char[2] = *pnw;

			int fid = getHardCodeFID(m_3chNb, three_char);
			wstrVec.push_back(pair<wstring, int>(wstring(L"3ch_")+three_char, fid));
			if (*pBase != fid)
			{
				fprintf(stderr, "Error: inconsistent feature\n");
				system("pause");
			};

			if (m_verbose)
				fwprintf(stderr, L"<3char: %ls, id %d>", three_char, *pBase);
		}
	}

	if (m_verbose)
		fwprintf(stderr, L"\n");
}

