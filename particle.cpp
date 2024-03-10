//=============================================================================
//
// パーティクル処理 [particle.cpp]
// Author : HAL東京 高橋諒
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "input.h"
#include "camera.h"
#include "model.h"
#include "shadow.h"
#include "light.h"
#include "particle.h"
#include "player.h"
#include "debugproc.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_MAX			(1)			// テクスチャの数

#define	PARTICLE_SIZE_X		(40.0f)		// 頂点サイズ
#define	PARTICLE_SIZE_Y		(40.0f)		// 頂点サイズ
#define	PARTICLE_SIZE		(20.0f)		// 頂点サイズ
#define	VALUE_MOVE_PARTICLE	(5.0f)		// 移動速度


//#define	DISP_SHADOW						// 影の表示
#undef DISP_SHADOW

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexParticle(void);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer* g_VertexBuffer = NULL;		// 頂点バッファ

static ID3D11ShaderResourceView* g_Texture[TEXTURE_MAX] = { NULL };	// テクスチャ情報
static int							g_TexNo;					// テクスチャ番号

static PARTICLE					g_aParticle[MAX_PARTICLE];		// パーティクルワーク
static XMFLOAT3					g_posBase;						// ビルボード発生位置
static float					g_fWidthBase = 1.0f;			// 基準の幅
static float					g_fHeightBase = 10.0f;			// 基準の高さ
static float					g_roty = 0.0f;					// 移動方向
static float					g_spd = 0.0f;					// 移動スピード

static char* g_TextureName[] =
{
	"data/TEXTURE/effect000.jpg",
};

