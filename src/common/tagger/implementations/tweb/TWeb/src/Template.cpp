#include"Template.h"

bool CTemplateManager::
Save(const string &strPath)
{
	FILE *fp  = fopen(strPath.c_str(), "w");
	CHK_OPEN_FAILED(fp, strPath.c_str());
	fprintf(fp, "window size:%d %d\n",  m_leftOffset,  m_rightOffset);
	
	// save kernels
	fprintf(fp, "$kernels\n");
	for (int i = 0; i < (int)m_kernelVec.size(); ++i)
	{
		if (i < m_nDyKernels)
			fprintf(fp, "dynamic_%s\n", wstr2utf8(m_kernelVec[i]).c_str());
		else
			fprintf(fp, "%s\n", wstr2utf8(m_kernelVec[i]).c_str());
	}

	// save template
	fprintf(fp, "$posTemplate\n");
	for (size_t i = 0; i < m_tempVector.size(); ++i)
		fprintf(fp,  "%s\n",  wstr2utf8(m_tempVector[i]->name).c_str());

	fclose(fp);
	return true;
}


bool CTemplateManager::
ReadingTemplate(const string & strPath)
{
	FILE *pFile = fopen(strPath.c_str(), "r");
	CHK_OPEN_FAILED(pFile, strPath.c_str());
	
	const wchar_t *strTypes[] = {L"$kernels", L"$posTemplate"};
	const wchar_t *pKeyWindow = L"window size:";
	const wchar_t *pwzDyPrefix= L"dynamic_";		// dynamic kernel prefix

	enum templateType {KERNELS, POS_TEMPLATE, UNK};

	int curType		= -1;
	m_nDyKernels	= 0;
	m_nDyTemplates	= 0;

  const int MAX_BUF_SIZE = 65535;
	wchar_t pBuf[MAX_BUF_SIZE];
	int lineId = 0;
	while (fgetws(pBuf, MAX_BUF_SIZE, pFile))
	{
		++lineId;
		removeNewLine(pBuf);
		// empty line 
		if (wcslen(pBuf) == 0 || (pBuf[0] == ':' && pBuf[1] == ':'))
			continue;

		// reading window size
		if (wcsncmp(pBuf, pKeyWindow, wcslen(pKeyWindow)) == 0)
		{
			wchar_t *p = pBuf + wcslen(pKeyWindow);
			if (swscanf(p, L"%d%d", &m_leftOffset, &m_rightOffset) != 2)
			{
				fprintf(stderr, "Error: invalid line %s\n", 
						wstr2utf8(pBuf).c_str());
				exit(0);
			}
			continue;
		}


		// determine feature type 
		if (pBuf[0] == '$')	
		{
			bool find = false;
			for (size_t i = 0; i < sizeof(strTypes)/sizeof(wchar_t *); ++i)
			{
				if (wcscmp(strTypes[i], pBuf) == 0)
				{
					find = true;
					curType = i;
					break;
				}
			}

			// feature type unspecified
			if (find == false)
			{
				fprintf(stderr, "Error: unknown template type: %s\n", 
						wstr2utf8(pBuf).c_str());
				exit(0);
			}
			continue;
		}


		/* templates */
		if (curType == KERNELS)
		{
			if (m_kernelMap.find(pBuf) != m_kernelMap.end())
			{
				fprintf(stderr, "Warning: Duplicate kernel: %s\n", 
						wstr2utf8(pBuf).c_str());
				continue;
			}

			//	for dynamic kernels, 11/21/2012
			wchar_t *pKernel = pBuf;
			if (wcsncmp(pBuf, pwzDyPrefix, wcslen(pwzDyPrefix)) == 0)
			{
				pKernel	+= wcslen(pwzDyPrefix);
				m_nDyKernels ++;
			}


			int newId = (int)m_kernelMap.size();
			m_kernelMap[pKernel] = newId;		
			m_kernelVec.push_back(pKernel);		
		}
		else if (POS_TEMPLATE)
		{
			//  create template
			STemplate *pTmp	= new STemplate(pBuf,  m_kernelMap,  m_nDyKernels);
			m_tempVector.push_back(pTmp);
			if (pTmp->m_type == _DYNAMIC)
				m_nDyTemplates ++;
		}
	}

	fclose(pFile);
	return true;
}

void CTemplateManager::Display()
{
	fwprintf(stderr, L"window: <%d %d>\n", m_leftOffset,  m_rightOffset);
	
	fwprintf(stderr, L"kernels:\n");
	map<wstring, int>::iterator iter = m_kernelMap.begin();
	for (; iter != m_kernelMap.end(); ++iter)
		fwprintf(stderr, L"<%s %d> ", iter->first.c_str(), iter->second);
	

	fwprintf(stderr, L"\ntemplates:\n");
	for (size_t i = 0; i < m_tempVector.size(); ++i)
		m_tempVector[i]->Display(m_kernelMap, m_kernelVec);
}

