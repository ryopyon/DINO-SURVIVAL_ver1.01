//=============================================================================
//
// �e���ˏ��� [bullet.h]
// Author : HAL���� ������
//
//=============================================================================
#pragma once
#include "main.h"
#include "renderer.h"
#include "model.h"


//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MAX_BULLET				(256)		// �e�ő吔
#define MODEL_MAX_MATERIAL		(16)		// �P���f����Max�}�e���A����

#define	BULLET_RANGE_RED		(150.0f)	// �e�̔򋗗�
#define	BULLET_RANGE_BLUE		(300.0f)
#define	BULLET_RANGE_GREEN		(200.0f)
#define	BULLET_RANGE_YELLOW		(400.0f)

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

} BULLET;

enum BULLET_TYPE
{
	BULLET_RED,
	BULLET_BLUE,
	BULLET_GREEN,
	BULLET_YELLOW,

	BULLET_NONE,

};


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitBullet(void);
void UninitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);

int SetBullet(XMFLOAT3 pos, XMFLOAT3 rot);
int SetBullet(XMFLOAT3 pos, XMFLOAT3 rot, XMFLOAT4 color, int type);
void ReleaseBullet(int id);

BULLET* GetBullet(void);

XMVECTOR LinearInterpolation(XMVECTOR P0, XMVECTOR P1, float s);
XMVECTOR QuadraticBezierCurve(XMVECTOR p0, XMVECTOR p1, XMVECTOR p2, float s);