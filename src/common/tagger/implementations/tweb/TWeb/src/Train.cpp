#include <ctime>
#include "Lexicon.h"
#include "util.h"
#include "Config.h"
#include "Template.h"
#include "BaseExtractor.h"
#include "EnExtractor.h"
#include "ChExtractor.h"
#include "EasyFirstPOS.h"
#include "NNScorer.h"

int Tagging(vector<_SEN*> &rSenVec,   CTagger &rTagger);

CBaseExtractor * 
CreateExtractor(CTemplateManager &tmpMgr, 
								CWordTagManager &lexicon,
								int l, 	int r, 
								bool bInsert, bool fVerbose,
								bool eng)
{
  if (eng == true)
    return new CEnFeatureExtractor(&tmpMgr, &lexicon, //&widMap, 
																	 l, r, bInsert, fVerbose); 
  else
    return new CChFeatureExtractor(&tmpMgr, &lexicon, //&widMap, 
																	 l, r, bInsert, fVerbose); 
}


SEvalRecorder 
EvalSentences(vector<_SEN*> &rGuesses, 
							vector<_SEN*> &rGold,
							const string &strPath)
{
	if (rGuesses.size() != rGold.size())
	{
		fprintf(stderr, "Error: sentence number inconsistent, gold %d vs guess %d\n",
			(int)rGold.size(), (int)rGuesses.size());
		exit(0) ;
	}

	SEvalRecorder rec;
	for (size_t i = 0; i < rGuesses.size(); ++i)
	{
		if (rGuesses[i]->Length() != rGold[i]->Length())
		{
			fprintf(stderr, "Error sentence length inconsistent %d vs %d\n",
				(int)rGuesses[i]->Length(), (int)rGold[i]->Length());
			exit(0);
		}

		_SEN *pGuess = rGuesses[i], *pGold = rGold[i];
		for (int wind = 0; wind < pGuess->Length(); ++wind)
		{
			bool isOOV = CGlobalMap::s_WIDMap.ID(pGold->Word(wind)) == 
									 CGlobalMap::s_WIDMap.UnkID();
			if (//(pGold->GForm(wind) == wstring(L"url")) &&
					// pGold->TAG(wind) == wstring(L"ADD"))|| 
					pGuess->TID(wind) == pGold->TID(wind))
			{
				rec.nCorrect++;
				rec.nOOVCorr += isOOV;
			}
		}
		rec.nTotal	+= rGold[i]->Length();
	
	}

/*
	FILE *fp = fopen(("eval" + strPath + ".txt").c_str(), "w");
	if (fp != NULL)
	{
		for (int i = 0; i < rGuesses.size(); ++i)
		{
			_SEN *pSen = rGuesses[i];
			for (int k = 0; k < pSen->Length(); ++k)
			{
				string tag  = pSen->GForm(k) == wstring(L"url")?
											"ADD": wstr2utf8(pSen->Tag(k));
				fprintf(fp, "%-15s %-15s %s\n", wstr2utf8(pSen->Word(k)).c_str(),
								wstr2utf8(pSen->GForm(k)).c_str(), tag.c_str());  
			}
			fprintf(fp, "\n");
		}
		fclose(fp);
	}
*/
	rec.accuracy = 100.0 * rec.nCorrect/rec.nTotal;	
	return rec;
}


void Save(const string & path, CTagger &tagger)
{
	string confPath   = CConfig::strModelPath + path + ".config";
	string featurePath= CConfig::strModelPath + path + ".feature"; 
	string tempPath   = CConfig::strModelPath + path + ".temp";
	string modelPath  = CConfig::strModelPath + path + ".NN";
	string MapPath    = CConfig::strModelPath + path + ".map";
	fprintf(stderr, "Save config\n");
	CConfig::SaveConfig(confPath);			// save configuration
	fprintf(stderr, "Save POS tags\n");
	CGlobalMap::Save(MapPath);
	fprintf(stderr, "Save Scorer\n");
	tagger.GetNNScorer()->Save(modelPath);	
	fprintf(stderr, "Save features\n");
	CBaseExtractor *pExtor = tagger.GetExtractor();
	pExtor->Save(featurePath);
	pExtor->GetTempMgr()->Save(tempPath);					// save templates 
}

