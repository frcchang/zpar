/****************************************************************
 *                                                              *
 * deplabeler.h - the definitions for the dependency labeler.   *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2007.8                         *
 *                                                              *
 ****************************************************************/

#ifndef _DEPLABELER_IMPL_H
#define _DEPLABELER_IMPL_H 

#include "deplabeler_base.h"

/*===============================================================
 *
 * Global items
 *
 *==============================================================*/

#include "weight.h"

namespace TARGET_LANGUAGE {

/*===============================================================
 *
 * CDepLabeler - the dependency labeler for English 
 *
 *==============================================================*/

class CDepLabeler : public CDepLabelerBase {

private:

   vector< CTaggedWord<CTag, TAG_SEPARATOR> > m_lCache;
   int m_nTrainingRound;
   int m_nTotalErrors;
   bool m_bScoreModified;
   int m_nScoreIndex;

public:
   // constructor and destructor
   CDepLabeler( const string &sFeatureDBPath , bool bTrain ) : CDepLabelerBase(sFeatureDBPath, bTrain) { 
      m_weights = new deplabeler :: CWeight(sFeatureDBPath, bTrain );
      m_nTrainingRound = 0; 
      m_nTotalErrors = 0;
      if (bTrain) m_nScoreIndex = CScore<deplabeler::SCORE_TYPE>::eNonAverage ; else m_nScoreIndex = CScore<deplabeler::SCORE_TYPE>::eAverage ;
   }
   ~CDepLabeler() {
      delete m_weights;
   }
   CDepLabeler( CDepLabeler &deplabeler) : CDepLabelerBase(deplabeler) { 
      THROW("Unsupported constructor");;
   }

public:
   void label( const CDependencyTree &sentence , CLabeledDependencyTree *retval , int nBest=1 , deplabeler::SCORE_TYPE *scores=0 ) ;
   void train( const CLabeledDependencyTree &correct ) ;

   void finishtraining() {
      static_cast<deplabeler::CWeight*>(m_weights)->computeAverageFeatureWeights(m_nTrainingRound);
      static_cast<deplabeler::CWeight*>(m_weights)->saveScores();
      cout << "Total number of training errors are: " << m_nTotalErrors << endl;
   }

private:
   enum SCORE_UPDATE {eAdd=0, eSubtract};

   void train_link( const CLabeledDependencyTree &correct , const unsigned long &index ) ;

   void work( const CDependencyTree &sentence , CLabeledDependencyTree *retval, const CLabeledDependencyTree *correct, const unsigned long &index ) ; 

   inline deplabeler::SCORE_TYPE getOrUpdateArcLabelScore( const int &head_index, const int &dep_index, const unsigned long &label, deplabeler::SCORE_TYPE amount=0, int round=0 );

};

}; // namespace TARGET_LANGUAGE

#endif


