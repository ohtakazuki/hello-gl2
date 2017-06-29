#include    "SampleList.h"
#include "support/support.h"

// プロトタイプ宣言用のマクロ
#define SAMPLE_PROTOTYPES(name) extern void sample_##name##_initialize(GLApplication *); \
                                extern void sample_##name##_resized(GLApplication *); \
                                extern void sample_##name##_rendering(GLApplication *); \
                                extern void sample_##name##_destroy(GLApplication *)

/*    CHAPTER    */
SAMPLE_PROTOTYPES(Skelton);
SAMPLE_PROTOTYPES(Clear);
SAMPLE_PROTOTYPES(RandomClear);

/*    CHAPTER    */
SAMPLE_PROTOTYPES(RenderingTriangle);
SAMPLE_PROTOTYPES(RenderingTriangleNoViewport);
SAMPLE_PROTOTYPES(RenderingTriangleFunction);
SAMPLE_PROTOTYPES(RenderingPoint);
SAMPLE_PROTOTYPES(RenderingLine);
SAMPLE_PROTOTYPES(RenderingQuad);

/*    CHAPTER    */
SAMPLE_PROTOTYPES(ShaderUniformColor);
SAMPLE_PROTOTYPES(ShaderUniformPos);
SAMPLE_PROTOTYPES(VertexColor);

/*    CHAPTER    */
SAMPLE_PROTOTYPES(LoadTexture);
SAMPLE_PROTOTYPES(TextureUvRange);
SAMPLE_PROTOTYPES(TextureOpWrapRepeat);
SAMPLE_PROTOTYPES(TextureOpWrapClamp);
SAMPLE_PROTOTYPES(TextureOpWrapMirroredRepeat);
SAMPLE_PROTOTYPES(TextureOpMagFilter);
SAMPLE_PROTOTYPES(TextureOpMinFilter);
SAMPLE_PROTOTYPES(TextureOpMipmapNMN);
SAMPLE_PROTOTYPES(TextureOpMipmapLMN);
SAMPLE_PROTOTYPES(TextureOpMipmapNML);
SAMPLE_PROTOTYPES(TextureOpMipmapLML);
SAMPLE_PROTOTYPES(TextureOpMipmapLML_ex);
SAMPLE_PROTOTYPES(TextureOpMipmapLML_ex2);
SAMPLE_PROTOTYPES(TextureOpMipmapGen);

/*    CHAPTER    */
SAMPLE_PROTOTYPES(TextureOpNpot);
SAMPLE_PROTOTYPES(TextureOpNpotRepeat);
SAMPLE_PROTOTYPES(TextureOpNpotAuto);
SAMPLE_PROTOTYPES(TextureOpNpotAutoRepeat);

/*    CHAPTER    */
SAMPLE_PROTOTYPES(BlendEnable);
SAMPLE_PROTOTYPES(LoadCompressTextureEtc1Pkm);
SAMPLE_PROTOTYPES(LoadCompressTextureEtc1Ktx);
SAMPLE_PROTOTYPES(LoadCompressTexturePvrPvrtc);


/*    CHAPTER    */
SAMPLE_PROTOTYPES(LoadTextureFunction);
SAMPLE_PROTOTYPES(TextureUse2);
SAMPLE_PROTOTYPES(2UnitTexture);
SAMPLE_PROTOTYPES(BlendTextureChannel);

/*    CHAPTER    */
SAMPLE_PROTOTYPES(RenderingPixelQuad);
SAMPLE_PROTOTYPES(RenderingPixelQuadDirectTex);
SAMPLE_PROTOTYPES(RenderingPixelQuadPixelUv);
SAMPLE_PROTOTYPES(RenderingPixelQuadPixelUv1px);

