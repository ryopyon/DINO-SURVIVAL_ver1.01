//=============================================================================
//
// エネミーUI処理 [ui_enemy.cpp]
// Author : HAL東京 高橋諒
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "input.h"
#include "camera.h"
#include "shadow.h"
#include "ui_enemy.h"
#include "enemy.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_MAX				(2)					// テクスチャの数

#define	UI_ENEMY_HP_WIDTH		(10.0f)				// 頂点サイズ
#define	UI_ENEMY_HP_HEIGHT		(2.0f)				// 頂点サイズ


#define	UI_ENEMY_HP_OFFSET_Y	(28.0f)				// オフセット


#define	MAX_UI_ENEMY			(MAX_ENEMY)			// エネミーUI最大数


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexUi_Enemy(void);
void ChangeColor(XMFLOAT4 color, float width, float depth);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer					*g_VertexBuffer = NULL;	// 頂点バッファ
static ID3D11ShaderResourceView		*g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報

static UI_ENEMY						g_aUi_Enemy[MAX_UI_ENEMY];	// エネミーUIワーク
static BOOL							g_bAlpaTest;		// アルファテストON/OFF

static int							g_TexNo;			// テクスチャ番号

static char *g_TextureName[] =
{
	"data/TEXTURE/game/gage_kara.jpg",
};

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitUi_Enemy(void)
{
	MakeVertexUi_Enemy();

	// テクスチャ生成
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		g_Texture[i] = NULL;
		D3DX11CreateShaderResourceViewFromFile(GetDevice(),
			g_TextureName[i],
			NULL,
			NULL,
			&g_Texture[i],
			NULL);
	}


	// エネミーUIワークの初期化
	for(int nCntUi_Enemy = 0; nCntUi_Enemy < MAX_UI_ENEMY; nCntUi_Enemy++)
	{
		ZeroMemory(&g_aUi_Enemy[nCntUi_Enemy].material, sizeof(g_aUi_Enemy[nCntUi_Enemy].material));
		g_aUi_Enemy[nCntUi_Enemy].material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

		g_aUi_Enemy[nCntUi_Enemy].pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_aUi_Enemy[nCntUi_Enemy].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);
		g_aUi_Enemy[nCntUi_Enemy].fWidth = UI_ENEMY_HP_WIDTH;
		g_aUi_Enemy[nCntUi_Enemy].fHeight = UI_ENEMY_HP_HEIGHT;
		g_aUi_Enemy[nCntUi_Enemy].use = TRUE;
	}

	g_bAlpaTest = TRUE;

	ENEMY* enemy = GetEnemy();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitUi_Enemy(void)
{
	for(int nCntTex = 0; nCntTex < TEXTURE_MAX; nCntTex++)
	{
		if(g_Texture[nCntTex] != NULL)
		{// テクスチャの解放
			g_Texture[nCntTex]->Release();
			g_Texture[nCntTex] = NULL;
		}
	}

	if(g_VertexBuffer != NULL)
	{// 頂点バッファの解放
		g_VertexBuffer->Release();
		g_VertexBuffer = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateUi_Enemy(void)
{
	ENEMY* enemy = GetEnemy();

	for (int i = 0; i < MAX_ENEMY; i++)
	{
		if (enemy[i].use)
		{
			g_aUi_Enemy[i].pos = enemy[i].pos;
		}
	}


#ifdef _DEBUG
	// アルファテストON/OFF
	if(GetKeyboardTrigger(DIK_F1))
	{
		g_bAlpaTest = g_bAlpaTest ? FALSE: TRUE;
	}

#endif

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawUi_Enemy(void)
{
	ENEMY* enemy = GetEnemy();

	// αテスト設定
	if (g_bAlpaTest == TRUE)
	{
		// αテストを有効に
		SetAlphaTestEnable(TRUE);
	}

	// ライティングを無効
	SetLightEnable(FALSE);

	XMMATRIX mtxScl, mtxTranslate, mtxWorld, mtxView;
	CAMERA *cam = GetCamera();

	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// プリミティブトポロジ設定
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	for(int i = 0; i < MAX_ENEMY; i++)
	{
		if( (enemy[i].use == TRUE) && (enemy[i].EntryTime == FALSE) && (enemy[i].LeaveTime == FALSE) )
		{
			// ワールドマトリックスの初期化
			mtxWorld = XMMatrixIdentity();

			// ビューマトリックスを取得
			mtxView = XMLoadFloat4x4(&cam->mtxView);

			// 正方行列（直交行列）を転置行列させて逆行列を作ってる版(速い)
			mtxWorld.r[0].m128_f32[0] = mtxView.r[0].m128_f32[0];
			mtxWorld.r[0].m128_f32[1] = mtxView.r[1].m128_f32[0];
			mtxWorld.r[0].m128_f32[2] = mtxView.r[2].m128_f32[0];

			mtxWorld.r[1].m128_f32[0] = mtxView.r[0].m128_f32[1];
			mtxWorld.r[1].m128_f32[1] = mtxView.r[1].m128_f32[1];
			mtxWorld.r[1].m128_f32[2] = mtxView.r[2].m128_f32[1];

			mtxWorld.r[2].m128_f32[0] = mtxView.r[0].m128_f32[2];
			mtxWorld.r[2].m128_f32[1] = mtxView.r[1].m128_f32[2];
			mtxWorld.r[2].m128_f32[2] = mtxView.r[2].m128_f32[2];


			// スケールを反映
			mtxScl = XMMatrixScaling(g_aUi_Enemy[i].scl.x, g_aUi_Enemy[i].scl.y, g_aUi_Enemy[i].scl.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

			// 移動を反映
			mtxTranslate = XMMatrixTranslation(g_aUi_Enemy[i].pos.x, g_aUi_Enemy[i].pos.y + UI_ENEMY_HP_OFFSET_Y, g_aUi_Enemy[i].pos.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

			// ワールドマトリックスの設定
			SetWorldMatrix(&mtxWorld);

			// マテリアル設定
			MATERIAL temp = g_aUi_Enemy[i].material;
			temp.Diffuse= XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

			SetMaterial(g_aUi_Enemy[i].material);

			// テクスチャ設定
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_TexNo]);

			//　先に空のゲージを描画
			ChangeColor(XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), UI_ENEMY_HP_WIDTH * 0.5f, 0.0f);

			// ポリゴンの描画
			GetDeviceContext()->Draw(4, 0);

			// エネミーのHP量で変動
			float enemyHpRatio;
			enemyHpRatio = enemy[i].hp / (float)ENEMY_HP_MAX; // （現在のエネミーのHP）/（エネミーの最大HP）

			// マテリアル設定
			SetMaterial(g_aUi_Enemy[i].material);

			// HPを描画
			ChangeColor(XMFLOAT4(0.2f, 1.0f, 0.2f, 1.0f), (UI_ENEMY_HP_WIDTH * enemyHpRatio)
				- (UI_ENEMY_HP_WIDTH * 0.5f), -0.1f);

			 //ポリゴンの描画
			GetDeviceContext()->Draw(4, 0);

		}
	}

	// ライティングを有効に
	SetLightEnable(TRUE);

	// αテストを無効に
	SetAlphaTestEnable(FALSE);
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
HRESULT MakeVertexUi_Enemy(void)
{
	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);

	// 頂点バッファに値をセットする
	D3D11_MAPPED_SUBRESOURCE msr;
	GetDeviceContext()->Map(g_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

	float fWidth = UI_ENEMY_HP_WIDTH;
	float fHeight = UI_ENEMY_HP_HEIGHT;

	// 頂点座標の設定
	vertex[0].Position = XMFLOAT3(-fWidth / 2.0f, fHeight, 0.0f);
	vertex[1].Position = XMFLOAT3(fWidth / 4.0f, fHeight, 0.0f);
	vertex[2].Position = XMFLOAT3(-fWidth / 2.0f, 0.0f, 0.0f);
	vertex[3].Position = XMFLOAT3(fWidth / 4.0f, 0.0f, 0.0f);

	// 法線の設定
	vertex[0].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	vertex[1].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	vertex[2].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	vertex[3].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

	// 拡散光の設定
	vertex[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	// テクスチャ座標の設定
	vertex[0].TexCoord = XMFLOAT2(0.0f, 0.0f);
	vertex[1].TexCoord = XMFLOAT2(1.0f, 0.0f);
	vertex[2].TexCoord = XMFLOAT2(0.0f, 1.0f);
	vertex[3].TexCoord = XMFLOAT2(1.0f, 1.0f);

	GetDeviceContext()->Unmap(g_VertexBuffer, 0);

	return S_OK;
}

//=============================================================================
// エネミーUIのパラメータをセット
//=============================================================================
int SetUi_Enemy(XMFLOAT3 pos, float fWidth, float fHeight, XMFLOAT4 col)
{
	ENEMY* enemy = GetEnemy();

	int nIdxUi_Enemy = -1;

	for(int nCntUi_Enemy = 0; nCntUi_Enemy < MAX_UI_ENEMY; nCntUi_Enemy++)
	{
		if(!g_aUi_Enemy[nCntUi_Enemy].use)
		{
			g_aUi_Enemy[nCntUi_Enemy].pos = pos;
			g_aUi_Enemy[nCntUi_Enemy].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);
			g_aUi_Enemy[nCntUi_Enemy].fWidth = fWidth;
			g_aUi_Enemy[nCntUi_Enemy].fHeight = fHeight;
			g_aUi_Enemy[nCntUi_Enemy].use = TRUE;

			// HPバーの位置をエネミーの真上に設定
			g_aUi_Enemy[nCntUi_Enemy].hpBarPos = 
				XMFLOAT3(enemy[nCntUi_Enemy].pos.x, 
							enemy[nCntUi_Enemy].pos.y + 100.0f, 
							enemy[nCntUi_Enemy].pos.z);

			nIdxUi_Enemy = nCntUi_Enemy;

			break;
		}
	}

	return nIdxUi_Enemy;
}

void ChangeColor(XMFLOAT4 color, float width, float depth)
{
	// 頂点バッファに値をセットする
	D3D11_MAPPED_SUBRESOURCE msr;
	GetDeviceContext()->Map(g_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
	
	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;
	
	float fWidth = UI_ENEMY_HP_WIDTH;
	float fHeight = UI_ENEMY_HP_HEIGHT;
	

	// 頂点座標の設定
	vertex[0].Position = XMFLOAT3(-fWidth / 2.0f, fHeight, depth);
	vertex[1].Position = XMFLOAT3(width, fHeight, depth);
	vertex[2].Position = XMFLOAT3(-fWidth / 2.0f, 0.0f, depth);
	vertex[3].Position = XMFLOAT3(width, 0.0f, depth);
	
	// 法線の設定
	vertex[0].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	vertex[1].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	vertex[2].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	vertex[3].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	
	 //拡散光の設定
	vertex[0].Diffuse = color;
	vertex[1].Diffuse = color;
	vertex[2].Diffuse = color;
	vertex[3].Diffuse = color;
	
	// テクスチャ座標の設定
	vertex[0].TexCoord = XMFLOAT2(0.0f, 0.0f);
	vertex[1].TexCoord = XMFLOAT2(1.0f, 0.0f);
	vertex[2].TexCoord = XMFLOAT2(0.0f, 1.0f);
	vertex[3].TexCoord = XMFLOAT2(1.0f, 1.0f);
	
	GetDeviceContext()->Unmap(g_VertexBuffer, 0);
	}

//=============================================================================
// エネミーの取得
//=============================================================================
UI_ENEMY* GetUI_Enemy(void)
{
	return &g_aUi_Enemy[0];
}
