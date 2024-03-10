//=============================================================================
//
// リザルト画面処理 [result.cpp]
// Author : HAL東京 高橋諒
//
//=============================================================================
#include "result.h"
#include "input.h"
#include "score.h"
#include "fade.h"
#include "sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_WIDTH				(SCREEN_WIDTH)	// 背景サイズ
#define TEXTURE_HEIGHT				(SCREEN_HEIGHT)	// 
#define TEXTURE_MAX					(5)				// テクスチャの数


#define CUR_WIDTH					(50)			//カーソルサイズ
#define CUR_HEIGHT					(50)


#define RESULT_CUR1_X				(700.0f)		//	カーソルの位置
#define RESULT_CUR1_Y				(340.0f)

#define RESULT_CUR2_X				(850.0f)
#define RESULT_CUR2_Y				(390.0f)

#define RESULT_CUR3_X				(700.0f)
#define RESULT_CUR3_Y				(440.0f)


// アニメーション用
#define ANIM_WAIT_TIME				(90)
#define ANIM_FADE_TIME				(90)
#define ANIM_TOTAL_TIME				(ANIM_WAIT_TIME + ANIM_FADE_TIME)

#define ENEMY_WAIT					(4)

#define TEXTURE_PATTERN_DIVIDE_X	(7)		// アニメパターンのテクスチャ内分割数（X)
#define TEXTURE_PATTERN_DIVIDE_Y	(1)		// アニメパターンのテクスチャ内分割数（Y)
#define ANIM_PATTERN_NUM			(TEXTURE_PATTERN_DIVIDE_X*TEXTURE_PATTERN_DIVIDE_Y)	// アニメーションパターン数

enum
{
	RESULT_RESULT,
	RESULT_NUMBER,
	RESULT_EGREEN,
	RESULT_EBLUE,
	RESULT_ERED,

	RESULT_COLOR_MAX,
};

enum
{
	RESULT_TITLE,
	RESULT_RETRY,
	RESULT_EXIT,

	RESULT_MAX,
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;		// 頂点情報
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報

static char *g_TexturName[TEXTURE_MAX] = {
	"data/TEXTURE/result/Result01.png",
	"data/TEXTURE/number16x32.png",
	"data/TEXTURE/cursor/Egreen.png",
	"data/TEXTURE/cursor/Eblue.png",
	"data/TEXTURE/cursor/Ered.png",
};


static BOOL						g_Use;						// TRUE:使っている  FALSE:未使用
static float					g_w, g_h;					// 幅と高さ
static XMFLOAT3					g_Pos;						// ポリゴンの座標
static int						g_TexNo;					// テクスチャ番号

static float EanimCnt;
static int Epattern;

static int cursor;

// ワイプ
static BOOL						g_wipe = FALSE;
static float					g_wipeSize = 0.0f;

static BOOL						g_Load = FALSE;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitResult(void)
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
	g_Use   = TRUE;
	g_w     = TEXTURE_WIDTH;
	g_h     = TEXTURE_HEIGHT;
	g_Pos   = { g_w / 2, 50.0f, 0.0f };
	g_TexNo = 0;


	// モノクロを直す
	SetMonochrome(FALSE);

	// ワイプを初期化
	SetWipe(WIPE_MODE_NONE, XMFLOAT4(1.0f, 1.0f, 0.0f, 0.0f), g_wipeSize);

