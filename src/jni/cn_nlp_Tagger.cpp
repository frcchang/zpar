#include "cn_nlp_Tagger.h"
#include "tagger.h"
#include "knowledge/tagdict.h"
#include <ext/hash_map>
#include "linguistics/word_tokenized.h"

#include "definitions.h"
#include "reader.h"
#include "writer.h"
#include "stdlib.h"

using namespace chinese;
const unsigned long nMaxSentSize=512;
std::string separator="_";
const int nMaxWordLength=20;
bool shouldUseUserDict=false;

CTagger *taggerObj;

CWordDictionary m_Knowledge;

void loadKnowledge(const std::string &sKnowledgePath) {
  //      std::cout << "Knowledge is provided but not used." << std::endl;
  std::cerr << "Loading knowledge from "<<sKnowledgePath<<"... "<<std::endl;
  if(sKnowledgePath.length()==0)
  {
    std::cerr<<"Knowledge file does not exist."<<std::endl;
    return;
  }

  std::ifstream is(sKnowledgePath.c_str());
  if (!is)
  {
    std::cerr<<"Knowledge file "<< sKnowledgePath << " does not exist."<<std::endl;
    return;
  }
  int nWordCount=0;
  std::string s, word, tag ;
  unsigned long long i ;
  getline(is, s);
  while( is && !(s.empty()) ) {
    std::istringstream iss(s);
    iss >> word >> tag ;
    i = CTag(tag).code() ;
    ASSERT(i, "Unrecognized tag in the input file for the dict: "<<tag);
    m_Knowledge[word]=i;//|= (static_cast<unsigned long long>(1)<<i) ;
    //std::cout << word << "\t" << i << std::endl ;
    getline(is, s);
    shouldUseUserDict=true;
    nWordCount++;
  }
  //is>>m_Knowledge;
  //        CWordDictionary::iterator it;
  //       it = m_Knowledge.begin() ;
  //       while (it != m_Knowledge.end()) {
  //          std::cout << it.first().str() << "\t" << it.second() << std::endl ;
  //          ++it ;
  //       }

  std::cerr<<"nWordCount = "<<nWordCount<<std::endl;
  is.close();
  std::cerr << "done." << std::endl;
}

void changePOS(const CTwoStringVector *sentence,CTwoStringVector *new_sentence)
{
  new_sentence->clear();
  for (int i=0; i<sentence->size(); ++i) {
    CWord word(sentence->at(i).first);
    if(m_Knowledge.element(word) && m_Knowledge.lookup(word)!=35)
    {
      new_sentence->push_back(std::make_pair(sentence->at(i).first,CTag(m_Knowledge.lookup(word)).str()));
    }
    else
    {
      new_sentence->push_back(sentence->at(i));
    }
  }
}

/*----------------------------------------------------------------
 *
 * recordSegmentation - record a segmented sentence with bitarr.
 *
 * Input raw is the raw format of input tagged, it is provided
 * here because the caller already did UntagAndDesegmentSent
 *
 *---------------------------------------------------------------*/

