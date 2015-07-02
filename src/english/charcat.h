// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                              *
 * charcat.h - the character category dictionary for English.   *
 *                                                              *
 * The values of different character categories are hard coded  *
 * Author: Yue Zhang                                            *
 *                                                              *
 ****************************************************************/

#ifndef _ENGLISH_CHAR_CATEGORY_H
#define _ENGLISH_CHAR_CATEGORY_H

static const CWord CH_COMMA = CWord(",");
static const CWord CH_PERIOD = CWord(".");
static const CWord CH_QUESTN = CWord("?");
static const CWord CH_EXCL = CWord("!");
static const CWord CH_DASH = CWord("-");
static const CWord CH_DASH2 = CWord("--");
static const CWord CH_LEFT_ROUND_BRACKET = CWord("(");
static const CWord CH_LEFT_ROUND_BRACKET_EX = CWord("-LRB-");
static const CWord CH_RIGHT_ROUND_BRACKET = CWord(")");
static const CWord CH_RIGHT_ROUND_BRACKET_EX = CWord("-RRB-");
static const CWord CH_LEFT_CURLY_BRACKET = CWord("{");
static const CWord CH_LEFT_CURLY_BRACKET_EX = CWord("-LCB-");
static const CWord CH_RIGHT_CURLY_BRACKET = CWord("}");
static const CWord CH_RIGHT_CURLY_BRACKET_EX = CWord("-RCB-");
static const CWord CH_LEFT_SINGLE_QUOTE = CWord("`");
static const CWord CH_RIGHT_SINGLE_QUOTE = CWord("'");
static const CWord CH_LEFT_QUOTE = CWord("``");
static const CWord CH_RIGHT_QUOTE = CWord("''");
static const CWord CH_DOUBLE_QUOTE = CWord("\"");
static const CWord CH_COLON = CWord(":");
static const CWord CH_SEMI_COLON = CWord(";");
static const CWord CH_ELLIPSIS = CWord("...");

static const CWord starting_brackets[] = {
   CH_LEFT_ROUND_BRACKET, CH_LEFT_ROUND_BRACKET_EX, CH_LEFT_CURLY_BRACKET, CH_LEFT_CURLY_BRACKET_EX, CH_LEFT_SINGLE_QUOTE, CH_LEFT_QUOTE
};

static const CWord ending_brackets[] = {
   CH_RIGHT_ROUND_BRACKET, CH_RIGHT_ROUND_BRACKET_EX, CH_RIGHT_CURLY_BRACKET, CH_RIGHT_CURLY_BRACKET_EX, CH_RIGHT_SINGLE_QUOTE, CH_RIGHT_QUOTE
};

static const CWord sentence_separators[] = {
   CH_PERIOD, CH_EXCL, CH_QUESTN
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

inline bool isSentenceSeparator(const CWord &word) {
   for (unsigned long i=0; i<sizeof(sentence_separators)/sizeof(CWord); ++i)
      if (word==sentence_separators[i])
         return true;
   return false;
}

inline unsigned matchPunctuation(const CWord &word) {
   unsigned retval=0;
   if (word==CH_COMMA) { return 1<<retval; }
   ++retval; if (word==CH_PERIOD) { return 1<<retval; }
   ++retval; if (word==CH_QUESTN) { return 1<<retval; }
   ++retval; if (word==CH_EXCL) { return 1<<retval; }
   ++retval; if (word==CH_DASH) { return 1<<retval; }
   ++retval; if (word==CH_DASH2) { return 1<<retval; }
   ++retval; if (word==CH_LEFT_ROUND_BRACKET) { return 1<<retval; }
   if (word==CH_LEFT_ROUND_BRACKET_EX) { return 1<<retval; }
   ++retval; if (word==CH_RIGHT_ROUND_BRACKET) { return 1<<retval; }
   if (word==CH_RIGHT_ROUND_BRACKET_EX) { return 1<<retval; }
   ++retval; if (word==CH_LEFT_CURLY_BRACKET) { return 1<<retval; }
   if (word==CH_LEFT_CURLY_BRACKET_EX) { return 1<<retval; }
   ++retval; if (word==CH_RIGHT_CURLY_BRACKET) { return 1<<retval; }
   if (word==CH_RIGHT_CURLY_BRACKET_EX) { return 1<<retval; }
   ++retval; if (word==CH_LEFT_SINGLE_QUOTE) { return 1<<retval; }
   ++retval; if (word==CH_RIGHT_SINGLE_QUOTE) { return 1<<retval; }
   ++retval; if (word==CH_LEFT_QUOTE) { return 1<<retval; }
   ++retval; if (word==CH_RIGHT_QUOTE) { return 1<<retval; }
   ++retval; if (word==CH_DOUBLE_QUOTE) { return 1<<retval; }
   ++retval; if (word==CH_COLON) { return 1<<retval; }
   ++retval; if (word==CH_SEMI_COLON) { return 1<<retval; }
   ++retval; if (word==CH_ELLIPSIS) { return 1<<retval; }
   assert(retval<sizeof(unsigned)* 8);
   return 0;
}

#endif
