//=============================================================================
//
// ポーズ処理 [pause.cpp]
// Author : HAL東京 高橋諒
//
//=============================================================================
#include "main.h"
#include "pause.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "sprite.h"
#include "game.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_WIDTH				(SCREEN_WIDTH)	// 背景サイズ
#define TEXTURE_HEIGHT				(SCREEN_HEIGHT)	// 

#define TEXTURE_MAX					(6)


#define PAUSE_CUR1_X				(760.0f)		//	カーソルの位置
#define PAUSE_CUR1_Y				(290.0f)

#define PAUSE_CUR2_X				(760.0f)
#define PAUSE_CUR2_Y				(350.0f)

#define PAUSE_CUR3_X				(760.0f)		
#define PAUSE_CUR3_Y				(405.0f)

#define PAUSE_CUR4_X				(760.0f)
#define PAUSE_CUR4_Y				(460.0f)

#define CUR_WIDTH					(50)			//カーソルサイズ
#define CUR_HEIGHT					(50)


#define ENEMY_WAIT					(4)				// アニメーションの待ちフレーム数

#define TEXTURE_PATTERN_DIVIDE_X	(7)				// アニメパターンのテクスチャ内分割数（X)
#define TEXTURE_PATTERN_DIVIDE_Y	(1)				// アニメパターンのテクスチャ内分割数（Y)
#define ANIM_PATTERN_NUM			(TEXTURE_PATTERN_DIVIDE_X*TEXTURE_PATTERN_DIVIDE_Y)	// アニメーションパターン数

enum
{
	PAUSE_CUR_EYELLOW,
	PAUSE_CUR_EGREEN,
	PAUSE_CUR_EBLUE,
	PAUSE_CUR_ERED,

	PAUSE_CUR_MAX,
};

enum 
{
	BG = 4,
	BG_MENU,
};

enum 
{
	PAUSE_CONTINUE,
	PAUSE_RETRY,
	PAUSE_TITLE,
	PAUSE_EXIT,
};



//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************


static ID3D11Buffer* g_VertexBuffer = NULL;				// 頂点情報
static ID3D11ShaderResourceView* g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報

static char* g_TexturName[TEXTURE_MAX] = {
	"data/TEXTURE/cursor/Eyellow.png",
	"data/TEXTURE/cursor/Egreen.png",
	"data/TEXTURE/cursor/Eblue.png",
	"data/TEXTURE/cursor/Ered.png",
	"data/TEXTURE/fade_black.png",
	"data/TEXTURE/pause/pause.png",
};

static BOOL						g_Use;						// TRUE:使っている  FALSE:未使用
static float					g_w, g_h;					// 幅と高さ
static XMFLOAT3					g_Pos;						// ポリゴンの座標
static int						g_TexNo;					// テクスチャ番号

static BOOL						g_ResetPause;				// リセットしてるかどうかの判定

static int cursor;
static int Epattern;
static float EanimCnt;


