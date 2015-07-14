// Copyright (C) University of Oxford 2010
/***************************************************************
 *
 * The tagger's agenda n chart implementation
 *
 * Yue Zhang, 2007
 *
 ***************************************************************/

#ifndef TAGGER_AGENDACHART_IMPL_H
#define TAGGER_AGENDACHART_IMPL_H

#include "tagger_base.h"

namespace chinese {

namespace tagger {

class CSubStateItem : public CStateItem {

protected:
   SCORE_TYPE sub_score; // score of the last word

public:
   void follow(const CSubStateItem &gold) {
      assert(size()<=gold.size());
      if (size() == 0) {
         append(0, gold.getTag(0).code());
         return;
      }
      const unsigned &last_character = getWordEnd(size()-1);
      const unsigned &gold_last = gold.getWordEnd(size()-1);
      assert(last_character<=gold_last);
      if (last_character<gold_last) {
         replaceIndex(last_character+1);
         return;
      }
      else {
         assert (size() < gold.size());
         append(last_character+1, gold.getTag(size()).code());
         return;
      }
//      else {
//         const unsigned &last_character = getWordEnd(size()-1);
//         const unsigned &gold_last = gold.getWordEnd(size()-1);
//         assert(last_character<gold_last);
//         replaceIndex(last_character+1);
//         return;
//      }
   }

   double hammingloss_word(const CSubStateItem* gold) const
   {
	   int character_num_curstate = charactersize();
	   int character_num_gold = gold->charactersize();
	   assert(character_num_curstate == character_num_gold);
	   if(character_num_curstate <= 0) return 0;

	   double loss = 0.0;
	   for(int i = 0; i < character_num_gold; i++)
	   {

		   int charTag_cur = 1;
		   int wordIndex = 0;
		   while(m_lWords[wordIndex] < i && wordIndex < m_nLength)
		   {
			   wordIndex++;
		   }

		   if( i == 0 || m_lWords[wordIndex-1]+1 == i)
		   {
			   charTag_cur = 2;
		   }


		   int charTag_gold = 1;

		   wordIndex = 0;
		   while(gold->m_lWords[wordIndex] < i && wordIndex < gold->m_nLength)
		   {
			   wordIndex++;
		   }

		   if( i == 0 || gold->m_lWords[wordIndex-1]+1 == i)
		   {
			   charTag_gold = 2;
		   }


		   if(charTag_cur != charTag_gold)
		   {
			   loss = loss +1;
		   }

	   }

	   return loss;
   }



   // hammingloss tagging
   double hammingloss_tag(const CSubStateItem* gold) const
   {
	   return hammingloss_wordtag(gold) - hammingloss_word(gold);
   }



   // hammingloss wordsegmentation and tagging

   double hammingloss_wordtag(const CSubStateItem* gold) const
   {
	   int character_num_curstate = charactersize();
	   int character_num_gold = gold->charactersize();
	   assert(character_num_curstate == character_num_gold);
	   if(character_num_curstate <= 0) return 0;

	   double loss = 0.0;
	   for(int i = 0; i < character_num_gold; i++)
	   {
		   int charTag_cur = -1;
		   int wordIndex = 0;
		   while(m_lWords[wordIndex] < i && wordIndex < m_nLength)
		   {
			   wordIndex++;
		   }

		   if( i == 0 || m_lWords[wordIndex-1]+1 == i)
		   {
			   charTag_cur = 2 * m_lTags[wordIndex].code();
		   }
		   else
		   {
			   charTag_cur = 2 * m_lTags[wordIndex].code() +1;
		   }


		   int charTag_gold = -1;

		   wordIndex = 0;
		   while(gold->m_lWords[wordIndex] < i && wordIndex < gold->m_nLength)
		   {
			   wordIndex++;
		   }

		   if( i == 0 || gold->m_lWords[wordIndex-1]+1 == i)
		   {
			   charTag_gold = 2 * gold->m_lTags[wordIndex].code();
		   }
		   else
		   {
			   charTag_gold = 2 * gold->m_lTags[wordIndex].code() + 1;
		   }

		   if( charTag_cur != charTag_gold )
		   {
			   loss = loss +1;
		   }

	   }

	   return loss;
   }


   double hammingloss_word_prec_recal(const CSubStateItem* gold) const
   {
	   int character_num_curstate = charactersize();
	   int character_num_gold = gold->charactersize();
	   assert(character_num_curstate == character_num_gold);
	   if(character_num_curstate <= 0) return 0;

	   double loss = 0.0;

	   int match = 0;
	   int total = m_nLength + gold->m_nLength;
	   int last_pStart = 0, last_gStart = 0;
	   for(int i = 0, j = 0; i < m_nLength && j < gold->m_nLength; )
	   {
		   if(m_lWords[i] == gold->m_lWords[j] && last_pStart == last_gStart)
		   {
			   last_pStart = m_lWords[i]+1;
			   last_gStart = gold->m_lWords[j]+1;
			   i++;j++;
			   match++;
		   }
		   else if (m_lWords[i] == gold->m_lWords[j])
		   {
			   last_pStart = m_lWords[i]+1;
			   last_gStart = gold->m_lWords[j]+1;
			   i++;j++;
		   }
		   else if (m_lWords[i] < gold->m_lWords[j])
		   {
			   last_pStart = m_lWords[i]+1; i++;
		   }
		   else
		   {
			   last_gStart = gold->m_lWords[j]+1;j++;
		   }

	   }

	   loss = total - 2.0 * match;

	   return loss;
   }



