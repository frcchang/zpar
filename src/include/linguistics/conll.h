// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                              *
 * conll.h - input and output formats for conll dependency tree *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 ****************************************************************/

#ifndef _DEPENDENCY_CONLL_H
#define _DEPENDENCY_CONLL_H

#include "dependency.h"

/*==============================================================
 *
 * CCCoNLLInputNode
 *
 *==============================================================*/

class CCoNLLInputNode : public CDependencyTreeNode {

public:
   int id;
   string word; // same as dependency
   string lemma;
   string ctag;
   string tag;  // same as dependency
   string feats;

public:
   CCoNLLInputNode( ) : id(-1) { }
   CCoNLLInputNode( const int &d, const string &w, const string &l, const string &c, const string &t, const string &f) : id(d), word(w), lemma(l), ctag(c), tag(t), feats(f) { }
   virtual ~CCoNLLInputNode() {}

};

/*==============================================================
 *
 * CCCoNLLOutputNode
 *
 *==============================================================*/

class CCoNLLOutputNode : public CCoNLLInputNode {

public:
   int head;     // same as dependency
   string label;
   int phead;
   string plabel;

public:
   CCoNLLOutputNode( ) : head(DEPENDENCY_LINK_NO_HEAD), phead(DEPENDENCY_LINK_NO_HEAD) { }
   CCoNLLOutputNode( const int &d, const string &w, const string &l, const string &c, const string &t, const string &f, const int &h, const string &la, const int &p, const string &pl ) : CCoNLLInputNode(d, w, l, c, t, f), head(h), label(la), phead(p), plabel(pl) { }
   virtual ~CCoNLLOutputNode() {}

};

//==============================================================

inline istream & operator >> (istream &is, CCoNLLInputNode &node) {
   string line;

   getline(is, line, '\t');
   ASSERT(is && !line.empty(), "Not well formatted CoNLL data");
   istringstream iss_id(line);
   iss_id >> node.id;

   getline(is, line, '\t');
   ASSERT(is && !line.empty(), "Not well formatted CoNLL data");
   node.word = line;

   getline(is, line, '\t');
   ASSERT(is && !line.empty(), "Not well formatted CoNLL data");
   node.lemma = line;

   getline(is, line, '\t');
   ASSERT(is && !line.empty(), "Not well formatted CoNLL data");
   node.ctag = line;

   getline(is, line, '\t');
   ASSERT(is && !line.empty(), "Not well formatted CoNLL data");
   node.tag = line;

   getline(is, line, '\t');
   ASSERT(is && !line.empty(), "Not well formatted CoNLL data");
   node.feats = line;
   return is ;
}

inline ostream & operator << (ostream &os, const CCoNLLInputNode &node) {
   os << node.id << '\t' << node.word << '\t' << node.lemma << "\t" << node.ctag << '\t' << node.tag << '\t' << node.feats;
   return os ;
}

inline istream & operator >> (istream &is, CCoNLLOutputNode &node) {
   string line;

   is >> static_cast<CCoNLLInputNode&>(node);

   getline(is, line, '\t');
   ASSERT(is && !line.empty(), "Not well formatted CoNLL data");
   istringstream iss_head(line);
   iss_head >> node.head;

   getline(is, line, '\t');
   ASSERT(is && !line.empty(), "Not well formatted CoNLL data");
   node.label = line;

   getline(is, line, '\t');
   ASSERT(is && !line.empty(), "Not well formatted CoNLL data");
   if (line=="_") {
      node.phead = DEPENDENCY_LINK_NO_HEAD;
   }
   else {
      istringstream iss_phead(line);
      iss_phead >> node.phead;
   }

   getline(is, line, '\t');
   ASSERT(is && !line.empty(), "Not well formatted CoNLL data");
   node.plabel = line;
   return is ;
}

inline ostream & operator << (ostream &os, const CCoNLLOutputNode &node) {
   // get phead formatted correct by inserting underscore
   ostringstream oss;
   if (node.phead==DEPENDENCY_LINK_NO_HEAD)
      oss << '_';
   else
      oss << node.phead;
   
   os << static_cast<const CCoNLLInputNode&>(node) << "\t" << node.head << '\t' << node.label << '\t' << oss.str() << '\t' << node.plabel;
   return os ;
}

/*==============================================================
 *
 * CCoNLLInput
 *
 *==============================================================*/

class CCoNLLInput : public vector<CCoNLLInputNode>{

public:
   CCoNLLInput() {clear();}

public:
   void clear() {
      vector<CCoNLLInputNode>::clear();
      push_back(CCoNLLInputNode(0, "", "", "", "", ""));
   }
};

inline std::istream & operator >> (std::istream &is, CCoNLLInput &sent) {
   sent.clear();
   string line;
   getline(is, line);

   while(is && !line.empty())
   {
      CCoNLLInputNode node;
      istringstream iss(line);
      iss >> node ;
      sent.push_back( node );
      getline(is, line);
   }
   return is ;
}

inline std::ostream & operator << (std::ostream &os, const CCoNLLInput &sent) {
   for (int i=1; i<sent.size(); ++i)
      os << sent.at(i) << endl ;
   os << endl ;
   return os ;
}

/*==============================================================
 *
 * CCoNLLOutput
 *
 *==============================================================*/

class CCoNLLOutput : public vector<CCoNLLOutputNode> {

public:
   CCoNLLOutput() {clear();}

public:
   void clear() {
      vector<CCoNLLOutputNode>::clear();
      push_back(CCoNLLOutputNode(0, "", "", "", "", "", DEPENDENCY_LINK_NO_HEAD, "", DEPENDENCY_LINK_NO_HEAD, ""));
   }
};

inline std::istream & operator >> (std::istream &is, CCoNLLOutput &sent) {
   sent.clear();
   string line;
   getline(is, line);

   while(is && !line.empty())
   {
      CCoNLLOutputNode node;
      istringstream iss(line);
      iss >> node ;
      sent.push_back( node );
      getline(is, line);
   }
   return is ;
}

inline std::ostream & operator << (std::ostream &os, const CCoNLLOutput &sent) {
   for (int i=1; i<sent.size(); ++i)
      os << sent.at(i) << endl ;
   os << endl ;
   return os ;
}

#endif
