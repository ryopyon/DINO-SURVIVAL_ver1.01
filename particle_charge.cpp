//=============================================================================
//
// チャージパーティクル処理 [particle_charge.cpp]
// Author : HAL東京 高橋諒
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "input.h"
#include "camera.h"
#include "light.h"
#include "particle_charge.h"
#include "player.h"
#include "debugproc.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_MAX					(1)			// テクスチャの数
#define	MAX_PARTICLE_CHARGE			(1028)		// パーティクル最大数
#define	PARTICLE_CHARGE_SIZE_X		(40.0f)		// 頂点サイズ
#define	PARTICLE_CHARGE_SIZE_Y		(40.0f)		// 頂点サイズ


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexParticleCharge(void);


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ID3D11Buffer*				g_VertexBuffer = NULL;					// 頂点バッファ
static ID3D11ShaderResourceView*	g_Texture[TEXTURE_MAX] = { NULL };		// テクスチャ情報
static int							g_TexNo;								// テクスチャ番号

static PARTICLE_CHARGE				g_ParticleCharge[MAX_PARTICLE_CHARGE];				// パーティクルワーク

static char* g_TextureName[] =
{
	"data/TEXTURE/effect000.jpg",
};

static BOOL			g_Load = FALSE;


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitParticleCharge(void)
{
	// 頂点情報の作成
	MakeVertexParticleCharge();

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
	for (int i = 0; i < MAX_PARTICLE_CHARGE; i++)
	{
		g_ParticleCharge[i].pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_ParticleCharge[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_ParticleCharge[i].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);
		g_ParticleCharge[i].move = XMFLOAT3(1.0f, 1.0f, 1.0f);

		ZeroMemory(&g_ParticleCharge[i].material, sizeof(g_ParticleCharge[i].material));
		g_ParticleCharge[i].material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

		g_ParticleCharge[i].life = 0;
		g_ParticleCharge[i].use = FALSE;
	}

	g_Load = TRUE;

	return S_OK;
}


