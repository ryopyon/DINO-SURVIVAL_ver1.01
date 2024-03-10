//=============================================================================
//
// エネミーモデル処理 [enemy.h]
// Author : HAL東京 高橋諒
//
//=============================================================================
#pragma once
#include "model.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_ENEMY				(40)					// エネミーの数
#define MAX_ENEMY_TYPE			(4)						// エネミーの種類数

#define	ENEMY_SIZE				(5.0f)					// 当たり判定の大きさ

#define	ENEMY_HP_MAX			(600)					// HP最大値

//#define	ENEMY_DAMAGE_VALUE		(100)					// エネミーが与えるダメージ量
#define	ENEMY_DAMAGE_VALUE		(170)					// エネミーが与えるダメージ量

#define	ENEMY_RED_SPD			(0.4)					// エネミー（赤）のスピード
#define	ENEMY_BLUE_SPD			(0.8)					// エネミー（青）のスピード
#define	ENEMY_GREEN_SPD			(1.2)					// エネミー（緑）のスピード
#define	ENEMY_YELLOW_SPD		(1.6)					// エネミー（黄）のスピード

#define	ENEMY_SPAWN_RANGE_X		(1100)					// エネミーの自動生成範囲
#define	ENEMY_SPAWN_RANGE_Z		(1100)				

//*****************************************************************************
// 構造体定義
//*****************************************************************************
struct ENEMY
{
	XMFLOAT4X4			mtxWorld;			// ワールドマトリックス
	XMFLOAT3			pos;				// モデルの位置
	XMFLOAT3			rot;				// モデルの向き(回転)
	XMFLOAT3			scl;				// モデルの大きさ(スケール)
	float				spd;				// 移動スピード
	float				size;				// 当たり判定の大きさ

	BOOL				use;
	BOOL				EntryTime;			// 登場シーン
	BOOL				LeaveTime;			// 退場シーン

	float				burnTime;			// 火状態時間
	float				wetTime;			// 水状態時間
	float				windyTime;			// 風状態時間
	float				shockTime;			// 雷状態時間

	int					hp;					// hp

	int					damage;				// エネミーが与えるダメージ量

	int					type;
	int					state;
	int					shadowIdx;			// 影のインデックス番号

	ENEMY* parent;			// 自分が親ならNULL、自分が子供なら親のplayerアドレス

	float				time;				// 線形補間用
	int					tblNo;				// 行動データのテーブル番号
	int					tblMax;				// そのテーブルのデータ数

	int					modelIdx;

	// dissolve
	DISSOLVE			dissolve;

};

struct ENEMY_MODEL
{
	BOOL				load;
	DX11_MODEL			model;				// モデル情報
	XMFLOAT4			diffuse[MODEL_MAX_MATERIAL];	// モデルの色
};

enum ENEMY_COLOR
{
	ENEMY_RED,
	ENEMY_BLUE,
	ENEMY_GREEN,
	ENEMY_YELLOW,

	ENEMY_COLOR_MAX
};


enum
{
	ENEMY_ABNORMAL_NONE,
	ENEMY_ABNORMAL_BURN,
	ENEMY_ABNORMAL_WET,
	ENEMY_ABNORMAL_WINDY,
	ENEMY_ABNORMAL_SHOCK,

};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);

ENEMY *GetEnemy(void);

void SetEnemy(void);

void ChangeModelEnemy(int type, int id);
