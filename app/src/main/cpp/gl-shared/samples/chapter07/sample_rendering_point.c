#include <GLES2/gl2.h>
#include "../../support/support.h"

typedef struct {
    // レンダリング用シェーダープログラム
    GLuint shader_program;

    // 位置情報属性
    GLint attr_pos;
} Extension_RenderingPoint;

/**
 * アプリの初期化を行う
 */
void sample_RenderingPoint_initialize(GLApplication *app) {
    // サンプルアプリ用のメモリを確保する
    app->extension = (Extension_RenderingPoint*) malloc(sizeof(Extension_RenderingPoint));
    // サンプルアプリ用データを取り出す
    Extension_RenderingPoint *extension = (Extension_RenderingPoint*) app->extension;

    // シェーダーを用意する
    {
        const GLchar *vertex_shader_source = "attribute mediump vec4 attr_pos;"
                "void main() {"
                "   gl_Position = attr_pos;"
                "   gl_PointSize = 15.0;"
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

    // gl_PointSizeに設定可能な値を取得する
    {
        GLfloat minmaxPointSize[2] = { 0 };
        glGetFloatv(GL_ALIASED_POINT_SIZE_RANGE, minmaxPointSize);
        const GLfloat minPointSize = minmaxPointSize[0];
        const GLfloat maxPointSize = minmaxPointSize[1];
        __logf("Device Spec[ %f <= size <= %f ]", minPointSize, maxPointSize);
    }
}

/**
 * レンダリングエリアが変更された
 */
void sample_RenderingPoint_resized(GLApplication *app) {
    // 描画領域を設定する
    glViewport(0, 0, app->surface_width, app->surface_height);
}

/**
 * アプリのレンダリングを行う
 * 毎秒60回前後呼び出される。
 */
void sample_RenderingPoint_rendering(GLApplication *app) {
    // サンプルアプリ用データを取り出す
    Extension_RenderingPoint *extension = (Extension_RenderingPoint*) app->extension;

    glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // attr_posを有効にする
    glEnableVertexAttribArray(extension->attr_pos);

    // 画面中央と右下へ描画する
    {
        const GLfloat positions[] = {
                // v0
                0.0f, 0.0f,
                // v1
                0.75f, -0.75f, };
        glVertexAttribPointer(extension->attr_pos, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*) positions);
        glDrawArrays(GL_POINTS, 0, 2);
    }

    // バックバッファをフロントバッファへ転送する。プラットフォームごとに内部の実装が異なる。
    ES20_postFrontBuffer(app);
}

/**
 * アプリのデータ削除を行う
 */
void sample_RenderingPoint_destroy(GLApplication *app) {
    // サンプルアプリ用データを取り出す
    Extension_RenderingPoint *extension = (Extension_RenderingPoint*) app->extension;

    // シェーダーの利用を終了する
    glUseProgram(0);
    assert(glGetError() == GL_NO_ERROR);

    // シェーダープログラムを廃棄する
    glDeleteProgram(extension->shader_program);
    assert(glGetError() == GL_NO_ERROR);

    // サンプルアプリ用のメモリを解放する
    free(app->extension);
}