bool CTemplateManager::Load(FILE *fp)
{
	if (fp == NULL)
		return false;
	char buf[65536];
	if(fgets(buf, 65535, fp) == NULL)
	{
		fprintf(stderr, "Error: none templates loaded\n");
		return false;
	}
	
	int nTemp = 0;
	if (sscanf(buf, "%d", &nTemp) != 1)
	{
		fprintf(stderr, "Error: number of templates!\n");
		return false;
	}


	if (nTemp != (int)m_tempVector.size())
    fprintf(stderr, "Error: # templates inconsistent %d vs %d\n", 
						nTemp, (int)m_tempVector.size());


	int kidAry[100];
	for (int i = 0; i < nTemp; ++i)
	{
		if (fgets(buf, 65535, fp) == NULL)
		{
			fprintf(stderr, "Error: model error format!\n");
			exit(0);
		}
		bool sameTemp = wcsncmp(utf82wstr(buf).c_str(),	
														m_tempVector[i]->name.c_str(), 
														m_tempVector[i]->name.size()) == 0;
		
    if (sameTemp == false)
      fprintf(stderr, "Error: inconsistent template %s vs %s\n", 
               wstr2utf8(m_tempVector[i]->name).c_str(), buf);

		if (fgets(buf, 65535, fp) == NULL)
		{
			fprintf(stderr, "Error: model error format!\n");
			exit(0);
		}
		
		int nFeatures = 0, nKernels = (int)m_tempVector[i]->m_vOfts.size();//m_pendKernel.size();
		if (sscanf(buf, "%d", &nFeatures) != 1)
		{
			fprintf(stderr, "Error: failed to load feature number!\n");
			exit(0);
		}

		/* for comma template */
		for (int k = 0; k < nFeatures; ++k)
		{
			if (fgets(buf, 65535, fp) == NULL)
			{
				fprintf(stderr, "Error: loading unfinished!");
				exit(0);
			}
      vector<char *> tokVec = Split(buf, " \t\r\n");
			int nKernelLoad = 0, gId = 0;
			for (size_t tokIdx = 0; tokIdx < tokVec.size(); ++ tokIdx)
      {
        char *p = tokVec[tokIdx];
        if (nKernelLoad != nKernels)
          sscanf(p, "%d", &kidAry[nKernelLoad]);
        else
          sscanf(p, "%d", &gId);
				++nKernelLoad;
			}

			if (nKernelLoad != nKernels + 1)
			{
				fprintf(stderr, "Error: inconsistent kernel number!");
				exit(0);
			}
			m_tempVector[i]->Insert(kidAry, nKernels, gId);
		}
	}

	return true;
}

bool CTemplateManager::
Save(FILE *fp)
{
	if (fp == NULL)
		return false;

	///* 1.save window size */
	//fwprintf(fp, L"%d %d\n", m_leftOffset, m_rightOffset);

	///* 2.save kernel map */
	//fwprintf(fp, L"%d\n", (int)m_kernelVec.size());
	//for (size_t i = 0; i < m_kernelVec.size(); ++i)
	//	fwprintf(fp, L"%s\n", m_kernelVec[i].c_str());
	//

	/* 1.save templats*/
	fprintf(fp, "%d\n", (int)m_tempVector.size());
	for (size_t i = 0; i < m_tempVector.size(); ++i)
	{
		STemplate *pTemp = m_tempVector[i];
		fprintf(fp, "%s\n", wstr2utf8(pTemp->name).c_str());

		int nDumb = 0;
		CHashMap<SKIDs, int, IdListHasher>::iterator end = pTemp->m_kernelVal2GlobalID.end(), iter;
		for (iter = pTemp->m_kernelVal2GlobalID.begin(); iter != end; ++ iter)
		{
			int gId = iter->second;
			if (gId == -1)
				nDumb ++;
		}

		int nSave = (int)pTemp->m_kernelVal2GlobalID.size() - nDumb;
		fprintf(fp, "%d\n", nSave);
		if (nSave == 0)
			continue; 

		for (iter = pTemp->m_kernelVal2GlobalID.begin(); iter != end; ++ iter)
		{
			int gId = iter->second;
			if (gId == -1)
				continue;

			SKIDs &refKey = iter->first;
			
			for (int k = 0; k < refKey.len; ++k)
				fprintf(fp, "%d ", refKey.vals[k]);
			fprintf(fp, "%d\n", gId);
		}
	}

	return true;
}