/*    CHAPTER    */
SAMPLE_PROTOTYPES(ShaderMatrixTranslate);
SAMPLE_PROTOTYPES(ShaderMatrixScale);
SAMPLE_PROTOTYPES(ShaderMatrixScale2);
SAMPLE_PROTOTYPES(ShaderMatrixRotate);
SAMPLE_PROTOTYPES(ShaderMatrixRotate2);
SAMPLE_PROTOTYPES(ShaderMatrixTRS);
SAMPLE_PROTOTYPES(ShaderMatrixSRT);
SAMPLE_PROTOTYPES(ShaderMatrixSRTTranspose);
SAMPLE_PROTOTYPES(ShaderMatrixSRTTransposeNotsupport);
SAMPLE_PROTOTYPES(ShaderMatrixMultiplyTRS);

/* CHAPTER */
SAMPLE_PROTOTYPES(ShaderMatrixSprite);
SAMPLE_PROTOTYPES(ShaderMatrixSpriteRotate);
SAMPLE_PROTOTYPES(ShaderMatrixSpriteAspect);
SAMPLE_PROTOTYPES(ShaderMatrixSpritePosFunction);
SAMPLE_PROTOTYPES(ShaderMatrixSpriteUV);
SAMPLE_PROTOTYPES(ShaderMatrixSpriteUVFunction);

#define SAMPLE_FUNCTIONS(name)  sample_##name##_initialize, sample_##name##_resized, sample_##name##_rendering, sample_##name##_destroy

static SampleInfo g_sample_initializes[] = {
//
        { "スケルトンプログラム", SAMPLE_FUNCTIONS(Skelton) },
        //
        { "単色で塗りつぶす", SAMPLE_FUNCTIONS(Clear) },
        //
        { "ランダムな色で塗りつぶす", SAMPLE_FUNCTIONS(RandomClear) },
        // 終端
        { "", NULL } };

static SampleInfo g_sample_triangles[] = {
//
        { "三角形を描画", SAMPLE_FUNCTIONS(RenderingTriangle) },
        //
        { "三角形を描画（glViewportなし）", SAMPLE_FUNCTIONS(RenderingTriangleNoViewport) },
        //
        { "三角形を描画（読込簡略化）", SAMPLE_FUNCTIONS(RenderingTriangleFunction) },
        // 終端
        { "", NULL } };

static SampleInfo g_sample_primitives[] = {
        //
        { "線を描画", SAMPLE_FUNCTIONS(RenderingLine) },
        //
        { "点を描画", SAMPLE_FUNCTIONS(RenderingPoint) },
        //
        { "四角形を描画", SAMPLE_FUNCTIONS(RenderingQuad) },
        // 終端
        { "", NULL } };

static SampleInfo g_sample_shader_uniforms[] = {
//
        { "描画色を設定する", SAMPLE_FUNCTIONS(ShaderUniformColor) },
        //
        { "頂点を動かす", SAMPLE_FUNCTIONS(ShaderUniformPos) },
        //
        { "頂点カラーを設定する", SAMPLE_FUNCTIONS(VertexColor) },
        // 終端
        { "", NULL } };

static SampleInfo g_sample_texture_load[] = {
//
        { "テクスチャ作成", SAMPLE_FUNCTIONS(LoadTexture) },
        // 終端
        { "", NULL } };

