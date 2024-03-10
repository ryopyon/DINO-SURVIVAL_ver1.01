//=============================================================================
//
// エネミーモデル処理 [enemy.cpp]
// Author : HAL東京 高橋諒
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "model.h"
#include "input.h"
#include "debugproc.h"
#include "player.h"
#include "enemy.h"
#include "bullet_charge.h"
#include "ui_enemy.h"
#include "collision.h"
#include "shadow.h"
#include "particle.h"
#include "flag.h"
#include "keyframe.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MODEL_ENEMY_RED				"data/MODEL/akamaru/akamaru_body.obj"		// 読み込むモデル名
#define	MODEL_ENEMY_RED_HEAD		"data/MODEL/akamaru/akamaru_head.obj"		// 読み込むモデル名
#define	MODEL_ENEMY_RED_LLEG		"data/MODEL/akamaru/akamaru_leg_l.obj"		// 読み込むモデル名
#define	MODEL_ENEMY_RED_RLEG		"data/MODEL/akamaru/akamaru_leg_r.obj"		// 読み込むモデル名
#define	MODEL_ENEMY_RED_TAIL		"data/MODEL/akamaru/akamaru_tail.obj"		// 読み込むモデル名

#define	MODEL_ENEMY_BLUE			"data/MODEL/BlueSuke/BlueSuke_body.obj"		// 読み込むモデル名
#define	MODEL_ENEMY_BLUE_HEAD		"data/MODEL/BlueSuke/BlueSuke_head.obj"		// 読み込むモデル名
#define	MODEL_ENEMY_BLUE_LLEG		"data/MODEL/BlueSuke/BlueSuke_leg_l.obj"	// 読み込むモデル名
#define	MODEL_ENEMY_BLUE_RLEG		"data/MODEL/BlueSuke/BlueSuke_leg_r.obj"	// 読み込むモデル名
#define	MODEL_ENEMY_BLUE_TAIL		"data/MODEL/BlueSuke/BlueSuke_tail.obj"		// 読み込むモデル名

#define	MODEL_ENEMY_GREEN			"data/MODEL/Midorin/Midorin_body.obj"		// 読み込むモデル名
#define	MODEL_ENEMY_GREEN_HEAD		"data/MODEL/Midorin/Midorin_head.obj"		// 読み込むモデル名
#define	MODEL_ENEMY_GREEN_LLEG		"data/MODEL/Midorin/Midorin_leg_l.obj"		// 読み込むモデル名
#define	MODEL_ENEMY_GREEN_RLEG		"data/MODEL/Midorin/Midorin_leg_r.obj"		// 読み込むモデル名
#define	MODEL_ENEMY_GREEN_TAIL		"data/MODEL/Midorin/Midorin_tail.obj"		// 読み込むモデル名

#define	MODEL_ENEMY_YELLOW			"data/MODEL/Kiichan/Kiichan_body.obj"		// 読み込むモデル名
#define	MODEL_ENEMY_YELLOW_HEAD		"data/MODEL/Kiichan/Kiichan_head.obj"		// 読み込むモデル名
#define	MODEL_ENEMY_YELLOW_LLEG		"data/MODEL/Kiichan/Kiichan_leg_l.obj"		// 読み込むモデル名
#define	MODEL_ENEMY_YELLOW_RLEG		"data/MODEL/Kiichan/Kiichan_leg_r.obj"		// 読み込むモデル名
#define	MODEL_ENEMY_YELLOW_TAIL		"data/MODEL/Kiichan/Kiichan_tail.obj"		// 読み込むモデル名




//--------------------------------------------------------------------------------------
// キーフレーム変更部分
// アニメーションのディレクトリ
// 待機状態
#define ANIM_IDLE_BODY			"data/ANIMDATA/ENEMY/IDLE/RED_BODY.csv"
#define ANIM_IDLE_HEAD			"data/ANIMDATA/ENEMY/IDLE/RED_HEAD.csv"
#define ANIM_IDLE_LLEG			"data/ANIMDATA/ENEMY/IDLE/RED_LEG_L.csv"
#define ANIM_IDLE_RLEG			"data/ANIMDATA/ENEMY/IDLE/RED_LEG_R.csv"
#define ANIM_IDLE_TAIL			"data/ANIMDATA/ENEMY/IDLE/RED_TAIL.csv"
// 移動状態
#define ANIM_WALK_BODY			"data/ANIMDATA/ENEMY/WALK/ENEMY_BODY.csv"
#define ANIM_WALK_HEAD			"data/ANIMDATA/ENEMY/WALK/ENEMY_HEAD.csv"
#define ANIM_WALK_LLEG			"data/ANIMDATA/ENEMY/WALK/ENEMY_LEG_L.csv"
#define ANIM_WALK_RLEG			"data/ANIMDATA/ENEMY/WALK/ENEMY_LEG_R.csv"
#define ANIM_WALK_TAIL			"data/ANIMDATA/ENEMY/WALK/ENEMY_TAIL.csv"
//攻撃状態
#define ANIM_HINOKO_BODY		"data/ANIMDATA/ENEMY/HINOKO/RED_BODY.csv"
#define ANIM_HINOKO_HEAD		"data/ANIMDATA/ENEMY/HINOKO/RED_HEAD.csv"
#define ANIM_HINOKO_LLEG		"data/ANIMDATA/ENEMY/HINOKO/RED_LEG_L.csv"
#define ANIM_HINOKO_RLEG		"data/ANIMDATA/ENEMY/HINOKO/RED_LEG_R.csv"
#define ANIM_HINOKO_TAIL		"data/ANIMDATA/ENEMY/HINOKO/RED_TAIL.csv"
// ↑追加したい時はここに追加
//--------------------------------------------------------------------------------------

