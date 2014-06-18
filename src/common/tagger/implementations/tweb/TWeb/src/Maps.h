#pragma once


#include <string>
#include <vector>
#include <cassert>
#include <vector>
#include <list>
#include "util.h"
#include "Pool.h"

using std::list;
using std::wstring;
using std::vector;

class WStrHash
{
public:
	size_t operator()(const wchar_t * pwz) const 
	{
		const wchar_t *buf = pwz;
		unsigned int index = 1315423911;
		while(*buf)
			index^=((index<<5) + (index>>2) + *buf++);// 5.92

		return (index & 0x7FFFFFFF);
	}

	size_t operator()(const wstring & refStr) const
	{
		return operator()(refStr.c_str());
	}
};

class CHashInteger
{
public:
	size_t operator()(const int i)const
	{
		size_t index = 196613 * i + 98317;
		return index;
	}
};

class CStrHashMap
{
public:
	typedef std::pair<wstring, size_t>		ITEM_TYPE;
	typedef list<ITEM_TYPE>::iterator		iterator;
	typedef list<ITEM_TYPE>::const_iterator const_iterator;
private:
	list<ITEM_TYPE> m_lItemList;
	size_t m_nTableSize;
	size_t m_nItemNum;
	double m_occupyRate;
	struct SBucket
	{
		iterator beg, end;
	};
	WStrHash m_Hash;
	SBucket *m_pBuckets;
	
private:
	size_t nextPrime(size_t candidate)
	{
		candidate |= 1;
		while (((size_t)-1) != candidate)
		{
			size_t div = 3;
			size_t squ = div*div;
			while(squ < candidate && candidate % div)
			{
				squ += ++div * 4;
				++div;
			}

			if(candidate % div != 0)
				break;
			candidate += 2;
		}
	
		return candidate;
	}

	bool Grow()
	{
		/* 1. reallocate space */
		static size_t sizes[] ={	193, 389, 769, 1543, 3079, 6151, 12289, 24593,
									49157, 98317, 196613, 393241, 786433, 1572869, 3145739, 6291469,
									12582917, 25165843 };
		static size_t sizeLen = sizeof(sizes)/sizeof(size_t);
		size_t k = 1;
		for(k = 1; k < sizeLen && sizes[k] <= m_nTableSize; ++k);
	
		if(k != sizeLen)
			m_nTableSize = sizes[k];
		else
			m_nTableSize = nextPrime(m_nTableSize * 2);


		delete[] m_pBuckets;
		m_pBuckets = new SBucket[m_nTableSize];
		CHK_NEW_FAILED(m_pBuckets);
		iterator end = m_lItemList.end();
		for(size_t i = 0; i < m_nTableSize; ++i)
			m_pBuckets[i].beg = m_pBuckets[i].end = end;

		/* 2. rehash */
		iterator it = --end, beg = m_lItemList.begin() ;
		while(it != beg)
		{
			size_t index = m_Hash(it->first) % m_nTableSize;
			if(m_pBuckets[index].beg == m_pBuckets[index].end)
			{	
				/* 1. empty buckets */
				m_pBuckets[index].beg = m_pBuckets[index].end = it--;
				m_pBuckets[index].end ++;
			}
			else if(--m_pBuckets[index].beg != it)
			{	
				/* 2. here we need to rearrange the current element */
				m_lItemList.splice(m_pBuckets[index].end, m_lItemList, it--);
				++m_pBuckets[index].beg;
			}
			else 
				/* 3. only update the iterators */
				--it;
		}

		/* 3. handle the last item */
		size_t index = m_Hash(it->first) % m_nTableSize;
		if(m_pBuckets[index].beg == m_pBuckets[index].end)
		{	
			/* 1. empty buckets */
			m_pBuckets[index].beg = m_pBuckets[index].end = it;
			m_pBuckets[index].end ++;
		}
		else if(--m_pBuckets[index].beg != it)
		{	
			/* 2. here we need to rearrange the current element */
			m_lItemList.splice(m_pBuckets[index].end, m_lItemList, it);
			++m_pBuckets[index].beg;
		}

		return true;
	}

public:
	CStrHashMap()
	{
		m_nTableSize = 97, m_nItemNum = 0;
		m_occupyRate = 0.8;
		
		m_pBuckets = new SBucket[ m_nTableSize ];
		CHK_NEW_FAILED(m_pBuckets);
		iterator end = m_lItemList.end();
		for(size_t i = 0; i < m_nTableSize; ++i)
			m_pBuckets[i].beg = m_pBuckets[i].end = end;
	}
	~CStrHashMap()
	{
		delete[] m_pBuckets;
	}
	iterator begin()
	{
		return m_lItemList.begin();
	}

