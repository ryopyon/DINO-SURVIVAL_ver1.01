//=============================================================================
//
// �^�C�g����ʏ��� [title.h]
// Author : HAL���� ������
//
//=============================================================================
#pragma once

#include "main.h"
#include "renderer.h"
#include "debugproc.h"
#include "sprite.h"


//*****************************************************************************
// �}�N����`
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
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitTitle(void);
void UninitTitle(void);
void UpdateTitle(void);
void DrawTitle(void);


