//=============================================================================
//
// �e���ˏ��� [bullet_charge.cpp]
// Author : HAL���� ������
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "shadow.h"
#include "collision.h"
#include "player.h"
#include "enemy.h"
#include "bullet_charge.h"
#include "score.h"
#include "sound.h"
#include "sprite.h"
#include "camera.h"
#include "particle.h"
#include "particle_spark.h"
#include "debugproc.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_MAX			(1)				// �e�N�X�`���̐�

#define	BULLET_CHARGE_WIDTH			(20.0f)			// ���_�T�C�Y
#define	BULLET_CHARGE_HEIGHT		(20.0f)			// ���_�T�C�Y
#define	BULLET_CHARGE_SIZE			(20.0f)			// ���_�T�C�Y

#define BULLET_CHARGE_SHADOW_SIZE	(0.4f)			// �e�̑傫��

#define	BULLET_CHARGE_OFFSET_Y		(7.0f)			// ���_�T�C�Y

#define	BULLET_CHARGE_SPEED			(10.0f)			// �e�̈ړ��X�s�[�h

#define	BULLET_PARTICLE_LOOP		(4)				// �������o����

#define BULLET_BLUE_COLOR			{0.1f, 0.5f, 1.0f, 1.0f}		// �o���b�g�u���[�̐F

//*****************************************************************************
// �\���̒�`
//*****************************************************************************


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
//HRESULT MakeVertexBullet_Charge(void);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static BULLET_CHARGE						g_Bullet_Charge[MAX_BULLET_CHARGE];	// �e���[�N