	iterator end()
	{
		return m_lItemList.end();
	}

	const_iterator constEnd()
	{
		return m_lItemList.end();
	}

	size_t size()
	{
		return m_nItemNum;
	}

	iterator Insert(const wstring & key, size_t val)
	{
		return Insert(key.c_str(), val);
	}

	iterator Insert(const wchar_t * pwz, size_t val)
	{
		size_t index = m_Hash(pwz) % m_nTableSize;
		SBucket *bucket = &m_pBuckets[index];
		iterator iter = bucket->beg;

		for (; iter != bucket->end; ++iter)
		{
			if (wcscmp(iter->first.c_str(), pwz) == 0)
				return iter;
		}

		if((double)(++m_nItemNum)/m_nTableSize > m_occupyRate)
		{
			Grow();
			index = m_Hash(pwz) % m_nTableSize;
			bucket = & m_pBuckets[index];	
		}

		if(bucket->beg == bucket->end)
		{
			bucket->end = bucket->beg = m_lItemList.insert(m_lItemList.begin(), ITEM_TYPE(pwz, val));
			return bucket->end++;
		}
		else
			return m_lItemList.insert(bucket->end, ITEM_TYPE(pwz, val));
	}

	iterator Find(const wstring & refStr)
	{
		return Find(refStr.c_str());
	}

	iterator Find(const wchar_t * pwz)
	{
		if(m_nItemNum == 0)
			return m_lItemList.end();

		size_t index = m_Hash(pwz) % m_nTableSize;
		SBucket &backet = m_pBuckets[index];
		iterator it = backet.beg;
		for(; it != backet.end ; ++it)
			if(wcscmp(it->first.c_str(), pwz) == 0)
				return it;

		return m_lItemList.end();
	}

	const_iterator ConstFind(const wchar_t * pwz)
	{
		if(m_nItemNum == 0)
			return m_lItemList.end();

		size_t index = m_Hash(pwz) % m_nTableSize;
		SBucket &backet = m_pBuckets[index];
		iterator it = backet.beg;
		for(; it != backet.end ; ++it)
			if(wcscmp(it->first.c_str(), pwz) == 0)
				return it;

		return m_lItemList.end();
	}
};

template <typename T_Key, typename T_Value, typename T_Hasher>
class CHashMap
{
public:
	typedef std::pair<T_Key, T_Value>  ITEM_TYPE;
	
public:
	typedef typename std::list<ITEM_TYPE>::iterator  iterator;

private:
	list<ITEM_TYPE> m_lItemList;
	size_t m_nTableSize;
	size_t m_nItemNum;
	double m_occupyRate;
	struct SBucket
	{
		iterator beg, end;
	};
	
	
	T_Hasher m_Hash;
	SBucket *m_pBuckets;
	
private:
	size_t nextPrime(size_t candidate)
	{
		candidate |= 1;
		while (((size_t)-1) != candidate)
		{
			size_t div = 3;
			size_t squ = div*div;
			while(squ < candidate && candidate % div)
			{
				squ += ++div * 4;
				++div;
			}

			if(candidate % div != 0)
				break;
			candidate += 2;
		}
	
		return candidate;
	}

