#include "cn_nlp_Parser.h"
#include "definitions.h"
#include "depparser.h"
#include "reader.h"
#include "writer.h"
#include "stdlib.h"

using namespace TARGET_LANGUAGE;
const unsigned long nMaxSentSize=512;
const char separator_p='_';

CDepParser *parser;
void string2CStringVector(const char *str,CTwoStringVector *vReturn,bool bIgnoreSpace)
{
   vReturn->clear();
   char cTemp;
   //char separator_c=separator[0];
   std::string sWord;
   std::string sTag;
   bool bCurrentTag=false;
   // did we read anything?
   for(int i=0;i<strlen(str);i++) {
        cTemp=str[i];

      if (cTemp == ' ') {                       // otherwise, if we meet " "
         if (!sWord.empty() || !sTag.empty()) { // we have found another word
            if (sTag.empty()) {
               REPORT("Input file line " << str << ": not well formatted tag for" << sWord);
               vReturn->clear();
               return;
            }
            vReturn->push_back(std::make_pair(sWord, sTag));
            sWord = "";
            sTag = "";
            bCurrentTag = false;
         }
      }
//      else if ( cTemp == separator && sWord[sWord.size()-1] != '\\') { // otherwise, if we meet the separator char
      else if ( cTemp == separator_p ) { // otherwise, if we meet the separator char
         if (!bCurrentTag) { // current word
            bCurrentTag = true;
         }
         else { // already currently processing tag
            sWord += separator_p;
            sWord += sTag;
            sTag = "";
         }
      }
      else                                      // otherwise
         if (bCurrentTag)
            sTag += cTemp;
         else
            sWord += cTemp;
    }
    if (!sWord.empty() || !sTag.empty()) {        // we have found another word
      if (sTag.empty()) {
         REPORT("Input file line " << str << ": not well formatted tag for" << sWord);
         vReturn->clear();
         return;
      }

      vReturn->push_back(std::make_pair(sWord, sTag));
      sWord = "";
      sTag="";
   }
}
std::string CDependencyParse2string(const CDependencyParse sentence, std::string delimit) {
   //std::string str_sen="";
   std::string charArrSen;
   int nCharIndex=0;
   for (int i=0; i<sentence.size(); ++i) {
      charArrSen.append(sentence.at(i).word);
      charArrSen.append(delimit);
      charArrSen.append(sentence.at(i).tag);
      charArrSen.append(delimit);

      char *str=new char[10];
      sprintf(str,"%d",sentence.at(i).head);

      charArrSen.append(str);
      delete[]str;
      charArrSen.append(delimit);
      charArrSen.append(sentence.at(i).label);
      charArrSen.append("\n");
   }
   return charArrSen;
}

JNIEXPORT jstring JNICALL Java_cn_nlp_Parser_parseSentence
  (JNIEnv *env, jobject obj, jstring sSen)
  {
      bool bSegmented=false;
      bool bScores=false;
      unsigned long nBest=1;
      CTwoStringVector input_sent;
       CDependencyParse *output_sent = new CDependencyParse[nBest];
       depparser::SCORE_TYPE *scores=0;


       const char *strMsgPtr = env->GetStringUTFChars( sSen , 0);

        string2CStringVector(strMsgPtr,&input_sent,false);
        env->ReleaseStringUTFChars( sSen, strMsgPtr);


      if ( input_sent.size() >= depparser::MAX_SENTENCE_SIZE ) {
            std::cerr << "The size of the sentence is larger than the limit (" << depparser::MAX_SENTENCE_SIZE << "), skipping." << std::endl;
            for (int i=0; i<nBest; ++i) {
               output_sent[i].clear();
            }
         }
         else
         {
             parser->parse( input_sent , output_sent , nBest , scores ) ;
         }
        std::string str_output_sen=CDependencyParse2string(output_sent[0],"\t");
        for (int i=0; i<nBest; ++i) {
               output_sent[i].clear();
        }
        delete [] output_sent;
        delete[]scores;
      return env->NewStringUTF(str_output_sen.c_str());
  }

