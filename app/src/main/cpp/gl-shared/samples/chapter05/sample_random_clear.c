#include <time.h>
#include "../../support/support.h"


/**
 * アプリの初期化を行う
 */
void sample_RandomClear_initialize( GLApplication *app) {
    srand(time(NULL));
}

/**
 * レンダリングエリアが変更された
 */
void sample_RandomClear_resized( GLApplication *app) {
}

/**
 * アプリのレンダリングを行う
 * 毎秒60回前後呼び出される。
 */
void sample_RandomClear_rendering( GLApplication *app) {
    // 塗りつぶし色をランダムで変更する
    glClearColor((GLfloat)(rand() % 256) / 255.0f, (GLfloat)(rand() % 256) / 255.0f, (GLfloat)(rand() % 256) / 255.0f, 1.0f);
    
    // カラーバッファを単色で塗りつぶす。
    // 塗りつぶし色はglClearColor()で指定した色に依存する。
    glClear(GL_COLOR_BUFFER_BIT);

    // バックバッファをフロントバッファへ転送する。プラットフォームごとに内部の実装が異なる。
    ES20_postFrontBuffer(app);
}

/**
 * アプリのデータ削除を行う
 */
void sample_RandomClear_destroy( GLApplication *app) {
}
