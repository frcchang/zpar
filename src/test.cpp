// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                         
 * test.cpp 
 *                                                          
 * Author: Yue Zhang                                         
 *                                                      
 ****************************************************************/

#define SIMPLE_HASH
#include "definitions.h"
#include "table2.h"
#include "reader.h"
#include "writer.h"
#include "linguistics/conll.h"
#include "linguistics/generictag.h"
#include "linguistics/lemma.h"

class A {
public:
   enum {C=0, D};
};

class B : public A {
public:
   enum {C=A::C+1};
};

void testtable2() {
   CTable2<std::string, std::string, double> table2;
   std::ifstream is("./input.txt");
   is >> table2;
   std::cout << table2.lookup("'m", "!edge") << std::endl;
   std::cout << table2.lookup("OOV", "OOV") << std::endl;
   std::cout << table2.lookup("None", "1") << std::endl;
//   std::cout << table2;
   is.close();
}

void testreader() {
   CSentenceReader reader("tmp.txt");
   CTwoStringVector sent;
   CSentenceWriter writer;
   while (reader.readTaggedSentence(&sent, false, '/')) {
      writer.writeSentence(&sent);
   }
}

void testconllio() {
   // io
   CCoNLLInput input;
   CCoNLLOutput outout;
   while (std::cin >> input)
      std::cout << input;
   while (std::cin >> outout) {
      std::cout << outout;
   }
}

void testconlltag() {
   // tag
   CCoNLLCPOS pos;
   pos.load("CPOS a");
   pos.load("CPOS b");
   pos.load("CPOS c");
   for (unsigned i=0; i<pos.getTagset().count(); ++i)
      std::cout << CCoNLLCPOS(i) << ' ';
   CCoNLLFeats feats;
   feats.load("Feats a");
   feats.load("Feats b");
   feats.load("Feats c");
   for (unsigned i=0; i<feats.getTagset().count(); ++i)
      std::cout << CCoNLLFeats(i) << ' ';
   std::cout << std::endl;
}

void testconllfeats() {
   std::vector<CCoNLLFeats> feats;
   std::string s;
   std::cin >> s;
   readCoNLLFeats(feats, s);
   for (unsigned i=0; i<feats.size(); ++i)
      std::cout << feats[i] << ' '; 
   std::cout << std::endl;
   return;

}

void testgenerictags() {
   class CTag1 : public CGenericTag {
   public:
      CTag1() : CGenericTag() { }
      CTag1(const std::string &s) : CGenericTag() { load(s); }
      CTag1(const CTag1 &t) : CGenericTag(t) { }
      CTag1(const unsigned long &u) : CGenericTag() { assert(getTagset().count()>u); m_code=u; }
      CGenericTagset &getTagset() const { static CGenericTagset tagset; return tagset; }
   };
   class CTag2 : public CGenericTag {
   public:
      CTag2() : CGenericTag() { }
      CTag2(const std::string &s) : CGenericTag() { load(s); }
      CTag2(const CTag2 &t) : CGenericTag(t) { }
      CTag2(const unsigned long &u) : CGenericTag() { assert(getTagset().count()>u); m_code=u; }
      CGenericTagset &getTagset() const { static CGenericTagset tagset; return tagset; }
   };
   CTag1 tag1("1a");
   CTag2 tag2("2a");
   CTag1 tag3("1b");
   tag3.load("1c");
   for (unsigned i=0; i<tag1.getTagset().count(); ++i)
      std::cout << CTag1(i) << ' ';
   std::cout << std::endl;
   for (unsigned i=0; i<tag2.getTagset().count(); ++i)
      std::cout << CTag2(i) << ' ';
   std::cout << std::endl;
}

void testlemma() {
   CLemma lemma1("abc");
   CLemma lemma2;
   std::cin >> lemma2;
   std::cout << lemma1 << " " << lemma2 << std::endl;
}

int main(int argc, char**argv){
   try {
//   std::cout << B::C << B::D << std::endl;
//   std::cout << ~0UL << std::endl;
//   testtable2();
//   testreader();
//   testconllfeats();
//   testgenerictags();
 testlemma();
   }catch(const std::string &s) { std::cout << s << std::endl; }
};

