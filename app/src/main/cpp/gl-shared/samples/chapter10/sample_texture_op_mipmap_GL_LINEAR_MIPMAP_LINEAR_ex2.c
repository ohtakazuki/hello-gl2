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

} Extension_TextureOpMipmapLML_ex2;

/**
 * アプリの初期化を行う
 */
void sample_TextureOpMipmapLML_ex2_initialize(GLApplication *app) {
    // サンプルアプリ用のメモリを確保する
    app->extension = (Extension_TextureOpMipmapLML_ex2*) malloc(sizeof(Extension_TextureOpMipmapLML_ex2));
    // サンプルアプリ用データを取り出す
    Extension_TextureOpMipmapLML_ex2 *extension = (Extension_TextureOpMipmapLML_ex2*) app->extension;

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

    // Mipmap付きテクスチャの生成
    {
        glGenTextures(1, &extension->texture_id);
        assert(extension->texture_id != 0);
        assert(glGetError() == GL_NO_ERROR);

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        assert(glGetError() == GL_NO_ERROR);
        glBindTexture(GL_TEXTURE_2D, extension->texture_id);
        assert(glGetError() == GL_NO_ERROR);

        // TODO 解説
        // MEMO 正しい順番にmipmapテクスチャを設定しないと、正常なテクスチャとして認識されない
        // mipmapレベルに応じた画像を読み込む
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

            // コピー後は不要になるため、画像を解放する
            RawPixelImage_free(app, image);
        }

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        // フィルタをMIPMAP対応に変更する
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        assert(glGetError() == GL_NO_ERROR);

#if 0
        if(ES20_hasExtension("GL_EXT_texture_filter_anisotropic")) {
            // EX. 異方性フィルリングを適用することで、多少の緩和を行える
            // GL_EXT_texture_filter_anisotropicの拡張機能に対応している必要がある
            GLfloat anisotropy = 0;
            glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &anisotropy);
            __logf("GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT(%f)", anisotropy);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisotropy);
            assert(glGetError() == GL_NO_ERROR);
        }
#endif
    }

    // シェーダーの利用を開始する
    glUseProgram(extension->shader_program);
    assert(glGetError() == GL_NO_ERROR);

}

/**
 * レンダリングエリアが変更された
 */
void sample_TextureOpMipmapLML_ex2_resized(GLApplication *app) {
    // 描画領域を設定する
    glViewport(0, 0, app->surface_width, app->surface_height);
}

/**
 * アプリのレンダリングを行う
 * 毎秒60回前後呼び出される。
 */
void sample_TextureOpMipmapLML_ex2_rendering(GLApplication *app) {
    // サンプルアプリ用データを取り出す
    Extension_TextureOpMipmapLML_ex2 *extension = (Extension_TextureOpMipmapLML_ex2*) app->extension;

    glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // attr_posを有効にする
    glEnableVertexAttribArray(extension->attr_pos);
    glEnableVertexAttribArray(extension->attr_uv);
    // unif_textureへテクスチャを設定する
    glUniform1i(extension->unif_texture, 0);

    // 左上へ四角形描画
#if 0
    {
        const GLfloat position[] = {
            // v0(left top)
            -0.05f, 0.95f,
            // v1(left bottom)
            -0.95f, -0.95f,
            // v2(right top)
            0.05f, 0.95f,
            // v3(right bottom)
            0.95f, -0.95f,};

        const GLfloat uv[] = {
            // v0(left top)
            0, 0,
            // v1(left bottom)
            0, 1,
            // v2(right top)
            1, 0,
            // v3(right bottom)
            1, 1,};

        glVertexAttribPointer(extension->attr_pos, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*) position);
        glVertexAttribPointer(extension->attr_uv, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*) uv);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }
#endif

    // ポリゴンを複数に分割することでMipmap問題を回避する
    {
        // ポリゴンの分割数を指定する
        // デフォルトでは縦に512分割する
        const int DIV_QUADS = 512;


        int i = 0;

        const float position_offset_x = 0.9f / DIV_QUADS;
        const float position_offset_y = 1.8f / DIV_QUADS;
        const float uv_offset = 1.0f / DIV_QUADS;

        float pos_right = 0.05f;
        float pos_top = 0.95f;

        for (i = 0; i < DIV_QUADS; ++i) {

            const float CURRENT_TOP_RIGHT = pos_right;
            const float CURRENT_TOP_LEFT = -CURRENT_TOP_RIGHT;
            const float CURRENT_BTM_RIGHT = pos_right + position_offset_x;
            const float CURRENT_BTM_LEFT = -CURRENT_BTM_RIGHT;

            const float CURRENT_TOP = pos_top;
            const float CURRENT_BTM = pos_top - position_offset_y;

            const GLfloat position[] = {
            // v0(left top)
                    CURRENT_TOP_LEFT, CURRENT_TOP,
                    // v1(left bottom)
                    CURRENT_BTM_LEFT, CURRENT_BTM,
                    // v2(right top)
                    CURRENT_TOP_RIGHT, CURRENT_TOP,
                    // v3(right bottom)
                    CURRENT_BTM_RIGHT, CURRENT_BTM, };

            const GLfloat uv[] = {
            // v0(left top)
                    0, i * uv_offset,
                    // v1(left bottom)
                    0, (i + 1) * uv_offset,
                    // v2(right top)
                    1, i * uv_offset,
                    // v3(right bottom)
                    1, (i + 1) * uv_offset, };

            pos_right += position_offset_x;
            pos_top -= position_offset_y;

            glVertexAttribPointer(extension->attr_pos, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*) position);
            glVertexAttribPointer(extension->attr_uv, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*) uv);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        }
    }

    // バックバッファをフロントバッファへ転送する。プラットフォームごとに内部の実装が異なる。
    ES20_postFrontBuffer(app);
}

/**
 * アプリのデータ削除を行う
 */
void sample_TextureOpMipmapLML_ex2_destroy(GLApplication *app) {
    // サンプルアプリ用データを取り出す
    Extension_TextureOpMipmapLML_ex2 *extension = (Extension_TextureOpMipmapLML_ex2*) app->extension;

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
