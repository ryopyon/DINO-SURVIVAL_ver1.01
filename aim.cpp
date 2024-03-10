//=============================================================================
//
// aim処理 [aim.cpp]
// Author : HAL東京 高橋諒
//
//=============================================================================
#include "main.h"

#include "renderer.h"
#include "debugproc.h"
#include "player.h"
#include "aim.h"
#include "flag.h"
#include "sprite.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_MAX					(1)		// テクスチャの数

#define AIM_POS_X_RED				(540.0f)	// 赤のエイムカーソルの位置
#define AIM_POS_Y_RED				(250.0f)

#define AIM_POS_X_BLUE				(620.0f)	// 赤のエイムカーソルの位置
#define AIM_POS_Y_BLUE				(230.0f)

#define AIM_POS_X_GREEN				(510.0f)	// 赤のエイムカーソルの位置
#define AIM_POS_Y_GREEN				(223.0f)

#define AIM_POS_X_YELLOW			(590.0f)	// 赤のエイムカーソルの位置
#define AIM_POS_Y_YELLOW			(207.0f)

#define AIM_SIZE					(50.0f)		// エイムカーソルの大きさ

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer* g_VertexBuffer = NULL;		// 頂点情報
static ID3D11ShaderResourceView* g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報

static char* g_TexturName[TEXTURE_MAX] = {

	"data/TEXTURE/game/Aim01.png",
};


static BOOL						g_Use;						// TRUE:使っている  FALSE:未使用
static float					g_w, g_h;					// 幅と高さ
static XMFLOAT3					g_Pos;						// ポリゴンの座標
static int						g_TexNo;					// テクスチャ番号

static XMFLOAT4 color = { 0.0f, 0.0f, 0.0f, 1.0f };


static BOOL						g_Load = FALSE;


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitAim(void)
{
	ID3D11Device* pDevice = GetDevice();

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

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitAim(void)
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
void UpdateAim(void)
{
	PLAYER* player = GetPlayer();

	// 色とポジションを変える
	switch (player->type)
	{
	case PLAYER_RED:
		g_Pos.x = AIM_POS_X_RED;
		g_Pos.y = AIM_POS_Y_RED;
		color = { 1.0f, 0.0f, 0.0f, 1.0f };

		break;

	case PLAYER_BLUE:
		g_Pos.x = AIM_POS_X_BLUE;
		g_Pos.y = AIM_POS_Y_BLUE;
		color = { 0.0f, 0.0f, 1.0f, 1.0f };

		break;

	case PLAYER_GREEN:
		g_Pos.x = AIM_POS_X_GREEN;
		g_Pos.y = AIM_POS_Y_GREEN;
		color = { 0.0f, 1.0f, 0.0f, 1.0f };

		break;

	case PLAYER_YELLOW:
		g_Pos.x = AIM_POS_X_YELLOW;
		g_Pos.y = AIM_POS_Y_YELLOW;
		color = { 1.0f, 1.0f, 0.0f, 1.0f };

		break;
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawAim(void)
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

	// テクスチャ設定
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_TexNo]);

	// １枚のポリゴンの頂点とテクスチャ座標を設定
	SetSpriteColor(g_VertexBuffer, g_Pos.x, g_Pos.y, AIM_SIZE, AIM_SIZE, 0.0f, 0.0f, 1.0f, 1.0f,
						color);

	if (GetFlag() == FLAG_GAME_START || GetFlag() == FLAG_GAME_BATTLE)
	{
		// ポリゴン描画
		GetDeviceContext()->Draw(4, 0);
	}
}