enum TABLE_LIST
{
	IDLE_BODY,
	IDLE_HEAD,
	IDLE_LEG_L,
	IDLE_LEG_R,
	IDLE_TAIL,

	WALK_BODY,
	WALK_HEAD,
	WALK_LEG_L,
	WALK_LEG_R,
	WALK_TAIL,

	HINOKO_BODY,
	HINOKO_HEAD,
	HINOKO_LEG_L,
	HINOKO_LEG_R,
	HINOKO_TAIL,

	TABLE_MAX

};


#define ENEMY_PARTS_MAX				(5)										// エネミーのパーツの数
#define ENEMY_PARTS_MODEL_MAX		(MAX_ENEMY_TYPE * ENEMY_PARTS_MAX)		// 合計のパーツの数


#define	VALUE_MOVE					(5.0f)						// 移動量
#define	VALUE_ROTATE				(XM_PI * 0.02f)				// 回転量

#define ENEMY_SHADOW_SIZE			(0.4f)						// 影の大きさ
#define ENEMY_OFFSET_Y				(11.0f)						// エネミーの足元をあわせる

#define BURN_TIME_END				(50)						// 火状態が終わる時間
#define WET_TIME_END				(100)						// 水状態が終わる時間
#define WINDY_TIME_END				(50)						// 風状態が終わる時間
#define SHOCK_TIME_END				(100)						// 雷状態が終わる時間



//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

void WalkPlayer(void);
void HinokoPlayer(void);
void SetTblIdle(void);
void SetTblMove(void);
void SetTblHinoko(void);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static ENEMY			g_Enemy[MAX_ENEMY];							// エネミー

static ENEMY			g_Parts[MAX_ENEMY][ENEMY_PARTS_MAX];		// エネミーのパーツ用

static ENEMY_MODEL		g_PartsModel[MAX_ENEMY_TYPE * ENEMY_PARTS_MAX];	//エネミーの種類＊パーツ数


static char		g_ModelName[MAX_ENEMY_TYPE * ENEMY_PARTS_MAX][256] = 
{
	MODEL_ENEMY_RED,
	MODEL_ENEMY_RED_HEAD,
	MODEL_ENEMY_RED_LLEG,
	MODEL_ENEMY_RED_RLEG,
	MODEL_ENEMY_RED_TAIL,

	MODEL_ENEMY_BLUE,
	MODEL_ENEMY_BLUE_HEAD,
	MODEL_ENEMY_BLUE_LLEG,
	MODEL_ENEMY_BLUE_RLEG,
	MODEL_ENEMY_BLUE_TAIL,

	MODEL_ENEMY_GREEN,
	MODEL_ENEMY_GREEN_HEAD,
	MODEL_ENEMY_GREEN_LLEG,
	MODEL_ENEMY_GREEN_RLEG,
	MODEL_ENEMY_GREEN_TAIL,

	MODEL_ENEMY_YELLOW,
	MODEL_ENEMY_YELLOW_HEAD,
	MODEL_ENEMY_YELLOW_LLEG,
	MODEL_ENEMY_YELLOW_RLEG,
	MODEL_ENEMY_YELLOW_TAIL,

};

static int		g_EnemyCnt = MAX_ENEMY;
static int		spawncnt;				//スポーンカウント

