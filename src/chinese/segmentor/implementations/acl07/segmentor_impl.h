// Copyright (C) University of Oxford 2010
/**************************************************************** 
 *                                                              *
 * Specific implementation by agenda                            *
 *                                                              *
 * Yue Zhang, 2007                                              *
 *                                                              *
 ****************************************************************/

#ifndef SEGMENTOR_AGENDA_IMPL_H
#define SEGMENTOR_AGENDA_IMPL_H

#define iterate_templates(left, right)\
        left m_mapSeenWords right\
        left m_mapLastWordByWord right\
        left m_mapLastWordFirstChar right\
        left m_mapCurrentWordLastChar right\
        left m_mapFirstCharLastWordByWord right\
        left m_mapLastWordByLastChar right\
        left m_mapSeparateChars right\
        left m_mapConsecutiveChars right\
        left m_mapFirstAndLastChars right\
        left m_mapOneCharWord right\
        left m_mapLengthByFirstChar right\
        left m_mapLengthByLastChar right\
        left m_mapLengthByLastWord right\
        left m_mapLastLengthByWord right

/*===============================================================
 *
 * CWeight - the definition of weights, in feature
 *
 *==============================================================*/

class CWeight {

   typedef CScoreMap< CWord, SCORE_TYPE > CWordMap;
   typedef CScoreMap< CTwoWords, SCORE_TYPE > CTwoWordsMap;
   typedef CScoreMap< std::pair<CWord, int>, SCORE_TYPE > CWordIntMap;
   typedef CScoreMap< std::pair<CTwoWords, int>, SCORE_TYPE > CTwoWordsIntMap;
   typedef CScoreMap< unsigned long int, SCORE_TYPE > CIntMap;

public:
   CWordMap m_mapSeenWords;
   CTwoWordsMap m_mapLastWordByWord;
   CTwoWordsMap m_mapLastWordFirstChar;
   CTwoWordsMap m_mapCurrentWordLastChar;
   CTwoWordsMap m_mapFirstCharLastWordByWord;
   CTwoWordsMap m_mapLastWordByLastChar;
   CWordMap m_mapSeparateChars;
   CWordMap m_mapConsecutiveChars;
   CTwoWordsMap m_mapFirstAndLastChars;
   CWordMap m_mapOneCharWord;
   CWordIntMap m_mapLengthByFirstChar;
   CWordIntMap m_mapLengthByLastChar;
   CWordIntMap m_mapLengthByLastWord;
   CWordIntMap m_mapLastLengthByWord;

public:
   CWeight() : m_mapSeenWords("SeenWords", 65537) ,
               m_mapLastWordByWord("LastWordByWord", 65537) ,
               m_mapLastWordFirstChar("LastWordFirstChar", 65537) ,
               m_mapCurrentWordLastChar("CurrentWordLastChar", 65537) ,
               m_mapFirstCharLastWordByWord("FirstCharLastWordByWord", 65537) ,
               m_mapLastWordByLastChar("LastWordByLastChar", 65537) ,
               m_mapSeparateChars("SeparateChars", 65537) ,
               m_mapConsecutiveChars("ConsecutiveChars", 65537) ,
               m_mapFirstAndLastChars("FirstAndLastChars", 65537) ,
               m_mapOneCharWord("OneCharWord", 65537) ,
               m_mapLengthByFirstChar("LengthByFirstChar", 65537) ,
               m_mapLengthByLastChar("LengthByLastChar", 65537) ,
               m_mapLengthByLastWord("LengthByLastWord", 65537) ,
               m_mapLastLengthByWord("LastLengthByWord", 65537)
   { /* do nothing */ }
};

/*===============================================================
 *
 * The implementation specific part of segmentor is defined here. 
 *
 *==============================================================*/

class CSegmentorImpl {
   protected:
      CAgendaBeamDoubleIndice<CStateItem> *m_Agenda;
   public:
      CSegmentorImpl() {
         m_Agenda = new CAgendaBeamDoubleIndice<CStateItem>(BEAM_SIZE);
      }
      virtual ~CSegmentorImpl() { delete m_Agenda; }
};

#endif
