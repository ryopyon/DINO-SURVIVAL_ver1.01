//=============================================================================
//
// プレイヤー処理 [player.cpp]
// Author : HAL東京 高橋諒
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "light.h"
#include "input.h"
#include "camera.h"
#include "model.h"
#include "player.h"
#include "enemy.h"
#include "aim.h"
#include "shadow.h"
#include "bullet.h"
#include "bullet_charge.h"
#include "debugproc.h"
#include "meshfield.h"
#include "collision.h"
#include "score.h"
#include "sound.h"
#include "particle.h"
#include "particle_charge.h"
#include "flag.h"
#include "keyframe.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MODEL_PLAYER_RED			"data/MODEL/red/red_body.obj"			// 読み込むモデル名
#define	MODEL_PLAYER_RED_HEAD		"data/MODEL/red/red_head.obj"			// 読み込むモデル名
#define	MODEL_PLAYER_RED_LLEG		"data/MODEL/red/red_leg_l.obj"			// 読み込むモデル名
#define	MODEL_PLAYER_RED_RLEG		"data/MODEL/red/red_leg_r.obj"			// 読み込むモデル名
#define	MODEL_PLAYER_RED_TAIL		"data/MODEL/red/red_tail.obj"			// 読み込むモデル名

#define	MODEL_PLAYER_BLUE			"data/MODEL/blue/blue_body.obj"			// 読み込むモデル名
#define	MODEL_PLAYER_BLUE_HEAD		"data/MODEL/blue/blue_head.obj"			// 読み込むモデル名
#define	MODEL_PLAYER_BLUE_LLEG		"data/MODEL/blue/blue_leg_l.obj"		// 読み込むモデル名
#define	MODEL_PLAYER_BLUE_RLEG		"data/MODEL/blue/blue_leg_r.obj"		// 読み込むモデル名
#define	MODEL_PLAYER_BLUE_TAIL		"data/MODEL/blue/blue_tail.obj"			// 読み込むモデル名

#define	MODEL_PLAYER_GREEN			"data/MODEL/green/green_body.obj"		// 読み込むモデル名
#define	MODEL_PLAYER_GREEN_HEAD		"data/MODEL/green/green_head.obj"		// 読み込むモデル名
#define	MODEL_PLAYER_GREEN_LLEG		"data/MODEL/green/green_leg_l.obj"		// 読み込むモデル名
#define	MODEL_PLAYER_GREEN_RLEG		"data/MODEL/green/green_leg_r.obj"		// 読み込むモデル名
#define	MODEL_PLAYER_GREEN_TAIL		"data/MODEL/green/green_tail.obj"		// 読み込むモデル名

#define	MODEL_PLAYER_YELLOW			"data/MODEL/yellow/yellow_body.obj"		// 読み込むモデル名
#define	MODEL_PLAYER_YELLOW_HEAD	"data/MODEL/yellow/yellow_head.obj"		// 読み込むモデル名
#define	MODEL_PLAYER_YELLOW_LLEG	"data/MODEL/yellow/yellow_leg_l.obj"	// 読み込むモデル名
#define	MODEL_PLAYER_YELLOW_RLEG	"data/MODEL/yellow/yellow_leg_r.obj"	// 読み込むモデル名
#define	MODEL_PLAYER_YELLOW_TAIL	"data/MODEL/yellow/yellow_tail.obj"		// 読み込むモデル名

//--------------------------------------------------------------------------------------
// キーフレーム変更部分
// アニメーションのディレクトリ
// 待機状態
#define ANIM_IDLE_BODY			"data/ANIMDATA/PLAYER/IDLE/RED_BODY.csv"
#define ANIM_IDLE_HEAD			"data/ANIMDATA/PLAYER/IDLE/RED_HEAD.csv"
#define ANIM_IDLE_LLEG			"data/ANIMDATA/PLAYER/IDLE/RED_LEG_L.csv"
#define ANIM_IDLE_RLEG			"data/ANIMDATA/PLAYER/IDLE/RED_LEG_R.csv"
#define ANIM_IDLE_TAIL			"data/ANIMDATA/PLAYER/IDLE/RED_TAIL.csv"
// 移動状態
#define ANIM_WALK_BODY			"data/ANIMDATA/PLAYER/WALK/RED_BODY.csv"
#define ANIM_WALK_HEAD			"data/ANIMDATA/PLAYER/WALK/RED_HEAD.csv"
#define ANIM_WALK_LLEG			"data/ANIMDATA/PLAYER/WALK/RED_LEG_L.csv"
#define ANIM_WALK_RLEG			"data/ANIMDATA/PLAYER/WALK/RED_LEG_R.csv"
#define ANIM_WALK_TAIL			"data/ANIMDATA/PLAYER/WALK/RED_TAIL.csv"
//攻撃状態
#define ANIM_HINOKO_BODY		"data/ANIMDATA/PLAYER/HINOKO/RED_BODY.csv"
#define ANIM_HINOKO_HEAD		"data/ANIMDATA/PLAYER/HINOKO/RED_HEAD.csv"
#define ANIM_HINOKO_LLEG		"data/ANIMDATA/PLAYER/HINOKO/RED_LEG_L.csv"
#define ANIM_HINOKO_RLEG		"data/ANIMDATA/PLAYER/HINOKO/RED_LEG_R.csv"
#define ANIM_HINOKO_TAIL		"data/ANIMDATA/PLAYER/HINOKO/RED_TAIL.csv"
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

#define	VALUE_MOVE					(2.0f)							// 移動量
#define	VALUE_ROTATE				(D3DX_PI * 0.02f)				// 回転量

#define PLAYER_SHADOW_SIZE			(0.4f)							// 影の大きさ
#define PLAYER_OFFSET_Y				(8.0f)							// プレイヤーの足元をあわせる

#define PLAYER_PARTS_MAX			(5)								// プレイヤーのパーツの数
#define PLAYER_PARTS_MODEL_MAX		(MAX_PLAYER_TYPE * PLAYER_PARTS_MAX)		// 合計のパーツの数

#define PLAYER_PARTS_SIZE			(10.0f)							// プレイヤーのパーツの大きさ