void recordSegmentation(const CStringVector *raw, const CStringVector* segmented, CBitArray &retval) {
  std::vector<int> indice;
  indice.clear();
  for (int i=0; i<raw->size(); ++i) {
    for (int j=0; j<raw->at(i).size(); ++j)
      indice.push_back(i);
  }
  retval.clear();
  int start=0;
  for (int i=0; i<segmented->size(); ++i) {
    int word_start = indice[start];
    int word_end = indice[start+segmented->at(i).size()-1];
    start += segmented->at(i).size();
    retval.set(word_end);
  }
}
void string2CStringVector(const char *str,CStringVector *vReturn,bool bIgnoreSpace)
{
  vReturn->clear();
  CStringVector *vMiddle = new CStringVector;
  char cTemp;
  std::string sWord;                                // std::string for next word
  bool bReadSomething = false;                 // did we read anything?
  for(int i=0;i<strlen(str);i++) {
    cTemp=str[i];
    if (bIgnoreSpace && cTemp == ' ') {       // otherwise, if we meet " "
      if (!sWord.empty()) {                  // we have found another word
        getCharactersFromUTF8String(sWord, vMiddle);
        sWord = "";
      }
    }
    else
    {
      sWord += cTemp;
      if (isOneUTF8Character(sWord)) {
        getCharactersFromUTF8String(sWord, vMiddle);
        sWord = "";
      }

    }
  }

  int i=0;
  //
  for(;i<vMiddle->size()-1;i++)
  {
    std::string sWord=vMiddle->at(i)+vMiddle->at(i+1);
    std::vector<int> vecInDictID;
    std::vector<std::string> vecInDictWord;

    if(shouldUseUserDict==1 && m_Knowledge.element(sWord))
    {
      int j=1;
      for( j=2;i+j<vMiddle->size() && j<nMaxWordLength;j++)
      {
        sWord.append(vMiddle->at(i+j));
        if(shouldUseUserDict==1 && m_Knowledge.element(sWord))
        {
          vecInDictID.push_back(j);
          vecInDictWord.push_back(sWord);
        }
      }
      if(vecInDictID.size()>0)
      {
        vReturn->push_back(vecInDictWord.at(vecInDictID.size()-1));
        i+=vecInDictID.at(vecInDictID.size()-1);
      }
      else{
        vReturn->push_back(vMiddle->at(i)+vMiddle->at(i+1));
        i+=1;
      }
    }
    else{
      vReturn->push_back(vMiddle->at(i));
    }
  }
  if(i==vMiddle->size()-1)
  {
    vReturn->push_back(vMiddle->at(i));
  }
  delete vMiddle;
}
void CStringVector2CStringVector(CStringVector *vReturn,CStringVector *vMiddle,bool bIgnoreSpace)
{
  vMiddle->clear();
  int i=0;
  for(;i<vReturn->size()-1;i++)
  {
    std::string sWord=vReturn->at(i)+vReturn->at(i+1);
    std::vector<int> vecInDictID;
    std::vector<std::string> vecInDictWord;

    if(shouldUseUserDict==1 && m_Knowledge.element(sWord))
    {
      int j=1;
      for( j=2;i+j<vReturn->size() && j<nMaxWordLength;j++)
      {
        sWord.append(vReturn->at(i+j));
        if(shouldUseUserDict==1 && m_Knowledge.element(sWord))
        {
          vecInDictID.push_back(j);
          vecInDictWord.push_back(sWord);
        }
      }
      if(vecInDictID.size()>0)
      {
        vMiddle->push_back(vecInDictWord.at(vecInDictID.size()-1));
        i+=vecInDictID.at(vecInDictID.size()-1);
      }
      else{
        vMiddle->push_back(vReturn->at(i)+vReturn->at(i+1));
        i+=1;
      }
    }
    else{

      vMiddle->push_back(vReturn->at(i));
    }
  }
  if(i==vReturn->size()-1)
  {
    vMiddle->push_back(vReturn->at(i));
  }
}
std::string CTwoStringVector2string(const CTwoStringVector *sentence, std::string separator) {
  //std::string str_sen="";
  std::string charArrSen;
  int nCharIndex=0;
  for (int i=0; i<sentence->size(); ++i) {
    if (i>0)
      charArrSen.append(" ");
    charArrSen.append(sentence->at(i).first);
    charArrSen.append(separator);
    charArrSen.append(sentence->at(i).second);
  }

  //   const char charArrSen[sentence->size()];
  //   int nCharIndex=0;
  //   for (int i=0; i<sentence->size(); ++i) {
  //      if (i>0)
  //         charArrSen[nCharIndex++]=' ';
  //      for (int j=0; j<sentence->at(i).first.length(); j++)
  //         charArrSen[nCharIndex++]=(sentence->at(i).first[j]);
  //      charArrSen[nCharIndex++]=(separator);
  //      for (int j=0; j<sentence->at(i).second.length(); j++)
  //         charArrSen[nCharIndex++]=(sentence->at(i).second[j]);
  //   }
  return charArrSen;
}


  JNIEXPORT jstring JNICALL Java_cn_nlp_Tagger_tagSentence
