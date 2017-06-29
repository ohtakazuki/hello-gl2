//
//  es20_chapter00_sample00.c
//  OpenGL_ES_20
//
//  Created by 山下 武志 on 2013/02/04.
//  Copyright (c) 2013年 山下 武志. All rights reserved.
//

#include <GLES2/gl2.h>
#include "../../support/support_gl_Vector.h"
#include "../../support/support.h"

typedef struct {
    // レンダリング用シェーダープログラム
    GLint shader_program;

    // 位置情報属性
    GLint attr_pos;

    // 頂点シェーダのuniform
    GLint unif_move_pos;

    /**
     * XY位置
     */
    vec2 pos;
} Extension_ShaderUniformPos;

/**
 * アプリの初期化を行う
 */
void sample_ShaderUniformPos_initialize(GLApplication *app) {
    // サンプルアプリ用のメモリを確保する
    app->extension = (Extension_ShaderUniformPos*) malloc(sizeof(Extension_ShaderUniformPos));
    // サンプルアプリ用データを取り出す
    Extension_ShaderUniformPos *extension = (Extension_ShaderUniformPos*) app->extension;

    // 頂点シェーダーを用意する
    {
        const GLchar *vertex_shader_source = "attribute mediump vec4 attr_pos;"
        // プログラムから設定する変数
        // この変数の値だけ、頂点位置が動く
                        "uniform mediump vec2 unif_move_pos;"
                        "void main() {"
                        "   gl_Position = attr_pos;"
        //  unif_move_posの値を更に加算する
                        "   gl_Position.xy += unif_move_pos;"
                        "}";

        const GLchar *fragment_shader_source = "void main() {"
                "   gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);"
                "}";

        // コンパイルとリンクを行う
        extension->shader_program = Shader_createProgramFromSource(vertex_shader_source, fragment_shader_source);
    }

    // attributeを取り出す
    {
        extension->attr_pos = glGetAttribLocation(extension->shader_program, "attr_pos");
        assert(extension->attr_pos >= 0);
    }

    // TODO 解説
    // uniform変数のlocationを取得する
    {
        extension->unif_move_pos = glGetUniformLocation(extension->shader_program, "unif_move_pos");
        assert(extension->unif_move_pos >= 0);
    }

    // シェーダーの利用を開始する
    glUseProgram(extension->shader_program);
    assert(glGetError() == GL_NO_ERROR);

    {
        // XY座標初期化
        extension->pos.x = 0;
        extension->pos.y = 0;
    }
}

/**
 * レンダリングエリアが変更された
 */
void sample_ShaderUniformPos_resized(GLApplication *app) {
    // 描画領域を設定する
    glViewport(0, 0, app->surface_width, app->surface_height);
}

/**
 * アプリのレンダリングを行う
 * 毎秒60回前後呼び出される。
 */
void sample_ShaderUniformPos_rendering(GLApplication *app) {
    // サンプルアプリ用データを取り出す
    Extension_ShaderUniformPos *extension = (Extension_ShaderUniformPos*) app->extension;

    glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // attr_posを有効にする
    glEnableVertexAttribArray(extension->attr_pos);

    const GLfloat position[] = {
    // v4(left top)
            0.0, 0.5f,
            // v5(left bottom)
            0.0f, 0.0f,
            // v6(right top)
            0.5f, 0.5f,
            // v7(right bottom)
            0.5f, 0.0f, };
    // TODO 解説
    // 頂点の設定は一度だけでいい
    glVertexAttribPointer(extension->attr_pos, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*) position);

    {
        // XYの位置に適当に加算を行う
        extension->pos.x += 0.01f;
        extension->pos.y += 0.02f;

        // 一定を超えたら値をリセットする
        if (extension->pos.x > 1.0f) {
            extension->pos.x = -1;
        }
        if (extension->pos.y > 1.0f) {
            extension->pos.y = -1;
        }

        // uniformへXYの位置を転送する
        glUniform2f(extension->unif_move_pos, extension->pos.x, extension->pos.y);
//        glUniform2fv(extension->unif_move_pos, 1, (GLfloat*) &extension->pos);
    }

// 四角形の描画を行う
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

// バックバッファをフロントバッファへ転送する。プラットフォームごとに内部の実装が異なる。
    ES20_postFrontBuffer(app);
}

/**
 * アプリのデータ削除を行う
 */
void sample_ShaderUniformPos_destroy(GLApplication *app) {
// サンプルアプリ用データを取り出す
    Extension_ShaderUniformPos *extension = (Extension_ShaderUniformPos*) app->extension;

// シェーダーの利用を終了する
    glUseProgram(0);
    assert(glGetError() == GL_NO_ERROR);

// シェーダープログラムを廃棄する
    glDeleteProgram(extension->shader_program);
    assert(glGetError() == GL_NO_ERROR);

// サンプルアプリ用のメモリを解放する
    free(app->extension);
}
