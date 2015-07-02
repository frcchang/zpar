#ifndef __GUARD__UTIL
#define __GUARD__UTIL
#include <cstring>
#include <algorithm>
#include <functional>
#include <vector>
#include <string>
#include <locale>
#include <map>
#ifndef WIN32
#include <cstdlib>
#else
#include <Windows.h>
#endif

#include "Pool.h"
using std::string;
using std::wstring;
using std::vector;

template<class T>
inline void CHK_NEW_FAILED(T *p)
{
	if (p == NULL)
	{
		fprintf(stderr, "New failed\n");
		exit(0);
	}
}

inline void CHK_OPEN_FAILED(FILE *fp, const wchar_t *path)
{
	if (fp == NULL)
	{
		fwprintf(stderr, L"Error: failed to open %s\n", path);
		exit(0);
	}
}


inline void CHK_OPEN_FAILED(FILE *fp, const char *path)
{
	if (fp == NULL)
	{
		fprintf(stderr, "Error: failed to open %s\n", path);
		exit(0);
	}
}

inline void removeNewLine(wchar_t * buf)
{
	wchar_t *p = buf;
	while (*p && *p != '\r' && *p != '\n')
		++p;
	*p = 0;
}

inline void removeNewLine(char * buf)
{
	char *p = buf;
	while (*p && *p != '\r' && *p != '\n')
		++p;
	*p = 0;
}



struct mapWSTRCmp
{
	bool operator ()(std::pair<std::wstring, int> &l, std::pair<std::wstring, int> &r)
	{
		return l.second > r.second;
	}
};

template<typename t1, typename t2, typename t3>
void mapSort(std::map<t1, t2> &rmap, vector<std::pair<t1, t2> > &vec)
{
	typename std::map<t1, t2>::iterator iter = rmap.begin(), end = rmap.end();

	for (; iter != end; ++iter)
		vec.push_back(std::pair<t1,t2>(iter->first, iter->second));
	
	std::sort(vec.begin(), vec.end(), t3());
}




#if defined(_MSC_VER) || defined(__BORLANDC__)
/* In linux, we can use finite directly,
 * In windows, we have to use _finite instead
 */
inline int finite(double x) 	{return _finite(x);}
inline int nan(double x)			{return _isnan(x);}
#endif


#ifndef WIN32
inline char * itoa(int val, char *pBuf, int base) // the base argument does not effect the result
{
  sprintf(pBuf, "%d", val);
  return pBuf;
}
#endif

char	*copyStr(const char *pszStr,	CPool *pPool = NULL);
wchar_t *copyWcs(const wchar_t *pwzStr, CPool *pPool = NULL);
enum MALT_GEN_INDEX {_GWORD, _GGenFORM, _GTAG};//, _GHEAD_INDEX, _GLABEL};


inline string GetNameFromDir(const string &dir)
{
	return dir.substr(dir.rfind("/") + 1);
}


inline void initLocale()
{
#ifndef WIN32
	setlocale(LC_ALL, "zh_CN.UTF-8" );
#else
	setlocale(LC_ALL, ".936" );
#endif
}

vector<char *> Split(char *pBuf, const char *pDelem);
vector<wchar_t *> Split(wchar_t *pBuf, const wchar_t *pDelem);


vector<int> Shuffel(int nElems);

class CCodeConversion
{
public:

	static bool UTF8ToUnicode(const char *line,  wchar_t *pwBuf,  size_t BufLen)
	{		
		if (BufLen <= 0)
			return false;
#ifndef WIN32
		setlocale(LC_ALL, "zh_CN.UTF-8");
		size_t size = mbstowcs(NULL, line, 0);
		if (BufLen < size + 1)
			return false;
		
		mbstowcs(pwBuf, line, size+1);
#else
		size_t size = MultiByteToWideChar(CP_UTF8, 0, line, -1, NULL, 0);
		if (size > BufLen)
			return false;

		MultiByteToWideChar(CP_UTF8, 0, line, -1, pwBuf, size);
#endif
		return true;
	}

	static wstring UTF8ToUnicode(const char* line)
	{
#ifndef WIN32
		setlocale(LC_ALL, "zh_CN.UTF-8");
		size_t size = mbstowcs(NULL, line, 0);
		wchar_t* wcstr = new wchar_t[size + 1];
		if (!wcstr)
			return L"";
		mbstowcs(wcstr, line, size+1);
#else
		size_t size = MultiByteToWideChar(CP_UTF8, 0, line, -1, NULL, 0);
		wchar_t* wcstr = new wchar_t[size];
		if (!wcstr)
			return L"";
		MultiByteToWideChar(CP_UTF8, 0, line, -1, wcstr, size);	
#endif
		wstring final(wcstr);
		delete[] wcstr;

		return final;
	}

