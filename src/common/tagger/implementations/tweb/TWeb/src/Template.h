#ifndef __TEMPLATE_H__
#define __TEMPLATE_H__

#include "Maps.h"
#include "util.h"

using std::map;
struct SKIDs//SKIDs
{
	int *vals;
	int len;

	SKIDs()
	{
		vals = NULL;
		len = 0;
	}
	SKIDs(int *pVal, int l)
	{
		len = l;
		vals = new int[l];
		assert(vals);
		memcpy(vals, pVal, sizeof(int) * l);
	}

	SKIDs(const SKIDs & r)
	{
		len = r.len;
		vals = new int[len];
		assert(vals);
		memcpy(vals, r.vals, sizeof(int) * len);
	}

	~SKIDs()
	{
		if (vals != NULL)
			delete [] vals;
		vals = NULL;
		len = 0;
	}

	bool operator == (const SKIDs &r)const
	{
		for (int i = 0; i < r.len; ++i)
			if (vals[i] != r.vals[i])
				return false;

		return true;
	}
};
	
class IdListHasher
{
public:
	size_t operator()(const SKIDs & pval) const 
	{
		int *buf = pval.vals;
		unsigned int index = 1315423911;
		int i = 0; 
		while(i++ < pval.len)
			index^=((index<<5) + (index>>2) + *buf++);

		return (index & 0x7FFFFFFF);
	}
};

class CTemplateManager
{
	enum F_TYPE {_STATIC, _DYNAMIC};
public:
	struct SOfts//Pend_Kernel
	{
		int   wOft;
		int   kIdx;
		F_TYPE	kType;			// static or dynamic kernel

		SOfts(int w, int k, /*bool wIndex,*/ F_TYPE kernelType)
		{
			wOft = w;
			kIdx = k;
			kType	= kernelType;
		}

		SOfts()
		{
			wOft	= -1;
			kType	= _STATIC;
		}
	};

	typedef int PENDING_INDEX_TYPE;
	typedef vector<SOfts>::iterator PEND_KERNEL_ITER;
	
	/* unigram, bigram and pp */
	struct STemplate
	{
		/* it is somehow redundancy */
		vector<SOfts> m_vOfts;
		CHashMap<SKIDs, int, IdListHasher> m_kernelVal2GlobalID;
		wstring name;

		int m_limitLen;
		int m_limitOft;
		/* for length limit new of pos tag */
		F_TYPE	m_type;

		bool  IsDynamicTemplate()					{return m_type == _DYNAMIC;}

		/* member functions */
		int GetGlobalId(int *kernelIds, int nKernels)
		{
			SKIDs temp;
			temp.vals = kernelIds;
			temp.len = nKernels;

			CHashMap<SKIDs, int, IdListHasher>::iterator findIter = m_kernelVal2GlobalID.Find(temp);
			temp.vals = NULL;
			if (findIter != m_kernelVal2GlobalID.end())
				return findIter->second;
			else
				return -1;
		}

		bool operator == (STemplate &r)
		{
			if (name != r.name || m_type != r.m_type)
				return false;
		
			if (m_vOfts.size() != r.m_vOfts.size())
				return false;

			
			if (m_kernelVal2GlobalID.size() != r.m_kernelVal2GlobalID.size())
				return false;

			CHashMap<SKIDs, int, IdListHasher>::iterator iter = m_kernelVal2GlobalID.begin(), rIter;
			for (; iter != m_kernelVal2GlobalID.end(); ++iter)
			{
				rIter = r.m_kernelVal2GlobalID.Find(iter->first);
				if (rIter == r.m_kernelVal2GlobalID.end())
					return false;

				if (iter->second != rIter->second)
					return false;
			}


			return true;
		}

		void Insert(int *kernelIds, int nKernels, int globalId)
		{
			SKIDs temp;
			temp.vals = kernelIds;
			temp.len = nKernels;

			m_kernelVal2GlobalID.Insert(temp, globalId);
			temp.vals = NULL;
		}

		void Display(const map<wstring, int> &refKernelMap, const vector<wstring> &kernelVec)
		{
			fprintf(stderr, "%s, type %s :", wstr2utf8(name).c_str(), 
							m_type == _DYNAMIC ? "DYNAMIC":"STATIC");
			for (size_t i = 0; i < m_vOfts.size(); ++i)
			{
				const char *p;
				if (m_vOfts[i].wOft <= 0)
					p = "l";
				else
					p = "r";

				int kIdx = m_vOfts[i].kIdx;
				fprintf(stderr, "<%s%d %s> ", p, m_vOfts[i].wOft, wstr2utf8(kernelVec[kIdx]).c_str());
			}
		}

