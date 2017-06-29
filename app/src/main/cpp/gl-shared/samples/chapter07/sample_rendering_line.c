#include <GLES2/gl2.h>
#include "../../support/support.h"

typedef struct {
    // レンダリング用シェーダープログラム
    GLuint shader_program;

    // 位置情報属性
    GLint attr_pos;
} Extension_RenderingLine;

/**
 * アプリの初期化を行う
 */
void sample_RenderingLine_initialize(GLApplication *app) {
    // サンプルアプリ用のメモリを確保する
    app->extension = (Extension_RenderingLine*) malloc(sizeof(Extension_RenderingLine));
    // サンプルアプリ用データを取り出す
    Extension_RenderingLine *extension = (Extension_RenderingLine*) app->extension;

    // シェーダーを用意する
    {
        const GLchar *vertex_shader_source = "attribute mediump vec4 attr_pos;"
                "void main() {"
                "   gl_Position = attr_pos;"
                "}";

        const GLchar *fragment_shader_source = "void main() {"
                "   gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);"
                "}";

        // コンパイル&リンクを行う
        extension->shader_program = Shader_createProgramFromSource(vertex_shader_source, fragment_shader_source);
    }

    // attributeを取り出す
    {
        extension->attr_pos = glGetAttribLocation(extension->shader_program, "attr_pos");
        assert(extension->attr_pos >= 0);
    }

    // シェーダーの利用を開始する
    glUseProgram(extension->shader_program);
    assert(glGetError() == GL_NO_ERROR);

    // glLineWidthに設定可能な値を取得する
    {
        GLfloat minmaxWidth[2] = { 0 };
        glGetFloatv(GL_ALIASED_LINE_WIDTH_RANGE, minmaxWidth);
        const GLfloat minLineWidth = minmaxWidth[0];
        const GLfloat maxLineWidth = minmaxWidth[1];
        __logf("Device Spec[ %f <= width <= %f ]", minLineWidth, maxLineWidth);
    }
}

/**
 * レンダリングエリアが変更された
 */
void sample_RenderingLine_resized(GLApplication *app) {
    // 描画領域を設定する
    glViewport(0, 0, app->surface_width, app->surface_height);
}

/**
 * アプリのレンダリングを行う
 * 毎秒60回前後呼び出される。
 */
void sample_RenderingLine_rendering(GLApplication *app) {
    // サンプルアプリ用データを取り出す
    Extension_RenderingLine *extension = (Extension_RenderingLine*) app->extension;

    glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glEnableVertexAttribArray(extension->attr_pos);

    // 画面を十字に描画する
    {
        // 線の太さを指定する
        glLineWidth(1.0f);

        const GLfloat position[] = {
            // 直線1
                // v0
                -1.0f, 0.0f,
                // v1
                1.0f, 0.0f,
            // 直線2
                // v2
                0.0f, 1.0f,
                // v3
                0.0f, -1.0f };
        glVertexAttribPointer(extension->attr_pos, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*) position);
        // ライン２本、4頂点を描画する
        glDrawArrays(GL_LINES, 0, 4);
    }
    // 画面中央にコの字を描画する
    {
        // 線の太さを指定する
        glLineWidth(5.0f);

        const GLfloat position[] = {
                // v4
                -0.5f, 0.5f,
                // v5
                0.5f, 0.5f ,
                // v6
                0.5f, -0.5f,
                // v7
                -0.5f, -0.5f,
        };
        glVertexAttribPointer(extension->attr_pos, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*) position);
        // 4頂点を描画する
        glDrawArrays(GL_LINE_STRIP, 0, 4);
    }
    // 画面外周近くにロの字を描画する
    {
        // 線の太さを指定する
        glLineWidth(10.0f);

        const GLfloat position[] = {
                // v8
                -0.75f, 0.75f,
                // v9
                0.75f, 0.75f,
                // v10
                0.75f, -0.75f,
                // v11
                -0.75f, -0.75f,

        };
        glVertexAttribPointer(extension->attr_pos, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*) position);
        // 4頂点を描画する
        glDrawArrays(GL_LINE_LOOP, 0, 4);
    }

    // バックバッファをフロントバッファへ転送する。プラットフォームごとに内部の実装が異なる。
    ES20_postFrontBuffer(app);
}

/**
 * アプリのデータ削除を行う
 */
void sample_RenderingLine_destroy(GLApplication *app) {
    // サンプルアプリ用データを取り出す
    Extension_RenderingLine *extension = (Extension_RenderingLine*) app->extension;

    // シェーダーの利用を終了する
    glUseProgram(0);
    assert(glGetError() == GL_NO_ERROR);

    // シェーダープログラムを廃棄する
    glDeleteProgram(extension->shader_program);
    assert(glGetError() == GL_NO_ERROR);

    // サンプルアプリ用のメモリを解放する
    free(app->extension);
}
