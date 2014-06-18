#include "util.h"

char * copyStr(const char *pszStr, CPool *pPool)
{
	if (pszStr == NULL)
	{
		fprintf(stderr, "Error: copy null str\n");
		return NULL;
	}

	char *pszRes = NULL;
	if (pPool == NULL)
		pszRes = new char[strlen(pszStr) + 1];
	else
		pszRes = (char *)pPool->Allocate(strlen(pszStr) + 1);

	CHK_NEW_FAILED(pszRes);

	strncpy(pszRes, pszStr, strlen(pszStr) + 1);
	return pszRes;
}

//--------------------------------------------------------------------------------
wchar_t *copyWcs(const wchar_t *pwzStr, CPool *pPool)
{
	if (pwzStr == NULL)
	{
		fprintf(stderr, "Error: copy null str\n");
		return NULL;
	}

	wchar_t *pwzRes = NULL;
	if (pPool == NULL)
		pwzRes = new wchar_t[wcslen(pwzStr) + 1];
	else
		pwzRes = (wchar_t *)pPool->Allocate(sizeof(wchar_t) * (wcslen(pwzStr) + 1));

	CHK_NEW_FAILED(pwzRes);

	wcsncpy(pwzRes, pwzStr, wcslen(pwzStr) + 1);
	return pwzRes;
}

//--------------------------------------------------------------------------------
vector<char *> Split(char *pBuf, const char *pDelem)
{
	vector<char *> resVec;
	if (pBuf == NULL || pDelem == NULL)
		return resVec;

	char *p = pBuf;
	while (true)
	{
		while (*p && strchr(pDelem, *p) != NULL)
			*p ++ = 0;

		if (!*p)
			break;

		resVec.push_back(p);
		while (*p && strchr(pDelem, *p) == NULL)
			p++;
	}

	return resVec;
}

//--------------------------------------------------------------------------------
vector<wchar_t *> Split(wchar_t *pBuf, const wchar_t *pDelem)
{
	vector<wchar_t *> resVec;
	if (pBuf == NULL || pDelem == NULL)
		return resVec;

	wchar_t *p = pBuf;
	while (true)
	{
		while (*p && wcschr(pDelem, *p) != NULL)
			*p ++ = 0;

		if (!*p)
			break;

		resVec.push_back(p);
		while (*p && wcschr(pDelem, *p) == NULL)
			p++;
	}

	return resVec;
}

//--------------------------------------------------------------------------------
vector<int> Shuffel(int nElems)
{
	vector<int> resVec;
	if (nElems == 0)
		return resVec;

	resVec.resize(nElems);
	for (int i = 0; i < nElems; ++i)
		resVec[i] = i;
 
	random_shuffle(resVec.begin(), resVec.end());
	return resVec;
}

//--------------------------------------------------------------------------------
bool Tokenize(wchar_t * pwzLine, vector <wchar_t *> & rResVec, bool ConllFormat)
{
	rResVec.clear();
	if (ConllFormat == true)
	{
		fprintf(stderr, "Conll format unsupport\n");
		return false;
	}
	else
	{
    rResVec = Split(pwzLine, L" \r\t\n");
		return (int)rResVec.size() >= _GTAG+ 1;
	}
}
//--------------------------------------------------------------------------------
