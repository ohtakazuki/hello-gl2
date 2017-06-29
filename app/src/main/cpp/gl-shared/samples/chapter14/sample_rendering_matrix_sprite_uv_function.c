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

    /**
     * 回転角
     */
    GLfloat rotate;

    // 描画フレーム
    int frame;

    // 描画対象のテクスチャ
    Texture *texture;
} Extension_ShaderMatrixSpriteUVFunction;

/**
 * アプリの初期化を行う
 */
void sample_ShaderMatrixSpriteUVFunction_initialize(GLApplication *app) {
    // サンプルアプリ用のメモリを確保する
    app->extension = (Extension_ShaderMatrixSpriteUVFunction*) malloc(sizeof(Extension_ShaderMatrixSpriteUVFunction));
    // サンプルアプリ用データを取り出す
    Extension_ShaderMatrixSpriteUVFunction *extension = (Extension_ShaderMatrixSpriteUVFunction*) app->extension;

    {
        // 初期化
        extension->rotate = 0;
        extension->frame = 0;
    }

    // 頂点シェーダーを用意する
    {
        const GLchar *vertex_shader_source =
        //
                "uniform mediump mat4 unif_matrix;"
                        "uniform mediump mat4 unif_uvmatrix;"
                        "attribute mediump vec4 attr_pos;"
                        "attribute mediump vec4 attr_uv;"// vec4へ変更
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

    // Textureを読み込む
    {
        extension->texture = Texture_load(app, "textureatlas.png", TEXTURE_RAW_RGBA8);
        assert(extension->texture != NULL);
    }

    // シェーダーの利用を開始する
    glUseProgram(extension->shader_program);
    assert(glGetError() == GL_NO_ERROR);
}

/**
 * レンダリングエリアが変更された
 */
void sample_ShaderMatrixSpriteUVFunction_resized(GLApplication *app) {
    // 描画領域を設定する
    glViewport(0, 0, app->surface_width, app->surface_height);
}

/**
 * アプリのレンダリングを行う
 * 毎秒60回前後呼び出される。
 */
void sample_ShaderMatrixSpriteUVFunction_rendering(GLApplication *app) {
    // サンプルアプリ用データを取り出す
    Extension_ShaderMatrixSpriteUVFunction *extension = (Extension_ShaderMatrixSpriteUVFunction*) app->extension;

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

        // データテーブルを用意する
        struct AtlasTable {
            /**
             * 画像内のX座標
             */
            int x;
            /**
             * 画像内のY座標
             */
            int y;
            /**
             * 元画像の幅
             */
            int width;
            /**
             * 元画像の高さ
             */
            int height;
        } imagetable[] = {
        // アトラス化された画像のデータテーブル
        //   x, y, width, height
                { 513, 1, 458, 402 }, // 鹿の画像
                { 301, 513, 128, 128 }, //
                { 1003, 61, 16, 16 }, //
                { 513, 403, 256, 256 }, //
                { 1, 513, 300, 400 }, //
                { 971, 61, 32, 32 }, //
                { 1, 913, 64, 64 }, //
                { 1003, 77, 8, 8 }, //
                { 1, 1, 512, 512 }, //
                { 971, 1, 50, 60 }, //
                };

        struct AtlasTable image = imagetable[(extension->frame / 100) % 10]; // 100フレームごとにテクスチャが切り替わるようにする

        // 行列をアップロードする
        {
            const GLint spriteX = 0;
            const GLint spriteY = 0;
            const GLint spriteWidth = image.width;
            const GLint spriteHeight = image.height;

            mat4 matrix = Sprite_createPositionMatrix(app->surface_width, app->surface_height, spriteX, spriteY, spriteWidth, spriteHeight, extension->rotate);
            // アップロード
            glUniformMatrix4fv(extension->unif_matrix, 1, GL_FALSE, (GLfloat*) matrix.m);

            // 回転角を増やす
            extension->rotate += 1.0f;
        }

        // UV行列をアップロードする
        {

            mat4 matrix = Sprite_createUvMatrix(extension->texture->width, extension->texture->height, image.x, image.y, image.width, image.height);
            glUniformMatrix4fv(extension->unif_uvmatrix, 1, GL_FALSE, (GLfloat*) matrix.m);

        }

        // 描画フレームを進める
        extension->frame++;
        glBindTexture(GL_TEXTURE_2D, extension->texture->id);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }

    // バックバッファをフロントバッファへ転送する。プラットフォームごとに内部の実装が異なる。
    ES20_postFrontBuffer(app);
}

/**
 * アプリのデータ削除を行う
 */
void sample_ShaderMatrixSpriteUVFunction_destroy(GLApplication *app) {
    // サンプルアプリ用データを取り出す
    Extension_ShaderMatrixSpriteUVFunction *extension = (Extension_ShaderMatrixSpriteUVFunction*) app->extension;

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
