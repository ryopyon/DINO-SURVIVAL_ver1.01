//=============================================================================
//
// ゲーム画面処理 [game.cpp]
// Author : HAL東京 高橋諒
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "model.h"
#include "game.h"
#include "camera.h"
#include "input.h"
#include "sound.h"
#include "fade.h"

#include "player.h"
#include "enemy.h"
#include "boss.h"
#include "meshfield.h"
#include "meshwall.h"
#include "light.h"
#include "shadow.h"
#include "obj.h"
#include "BG.h"
#include "aim.h"
#include "bullet.h"
#include "bullet_charge.h"
#include "score.h"
#include "particle.h"
#include "particle_spark.h"
#include "particle_charge.h"
#include "collision.h"
#include "debugproc.h"
#include "ui.h"
#include "ui_enemy.h"
#include "event.h"
#include "flag.h"

#include "pause.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MONOKURO_END		(150)			// モノクロが終わる時間



//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void CheckHit(void);



//*****************************************************************************
// グローバル変数
//*****************************************************************************
static GAME		g_Game;

static int		g_ViewPortType_Game = TYPE_FULL_SCREEN;

static int		g_MonokuroCnt = 0;

// ワイプ
static BOOL						g_wipe = FALSE;
static float					g_wipeSize = 0.0f;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitGame(void)
{
	g_MonokuroCnt = 0;

	// モノクロを直す
	SetMonochrome(FALSE);

	// ワイプを初期化
	SetWipe(WIPE_MODE_NONE, XMFLOAT4(1.0f, 1.0f, 0.0f, 0.0f), g_wipeSize);

	// ポーズの初期化
	g_Game.bPause = FALSE;

	// フィールドの初期化
	InitMeshField(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), 100, 100, 13.0f, 13.0f);

	// ライトを有効化	
	InitLight();

	// 影の初期化処理
	InitShadow();

	// プレイヤーの初期化
	InitPlayer();

	// エネミーの初期化
	InitEnemy();

	// ボスの初期化
	InitBoss();

	// 壁の初期化
	InitMeshWall(XMFLOAT3(0.0f, 0.0f, MAP_TOP), XMFLOAT3(0.0f, 0.0f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 16, 1, 80.0f, 40.0f);
	InitMeshWall(XMFLOAT3(MAP_LEFT, 0.0f, 0.0f), XMFLOAT3(0.0f, -XM_PI * 0.50f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 16, 1, 80.0f, 40.0f);
	InitMeshWall(XMFLOAT3(MAP_RIGHT, 0.0f, 0.0f), XMFLOAT3(0.0f, XM_PI * 0.50f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 16, 1, 80.0f, 40.0f);
	InitMeshWall(XMFLOAT3(0.0f, 0.0f, MAP_DOWN), XMFLOAT3(0.0f,  XM_PI, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 16, 1, 80.0f, 40.0f);

	// 壁(裏側用の半透明)
	InitMeshWall(XMFLOAT3(0.0f, 0.0f, MAP_TOP), XMFLOAT3(0.0f,    XM_PI, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 0.25f), 16, 1, 80.0f, 40.0f);
	InitMeshWall(XMFLOAT3(MAP_LEFT, 0.0f, 0.0f), XMFLOAT3(0.0f,   XM_PI * 0.50f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 0.25f), 16, 1, 80.0f, 40.0f);
	InitMeshWall(XMFLOAT3(MAP_RIGHT, 0.0f, 0.0f), XMFLOAT3(0.0f, -XM_PI * 0.50f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 0.25f), 16, 1, 80.0f, 40.0f);
	InitMeshWall(XMFLOAT3(0.0f, 0.0f, MAP_DOWN), XMFLOAT3(0.0f, 0.0f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 0.25f), 16, 1, 80.0f, 40.0f);


	// オブジェクトを生成
	InitObj();

	// 背景を生成
	InitBG();

	// エイムモデルを生やす
	InitAim();

	// 弾の初期化
	InitBullet();

	// チャージ弾の初期化
	InitBullet_Charge();

	// スコアの初期化
	InitScore();

	// ポーズの初期化
	InitPause();

	// UIの初期化
	InitUi();

	// エネミーUIの初期化
	InitUi_Enemy();

	// イベントの初期化
	InitEvent();

	// パーティクルの初期化
	InitParticle();

	// パーティクルスパークの初期化
	InitParticleSpark();

	// パーティクルチャージの初期化
	InitParticleCharge();

	// フラグの初期化
	InitFlag();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitGame(void)
{
	// パーティクルの終了処理
	UninitParticle();

	// パーティクルスパークの終了処理
	UninitParticleSpark();

	// パーティクルチャージの終了処理
	UninitParticleCharge();

	// スコアの終了処理
	UninitScore();

	// UIの終了処理
	UninitUi();

	// エネミーUIの終了処理
	UninitUi_Enemy();

	// 弾の終了処理
	UninitBullet();

	// チャージ弾の終了処理
	UninitBullet_Charge();

	// オブジェクトの終了処理
	UninitObj();

	// 背景の終了処理
	UninitBG();

	// エイムモデルの終了処理
	UninitAim();

	// 壁の終了処理
	UninitMeshWall();

	// 地面の終了処理
	UninitMeshField();

	// ボスの終了処理
	UninitBoss();

	// エネミーの終了処理
	UninitEnemy();

	// プレイヤーの終了処理
	UninitPlayer();

	// 影の終了処理
	UninitShadow();

	// イベントの終了処理
	UninitEvent();

	// ポーズの終了処理
	UninitPause();

	UninitFlag();

}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateGame(void)
{
#ifdef _DEBUG
	if (GetKeyboardTrigger(DIK_V))
	{
		g_ViewPortType_Game = (g_ViewPortType_Game + 1) % TYPE_NONE;
		SetViewPort(g_ViewPortType_Game);
	}

	PrintDebugProc("GameFlag:%d\n", GetFlag());


#endif




	if (g_Game.bPause == TRUE)
	{
		UpdatePause();
	}
	else
	{
		ResetPause();

		// カメラ更新(応急処置)
		UpdateCamera();

		// 地面処理の更新
		UpdateMeshField();

		// イベント中は止める
		if (GetFlag() != FLAG_GAME_EVENT_PILLAR && GetFlag() != FLAG_GAME_EVENT_FIRE)
		{

			// エネミーの更新処理
			UpdateEnemy();

			// プレイヤーの更新処理
			UpdatePlayer();
		}


		// ボスの更新処理
		UpdateBoss();

		// 壁処理の更新
		UpdateMeshWall();

		// エイムモデルの更新処理
		UpdateAim();


		// オブジェクトの更新処理
		UpdateObj();

		// 背景の更新処理
		UpdateBG();

		// 弾の更新処理
		UpdateBullet();

		// チャージ弾の更新処理
		UpdateBullet_Charge();

		// パーティクルの更新処理
		UpdateParticle();

		// パーティクルスパークの更新処理
		UpdateParticleSpark();

		// パーティクルチャージの更新処理
		UpdateParticleCharge();

		// 影の更新処理
		UpdateShadow();

		// 当たり判定処理
		CheckHit();

		// スコアの更新処理
		UpdateScore();

		// UIの更新処理
		UpdateUi();

		// エネミーUIの更新処理
		UpdateUi_Enemy();

		// イベントの更新処理
		UpdateEvent();

	}

	if (GetKeyboardTrigger(DIK_P) || IsButtonTriggered(0, BUTTON_START))
	{
		PlaySound(SOUND_LABEL_SE_CRICK);
		g_Game.bPause = g_Game.bPause ? FALSE : TRUE;
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawGame0(void)
{
	// 3Dの物を描画する処理
	// 地面の描画処理
	DrawMeshField();

	// 影の描画処理
	DrawShadow();

	// ボスの描画処理
	DrawBoss();

	// エネミーの描画処理
	DrawEnemy();

	// プレイヤーの描画処理
	DrawPlayer();

	// 弾の描画処理
	DrawBullet();

	// チャージ弾の描画処理
	DrawBullet_Charge();

	// 壁の描画処理
	DrawMeshWall();

	// オブジェクトの描画処理
	DrawObj();

	// 背景の描画処理
	DrawBG();

	// パーティクルの描画処理
	DrawParticle();

	// パーティクルスパークの描画処理
	DrawParticleSpark();

	// パーティクルチャージの描画処理
	DrawParticleCharge();

	// イベントの描画処理
	DrawEvent();

	// エネミーUIの描画処理
	DrawUi_Enemy();


	// 2Dの物を描画する処理
	// Z比較なし
	SetDepthEnable(FALSE);

	// ライティングを無効
	SetLightEnable(FALSE);

	// UIの描画処理
	DrawUi();

	// エイムモデルの描画処理
	DrawAim();

	// バトル中に描画
	if (GetFlag() == FLAG_GAME_BATTLE)
	{
		// スコアの描画処理
		DrawScore();
	}


	if (g_Game.bPause == TRUE)
	{
		// ポーズの描画処理
		DrawPause();
	}

	// ライティングを有効に
	SetLightEnable(TRUE);

	// Z比較あり
	SetDepthEnable(TRUE);

}


void DrawGame(void)
{
	XMFLOAT3 pos;


#ifdef _DEBUG
	// デバッグ表示
	PrintDebugProc("ViewPortType:%d\n", g_ViewPortType_Game);

#endif

	// プレイヤー視点
	pos = GetPlayer()->pos;
	pos.y = 0.0f;			// カメラ酔いを防ぐためにクリアしている


	if (GetFlag() != FLAG_GAME_EVENT_FIRE && GetFlag() != FLAG_GAME_EVENT_PILLAR)
	{
		SetCameraAT(pos);
	}

	SetCamera();


	switch(g_ViewPortType_Game)
	{
	case TYPE_FULL_SCREEN:
		SetViewPort(TYPE_FULL_SCREEN);
		DrawGame0();
		break;

	case TYPE_LEFT_HALF_SCREEN:
	case TYPE_RIGHT_HALF_SCREEN:
		SetViewPort(TYPE_LEFT_HALF_SCREEN);
		DrawGame0();

		// エネミー視点
		pos = GetEnemy()->pos;
		pos.y = 0.0f;
		SetCameraAT(pos);
		SetCamera();
		SetViewPort(TYPE_RIGHT_HALF_SCREEN);
		DrawGame0();
		break;

	case TYPE_UP_HALF_SCREEN:
	case TYPE_DOWN_HALF_SCREEN:
		SetViewPort(TYPE_UP_HALF_SCREEN);
		DrawGame0();

		// エネミー視点
		pos = GetEnemy()->pos;
		pos.y = 0.0f;
		SetCameraAT(pos);
		SetCamera();
		SetViewPort(TYPE_DOWN_HALF_SCREEN);
		DrawGame0();
		break;

	}

}


//=============================================================================
// 当たり判定処理
//=============================================================================
void CheckHit(void)
{
	ENEMY *enemy = GetEnemy();							// エネミーのポインターを初期化
	PLAYER *player = GetPlayer();						// プレイヤーのポインターを初期化
	BULLET *bullet = GetBullet();						// 弾のポインターを初期化
	BULLET_CHARGE *bullet_charge = GetBullet_Charge();	// 弾のポインターを初期化
	EVENT* event = GetEvent();							// フラグのポインターを初期化
	PARTICLE* particle = GetParticle();					// パーティクルのポインターを初期化


	XMFLOAT3 PlayerLowerPos = XMFLOAT3(player->pos.x, player->pos.y - 3.0f, player->pos.z);
	XMFLOAT3 PlayerUpperPos = XMFLOAT3(player->pos.x, player->pos.y + 3.0f, player->pos.z);

	// 敵とプレイヤーキャラ
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		//敵の有効フラグをチェックする
		if (enemy[i].use == FALSE || enemy[i].EntryTime || enemy[i].LeaveTime)
			continue;


		//カプセルの当たり判定
		if(CapsuleCollision(PlayerUpperPos, PlayerLowerPos, enemy[i].pos, player->size, enemy[i].size))
			{
			// 敵キャラクターは倒される
			enemy[i].use = FALSE;
			ReleaseShadow(enemy[i].shadowIdx);
			player->hp -= enemy[i].damage;
			PlaySound(SOUND_LABEL_SE_DAMAGE);

			// 弾が当たった時にパーティクル発生
			switch (player->type)
			{
			case PLAYER_RED:
				SetParticle(player->pos, XMFLOAT3(2.0f, 2.0f, 2.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f),
					TYPE_CHANGE_PLAYER, BLEND_MODE_SUBTRACT, 0.0f, 0.0f, 90);
				break;

			case PLAYER_BLUE:
				SetParticle(player->pos, XMFLOAT3(2.0f, 2.0f, 2.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.9f, 0.5f, 0.0f, 1.0f),
					TYPE_CHANGE_PLAYER, BLEND_MODE_SUBTRACT, 0.0f, 0.0f, 90);
				break;

			case PLAYER_GREEN:
				SetParticle(player->pos, XMFLOAT3(2.0f, 2.0f, 2.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.9f, 0.0f, 0.9f, 1.0f),
					TYPE_CHANGE_PLAYER, BLEND_MODE_SUBTRACT, 0.0f, 0.0f, 90);
				break;

			case PLAYER_YELLOW:
				SetParticle(player->pos, XMFLOAT3(2.0f, 2.0f, 2.0f), XMFLOAT3(0.0f, 5.0f, 0.0f), XMFLOAT4(0.0f, 0.0f, 0.9f, 1.0f),
					TYPE_CHANGE_PLAYER, BLEND_MODE_SUBTRACT, 0.0f, 0.0f, 90);
				break;
			}

		}

	}

	// 球関連
	for (int i = 0; i < MAX_BULLET; i++)
	{
		//弾の有効フラグをチェックする
		if (bullet[i].bUse == FALSE)
			continue;

		for (int j = 0; j < 1; j++)
		{
			if (event->use == FALSE) continue;
			if (GetFlag() >= FLAG_GAME_EVENT_FIRE) continue;

			// BCの当たり判定
			if (CollisionBC(bullet[i].pos, event->pos, bullet[i].size, event->size))
			{
				// 火花を散らす
				for (int p = 0; p < 20; p++)
				{
					XMFLOAT3 move = CalcParticleSparkMove(event->pos);

					SetParticleSpark(bullet[i].pos, move, 30, TRUE);
				}

				PlaySound(SOUND_LABEL_SE_FIRE);

				// フラグの切り替え
				SetFlag(FLAG_GAME_EVENT_FIRE);
			}
		}

		// 敵と当たってるか調べる
		for (int j = 0; j < MAX_ENEMY; j++)
		{
			//敵の有効フラグをチェックする
			if (enemy[j].use == FALSE || enemy[j].EntryTime || enemy[j].LeaveTime)
				continue;

			XMFLOAT3 EnemyLowerPos = XMFLOAT3(enemy[j].pos.x, enemy[j].pos.y - 4.0f, enemy[j].pos.z);
			XMFLOAT3 EnemyUpperPos = XMFLOAT3(enemy[j].pos.x, enemy[j].pos.y + 4.0f, enemy[j].pos.z);

			//カプセルの当たり判定
			if (CapsuleCollision(EnemyUpperPos, EnemyLowerPos, bullet[i].pos, enemy[j].size, bullet[i].size))
			{
				// バレットを消す
				ReleaseBullet(i);

				enemy[j].hp -= player->damage;

				// 弾が当たった時にパーティクル発生
				switch (enemy[j].type)
				{
				case ENEMY_RED:
					SetParticle(enemy[j].pos, XMFLOAT3(2.0f, 2.0f, 2.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f),
						TYPE_CHANGE_PLAYER, BLEND_MODE_SUBTRACT, 0.0f, 0.0f, 90);

					break;

				case ENEMY_BLUE:
					SetParticle(enemy[j].pos, XMFLOAT3(2.0f, 2.0f, 2.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.9f, 0.5f, 0.0f, 1.0f),
						TYPE_CHANGE_PLAYER, BLEND_MODE_SUBTRACT, 0.0f, 0.0f, 90);

					break;

				case ENEMY_GREEN:
					SetParticle(enemy[j].pos, XMFLOAT3(2.0f, 2.0f, 2.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.9f, 0.0f, 0.9f, 1.0f),
						TYPE_CHANGE_PLAYER, BLEND_MODE_SUBTRACT, 0.0f, 0.0f, 90);

					break;

				case ENEMY_YELLOW:
					SetParticle(enemy[j].pos, XMFLOAT3(2.0f, 2.0f, 2.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.0f, 0.0f, 0.9f, 1.0f),
						TYPE_CHANGE_PLAYER, BLEND_MODE_SUBTRACT, 0.0f, 0.0f, 90);

					break;
				}

				switch (bullet->type)
				{
				case BULLET_RED:
					enemy[j].state = ENEMY_ABNORMAL_BURN;
					break;

				case BULLET_BLUE:
					enemy[j].state = ENEMY_ABNORMAL_WET;
					break;

				case BULLET_GREEN:
					enemy[j].state = ENEMY_ABNORMAL_WINDY;
					break;

				case BULLET_YELLOW:
					enemy[j].state = ENEMY_ABNORMAL_SHOCK;
					break;

				}


				if (enemy[j].hp <= 0)
				{
					// dissolve開始
					enemy[j].LeaveTime = TRUE;
					ReleaseShadow(enemy[j].shadowIdx);

					// スコア
					switch (enemy[j].type)
					{
					case ENEMY_RED:
						AddScore(10);
						break;

					case ENEMY_BLUE:
						AddScore(20);
						break;

					case ENEMY_GREEN:
						AddScore(30);
						break;

					case ENEMY_YELLOW:
						AddScore(40);
						break;
					}

				}
			}
		}

	}

	// プレイヤーのチャージ弾と敵
	for (int i = 0; i < MAX_BULLET_CHARGE; i++)
	{
		//弾の有効フラグをチェックする
		if (bullet_charge[i].bUse == FALSE)
			continue;

		for (int j = 0; j < 1; j++)
		{
			if (event->use == FALSE) continue;
			if (GetFlag() >= FLAG_GAME_EVENT_FIRE) continue;

			// BCの当たり判定
			if (CollisionBC(bullet_charge[i].pos, event->pos, bullet_charge[i].size, event->size))
			{
				// 火花を散らす
				for (int p = 0; p < 20; p++)
				{
					XMFLOAT3 move = CalcParticleSparkMove(event->pos);

					SetParticleSpark(bullet_charge[i].pos, move, 30, TRUE);
				}

				PlaySound(SOUND_LABEL_SE_FIRE);

				// フラグの切り替え
				SetFlag(FLAG_GAME_EVENT_FIRE);
			}
		}

		// 敵と当たってるか調べる
		for (int j = 0; j < MAX_ENEMY; j++)
		{
			//敵の有効フラグをチェックする
			if (enemy[j].use == FALSE || enemy[j].EntryTime || enemy[j].LeaveTime)
				continue;

			XMFLOAT3 EnemyLowerPos = XMFLOAT3(enemy[j].pos.x, enemy[j].pos.y - 4.0f, enemy[j].pos.z);
			XMFLOAT3 EnemyUpperPos = XMFLOAT3(enemy[j].pos.x, enemy[j].pos.y + 4.0f, enemy[j].pos.z);

			//カプセルの当たり判定
			if (CapsuleCollision(EnemyUpperPos, EnemyLowerPos, bullet_charge[i].pos, enemy[j].size, bullet_charge[i].size))
			{
				// バレットを消す
				ReleaseBullet(i);

				enemy[j].hp -= (player->damage * 2);

				// 弾が当たった時にパーティクル発生
				switch (enemy[j].type)
				{
				case ENEMY_RED:
					SetParticle(enemy[j].pos, XMFLOAT3(2.0f, 2.0f, 2.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f),
						TYPE_CHANGE_PLAYER, BLEND_MODE_SUBTRACT, 0.0f, 0.0f, 90);

					break;

				case ENEMY_BLUE:
					SetParticle(enemy[j].pos, XMFLOAT3(2.0f, 2.0f, 2.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.9f, 0.5f, 0.0f, 1.0f),
						TYPE_CHANGE_PLAYER, BLEND_MODE_SUBTRACT, 0.0f, 0.0f, 90);

					break;

				case ENEMY_GREEN:
					SetParticle(enemy[j].pos, XMFLOAT3(2.0f, 2.0f, 2.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.9f, 0.0f, 0.9f, 1.0f),
						TYPE_CHANGE_PLAYER, BLEND_MODE_SUBTRACT, 0.0f, 0.0f, 90);

					break;

				case ENEMY_YELLOW:
					SetParticle(enemy[j].pos, XMFLOAT3(2.0f, 2.0f, 2.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.0f, 0.0f, 0.9f, 1.0f),
						TYPE_CHANGE_PLAYER, BLEND_MODE_SUBTRACT, 0.0f, 0.0f, 90);

					break;
				}

				switch (bullet_charge->type)
				{
				case BULLET_RED:
					enemy[j].state = ENEMY_ABNORMAL_BURN;
					break;

				case BULLET_BLUE:
					enemy[j].state = ENEMY_ABNORMAL_WET;
					break;

				case BULLET_GREEN:
					enemy[j].state = ENEMY_ABNORMAL_WINDY;
					break;

				case BULLET_YELLOW:
					enemy[j].state = ENEMY_ABNORMAL_SHOCK;
					break;

				}


				if (enemy[j].hp <= 0)
				{
					// dissolve開始
					enemy[j].LeaveTime = TRUE;
					ReleaseShadow(enemy[j].shadowIdx);

					// スコア
					switch (enemy[j].type)
					{
					case ENEMY_RED:
						AddScore(10);
						break;

					case ENEMY_BLUE:
						AddScore(20);
						break;

					case ENEMY_GREEN:
						AddScore(30);
						break;

					case ENEMY_YELLOW:

						AddScore(40);
						break;
					}

				}
			}
		}

	}
	// パーティクルと敵
	for (int i = 0; i < MAX_PARTICLE; i++)
	{
		//弾の有効フラグをチェックする
		if (particle[i].use == FALSE) continue;
		if (particle[i].type != TYPE_HINOKO_CHARGE_YELLOW) continue;


		for (int j = 0; j < 1; j++)
		{
			if (event->use == FALSE) continue;
			if (GetFlag() >= FLAG_GAME_EVENT_FIRE) continue;

			// BCの当たり判定
			if (CollisionBC(particle[i].pos, event->pos, particle[i].fSizeX, event->size))
			{
				// 火花を散らす
				for (int p = 0; p < 20; p++)
				{
					XMFLOAT3 move = CalcParticleSparkMove(event->pos);

					SetParticleSpark(particle[i].pos, move, 30, TRUE);
				}

				PlaySound(SOUND_LABEL_SE_FIRE);

				// フラグの切り替え
				SetFlag(FLAG_GAME_EVENT_FIRE);
			}

		}

		// 敵と当たってるか調べる
		for (int j = 0; j < MAX_ENEMY; j++)
		{
			//敵の有効フラグをチェックする
			if (enemy[j].use == FALSE || enemy[j].EntryTime || enemy[j].LeaveTime)
				continue;

			XMFLOAT3 EnemyLowerPos = XMFLOAT3(enemy[j].pos.x, enemy[j].pos.y - 5.0f, enemy[j].pos.z);
			XMFLOAT3 EnemyUpperPos = XMFLOAT3(enemy[j].pos.x, enemy[j].pos.y + 5.0f, enemy[j].pos.z);

			//カプセルの当たり判定
			if (CapsuleCollision(EnemyUpperPos, EnemyLowerPos, particle[i].pos, enemy[j].size, particle[i].fSizeX))
			{
				// バレットを消す
				ReleaseBullet(i);

				enemy[j].hp -= (player->damage * 2);

				// 弾が当たった時にパーティクル発生
				switch (enemy[j].type)
				{
				case ENEMY_RED:
					SetParticle(enemy[j].pos, XMFLOAT3(2.0f, 2.0f, 2.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f),
						TYPE_CHANGE_PLAYER, BLEND_MODE_SUBTRACT, 0.0f, 0.0f, 90);

					break;

				case ENEMY_BLUE:
					SetParticle(enemy[j].pos, XMFLOAT3(2.0f, 2.0f, 2.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.9f, 0.5f, 0.0f, 1.0f),
						TYPE_CHANGE_PLAYER, BLEND_MODE_SUBTRACT, 0.0f, 0.0f, 90);

					break;

				case ENEMY_GREEN:
					SetParticle(enemy[j].pos, XMFLOAT3(2.0f, 2.0f, 2.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.9f, 0.0f, 0.9f, 1.0f),
						TYPE_CHANGE_PLAYER, BLEND_MODE_SUBTRACT, 0.0f, 0.0f, 90);

					break;

				case ENEMY_YELLOW:
					SetParticle(enemy[j].pos, XMFLOAT3(2.0f, 2.0f, 2.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.0f, 0.0f, 0.9f, 1.0f),
						TYPE_CHANGE_PLAYER, BLEND_MODE_SUBTRACT, 0.0f, 0.0f, 90);

					break;
				}

				// 状態異常にする
				/*switch (bullet_charge->type)
				{
				case BULLET_RED:
					enemy[j].state = ENEMY_ABNORMAL_BURN;
					break;

				case BULLET_BLUE:
					enemy[j].state = ENEMY_ABNORMAL_WET;
					break;

				case BULLET_GREEN:
					enemy[j].state = ENEMY_ABNORMAL_WINDY;
					break;

				case BULLET_YELLOW:
					enemy[j].state = ENEMY_ABNORMAL_SHOCK;
					break;

				}*/

				if (enemy[j].hp <= 0)
				{
					// dissolve開始
					enemy[j].LeaveTime = TRUE;
					ReleaseShadow(enemy[j].shadowIdx);

					// スコア
					switch (enemy[j].type)
					{
					case ENEMY_RED:
						AddScore(10);
						break;

					case ENEMY_BLUE:
						AddScore(20);
						break;

					case ENEMY_GREEN:
						AddScore(30);
						break;

					case ENEMY_YELLOW:
						AddScore(40);
						break;
					}

				}
			}
		}

	}

	// ゲームオーバー処理
	if (player->state == STATE_DIE)
	{
		// モノクロ
		SetMonochrome(TRUE);

		g_MonokuroCnt++;

		if (g_MonokuroCnt == MONOKURO_END)
		{
			g_MonokuroCnt = 0;
			SetFade(FADE_OUT, MODE_RESULT);
		}
	}
}


//=============================================================================
// プレイヤー情報を取得
//=============================================================================
GAME* GetGame(void)
{
	return &g_Game;
}
