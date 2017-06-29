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

    // テクスチャのUniform
    GLint unif_texture;

    // 読み込んだテクスチャ
    Texture *textures[2];

} Extension_LoadTextureFunction;

/**
 * アプリの初期化を行う
 */
void sample_LoadTextureFunction_initialize( GLApplication *app) {
    // サンプルアプリ用のメモリを確保する
    app->extension = (Extension_LoadTextureFunction*) malloc(sizeof(Extension_LoadTextureFunction));
    // サンプルアプリ用データを取り出す
    Extension_LoadTextureFunction *extension = (Extension_LoadTextureFunction*) app->extension;

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
        // TODO 読み込み関数を解説
        {
            extension->textures[0] = Texture_load(app, "texture_rgb_512x512.png", TEXTURE_RAW_RGBA8);
            assert(extension->textures[0] != NULL);
        }
        {
            extension->textures[1] = Texture_load(app, "texture_rgb_256x256.png", TEXTURE_RAW_RGB8);
            assert(extension->textures[1] != NULL);
        }
    }

    // シェーダーの利用を開始する
    glUseProgram(extension->shader_program);
    assert(glGetError() == GL_NO_ERROR);
}

/**
 * レンダリングエリアが変更された
 */
void sample_LoadTextureFunction_resized( GLApplication *app) {
    // 描画領域を設定する
    glViewport(0, 0, app->surface_width, app->surface_height);
}

/**
 * アプリのレンダリングを行う
 * 毎秒60回前後呼び出される。
 */
void sample_LoadTextureFunction_rendering( GLApplication *app) {
    // サンプルアプリ用データを取り出す
    Extension_LoadTextureFunction *extension = (Extension_LoadTextureFunction*) app->extension;

    glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // attr_posを有効にする
    glEnableVertexAttribArray(extension->attr_pos);
    glEnableVertexAttribArray(extension->attr_uv);
    // unif_textureへテクスチャを設定する
    glUniform1i(extension->unif_texture, 0);

    // 四角形描画
    {
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

        // 左のポリゴンを描画
        {
            const GLfloat position[] = {
                    // v0(left top)
                    -0.95f, 0.75f,
                    // v1(left bottom)
                    -0.95f, -0.75f,
                    // v2(right top)
                    -0.05f, 0.75f,
                    // v3(right bottom)
                    -0.05f, -0.75f, };

            glVertexAttribPointer(extension->attr_pos, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*) position);

            // テクスチャを関連付ける
            glBindTexture(GL_TEXTURE_2D, extension->textures[0]->id);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        }

        // 右のポリゴンを描画
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

            glVertexAttribPointer(extension->attr_pos, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*) position);
            // テクスチャを関連付ける
            glBindTexture(GL_TEXTURE_2D, extension->textures[1]->id);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        }
    }

    // バックバッファをフロントバッファへ転送する。プラットフォームごとに内部の実装が異なる。
    ES20_postFrontBuffer(app);
}

/**
 * アプリのデータ削除を行う
 */
void sample_LoadTextureFunction_destroy( GLApplication *app) {
    // サンプルアプリ用データを取り出す
    Extension_LoadTextureFunction *extension = (Extension_LoadTextureFunction*) app->extension;

    // シェーダーの利用を終了する
    glUseProgram(0);
    assert(glGetError() == GL_NO_ERROR);

    // シェーダープログラムを廃棄する
    glDeleteProgram(extension->shader_program);
    assert(glGetError() == GL_NO_ERROR);

    // TODO 解説
    // テクスチャを解放
    Texture_free(extension->textures[0]);
    Texture_free(extension->textures[1]);

    // サンプルアプリ用のメモリを解放する
    free(app->extension);
}