(JNIEnv *env, jobject obj, jstring sSen)
{
  try {
    bool bSegmented=false;
    bool bScores=false;
    unsigned long nBest=1;
    CStringVector *input_sent = new CStringVector;
    CTwoStringVector *output_sent = new CTwoStringVector[nBest];
    CTwoStringVector *output_sent_new=new CTwoStringVector;
    tagger::SCORE_TYPE *output_scores;


    const char *strMsgPtr = env->GetStringUTFChars( sSen , 0);
    string2CStringVector(strMsgPtr,input_sent,false);
    env->ReleaseStringUTFChars( sSen, strMsgPtr);


    if ( input_sent->size() >= tagger::MAX_SENTENCE_SIZE ) {
      std::cerr << "The size of the sentence is larger than the limit (" << tagger::MAX_SENTENCE_SIZE << "), skipping." << std::endl;
      for (int i=0; i<nBest; ++i) {
        output_sent[i].clear();
      }
    }
    else
    {
      taggerObj->tag(input_sent, output_sent, output_scores, nBest);
      changePOS(output_sent,output_sent_new);
    }
    std::string str_output_sen=CTwoStringVector2string(output_sent_new,separator);
    delete input_sent;
    delete output_sent_new;
    delete [] output_sent;
    return env->NewStringUTF(str_output_sen.c_str());
  } catch(
      const std::string&e) {std::cerr<<"Error: "<<e<<std::endl;
      return env->NewStringUTF("Error");
  }
}

/*
 * Class:     cn_nlp_Tagger
 * Method:    init
 * Signature: (Ljava/lang/String;Ljava/lang/String;)I
 */
  JNIEXPORT jint JNICALL Java_cn_nlp_Tagger_init
(JNIEnv *env, jobject obj, jstring sModelFile, jstring sUserDictFile)
{
  try {
    const char *strMsgPtr = env->GetStringUTFChars( sModelFile , 0);
    const char *strMsgDic = env->GetStringUTFChars( sUserDictFile , 0);
    std::string sFeatureDBPath=strMsgPtr;
    std::string sKnowledgePath=strMsgDic;

    loadKnowledge(sKnowledgePath);
    std::cerr<<"sFeatureDBPath="<<sFeatureDBPath<<std::endl;
    taggerObj=new CTagger(sFeatureDBPath,false, nMaxSentSize, true);

    env->ReleaseStringUTFChars( sModelFile, strMsgPtr);
    env->ReleaseStringUTFChars( sUserDictFile, strMsgDic);
  } catch(const std::string&e) {std::cerr<<"Error: "<<e<<std::endl;return 1;}
  return 1;
}

/*
 * Class:     cn_nlp_Tagger
 * Method:    tagFile
 * Signature: (Ljava/lang/String;Ljava/lang/String;)I
 */
  JNIEXPORT jint JNICALL Java_cn_nlp_Tagger_tagFile
