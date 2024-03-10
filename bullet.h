//=============================================================================
//
// 弾発射処理 [bullet.h]
// Author : HAL東京 高橋諒
//
//=============================================================================
#pragma once
#include "main.h"
#include "renderer.h"
#include "model.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MAX_BULLET				(256)		// 弾最大数
#define MODEL_MAX_MATERIAL		(16)		// １モデルのMaxマテリアル数

#define	BULLET_RANGE_RED		(150.0f)	// 弾の飛距離
#define	BULLET_RANGE_BLUE		(300.0f)
#define	BULLET_RANGE_GREEN		(200.0f)
#define	BULLET_RANGE_YELLOW		(400.0f)

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

} BULLET;

enum BULLET_TYPE
{
	BULLET_RED,
	BULLET_BLUE,
	BULLET_GREEN,
	BULLET_YELLOW,

	BULLET_NONE,

};


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitBullet(void);
void UninitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);

int SetBullet(XMFLOAT3 pos, XMFLOAT3 rot);
int SetBullet(XMFLOAT3 pos, XMFLOAT3 rot, XMFLOAT4 color, int type);
void ReleaseBullet(int id);

BULLET* GetBullet(void);

XMVECTOR LinearInterpolation(XMVECTOR P0, XMVECTOR P1, float s);
XMVECTOR QuadraticBezierCurve(XMVECTOR p0, XMVECTOR p1, XMVECTOR p2, float s);