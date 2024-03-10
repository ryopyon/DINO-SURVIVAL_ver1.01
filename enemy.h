//=============================================================================
//
// �G�l�~�[���f������ [enemy.h]
// Author : HAL���� ������
//
//=============================================================================
#pragma once
#include "model.h"


//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_ENEMY				(40)					// �G�l�~�[�̐�
#define MAX_ENEMY_TYPE			(4)						// �G�l�~�[�̎�ސ�

#define	ENEMY_SIZE				(5.0f)					// �����蔻��̑傫��

#define	ENEMY_HP_MAX			(600)					// HP�ő�l

//#define	ENEMY_DAMAGE_VALUE		(100)					// �G�l�~�[���^����_���[�W��
#define	ENEMY_DAMAGE_VALUE		(170)					// �G�l�~�[���^����_���[�W��

#define	ENEMY_RED_SPD			(0.4)					// �G�l�~�[�i�ԁj�̃X�s�[�h
#define	ENEMY_BLUE_SPD			(0.8)					// �G�l�~�[�i�j�̃X�s�[�h
#define	ENEMY_GREEN_SPD			(1.2)					// �G�l�~�[�i�΁j�̃X�s�[�h
#define	ENEMY_YELLOW_SPD		(1.6)					// �G�l�~�[�i���j�̃X�s�[�h

#define	ENEMY_SPAWN_RANGE_X		(1100)					// �G�l�~�[�̎��������͈�
#define	ENEMY_SPAWN_RANGE_Z		(1100)				

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
struct ENEMY
{
	XMFLOAT4X4			mtxWorld;			// ���[���h�}�g���b�N�X
	XMFLOAT3			pos;				// ���f���̈ʒu
	XMFLOAT3			rot;				// ���f���̌���(��])
	XMFLOAT3			scl;				// ���f���̑傫��(�X�P�[��)
	float				spd;				// �ړ��X�s�[�h
	float				size;				// �����蔻��̑傫��

	BOOL				use;
	BOOL				EntryTime;			// �o��V�[��
	BOOL				LeaveTime;			// �ޏ�V�[��

	float				burnTime;			// �Ώ�Ԏ���
	float				wetTime;			// ����Ԏ���
	float				windyTime;			// ����Ԏ���
	float				shockTime;			// ����Ԏ���

	int					hp;					// hp

	int					damage;				// �G�l�~�[���^����_���[�W��

	int					type;
	int					state;
	int					shadowIdx;			// �e�̃C���f�b�N�X�ԍ�

	ENEMY* parent;			// �������e�Ȃ�NULL�A�������q���Ȃ�e��player�A�h���X

	float				time;				// ���`��ԗp
	int					tblNo;				// �s���f�[�^�̃e�[�u���ԍ�
	int					tblMax;				// ���̃e�[�u���̃f�[�^��

	int					modelIdx;

	// dissolve
	DISSOLVE			dissolve;

};

struct ENEMY_MODEL
{
	BOOL				load;
	DX11_MODEL			model;				// ���f�����
	XMFLOAT4			diffuse[MODEL_MAX_MATERIAL];	// ���f���̐F
};

enum ENEMY_COLOR
{
	ENEMY_RED,
	ENEMY_BLUE,
	ENEMY_GREEN,
	ENEMY_YELLOW,

	ENEMY_COLOR_MAX
};


enum
{
	ENEMY_ABNORMAL_NONE,
	ENEMY_ABNORMAL_BURN,
	ENEMY_ABNORMAL_WET,
	ENEMY_ABNORMAL_WINDY,
	ENEMY_ABNORMAL_SHOCK,

};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);

ENEMY *GetEnemy(void);

void SetEnemy(void);

void ChangeModelEnemy(int type, int id);
