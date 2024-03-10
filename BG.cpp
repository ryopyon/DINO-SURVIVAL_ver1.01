//=============================================================================
//
// BG���f������ [BG.cpp]
// Author : HAL���� ������
//
//=============================================================================
#include "main.h"
#include "model.h"

#include "renderer.h"
#include "input.h"
#include "camera.h"
#include "debugproc.h"
#include "BG.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MODEL_BG		"data/MODEL/BG/BG.obj"			// �ǂݍ��ރ��f����

#define MAX_BG			(1)

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static BG							g_BG;					// �|���S���f�[�^
static BG_MODEL						g_BGModel;				// �|���S���f�[�^
static int							g_TexNo;				// �e�N�X�`���ԍ�



//=============================================================================
// ����������
//=============================================================================
HRESULT InitBG(void)
{

	LoadModel(MODEL_BG, &g_BGModel.model);


	// �ʒu�E��]�E�X�P�[���̏����ݒ�
	g_BG.pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_BG.rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_BG.scl = XMFLOAT3(300.0f, 300.0f, 300.0f);
	g_BG.modelIdx = NULL;

	g_BGModel.load = TRUE;
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitBG(void)
{

	// ���f���̉������
	if (g_BGModel.load)
	{
		UnloadModel(&g_BGModel.model);
		g_BGModel.load = false;

	}
}
//=============================================================================
// �X�V����
//=============================================================================
void UpdateBG(void)
{
	g_BG.rot.y += 0.001f;	// ������Ƃ�����
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawBG(void)
{
	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

	//�w�i�̕`��

	// �J�����O����
	SetCullingMode(CULL_MODE_FRONT);

	//���[���h�}�g���b�N�X�̏�����y
	mtxWorld = XMMatrixIdentity();

	// �X�P�[���𔽉f
	mtxScl = XMMatrixScaling(g_BG.scl.x, g_BG.scl.y, g_BG.scl.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

	// ��]�𔽉f
	mtxRot = XMMatrixRotationRollPitchYaw(g_BG.rot.x, g_BG.rot.y, g_BG.rot.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

	// �ړ��𔽉f
	mtxTranslate = XMMatrixTranslation(g_BG.pos.x, g_BG.pos.y, g_BG.pos.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

	// ���[���h�}�g���b�N�X�̐ݒ�
	SetWorldMatrix(&mtxWorld);

	XMStoreFloat4x4(&g_BG.mtxWorld, mtxWorld);

	//���f���`��
	DrawModel(&g_BGModel.model);

	// �J�����O����
	SetCullingMode(CULL_MODE_BACK);
}