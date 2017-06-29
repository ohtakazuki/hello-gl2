//
//  es20_chapter00_sample00.c
//  OpenGL_ES_20
//
//  Created by 山下 武志 on 2013/02/04.
//  Copyright (c) 2013年 山下 武志. All rights reserved.
//

#include <GLES2/gl2.h>
#include "../../support/support.h"

typedef struct {
    // レンダリング用シェーダープログラム
    GLint shader_program;

    // 位置情報属性
    GLint attr_pos;

    // 頂点カラー属性
    GLint attr_color;
} Extension_VertexColor;

/**
 * アプリの初期化を行う
 */
void sample_VertexColor_initialize(GLApplication *app) {
    // サンプルアプリ用のメモリを確保する
    app->extension = (Extension_VertexColor*) malloc(sizeof(Extension_VertexColor));
    // サンプルアプリ用データを取り出す
    Extension_VertexColor *extension = (Extension_VertexColor*) app->extension;

    // 頂点シェーダーを用意する
    {
        const GLchar *vertex_shader_source =
        //
                "attribute mediump vec4 attr_pos;"
                // 頂点カラー
                "attribute lowp vec4 attr_color;"
                "varying lowp vec4 vary_color;"
                "void main() {"
                "   gl_Position = attr_pos;"
                // 頂点カラー出力
                "   vary_color = attr_color;"
                "}";

        const GLchar *fragment_shader_source =
//
                "varying lowp vec4 vary_color;"
                "void main() {"
                "   gl_FragColor = vary_color;"
                "}";

        // コンパイルとリンクを行う
        extension->shader_program = Shader_createProgramFromSource(vertex_shader_source, fragment_shader_source);
    }

    // attributeを取り出す
    {
        extension->attr_pos = glGetAttribLocation(extension->shader_program, "attr_pos");
        assert(extension->attr_pos >= 0);

        extension->attr_color = glGetAttribLocation(extension->shader_program, "attr_color");
        assert(extension->attr_color >= 0);
    }

    // シェーダーの利用を開始する
    glUseProgram(extension->shader_program);
    assert(glGetError() == GL_NO_ERROR);

    // シェーダーで使用可能なベクトル数を取得する
    {
        GLint   vary_vectors = 0;
        glGetIntegerv(GL_MAX_VARYING_VECTORS, &vary_vectors);

        __logf("Max Varying Vectors / %d", vary_vectors);
    }
}

/**
 * レンダリングエリアが変更された
 */
void sample_VertexColor_resized(GLApplication *app) {
    // 描画領域を設定する
    glViewport(0, 0, app->surface_width, app->surface_height);
}

/**
 * アプリのレンダリングを行う
 * 毎秒60回前後呼び出される。
 */
void sample_VertexColor_rendering(GLApplication *app) {
    // サンプルアプリ用データを取り出す
    Extension_VertexColor *extension = (Extension_VertexColor*) app->extension;

    glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // attribute変数を有効にする
    glEnableVertexAttribArray(extension->attr_pos);
    glEnableVertexAttribArray(extension->attr_color);

    // 画面中央へ描画する
    const GLfloat position[] = {
    // v0
            0.0f, 1.0f,
            // v1
            1.0f, -1.0f,
            // v2
            -1.0f, -1.0f };

    // 頂点カラーを設定する
    const GLubyte color[] = {
            // v0 rgb
            255, 0, 0,
            // v1 rgb
            0, 255, 0,
            // v2 rgb
            0, 0, 255,
    };

    glVertexAttribPointer(extension->attr_pos, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*) position);
    glVertexAttribPointer(extension->attr_color, 3, GL_UNSIGNED_BYTE, GL_TRUE, 0, color);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // バックバッファをフロントバッファへ転送する。プラットフォームごとに内部の実装が異なる。
    ES20_postFrontBuffer(app);
}

/**
 * アプリのデータ削除を行う
 */
void sample_VertexColor_destroy(GLApplication *app) {
    // サンプルアプリ用データを取り出す
    Extension_VertexColor *extension = (Extension_VertexColor*) app->extension;

    // シェーダーの利用を終了する
    glUseProgram(0);
    assert(glGetError() == GL_NO_ERROR);

    // シェーダープログラムを廃棄する
    glDeleteProgram(extension->shader_program);
    assert(glGetError() == GL_NO_ERROR);

    // サンプルアプリ用のメモリを解放する
    free(app->extension);
}
