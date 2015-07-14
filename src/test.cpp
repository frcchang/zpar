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
#include "learning/perceptron/hashmap_score_packed.h"
//#include "hash_small.h"
#include "linkedlist.h"
#include "linguistics/word_tokenized.h"
#include "english/tags.h"
#include "hash.h"
#include "linguistics/word.h"
#include "vector_stream.h"
#include "bitarray.h"

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
   CCoNLLOutput output;
   while (std::cin >> input)
      std::cout << input;
   while (std::cin >> output) {
      std::cout << output;
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

void testpackedscores() {
   CPackedScoreMap<std::string, double, 10> map("test", 65536);
   CPackedScoreType<double, 10> score;
   for (unsigned i=0; i<10; ++i) {
      map.getOrUpdateScore( score , "a1" , i , CScore<double>::eNonAverage , i , 1 );
      score[i] = 1;
   }
   map.getOrUpdateScore( score , "a1" , 0 , CScore<double>::eNonAverage , 1 , 2 );
   map.getOrUpdateScore( score , "a2" , 0 , CScore<double>::eNonAverage , 1 , 2 );
   map.computeAverage(2);
   std::cout << map << std::endl;

   map.getOrUpdateScore( score , "a1" , 0 , CScore<double>::eNonAverage , 0 , 0 );
   std::cout << score << std::endl;

   std::istringstream iss("test 2\na1	:	 [ 1 / 1 , 1 / 2 , 2 / 4 , 3 / 6 , 4 / 8 , 5 / 10 , 6 / 12 , 7 / 14 , 8 / 16 , 9 / 18 ]\na2	:	 [ 1 / 1 , 0 / 0 , 0 / 0 , 0 / 0 , 0 / 0 , 0 / 0 , 0 / 0 , 0 / 0 , 0 / 0 , 0 / 0 ]\n\n");
   iss >> map;
   map.getOrUpdateScore( score , "a2" , 0 , CScore<double>::eAverage , 0 , 0 );
   std::cout << score << std::endl;
}

//void testsmallhash() {
//   CSmallHashMap<unsigned, std::string, 10> t;
//   t[1] = "abc";
//   t[5] = "def";
//   std::cout << t;
//   std::cin >> t;
//   std::cout << t<< std::endl;
//}

void testlinkedlist() {
   CLinkedList<unsigned, std::string> t;
   t[1] = "abc";
   t[5] = "def";
   std::cout << t;
   std::cin >> t;
   std::cout << t<< std::endl;
}

void testinitializer() {
   class A {
   public:
      int a;
      int b;
   public:
      A() : a(0) {}
   };
   class B {
   public:
      int a;
      int b;
   };
   A *a = new A();
   B *b = new B();
   std::cout << a->a << ' ' << a->b << ' ' << b->a << ' ' << b->b << std::endl;
   delete a;
   delete b;
}

void testhashmap(const char *f) {
   CHashMap<CWord, int> map(65536);
   std::ifstream i(f);
   i>>map;
   i.close();
   std::cout << map;
   return;
}

void test_word_vector_int_map() {
   CHashMap< CWord, std::vector<int> > m(128);
   std::ifstream is("tmp.txt");
   is >> m;
   std::cout << m;
}

class C {
public: int i;
};

void testbitarray() {
   CBitArray a, b;
   std::string s = "00101010000000000000000000000000000000000000000000000";
   std::istringstream iss(s);
   std::string t = "00101010";
   std::istringstream ist(t);
   ist >> a;
   iss >> b;
   std::cout << b << std::endl;
   b.add(a);
   std::cout << b << std::endl;
   b.add(1);
   std::cout << b << std::endl;
}

int main(int argc, char**argv){
   try {
//   std::cout << B::C << B::D << std::endl;
//   std::cout << ~0UL << std::endl;
//   testtable2();
//   testreader();
//   testconllfeats();
//   testgenerictags();
//   testlemma();
//   testpackedscores();
//   testsmallhash();
//   testlinkedlist();
//   testinitializer();
//   testhashmap(argv[1]);
//      test_word_vector_int_map();
//   C* c = new C();
//   c -> i = 1;
//   delete c;
   testbitarray();
   }catch(const std::string &s) { std::cout << s << std::endl; }
};

