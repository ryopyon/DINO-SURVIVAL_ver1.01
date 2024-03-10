//=============================================================================
//
// エネミーUI処理 [ui_enemy.h]
// Author : HAL東京 高橋諒
//
//=============================================================================
#pragma once


//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	XMFLOAT3 pos;           // エネミーの位置
	XMFLOAT3 hpBarPos;      // HPバーの位置
	XMFLOAT3 scl;           // スケール
	MATERIAL material;      // マテリアル
	float fWidth;           // 幅
	float fHeight;          // 高さ
	BOOL use;               // 使用しているかどうか

} UI_ENEMY;


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitUi_Enemy(void);
void UninitUi_Enemy(void);
void UpdateUi_Enemy(void);
void DrawUi_Enemy(void);

int SetUi_Enemy(XMFLOAT3 pos, float fWidth, float fHeight, XMFLOAT4 col);

UI_ENEMY* GetUI_Enemy(void);