/*
 * Class:     cn_nlp_Tagger
 * Method:    init
 * Signature: (Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_cn_nlp_Parser_init
  (JNIEnv *env, jobject obj, jstring sModelFile)
  {
      const char *strMsgPtr = env->GetStringUTFChars( sModelFile , 0);
      std::string sFeatureDBPath=strMsgPtr;
      std::cerr<<"sFeatureDBPath="<<sFeatureDBPath<<std::endl;
      parser=new CDepParser(sFeatureDBPath, false, false);
      env->ReleaseStringUTFChars( sModelFile, strMsgPtr);
      return 1;
  }

/*
 * Class:     cn_nlp_Tagger
 * Method:    tagFile
 * Signature: (Ljava/lang/String;Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_cn_nlp_Parser_parseFile
  (JNIEnv *env, jobject obj, jstring sInputFile, jstring sOutputFile)
  {
      std::cerr << "Parsing started" << std::endl;
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

      CSentenceReader *input_reader;
   std::ifstream *is;
   std::ofstream os(sOutputFile_.c_str());
   std::ofstream *os_scores=0;
   depparser::SCORE_TYPE *scores=0;
   assert(os.is_open());
#ifdef JOINT_MORPH
   CStringVector input_sent;
#else
   CTwoStringVector input_sent;
#endif
   CCoNLLInput input_conll;
   CDependencyParse *output_sent;
   CCoNLLOutput *output_conll;
   depparser::CSuperTag *supertags;
   std::ifstream *is_supertags = 0;

   supertags = 0;
   bool bCoNLL=false;
//   if (!sSuperPath.empty()) {
//      supertags = new depparser::CSuperTag();
//      is_supertags = new std::ifstream(sSuperPath.c_str());
//      parser.setSuperTags(supertags);
//   }

   is = 0;
   input_reader = 0;
   if (bCoNLL)
      is = new std::ifstream(sInputFile_.c_str());
   else
      input_reader = new CSentenceReader(sInputFile_);

   int nCount=0;
   bool bReadSuccessful;

   if (bScores) {
      scores = new depparser::SCORE_TYPE[nBest];
      os_scores = new std::ofstream(std::string(sOutputFile_+".scores").c_str());
   }

   output_conll = 0;
   output_sent = 0;
   if (bCoNLL)
      output_conll = new CCoNLLOutput[nBest];
   else
      output_sent = new CDependencyParse[nBest];

   // Read the next example
   if (bCoNLL)
      bReadSuccessful = ( (*is) >> input_conll );
   else
#ifdef JOINT_MORPH
	  bReadSuccessful = input_reader->readRawSentence(&input_sent, false, true);
#else
      bReadSuccessful = input_reader->readTaggedSentence(&input_sent, false, TAG_SEPARATOR);
#endif
   while( bReadSuccessful ) {
      if(nCount%100==0)
        std::cerr<<"Sentence " << nCount<<std::endl;
      TRACE("Sentence " << nCount);
      ++ nCount;

      // check size
      if ( (bCoNLL && input_conll.size() > depparser::MAX_SENTENCE_SIZE) ||
           (!bCoNLL && input_sent.size() > depparser::MAX_SENTENCE_SIZE) ) {
         WARNING("The sentence is longer than system limitation, skipping it.");
         for (unsigned i=0; i<nBest; ++i) {
            if (bCoNLL)
               output_conll[i].clear();
            else
               output_sent[i].clear();
            if (bScores) scores[i]=0;
         }
      }
      else {

         // Find decoder output
         if (supertags) {
            if (bCoNLL)
               supertags->setSentenceSize( input_conll.size() );
            else
               supertags->setSentenceSize( input_sent.size() );
            (*is_supertags) >> *supertags;
         }

         if (bCoNLL)
            parser->parse_conll( input_conll , output_conll , nBest , scores );
         else
            parser->parse( input_sent , output_sent , nBest , scores ) ;

      }

      // Ouptut sent
      for (unsigned i=0; i<nBest; ++i) {
         if (bCoNLL)
            os << output_conll[i];
         else
            os << output_sent[i] ;
         if (bScores) *os_scores << scores[i] << std::endl;
      }

      // Read the next example
      if (bCoNLL)
         bReadSuccessful = ( (*is) >> input_conll );
      else
#ifdef JOINT_MORPH
	  bReadSuccessful = input_reader->readRawSentence(&input_sent, false, true);
#else
      bReadSuccessful = input_reader->readTaggedSentence(&input_sent, false, TAG_SEPARATOR);
#endif
   }

   if (bCoNLL)
      delete [] output_conll;
   else
      delete [] output_sent ;
   os.close();

   if (bScores) {
      os_scores->close();
      delete os_scores;
      delete []scores;
   }

   if (bCoNLL)
      delete is;
   else
      delete input_reader;

   if (supertags) {
      delete supertags;
      is_supertags->close();
      delete is_supertags;
   }

   std::cerr << "Parsing has finished successfully. Total time taken is: " << double(clock()-time_start)/CLOCKS_PER_SEC << std::endl;
      return 1;
  }
//int main(int argc, char* argv[]) {
//
//    std::string sFeatureDBPath="model_1";
//    std::string sKnowledgePath="userdict.txt";
//    taggerObj.init(sFeatureDBPath);
//    taggerObj.loadKnowledge(sKnowledgePath);
//}