double EvalDevTest(CTagger &tagger, FILE *fpLog,
								 vector<vector<_SEN *> > &devTestGold,
								 vector<vector<_SEN *> > &devTest,
								 vector<string> &paths)
{
	// evaluate dev set
	assert(devTestGold.size() == devTest.size());
	assert(devTest.size() == paths.size());

	if (paths.size() == 0)
		return 0;

	double total = 0, totalCrr = 0, avgAcc = 0.0;
	vector<double> vTotal, vCorr;
	for (size_t i = 0; i < devTest.size(); ++i)
	{
		Tagging(devTest[i], tagger); 
		SEvalRecorder eRec = EvalSentences(devTest[i], devTestGold[i], "");
		vTotal.push_back(eRec.nTotal);
		vCorr.push_back(eRec.nCorrect);

		if (paths[i].find("wsj") == paths[i].npos)
		{
			total += eRec.nTotal;
			totalCrr += eRec.nCorrect;
		}
	}

	for (size_t i = 0; i < devTest.size(); ++i)
	{
		fprintf(stderr, "%-16s:", paths[i].c_str());
		fprintf(stderr, "%-4.2f%%, %-6d/%-6d\n", 100.0 * vCorr[i]/vTotal[i],
				   (int)vCorr[i], (int)vTotal[i]); 
		if (fpLog != NULL)
		{
			fprintf(fpLog, "%-16s:", paths[i].c_str());
			fprintf(fpLog, " %-6d/%-6d, accuracy %.2f%%\n", 
						(int)vCorr[i], (int)vTotal[i], 100.0 * vCorr[i]/vTotal[i]);
		}
	}
		
	avgAcc = total == 0 ? 100.0 * vCorr[0]/vTotal[0] : 
												100.0 * totalCrr/ total;
	fprintf(stderr, "Average accuracy (wsj part excluded) %.2f%%\n", avgAcc);
	if (fpLog != NULL)
		fprintf(fpLog, "Average accuracy (wsj part excluded) %.2f%%\n", avgAcc);
		
	return avgAcc;
}


void Train(vector<_SEN *> &senTrain,		 
					 vector<_SEN *> &senTrainGoldTag,	
					 vector<vector<_SEN *> > &senDev,
					 vector<vector<_SEN *> > &senDevGold,	 
					 vector<string>	&devNames,
					 vector<vector<_SEN *> > &senTest,
					 vector<vector<_SEN *> > &senTestGold,	 
					 vector<string>	&testNames,
					 CTagger &tagger) 
{
	CPool trainingPool;
	vector<_SEN *> senTrainSubsetGold, senTrainSubset;
	for (size_t i = 0; i < senTrainGoldTag.size(); i += 12)
		senTrainSubsetGold.push_back(senTrainGoldTag[i]);
	senTrainSubset = _SEN::CopySentences(senTrainSubsetGold, trainingPool);

	double bestDevAcc = 0, bestTestAccByDev	= 0, bestTestAcc = 0; 
	int bestDevIter = 0,  bestTestIter = 0;

	string path    = CConfig::BuildPath();
	string logPath = path+ ".log";
	fprintf(stderr, "\npath: %s\n", path.c_str());

	FILE *fpLog = fopen((CConfig::strLogDir + "/" + logPath).c_str(), "w");
	if (fpLog == NULL)
		fprintf(stderr, "Warning: Log file open failed\n");
	
	// OK, here we go...
	clock_t totalStart = 0;
	for (int nIter = 0; nIter < CConfig::nRound; ++ nIter)
	{
		fprintf(stderr, "\n-----------------------Training %d round----------------------\n", nIter);
		vector<int> randIDs = Shuffel((int)senTrainGoldTag.size());
		if (nIter < 5)
			tagger.SetIMode(true);
		
		tagger.ResetStatis();
		clock_t start = clock();
		tagger.GetNNScorer()->UnAverage();
		for (size_t i = 0; i < randIDs.size(); ++ i)
		{
			tagger.TrainGreedy(senTrain[randIDs[i]], 
												 senTrainGoldTag[randIDs[i]], 
												 CConfig::fRate, CConfig::fMargin);
			
			if (i != 0 && i % 400 == 0)
			{
				CTagger::SStatics statics = tagger.GetStatics();
				fprintf(stderr, "%lu sen, %d updates\r", i, statics.nRetry);
			}
		}

		tagger.GetNNScorer()->AverageParameter();
		CTagger::SStatics statics = tagger.GetStatics();
		double secs = (double)(clock() - start)/CLOCKS_PER_SEC;
		fprintf(stderr, "%d sen, %d updates, %.2f secs\n",
			(int)randIDs.size(),	statics.nRetry,		secs);

//		fprintf(stderr, "%d invalid, %d no valid, %d traceback\n",
//			statics.nInvalid,	statics.nNoValid,  statics.nTraceBack);
//		tagger.SetIMode(false);

		fprintf(stderr, "\nTagging training set ...\n");
		Tagging(senTrainSubset, tagger);
		SEvalRecorder eRec = EvalSentences(senTrainSubset, senTrainSubsetGold, "");
		fprintf(stderr, "Total %d, correct %d, accuracy %.2f%%\n", 
			eRec.nTotal,  eRec.nCorrect,  100.0 * eRec.nCorrect/eRec.nTotal);

		if (fpLog != NULL)
			fprintf(fpLog, "Iter %d, trainAcc %.2f%%, ", nIter, eRec.accuracy);

		// evaluate dev set
		fprintf(stderr, "\nTagging dev set(s) ...\n");
		double devAvgAcc = EvalDevTest(tagger, fpLog, senDevGold, senDev, devNames);
		fprintf(stderr, "\nTagging test set(s) ...\n");
		double testAvgAcc = EvalDevTest(tagger, fpLog, senTestGold, senTest, testNames);
		if (bestDevAcc < devAvgAcc)
		{
			bestDevAcc = devAvgAcc;
			bestTestAcc = testAvgAcc;
			bestDevIter = nIter;

			if (nIter >= 6)
				Save(path, tagger);
		}
	}

	
	double secs = (double)(clock() - totalStart)/CLOCKS_PER_SEC;
	if (fpLog != NULL)
	{
		fprintf(fpLog, "\nbestDevIter %d, bestDevAcc %.2f%%, bestTestAccByDev %.2f%%\nbestTestIter %d, bestTestAcc %.2f%%\n",
			bestDevIter, bestDevAcc, bestTestAccByDev, bestTestIter, bestTestAcc);
		fclose(fpLog);
	}
	fprintf(stderr, "\nbestDevIter %d, bestDevAcc %.2f%%, bestTestAccByDev %.2f%%\nbestTestIter %d, bestTestAcc %.2f%%\n",
		bestDevIter, bestDevAcc, bestTestAccByDev, bestTestIter, bestTestAcc);
	fprintf(stderr, "%.2f secs or %.2f hours eclipsed\n", secs, secs/3600);
}


