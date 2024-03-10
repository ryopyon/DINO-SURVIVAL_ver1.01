//=============================================================================
//
// チュートリアル画面処理 [Tutorial.cpp]
// Author : HAL東京 高橋諒
//
//=============================================================================
#include "result.h"
#include "input.h"
#include "score.h"
#include "fade.h"
#include "tutorial.h"
#include "sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_WIDTH				(SCREEN_WIDTH)	// 背景サイズ
#define TEXTURE_HEIGHT				(SCREEN_HEIGHT)	// 
#define TEXTURE_MAX					(4)				// テクスチャの数

#define TEXTURE_TRIGGER_HEIGHT		(50)			// トリガーの縦の大きさ

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;		// 頂点情報
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報

static char *g_TexturName[TEXTURE_MAX] = {
	"data/TEXTURE/tutorial/tutorial.png",
	"data/TEXTURE/fade_black.png",
	"data/TEXTURE/tutorial/sousa.png",
	"data/TEXTURE/tutorial/trigger.png",

};


static BOOL						g_Use;						// TRUE:使っている  FALSE:未使用
static float					g_w, g_h;					// 幅と高さ
static XMFLOAT3					g_Pos;						// ポリゴンの座標
static int						g_TexNo;					// テクスチャ番号

static float	alpha;
static BOOL	flag_alpha;

static BOOL						g_Load = FALSE;

// ワイプ
static BOOL						g_wipe = FALSE;
static float					g_wipeSize = 0.0f;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitTutorial(void)
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

	alpha = 1.0f;
	flag_alpha = TRUE;

	// ワイプを初期化
	SetWipe(WIPE_MODE_NONE, XMFLOAT4(1.0f, 1.0f, 0.0f, 0.0f), g_wipeSize);

	// BGM再生
	PlaySound(SOUND_LABEL_BGM_BGM2);

	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitTutorial(void)
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
void UpdateTutorial(void)
{
	if (flag_alpha == TRUE)
	{
		alpha -= 0.01f;
		if (alpha <= 0.5f)
		{
			alpha = 0.5f;
			flag_alpha = FALSE;
		}
	}
	else
	{
		alpha += 0.01f;
		if (alpha >= 1.0f)
		{
			alpha = 1.0f;
			flag_alpha = TRUE;
		}
	}


	if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(0, BUTTON_B))
	{// Enter、Bを押したら、ステージを切り替える
		PlaySound(SOUND_LABEL_SE_KETTEI);
		SetFade(FADE_OUT, MODE_GAME);
	}
	if (GetKeyboardTrigger(DIK_SPACE) || IsButtonTriggered(0, BUTTON_A))
	{// Spase、Aを押したら、ステージを切り替える
		PlaySound(SOUND_LABEL_SE_KETTEI);
		SetFade(FADE_OUT, MODE_TITLE);
	}


}

//=============================================================================
// 描画処理
//=============================================================================
void DrawTutorial(void)
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


	// 背景を描画
	{
		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[TUTORIAL_TITLE]);

		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSpriteLeftTop(g_VertexBuffer, 0.0f, 0.0f, g_w, g_h, 0.0f, 0.0f, 1.0f, 1.0f);

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);
	}

	// 半透明の黒い背景を描画
	{
		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[TUTORIAL_FADE]);

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

	// 操作を描画
	{
		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[TUTORIAL_SOUSA]);

		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSprite(g_VertexBuffer, SCREEN_CENTER_X, SCREEN_CENTER_Y, g_w * 0.8f, g_h * 0.8f, 0.0f, 0.0f, 1.0f, 1.0f);

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);
	}

	// 導入を描画
	{
		// テクスチャ設定
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[TUTORIAL_TRIGGER]);

		// １枚のポリゴンの頂点とテクスチャ座標を設定
		SetSpriteLTColor(g_VertexBuffer, 0.0f, SCREEN_HEIGHT - TEXTURE_TRIGGER_HEIGHT, SCREEN_WIDTH, TEXTURE_TRIGGER_HEIGHT, 0.0f, 0.0f, 1.0f, 1.0f,
			XMFLOAT4(1.0f, 1.0f, 1.0f, alpha));

		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);
	}

}




