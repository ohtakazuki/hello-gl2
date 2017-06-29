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
    GLuint shader_program;

    // 位置情報属性
    GLint attr_pos;
} Extension_RenderingPixelQuad;

/**
 * アプリの初期化を行う
 */
void sample_RenderingPixelQuad_initialize( GLApplication *app) {
    // サンプルアプリ用のメモリを確保する
    app->extension = (Extension_RenderingPixelQuad*) malloc(sizeof(Extension_RenderingPixelQuad));
    // サンプルアプリ用データを取り出す
    Extension_RenderingPixelQuad *extension = (Extension_RenderingPixelQuad*) app->extension;

    // 頂点シェーダーを用意する
    {
        const GLchar *vertex_shader_source =
                "attribute mediump vec4 attr_pos;"
                "void main() {"
                "   gl_Position = attr_pos;"
                "}";

        const GLchar *fragment_shader_source = "void main() {"
                "   gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);"
                "}";

        // コンパイルとリンクを行う
        extension->shader_program = Shader_createProgramFromSource(vertex_shader_source, fragment_shader_source);
    }

    // attributeを取り出す
    {
        extension->attr_pos = glGetAttribLocation(extension->shader_program, "attr_pos");
        assert(extension->attr_pos >= 0);
    }

    // シェーダーの利用を開始する
    glUseProgram(extension->shader_program);
    assert(glGetError() == GL_NO_ERROR);
}

/**
 * レンダリングエリアが変更された
 */
void sample_RenderingPixelQuad_resized( GLApplication *app) {
    // 描画領域を設定する
    glViewport(0, 0, app->surface_width, app->surface_height);
}

/**
 * アプリのレンダリングを行う
 * 毎秒60回前後呼び出される。
 */
void sample_RenderingPixelQuad_rendering( GLApplication *app) {
    // サンプルアプリ用データを取り出す
    Extension_RenderingPixelQuad *extension = (Extension_RenderingPixelQuad*) app->extension;

    glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // attr_posを有効にする
    glEnableVertexAttribArray(extension->attr_pos);

// このブロックはカリングを含めて正しい順番で頂点が定義されていることをチェックします
#if 0   /* カリングチェック */
    {
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
    }
#endif  /* カリングチェック */

    {
        // TODO 解説
        // 四角形のXYWHを指定する
        const GLint pixel_x = 50;
        const GLint pixel_y = 100;
        const GLint pixel_width = 200;
        const GLint pixel_height = 300;

        GLfloat VERTEX_LEFT = ((GLfloat) pixel_x / (GLfloat) app->surface_width) * 2.0f - 1.0f;
        GLfloat VERTEX_RIGHT = ((GLfloat) (pixel_x + pixel_width) / (GLfloat) app->surface_width) * 2.0f - 1.0f;

        GLfloat VERTEX_TOP = (((GLfloat) pixel_y / (GLfloat) app->surface_height) * 2.0f - 1.0f) * -1.0f;
        GLfloat VERTEX_BOTTOM = (((GLfloat) (pixel_y + pixel_height) / (GLfloat) app->surface_height) * 2.0f - 1.0f) * -1.0f;

        // 左上へ四角形描画
        const GLfloat position[] = {
        //
        // v1(left top)
                VERTEX_LEFT, VERTEX_TOP,
                // v2(left bottom)
                VERTEX_LEFT, VERTEX_BOTTOM,
                // v3(right top)
                VERTEX_RIGHT, VERTEX_TOP,
                // v4(right bottom)
                VERTEX_RIGHT, VERTEX_BOTTOM, };

        glVertexAttribPointer(extension->attr_pos, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*) position);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }
    {
        // TODO 解説
        // 四角形を右下に表示する
        const GLint pixel_width = 150;
        const GLint pixel_height = 100;
        const GLint pixel_x = app->surface_width - pixel_width - 1; // はみ出していないことを確認するため、１ピクセル横にずらす
        const GLint pixel_y = app->surface_height - pixel_height - 1; // はみ出していないことを確認するため、１ピクセル横にずらす

        GLfloat VERTEX_LEFT = ((GLfloat) pixel_x / (GLfloat) app->surface_width) * 2.0f - 1.0f;
        GLfloat VERTEX_RIGHT = ((GLfloat) (pixel_x + pixel_width) / (GLfloat) app->surface_width) * 2.0f - 1.0f;

        GLfloat VERTEX_TOP = (((GLfloat) pixel_y / (GLfloat) app->surface_height) * 2.0f - 1.0f) * -1.0f;
        GLfloat VERTEX_BOTTOM = (((GLfloat) (pixel_y + pixel_height) / (GLfloat) app->surface_height) * 2.0f - 1.0f) * -1.0f;

        // 右下へ四角形描画
        const GLfloat position[] = {
        //
        // v0(left top)
                VERTEX_LEFT, VERTEX_TOP,
                // v1(left bottom)
                VERTEX_LEFT, VERTEX_BOTTOM,
                // v2(right top)
                VERTEX_RIGHT, VERTEX_TOP,
                // v3(right bottom)
                VERTEX_RIGHT, VERTEX_BOTTOM, };

        glVertexAttribPointer(extension->attr_pos, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*) position);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }

    // バックバッファをフロントバッファへ転送する。プラットフォームごとに内部の実装が異なる。
    ES20_postFrontBuffer(app);
}

/**
 * アプリのデータ削除を行う
 */
void sample_RenderingPixelQuad_destroy( GLApplication *app) {
    // サンプルアプリ用データを取り出す
    Extension_RenderingPixelQuad *extension = (Extension_RenderingPixelQuad*) app->extension;

    // シェーダーの利用を終了する
    glUseProgram(0);
    assert(glGetError() == GL_NO_ERROR);

    // シェーダープログラムを廃棄する
    glDeleteProgram(extension->shader_program);
    assert(glGetError() == GL_NO_ERROR);

    // サンプルアプリ用のメモリを解放する
    free(app->extension);
}
