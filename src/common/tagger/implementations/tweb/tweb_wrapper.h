/****************************************************************
 *                                                              *
 * Wrapper for TWeb POS tagger.                                 *
 *                                                              *
 * Author: Jia Zhongye                                          *
 *                                                              *
 * SUTD, 2014.06                                                *
 *                                                              *
 ****************************************************************/

#ifndef _TWEB_WRAPPER
#define _TWEB_WRAPPER
#include <string>
#include <vector>

typedef std::vector<std::string> CStringVector;
typedef std::vector<std::pair<std::string, std::string> > CTwoStringVector;
//#include "linguistics/sentence_string.h"
namespace _tweb_impl
{
  struct _tweb_wrapper;
  _tweb_wrapper * _init_wrapper();
  void _load_model(_tweb_wrapper * tweb, const std::string &strModelPrefix);
  void _tag(_tweb_wrapper * tagger, CStringVector * sentence , CTwoStringVector * vReturn , int nBest = 0, double * out_scores = NULL);
  void _del_tagger(_tweb_wrapper * tweb);
}
#endif
