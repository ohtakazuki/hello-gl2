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

    // UV情報属性
    GLint attr_uv;

    // テクスチャのUniform
    GLint unif_texture;

    /**
     * 読み込んだテクスチャ
     */
    GLenum texture_id[2];
} Extension_TextureUse2;

/**
 * アプリの初期化を行う
 */
void sample_TextureUse2_initialize(GLApplication *app) {
    // サンプルアプリ用のメモリを確保する
    app->extension = (Extension_TextureUse2*) malloc(sizeof(Extension_TextureUse2));
    // サンプルアプリ用データを取り出す
    Extension_TextureUse2 *extension = (Extension_TextureUse2*) app->extension;

    // 頂点シェーダーを用意する
    {
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

    // uniformを取り出す
    {
        extension->unif_texture = glGetUniformLocation(extension->shader_program, "texture");
        assert(extension->unif_texture >= 0);
    }

    {
#if 1
        // 2枚一気にテクスチャを作成する
        glGenTextures(2, extension->texture_id);
#else
        // for文で記述することも出来る
        {
            int i = 0;
            for (i = 0; i < 2; ++i) {
                glGenTextures(1, &extension->texture_id[i]);
            }
        }
#endif

        assert(extension->texture_id[0] != 0);
        assert(extension->texture_id[1] != 0);
        assert(glGetError() == GL_NO_ERROR);

        // 画像Aを読み込む
        // RGBAテクスチャとして読み込む
        {
            glBindTexture(GL_TEXTURE_2D, extension->texture_id[0]);
            {
                int mipmap_level = 0;
                const char *file_names[] = {
                //
                // mipmap level 0
                        "texture_rgb_512x512.png",
                        // mipmap level 1
                        "texture_rgb_256x256.png",
                        // mipmap level 2
                        "texture_rgb_128x128.png",
                        // mipmap level 3
                        "texture_rgb_64x64.png",
                        // mipmap level 4
                        "texture_rgb_32x32.png",
                        // mipmap level 5
                        "texture_rgb_16x16.png",
                        // mipmap level 6
                        "texture_rgb_8x8.png",
                        // mipmap level 7
                        "texture_rgb_4x4.png",
                        // mipmap level 8
                        "texture_rgb_2x2.png",
                        // mipmap level 9
                        "texture_rgb_1x1.png", };

                for (mipmap_level = 0; mipmap_level < 10; ++mipmap_level) {
                    RawPixelImage *image = NULL;
                    image = RawPixelImage_load(app, file_names[mipmap_level], TEXTURE_RAW_RGBA8);
                    // 正常に読み込まれたかをチェック
                    assert(image != NULL);

                    // VRAMへピクセル情報をコピーする
                    glTexImage2D(GL_TEXTURE_2D, mipmap_level, GL_RGBA, image->width, image->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image->pixel_data);
                    assert(glGetError() == GL_NO_ERROR);

                    // コピー後は不要になるため、ピクセル画素を解放する
                    RawPixelImage_free(app, image);
                }

            }

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            assert(glGetError() == GL_NO_ERROR);

        }

        // 画像Bを読み込む
        // RGBテクスチャとして読み込む
        {
            glBindTexture(GL_TEXTURE_2D, extension->texture_id[1]);
            {
                assert(glGetError() == GL_NO_ERROR);
                RawPixelImage *image = NULL;
                image = RawPixelImage_load(app, "texture_rgb_300x400.png", TEXTURE_RAW_RGBA8);
                // 正常に読み込まれたかをチェック
                assert(image != NULL);

                // VRAMへピクセル情報をコピーする
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->width, image->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image->pixel_data);
                assert(glGetError() == GL_NO_ERROR);

                // コピー後は不要になるため、ピクセル画素を解放する
                RawPixelImage_free(app, image);
            }

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            assert(glGetError() == GL_NO_ERROR);

        }

    }

    // シェーダーの利用を開始する
    glUseProgram(extension->shader_program);
    assert(glGetError() == GL_NO_ERROR);

}

