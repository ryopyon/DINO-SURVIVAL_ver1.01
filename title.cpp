//=============================================================================
//
// タイトル画面処理 [title.cpp]
// Author : HAL東京 高橋諒
//
//=============================================================================
#include "title.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "renderer.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_WIDTH					(SCREEN_WIDTH)			// 背景サイズ
#define TEXTURE_HEIGHT					(SCREEN_HEIGHT)
#define TEXTURE_MAX						(9)						// テクスチャの数

// タイトルロゴ
#define TEXTURE_TITLELOGO_WIDTH			(600)					// タイトルロゴサイズ
#define TEXTURE_TITLELOGO_HEIGHT		(300)

#define TEXTURE_TITLELOGO_X				(SCREEN_CENTER_X)	// タイトルロゴの位置
#define TEXTURE_TITLELOGO_Y				(150)

// カーソル
#define CUR_WIDTH						(50)					//	カーソルサイズ
#define CUR_HEIGHT						(50)

#define TITLE_CUR1_X					(370.0f)				//	カーソルの位置
#define TITLE_CUR1_Y					(275.0f)

#define TITLE_CUR2_X					(548.0f)
#define TITLE_CUR2_Y					(323.0f)

#define TITLE_CUR3_X					(370.0f)
#define TITLE_CUR3_Y					(370.0f)

#define TITLE_CUR4_X					(548.0f)
#define TITLE_CUR4_Y					(417.0f)


//フェードのアニメーション用(LOGO)
#define ANIM_LOGO_WAIT_TIME				(90)	//黒 → 会社ロゴ
#define ANIM_LOGO_FADE_TIME				(90)	//会社ロゴ → 黒
#define ANIM_LOGO_TOTAL_TIME			(ANIM_LOGO_WAIT_TIME + ANIM_LOGO_FADE_TIME)

//フェードのアニメーション用(TITLE)
#define ANIM_TITLE_WAIT_TIME			(0)
#define ANIM_TITLE_FADE_TIME			(45)
#define ANIM_TITLE_TOTAL_TIME			(ANIM_TITLE_WAIT_TIME + ANIM_TITLE_FADE_TIME)

// ワイプ
#define WIPE_RATE						(30.0f)
#define WIPE_BORDER1					(500.0f)
#define WIPE_BORDER2					(1900.0f)

// エネミーのアニメーション用
#define ENEMY_WAIT						(4)

#define TEXTURE_PATTERN_DIVIDE_X		(7)		// アニメパターンのテクスチャ内分割数（X)
#define TEXTURE_PATTERN_DIVIDE_Y		(1)		// アニメパターンのテクスチャ内分割数（Y)
#define ANIM_PATTERN_NUM				(TEXTURE_PATTERN_DIVIDE_X*TEXTURE_PATTERN_DIVIDE_Y)	// アニメーションパターン数


enum
{
	ANIM_LOGO,	//= 0
	ANIM_TITLE,	
	TITLE_MENU,
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;				// 頂点情報
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報

static char *g_TexturName[TEXTURE_MAX] = {
	"data/TEXTURE/title/logo.png",
	"data/TEXTURE/title/title.png",
	"data/TEXTURE/title/titlelogo.png",
	"data/TEXTURE/effect000.jpg",
	"data/TEXTURE/cursor/Eyellow.png",
	"data/TEXTURE/cursor/Egreen.png",
	"data/TEXTURE/cursor/Eblue.png",
	"data/TEXTURE/cursor/Ered.png",
	"data/TEXTURE/fade_black.png",
};


static BOOL						g_Use;						// TRUE:使っている  FALSE:未使用
static float					g_w, g_h;					// 幅と高さ
static XMFLOAT3					g_Pos;						// ポリゴンの座標
static int						g_TexNo;					// テクスチャ番号

// ワイプ
static BOOL						g_wipe = FALSE;
static float					g_wipeSize = 0.0f;

static float					alpha;
static BOOL						flag_alpha;

static BOOL						g_Load = FALSE;

int								animCnt;
int								animMode;

float							EanimCnt;
int								Epattern;

static float					effect_dx;
static float					effect_dy;

int								cursor;
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitTitle(void)
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
	g_Pos   = XMFLOAT3(200.0f, 200.0f, 0.0f);
	g_TexNo = 0;

	alpha = 1.0f;
	flag_alpha = TRUE;