static BOOL						g_Load = FALSE;


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitPause(void)
{
	ID3D11Device *pDevice = GetDevice();

	//テクスチャ生成
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		g_Texture[i] = NULL;
		D3DX11CreateShaderResourceViewFromFile(GetDevice(),
			g_TexturName[i],
			NULL,
			NULL,
			&g_Texture[i],
			NULL);
	}


	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);

	// 変数の初期化
	g_Use = TRUE;
	g_w = TEXTURE_WIDTH;
	g_h = TEXTURE_HEIGHT;
	g_Pos = { g_w / 2, 50.0f, 0.0f };
	g_TexNo = 0;
	cursor = 0;

	g_ResetPause = FALSE;

	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitPause(void)
{
	if (g_Load == FALSE) return;

	if (g_VertexBuffer)
	{
		g_VertexBuffer->Release();
		g_VertexBuffer = NULL;
	}

	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		if (g_Texture[i])
		{
			g_Texture[i]->Release();
			g_Texture[i] = NULL;
		}
	}

	g_Load = FALSE;
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdatePause(void)
{
	GAME* game = GetGame();

	//ポーズ中の時にポーズ画面処理
	if (game->bPause == TRUE)
	{
		// アニメーション  
		EanimCnt += 1.0f;
		if (EanimCnt > ENEMY_WAIT)
		{
			EanimCnt = 0.0f;
			// パターンの切り替え
			Epattern = (Epattern + 1) % ANIM_PATTERN_NUM;
		}

		// カーソルの切り替え
		if (GetKeyboardTrigger(DIK_DOWN) || GetKeyboardTrigger(DIK_S) || IsButtonTriggered(0, STICK_L_DOWN) || IsButtonTriggered(0, BUTTON_DOWN))
		{
			PlaySound(SOUND_LABEL_SE_CRICK);
			cursor = (++cursor + PAUSE_CUR_MAX) % PAUSE_CUR_MAX;
		}
		if (GetKeyboardTrigger(DIK_UP) || GetKeyboardTrigger(DIK_W) || IsButtonTriggered(0, STICK_L_UP) || IsButtonTriggered(0, BUTTON_UP))
		{
			PlaySound(SOUND_LABEL_SE_CRICK);
			cursor = (--cursor + PAUSE_CUR_MAX) % PAUSE_CUR_MAX;
		}



		switch (cursor)		//カーソルの表記
		{
		case PAUSE_CONTINUE:
			g_TexNo = PAUSE_CUR_EGREEN;
			g_Pos = XMFLOAT3(PAUSE_CUR1_X, PAUSE_CUR1_Y, 0.0f);

			if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(0, BUTTON_START) || IsButtonTriggered(0, BUTTON_B))
			{// ステージ切り替え
				PlaySound(SOUND_LABEL_SE_KETTEI);
				game->bPause = FALSE;
			}
			break;

		case PAUSE_RETRY:
			g_TexNo = PAUSE_CUR_EYELLOW;
			g_Pos = XMFLOAT3(PAUSE_CUR2_X, PAUSE_CUR2_Y, 0.0f);

			if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(0, BUTTON_START) || IsButtonTriggered(0, BUTTON_B))
			{// ステージ切り替え
				PlaySound(SOUND_LABEL_SE_KETTEI);
				SetFade(FADE_OUT, MODE_GAME);
				game->bPause = FALSE;
			}
			break;

		case PAUSE_TITLE:
			g_TexNo = PAUSE_CUR_EBLUE;
			g_Pos = XMFLOAT3(PAUSE_CUR3_X, PAUSE_CUR3_Y, 0.0f);

			if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(0, BUTTON_START) || IsButtonTriggered(0, BUTTON_B))
			{// ステージ切り替え
				PlaySound(SOUND_LABEL_SE_KETTEI);
				SetFade(FADE_OUT, MODE_TITLE);
			}
			break;

		case PAUSE_EXIT:
			g_TexNo = PAUSE_CUR_ERED;
			g_Pos = XMFLOAT3(PAUSE_CUR4_X, PAUSE_CUR4_Y, 0.0f);

			if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(0, BUTTON_START) || IsButtonTriggered(0, BUTTON_B))
			{
				GameLoop();
			}
			break;

		}
	}

	g_ResetPause = FALSE;

#ifdef _DEBUG	// デバッグ情報を表示する
	
#endif

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawPause(void)
{
	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// マトリクス設定
	SetWorldViewProjection2D();

	// プリミティブトポロジ設定
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);



	// 半透明の黒い背景を描画
	{
		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[BG]);
		
		//表示位置
		float pw = SCREEN_WIDTH;
		float ph = SCREEN_HEIGHT;
		float px = SCREEN_WIDTH / 2;
		float py = SCREEN_HEIGHT / 2;

		float tw = 1.0f;				// テクスチャの幅
		float th = 1.0f;				// テクスチャの高さ
		float tx = 0.0f;				// テクスチャの左上X座標
		float ty = 0.0f;				// テクスチャの左上Y座標

		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSpriteColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
			XMFLOAT4(1.0f, 1.0f, 1.0f, 0.5f));

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);
	}

	// ポーズメニューを描画
	{
		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[BG_MENU]);

		//表示位置
		float pw = SCREEN_WIDTH;
		float ph = SCREEN_HEIGHT;
		float px = SCREEN_WIDTH / 2;
		float py = SCREEN_HEIGHT / 2;

		float tw = 1.0f;				// テクスチャの幅
		float th = 1.0f;				// テクスチャの高さ
		float tx = 0.0f;				// テクスチャの左上X座標
		float ty = 0.0f;				// テクスチャの左上Y座標

		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSpriteColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);

	}

	//カーソルの描画
	{
		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_TexNo]);

		//アニメーション用
		float tw = 1.0f / TEXTURE_PATTERN_DIVIDE_X;	// テクスチャの幅
		float th = 1.0f / TEXTURE_PATTERN_DIVIDE_Y;	// テクスチャの高さ
		float tx = (float)(Epattern % TEXTURE_PATTERN_DIVIDE_X) * tw;	// テクスチャの左上X座標
		float ty = (float)(Epattern / TEXTURE_PATTERN_DIVIDE_X) * th;	// テクスチャの左上Y座標

		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSpriteLeftTop(g_VertexBuffer, g_Pos.x, g_Pos.y, CUR_WIDTH, CUR_HEIGHT, tx, ty, tw, th);

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);
	}

}

void ResetPause(void)
{
	if (g_ResetPause == TRUE)  return;
	g_Use = TRUE;
	g_w = TEXTURE_WIDTH;
	g_h = TEXTURE_HEIGHT;
	g_Pos = { g_w / 2, 50.0f, 0.0f };
	g_TexNo = 0;
	cursor = 0;

	g_ResetPause = TRUE;

}

