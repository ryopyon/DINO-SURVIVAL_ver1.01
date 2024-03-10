//=============================================================================
//
// �e���ˏ��� [bullet_charge.h]
// Author : HAL���� ������
//
//=============================================================================
#pragma once
#include "main.h"
#include "renderer.h"
#include "model.h"
#include "bullet.h"



//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MAX_BULLET_CHARGE				(256)	// �e�ő吔
#define MODEL_MAX_MATERIAL				(16)		// �P���f����Max�}�e���A����

#define	BULLET_CHARGE_RANGE_RED			(150.0f)	// �e�̔򋗗�
#define	BULLET_CHARGE_RANGE_BLUE		(300.0f)
#define	BULLET_CHARGE_RANGE_GREEN		(200.0f)
#define	BULLET_CHARGE_RANGE_YELLOW		(100.0f)

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct
{
	XMFLOAT3	pos;			// �ʒu
	XMFLOAT3	rot;			// �p�x
	float		spd;			// �ړ���
	int			shadowIdx;		// �eID
	BOOL		bUse;			// �g�p���Ă��邩�ǂ���

	float		size;			// �����蔻��̑傫��

	XMFLOAT3	P0;				//����_�o�P
	XMFLOAT3	P1;				//����_�o�P
	XMFLOAT3	Tpos;
	float		s;				//���`��ԗp�p�����[�^��

	int			type;
	XMFLOAT4	color;

} BULLET_CHARGE;

enum BULLET_CHARGE_TYPE
{
	BULLET_CHARGE_RED,
	BULLET_CHARGE_BLUE,
	BULLET_CHARGE_GREEN,
	BULLET_CHARGE_YELLOW,

	BULLET_CHARGE_NONE,

};


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitBullet_Charge(void);
void UninitBullet_Charge(void);
void UpdateBullet_Charge(void);
void DrawBullet_Charge(void);

int SetBullet_Charge(XMFLOAT3 pos, XMFLOAT3 rot);
void ReleaseBullet_Charge(int id);

BULLET_CHARGE* GetBullet_Charge(void);