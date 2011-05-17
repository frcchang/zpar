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

#define ID(x) x

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
   CWeight(const unsigned &TABLE_SIZE=DEFAULT_TABLE_SIZE) : 
               m_mapCharUnigram("CharUnigram", TABLE_SIZE, false),
               m_mapCharBigram("CharBigram", TABLE_SIZE, false),
               m_mapCharTrigram("CharTrigram", TABLE_SIZE, false),
               m_mapSeenWords("SeenWords", TABLE_SIZE, false),
               m_mapLastWordByWord("LastWordByWord", TABLE_SIZE, false),
               m_mapWordAndNextChar("WordAndNextChar", TABLE_SIZE, false),
               m_mapWordAndPrevChar("WordAndPrevChar", TABLE_SIZE, false),
               m_mapFirstCharLastWordByWord("FirstCharLastWordByWord", TABLE_SIZE, false),
               m_mapLastWordByLastChar("LastWordByLastChar", TABLE_SIZE, false),
               m_mapSeparateChars("SeparateChars", TABLE_SIZE, false),
               m_mapConsecutiveChars("ConsecutiveChars", TABLE_SIZE, false),
               m_mapFirstAndLastChars("FirstAndLastChars", TABLE_SIZE, false),
               m_mapFirstCharAndChar("FirstCharAndChar", TABLE_SIZE, false),
               m_mapOneCharWord("OneCharWord", TABLE_SIZE, false),
               m_mapLengthByFirstChar("LengthByFirstChar", TABLE_SIZE, false),
               m_mapLengthByLastChar("LengthByLastChar", TABLE_SIZE, false),
               m_mapLengthByLastWord("LengthByLastWord", TABLE_SIZE, false),
               m_mapLastLengthByWord("LastLengthByWord", TABLE_SIZE, false),
               m_mapLexiconWord("LexiconWord", TABLE_SIZE, false), 
               m_mapCharCatUnigram("CharCatUnigram", TABLE_SIZE, false),
               m_mapCharCatBigram("CharCatBigram", TABLE_SIZE, false), 
               m_mapCharCatTrigram("CharCatTrigram", TABLE_SIZE, false) {
   }

public:
   void clear() {
      iterate_templates(,.clear(););
   }
   void scale(const SCORE_TYPE &scale, const int &round) {
      iterate_templates(,ID(.scaleCurrent(scale, round);));
   }
   void addCurrent(CWeight &w, const int &round) {
      m_mapCharUnigram.addCurrent(w.m_mapCharUnigram, round);
      m_mapCharBigram.addCurrent(w.m_mapCharBigram, round);
      m_mapCharTrigram.addCurrent(w.m_mapCharTrigram, round);
      m_mapSeenWords.addCurrent(w.m_mapSeenWords, round);
      m_mapLastWordByWord.addCurrent(w.m_mapLastWordByWord, round);
      m_mapWordAndNextChar.addCurrent(w.m_mapWordAndNextChar, round);
      m_mapWordAndPrevChar.addCurrent(w.m_mapWordAndPrevChar, round);
      m_mapFirstCharLastWordByWord.addCurrent(w.m_mapFirstCharLastWordByWord, round);
      m_mapLastWordByLastChar.addCurrent(w.m_mapLastWordByLastChar, round);
      m_mapSeparateChars.addCurrent(w.m_mapSeparateChars, round);
      m_mapConsecutiveChars.addCurrent(w.m_mapConsecutiveChars, round);
      m_mapFirstAndLastChars.addCurrent(w.m_mapFirstAndLastChars, round);
      m_mapFirstCharAndChar.addCurrent(w.m_mapFirstCharAndChar, round);
      m_mapOneCharWord.addCurrent(w.m_mapOneCharWord, round);
      m_mapLengthByFirstChar.addCurrent(w.m_mapLengthByFirstChar, round);
      m_mapLengthByLastChar.addCurrent(w.m_mapLengthByLastChar, round);
      m_mapLengthByLastWord.addCurrent(w.m_mapLengthByLastWord, round);
      m_mapLastLengthByWord.addCurrent(w.m_mapLastLengthByWord, round);
      m_mapLexiconWord.addCurrent(w.m_mapLexiconWord, round);
      m_mapCharCatUnigram.addCurrent(w.m_mapCharCatUnigram, round);
      m_mapCharCatBigram.addCurrent(w.m_mapCharCatBigram, round);
      m_mapCharCatTrigram.addCurrent(w.m_mapCharCatTrigram, round);
   }

   ~CWeight() {
      iterate_templates(,.freePoolMemory(););
   }
};

/*===============================================================
 *
 * The implementation specific part of segmentor is defined here. 
 *
 *==============================================================*/

class CSegmentorImpl {
protected:
   CWeight m_delta;
public:
   CSegmentorImpl() : m_delta(128) { iterate_templates(m_delta.,.init();); }
   CWeight &getDelta() { return m_delta; }
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
