//=============================================================================
//
// UI処理 [ui.cpp]
// Author : HAL東京 高橋諒
//
//=============================================================================
#include "ui.h"
#include "input.h"
#include "player.h"
#include "flag.h"
#include "score.h"
#include "fade.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_WIDTH					(SCREEN_WIDTH)	// 背景サイズ
#define TEXTURE_HEIGHT					(SCREEN_HEIGHT)
#define TEXTURE_MAX						(9)	// テクスチャの数


// HPとMPのロゴ
#define TEXTURE_HPMP_ROGO_X				(TEXTURE_PLAYER_ICON_WIDTH + 54)	// HPとMPロゴの位置
#define TEXTURE_HPMP_ROGO_Y				(44)

#define TEXTURE_HPMP_ROGO_WIDTH			(64)	// HPとMPロゴのサイズ
#define TEXTURE_HPMP_ROGO_HEIGHT		(64)


// HPのカラゲージ
#define TEXTURE_KARAGAGE_HP_X			(TEXTURE_PLAYER_ICON_WIDTH + TEXTURE_HPMP_ROGO_WIDTH + 50)	// HPのカラゲージの位置
#define TEXTURE_KARAGAGE_HP_Y			(20)

#define TEXTURE_KARAGAGE_HP_WIDTH		(300)	// HPのカラゲージのサイズ
#define TEXTURE_KARAGAGE_HP_HEIGHT		(16)

// HPのゲージ
#define TEXTURE_GAGE_HP_X				(TEXTURE_PLAYER_ICON_WIDTH + TEXTURE_HPMP_ROGO_WIDTH + 50)	// HPのゲージの位置
#define TEXTURE_GAGE_HP_Y				(20)

#define TEXTURE_GAGE_HP_WIDTH			(300)	// HPのゲージのサイズ
#define TEXTURE_GAGE_HP_HEIGHT			(16)



// MPのカラゲージ
#define TEXTURE_KARAGAGE_MP_X			(TEXTURE_PLAYER_ICON_WIDTH + TEXTURE_HPMP_ROGO_WIDTH + 50)	// MPのカラゲージの位置
#define TEXTURE_KARAGAGE_MP_Y			(48)

#define TEXTURE_KARAGAGE_MP_WIDTH		(300)	// MPのカラゲージのサイズ
#define TEXTURE_KARAGAGE_MP_HEIGHT		(16)

// MPのゲージ
#define TEXTURE_GAGE_MP_X				(TEXTURE_PLAYER_ICON_WIDTH + TEXTURE_HPMP_ROGO_WIDTH + 50)	// MPのゲージの位置
#define TEXTURE_GAGE_MP_Y				(48)						

#define TEXTURE_GAGE_MP_WIDTH			(300)	// MPのゲージのサイズ
#define TEXTURE_GAGE_MP_HEIGHT			(16)

// カラチャージゲージのゲージ
#define TEXTURE_KARAGAGE_CHARGE_X		(TEXTURE_PLAYER_ICON_WIDTH + TEXTURE_HPMP_ROGO_WIDTH + 50)	// チャージゲージのゲージの位置
#define TEXTURE_KARAGAGE_CHARGE_Y		(62)						

#define TEXTURE_KARAGAGE_CHARGE_WIDTH	(150)	// チャージゲージのゲージのサイズ
#define TEXTURE_KARAGAGE_CHARGE_HEIGHT	(8)

// チャージゲージのゲージ
#define TEXTURE_GAGE_CHARGE_X			(TEXTURE_PLAYER_ICON_WIDTH + TEXTURE_HPMP_ROGO_WIDTH + 50)	// チャージゲージのゲージの位置
#define TEXTURE_GAGE_CHARGE_Y			(62)						

#define TEXTURE_GAGE_CHARGE_WIDTH		(150)	// チャージゲージのゲージのサイズ
#define TEXTURE_GAGE_CHARGE_HEIGHT		(8)


