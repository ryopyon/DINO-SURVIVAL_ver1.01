//=============================================================================
//
// �p�[�e�B�N������ [particle.h]
// Author : HAL���� ������
//
//=============================================================================
#pragma once


//*****************************************************************************
// �}�N����`
//*****************************************************************************
enum
{
	PARTICLE_CHARGE_RED,
	PARTICLE_CHARGE_BLUE,
	PARTICLE_CHARGE_GREEN,
	PARTICLE_CHARGE_YELLOW,
};

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
struct PARTICLE_CHARGE
{
	XMFLOAT3		pos;			// �ʒu
	XMFLOAT3		rot;			// ��]
	XMFLOAT3		scl;			// �X�P�[��
	XMFLOAT3		move;			// �ړ���
	MATERIAL		material;		// �}�e���A��
	XMFLOAT4		RGBa;			// �ێ��pRGBa
	XMFLOAT3		sPos;			// �J�n�ʒu(���`��ԗp)
	XMFLOAT3		ePos;			// �ڕW�ʒu(���`��ԗp)
	float			lerpTime;		// ���`��ԗp�̎���
	int				type;			// �^�C�v
	int				life;			// ����
	BOOL			use;			// �g�p���Ă��邩�ǂ���
};


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT		InitParticleCharge(void);
void		UninitParticleCharge(void);
void		UpdateParticleCharge(void);
void		DrawParticleCharge(void);

int			SetParticleCharge(XMFLOAT3 pos, XMFLOAT3 ePos, XMFLOAT3 scl, XMFLOAT4 col, int type, int life);
void		SetColorParticleCharge(int index, XMFLOAT4 color);