static SampleInfo g_sample_texture_params[] = {
//
        { "頂点のUV範囲を指定", SAMPLE_FUNCTIONS(TextureUvRange) },
        //
        { "WRAP設定(REPEAT)", SAMPLE_FUNCTIONS(TextureOpWrapRepeat) },
        //
        { "WRAP設定(CLAMP_TO_EDGE)", SAMPLE_FUNCTIONS(TextureOpWrapClamp) },
        //
        { "WRAP設定(MIRRORED_REPEAT)", SAMPLE_FUNCTIONS(TextureOpWrapMirroredRepeat) },
        //
        { "拡大フィルタ処理", SAMPLE_FUNCTIONS(TextureOpMagFilter) },
        //
        { "縮小フィルタ処理", SAMPLE_FUNCTIONS(TextureOpMinFilter) },
        //
        { "Mipmap(GL_NEAREST_MIPMAP_NEAREST)", SAMPLE_FUNCTIONS(TextureOpMipmapNMN) },
        //
        { "Mipmap(GL_LINEAR_MIPMAP_NEAREST)", SAMPLE_FUNCTIONS(TextureOpMipmapLMN) },
        //
        { "Mipmap(GL_NEAREST_MIPMAP_LINEAR)", SAMPLE_FUNCTIONS(TextureOpMipmapNML) },
        //
        { "Mipmap(GL_LINEAR_MIPMAP_LINEAR)", SAMPLE_FUNCTIONS(TextureOpMipmapLML) },
        //
        { "mipmap自動生成", SAMPLE_FUNCTIONS(TextureOpMipmapGen) },
        //
        { "Mipmapの問題点", SAMPLE_FUNCTIONS(TextureOpMipmapLML_ex) },
        //
        { "Mipmapの問題点を回避", SAMPLE_FUNCTIONS(TextureOpMipmapLML_ex2) },
        //
        { "npotテクスチャ", SAMPLE_FUNCTIONS(TextureOpNpot) },
        //
        { "npotテクスチャ(REPEAT)", SAMPLE_FUNCTIONS(TextureOpNpotRepeat) },
        //
        { "npot拡張による分岐", SAMPLE_FUNCTIONS(TextureOpNpotAuto) },
        //
        { "拡張機能互換シェーダー", SAMPLE_FUNCTIONS(TextureOpNpotAutoRepeat) },

        // 終端
        { "", NULL } };

static SampleInfo g_sample_compress_texture[] = {
//
        { "透過の有効化", SAMPLE_FUNCTIONS(BlendEnable) },
        //
        { "ETC1圧縮テクスチャ(PKMファイル)", SAMPLE_FUNCTIONS(LoadCompressTextureEtc1Pkm) },
        //
        { "ETC1圧縮テクスチャ(KTXファイル)", SAMPLE_FUNCTIONS(LoadCompressTextureEtc1Ktx) },
        //
        { "PVR圧縮テクスチャ(PVRTCファイル)", SAMPLE_FUNCTIONS(LoadCompressTexturePvrPvrtc) },
        //

        // 終端
        { "", NULL } };

static SampleInfo g_sample_texture_muti[] = {
//
        { "２枚のテクスチャを読み込む", SAMPLE_FUNCTIONS(TextureUse2) },
        //
        { "読込の関数化（実装例）", SAMPLE_FUNCTIONS(LoadTextureFunction) },
        //
        { "テクスチャユニットを切り替える", SAMPLE_FUNCTIONS(2UnitTexture) },
        //
        { "複数テクスチャを同時に利用する", SAMPLE_FUNCTIONS(BlendTextureChannel) },
//
// 終端
        { "", NULL } };

static SampleInfo g_sample_sprite[] = {
//
        { "ピクセル指定で四角形描画", SAMPLE_FUNCTIONS(RenderingPixelQuad) },
        //
        { "dot by dotでテクスチャ描画", SAMPLE_FUNCTIONS(RenderingPixelQuadDirectTex) },
        //
        { "ピクセル指定でテクスチャ描画", SAMPLE_FUNCTIONS(RenderingPixelQuadPixelUv) },
        //
        { "ピクセル指定でテクスチャ描画（参考用）", SAMPLE_FUNCTIONS(RenderingPixelQuadPixelUv1px) },
// 終端
        { "", NULL } };

