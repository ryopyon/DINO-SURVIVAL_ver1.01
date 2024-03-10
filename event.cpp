//=============================================================================
//
// �C�x���g���� [event.cpp]
// Author : HAL���� ������
//
//=============================================================================
#include "main.h"
#include "model.h"
#include "renderer.h"
#include "input.h"
#include "debugproc.h"
#include "event.h"
#include "shadow.h"
#include "light.h"
#include "particle.h"
#include "fade.h"
#include "flag.h"
#include "camera.h"
#include "sound.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	VALUE_MOVE			(5.0f)						// �ړ���
#define	VALUE_ROTATE		(XM_PI * 0.02f)				// ��]��

#define HI_SHADOW_SIZE		(0.4f)						// �e�̑傫��
#define HI_OFFSET_Y			(5.9f)						// �G�l�~�[�̑��������킹��

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static EVENT			g_Hi[MAX_HI];				// �΂̐�

// �C�x���g�p�̐��`�ۊǃf�[�^
static INTERPOLATION_DATA g_MoveTbl0[] = {
	//���W								��]��							�g�嗦						�F
	{ XMFLOAT3( 0.0f,  7.0f,  -350.0f), 	XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f),	120 },
	{ XMFLOAT3( 0.0f,  7.0f,  -350.0f), 	XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f),	120 },

	{ XMFLOAT3(50.0f,  50.0f, -300.0f), 	XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f),	60 },
	{ XMFLOAT3(50.0f,  45.0f, -300.0f), 	XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f),	10 },
	{ XMFLOAT3(50.0f,  50.0f, -300.0f), 	XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f),	60 },

	{ XMFLOAT3(-50.0f, 50.0f, -300.0f), 	XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f),	60 },
	{ XMFLOAT3(-50.0f, 45.0f, -300.0f),		XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f), 10 },
	{ XMFLOAT3(-50.0f, 50.0f, -300.0f),		XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f),	60 },

	{ XMFLOAT3(-50.0f, 50.0f, -200.0f),		XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(-50.0f, 45.0f, -200.0f),		XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f), 10 },
	{ XMFLOAT3(-50.0f, 50.0f, -200.0f),		XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f),	60 },

	{ XMFLOAT3( 50.0f, 50.0f, -200.0f),		XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f),	60 },
	{ XMFLOAT3( 50.0f, 45.0f, -200.0f),		XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f),	10 },
	{ XMFLOAT3( 50.0f, 50.0f, -200.0f),		XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f),	60 },

	{ XMFLOAT3( 50.0f, 50.0f, -100.0f),		XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3( 50.0f, 45.0f, -100.0f),		XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f), 10 },
	{ XMFLOAT3( 50.0f, 50.0f, -100.0f),		XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },

	{ XMFLOAT3(-50.0f, 50.0f, -100.0f),		XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(-50.0f, 45.0f, -100.0f),		XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f), 10 },
	{ XMFLOAT3(-50.0f, 50.0f, -100.0f),		XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },

	{ XMFLOAT3(-50.0f, 50.0f,  0.0f),		XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(-50.0f, 45.0f,  0.0f),		XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f), 10 },
	{ XMFLOAT3(-50.0f, 50.0f,  0.0f),		XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },

	{ XMFLOAT3( 50.0f, 50.0f,  0.0f),		XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3( 50.0f, 45.0f,  0.0f),		XMFLOAT3(0.0f, 0.0f, 0.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f),  1 },
};

int g_cnt;


static INTERPOLATION_DATA* g_standbyTblAdr[] =
{
	g_MoveTbl0,
};


