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

#include "utils.h"
#include "dependency.h"
#include "dependencylabel.h"
#include "generictag.h"

/*==============================================================
 *
 * CCCoNLLInputNode
 *
 *==============================================================*/

class CCoNLLInputNode : public CDependencyTreeNode {

public:
   int id;
   std::string word; // same as dependency
   std::string lemma;
   std::string ctag;
   std::string tag;  // same as dependency
   std::string feats;

public:
   CCoNLLInputNode( ) : id(-1) { }
   CCoNLLInputNode( const int &d, const std::string &w, const std::string &l, const std::string &c, const std::string &t, const std::string &f) : id(d), word(w), lemma(l), ctag(c), tag(t), feats(f) { }
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
   std::string label;
   int phead;
   std::string plabel;

public:
   CCoNLLOutputNode( ) : head(DEPENDENCY_LINK_NO_HEAD), phead(DEPENDENCY_LINK_NO_HEAD) { }
   CCoNLLOutputNode( const int &d, const std::string &w, const std::string &l, const std::string &c, const std::string &t, const std::string &f, const int &h, const std::string &la, const int &p, const std::string &pl ) : CCoNLLInputNode(d, w, l, c, t, f), head(h), label(la), phead(p), plabel(pl) { }
   virtual ~CCoNLLOutputNode() {}

};

//==============================================================

inline std::istream & operator >> (std::istream &is, CCoNLLInputNode &node) {
   std::string line;

   getline(is, line, '\t');
   ASSERT(is && !line.empty(), "Not well formatted CoNLL data (id not found)");
   std::istringstream iss_id(line);
   iss_id >> node.id;

   getline(is, line, '\t'); 
   ASSERT(is && !line.empty(), "Not well formatted CoNLL data (word not found);");
   node.word = line;

   getline(is, line, '\t');
   ASSERT(is && !line.empty(), "Not well formatted CoNLL data (lemma not found)");
   node.lemma = line;

   getline(is, line, '\t');
   ASSERT(is && !line.empty(), "Not well formatted CoNLL data (cpos not found)");
   node.ctag = line;

   getline(is, line, '\t');
   ASSERT(is && !line.empty(), "Not well formatted CoNLL data (pos not found(");
   node.tag = line;

   getline(is, line, '\t');
   ASSERT(is && !line.empty(), "Not well formatted CoNLL data (features not found)");
   node.feats = line;
   return is ;
}

inline std::ostream & operator << (std::ostream &os, const CCoNLLInputNode &node) {
   os << node.id << '\t' << node.word << '\t' << node.lemma << "\t" << node.ctag << '\t' << node.tag << '\t' << node.feats;
   return os ;
}

inline std::istream & operator >> (std::istream &is, CCoNLLOutputNode &node) {
   std::string line;

   is >> static_cast<CCoNLLInputNode&>(node);

   getline(is, line, '\t');
   ASSERT(is && !line.empty(), "Not well formatted CoNLL data");
   std::istringstream iss_head(line);
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
      std::istringstream iss_phead(line);
      iss_phead >> node.phead;
   }

   getline(is, line, '\t');
   ASSERT(is && !line.empty(), "Not well formatted CoNLL data");
   node.plabel = line;
   return is ;
}

inline std::ostream & operator << (std::ostream &os, const CCoNLLOutputNode &node) {
   // get phead formatted correct by inserting underscore
   std::ostringstream oss;
   if (node.phead==DEPENDENCY_LINK_NO_HEAD)
      oss << '_';
   else
      oss << node.phead;
   
   os << static_cast<const CCoNLLInputNode&>(node) << "\t" << node.head << '\t' << (node.label.empty()?"_":node.label) << '\t' << oss.str() << '\t' << (node.plabel.empty()?"_":node.plabel);
   return os ;
}

/*==============================================================
 *
 * CCoNLLInput
 *
 *==============================================================*/

