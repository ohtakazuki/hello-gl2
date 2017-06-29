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

    // ポリゴンの表示サイズ
    float polygon_size;

} Extension_TextureOpMipmapGen;

/**
 * アプリの初期化を行う
 */
void sample_TextureOpMipmapGen_initialize( GLApplication *app) {
    // サンプルアプリ用のメモリを確保する
    app->extension = (Extension_TextureOpMipmapGen*) malloc(sizeof(Extension_TextureOpMipmapGen));
    // サンプルアプリ用データを取り出す
    Extension_TextureOpMipmapGen *extension = (Extension_TextureOpMipmapGen*) app->extension;

    // ポリゴンの大きさを指定する
    {
        extension->polygon_size = 0.5f;
    }

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

    // テクスチャの生成
    {
        glGenTextures(1, &extension->texture_id);
        assert(extension->texture_id != 0);
        assert(glGetError() == GL_NO_ERROR);

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        assert(glGetError() == GL_NO_ERROR);
        glBindTexture(GL_TEXTURE_2D, extension->texture_id);
        assert(glGetError() == GL_NO_ERROR);

        // Mipmapレベル0のテクスチャのみを読み込む
        {
            RawPixelImage *image = NULL;
            image = RawPixelImage_load(app, "texture_rgb_512x512.png", TEXTURE_RAW_RGBA8);
            // 正常に読み込まれたかをチェック
            assert(image != NULL);

            // VRAMへピクセル情報をコピーする
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->width, image->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image->pixel_data);
            assert(glGetError() == GL_NO_ERROR);

            // コピー後は不要になるため、ピクセル画素を解放する
            RawPixelImage_free(app, image);
        }
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        // フィルタをMIPMAP対応に変更する
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        assert(glGetError() == GL_NO_ERROR);

        // mipmapを自動生成する
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    // シェーダーの利用を開始する
    glUseProgram(extension->shader_program);
    assert(glGetError() == GL_NO_ERROR);

}

/**
 * レンダリングエリアが変更された
 */
void sample_TextureOpMipmapGen_resized( GLApplication *app) {
    // 描画領域を設定する
    glViewport(0, 0, app->surface_width, app->surface_height);
}

/**
 * アプリのレンダリングを行う
 * 毎秒60回前後呼び出される。
 */
void sample_TextureOpMipmapGen_rendering( GLApplication *app) {
    // サンプルアプリ用データを取り出す
    Extension_TextureOpMipmapGen *extension = (Extension_TextureOpMipmapGen*) app->extension;

    glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // attr_posを有効にする
    glEnableVertexAttribArray(extension->attr_pos);
    glEnableVertexAttribArray(extension->attr_uv);
    // unif_textureへテクスチャを設定する
    glUniform1i(extension->unif_texture, 0);

    // ポリゴンの大きさを動的に指定する
    const GLfloat LEFT = -extension->polygon_size;
    const GLfloat TOP = extension->polygon_size;
    const GLfloat RIGHT = extension->polygon_size;
    const GLfloat BOTTOM = -extension->polygon_size;

    // 左上へ四角形描画
    {
        const GLfloat position[] = {
        // v0(left top)
                LEFT, TOP,
                // v1(left bottom)
                LEFT, BOTTOM,
                // v2(right top)
                RIGHT, TOP,
                // v3(right bottom)
                RIGHT, BOTTOM, };

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

    // ポリゴンを少しだけ小さくする
    extension->polygon_size *= 0.999f;
    if (extension->polygon_size < 0.01f) {
        // ある程度小さくなったら元に戻す
        extension->polygon_size = 0.5f;
    }

    // バックバッファをフロントバッファへ転送する。プラットフォームごとに内部の実装が異なる。
    ES20_postFrontBuffer(app);
}

/**
 * アプリのデータ削除を行う
 */
void sample_TextureOpMipmapGen_destroy( GLApplication *app) {
    // サンプルアプリ用データを取り出す
    Extension_TextureOpMipmapGen *extension = (Extension_TextureOpMipmapGen*) app->extension;

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