// スキップ
#define TEXTURE_SKIP_X					(SCREEN_WIDTH - TEXTURE_SKIP_WIDTH - 20.0f)	// スキップの位置
#define TEXTURE_SKIP_Y					(SCREEN_HEIGHT - TEXTURE_SKIP_HEIGHT - 10.0f)

#define TEXTURE_SKIP_WIDTH				(150)	// スキップの位置
#define TEXTURE_SKIP_HEIGHT				(40)

// 火を攻撃
#define TEXTURE_HIWO_X					(SCREEN_CENTER_X)	// 火を攻撃の位置
#define TEXTURE_HIWO_Y					(SCREEN_HEIGHT - TEXTURE_HIWO_HEIGHT)						

#define TEXTURE_HIWO_WIDTH				(300)	// 火を攻撃のサイズ
#define TEXTURE_HIWO_HEIGHT				(40)


// プレイヤーアイコン
#define TEXTURE_ICON_X					(8)											// アイコンの位置
#define TEXTURE_ICON_Y					(8)

#define TEXTURE_ICON_RED_X				(SCREEN_WIDTH - TEXTURE_ICON_WIDTH)			// 赤アイコンの位置
#define TEXTURE_ICON_RED_Y				(90)

#define TEXTURE_ICON_BLUE_X				(SCREEN_WIDTH - TEXTURE_ICON_WIDTH)			// 青アイコンの位置
#define TEXTURE_ICON_BLUE_Y				(170)

#define TEXTURE_ICON_GREEN_X			(SCREEN_WIDTH - TEXTURE_ICON_WIDTH)			// 緑アイコンの位置
#define TEXTURE_ICON_GREEN_Y			(250)

#define TEXTURE_ICON_YELLOW_X			(SCREEN_WIDTH - TEXTURE_ICON_WIDTH)			// 黄アイコンの位置
#define TEXTURE_ICON_YELLOW_Y			(330)


#define TEXTURE_ICON_WIDTH				(60)	// プレイヤーアイコンのサイズ
#define TEXTURE_ICON_HEIGHT				(60)

#define TEXTURE_PLAYER_ICON_WIDTH		(140)	// 左上のプレイヤーアイコンのサイズ
#define TEXTURE_PLAYER_ICON_HEIGHT		(140)

#define TEXTURE_ICON_DISTANCE			(20)

enum
{
	UI_GAGE_KARA,
	UI_HP_MP,
	UI_SKIP,
	UI_HIWO,
	UI_PLAYER_ICON_RED,
	UI_PLAYER_ICON_BLUE,
	UI_PLAYER_ICON_GREEN,
	UI_PLAYER_ICON_YELLOW,
	UI_PLAYER_ICON,

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
	"data/TEXTURE/game/gage_kara.jpg",
	"data/TEXTURE/game/hp_mp.jpg",
	"data/TEXTURE/game/skip.png",
	"data/TEXTURE/game/hiwo.png",
	"data/TEXTURE/cursor/Red.png",
	"data/TEXTURE/cursor/Blue.png",
	"data/TEXTURE/cursor/Green.png",
	"data/TEXTURE/cursor/Yellow.png",
	"data/TEXTURE/cursor/playericon.png",
};


static BOOL						g_Use;						// TRUE:使っている  FALSE:未使用
static float					g_w, g_h;					// 幅と高さ
static XMFLOAT3					g_Pos;						// ポリゴンの座標
static int						g_TexNo;					// テクスチャ番号

static float					alpha;
static BOOL						flag_alpha;


