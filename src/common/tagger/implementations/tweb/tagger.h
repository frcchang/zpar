/****************************************************************
 *                                                              *
 * tagger.h - Interface to Zpar for TWeb POS tagger.            *
 *                                                              *
 * Author: Jia Zhongye                                          *
 *                                                              *
 * SUTD, 2014.06                                                *
 *                                                              *
 ****************************************************************/

#ifndef _COMMON_TAGGER_H
#define _COMMON_TAGGER_H

#include "tagger_impl.h"
#include "tagger_include.h"

/*===============================================================
 *
 * Global items
 *
 *==============================================================*/

#include "knowledge/tagdict.h" 
#include "tweb_wrapper.h"

namespace TARGET_LANGUAGE {

  /*===============================================================
   *
   * CTagger - the tagger wrapper class
   *
   *
   *
   *==============================================================*/

  class CTagger {

    private:

      bool m_bTrain;              // whether training
      int m_nTrainingRound;       // the number of sentence

      unsigned m_nMaxSentenceSize;

      _tweb_impl::_tweb_wrapper * m_tweb;

    public:
      CTagger(const std::string &sFeatureDBPath, bool bTrain=false)
        : m_tweb(_tweb_impl::_init_wrapper()),
          m_bTrain(bTrain)
      {
        _tweb_impl::_load_model( m_tweb, sFeatureDBPath);
      }
      ~CTagger() { 
        _tweb_impl::_del_tagger( m_tweb);
      }
      CTagger(CTagger& tagger) { ASSERT(1==0, "No copy constructor."); }

    public:
      void loadTagDictionary(const std::string &sTagDictPath) {
      }

      void loadKnowledge(const std::string &sKnowledge) {
      }

    public:
      bool train(const CTwoStringVector *correct);
      void finishTraining()
      {}
      void tag(CStringVector *sentence, CTwoStringVector *retval, int nBest=1, double *out_scores=NULL)
      {
        _tweb_impl::_tag(m_tweb, sentence , retval, nBest, out_scores);
      }
  };
}; // namespace TARGET_LANGUAGE

#endif


