#include "GlobalMap.h"
CStrIDMap CGlobalMap::s_WIDMap(0);
CStrIDMap CGlobalMap::s_GIDMap(0);
CStrIDMap CGlobalMap::s_POSIDMap(-1);
	
#define MAX_POS_LEN  1<<8
#define MAX_BUF_LEN  1<<10
void CStrIDMap::Save(FILE *fp)
{
	fwprintf(fp, L"%d\n", m_StrVec.size());
	for (size_t i = 0; i < m_StrVec.size(); ++i)
		fwprintf(fp, L"%ls\n", m_StrVec[i].c_str());
}

void CStrIDMap::Load(FILE *fp)
{
	wchar_t buf[MAX_POS_LEN];
	int nKey = 0, nLoad = 0;
	if (fgetws(buf, MAX_POS_LEN, fp) == NULL ||
			swscanf(buf, L"%d", &nKey) != 1)
	{
		fprintf(stderr, "Loading maps failed %s\n", wstr2utf8(buf).c_str());
		exit(0);
	}

	while (nLoad < nKey && fgetws(buf, MAX_POS_LEN, fp) != NULL)
	{
		removeNewLine(buf);
		if (m_Str2IDMap.find(buf) != m_Str2IDMap.end() &&
				m_UnkStr != buf)
		{
			fprintf(stderr, "Error: duplicate item %s\n", 
							wstr2utf8(buf).c_str());
			exit(0);
		}
		Add(buf);
		++nLoad;
	}

	if (nLoad != nKey)
	{
		fprintf(stderr, "Map loading incomplete %d vs %d\n", nLoad, nKey);
		exit(0);
	}
}


void WRRBMDictLoader(const string &path)
{
	FILE *fp = fopen(path.c_str(), "r");
	if (fp == NULL)
	{
		fprintf(stderr, "%s open failed\n", path.c_str());
		exit(0);
	}

	char buf[MAX_BUF_LEN];
	int lineNum = 0;
	wstring strGib = L"Gibberish";
	while (fgets(buf, MAX_BUF_LEN, fp) != NULL)
	{
		++ lineNum;
		char *pSpace = strchr(buf,' ');
		if (pSpace == NULL)
		{
			fprintf(stderr, "Loading %s line %d failed\n", path.c_str(), lineNum);
			exit(0);
		}

		*pSpace = 0;
		wstring key = utf82wstr(buf);
		if (key.size() == 0)
			CGlobalMap::s_GIDMap.Add(strGib.append(1, L'.'));
		else
			CGlobalMap::s_GIDMap.Add(key);
	}
	fclose(fp);
}

//--------------------------------------------------------
void CGlobalMap::Save(const string &path)
{
	FILE *fp = fopen(path.c_str(), "w");
	if (fp == NULL)
	{
		fprintf(stderr, "%s open failed\n", path.c_str());
		exit(0);
	}

	s_WIDMap.Save(fp);
	s_GIDMap.Save(fp);
	s_POSIDMap.Save(fp);
	fclose(fp);
}

void CGlobalMap::Load(const string &path)
{
	FILE *fp = fopen(path.c_str(), "r");
	if (fp == NULL)
	{
		fprintf(stderr, "%s open failed\n", path.c_str());
		exit(0);
	}

	s_WIDMap.Load(fp);
	s_GIDMap.Load(fp);
	s_POSIDMap.Load(fp);
	fclose(fp);
}
