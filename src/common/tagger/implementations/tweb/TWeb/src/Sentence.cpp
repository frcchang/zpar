#include "Sentence.h"
vector<_SEN *> _SEN::
CopySentences(vector<_SEN *> &rSens, CPool &rPool)
{
	vector<_SEN *> SenCopied;
	for (size_t k = 0; k < rSens.size(); ++k)
	{
		_SEN &refSen = *rSens[k]; 
		_SEN *pSen   = (_SEN *)rPool.Allocate(sizeof(_SEN));
		pSen->m_len = (int)refSen.Length();
		pSen->m_pNodes = (SSenNode **)rPool.Allocate(sizeof(SSenNode*) * pSen->Length());

		for (int i = 0; i < pSen->Length(); ++i)
		{
			pSen->m_pNodes[i] = SSenNode::MakeNode(rPool);
			pSen->m_pNodes[i]->m_pwzWord  = refSen.Word(i);
			pSen->m_pNodes[i]->m_pwzGForm = refSen.GForm(i);
			pSen->m_pNodes[i]->m_gID = refSen.GID(i);
			pSen->m_pNodes[i]->m_wID = refSen.WID(i);
		}
		SenCopied.push_back(pSen);
	}
	return SenCopied;
}

void _SEN::ClearFeatures()
{
	for (int i = 0; i < m_len; ++i)
		m_pNodes[i]->ClearFeatures();
}
	
void _SEN::Display(FILE *fp, bool wordOnly)
{
	for (int i = 0; i < m_len; ++i)
	{
		fprintf(fp, "%s", wstr2utf8(Word(i)).c_str());
		if (wordOnly == false)
			fprintf(fp, "_%s", Tagged(i) ? wstr2utf8(Tag(i)).c_str(): "unTagged");
		fprintf(fp, "  ");
	}
	fprintf(fp, "\n");
}

_SEN * SSentence::
CreateSentence(CPool &rPool, vector<wstring> lines, bool updateMap)
{
	_SEN *pSen = (_SEN *)rPool.Allocate(sizeof(_SEN));
	pSen->m_pNodes = (SSenNode **)rPool.Allocate(sizeof(SSenNode*) * lines.size());
	pSen->m_len	   = lines.size();
	
	const int BUF_LEN = 65535;
	wchar_t buf[BUF_LEN];
	vector<wchar_t *> itemVec;
	CStrIDMap &wordIDMap  = CGlobalMap::s_WIDMap;
	CStrIDMap &posIDMap   = CGlobalMap::s_POSIDMap; 
	CStrIDMap &gFormIDMap = CGlobalMap::s_GIDMap;
	for (size_t i = 0; i < lines.size(); ++i)
	{
		pSen->m_pNodes[i] = SSenNode::MakeNode(rPool);
		wcscpy(buf, lines[i].c_str());
		
		if (Tokenize(buf, itemVec) == false)
		{
			fprintf(stderr, "Invalid format %s\n", wstr2utf8(lines[i]).c_str());
			exit(0);
		}

		if (updateMap == true)
		{
			wordIDMap.Add(itemVec[_GWORD]);
			gFormIDMap.Add(itemVec[_GGenFORM]);
			posIDMap.Add(itemVec[_GTAG]);
		}

		pSen->m_pNodes[i]->m_pwzWord  = copyWcs(itemVec[_GWORD],  &rPool);
		pSen->m_pNodes[i]->m_pwzGForm = copyWcs(itemVec[_GGenFORM], &rPool);
		pSen->SetWID(i, wordIDMap.ID(itemVec[_GWORD]));
		pSen->SetGID(i, gFormIDMap.ID(itemVec[_GGenFORM]));
		pSen->SetTag(i, posIDMap.ID(itemVec[_GTAG]));
	}

	return pSen;
}
	

const int CReader::BUF_LEN = 1<<16;
vector<_SEN *> CReader::
ReadSentences(const string &strPath, CPool &rPool, bool updateMap)
{
	FILE * fpIn = fopen(strPath.c_str(), "r");
	if (fpIn == NULL)
	{
		fprintf(stderr, "%s open failed\n", strPath.c_str());
		exit(0);
	}

	static char buf[BUF_LEN];
	vector<wstring> lines;
	vector<_SEN*> senVec;
	int nLine = 0;
	while (fgets(buf, BUF_LEN, fpIn) != NULL)
	{
		if (++nLine % 100000 == 0)
			fprintf(stderr, "Reading %d lines from %s....\r", 
							nLine, strPath.c_str());
		
		removeNewLine(buf);
		if (strlen(buf) == 0)
		{
			if (lines.size() != 0)
			{
				_SEN *pSen = SSentence::CreateSentence(rPool, lines, updateMap);
				senVec.push_back(pSen);
				lines.clear();
			}
		}
		else
		{
			wstring line = CCodeConversion::UTF8ToUnicode(buf);
			lines.push_back(line);
		}
	}

	// handle the last one
	if (lines.size() != 0)
	{
		_SEN *pSen = _SEN::CreateSentence(rPool, lines, updateMap);
		senVec.push_back(pSen);
		lines.clear();
	}

	fprintf(stderr, "%-7d sen read from %s\n", (int)senVec.size(), strPath.c_str());
	fclose(fpIn);
	return senVec;
}


