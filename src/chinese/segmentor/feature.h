// Copyright (C) University of Oxford 2010
#ifndef _CHINESE_SEGMENTOR_FEAT_H
#define _CHINESE_SEGMENTOR_FEAT_H

/*===============================================================
 *
 * CFeatureHandle - handles the features for the segmentor
 *
 *==============================================================*/

class CFeatureHandle {
public:
   bool m_bRule; // use rules segmentation?
   CWordDictionary *m_CharCat;
   CLexiconSet *m_WordLst;
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
   CFeatureHandle(CSegmentor* pSegmentor, string sFileName, bool bTrain, bool bRules) : m_parent(pSegmentor), m_sFeatureDB(sFileName), m_bTrain(bTrain), m_CharCat(0), m_WordLst(0), m_zeroScore(), m_bRule(bRules) { loadScores(); } 
   ~CFeatureHandle() { 
      if (m_bScoreModified) saveScores(); 
      if (m_CharCat) { delete m_CharCat; }
      if (m_WordLst) { delete m_WordLst; }
   }
public:
   //
   // PUBLIC INTERFACES
   //
   // two score updating methods
   enum SCORE_UPDATE {eAdd=0, eSubtract};
   // get the global score for a segmented sentence or section
   SCORE_TYPE getGlobalScore(const CStringVector* sentence, const CStateItem* item);
   SCORE_TYPE getLocalScore(const CStringVector* sentence, const CStateItem* item, int index);
#ifdef NO_NEG_FEATURE
   // collect the set of features from data
   void extractPosFeatures(const CStringVector *sent);
#endif
   // update the built-in weight vector for this feature object specifically
   void updateScoreVector(const CStringVector* output, const CStringVector* correct, int round=0);
   // compute the total or average feature vector after update
   void computeAverageFeatureWeights(int round=0) {
      cout << "Computing averaged feature scores ... ";
      iterate_templates(,.computeAverage(round););
      cout << "done" << endl;
   }

   // load the weight vectors from the database
   void loadScores() {
      // initialize
      clock_t time_start = clock();
      string line;
      cout << "Loading model ... "; cout.flush();
      ifstream is(m_sFeatureDB.c_str());
      if (!is.is_open()) { 
         cout << "empty."<<endl; 
         iterate_templates(,.init(););
         return; 
      }
      // use char cat information?
      bool bCharCat; 
      getline(is, line);
      istringstream(line) >> bCharCat;
      if (bCharCat) {
         cout << "loading charcat ... "; cout.flush();
         if (m_CharCat==0) {
            m_CharCat = new CWordDictionary(2719);
            is >> static_cast<CWordDictionary&>(*m_CharCat); //is
         } 
         else THROW("CSegmentor feature: m_CharCat already loaded.");
      }
      // use lexicon?
      bool bLexicon;
      getline(is, line);
      istringstream(line) >> bLexicon;
      if (bLexicon) {
         cout << "loading lexicon ... "; cout.flush();
         if (m_WordLst==0) {
            m_WordLst = new CLexiconSet;
            is >> (*m_WordLst); //is
         } 
         else THROW("CSegmentor feature: m_WordLst already loaded.");
      }
      // use rules?
      getline(is, line);
      istringstream(line) >> m_bRule;
      // load features by iterating templates defined by implementation
      iterate_templates(is>>,;);
      // finalize
      is.close();
      m_bScoreModified = false;
      cout << "done (" << double(clock()-time_start)/CLOCKS_PER_SEC << "s)." << endl ;
   }

   // save the weight vectors into the database
   void saveScores() {
      // initialization
      cout << "Saving model ... "; cout.flush();
      ofstream os(m_sFeatureDB.c_str());
      assert(os.is_open());
      // save charcat
      os << (m_CharCat?1:0) << endl;
      if (m_CharCat) os << static_cast<CWordDictionary &>(*m_CharCat);
      // save lexicon
      os << (m_WordLst?1:0) << endl; 
      if (m_WordLst) os << (*m_WordLst);
      // save rule
      os << m_bRule << endl;
      // save features
#ifdef DEBUG
      iterate_templates(,.trace(););
#endif
      iterate_templates(os<<,;);
      // finalization
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
   void updateLocalFeatureVector(SCORE_UPDATE method, const CStringVector* output, int index, int round=0);
};

#endif
