#ifndef __LEXICON_H__
#define __LEXICON_H__

#include "Maps.h"
#include "GlobalMap.h"
#include "Sentence.h"
class CWordTagManager
{
private:
	int m_cutoff;
public:
	CWordTagManager(int cutoff = 6)
	{
		m_cutoff = cutoff;
	}
	enum WORD_CH {WORD, B_CH, E_CH, INNER_CH};
	bool SeenWordTag(const wstring &word, int POSId, WORD_CH wc)
	{
		CHashMap<wstring, bool *, WStrHash> &rMap = wc == WORD ? m_wTagFlagsMap:
													wc == B_CH ? m_bchTagFlagsMap:
													wc == E_CH ? m_echTagFlagsMap:m_ichTagFlagsMap;

		CHashMap<wstring, bool *, WStrHash>::iterator iter;
		iter = rMap.Find(word);

		if (iter == rMap.end() || (iter->second)[POSId] == false)
			return false;

		return true;
	}

	int GetWordCount(const wstring &word)
	{
		CHashMap<wstring, int, WStrHash>::iterator iter = m_wCounter.Find(word);

		if (iter == m_wCounter.end())
			return 0;

		return iter->second;
	}

	bool FreqWord(const wstring &word)
	{
		CHashMap<wstring, int, WStrHash>::iterator iter = m_wCounter.Find(word);

		if (iter == m_wCounter.end())
			return false;
		if (iter->second <= m_cutoff)
			return false;
		
		return true;
	}

	bool SeenWord(const wstring &word)
	{
		return m_wCounter.Find(word) != m_wCounter.end();
		//return m_wTagsMap.Find(word) != m_wTagsMap.end();
	}

	bool SeenWordTag(const wstring &word, const wstring &tag, WORD_CH wc)
	{
		CStrIDMap & posIDMap = CGlobalMap::s_POSIDMap;
		int posId = posIDMap.ID(tag.c_str());
		assert(posId >=0 && posId < (int)posIDMap.Size());
		return SeenWordTag(word, posId, wc);
	}

	

	bool GetTags(const wstring &word, const wchar_t **pTags, WORD_CH wc)
	{
		CHashMap<wstring, wstring, WStrHash> &rMap = wc == WORD ? m_wTagsMap:
													 wc == B_CH ? m_bchTagsMap:
													 wc == E_CH ? m_echTagsMap: m_ichTagsMap;
		CHashMap<wstring, wstring, WStrHash>::iterator iter = rMap.Find(word);
		if (iter == rMap.end())
			return false;
		else
		{
			*pTags = iter->second.c_str();
			return true;
		}
	}

	void Display(WORD_CH wc, FILE *fp)
	{
		CHashMap<wstring, wstring, WStrHash>::iterator iter, end;
		CHashMap<wstring, wstring, WStrHash> &rMap = wc == WORD ? m_wTagsMap:
													 wc == B_CH ? m_bchTagsMap:
													 wc == E_CH ? m_echTagsMap: m_ichTagsMap;
		fwprintf(fp, L"%s:\n",	wc == WORD ? L"wordTags":
								wc == B_CH ? L"beginTags":
								wc == E_CH ? L"endTags":L"innerTags");

		for (iter = rMap.begin(), end = rMap.end(); iter != end; ++iter)
		{
			if (wc == WORD)
				fwprintf(fp, L"<%s %d>: %s\n", iter->first.c_str(),   m_wCounter.Find(iter->first)->second
												 , iter->second.c_str());
			else
				fwprintf(fp, L"%s: %s\n", iter->first.c_str(), iter->second.c_str());
		}
	}

	void Display(const char *pszWordTags = NULL)
	{
		FILE *fp = pszWordTags != NULL ? fopen(pszWordTags, "w"):stderr;

		CHK_OPEN_FAILED(fp, pszWordTags);
		Display(WORD, fp);
		Display(B_CH, fp);
		Display(E_CH, fp);
		Display(INNER_CH, fp);


		if (pszWordTags != NULL)
			fclose(fp);
	}

