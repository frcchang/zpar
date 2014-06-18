#pragma once
#include<list>
#include<cstdlib>
#include<cstdio>
using std::list;
class CPool
{
	struct SBlock
	{
		char *m_pData;
		size_t m_nBlkSize, m_nUsed;
	};
	const static size_t BLOCK_SIZE = 40960; 
public:
	CPool(void):pCurBlock(NULL), totalSize(0){}
	~CPool(void);
	void * Allocate(size_t size);
	void Recycle();
	void Free();
	void Test();
	void ShowSize()
	{
		fprintf(stderr, "Pool Size %lu k\n", totalSize >> 10);
	}
private:
	SBlock * allocateBlock(size_t size)
	{
		size_t allocSize = size > BLOCK_SIZE ? size : BLOCK_SIZE;
		SBlock * pBlock = (SBlock *)std::malloc(allocSize + sizeof(SBlock));
		if (pBlock == NULL)
		{
			fprintf(stderr, "malloc failed\n");
			exit(0);
		}
		pBlock->m_pData = (char*)((SBlock*)pBlock + 1);
		pBlock->m_nUsed = 0;
		pBlock->m_nBlkSize = allocSize;
		totalSize += allocSize;
		return pBlock;
	}
	list<SBlock *> m_listUsed, m_listFree;
	list<SBlock *>::iterator it, end;
	SBlock * pCurBlock;
	size_t   totalSize;
};



