// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                              *
 * reader.cpp - the sentence reader classes definition          *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2006.10                        *
 *                                                              *
 ****************************************************************/

#include "reader.h"

/*===============================================================
 *
 * CSentenceReader - read sentence
 *
 *==============================================================*/

/*---------------------------------------------------------------
 *
 * readRawCharacter - read a raw character in utf
 *
 *---------------------------------------------------------------*/

bool CSentenceReader::readRawCharacter(std::string *retval) {
   char cTemp;
   std::string sWord;                                // std::string for next word
   bool bReadSomething = false;                 // did we read anything?
   while (m_iStream->get(cTemp)) {              // still have something there
      bReadSomething = true;
      if (cTemp == '\r')
         continue;
      sWord += cTemp;
      if (isOneUTF8Character(sWord)) {
         *retval = sWord;
         break;
      }
   }
   return bReadSomething;
}

/*---------------------------------------------------------------
 *
 * readRawSentence - read a raw sentence
 *
 * The input file should contain tokenised sentences each in a line, 
 * with space separated words and punctuations. 
 * In the Chinese case, each character should be separated by space.
 *
 * bIgnoreSpace indicates whether spaces should be also included
 * into the returned sentence. If they are not included, they will
 * simply be dropped std::cout. 
 *
 * The return value shows if anything was read from the file
 *
 *--------------------------------------------------------------*/

bool CSentenceReader::readRawSentence(CStringVector *vReturn, bool bSkipEmptyLines, bool bIgnoreSpace) {
   assert(vReturn != NULL);
   vReturn->clear();
   char cTemp;
   std::string sWord;                                // std::string for next word
   bool bReadSomething = false;                 // did we read anything?
   while (m_iStream->get(cTemp)) {              // still have something there
      bReadSomething = true;
      if (cTemp == '\r')
         continue;
      if (cTemp == '\n') {                      // if we meet EOL, return std::string
         m_nLine++;                             // new line
         if (!sWord.empty()) {                  // we have found another word
            getCharactersFromUTF8String(sWord, vReturn); 
         }
         if (vReturn->empty()) {
            if (bSkipEmptyLines)
               continue;
         }
         return bReadSomething;
      }

      if (bIgnoreSpace && cTemp == ' ') {       // otherwise, if we meet " "
         if (!sWord.empty()) {                  // we have found another word
            getCharactersFromUTF8String(sWord, vReturn); 
            sWord = "";
         }
      }
      else                                      // otherwise
         sWord += cTemp;
   }
   if (!sWord.empty()) {                        // we have found another word
      getCharactersFromUTF8String(sWord, vReturn); 
      sWord = "";
      TRACE((unsigned long int)(cTemp))
   }
   return bReadSomething;
};

/*---------------------------------------------------------------
 *
 * readSegmentedSentence - read a segmented sentence
 *
 * The input file should contain tokenised sentences each in a line, 
 * with space separated words and punctuations. 
 * In the Chinese case, each character should be separated by space.
 *
 * The return value shows if anything was read from the file
 *
 *--------------------------------------------------------------*/

bool CSentenceReader::readSegmentedSentence(CStringVector *vReturn, bool bSkipEmptyLines) {
   assert(vReturn != NULL);
   vReturn->clear();
   char cTemp;
   std::string sWord;                                // std::string for next word
   bool bReadSomething = false;
   while (m_iStream->get(cTemp)) {              // still have something there
      bReadSomething = true;
      if (cTemp == '\r')
         continue;
      if (cTemp == '\n') {                      // if we meet EOL, return std::string
         m_nLine++;                             // new line
         if (!sWord.empty()) {                  // we have found another word
            vReturn->push_back(sWord); 
         }
         if (vReturn->empty()) {
            if (bSkipEmptyLines)
               continue;
         }
         return bReadSomething;
      }
      if (cTemp == ' ') {                       // otherwise, if we meet " "
         if (!sWord.empty()) {                  // we have found another word
            vReturn->push_back(sWord); 
            sWord = "";
         }
      }
      else                                      // otherwise
         sWord += cTemp;
   }
   if (!sWord.empty()) {                        // we have found another word
      vReturn->push_back(sWord); 
      sWord = "";
   }
   return bReadSomething;
};

/*---------------------------------------------------------------
 *
 * readTaggedSentence - read a tagged sentence
 *
 * The input file should contain tagged sentences each in a line, 
 * with space separated words and punctuations. 
 * Each word and its pos tag are divided by a separator char. 
 *
 * The return value shows if anything was read from the file
 *
 *--------------------------------------------------------------*/