#define HINOKO_COST					(15)							// ひのこのコスト
#define CHARGE_COST					(60)							// チャージコスト

#define CHANGE_MPGAIN				(40)							// プレイヤー交代時のMP回復量
#define CHANGE_COOLTIME				(60)							// プレイヤー交代のクールタイム

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void ChangeHinokoColor(void);
void hpGain(void);
void mpGain(void);
void AddMP(int amount);
void SubtractMP(int amount);

void InputMovePlayer(void);
void InputHinokoPlayer(void);
void InputCharge(void);
void WalkPlayer(void);
void HinokoPlayer(void);
void SetTblIdle(void);
void SetTblMove(void);
void SetTblHinoko(void);
void InputQuickTurn(void);

void RecastChangePlayer();

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static PLAYER			g_Player;						// プレイヤー

static PLAYER			g_Parts[MAX_PLAYER][PLAYER_PARTS_MAX];		// プレイヤーのパーツ用

static PLAYER_MODEL		g_PartsModel[MAX_PLAYER_TYPE * PLAYER_PARTS_MAX];	// プレイヤーの種類＊パーツ数

static char		g_ModelName[MAX_PLAYER_TYPE * PLAYER_PARTS_MAX][256] =
{
	MODEL_PLAYER_RED,
	MODEL_PLAYER_RED_HEAD,
	MODEL_PLAYER_RED_LLEG,
	MODEL_PLAYER_RED_RLEG,
	MODEL_PLAYER_RED_TAIL,

	MODEL_PLAYER_BLUE,
	MODEL_PLAYER_BLUE_HEAD,
	MODEL_PLAYER_BLUE_LLEG,
	MODEL_PLAYER_BLUE_RLEG,
	MODEL_PLAYER_BLUE_TAIL,

	MODEL_PLAYER_GREEN,
	MODEL_PLAYER_GREEN_HEAD,
	MODEL_PLAYER_GREEN_LLEG,
	MODEL_PLAYER_GREEN_RLEG,
	MODEL_PLAYER_GREEN_TAIL,

	MODEL_PLAYER_YELLOW,
	MODEL_PLAYER_YELLOW_HEAD,
	MODEL_PLAYER_YELLOW_LLEG,
	MODEL_PLAYER_YELLOW_RLEG,
	MODEL_PLAYER_YELLOW_TAIL,

};

static XMFLOAT4 color = { 0.0f, 0.0f, 0.0f, 1.0f };

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

static float		roty = 0.0f;
BOOL				soundF;
static LIGHT		g_Light;

int hinokoCnt;	// ひのこ中のフレームカウント


