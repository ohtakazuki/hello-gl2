#include "../../support/support.h"


/**
 * アプリの初期化を行う
 */
void sample_Clear_initialize( GLApplication *app) {
}

/**
 * レンダリングエリアが変更された
 */
void sample_Clear_resized( GLApplication *app) {
}


/**
 * アプリのレンダリングを行う
 * 毎秒60回前後呼び出される。
 */
void sample_Clear_rendering( GLApplication *app) {
    // 塗りつぶし色をRGBAの順番で指定する。
    // 色は0.0f〜1.0fの間で指定する。一般的な0〜255の指定では無いことに留意する。
    glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
    
    // カラーバッファを単色で塗りつぶす。
    // 塗りつぶし色はglClearColor()で指定した色に依存する。
    glClear(GL_COLOR_BUFFER_BIT);


    // バックバッファをフロントバッファへ転送する。プラットフォームごとに内部の実装が異なる。
    ES20_postFrontBuffer(app);
}

/**
 * アプリのデータ削除を行う
 */
void sample_Clear_destroy( GLApplication *app) {
}
