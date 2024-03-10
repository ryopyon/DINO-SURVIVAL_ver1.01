//=============================================================================
//
// �{�X���f������ [boss.cpp]
// Author : GP11B132 14 ������GP11B132 14 �����@��
//
//=============================================================================
#include "main.h"
#include "model.h"
#include "renderer.h"
#include "input.h"
#include "debugproc.h"
#include "boss.h"
#include "shadow.h"
#include "light.h"
#include "particle.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MODEL_BOSS			"data/MODEL/boss/boss.obj"	// �ǂݍ��ރ��f����

#define	VALUE_MOVE			(5.0f)						// �ړ���
#define	VALUE_ROTATE		(XM_PI * 0.02f)				// ��]��

#define BOSS_SHADOW_SIZE	(0.4f)						// �e�̑傫��
#define BOSS_OFFSET_Y		(100.0f)					// �{�X�̑��������킹��



//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static BOSS			g_Boss[MAX_BOSS];				// �G�l�~�[

int g_Boss_load = 0;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitBoss(void)
{
	for (int i = 0; i < MAX_BOSS; i++)
	{
		LoadModel(MODEL_BOSS, &g_Boss[i].model);


		g_Boss[i].load = TRUE;

		g_Boss[i].pos = XMFLOAT3(0.0f, BOSS_OFFSET_Y, 800.0f);
		g_Boss[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Boss[i].scl = XMFLOAT3(0.5f, 0.4f, 0.4f);

		g_Boss[i].spd = 0.0f;			// �ړ��X�s�[�h�N���A
		g_Boss[i].size = BOSS_SIZE;	// �����蔻��̑傫��

		g_Boss[i].use = TRUE;		// TRUE:�����Ă�

	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitBoss(void)
{

	for (int i = 0; i < MAX_BOSS; i++)
	{
		if (g_Boss[i].load)
		{
			UnloadModel(&g_Boss[i].model);
			g_Boss[i].load = FALSE;
		}
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateBoss(void)
{
#ifdef _DEBUG

	if (GetKeyboardTrigger(DIK_P))
	{
		// ���f���̐F��ύX
		for (int j = 0; j < g_Boss[0].model.SubsetNum; j++)
		{
			SetModelDiffuse(&g_Boss[0].model, j, XMFLOAT4(1.0f, 0.0f, 0.0f, 0.5f));
		}
	}

	if (GetKeyboardTrigger(DIK_L))
	{
		// ���f���̐F�����ɖ߂��Ă���
		for (int j = 0; j < g_Boss[0].model.SubsetNum; j++)
		{
			SetModelDiffuse(&g_Boss[0].model, j, g_Boss[0].diffuse[j]);
		}
	}


#endif


}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawBoss(void)
{
	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

	// �J�����O����
	SetCullingMode(CULL_MODE_NONE);



	// ���C�e�B���O�𖳌�
	SetLightEnable(FALSE);

	for (int i = 0; i < MAX_BOSS; i++)
	{
		//boss
		if (g_Boss[i].use == TRUE)
		{
			// ���C�e�B���O�𖳌�
			SetLightEnable(TRUE);

			// ���[���h�}�g���b�N�X�̏�����
			mtxWorld = XMMatrixIdentity();

			// �X�P�[���𔽉f
			mtxScl = XMMatrixScaling(g_Boss[i].scl.x * 30, g_Boss[i].scl.y * 30, g_Boss[i].scl.z * 30);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

			// ��]�𔽉f
			mtxRot = XMMatrixRotationRollPitchYaw(g_Boss[i].rot.x, g_Boss[i].rot.y + XM_PI, g_Boss[i].rot.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

			// �ړ��𔽉f
			mtxTranslate = XMMatrixTranslation(g_Boss[i].pos.x, g_Boss[i].pos.y - 50.0f, g_Boss[i].pos.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

			// ���[���h�}�g���b�N�X�̐ݒ�
			SetWorldMatrix(&mtxWorld);

			XMStoreFloat4x4(&g_Boss[i].mtxWorld, mtxWorld);


			// ���f���`��
			DrawModel(&g_Boss[0].model);
		}
	}
	// �J�����O�ݒ��߂�
	SetCullingMode(CULL_MODE_BACK);
	// ���C�e�B���O�𖳌�
	SetLightEnable(TRUE);

}

//=============================================================================
// �{�X�̎擾
//=============================================================================
BOSS* GetBoss()
{
	return &g_Boss[0];
}
