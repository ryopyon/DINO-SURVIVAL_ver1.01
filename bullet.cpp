//=============================================================================
//
// �e���ˏ��� [bullet.cpp]
// Author : HAL���� ������
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "shadow.h"
#include "collision.h"
#include "player.h"
#include "enemy.h"
#include "bullet.h"
#include "score.h"
#include "sprite.h"
#include "camera.h"
#include "particle.h"
#include "particle_spark.h"
#include "debugproc.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_MAX			(1)				// �e�N�X�`���̐�

#define	BULLET_WIDTH		(20.0f)			// ���_�T�C�Y
#define	BULLET_HEIGHT		(20.0f)			// ���_�T�C�Y
#define	BULLET_SIZE			(20.0f)			// �T�C�Y

#define BULLET_SHADOW_SIZE	(0.4f)			// �e�̑傫��

#define	BULLET_OFFSET_Y		(7.0f)			// ���_�T�C�Y

#define	BULLET_SPEED		(10.0f)			// �e�̈ړ��X�s�[�h



//*****************************************************************************
// �\���̒�`
//*****************************************************************************


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
//HRESULT MakeVertexBullet(void);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************

static BULLET						g_Bullet[MAX_BULLET];	// �e���[�N
static int							g_TexNo;				// �e�N�X�`���ԍ�

