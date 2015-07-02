// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                              *
 * charcat.h - the character category dictionary for Chinese.   *
 *                                                              *
 * The values of different character categories are hard coded  *
 * for segmentation.                                            *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2007.11                        *
 *                                                              *
 ****************************************************************/

#ifndef _CHAR_CATEGORY_H
#define _CHAR_CATEGORY_H

#include "knowledge/worddict.h"

static const std::string letters[] = {
                    "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", 
                    "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", 
                    "Y", "Z", "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", 
                    "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", 
                    "w", "x", "y", "z", 
                    "Ａ", "Ｂ", "Ｃ", "Ｄ", "Ｅ", "Ｆ", "Ｇ", "Ｈ", "Ｉ", 
                    "Ｊ", "Ｋ", "Ｌ", "Ｍ", "Ｎ", "Ｏ", "Ｐ", "Ｑ", "Ｒ", 
                    "Ｓ", "Ｔ", "Ｕ", "Ｖ", "Ｗ", "Ｘ", "Ｙ", "Ｚ", "ａ", 
                    "ｂ", "ｃ", "ｄ", "ｅ", "ｆ", "ｇ", "ｈ", "ｉ", "ｊ", 
                    "ｋ", "ｌ", "ｍ", "ｎ", "ｏ", "ｐ", "ｑ", "ｒ", "ｓ", 
                    "ｔ", "ｕ", "ｖ", "ｗ", "ｘ", "ｙ", "ｚ"};

static const std::string numbers[] = {
   "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", 
   "０", "１", "２", "３", "４", "５", "６", "７", "８", "９"}; 

static const CWord CH_COMMA = CWord("，");
static const CWord CH_PERIOD = CWord("。");
static const CWord CH_DUN = CWord("、");
static const CWord CH_SEMICOLON = CWord("；");
static const CWord CH_COLON = CWord("：");
static const CWord CH_EXCLAMATION = CWord("！");
static const CWord CH_QUESTION = CWord("？");
static const CWord CH_LRB = CWord("（");
static const CWord CH_LQ = CWord("“");
static const CWord CH_LBK = CWord("《");
static const CWord CH_RRB = CWord("）");
static const CWord CH_RQ = CWord("”");
static const CWord CH_RBK = CWord("》");
static const CWord CH_ELLIPSIS = CWord("…");
//

static const CWord starting_brackets[] = {
   CH_LRB, CH_LQ, CH_LBK
};

static const CWord ending_brackets[] = {
   CH_RRB, CH_RQ, CH_RBK
};

static const CWord separating_punctuations[] = {
   CH_COMMA, CH_PERIOD, CH_DUN, CH_SEMICOLON, CH_EXCLAMATION, CH_QUESTION, CH_ELLIPSIS
};

static const CWord sentence_separators[] = {
   CH_PERIOD, CH_EXCLAMATION, CH_QUESTION
};

inline int getStartingBracket(const CWord &word) {
   for (int i=0; i<sizeof(starting_brackets)/sizeof(CWord); ++i)
      if (word==starting_brackets[i])
         return i;
   return -1;
}

inline int getEndingBracket(const CWord &word) {
   for (unsigned i=0; i<sizeof(ending_brackets)/sizeof(CWord); ++i)
      if (word==ending_brackets[i])
         return i;
   return -1;
}

inline int getSeparatingPunctuation(const CWord &word) {
   for (unsigned i=0; i<sizeof(separating_punctuations)/sizeof(CWord); ++i)
      if (word==separating_punctuations[i])
         return i;
   return -1;
}

inline bool isSentenceSeparator(const CWord &word) {
   for (unsigned long i=0; i<sizeof(sentence_separators)/sizeof(CWord); ++i)
      if (word==sentence_separators[i])
         return true;
   return false;
}

inline bool canEndSentence(const CWord &word) {
   return isSentenceSeparator(word) || (getEndingBracket(word) != -1);
}

inline bool isPunctuation(const CWord &word) {
   return word == CH_COMMA ||
          word == CH_PERIOD ||
          word == CH_DUN ||
          word == CH_SEMICOLON ||
          word == CH_COLON ||
          word == CH_EXCLAMATION ||
          word == CH_QUESTION ||
          word == CH_LRB ||
          word == CH_LQ ||
          word == CH_LBK ||
          word == CH_RRB ||
          word == CH_RQ ||
          word == CH_RBK 
          ;
}

/*===============================================================
 *
 * CCharCatDictionary - the character category record
 *
 *==============================================================*/

class CCharCatDictionary : public CWordDictionary {

public:
   CCharCatDictionary() : CWordDictionary(199) { 
      for (unsigned i=0; i<sizeof(letters)/sizeof(std::string); ++i) 
         (*this)[CWord(letters[i])] = eFW ;
      for (unsigned i=0; i<sizeof(numbers)/sizeof(std::string); ++i) 
         (*this)[CWord(numbers[i])] = eCD ;
   }

public:
   enum CHAR_TYPE { eUnknown=0, eFW, eCD } ;

   bool isFWorCD( const std::string &s ) const {
      unsigned long int type = find( s, 0 );
      return type==eFW || type==eCD;
   }

   bool isFW( const CWord &s ) const {
      unsigned long int type = find( s, 0 );
      return type==eFW;
   }

   bool isCD( const CWord &s ) const {
      unsigned long int type = find( s, 0 );
      return type==eCD;
   }

};

#endif
