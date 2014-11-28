#include "definitions.h"
#include "tagger.h"
#include "conparser.h"
#include "depparser.h"
#include "reader.h"
#include "writer.h"
#include "stdlib.h"

#include "zpar.h"
#include "libzpar.h"

#define __DO_MACRO_CAT(fn, lang) fn ## _ ## lang
#define __DECLARE_ZPAR_API(fn, lang) __DO_MACRO_CAT( fn, lang)

#define MAX_SENTENCE_SIZE 512

#define __ZPAR_ALL_LANGUAGES_chinese 0
#define __ZPAR_ALL_LANGUAGES_english 1
#define __ZPAR_ALL_LANGUAGES_generic 2
#define IS_CHINESE(x) (__DO_MACRO_CAT(__ZPAR_ALL_LANGUAGES, x) == __ZPAR_ALL_LANGUAGES_chinese )
#define MACRO_CAT(x) __DO_MACRO_CAT(__ZPAR_ALL_LANGUAGES, x)

using namespace TARGET_LANGUAGE;

ZPAR * __DECLARE_ZPAR_API(zpar_open, TARGET_LANGUAGE)(  const char * sFeaturePath, const char * option)
{
  if ( option == NULL ) {
    return NULL;
  }
  if ( !( option[0] == 't'
        || option[0] == 'c'
        || option[0] == 'd'
#if IS_CHINESE(TARGET_LANGUAGE)
        || option[0] == 's'
        || option[0] == 'z'
#endif
        ) ) {
    return NULL;
  }
  ZPAR * zpar = new ZPAR;
  std::string base(sFeaturePath);

  if (option[0] == 'c'
#if IS_CHINESE(TARGET_LANGUAGE)
        || option[0] == 'z'
#endif
      )  {
#ifdef JOINT_CONPARSER
    CConParser * h = new CConParser(base+"/conparser", MAX_SENTENCE_SIZE, false);
#else
    CTagger * t = new CTagger(base+"/tagger", false);
    zpar->tagger = static_cast<void*>(t);
    CConParser * h = new CConParser(base+"/conparser", false);
#endif
    zpar->parser = static_cast<void*>(h);
    if ( option[0] == 'c' ) {
      zpar->task = CONPARSER;
    } else {
      zpar->task = CHARACTER;
    }
  } else if (option[0] == 'd' ) {
#ifdef JOINT_CONPARSER
    CConParser * t = new CConParser(base+"/conparser", MAX_SENTENCE_SIZE, false);
#else
    CTagger * t = new CTagger(base+"/tagger", false);
#endif
    zpar->tagger = static_cast<void*>(t);
    CDepParser * h = new CDepParser(base+"/depparser", false);
    zpar->parser = static_cast<void*>(h);
    zpar->task = DEPPARSER;
  } else if ( option[0] == 't' 
#if IS_CHINESE(TARGET_LANGUAGE)
        || option[0] == 's'
#endif
      ) {
#ifdef JOINT_CONPARSER
    CConParser * t = new CConParser(base+"/conparser", MAX_SENTENCE_SIZE, false);
#else
    CTagger * t = new CTagger(base+"/tagger", false);
#endif
    zpar->tagger = static_cast<void*>(t);
    if ( option[0] == 't' ) {
      zpar->task = POSTAGGER;
    } else {
      zpar->task = SEGMENTOR;
    }
  }

  return zpar;
}


int __DECLARE_ZPAR_API(zpar_close, TARGET_LANGUAGE)( ZPAR * zpar)
{
  if ( zpar == NULL ) {
    return -1;
  }
  switch ( zpar->task ) {
    case POSTAGGER:
#ifdef JOINT_CONPARSER
      delete static_cast<CConParser*>(zpar->tagger);
#else
      delete static_cast<CTagger*>(zpar->tagger);
#endif
      break;
    case CONPARSER:
      delete static_cast<CConParser*>(zpar->parser);
#ifndef JOINT_CONPARSER
      delete static_cast<CTagger*>(zpar->tagger);
#endif
      break;
    case DEPPARSER:
      delete static_cast<CDepParser*>(zpar->parser);
#ifdef JOINT_CONPARSER
      delete static_cast<CConParser*>(zpar->tagger);
#else
      delete static_cast<CTagger*>(zpar->tagger);
#endif
      break;
    default:
      return -1;
  }
  delete zpar;
  return 0;
}

