//=============================================================================
//
// �p�[�e�B�N������ [particle.h]
// Author : HAL���� ������
//
//=============================================================================
#pragma once

#define	MAX_PARTICLE		(2048)		// �p�[�e�B�N���ő吔

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct
{
	XMFLOAT3		pos;			// �ʒu
	XMFLOAT3		rot;			// ��]
	XMFLOAT3		scale;			// �X�P�[��
	XMFLOAT3		move;			// �ړ���
	float			angle;			// ���ʁi�Ηp�j
	float			radius;			// ���ʁi�Ηp�j
	XMFLOAT3		basepos;		// ��钆�S�i�Ηp�j
	MATERIAL		material;		// �}�e���A��
	XMFLOAT4		RGBa;			// �ۑ��pRGBa
	float			fSizeX;			// ��
	float			fSizeY;			// ����
	int				nIdxShadow;		// �eID
	int				nLife;			// ����
	int				type;
	BOOL			use;			// �g�p���Ă��邩�ǂ���
	BLEND_MODE		brendmode;


} PARTICLE;


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
enum TYPE
{
	TYPE_HINOKO_RED,
	TYPE_HINOKO_BLUE,
	TYPE_HINOKO_GREEN,
	TYPE_HINOKO_YELLOW,

	TYPE_HINOKO_CHARGE_RED,
	TYPE_HINOKO_CHARGE_BLUE,
	TYPE_HINOKO_CHARGE_GREEN,
	TYPE_HINOKO_CHARGE_YELLOW,

	TYPE_CHANGE_PLAYER,
	TYPE_SNOW,
	TYPE_ATTACK,
};

HRESULT InitParticle(void);
void UninitParticle(void);
void UpdateParticle(void);
void DrawParticle(void);

int SetParticle(XMFLOAT3 pos, XMFLOAT3 scale, XMFLOAT3 move, XMFLOAT4 col, int type, BLEND_MODE brendmode, float fSizeX, float fSizeY, int nLife);
int SetParticle(XMFLOAT3 pos, XMFLOAT3 scale, XMFLOAT3 move, XMFLOAT4 col, int type, BLEND_MODE brendmode, float fSizeX, float fSizeY, int nLife, XMFLOAT3 basepos, float angle);
void SetColorParticle(int nIdxParticle, XMFLOAT4 col);
PARTICLE* GetParticle(void);

