//=============================================================================
//
// objモデル処理 [0bj.cpp]
// Author : HAL東京 高橋諒
//
//=============================================================================
#include "main.h"
#include "model.h"

#include "renderer.h"
#include "input.h"
#include "camera.h"
#include "debugproc.h"
#include "obj.h"
#include "player.h"
#include "event.h"
#include "sound.h"
#include "flag.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MODEL_PILLAR		"data/MODEL/pillar/pillar.obj"			// 読み込むモデル名

#define MAX_PILLAR		(4)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static OBJ							g_Obj;				// ポリゴンデータ
static OBJ_MODEL					g_ObjModel;				// ポリゴンデータ
static int							g_TexNo;				// テクスチャ番号



//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitObj(void)
{

	LoadModel(MODEL_PILLAR, &g_ObjModel.model);


	// 位置・回転・スケールの初期設定
	g_Obj.pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_Obj.rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_Obj.scl = XMFLOAT3(10.0f, 10.0f, 10.0f);
	g_Obj.use = TRUE;

	g_Obj.modelIdx = NULL;

	g_TexNo = 0;
	g_ObjModel.load = TRUE;
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitObj(void)
{

	// モデルの解放処理
	if (g_ObjModel.load)
	{
		UnloadModel(&g_ObjModel.model);
		g_ObjModel.load = false;

	}
}
//=============================================================================
// 更新処理
//=============================================================================
void UpdateObj(void)
{
	CAMERA* cam = GetCamera();
	PLAYER* player = GetPlayer();
	EVENT* HI = GetEvent();

	if (g_Obj.use == FALSE)return;

	if (GetFlag() == FLAG_GAME_EVENT_PILLAR)
	{
		float x = (float)(rand() % 4 - 2) * 0.1f;
		float z = (float)(rand() % 4 - 2) * 0.1f;

		g_Obj.pos.x -= x;
		g_Obj.pos.y -= 0.3f;
		g_Obj.pos.z -= z;

		g_Obj.rot.y += 0.1f;
	}

	if (g_Obj.pos.y <= -10.0f)
	{
		for (int i = 0; i < MAX_HI; i++)
		{
			HI[i].use = FALSE;
		}
	}

	if (g_Obj.pos.y <= -70.0f)
	{
		SetFlag(FLAG_GAME_BATTLE);

		// バトルに入ったら上限にもどす
		player->hp = PLAYER_HP_MAX;
		player->mp = PLAYER_MP_MAX;
		cam->pos.y = POS_Y_CAM; // カメラの座標を元に戻している

		// BGM再生
		PlaySound(SOUND_LABEL_BGM_GAME);

		g_Obj.use = FALSE;
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawObj(void)
{
	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

	for (int i = 0; i < MAX_PILLAR; i++)
	{
		//柱の描画
		//ワールドマトリックスの初期化y
		mtxWorld = XMMatrixIdentity();

		// スケールを反映
		mtxScl = XMMatrixScaling(1.0, 1.0, 1.0);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// 回転を反映
		mtxRot = XMMatrixRotationRollPitchYaw(g_Obj.rot.x, g_Obj.rot.y, g_Obj.rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// 移動を反映
		mtxTranslate = XMMatrixTranslation(g_Obj.pos.x - 50.0f, g_Obj.pos.y, g_Obj.pos.z - 300.0f + (i * 100.0f));
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ワールドマトリックスの設定
		SetWorldMatrix(&mtxWorld);

		XMStoreFloat4x4(&g_Obj.mtxWorld, mtxWorld);

		//モデル描画
		DrawModel(&g_ObjModel.model);
	}



	for (int i = 0; i < MAX_PILLAR; i++)
	{
		//柱の描画
		//ワールドマトリックスの初期化y
		mtxWorld = XMMatrixIdentity();

		// スケールを反映
		mtxScl = XMMatrixScaling(1.0, 1.0, 1.0);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// 回転を反映
		mtxRot = XMMatrixRotationRollPitchYaw(g_Obj.rot.x, g_Obj.rot.y, g_Obj.rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// 移動を反映
		mtxTranslate = XMMatrixTranslation(g_Obj.pos.x + 50.0f, g_Obj.pos.y, g_Obj.pos.z - 300.0f + (i * 100.0f));
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ワールドマトリックスの設定
		SetWorldMatrix(&mtxWorld);

		XMStoreFloat4x4(&g_Obj.mtxWorld, mtxWorld);

		//モデル描画
		DrawModel(&g_ObjModel.model);
	}
}


OBJ* GetObj(void)
{
	return &g_Obj;
}
