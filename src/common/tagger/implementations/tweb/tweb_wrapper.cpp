/****************************************************************
 *                                                              *
 * Wrapper for TWeb POS tagger.                                 *
 *                                                              *
 * Author: Jia Zhongye                                          *
 *                                                              *
 * SUTD, 2014.06                                                *
 *                                                              *
 ****************************************************************/

#include <string>
#include <iostream>
#include <sstream>
#include <algorithm>

#include "TWeb/src/EasyFirstPOS.h"
#include "TWeb/src/EnExtractor.h"
#include "TWeb/src/ChExtractor.h"
#include "TWeb/src/util.h"
#include "TWeb/src/Config.h"

#include "tweb_wrapper.h"

using namespace std;

void Load(const string &strPath, 
    CNNScorer &scorer,
    CTemplateManager &tempMgr,
    CBaseExtractor **pExtor);
namespace _tweb_impl
{
  static bool is_url( const wstring & in) {
    const static wstring head (L"http");
    const static wstring head_sep (L"://");
    auto ptr = begin(in);
    if ( in.compare(0, head.size(), head) == 0 ) {
      //cerr << "match: http.*" << endl;
      ptr += head.size();
      if ( ptr!= end(in) && *ptr == L's' ) {
        //cerr << "match: https.*" << endl;
        ptr++;
      }
      if ( in.compare(ptr-begin(in), head_sep.size(), head_sep) == 0 ) {
        //cerr << "match http[s]?://.*" << endl;
        ptr += head_sep.size();
        auto ptr_next = find_if(ptr, end(in), [&](wchar_t x) { return x == L':' || x == L'/'; });
        if ( ptr != end(in) && ptr_next != ptr ) {
          //cerr << "match http[s]?://[^:/]+" << endl;
          if ( ::iswalnum(*ptr ) ) {
            //cerr << "match http[s]?://[:alnum:][^:/]*" << endl;
            ptr++;
            if ( ptr != end(in)) {
              //cerr << "match http[s]?://[:alnum:][^:/].+" << endl;
              if ( ::isalnum(*(ptr_next-1)) ) {
                //cerr << "match http[s]?://[:alnum:]([^:/]*[:alnum:])?.*" << endl;
                if ( ptr_next != end(in) && *ptr_next == L':' ) {
                  ptr_next++;
                  //cerr << "match http[s]?://[:alnum:]([^:/]*[:alnum])?:.*" << endl;
                  auto ptr_next_next = find(ptr_next, end(in), L'/');
                  if ( ptr_next != ptr_next_next ) {
                    //cerr << "match http[s]?://[:alnum:]([^:/]*[:alnum:])?:.+" << endl;
                    ptr_next = find_if_not(ptr_next, ptr_next_next, ::isdigit);
                    if ( ptr_next != ptr_next_next ) {
                      return false;
                    }
                    //cerr << "match http[s]?://[:alnum:]([^:/]*[:alnum:])?:[:digit:]+" << endl;
                  } else {
                    return false;
                  }
                }
                //cerr << "match http[s]?://[:alnum:]([^:/]*[:alnum:])?(:[:digit:]+)?.*" << endl;
                if ( ptr_next == end(in) ) {
                  //cerr << "match http[s]?://[:alnum:]([^:/]*[:alnum:])?(:[:digit:]+)?" << endl;
                  return true;
                } else {
                  //cerr << "match http[s]?://[:alnum:]([^:/]*[:alnum:])?(:[:digit:]+)?.+" << endl;
                  if ( *ptr_next != L'/' ) {
                    return false;
                  }
                  //cerr << "match http[s]?://[:alnum:]([^:/]*[:alnum:])?(:[:digit:]+)?/.*" << endl;
                  ptr_next++;
                  return true;
                }
              }
            }
          }
        }
      }
    }
    return false;
  }
  static wstring _generalize(const wstring & in_word)
  {
    wstring word(in_word);
    transform(begin(word), end(word), begin(word), ::towlower);
    if ( is_url(word) ) {
      return L"url";
    }
    wstringstream ws;
    wstring::iterator pos_prev = begin(word);
    wstring::iterator pos = find_if(pos_prev, end(word), ::iswdigit );
    while ( pos != end(word) ) {
      for_each( pos_prev, pos, [&]( wchar_t x ){ ws << x ;});
      ws << L"#dg";
      pos_prev = find_if_not(pos, end(word), ::iswdigit );
      pos = find_if(pos_prev, end(word), ::iswdigit );
    }
    if ( pos_prev != end(word) ) {
      for_each( pos_prev, pos, [&]( wchar_t x ){ ws << x ;});
    }

    word = (ws.str());
    return word;
  }
  struct _tweb_wrapper
  {

