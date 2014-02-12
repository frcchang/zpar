// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                              *
 * sentence_template.h - the general template for a sentence    *
 *                       that has the format of CONLL data      *
 *                                                              *
 * Yue Zhang                                                    *
 *                                                              *
 * Oxford University Computing Laboratory. 2008.3.              *
 *                                                              *
 ****************************************************************/

#ifndef _SENTENCE_TEMPLATE_H
#define _SENTENCE_TEMPLATE_H

/*==============================================================
 *
 * CSentenceTemplate
 *
 *==============================================================*/

template<typename CSentenceNode>
class CSentenceTemplate : public std::vector<CSentenceNode> {

public:
   CSentenceTemplate() {}
   virtual ~CSentenceTemplate() {}
};

//==============================================================

template <typename CSentenceNode>
inline std::istream & operator >> (std::istream &is, CSentenceTemplate<CSentenceNode> &sent) {
   sent.clear();
   std::string line;
   getline(is, line);

   while(is && !line.empty())
   {
//      std::cout << line << line.empty() << std::endl;
      CSentenceNode node;
      std::istringstream iss(line);
      iss >> node ;
      sent.push_back( node );
      getline(is, line);
   }
   return is ;
}

template <typename CSentenceNode>
inline std::ostream & operator << (std::ostream &os, const CSentenceTemplate<CSentenceNode> &sent) {
   for (unsigned i=0; i<sent.size(); ++i)
      os << sent.at(i) << std::endl ;
   os << std::endl ;
   return os ;
}

#endif
