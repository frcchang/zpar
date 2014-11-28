#ifndef _LIBZPAR_H
#define _LIBZPAR_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { POSTAGGER, CONPARSER, DEPPARSER, SEGMENTOR, CHARACTER, } task_type; 
typedef struct _ZPAR {
  void * tagger;
  void * parser;
  task_type task;
} ZPAR;

#ifdef __cplusplus
};
#endif

#endif