	effect_dx = 100.0f;
	effect_dy = 100.0f;

	cursor = 0;

	animCnt = 0;
	animMode = 0;

	EanimCnt = 0;
	Epattern = 0;

	// ワイプを初期化
	SetWipe(WIPE_MODE_NONE, XMFLOAT4(1.0f, 1.0f, 0.0f, 0.0f), g_wipeSize);

	// モノクロを直す
	SetMonochrome(FALSE);

	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitTitle(void)
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
void UpdateTitle(void)
{
	if (animMode == ANIM_LOGO)	//タイトルを描画してからタイトルにシーン遷移
	{
		if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(0, BUTTON_START) || IsButtonTriggered(0, BUTTON_B))
		{
			animMode = ANIM_TITLE;
			animCnt = 0;
		}

		animCnt++;

		if(animCnt == ANIM_LOGO_TOTAL_TIME)
		{
			animMode = ANIM_TITLE;
			animCnt = 0;
		}
	}

	else if (animMode == ANIM_TITLE)	//タイトルを描画してからタイトルにシーン遷移
	{
		animCnt++;
		if (animCnt == ANIM_TITLE_TOTAL_TIME)
		{
			animMode = TITLE_MENU;
			PlaySound(SOUND_LABEL_BGM_TITLE);
		}
	}



