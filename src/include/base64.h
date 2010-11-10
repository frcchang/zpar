// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                              *
 * base64.h - base 64 utilities definitions                     *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2006.10                        *
 *                                                              *
 ****************************************************************/

#ifndef _UTILITIES_BASE64_H
#define _UTILITIES_BASE64_H 1

/*===============================================================
 *
 * Encode utilities
 *
 *==============================================================*/

#define BASE64_STR "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"

/*---------------------------------------------------------------
 *
 * base64Encode - base64 encode
 *
 * Input: integer between 0 64
 *
 * Return: char
 *
 *--------------------------------------------------------------*/

inline
char base64Encode(int input) {
static std::string base64Str = BASE64_STR;
return base64Str[input];
}

/*---------------------------------------------------------------
 *
 * base64Decode - base64 decode
 *
 * Input: char in the table
 *
 * Return: int or -1 if missed
 *
 *--------------------------------------------------------------*/

inline
int base64Decode(char input) {
static std::string base64Str = BASE64_STR;
for (int i=0; i<64; ++i)
   if (base64Str[i]==input) return i;
return -1;
}

#endif

