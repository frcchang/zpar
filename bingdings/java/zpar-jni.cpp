#include <cstdio>
#include <cstring>

#include <string>
#include "zpar-jni.h"

#ifdef __cplusplus
extern "C" {
#endif
  typedef ZPAR * (*zpar_open_func)(  const char * sFeaturePath, const char * option);
  typedef int (*zpar_close_func)( ZPAR * zpar);
  typedef char * (*zpar_process_func)( ZPAR * zpar, const char * input, size_t * len);
  typedef int (*zpar_release_func)( char * output);
  typedef int (*zpar_batch_func)( ZPAR * zpar, const char * inputf, const char * outputf);

  typedef struct _JZPAR
  {
    ZPAR * zpar;

    zpar_open_func open;
    zpar_close_func close;
    zpar_process_func process;
    zpar_release_func release;
    zpar_batch_func batch;

  } JZPAR;
#ifdef __cplusplus
};
#endif

JNIEXPORT jlong JNICALL Java_ZPar_doOpen(JNIEnv * env, jobject obj, jstring _model, jstring _task, jstring _lang)
{
  const char * model= env->GetStringUTFChars(_model, 0);
  const char * task = env->GetStringUTFChars(_task, 0);
  const char * __lang = env->GetStringUTFChars(_lang, 0);
  std::string lang = env->GetStringUTFChars(_lang, 0);

  JZPAR * jzpar = new JZPAR;
  if ( lang == "english" ) {
    jzpar->open = zpar_open_english;
    jzpar->close = zpar_close_english;
    jzpar->process = zpar_process_english;
    jzpar->release = zpar_release_english;
    jzpar->batch = zpar_batch_english;
  } else if ( lang == "generic" ) {
    jzpar->open = zpar_open_generic;
    jzpar->close = zpar_close_generic;
    jzpar->process = zpar_process_generic;
    jzpar->release = zpar_release_generic;
    jzpar->batch = zpar_batch_generic;
  } else if ( lang == "chinese" ) {
    jzpar->open = zpar_open_chinese;
    jzpar->close = zpar_close_chinese;
    jzpar->process = zpar_process_chinese;
    jzpar->release = zpar_release_chinese;
    jzpar->batch = zpar_batch_chinese;
  } else {
    delete jzpar;
    return 0;
  }
  jzpar->zpar = jzpar->open(model, task);

  env->ReleaseStringUTFChars(_model, model);
  env->ReleaseStringUTFChars(_task, task);
  env->ReleaseStringUTFChars(_lang, __lang);
  return (long)jzpar;
}

JNIEXPORT jint JNICALL Java_ZPar_doClose(JNIEnv * env, jobject obj, jlong zpar)
{
  JZPAR * jzpar = (JZPAR*)zpar;
  jint ret = jzpar->close(jzpar->zpar);
}

JNIEXPORT jstring JNICALL Java_ZPar_doProcess(JNIEnv * env, jobject obj, jlong zpar, jstring _sent)
{
  const char * sent = env->GetStringUTFChars(_sent, 0);
  size_t sz = strlen(sent);
  JZPAR * jzpar = (JZPAR*)zpar;
  char * _out = jzpar->process(jzpar->zpar, sent, &sz);
  jstring out = env->NewStringUTF(_out);
  jzpar->release(_out);
  env->ReleaseStringUTFChars(_sent, sent);
  return out;
}

JNIEXPORT jint JNICALL Java_ZPar_doBatch(JNIEnv * env, jobject obj, jlong zpar, jstring _inf, jstring _outf)
{
  const char * inf = env->GetStringUTFChars(_inf, 0);
  const char * outf = env->GetStringUTFChars(_outf, 0);
  JZPAR * jzpar = (JZPAR*)zpar;
  jint ret = jzpar->batch(jzpar->zpar, inf, outf);
  env->ReleaseStringUTFChars(_inf, inf);
  env->ReleaseStringUTFChars(_outf, outf);
  return ret;
}