//当たり判定用
enum
{
	tbl_left,
	tbl_right,
};


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitPlayer(void)
{
	//--------------------------------------------------------------------------------------
// キーフレーム変更部分
// アニメーションデータをファイルから読み込む 
	for (int i = 0; i < TABLE_MAX; i++)
	{
		LoadAnimDataCSV(g_AnimDir[i], *g_TblAdr[i]);
	}
	//--------------------------------------------------------------------------------------

	g_Player.pos = XMFLOAT3(-100.0f, PLAYER_OFFSET_Y, -500.0f);
	g_Player.rot = XMFLOAT3(0.0f, XM_PI, 0.0f);
	g_Player.scl = XMFLOAT3(0.5f, 0.5f, 0.5f);

	g_Player.spd = 0.0f;										// 移動スピードクリア

	g_Player.hp = PLAYER_HP_MAX;								// HP最大値
	g_Player.mp = PLAYER_MP_MAX;								// MP最大値

	g_Player.use = TRUE;										// TRUE:生きてる
	g_Player.size = PLAYER_SIZE;								// 当たり判定の大きさ

	g_Player.hinokoColor = XMFLOAT4(1.0f, 0.5f, 0.1f, 0.85f);
	g_Player.chargeCnt = 0;										// チャージ時間
	g_Player.changeCnt = CHANGE_COOLTIME;						// プレイヤー交代クールタイム処理用のカウント

	g_Player.state = STATE_IDLE;								// 状態

	g_Player.modelIdx = NULL;									// モデル番号

	g_Player.dissolve.Enable = FALSE;							// ディゾルブ
	g_Player.dissolve.threshold = 0.0f;


	// ここでプレイヤー用の影を作成している
	XMFLOAT3 pos = g_Player.pos;
	pos.y -= (PLAYER_OFFSET_Y - 0.1f);
	g_Player.shadowIdx = CreateShadow(pos, PLAYER_SHADOW_SIZE, PLAYER_SHADOW_SIZE);

	roty = 0.0f;

	g_Player.parent = NULL;			// 本体（親）なのでNULLを入れる

	//	モデルの読み込み
	for (int i = 0; i < MAX_PLAYER_TYPE * PLAYER_PARTS_MAX; i++)
	{
		LoadModel(g_ModelName[i], &g_PartsModel[i].model);
		g_PartsModel[i].load = TRUE;
	}

	// 階層アニメーションの初期化
	for (int i = 0; i < MAX_PLAYER; i++)
	{
		for (int j = 0; j < PLAYER_PARTS_MAX; j++)
		{
			g_Parts[i][j].use = FALSE;

			// 位置・回転・スケールの初期設定
			g_Parts[i][j].pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
			g_Parts[i][j].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
			g_Parts[i][j].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);

			// 親子関係
			g_Parts[i][j].parent = &g_Player;		// ← ここに親のアドレスを入れる

			// 階層アニメーション用のメンバー変数の初期化
			g_Parts[i][j].time = 0.0f;			// 線形補間用のタイマーをクリア
			g_Parts[i][j].tblNo = 0;			// 再生する行動データテーブルNoをセット
			g_Parts[i][j].tblMax = 0;			// 再生する行動データテーブルのレコード数をセット

			g_Parts[i][j].modelIdx = NULL;		// 番号
		}
	}

	g_Parts[0][0].use = TRUE;
	g_Parts[0][0].tblNo = IDLE_BODY;			// 再生するアニメデータの先頭アドレスをセット
	g_Parts[0][0].tblMax = (int)g_TblAdr[g_Parts[0][0].tblNo]->size();	// ← 変更部分

	g_Parts[0][1].use = TRUE;
	g_Parts[0][1].parent = &g_Parts[0][0];	// 親をセット
	g_Parts[0][1].tblNo = IDLE_HEAD;			// 再生するアニメデータの先頭アドレスをセット
	g_Parts[0][1].tblMax = (int)g_TblAdr[g_Parts[0][1].tblNo]->size();	// ← 変更部分

	g_Parts[0][2].use = TRUE;
	g_Parts[0][2].parent = &g_Parts[0][0];	// 親をセット
	g_Parts[0][2].tblNo = IDLE_LEG_L;			// 再生するアニメデータの先頭アドレスをセット
	g_Parts[0][2].tblMax = (int)g_TblAdr[g_Parts[0][2].tblNo]->size();	// ← 変更部分

	g_Parts[0][3].use = TRUE;
	g_Parts[0][3].parent = &g_Parts[0][0];	// 親をセット
	g_Parts[0][3].tblNo = IDLE_LEG_R;			// 再生するアニメデータの先頭アドレスをセット
	g_Parts[0][3].tblMax = (int)g_TblAdr[g_Parts[0][3].tblNo]->size();	// ← 変更部分

	g_Parts[0][4].use = TRUE;
	g_Parts[0][4].parent = &g_Parts[0][0];	// 親をセット
	g_Parts[0][4].tblNo = IDLE_TAIL;			// 再生するアニメデータの先頭アドレスをセット
	g_Parts[0][4].tblMax = (int)g_TblAdr[g_Parts[0][4].tblNo]->size();	// ← 変更部分


	// モデルのディフューズを保存しておく。色変え対応の為。
	GetModelDiffuse(&g_PartsModel[0].model, &g_PartsModel[0].diffuse[0]);

	GetModelDiffuse(&g_PartsModel[0].model, &g_PartsModel[0].diffuse[0]);
	GetModelDiffuse(&g_PartsModel[1].model, &g_PartsModel[1].diffuse[0]);
	GetModelDiffuse(&g_PartsModel[2].model, &g_PartsModel[2].diffuse[0]);
	GetModelDiffuse(&g_PartsModel[3].model, &g_PartsModel[3].diffuse[0]);
	GetModelDiffuse(&g_PartsModel[4].model, &g_PartsModel[4].diffuse[0]);

	ChangeModelPlayer(PLAYER_RED,0);


	soundF = FALSE;

	hinokoCnt = 0;

	// クォータニオンの初期化
	XMStoreFloat4(&g_Player.Quaternion, XMQuaternionIdentity());

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitPlayer(void)
{
	for (int i = 0; i < MAX_PLAYER_TYPE * PLAYER_PARTS_MAX; i++)
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
void UpdatePlayer(void)
{
	CAMERA *cam = GetCamera();
	ENEMY* enemy = GetEnemy();
	BULLET* bullet = GetBullet();

	XMVECTOR vCameraAt = XMLoadFloat3(&cam->at);
	XMVECTOR vPlayerPos = XMLoadFloat3(&g_Player.pos);
	XMVECTOR vec = vPlayerPos - vCameraAt;
	float anglePlayer = atan2f(vec.m128_f32[0], vec.m128_f32[2]);

	g_Player.moving = FALSE;

	if (g_Player.state != STATE_DIE)
	{
		mpGain();
		//hpGain();
	}

	RecastChangePlayer();

	switch (g_Player.state)
	{
	case STATE_IDLE:
		InputMovePlayer();
		InputHinokoPlayer();
		InputCharge();
		InputQuickTurn();
		WalkPlayer();
		SetTblIdle();
		break;

	case STATE_WALK:
		InputMovePlayer();
		InputHinokoPlayer();
		InputCharge();
		InputQuickTurn();
		WalkPlayer();
		SetTblMove();
		break;

	case STATE_HINOKO:
		InputMovePlayer();
		InputHinokoPlayer();
		InputCharge();
		InputQuickTurn();
		WalkPlayer();
		HinokoPlayer();
		SetTblHinoko();
		break;

	default:
		break;
	}

	g_Player.spd *= 0.8f;
	g_Player.rot.y = anglePlayer;


	// レイキャストして足元の高さを求める
	XMFLOAT3 HitPosition;		// 交点
	XMFLOAT3 Normal;			// ぶつかったポリゴンの法線ベクトル（向き）
	BOOL ans = RayHitField(g_Player.pos, &HitPosition, &Normal);

	if (ans)
	{
		g_Player.pos.y = HitPosition.y + PLAYER_OFFSET_Y;
	}
	else
	{
		g_Player.pos.y = PLAYER_OFFSET_Y;
		Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	}

	// 攻撃してないときにモデルチェンジ
	if (g_Player.attack == FALSE && g_Player.changeCnt >= CHANGE_COOLTIME)
	{
		BOOL change = FALSE;	// モデルチェンジの判別用

		// モデルチェンジ処理１ ※上下シフトによる切り替え
		{
			if (GetKeyboardTrigger(DIK_O) || IsButtonTriggered(0, BUTTON_L2))
			{
				g_Player.type = (++g_Player.type + PLAYER_COLOR_MAX) % PLAYER_COLOR_MAX;
				change = TRUE;
			}
			else if (GetKeyboardTrigger(DIK_U) || IsButtonTriggered(0, BUTTON_L))
			{
				g_Player.type = (--g_Player.type + PLAYER_COLOR_MAX) % PLAYER_COLOR_MAX;
				change = TRUE;
			}
		}

		// モデルチェンジ処理２ ※指定切り替え
		{
			if ((GetKeyboardTrigger(DIK_1) || IsButtonTriggered(0, BUTTON_UP)) && g_Player.type != PLAYER_RED)
			{
				g_Player.type = PLAYER_RED;
				change = TRUE;
			}
			else if ((GetKeyboardTrigger(DIK_2) || IsButtonTriggered(0, BUTTON_RIGHT)) && g_Player.type != PLAYER_BLUE)
			{
				g_Player.type = PLAYER_BLUE;
				change = TRUE;
			}
			else if ((GetKeyboardTrigger(DIK_3) || IsButtonTriggered(0, BUTTON_DOWN)) && g_Player.type != PLAYER_GREEN)
			{
				g_Player.type = PLAYER_GREEN;
				change = TRUE;
			}
			else if ((GetKeyboardTrigger(DIK_4) || IsButtonTriggered(0, BUTTON_LEFT)) && g_Player.type != PLAYER_YELLOW)
			{
				g_Player.type = PLAYER_YELLOW;
				change = TRUE;
			}
		}

		// フラグがTRUEの場合、モデルチェンジ
		if (change == TRUE)
		{
			switch (g_Player.type)
			{
			case PLAYER_RED:
				ChangeModelPlayer(PLAYER_RED, 0);
				SetParticle(g_Player.pos, XMFLOAT3(2.0f, 2.0f, 2.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(1.0f, 0.5f, 0.1f, 0.85f),
					TYPE_CHANGE_PLAYER, BLEND_MODE_ADD, 0.0f, 0.0f, 30);
				break;

			case PLAYER_BLUE:
				ChangeModelPlayer(PLAYER_BLUE, 0);
				SetParticle(g_Player.pos, XMFLOAT3(2.0f, 2.0f, 2.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.1f, 0.5f, 1.0f, 0.85f),
					TYPE_CHANGE_PLAYER, BLEND_MODE_ADD, 0.0f, 0.0f, 30);
				break;

			case PLAYER_GREEN:
				ChangeModelPlayer(PLAYER_GREEN, 0);
				SetParticle(g_Player.pos, XMFLOAT3(2.0f, 2.0f, 2.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.1f, 1.0f, 0.1f, 0.85f),
					TYPE_CHANGE_PLAYER, BLEND_MODE_ADD, 0.0f, 0.0f, 30);
				break;

			case PLAYER_YELLOW:
				ChangeModelPlayer(PLAYER_YELLOW, 0);
				SetParticle(g_Player.pos, XMFLOAT3(2.0f, 2.0f, 2.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 0.1f, 0.85f),
					TYPE_CHANGE_PLAYER, BLEND_MODE_ADD, 0.0f, 0.0f, 30);
				break;
			}

			//g_Player.mp += CHANGE_MPGAIN;
			AddMP(CHANGE_MPGAIN);
			g_Player.changeCnt = 0;    // プレイヤーチェンジ後、リキャストカウントを0にする
			PlaySound(SOUND_LABEL_SE_CHANGE);
		}

		//if ((GetKeyboardTrigger(DIK_1) || IsButtonTriggered(0,BUTTON_UP)) && g_Player.type != PLAYER_RED)
		//{
		//	ChangeModelPlayer(PLAYER_RED,0);
		//	SetParticle(g_Player.pos, XMFLOAT3(2.0f, 2.0f, 2.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(1.0f, 0.5f, 0.1f, 0.85f),
		//		TYPE_CHANGE_PLAYER, BLEND_MODE_ADD, 0.0f, 0.0f, 30);
		//	PlaySound(SOUND_LABEL_SE_CHANGE);

		//	g_Player.mp += CHANGE_MPGAIN;
		//	g_Player.changeCnt = 0;    // プレイヤーチェンジ後、リキャストカウントを0にする
		//	g_Player.type = PLAYER_RED;
		//}

		//else if ((GetKeyboardTrigger(DIK_2) || IsButtonTriggered(0, BUTTON_RIGHT)) && g_Player.type != PLAYER_BLUE)
		//{
		//	ChangeModelPlayer(PLAYER_BLUE,0);
		//	SetParticle(g_Player.pos, XMFLOAT3(2.0f, 2.0f, 2.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.1f, 0.5f, 1.0f, 0.85f),
		//		TYPE_CHANGE_PLAYER, BLEND_MODE_ADD, 0.0f, 0.0f, 30);
		//	PlaySound(SOUND_LABEL_SE_CHANGE);

		//	g_Player.mp += CHANGE_MPGAIN;
		//	g_Player.changeCnt = 0;    // プレイヤーチェンジ後、リキャストカウントを0にする
		//	g_Player.type = PLAYER_BLUE;
		//}

		//else if ((GetKeyboardTrigger(DIK_3) || IsButtonTriggered(0, BUTTON_DOWN)) && g_Player.type != PLAYER_GREEN)
		//{
		//	ChangeModelPlayer(PLAYER_GREEN,0);
		//	SetParticle(g_Player.pos, XMFLOAT3(2.0f, 2.0f, 2.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.1f, 1.0f, 0.1f, 0.85f),
		//		TYPE_CHANGE_PLAYER, BLEND_MODE_ADD, 0.0f, 0.0f, 30);
		//	PlaySound(SOUND_LABEL_SE_CHANGE);

		//	g_Player.mp += CHANGE_MPGAIN;
		//	g_Player.changeCnt = 0;    // プレイヤーチェンジ後、リキャストカウントを0にする
		//	g_Player.type = PLAYER_GREEN;
		//}

		//else if ((GetKeyboardTrigger(DIK_4) || IsButtonTriggered(0, BUTTON_LEFT)) && g_Player.type != PLAYER_YELLOW)
		//{
		//	ChangeModelPlayer(PLAYER_YELLOW,0);
		//	SetParticle(g_Player.pos, XMFLOAT3(2.0f, 2.0f, 2.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 0.1f, 0.85f),
		//		TYPE_CHANGE_PLAYER, BLEND_MODE_ADD, 0.0f, 0.0f, 30);
		//	PlaySound(SOUND_LABEL_SE_CHANGE);

		//	g_Player.mp += CHANGE_MPGAIN;
		//	g_Player.changeCnt = 0;    // プレイヤーチェンジ後、リキャストカウントを0にする
		//	g_Player.type = PLAYER_YELLOW;
		//}
	}

	ChangeHinokoColor();	// 色変えのタイミング

	hinokoCnt++;

	if (hinokoCnt > 60)
	{
		g_Player.attack = FALSE;
		hinokoCnt = 0;
	}

	if (g_Player.state != STATE_DIE)
	{
		//何もしてないときはアイドル状態に戻す
		if ((g_Player.moving == FALSE) && (g_Player.attack == FALSE))
		{
			g_Player.state = STATE_IDLE;
		}
	}

	// 影もプレイヤーの位置に合わせる
	XMFLOAT3 pos = g_Player.pos;
	pos.y -= (PLAYER_OFFSET_Y - 0.1f);
	SetPositionShadow(g_Player.shadowIdx, pos);

	// 階層アニメーション
	for (int i = 0; i < PLAYER_PARTS_MAX; i++)
	{
		// 使われているなら処理する
		if ((g_Parts[0][i].use == TRUE) && (g_Parts[0][i].tblMax > 0))
		{	// 線形補間の処理
			int nowNo = (int)g_Parts[0][i].time;			// 整数分であるテーブル番号を取り出している
			int maxNo = g_Parts[0][i].tblMax;				// 登録テーブル数を数えている
			int nextNo = (nowNo + 1) % maxNo;			// 移動先テーブルの番号を求めている

			//--------------------------------------------------------------------------------------
			// キーフレーム変更部分
			//INTERPOLATION_DATA* tbl = g_AnimTblAdr[g_Parts[i].tblNo];	// 行動テーブルのアドレスを取得
			std::vector<INTERPOLATION_DATA> tbl = *g_TblAdr[g_Parts[0][i].tblNo];
			//--------------------------------------------------------------------------------------

			XMVECTOR nowPos = XMLoadFloat3(&tbl[nowNo].pos);	// XMVECTORへ変換
			XMVECTOR nowRot = XMLoadFloat3(&tbl[nowNo].rot);	// XMVECTORへ変換
			XMVECTOR nowScl = XMLoadFloat3(&tbl[nowNo].scl);	// XMVECTORへ変換

			XMVECTOR Pos = XMLoadFloat3(&tbl[nextNo].pos) - nowPos;	// XYZ移動量を計算している
			XMVECTOR Rot = XMLoadFloat3(&tbl[nextNo].rot) - nowRot;	// XYZ回転量を計算している
			XMVECTOR Scl = XMLoadFloat3(&tbl[nextNo].scl) - nowScl;	// XYZ拡大率を計算している

			float nowTime = g_Parts[0][i].time - nowNo;	// 時間部分である少数を取り出している

			Pos *= nowTime;								// 現在の移動量を計算している
			Rot *= nowTime;								// 現在の回転量を計算している
			Scl *= nowTime;								// 現在の拡大率を計算している

			// 計算して求めた移動量を現在の移動テーブルXYZに足している＝表示座標を求めている
			XMStoreFloat3(&g_Parts[0][i].pos, nowPos + Pos);

			// 計算して求めた回転量を現在の移動テーブルに足している
			XMStoreFloat3(&g_Parts[0][i].rot, nowRot + Rot);

			// 計算して求めた拡大率を現在の移動テーブルに足している
			XMStoreFloat3(&g_Parts[0][i].scl, nowScl + Scl);

			// frameを使て時間経過処理をする
			g_Parts[0][i].time += 1.0f / tbl[nowNo].frame;	// 時間を進めている
			if ((int)g_Parts[0][i].time >= maxNo)			// 登録テーブル最後まで移動したか？
			{
				g_Parts[0][i].time -= maxNo;				// ０番目にリセットしつつも小数部分を引き継いでいる

				switch (g_Player.setTbl)
				{
				case STATE_HINOKO:
					g_Parts[0][i].tblMax = -1;
					break;
				}
			}

		}

	}

	// ゲームオーバー処理
	if (g_Player.hp <= 0)
	{
		g_Player.state = STATE_DIE;
	}

	if (g_Player.state == STATE_DIE)
	{
		g_Player.dissolve.Enable = TRUE;
		g_Player.dissolve.threshold += 0.01f;
	}

	{	// ポイントライトのテスト
		LIGHT* light = GetLightData(1);
		XMFLOAT3 pos = g_Player.pos;

		pos.y += 70.0f;

		light->Position = pos;
		light->Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		light->Ambient = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		light->Type = LIGHT_TYPE_POINT;
		light->Enable = TRUE;
		SetLightData(1, light);
	}



	//////////////////////////////////////////////////////////////////////
	// 姿勢制御
	//////////////////////////////////////////////////////////////////////

	XMVECTOR vx, nvx, up;
	XMVECTOR quat;
	float len, angle;


	g_Player.UpVector = Normal;
	up = { 0.0f, 1.0f, 0.0f, 0.0f };
	vx = XMVector3Cross(up, XMLoadFloat3(&g_Player.UpVector));

	nvx = XMVector3Length(vx);
	XMStoreFloat(&len, nvx);
	nvx = XMVector3Normalize(vx);
	angle = asinf(len);

	quat = XMQuaternionRotationNormal(nvx, angle);

	quat = XMQuaternionSlerp(XMLoadFloat4(&g_Player.Quaternion), quat, 0.05f);
	XMStoreFloat4(&g_Player.Quaternion, quat);


#ifdef _DEBUG
	// デバッグ表示
	char state[STATE_MAX][256] = 
	{
		"STATE_IDLE",
		"STATE_WALK",
		"STATE_HINOKO",
		"STATE_DIE",
	};

	PrintDebugProc("Player X:%f Y:%f Z:% N:%f\n", g_Player.pos.x, g_Player.pos.y, g_Player.pos.z, Normal.y);
	PrintDebugProc("HP :%d\n", g_Player.hp);
	PrintDebugProc("MP :%d\n", g_Player.mp);
	PrintDebugProc("state :%s\n", state[g_Player.state]);

	if (GetKeyboardTrigger(DIK_0))
	{
		g_Player.hp = 0;
	}

	if (GetKeyboardTrigger(DIK_9))
	{
		g_Player.mp = PLAYER_MP_MAX;
	}


#endif

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawPlayer(void)
{
	if (g_Player.use == FALSE) return;

	// カリング無効
	SetCullingMode(CULL_MODE_NONE);

	// dissolve
	SetDissolve(&g_Player.dissolve);
	SetAlphaTestEnable(g_Player.dissolve.Enable);

	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld, quatMatrix;

	// カリング無効
	SetCullingMode(CULL_MODE_NONE);

	// ワールドマトリックスの初期化
	mtxWorld = XMMatrixIdentity();

	// スケールを反映
	mtxScl = XMMatrixScaling(g_Player.scl.x, g_Player.scl.y, g_Player.scl.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

	// 回転を反映
	mtxRot = XMMatrixRotationRollPitchYaw(g_Player.rot.x, g_Player.rot.y + XM_PI, g_Player.rot.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

	// クォータニオンを反映
	quatMatrix = XMMatrixRotationQuaternion(XMLoadFloat4(&g_Player.Quaternion));
	mtxWorld = XMMatrixMultiply(mtxWorld, quatMatrix);

	// 移動を反映
	mtxTranslate = XMMatrixTranslation(g_Player.pos.x, g_Player.pos.y, g_Player.pos.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

	// ワールドマトリックスの設定
	SetWorldMatrix(&mtxWorld);

	XMStoreFloat4x4(&g_Player.mtxWorld, mtxWorld);



	// 階層アニメーション
	for (int i = 0; i < PLAYER_PARTS_MAX; i++)
	{
		// ワールドマトリックスの初期化
		mtxWorld = XMMatrixIdentity();

		// スケールを反映
		mtxScl = XMMatrixScaling(g_Parts[0][i].scl.x, g_Parts[0][i].scl.y, g_Parts[0][i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// 回転を反映
		mtxRot = XMMatrixRotationRollPitchYaw(g_Parts[0][i].rot.x, g_Parts[0][i].rot.y, g_Parts[0][i].rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// 移動を反映
		mtxTranslate = XMMatrixTranslation(g_Parts[0][i].pos.x, g_Parts[0][i].pos.y, g_Parts[0][i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		if (g_Parts[0][i].parent != NULL)	// 子供だったら親と結合する
		{
			mtxWorld = XMMatrixMultiply(mtxWorld, XMLoadFloat4x4(&g_Parts[0][i].parent->mtxWorld));
																			// ↑
																			// g_Player.mtxWorldを指している
		}

		XMStoreFloat4x4(&g_Parts[0][i].mtxWorld, mtxWorld);

		// 使われているなら処理する
		if (g_Parts[0][i].use == FALSE) continue;

		// ワールドマトリックスの設定
		SetWorldMatrix(&mtxWorld);

		// モデル描画
		DrawModel(&g_PartsModel[g_Parts[0][i].modelIdx].model);

	}

	// dissolve
	SetDissolveEnable(FALSE);
	SetAlphaTestEnable(g_Player.dissolve.Enable);

	// カリング設定を戻す
	SetCullingMode(CULL_MODE_BACK);
}


//=============================================================================
// プレイヤー情報を取得
//=============================================================================
PLAYER *GetPlayer(void)
{
	return &g_Player;
}



// 子の当たり判定(パーツの番号、パーツ半径、エネミーの座標、エネミーの半径)
BOOL Collision(int index, float r, XMFLOAT3 epos, float er)
{
	// パーツの子の付け根の座標を求めている
	XMMATRIX mtx = XMLoadFloat4x4(&g_Parts[0][index].mtxWorld);

	// 求めた座標を入れてる
	XMFLOAT3 pos = XMFLOAT3(mtx.r[3].m128_f32[0], mtx.r[3].m128_f32[1], mtx.r[3].m128_f32[2]);

	//BCの当たり判定の結果を返す
	return CollisionBC(pos, epos, r, er);
}

//=============================================================================
// キーボードの入力処理
//=============================================================================

BOOL AnyKeyPress(void)
{
	// どれかが押されたら
	return (MoveLeftKeyboard() || MoveRightKeyboard() || MoveUpKeyboard() || MoveDownKeyboard());

}

// 左移動
BOOL MoveLeftKeyboard(void)
{
	return GetKeyboardPress(DIK_A);
}

// 右移動
BOOL MoveRightKeyboard(void)
{
	return GetKeyboardPress(DIK_D);

}

// 上移動
BOOL MoveUpKeyboard(void)
{
	return GetKeyboardPress(DIK_W);
}

// 下移動
BOOL MoveDownKeyboard(void)
{
	return GetKeyboardPress(DIK_S);
}


//=============================================================================
// スティックの入力処理
//=============================================================================

// どれかが押されたら
BOOL AnyStickpress(void)
{
	return (MoveLeftStick() || MoveRightStick() || MoveUpStick() || MoveDownStick());
}

// 左移動
BOOL MoveLeftStick(void)
{
	return IsButtonPressed(0, STICK_L_LEFT);
}

// 右移動
BOOL MoveRightStick(void)
{
	return IsButtonPressed(0, STICK_L_RIGHT);
}

// 上移動
BOOL MoveUpStick(void)
{
	return IsButtonPressed(0, STICK_L_UP);
}

// 下移動
BOOL MoveDownStick(void)
{
	return IsButtonPressed(0, STICK_L_DOWN);
}

void ChangeModelPlayer(int type, int id)
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
		g_Player.type = PLAYER_RED;
		break;

	case ENEMY_BLUE:
		g_Parts[id][0].modelIdx = 5;
		g_Parts[id][1].modelIdx = 6;
		g_Parts[id][2].modelIdx = 7;
		g_Parts[id][3].modelIdx = 8;
		g_Parts[id][4].modelIdx = 9;
		g_Player.type = PLAYER_BLUE;
		break;

	case ENEMY_GREEN:
		g_Parts[id][0].modelIdx = 10;
		g_Parts[id][1].modelIdx = 11;
		g_Parts[id][2].modelIdx = 12;
		g_Parts[id][3].modelIdx = 13;
		g_Parts[id][4].modelIdx = 14;
		g_Player.type = PLAYER_GREEN;
		break;

	case ENEMY_YELLOW:
		g_Parts[id][0].modelIdx = 15;
		g_Parts[id][1].modelIdx = 16;
		g_Parts[id][2].modelIdx = 17;
		g_Parts[id][3].modelIdx = 18;
		g_Parts[id][4].modelIdx = 19;
		g_Player.type = PLAYER_YELLOW;
		break;

	}

	
}

// 毎フレームごとにひのこのカウントが０の時に、色を変える
void ChangeHinokoColor(void)
{
	if (g_Player.attack == FALSE)
	{
		switch (g_Player.type)
		{
		case PLAYER_RED:
			g_Player.hinokoColor = XMFLOAT4(1.0f, 0.5f, 0.1f, 0.85f);
			break;

		case PLAYER_BLUE:
			g_Player.hinokoColor = XMFLOAT4(0.1f, 0.5f, 1.0f, 0.85f);
			break;

		case PLAYER_GREEN:
			g_Player.hinokoColor = XMFLOAT4(0.1f, 1.0f, 0.1f, 0.85f);
			break;

		case PLAYER_YELLOW:
			g_Player.hinokoColor = XMFLOAT4(1.0f, 1.0f, 0.1f, 0.85f);
			break;

		default:
			break;
		}
	}
}

// HPの自然回復
void hpGain(void)
{
	g_Player.HPGainCnt++;
	if (g_Player.HPGainCnt % HP_GAIN_FRAME == 0)
	{
		if (g_Player.hp < PLAYER_HP_MAX)
		{
			g_Player.hp++;
		}
		g_Player.HPGainCnt = 0;
	}
}

// MPの自然回復
void mpGain(void)
{
	g_Player.MPGainCnt++;
	if (g_Player.MPGainCnt % MP_GAIN_FRAME == 0)
	{
		if (g_Player.mp < PLAYER_MP_MAX)
		{
			g_Player.mp++;
		}
		g_Player.MPGainCnt = 0;
	}
}

// MPの回復
void AddMP(int amount)
{
	g_Player.mp += amount;

	if (g_Player.mp > PLAYER_MP_MAX) g_Player.mp = PLAYER_MP_MAX;
}

// MPの消費
void SubtractMP(int amount)
{
	g_Player.mp -= amount;

	if (g_Player.mp < 0) g_Player.mp = 0;
}

void InputMovePlayer(void)
{
	// 移動処理
	if (AnyKeyPress())
	{
		g_Player.moving = TRUE;

		if (g_Player.state != STATE_HINOKO)
		{
			g_Player.state = STATE_WALK;
		}

		g_Player.spd = VALUE_MOVE;

		float x = 0.0f;
		float z = 0.0f;

		if (MoveLeftKeyboard())		x = -1.0f;
		if (MoveRightKeyboard())	x = 1.0f;
		if (MoveUpKeyboard())		z = -1.0f;
		if (MoveDownKeyboard())		z = 1.0f;


		float angle = atan2f(x, z) * -1.0f;
		g_Player.dir = angle;
	}

	if (AnyStickpress())
	{
		g_Player.moving = TRUE;

		if (g_Player.state != STATE_HINOKO)
		{
		g_Player.state = STATE_WALK;
		}

		g_Player.spd = VALUE_MOVE;
		DIJOYSTATE2 dijs = GetDeviceState();
		float angle = atan2f((float)dijs.lX, (float)dijs.lY) * -1.0f;
		g_Player.dir = angle;
	}
}

void WalkPlayer(void)
{

	XMFLOAT3 OldPos = g_Player.pos;

	// Key入力があったら移動処理する
	CAMERA* cam = GetCamera();

	if (g_Player.spd > 0.0f)
	{
		g_Player.rot.y = g_Player.dir + cam->rot.y;

		// 入力のあった方向へプレイヤーを向かせて移動させる
		g_Player.pos.x -= sinf(g_Player.rot.y) * g_Player.spd;
		g_Player.pos.z -= cosf(g_Player.rot.y) * g_Player.spd;
	}

	switch (g_Player.state)
	{
	case STATE_WALK:
		if (g_Player.spd < VALUE_MOVE) g_Player.state = STATE_IDLE;
		break;

	default:
		break;
	}


	// フィールドの外に出たらプレイヤーを戻す処理
	if (g_Player.pos.x - g_Player.size < MAP_LEFT
		|| g_Player.pos.x + g_Player.size> MAP_RIGHT
		|| g_Player.pos.z - g_Player.size< MAP_DOWN
		|| g_Player.pos.z + g_Player.size> MAP_TOP)
	{
		g_Player.pos = OldPos;
	}

}

void InputHinokoPlayer(void)
{
	// 弾発射処理
	if (GetKeyboardTrigger(DIK_SPACE) || IsButtonTriggered(0,BUTTON_R))
	{
		hinokoCnt = 0;
		g_Player.state = STATE_HINOKO;
		g_Player.attack = TRUE;
		PlaySound(SOUND_LABEL_SE_FIRE);

		if (g_Player.mp >= HINOKO_COST)
		{
			SetBullet(g_Player.pos, g_Player.rot);

			switch (g_Player.type)
			{
			case PLAYER_RED:
				g_Player.mp -= HINOKO_COST;
				g_Player.damage = PLAYER_RED_DAMAGE_VALUE;
				break;

			case PLAYER_BLUE:
				g_Player.mp -= (HINOKO_COST * 2);
				g_Player.damage = PLAYER_GREEN_DAMAGE_VALUE;
				break;

			case PLAYER_GREEN:
				g_Player.mp -= HINOKO_COST;
				g_Player.damage = PLAYER_BLUE_DAMAGE_VALUE;
				break;

			case PLAYER_YELLOW:
				g_Player.mp -= (HINOKO_COST * 2);
				g_Player.damage = PLAYER_YELLOW_DAMAGE_VALUE;
				break;
			}
		}
	}
}

void InputCharge(void)
{
	BULLET* bullet = GetBullet();

	// 弾発射処理
	if (GetKeyboardRelease(DIK_RETURN) || IsButtonRelease(0, BUTTON_R2))
	{
		soundF = FALSE;
		StopSound(SOUND_LABEL_SE_CHARGE);

		if (g_Player.chargeCnt == CHARGE_CNT_MAX && g_Player.mp > CHARGE_COST)
		{
			PlaySound(SOUND_LABEL_SE_FIRE);
			SetBullet_Charge(g_Player.pos, g_Player.rot);
			g_Player.mp -= CHARGE_COST;
			g_Player.chargeCnt = 0;
		}
	}

	// チャージ時処理
	if (GetKeyboardPress(DIK_RETURN) || IsButtonPressed(0,BUTTON_R2))
	{
		if (soundF == FALSE)
		{
			PlaySound(SOUND_LABEL_SE_CHARGE);
			soundF = TRUE;
		}

		g_Player.state = STATE_HINOKO;
		g_Player.attack = TRUE;

		if (g_Player.chargeCnt < CHARGE_CNT_MAX && g_Player.mp >= CHARGE_COST)
		{
			g_Player.chargeCnt++;
		}

		// チャージ中の集まるパーティクル
		if (g_Player.mp >= CHARGE_COST)
		{
			XMFLOAT3 sPos;
			sPos.x = g_Player.pos.x - (rand() % 101 - 50);
			sPos.y = g_Player.pos.y - (rand() % 101 - 50);
			sPos.z = g_Player.pos.z - (rand() % 101 - 50);

			XMFLOAT3 ePos = g_Player.pos;

			switch (g_Player.type)
			{
			case PLAYER_RED:
				g_Player.damage = PLAYER_RED_DAMAGE_VALUE * 2;
				color = { 1.0f, 0.5f, 0.1f, 1.0f };
				SetParticleCharge(sPos, ePos, { 0.1f, 0.1f, 0.1f }, color, PARTICLE_CHARGE_RED, 60);
				break;

			case PLAYER_BLUE:
				g_Player.damage = PLAYER_GREEN_DAMAGE_VALUE * 2;
				color = { 0.1f, 0.5f, 1.0f, 1.0f };
				SetParticleCharge(sPos, ePos, { 0.1f, 0.1f, 0.1f }, color, PARTICLE_CHARGE_BLUE, 60);
				break;

			case PLAYER_GREEN:
				g_Player.damage = PLAYER_BLUE_DAMAGE_VALUE * 2;
				color = { 0.1f, 1.0f, 0.1f, 1.0f };
				SetParticleCharge(sPos, ePos, { 0.1f, 0.1f, 0.1f }, color, PARTICLE_CHARGE_GREEN, 60);
				break;

			case PLAYER_YELLOW:
				g_Player.damage = PLAYER_YELLOW_DAMAGE_VALUE * 2;
				color = { 1.0f, 1.0f, 0.1f, 1.0f };
				SetParticleCharge(sPos, ePos, { 0.1f, 0.1f, 0.1f }, color, PARTICLE_CHARGE_YELLOW, 60);
				break;
			}
		}
	}
	else
	{
		if(g_Player.chargeCnt > 0)
		{
			g_Player.chargeCnt--;
		}
	}

}

void HinokoPlayer(void)
{
	// 状態遷移
	{
		BOOL ans = TRUE;

		// すべてのパーツの最大テーブル数が -1 になっている場合(≒アニメーションが終了している場合)、状態遷移させる
		for (int i = 0; i < PLAYER_PARTS_MAX; i++)
		{
			if (g_Parts[0][i].tblMax != -1) ans = FALSE;
		}

		if (ans == TRUE) g_Player.state = STATE_IDLE;
	}

	hinokoCnt++;
	if (hinokoCnt > 60)
	{
		g_Player.attack = FALSE;
		hinokoCnt = 0;
	}

	if ((g_Player.moving == FALSE) && (g_Player.attack == FALSE))
	{
		g_Player.state = STATE_IDLE;
	}

}


void SetTblIdle(void)
{
	switch (g_Player.setTbl)
	{
	case STATE_IDLE:
		break;

	default:
		for (int i = 0; i < PLAYER_PARTS_MAX; i++)
		{
			if (g_Parts[0][i].use == FALSE) continue;

			g_Parts[0][i].time = 0.0f;
			g_Parts[0][i].tblNo = i + IDLE_BODY;
			g_Parts[0][i].tblMax = (int)g_TblAdr[g_Parts[0][i].tblNo]->size();

		}
		g_Player.setTbl = STATE_IDLE;
		break;
	}
}

void SetTblMove(void)
{
	switch (g_Player.setTbl)
	{
	case STATE_WALK:
		break;

	default:
		for (int i = 0; i < PLAYER_PARTS_MAX; i++)
		{
			if (g_Parts[0][i].use == FALSE) continue;

			g_Parts[0][i].time = 0.0f;
			g_Parts[0][i].tblNo = i + WALK_BODY;
			g_Parts[0][i].tblMax = (int)g_TblAdr[g_Parts[0][i].tblNo]->size();

		}
		g_Player.setTbl = STATE_WALK;
		break;
	}
}

void SetTblHinoko(void)
{
	switch (g_Player.setTbl)
	{
	case STATE_HINOKO:
		break;

	default:
		for (int i = 0; i < PLAYER_PARTS_MAX; i++)
		{
			if (g_Parts[0][i].use == FALSE) continue;

			g_Parts[0][i].time = 0.0f;
			g_Parts[0][i].tblNo = i + HINOKO_BODY;
			g_Parts[0][i].tblMax = (int)g_TblAdr[g_Parts[0][i].tblNo]->size();

		}
		g_Player.setTbl = STATE_HINOKO;
		break;
	}
}

void InputQuickTurn(void)
{
	if (GetKeyboardTrigger(DIK_R))
	{
		CAMERA* cam = GetCamera();

		g_Player.rot.y += XM_PI;

		if (g_Player.rot.y > XM_PI)
		{
			g_Player.rot.y -= XM_2PI;
		}
	}
}


void  RecastChangePlayer()
{
	// プレイヤーチェンジカウントがリキャスト時間より小さい場合
	if (g_Player.changeCnt < CHANGE_COOLTIME)
	{
		g_Player.changeCnt++;    // リキャストカウントを進める
	}
}