//--------------------------------------------------------------------------------------
// キーフレーム変更部分
// アニメーションのディレクトリの配列
static char* g_AnimDir[] =
{
	// 待機状態
	ANIM_IDLE_BODY,
	ANIM_IDLE_HEAD,
	ANIM_IDLE_LLEG,
	ANIM_IDLE_RLEG,
	ANIM_IDLE_TAIL,
	// 移動状態
	ANIM_WALK_BODY,
	ANIM_WALK_HEAD,
	ANIM_WALK_LLEG,
	ANIM_WALK_RLEG,
	ANIM_WALK_TAIL,
	// 攻撃状態
	ANIM_HINOKO_BODY,
	ANIM_HINOKO_HEAD,
	ANIM_HINOKO_LLEG,
	ANIM_HINOKO_RLEG,
	ANIM_HINOKO_TAIL,

	// ↑追加したい時はここに追加
};
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// キーフレーム変更部分
// プレイヤーの階層アニメーションデータ
// 待機状態
static std::vector<INTERPOLATION_DATA>  idle_body;
static std::vector<INTERPOLATION_DATA>  idle_head;
static std::vector<INTERPOLATION_DATA>  idle_legl;
static std::vector<INTERPOLATION_DATA>  idle_legr;
static std::vector<INTERPOLATION_DATA>  idle_tail;
// 移動状態
static std::vector<INTERPOLATION_DATA>  walk_body;
static std::vector<INTERPOLATION_DATA>  walk_head;
static std::vector<INTERPOLATION_DATA>  walk_legl;
static std::vector<INTERPOLATION_DATA>  walk_legr;
static std::vector<INTERPOLATION_DATA>  walk_tail;
// 攻撃状態
static std::vector<INTERPOLATION_DATA>  hinoko_body;
static std::vector<INTERPOLATION_DATA>  hinoko_head;
static std::vector<INTERPOLATION_DATA>  hinoko_legl;
static std::vector<INTERPOLATION_DATA>  hinoko_legr;
static std::vector<INTERPOLATION_DATA>  hinoko_tail;
// ↑追加したい時はここに追加
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// キーフレーム変更部分
// プレイヤーの階層アニメーションデータの配列
static std::vector<INTERPOLATION_DATA>* g_TblAdr[] =
{
	&idle_body,
	&idle_head,
	&idle_legl,
	&idle_legr,
	&idle_tail,

	&walk_body,
	&walk_head,
	&walk_legl,
	&walk_legr,
	&walk_tail,

	&hinoko_body,
	&hinoko_head,
	&hinoko_legl,
	&hinoko_legr,
	&hinoko_tail,
	// ↑追加したい時はここに追加
};
//--------------------------------------------------------------------------------------

//歩く
// プレイヤーの左右パーツを動かしているアニメデータ
static INTERPOLATION_DATA move_tbl_body[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 5.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),    XMFLOAT3(1.0f, 1.0f, 1.0f), 30 },
	{ XMFLOAT3(0.0f, 3.0f, 0.2f), XMFLOAT3(0.0f, 0.0f, 0.0f),    XMFLOAT3(1.0f, 1.0f, 1.0f), 30 },

};

static INTERPOLATION_DATA move_tbl_head[] = {	// pos, rot, scl,frame
	{ XMFLOAT3(0.0f, 6.0f, 5.0f), XMFLOAT3(XM_PIDIV4 * 0.25f, 0.0f, 0.0f),   XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(0.0f, 6.0f, 5.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),   XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },

};

static INTERPOLATION_DATA move_tbl_lleg[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(-4.0f, -8.0f, 0.0f), XMFLOAT3(XM_PI / 4, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 30 },
	{ XMFLOAT3(-4.0f, -8.0f, 0.0f), XMFLOAT3(-XM_PI / 4, 0.0f, 0.0f),    XMFLOAT3(1.0f, 1.0f, 1.0f), 30 },

};

static INTERPOLATION_DATA move_tbl_rleg[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(4.0f, -8.0f, 0.0f), XMFLOAT3(-XM_PI / 4, 0.0f, 0.0f),    XMFLOAT3(1.0f, 1.0f, 1.0f), 30 },
	{ XMFLOAT3(4.0f, -8.0f, 0.0f), XMFLOAT3(XM_PI / 4, 0.0f, 0.0f),    XMFLOAT3(1.0f, 1.0f, 1.0f), 30 },

};

static INTERPOLATION_DATA move_tbl_tail[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, -3.0f, -6.0f), XMFLOAT3(-0.2f,  XM_PIDIV4,  XM_PIDIV4),   XMFLOAT3(1.0f, 1.0f, 1.0f), 30 },
	{ XMFLOAT3(0.0f, -3.0f, -6.0f), XMFLOAT3(-0.2f, -XM_PIDIV4, -XM_PIDIV4),   XMFLOAT3(1.0f, 1.0f, 1.0f), 30 },

};

