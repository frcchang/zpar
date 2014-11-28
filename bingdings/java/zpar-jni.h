#include <jni.h>

#ifndef __ZPAR_JNI_H
#define __ZPAR_JNI_H
#include "zpar.h"

#ifdef __cplusplus
extern "C" {
#endif

  JNIEXPORT jlong JNICALL Java_ZPar_doOpen
    (JNIEnv * env, jobject obj, jstring _model, jstring _task, jstring _lang);

  JNIEXPORT jint JNICALL Java_ZPar_doClose
    (JNIEnv * env, jobject obj, jlong zpar);

  JNIEXPORT jstring JNICALL Java_ZPar_doProcess
    (JNIEnv * env, jobject obj, jlong zpar, jstring sent);

  JNIEXPORT jint JNICALL Java_ZPar_doBatch
    (JNIEnv * env, jobject obj, jlong zpar, jstring _inf, jstring _outf);

#ifdef __cplusplus
}
#endif

#endif