static BOOL							g_Load = FALSE;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitParticle(void)
{
	// 頂点情報の作成
	MakeVertexParticle();

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

	g_TexNo = 0;

	// パーティクルワークの初期化
	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		g_aParticle[nCntParticle].pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_aParticle[nCntParticle].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_aParticle[nCntParticle].scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
		g_aParticle[nCntParticle].move = XMFLOAT3(1.0f, 1.0f, 1.0f);
		g_aParticle[nCntParticle].angle = 0.0f;
		g_aParticle[nCntParticle].radius = 0.0f;
		g_aParticle[nCntParticle].basepos = XMFLOAT3(0.0f, 0.0f, 0.0f);

		ZeroMemory(&g_aParticle[nCntParticle].material, sizeof(g_aParticle[nCntParticle].material));
		g_aParticle[nCntParticle].material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

		g_aParticle[nCntParticle].fSizeX = PARTICLE_SIZE;
		g_aParticle[nCntParticle].fSizeY = PARTICLE_SIZE;
		g_aParticle[nCntParticle].nIdxShadow = -1;
		g_aParticle[nCntParticle].nLife = 0;
		g_aParticle[nCntParticle].type = 0;
		g_aParticle[nCntParticle].use = FALSE;
		g_aParticle[nCntParticle].brendmode = BLEND_MODE_NONE;
	}


	g_posBase = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_roty = 0.0f;
	g_spd = 0.0f;

	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitParticle(void)
{
	if (g_Load == FALSE) return;

	//テクスチャの解放
	for (int nCntTex = 0; nCntTex < TEXTURE_MAX; nCntTex++)
	{
		if (g_Texture[nCntTex] != NULL)
		{
			g_Texture[nCntTex]->Release();
			g_Texture[nCntTex] = NULL;
		}
	}

	// 頂点バッファの解放
	if (g_VertexBuffer != NULL)
	{
		g_VertexBuffer->Release();
		g_VertexBuffer = NULL;
	}

	g_Load = FALSE;
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateParticle(void)
{
		int particleCnt = 0;
	{

		for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
		{
			if (g_aParticle[nCntParticle].use)
			{// 使用中
				g_aParticle[nCntParticle].pos.x += g_aParticle[nCntParticle].move.x;
				g_aParticle[nCntParticle].pos.z += g_aParticle[nCntParticle].move.z;

				g_aParticle[nCntParticle].pos.y += g_aParticle[nCntParticle].move.y;


#ifdef DISP_SHADOW
				if (g_aParticle[nCntParticle].nIdxShadow != -1)
				{// 影使用中
					float colA;

					// 影の位置設定
					SetPositionShadow(g_aParticle[nCntParticle].nIdxShadow, XMFLOAT3(g_aParticle[nCntParticle].pos.x, 0.1f, g_aParticle[nCntParticle].pos.z));

					// 影の色の設定
					colA = g_aParticle[nCntParticle].material.Diffuse.w;
					SetColorShadow(g_aParticle[nCntParticle].nIdxShadow, XMFLOAT4(0.5f, 0.5f, 0.5f, colA));
				}
#endif

				g_aParticle[nCntParticle].nLife--;

				if (g_aParticle[nCntParticle].nLife <= 0)
				{
					// 消す処理
					g_aParticle[nCntParticle].use = FALSE;
					g_aParticle[nCntParticle].angle = 0.0f;
					g_aParticle[nCntParticle].radius = 0.0f;

					ReleaseShadow(g_aParticle[nCntParticle].nIdxShadow);
					g_aParticle[nCntParticle].nIdxShadow = -1;
				}
				else
				{
					if (g_aParticle[nCntParticle].nLife <= 80)
					{

						if (g_aParticle[nCntParticle].scale.x > 0.0f)
						{
							switch (g_aParticle[nCntParticle].type)
							{
							case TYPE_HINOKO_RED:
							case TYPE_HINOKO_BLUE:
							case TYPE_CHANGE_PLAYER:
								g_aParticle[nCntParticle].scale.x -= 0.025f;
								g_aParticle[nCntParticle].scale.y -= 0.025f;
								g_aParticle[nCntParticle].scale.z -= 0.025f;
								break;

							case TYPE_HINOKO_GREEN:
							case TYPE_HINOKO_YELLOW:
								break;

							case TYPE_HINOKO_CHARGE_GREEN:	// 竜巻
								g_aParticle[nCntParticle].angle += 1.0f;

								if (g_aParticle[nCntParticle].angle >= XM_PI)
								{
									g_aParticle[nCntParticle].angle -= XM_2PI;
								}
									g_aParticle[nCntParticle].radius += 0.5f;

								// 螺旋の進行を計算
								g_aParticle[nCntParticle].pos.x = g_aParticle[nCntParticle].basepos.x + 
									g_aParticle[nCntParticle].radius * sinf(g_aParticle[nCntParticle].angle);
								g_aParticle[nCntParticle].pos.x += (float)(rand() % 3);

								g_aParticle[nCntParticle].pos.z = g_aParticle[nCntParticle].basepos.z + 
									g_aParticle[nCntParticle].radius * cosf(g_aParticle[nCntParticle].angle);

								break;

							case TYPE_HINOKO_CHARGE_YELLOW:	// 雷

								if (g_aParticle[nCntParticle].pos.y <= g_aParticle[nCntParticle].fSizeY / 8)
								{// 着地した

									g_aParticle[nCntParticle].pos.y = g_aParticle[nCntParticle].fSizeY / 8;
									g_aParticle[nCntParticle].move.y = 0.0f;
									g_aParticle[nCntParticle].radius += 1.0f;
									g_aParticle[nCntParticle].scale = { 0.1f, 0.1f, 0.1f };

									// 螺旋の進行を計算
									g_aParticle[nCntParticle].pos.x = g_aParticle[nCntParticle].basepos.x +
										g_aParticle[nCntParticle].radius * sinf(g_aParticle[nCntParticle].angle);
									g_aParticle[nCntParticle].pos.x += (float)(rand() % 7);

									g_aParticle[nCntParticle].pos.z = g_aParticle[nCntParticle].basepos.z +
										g_aParticle[nCntParticle].radius * cosf(g_aParticle[nCntParticle].angle);
									g_aParticle[nCntParticle].pos.z += (float)(rand() % 7);


								}
								else	// 広げる
								{
									g_aParticle[nCntParticle].radius = 0.5f;

									// 螺旋の進行を計算
									g_aParticle[nCntParticle].pos.x = g_aParticle[nCntParticle].basepos.x +
										g_aParticle[nCntParticle].radius;

									g_aParticle[nCntParticle].pos.x += (float)(rand() % 3);
									g_aParticle[nCntParticle].pos.z = g_aParticle[nCntParticle].basepos.z +
										g_aParticle[nCntParticle].radius;

								}

								break;

							case TYPE_SNOW:	// 雪
								g_aParticle[nCntParticle].scale.x -= 0.001f;
								g_aParticle[nCntParticle].scale.y -= 0.001f;
								g_aParticle[nCntParticle].scale.z -= 0.001f;
								// α値設定
								g_aParticle[nCntParticle].material.Diffuse.w -= 0.01f;
								if (g_aParticle[nCntParticle].material.Diffuse.w < 0.0f)
								{
									g_aParticle[nCntParticle].material.Diffuse.w = 0.0f;
								}

								if (g_aParticle[nCntParticle].pos.y <= 5.0f)
								{
									g_aParticle[nCntParticle].move.x = 0.0f;
									g_aParticle[nCntParticle].move.z = 0.0f;

								}

								break;
							}

						}
						else
						{
							g_aParticle[nCntParticle].use = FALSE;
						}
					}
				}

				particleCnt++;
			}
		}



		// パーティクル発生(雪)
		{
			XMFLOAT3 pos;
			XMFLOAT3 move;
			float fAngle, fLength;
			int nLife;
			float fSize;

			pos = g_posBase;

			fAngle = (float)(rand() % 628 - 314) / 100.0f;
			fLength = rand() % (int)(g_fWidthBase * 200) / 100.0f - g_fWidthBase;

			move.x = sinf(fAngle) * fLength;
			move.y = -0.5f;
			move.z = cosf(fAngle) * fLength;

			nLife = rand() % 300 + 100;

			fSize = (float)(rand() % 30 + 10);

			pos.x = (float)(rand() % 800 - 400);
			pos.y = 200.0f;
			pos.z = (float)(rand() % 800 - 400);

			//ビルボードの設定
			SetParticle(pos, XMFLOAT3(0.1f, 0.1f, 0.1f), move, XMFLOAT4(1.0f, 1.0f, 1.0f, 0.85f), 
				TYPE_SNOW, BLEND_MODE_SUBTRACT, fSize, fSize, nLife);
		}
	}

#ifdef _DEBUG
	// デバッグ表示
	PrintDebugProc("particleCnt %d\n", particleCnt);
#endif

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawParticle(void)
{
	XMMATRIX mtxScl, mtxTranslate, mtxWorld, mtxView;
	CAMERA* cam = GetCamera();

	// ライティングを無効に
	SetLightEnable(FALSE);

	// Z比較無し
	SetDepthEnable(FALSE);

	// フォグ無効
	SetFogEnable(FALSE);

	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// プリミティブトポロジ設定
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// テクスチャ設定
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_TexNo]);

	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		if (g_aParticle[nCntParticle].use)
		{
			// 加算合成に設定
			SetBlendState(g_aParticle[nCntParticle].brendmode);

			// ワールドマトリックスの初期化
			mtxWorld = XMMatrixIdentity();

			// ビューマトリックスを取得
			mtxView = XMLoadFloat4x4(&cam->mtxView);


			// 処理が速いしお勧め
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
			mtxScl = XMMatrixScaling(g_aParticle[nCntParticle].scale.x, g_aParticle[nCntParticle].scale.y, g_aParticle[nCntParticle].scale.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

			// 移動を反映
			mtxTranslate = XMMatrixTranslation(g_aParticle[nCntParticle].pos.x, g_aParticle[nCntParticle].pos.y, g_aParticle[nCntParticle].pos.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

			// ワールドマトリックスの設定
			SetWorldMatrix(&mtxWorld);

			// マテリアル設定
			SetMaterial(g_aParticle[nCntParticle].material);

			// ポリゴンの描画
			GetDeviceContext()->Draw(4, 0);
		}
	}

	// ライティングを有効に
	SetLightEnable(TRUE);

	// 通常ブレンドに戻す
	SetBlendState(BLEND_MODE_ALPHABLEND);

	// Z比較有効
	SetDepthEnable(TRUE);

	// フォグ有効
	SetFogEnable(GetFogEnable());

}

//=============================================================================
// 頂点情報の作成
//=============================================================================
HRESULT MakeVertexParticle(void)
{
	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);

	{//頂点バッファの中身を埋める
		D3D11_MAPPED_SUBRESOURCE msr;
		GetDeviceContext()->Map(g_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

		// 頂点座標の設定
		vertex[0].Position = XMFLOAT3(-PARTICLE_SIZE_X / 2, PARTICLE_SIZE_Y / 2, 0.0f);
		vertex[1].Position = XMFLOAT3(PARTICLE_SIZE_X / 2, PARTICLE_SIZE_Y / 2, 0.0f);
		vertex[2].Position = XMFLOAT3(-PARTICLE_SIZE_X / 2, -PARTICLE_SIZE_Y / 2, 0.0f);
		vertex[3].Position = XMFLOAT3(PARTICLE_SIZE_X / 2, -PARTICLE_SIZE_Y / 2, 0.0f);

		// 法線の設定
		vertex[0].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
		vertex[1].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
		vertex[2].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
		vertex[3].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

		// 反射光の設定
		vertex[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[2].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[3].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

		// テクスチャ座標の設定
		vertex[0].TexCoord = { 0.0f, 0.0f };
		vertex[1].TexCoord = { 1.0f, 0.0f };
		vertex[2].TexCoord = { 0.0f, 1.0f };
		vertex[3].TexCoord = { 1.0f, 1.0f };

		GetDeviceContext()->Unmap(g_VertexBuffer, 0);
	}

	return S_OK;
}

//=============================================================================
// マテリアルカラーの設定
//=============================================================================
void SetColorParticle(int nIdxParticle, XMFLOAT4 col)
{
	g_aParticle[nIdxParticle].material.Diffuse = col;
}

//=============================================================================
// パーティクルの発生処理
//=============================================================================
int SetParticle(XMFLOAT3 pos, XMFLOAT3 scale, XMFLOAT3 move, XMFLOAT4 col, int type, BLEND_MODE brendmode, float fSizeX, float fSizeY, int nLife)
{
	int nIdxParticle = -1;

	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		if (!g_aParticle[nCntParticle].use)
		{
			g_aParticle[nCntParticle].pos = pos;
			g_aParticle[nCntParticle].rot = { 0.0f, 0.0f, 0.0f };
			g_aParticle[nCntParticle].scale = scale;
			g_aParticle[nCntParticle].move = move;
			g_aParticle[nCntParticle].material.Diffuse = col;
			g_aParticle[nCntParticle].RGBa = col;
			g_aParticle[nCntParticle].fSizeX = fSizeX;
			g_aParticle[nCntParticle].fSizeY = fSizeY;
			g_aParticle[nCntParticle].nLife = nLife;
			g_aParticle[nCntParticle].type = type;
			g_aParticle[nCntParticle].use = TRUE;
			g_aParticle[nCntParticle].brendmode = brendmode;

			nIdxParticle = nCntParticle;

#ifdef DISP_SHADOW
			// 影の設定
			g_aParticle[nCntParticle].nIdxShadow = CreateShadow(XMFLOAT3(pos.x, 0.1f, pos.z), 0.8f, 0.8f);		// 影の設定
			if (g_aParticle[nCntParticle].nIdxShadow != -1)
			{
				SetColorShadow(g_aParticle[nCntParticle].nIdxShadow, XMFLOAT4(1.0f, 1.0f, 1.0f, 0.5f));
			}
#endif

			break;
		}
	}

	return nIdxParticle;
}

// 竜巻用
int SetParticle(XMFLOAT3 pos, XMFLOAT3 scale, XMFLOAT3 move, XMFLOAT4 col, int type, BLEND_MODE brendmode, float fSizeX, float fSizeY, int nLife, XMFLOAT3 basepos, float angle)
{
	int nIdxParticle = -1;

	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		if (!g_aParticle[nCntParticle].use)
		{
			g_aParticle[nCntParticle].pos = pos;
			g_aParticle[nCntParticle].rot = { 0.0f, 0.0f, 0.0f };
			g_aParticle[nCntParticle].scale = scale;
			g_aParticle[nCntParticle].move = move;
			g_aParticle[nCntParticle].basepos = basepos;
			g_aParticle[nCntParticle].angle = angle;
			g_aParticle[nCntParticle].material.Diffuse = col;
			g_aParticle[nCntParticle].RGBa = col;
			g_aParticle[nCntParticle].fSizeX = fSizeX;
			g_aParticle[nCntParticle].fSizeY = fSizeY;
			g_aParticle[nCntParticle].nLife = nLife;
			g_aParticle[nCntParticle].type = type;
			g_aParticle[nCntParticle].use = TRUE;
			g_aParticle[nCntParticle].brendmode = brendmode;

			nIdxParticle = nCntParticle;

#ifdef DISP_SHADOW
			// 影の設定
			g_aParticle[nCntParticle].nIdxShadow = CreateShadow(XMFLOAT3(pos.x, 0.1f, pos.z), 0.8f, 0.8f);		// 影の設定
			if (g_aParticle[nCntParticle].nIdxShadow != -1)
			{
				SetColorShadow(g_aParticle[nCntParticle].nIdxShadow, XMFLOAT4(1.0f, 1.0f, 1.0f, 0.5f));
			}
#endif

			break;
		}
	}

	return nIdxParticle;
}

//=============================================================================
// パーティクル情報を取得
//=============================================================================
PARTICLE* GetParticle(void)
{
	return &g_aParticle[0];
}
