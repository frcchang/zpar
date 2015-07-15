// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                              *
 * segmentor.h - the sentence segmentor.                        *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2006.10                        *
 *                                                              *
 ****************************************************************/

#ifndef CHINESE_SEGMENTOR_H
#define CHINESE_SEGMENTOR_H 1

#include "base_include.h"

/*===============================================================
 *
 * Global items
 *
 *==============================================================*/

namespace chinese {

class CSegmentor;

namespace segmentor {

#include "feature.h"

} // namespace segmentor

/*===============================================================
 *
 * CSegmentor - the segmentor for Chinese
 *
 * Segment a sentence character by character, generating possible
 * output sentences into a beam agenda.
 *
 * Given an input sentence, m_lWordCache caches the std::string words
 * in certain positions.
 * For example: suppose that a sentence is
 *              A B C D E F
 *              m_lWordCache[0][0] = A
 *              m_lWordCache[0][4] = A B C D E
 *              m_lWordCache[1][5] = B C D E F
 * Note: there are space wastes, but this seem to be the most time
 *       efficient.
 *
 *==============================================================*/

class CSegmentor : public segmentor::CSegmentorImpl {

private:
   unsigned long *m_lWordCache;
   bool m_bTrain;
   segmentor::CFeatureHandle *m_Feature;

//-------------------------------------------------------------
// Constructor destructor
public:
   CSegmentor(const std::string &sFeatureDBPath, bool bTrain=false, const std::string &sCharCatFile="", const std::string &sLexiconFile="", bool bRule=false) : segmentor::CSegmentorImpl(), m_bTrain(bTrain) {
      // load features
      m_Feature = new segmentor::CFeatureHandle(this, sFeatureDBPath, bTrain, bRule);
      // initialize word cache
      m_lWordCache = new unsigned long[segmentor::MAX_SENTENCE_SIZE*segmentor::MAX_SENTENCE_SIZE];
      // load knowledge
      if (!bTrain) {
         if (!sCharCatFile.empty())
            THROW("CSegmentor::CSegmentor received sCharCat file in decoding mode, which is unexpected.");
         if (!sLexiconFile.empty())
            THROW("CSegmentor::CSegmentor received sLexicon file in decoding mode, which is unexpected.");
      }
      else {
         // first time training
         if (FileExists(sFeatureDBPath)==false) {
            // define items for new db
            if (!sCharCatFile.empty()) loadCharCat(sCharCatFile);
            if (!sLexiconFile.empty()) loadLexiconDict(sLexiconFile);
         }
         else { // model already exists
            if (!sCharCatFile.empty())
               THROW("CSegmentor::CSegmentor received sCharCat file, but model exists (with charcat)");
            if (!sLexiconFile.empty())
               THROW("CSegmentor::CSegmentor received sLexicon file, but it is useful only during first-time training, where model does not exist");
         }
      }
   }
   virtual ~CSegmentor() {
      delete m_Feature;
      delete [] m_lWordCache;
   }
   CSegmentor(CSegmentor& segmentor) { THROW("CSegmentor does not support copy constructor!"); }

//-------------------------------------------------------------
// Main interface
public:
   void train(const CStringVector* sentence, const CStringVector *correct, int &round);
   void segment(const CStringVector* sentence, CStringVector *retval, double *out_scores=NULL, int nBest=1);

   bool isTraining() const { return m_bTrain; }

#ifdef NO_NEG_FEATURE

   void extractPosFeatures( const CStringVector *sent ) {
      this->m_Feature->extractPosFeatures(sent);
   }

#endif

   void updateScores(const CStringVector* output, const CStringVector* correct, int round) {
      m_Feature->updateScoreVector(output, correct, round);
   }
   void finishTraining(int round) {
      m_Feature->computeAverageFeatureWeights(round);
      m_Feature->saveScores();
   }

   segmentor::CWeight &getWeights() { return m_Feature->getWeights(); }

//-------------------------------------------------------------
// Knowledge related
public:
   void loadCharCat(const std::string &sFile) {
      if (m_Feature->m_CharCat==0) {
         std::ifstream is(sFile.c_str());
         if (!is.is_open())
            THROW("the file " << sFile << " is unavailable.");
         m_Feature->m_CharCat = new CWordDictionary(2719);
         is >> static_cast<CWordDictionary&>(*m_Feature->m_CharCat);
         is.close();
         return;
      }
      THROW("CSegmentor: loadCharCat called multiple times.");
   }
   void loadLexiconDict(const std::string &sFile) {
      if (m_Feature->m_WordLst==0) {
         m_Feature->m_WordLst = new CLexiconSet;
         std::ifstream is(sFile.c_str());
         if (!is.is_open()) THROW("the file " << sFile << " is unavailable.");
         is >> (*m_Feature->m_WordLst);
         is.close();
         return;
      }
      THROW("CSegmentor: loadLexiconDict called twice.");
   }

   bool wordInLexicon(const CWord &word) {
      if (m_Feature->m_WordLst==0)
         return false;
      return m_Feature->m_WordLst->find(word, 0); // non-zero value means member
   }
   bool hasCharTypeKnowledge() {
      return m_Feature->m_CharCat != 0;
   }
   unsigned charType(const CWord &ch) {
      return m_Feature->m_CharCat->lookup(ch);
   }

//-------------------------------------------------------------
// Word cache
public:
   const unsigned long& findWordFromCache(const int &start, const int &length, const CStringVector* sentence) {
      if (m_lWordCache[ start * segmentor::MAX_SENTENCE_SIZE + length - 1 ] == ~0L) { // empty std::string
         static std::string temp;
         static unsigned long int i;
         temp.clear();
         for ( i = start; i < start+length ; ++i )
            temp += sentence->at(i) ;
         m_lWordCache[ start * segmentor::MAX_SENTENCE_SIZE + length - 1 ] = CWord(temp, false).code();
      }
      return m_lWordCache[ start * segmentor::MAX_SENTENCE_SIZE + length - 1 ];
   }
   const unsigned long& replaceWordToCache(const int &start, const int &length, const CStringVector* sentence) {
      assert(start+length<=sentence->size());
      if (m_lWordCache[ start * segmentor::MAX_SENTENCE_SIZE + length - 1 ] == ~0L || m_lWordCache[ start * segmentor::MAX_SENTENCE_SIZE + length - 1 ] == CWord::UNKNOWN) { // empty std::string
         static std::string temp;
         static unsigned long int i;
         temp.clear();
         for ( i = start; i < start+length ; ++i )
            temp += sentence->at(i) ;
         m_lWordCache[ start * segmentor::MAX_SENTENCE_SIZE + length - 1 ] = CWord(temp).code();
      }
      return m_lWordCache[ start * segmentor::MAX_SENTENCE_SIZE + length - 1 ];
   }
   void clearWordCache() {
      memset(m_lWordCache, 255, sizeof(m_lWordCache[0])*segmentor::MAX_SENTENCE_SIZE*segmentor::MAX_SENTENCE_SIZE);
//      for (int i=0; i<segmentor::MAX_SENTENCE_SIZE; ++i)
//         for (int j=0; j<segmentor::MAX_SENTENCE_SIZE; j++)
//            m_lWordCache[i*segmentor::MAX_SENTENCE_SIZE+j].clear();
   }
};

} // namespace chinese

#endif


