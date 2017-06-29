package com.android.gl2jni.data;

import android.content.Context;

import com.android.gl2jni.egl.EGLDevice;
import com.android.gl2jni.egl.EGLManager;

/**
 * Androidのプラットフォーム用データ
 */
public class AndroidPlatformData {
    /**
     * EGLサポートクラス
     */
    public EGLManager eglManager = null;

    /**
     * 画面に反映が行われるWindowデバイス
     * サンプルアプリの基本的な動作はこちらのデバイスを利用する
     */
    public EGLDevice windowDevice = null;

    /**
     * アプリコンテキスト（リソース読み込み用）
     */
    public Context context = null;
}