BOOL		g_soundF;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitBullet_Charge(void)
{
	// �e���[�N�̏�����
	for (int nCntBullet_Charge = 0; nCntBullet_Charge < MAX_BULLET_CHARGE; nCntBullet_Charge++)
	{
		g_Bullet_Charge[nCntBullet_Charge].pos = { 0.0f, 0.0f, 0.0f };
		g_Bullet_Charge[nCntBullet_Charge].rot = { 0.0f, 0.0f, 0.0f };
		g_Bullet_Charge[nCntBullet_Charge].spd = BULLET_CHARGE_SPEED;
		g_Bullet_Charge[nCntBullet_Charge].bUse = FALSE;
		g_Bullet_Charge[nCntBullet_Charge].size = BULLET_CHARGE_SIZE;	// �����蔻��̑傫��
		g_Bullet_Charge[nCntBullet_Charge].type = BULLET_CHARGE_NONE;
		g_Bullet_Charge[nCntBullet_Charge].color = { 1.0f, 1.0f, 1.0f, 1.0f };

	}

	g_soundF = FALSE;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitBullet_Charge(void)
{

}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateBullet_Charge(void)
{
	PLAYER* player = GetPlayer();
	ENEMY* enemy = GetEnemy();
	CAMERA* cam = GetCamera();

	int Cnt = 0;

	for (int i = 0; i < MAX_BULLET_CHARGE; i++)
	{
		if (g_Bullet_Charge[i].bUse == TRUE)
		{
			XMVECTOR vP0 ;
			XMVECTOR vP1 ;
			XMVECTOR vP2 ;
			float s;

			XMVECTOR vAns;
			XMVECTOR vold_pos;
			XMVECTOR vec;
			float angle;

			// �e�̈ړ�����
			g_Bullet_Charge[i].s += 0.025f;


			switch (g_Bullet_Charge[i].type)
			{
			case BULLET_CHARGE_RED:
				if (g_Bullet_Charge[i].s <= 1.0f)
				{
					vP0 = XMLoadFloat3(&g_Bullet_Charge[i].P0);
					vP1 = XMLoadFloat3(&g_Bullet_Charge[i].P1);
					vP2 = XMLoadFloat3(&g_Bullet_Charge[i].Tpos);

					// �i�ޓx������ύX
					//s = g_Bullet[i].s;
					s = g_Bullet_Charge[i].s * g_Bullet_Charge[i].s;
					//s = g_Bullet[i].s * (2 - g_Bullet[i].s);
					//s = g_Bullet[i].s * (g_Bullet[i].s * (3 - 2 * g_Bullet[i].s));

					vAns = QuadraticBezierCurve(vP0, vP1, vP2, s);

					XMStoreFloat3(&g_Bullet_Charge[i].pos, vAns);
					vold_pos = XMLoadFloat3(&g_Bullet_Charge[i].pos);

					vec = vAns - vold_pos;		//�ړ��O�Ƃ��Ƃ̍��W�̍����i�p�x�j
					angle = atan2f(vec.m128_f32[1], vec.m128_f32[0]);		//���̍������g���Ċp�x�����߂Ă���

					SetParticle(g_Bullet_Charge[i].pos, XMFLOAT3(0.7f, 0.7f, 0.7f),
						XMFLOAT3(0.0f, 0.0f, 0.0f), g_Bullet_Charge[i].color,
						TYPE_HINOKO_RED, BLEND_MODE_ADD, 0.0f, 0.0f, 30);

				}
				// ����
				else 
				{
					if (g_soundF == FALSE)
					{
						PlaySound(SOUND_LABEL_SE_EXPROSION);
						g_soundF = TRUE;
					}

					g_Bullet_Charge[i].size = 0.7f * BULLET_CHARGE_SIZE * powf(g_Bullet_Charge[i].s, 3);

					XMFLOAT3 scl = { 0.7f * powf(g_Bullet_Charge[i].s, 3),
										0.7f * powf(g_Bullet_Charge[i].s, 3),
										0.7f * powf(g_Bullet_Charge[i].s, 3) };

					SetParticle(g_Bullet_Charge[i].pos, scl,
						XMFLOAT3(0.0f, 0.0f, 0.0f), g_Bullet_Charge[i].color,
						TYPE_HINOKO_CHARGE_RED, BLEND_MODE_ADD, 0.0f, 0.0f, 30);

				}

				if (g_Bullet_Charge[i].s > 2.0f)
				{
					ReleaseBullet_Charge(i);
				}
				break;

			case BULLET_CHARGE_BLUE:
				if (g_Bullet_Charge[i].s <= 1.0f)
				{
					vP0 = XMLoadFloat3(&g_Bullet_Charge[i].P0);
					vP1 = XMLoadFloat3(&g_Bullet_Charge[i].P1);
					vP2 = XMLoadFloat3(&g_Bullet_Charge[i].Tpos);

					// �i�ޓx������ύX
					//s = g_Bullet[i].s;
					s = g_Bullet_Charge[i].s * g_Bullet_Charge[i].s;
					//s = g_Bullet[i].s * (2 - g_Bullet[i].s);
					//s = g_Bullet[i].s * (g_Bullet[i].s * (3 - 2 * g_Bullet[i].s));

					vAns = QuadraticBezierCurve(vP0, vP1, vP2, s);

					XMStoreFloat3(&g_Bullet_Charge[i].pos, vAns);
					vold_pos = XMLoadFloat3(&g_Bullet_Charge[i].pos);

					vec = vAns - vold_pos;		//�ړ��O�Ƃ��Ƃ̍��W�̍����i�p�x�j
					angle = atan2f(vec.m128_f32[1], vec.m128_f32[0]);		//���̍������g���Ċp�x�����߂Ă���

					SetParticle(g_Bullet_Charge[i].pos, XMFLOAT3(0.7f, 0.7f, 0.7f),
						XMFLOAT3(0.0f, 0.0f, 0.0f), g_Bullet_Charge[i].color,
						TYPE_HINOKO_BLUE, BLEND_MODE_ADD, 0.0f, 0.0f, 30);

				}
				// �g�U
				if (g_Bullet_Charge[i].s >= 1.0f)
				{
					if (g_soundF == FALSE)
					{
						PlaySound(SOUND_LABEL_SE_BUBBLE);
						g_soundF = TRUE;
					}

					// ���e������S�����ɔ�΂�
					XMFLOAT3 rot = g_Bullet_Charge[i].rot;

					rot.y += XM_PIDIV4;
					SetBullet(g_Bullet_Charge[i].pos, rot, BULLET_BLUE_COLOR, BULLET_BLUE);

					rot.y += XM_PIDIV2;
					SetBullet(g_Bullet_Charge[i].pos, rot, BULLET_BLUE_COLOR, BULLET_BLUE);

					rot.y += XM_PIDIV2;
					SetBullet(g_Bullet_Charge[i].pos, rot, BULLET_BLUE_COLOR, BULLET_BLUE);

					rot.y += XM_PIDIV2;
					SetBullet(g_Bullet_Charge[i].pos, rot, BULLET_BLUE_COLOR, BULLET_BLUE);

				}
				if (g_Bullet_Charge[i].s > 2.0f)
				{
					ReleaseBullet_Charge(i);
				}
				break;

			case BULLET_CHARGE_GREEN:
				if (g_Bullet_Charge[i].s <= 1.0f)
				{
					SetParticle(g_Bullet_Charge[i].pos, XMFLOAT3(0.2f, 0.2f, 0.2f), XMFLOAT3(0.0f, 0.0f, 0.0f), g_Bullet_Charge[i].color, TYPE_HINOKO_YELLOW, BLEND_MODE_ADD, 0.0f, 0.0f, 30);

					// �i�ޓx������ύX
					//s = g_Bullet_Charge[i].s;
					//s = g_Bullet_Charge[i].s * g_Bullet_Charge[i].s * g_Bullet_Charge[i].s * g_Bullet_Charge[i].s;
					s = powf(g_Bullet_Charge[i].s, 10);
					//s = g_Bullet_Charge[i].s * (2 - g_Bullet_Charge[i].s);
					//s = g_Bullet_Charge[i].s * (g_Bullet_Charge[i].s * (3 - 2 * g_Bullet_Charge[i].s));


					if (g_Bullet_Charge[i].s > 0.02f)
					{
						// ���p�[�e�B�N�����o����
						int numParticle = 8;

						// �q���Č����邽�߂Ƀp�[�e�B�N�������̂����l�iS�j
						float alignS = 0.02f / numParticle;

						for (int j = 0; j < numParticle; j++)
						{
							float olds = powf(g_Bullet_Charge[i].s - alignS * (j + 1), 10);

							float oldx = g_Bullet_Charge[i].P0.x * (1.0f - olds) + g_Bullet_Charge[i].Tpos.x * (olds);
							float oldz = g_Bullet_Charge[i].P0.z * (1.0f - olds) + g_Bullet_Charge[i].Tpos.z * (olds);

							SetParticle(XMFLOAT3(oldx, g_Bullet_Charge[i].pos.y, oldz), XMFLOAT3(0.2f, 0.2f, 0.2f),
								XMFLOAT3(0.0f, 0.0f, 0.0f), g_Bullet_Charge[i].color, TYPE_HINOKO_YELLOW, BLEND_MODE_ADD, 0.0f, 0.0f, 30);
						}
					}

						// �e�̈ړ�����
						g_Bullet_Charge[i].pos.x = g_Bullet_Charge[i].P0.x * (1.0f - s) + g_Bullet_Charge[i].Tpos.x * s;
						g_Bullet_Charge[i].pos.z = g_Bullet_Charge[i].P0.z * (1.0f - s) + g_Bullet_Charge[i].Tpos.z * s;

				}
				// ����
				else if (g_Bullet_Charge[i].s > 1.0f && g_Bullet_Charge[i].s < 5.0f)
				{
					if (g_soundF == FALSE)
					{
						PlaySound(SOUND_LABEL_SE_WIND);
						g_soundF = TRUE;
					}

					XMFLOAT3 pos = g_Bullet_Charge[i].pos;

					for (int j = 0; j < BULLET_PARTICLE_LOOP; j++)
					{
						float angle = XM_PI / BULLET_PARTICLE_LOOP * j;

						SetParticle(pos, XMFLOAT3(0.1f, 0.1f, 0.1f),XMFLOAT3(0.0f, 0.8f, 0.0f),
							g_Bullet_Charge[i].color,TYPE_HINOKO_CHARGE_GREEN, BLEND_MODE_ADD, 0.0f, 0.0f, 80, pos, angle);

					}

				}
				if (g_Bullet_Charge[i].s > 5.0f)
				{
					ReleaseBullet_Charge(i);
				}

				break;

			case BULLET_CHARGE_YELLOW:
				if (g_Bullet_Charge[i].s <= 1.0f)
				{
					SetParticle(g_Bullet_Charge[i].pos, XMFLOAT3(0.2f, 0.2f, 0.2f), XMFLOAT3(0.0f, 0.0f, 0.0f), g_Bullet_Charge[i].color, TYPE_HINOKO_YELLOW, BLEND_MODE_ADD, 0.0f, 0.0f, 30);

					// �i�ޓx������ύX
					//s = g_Bullet_Charge[i].s;
					//s = g_Bullet_Charge[i].s * g_Bullet_Charge[i].s * g_Bullet_Charge[i].s * g_Bullet_Charge[i].s;
					s = powf(g_Bullet_Charge[i].s, 10);
					//s = g_Bullet_Charge[i].s * (2 - g_Bullet_Charge[i].s);
					//s = g_Bullet_Charge[i].s * (g_Bullet_Charge[i].s * (3 - 2 * g_Bullet_Charge[i].s));


					if (g_Bullet_Charge[i].s > 0.02f)
					{
						// ���p�[�e�B�N�����o����
						int numParticle = 8;

						// �q���Č����邽�߂Ƀp�[�e�B�N�������̂����l�iS�j
						float alignS = 0.02f / numParticle;

						for (int j = 0; j < numParticle; j++)
						{
							float olds = powf(g_Bullet_Charge[i].s - alignS * (j + 1), 10);

							float oldx = g_Bullet_Charge[i].P0.x * (1.0f - olds) + g_Bullet_Charge[i].Tpos.x * (olds);
							float oldz = g_Bullet_Charge[i].P0.z * (1.0f - olds) + g_Bullet_Charge[i].Tpos.z * (olds);

							SetParticle(XMFLOAT3(oldx, g_Bullet_Charge[i].pos.y, oldz), XMFLOAT3(0.2f, 0.2f, 0.2f),
								XMFLOAT3(0.0f, 0.0f, 0.0f), g_Bullet_Charge[i].color, TYPE_HINOKO_YELLOW, BLEND_MODE_ADD, 3.0f, 3.0f, 30);
						}
					}

					// �e�̈ړ�����
					g_Bullet_Charge[i].pos.x = g_Bullet_Charge[i].P0.x * (1.0f - s) + g_Bullet_Charge[i].Tpos.x * s;
					g_Bullet_Charge[i].pos.z = g_Bullet_Charge[i].P0.z * (1.0f - s) + g_Bullet_Charge[i].Tpos.z * s;

				}
				// ��
				else if (g_Bullet_Charge[i].s > 1.0f && g_Bullet_Charge[i].s < 7.0f)
				{
					if (g_soundF == FALSE)
					{
						PlaySound(SOUND_LABEL_SE_THUNDER);
						g_soundF = TRUE;
					}

					XMFLOAT3 pos = g_Bullet_Charge[i].pos;

					pos.y = 100.0f;


					for (int j = 0; j < 10; j++)
					{
						float angle = XM_2PI / 10 * j;

						SetParticle(pos, XMFLOAT3(0.2f, 0.2f, 0.2f), XMFLOAT3(0.0f, -5.0f, 0.0f),
							g_Bullet_Charge[i].color, TYPE_HINOKO_CHARGE_YELLOW, BLEND_MODE_ADD, 
							g_Bullet_Charge[i].size, g_Bullet_Charge[i].size, 80, pos, angle);

					}

				}
				if (g_Bullet_Charge[i].s > 7.0f)
				{
					ReleaseBullet_Charge(i);
				}

				break;
			}

			// �e�̈ʒu�ݒ�
			SetPositionShadow(g_Bullet_Charge[i].shadowIdx, XMFLOAT3(g_Bullet_Charge[i].pos.x, 0.1f, g_Bullet_Charge[i].pos.z));


			// �t�B�[���h�̊O�ɏo����e����������
			if (g_Bullet_Charge[i].pos.x < MAP_LEFT
				|| g_Bullet_Charge[i].pos.x > MAP_RIGHT
				|| g_Bullet_Charge[i].pos.z < MAP_DOWN
				|| g_Bullet_Charge[i].pos.z > MAP_TOP)
			{
				ReleaseBullet_Charge(i);

			}

			Cnt++;	// �f�o�b�N�p
		}
	}


#ifdef _DEBUG
	// �f�o�b�O�\��
	PrintDebugProc("Bullet_Charge[i] X:%f Y:%f Z:%f \n", g_Bullet_Charge[0].pos.x, g_Bullet_Charge[0].pos.y, g_Bullet_Charge[0].pos.z);
	PrintDebugProc("g_Bullet_Charge[i].s %f\n", g_Bullet_Charge[0].s);
	PrintDebugProc("Cnt %d\n", Cnt);
#endif


}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawBullet_Charge(void)
{

	// ���C�e�B���O��L����
	SetLightEnable(TRUE);

	// �J�����O�ݒ��߂�
	SetCullingMode(CULL_MODE_BACK);

}


//=============================================================================
// �e�̃p�����[�^���Z�b�g
//=============================================================================
int SetBullet_Charge(XMFLOAT3 pos, XMFLOAT3 rot)
{
	PLAYER* player = GetPlayer();
	ENEMY* enemy = GetEnemy();
	CAMERA* cam = GetCamera();

	int nIdxBullet_Charge = -1;

	for (int nCntBullet_Charge = 0; nCntBullet_Charge < MAX_BULLET_CHARGE; nCntBullet_Charge++)
	{
		if (!g_Bullet_Charge[nCntBullet_Charge].bUse)
		{
			pos.x -= sinf(rot.y) * 25.0f;
			pos.z -= cosf(rot.y) * 25.0f;
			pos.y += 7.0f;

			g_Bullet_Charge[nCntBullet_Charge].pos = pos;
			g_Bullet_Charge[nCntBullet_Charge].P0 = pos;
			g_Bullet_Charge[nCntBullet_Charge].rot = rot;
			g_Bullet_Charge[nCntBullet_Charge].bUse = TRUE;
			g_Bullet_Charge[nCntBullet_Charge].size = BULLET_CHARGE_SIZE;
			g_Bullet_Charge[nCntBullet_Charge].type = player->type;
			g_Bullet_Charge[nCntBullet_Charge].color = player->hinokoColor;


			// �e�̐ݒ�
			g_Bullet_Charge[nCntBullet_Charge].shadowIdx = CreateShadow(g_Bullet_Charge[nCntBullet_Charge].pos, 0.5f, 0.5f);

			// �N������
			switch (g_Bullet_Charge[nCntBullet_Charge].type)
			{
			case BULLET_CHARGE_RED:
				g_Bullet_Charge[nCntBullet_Charge].P1 = pos;

				g_Bullet_Charge[nCntBullet_Charge].P1.x = pos.x - sinf(rot.y);
				g_Bullet_Charge[nCntBullet_Charge].P1.z = pos.z - cosf(rot.y);

				g_Bullet_Charge[nCntBullet_Charge].P1.y = 50.0f;

				g_Bullet_Charge[nCntBullet_Charge].Tpos.x = player->pos.x - sinf(player->rot.y) * BULLET_CHARGE_RANGE_RED;		// �ڕW���W���Z�b�g
				g_Bullet_Charge[nCntBullet_Charge].Tpos.y = 0.0f;		// �ڕW���W���Z�b�g
				g_Bullet_Charge[nCntBullet_Charge].Tpos.z = player->pos.z - cosf(player->rot.y) * BULLET_CHARGE_RANGE_RED;		// �ڕW���W���Z�b�g
				break;

			case BULLET_CHARGE_BLUE:
				g_Bullet_Charge[nCntBullet_Charge].P1 = pos;

				g_Bullet_Charge[nCntBullet_Charge].P1.x = pos.x - sinf(rot.y) + (float)(rand() % 100 - 50);
				g_Bullet_Charge[nCntBullet_Charge].P1.z = pos.z - cosf(rot.y) + (float)(rand() % 100 - 50);

				g_Bullet_Charge[nCntBullet_Charge].P1.y = (float)(rand() % 50);

				g_Bullet_Charge[nCntBullet_Charge].Tpos.x = player->pos.x - sinf(player->rot.y) * BULLET_CHARGE_RANGE_BLUE;		// �ڕW���W���Z�b�g
				g_Bullet_Charge[nCntBullet_Charge].Tpos.y = 0.0f;		// �ڕW���W���Z�b�g
				g_Bullet_Charge[nCntBullet_Charge].Tpos.z = player->pos.z - cosf(player->rot.y) * BULLET_CHARGE_RANGE_BLUE;		// �ڕW���W���Z�b�g
				break;

			case BULLET_CHARGE_GREEN:
				g_Bullet_Charge[nCntBullet_Charge].P1 = { 0.0f, 0.0f, 0.0f };	// �g��Ȃ�����O�����Ă�

				g_Bullet_Charge[nCntBullet_Charge].Tpos.x = player->pos.x - sinf(player->rot.y) * BULLET_CHARGE_RANGE_GREEN;		// �ڕW���W���Z�b�g
				g_Bullet_Charge[nCntBullet_Charge].Tpos.y = 0.0f;		// �ڕW���W���Z�b�g
				g_Bullet_Charge[nCntBullet_Charge].Tpos.z = player->pos.z - cosf(player->rot.y) * BULLET_CHARGE_RANGE_GREEN;		// �ڕW���W���Z�b�g

				break;

			case BULLET_CHARGE_YELLOW:
				g_Bullet_Charge[nCntBullet_Charge].P1 = { 0.0f, 0.0f, 0.0f };	// �g��Ȃ�����O�����Ă�

				g_Bullet_Charge[nCntBullet_Charge].Tpos.x = player->pos.x - sinf(player->rot.y) * BULLET_CHARGE_RANGE_YELLOW;		// �ڕW���W���Z�b�g
				g_Bullet_Charge[nCntBullet_Charge].Tpos.y = 0.0f;		// �ڕW���W���Z�b�g
				g_Bullet_Charge[nCntBullet_Charge].Tpos.z = player->pos.z - cosf(player->rot.y) * BULLET_CHARGE_RANGE_YELLOW;		// �ڕW���W���Z�b�g

				break;
			}

			nIdxBullet_Charge = nCntBullet_Charge;

			break;
		}
	}

	return nIdxBullet_Charge;
}

//=============================================================================
// �e�̎擾
//=============================================================================
BULLET_CHARGE* GetBullet_Charge(void)
{
	return &(g_Bullet_Charge[0]);
}

void ReleaseBullet_Charge(int id)
{
	g_Bullet_Charge[id].bUse = FALSE;
	g_Bullet_Charge[id].s = 0.0f;
	ReleaseShadow(g_Bullet_Charge[id].shadowIdx);

	g_soundF = FALSE;

	switch (g_Bullet_Charge->type)
	{
	case BULLET_CHARGE_RED:
		StopSound(SOUND_LABEL_SE_EXPROSION);
		break;

	case BULLET_CHARGE_BLUE:
		StopSound(SOUND_LABEL_SE_BUBBLE);
		break;

	case BULLET_CHARGE_GREEN:
		StopSound(SOUND_LABEL_SE_WIND);
		break;

	case BULLET_CHARGE_YELLOW:
		StopSound(SOUND_LABEL_SE_THUNDER);
		break;
	}
}