static SampleInfo g_sample_shader_move_vertex[] = {
//
        { "移動行列で頂点を制御する", SAMPLE_FUNCTIONS(ShaderMatrixTranslate) },
        //
        { "拡大縮小行列で頂点を制御する", SAMPLE_FUNCTIONS(ShaderMatrixScale) },
        //
        { "拡大縮小行列で頂点を制御する(原点の例外)", SAMPLE_FUNCTIONS(ShaderMatrixScale2) },
        //
        { "回転行列で頂点を制御する", SAMPLE_FUNCTIONS(ShaderMatrixRotate) },
        //
        { "回転行列で頂点を制御する(原点の例外)", SAMPLE_FUNCTIONS(ShaderMatrixRotate2) },
        //
        { "複数の行列を利用して制御する(T/R/S)", SAMPLE_FUNCTIONS(ShaderMatrixTRS) },
        //
        { "複数の行列を利用して制御する(S/R/T)", SAMPLE_FUNCTIONS(ShaderMatrixSRT) },
        //
        { "複数の行列を利用して制御する(転置S/R/T)", SAMPLE_FUNCTIONS(ShaderMatrixSRTTranspose) },
        //
        { "非対応端末で転置行列を利用する", SAMPLE_FUNCTIONS(ShaderMatrixSRTTransposeNotsupport) },
        //
        { "乗算済みの行列を利用して制御する(T/R/S)", SAMPLE_FUNCTIONS(ShaderMatrixMultiplyTRS) },
// 終端
        { "", NULL } };

static SampleInfo g_sample_shader_matrix_sprite[] = {
//
        { "行列でスプライト座標へ変換する", SAMPLE_FUNCTIONS(ShaderMatrixSprite) },
        //
        { "行列で回転したスプライト座標へ変換する", SAMPLE_FUNCTIONS(ShaderMatrixSpriteRotate) },
        //
        { "行列で回転したスプライト座標へ変換する（歪み補正）", SAMPLE_FUNCTIONS(ShaderMatrixSpriteAspect) },
        //
        { "行列でスプライト座標へ変換する（関数化）", SAMPLE_FUNCTIONS(ShaderMatrixSpritePosFunction) },
        //
        { "行列でUV座標を変換する", SAMPLE_FUNCTIONS(ShaderMatrixSpriteUV) },
        //
        { "行列でUV座標を変換する（関数化）", SAMPLE_FUNCTIONS(ShaderMatrixSpriteUVFunction) },
        // 終端
        { "", NULL } };

#define BOOKCHAPTER_INDEX_OFFSET -1

static ChapterInfo g_chapter_info[] = {
//
        { "Hello OpenGL ES 2.0!!", 5, g_sample_initializes },
        //
        { "三角形の事始め", 6, g_sample_triangles },
        //
        { "三角形以外のプリミティブ", 7, g_sample_primitives },
        //
        { "シェーダーの基礎", 8, g_sample_shader_uniforms },
        //
        { "テクスチャの基礎", 9, g_sample_texture_load },
        //
        { "テクスチャ用のパラメータ", 10, g_sample_texture_params },
        //
        { "複数枚のテクスチャを利用する", 11, g_sample_texture_muti },
        //
        { "スプライト描画", 12, g_sample_sprite },
        //
        { "シェーダーと行列", 13, g_sample_shader_move_vertex },
        //
        { "行列の実践的な利用(2D編)", 14, g_sample_shader_matrix_sprite },
        //
        { "高度なテクスチャ利用", 15, g_sample_compress_texture },
// 終端
        { "", -1, NULL }, };

/**
 * アプリで解説するチャプター数を取得する
 */
int SampleList_getChapterNum() {
    int result = 0;

// サンプルアプリ情報が設定されているなら、次を参照する
    while (g_chapter_info[result].sample_info != NULL) {
        ++result;
    }
    return result;
}

/**
 * 任意の番号のチャプター番号を返す。
 */
ChapterInfo* SampleList_getChapter(int chapter_num) {
    return g_chapter_info + chapter_num;
}

/**
 * チャプターに所属するサンプル数を取得する。
 */
int SamplesList_getChapterSampleNum(int chapter_num) {
    SampleInfo *info = g_chapter_info[chapter_num].sample_info;
    if (!info) {
        return 0;
    }

    int result = 0;
    while (info[result].func_initialize) {
        ++result;
    }
    return result;
}

/**
 * チャプターサンプルの情報を取得する
 */
SampleInfo* SampleList_getSample(int chapter_num, int sample_num) {
    return g_chapter_info[chapter_num].sample_info + sample_num;
}