	bool Grow()
	{
		/* 1. reallocate space */
		static size_t sizes[] ={193, 769,  12289,  196613,  1572869, 3145739, 6291469,
								12582917, 25165843 };
		static size_t sizeLen = sizeof(sizes)/sizeof(size_t);
		size_t k = 1;
		for(k = 1; k < sizeLen && sizes[k] <= m_nTableSize; ++k);
	
		if(k != sizeLen)
			m_nTableSize = sizes[k];
		else
			m_nTableSize = nextPrime(m_nTableSize * 2);


		delete[] m_pBuckets;
		m_pBuckets = new SBucket[m_nTableSize];
		assert(m_pBuckets);
		iterator end = m_lItemList.end();
		for(size_t i = 0; i < m_nTableSize; ++i)
			m_pBuckets[i].beg = m_pBuckets[i].end = end;

		/* 2. rehash */
		iterator it = --end, beg = m_lItemList.begin() ;
		while(it != beg)
		{
			size_t index = m_Hash(it->first) % m_nTableSize;
			if(m_pBuckets[index].beg == m_pBuckets[index].end)
			{	
				/* 1. empty buckets */
				m_pBuckets[index].beg = m_pBuckets[index].end = it--;
				m_pBuckets[index].end ++;
			}
			else if(--m_pBuckets[index].beg != it)
			{	
				/* 2. here we need to rearrange the current element */
				m_lItemList.splice(m_pBuckets[index].end, m_lItemList, it--);
				++m_pBuckets[index].beg;
			}
			else 
				/* 3. only update the iterators */
				--it;
		}

		/* 3. handle the last item */
		size_t index = m_Hash(it->first) % m_nTableSize;
		if(m_pBuckets[index].beg == m_pBuckets[index].end)
		{	
			/* 1. empty buckets */
			m_pBuckets[index].beg = m_pBuckets[index].end = it;
			m_pBuckets[index].end ++;
		}
		else if(--m_pBuckets[index].beg != it)
		{	
			/* 2. here we need to rearrange the current element */
			m_lItemList.splice(m_pBuckets[index].end, m_lItemList, it);
			++m_pBuckets[index].beg;
		}

		return true;
	}

public:
	CHashMap()
	{
		m_nTableSize = 97, m_nItemNum = 0;
		m_occupyRate = 0.8;
		
		m_pBuckets = new SBucket[ m_nTableSize ];
		assert(m_pBuckets);
		iterator end = m_lItemList.end();
		for(size_t i = 0; i < m_nTableSize; ++i)
			m_pBuckets[i].beg = m_pBuckets[i].end = end;
	}
	~CHashMap()
	{
		delete[] m_pBuckets;
	}
	iterator begin()
	{
		return m_lItemList.begin();
	}

	iterator end()
	{
		return m_lItemList.end();
	}

	size_t size()
	{
		return m_nItemNum;
	}

	iterator Insert(const T_Key & key, T_Value val)
	{
		size_t index = m_Hash(key) % m_nTableSize;
		SBucket *bucket = &m_pBuckets[index];
		iterator iter = bucket->beg;

		for (; iter != bucket->end; ++iter)
		{
			if (iter->first == key)
			{
				iter->second = val;
				return iter;
			}
		}

		if((double)(++m_nItemNum)/m_nTableSize > m_occupyRate)
		{
			Grow();
			index = m_Hash(key) % m_nTableSize;
			bucket = &m_pBuckets[index];	
		}

		if(bucket->beg == bucket->end)
		{
			bucket->end = bucket->beg = m_lItemList.insert(m_lItemList.begin(), ITEM_TYPE(key, val));
			return bucket->end++;
		}
		else
			return m_lItemList.insert(bucket->end, ITEM_TYPE(key, val));
	}

	
	iterator Find(const T_Key &key)
	{
		if(m_nItemNum == 0)
			return m_lItemList.end();

		size_t index = m_Hash(key) % m_nTableSize;
		SBucket &backet = m_pBuckets[index];
		iterator it = backet.beg;
		for(; it != backet.end ; ++it)
			if(it->first == key)
				return it;

		return m_lItemList.end();
	}
};

