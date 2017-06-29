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

    // シェーダーで適用する行列
    GLint unif_matrix;

    // シェーダーで適用するUV行列
    GLint unif_uvmatrix;

    // テクスチャ
    Texture *texture;
} Extension_BlendEnable;

/**
 * アプリの初期化を行う
 */
void sample_BlendEnable_initialize(GLApplication *app) {
    // サンプルアプリ用のメモリを確保する
    app->extension = (Extension_BlendEnable*) malloc(sizeof(Extension_BlendEnable));
    // サンプルアプリ用データを取り出す
    Extension_BlendEnable *extension = (Extension_BlendEnable*) app->extension;

    // 頂点シェーダーを用意する
    {
        const GLchar *vertex_shader_source =
        //
                "uniform mediump mat4 unif_matrix;"
                        "uniform mediump mat4 unif_uvmatrix;"
                        "attribute mediump vec4 attr_pos;"
                        "attribute mediump vec4 attr_uv;"
                        "varying mediump vec2 vary_uv;"
                        "void main() {"
                        "   gl_Position = unif_matrix * attr_pos;"
                        "   vary_uv = (unif_uvmatrix * attr_uv).xy;"
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
        extension->unif_matrix = glGetUniformLocation(extension->shader_program, "unif_matrix");
        extension->unif_uvmatrix = glGetUniformLocation(extension->shader_program, "unif_uvmatrix");
        assert(extension->unif_matrix >= 0);
        assert(extension->unif_uvmatrix >= 0);
    }

    // テクスチャを読み込む
    {
        extension->texture = Texture_load(app, "texture_rgba_64x64.png", TEXTURE_RAW_RGBA8);
        assert(extension->texture);
    }

    // シェーダーの利用を開始する
    glUseProgram(extension->shader_program);
    assert(glGetError() == GL_NO_ERROR);

    // TODO 解説
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

/**
 * レンダリングエリアが変更された
 */
void sample_BlendEnable_resized(GLApplication *app) {
    // 描画領域を設定する
    glViewport(0, 0, app->surface_width, app->surface_height);
}

/**
 * アプリのレンダリングを行う
 * 毎秒60回前後呼び出される。
 */
void sample_BlendEnable_rendering(GLApplication *app) {
    // サンプルアプリ用データを取り出す
    Extension_BlendEnable *extension = (Extension_BlendEnable*) app->extension;

    glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // attr_posを有効にする
    glEnableVertexAttribArray(extension->attr_pos);
    glEnableVertexAttribArray(extension->attr_uv);

    // 四角形描画
    {
        // この章のサンプルではpositionとUVの位置を変更しない

        const GLfloat position[] = {
        // v0(left top)
                -0.5f, 0.5f,
                // v1(left bottom)
                -0.5f, -0.5f,
                // v2(right top)
                0.5f, 0.5f,
                // v3(right bottom)
                0.5f, -0.5f, };

        //
        const GLfloat uv[] = {
        // v0(left top)
                0, 0,
                // v1(left bottom)
                0, 1,
                // v2(right top)
                1, 0,
                // v3(right bottom)
                1, 1, };

        // 頂点情報を関連付ける
        glVertexAttribPointer(extension->attr_pos, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*) position);
        glVertexAttribPointer(extension->attr_uv, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*) uv);

        // 図形1
        {
            // 行列をアップロードする
            {
                const GLint spriteX = 0;
                const GLint spriteY = 0;
                const GLint spriteWidth = 512;
                const GLint spriteHeight = 512;

                mat4 matrix = Sprite_createPositionMatrix(app->surface_width, app->surface_height, spriteX, spriteY, spriteWidth, spriteHeight, 0);
                // アップロード
                glUniformMatrix4fv(extension->unif_matrix, 1, GL_FALSE, (GLfloat*) matrix.m);
            }

            // UV行列をアップロードする
            {

                mat4 matrix = Sprite_createUvMatrix(extension->texture->width, extension->texture->height, 0, 0, extension->texture->width, extension->texture->height);
                glUniformMatrix4fv(extension->unif_uvmatrix, 1, GL_FALSE, (GLfloat*) matrix.m);
            }

            glBindTexture(GL_TEXTURE_2D, extension->texture->id);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        }

        // 図形2
        {
            // 行列をアップロードする
            {
                const GLint spriteX = 128;
                const GLint spriteY = 128;
                const GLint spriteWidth = 512;
                const GLint spriteHeight = 512;

                mat4 matrix = Sprite_createPositionMatrix(app->surface_width, app->surface_height, spriteX, spriteY, spriteWidth, spriteHeight, 0);
                // アップロード
                glUniformMatrix4fv(extension->unif_matrix, 1, GL_FALSE, (GLfloat*) matrix.m);
            }

            // UV行列をアップロードする
            {

                mat4 matrix = Sprite_createUvMatrix(extension->texture->width, extension->texture->height, 0, 0, extension->texture->width, extension->texture->height);
                glUniformMatrix4fv(extension->unif_uvmatrix, 1, GL_FALSE, (GLfloat*) matrix.m);
            }

            glBindTexture(GL_TEXTURE_2D, extension->texture->id);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        }
    }

    // バックバッファをフロントバッファへ転送する。プラットフォームごとに内部の実装が異なる。
    ES20_postFrontBuffer(app);
}

/**
 * アプリのデータ削除を行う
 */
void sample_BlendEnable_destroy(GLApplication *app) {
    // サンプルアプリ用データを取り出す
    Extension_BlendEnable *extension = (Extension_BlendEnable*) app->extension;

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
