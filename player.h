//=============================================================================
//
// モデル処理 [player.h]
// Author : HAL東京 高橋諒
//
//=============================================================================
#pragma once
#include "model.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_PLAYER							(1)					// プレイヤーの数
#define MAX_PLAYER_TYPE						(4)					// プレイヤーの種類数

#define	PLAYER_SIZE							(18.0f)				// 当たり判定の大きさ

#define	PLAYER_HP_MAX						(500)				// HP最大値
#define PLAYER_MP_MAX						(600)				// MP最大値

#define	PLAYER_RED_DAMAGE_VALUE				(200)				// プレイヤー（赤）が与えるダメージ量
#define	PLAYER_BLUE_DAMAGE_VALUE			(300)				// プレイヤー（青）が与えるダメージ量
#define	PLAYER_GREEN_DAMAGE_VALUE			(200)				// プレイヤー（緑）が与えるダメージ量
#define	PLAYER_YELLOW_DAMAGE_VALUE			(300)				// プレイヤー（黄）が与えるダメージ量

#define HP_GAIN_FRAME						(10)				// HPの回復速度
#define MP_GAIN_FRAME						(10)				// MPの回復速度
#define CHARGE_CNT_MAX						(60)				// チャージ時間

//*****************************************************************************
// 構造体定義
//*****************************************************************************
struct PLAYER
{
	XMFLOAT4X4			mtxWorld;			// ワールドマトリックス
	XMFLOAT3			pos;				// モデルの位置
	XMFLOAT3			rot;				// モデルの向き(回転)
	XMFLOAT3			scl;				// モデルの大きさ(スケール)
	float				dir;				// 向き

	int					hp;					// hp
	int					mp;					// MP

	int					damage;				// プレイヤーが与えるダメージ量

	float				spd;				// 移動スピード
	float				size;				

	int					shadowIdx;			// 影のインデックス番号

	BOOL				use;

	int					type;				// プレイヤーの色　タイプ
	int					state;				// 状態
	int					setTbl;				// セットされているテーブル
	XMFLOAT4			hinokoColor;		// ひのこの色
	int					HPGainCnt;			// HP回復量
	int					MPGainCnt;			// MP回復量
	int					chargeCnt;			// チャージ時間
	int					changeCnt;			// プレイヤー交代クールタイム処理用のカウント

	int					modelIdx;


	// 階層アニメーション用のメンバー変数
	float				time;				// 線形補間用
	int					tblNo;				// 行動データのテーブル番号
	int					tblMax;				// そのテーブルのデータ数

	// 親は、NULL、子供は親のアドレスを入れる
	PLAYER				*parent;			// 自分が親ならNULL、自分が子供なら親のplayerアドレス

	// クォータニオン
	XMFLOAT4			Quaternion;

	XMFLOAT3			UpVector;			// 自分が立っている所


	BOOL moving;
	BOOL attack;

	// dissolve
	DISSOLVE		dissolve;

};

struct PLAYER_MODEL
{
	BOOL				load;
	DX11_MODEL			model;				// モデル情報
	XMFLOAT4			diffuse[MODEL_MAX_MATERIAL];	// モデルの色
};

enum
{
	STATE_IDLE,
	STATE_WALK,
	STATE_HINOKO,
	STATE_DIE,

	STATE_MAX
};

enum PLAYER_COLOR
{
	PLAYER_RED,
	PLAYER_BLUE,
	PLAYER_GREEN,
	PLAYER_YELLOW,

	PLAYER_COLOR_MAX
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);

PLAYER *GetPlayer(void);

BOOL Collision(int index, float r, XMFLOAT3 epos, float er);

BOOL MoveLeftStick(void);
BOOL MoveRightStick(void);
BOOL MoveUpStick(void);
BOOL MoveDownStick(void);

//キーボード
BOOL AnyKeyPress(void);
BOOL MoveLeftKeyboard(void);
BOOL MoveRightKeyboard(void);
BOOL MoveUpKeyboard(void);
BOOL MoveDownKeyboard(void);

//ゲームパット
BOOL AnyStickpress(void);
BOOL MoveLeftStick(void);
BOOL MoveRightStick(void);
BOOL MoveUpStick(void);
BOOL MoveDownStick(void);


void ChangeModelPlayer(int type, int id);
