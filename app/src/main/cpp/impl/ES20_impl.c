/*
 * ES20_impl.c
 *
 *  Created on: 2013/02/12
 */
#include <jni.h>
#include <string.h>
#include <GLES2/gl2.h>
#include "../support_ndk.h"
#include "signatures.h"
#include "../gl-shared/support/support.h"

/**
 * Class
 */
static jclass ES20_class = NULL;

/**
 * bind()メソッド
 */
static jmethodID method_bind = NULL;

/**
 * unbind()メソッド
 */
static jmethodID method_unbind = NULL;

/**
 * postFrontBuffer()メソッド
 */
static jmethodID method_postFrontBuffer = NULL;

/**
 * sjis2utf8()メソッド
 */
static jmethodID method_sjis2utf8 = NULL;

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_gles20_util_ES20_initializeNative(JNIEnv *env, jobject _this) {
    ndk_support_initialize(env);

    // load class object
    ES20_class = ndk_loadClass(env, ES20_CLASS_SIGNATURE, true);

    // load methods
    {
        method_bind = ndk_loadMethod(env, ES20_class, "bind", "(L" GLApplication_CLASS_SIGNATURE ";)I", true);
        method_unbind = ndk_loadMethod(env, ES20_class, "unbind", "(L"GLApplication_CLASS_SIGNATURE";)V", true);
        method_postFrontBuffer = ndk_loadMethod(env, ES20_class, "postFrontBuffer", "(L"GLApplication_CLASS_SIGNATURE";)V", true);
        method_sjis2utf8 = ndk_loadMethod(env, ES20_class, "sjis2utf8", "([B)[B", true);
    }
}

/**
 * ES20コンテキストを専有する.
 * 成功した場合はES20_NO_ERRORを返す。
 */
int ES20_bind(GLApplication *app) {
    NDKPlatform *platform = (NDKPlatform*) app->platform;
    JNIEnv *env = ndk_current_JNIEnv();
    return (*env)->CallStaticIntMethod(env, ES20_class, method_bind, platform->jGLApplication);
}

/**
 * ES20コンテキストを専有解除する
 */
void ES20_unbind(GLApplication *app) {
    NDKPlatform *platform = (NDKPlatform*) app->platform;
    JNIEnv *env = ndk_current_JNIEnv();
    (*env)->CallStaticVoidMethod(env, ES20_class, method_unbind, platform->jGLApplication);
}

/**
 * 描画結果を画面へ反映する。
 * この関数を呼び出すまで、画面には反映されない。
 */
void ES20_postFrontBuffer(GLApplication *app) {
    NDKPlatform *platform = (NDKPlatform*) app->platform;
    JNIEnv *env = ndk_current_JNIEnv();
    (*env)->CallStaticVoidMethod(env, ES20_class, method_postFrontBuffer, platform->jGLApplication);
}

/**
 * SJIS文字列をUTF8に変換する
 */
void ES20_sjis2utf8(GLchar *str, const int array_length) {
    const int str_len = strlen(str);
    if (!str_len) {
        return;
    }

    JNIEnv *env = ndk_current_JNIEnv();

    const jbyteArray array = (jbyteArray) (*env)->NewByteArray(env, str_len);
    // データをコピーする
    {
        (*env)->SetByteArrayRegion(env, array, 0, str_len, (jbyte*) str);
    }
    // データを変換する
    const jbyteArray cnv = (jbyteArray) (*env)->CallStaticObjectMethod(env, ES20_class, method_sjis2utf8, array);
    int env_len = (*env)->GetArrayLength(env, cnv);
    if (env_len >= array_length) {
        env_len = array_length - 1;
    }

    // データを書き戻す
    (*env)->GetByteArrayRegion(env, cnv, 0, env_len, str);

    // 配列を解放する
    (*env)->DeleteLocalRef(env, array);
    (*env)->DeleteLocalRef(env, cnv);

    // ターミネートする
    str[env_len] = '\0';
}