	void CollectTagSet(WORD_CH wc)
	{

		CHashMap<wstring, wstring, WStrHash> &rMap = wc == WORD ? m_wTagsMap:
													 wc == B_CH ? m_bchTagsMap:
													 wc == E_CH ? m_echTagsMap: m_ichTagsMap;

		CHashMap<wstring, bool *, WStrHash> &rFlagsMap = wc == WORD ? m_wTagFlagsMap:
														 wc == B_CH ? m_bchTagFlagsMap:
													     wc == E_CH ? m_echTagFlagsMap:m_ichTagFlagsMap;
		
		CHashMap<wstring, bool *, WStrHash>::iterator iter = rFlagsMap.begin(), end = rFlagsMap.end();
		CStrIDMap & posIDMap = CGlobalMap::s_POSIDMap;
		vector<wstring> &rPosVec = posIDMap.GetStrVec();
		for (; iter != end; ++iter)
		{
			wstring &rWord = iter->first;
			bool *pFlags = iter->second;
			wstring tags(L"");
			for (size_t i = 0; i < rPosVec.size(); ++i)
			{
				if (pFlags[i] == false)
					continue;
				
				tags.append(rPosVec[i].c_str());
				tags.append(L"_");
			}
			rMap.Insert(rWord, tags);
		}
	}



	//void InitWordTagManager(vector<SENTENCE_TYPE *> &sens)
	void InitWordTagManager(vector<SSentence *> &sens)
	{
		assert(sens.size() != 0);
		wchar_t chBuf[2] = {0};

		/* count word frequences */
		CHashMap<wstring, int, WStrHash>::iterator iter;
		for (size_t i = 0; i < sens.size(); ++i)
		{
			SSentence &rSen = *sens[i];

//			for (size_t ind = 0; ind < rSen.size(); ++ind)
			for (int ind = 0; ind < rSen.Length(); ++ind)
			{
				const wchar_t *pWord	= rSen.Word(ind);
				iter = m_wCounter.Find(pWord);
				if (iter == m_wCounter.end())
					m_wCounter.Insert(pWord, 1);
				else
					iter->second += 1;
			}
		}


		for (size_t i = 0; i < sens.size(); ++i)
		{
			//SENTENCE_TYPE &rSen = *sens[i];
			SSentence &rSen = *sens[i];
			for (int ind = 0; ind < rSen.Length(); ++ind)
			{
				const wchar_t *pWord	= rSen.Word(ind);
				const wchar_t *pTag	= rSen.Tag(ind);
//				iter = m_tagIDMap.Find(pTag);
//				if (iter == m_tagIDMap.end())
//					m_tagIDMap.InsertWordTag(pTag, m_tagIDMap.size());

				iter = m_wCounter.Find(pWord);
				// word tags are only used as constraints 
				if (iter->second > m_cutoff)
					InsertWordTag(pWord, pTag, WORD);
				
				// inner char to meet the length constraints 
//				if (word.size() > 1)
				int wLen = wcslen(pWord);
				if (wLen > 1)
				{
					// begin 
					chBuf[0] = pWord[0];
					InsertWordTag(chBuf, pTag, B_CH);
					// end 
					chBuf[0] = pWord[wLen - 1];
					InsertWordTag(chBuf, pTag, E_CH);

					// inner_characters 
					for (int cind = 1;  cind < wLen-1;  ++ cind)
					{
						chBuf[0] = pWord[cind];
						InsertWordTag(chBuf, pTag, INNER_CH);
					}
				}
			}
		}

		CollectTagSet(WORD);
		CollectTagSet(B_CH);
		CollectTagSet(E_CH);
		CollectTagSet(INNER_CH);
	}

	void InsertWordTag(const wchar_t *pwzWord, const wchar_t *pwzTag, WORD_CH wc)
	{
		CStrIDMap & posIDMap = CGlobalMap::s_POSIDMap;
		int tagId = posIDMap.ID(pwzTag);
		assert(tagId != -1);

		CHashMap<wstring, bool *, WStrHash> &rMap = wc == WORD ? m_wTagFlagsMap:
													wc == B_CH ? m_bchTagFlagsMap:
													wc == E_CH ? m_echTagFlagsMap:m_ichTagFlagsMap;
		CHashMap<wstring, bool *, WStrHash>::iterator iter;

		iter = rMap.Find(pwzWord);

		if (iter != rMap.end())
			(iter->second)[tagId] = true;
		else
		{
			int posNum = posIDMap.Size();
			bool *pFlags = (bool*)m_pool.Allocate(sizeof(bool) * posNum);
			memset(pFlags, 0, sizeof(bool) * posNum);

			pFlags[tagId] = true;
			rMap.Insert(pwzWord, pFlags);
		}
	}

	


#if 0
	unsigned int CountLength(CHashMap<wstring, wstring, WStrHash> &xTagMap)
	{
		unsigned int length = 0;
		CHashMap<wstring, wstring, WStrHash>::iterator iter = xTagMap.begin();
		for (; iter != xTagMap.end(); ++iter)
		{
			string key = CCodeConversion::UnicodeToUTF8(iter->first);
			string val = CCodeConversion::UnicodeToUTF8(iter->second);

			length += key.size() + val.size() + 2;
		}
		return length;
	}
#endif 