/**
 * レンダリングエリアが変更された
 */
void sample_TextureUse2_resized(GLApplication *app) {
    // 描画領域を設定する
    glViewport(0, 0, app->surface_width, app->surface_height);
}

/**
 * アプリのレンダリングを行う
 * 毎秒60回前後呼び出される。
 */
void sample_TextureUse2_rendering(GLApplication *app) {
    // サンプルアプリ用データを取り出す
    Extension_TextureUse2 *extension = (Extension_TextureUse2*) app->extension;

    glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // attr_posを有効にする
    glEnableVertexAttribArray(extension->attr_pos);
    glEnableVertexAttribArray(extension->attr_uv);
    // unif_textureへテクスチャを設定する
    glUniform1i(extension->unif_texture, 0);

// このブロックはカリングを含めて正しい順番で頂点が定義されていることをチェックします
#if 0   /* カリングチェック */
    {
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
    }
#endif  /* カリングチェック */

    {
        // 左のポリゴンを描画
        {
            const GLfloat position[] = {
            // triangle 0
            // v0(left top)
                    -0.95f, 0.75f,
                    // v1(left bottom)
                    -0.95f, -0.75f,
                    // v2(right top)
                    -0.05f, 0.75f,
                    // v3(right bottom)
                    -0.05f, -0.75f, };

            // 0.0〜1.0の範囲を設定
            const GLfloat uv[] = {
            // v0(left top)
                    0.0f, 0.0f,
                    // v1(left bottom)
                    0.0f, 1.0f,
                    // v2(right top)
                    1.0f, 0.0f,
                    // v3(right bottom)
                    1.0f, 1.0f, };

            glVertexAttribPointer(extension->attr_uv, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*) uv);
            glVertexAttribPointer(extension->attr_pos, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*) position);

            // テクスチャを関連付ける
            glBindTexture(GL_TEXTURE_2D, extension->texture_id[0]);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        }

        // 右へポリゴンを描画
        {
            const GLfloat position[] = {
            // v0(left top)
                    0.05f, 0.75f,
                    // v1(left bottom)
                    0.05f, -0.75f,
                    // v2(right top)
                    0.95f, 0.75f,
                    // v3(right bottom)
                    0.95f, -0.75f, };

            // -1.0f〜2.0fの範囲を設定
            const GLfloat uv[] = {
            // v0(left top)
                    -1.0f, -1.0f,
                    // v1(left bottom)
                    -1.0f, 2.0f,
                    // v2(right top)
                    2.0f, -1.0f,
                    // v3(right bottom)
                    2.0f, 2.0f, };

            glVertexAttribPointer(extension->attr_uv, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*) uv);
            glVertexAttribPointer(extension->attr_pos, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*) position);
            // テクスチャを関連付ける
            glBindTexture(GL_TEXTURE_2D, extension->texture_id[1]);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        }
    }

    // バックバッファをフロントバッファへ転送する。プラットフォームごとに内部の実装が異なる。
    ES20_postFrontBuffer(app);
}

/**
 * アプリのデータ削除を行う
 */
void sample_TextureUse2_destroy(GLApplication *app) {
    // サンプルアプリ用データを取り出す
    Extension_TextureUse2 *extension = (Extension_TextureUse2*) app->extension;

    // シェーダーの利用を終了する
    glUseProgram(0);
    assert(glGetError() == GL_NO_ERROR);

    // シェーダープログラムを廃棄する
    glDeleteProgram(extension->shader_program);
    assert(glGetError() == GL_NO_ERROR);

#if 0
    // テクスチャを解放
    glDeleteTextures(2, extension->texture_id);
#else
    int i = 0;
    for (i = 0; i < 2; ++i) {
        glDeleteTextures(1, &extension->texture_id[i]);
    }
#endif

    // サンプルアプリ用のメモリを解放する
    free(app->extension);
}