//=============================================================================
// 終了処理
//=============================================================================
void UninitParticleCharge(void)
{
	if (g_Load == FALSE) return;

	//テクスチャの解放
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		if (g_Texture[i] != NULL)
		{
			g_Texture[i]->Release();
			g_Texture[i] = NULL;
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
void UpdateParticleCharge(void)
{
	PLAYER* player = GetPlayer();

	int pcCnt = 0;

	for (int i = 0; i < MAX_PARTICLE_CHARGE; i++)
	{
		// 使用中
		if (g_ParticleCharge[i].use == FALSE) continue;

		g_ParticleCharge[i].ePos = player->pos;

		// 移動処理
		{
			XMVECTOR nPos = XMLoadFloat3(&g_ParticleCharge[i].sPos);	// XMVECTORへ変換
			XMVECTOR Pos = XMLoadFloat3(&g_ParticleCharge[i].ePos) - nPos;	// 座標を計算している
			Pos *= g_ParticleCharge[i].lerpTime;
			// 計算して求めた座標を現在の移動テーブルに足している
			XMStoreFloat3(&g_ParticleCharge[i].pos, nPos + Pos);
			g_ParticleCharge[i].lerpTime += 1.0f / g_ParticleCharge[i].life;
		}

		// 残留時間を超えたらパーティクルを未使用状態にする
		{
			g_ParticleCharge[i].life--;
			if (g_ParticleCharge[i].life <= 0)
			{
				g_ParticleCharge[i].use = FALSE;
			}
			else
			{
				if (g_ParticleCharge[i].life <= 80)
				{
					float colorX = g_ParticleCharge[i].RGBa.x;
					float colorY = g_ParticleCharge[i].RGBa.y;
					float colorZ = g_ParticleCharge[i].RGBa.z;
					g_ParticleCharge[i].material.Diffuse.x = colorX - (float)(80 - g_ParticleCharge[i].life) / 80 * colorX;
					g_ParticleCharge[i].material.Diffuse.y = colorY - (float)(80 - g_ParticleCharge[i].life) / 80 * colorY;
					g_ParticleCharge[i].material.Diffuse.z = colorZ - (float)(80 - g_ParticleCharge[i].life) / 80 * colorZ;
				}

				// 残留時間が20フレーム以下の場合、α値を下げていく
				if (g_ParticleCharge[i].life <= 20)
				{
					// α値設定
					g_ParticleCharge[i].material.Diffuse.w -= 0.05f;
					if (g_ParticleCharge[i].material.Diffuse.w < 0.0f)
					{
						g_ParticleCharge[i].material.Diffuse.w = 0.0f;
					}
				}
			}
		}

		pcCnt++;
	}

#ifdef _DEBUG
	// デバッグ表示
	PrintDebugProc("pcCnt %d\n", pcCnt);
#endif


}


//=============================================================================
// 描画処理
//=============================================================================
void DrawParticleCharge(void)
{
	XMMATRIX mtxScl, mtxTranslate, mtxWorld, mtxView;
	CAMERA* cam = GetCamera();

	SetLightEnable(FALSE);				// ライティングを無効に
	SetBlendState(BLEND_MODE_ADD);		// 加算合成に設定
	SetDepthEnable(FALSE);				// Z比較無し
	SetFogEnable(FALSE);				// フォグ無効

	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// プリミティブトポロジ設定
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// テクスチャ設定
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_TexNo]);

	for (int i = 0; i < MAX_PARTICLE_CHARGE; i++)
	{
		if (g_ParticleCharge[i].use)
		{
			// ワールドマトリックスの初期化
			mtxWorld = XMMatrixIdentity();

			// ビューマトリックスを取得
			mtxView = XMLoadFloat4x4(&cam->mtxView);
			if (GetViewPortType() == TYPE_LEFT_HALF_SCREEN) mtxView = XMLoadFloat4x4(&cam[0].mtxView);
			if (GetViewPortType() == TYPE_RIGHT_HALF_SCREEN) mtxView = XMLoadFloat4x4(&cam[1].mtxView);
			if (GetViewPortType() == TYPE_UP_HALF_SCREEN) mtxView = XMLoadFloat4x4(&cam[0].mtxView);
			if (GetViewPortType() == TYPE_DOWN_HALF_SCREEN) mtxView = XMLoadFloat4x4(&cam[1].mtxView);


			// ビルボード
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
			mtxScl = XMMatrixScaling(g_ParticleCharge[i].scl.x, g_ParticleCharge[i].scl.y, g_ParticleCharge[i].scl.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

			// 移動を反映
			mtxTranslate = XMMatrixTranslation(g_ParticleCharge[i].pos.x, g_ParticleCharge[i].pos.y, g_ParticleCharge[i].pos.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

			// ワールドマトリックスの設定
			SetWorldMatrix(&mtxWorld);

			// マテリアル設定
			SetMaterial(g_ParticleCharge[i].material);

			// ポリゴンの描画
			GetDeviceContext()->Draw(4, 0);
		}
	}

	SetLightEnable(TRUE);						// ライティングを有効に
	SetBlendState(BLEND_MODE_ALPHABLEND);		// 通常ブレンドに戻す
	SetDepthEnable(TRUE);						// Z比較有効
	SetFogEnable(GetFogEnable());				// フォグ有効
}


//=============================================================================
// 頂点情報の作成
//=============================================================================
HRESULT MakeVertexParticleCharge(void)
{
	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);

	//頂点バッファの中身を埋める
	{
		D3D11_MAPPED_SUBRESOURCE msr;
		GetDeviceContext()->Map(g_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

		// 頂点座標の設定
		vertex[0].Position = XMFLOAT3(-PARTICLE_CHARGE_SIZE_X / 2,  PARTICLE_CHARGE_SIZE_Y / 2, 0.0f);
		vertex[1].Position = XMFLOAT3( PARTICLE_CHARGE_SIZE_X / 2,  PARTICLE_CHARGE_SIZE_Y / 2, 0.0f);
		vertex[2].Position = XMFLOAT3(-PARTICLE_CHARGE_SIZE_X / 2, -PARTICLE_CHARGE_SIZE_Y / 2, 0.0f);
		vertex[3].Position = XMFLOAT3( PARTICLE_CHARGE_SIZE_X / 2, -PARTICLE_CHARGE_SIZE_Y / 2, 0.0f);

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
void SetColorParticleCharge(int index, XMFLOAT4 color)
{
	g_ParticleCharge[index].material.Diffuse = color;
}

//=============================================================================
// パーティクルの発生処理
//=============================================================================
int SetParticleCharge(XMFLOAT3 pos, XMFLOAT3 ePos, XMFLOAT3 scl, XMFLOAT4 col, int type, int life)
{
	int idx = -1;

	for (int i = 0; i < MAX_PARTICLE_CHARGE; i++)
	{
		if (g_ParticleCharge[i].use == FALSE)
		{
			g_ParticleCharge[i].pos = pos;
			g_ParticleCharge[i].scl = scl;
			g_ParticleCharge[i].material.Diffuse = col;
			g_ParticleCharge[i].type = type;
			g_ParticleCharge[i].life = life;
			g_ParticleCharge[i].RGBa = col;
			g_ParticleCharge[i].sPos = pos;
			g_ParticleCharge[i].ePos = ePos;
			g_ParticleCharge[i].lerpTime = 0.0f;
			g_ParticleCharge[i].use = TRUE;

			idx = i;

			break;
		}
	}

	return idx;
}