   // hammingloss tagging
   double hammingloss_tag_prec_recal(const CSubStateItem* gold) const
   {
	   return hammingloss_wordtag_prec_recal(gold) - hammingloss_word_prec_recal(gold);
   }



   // hammingloss wordsegmentation and tagging

   double hammingloss_wordtag_prec_recal(const CSubStateItem* gold) const
   {
	   int character_num_curstate = charactersize();
	   int character_num_gold = gold->charactersize();
	   assert(character_num_curstate == character_num_gold);
	   if(character_num_curstate <= 0) return 0;

	   double loss = 0.0;

	   int match = 0;
	   int total = m_nLength + gold->m_nLength;
	   int last_pStart = 0, last_gStart = 0;
	   for(int i = 0, j = 0; i < m_nLength && j < gold->m_nLength; )
	   {
		   if(m_lWords[i] == gold->m_lWords[j] && last_pStart == last_gStart)
		   {
			   last_pStart = m_lWords[i]+1;
			   last_gStart = gold->m_lWords[j]+1;
			   i++;j++;
			   if(m_lTags[i]==gold->m_lTags[j])match++;
		   }
		   else if (m_lWords[i] == gold->m_lWords[j])
		   {
			   last_pStart = m_lWords[i]+1;
			   last_gStart = gold->m_lWords[j]+1;
			   i++;j++;
		   }
		   else if (m_lWords[i] < gold->m_lWords[j])
		   {
			   last_pStart = m_lWords[i]+1; i++;
		   }
		   else
		   {
			   last_gStart = gold->m_lWords[j]+1;j++;
		   }

	   }

	   loss = total - 2.0 * match;

	   return loss;
   }


};

}

/*===============================================================
 *
 * The implementation-specific part of tagger is defined here.
 *
 *==============================================================*/

class CTagger : public CTaggerBase {

protected:
   CAgendaBeam<tagger::CSubStateItem> m_Agenda;
   CWordCache m_WordCache;
   tagger::CSubStateItem m_goldState;
   unsigned m_nScoreIndex;
   bool m_bTrainingError;

   tagger::CWeight *m_delta_word;
   tagger::CWeight *m_delta_tag;

public:
   CTagger(const std::string &sFeatureDBPath, bool bTrain, unsigned long nMaxSentSize, const std::string &sKnowledgePath, bool bSegmentationRules) : m_Agenda(tagger::AGENDA_SIZE) , CTaggerBase(sFeatureDBPath, bTrain, nMaxSentSize, sKnowledgePath, bSegmentationRules) , m_WordCache(nMaxSentSize), m_delta_word(0), m_delta_tag(0){
      if (bTrain)
      {
    	  m_nScoreIndex = CScore<tagger::SCORE_TYPE>::eNonAverage;
    	  m_delta_word = new tagger::CWeight("", true, false, 257);
    	  m_delta_tag = new tagger::CWeight("", true, false, 257);
      }
      else m_nScoreIndex = CScore<tagger::SCORE_TYPE>::eAverage;
      ASSERT(sizeof(unsigned long long)>=CTag::SIZE, "The tagger requires the size of unsigned-long greater than" << CTag::SIZE); // tag dict
   }
   virtual ~CTagger() {
	   if(m_delta_word) delete m_delta_word;
	   if(m_delta_tag) delete m_delta_tag;
   }

protected:
   void loadKnowledge(const std::string &sKnowledgePath) {
      std::cerr << "Knowledge is provided but not used." << std::endl;
//      std::cout << "Loading knowledge ... ";
//      m_weights->newKnowledge();
//      std::ifstream ifs(sKnowledgePath.c_str());
//      if (!ifs) THROW("Knowledge file " << sKnowledgePath << " is not accessible.");
//      ifs >> (*m_weights->m_Knowledge);
//      ifs.close();
//      std::cout << "done." << std::endl;
   }
   inline bool canAssignTag(const CWord &word, const unsigned long &tag) {
      return ( m_weights->m_mapWordFrequency.find( word, 0 ) <
                  m_weights->m_nMaxWordFrequency/5000+5 &&
               PENN_TAG_CLOSED[ tag ] == false  ) ||
             m_weights->m_mapTagDictionary.lookup( word, tag );
   }
   inline bool canStartWord(const CStringVector &sentence, const unsigned long &tag, const unsigned long &index) {
      if (PENN_TAG_CLOSED[ tag ] || tag == PENN_TAG_CD ) {
         static int tmp_i;
         // if the first character doesn't match, don't search
         if ( m_weights->m_mapCanStart.lookup( m_WordCache.find( index, index, &sentence ), tag ) == false)
            return false;
         // if it matches, search from the next characters
         if ( tag == PENN_TAG_CD ) return true; // don't search for CD assume correct
         for (tmp_i=0; tmp_i<m_weights->m_maxLengthByTag[tag]; ++tmp_i) {
            if ( m_weights->m_mapTagDictionary.lookup( m_WordCache.find( index, std::min(index+tmp_i, static_cast<unsigned long>(sentence.size())-1), &sentence ), tag ) )
               return true;
         }
         return false;
      }
      return true;
   }

protected:
   virtual void work(const CStringVector *sentence, CTwoStringVector *retval, double *out_scores=NULL, unsigned long nBest=1, const CBitArray *prunes=NULL);

public:
   enum SCORE_UPDATE {eSubtract=0, eAdd};

