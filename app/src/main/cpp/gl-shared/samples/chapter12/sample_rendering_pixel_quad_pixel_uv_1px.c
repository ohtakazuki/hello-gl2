//
//  es20_chapter00_sample00.c
//  OpenGL_ES_20
//
//  Created by 山下 武志 on 2013/02/04.
//  Copyright (c) 2013年 山下 武志. All rights reserved.
//

#include <GLES2/gl2.h>
#include "../../support/support_gl_Texture.h"
#include "../../support/support.h"

typedef struct {
    // レンダリング用シェーダープログラム
    GLuint shader_program;

    // 位置情報属性
    GLint attr_pos;

    // UV情報属性
    GLint attr_uv;

    // 描画対象のテクスチャ
    Texture *texture;
} Extension_RenderingPixelQuadPixelUv1px;

/**
 * アプリの初期化を行う
 */
void sample_RenderingPixelQuadPixelUv1px_initialize( GLApplication *app) {
    // サンプルアプリ用のメモリを確保する
    app->extension = (Extension_RenderingPixelQuadPixelUv1px*) malloc(sizeof(Extension_RenderingPixelQuadPixelUv1px));
    // サンプルアプリ用データを取り出す
    Extension_RenderingPixelQuadPixelUv1px *extension = (Extension_RenderingPixelQuadPixelUv1px*) app->extension;

    // 頂点シェーダーを用意する
    {
        // TODO 解説
        const GLchar *vertex_shader_source =
        //
                "attribute mediump vec4 attr_pos;"
                        "attribute mediump vec2 attr_uv;"
                        "varying mediump vec2 vary_uv;"
                        "void main() {"
                        "   gl_Position = attr_pos;"
                        "   vary_uv = attr_uv;"
                        "}";

        const GLchar *fragment_shader_source =
        //
                "uniform sampler2D texture;"
                        "varying mediump vec2 vary_uv;"
                        "void main() {"
                        "   gl_FragColor = texture2D(texture, vary_uv);"
                        "}";

        // コンパイルとリンクを行う
        extension->shader_program = Shader_createProgramFromSource(vertex_shader_source, fragment_shader_source);
    }

    // attributeを取り出す
    {
        extension->attr_pos = glGetAttribLocation(extension->shader_program, "attr_pos");
        assert(extension->attr_pos >= 0);

        extension->attr_uv = glGetAttribLocation(extension->shader_program, "attr_uv");
        assert(extension->attr_uv >= 0);
    }

    // Textureを読み込む
    {
        extension->texture = Texture_load(app, "texture_rgb_asymmetry.png", TEXTURE_RAW_RGBA8);
        assert(extension->texture != NULL);
    }

    // シェーダーの利用を開始する
    glUseProgram(extension->shader_program);
    assert(glGetError() == GL_NO_ERROR);
}

/**
 * レンダリングエリアが変更された
 */
void sample_RenderingPixelQuadPixelUv1px_resized( GLApplication *app) {
    // 描画領域を設定する
    glViewport(0, 0, app->surface_width, app->surface_height);
}

/**
 * アプリのレンダリングを行う
 * 毎秒60回前後呼び出される。
 */
