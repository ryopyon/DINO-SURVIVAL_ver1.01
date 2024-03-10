//=============================================================================
//
// イベント処理 [event.cpp]
// Author : HAL東京 高橋諒
//
//=============================================================================
#include "main.h"
#include "model.h"
#include "renderer.h"
#include "input.h"
#include "debugproc.h"
#include "event.h"
#include "shadow.h"
#include "light.h"
#include "particle.h"
#include "fade.h"
#include "flag.h"
#include "camera.h"
#include "sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	VALUE_MOVE			(5.0f)						// 移動量
#define	VALUE_ROTATE		(XM_PI * 0.02f)				// 回転量

#define HI_SHADOW_SIZE		(0.4f)						// 影の大きさ
#define HI_OFFSET_Y			(5.9f)						// エネミーの足元をあわせる

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static EVENT			g_Hi[MAX_HI];				// 火の数

// イベント用の線形保管データ
static INTERPOLATION_DATA g_MoveTbl0[] = {
	//座標								回転率							拡大率						色
	{ XMFLOAT3( 0.0f,  7.0f,  -350.0f), 	XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f),	120 },
	{ XMFLOAT3( 0.0f,  7.0f,  -350.0f), 	XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f),	120 },

	{ XMFLOAT3(50.0f,  50.0f, -300.0f), 	XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f),	60 },
	{ XMFLOAT3(50.0f,  45.0f, -300.0f), 	XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f),	10 },
	{ XMFLOAT3(50.0f,  50.0f, -300.0f), 	XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f),	60 },

	{ XMFLOAT3(-50.0f, 50.0f, -300.0f), 	XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f),	60 },
	{ XMFLOAT3(-50.0f, 45.0f, -300.0f),		XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f), 10 },
	{ XMFLOAT3(-50.0f, 50.0f, -300.0f),		XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f),	60 },

	{ XMFLOAT3(-50.0f, 50.0f, -200.0f),		XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(-50.0f, 45.0f, -200.0f),		XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f), 10 },
	{ XMFLOAT3(-50.0f, 50.0f, -200.0f),		XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f),	60 },

	{ XMFLOAT3( 50.0f, 50.0f, -200.0f),		XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f),	60 },
	{ XMFLOAT3( 50.0f, 45.0f, -200.0f),		XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f),	10 },
	{ XMFLOAT3( 50.0f, 50.0f, -200.0f),		XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f),	60 },

	{ XMFLOAT3( 50.0f, 50.0f, -100.0f),		XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3( 50.0f, 45.0f, -100.0f),		XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f), 10 },
	{ XMFLOAT3( 50.0f, 50.0f, -100.0f),		XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },

	{ XMFLOAT3(-50.0f, 50.0f, -100.0f),		XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(-50.0f, 45.0f, -100.0f),		XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f), 10 },
	{ XMFLOAT3(-50.0f, 50.0f, -100.0f),		XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },

	{ XMFLOAT3(-50.0f, 50.0f,  0.0f),		XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(-50.0f, 45.0f,  0.0f),		XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f), 10 },
	{ XMFLOAT3(-50.0f, 50.0f,  0.0f),		XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },

	{ XMFLOAT3( 50.0f, 50.0f,  0.0f),		XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3( 50.0f, 45.0f,  0.0f),		XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f),  1 },
};

int g_cnt;


static INTERPOLATION_DATA* g_standbyTblAdr[] =
{
	g_MoveTbl0,
};


