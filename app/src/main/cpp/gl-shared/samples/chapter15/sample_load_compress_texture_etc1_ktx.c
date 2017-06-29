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

    // 読み込んだテクスチャ
    GLuint texture_id;
} Extension_LoadCompressTextureEtc1Ktx;

/**
 * アプリの初期化を行う
 */
void sample_LoadCompressTextureEtc1Ktx_initialize( GLApplication *app) {
    // 拡張機能チェック
    if (!ES20_hasExtension("GL_OES_compressed_ETC1_RGB8_texture")) {
        GLApplication_abortWithMessage(app, "非対応の拡張機能です。\nGL_OES_compressed_ETC1_RGB8_texture");
        return;
    }

    // サンプルアプリ用のメモリを確保する
    app->extension = (Extension_LoadCompressTextureEtc1Ktx*) malloc(sizeof(Extension_LoadCompressTextureEtc1Ktx));
    // サンプルアプリ用データを取り出す
    Extension_LoadCompressTextureEtc1Ktx *extension = (Extension_LoadCompressTextureEtc1Ktx*) app->extension;

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
        glGenTextures(1, &extension->texture_id);
        assert(extension->texture_id != 0);
        assert(glGetError() == GL_NO_ERROR);

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        assert(glGetError() == GL_NO_ERROR);
        glBindTexture(GL_TEXTURE_2D, extension->texture_id);
        assert(glGetError() == GL_NO_ERROR);

        // 画像ピクセルを読み込む
        {
            // ETC1が保存されたKTXファイルを読み込む
            KtxImage *ktx = KtxImage_load(app, "texture_etc1_512x512.ktx");
            // 正常に読み込まれたかをチェック
            assert(ktx != NULL);

            // VRAMへピクセル情報をアップロードする
            int miplevel = 0;
            int width = ktx->width;
            int height = ktx->height;
            for (miplevel = 0; miplevel < ktx->mipmaps; ++miplevel) {
                int image_size = ktx->image_length_table[miplevel];
                void* image_ptr = ktx->image_table[miplevel];
                glCompressedTexImage2D(GL_TEXTURE_2D, miplevel, ktx->format, width, height, 0, image_size, image_ptr);
                // 幅高さを半分にする
                width /= 2;
                height /= 2;
            }
            assert(glGetError() == GL_NO_ERROR);

            // コピー後は不要になるため、ピクセル画素を解放する
            KtxImage_free(app, ktx);
        }

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
        assert(glGetError() == GL_NO_ERROR);

        // テクスチャ操作が終わったため、バインドを元に戻す
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    // シェーダーの利用を開始する
    glUseProgram(extension->shader_program);
    assert(glGetError() == GL_NO_ERROR);

    // ブレンドを有効
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

/**
 * レンダリングエリアが変更された
 */
void sample_LoadCompressTextureEtc1Ktx_resized( GLApplication *app) {
    // 描画領域を設定する
    glViewport(0, 0, app->surface_width, app->surface_height);
}

/**
 * アプリのレンダリングを行う
 * 毎秒60回前後呼び出される。
 */
void sample_LoadCompressTextureEtc1Ktx_rendering( GLApplication *app) {
    // サンプルアプリ用データを取り出す
    Extension_LoadCompressTextureEtc1Ktx *extension = (Extension_LoadCompressTextureEtc1Ktx*) app->extension;

    glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // attr_posを有効にする
    glEnableVertexAttribArray(extension->attr_pos);
    glEnableVertexAttribArray(extension->attr_uv);
    // unif_textureへテクスチャを設定する
    glBindTexture(GL_TEXTURE_2D, extension->texture_id);
    glUniform1i(extension->unif_texture, 0);


    // 四角形描画
    {
        const GLfloat position[] = {
        // triangle 0
        // v0(left top)
                -0.75f, 0.75f,
                // v1(left bottom)
                -0.75f, -0.75f,
                // v2(right top)
                0.75f, 0.75f,

                // triangle 1
                // v2(right top)
                0.75f, 0.75f,
                // v1(left bottom)
                -0.75f, -0.75f,
                // v3(right bottom)
                0.75f, -0.75f, };

        // -1.0f〜2.0fの範囲を設定してレンダリング結果をチェックさせる
        const GLfloat uv[] = {
        // triangle 0
        // v0(left top)
                -1.0f, -1.0f,
                // v1(left bottom)
                -1.0f, 2.0f,
                // v2(right top)
                2.0f, -1.0f,

                // triangle 1
                // v2(right top)
                2.0f, -1.0f,
                // v1(left bottom)
                -1.0f, 2.0f,
                // v3(right bottom)
                2.0f, 2.0f, };

        glVertexAttribPointer(extension->attr_pos, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*) position);
        glVertexAttribPointer(extension->attr_uv, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*) uv);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }


    // バックバッファをフロントバッファへ転送する。プラットフォームごとに内部の実装が異なる。
    ES20_postFrontBuffer(app);
}

/**
 * アプリのデータ削除を行う
 */
void sample_LoadCompressTextureEtc1Ktx_destroy( GLApplication *app) {

    // TODO 解説
    // 中断されている場合は何もしない
    if (GLApplication_isAbort(app)) {
        return;
    }

    // サンプルアプリ用データを取り出す
    Extension_LoadCompressTextureEtc1Ktx *extension = (Extension_LoadCompressTextureEtc1Ktx*) app->extension;

    // シェーダーの利用を終了する
    glUseProgram(0);
    assert(glGetError() == GL_NO_ERROR);

    // シェーダープログラムを廃棄する
    glDeleteProgram(extension->shader_program);
    assert(glGetError() == GL_NO_ERROR);

    // テクスチャを廃棄する
    glBindTexture(GL_TEXTURE_2D, 0);
    glDeleteTextures(1, &extension->texture_id);
    assert(glGetError() == GL_NO_ERROR);

    // サンプルアプリ用のメモリを解放する
    free(app->extension);
}