		STemplate(wchar_t * pTempLine, 
							const map<wstring, int> &refKernelMap, 
							int nDyKernels): name(pTempLine)
		{
//			fprintf(stderr, "Current template: %s\n", wstr2utf8(pTempLine).c_str());
			m_type = _STATIC;		// 21/11/2012
			wchar_t *pLenLimit = wcschr(pTempLine, ' ');
			if (pLenLimit == NULL)
			{
				fprintf(stderr, "Error: template error fromat %s\n", 
								wstr2utf8(pTempLine).c_str());
				exit(0);
			}
			*pLenLimit ++ = 0;
			if (swscanf(pLenLimit, L"%d%d", &m_limitOft, &m_limitLen) != 2)
			{
				fprintf(stderr, "Error: template error fromat %s\n", 
								wstr2utf8(pTempLine).c_str());
				exit(0);
			}

      vector<wchar_t *> kVec = Split(pTempLine, L"@");
      for (size_t kidx = 0; kidx < kVec.size(); ++kidx)
			{
        wchar_t *p = kVec[kidx];
				wchar_t *pKernel = wcschr(p, '_');
				if (pKernel == NULL)
				{
					fprintf(stderr, "Error: Invalid template %s, length %d\n", 
							wstr2utf8(pKernel).c_str(), (int)wcslen(pKernel));
					exit(0);
				}
				

				map<wstring, int>::const_iterator iter = refKernelMap.find(++pKernel);
				if (iter == refKernelMap.end())
				{
					fprintf(stderr, "Error: Invalid template %s, length %d\n", 
							wstr2utf8(pKernel).c_str(), (int)wcslen(pKernel));
					exit(0);
				}

				int kernelId = iter->second;
				p += 2;
				int wOft = p[-1] == 'r' ? *p - '0' : '0' - *p;
//				fprintf(stderr, "%s woft %d\n", wstr2utf8(p - 1).c_str(), wOft);
				//	check kernel type	11/21/2012
				F_TYPE fType = iter->second < nDyKernels ? _DYNAMIC: _STATIC;
				if (fType == _DYNAMIC)		//	check kernel type	11/21/2012
					m_type = _DYNAMIC;		//	check kernel type	11/21/2012

				m_vOfts.push_back(SOfts(wOft, kernelId, /*isWordIndex,*/ fType));
			}
		}
	};

public:
	~CTemplateManager()
	{
		for (size_t i = 0; i < m_tempVector.size(); ++i)
		{
			delete m_tempVector[i];
			m_tempVector[i] = NULL;
		}
	}

	CTemplateManager()
	{
		m_nDyKernels	= 0;
		m_nDyTemplates	= 0;
		m_leftOffset	= 0;
		m_rightOffset	= 0;
	}

//	void Compacting(Perceptron::CSparseScorer &rScorer);

	void GetTemplates(vector<STemplate *> **refTemplateVec)
	{
		*refTemplateVec = &m_tempVector;
	}
	void GetWindowSize(int &refLeftOffset, int &refRightOffset)
	{
		refLeftOffset = m_leftOffset;
		refRightOffset = m_rightOffset;
	}

	const map<wstring, int> & GetKernelMap()		{return m_kernelMap;}
	int GetTemplateNum()												{return (int)m_tempVector.size();}
	const vector<wstring>& GetKernelVec()				{return m_kernelVec;}	
	int GetKernelNum()													{return (int)m_kernelMap.size();}
	int GetDKernelNum()													{return m_nDyKernels;}
	bool ReadingTemplate(const string &strPath);
	bool Save(const string & strPath);
	bool Save(FILE *fp);
	bool Load(FILE *fp);
	void Display();

	bool operator == (CTemplateManager &r)
	{
		if (m_kernelMap.size() != r.m_kernelMap.size())
			return false;

		for (int i = 0; i < (int)m_kernelVec.size(); ++i)
			if (m_kernelVec[i] != r.m_kernelVec[i])
				return false;

		if (m_tempVector.size() != r.m_tempVector.size())
			return false;

		if (m_leftOffset != r.m_leftOffset || 
			m_rightOffset != r.m_rightOffset)
			return false;

		for (size_t i = 0; i < m_tempVector.size(); ++i)
		{
			STemplate &lTemp = *(m_tempVector[i]), &rTemp = *(r.m_tempVector[i]);
			if ((lTemp == rTemp) == false)
				return false;
		}

		return true;
	}


private:
	vector<STemplate *>		m_tempVector;		 
	vector<wstring>			  m_kernelVec;		// first k kernels are dynamic templates
	map<wstring, int>		  m_kernelMap;		// record each kernel and slot pair
	
	int m_nDyTemplates;	  // number of dynamic templates  21/11/2012
	int m_nDyKernels;			// number of dynamic kernels	21/11/2012

	int m_leftOffset;			// left/right window size
	int m_rightOffset;
};



#endif  /*__TEMPLATE_H__*/