void sample_RenderingPixelQuadPixelUv1px_rendering( GLApplication *app) {
    // サンプルアプリ用データを取り出す
    Extension_RenderingPixelQuadPixelUv1px *extension = (Extension_RenderingPixelQuadPixelUv1px*) app->extension;

    glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // attr_posを有効にする
    glEnableVertexAttribArray(extension->attr_pos);
    glEnableVertexAttribArray(extension->attr_uv);

// このブロックはカリングを含めて正しい順番で頂点が定義されていることをチェックします
#if 0   /* カリングチェック */
    {
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
    }
#endif  /* カリングチェック */

    // 左上パーツ描画
    {
        // TODO 解説
        // 四角形のXYWHを指定する
        const GLint pixel_x = 1;
        const GLint pixel_y = 1;
        const GLint pixel_width = extension->texture->width / 2;
        const GLint pixel_height = extension->texture->height / 2;

        const GLint tex_pixel_x = 0;
        const GLint tex_pixel_y = 0;
        const GLint tex_pixel_width = extension->texture->width / 2;
        const GLint tex_pixel_height = extension->texture->height / 2;

        GLfloat VERTEX_LEFT = ((GLfloat) pixel_x / (GLfloat) app->surface_width) * 2.0f - 1.0f;
        GLfloat VERTEX_TOP = ((GLfloat) pixel_y / (GLfloat) app->surface_height) * 2.0f - 1.0f;
        GLfloat VERTEX_RIGHT = VERTEX_LEFT + ((GLfloat) pixel_width / (GLfloat) app->surface_width) * 2.0f;
        GLfloat VERTEX_BOTTOM = VERTEX_TOP + ((GLfloat) pixel_height / (GLfloat) app->surface_height) * 2.0f;

        // 上下を反転させる top = -top;
        VERTEX_TOP = -VERTEX_TOP;
        VERTEX_BOTTOM = -VERTEX_BOTTOM;

        GLfloat UV_LEFT = (GLfloat) tex_pixel_x / (GLfloat) extension->texture->width;
        GLfloat UV_TOP = (GLfloat) tex_pixel_y / (GLfloat) extension->texture->height;
        GLfloat UV_RIGHT = UV_LEFT + (GLfloat) tex_pixel_width / (GLfloat) extension->texture->width;
        GLfloat UV_BOTTOM = UV_TOP + (GLfloat) tex_pixel_height / (GLfloat) extension->texture->height;

        // 左上へ四角形描画
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

        // TODO 解説
        // 指定したUV座標に変換する
        const GLfloat uv[] = {
        // triangle 0
        // v0(left top)
                UV_LEFT, UV_TOP,
                // v1(left bottom)
                UV_LEFT, UV_BOTTOM,
                // v2(right top)
                UV_RIGHT, UV_TOP,
                // v3(right bottom)
                UV_RIGHT, UV_BOTTOM, };

        glVertexAttribPointer(extension->attr_pos, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*) position);
        glVertexAttribPointer(extension->attr_uv, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*) uv);

        glBindTexture(GL_TEXTURE_2D, extension->texture->id);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }

    // 右上パーツ描画
    {
        // TODO 解説
        // 四角形のXYWHを指定する
        const GLint pixel_x = 1 + extension->texture->width / 2 + 1;
        const GLint pixel_y = 1;
        const GLint pixel_width = extension->texture->width / 2;
        const GLint pixel_height = extension->texture->height / 2;

        const GLint tex_pixel_x = extension->texture->width / 2;
        const GLint tex_pixel_y = 0;
        const GLint tex_pixel_width = extension->texture->width / 2;
        const GLint tex_pixel_height = extension->texture->height / 2;

        GLfloat VERTEX_LEFT = ((GLfloat) pixel_x / (GLfloat) app->surface_width) * 2.0f - 1.0f;
        GLfloat VERTEX_TOP = ((GLfloat) pixel_y / (GLfloat) app->surface_height) * 2.0f - 1.0f;
        GLfloat VERTEX_RIGHT = VERTEX_LEFT + ((GLfloat) pixel_width / (GLfloat) app->surface_width) * 2.0f;
        GLfloat VERTEX_BOTTOM = VERTEX_TOP + ((GLfloat) pixel_height / (GLfloat) app->surface_height) * 2.0f;

        // 上下を反転させる top = -top;
        VERTEX_TOP = -VERTEX_TOP;
        VERTEX_BOTTOM = -VERTEX_BOTTOM;

        GLfloat UV_LEFT = (GLfloat) tex_pixel_x / (GLfloat) extension->texture->width;
        GLfloat UV_TOP = (GLfloat) tex_pixel_y / (GLfloat) extension->texture->height;
        GLfloat UV_RIGHT = UV_LEFT + (GLfloat) tex_pixel_width / (GLfloat) extension->texture->width;
        GLfloat UV_BOTTOM = UV_TOP + (GLfloat) tex_pixel_height / (GLfloat) extension->texture->height;

        // 左上へ四角形描画
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

        // TODO 解説
        // 指定したUV座標に変換する
        const GLfloat uv[] = {
        // triangle 0
        // v0(left top)
                UV_LEFT, UV_TOP,
                // v1(left bottom)
                UV_LEFT, UV_BOTTOM,
                // v2(right top)
                UV_RIGHT, UV_TOP,
                // v3(right bottom)
                UV_RIGHT, UV_BOTTOM, };

        glVertexAttribPointer(extension->attr_pos, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*) position);
        glVertexAttribPointer(extension->attr_uv, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*) uv);

        glBindTexture(GL_TEXTURE_2D, extension->texture->id);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }

    // 左下パーツ描画
    {
        // TODO 解説
        // 四角形のXYWHを指定する
        const GLint pixel_x = 1;
        const GLint pixel_y = 1 + extension->texture->height / 2 + 1;
        const GLint pixel_width = extension->texture->width / 2;
        const GLint pixel_height = extension->texture->height / 2;

        const GLint tex_pixel_x = 0;
        const GLint tex_pixel_y = extension->texture->height / 2;
        const GLint tex_pixel_width = extension->texture->width / 2;
        const GLint tex_pixel_height = extension->texture->height / 2;

        GLfloat VERTEX_LEFT = ((GLfloat) pixel_x / (GLfloat) app->surface_width) * 2.0f - 1.0f;
        GLfloat VERTEX_TOP = ((GLfloat) pixel_y / (GLfloat) app->surface_height) * 2.0f - 1.0f;
        GLfloat VERTEX_RIGHT = VERTEX_LEFT + ((GLfloat) pixel_width / (GLfloat) app->surface_width) * 2.0f;
        GLfloat VERTEX_BOTTOM = VERTEX_TOP + ((GLfloat) pixel_height / (GLfloat) app->surface_height) * 2.0f;

        // 上下を反転させる top = -top;
        VERTEX_TOP = -VERTEX_TOP;
        VERTEX_BOTTOM = -VERTEX_BOTTOM;

        GLfloat UV_LEFT = (GLfloat) tex_pixel_x / (GLfloat) extension->texture->width;
        GLfloat UV_TOP = (GLfloat) tex_pixel_y / (GLfloat) extension->texture->height;
        GLfloat UV_RIGHT = UV_LEFT + (GLfloat) tex_pixel_width / (GLfloat) extension->texture->width;
        GLfloat UV_BOTTOM = UV_TOP + (GLfloat) tex_pixel_height / (GLfloat) extension->texture->height;

        // 左上へ四角形描画
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

        // TODO 解説
        // 指定したUV座標に変換する
        const GLfloat uv[] = {
        // triangle 0
        // v0(left top)
                UV_LEFT, UV_TOP,
                // v1(left bottom)
                UV_LEFT, UV_BOTTOM,
                // v2(right top)
                UV_RIGHT, UV_TOP,
                // v3(right bottom)
                UV_RIGHT, UV_BOTTOM, };

        glVertexAttribPointer(extension->attr_pos, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*) position);
        glVertexAttribPointer(extension->attr_uv, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*) uv);

        glBindTexture(GL_TEXTURE_2D, extension->texture->id);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }

    // 右上パーツ描画
    {
        // TODO 解説
        // 四角形のXYWHを指定する
        const GLint pixel_x = 1 + extension->texture->width / 2 + 1;
        const GLint pixel_y = 1 + extension->texture->height / 2 + 1;
        const GLint pixel_width = extension->texture->width / 2;
        const GLint pixel_height = extension->texture->height / 2;

        const GLint tex_pixel_x = extension->texture->width / 2;
        const GLint tex_pixel_y = extension->texture->height / 2;
        const GLint tex_pixel_width = extension->texture->width / 2;
        const GLint tex_pixel_height = extension->texture->height / 2;

        GLfloat VERTEX_LEFT = ((GLfloat) pixel_x / (GLfloat) app->surface_width) * 2.0f - 1.0f;
        GLfloat VERTEX_TOP = ((GLfloat) pixel_y / (GLfloat) app->surface_height) * 2.0f - 1.0f;
        GLfloat VERTEX_RIGHT = VERTEX_LEFT + ((GLfloat) pixel_width / (GLfloat) app->surface_width) * 2.0f;
        GLfloat VERTEX_BOTTOM = VERTEX_TOP + ((GLfloat) pixel_height / (GLfloat) app->surface_height) * 2.0f;

        // 上下を反転させる top = -top;
        VERTEX_TOP = -VERTEX_TOP;
        VERTEX_BOTTOM = -VERTEX_BOTTOM;

        GLfloat UV_LEFT = (GLfloat) tex_pixel_x / (GLfloat) extension->texture->width;
        GLfloat UV_TOP = (GLfloat) tex_pixel_y / (GLfloat) extension->texture->height;
        GLfloat UV_RIGHT = UV_LEFT + (GLfloat) tex_pixel_width / (GLfloat) extension->texture->width;
        GLfloat UV_BOTTOM = UV_TOP + (GLfloat) tex_pixel_height / (GLfloat) extension->texture->height;

        // 左上へ四角形描画
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

        // TODO 解説
        // 指定したUV座標に変換する
        const GLfloat uv[] = {
        // triangle 0
        // v0(left top)
                UV_LEFT, UV_TOP,
                // v1(left bottom)
                UV_LEFT, UV_BOTTOM,
                // v2(right top)
                UV_RIGHT, UV_TOP,
                // v3(right bottom)
                UV_RIGHT, UV_BOTTOM, };

        glVertexAttribPointer(extension->attr_pos, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*) position);
        glVertexAttribPointer(extension->attr_uv, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*) uv);

        glBindTexture(GL_TEXTURE_2D, extension->texture->id);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }

    // バックバッファをフロントバッファへ転送する。プラットフォームごとに内部の実装が異なる。
    ES20_postFrontBuffer(app);
}

/**
 * アプリのデータ削除を行う
 */
void sample_RenderingPixelQuadPixelUv1px_destroy( GLApplication *app) {
    // サンプルアプリ用データを取り出す
    Extension_RenderingPixelQuadPixelUv1px *extension = (Extension_RenderingPixelQuadPixelUv1px*) app->extension;

    // シェーダーの利用を終了する
    glUseProgram(0);
    assert(glGetError() == GL_NO_ERROR);

    // シェーダープログラムを廃棄する
    glDeleteProgram(extension->shader_program);
    assert(glGetError() == GL_NO_ERROR);

    glBindTexture(GL_TEXTURE_2D, 0);
    Texture_free(extension->texture);

    // サンプルアプリ用のメモリを解放する
    free(app->extension);
}
