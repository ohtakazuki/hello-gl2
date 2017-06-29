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

    /**
     * 回転角
     */
    GLfloat rotate;

    // 描画対象のテクスチャ
    Texture *texture;
} Extension_ShaderMatrixSpritePosFunction;

/**
 * アプリの初期化を行う
 */
void sample_ShaderMatrixSpritePosFunction_initialize(GLApplication *app) {
    // サンプルアプリ用のメモリを確保する
    app->extension = (Extension_ShaderMatrixSpritePosFunction*) malloc(sizeof(Extension_ShaderMatrixSpritePosFunction));
    // サンプルアプリ用データを取り出す
    Extension_ShaderMatrixSpritePosFunction *extension = (Extension_ShaderMatrixSpritePosFunction*) app->extension;

    {
        // 初期化
        extension->rotate = 0;
    }

    // 頂点シェーダーを用意する
    {
        const GLchar *vertex_shader_source =
        //
                "uniform mediump mat4 unif_matrix;"
                        "attribute mediump vec4 attr_pos;"
                        "attribute mediump vec2 attr_uv;"
                        "varying mediump vec2 vary_uv;"
                        "void main() {"
                        "   gl_Position = unif_matrix * attr_pos;"
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
        extension->unif_matrix = glGetUniformLocation(extension->shader_program, "unif_matrix");
        assert(extension->unif_matrix >= 0);
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
void sample_ShaderMatrixSpritePosFunction_resized(GLApplication *app) {
    // 描画領域を設定する
    glViewport(0, 0, app->surface_width, app->surface_height);
}

/**
 * アプリのレンダリングを行う
 * 毎秒60回前後呼び出される。
 */
void sample_ShaderMatrixSpritePosFunction_rendering(GLApplication *app) {
    // サンプルアプリ用データを取り出す
    Extension_ShaderMatrixSpritePosFunction *extension = (Extension_ShaderMatrixSpritePosFunction*) app->extension;

    glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // attr_posを有効にする
    glEnableVertexAttribArray(extension->attr_pos);
    glEnableVertexAttribArray(extension->attr_uv);

    // 四角形描画
    {
        // TODO 解説
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

        // TODO 解説
        // 行列をアップロードする
        {
            const GLint spriteX = 256;
            const GLint spriteY = 128;
            const GLint spriteWidth = 512;
            const GLint spriteHeight = 512;

            mat4 matrix = Sprite_createPositionMatrix(app->surface_width, app->surface_height, spriteX, spriteY, spriteWidth, spriteHeight, extension->rotate);
            // アップロード
            glUniformMatrix4fv(extension->unif_matrix, 1, GL_FALSE, (GLfloat*) matrix.m);

            // 回転角を増やす
            extension->rotate += 1.0f;
        }

        glBindTexture(GL_TEXTURE_2D, extension->texture->id);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }

    // バックバッファをフロントバッファへ転送する。プラットフォームごとに内部の実装が異なる。
    ES20_postFrontBuffer(app);
}

/**
 * アプリのデータ削除を行う
 */
void sample_ShaderMatrixSpritePosFunction_destroy(GLApplication *app) {
    // サンプルアプリ用データを取り出す
    Extension_ShaderMatrixSpritePosFunction *extension = (Extension_ShaderMatrixSpritePosFunction*) app->extension;

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