//=============================================================================
// ����������
//=============================================================================
HRESULT InitBullet(void)
{
	g_TexNo = 0;

	// �e���[�N�̏�����
	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		g_Bullet[nCntBullet].pos = { 0.0f, 0.0f, 0.0f };
		g_Bullet[nCntBullet].rot = { 0.0f, 0.0f, 0.0f };
		g_Bullet[nCntBullet].spd = BULLET_SPEED;
		g_Bullet[nCntBullet].bUse = FALSE;
		g_Bullet[nCntBullet].size = BULLET_SIZE;	// �����蔻��̑傫��
		g_Bullet[nCntBullet].type = BULLET_NONE;
		g_Bullet[nCntBullet].color = { 1.0f, 1.0f, 1.0f, 1.0f };

	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitBullet(void)
{

}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateBullet(void)
{
	PLAYER* player = GetPlayer();
	ENEMY* enemy = GetEnemy();
	CAMERA* cam = GetCamera();

	int Cnt = 0;

	for (int i = 0; i < MAX_BULLET; i++)
	{
		if (g_Bullet[i].bUse == TRUE)
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
			g_Bullet[i].s += 0.025f;

			if (g_Bullet[i].s > 1.0f)
			{
				ReleaseBullet(i);
			}

			switch (g_Bullet[i].type)
			{
			case BULLET_RED:
			case BULLET_BLUE:

				vP0 = XMLoadFloat3(&g_Bullet[i].P0);
				vP1 = XMLoadFloat3(&g_Bullet[i].P1);
				vP2 = XMLoadFloat3(&g_Bullet[i].Tpos);

				// �i�ޓx������ύX
				//s = g_Bullet[i].s;
				s = g_Bullet[i].s * g_Bullet[i].s;
				//s = g_Bullet[i].s * (2 - g_Bullet[i].s);
				//s = g_Bullet[i].s * (g_Bullet[i].s * (3 - 2 * g_Bullet[i].s));

				vAns = QuadraticBezierCurve(vP0, vP1, vP2, s);

				XMStoreFloat3(&g_Bullet[i].pos, vAns);
				vold_pos = XMLoadFloat3(&g_Bullet[i].pos);

				vec = vAns - vold_pos;									//�ړ��O�Ƃ��Ƃ̍��W�̍����i�p�x�j
				angle = atan2f(vec.m128_f32[1], vec.m128_f32[0]);		//���̍������g���Ċp�x�����߂Ă���

				SetParticle(g_Bullet[i].pos, XMFLOAT3(0.7f, 0.7f, 0.7f), 
					XMFLOAT3(0.0f, 0.0f, 0.0f), g_Bullet[i].color, 
					TYPE_HINOKO_RED, BLEND_MODE_ADD, 0.0f, 0.0f, 30);

				break;

			case BULLET_GREEN:
			case BULLET_YELLOW:
				SetParticle(g_Bullet[i].pos, XMFLOAT3(0.2f, 0.2f, 0.2f), 
					XMFLOAT3(0.0f, 0.0f, 0.0f), g_Bullet[i].color, 
					TYPE_HINOKO_YELLOW, BLEND_MODE_ADD, 0.0f, 0.0f, 30);

				// �i�ޓx������ύX
				//s = g_Bullet[i].s;
				//s = g_Bullet[i].s * g_Bullet[i].s * g_Bullet[i].s * g_Bullet[i].s;
				s = powf(g_Bullet[i].s, 10);
				//s = g_Bullet[i].s * (2 - g_Bullet[i].s);
				//s = g_Bullet[i].s * (g_Bullet[i].s * (3 - 2 * g_Bullet[i].s));


				if (g_Bullet[i].s > 0.02f)
				{
					// ���p�[�e�B�N�����o����
					int numParticle = 8;

					// �q���Č����邽�߂Ƀp�[�e�B�N�������̂����l�iS�j
					float alignS = 0.02f / numParticle;

					for (int j = 0; j < numParticle; j++)
					{
						float olds = powf(g_Bullet[i].s - alignS * (j + 1), 10);

						float oldx = g_Bullet[i].P0.x * (1.0f - olds) + g_Bullet[i].Tpos.x * (olds);
						float oldz = g_Bullet[i].P0.z * (1.0f - olds) + g_Bullet[i].Tpos.z * (olds);

						SetParticle(XMFLOAT3(oldx, g_Bullet[i].pos.y, oldz), XMFLOAT3(0.2f, 0.2f, 0.2f), 
							XMFLOAT3(0.0f, 0.0f, 0.0f), g_Bullet[i].color, 
							TYPE_HINOKO_YELLOW, BLEND_MODE_ADD, 0.0f, 0.0f, 30);
					}
				}

				if (g_Bullet[i].s <= 1.0f)
				{
					// �e�̈ړ�����
					g_Bullet[i].pos.x = g_Bullet[i].P0.x * (1.0f - s) + g_Bullet[i].Tpos.x * s;
					g_Bullet[i].pos.z = g_Bullet[i].P0.z * (1.0f - s) + g_Bullet[i].Tpos.z * s;

				}
				break;
			}

			// �e�̈ʒu�ݒ�
			SetPositionShadow(g_Bullet[i].shadowIdx, XMFLOAT3(g_Bullet[i].pos.x, 0.1f, g_Bullet[i].pos.z));


			// �t�B�[���h�̊O�ɏo����e����������
			if (g_Bullet[i].pos.x < MAP_LEFT
				|| g_Bullet[i].pos.x > MAP_RIGHT
				|| g_Bullet[i].pos.z < MAP_DOWN
				|| g_Bullet[i].pos.z > MAP_TOP)
			{
				ReleaseBullet(i);

				// �ΉԂ��o��
				for (int p = 0; p < 16; p++)
				{
					XMFLOAT3 move = CalcParticleSparkMove(g_Bullet[i].pos);

					SetParticleSpark(g_Bullet[i].pos, move, 30, TRUE);
				}

			}

			Cnt++;
		}
	}


#ifdef _DEBUG
	// �f�o�b�O�\��
	PrintDebugProc("Bullet[i] X:%f Y:%f Z:%f \n", g_Bullet[0].pos.x, g_Bullet[0].pos.y, g_Bullet[0].pos.z);
	PrintDebugProc("g_Bullet[i].s %f\n", g_Bullet[0].s);
	PrintDebugProc("Cnt %d\n", Cnt);
#endif


}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawBullet(void)
{

	// ���C�e�B���O��L����
	SetLightEnable(TRUE);

	// �J�����O�ݒ��߂�
	SetCullingMode(CULL_MODE_BACK);

}


