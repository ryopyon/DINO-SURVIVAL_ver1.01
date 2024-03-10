//=============================================================================
//
// �G�l�~�[UI���� [ui_enemy.h]
// Author : HAL���� ������
//
//=============================================================================
#pragma once


//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct
{
	XMFLOAT3 pos;           // �G�l�~�[�̈ʒu
	XMFLOAT3 hpBarPos;      // HP�o�[�̈ʒu
	XMFLOAT3 scl;           // �X�P�[��
	MATERIAL material;      // �}�e���A��
	float fWidth;           // ��
	float fHeight;          // ����
	BOOL use;               // �g�p���Ă��邩�ǂ���

} UI_ENEMY;


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitUi_Enemy(void);
void UninitUi_Enemy(void);
void UpdateUi_Enemy(void);
void DrawUi_Enemy(void);

int SetUi_Enemy(XMFLOAT3 pos, float fWidth, float fHeight, XMFLOAT4 col);

UI_ENEMY* GetUI_Enemy(void);


