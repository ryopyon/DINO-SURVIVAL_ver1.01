//=============================================================================
//
// 弾発射処理 [bullet_charge.h]
// Author : HAL東京 高橋諒
//
//=============================================================================
#pragma once
#include "main.h"
#include "renderer.h"
#include "model.h"
#include "bullet.h"



//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MAX_BULLET_CHARGE				(256)	// 弾最大数
#define MODEL_MAX_MATERIAL				(16)		// １モデルのMaxマテリアル数

#define	BULLET_CHARGE_RANGE_RED			(150.0f)	// 弾の飛距離
#define	BULLET_CHARGE_RANGE_BLUE		(300.0f)
#define	BULLET_CHARGE_RANGE_GREEN		(200.0f)
#define	BULLET_CHARGE_RANGE_YELLOW		(100.0f)

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	XMFLOAT3	pos;			// 位置
	XMFLOAT3	rot;			// 角度
	float		spd;			// 移動量
	int			shadowIdx;		// 影ID
	BOOL		bUse;			// 使用しているかどうか

	float		size;			// 当たり判定の大きさ

	XMFLOAT3	P0;				//制御点Ｐ１
	XMFLOAT3	P1;				//制御点Ｐ１
	XMFLOAT3	Tpos;
	float		s;				//線形補間用パラメータｓ

	int			type;
	XMFLOAT4	color;

} BULLET_CHARGE;

enum BULLET_CHARGE_TYPE
{
	BULLET_CHARGE_RED,
	BULLET_CHARGE_BLUE,
	BULLET_CHARGE_GREEN,
	BULLET_CHARGE_YELLOW,

	BULLET_CHARGE_NONE,

};


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitBullet_Charge(void);
void UninitBullet_Charge(void);
void UpdateBullet_Charge(void);
void DrawBullet_Charge(void);

int SetBullet_Charge(XMFLOAT3 pos, XMFLOAT3 rot);
void ReleaseBullet_Charge(int id);

BULLET_CHARGE* GetBullet_Charge(void);