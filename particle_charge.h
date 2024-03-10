//=============================================================================
//
// パーティクル処理 [particle.h]
// Author : HAL東京 高橋諒
//
//=============================================================================
#pragma once


//*****************************************************************************
// マクロ定義
//*****************************************************************************
enum
{
	PARTICLE_CHARGE_RED,
	PARTICLE_CHARGE_BLUE,
	PARTICLE_CHARGE_GREEN,
	PARTICLE_CHARGE_YELLOW,
};

//*****************************************************************************
// 構造体定義
//*****************************************************************************
struct PARTICLE_CHARGE
{
	XMFLOAT3		pos;			// 位置
	XMFLOAT3		rot;			// 回転
	XMFLOAT3		scl;			// スケール
	XMFLOAT3		move;			// 移動量
	MATERIAL		material;		// マテリアル
	XMFLOAT4		RGBa;			// 保持用RGBa
	XMFLOAT3		sPos;			// 開始位置(線形補間用)
	XMFLOAT3		ePos;			// 目標位置(線形補間用)
	float			lerpTime;		// 線形補間用の時間
	int				type;			// タイプ
	int				life;			// 寿命
	BOOL			use;			// 使用しているかどうか
};


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT		InitParticleCharge(void);
void		UninitParticleCharge(void);
void		UpdateParticleCharge(void);
void		DrawParticleCharge(void);

int			SetParticleCharge(XMFLOAT3 pos, XMFLOAT3 ePos, XMFLOAT3 scl, XMFLOAT4 col, int type, int life);
void		SetColorParticleCharge(int index, XMFLOAT4 color);

