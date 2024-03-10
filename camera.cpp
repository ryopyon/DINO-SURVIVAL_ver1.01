//=============================================================================
//
// カメラ処理 [camera.cpp]
// Author : HAL東京 高橋諒
//
//=============================================================================
#include "main.h"
#include "input.h"
#include "camera.h"
#include "debugproc.h"
#include "player.h"
#include "light.h"
#include "flag.h"
#include "fade.h"
#include "sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	VIEW_ANGLE		(XMConvertToRadians(45.0f))						// ビュー平面の視野角
#define	VIEW_ASPECT		((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT)	// ビュー平面のアスペクト比	
#define	VIEW_NEAR_Z		(10.0f)											// ビュー平面のNearZ値
#define	VIEW_FAR_Z		(10000.0f)										// ビュー平面のFarZ値

#define	VALUE_MOVE_CAMERA	(2.0f)										// カメラの移動量
#define	VALUE_ROTATE_CAMERA	(XM_PI * 0.01f)								// カメラの回転量

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static CAMERA			g_Camera;		// カメラデータ

static INTERPOLATION_DATA_CAMAERA g_MoveTbl0[] = {
	//視点									注視点								回転						時間
	{ XMFLOAT3(0.0f,    7.0f, -450.0f), 	XMFLOAT3(0.0f,    7.0f, -350.0f), 	XMFLOAT3(1.0f, 1.0f, 1.0f),	120 },
	{ XMFLOAT3(0.0f,    7.0f, -450.0f), 	XMFLOAT3(0.0f,    7.0f, -350.0f), 	XMFLOAT3(1.0f, 1.0f, 1.0f),	120 },
	{ XMFLOAT3(80.0f,  30.0f, -400.0f), 	XMFLOAT3(50.0f,  50.0f, -300.0f), 	XMFLOAT3(1.0f, 1.0f, 1.0f),	60 },
	{ XMFLOAT3(80.0f,  30.0f, -400.0f), 	XMFLOAT3(50.0f,  50.0f, -300.0f), 	XMFLOAT3(1.0f, 1.0f, 1.0f),	70 },

	{ XMFLOAT3(-80.0f, 30.0f, -400.0f), 	XMFLOAT3(-50.0f, 50.0f, -300.0f), 	XMFLOAT3(1.0f, 1.0f, 1.0f),	60 },
	{ XMFLOAT3(-80.0f, 30.0f, -400.0f),		XMFLOAT3(-50.0f, 50.0f, -300.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f),	70 },

	{ XMFLOAT3(-80.0f, 30.0f, -300.0f),		XMFLOAT3(-50.0f, 50.0f, -200.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(-80.0f, 30.0f, -300.0f),		XMFLOAT3(-50.0f, 50.0f, -200.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f),	70 },

	{ XMFLOAT3(80.0f, 30.0f, -300.0f),		XMFLOAT3(50.0f, 50.0f, -200.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f),	60 },
	{ XMFLOAT3(80.0f, 30.0f, -300.0f),		XMFLOAT3(50.0f, 50.0f, -200.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f),	70 },


	{ XMFLOAT3(200.0f, 70.0f, -300.0f),		XMFLOAT3(50.0f, 50.0f, -200.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f),	420 },
	{ XMFLOAT3(200.0f, 70.0f, -300.0f),		XMFLOAT3(50.0f, 50.0f, -200.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f),	120 },
	{ XMFLOAT3(200.0f, 70.0f, -300.0f),		XMFLOAT3(50.0f, 50.0f, -200.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f),	120 },

	{ XMFLOAT3(0.0f,   25.0f, -450.0f), 	XMFLOAT3(0.0f,   7.0f, -350.0f), 	XMFLOAT3(1.0f, 1.0f, 1.0f),	120 },
};

static INTERPOLATION_DATA_CAMAERA* g_standbyTblAdr[] =
{
	g_MoveTbl0,
};

static int				g_ViewPortType = TYPE_FULL_SCREEN;

static float			g_TargetRotY;

//=============================================================================
// 初期化処理
//=============================================================================
void InitCamera(void)
{
	g_Camera.pos = { POS_X_CAM, POS_Y_CAM, POS_Z_CAM };
	g_Camera.at  = { 0.0f, 0.0f, 0.0f };
	g_Camera.up  = { 0.0f, 1.0f, 0.0f };
	g_Camera.rot = { 0.0f, 0.0f, 0.0f };

	g_Camera.state = CAMERA_STATE_NONE;

	// 視点と注視点の距離を計算
	float vx, vz;
	vx = g_Camera.pos.x - g_Camera.at.x;
	vz = g_Camera.pos.z - g_Camera.at.z;
	g_Camera.len = sqrtf(vx * vx + vz * vz);
	
	// ビューポートタイプの初期化
	SetViewPort(g_ViewPortType);

	// 0番だけ線形補間で動かしてみる
	g_Camera.time = 0.0f;		// 線形補間用のタイマーをクリア
	g_Camera.tblNo = 0;		// 再生するアニメデータの先頭アドレスをセット
	g_Camera.tblMax = sizeof(g_MoveTbl0) / sizeof(INTERPOLATION_DATA_CAMAERA);	// 再生するアニメデータのレコード数をセット

}


//=============================================================================
// カメラの終了処理
//=============================================================================
void UninitCamera(void)
{

}


//=============================================================================
// カメラの更新処理
//=============================================================================
void UpdateCamera(void)
{
	if (g_Camera.state == CAMERA_STATE_NONE)
	{
		// 半回転(QuickTurn)させる
		if (GetKeyboardTrigger(DIK_K) || IsButtonTriggered(0, BUTTON_RS))
		{
			g_Camera.state = CAMERA_STATE_QUICKTURN;

			g_TargetRotY = g_Camera.rot.y + XM_PI;

			if (g_TargetRotY > XM_PI)
			{
				g_TargetRotY -= XM_2PI;
			}

		}

		// 視点旋回「左」
		if (GetKeyboardPress(DIK_L) || IsButtonPressed(0, STICK_R_RIGHT))
		{
			g_Camera.rot.y += VALUE_ROTATE_CAMERA;
			if (g_Camera.rot.y > XM_PI)
			{
				g_Camera.rot.y -= XM_PI * 2.0f;
			}

			g_Camera.pos.x = g_Camera.at.x - sinf(g_Camera.rot.y) * g_Camera.len;
			g_Camera.pos.z = g_Camera.at.z - cosf(g_Camera.rot.y) * g_Camera.len;
		}

		// 視点旋回「右」
		if (GetKeyboardPress(DIK_J) || IsButtonPressed(0, STICK_R_LEFT))
		{
			g_Camera.rot.y -= VALUE_ROTATE_CAMERA;
			if (g_Camera.rot.y < -XM_PI)
			{
				g_Camera.rot.y += XM_PI * 2.0f;
			}

			g_Camera.pos.x = g_Camera.at.x - sinf(g_Camera.rot.y) * g_Camera.len;
			g_Camera.pos.z = g_Camera.at.z - cosf(g_Camera.rot.y) * g_Camera.len;
		}


	}
	else if (g_Camera.state == CAMERA_STATE_QUICKTURN)
	{
			float diff = fabsf(g_TargetRotY - g_Camera.rot.y);	// マイナスを消す関数（絶対値）

			g_Camera.rot.y += diff * 0.2f;

			if (g_Camera.rot.y > XM_PI)
			{
				g_Camera.rot.y -= XM_PI * 2.0f;
			}

			if (diff < 0.05f)
			{
				g_Camera.state = CAMERA_STATE_NONE;
			}
		
	}

	// イベントシーン
	if (GetFlag() == FLAG_GAME_EVENT_FIRE)
	{
		{
			XMFLOAT3 pos = g_Camera.pos;
			XMFLOAT3 at = g_Camera.at;
			g_MoveTbl0[0].pos = pos;
			g_MoveTbl0[0].at = at;

		}

		if (g_Camera.tblMax > 0)	// 線形補間を実行する？
		{
			// 線形補間の処理
			int nowNo = (int)g_Camera.time;			// 整数分であるテーブル番号を取り出している
			int maxNo = g_Camera.tblMax;				// 登録テーブル数を数えている
			int nextNo = (nowNo + 1) % maxNo;			// 移動先テーブルの番号を求めている

			INTERPOLATION_DATA_CAMAERA* tbl = g_standbyTblAdr[g_Camera.tblNo];	// 行動テーブルのアドレスを取得

			XMVECTOR nowPos = XMLoadFloat3(&tbl[nowNo].pos);	// XMVECTORへ変換
			XMVECTOR nowAt = XMLoadFloat3(&tbl[nowNo].at);	// XMVECTORへ変換

			XMVECTOR Pos = XMLoadFloat3(&tbl[nextNo].pos) - nowPos;	// XYZ移動量を計算している
			XMVECTOR At = XMLoadFloat3(&tbl[nextNo].at) - nowAt;	// XYZ移動量を計算している

			float nowTime = g_Camera.time - nowNo;	// 時間部分である少数を取り出している

			Pos *= nowTime;								// 現在の移動量を計算している
			At *= nowTime;								// 現在の移動量を計算している

			// 計算して求めた移動量を現在の移動テーブルXYZに足している＝表示座標を求めている
			XMStoreFloat3(&g_Camera.pos, nowPos + Pos);
			XMStoreFloat3(&g_Camera.at, nowAt + At);

			// frameを使て時間経過処理をする
			g_Camera.time += 1.0f / tbl[nowNo].frame;	// 時間を進めている

			if ((int)g_Camera.time >= maxNo -1)			// 登録テーブル最後まで移動したか？
			{
				SetFlag(FLAG_GAME_EVENT_PILLAR);
				PlaySound(SOUND_LABEL_SE_PILLAR_DOWN);
				g_Camera.tblMax = -1;
				g_Camera.pos.y = POS_Y_CAM; // カメラの座標を元に戻している
			}
		}
	}
#ifdef _DEBUG

	if (GetKeyboardPress(DIK_T))
	{// 注視点移動「上」
		g_Camera.at.y += VALUE_MOVE_CAMERA;
	}

	if (GetKeyboardPress(DIK_B))
	{// 注視点移動「下」
		g_Camera.at.y -= VALUE_MOVE_CAMERA;
	}

	if (GetKeyboardPress(DIK_U))
	{// 近づく
		g_Camera.len -= VALUE_MOVE_CAMERA;
		g_Camera.pos.x = g_Camera.at.x - sinf(g_Camera.rot.y) * g_Camera.len;
		g_Camera.pos.z = g_Camera.at.z - cosf(g_Camera.rot.y) * g_Camera.len;
	}

	if (GetKeyboardPress(DIK_M))
	{// 離れる
		g_Camera.len += VALUE_MOVE_CAMERA;
		g_Camera.pos.x = g_Camera.at.x - sinf(g_Camera.rot.y) * g_Camera.len;
		g_Camera.pos.z = g_Camera.at.z - cosf(g_Camera.rot.y) * g_Camera.len;
	}

	if (GetKeyboardPress(DIK_Q))
	{// 注視点旋回「左」
		g_Camera.rot.y -= VALUE_ROTATE_CAMERA;
		if (g_Camera.rot.y < -XM_PI)
		{
			g_Camera.rot.y += XM_PI * 2.0f;
		}

		g_Camera.at.x = g_Camera.pos.x + sinf(g_Camera.rot.y) * g_Camera.len;
		g_Camera.at.z = g_Camera.pos.z + cosf(g_Camera.rot.y) * g_Camera.len;
	}

	if (GetKeyboardPress(DIK_E))
	{// 注視点旋回「右」
		g_Camera.rot.y += VALUE_ROTATE_CAMERA;
		if (g_Camera.rot.y > XM_PI)
		{
			g_Camera.rot.y -= XM_PI * 2.0f;
		}

		g_Camera.at.x = g_Camera.pos.x + sinf(g_Camera.rot.y) * g_Camera.len;
		g_Camera.at.z = g_Camera.pos.z + cosf(g_Camera.rot.y) * g_Camera.len;
	}

	if (GetKeyboardPress(DIK_Y))
	{// 視点移動「上」
		g_Camera.pos.y += VALUE_MOVE_CAMERA;
	}

	if (GetKeyboardPress(DIK_N))
	{// 視点移動「下」
		g_Camera.pos.y -= VALUE_MOVE_CAMERA;
	}

#endif



//#ifdef _DEBUG	// デバッグ情報を表示する
//	PrintDebugProc("Camera:ZC QE TB YN UM R\n");
//	PrintDebugProc("Camera.rot.y:%f\n", g_Camera.rot.y);
//	PrintDebugProc("Camera.pos.x:%f Camera.pos.y:%f Camera.pos.z:%f\n", g_Camera.pos.x,g_Camera.pos.y,g_Camera.pos.z);
//	PrintDebugProc("Camera.at.x:%f Camera.at.y:%f Camera.at.z:%f\n", g_Camera.at.x,g_Camera.at.y,g_Camera.at.z);
//	PrintDebugProc("Camera.state:%d\n", g_Camera.state);
//	PrintDebugProc("g_TargetRotY:%f\n", g_TargetRotY);
//#endif
}


//=============================================================================
// カメラの更新
//=============================================================================
void SetCamera(void) 
{
	// ビューマトリックス設定
	XMMATRIX mtxView;
	mtxView = XMMatrixLookAtLH(XMLoadFloat3(&g_Camera.pos), XMLoadFloat3(&g_Camera.at), XMLoadFloat3(&g_Camera.up));
	SetViewMatrix(&mtxView);
	XMStoreFloat4x4(&g_Camera.mtxView, mtxView);

	XMMATRIX mtxInvView;
	mtxInvView = XMMatrixInverse(nullptr, mtxView);
	XMStoreFloat4x4(&g_Camera.mtxInvView, mtxInvView);


	// プロジェクションマトリックス設定
	XMMATRIX mtxProjection;
	mtxProjection = XMMatrixPerspectiveFovLH(VIEW_ANGLE, VIEW_ASPECT, VIEW_NEAR_Z, VIEW_FAR_Z);

	SetProjectionMatrix(&mtxProjection);
	XMStoreFloat4x4(&g_Camera.mtxProjection, mtxProjection);

	SetShaderCamera(g_Camera.pos);
}


//=============================================================================
// カメラの取得
//=============================================================================
CAMERA *GetCamera(void) 
{
	return &g_Camera;
}

//=============================================================================
// ビューポートの設定
//=============================================================================
void SetViewPort(int type)
{
	ID3D11DeviceContext *g_ImmediateContext = GetDeviceContext();
	D3D11_VIEWPORT vp;

	g_ViewPortType = type;

	// ビューポート設定
		vp.Width = (FLOAT)SCREEN_WIDTH;
		vp.Height = (FLOAT)SCREEN_HEIGHT;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;

	
	g_ImmediateContext->RSSetViewports(1, &vp);

}


int GetViewPortType(void)
{
	return g_ViewPortType;
}


// カメラの視点と注視点をセット
void SetCameraAT(XMFLOAT3 pos)
{
	PLAYER* player = GetPlayer();

		// 引数で受け取った座標の右前
		g_Camera.at.x = pos.x + sinf(g_Camera.rot.y + XM_PIDIV4 * 0.5f) * 80.0f;
		g_Camera.at.y = 7.0f;
		g_Camera.at.z = pos.z + cosf(g_Camera.rot.y + XM_PIDIV4 * 0.5f) * 80.0f;

		// 引数で受け取った座標の右後
		g_Camera.pos.x = pos.x - sinf(g_Camera.rot.y - XM_PI * 0.125f) * 60.0f;
		g_Camera.pos.z = pos.z - cosf(g_Camera.rot.y - XM_PI * 0.125f) * 60.0f;
}