int g_Hi_load = 0;
int g_lightF;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitEvent(void)
{


	for (int i = 0; i < MAX_HI; i++)
	{
		g_Hi[i].pos = XMFLOAT3(0.0f, HI_OFFSET_Y, -350.0f);
		g_Hi[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Hi[i].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);

		g_Hi[i].spd  = 0.0f;			// 移動スピードクリア
		g_Hi[i].size = HI_SIZE;			// 当たり判定の大きさ

		// 0番だけ線形補間で動かしてみる
		g_Hi[i].time = 0.0f;		// 線形補間用のタイマーをクリア
		g_Hi[i].tblNo = 0;		// 再生するアニメデータの先頭アドレスをセット
		g_Hi[i].tblMax = 0;	// 再生するアニメデータのレコード数をセット

		g_Hi[i].use = FALSE;		// TRUE:生きてる

	}

	// 0番だけ線形補間で動かしてみる
	g_Hi[0].time = 0.0f;		// 線形補間用のタイマーをクリア
	g_Hi[0].tblNo = 0;		// 再生するアニメデータの先頭アドレスをセット
	g_Hi[0].tblMax = sizeof(g_MoveTbl0) / sizeof(INTERPOLATION_DATA);	// 再生するアニメデータのレコード数をセット
	g_Hi[0].use = TRUE;

	g_cnt = 0;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitEvent(void)
{

}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateEvent(void)
{
	for (int i = 0; i < MAX_HI; i++)
	{
		if (g_Hi[i].use == TRUE)		// この火が使われている？
		{								// Yes
			if (g_Hi[i].tblMax > 0 && GetFlag() == FLAG_GAME_EVENT_FIRE)	// 線形補間を実行する？
			{	// 線形補間の処理
				int nowNo = (int)g_Hi[i].time;			// 整数分であるテーブル番号を取り出している
				int maxNo = g_Hi[i].tblMax;				// 登録テーブル数を数えている
				int nextNo = (nowNo + 1) % maxNo;			// 移動先テーブルの番号を求めている

				INTERPOLATION_DATA* tbl = g_standbyTblAdr[g_Hi[i].tblNo];	// 行動テーブルのアドレスを取得

				XMVECTOR nowPos = XMLoadFloat3(&tbl[nowNo].pos);	// XMVECTORへ変換

				XMVECTOR Pos = XMLoadFloat3(&tbl[nextNo].pos) - nowPos;	// XYZ移動量を計算している

				float nowTime = g_Hi[i].time - nowNo;	// 時間部分である少数を取り出している

				Pos *= nowTime;								// 現在の移動量を計算している

				// 計算して求めた移動量を現在の移動テーブルXYZに足している＝表示座標を求めている
				XMStoreFloat3(&g_Hi[i].pos, nowPos + Pos);


				// frameを使て時間経過処理をする
				g_Hi[i].time += 1.0f / tbl[nowNo].frame;	// 時間を進めている

				g_cnt++;

				switch (g_cnt)
				{
				case 300:
				case 430:
				case 560:
				case 690:
				case 820:
				case 950:
				case 1080:
				case 1210:
					for (int j = 1; j < MAX_HI; j++)
					{
						if (g_Hi[j].use == FALSE)
						{
							g_Hi[j].pos = g_Hi[i].pos;
							g_Hi[j].use = TRUE;
							PlaySound(SOUND_LABEL_SE_FIRE);
							break;
						}
					}
					break;

				}

				// イベントスキップ
				if (GetKeyboardTrigger(DIK_RIGHT) || IsButtonTriggered(0, BUTTON_B))
				{
					EventSkip();
				}


				if ((int)g_Hi[i].time >= maxNo - 1)			// 登録テーブル最後まで移動したか？
				{
					g_Hi[i].time -= maxNo;				// ０番目にリセットしつつも小数部分を引き継いでいる

					g_Hi[i].use = FALSE;

					// 明かりをつける
					{
						LIGHT* light = GetLightData(0);

						// 明るさを徐々に上げる係数
						float brightnessIncreaseRate = 0.1f; // 徐々に上げる速度を調整するための値

						// 現在の明るさを取得
						float currentBrightness = light[0].Diffuse.x; // Diffuse.xを明るさとして使用

						for (int i = 0; i < 10; i++)
						{
							// 明るさを徐々に上げる
							currentBrightness += brightnessIncreaseRate; // 明るさを増加させる

							// 明るさが1より大きい場合は1にクランプする
							if (currentBrightness > 1.0f)
							{
								currentBrightness = 1.0f;
							}

							// 新しいDiffuse値を設定
							light[0].Diffuse = XMFLOAT4(currentBrightness, currentBrightness, currentBrightness, 1.0f); // 各色成分を同じ値に設定

							SetLightData(0, &light[0]);
						}
						
					}

				}

			}

			// 火の処理
			float hi_x = (float)(rand() % 4 - 2) * 0.1f;
			float hi_z = (float)(rand() % 4 - 2) * 0.1f;
			SetParticle(g_Hi[i].pos, XMFLOAT3(0.7f, 0.7f, 0.7f), XMFLOAT3(hi_x, 1.25f, hi_z), XMFLOAT4(1.0f, 0.5f, 0.1f, 0.85f), TYPE_HINOKO_RED, BLEND_MODE_ADD, 0.0f, 0.0f, 30);

		}
	}

#ifdef _DEBUG

	PrintDebugProc(" Count: %d\n", g_cnt);

#endif


}

//=============================================================================
// 描画処理
//=============================================================================
void DrawEvent(void)
{

}

//=============================================================================
// エネミーの取得
//=============================================================================
EVENT *GetEvent()
{
	return &g_Hi[0];
}

void EventSkip(void)
{
	CAMERA* cam = GetCamera();

	for (int i = 0; i < MAX_HI; i++)
	{
		if (g_Hi[i].use)continue;

		XMFLOAT3 pos = XMFLOAT3(0.0f, 0.0f, 0.0f);

		switch (i)
		{
			case 1:
				pos = XMFLOAT3( 50.0f, 45.0f, -300.0f);
			break;

			case 2:
				pos = XMFLOAT3(-50.0f, 45.0f, -300.0f);
			break;

			case 3:
				pos = XMFLOAT3(-50.0f, 45.0f, -200.0f);
			break;

			case 4:
				pos = XMFLOAT3( 50.0f, 45.0f, -200.0f);
			break;

			case 5:
				pos = XMFLOAT3( 50.0f, 45.0f, -100.0f);

			case 6:
				pos = XMFLOAT3(-50.0f, 45.0f, -100.0f);
			break;

			case 7:
				pos = XMFLOAT3(-50.0f, 45.0f,  0.0f);
			break;

			case 8:
				pos = XMFLOAT3( 50.0f, 45.0f, 0.0f);
			break;

		}

		g_Hi[i].pos = pos;
		g_Hi[i].use = TRUE;
	}

	g_Hi[0].use = FALSE;
	cam->pos = XMFLOAT3(0.0f, 25.0f, -450.0f);
	cam->at = XMFLOAT3(0.0f, 7.0f, -350.0f);
	SetFlag(FLAG_GAME_EVENT_PILLAR);

	{
		LIGHT* light = GetLightData(0);

		// 明るさを徐々に上げる係数
		float brightnessIncreaseRate = 0.1f; // 徐々に上げる速度を調整するための値

		// 現在の明るさを取得
		float currentBrightness = light[0].Diffuse.x; // Diffuse.xを明るさとして使用

		for (int i = 0; i < 10; i++)
		{
			// 明るさを徐々に上げる
			currentBrightness += brightnessIncreaseRate; // 明るさを増加させる

			// 明るさが1より大きい場合は1にクランプする
			if (currentBrightness > 1.0f)
			{
				currentBrightness = 1.0f;
			}

			// 新しいDiffuse値を設定
			light[0].Diffuse = XMFLOAT4(currentBrightness, currentBrightness, currentBrightness, 1.0f); // 各色成分を同じ値に設定

			SetLightData(0, &light[0]);

		}
	}

	PlaySound(SOUND_LABEL_SE_PILLAR_DOWN);
	PlaySound(SOUND_LABEL_SE_FIRE);

}