//
//  es20_chapter00_sample00.c
//  OpenGL_ES_20
//
//  Created by 山下 武志 on 2013/02/04.
//  Copyright (c) 2013年 山下 武志. All rights reserved.
//

#include <GLES2/gl2.h>
#include "../../support/support.h"

typedef struct {
    // レンダリング用シェーダープログラム
    GLuint   shader_program;
    
    // TODO 解説
    // メンバが削られている
    
    // 位置情報属性
    GLint   attr_pos;
}Extension_ShaderFunctionOptimizeShort;

/**
 * アプリの初期化を行う
 */
void sample_RenderingTriangleFunction_initialize( GLApplication *app) {
    // サンプルアプリ用のメモリを確保する
    app->extension = (Extension_ShaderFunctionOptimizeShort*)malloc(sizeof(Extension_ShaderFunctionOptimizeShort));
    // サンプルアプリ用データを取り出す
    Extension_ShaderFunctionOptimizeShort *extension = (Extension_ShaderFunctionOptimizeShort*)app->extension;

    // 頂点シェーダーを用意する
    {
        const GLchar *vertex_shader_source =
        "attribute mediump vec4 attr_pos;"
        "void main() {"
        "   gl_Position = attr_pos;"
        "}";
        
        const GLchar *fragment_shader_source =
        "void main() {"
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
    
    // シェーダーの利用を開始する
    glUseProgram(extension->shader_program);
    assert(glGetError() == GL_NO_ERROR);
}

/**
 * レンダリングエリアが変更された
 */
void sample_RenderingTriangleFunction_resized( GLApplication *app) {
    // 描画領域を設定する
    glViewport(0, 0, app->surface_width, app->surface_height);
}


/**
 * アプリのレンダリングを行う
 * 毎秒60回前後呼び出される。
 */
void sample_RenderingTriangleFunction_rendering( GLApplication *app) {
    // サンプルアプリ用データを取り出す
    Extension_ShaderFunctionOptimizeShort *extension = (Extension_ShaderFunctionOptimizeShort*)app->extension;
    
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

    glVertexAttribPointer(extension->attr_pos, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)position);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // バックバッファをフロントバッファへ転送する。プラットフォームごとに内部の実装が異なる。
    ES20_postFrontBuffer(app);
}

/**
 * アプリのデータ削除を行う
 */
void sample_RenderingTriangleFunction_destroy( GLApplication *app) {
    // サンプルアプリ用データを取り出す
    Extension_ShaderFunctionOptimizeShort *extension = (Extension_ShaderFunctionOptimizeShort*)app->extension;
    
    // シェーダーの利用を終了する
    glUseProgram(0);
    assert(glGetError() == GL_NO_ERROR);
    
    // シェーダープログラムを廃棄する
    glDeleteProgram(extension->shader_program);
    assert(glGetError() == GL_NO_ERROR);
    
    // サンプルアプリ用のメモリを解放する
    free(app->extension);
}