	static wstring UTF8ToUnicode(const string& line)
	{
#ifndef WIN32
		setlocale(LC_ALL, "zh_CN.UTF-8");
		size_t size = mbstowcs(NULL, line.c_str(), 0);
		wchar_t* wcstr = new wchar_t[size + 1];
		if (!wcstr)
			return L"";
		mbstowcs(wcstr, line.c_str(), size+1);
#else
		size_t size = MultiByteToWideChar(CP_UTF8, 0, line.c_str(), -1, NULL, 0);
		wchar_t* wcstr = new wchar_t[size];
		if (!wcstr)
			return L"";
		MultiByteToWideChar(CP_UTF8, 0, line.c_str(), -1, wcstr, size);	
#endif
		wstring final(wcstr);
		delete[] wcstr;

		return final;
	}

	static string UnicodeToUTF8(const wstring& line)
	{
#ifndef WIN32
		setlocale(LC_ALL, "zh_CN.UTF-8");
		size_t size = wcstombs(NULL, line.c_str(), 0);
		char* mbstr = new char[size + 1];
		if (!mbstr)
			return "";
		wcstombs(mbstr, line.c_str(), size+1);
#else
		size_t size = WideCharToMultiByte(CP_UTF8, 0, line.c_str(), -1, NULL, 0, NULL, NULL);
		char* mbstr = new char[size];
		if (!mbstr)
			return "";
		WideCharToMultiByte(CP_UTF8, 0, line.c_str(), -1, mbstr, size, NULL, NULL);
#endif
		string final(mbstr);
		delete[] mbstr;

		return final;
	}

	static wstring GBToUnicode(const char* line)
	{
#ifndef WIN32
		setlocale(LC_ALL, "zh_CN.GB2312");
		size_t size = mbstowcs(NULL, line, 0);
		wchar_t* wcstr = new wchar_t[size + 1];
		if (!wcstr)
			return L"";
		mbstowcs(wcstr, line, size+1);
#else
		size_t size = MultiByteToWideChar(CP_ACP, 0, line, -1, NULL, 0);
		wchar_t* wcstr = new wchar_t[size];
		if (!wcstr)
			return L"";
		MultiByteToWideChar(CP_ACP, 0, line, -1, wcstr, size);	
#endif
		wstring final(wcstr);
		delete[] wcstr;

		return final;
	}

	static wstring GBToUnicode(const string& line)
	{
#ifndef WIN32
		setlocale(LC_ALL, "zh_CN.GB2312");
		size_t size = mbstowcs(NULL, line.c_str(), 0);
		wchar_t* wcstr = new wchar_t[size + 1];
		if (!wcstr)
			return L"";
		mbstowcs(wcstr, line.c_str(), size+1);
#else
		size_t size = MultiByteToWideChar(CP_ACP, 0, line.c_str(), -1, NULL, 0);
		wchar_t* wcstr = new wchar_t[size];
		if (!wcstr)
			return L"";
		MultiByteToWideChar(CP_ACP, 0, line.c_str(), -1, wcstr, size);	
#endif
		wstring final(wcstr);
		delete[] wcstr;

		return final;
	}

	static string UnicodeToGB(const wstring& line)
	{
#ifndef WIN32
		setlocale(LC_ALL, "zh_CN.GB2312");
		size_t size = wcstombs(NULL, line.c_str(), 0);
		char* mbstr = new char[size + 1];
		if (!mbstr)
			return "";
		wcstombs(mbstr, line.c_str(), size+1);
#else
		size_t size = WideCharToMultiByte(CP_ACP, 0, line.c_str(), -1, NULL, 0, NULL, NULL);
		char* mbstr = new char[size];
		if (!mbstr)
			return "";
		WideCharToMultiByte(CP_ACP, 0, line.c_str(), -1, mbstr, size, NULL, NULL);
#endif
		string final(mbstr);
		delete[] mbstr;

		return final;
	}
};

string inline wstr2utf8(const wstring & wstr)
{
	return CCodeConversion::UnicodeToUTF8(wstr);
}

wstring inline utf82wstr(const string & str)
{
	return CCodeConversion::UTF8ToUnicode(str);
}

template<typename t1, typename t2>
struct PairGreaterSecond
{
	bool operator()(const std::pair<t1, t2> &l, const std::pair<t1, t2> &r)
	{
		return l.second > r.second;
	}
};

template<typename t1, typename t2>
struct PairGreaterFirst
{
	bool operator()(const std::pair<t1, t2> &l, const std::pair<t1, t2> &r)
	{
		return l.first> r.first;
	}
};



bool Tokenize(wchar_t * pwzLine, 
							vector <wchar_t *> & rResVec, 
							bool ConllFormat = false);

struct SEvalRecorder
{
	int nTotal;
	int nCorrect;
	int nOOV;
	int nOOVCorr;
	double accuracy;

	SEvalRecorder()
	{
		nTotal = 0;
		nCorrect = 0;
		accuracy = 0;
		nOOV = 0;
		nOOVCorr = 0;
	}
};
#endif
