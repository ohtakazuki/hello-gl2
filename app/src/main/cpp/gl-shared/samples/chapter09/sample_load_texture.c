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

} Extension_LoadTexture;

/**
 * アプリの初期化を行う
 */
void sample_LoadTexture_initialize(GLApplication *app) {
    // サンプルアプリ用のメモリを確保する
    app->extension = (Extension_LoadTexture*) malloc(sizeof(Extension_LoadTexture));
    // サンプルアプリ用データを取り出す
    Extension_LoadTexture *extension = (Extension_LoadTexture*) app->extension;

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
                "uniform sampler2D unif_texture;"
                        "varying mediump vec2 vary_uv;"
                        "void main() {"
                        "   gl_FragColor = texture2D(unif_texture, vary_uv);"
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
        extension->unif_texture = glGetUniformLocation(extension->shader_program, "unif_texture");
        assert(extension->unif_texture >= 0);
    }

    // テクスチャの生成を行う
    {
        glGenTextures(1, &extension->texture_id);
        assert(extension->texture_id != 0);
        assert(glGetError() == GL_NO_ERROR);

        // 現状ではおまじない扱い
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        assert(glGetError() == GL_NO_ERROR);

        glBindTexture(GL_TEXTURE_2D, extension->texture_id);
        assert(glGetError() == GL_NO_ERROR);

        // 画像ピクセルを読み込む
        {
            RawPixelImage *image = RawPixelImage_load(app, "texture_rgb_asymmetry.png", TEXTURE_RAW_RGBA8);
            // 正常に読み込まれたかをチェック
            assert(image != NULL);

            // VRAMへピクセル情報をコピーする
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->width, image->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image->pixel_data);
            assert(glGetError() == GL_NO_ERROR);

            // コピー後は不要になるため、ピクセル画素を解放する
            RawPixelImage_free(app, image);
        }

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        assert(glGetError() == GL_NO_ERROR);
    }

    // シェーダーの利用を開始する
    glUseProgram(extension->shader_program);
    assert(glGetError() == GL_NO_ERROR);

}

/**
 * レンダリングエリアが変更された
 */
void sample_LoadTexture_resized(GLApplication *app) {
    // 描画領域を設定する
    glViewport(0, 0, app->surface_width, app->surface_height);
}

/**
 * アプリのレンダリングを行う
 * 毎秒60回前後呼び出される。
 */
void sample_LoadTexture_rendering(GLApplication *app) {
    // サンプルアプリ用データを取り出す
    Extension_LoadTexture *extension = (Extension_LoadTexture*) app->extension;

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

    // 四角形描画
    {
        const GLfloat position[] = {
        // v0(left top)
                -0.75f, 0.75f,
                // v1(left bottom)
                -0.75f, -0.75f,
                // v2(right top)
                0.75f, 0.75f,
                // v3(right bottom)
                0.75f, -0.75f, };

        const GLfloat uv[] = {
        // v0(left top)
                0, 0,
                // v1(left bottom)
                0, 1,
                // v2(right top)
                1, 0,
                // v3(right bottom)
                1, 1, };

        glVertexAttribPointer(extension->attr_pos, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*) position);
        glVertexAttribPointer(extension->attr_uv, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*) uv);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }

    // バックバッファをフロントバッファへ転送する。プラットフォームごとに内部の実装が異なる。
    ES20_postFrontBuffer(app);
}

/**
 * アプリのデータ削除を行う
 */
void sample_LoadTexture_destroy(GLApplication *app) {
    // サンプルアプリ用データを取り出す
    Extension_LoadTexture *extension = (Extension_LoadTexture*) app->extension;

    // シェーダーの利用を終了する
    glUseProgram(0);
    assert(glGetError() == GL_NO_ERROR);

    // シェーダープログラムを廃棄する
    glDeleteProgram(extension->shader_program);
    assert(glGetError() == GL_NO_ERROR);

#if 1
    // テクスチャを廃棄する
    glBindTexture(GL_TEXTURE_2D, 0);
    glDeleteTextures(1, &extension->texture_id);
    assert(glGetError() == GL_NO_ERROR);
#else // glBindTextureを行わずに直接解放を行う
    // texture_idがテクスチャであることを検証する
    assert(glIsTexture(extension->texture_id) == GL_TRUE);
    // glBindTextureされているテクスチャがtexture_idであることを検証する
    {
        GLuint bindedTexture = -1;
        glGetIntegerv(GL_TEXTURE_BINDING_2D, &bindedTexture);
        assert(bindedTexture == extension->texture_id);
    }
    // アンバインドせずにテクスチャを廃棄する
    glDeleteTextures(1, &extension->texture_id);
    assert(glGetError() == GL_NO_ERROR);
    // テクスチャIDが無効であることをチェックする
    assert(glIsTexture(extension->texture_id) == GL_FALSE);
    // glBindTextureされているテクスチャが0であることを検証する
    {
        GLuint bindedTexture = -1;
        glGetIntegerv(GL_TEXTURE_BINDING_2D, &bindedTexture);
        assert(bindedTexture != extension->texture_id);
        assert(bindedTexture == 0);
    }
#endif

    // サンプルアプリ用のメモリを解放する
    free(app->extension);
}
