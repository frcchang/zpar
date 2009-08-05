#ifndef _CHINESE_SEGMENTOR_FEAT_H
#define _CHINESE_SEGMENTOR_FEAT_H

/*===============================================================
 *
 * CFeatureHandle - handles the features for the segmentor
 *
 *==============================================================*/

class CFeatureHandle {
   private:
      //
      // PRIVATE MEMBER VARIABLES
      //
      bool m_bScoreModified; // scores modified in memory but has not been committed to database
      string m_sFeatureDB;   // the name of the feature database, currently sqlitedb
      CWeight m_weights;
      const CScore<SCORE_TYPE> m_zeroScore;
      CSegmentor* m_parent;
   public:
      //
      // CONSTRUCTOR AND DESTRUCTOR METHODS
      // 
      CFeatureHandle(CSegmentor* pSegmentor, string sFileName, bool bTrain=false) : m_parent(pSegmentor), m_zeroScore() { m_sFeatureDB = sFileName; m_bTrain = bTrain; loadScores(); } 
      ~CFeatureHandle() { if (m_bScoreModified) saveScores(); }
   public:
      //
      // PUBLIC INTERFACES
      //
      // two score updating methods
      enum SCORE_UPDATE {eAdd=0, eSubtract};
      // get the global score for a segmented sentence or section
      SCORE_TYPE getGlobalScore(const CSentenceRaw* sentence, const CStateItem* item);
      SCORE_TYPE getLocalScore(const CSentenceRaw* sentence, const CStateItem* item, int index);
#ifdef NO_NEG_FEATURE
      // collect the set of features from data
      void extractPosFeatures(const CSentenceRaw *sent);
#endif
      // update the built-in weight vector for this feature object specifically
      void updateScoreVector(const CSentenceRaw* output, const CSentenceRaw* correct, int round=0);
      // compute the total or average feature vector after update
      void computeAverageFeatureWeights(int round=0);
      // load the weight vectors from the database
      void loadScores();
      void loadScoreFor(string sFeatureDB, int which);
      // save the weight vectors into the database
      void saveScores();
      void saveScoreFor(string sFeatureDB, int which);
      // cached words and tags
      inline bool isWordKnown(const CWord &word) { return m_weights.m_mapSeenWords.element(word); }
      CWeight &getWeights() { return m_weights; }
      bool m_bTrain;
   private:
      //
      // PRIVATE METHODS
      //
      // add local features to a global feature vector (first param)
      void updateLocalFeatureVector(SCORE_UPDATE method, const CSentenceRaw* output, int index, int round=0);
};

#endif