class CCoNLLInput : public std::vector<CCoNLLInputNode>{

public:
   CCoNLLInput() {clear();}

public:
   void clear() {
      std::vector<CCoNLLInputNode>::clear();
      push_back(CCoNLLInputNode(0, "", "", "-BEGIN-", "-BEGIN-", ""));
   }

   void toTwoStringVector(CTwoStringVector &out) const {
      out.clear();
      for (unsigned i=0; i<size(); ++i) {
         out.push_back(std::make_pair(at(i).word, at(i).tag));
      }
   }

   void toStringVector(CStringVector &out) const {
         out.clear();
         for (unsigned i=0; i<size(); ++i) {
            out.push_back(at(i).word);
      }
   }
};

inline std::istream & operator >> (std::istream &is, CCoNLLInput &sent) {
   sent.clear();
   std::string line;
   getline(is, line);

   while(is && !lstrip(line).empty())
   {
      CCoNLLInputNode node;
      std::istringstream iss(rstrip(line));
      iss >> node ;
      sent.push_back( node );
      getline(is, line);
   }
   return is ;
}

inline std::ostream & operator << (std::ostream &os, const CCoNLLInput &sent) {
   for (unsigned i=1; i<sent.size(); ++i)
      os << sent.at(i) << std::endl ;
   os << std::endl ;
   return os ;
}

/*==============================================================
 *
 * CCoNLLOutput
 *
 *==============================================================*/

class CCoNLLOutput : public std::vector<CCoNLLOutputNode> {

public:
   CCoNLLOutput() {clear();}

public:
   void clear() {
      std::vector<CCoNLLOutputNode>::clear();
      push_back(CCoNLLOutputNode(0, "-NONE-", "_", "_", "-BEGIN-", "_", DEPENDENCY_LINK_NO_HEAD, "ROOT", DEPENDENCY_LINK_NO_HEAD, ""));
//      push_back(CCoNLLOutputNode(0, "", "", "-BEGIN-", "-BEGIN-", "", DEPENDENCY_LINK_NO_HEAD, "", DEPENDENCY_LINK_NO_HEAD, ""));
   }

   void copy(const CCoNLLOutput &input) {
      resize(input.size());
      for (unsigned i=0; i<input.size(); ++i) {
         at(i) = input.at(i);
      }
   }

   void fromCoNLLInput(const CCoNLLInput &input) {
      resize(input.size());
      for (unsigned i=0; i<input.size(); ++i) {
         static_cast<CCoNLLInputNode&>(at(i)) = input.at(i);
      }
   }

   void copyDependencyHeads(const CDependencyTree &input) {
      for (unsigned i=0; i<input.size(); ++i) {
         at(i).head = input.at(i).head;
      }
   }

   void copyDependencyHeads(const CLabeledDependencyTree &input) {
      for (unsigned i=0; i<input.size(); ++i) {
         at(i).head = input.at(i).head;
         at(i).label = input.at(i).label;
      }
   }

   void copyDependencyHeads(const CExtendedDependencyTree &input) {
      for (unsigned i=0; i<input.size(); ++i) {
         at(i).head = input.at(i).head;
      }
   }

   void copyDependencyHeads(const CExtendedLabeledDependencyTree &input) {
      for (unsigned i=0; i<input.size(); ++i) {
         at(i).head = input.at(i).head;
         at(i).label = input.at(i).label;
      }
   }

   void copyDependencyLabels(const CLabeledDependencyTree &input) {
      for (unsigned i=0; i<input.size(); ++i) {
         at(i).label = input.at(i).label;
      }
   }

   void copyTagsFeatsAndLemmas(const CExtendedDependencyTree &input) {
	      for (unsigned i=0; i<input.size(); ++i) {
	         at(i).tag = input.at(i).tag;
	      }
	      for (unsigned i=0; i<input.size(); ++i) {
	         at(i).feats = input.at(i).feats;
	      }
	      for (unsigned i=0; i<input.size(); ++i) {
	         at(i).lemma = input.at(i).lemma;
	      }
   }