string GetFileNameFromDir(string path)
{
	return path.substr(path.rfind('/') + 1, path.find('.') - path.rfind('/') - 1);
}


void LoadDevTest(vector<string> &paths, 
								 vector<vector<_SEN*> > &goldSen,
								 vector<vector<_SEN*> > &senNoTag,
								 CPool &pool)
{
	for (size_t i = 0; i < paths.size(); ++i)
	{
		goldSen.push_back(CReader::ReadSentences(paths[i], pool, false));
		senNoTag.push_back(_SEN::CopySentences(goldSen.back(), pool));		// tags are not copied
		paths[i] = GetFileNameFromDir(paths[i]);
	}
}

void Training(const string &strPath)
{
	if (CConfig::ReadConfig(strPath) == false)
	{
		fprintf(stderr, "Reading config file failed\n");
		return ;
	}
	
	// init global g-form map
	SWRRBM *pRBM =  NULL;
	if (CConfig::strRBMPrefix != CConfig::strNull)
		pRBM = SWRRBM::RBMFromFile(CConfig::strRBMPrefix);
	CRBMNN rbmNN(pRBM, false);	

	// reading training, dev and test data
	CPool trainingPool;
	vector<_SEN *> senTrain,   senTrainGoldTag;
	vector<vector<_SEN *> > senDev,	senDevGold;
	vector<vector<_SEN *> > senTest,	senTestGold;
	vector<string> devNames, testNames;

	senTrainGoldTag = CReader::ReadSentences(CConfig::strTrainPath, trainingPool, true);
	senTrain = _SEN::CopySentences(senTrainGoldTag,  trainingPool);
	LoadDevTest(CConfig::vDevPaths, senDevGold, senDev, trainingPool);
	LoadDevTest(CConfig::vTestPaths, senTestGold, senTest, trainingPool);

	// extract training sub-set
	CTemplateManager tpMgr;
	if (tpMgr.ReadingTemplate(CConfig::strTempPath) == false)
	{
		fprintf(stderr, "Reading template failed %s\n", CConfig::strTempPath.c_str());
		exit(0);
	}
	
	bool featureVerbose = false;
	bool tagVerbose = CConfig::bTagVerbose;
	bool insertMode	= true;
	CWordTagManager  lexicon(CConfig::nCutoff);
	CBaseExtractor *pExtractor = CreateExtractor(tpMgr, lexicon, 
															 rbmNN.Left(),	 rbmNN.Right(),	
															 insertMode, featureVerbose, 
															 CConfig::bEnTagger);

	CNNScorer scorer(&rbmNN, CConfig::vIHSize[0], CGlobalMap::s_POSIDMap.Size());//CPOSIDMap::GetPOSNum()); 
	CTagger tagger(tagVerbose, false);
	tagger.Init(pExtractor,  &lexicon,  &scorer, 	CConfig::nBS);
	Train(senTrain, senTrainGoldTag, 
				senDev,		senDevGold, 	CConfig::vDevPaths,
				senTest, 	senTestGold, 	CConfig::vTestPaths, tagger);
  delete pExtractor;
	
	if (pRBM != NULL);
		delete pRBM;
}

