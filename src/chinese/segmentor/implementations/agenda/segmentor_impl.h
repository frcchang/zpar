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

const int CHAR_TYPE_SIZE = 4;
const unsigned DEFAULT_TABLE_SIZE = 65536;

#define iterate_templates(left,right) \
   left m_mapCharUnigram right\
   left m_mapCharBigram right\
   left m_mapCharTrigram right\
   left m_mapSeenWords right\
   left m_mapLastWordByWord right\
   left m_mapWordAndNextChar right\
   left m_mapWordAndPrevChar right\
   left m_mapFirstCharLastWordByWord right\
   left m_mapLastWordByLastChar right\
   left m_mapSeparateChars right\
   left m_mapConsecutiveChars right\
   left m_mapFirstAndLastChars right\
   left m_mapFirstCharAndChar right\
   left m_mapOneCharWord right\
   left m_mapLengthByFirstChar right\
   left m_mapLengthByLastChar right\
   left m_mapLengthByLastWord right\
   left m_mapLastLengthByWord right\
   left m_mapLexiconWord right\
   left m_mapCharCatUnigram right\
   left m_mapCharCatBigram right\
   left m_mapCharCatTrigram right

/*===============================================================
 *
 * CWeight - the definition of weights, in feature
 *
 *==============================================================*/

class CWeight {

public:
   typedef CScoreMapType< CWord, SCORE_TYPE > CWordMap;
   typedef CScoreMapType< CTwoWords, SCORE_TYPE > CTwoWordsMap;
   typedef CScoreMapType< std::pair<int, int>, SCORE_TYPE > CPairIntMap;
   typedef CScoreMapType< std::pair<CWord, int>, SCORE_TYPE > CWordIntMap;
   typedef CScoreMapType< std::pair<CTwoWords, int>, SCORE_TYPE > CTwoWordsIntMap;
   typedef CScoreMapType< unsigned long int, SCORE_TYPE > CIntMap;

   // char based features
   CWordIntMap m_mapCharUnigram;
   CWordIntMap m_mapCharBigram;
   CWordIntMap m_mapCharTrigram;

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
   CTwoWordsMap m_mapFirstCharAndChar;
   CWordMap m_mapOneCharWord;
   CWordIntMap m_mapLengthByFirstChar;
   CWordIntMap m_mapLengthByLastChar;
   CWordIntMap m_mapLengthByLastWord;
   CWordIntMap m_mapLastLengthByWord;

   // word knowledge features
   CPairIntMap m_mapLexiconWord;
   CPairIntMap m_mapCharCatUnigram;
   CPairIntMap m_mapCharCatBigram;
   CPairIntMap m_mapCharCatTrigram;

public:
   CWeight() : m_mapCharUnigram("CharUnigram", DEFAULT_TABLE_SIZE, false),
               m_mapCharBigram("CharBigram", DEFAULT_TABLE_SIZE, false),
               m_mapCharTrigram("CharTrigram", DEFAULT_TABLE_SIZE, false),
               m_mapSeenWords("SeenWords", DEFAULT_TABLE_SIZE, false),
               m_mapLastWordByWord("LastWordByWord", DEFAULT_TABLE_SIZE, false),
               m_mapWordAndNextChar("WordAndNextChar", DEFAULT_TABLE_SIZE, false),
               m_mapWordAndPrevChar("WordAndPrevChar", DEFAULT_TABLE_SIZE, false),
               m_mapFirstCharLastWordByWord("FirstCharLastWordByWord", DEFAULT_TABLE_SIZE, false),
               m_mapLastWordByLastChar("LastWordByLastChar", DEFAULT_TABLE_SIZE, false),
               m_mapSeparateChars("SeparateChars", DEFAULT_TABLE_SIZE, false),
               m_mapConsecutiveChars("ConsecutiveChars", DEFAULT_TABLE_SIZE, false),
               m_mapFirstAndLastChars("FirstAndLastChars", DEFAULT_TABLE_SIZE, false),
               m_mapFirstCharAndChar("FirstCharAndChar", DEFAULT_TABLE_SIZE, false),
               m_mapOneCharWord("OneCharWord", DEFAULT_TABLE_SIZE, false),
               m_mapLengthByFirstChar("LengthByFirstChar", DEFAULT_TABLE_SIZE, false),
               m_mapLengthByLastChar("LengthByLastChar", DEFAULT_TABLE_SIZE, false),
               m_mapLengthByLastWord("LengthByLastWord", DEFAULT_TABLE_SIZE, false),
               m_mapLastLengthByWord("LastLengthByWord", DEFAULT_TABLE_SIZE, false),
               m_mapLexiconWord("LexiconWord", 7, false), 
               m_mapCharCatUnigram("CharCatUnigram", 997, false),
               m_mapCharCatBigram("CharCatBigram", 997, false), 
               m_mapCharCatTrigram("CharCatTrigram", 997, false) {
   }
};

/*===============================================================
 *
 * The implementation specific part of segmentor is defined here. 
 *
 *==============================================================*/

class CSegmentorImpl {
};

//===============================================================

inline int normalizeLength(const int &length) {
   return length<LENGTH_MAX-1?length:LENGTH_MAX-1;
}

inline int encodeCharSegmentation(const bool &b, const bool &e) {
   return ( b ? 2 : 0 ) + ( e ? 1 : 0 ) ; // 00 01 10 11
}

inline int encodeCharInfoAndPosition(const int &char_info, const int &pos) {
   assert( pos >= -3 && pos <= 3 ); // pos+3 0->6
   return (char_info<<3) | (pos+3);
}

#endif
