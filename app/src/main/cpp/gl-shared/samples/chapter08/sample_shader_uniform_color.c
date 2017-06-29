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

    // フラグメントシェーダの描画色
    GLint unif_color;
} Extension_ShaderUniformColor;

/**
 * アプリの初期化を行う
 */
void sample_ShaderUniformColor_initialize(GLApplication *app) {
    // サンプルアプリ用のメモリを確保する
    app->extension = (Extension_ShaderUniformColor*) malloc(sizeof(Extension_ShaderUniformColor));
    // サンプルアプリ用データを取り出す
    Extension_ShaderUniformColor *extension = (Extension_ShaderUniformColor*) app->extension;

    // 頂点シェーダーを用意する
    {
        const GLchar *vertex_shader_source =
                "attribute mediump vec4 attr_pos;"
                "void main() {"
                "   gl_Position = attr_pos;"
                "}";

        const GLchar *fragment_shader_source =
                "uniform lowp vec4 unif_color;"
                "void main() {"
                "   gl_FragColor = unif_color;"
                "}";

        // コンパイルとリンクを行う
        extension->shader_program = Shader_createProgramFromSource(vertex_shader_source, fragment_shader_source);
    }

    // attributeを取り出す
    {
        extension->attr_pos = glGetAttribLocation(extension->shader_program, "attr_pos");
        assert(extension->attr_pos >= 0);
    }

    // uniform変数のlocationを取得する
    {
        extension->unif_color = glGetUniformLocation(extension->shader_program, "unif_color");
        assert(extension->unif_color >= 0);
    }

    // シェーダーの利用を開始する
    glUseProgram(extension->shader_program);
    assert(glGetError() == GL_NO_ERROR);


    // シェーダーで使用可能なベクトル数を取得する
    {
        GLint   vert_vectors = 0;
        GLint   frag_vectors = 0;
        glGetIntegerv(GL_MAX_VERTEX_UNIFORM_VECTORS, &vert_vectors);
        glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_VECTORS, &frag_vectors);

        __logf("Max Uniform Vectors / Vert(%d) Frag(%d)", vert_vectors, frag_vectors);
    }
}

/**
 * レンダリングエリアが変更された
 */
void sample_ShaderUniformColor_resized(GLApplication *app) {
    // 描画領域を設定する
    glViewport(0, 0, app->surface_width, app->surface_height);
}

/**
 * アプリのレンダリングを行う
 * 毎秒60回前後呼び出される。
 */
void sample_ShaderUniformColor_rendering(GLApplication *app) {
    // サンプルアプリ用データを取り出す
    Extension_ShaderUniformColor *extension = (Extension_ShaderUniformColor*) app->extension;

    glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // attr_posを有効にする
    glEnableVertexAttribArray(extension->attr_pos);

    // 左上へ四角形描画
    {
        const GLfloat position[] = {
                // v0(left top)
                -0.75f, 0.75f,
                // v1(left bottom)
                -0.75f, 0.25f,
                // v2(right top)
                -0.25f, 0.75f,
                // v3(right bottom)
                -0.25f, 0.25f, };

        // 描画色を指定する
        {
            //                                   R,    G,    B,    A
            glUniform4f(extension->unif_color, 1.0f, 0.0f, 1.0f, 1.0f);
        }
        glVertexAttribPointer(extension->attr_pos, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*) position);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }

    // 右上へ四角形描画
    {
        const GLfloat position[] = {
                // v4(left top)
                0.25f, 0.75f,
                // v5(left bottom)
                0.25f, 0.25f,
                // v6(right top)
                0.75f, 0.75f,
                // v7(right bottom)
                0.75f, 0.25f, };

        // 描画色を配列指定する
        {
            const GLfloat poly_color[] = {
                    // R
                    1.0f,
                    // G
                    1.0f,
                    // B
                    0.0f,
                    // A
                    1.0f, };
            glUniform4fv(extension->unif_color, 1, poly_color);
        }

        glVertexAttribPointer(extension->attr_pos, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*) position);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }

    // バックバッファをフロントバッファへ転送する。プラットフォームごとに内部の実装が異なる。
    ES20_postFrontBuffer(app);
}

/**
 * アプリのデータ削除を行う
 */
void sample_ShaderUniformColor_destroy(GLApplication *app) {
    // サンプルアプリ用データを取り出す
    Extension_ShaderUniformColor *extension = (Extension_ShaderUniformColor*) app->extension;

    // シェーダーの利用を終了する
    glUseProgram(0);
    assert(glGetError() == GL_NO_ERROR);

    // シェーダープログラムを廃棄する
    glDeleteProgram(extension->shader_program);
    assert(glGetError() == GL_NO_ERROR);

    // サンプルアプリ用のメモリを解放する
    free(app->extension);
}