static INTERPOLATION_DATA* g_AnimTblAdr[] =
{
	move_tbl_body,
	move_tbl_head,
	move_tbl_lleg,
	move_tbl_rleg,
	move_tbl_tail,

};

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitEnemy(void)
{
	//--------------------------------------------------------------------------------------
// キーフレーム変更部分
// アニメーションデータをファイルから読み込む 
	for (int i = 0; i < TABLE_MAX; i++)
	{
		LoadAnimDataCSV(g_AnimDir[i], *g_TblAdr[i]);
	}
	//--------------------------------------------------------------------------------------

	//	データの読み込み
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		g_Enemy[i].pos = XMFLOAT3(0.0f, ENEMY_OFFSET_Y, 0.0f);
		g_Enemy[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Enemy[i].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);

		g_Enemy[i].spd  = 0.0f;						// 移動スピードクリア
		g_Enemy[i].size = ENEMY_SIZE;				// 当たり判定の大きさ

		g_Enemy[i].hp = ENEMY_HP_MAX;				// HPの最大量
		g_Enemy[i].damage = ENEMY_DAMAGE_VALUE;		// エネミーが与えるダメージ量
		

		g_Enemy[i].use = FALSE;						// TRUE:生きてる
		g_Enemy[i].EntryTime = FALSE;				// 登場シーン
		g_Enemy[i].LeaveTime = FALSE;				// 退場シーン

		g_Enemy[i].burnTime  = 0.0f;				// 火状態時間
		g_Enemy[i].wetTime   = 0.0f;				// 水状態時間
		g_Enemy[i].windyTime = 0.0f;				// 風状態時間
		g_Enemy[i].shockTime = 0.0f;				// 雷状態時間

		g_Enemy[i].time = 0.0f;						// 線形補間用のタイマーをクリア
		g_Enemy[i].tblNo = 0;						// 再生する行動データテーブルNoをセット
		g_Enemy[i].tblMax = 0;						// 再生する行動データテーブルのレコード数をセット

		g_Enemy[i].parent = NULL;					// 本体（親）なのでNULLを入れる

		g_Enemy[i].state = ENEMY_ABNORMAL_NONE;

		g_Enemy[i].modelIdx = NULL;					// モデル番号


		g_Enemy[i].shadowIdx = -1;					// ここでは影は作らない

		g_Enemy[i].dissolve.Enable = TRUE;			// ディゾルブのフラグ

		g_Enemy[i].dissolve.threshold = 1.0f;		// しきい値

	}

	//	モデルの読み込み
	for (int i = 0; i < MAX_ENEMY_TYPE * ENEMY_PARTS_MAX; i++)
	{
		LoadModel(g_ModelName[i], &g_PartsModel[i].model);
		g_PartsModel[i].load = TRUE;
	}


	g_EnemyCnt = 0;
	spawncnt = 0;

	// 階層アニメーションの初期化
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		for (int j = 0; j < ENEMY_PARTS_MAX; j++)
		{ 
			g_Parts[i][j].use = FALSE;

			// 位置・回転・スケールの初期設定
			g_Parts[i][j].pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
			g_Parts[i][j].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
			g_Parts[i][j].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);

			// 親子関係
			g_Parts[i][j].parent = &g_Enemy[i];		// ← ここに親のアドレスを入れる

			// 階層アニメーション用のメンバー変数の初期化
			g_Parts[i][j].time = 0.0f;			// 線形補間用のタイマーをクリア
			g_Parts[i][j].tblNo = 0;			// 再生する行動データテーブルNoをセット
			g_Parts[i][j].tblMax = 0;			// 再生する行動データテーブルのレコード数をセット

			g_Parts[i][j].modelIdx = NULL;		// 番号
		}
	}




	for (int i = 0; i < MAX_ENEMY; i++)
	{
		g_Parts[i][0].use = TRUE;
		g_Parts[i][0].tblNo = WALK_BODY;			// 再生するアニメデータの先頭アドレスをセット
		g_Parts[i][0].tblMax = (int)g_TblAdr[g_Parts[i][0].tblNo]->size();	// ← 変更部分

		g_Parts[i][1].use = TRUE;
		g_Parts[i][1].parent = &g_Parts[i][0];	// 親をセット
		g_Parts[i][1].tblNo = WALK_HEAD;			// 再生するアニメデータの先頭アドレスをセット
		g_Parts[i][1].tblMax = (int)g_TblAdr[g_Parts[i][1].tblNo]->size();	// ← 変更部分

		g_Parts[i][2].use = TRUE;
		g_Parts[i][2].parent = &g_Parts[i][0];	// 親をセット
		g_Parts[i][2].tblNo = WALK_LEG_L;			// 再生するアニメデータの先頭アドレスをセット
		g_Parts[i][2].tblMax = (int)g_TblAdr[g_Parts[i][2].tblNo]->size();	// ← 変更部分

		g_Parts[i][3].use = TRUE;
		g_Parts[i][3].parent = &g_Parts[i][0];	// 親をセット
		g_Parts[i][3].tblNo = WALK_LEG_R;			// 再生するアニメデータの先頭アドレスをセット
		g_Parts[i][3].tblMax = (int)g_TblAdr[g_Parts[i][3].tblNo]->size();	// ← 変更部分

		g_Parts[i][4].use = TRUE;
		g_Parts[i][4].parent = &g_Parts[i][0];	// 親をセット
		g_Parts[i][4].tblNo = WALK_LEG_R;			// 再生するアニメデータの先頭アドレスをセット
		g_Parts[i][4].tblMax = (int)g_TblAdr[g_Parts[i][4].tblNo]->size();	// ← 変更部分
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitEnemy(void)
{	
	for (int i = 0; i < MAX_ENEMY_TYPE * ENEMY_PARTS_MAX; i++)
	{
		if (g_PartsModel[i].load)
		{
			UnloadModel(&g_PartsModel[i].model);
			g_PartsModel[i].load = FALSE;
		}
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateEnemy(void)
{
	PLAYER* player = GetPlayer();
	BULLET_CHARGE* bullet_charge = GetBullet_Charge();
	UI_ENEMY* Ui_Enemy = GetUI_Enemy();


	g_EnemyCnt = 0;	// 生きてるエネミーの数

	spawncnt += (10);

	if (spawncnt >= 1000 && GetFlag() == FLAG_GAME_BATTLE)
	{
		SetEnemy();
		spawncnt = 0;
	}


	for (int i = 0; i < MAX_ENEMY; i++)
	{
		if (g_Enemy[i].use == TRUE)		// このエネミーが使われている？
		{								// Yes
			XMFLOAT3 OldPos = g_Enemy[i].pos;

			// 出てくるとき
			if (g_Enemy[i].EntryTime == TRUE)
			{
				g_Enemy[i].dissolve.threshold -= 0.01f;

				if (g_Enemy[i].dissolve.threshold <= 0.0f)
				{
					g_Enemy[i].EntryTime = FALSE;
				}
			}
			// 消えるとき
			if (g_Enemy[i].LeaveTime == TRUE)
			{
				Ui_Enemy[i].use = FALSE;

				g_Enemy[i].dissolve.threshold += 0.01f;

				g_Enemy[i].pos.y += 0.3f;

				g_Enemy[i].scl.x -= 0.01f;
				g_Enemy[i].scl.y -= 0.01f;
				g_Enemy[i].scl.z -= 0.01f;

				if (g_Enemy[i].scl.x <= 0.0f)
				{
					g_Enemy[i].LeaveTime = FALSE;
					g_Enemy[i].use = FALSE;
				}
			}

			else // 竜巻発生時は竜巻に向ける  
			{
				if (bullet_charge->type == BULLET_CHARGE_GREEN && bullet_charge->bUse == TRUE)
				{
					XMVECTOR epos = XMLoadFloat3(&g_Enemy[i].pos);
					XMVECTOR vec = (XMLoadFloat3(&bullet_charge->pos) - epos);		//自分と竜巻との差分を求める
					float angle = atan2f(vec.m128_f32[2], vec.m128_f32[0]);			//その差分を使って角度を求めている

					if (g_Enemy[i].EntryTime == FALSE && g_Enemy[i].LeaveTime == FALSE)
					{
						g_Enemy[i].pos.x += cosf(angle) * g_Enemy[i].spd;					//angleの方向へ移動
						g_Enemy[i].pos.z += sinf(angle) * g_Enemy[i].spd;					//angleの方向へ移動
					}

				}
				else
				{
					XMVECTOR epos = XMLoadFloat3(&g_Enemy[i].pos);
					XMVECTOR vec = (XMLoadFloat3(&player[0].pos) - epos);		//自分とプレイヤーとの差分を求める
					float angle = atan2f(vec.m128_f32[2], vec.m128_f32[0]);		//その差分を使って角度を求めている

					if (g_Enemy[i].EntryTime == FALSE && g_Enemy[i].LeaveTime == FALSE)
					{
						g_Enemy[i].pos.x += cosf(angle) * g_Enemy[i].spd;					//angleの方向へ移動
						g_Enemy[i].pos.z += sinf(angle) * g_Enemy[i].spd;					//angleの方向へ移動
					}

					//相手と自分の差分を求める
					//差分を使って角度を求める(atan2f(アークタンジェント))
					//求めた角度へ一歩近づける

					g_Enemy[i].rot.y = (angle + XM_PI / 2) * -1;	//ホーミング方向へ回転
				}

				// 階層アニメーション
				for (int j = 0; j < ENEMY_PARTS_MAX; j++)
				{
					// 使われているなら処理する
					if ((g_Parts[i][j].use == TRUE) && (g_Parts[i][j].tblMax > 0))
					{	// 線形補間の処理
						int nowNo = (int)g_Parts[i][j].time;			// 整数分であるテーブル番号を取り出している
						int maxNo = g_Parts[i][j].tblMax;				// 登録テーブル数を数えている
						int nextNo = (nowNo + 1) % maxNo;			// 移動先テーブルの番号を求めている

						//--------------------------------------------------------------------------------------
						// キーフレーム変更部分
						//INTERPOLATION_DATA* tbl = g_AnimTblAdr[g_Parts[i].tblNo];	// 行動テーブルのアドレスを取得
						std::vector<INTERPOLATION_DATA> tbl = *g_TblAdr[g_Parts[i][j].tblNo];
						//--------------------------------------------------------------------------------------

						XMVECTOR nowPos = XMLoadFloat3(&tbl[nowNo].pos);	// XMVECTORへ変換
						XMVECTOR nowRot = XMLoadFloat3(&tbl[nowNo].rot);	// XMVECTORへ変換
						XMVECTOR nowScl = XMLoadFloat3(&tbl[nowNo].scl);	// XMVECTORへ変換

						XMVECTOR Pos = XMLoadFloat3(&tbl[nextNo].pos) - nowPos;	// XYZ移動量を計算している
						XMVECTOR Rot = XMLoadFloat3(&tbl[nextNo].rot) - nowRot;	// XYZ回転量を計算している
						XMVECTOR Scl = XMLoadFloat3(&tbl[nextNo].scl) - nowScl;	// XYZ拡大率を計算している

						float nowTime = g_Parts[i][j].time - nowNo;	// 時間部分である少数を取り出している

						Pos *= nowTime;								// 現在の移動量を計算している
						Rot *= nowTime;								// 現在の回転量を計算している
						Scl *= nowTime;								// 現在の拡大率を計算している

						// 計算して求めた移動量を現在の移動テーブルXYZに足している＝表示座標を求めている
						XMStoreFloat3(&g_Parts[i][j].pos, nowPos + Pos);

						// 計算して求めた回転量を現在の移動テーブルに足している
						XMStoreFloat3(&g_Parts[i][j].rot, nowRot + Rot);

						// 計算して求めた拡大率を現在の移動テーブルに足している
						XMStoreFloat3(&g_Parts[i][j].scl, nowScl + Scl);

						// frameを使て時間経過処理をする
						g_Parts[i][j].time += 1.0f / tbl[nowNo].frame;	// 時間を進めている
						if ((int)g_Parts[i][j].time >= maxNo)			// 登録テーブル最後まで移動したか？
						{
							g_Parts[i][j].time -= maxNo;				// ０番目にリセットしつつも小数部分を引き継いでいる
						}
					}
				}
			}

			// 状態異常処理
			{	
				//火
				if (g_Enemy[i].state == ENEMY_ABNORMAL_BURN)
				{
					g_Enemy[i].burnTime++;
					g_Enemy[i].hp--;

					if (g_Enemy[i].burnTime >= BURN_TIME_END)
					{
						g_Enemy[i].state = ENEMY_ABNORMAL_NONE;
						g_Enemy[i].burnTime = 0;

					}
				}

				// 水
				if (g_Enemy[i].state == ENEMY_ABNORMAL_WET)
				{
					g_Enemy[i].wetTime++;

					switch (g_Enemy[i].type)
					{
					case ENEMY_RED:
						g_Enemy[i].spd = (float)ENEMY_RED_SPD / 2;

						break;

					case ENEMY_BLUE:
						g_Enemy[i].spd = (float)ENEMY_BLUE_SPD / 2;

						break;

					case ENEMY_GREEN:
						g_Enemy[i].spd = (float)ENEMY_GREEN_SPD / 2;

						break;

					case ENEMY_YELLOW:
						g_Enemy[i].spd = (float)ENEMY_YELLOW_SPD / 2;

						break;
					}

					if (g_Enemy[i].wetTime >= WET_TIME_END)
					{
						g_Enemy[i].state = ENEMY_ABNORMAL_NONE;

						g_Enemy[i].wetTime = 0;
					}

				}

			}

			// 影もプレイヤーの位置に合わせる
			XMFLOAT3 pos = g_Enemy[i].pos;
			pos.y -= (ENEMY_OFFSET_Y - 0.1f);
			SetPositionShadow(g_Enemy[i].shadowIdx, pos);

			for (int j = 0; j < MAX_ENEMY; j++)
			{
				if (i == j) continue;


				// 敵同士の当たり判定
				float tx = g_Enemy[i].pos.x;
				float tz = g_Enemy[i].pos.z;

				XMFLOAT3 EnemyLowerPos = XMFLOAT3(g_Enemy[i].pos.x, g_Enemy[i].pos.y - 3.0f, g_Enemy[i].pos.z);
				XMFLOAT3 EnemyUpperPos = XMFLOAT3(g_Enemy[i].pos.x, g_Enemy[i].pos.y + 3.0f, g_Enemy[i].pos.z);

				//カプセルの当たり判定
				if (CapsuleCollision(EnemyUpperPos, EnemyLowerPos, g_Enemy[j].pos, g_Enemy[i].size, g_Enemy[j].size))
				{
					g_Enemy[i].pos.x = OldPos.x;
					g_Enemy[i].pos.z = OldPos.z;
				}
			}

			if (g_Enemy[i].hp <= 0)
			{
				// dissolve開始
				g_Enemy[i].LeaveTime = TRUE;
				ReleaseShadow(g_Enemy[i].shadowIdx);
			}
		}


	}

#ifdef _DEBUG
	// デバッグ表示
	PrintDebugProc("LeaveTime %d \n", g_Enemy[0].LeaveTime);
	PrintDebugProc("Enemy[0].hp %d \n", g_Enemy[0].hp);



#endif

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawEnemy(void)
{
	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;
	// カリング無効
	SetCullingMode(CULL_MODE_NONE);

	for (int i = 0; i < MAX_ENEMY; i++)
	{
		// dissolve
		SetDissolve(&g_Enemy[i].dissolve);
		SetAlphaTestEnable(g_Enemy[i].dissolve.Enable);

		if (g_Enemy[i].use == FALSE) continue;

		// ワールドマトリックスの初期化
		mtxWorld = XMMatrixIdentity();

		// スケールを反映
		mtxScl = XMMatrixScaling(g_Enemy[i].scl.x, g_Enemy[i].scl.y, g_Enemy[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// 回転を反映
		mtxRot = XMMatrixRotationRollPitchYaw(g_Enemy[i].rot.x, g_Enemy[i].rot.y + XM_PI, g_Enemy[i].rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// 移動を反映
		mtxTranslate = XMMatrixTranslation(g_Enemy[i].pos.x, g_Enemy[i].pos.y, g_Enemy[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ワールドマトリックスの設定
		SetWorldMatrix(&mtxWorld);

		XMStoreFloat4x4(&g_Enemy[i].mtxWorld, mtxWorld);


		// 階層アニメーション
		// Parts描画
		for (int j = 0; j < ENEMY_PARTS_MAX; j++)
		{
			// ワールドマトリックスの初期化
			mtxWorld = XMMatrixIdentity();

			// スケールを反映
			mtxScl = XMMatrixScaling(g_Parts[i][j].scl.x, g_Parts[i][j].scl.y, g_Parts[i][j].scl.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

			// 回転を反映
			mtxRot = XMMatrixRotationRollPitchYaw(g_Parts[i][j].rot.x, g_Parts[i][j].rot.y, g_Parts[i][j].rot.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

			// 移動を反映
			mtxTranslate = XMMatrixTranslation(g_Parts[i][j].pos.x, g_Parts[i][j].pos.y, g_Parts[i][j].pos.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

			if (g_Parts[i][j].parent != NULL)	// 子供だったら親と結合する
			{
				mtxWorld = XMMatrixMultiply(mtxWorld, XMLoadFloat4x4(&g_Parts[i][j].parent->mtxWorld));
				// ↑
				// g_Enemy.mtxWorldを指している
			}

			XMStoreFloat4x4(&g_Parts[i][j].mtxWorld, mtxWorld);

			// 使われているなら処理する
			if (g_Parts[i][j].use == FALSE) continue;

			// ワールドマトリックスの設定
			SetWorldMatrix(&mtxWorld);

			// モデル描画
			DrawModel(&g_PartsModel[g_Parts[i][j].modelIdx].model);
		}

		// dissolve
		SetDissolveEnable(FALSE);
		SetAlphaTestEnable(g_Enemy[i].dissolve.Enable);

	}



	// カリング設定を戻す
	SetCullingMode(CULL_MODE_BACK);
}

//=============================================================================
// エネミーの取得
//=============================================================================
ENEMY *GetEnemy()
{
	return &g_Enemy[0];
}


void SetEnemy(void)
{
	int start = rand() % MAX_ENEMY;

	for (int n = start; n < MAX_ENEMY + start; n++)
	{
		int i = n % MAX_ENEMY;

		if (g_Enemy[i].use == FALSE)        // 未使用状態のenemyを見つける
		{
			g_Enemy[i].type = rand() % ENEMY_COLOR_MAX;	//色の変更
			g_Enemy[i].state = ENEMY_ABNORMAL_NONE;

			ChangeModelEnemy(g_Enemy[i].type, i);

			XMFLOAT3 pos;

			pos.x = (float)(rand() % ENEMY_SPAWN_RANGE_X) - (ENEMY_SPAWN_RANGE_X * 0.5f);
			pos.y = ENEMY_OFFSET_Y;
			pos.z = (float)(rand() % ENEMY_SPAWN_RANGE_Z) - (ENEMY_SPAWN_RANGE_Z * 0.5f);

			g_Enemy[i].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);

			g_Enemy[i].use = TRUE;           // 使用状態へ変更する
			g_Enemy[i].EntryTime = TRUE;

			g_Enemy[i].pos = pos;            // 座標をセット

			g_Enemy[i].hp = ENEMY_HP_MAX;            // 

			pos.y -= (ENEMY_OFFSET_Y - 0.1f);
			g_Enemy[i].shadowIdx = CreateShadow(pos, ENEMY_SHADOW_SIZE, ENEMY_SHADOW_SIZE);


			XMFLOAT4 color = { 0.0f,0.0f,0.0f,0.0f };

			switch (g_Enemy[i].type)
			{
			case ENEMY_RED:
				SetParticle(g_Enemy[i].pos, XMFLOAT3(2.0f, 2.0f, 2.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f),
					TYPE_CHANGE_PLAYER, BLEND_MODE_SUBTRACT, 0.0f, 0.0f, 90);
				color = { 1.0f, 0.0f, 0.0f, 1.0f };
				g_Enemy[i].spd = (float)ENEMY_RED_SPD;

				break;

			case ENEMY_BLUE:
				SetParticle(g_Enemy[i].pos, XMFLOAT3(2.0f, 2.0f, 2.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.9f, 0.5f, 0.0f, 1.0f),
					TYPE_CHANGE_PLAYER, BLEND_MODE_SUBTRACT, 0.0f, 0.0f, 90);
				color = { 0.0f, 0.0f, 1.0f, 1.0f };
				g_Enemy[i].spd = (float)ENEMY_BLUE_SPD;

				break;

			case ENEMY_GREEN:
				SetParticle(g_Enemy[i].pos, XMFLOAT3(2.0f, 2.0f, 2.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.9f, 0.0f, 0.9f, 1.0f),
					TYPE_CHANGE_PLAYER, BLEND_MODE_SUBTRACT, 0.0f, 0.0f, 90);
				color = { 0.0f, 1.0f, 0.0f, 1.0f };
				g_Enemy[i].spd = (float)ENEMY_GREEN_SPD;

				break;

			case ENEMY_YELLOW:
				SetParticle(g_Enemy[i].pos, XMFLOAT3(2.0f, 2.0f, 2.0f), XMFLOAT3(0.0f, 5.0f, 0.0f), XMFLOAT4(0.0f, 0.0f, 0.9f, 1.0f),
					TYPE_CHANGE_PLAYER, BLEND_MODE_SUBTRACT, 0.0f, 0.0f, 90);
				color = { 1.0f, 1.0f, 0.0f, 1.0f };
				g_Enemy[i].spd = (float)ENEMY_YELLOW_SPD;

				break;
			}

			g_Enemy[i].dissolve.DissolveColor = color;

			return;                            // 1発セットしたので終了する
		}
	}
	
}

void ChangeModelEnemy(int type, int id)
{
	// 番号の振り分け
	switch (type)
	{
	case ENEMY_RED:
		g_Parts[id][0].modelIdx = 0;
		g_Parts[id][1].modelIdx = 1;
		g_Parts[id][2].modelIdx = 2;
		g_Parts[id][3].modelIdx = 3;
		g_Parts[id][4].modelIdx = 4;
		g_Enemy[id].type = ENEMY_RED;
		break;

	case ENEMY_BLUE:
		g_Parts[id][0].modelIdx = 5;
		g_Parts[id][1].modelIdx = 6;
		g_Parts[id][2].modelIdx = 7;
		g_Parts[id][3].modelIdx = 8;
		g_Parts[id][4].modelIdx = 9;
		g_Enemy[id].type = ENEMY_BLUE;
		break;

	case ENEMY_GREEN:
		g_Parts[id][0].modelIdx = 10;
		g_Parts[id][1].modelIdx = 11;
		g_Parts[id][2].modelIdx = 12;
		g_Parts[id][3].modelIdx = 13;
		g_Parts[id][4].modelIdx = 14;
		g_Enemy[id].type = ENEMY_GREEN;
		break;

	case ENEMY_YELLOW:
		g_Parts[id][0].modelIdx = 15;
		g_Parts[id][1].modelIdx = 16;
		g_Parts[id][2].modelIdx = 17;
		g_Parts[id][3].modelIdx = 18;
		g_Parts[id][4].modelIdx = 19;
		g_Enemy[id].type = ENEMY_YELLOW;
		break;

	}
}