	else if (animMode == TITLE_MENU)
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
			cursor = (++cursor + TITLE_MAX) % TITLE_MAX;	//++cursorにした方が見やすいため
		}
		if (GetKeyboardTrigger(DIK_UP) || GetKeyboardTrigger(DIK_W) || IsButtonTriggered(0, STICK_L_UP) || IsButtonTriggered(0, BUTTON_UP))
		{
			PlaySound(SOUND_LABEL_SE_CRICK);
			cursor = (--cursor + TITLE_MAX) % TITLE_MAX;	//（-1 + 4）% 4
		}

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


		switch (cursor)		//カーソルの表記
		{
		case TUTORIAL:
			g_TexNo = TITLE_ERED;
			g_Pos = XMFLOAT3(TITLE_CUR1_X, TITLE_CUR1_Y, 0.0f);

			if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(0, BUTTON_START) || IsButtonTriggered(0, BUTTON_B))
			{// ステージ切り替え
				PlaySound(SOUND_LABEL_SE_KETTEI);
				g_wipe = TRUE;
			}
			break;

		case GAMESTART:
			g_TexNo = TITLE_EBLUE;
			g_Pos = XMFLOAT3(TITLE_CUR2_X, TITLE_CUR2_Y, 0.0f);

			if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(0, BUTTON_START) || IsButtonTriggered(0, BUTTON_B))
			{
				// ステージ切り替え
				PlaySound(SOUND_LABEL_SE_KETTEI);
				g_wipe = TRUE;
			}
			break;

		case CREDIT:
			g_TexNo = TITLE_EGREEN;
			g_Pos = XMFLOAT3(TITLE_CUR3_X, TITLE_CUR3_Y, 0.0f);

			if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(0, BUTTON_START) || IsButtonTriggered(0, BUTTON_B))
			{// ステージ切り替え
				PlaySound(SOUND_LABEL_SE_KETTEI);
				g_wipe = TRUE;
			}
			break;

		case EXIT:
			g_TexNo = TITLE_EYELLOW;
			g_Pos = XMFLOAT3(TITLE_CUR4_X, TITLE_CUR4_Y, 0.0f);

			if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(0, BUTTON_START) || IsButtonTriggered(0, BUTTON_B))
			{
				GameLoop();
			}
		}
	}

	// ワイプ処理
	if (g_wipe)
	{
		if (cursor != GAMESTART)
		{
			// WIPE_RATEとかの値はアニメーション見ながら調整してください (ワイプの種類によっていい感じになる値全然違うのでいろいろ試してみて)
			g_wipeSize += WIPE_RATE;					// 徐々にワイプの範囲を増やす
			SetWipe(WIPE_MODE_FLIP, XMFLOAT4(1.0f, 1.0f, 0.0f, 0.0f), g_wipeSize);

			if (g_wipeSize >= WIPE_BORDER1)
			{
				switch (cursor)
				{
				case TUTORIAL:
					SetFade(FADE_OUT, MODE_TUTORIAL);	// 途中からfadeを入れ始める
					break;

				case CREDIT:
					SetFade(FADE_OUT, MODE_CREDIT);		// 途中からfadeを入れ始める
					break;
				}
			}
			if (g_wipeSize >= WIPE_BORDER2)
			{
				g_wipeSize = 0.0f;
				SetWipe(WIPE_MODE_NONE, XMFLOAT4(1.0f, 1.0f, 0.0f, 0.0f), g_wipeSize);			// ワイプの終了
				g_wipe = FALSE;
			}
		}
		else
		{
			SetFade(FADE_OUT, MODE_GAME);
			g_wipe = FALSE;
		}

	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawTitle(void)
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

	switch (animMode)
	{
		case ANIM_LOGO:
		{

			// 会社ロゴの描画
			{
				GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[TITLE_LOGO]);

				// １枚のポリゴンの頂点とテクスチャ座標を設定
				SetSpriteLeftTop(g_VertexBuffer, 0.0f, 0.0f, TEXTURE_WIDTH, TEXTURE_HEIGHT, 0.0f, 0.0f, 1.0f, 1.0f);

				// ポリゴン描画
				GetDeviceContext()->Draw(4, 0);
			}

			// 会社ロゴのフェードを描画
			{
				GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[TITLE_FADE_BLACK]);

				// アルファ値
				XMFLOAT4 RGBA = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
				RGBA.w = (1.0f / ANIM_LOGO_FADE_TIME) * (animCnt - ANIM_LOGO_WAIT_TIME);

				// １枚のポリゴンの頂点とテクスチャ座標を設定
				SetSpriteColor(g_VertexBuffer, TEXTURE_WIDTH / 2, TEXTURE_HEIGHT / 2, TEXTURE_WIDTH, TEXTURE_HEIGHT,
					0.0f, 0.0f, 1.0f, 1.0f,
					RGBA);


				// ポリゴン描画
				GetDeviceContext()->Draw(4, 0);
			}

		}
			break;

		case ANIM_TITLE:
		{

			// タイトルの描画
			{
				GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[TITLE_TITLE]);

				// １枚のポリゴンの頂点とテクスチャ座標を設定
				SetSpriteLeftTop(g_VertexBuffer, 0.0f, 0.0f, TEXTURE_WIDTH, TEXTURE_HEIGHT, 0.0f, 0.0f, 1.0f, 1.0f);

				// ポリゴン描画
				GetDeviceContext()->Draw(4, 0);
			}

			// タイトルのフェードを描画
			{
				GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[TITLE_FADE_BLACK]);

				// アルファ値
				XMFLOAT4 RGBA = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
				RGBA.w = 1.0f - (1.0f / ANIM_TITLE_FADE_TIME) * (animCnt - ANIM_TITLE_WAIT_TIME);

				SetSpriteColor(g_VertexBuffer, TEXTURE_WIDTH / 2, TEXTURE_HEIGHT / 2, TEXTURE_WIDTH, TEXTURE_HEIGHT,
					0.0f, 0.0f, 1.0f, 1.0f,
					RGBA);


				// ポリゴン描画
				GetDeviceContext()->Draw(4, 0);
			}

		}
			break;

		case TITLE_MENU:

		// 背景を描画
		{
			// テクスチャ設定
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[TITLE_TITLE]);

			// １枚のポリゴンの頂点とテクスチャ座標を設定
			SetSpriteLeftTop(g_VertexBuffer, 0.0f, 0.0f, TEXTURE_WIDTH, TEXTURE_HEIGHT, 0.0f, 0.0f, 1.0f, 1.0f);

			// ポリゴン描画
			GetDeviceContext()->Draw(4, 0);
		}

		// タイトルロゴを描画
		{
			// テクスチャ設定
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[TITLE_TITLELOGO]);

			// １枚のポリゴンの頂点とテクスチャ座標を設定
			SetSpriteColor(g_VertexBuffer, TEXTURE_TITLELOGO_X, TEXTURE_TITLELOGO_Y,
				TEXTURE_TITLELOGO_WIDTH, TEXTURE_TITLELOGO_HEIGHT, 0.0f, 0.0f, 1.0f, 1.0f,
				XMFLOAT4(1.0f, 1.0f, 1.0f, alpha));

			// ポリゴン描画
			GetDeviceContext()->Draw(4, 0);
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

		SetBlendState(BLEND_MODE_ALPHABLEND);	// 半透明処理を元に戻す

		break;
	}
}





