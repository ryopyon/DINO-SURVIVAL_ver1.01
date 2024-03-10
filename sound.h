//=============================================================================
//
// サウンド処理 [sound.h]
// Author : HAL東京 高橋諒
// 
//=============================================================================
#pragma once

#include <windows.h>
#include "xaudio2.h"						// サウンド処理で必要

//*****************************************************************************
// サウンドファイル
//*****************************************************************************
enum
{
	SOUND_LABEL_BGM_TITLE,		// title
	SOUND_LABEL_BGM_BGM2,		// BGM2
	SOUND_LABEL_BGM_GAME,		// game
	SOUND_LABEL_BGM_RESULT,		// result
	SOUND_LABEL_SE_FIRE,		// 火
	SOUND_LABEL_SE_CHARGE,		// チャージ音
	SOUND_LABEL_SE_EXPROSION,	// 貯め攻撃 火
	SOUND_LABEL_SE_BUBBLE,		// 貯め攻撃 水
	SOUND_LABEL_SE_WIND,		// 貯め攻撃 風
	SOUND_LABEL_SE_THUNDER,		// 貯め攻撃 雷
	SOUND_LABEL_SE_KETTEI,		// 決定音
	SOUND_LABEL_SE_CRICK,		// カーソル操作音
	SOUND_LABEL_SE_PILLAR_DOWN,	// 柱が下がる時の音
	SOUND_LABEL_SE_DAMAGE,		// ダメージ音
	SOUND_LABEL_SE_CHANGE,		// プレイヤーチェンジ音


	SOUND_LABEL_MAX,
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
bool InitSound(HWND hWnd);
void UninitSound(void);
void PlaySound(int label);
void StopSound(int label);
void StopSound(void);

void SetVolumeSound(float volume);
void SetVolumeSound(int label, float volume);

void SetFrequencyRatio(float pitch);	//速度調整
void SetFrequencyRatio(int label, float pitch);

void SetOutputMatrixSound(int label, float left, float right);	//LRの振り分け