   virtual bool train(const CStringVector *sentence, const CTwoStringVector *correct);
   virtual void tag(const CStringVector *sentence, CTwoStringVector *retval, double *out_scores=NULL, unsigned long nBest=1, const CBitArray *prunes=NULL);

   void finishTraining(unsigned long nTotalNumberOfTrainingExamples) {
      m_weights->computeAverageFeatureWeights(nTotalNumberOfTrainingExamples);
      m_weights->saveScores();
   }

public:
   tagger::SCORE_TYPE getGlobalScore(const CTwoStringVector* tagged) {
      THROW("This method is not supported in this implementation.");
   }
   void updateScores(const CTwoStringVector* tagged, const CTwoStringVector* correct, unsigned long round) {
      THROW("This method is not supported in this implementation.");
   }
   tagger::SCORE_TYPE getOrUpdateLocalScore(const CStringVector *tagged, const tagger::CSubStateItem *item, unsigned long index, tagger::SCORE_TYPE amount=0, unsigned long round=0) {
      THROW("This method is not supported in this implementation.");
   }

   inline void updateScoreForState(const CStringVector *sentence, const tagger::CSubStateItem *item, const tagger::SCORE_TYPE &amount) {
      static unsigned tmp_i, tmp_j;
      static tagger::SCORE_TYPE scorew, scoret;
      for (tmp_i=0; tmp_i<item->size(); ++tmp_i) {
         getOrUpdateSeparateScore(sentence, item, tmp_i, scorew, scoret, amount, m_nTrainingRound);
         for (tmp_j=item->getWordStart(tmp_i)+1; tmp_j<item->getWordEnd(tmp_i)+1; ++tmp_j)
            getOrUpdateAppendScore(sentence, item, tmp_i, tmp_j, scorew, scoret, amount, m_nTrainingRound);
      }
   }

   inline void updateScoreForStates(const CStringVector *sentence, const tagger::CSubStateItem *gold, const tagger::CSubStateItem *item)
   {
	   m_delta_word->clear();
	   m_delta_tag->clear();

	   updateScoreForState(sentence, gold, 1);
	   updateScoreForState(sentence, item, -1);


	   {
		   static double lossw, losst, fov;
		   lossw = item->hammingloss_word_prec_recal(gold);
		   losst = item->hammingloss_wordtag_prec_recal(gold);

		   //may be gold->scorew > item->scorew but gold->score < item->score, similar for tag score.
		   //if(item->scorew - gold->scorew > -1e-20)
		   double m_delta_word_norm = m_delta_word->norm2();
		   if (m_delta_word_norm > 1e-20)
		   {
			   fov = (item->scorew - gold->scorew + std::sqrt(lossw))/(m_delta_word_norm + 50.0);
			   m_delta_word->scaleCurrent(fov, m_nTrainingRound);
			   m_weights->addCurrent(m_delta_word, m_nTrainingRound);
		   }


		  //if(item->scoret - gold->scoret > -1e-20)
		   double m_delta_tag_norm = m_delta_tag->norm2();
		   if(m_delta_tag_norm > 1e-20)
		   {
			   fov = (item->scoret - gold->scoret + std::sqrt(losst))/(m_delta_tag_norm + 50.0);
			   m_delta_tag->scaleCurrent(fov, m_nTrainingRound);
			   m_weights->addCurrent(m_delta_tag, m_nTrainingRound);
		   }
   	   }

	   /*
	   {
		   m_weights->addCurrent(m_delta_word, m_nTrainingRound);
		   m_weights->addCurrent(m_delta_tag, m_nTrainingRound);
	   }*/


   }
   tagger::SCORE_TYPE getOrUpdateSeparateScore(const CStringVector *tagged, const tagger::CSubStateItem *item, unsigned long index, tagger::SCORE_TYPE& scorew, tagger::SCORE_TYPE& scoret, tagger::SCORE_TYPE amount=0, unsigned long round=0);
   tagger::SCORE_TYPE getOrUpdateAppendScore(const CStringVector *tagged, const tagger::CSubStateItem *item, unsigned long index, unsigned long char_index, tagger::SCORE_TYPE& scorew, tagger::SCORE_TYPE& scoret, tagger::SCORE_TYPE amount=0, unsigned long round=0);

};

} // namespace chinese





#endif
