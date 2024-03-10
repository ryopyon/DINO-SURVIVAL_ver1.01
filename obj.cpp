//=============================================================================
//
// obj���f������ [0bj.cpp]
// Author : HAL���� ������
//
//=============================================================================
#include "main.h"
#include "model.h"

#include "renderer.h"
#include "input.h"
#include "camera.h"
#include "debugproc.h"
#include "obj.h"
#include "player.h"
#include "event.h"
#include "sound.h"
#include "flag.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MODEL_PILLAR		"data/MODEL/pillar/pillar.obj"			// �ǂݍ��ރ��f����

#define MAX_PILLAR		(4)

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static OBJ							g_Obj;				// �|���S���f�[�^
static OBJ_MODEL					g_ObjModel;				// �|���S���f�[�^
static int							g_TexNo;				// �e�N�X�`���ԍ�



//=============================================================================
// ����������
//=============================================================================
HRESULT InitObj(void)
{

	LoadModel(MODEL_PILLAR, &g_ObjModel.model);


	// �ʒu�E��]�E�X�P�[���̏����ݒ�
	g_Obj.pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_Obj.rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_Obj.scl = XMFLOAT3(10.0f, 10.0f, 10.0f);
	g_Obj.use = TRUE;

	g_Obj.modelIdx = NULL;

	g_TexNo = 0;
	g_ObjModel.load = TRUE;
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitObj(void)
{

	// ���f���̉������
	if (g_ObjModel.load)
	{
		UnloadModel(&g_ObjModel.model);
		g_ObjModel.load = false;

	}
}
//=============================================================================
// �X�V����
//=============================================================================
void UpdateObj(void)
{
	CAMERA* cam = GetCamera();
	PLAYER* player = GetPlayer();
	EVENT* HI = GetEvent();

	if (g_Obj.use == FALSE)return;

	if (GetFlag() == FLAG_GAME_EVENT_PILLAR)
	{
		float x = (float)(rand() % 4 - 2) * 0.1f;
		float z = (float)(rand() % 4 - 2) * 0.1f;

		g_Obj.pos.x -= x;
		g_Obj.pos.y -= 0.3f;
		g_Obj.pos.z -= z;

		g_Obj.rot.y += 0.1f;
	}

	if (g_Obj.pos.y <= -10.0f)
	{
		for (int i = 0; i < MAX_HI; i++)
		{
			HI[i].use = FALSE;
		}
	}

	if (g_Obj.pos.y <= -70.0f)
	{
		SetFlag(FLAG_GAME_BATTLE);

		// �o�g���ɓ����������ɂ��ǂ�
		player->hp = PLAYER_HP_MAX;
		player->mp = PLAYER_MP_MAX;
		cam->pos.y = POS_Y_CAM; // �J�����̍��W�����ɖ߂��Ă���

		// BGM�Đ�
		PlaySound(SOUND_LABEL_BGM_GAME);

		g_Obj.use = FALSE;
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawObj(void)
{
	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;

	for (int i = 0; i < MAX_PILLAR; i++)
	{
		//���̕`��
		//���[���h�}�g���b�N�X�̏�����y
		mtxWorld = XMMatrixIdentity();

		// �X�P�[���𔽉f
		mtxScl = XMMatrixScaling(1.0, 1.0, 1.0);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// ��]�𔽉f
		mtxRot = XMMatrixRotationRollPitchYaw(g_Obj.rot.x, g_Obj.rot.y, g_Obj.rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// �ړ��𔽉f
		mtxTranslate = XMMatrixTranslation(g_Obj.pos.x - 50.0f, g_Obj.pos.y, g_Obj.pos.z - 300.0f + (i * 100.0f));
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ���[���h�}�g���b�N�X�̐ݒ�
		SetWorldMatrix(&mtxWorld);

		XMStoreFloat4x4(&g_Obj.mtxWorld, mtxWorld);

		//���f���`��
		DrawModel(&g_ObjModel.model);
	}



	for (int i = 0; i < MAX_PILLAR; i++)
	{
		//���̕`��
		//���[���h�}�g���b�N�X�̏�����y
		mtxWorld = XMMatrixIdentity();

		// �X�P�[���𔽉f
		mtxScl = XMMatrixScaling(1.0, 1.0, 1.0);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// ��]�𔽉f
		mtxRot = XMMatrixRotationRollPitchYaw(g_Obj.rot.x, g_Obj.rot.y, g_Obj.rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// �ړ��𔽉f
		mtxTranslate = XMMatrixTranslation(g_Obj.pos.x + 50.0f, g_Obj.pos.y, g_Obj.pos.z - 300.0f + (i * 100.0f));
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ���[���h�}�g���b�N�X�̐ݒ�
		SetWorldMatrix(&mtxWorld);

		XMStoreFloat4x4(&g_Obj.mtxWorld, mtxWorld);

		//���f���`��
		DrawModel(&g_ObjModel.model);
	}
}


OBJ* GetObj(void)
{
	return &g_Obj;
}
