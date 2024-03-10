//=============================================================================
//
// パーティクル処理 [particle.h]
// Author : HAL東京 高橋諒
//
//=============================================================================
#pragma once

#define	MAX_PARTICLE		(2048)		// パーティクル最大数

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	XMFLOAT3		pos;			// 位置
	XMFLOAT3		rot;			// 回転
	XMFLOAT3		scale;			// スケール
	XMFLOAT3		move;			// 移動量
	float			angle;			// 回る量（緑用）
	float			radius;			// 回る量（緑用）
	XMFLOAT3		basepos;		// 回る中心（緑用）
	MATERIAL		material;		// マテリアル
	XMFLOAT4		RGBa;			// 保存用RGBa
	float			fSizeX;			// 幅
	float			fSizeY;			// 高さ
	int				nIdxShadow;		// 影ID
	int				nLife;			// 寿命
	int				type;
	BOOL			use;			// 使用しているかどうか
	BLEND_MODE		brendmode;


} PARTICLE;


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
enum TYPE
{
	TYPE_HINOKO_RED,
	TYPE_HINOKO_BLUE,
	TYPE_HINOKO_GREEN,
	TYPE_HINOKO_YELLOW,

	TYPE_HINOKO_CHARGE_RED,
	TYPE_HINOKO_CHARGE_BLUE,
	TYPE_HINOKO_CHARGE_GREEN,
	TYPE_HINOKO_CHARGE_YELLOW,

	TYPE_CHANGE_PLAYER,
	TYPE_SNOW,
	TYPE_ATTACK,
};

HRESULT InitParticle(void);
void UninitParticle(void);
void UpdateParticle(void);
void DrawParticle(void);

int SetParticle(XMFLOAT3 pos, XMFLOAT3 scale, XMFLOAT3 move, XMFLOAT4 col, int type, BLEND_MODE brendmode, float fSizeX, float fSizeY, int nLife);
int SetParticle(XMFLOAT3 pos, XMFLOAT3 scale, XMFLOAT3 move, XMFLOAT4 col, int type, BLEND_MODE brendmode, float fSizeX, float fSizeY, int nLife, XMFLOAT3 basepos, float angle);
void SetColorParticle(int nIdxParticle, XMFLOAT4 col);
PARTICLE* GetParticle(void);