	bool SaveXTagMap(FILE *fp, CHashMap<wstring, wstring, WStrHash> &xTagMap)
	{
		CHashMap<wstring, wstring, WStrHash>::iterator iter = xTagMap.begin();
		for (; iter != xTagMap.end(); ++iter)
		{
			string key = CCodeConversion::UnicodeToUTF8(iter->first);
			string val = CCodeConversion::UnicodeToUTF8(iter->second);

			fwrite(key.c_str(),  sizeof(char),  key.size() + 1,  fp);
			fwrite(val.c_str(),  sizeof(char),  val.size() + 1,  fp);
		}
		return true;
	}

	struct Head
	{
		size_t m_nWTItem;
		size_t m_nBTItem;
		size_t m_nETItem;
		size_t m_nITItem;
		size_t m_nWords;
		size_t m_cutoff;
	};

	bool Save(const string &strPath)
	{
		FILE *fp = fopen(strPath.c_str(), "wb");
		CHK_OPEN_FAILED(fp, strPath.c_str());
		
		Head header;
		header.m_nWTItem = m_wTagsMap.size();
		header.m_nBTItem = m_bchTagsMap.size();
		header.m_nETItem = m_echTagsMap.size();
		header.m_nITItem = m_ichTagsMap.size();
		header.m_nWords  = m_wCounter.size();
		header.m_cutoff	 = m_cutoff;

		fwrite(&header, sizeof(header), 1, fp);
		SaveXTagMap(fp, m_wTagsMap);
		SaveXTagMap(fp, m_bchTagsMap);
		SaveXTagMap(fp, m_echTagsMap);
		SaveXTagMap(fp, m_ichTagsMap);


		// save word counter
		if (m_wCounter.size() > 0)
		{
			CHashMap<wstring, int, WStrHash>::iterator iter = m_wCounter.begin();
			vector<int> counts(m_wCounter.size());
		
			// save keys
			for (int i = 0; iter != m_wCounter.end(); ++iter, ++i)
			{
				string key = CCodeConversion::UnicodeToUTF8(iter->first);
				counts[i] = iter->second;
				fwrite(key.c_str(),  sizeof(char),  key.size() + 1,  fp);
			}
		
			// save counts
			fwrite(&counts[0], sizeof(int), counts.size(), fp);
		}


		fclose(fp);
		return true;
	}


	bool Load(const string & strPath)
	{
		FILE *fp = fopen(strPath.c_str(), "rb");
		CHK_OPEN_FAILED(fp, strPath.c_str());
		fseek(fp, 0, SEEK_END);
		size_t fSize = ftell(fp);
		fseek(fp, 0, SEEK_SET);

		char  *pBuf = new char[fSize];
		CHK_NEW_FAILED(pBuf);
		fread(pBuf, 1, fSize, fp);
		
		
		Head *pHeader = (Head *)pBuf;
		char *p = pBuf + sizeof(*pHeader);

		m_cutoff = pHeader->m_cutoff;
		loadXTagMap(&p,  pHeader->m_nWTItem,  m_wTagsMap);
		InitXTagFlags(m_wTagsMap, m_wTagFlagsMap);

		loadXTagMap(&p,  pHeader->m_nBTItem,  m_bchTagsMap);
		loadXTagMap(&p,  pHeader->m_nETItem,  m_echTagsMap);
		loadXTagMap(&p,  pHeader->m_nITItem,  m_ichTagsMap);


		vector<char *> wordVec;
		for (int i = 0; i < (int)pHeader->m_nWords; ++i)
		{
			char *pKey = p;
			p += strlen(pKey) + 1;
			wordVec.push_back(pKey);
		}

		for (int i = 0; i < (int)pHeader->m_nWords; ++i)
		{
			int count = *((int*) p);
			m_wCounter.Insert(CCodeConversion::UTF8ToUnicode(wordVec[i]), count);
			p += sizeof(int);
		}


		fclose(fp);
		delete pBuf;
		return true;
	}

