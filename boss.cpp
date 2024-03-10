//=============================================================================
//
// ボスモデル処理 [boss.cpp]
// Author : GP11B132 14 高橋諒GP11B132 14 高橋　諒
//
//=============================================================================
#include "main.h"
#include "model.h"
#include "renderer.h"
#include "input.h"
#include "debugproc.h"
#include "boss.h"
#include "shadow.h"
#include "light.h"
#include "particle.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MODEL_BOSS			"data/MODEL/boss/boss.obj"	// 読み込むモデル名

#define	VALUE_MOVE			(5.0f)						// 移動量
#define	VALUE_ROTATE		(XM_PI * 0.02f)				// 回転量

#define BOSS_SHADOW_SIZE	(0.4f)						// 影の大きさ
#define BOSS_OFFSET_Y		(100.0f)					// ボスの足元をあわせる



//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static BOSS			g_Boss[MAX_BOSS];				// エネミー

int g_Boss_load = 0;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitBoss(void)
{
	for (int i = 0; i < MAX_BOSS; i++)
	{
		LoadModel(MODEL_BOSS, &g_Boss[i].model);


		g_Boss[i].load = TRUE;

		g_Boss[i].pos = XMFLOAT3(0.0f, BOSS_OFFSET_Y, 800.0f);
		g_Boss[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Boss[i].scl = XMFLOAT3(0.5f, 0.4f, 0.4f);

		g_Boss[i].spd = 0.0f;			// 移動スピードクリア
		g_Boss[i].size = BOSS_SIZE;	// 当たり判定の大きさ

		g_Boss[i].use = TRUE;		// TRUE:生きてる

	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitBoss(void)
{

	for (int i = 0; i < MAX_BOSS; i++)
	{
		if (g_Boss[i].load)
		{
			UnloadModel(&g_Boss[i].model);
			g_Boss[i].load = FALSE;
		}
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateBoss(void)
{
#ifdef _DEBUG

	if (GetKeyboardTrigger(DIK_P))
	{
		// モデルの色を変更
		for (int j = 0; j < g_Boss[0].model.SubsetNum; j++)
		{
			SetModelDiffuse(&g_Boss[0].model, j, XMFLOAT4(1.0f, 0.0f, 0.0f, 0.5f));
		}
	}

	if (GetKeyboardTrigger(DIK_L))
	{
		// モデルの色を元に戻している
		for (int j = 0; j < g_Boss[0].model.SubsetNum; j++)
		{
			SetModelDiffuse(&g_Boss[0].model, j, g_Boss[0].diffuse[j]);
		}
	}


#endif


}

//=============================================================================
// 描画処理
//=============================================================================
void DrawBoss(void)
{
	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

	// カリング無効
	SetCullingMode(CULL_MODE_NONE);



	// ライティングを無効
	SetLightEnable(FALSE);

	for (int i = 0; i < MAX_BOSS; i++)
	{
		//boss
		if (g_Boss[i].use == TRUE)
		{
			// ライティングを無効
			SetLightEnable(TRUE);

			// ワールドマトリックスの初期化
			mtxWorld = XMMatrixIdentity();

			// スケールを反映
			mtxScl = XMMatrixScaling(g_Boss[i].scl.x * 30, g_Boss[i].scl.y * 30, g_Boss[i].scl.z * 30);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

			// 回転を反映
			mtxRot = XMMatrixRotationRollPitchYaw(g_Boss[i].rot.x, g_Boss[i].rot.y + XM_PI, g_Boss[i].rot.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

			// 移動を反映
			mtxTranslate = XMMatrixTranslation(g_Boss[i].pos.x, g_Boss[i].pos.y - 50.0f, g_Boss[i].pos.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

			// ワールドマトリックスの設定
			SetWorldMatrix(&mtxWorld);

			XMStoreFloat4x4(&g_Boss[i].mtxWorld, mtxWorld);


			// モデル描画
			DrawModel(&g_Boss[0].model);
		}
	}
	// カリング設定を戻す
	SetCullingMode(CULL_MODE_BACK);
	// ライティングを無効
	SetLightEnable(TRUE);

}

//=============================================================================
// ボスの取得
//=============================================================================
BOSS* GetBoss()
{
	return &g_Boss[0];
}
