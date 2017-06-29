#include <GLES2/gl2.h>
#include "../../support/support.h"

typedef struct {
    // レンダリング用シェーダープログラム
    GLuint shader_program;

    // 頂点シェーダー
    GLuint vert_shader;

    // フラグメントシェーダー
    GLuint frag_shader;

    // 位置情報属性
    GLint attr_pos;
} Extension_RenderingTriangleNoViewport;

/**
 * アプリの初期化を行う
 */
void sample_RenderingTriangleNoViewport_initialize(GLApplication *app) {
    // サンプルアプリ用のメモリを確保する
    app->extension = (Extension_RenderingTriangleNoViewport*) malloc(sizeof(Extension_RenderingTriangleNoViewport));
    // サンプルアプリ用データを取り出す
    Extension_RenderingTriangleNoViewport *extension = (Extension_RenderingTriangleNoViewport*) app->extension;

    // 頂点シェーダーを用意する
    {
        const GLchar *vertex_shader_source =
                //
                "attribute mediump vec4 attr_pos;"
                "void main() {"
                "   gl_Position = attr_pos;"
                "}";

        // シェーダーオブジェクトを作成する
        extension->vert_shader = glCreateShader(GL_VERTEX_SHADER);
        assert(extension->vert_shader != 0);
        assert(glGetError() == GL_NO_ERROR);

        glShaderSource(extension->vert_shader, 1, &vertex_shader_source, NULL);
        glCompileShader(extension->vert_shader);

        // コンパイルエラーをチェックする
        {

            GLint compileSuccess = 0;
            glGetShaderiv(extension->vert_shader, GL_COMPILE_STATUS, &compileSuccess);
            if (compileSuccess == GL_FALSE) {
                // エラーが発生した
                GLint infoLen = 0;
                // エラーメッセージを取得
                glGetShaderiv(extension->vert_shader, GL_INFO_LOG_LENGTH, &infoLen);
                if (infoLen > 1) {
                    GLchar *message = (GLchar*) calloc(infoLen, sizeof(GLchar));
                    glGetShaderInfoLog(extension->vert_shader, infoLen, NULL, message);
                    __log(message);
                    free((void*) message);
                } else {
                    __log("comple error not info...");
                }
            }
            assert(compileSuccess == GL_TRUE);
        }
    }
    // フラグメントシェーダーを用意する
    {
        const GLchar *fragment_shader_source =
                "void main() {"
                "   gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);"
                "}";
        // シェーダーオブジェクトを作成する
        extension->frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
        assert(extension->frag_shader != 0);
        glShaderSource(extension->frag_shader, 1, &fragment_shader_source, NULL);
        glCompileShader(extension->frag_shader);

        // コンパイルエラーをチェックする
        {

            GLint compileSuccess = 0;
            glGetShaderiv(extension->frag_shader, GL_COMPILE_STATUS, &compileSuccess);
            if (compileSuccess == GL_FALSE) {
                // エラーが発生した
                GLint infoLen = 0;
                // エラーメッセージを取得
                glGetShaderiv(extension->frag_shader, GL_INFO_LOG_LENGTH, &infoLen);
                if (infoLen > 1) {
                    GLchar *message = (GLchar*) calloc(infoLen, sizeof(GLchar));
                    glGetShaderInfoLog(extension->frag_shader, infoLen, NULL, message);
                    __log(message);
                    free((void*) message);
                } else {
                    __log("comple error not info...");
                }
            }
            assert(compileSuccess == GL_TRUE);
        }
    }
    // リンクを行う
    {
        extension->shader_program = glCreateProgram();
        assert(extension->shader_program != 0);

        glAttachShader(extension->shader_program, extension->vert_shader); // 頂点シェーダーとプログラムを関連付ける
        assert(glGetError() == GL_NO_ERROR);
        glAttachShader(extension->shader_program, extension->frag_shader); // フラグメントシェーダーとプログラムを関連付ける
        assert(glGetError() == GL_NO_ERROR);

        // リンク処理を行う
        glLinkProgram(extension->shader_program);

        // リンクエラーをチェックする
        {

            GLint linkSuccess = 0;
            glGetProgramiv(extension->shader_program, GL_LINK_STATUS, &linkSuccess);

            if (linkSuccess == GL_FALSE) {
                // エラーが発生したため、状態をチェックする
                GLint infoLen = 0;
                // エラーメッセージを取得
                glGetProgramiv(extension->shader_program, GL_INFO_LOG_LENGTH, &infoLen);
                if (infoLen > 1) {
                    GLchar *message = (GLchar*) calloc(infoLen, sizeof(GLchar));
                    glGetProgramInfoLog(extension->shader_program, infoLen, NULL, message);
                    __log(message);
                    free((void*) message);
                }
            }

            // GL_NO_ERRORであることを検証する
            assert(linkSuccess == GL_TRUE);
        }
    }

    // attributeを取り出す
    {
        extension->attr_pos = glGetAttribLocation(extension->shader_program, "attr_pos");
        assert(extension->attr_pos >= 0);
    }

    // シェーダーの利用を開始する
    glUseProgram(extension->shader_program);
    assert(glGetError() == GL_NO_ERROR);
}

/**
 * レンダリングエリアが変更された
 */
void sample_RenderingTriangleNoViewport_resized(GLApplication *app) {
    // 描画領域を設定する
// コメント化して、glViewportを呼び出さないようにする
//    glViewport(0, 0, app->surface_width, app->surface_height);
}

/**
 * アプリのレンダリングを行う
 * 毎秒60回前後呼び出される。
 */
void sample_RenderingTriangleNoViewport_rendering(GLApplication *app) {
    // サンプルアプリ用データを取り出す
    Extension_RenderingTriangleNoViewport *extension = (Extension_RenderingTriangleNoViewport*) app->extension;

    glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // attr_posを有効にする
    glEnableVertexAttribArray(extension->attr_pos);

    // 画面中央へ描画する
    const GLfloat position[] = {
            // v0
            0.0f, 1.0f,
            // v1
            1.0f, -1.0f,
            // v2
            -1.0f, -1.0f
    };

    glVertexAttribPointer(extension->attr_pos, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*) position);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // バックバッファをフロントバッファへ転送する。プラットフォームごとに内部の実装が異なる。
    ES20_postFrontBuffer(app);
}

/**
 * アプリのデータ削除を行う
 */
void sample_RenderingTriangleNoViewport_destroy(GLApplication *app) {
    // サンプルアプリ用データを取り出す
    Extension_RenderingTriangleNoViewport *extension = (Extension_RenderingTriangleNoViewport*) app->extension;

    // シェーダーの利用を終了する
    glUseProgram(0);
    assert(glGetError() == GL_NO_ERROR);

    // シェーダープログラムを廃棄する
    glDeleteProgram(extension->shader_program);
    assert(glGetError() == GL_NO_ERROR);
    // シェーダーオブジェクトを廃棄する
    glDeleteShader(extension->vert_shader);
    assert(glGetError() == GL_NO_ERROR);
    // シェーダーオブジェクトを廃棄する
    glDeleteShader(extension->frag_shader);
    assert(glGetError() == GL_NO_ERROR);

    // サンプルアプリ用のメモリを解放する
    free(app->extension);
}