//=============================================================================
// �e�̃p�����[�^���Z�b�g
//=============================================================================
int SetBullet(XMFLOAT3 pos, XMFLOAT3 rot)
{
	PLAYER* player = GetPlayer();
	ENEMY* enemy = GetEnemy();
	CAMERA* cam = GetCamera();

	int nIdxBullet = -1;

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (!g_Bullet[nCntBullet].bUse)
		{
			pos.x -= sinf(rot.y) * 25.0f;
			pos.z -= cosf(rot.y) * 25.0f;
			pos.y += 7.0f;

			g_Bullet[nCntBullet].pos = pos;
			g_Bullet[nCntBullet].P0 = pos;
			g_Bullet[nCntBullet].rot = rot;
			g_Bullet[nCntBullet].bUse = TRUE;
			g_Bullet[nCntBullet].type = player->type;
			g_Bullet[nCntBullet].color = player->hinokoColor;


			// �e�̐ݒ�
			g_Bullet[nCntBullet].shadowIdx = CreateShadow(g_Bullet[nCntBullet].pos, 0.5f, 0.5f);

			// �N������
			switch (g_Bullet[nCntBullet].type)
			{
			case BULLET_RED:
				g_Bullet[nCntBullet].P1 = pos;

				g_Bullet[nCntBullet].P1.x = pos.x - sinf(rot.y) + (float)(rand() % 100 - 50);
				g_Bullet[nCntBullet].P1.z = pos.z - cosf(rot.y) + (float)(rand() % 100 - 50);

				g_Bullet[nCntBullet].P1.y = (float)(rand() % 50);

				g_Bullet[nCntBullet].Tpos.x = player->pos.x - sinf(player->rot.y) * BULLET_RANGE_RED;		// �ڕW���W���Z�b�g
				g_Bullet[nCntBullet].Tpos.y = 0.0f;		// �ڕW���W���Z�b�g
				g_Bullet[nCntBullet].Tpos.z = player->pos.z - cosf(player->rot.y) * BULLET_RANGE_RED;		// �ڕW���W���Z�b�g
				break;

			case BULLET_BLUE:
				g_Bullet[nCntBullet].P1 = pos;

				g_Bullet[nCntBullet].P1.x = pos.x - sinf(rot.y) + (float)(rand() % 100 - 50);
				g_Bullet[nCntBullet].P1.z = pos.z - cosf(rot.y) + (float)(rand() % 100 - 50);

				g_Bullet[nCntBullet].P1.y = (float)(rand() % 50);

				g_Bullet[nCntBullet].Tpos.x = pos.x - sinf(rot.y) * BULLET_RANGE_BLUE;		// �ڕW���W���Z�b�g
				g_Bullet[nCntBullet].Tpos.y = 0.0f;		// �ڕW���W���Z�b�g
				g_Bullet[nCntBullet].Tpos.z = pos.z - cosf(rot.y) * BULLET_RANGE_BLUE;		// �ڕW���W���Z�b�g
				break;

			case BULLET_GREEN:
				g_Bullet[nCntBullet].P1 = { 0.0f, 0.0f, 0.0f };	// �g��Ȃ�����O�����Ă�

				g_Bullet[nCntBullet].Tpos.x = player->pos.x - sinf(player->rot.y) * BULLET_RANGE_GREEN;		// �ڕW���W���Z�b�g
				g_Bullet[nCntBullet].Tpos.y = 0.0f;		// �ڕW���W���Z�b�g
				g_Bullet[nCntBullet].Tpos.z = player->pos.z - cosf(player->rot.y) * BULLET_RANGE_GREEN;		// �ڕW���W���Z�b�g

				break;

			case BULLET_YELLOW:
				g_Bullet[nCntBullet].P1 = { 0.0f, 0.0f, 0.0f };	// �g��Ȃ�����O�����Ă�

				g_Bullet[nCntBullet].Tpos.x = player->pos.x - sinf(player->rot.y) * BULLET_RANGE_YELLOW;		// �ڕW���W���Z�b�g
				g_Bullet[nCntBullet].Tpos.y = 0.0f;		// �ڕW���W���Z�b�g
				g_Bullet[nCntBullet].Tpos.z = player->pos.z - cosf(player->rot.y) * BULLET_RANGE_YELLOW;		// �ڕW���W���Z�b�g

				break;
			}

			nIdxBullet = nCntBullet;

			break;
		}
	}

	return nIdxBullet;
}

