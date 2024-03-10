//=============================================================================
//
// タイトル画面処理 [title.h]
// Author : HAL東京 高橋諒
//
//=============================================================================
#pragma once

#include "main.h"
#include "renderer.h"
#include "debugproc.h"
#include "sprite.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************
enum 
{
	TITLE_LOGO,
	TITLE_TITLE,
	TITLE_TITLELOGO,
	TITLE_EFFECT,
	TITLE_EYELLOW,
	TITLE_EGREEN,
	TITLE_EBLUE,
	TITLE_ERED,
	TITLE_FADE_BLACK,

	TITLE_TEX_MAX

};

enum 
{
	TUTORIAL,
	GAMESTART,
	CREDIT,
	EXIT,

	TITLE_MAX,
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitTitle(void);
void UninitTitle(void);
void UpdateTitle(void);
void DrawTitle(void);