(JNIEnv *env, jobject obj, jstring sInputFile, jstring sOutputFile)
{
  try {
    std::cerr << "Tagging started" << std::endl;
    int time_start = clock();

    bool bSegmented=false;
    bool bScores=false;
    unsigned long nBest=1;

    const char *strMsgPtr = env->GetStringUTFChars( sInputFile , 0);
    std::string sInputFile_=strMsgPtr;

    const char *strMsgPtrO = env->GetStringUTFChars( sOutputFile , 0);
    std::string sOutputFile_=strMsgPtrO;

    env->ReleaseStringUTFChars( sInputFile, strMsgPtr);
    env->ReleaseStringUTFChars( sOutputFile, strMsgPtrO);

    //std::cout<<"sInputFile="<<sInputFile_<<std::endl;

    CSentenceReader input_reader(sInputFile_);
    CSentenceWriter output_writer(sOutputFile_);
    std::ofstream *score_writer;
    CStringVector *input_sent = new CStringVector;
    CStringVector *middle_sent = new CStringVector;
    CTwoStringVector *output_sent_new=new CTwoStringVector;
    CTwoStringVector *output_sent;
    tagger::SCORE_TYPE *output_scores;

    int nCount=0;

    output_sent = new CTwoStringVector[nBest];
    if (bScores) {
      output_scores = new tagger::SCORE_TYPE[nBest];
      score_writer = new std::ofstream(std::string(sOutputFile_+".scores").c_str());
    }
    else
      output_scores = NULL;

    CBitArray prunes(tagger::MAX_SENTENCE_SIZE);
    CStringVector *unsegmented_sent;
    if (bSegmented) unsegmented_sent = new CStringVector;

    static bool bReadSuccessful;
    if (bSegmented)
      bReadSuccessful = input_reader.readSegmentedSentence(input_sent);
    else
      bReadSuccessful = input_reader.readRawSentence(input_sent, true, false);

    static int i;

    // If we read segmented sentence, we will ignore spaces from input.
    while( bReadSuccessful ) {
      if(nCount%100==0)
        std::cerr<<"Sentence " << nCount<<std::endl;
      TRACE("Sentence " << nCount);
      ++ nCount;
      //
      // Find decoder output
      //
      if (bSegmented) {
        DesegmentSentence(input_sent, unsegmented_sent);
        recordSegmentation( unsegmented_sent, input_sent, prunes );
        // check sentence size
        if ( unsegmented_sent->size() >= tagger::MAX_SENTENCE_SIZE ) {
          std::cerr << "The size of the sentence is larger than the limit (" << tagger::MAX_SENTENCE_SIZE << "), skipping." << std::endl;
          for (i=0; i<nBest; ++i) {
            output_sent[i].clear();
            if (bScores) output_scores[i]=0;
          }
        }
        else
          taggerObj->tag(unsegmented_sent, output_sent, output_scores, nBest, &prunes);
      }
      else {
        if ( input_sent->size() >= tagger::MAX_SENTENCE_SIZE ) {
          std::cerr << "The size of the sentence is larger than the limit (" << tagger::MAX_SENTENCE_SIZE << "), skipping." << std::endl;
          for (i=0; i<nBest; ++i) {
            output_sent[i].clear();
            if (bScores) output_scores[i]=0;
          }
        }
        else
        {
          middle_sent->clear();
          CStringVector2CStringVector(input_sent,middle_sent,false);
          taggerObj->tag(middle_sent, output_sent, output_scores, nBest);
          changePOS(output_sent,output_sent_new);
        }
      }
      //
      // Ouptut sent
      //
      output_writer.writeSentence(output_sent_new);
      //      for (i=0; i<nBest; ++i) {
      //         output_writer.writeSentence(output_sent_new);
      //         if (bScores) (*score_writer)<<output_scores[i]<<std::endl;
      //      }
      if (bSegmented)
        bReadSuccessful = input_reader.readSegmentedSentence(input_sent);
      else
        bReadSuccessful = input_reader.readRawSentence(input_sent, true, false);
    }

    delete input_sent;
    delete output_sent_new;
    delete middle_sent;
    delete [] output_sent;
    if (bScores) {
      delete [] output_scores;
      score_writer->close(); delete score_writer; // remove file
    }

    if (bSegmented) delete unsegmented_sent;

    std::cerr << "Tagging has finished successfully. Total time taken is: " << double(clock()-time_start)/CLOCKS_PER_SEC << std::endl;
    return 1;
  } catch(const std::string&e) {std::cerr<<"Error: "<<e<<std::endl;return 1;}
  return 1;
}

