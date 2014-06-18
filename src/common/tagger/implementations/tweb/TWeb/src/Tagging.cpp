#include "EasyFirstPOS.h"
#include "EnExtractor.h"
#include "ChExtractor.h"
#include "util.h"
#include "Config.h"
SEvalRecorder 
EvalSentences(vector<_SEN*> &rGuesses, 	
							vector<_SEN*> &rGold,  
							const string &strPath);

CBaseExtractor * 
LoadExtractor(const string & strPath,
 						  CTemplateManager & tmpMgr)
{
	fprintf(stderr, "\nLoading from %s\n", strPath.c_str());
	FILE *fp = fopen(strPath.c_str(), "r");
	if (fp == NULL)
	{
		fprintf(stderr, "Loading %s failed\n", strPath.c_str());
		exit(0);
	}

	char buf[1024];
	fgets(buf, 1024, fp);
	removeNewLine(buf);
	CBaseExtractor *pExtor = NULL;
	if (string(buf) == CEnFeatureExtractor().GetName())
		pExtor = new CEnFeatureExtractor();
	else if (string (buf) == CChFeatureExtractor().GetName())
		pExtor = new CChFeatureExtractor();
	else
	{
		fprintf(stderr, "Error: unknown extractor type %s\n", buf);
		exit(0);
	}

	pExtor->SetTempMgr(&tmpMgr);
	pExtor->Load(fp);
	fclose(fp);
	return pExtor;
}

void Tagging(vector<_SEN*> &rSenVec,   CTagger &rTagger)
{
	rTagger.SetIMode(false);
	clock_t start = clock();
	for (size_t i = 0; i < rSenVec.size(); ++i)
	{
		if (i != 0 && i % 500 == 0)
			fprintf(stderr, "Tagging %lu sentence\r", i);
		rTagger.Tagging(rSenVec[i]);
//		_SEN *pRes = rTagger.Tagging(rSenVec[i]);
//		for (int k = 0; k < pRes->Length(); ++k)
//			if (rTagger.GetLexicon()->SeenWord(rSenVec[i]->Word(k)) == false)
//				++nOOV;
	}

	double secs = 1.0*(clock() - start)/CLOCKS_PER_SEC;
	fprintf(stderr, "Total %d sentences, %.2f secs eclipsed, %.2f sens per sec\n", 
		(int)rSenVec.size(),  secs,	 rSenVec.size()/secs);
//	return nOOV;
}

void Load(const string &strPath, 
					CNNScorer &scorer,
					CTemplateManager &tempMgr,
					CBaseExtractor **pExtor)
{
	string confPath   =strPath + ".config";
	string featurePath=strPath + ".feature"; 
	string tempPath   =strPath + ".temp";
	string modelPath  =strPath + ".NN";
	string mapPath    =strPath + ".map";
	fprintf(stderr, "Loading Config\n");
	CConfig::LoadConfig(confPath);
	fprintf(stderr, "Loading POS Labels\n");
	CGlobalMap::Load(mapPath);
	fprintf(stderr, "Loading NNScorer\n");
	scorer.Load(modelPath);
	fprintf(stderr, "Reading templates\n");
	tempMgr.ReadingTemplate(tempPath);

	fprintf(stderr, "Loading Features\n");
	*pExtor = LoadExtractor(featurePath, tempMgr); 
	(*pExtor)->SetWindow(scorer.GetRBMNN()->Left(),
										scorer.GetRBMNN()->Right());//rbmNN.Left(), rbmNN.Right());
	fprintf(stderr, "Loading complete\n");
}


void Tagging(const string &strModelPrefix, 
						 const string &strIn, 
						 const string &strOut)
{
	CRBMNN rbmNN;
	CNNScorer nnScorer(&rbmNN);
	CWordTagManager lexicon;
	CTemplateManager tempMgr;
	CBaseExtractor *pExtor = NULL;
	CTagger tagger(false, false);
	
	string strPrefix = strModelPrefix;
	if (strPrefix.back() == '.')
		strPrefix.erase(strPrefix.begin() + strPrefix.size() - 1);
	
	Load(strPrefix, nnScorer, tempMgr, &pExtor);
	tagger.Init(pExtor, &lexicon, &nnScorer, CConfig::nBS);
	CPool pool;
	
	vector<_SEN *> senVecGold = CReader::ReadSentences(strIn, pool);
	vector<_SEN *> senVec = _SEN::CopySentences(senVecGold, pool);
	
	fprintf(stderr, "Total %d sentences\n", (int)senVec.size());
	Tagging(senVec, tagger);	
	
	FILE *fpOut = fopen(strOut.c_str(), "w");
	assert(fpOut != NULL);
	for (size_t i = 0; i < senVec.size(); ++i)
	{
		_SEN *pRes = senVec[i];
		for (int k = 0; k < pRes->Length(); ++k)
			fprintf(fpOut, "%-15s %-15s %s\n",	wstr2utf8(pRes->Word(k)).c_str(), 
							wstr2utf8(pRes->GForm(k)).c_str(),
							pRes->GForm(k) == wstring(L"url") ? "ADD" : wstr2utf8(pRes->Tag(k)).c_str());
		fprintf(fpOut, "\n");
	}

	fclose(fpOut);
	SEvalRecorder eRec = EvalSentences(senVec, senVecGold, "");
	fprintf(stderr, "Total %d, correct %d, accuracy %.2f%%\n", 
			eRec.nTotal,  eRec.nCorrect,  100.0 * eRec.nCorrect/eRec.nTotal);
	delete tagger.GetExtractor();
}