static bool zpar_process_one( ZPAR * zpar, const char * input, size_t * _len, std::ostream & ss)
{
  std::istringstream iss(input);
  CStringVector input_sent;
#if IS_CHINESE(TARGET_LANGUAGE)
  CSentenceReader input_reader(&iss);
  input_reader.readRawSentence(&input_sent, true, false);
#else
  std::string word;
  while ( iss >> word ) {
    input_sent.push_back(word);
  }
#endif
  if ( zpar->task == POSTAGGER ) {
    CTwoStringVector tagged_sent; 
#ifdef JOINT_CONPARSER
    CConParser & conparser = *static_cast<CConParser*>(zpar->tagger);
    CJointTree conparsed_sent;
    conparser.parse(input_sent, &conparsed_sent, false);
    UnparseSentence(&conparsed_sent, &tagged_sent ) ;
#else
    CTagger & tagger = *static_cast<CTagger*>(zpar->tagger);
    tagger.tag(&input_sent, &tagged_sent);
#endif

    if ( tagged_sent.empty() ) {
      return true;
    }
    CTwoStringVector::const_iterator i = tagged_sent.begin();
    ss << i->first << "/" << i->second;
    for ( i++; i < tagged_sent.end(); i++) {
      ss << " " << i->first << "/" << i->second;
    }
  } else if ( zpar->task == CONPARSER
#ifdef JOINT_CONPARSER
      || zpar->task == CHARACTER
#endif
      ) {
#ifndef JOINT_CONPARSER
    CTagger & tagger = *static_cast<CTagger*>(zpar->tagger);
    CTwoStringVector tagged_sent; 
    tagger.tag(&input_sent, &tagged_sent);

    if ( tagged_sent.empty() ) {
      return true;
    }
    CConParser & conparser = *static_cast<CConParser*>(zpar->parser);
    CCFGTree conparsed_sent; 
    conparser.parse(tagged_sent, &conparsed_sent);
#else
    CConParser & conparser = *static_cast<CConParser*>(zpar->parser);
    CJointTree conparsed_sent;
    conparser.parse(input_sent, &conparsed_sent, 0, 1);
#endif
#ifdef JOINT_CONPARSER
    if ( zpar->task == CHARACTER ) {
      ss << conparsed_sent.str_unbinarizedall();
    } else 
#endif
    {
      ss << conparsed_sent.str_unbinarized();
    }
  } else if ( zpar->task == DEPPARSER ) {
    CTwoStringVector tagged_sent; 
#ifdef JOINT_CONPARSER
    CConParser & conparser = *static_cast<CConParser*>(zpar->tagger);
    CJointTree conparsed_sent;
    conparser.parse(input_sent, &conparsed_sent, false);
    UnparseSentence(&conparsed_sent, &tagged_sent ) ;
#else
    CTagger & tagger = *static_cast<CTagger*>(zpar->tagger);
    tagger.tag(&input_sent, &tagged_sent);
#endif

    if ( tagged_sent.empty() ) {
      return true;
    }
    CDepParser & depparser = *static_cast<CDepParser*>(zpar->parser);
    CLabeledDependencyTree depparseed_sent;
    depparser.parse(tagged_sent, &depparseed_sent);
    for ( int i = 0; i < depparseed_sent.size(); i++) {
      ss << depparseed_sent[i] << std::endl;
    }
#ifdef JOINT_CONPARSER
  } else if ( zpar->task == SEGMENTOR ) {
    CConParser & conparser = *static_cast<CConParser*>(zpar->tagger);
    CJointTree conparsed_sent;
    conparser.parse(input_sent, &conparsed_sent, false);
    CTwoStringVector tagged_sent; 
    UnparseSentence(&conparsed_sent, &tagged_sent ) ;
    CStringVector segment_sent; 
    UntagSentence( &tagged_sent, &segment_sent);
    CStringVector::const_iterator i = segment_sent.begin();
    ss << *i;
    for ( i++; i < segment_sent.end(); i++) {
      ss << " " << *i;
    }
#endif
  } else {
    return false;
  }
  return true;
}
char * __DECLARE_ZPAR_API(zpar_process, TARGET_LANGUAGE)( ZPAR * zpar, const char * input, size_t * _len)
{
  if ( zpar == NULL ) {
    return NULL;
  }
  if ( input == NULL ) {
    return NULL;
  }
  char * output = NULL;
  const size_t len = *_len;
  if ( len == 0 ) {
    output = new char[1];
    output[0] = '\0';
    return output;
  }
  std::stringstream ss;
  bool ret = zpar_process_one(zpar, input, _len, ss);
  if ( not ret) {
    return NULL;
  }
  ss.seekg(0, ss.end);
  size_t outlen = ss.tellg();
  output = new char[outlen+1];
  ss.seekg(0, ss.beg);
  ss.read(output, outlen);
  output[outlen] = '\0';
  *_len = outlen;
  return output;
}

int __DECLARE_ZPAR_API(zpar_release, TARGET_LANGUAGE)( char * output)
{
  if ( output == NULL ) {
    return -1;
  }
  delete [] output;
  return 0;
}

int __DECLARE_ZPAR_API(zpar_batch, TARGET_LANGUAGE)( ZPAR * zpar, const char * inputf, const char * outputf)
{
  if ( zpar == NULL ) {
    return -1;
  }
  if ( inputf == NULL ) {
    return -1;
  }
  if ( outputf == NULL ) {
    return -1;
  }
  std::ifstream ifs(inputf);
  std::ofstream ofs(outputf);
  std::string line;
  int line_nr = 0;
  while ( getline( ifs, line) ) {

    if ( line.empty() ) {
      ofs << std::endl;
      line_nr++;
      continue;
    }

    size_t sz = line.length();
    bool ret = zpar_process_one(zpar, line.c_str(), &sz, ofs);
    if ( not ret ) {
      return line_nr;
    }
    ofs << std::endl;

    line_nr++;
  }
  return line_nr;
}

