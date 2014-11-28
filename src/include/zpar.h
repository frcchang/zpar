#ifndef _ZPAR_H
#define _ZPAR_H

#ifdef __cplusplus
#include <cstddef>
#else
#include <stddef.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _ZPAR ZPAR;

#define __DECLARE_ZPAR_API(lang) \
ZPAR * zpar_open_##lang(  const char * sFeaturePath, const char * option);\
int zpar_close_##lang( ZPAR * zpar);\
char * zpar_process_##lang( ZPAR * zpar, const char * input, size_t * len);\
int zpar_release_##lang( char * output);\
int zpar_batch_##lang( ZPAR * zpar, const char * inputf, const char * outputf);\

__DECLARE_ZPAR_API(generic)
__DECLARE_ZPAR_API(chinese)
__DECLARE_ZPAR_API(english)
#undef __DECLARE_ZPAR_API

#ifdef __cplusplus
};
#endif

#endif
