//=============================================================================
//
// BGモデル処理 [BG.cpp]
// Author : HAL東京 高橋諒
//
//=============================================================================
#include "main.h"
#include "model.h"

#include "renderer.h"
#include "input.h"
#include "camera.h"
#include "debugproc.h"
#include "BG.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MODEL_BG		"data/MODEL/BG/BG.obj"			// 読み込むモデル名

#define MAX_BG			(1)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static BG							g_BG;					// ポリゴンデータ
static BG_MODEL						g_BGModel;				// ポリゴンデータ
static int							g_TexNo;				// テクスチャ番号



//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitBG(void)
{

	LoadModel(MODEL_BG, &g_BGModel.model);


	// 位置・回転・スケールの初期設定
	g_BG.pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_BG.rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_BG.scl = XMFLOAT3(300.0f, 300.0f, 300.0f);
	g_BG.modelIdx = NULL;

	g_BGModel.load = TRUE;
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitBG(void)
{

	// モデルの解放処理
	if (g_BGModel.load)
	{
		UnloadModel(&g_BGModel.model);
		g_BGModel.load = false;

	}
}
//=============================================================================
// 更新処理
//=============================================================================
void UpdateBG(void)
{
	g_BG.rot.y += 0.001f;	// ちょっとだけ回す
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawBG(void)
{
	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

	//背景の描画

	// カリング無効
	SetCullingMode(CULL_MODE_FRONT);

	//ワールドマトリックスの初期化y
	mtxWorld = XMMatrixIdentity();

	// スケールを反映
	mtxScl = XMMatrixScaling(g_BG.scl.x, g_BG.scl.y, g_BG.scl.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

	// 回転を反映
	mtxRot = XMMatrixRotationRollPitchYaw(g_BG.rot.x, g_BG.rot.y, g_BG.rot.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

	// 移動を反映
	mtxTranslate = XMMatrixTranslation(g_BG.pos.x, g_BG.pos.y, g_BG.pos.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

	// ワールドマトリックスの設定
	SetWorldMatrix(&mtxWorld);

	XMStoreFloat4x4(&g_BG.mtxWorld, mtxWorld);

	//モデル描画
	DrawModel(&g_BGModel.model);

	// カリング無効
	SetCullingMode(CULL_MODE_BACK);
}