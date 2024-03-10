//=============================================================================
//
// ���f������ [player.h]
// Author : HAL���� ������
//
//=============================================================================
#pragma once
#include "model.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_PLAYER							(1)					// �v���C���[�̐�
#define MAX_PLAYER_TYPE						(4)					// �v���C���[�̎�ސ�

#define	PLAYER_SIZE							(18.0f)				// �����蔻��̑傫��

#define	PLAYER_HP_MAX						(500)				// HP�ő�l
#define PLAYER_MP_MAX						(600)				// MP�ő�l

#define	PLAYER_RED_DAMAGE_VALUE				(200)				// �v���C���[�i�ԁj���^����_���[�W��
#define	PLAYER_BLUE_DAMAGE_VALUE			(300)				// �v���C���[�i�j���^����_���[�W��
#define	PLAYER_GREEN_DAMAGE_VALUE			(200)				// �v���C���[�i�΁j���^����_���[�W��
#define	PLAYER_YELLOW_DAMAGE_VALUE			(300)				// �v���C���[�i���j���^����_���[�W��

#define HP_GAIN_FRAME						(10)				// HP�̉񕜑��x
#define MP_GAIN_FRAME						(10)				// MP�̉񕜑��x
#define CHARGE_CNT_MAX						(60)				// �`���[�W����

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
struct PLAYER
{
	XMFLOAT4X4			mtxWorld;			// ���[���h�}�g���b�N�X
	XMFLOAT3			pos;				// ���f���̈ʒu
	XMFLOAT3			rot;				// ���f���̌���(��])
	XMFLOAT3			scl;				// ���f���̑傫��(�X�P�[��)
	float				dir;				// ����

	int					hp;					// hp
	int					mp;					// MP

	int					damage;				// �v���C���[���^����_���[�W��

	float				spd;				// �ړ��X�s�[�h
	float				size;				

	int					shadowIdx;			// �e�̃C���f�b�N�X�ԍ�

	BOOL				use;

	int					type;				// �v���C���[�̐F�@�^�C�v
	int					state;				// ���
	int					setTbl;				// �Z�b�g����Ă���e�[�u��
	XMFLOAT4			hinokoColor;		// �Ђ̂��̐F
	int					HPGainCnt;			// HP�񕜗�
	int					MPGainCnt;			// MP�񕜗�
	int					chargeCnt;			// �`���[�W����
	int					changeCnt;			// �v���C���[���N�[���^�C�������p�̃J�E���g

	int					modelIdx;


	// �K�w�A�j���[�V�����p�̃����o�[�ϐ�
	float				time;				// ���`��ԗp
	int					tblNo;				// �s���f�[�^�̃e�[�u���ԍ�
	int					tblMax;				// ���̃e�[�u���̃f�[�^��

	// �e�́ANULL�A�q���͐e�̃A�h���X������
	PLAYER				*parent;			// �������e�Ȃ�NULL�A�������q���Ȃ�e��player�A�h���X

	// �N�H�[�^�j�I��
	XMFLOAT4			Quaternion;

	XMFLOAT3			UpVector;			// �����������Ă��鏊


	BOOL moving;
	BOOL attack;

	// dissolve
	DISSOLVE		dissolve;

};

struct PLAYER_MODEL
{
	BOOL				load;
	DX11_MODEL			model;				// ���f�����
	XMFLOAT4			diffuse[MODEL_MAX_MATERIAL];	// ���f���̐F
};

enum
{
	STATE_IDLE,
	STATE_WALK,
	STATE_HINOKO,
	STATE_DIE,

	STATE_MAX
};

enum PLAYER_COLOR
{
	PLAYER_RED,
	PLAYER_BLUE,
	PLAYER_GREEN,
	PLAYER_YELLOW,

	PLAYER_COLOR_MAX
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);

PLAYER *GetPlayer(void);

BOOL Collision(int index, float r, XMFLOAT3 epos, float er);

BOOL MoveLeftStick(void);
BOOL MoveRightStick(void);
BOOL MoveUpStick(void);
BOOL MoveDownStick(void);

//�L�[�{�[�h
BOOL AnyKeyPress(void);
BOOL MoveLeftKeyboard(void);
BOOL MoveRightKeyboard(void);
BOOL MoveUpKeyboard(void);
BOOL MoveDownKeyboard(void);

//�Q�[���p�b�g
BOOL AnyStickpress(void);
BOOL MoveLeftStick(void);
BOOL MoveRightStick(void);
BOOL MoveUpStick(void);
BOOL MoveDownStick(void);


void ChangeModelPlayer(int type, int id);