	bool loadXTagMap(char **p, int nItem, CHashMap<wstring, wstring, WStrHash> &rXTagMap)
	{
		for (int i = 0; i < nItem; ++i)
		{
			char *pKey = *p;
			*p += strlen(pKey) + 1;

			char *pVal = *p;
			*p += strlen(pVal) + 1;
			
			rXTagMap.Insert(CCodeConversion::UTF8ToUnicode(pKey),  CCodeConversion::UTF8ToUnicode(pVal));
		}
		return true;
	}

	bool operator == (CWordTagManager &r)
	{
		if (m_cutoff != r.m_cutoff)
			return false;

		if (cmpMaps(m_bchTagsMap, r.m_bchTagsMap) == false)
			return false;

		if (cmpMaps(m_echTagsMap, r.m_echTagsMap) == false)
			return false;

		if (cmpMaps(m_ichTagsMap, r.m_ichTagsMap) == false)
			return false;

		if (cmpMaps(m_wTagsMap, r.m_wTagsMap) == false)
			return false;

		if (cmpMaps(m_wCounter, r.m_wCounter) == false)
			return false;

		return true;
	}

	template<typename type>
	bool cmpMaps(CHashMap<wstring, type, WStrHash> &map1,
				 CHashMap<wstring, type, WStrHash> &map2)
	{
		typename CHashMap<wstring, type, WStrHash>::iterator iter = map1.begin();
		for (; iter != map1.end(); ++iter)
		{
			if (map2.Find(iter->first) == map2.end() ||
				map2.Find(iter->first)->second != iter->second)
				return false;
		}

		for (iter = map2.begin(); iter != map2.end(); ++iter)
		{
			if (map1.Find(iter->first) == map1.end() ||
				map1.Find(iter->first)->second != iter->second)
				return false;
		}
		return true;
	}

	void InitXTagFlags(CHashMap<wstring, wstring, WStrHash> &xtagMap,
						CHashMap<wstring, bool *, WStrHash> &xTagFlags)
	{
		CHashMap<wstring, wstring, WStrHash>::iterator iter = xtagMap.begin();
		wchar_t tagBuf[65536];
		for (; iter != xtagMap.end(); ++iter)
		{
			wstring word = iter->first;
			wstring tags = iter->second;


			// initialize flag map
			wcscpy(tagBuf, tags.c_str());
			vector<wchar_t *> tagVec = Split(tagBuf, L"_");

			bool *pFlags = (bool *)m_pool.Allocate(sizeof(bool) * CGlobalMap::s_POSIDMap.Size());
			memset(pFlags, 0, sizeof(bool) * CGlobalMap::s_POSIDMap.Size());
			for (size_t k = 0; k < tagVec.size(); ++k)
			{
				int posId = CGlobalMap::s_POSIDMap.ID(tagVec[k]);
				pFlags[posId] = true;
			}


			// insert into flag map
			xTagFlags.Insert(word, pFlags);
		}
	}

	CHashMap<wstring, int, WStrHash> &GetWCounter(){return m_wCounter;}

private:
	/* word based */
	CHashMap<wstring, bool *, WStrHash> m_wTagFlagsMap;
	CHashMap<wstring, wstring, WStrHash> m_wTagsMap;

	/* character based dictionary */
	CHashMap<wstring, bool *, WStrHash> m_bchTagFlagsMap;
	CHashMap<wstring, wstring, WStrHash> m_bchTagsMap;

	CHashMap<wstring, bool *, WStrHash> m_echTagFlagsMap;
	CHashMap<wstring, wstring, WStrHash> m_echTagsMap;

	/* inner char */
	CHashMap<wstring, bool *, WStrHash> m_ichTagFlagsMap;
	CHashMap<wstring, wstring, WStrHash> m_ichTagsMap;


	CHashMap<wstring, int, WStrHash> m_wCounter;
//	CHashMap<wstring, int, WStrHash> m_tagIDMap;
	CPool m_pool;
};
#endif  /*__LEXICON_H__*/