	// BGM再生
	PlaySound(SOUND_LABEL_BGM_RESULT);

	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitResult(void)
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
void UpdateResult(void)
{
	// アニメーション  
	EanimCnt += 1.0f;
	if (EanimCnt > ENEMY_WAIT)
	{
		EanimCnt = 0.0f;
		// パターンの切り替え
		Epattern = (Epattern + 1) % ANIM_PATTERN_NUM;
	}



	if (GetKeyboardTrigger(DIK_DOWN) || GetKeyboardTrigger(DIK_S) || IsButtonTriggered(0, STICK_L_DOWN) || IsButtonTriggered(0, BUTTON_DOWN))
	{
		PlaySound(SOUND_LABEL_SE_CRICK);
		cursor = (++cursor + RESULT_MAX) % RESULT_MAX;
	}
	if (GetKeyboardTrigger(DIK_UP) || GetKeyboardTrigger(DIK_W) || IsButtonTriggered(0, STICK_L_UP) || IsButtonTriggered(0, BUTTON_UP))
	{
		PlaySound(SOUND_LABEL_SE_CRICK);
		cursor = (--cursor + RESULT_MAX) % RESULT_MAX;
	}


	switch (cursor)		//カーソルの表記
	{
	case RESULT_TITLE:
		g_TexNo = RESULT_EBLUE;
		g_Pos = XMFLOAT3(RESULT_CUR1_X, RESULT_CUR1_Y, 0.0f);
		if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(0, BUTTON_START) || IsButtonTriggered(0, BUTTON_B))
		{// ステージ切り替え
			PlaySound(SOUND_LABEL_SE_KETTEI);
			SetFade(FADE_OUT, MODE_TITLE);
		}
		break;

	case RESULT_RETRY:
		g_TexNo = RESULT_EGREEN;
		g_Pos = XMFLOAT3(RESULT_CUR2_X, RESULT_CUR2_Y, 0.0f);

		if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(0, BUTTON_START) || IsButtonTriggered(0, BUTTON_B))
		{// ステージ切り替え
			PlaySound(SOUND_LABEL_SE_KETTEI);
			SetFade(FADE_OUT, MODE_GAME);
		}
		break;

	case RESULT_EXIT:
		g_TexNo = RESULT_ERED;
		g_Pos = XMFLOAT3(RESULT_CUR3_X, RESULT_CUR3_Y, 0.0f);

		if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(0, BUTTON_START) || IsButtonTriggered(0, BUTTON_B))
		{
			GameLoop();
		}
		break;
	}



#ifdef _DEBUG	// デバッグ情報を表示する
	
#endif

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawResult(void)
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

	// リザルトの背景を描画
	{
		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[RESULT_RESULT]);

		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSpriteLeftTop(g_VertexBuffer, 0.0f, 0.0f, g_w, g_h, 0.0f, 0.0f, 1.0f, 1.0f);

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);
	}


	// スコア表示
	{
		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[RESULT_NUMBER]);

		// 桁数分処理する
		int number = GetScore();
		for (int i = 0; i < SCORE_DIGIT; i++)
		{
			// 今回表示する桁の数字
			float x = (float)(number % 10);

			// スコアの位置やテクスチャー座標を反映
			float pw = 16*4;			// スコアの表示幅
			float ph = 32*4;			// スコアの表示高さ
			float px = 610.0f - i*pw;	// スコアの表示位置X
			float py = 300.0f;			// スコアの表示位置Y

			float tw = 1.0f / 10;		// テクスチャの幅
			float th = 1.0f / 1;		// テクスチャの高さ
			float tx = x * tw;			// テクスチャの左上X座標
			float ty = 0.0f;			// テクスチャの左上Y座標

			// １枚のポリゴンの頂点とテクスチャ座標を設定
			SetSpriteColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

			// ポリゴン描画
			GetDeviceContext()->Draw(4, 0);

			// 次の桁へ
			number /= 10;
		}

	}

	// カーソルを描画
	{
		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_TexNo]);

		//アニメーション用
		float tw = 1.0f / TEXTURE_PATTERN_DIVIDE_X;	// テクスチャの幅
		float th = 1.0f / TEXTURE_PATTERN_DIVIDE_Y;	// テクスチャの高さ
		float tx = (float)(Epattern % TEXTURE_PATTERN_DIVIDE_X) * tw;	// テクスチャの左上X座標
		float ty = (float)(Epattern / TEXTURE_PATTERN_DIVIDE_X) * th;	// テクスチャの左上Y座標

		if (cursor % 2 == 1)//右移動のエネミーだけ反対向きにする
		{
			tw *= -1;
			tx += 1.0f / TEXTURE_PATTERN_DIVIDE_X;
		}


		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSpriteLeftTop(g_VertexBuffer, g_Pos.x, g_Pos.y, CUR_WIDTH, CUR_HEIGHT, tx, ty, tw, th);

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);
	}
}