bool CSentenceReader::readTaggedSentence(CTwoStringVector *vReturn, bool bSkipEmptyLines, const char separator) {
   assert(vReturn != NULL);
   vReturn->clear();
   char cTemp;
   bool bCurrentTag = false;
   bool bReadSomething = false;
   std::string sWord;                                // std::string for next word
   std::string sTag;                                 // std::string for next tag
   while (m_iStream->get(cTemp)) {              // still have something there
      bReadSomething = true;
      if (cTemp == '\r')                        // ignore the windows \r
         continue;                              // and continue read in
      if (cTemp == '\n') {                      // if we meet EOL, return std::string
         m_nLine++;                             // next line
         if (!sWord.empty() || !sTag.empty()) { // we have found another word
            if (sTag.empty()) {
               REPORT("Input file line " << m_nLine << ": not well formatted tag for" << sWord);
               vReturn->clear();
               return bReadSomething;
            }
            vReturn->push_back(std::make_pair(sWord, sTag));
         }
         if (vReturn->empty()) {
            if (bSkipEmptyLines)
               continue;
         }
         return bReadSomething;
      }
      if (cTemp == ' ') {                       // otherwise, if we meet " "
         if (!sWord.empty() || !sTag.empty()) { // we have found another word
            if (sTag.empty()) {
               REPORT("Input file line " << m_nLine << ": not well formatted tag for" << sWord);
               vReturn->clear();
               return bReadSomething;
            }
            vReturn->push_back(std::make_pair(sWord, sTag));
            sWord = "";
            sTag = "";
            bCurrentTag = false;
         }
      }
//      else if ( cTemp == separator && sWord[sWord.size()-1] != '\\') { // otherwise, if we meet the separator char 
      else if ( cTemp == separator ) { // otherwise, if we meet the separator char 
         if (!bCurrentTag) { // current word
            bCurrentTag = true; 
         }
         else { // already currently processing tag
            sWord += separator; 
            sWord += sTag;
            sTag = "";
         }
      }
      else                                      // otherwise
         if (bCurrentTag)
            sTag += cTemp;
         else
            sWord += cTemp;
   }
   if (!sWord.empty() || !sTag.empty()) {        // we have found another word
      if (sTag.empty()) {
         REPORT("Input file line " << m_nLine << ": not well formatted tag for" << sWord);
         vReturn->clear();
         return bReadSomething;
      }
      vReturn->push_back(std::make_pair(sWord, sTag));
      sWord = "";
   } 
   return bReadSomething;
};

/*---------------------------------------------------------------
 * tokenizeWord - tokenize a word 
 *
 * Return: whether split words and pushed vReturn
 *
 *--------------------------------------------------------------*/

inline bool tokenizeWord(const std::string &w, CStringVector *v) {
   int n = w.size();
   if ( n > 3 ) {
      if ( w == "cannot" ) {
         v->push_back("can");
         v->push_back("not");
         return true;
      }
      if (w[n-2] == '\'' || w[n-3] == '\'') {
         std::string postfix = w.substr(n-3);
         if (postfix == "'ll" || postfix == "'re" || postfix == "'ve" || postfix == "n't") {
            v->push_back(w.substr(0, n-3));
            v->push_back(postfix);
            return true;
         }
      }
   }
   if ( n > 2 ) {
      if ( w[n-2] == '\'' ) {
         std::string postfix = w.substr(n-2);
         if (postfix == "'s" || postfix == "'m" || postfix == "'d" ) {
            v->push_back(w.substr(0, n-2));
            v->push_back(postfix);
            return true;
         }
      }
   }
   if ( n > 1 ) {
      if ( w[0] == '"' ) {
         v->push_back("``");
         if ( w[n-1] == '"' ) {
            v->push_back(w.substr(1, n-1));
            v->push_back("''");
         }
         else {
            v->push_back(w.substr(1));
         }
         return true;
      }
      else if ( w[n-1] == '"' ) {
         v->push_back(w.substr(0, n-1));
         v->push_back("''");
      }
      else {
         int m = n-1;
         while ( w[m] == ',' || w[m] == ';' || w[m] == ':' || w[m] == '?' || w[m] == '!' || w[m] == '\'' ) {
            v->push_back(w.substr(0, m));
            --m;
         }
         v->push_back(w.substr(m));
         return true;
      }
   }
   return false;
}

/*---------------------------------------------------------------
 *
 * readSegmentedSentenceAndTokenize - read a segmented sentence
 *
 * The input file should contain segmented sentences each in a line, 
 * with space separated words. Input must be sentence segmented. 
 *
 * The return value shows if anything was read from the file
 *
 *--------------------------------------------------------------*/

bool CSentenceReader::readSegmentedSentenceAndTokenize(CStringVector *vReturn, bool bSkipEmptyLines) {
   assert(vReturn != NULL);
   vReturn->clear();
   char cTemp;
   std::string sWord;                                // std::string for next word
   bool bReadSomething = false;
   while (m_iStream->get(cTemp)) {              // still have something there
      bReadSomething = true;
      if (cTemp == '\r')
         continue;
      if (cTemp == '\n') {                      // if we meet EOL, return std::string
         m_nLine++;                             // new line
         if (!sWord.empty()) {                  // we have found another word
            if (!tokenizeWord(sWord, vReturn)) { // tokenize word
               // end of sentence .
               if (sWord.size() > 1 && sWord[sWord.size()-1] == '.') {
                  vReturn->push_back(sWord.substr(0, sWord.size()-1));
                  vReturn->push_back(".");
               }
               else
                  vReturn->push_back(sWord); 
            }
         }
         if (vReturn->empty()) {
            if (bSkipEmptyLines)
               continue;
         }
         return bReadSomething;
      }
      if (cTemp == ' ') {                       // otherwise, if we meet " "
         if (!sWord.empty()) {                  // we have found another word
            if (!tokenizeWord(sWord, vReturn))
               vReturn->push_back(sWord); 
            sWord = "";
         }
      }
      else                                      // otherwise
         sWord += cTemp;
   }
   if (!sWord.empty()) {                        // we have found another word
      vReturn->push_back(sWord); 
      sWord = "";
   }
   return bReadSomething;
};

