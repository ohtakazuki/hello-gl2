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
    GLint unif_tex_color;
    GLint unif_tex_mask;

    // 読み込んだテクスチャ
    Texture *textures[2];

} Extension_BlendTextureChannel;

/**
 * アプリの初期化を行う
 */
void sample_BlendTextureChannel_initialize(GLApplication *app) {
    // サンプルアプリ用のメモリを確保する
    app->extension = (Extension_BlendTextureChannel*) malloc(sizeof(Extension_BlendTextureChannel));
    // サンプルアプリ用データを取り出す
    Extension_BlendTextureChannel *extension = (Extension_BlendTextureChannel*) app->extension;

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

        // 型抜きフラグメントシェーダ
        const GLchar *fragment_shader_source =
        //
                "uniform sampler2D tex_color;"
                        "uniform sampler2D tex_mask;"
                        "varying mediump vec2 vary_uv;"
                        "void main() {"
                        "   if(texture2D(tex_mask, vary_uv).r == 0.0) {"
                        "       discard;"
                        "   }"
                        "   gl_FragColor = texture2D(tex_color, vary_uv);"
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
        extension->unif_tex_color = glGetUniformLocation(extension->shader_program, "tex_color");
        extension->unif_tex_mask = glGetUniformLocation(extension->shader_program, "tex_mask");
        assert(extension->unif_tex_color >= 0);
        assert(extension->unif_tex_mask >= 0);
    }

    {
        // 色要素を読み込む
        {
            extension->textures[0] = Texture_load(app, "sample_blend_color.png", TEXTURE_RAW_RGBA8);
            assert(extension->textures[0] != NULL);
        }
        // マスク要素を読み込む
        {
            extension->textures[1] = Texture_load(app, "sample_blend_mask.png", TEXTURE_RAW_RGBA8);
            assert(extension->textures[1] != NULL);
        }

        // 色テクスチャを0番ユニットにバインドする
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, extension->textures[0]->id);

        // マスクテクスチャを1番ユニットにバインドする
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, extension->textures[1]->id);
    }

    // シェーダーの利用を開始する
    glUseProgram(extension->shader_program);
    assert(glGetError() == GL_NO_ERROR);
}

/**
 * レンダリングエリアが変更された
 */
void sample_BlendTextureChannel_resized(GLApplication *app) {
    // 描画領域を設定する
    glViewport(0, 0, app->surface_width, app->surface_height);
}

/**
 * アプリのレンダリングを行う
 * 毎秒60回前後呼び出される。
 */
void sample_BlendTextureChannel_rendering(GLApplication *app) {
    // サンプルアプリ用データを取り出す
    Extension_BlendTextureChannel *extension = (Extension_BlendTextureChannel*) app->extension;

    glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // attr_posを有効にする
    glEnableVertexAttribArray(extension->attr_pos);
    glEnableVertexAttribArray(extension->attr_uv);

    // 四角形描画
    {
        const GLfloat position[] = {
        // v0(left top)
                -0.95f, 0.95f,
                // v1(left bottom)
                -0.95f, -0.95f,
                // v2(right top)
                0.95f, 0.95f,
                // v3(right bottom)
                0.95f, -0.95f, };

        // 0.0f〜1.0fの範囲を設定してレンダリング結果をチェック
        const GLfloat uv[] = {
        // v0(left top)
                0.0f, 0.0f,
                // v1(left bottom)
                0.0f, 1.0f,
                // v2(right top)
                1.0f, 0.0f,
                // v3(right bottom)
                1.0f, 1.0f, };

        glVertexAttribPointer(extension->attr_pos, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*) position);
        glVertexAttribPointer(extension->attr_uv, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*) uv);

        // TODO 解説
        // 各チャンネルを転送する
        glUniform1i(extension->unif_tex_color, 0);
        glUniform1i(extension->unif_tex_mask, 1);

        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }

    // バックバッファをフロントバッファへ転送する。プラットフォームごとに内部の実装が異なる。
    ES20_postFrontBuffer(app);
}

/**
 * アプリのデータ削除を行う
 */
void sample_BlendTextureChannel_destroy(GLApplication *app) {
    // サンプルアプリ用データを取り出す
    Extension_BlendTextureChannel *extension = (Extension_BlendTextureChannel*) app->extension;

    // シェーダーの利用を終了する
    glUseProgram(0);
    assert(glGetError() == GL_NO_ERROR);

    // シェーダープログラムを廃棄する
    glDeleteProgram(extension->shader_program);
    assert(glGetError() == GL_NO_ERROR);

    // テクスチャを解放
    Texture_free(extension->textures[0]);
    Texture_free(extension->textures[1]);

    // サンプルアプリ用のメモリを解放する
    free(app->extension);
}
