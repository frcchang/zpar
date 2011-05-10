// Copyright (C) University of Oxford 2010
/**************************************************************** 
 *                                                              *
 * Specific implementation by agenda combined with action mt    *
 *                                                              *
 * Yue Zhang, 2007                                              *
 *                                                              *
 ****************************************************************/

#ifndef SEGMENTOR_ACTION_IMPL_H
#define SEGMENTOR_ACTION_IMPL_H

const int CHAR_CAT_BITS = 3;

#define iterate_templates(left,right) \
   left m_mapCharUnigram right\
   left m_mapCharBigram right\
   left m_mapSeenWords right\
   left m_mapLastWordByWord right\
   left m_mapWordAndNextChar right\
   left m_mapWordAndPrevChar right\
   left m_mapFirstCharLastWordByWord right\
   left m_mapLastWordByLastChar right\
   left m_mapSeparateChars right\
   left m_mapConsecutiveChars right\
   left m_mapFirstAndLastChars right\
   left m_mapOneCharWord right\
   left m_mapLengthByFirstChar right\
   left m_mapLengthByLastChar right\
   left m_mapLengthByLastWord right\
   left m_mapLastLengthByWord right\
   left m_mapLexiconWord right\
   left m_mapCharCatTrigram right

/*===============================================================
 *
 * CWeight - the definition of weights, in feature
 *
 *==============================================================*/

class CWeight {

public:
   typedef CScoreMap< CWord, SCORE_TYPE > CWordMap;
   typedef CScoreMap< CTwoWords, SCORE_TYPE > CTwoWordsMap;
   typedef CScoreMap< std::pair<CWord, int>, SCORE_TYPE > CWordIntMap;
   typedef CScoreMap< std::pair<CTwoWords, int>, SCORE_TYPE > CTwoWordsIntMap;
   typedef CScoreMap< unsigned long int, SCORE_TYPE > CIntMap;

   // char based features
   CWordIntMap m_mapCharUnigram;
   CWordIntMap m_mapCharBigram;

   // word based features
   CWordMap m_mapSeenWords;
   CTwoWordsMap m_mapLastWordByWord;
   CTwoWordsMap m_mapWordAndNextChar;
   CTwoWordsMap m_mapWordAndPrevChar;
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

   // word knowledge features
   CIntMap m_mapLexiconWord;
   CIntMap m_mapCharCatTrigram;

public:
   CWeight() : m_mapCharUnigram("CharUnigram", 65537),
               m_mapCharBigram("CharBigram", 65537),
               m_mapSeenWords("SeenWords", 65537),
               m_mapLastWordByWord("LastWordByWord", 65537),
               m_mapWordAndNextChar("WordAndNextChar", 65537),
               m_mapWordAndPrevChar("WordAndPrevChar", 65537),
               m_mapFirstCharLastWordByWord("FirstCharLastWordByWord", 65537),
               m_mapLastWordByLastChar("LastWordByLastChar", 65537),
               m_mapSeparateChars("SeparateChars", 65537),
               m_mapConsecutiveChars("ConsecutiveChars", 65537),
               m_mapFirstAndLastChars("FirstAndLastChars", 65537),
               m_mapOneCharWord("OneCharWord", 65537),
               m_mapLengthByFirstChar("LengthByFirstChar", 65537),
               m_mapLengthByLastChar("LengthByLastChar", 65537),
               m_mapLengthByLastWord("LengthByLastWord", 65537),
               m_mapLastLengthByWord("LastLengthByWord", 65537),
               m_mapLexiconWord("LexiconWord", 7), 
               m_mapCharCatTrigram("CharCatTrigram", 997) {
   }
};

/*===============================================================
 *
 * The implementation specific part of segmentor is defined here. 
 *
 *==============================================================*/

class CSegmentorImpl {
   public:
      CAgendaBeam<CStateItem> *m_Agenda;
      CSegmentorImpl() {
         m_Agenda = new CAgendaBeam<CStateItem>(BEAM_SIZE);
      }
      virtual ~CSegmentorImpl() { delete m_Agenda; }
};

//===============================================================

inline int normalizeLength(const int &length) {
   return length<LENGTH_MAX-1?length:LENGTH_MAX-1;
}

inline int encodeCharSegmentation(const bool &b, const bool &e) {
   return ( b ? 2 : 0 ) + ( e ? 1 : 0 ) ; // 00 01 10 11
}

inline int encodeCharInfoAndPosition(const int &char_info, const int &pos) {
   assert( pos >= -1 && pos <= 1 ); // pos+1 00 01 10
   return (char_info<<2) | (pos+1);
}

inline int encodeCharInfoAndType(const int &char_info, const int &charcat1, const int &charcat2, const int &charcat3) {
   assert( charcat1 >= 0 && charcat1 < (1<<CHAR_CAT_BITS) );
   assert( charcat2 >= 0 && charcat2 < (1<<CHAR_CAT_BITS) );
   assert( charcat3 >= 0 && charcat3 < (1<<CHAR_CAT_BITS) );
   return (char_info<<(CHAR_CAT_BITS*3)) | (charcat1<<(CHAR_CAT_BITS*2)) | (charcat2<<CHAR_CAT_BITS) | charcat3;
}

#endif