vector<SSentence *> CReader::
ReadSenPerLine(const string &strPath, CPool &rPool)
{
	fprintf(stderr, "Function not supported\n");
	exit(0);
	vector<_SEN *> senVec;
	return senVec;
}


//--------------------------------------------------------------------------------------------
bool CWriter::
WriteSentence(vector<_SEN *> &senVec, const char *pszPath)
{
	FILE *fp = fopen(pszPath, "w");
	CHK_OPEN_FAILED(fp, pszPath);

	for (size_t i = 0; i < senVec.size(); ++i)
	{
		SSentence *pSen = senVec[i];
		for (int k = 0; k < pSen->Length(); ++k)
		{
			string word = wstr2utf8(pSen->Word(k));
			string tag  = wstr2utf8(pSen->Tag(k));
			fprintf(fp, "%s\t%s\n",word.c_str(), tag.c_str());
		}
		fprintf(fp, "\n");
	}
	fclose(fp);
	return true;
}


bool CWriter::
WriteSentencePerLine(vector<_SEN *> &senVec, const char * pszPath)
{
  FILE *fpRes = fopen(pszPath, "w");
  if (fpRes == nullptr)
  {
    fprintf(stderr, "Error: open %s failed\n", pszPath);
    return false;
  }

  for (SSentence * pSen: senVec)
  {
    for (int wIdx = 0; wIdx < pSen->Length(); ++ wIdx)
      fprintf(fpRes, "%s/%s ", CCodeConversion::UnicodeToUTF8(pSen->Word(wIdx)).c_str(), 
                                CCodeConversion::UnicodeToUTF8(pSen->Tag(wIdx)).c_str());
    fprintf(fpRes, "\n"); 
  }

  fclose(fpRes);
  return true;
}


#if 0
void _SEN::CopyFeature(_SEN &rSen, CPool &rPool)
{
	for (int i = 0; i < m_len; ++i)
	{
		m_pNodes[i]->CopyNonPointerMember(rSen.m_pNodes[i]);
		m_pNodes[i]->m_pKernel  = (int *)rPool.Allocate(sizeof(int) * m_pNodes[i]->m_nKernel);
		memcpy(m_pNodes[i]->m_pKernel,  rSen.m_pNodes[i]->m_pKernel,  sizeof(int) * m_pNodes[i]->m_nKernel);

		m_pNodes[i]->m_pFIDs	= (int *)rPool.Allocate(sizeof(int) * m_pNodes[i]->m_nFIDs);
		memcpy(m_pNodes[i]->m_pFIDs,  rSen.m_pNodes[i]->m_pFIDs,  sizeof(int) * m_pNodes[i]->m_nFIDs);
	}
}

void _SEN:: Init(CPool &rPool, _SEN &refSen)
{
	m_len = (int)refSen.Length();
	m_pNodes = (SSenNode **)rPool.Allocate(sizeof(SSenNode*) * m_len);

	for (int i = 0; i < m_len; ++i)
	{
		m_pNodes[i] = SSenNode::MakeNode(rPool);
		m_pNodes[i]->m_pwzWord = refSen.Word(i);
		m_pNodes[i]->m_pwzWord = refSen.GForm(i);
		m_pNodes[i]->m_gID  = refSen.GID(i);
		m_pNodes[i]->m_wID	= refSen.WID(i);
	}
}
	
void _SEN:: CopyTags(_SEN &rSen, CPool &rPool)
{
	for (int i = 0; i < rSen.Length(); ++i)
		SetTag(i, rSen.TID(i));
}
	
_SEN * _SEN::CreateSentence(CPool &rPool, wchar_t *pwzline)
{
	fprintf(stderr, "CreateSentence, function Not Support\n");
	exit(0);

	if (pwzline == NULL)
	{
		fprintf(stderr, "Warning: Create sentence with NULL pointer\n");
		return NULL;
	}


	vector<wchar_t *> words = Split(pwzline,  L" \r\t\n");
	if (words.size() == 0)
	{
		fprintf(stderr, "Warning: Create sentence with empty line\n");
		return NULL;
	}

	SSentence *pSen = (SSentence*)rPool.Allocate(sizeof(SSentence));
	pSen->m_pNodes = (SSenNode **)rPool.Allocate(sizeof(SSenNode*) * words.size());
	pSen->m_len	   = words.size();
	for (size_t i = 0; i < words.size(); ++i)
	{
		pSen->m_pNodes[i] = SSenNode::MakeNode(rPool);
     
     wchar_t *pwzTag = wcschr(words[i], L'/');
     if (pwzTag != NULL)
       *pwzTag++ = 0;

		pSen->m_pNodes[i]->m_pwzWord = (wchar_t *)rPool.Allocate(sizeof(wchar_t) * (wcslen(words[i]) + 1) );
		wcscpy(pSen->m_pNodes[i]->m_pwzWord,  words[i]);

     // initialize tag if specified
     if (pwzTag != NULL)
     {
       pSen->m_pNodes[i]->m_pwzTag = (wchar_t *)rPool.Allocate(sizeof(wchar_t) * (wcslen(pwzTag) + 1) );
       wcscpy(pSen->m_pNodes[i]->m_pwzTag, pwzTag);
       pSen->m_pNodes[i]->m_isTagged = true;
     }
	}
	return pSen;
}

#endif
