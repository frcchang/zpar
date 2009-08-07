#ifndef _CHINESE_SEGMENTOR_FEAT_H
#define _CHINESE_SEGMENTOR_FEAT_H

/*===============================================================
 *
 * CFeatureHandle - handles the features for the segmentor
 *
 *==============================================================*/

class CFeatureHandle {
public:
      bool m_bCharCat; // use char cat information?
      bool m_bLexicon; // use lexicon?
      bool m_bRule; // use rules segmentation?
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
   void computeAverageFeatureWeights(int round=0) {
      cout << "Computing averaged feature scores ... ";
      iterate_templates(,.computeAverage(round););
      cout << " done" << endl;
   }

   // load the weight vectors from the database
   void loadScores() {
      cout << "Loading model ... "; cout.flush();
      ifstream is(m_sFeatureDB.c_str());
      is >> m_bCharCat >> m_bLexicon >> m_bRule;
      iterate_templates(is>>,;);
      is.close();
      m_bScoreModified = false;
      cout << "done." << endl ;
   }

   // save the weight vectors into the database
   void saveScores() {
      cout << "Saving model ... "; cout.flush();
      ofstream os(m_sFeatureDB.c_str());
      assert(os.is_open());
      os << m_bCharCat << endl << m_bLexicon << endl << m_bRule << endl;
      iterate_templates(os<<,;);
      os.close();
      m_bScoreModified = false;
      cout << "done." << endl ;
   }

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