template <typename T_Key, typename T_Value, typename T_Hasher>
class CCompactHashMap
{
public:
	struct HashNode
  {
    T_Key   first;
    T_Value second;
    HashNode * m_pNext;

    HashNode(const T_Key &key, const T_Value &val):first(key), second(val), m_pNext(NULL){}
  };
//  typedef typename HashNode ITEM_TYPE;
  typedef HashNode ITEM_TYPE;
	
  struct SBucket
  {
    ITEM_TYPE *head;
    ITEM_TYPE *tail;

    bool empty(){return tail == (ITEM_TYPE *)-1;}
    SBucket (){head = 0; tail = (ITEM_TYPE *)-1;}
  };

  struct SIter
  {
    ITEM_TYPE *m_pItem;

    SIter(ITEM_TYPE *pItem):m_pItem(pItem){}
    SIter():m_pItem(NULL){}

    ITEM_TYPE & operator *()  {return *m_pItem;}
    ITEM_TYPE * operator ->()   {return m_pItem;}
    SIter & operator ++ () 
    {
      m_pItem = m_pItem->m_pNext; 
      return *this;
    }
    bool operator == (const SIter &r)const {return m_pItem == r.m_pItem;}
    bool operator != (const SIter &r)const {return !(*this == r);}
  };

  typedef SIter iterator;

private:
  ITEM_TYPE *pItemList;

  SBucket * m_pHashTable;
	T_Hasher  m_Hash;
  size_t    m_nTableSize;
  size_t    m_nItemNum;
	double    m_occupyRate;
  CPool     m_pool;

	
private:
	size_t nextPrime(size_t candidate)
	{
		candidate |= 1;
		while (((size_t)-1) != candidate)
		{
			size_t div = 3;
			size_t squ = div*div;
			while(squ < candidate && candidate % div)
			{
				squ += ++div * 4;
				++div;
			}

			if(candidate % div != 0)
				break;
			candidate += 2;
		}
	
		return candidate;
	}


	bool Grow()
	{
		/* 1. reallocate space */
		static size_t sizes[] ={193, 769,  12289,  196613,  1572869, 3145739, 6291469,
								12582917, 25165843 };
		static size_t sizeLen = sizeof(sizes)/sizeof(size_t);
		size_t k = 1;
		for(k = 1; k < sizeLen && sizes[k] <= m_nTableSize; ++k);
	
		if(k != sizeLen)
			m_nTableSize = sizes[k];
		else
			m_nTableSize = nextPrime(m_nTableSize * 2);


		delete[] m_pHashTable;
		m_pHashTable = new SBucket[m_nTableSize];
		assert(m_pHashTable);
	

		/* 2. rehash */
    ITEM_TYPE *pCurr = pItemList;
//    ITEM_TYPE *pNext = pCurr->m_pNext;
    
    ITEM_TYPE *pPre  = NULL;
    int nRehash = 0;
    while (pCurr != NULL)
    {
      ++nRehash;
      size_t index = m_Hash(pCurr->first) % m_nTableSize;
      if (m_pHashTable[index].empty() == true)
      {
        m_pHashTable[index].head = m_pHashTable[index].tail = pCurr;

        pPre = pCurr;
        pCurr = pCurr->m_pNext;
      }
      else if (m_pHashTable[index].tail->m_pNext == pCurr)
      {
        m_pHashTable[index].tail = pCurr;
        m_pHashTable[index].tail->m_pNext = pCurr->m_pNext;

        pPre = pCurr;
        pCurr = pCurr->m_pNext;
      }
      else 
      {
        pPre->m_pNext = pCurr->m_pNext;
        pCurr->m_pNext = m_pHashTable[index].tail->m_pNext;
        m_pHashTable[index].tail->m_pNext = pCurr;
        m_pHashTable[index].tail = pCurr;

        // pre does not change
        pCurr = pPre->m_pNext;
      }
    }
    
		return true;
	}

public:
	CCompactHashMap()
	{
		m_nTableSize = 97;
    m_nItemNum   = 0;
		m_occupyRate = 0.8;
		
    m_pHashTable = new SBucket[m_nTableSize];
    pItemList = NULL;
  }