//=============================================================================
// �e�̃p�����[�^���Z�b�g �� �J���[&�^�C�v�w��
//=============================================================================
int SetBullet(XMFLOAT3 pos, XMFLOAT3 rot, XMFLOAT4 color, int type)
{
	PLAYER* player = GetPlayer();
	ENEMY* enemy = GetEnemy();
	CAMERA* cam = GetCamera();

	int nIdxBullet = -1;

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (!g_Bullet[nCntBullet].bUse)
		{
			pos.x -= sinf(rot.y) * 25.0f;
			pos.z -= cosf(rot.y) * 25.0f;
			pos.y += 7.0f;

			g_Bullet[nCntBullet].pos = pos;
			g_Bullet[nCntBullet].P0 = pos;
			g_Bullet[nCntBullet].rot = rot;
			g_Bullet[nCntBullet].bUse = TRUE;
			g_Bullet[nCntBullet].type = type;
			g_Bullet[nCntBullet].color = color;


			// �e�̐ݒ�
			g_Bullet[nCntBullet].shadowIdx = CreateShadow(g_Bullet[nCntBullet].pos, 0.5f, 0.5f);

			// �N������
			switch (g_Bullet[nCntBullet].type)
			{
			case BULLET_RED:
				g_Bullet[nCntBullet].P1 = pos;

				g_Bullet[nCntBullet].P1.x = pos.x - sinf(rot.y) + (float)(rand() % 100 - 50);
				g_Bullet[nCntBullet].P1.z = pos.z - cosf(rot.y) + (float)(rand() % 100 - 50);

				g_Bullet[nCntBullet].P1.y = (float)(rand() % 50);

				g_Bullet[nCntBullet].Tpos.x = pos.x - sinf(rot.y) * BULLET_RANGE_RED;		// �ڕW���W���Z�b�g
				g_Bullet[nCntBullet].Tpos.y = 0.0f;		// �ڕW���W���Z�b�g
				g_Bullet[nCntBullet].Tpos.z = pos.z - cosf(rot.y) * BULLET_RANGE_RED;		// �ڕW���W���Z�b�g
				break;

			case BULLET_BLUE:
				g_Bullet[nCntBullet].P1 = pos;

				g_Bullet[nCntBullet].P1.x = pos.x - sinf(rot.y) + (float)(rand() % 100 - 50);
				g_Bullet[nCntBullet].P1.z = pos.z - cosf(rot.y) + (float)(rand() % 100 - 50);

				g_Bullet[nCntBullet].P1.y = (float)(rand() % 50);

				g_Bullet[nCntBullet].Tpos.x = pos.x - sinf(rot.y) * BULLET_RANGE_BLUE;		// �ڕW���W���Z�b�g
				g_Bullet[nCntBullet].Tpos.y = 0.0f;		// �ڕW���W���Z�b�g
				g_Bullet[nCntBullet].Tpos.z = pos.z - cosf(rot.y) * BULLET_RANGE_BLUE;		// �ڕW���W���Z�b�g
				break;

			case BULLET_GREEN:
				g_Bullet[nCntBullet].P1 = { 0.0f, 0.0f, 0.0f };	// �g��Ȃ�����O�����Ă�

				g_Bullet[nCntBullet].Tpos.x = pos.x - sinf(rot.y) * BULLET_RANGE_GREEN;		// �ڕW���W���Z�b�g
				g_Bullet[nCntBullet].Tpos.y = 0.0f;		// �ڕW���W���Z�b�g
				g_Bullet[nCntBullet].Tpos.z = pos.z - cosf(rot.y) * BULLET_RANGE_GREEN;		// �ڕW���W���Z�b�g

				break;

			case BULLET_YELLOW:
				g_Bullet[nCntBullet].P1 = { 0.0f, 0.0f, 0.0f };	// �g��Ȃ�����O�����Ă�

				g_Bullet[nCntBullet].Tpos.x = pos.x - sinf(rot.y) * BULLET_RANGE_YELLOW;		// �ڕW���W���Z�b�g
				g_Bullet[nCntBullet].Tpos.y = 0.0f;		// �ڕW���W���Z�b�g
				g_Bullet[nCntBullet].Tpos.z = pos.z - cosf(rot.y) * BULLET_RANGE_YELLOW;		// �ڕW���W���Z�b�g

				break;
			}

			nIdxBullet = nCntBullet;

			break;
		}
	}

	return nIdxBullet;
}

//=============================================================================
// �e���������̏��� 
//=============================================================================
void ReleaseBullet(int id)
{
	g_Bullet[id].bUse = FALSE;
	g_Bullet[id].s = 0.0f;
	ReleaseShadow(g_Bullet[id].shadowIdx);
}

//=============================================================================
// �e�̎擾
//=============================================================================
BULLET* GetBullet(void)
{
	return &(g_Bullet[0]);
}

//=============================================================================
// �x�W�F�J�[�u�p
//=============================================================================

XMVECTOR LinearInterpolation(XMVECTOR P0, XMVECTOR P1, float s)
{
	XMVECTOR A0;
	A0 = P0 + (P1 - P0) * s;
	return A0;
}


XMVECTOR QuadraticBezierCurve(XMVECTOR p0, XMVECTOR p1, XMVECTOR p2, float s)
{
	XMVECTOR a0 = LinearInterpolation(p0, p1, s);
	XMVECTOR a1 = LinearInterpolation(p1, p2, s);
	XMVECTOR b0 = LinearInterpolation(a0, a1, s);

	return b0;
}