#ifndef _GLOBAL_MAP_H__
#define _GLOBAL_MAP_H__
#include <vector>
#include <string>
#include <cassert>
#include <unordered_map>
#include "util.h"
using std::string;
using std::wstring;
using std::vector;
using std::unordered_map;

class CStrIDMap
{
public:
	CStrIDMap(int unkID)								
	{
		assert(unkID <= 0);
		m_UnkStr = L"<unk>";
		m_UnkID  = unkID;
		if (unkID == 0)
			Add(m_UnkStr);
	}
	void Add(const wstring &wstr)
	{
		if (m_Str2IDMap.find(wstr) == m_Str2IDMap.end())
		{
			m_Str2IDMap[wstr] = m_StrVec.size();
			m_StrVec.push_back(wstr);
		}
	}

	int ID(const wstring &wstr)
	{
		unordered_map<wstring, int>::iterator iter = m_Str2IDMap.find(wstr);
		return iter == m_Str2IDMap.end() ? m_UnkID:iter->second;
	}


	void Save(FILE *fp);
	void Load(FILE *fp);

	void Save(const string & strPath)
	{
		FILE *fp = fopen(strPath.c_str(), "w");
		assert(fp);
		for (size_t i = 0; i < m_StrVec.size(); ++i)
			fwprintf(fp, L"%ls\n", m_StrVec[i].c_str());
		fclose(fp);
	}

	void Load(const string & strPath)
	{
		FILE *fp = fopen(strPath.c_str(), "r");
		assert(fp);
		const int MAX_POS_LEN = 1<<8;
		wchar_t buf[MAX_POS_LEN];
		while (fgetws(buf, MAX_POS_LEN, fp) != NULL)
		{
			removeNewLine(buf);
			if (m_Str2IDMap.find(buf) != m_Str2IDMap.end())
			{
				fprintf(stderr, "Error: duplicate item\n");
				exit(0);
			}
			Add(buf);	
		}
		fprintf(stderr, "Total %d items\n", (int)m_StrVec.size());
		fclose(fp);
	}
	
	int Size()											{return m_StrVec.size();}
	int UnkID()											{return m_UnkID;}
	vector<wstring> & GetStrVec()		{return m_StrVec;}
	const wstring & Str(int ID)			
	{
		if (ID == m_UnkID)
			return m_UnkStr;
		else if (ID < (int)m_StrVec.size()) 
			return m_StrVec[ID];
		else
		{
			fprintf(stderr, "ID %d Out of bound\n", ID);
			exit(0);
		}
	}
	
private:
	unordered_map<wstring, int> 		m_Str2IDMap;
	vector<wstring> 								m_StrVec;
	int															m_UnkID;
	wstring													m_UnkStr;
};

class CGlobalMap
{
public:
	static CStrIDMap s_WIDMap;
	static CStrIDMap s_GIDMap;
	static CStrIDMap s_POSIDMap;

	static void Save(const string &path);
	static void Load(const string &path);
};

void WRRBMDictLoader(const string &path);
#endif
