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
class CSentenceTemplate : public vector<CSentenceNode> {

public:
   CSentenceTemplate() {};
   virtual ~CSentenceTemplate() {};

};

//==============================================================

template <typename CSentenceNode>
inline istream & operator >> (istream &is, CSentenceTemplate<CSentenceNode> &sent) {
   sent.clear();
   string line;
//   while(is && line.empty())
//      getline(is, line);
   getline(is, line);

   while(is && !line.empty())
   {
      CSentenceNode node;
      istringstream iss(line);
      iss >> node ;
      sent.push_back( node );
      getline(is, line);
   }
   return is ;
}

template <typename CSentenceNode>
inline ostream & operator << (ostream &os, const CSentenceTemplate<CSentenceNode> &sent) {
   for (int i=0; i<sent.size(); ++i)
      os << sent.at(i) << endl ;
   os << endl ;
   return os ;
}

#endif