	~CCompactHashMap()
	{
    delete m_pHashTable;
    m_pHashTable = NULL;
    
    ITEM_TYPE *pCurr = pItemList;
    while (pCurr != NULL)
    {
      pCurr->first.~T_Key();
      pCurr->second.~T_Value();

      pCurr = pCurr->m_pNext;
    }
	}

	iterator begin()
	{
    if (m_nItemNum != 0)
    {
      iterator beg;
      beg.m_pItem = pItemList;
      return beg;
    }
    else
      return end();
	}

	iterator end()
	{
    return iterator(NULL);
	}

	size_t size()
	{
		return m_nItemNum;
	}

	iterator Insert(const T_Key & key, T_Value val)
	{
		size_t index    = m_Hash(key) % m_nTableSize;
		SBucket *pBucket = m_pHashTable + index;
    if (pBucket->empty() == false)
    {
      ITEM_TYPE *pItem = pBucket->head;
      while (true)
      {
        if (pItem->first == key)
			  {
				  pItem->second = val;
				  return iterator(pItem);
			  }

        if (pItem == pBucket->tail)
          break;

        pItem = pItem->m_pNext;
      }
    }

		if ((double)(++m_nItemNum)/m_nTableSize > m_occupyRate)
		{
			Grow();
			index   = m_Hash(key) % m_nTableSize;
			pBucket = m_pHashTable + index;	
		}

    ITEM_TYPE * pNewNode = (ITEM_TYPE*)m_pool.Allocate(sizeof(*pNewNode));
    new(pNewNode)ITEM_TYPE(key, val);

    if (pBucket->empty() == true)
		{
      if (pItemList == NULL)
        pItemList = pNewNode;
      else
      {
        pNewNode->m_pNext = pItemList;
        pItemList         = pNewNode;
      }

			pBucket->tail = pBucket->head = pItemList;
			return iterator(pItemList);
		}
		else
    {
      pNewNode->m_pNext     = pBucket->tail->m_pNext;
      pBucket->tail->m_pNext  = pNewNode;
      pBucket->tail = pNewNode;

      return iterator(pNewNode);
    }
	}

	iterator Find(const T_Key &key)
	{
		if(m_nItemNum == 0)
			return end();

		size_t index = m_Hash(key) % m_nTableSize;
    SBucket *pBucket = m_pHashTable + index;
    if (pBucket->empty() == false)
    {
      ITEM_TYPE *pItem = pBucket->head;
      while (true)
      {
        if (pItem->first == key)
          return iterator(pItem);

        if (pItem == pBucket->tail)
          break;

        pItem = pItem->m_pNext;
      }
    }

		return end();
	}
};


template <typename KEY,  typename HASHER>
class CNumberer
{
private:
	CHashMap<KEY, int, HASHER> m_map;
	vector<KEY *> m_keyVec;


public:
	typename CHashMap<KEY,int, HASHER>::iterator Insert(const KEY &key)
	{
		int id = (int)m_map.size();
		typename CHashMap<KEY,int, HASHER>::iterator iter = m_map.Find(key);
			
		if (iter != m_map.end())
			return iter;
		else
		{
			iter = m_map.Insert(key, id);
			m_keyVec.push_back(&(iter->first));
			return iter;
		}
	}

	int GetId(const KEY &key, bool insertIfNotExist)
	{
		typename CHashMap<KEY, int, HASHER>::iterator iter = m_map.Find(key);
		if (iter == m_map.end())
		{
			if (insertIfNotExist == false)
				return -1;
			else
				iter = Insert(key);
		}
		return iter->second;
	}

	KEY  *GetKey(int id)
	{
		assert(id >=0 && id < (int)m_map.size());
		return m_keyVec[id];
	}

	int size()
	{
		return (int)m_map.size();
	}
};


