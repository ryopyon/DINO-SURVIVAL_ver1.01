//=============================================================================
//
// �T�E���h���� [sound.h]
// Author : HAL���� ������
// 
//=============================================================================
#pragma once

#include <windows.h>
#include "xaudio2.h"						// �T�E���h�����ŕK�v

//*****************************************************************************
// �T�E���h�t�@�C��
//*****************************************************************************
enum
{
	SOUND_LABEL_BGM_TITLE,		// title
	SOUND_LABEL_BGM_BGM2,		// BGM2
	SOUND_LABEL_BGM_GAME,		// game
	SOUND_LABEL_BGM_RESULT,		// result
	SOUND_LABEL_SE_FIRE,		// ��
	SOUND_LABEL_SE_CHARGE,		// �`���[�W��
	SOUND_LABEL_SE_EXPROSION,	// ���ߍU�� ��
	SOUND_LABEL_SE_BUBBLE,		// ���ߍU�� ��
	SOUND_LABEL_SE_WIND,		// ���ߍU�� ��
	SOUND_LABEL_SE_THUNDER,		// ���ߍU�� ��
	SOUND_LABEL_SE_KETTEI,		// ���艹
	SOUND_LABEL_SE_CRICK,		// �J�[�\�����쉹
	SOUND_LABEL_SE_PILLAR_DOWN,	// ���������鎞�̉�
	SOUND_LABEL_SE_DAMAGE,		// �_���[�W��
	SOUND_LABEL_SE_CHANGE,		// �v���C���[�`�F���W��


	SOUND_LABEL_MAX,
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
bool InitSound(HWND hWnd);
void UninitSound(void);
void PlaySound(int label);
void StopSound(int label);
void StopSound(void);

void SetVolumeSound(float volume);
void SetVolumeSound(int label, float volume);

void SetFrequencyRatio(float pitch);	//���x����
void SetFrequencyRatio(int label, float pitch);

void SetOutputMatrixSound(int label, float left, float right);	//LR�̐U�蕪��
