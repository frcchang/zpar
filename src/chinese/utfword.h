// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                              *
 * utfword.h - the definitions for utf chinese words            *
 *                                                              *
 * This module provides the facility of recording the Chinese   *
 * word and information about its characters.                   *
 *                                                              *
 * A utf word is a std::vector of CWord, while each item in the list *
 * represents a character in the word. Since the definition of  *
 * CWord can be implemented in different ways, a proper file    *
 * that defines CWord must be included before using this file.  *
 * This file presumes that CWord has already been defined.      *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2007.10                        *
 *                                                              *
 ****************************************************************/

#ifndef _CHINESE_UTF_WORD_H
#define _CHINESE_UTF_WORD_H

#include "definitions.h"
#include "utils.h"
#include "taggedword.h"
// a definition of CWord must have been included previously

/*===============================================================
 *
 * definitions about UTF word
 *
 *==============================================================*/

// the word class, which includes hash
class CUTFWord : virtual public CWord {

protected:

   std::vector<CWord> m_characters;

public:

   CUTFWord( const std::string &word ) : CWord(word) {
      m_characters.clear();
      getCharactersFromUTF8String( word , &m_characters );
   }
   CUTFWord( const CWord &word ) : CWord(word) {
      m_characters.clear();
      getCharactersFromUTF8String( word.str() , &m_characters );
   }
   virtual ~CUTFWord() {}

public:

   const CWord &getCharacter(int index) const { return m_characters[index]; }
   const CWord &operator [](int index) const { return m_characters[index]; }

   int size() const { return m_characters.size(); }

};

/*===============================================================
 *
 * definitions about tagged UTF word
 *
 *==============================================================*/

// the word class, which includes hash
class CUTFTaggedWord : public CUTFWord, public CTaggedWord {

public:

   CUTFTaggedWord( const std::string &word , const CTag t ) : CWord(word) , CUTFWord(word) , CTaggedWord(word, t) {
   }
   CUTFTaggedWord( const CWord &word , const CTag t ) : CWord(word) , CUTFWord(word) , CTaggedWord(word, t) {
   }
   virtual ~CUTFTaggedWord() {}

};

#endif