    ::CRBMNN rbmNN;
    ::CNNScorer nnScorer;
    ::CWordTagManager lexicon;
    ::CTemplateManager tempMgr;
    ::CBaseExtractor *pExtor;
    ::CTagger tagger;
    ::CPool pool;
    _tweb_wrapper()
      : nnScorer(&rbmNN),
      pExtor(NULL),
      tagger(false, false)
    {}
    ~_tweb_wrapper()
    {
      delete tagger.GetExtractor();
    }
    void load_model(const string &strModelPrefix)
    {

      string strPrefix = strModelPrefix;
      if (strPrefix.back() == '.')
        strPrefix.erase(strPrefix.begin() + strPrefix.size() - 1);

      ::Load(strPrefix, nnScorer, tempMgr, &pExtor);
      tagger.Init(pExtor, &lexicon, &nnScorer, CConfig::nBS);
    }
    vector<_SEN *> _transform_sentence(const CStringVector & zpar_sen, CPool &rPool)
    {
      vector<wstring> lines( zpar_sen.size());
      transform( begin(zpar_sen), end(zpar_sen), begin(lines), ::utf82wstr);

      vector<_SEN*> senVec;

      _SEN *pSen = (_SEN *)rPool.Allocate(sizeof(_SEN));
      pSen->m_pNodes = (SSenNode **)rPool.Allocate(sizeof(SSenNode*) * lines.size());
      pSen->m_len	   = lines.size();

      CStrIDMap &wordIDMap  = CGlobalMap::s_WIDMap;
      CStrIDMap &gFormIDMap = CGlobalMap::s_GIDMap;
      //CStrIDMap &posIDMap   = CGlobalMap::s_POSIDMap; 
      for (size_t i = 0; i < lines.size(); ++i)
      {
        pSen->m_pNodes[i] = SSenNode::MakeNode(rPool);

        wstring gform = _generalize(lines[i]);
        pSen->m_pNodes[i]->m_pwzWord  = copyWcs(lines[i].c_str(),  &rPool);
        pSen->m_pNodes[i]->m_pwzGForm = copyWcs(gform.c_str(), &rPool);
        pSen->SetWID(i, wordIDMap.ID(lines[i].c_str()));
        pSen->SetGID(i, gFormIDMap.ID(gform.c_str()));
        //pSen->SetTag(i, posIDMap.ID(itemVec[_GTAG]));
      }

      senVec.push_back(pSen);
      return senVec;
    }
    void tag(CStringVector * sentence , CTwoStringVector * vReturn)
    {

      vReturn->clear();
      if ( sentence->size() == 0 ) {
        return;
      }
      vector<_SEN *> senVec = _transform_sentence(*sentence, pool);
      tagger.SetIMode(false);
      for (size_t i = 0; i < senVec.size(); ++i) {
        tagger.Tagging(senVec[i]);
      }
      for (size_t i = 0; i < senVec.size(); ++i)
      {
        _SEN *pRes = senVec[i];
        for (int k = 0; k < pRes->Length(); ++k)
          vReturn->push_back(make_pair(wstr2utf8(pRes->Word(k)), pRes->GForm(k) == wstring(L"url") ? "ADD" : wstr2utf8(pRes->Tag(k))));
      }

    }
  };

  _tweb_wrapper * _init_wrapper()
  {
    return new _tweb_wrapper();
  }
  void _load_model(_tweb_wrapper * tweb, const string &strModelPrefix)
  {
    tweb->load_model(strModelPrefix);
  }
  void _tag(_tweb_wrapper * tagger, CStringVector * sentence , CTwoStringVector * vReturn , int nBest, double * out_scores)
  {
    tagger->tag( sentence, vReturn);
  }
  void _del_tagger(_tweb_wrapper * tweb)
  {
    delete tweb;
  }

}
