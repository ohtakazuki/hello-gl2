#include <GLES2/gl2.h>
#include "../../support/support.h"

typedef struct {
    // レンダリング用シェーダープログラム
    GLuint   shader_program;
    
    // 位置情報属性
    GLint   attr_pos;
}Extension_RenderingQuad;

/**
 * アプリの初期化を行う
 */
void sample_RenderingQuad_initialize( GLApplication *app) {
    // サンプルアプリ用のメモリを確保する
    app->extension = (Extension_RenderingQuad*)malloc(sizeof(Extension_RenderingQuad));
    // サンプルアプリ用データを取り出す
    Extension_RenderingQuad *extension = (Extension_RenderingQuad*)app->extension;

    // 頂点シェーダーを用意する
    {
        const GLchar *vertex_shader_source =
        "attribute mediump vec4 attr_pos;"
        "void main() {"
        "   gl_Position = attr_pos;"
        "}"
        ;

        const GLchar *fragment_shader_source =
        "void main() {"
        "   gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);"
        "}"
        ;

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
}

/**
 * レンダリングエリアが変更された
 */
void sample_RenderingQuad_resized( GLApplication *app) {
    // 描画領域を設定する
    glViewport(0, 0, app->surface_width, app->surface_height);
}

/**
 * アプリのレンダリングを行う
 * 毎秒60回前後呼び出される。
 */
void sample_RenderingQuad_rendering( GLApplication *app) {
    // サンプルアプリ用データを取り出す
    Extension_RenderingQuad *extension = (Extension_RenderingQuad*)app->extension;
    
    glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    // attr_posを有効にする
    glEnableVertexAttribArray(extension->attr_pos);
    
// このブロックはカリングを含めて正しい順番で頂点が定義されていることをチェックします
#if 0   /* カリングチェック */
    {
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
    }
#endif  /* カリングチェック */
    
    // 左上へ四角形描画
    {
        const GLfloat position[] = {
            // triangle 0
            // v0(left top)
            -0.75f, 0.75f,
            // v1(left bottom)
            -0.75f, 0.25f,
            // v2(right top)
            -0.25f, 0.75f,
            
            // triangle 1
            // v3(right top)
            -0.25f, 0.75f,
            // v4(left bottom)
            -0.75f, 0.25f,
            // v5(right bottom)
            -0.25f, 0.25f,
        };
        
        glVertexAttribPointer(extension->attr_pos, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)position);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
    
    // 右上へ四角形描画
    {
        const GLfloat position[] = {
            // v0(left top)
            0.25f, 0.75f,
            // v1(left bottom)
            0.25f, 0.25f,
            // v2(right top)
            0.75f, 0.75f,
            // v3(right bottom)
            0.75f, 0.25f,
        };
        
        glVertexAttribPointer(extension->attr_pos, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)position);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }
    
    // 左下へ四角形描画
    {
        const GLfloat position[] = {
            // v0(right top)
            -0.25f, -0.25f,
            // v1(left top)
            -0.75f, -0.25f,
            // v2(left bottom)
            -0.75f, -0.75f,
            // v3(right bottom)
            -0.25f, -0.75f,
        };
        
        glVertexAttribPointer(extension->attr_pos, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)position);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    }

    // バックバッファをフロントバッファへ転送する。プラットフォームごとに内部の実装が異なる。
    ES20_postFrontBuffer(app);
}

/**
 * アプリのデータ削除を行う
 */
void sample_RenderingQuad_destroy( GLApplication *app) {
    // サンプルアプリ用データを取り出す
    Extension_RenderingQuad *extension = (Extension_RenderingQuad*)app->extension;
    
    // シェーダーの利用を終了する
    glUseProgram(0);
    assert(glGetError() == GL_NO_ERROR);
    
    // シェーダープログラムを廃棄する
    glDeleteProgram(extension->shader_program);
    assert(glGetError() == GL_NO_ERROR);

    // サンプルアプリ用のメモリを解放する
    free(app->extension);
}