   void copyTagsFeatsAndLemmas(const CExtendedLabeledDependencyTree &input) {
	      for (unsigned i=0; i<input.size(); ++i) {
	         at(i).tag = input.at(i).tag;
	      }
	      for (unsigned i=0; i<input.size(); ++i) {
	         at(i).feats = input.at(i).feats;
	      }
	      for (unsigned i=0; i<input.size(); ++i) {
	         at(i).lemma = input.at(i).lemma;
	      }
   }

   void toDependencyTree(CDependencyTree &out) const {
      out.clear();
      for (unsigned i=0; i<size(); ++i) {
         out.push_back(CDependencyTreeNode(at(i).word, at(i).tag, at(i).head));
      }
   }

   void toDependencyTree(CExtendedDependencyTree &out) const {
      out.clear();
      for (unsigned i=0; i<size(); ++i) {
         out.push_back(CExtendedDependencyTreeNode(at(i).word, at(i).tag, at(i).head, at(i).feats, at(i).lemma));
      }
   }

   void toDependencyTree(CLabeledDependencyTree &out) const {
      out.clear();
      for (unsigned i=0; i<size(); ++i) {
         out.push_back(CLabeledDependencyTreeNode(at(i).word, at(i).tag, at(i).head, at(i).label));
      }
   }

   void toDependencyTree(CExtendedLabeledDependencyTree &out) const {
      out.clear();
      for (unsigned i=0; i<size(); ++i) {
         out.push_back(CExtendedLabeledDependencyTreeNode(at(i).word, at(i).tag, at(i).head, at(i).feats, at(i).lemma, at(i).label));
      }
   }

   void toLabeledDependencyTree(CLabeledDependencyTree &out) const {
      out.clear();
      for (unsigned i=0; i<size(); ++i) {
    	  out.push_back(CLabeledDependencyTreeNode(at(i).word, at(i).tag, at(i).head, at(i).label));
      }
   }

   void toLabeledDependencyTree(CExtendedLabeledDependencyTree &out) const {
      out.clear();
      for (unsigned i=0; i<size(); ++i) {
    	  out.push_back(CExtendedLabeledDependencyTreeNode(at(i).word, at(i).tag, at(i).head, at(i).feats, at(i).lemma, at(i).label));
      }
   }



};

inline std::istream & operator >> (std::istream &is, CCoNLLOutput &sent) {
   sent.clear();
   std::string line;
   getline(is, line);

   while(is && !lstrip(line).empty())
   {
      CCoNLLOutputNode node;
      std::istringstream iss(rstrip(line));
      iss >> node ;
      sent.push_back( node );
      getline(is, line);
   }
   return is ;
}

inline std::ostream & operator << (std::ostream &os, const CCoNLLOutput &sent) {
   for (unsigned i=1; i<sent.size(); ++i)
      os << sent.at(i) << std::endl ;
   os << std::endl ;
   return os ;
}

/*==============================================================
 *
 * CCoNLLCPOS
 *
 *==============================================================*/

class CCoNLLCPOS : public CGenericTag {
public:
   CCoNLLCPOS() : CGenericTag() {}
   CCoNLLCPOS(const std::string &s) {load(s);}
   CCoNLLCPOS(const unsigned long &i) : CGenericTag(i) {}
   CCoNLLCPOS(const CCoNLLCPOS &c) : CGenericTag(c) {}
public:
   CGenericTagset &getTagset() const { static CGenericTagset tagset; return tagset; }
};

/*==============================================================
 *
 * CCoNLLFeats
 *
 *==============================================================*/

class CCoNLLFeats : public CGenericTag {
public:
   CCoNLLFeats() : CGenericTag() {}
   CCoNLLFeats(const std::string &s) {load(s);}
   CCoNLLFeats(const unsigned long &i) : CGenericTag(i) {}
   CCoNLLFeats(const CCoNLLFeats &c) : CGenericTag(c) {}
public:
   CGenericTagset &getTagset() const { static CGenericTagset tagset; return tagset; }
};

void readCoNLLFeats(std::vector<CCoNLLFeats> &output, const std::string &input);

#endif