int g_Hi_load = 0;
int g_lightF;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitEvent(void)
{


	for (int i = 0; i < MAX_HI; i++)
	{
		g_Hi[i].pos = XMFLOAT3(0.0f, HI_OFFSET_Y, -350.0f);
		g_Hi[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Hi[i].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);

		g_Hi[i].spd  = 0.0f;			// �ړ��X�s�[�h�N���A
		g_Hi[i].size = HI_SIZE;			// �����蔻��̑傫��

		// 0�Ԃ������`��Ԃœ������Ă݂�
		g_Hi[i].time = 0.0f;		// ���`��ԗp�̃^�C�}�[���N���A
		g_Hi[i].tblNo = 0;		// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
		g_Hi[i].tblMax = 0;	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

		g_Hi[i].use = FALSE;		// TRUE:�����Ă�

	}

	// 0�Ԃ������`��Ԃœ������Ă݂�
	g_Hi[0].time = 0.0f;		// ���`��ԗp�̃^�C�}�[���N���A
	g_Hi[0].tblNo = 0;		// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Hi[0].tblMax = sizeof(g_MoveTbl0) / sizeof(INTERPOLATION_DATA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g
	g_Hi[0].use = TRUE;

	g_cnt = 0;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitEvent(void)
{

}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateEvent(void)
{
	for (int i = 0; i < MAX_HI; i++)
	{
		if (g_Hi[i].use == TRUE)		// ���̉΂��g���Ă���H
		{								// Yes
			if (g_Hi[i].tblMax > 0 && GetFlag() == FLAG_GAME_EVENT_FIRE)	// ���`��Ԃ����s����H
			{	// ���`��Ԃ̏���
				int nowNo = (int)g_Hi[i].time;			// �������ł���e�[�u���ԍ������o���Ă���
				int maxNo = g_Hi[i].tblMax;				// �o�^�e�[�u�����𐔂��Ă���
				int nextNo = (nowNo + 1) % maxNo;			// �ړ���e�[�u���̔ԍ������߂Ă���

				INTERPOLATION_DATA* tbl = g_standbyTblAdr[g_Hi[i].tblNo];	// �s���e�[�u���̃A�h���X���擾

				XMVECTOR nowPos = XMLoadFloat3(&tbl[nowNo].pos);	// XMVECTOR�֕ϊ�

				XMVECTOR Pos = XMLoadFloat3(&tbl[nextNo].pos) - nowPos;	// XYZ�ړ��ʂ��v�Z���Ă���

				float nowTime = g_Hi[i].time - nowNo;	// ���ԕ����ł��鏭�������o���Ă���

				Pos *= nowTime;								// ���݂̈ړ��ʂ��v�Z���Ă���

				// �v�Z���ċ��߂��ړ��ʂ����݂̈ړ��e�[�u��XYZ�ɑ����Ă��遁�\�����W�����߂Ă���
				XMStoreFloat3(&g_Hi[i].pos, nowPos + Pos);


				// frame���g�Ď��Ԍo�ߏ���������
				g_Hi[i].time += 1.0f / tbl[nowNo].frame;	// ���Ԃ�i�߂Ă���

				g_cnt++;

				switch (g_cnt)
				{
				case 300:
				case 430:
				case 560:
				case 690:
				case 820:
				case 950:
				case 1080:
				case 1210:
					for (int j = 1; j < MAX_HI; j++)
					{
						if (g_Hi[j].use == FALSE)
						{
							g_Hi[j].pos = g_Hi[i].pos;
							g_Hi[j].use = TRUE;
							PlaySound(SOUND_LABEL_SE_FIRE);
							break;
						}
					}
					break;

				}

				// �C�x���g�X�L�b�v
				if (GetKeyboardTrigger(DIK_RIGHT) || IsButtonTriggered(0, BUTTON_B))
				{
					EventSkip();
				}


				if ((int)g_Hi[i].time >= maxNo - 1)			// �o�^�e�[�u���Ō�܂ňړ��������H
				{
					g_Hi[i].time -= maxNo;				// �O�ԖڂɃ��Z�b�g�������������������p���ł���

					g_Hi[i].use = FALSE;

					// �����������
					{
						LIGHT* light = GetLightData(0);

						// ���邳�����X�ɏグ��W��
						float brightnessIncreaseRate = 0.1f; // ���X�ɏグ�鑬�x�𒲐����邽�߂̒l

						// ���݂̖��邳���擾
						float currentBrightness = light[0].Diffuse.x; // Diffuse.x�𖾂邳�Ƃ��Ďg�p

						for (int i = 0; i < 10; i++)
						{
							// ���邳�����X�ɏグ��
							currentBrightness += brightnessIncreaseRate; // ���邳�𑝉�������

							// ���邳��1���傫���ꍇ��1�ɃN�����v����
							if (currentBrightness > 1.0f)
							{
								currentBrightness = 1.0f;
							}

							// �V����Diffuse�l��ݒ�
							light[0].Diffuse = XMFLOAT4(currentBrightness, currentBrightness, currentBrightness, 1.0f); // �e�F�����𓯂��l�ɐݒ�

							SetLightData(0, &light[0]);
						}
						
					}

				}

			}

			// �΂̏���
			float hi_x = (float)(rand() % 4 - 2) * 0.1f;
			float hi_z = (float)(rand() % 4 - 2) * 0.1f;
			SetParticle(g_Hi[i].pos, XMFLOAT3(0.7f, 0.7f, 0.7f), XMFLOAT3(hi_x, 1.25f, hi_z), XMFLOAT4(1.0f, 0.5f, 0.1f, 0.85f), TYPE_HINOKO_RED, BLEND_MODE_ADD, 0.0f, 0.0f, 30);

		}
	}

#ifdef _DEBUG

	PrintDebugProc(" Count: %d\n", g_cnt);

#endif


}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawEvent(void)
{

}

//=============================================================================
// �G�l�~�[�̎擾
//=============================================================================
EVENT *GetEvent()
{
	return &g_Hi[0];
}

void EventSkip(void)
{
	CAMERA* cam = GetCamera();

	for (int i = 0; i < MAX_HI; i++)
	{
		if (g_Hi[i].use)continue;

		XMFLOAT3 pos = XMFLOAT3(0.0f, 0.0f, 0.0f);

		switch (i)
		{
			case 1:
				pos = XMFLOAT3( 50.0f, 45.0f, -300.0f);
			break;

			case 2:
				pos = XMFLOAT3(-50.0f, 45.0f, -300.0f);
			break;

			case 3:
				pos = XMFLOAT3(-50.0f, 45.0f, -200.0f);
			break;

			case 4:
				pos = XMFLOAT3( 50.0f, 45.0f, -200.0f);
			break;

			case 5:
				pos = XMFLOAT3( 50.0f, 45.0f, -100.0f);

			case 6:
				pos = XMFLOAT3(-50.0f, 45.0f, -100.0f);
			break;

			case 7:
				pos = XMFLOAT3(-50.0f, 45.0f,  0.0f);
			break;

			case 8:
				pos = XMFLOAT3( 50.0f, 45.0f, 0.0f);
			break;

		}

		g_Hi[i].pos = pos;
		g_Hi[i].use = TRUE;
	}

	g_Hi[0].use = FALSE;
	cam->pos = XMFLOAT3(0.0f, 25.0f, -450.0f);
	cam->at = XMFLOAT3(0.0f, 7.0f, -350.0f);
	SetFlag(FLAG_GAME_EVENT_PILLAR);

	{
		LIGHT* light = GetLightData(0);

		// ���邳�����X�ɏグ��W��
		float brightnessIncreaseRate = 0.1f; // ���X�ɏグ�鑬�x�𒲐����邽�߂̒l

		// ���݂̖��邳���擾
		float currentBrightness = light[0].Diffuse.x; // Diffuse.x�𖾂邳�Ƃ��Ďg�p

		for (int i = 0; i < 10; i++)
		{
			// ���邳�����X�ɏグ��
			currentBrightness += brightnessIncreaseRate; // ���邳�𑝉�������

			// ���邳��1���傫���ꍇ��1�ɃN�����v����
			if (currentBrightness > 1.0f)
			{
				currentBrightness = 1.0f;
			}

			// �V����Diffuse�l��ݒ�
			light[0].Diffuse = XMFLOAT4(currentBrightness, currentBrightness, currentBrightness, 1.0f); // �e�F�����𓯂��l�ɐݒ�

			SetLightData(0, &light[0]);

		}
	}

	PlaySound(SOUND_LABEL_SE_PILLAR_DOWN);
	PlaySound(SOUND_LABEL_SE_FIRE);

}