static BOOL						g_Load = FALSE;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitUi(void)
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


	alpha = 1.0f;
	flag_alpha = TRUE;


	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitUi(void)
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
void UpdateUi(void)
{
	if (flag_alpha == TRUE)
	{
		alpha -= 0.02f;
		if (alpha <= 0.0f)
		{
			alpha = 0.0f;
			flag_alpha = FALSE;
		}
	}
	else
	{
		alpha += 0.02f;
		if (alpha >= 1.0f)
		{
			alpha = 1.0f;
			flag_alpha = TRUE;
		}
	}


#ifdef _DEBUG	// デバッグ情報を表示する
	
#endif

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawUi(void)
{
	PLAYER* player = GetPlayer();

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

	// イベント中は描画しない
	if (GetFlag() == FLAG_GAME_START || GetFlag() == FLAG_GAME_BATTLE)
	{
		// HPとMPのロゴを描画
		{
			// テクスチャ設定
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[UI_HP_MP]);


			//表示位置
			float pw = TEXTURE_HPMP_ROGO_WIDTH;			// HPとMPのロゴの表示幅
			float ph = TEXTURE_HPMP_ROGO_HEIGHT;		// HPとMPのロゴの表示高さ
			float px = TEXTURE_HPMP_ROGO_X;				// HPとMPのロゴの表示位置X	
			float py = TEXTURE_HPMP_ROGO_Y;				// HPとMPのロゴの表示位置Y

			float tw = 1.0f;							// テクスチャのロゴの幅
			float th = 1.0f;							// テクスチャのロゴの高さ
			float tx = 0.0f;							// テクスチャのロゴの左上X座標
			float ty = 0.0f;							// テクスチャのロゴの左上Y座標

			// １枚のポリゴンの頂点とテクスチャ座標を設定
			SetSpriteColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

			// ポリゴン描画
			GetDeviceContext()->Draw(4, 0);
		}

		// HPのカラのゲージを描画
		{
			// テクスチャ設定
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[UI_GAGE_KARA]);


			//表示位置
			float pw = TEXTURE_KARAGAGE_HP_WIDTH;			// HPのカラのゲージの表示幅
			float ph = TEXTURE_KARAGAGE_HP_HEIGHT;			// HPのカラのゲージの表示高さ
			float px = TEXTURE_KARAGAGE_HP_X;				// HPのカラのゲージの表示位置X	
			float py = TEXTURE_KARAGAGE_HP_Y;				// HPのカラのゲージの表示位置Y

			float tw = 1.0f;								// テクスチャの幅
			float th = 1.0f;								// テクスチャの高さ
			float tx = 0.0f;								// テクスチャの左上X座標
			float ty = 0.0f;								// テクスチャの左上Y座標

			// １枚のポリゴンの頂点とテクスチャ座標を設定
			SetSpriteLeftTop(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th);

			// ポリゴン描画
			GetDeviceContext()->Draw(4, 0);
		}

		// カラのチャージゲージを描画
		{
			// テクスチャ設定
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[UI_GAGE_KARA]);


			//表示位置
			float pw = TEXTURE_KARAGAGE_CHARGE_WIDTH;		// カラのチャージゲージの表示幅
			float ph = TEXTURE_KARAGAGE_CHARGE_HEIGHT;		// カラのチャージゲージの表示高さ
			float px = TEXTURE_KARAGAGE_CHARGE_X;			// カラのチャージゲージの表示位置X	
			float py = TEXTURE_KARAGAGE_CHARGE_Y;			// カラのチャージゲージの表示位置Y

			float tw = 1.0f;								// テクスチャの幅
			float th = 1.0f;								// テクスチャの高さ
			float tx = 0.0f;								// テクスチャの左上X座標
			float ty = 0.0f;								// テクスチャの左上Y座標

			// １枚のポリゴンの頂点とテクスチャ座標を設定
			SetSpriteLeftTop(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th);

			// ポリゴン描画
			GetDeviceContext()->Draw(4, 0);
		}

		// MPのカラのゲージを描画
		{
			// テクスチャ設定
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[UI_GAGE_KARA]);


			//表示位置
			float pw = TEXTURE_KARAGAGE_MP_WIDTH;			// MPのカラのゲージの表示幅
			float ph = TEXTURE_KARAGAGE_MP_HEIGHT;			// MPのカラのゲージの表示高さ
			float px = TEXTURE_KARAGAGE_MP_X;				// MPのカラのゲージの表示位置X	
			float py = TEXTURE_KARAGAGE_MP_Y;				// MPのカラのゲージの表示位置Y

			float tw = 1.0f;								// テクスチャの幅
			float th = 1.0f;								// テクスチャの高さ
			float tx = 0.0f;								// テクスチャの左上X座標
			float ty = 0.0f;								// テクスチャの左上Y座標

			// １枚のポリゴンの頂点とテクスチャ座標を設定
			SetSpriteLeftTop(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th);

			// ポリゴン描画
			GetDeviceContext()->Draw(4, 0);
		}

		// HPのゲージを描画
		{
			// テクスチャ設定
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[UI_GAGE_KARA]);

			float playerHpRatio;

			playerHpRatio = player->hp / (float)PLAYER_HP_MAX; // （現在のプレイヤーのHP）/（プレイヤーの最大HP）
					//表示位置
			float px = TEXTURE_GAGE_HP_X;				// HPのゲージの表示位置X	
			float py = TEXTURE_GAGE_HP_Y;				// HPのゲージの表示位置Y
			float pw = TEXTURE_GAGE_HP_WIDTH * playerHpRatio;			// HPのゲージの表示幅
			float ph = TEXTURE_GAGE_HP_HEIGHT;			// HPのゲージの表示高さ

			float tx = 0.0f;							// テクスチャの左上X座標
			float ty = 0.0f;							// テクスチャの左上Y座標
			float tw = 1.0f * playerHpRatio;							// テクスチャの幅
			float th = 1.0f;							// テクスチャの高さ

			SetSpriteLTColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th, XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f));

			GetDeviceContext()->Draw(4, 0);
		}

		// MPのゲージを描画
		{
			// テクスチャ設定
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[UI_GAGE_KARA]);

			float playerMpRatio;

			playerMpRatio = player->mp / (float)PLAYER_MP_MAX; // （現在のプレイヤーのHP）/（プレイヤーの最大HP）
					//表示位置
			float px = TEXTURE_GAGE_MP_X;				// HPのゲージの表示位置X	
			float py = TEXTURE_GAGE_MP_Y;				// HPのゲージの表示位置Y
			float pw = TEXTURE_GAGE_MP_WIDTH * playerMpRatio;			// HPのゲージの表示幅
			float ph = TEXTURE_GAGE_MP_HEIGHT;			// HPのゲージの表示高さ

			float tx = 0.0f;							// テクスチャの左上X座標
			float ty = 0.0f;							// テクスチャの左上Y座標
			float tw = 1.0f * playerMpRatio;							// テクスチャの幅
			float th = 1.0f;							// テクスチャの高さ

			SetSpriteLTColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th, XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f));

			GetDeviceContext()->Draw(4, 0);
		}

		// チャージゲージを描画
		{
			// テクスチャ設定
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[UI_GAGE_KARA]);

			float playerchargeRatio;

			playerchargeRatio = player->chargeCnt / (float)CHARGE_CNT_MAX; // （現在のプレイヤーのHP）/（プレイヤーの最大HP）
					//表示位置
			float px = TEXTURE_GAGE_CHARGE_X;				// HPのゲージの表示位置X	
			float py = TEXTURE_GAGE_CHARGE_Y;				// HPのゲージの表示位置Y
			float pw = TEXTURE_GAGE_CHARGE_WIDTH * playerchargeRatio;			// HPのゲージの表示幅
			float ph = TEXTURE_GAGE_CHARGE_HEIGHT;			// HPのゲージの表示高さ

			float tx = 0.0f;							// テクスチャの左上X座標
			float ty = 0.0f;							// テクスチャの左上Y座標
			float tw = 1.0f * playerchargeRatio;							// テクスチャの幅
			float th = 1.0f;							// テクスチャの高さ

			SetSpriteLTColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th, XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f));

			GetDeviceContext()->Draw(4, 0);
		}


		// アイコン赤を描画
		{
			// テクスチャ設定
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[UI_PLAYER_ICON_RED]);

			if (player->type != PLAYER_RED)
			{
				// １枚のポリゴンの頂点とテクスチャ座標を設定
				SetSpriteColor(g_VertexBuffer, TEXTURE_ICON_RED_X, TEXTURE_ICON_RED_Y,
					TEXTURE_ICON_WIDTH, TEXTURE_ICON_HEIGHT, 0.0f, 0.0f, 1.0f, 1.0f,
					XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0));

			}
			else
			{
				SetSpriteColor(g_VertexBuffer, TEXTURE_ICON_RED_X, TEXTURE_ICON_RED_Y,
					TEXTURE_ICON_WIDTH * 0.7, TEXTURE_ICON_HEIGHT * 0.7, 0.0f, 0.0f, 1.0f, 1.0f,
					XMFLOAT4(0.7f, 0.7f, 0.7f, 0.7f));

			}
			// ポリゴン描画
			GetDeviceContext()->Draw(4, 0);

		}

		// アイコン青を描画
		{
			// テクスチャ設定
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[UI_PLAYER_ICON_BLUE]);

			if (player->type != PLAYER_BLUE)
			{
				// １枚のポリゴンの頂点とテクスチャ座標を設定
				SetSpriteColor(g_VertexBuffer, TEXTURE_ICON_BLUE_X, TEXTURE_ICON_BLUE_Y,
					TEXTURE_ICON_WIDTH, TEXTURE_ICON_HEIGHT, 0.0f, 0.0f, 1.0f, 1.0f,
					XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0));

			}
			else
			{
				SetSpriteColor(g_VertexBuffer, TEXTURE_ICON_BLUE_X, TEXTURE_ICON_BLUE_Y,
					TEXTURE_ICON_WIDTH * 0.7, TEXTURE_ICON_HEIGHT * 0.7, 0.0f, 0.0f, 1.0f, 1.0f,
					XMFLOAT4(0.7f, 0.7f, 0.7f, 0.7f));

			}
			// ポリゴン描画
			GetDeviceContext()->Draw(4, 0);

		}

		// アイコン緑を描画
		{
			// テクスチャ設定
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[UI_PLAYER_ICON_GREEN]);

			if (player->type != PLAYER_GREEN)
			{
				// １枚のポリゴンの頂点とテクスチャ座標を設定
				SetSpriteColor(g_VertexBuffer, TEXTURE_ICON_GREEN_X, TEXTURE_ICON_GREEN_Y,
					TEXTURE_ICON_WIDTH, TEXTURE_ICON_HEIGHT, 0.0f, 0.0f, 1.0f, 1.0f,
					XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0));

			}
			else
			{
				SetSpriteColor(g_VertexBuffer, TEXTURE_ICON_GREEN_X, TEXTURE_ICON_GREEN_Y,
					TEXTURE_ICON_WIDTH * 0.7, TEXTURE_ICON_HEIGHT * 0.7, 0.0f, 0.0f, 1.0f, 1.0f,
					XMFLOAT4(0.7f, 0.7f, 0.7f, 0.7f));

			}
			// ポリゴン描画
			GetDeviceContext()->Draw(4, 0);

		}

		// アイコン黄を描画
		{
			// テクスチャ設定
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[UI_PLAYER_ICON_YELLOW]);

			if (player->type != PLAYER_YELLOW)
			{
				// １枚のポリゴンの頂点とテクスチャ座標を設定
				SetSpriteColor(g_VertexBuffer, TEXTURE_ICON_YELLOW_X, TEXTURE_ICON_YELLOW_Y,
					TEXTURE_ICON_WIDTH, TEXTURE_ICON_HEIGHT, 0.0f, 0.0f, 1.0f, 1.0f,
					XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0));

			}
			else
			{
				SetSpriteColor(g_VertexBuffer, TEXTURE_ICON_YELLOW_X, TEXTURE_ICON_YELLOW_Y,
					TEXTURE_ICON_WIDTH * 0.7, TEXTURE_ICON_HEIGHT * 0.7, 0.0f, 0.0f, 1.0f, 1.0f,
					XMFLOAT4(0.7f, 0.7f, 0.7f, 0.7f));

			}
			// ポリゴン描画
			GetDeviceContext()->Draw(4, 0);

		}

		// アイコンを描画
		{
			// テクスチャ設定
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[UI_PLAYER_ICON]);

			switch (player->type)
			{
			case PLAYER_RED:
					// １枚のポリゴンの頂点とテクスチャ座標を設定
					SetSpriteLTColor(g_VertexBuffer, TEXTURE_ICON_X, TEXTURE_ICON_Y,
						TEXTURE_PLAYER_ICON_WIDTH, TEXTURE_PLAYER_ICON_HEIGHT, 0.0f, 0.0f, 0.25f, 1.0f,
						XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0));
				break;

			case PLAYER_BLUE:
					// １枚のポリゴンの頂点とテクスチャ座標を設定
					SetSpriteLTColor(g_VertexBuffer, TEXTURE_ICON_X, TEXTURE_ICON_Y,
						TEXTURE_PLAYER_ICON_WIDTH, TEXTURE_PLAYER_ICON_HEIGHT, 0.25f, 0.0f, 0.25f, 1.0f,
						XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0));
				break;

			case PLAYER_GREEN:
					// １枚のポリゴンの頂点とテクスチャ座標を設定
					SetSpriteLTColor(g_VertexBuffer, TEXTURE_ICON_X, TEXTURE_ICON_Y,
						TEXTURE_PLAYER_ICON_WIDTH, TEXTURE_PLAYER_ICON_HEIGHT, 0.5f, 0.0f, 0.25f, 1.0f,
						XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0));
				break;

			case PLAYER_YELLOW:
					// １枚のポリゴンの頂点とテクスチャ座標を設定
					SetSpriteLTColor(g_VertexBuffer, TEXTURE_ICON_X, TEXTURE_ICON_Y,
						TEXTURE_PLAYER_ICON_WIDTH, TEXTURE_PLAYER_ICON_HEIGHT, 0.75f, 0.0f, 0.25f, 1.0f,
						XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0));
				break;

			}

			// ポリゴン描画
			GetDeviceContext()->Draw(4, 0);

		}

	}


	// 火の移動イベント中に描画
	if (GetFlag() == FLAG_GAME_EVENT_FIRE)
	{

		// スキップを描画
		{
			// テクスチャ設定
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[UI_SKIP]);

			// １枚のポリゴンの頂点とテクスチャ座標を設定
			SetSpriteLTColor(g_VertexBuffer, TEXTURE_SKIP_X, TEXTURE_SKIP_Y,
				TEXTURE_SKIP_WIDTH, TEXTURE_SKIP_HEIGHT, 0.0f, 0.0f, 1.0f, 1.0f,
				XMFLOAT4(1.0f, 1.0f, 1.0f, alpha));

			// ポリゴン描画
			GetDeviceContext()->Draw(4, 0);
		}
	}

	// イベントに入るまで描画
	if (GetFlag() == FLAG_GAME_START)
	{
		// 火を攻撃を描画
		{
			// テクスチャ設定
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[UI_HIWO]);

			// １枚のポリゴンの頂点とテクスチャ座標を設定
			SetSpriteColor(g_VertexBuffer, TEXTURE_HIWO_X, TEXTURE_HIWO_Y,
				TEXTURE_HIWO_WIDTH, TEXTURE_HIWO_HEIGHT, 0.0f, 0.0f, 1.0f, 1.0f,
				XMFLOAT4(1.0f, 1.0f, 1.0f, alpha));

			// ポリゴン描画
			GetDeviceContext()->Draw(4, 0);
		}

	}

}