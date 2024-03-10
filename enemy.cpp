//=============================================================================
//
// �G�l�~�[���f������ [enemy.cpp]
// Author : HAL���� ������
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "model.h"
#include "input.h"
#include "debugproc.h"
#include "player.h"
#include "enemy.h"
#include "bullet_charge.h"
#include "ui_enemy.h"
#include "collision.h"
#include "shadow.h"
#include "particle.h"
#include "flag.h"
#include "keyframe.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MODEL_ENEMY_RED				"data/MODEL/akamaru/akamaru_body.obj"		// �ǂݍ��ރ��f����
#define	MODEL_ENEMY_RED_HEAD		"data/MODEL/akamaru/akamaru_head.obj"		// �ǂݍ��ރ��f����
#define	MODEL_ENEMY_RED_LLEG		"data/MODEL/akamaru/akamaru_leg_l.obj"		// �ǂݍ��ރ��f����
#define	MODEL_ENEMY_RED_RLEG		"data/MODEL/akamaru/akamaru_leg_r.obj"		// �ǂݍ��ރ��f����
#define	MODEL_ENEMY_RED_TAIL		"data/MODEL/akamaru/akamaru_tail.obj"		// �ǂݍ��ރ��f����

#define	MODEL_ENEMY_BLUE			"data/MODEL/BlueSuke/BlueSuke_body.obj"		// �ǂݍ��ރ��f����
#define	MODEL_ENEMY_BLUE_HEAD		"data/MODEL/BlueSuke/BlueSuke_head.obj"		// �ǂݍ��ރ��f����
#define	MODEL_ENEMY_BLUE_LLEG		"data/MODEL/BlueSuke/BlueSuke_leg_l.obj"	// �ǂݍ��ރ��f����
#define	MODEL_ENEMY_BLUE_RLEG		"data/MODEL/BlueSuke/BlueSuke_leg_r.obj"	// �ǂݍ��ރ��f����
#define	MODEL_ENEMY_BLUE_TAIL		"data/MODEL/BlueSuke/BlueSuke_tail.obj"		// �ǂݍ��ރ��f����

#define	MODEL_ENEMY_GREEN			"data/MODEL/Midorin/Midorin_body.obj"		// �ǂݍ��ރ��f����
#define	MODEL_ENEMY_GREEN_HEAD		"data/MODEL/Midorin/Midorin_head.obj"		// �ǂݍ��ރ��f����
#define	MODEL_ENEMY_GREEN_LLEG		"data/MODEL/Midorin/Midorin_leg_l.obj"		// �ǂݍ��ރ��f����
#define	MODEL_ENEMY_GREEN_RLEG		"data/MODEL/Midorin/Midorin_leg_r.obj"		// �ǂݍ��ރ��f����
#define	MODEL_ENEMY_GREEN_TAIL		"data/MODEL/Midorin/Midorin_tail.obj"		// �ǂݍ��ރ��f����

#define	MODEL_ENEMY_YELLOW			"data/MODEL/Kiichan/Kiichan_body.obj"		// �ǂݍ��ރ��f����
#define	MODEL_ENEMY_YELLOW_HEAD		"data/MODEL/Kiichan/Kiichan_head.obj"		// �ǂݍ��ރ��f����
#define	MODEL_ENEMY_YELLOW_LLEG		"data/MODEL/Kiichan/Kiichan_leg_l.obj"		// �ǂݍ��ރ��f����
#define	MODEL_ENEMY_YELLOW_RLEG		"data/MODEL/Kiichan/Kiichan_leg_r.obj"		// �ǂݍ��ރ��f����
#define	MODEL_ENEMY_YELLOW_TAIL		"data/MODEL/Kiichan/Kiichan_tail.obj"		// �ǂݍ��ރ��f����




//--------------------------------------------------------------------------------------
// �L�[�t���[���ύX����
// �A�j���[�V�����̃f�B���N�g��
// �ҋ@���
#define ANIM_IDLE_BODY			"data/ANIMDATA/ENEMY/IDLE/RED_BODY.csv"
#define ANIM_IDLE_HEAD			"data/ANIMDATA/ENEMY/IDLE/RED_HEAD.csv"
#define ANIM_IDLE_LLEG			"data/ANIMDATA/ENEMY/IDLE/RED_LEG_L.csv"
#define ANIM_IDLE_RLEG			"data/ANIMDATA/ENEMY/IDLE/RED_LEG_R.csv"
#define ANIM_IDLE_TAIL			"data/ANIMDATA/ENEMY/IDLE/RED_TAIL.csv"
// �ړ����
#define ANIM_WALK_BODY			"data/ANIMDATA/ENEMY/WALK/ENEMY_BODY.csv"
#define ANIM_WALK_HEAD			"data/ANIMDATA/ENEMY/WALK/ENEMY_HEAD.csv"
#define ANIM_WALK_LLEG			"data/ANIMDATA/ENEMY/WALK/ENEMY_LEG_L.csv"
#define ANIM_WALK_RLEG			"data/ANIMDATA/ENEMY/WALK/ENEMY_LEG_R.csv"
#define ANIM_WALK_TAIL			"data/ANIMDATA/ENEMY/WALK/ENEMY_TAIL.csv"
//�U�����
#define ANIM_HINOKO_BODY		"data/ANIMDATA/ENEMY/HINOKO/RED_BODY.csv"
#define ANIM_HINOKO_HEAD		"data/ANIMDATA/ENEMY/HINOKO/RED_HEAD.csv"
#define ANIM_HINOKO_LLEG		"data/ANIMDATA/ENEMY/HINOKO/RED_LEG_L.csv"
#define ANIM_HINOKO_RLEG		"data/ANIMDATA/ENEMY/HINOKO/RED_LEG_R.csv"
#define ANIM_HINOKO_TAIL		"data/ANIMDATA/ENEMY/HINOKO/RED_TAIL.csv"
// ���ǉ����������͂����ɒǉ�
//--------------------------------------------------------------------------------------

enum TABLE_LIST
{
	IDLE_BODY,
	IDLE_HEAD,
	IDLE_LEG_L,
	IDLE_LEG_R,
	IDLE_TAIL,

	WALK_BODY,
	WALK_HEAD,
	WALK_LEG_L,
	WALK_LEG_R,
	WALK_TAIL,

	HINOKO_BODY,
	HINOKO_HEAD,
	HINOKO_LEG_L,
	HINOKO_LEG_R,
	HINOKO_TAIL,

	TABLE_MAX

};


#define ENEMY_PARTS_MAX				(5)										// �G�l�~�[�̃p�[�c�̐�
#define ENEMY_PARTS_MODEL_MAX		(MAX_ENEMY_TYPE * ENEMY_PARTS_MAX)		// ���v�̃p�[�c�̐�


#define	VALUE_MOVE					(5.0f)						// �ړ���
#define	VALUE_ROTATE				(XM_PI * 0.02f)				// ��]��

#define ENEMY_SHADOW_SIZE			(0.4f)						// �e�̑傫��
#define ENEMY_OFFSET_Y				(11.0f)						// �G�l�~�[�̑��������킹��

#define BURN_TIME_END				(50)						// �Ώ�Ԃ��I��鎞��
#define WET_TIME_END				(100)						// ����Ԃ��I��鎞��
#define WINDY_TIME_END				(50)						// ����Ԃ��I��鎞��
#define SHOCK_TIME_END				(100)						// ����Ԃ��I��鎞��



//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

void WalkPlayer(void);
void HinokoPlayer(void);
void SetTblIdle(void);
void SetTblMove(void);
void SetTblHinoko(void);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ENEMY			g_Enemy[MAX_ENEMY];							// �G�l�~�[

static ENEMY			g_Parts[MAX_ENEMY][ENEMY_PARTS_MAX];		// �G�l�~�[�̃p�[�c�p

static ENEMY_MODEL		g_PartsModel[MAX_ENEMY_TYPE * ENEMY_PARTS_MAX];	//�G�l�~�[�̎�ށ��p�[�c��


static char		g_ModelName[MAX_ENEMY_TYPE * ENEMY_PARTS_MAX][256] = 
{
	MODEL_ENEMY_RED,
	MODEL_ENEMY_RED_HEAD,
	MODEL_ENEMY_RED_LLEG,
	MODEL_ENEMY_RED_RLEG,
	MODEL_ENEMY_RED_TAIL,

	MODEL_ENEMY_BLUE,
	MODEL_ENEMY_BLUE_HEAD,
	MODEL_ENEMY_BLUE_LLEG,
	MODEL_ENEMY_BLUE_RLEG,
	MODEL_ENEMY_BLUE_TAIL,

	MODEL_ENEMY_GREEN,
	MODEL_ENEMY_GREEN_HEAD,
	MODEL_ENEMY_GREEN_LLEG,
	MODEL_ENEMY_GREEN_RLEG,
	MODEL_ENEMY_GREEN_TAIL,

	MODEL_ENEMY_YELLOW,
	MODEL_ENEMY_YELLOW_HEAD,
	MODEL_ENEMY_YELLOW_LLEG,
	MODEL_ENEMY_YELLOW_RLEG,
	MODEL_ENEMY_YELLOW_TAIL,

};

static int		g_EnemyCnt = MAX_ENEMY;
static int		spawncnt;				//�X�|�[���J�E���g

//--------------------------------------------------------------------------------------
// �L�[�t���[���ύX����
// �A�j���[�V�����̃f�B���N�g���̔z��
static char* g_AnimDir[] =
{
	// �ҋ@���
	ANIM_IDLE_BODY,
	ANIM_IDLE_HEAD,
	ANIM_IDLE_LLEG,
	ANIM_IDLE_RLEG,
	ANIM_IDLE_TAIL,
	// �ړ����
	ANIM_WALK_BODY,
	ANIM_WALK_HEAD,
	ANIM_WALK_LLEG,
	ANIM_WALK_RLEG,
	ANIM_WALK_TAIL,
	// �U�����
	ANIM_HINOKO_BODY,
	ANIM_HINOKO_HEAD,
	ANIM_HINOKO_LLEG,
	ANIM_HINOKO_RLEG,
	ANIM_HINOKO_TAIL,

	// ���ǉ����������͂����ɒǉ�
};
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// �L�[�t���[���ύX����
// �v���C���[�̊K�w�A�j���[�V�����f�[�^
// �ҋ@���
static std::vector<INTERPOLATION_DATA>  idle_body;
static std::vector<INTERPOLATION_DATA>  idle_head;
static std::vector<INTERPOLATION_DATA>  idle_legl;
static std::vector<INTERPOLATION_DATA>  idle_legr;
static std::vector<INTERPOLATION_DATA>  idle_tail;
// �ړ����
static std::vector<INTERPOLATION_DATA>  walk_body;
static std::vector<INTERPOLATION_DATA>  walk_head;
static std::vector<INTERPOLATION_DATA>  walk_legl;
static std::vector<INTERPOLATION_DATA>  walk_legr;
static std::vector<INTERPOLATION_DATA>  walk_tail;
// �U�����
static std::vector<INTERPOLATION_DATA>  hinoko_body;
static std::vector<INTERPOLATION_DATA>  hinoko_head;
static std::vector<INTERPOLATION_DATA>  hinoko_legl;
static std::vector<INTERPOLATION_DATA>  hinoko_legr;
static std::vector<INTERPOLATION_DATA>  hinoko_tail;
// ���ǉ����������͂����ɒǉ�
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// �L�[�t���[���ύX����
// �v���C���[�̊K�w�A�j���[�V�����f�[�^�̔z��
static std::vector<INTERPOLATION_DATA>* g_TblAdr[] =
{
	&idle_body,
	&idle_head,
	&idle_legl,
	&idle_legr,
	&idle_tail,

	&walk_body,
	&walk_head,
	&walk_legl,
	&walk_legr,
	&walk_tail,

	&hinoko_body,
	&hinoko_head,
	&hinoko_legl,
	&hinoko_legr,
	&hinoko_tail,
	// ���ǉ����������͂����ɒǉ�
};
//--------------------------------------------------------------------------------------

//����
// �v���C���[�̍��E�p�[�c�𓮂����Ă���A�j���f�[�^
static INTERPOLATION_DATA move_tbl_body[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, 5.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),    XMFLOAT3(1.0f, 1.0f, 1.0f), 30 },
	{ XMFLOAT3(0.0f, 3.0f, 0.2f), XMFLOAT3(0.0f, 0.0f, 0.0f),    XMFLOAT3(1.0f, 1.0f, 1.0f), 30 },

};

static INTERPOLATION_DATA move_tbl_head[] = {	// pos, rot, scl,frame
	{ XMFLOAT3(0.0f, 6.0f, 5.0f), XMFLOAT3(XM_PIDIV4 * 0.25f, 0.0f, 0.0f),   XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(0.0f, 6.0f, 5.0f), XMFLOAT3(0.0f, 0.0f, 0.0f),   XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },

};

static INTERPOLATION_DATA move_tbl_lleg[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(-4.0f, -8.0f, 0.0f), XMFLOAT3(XM_PI / 4, 0.0f, 0.0f),     XMFLOAT3(1.0f, 1.0f, 1.0f), 30 },
	{ XMFLOAT3(-4.0f, -8.0f, 0.0f), XMFLOAT3(-XM_PI / 4, 0.0f, 0.0f),    XMFLOAT3(1.0f, 1.0f, 1.0f), 30 },

};

static INTERPOLATION_DATA move_tbl_rleg[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(4.0f, -8.0f, 0.0f), XMFLOAT3(-XM_PI / 4, 0.0f, 0.0f),    XMFLOAT3(1.0f, 1.0f, 1.0f), 30 },
	{ XMFLOAT3(4.0f, -8.0f, 0.0f), XMFLOAT3(XM_PI / 4, 0.0f, 0.0f),    XMFLOAT3(1.0f, 1.0f, 1.0f), 30 },

};

static INTERPOLATION_DATA move_tbl_tail[] = {	// pos, rot, scl, frame
	{ XMFLOAT3(0.0f, -3.0f, -6.0f), XMFLOAT3(-0.2f,  XM_PIDIV4,  XM_PIDIV4),   XMFLOAT3(1.0f, 1.0f, 1.0f), 30 },
	{ XMFLOAT3(0.0f, -3.0f, -6.0f), XMFLOAT3(-0.2f, -XM_PIDIV4, -XM_PIDIV4),   XMFLOAT3(1.0f, 1.0f, 1.0f), 30 },

};

static INTERPOLATION_DATA* g_AnimTblAdr[] =
{
	move_tbl_body,
	move_tbl_head,
	move_tbl_lleg,
	move_tbl_rleg,
	move_tbl_tail,

};

//=============================================================================
// ����������
//=============================================================================
HRESULT InitEnemy(void)
{
	//--------------------------------------------------------------------------------------
// �L�[�t���[���ύX����
// �A�j���[�V�����f�[�^���t�@�C������ǂݍ��� 
	for (int i = 0; i < TABLE_MAX; i++)
	{
		LoadAnimDataCSV(g_AnimDir[i], *g_TblAdr[i]);
	}
	//--------------------------------------------------------------------------------------

	//	�f�[�^�̓ǂݍ���
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		g_Enemy[i].pos = XMFLOAT3(0.0f, ENEMY_OFFSET_Y, 0.0f);
		g_Enemy[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_Enemy[i].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);

		g_Enemy[i].spd  = 0.0f;						// �ړ��X�s�[�h�N���A
		g_Enemy[i].size = ENEMY_SIZE;				// �����蔻��̑傫��

		g_Enemy[i].hp = ENEMY_HP_MAX;				// HP�̍ő��
		g_Enemy[i].damage = ENEMY_DAMAGE_VALUE;		// �G�l�~�[���^����_���[�W��
		

		g_Enemy[i].use = FALSE;						// TRUE:�����Ă�
		g_Enemy[i].EntryTime = FALSE;				// �o��V�[��
		g_Enemy[i].LeaveTime = FALSE;				// �ޏ�V�[��

		g_Enemy[i].burnTime  = 0.0f;				// �Ώ�Ԏ���
		g_Enemy[i].wetTime   = 0.0f;				// ����Ԏ���
		g_Enemy[i].windyTime = 0.0f;				// ����Ԏ���
		g_Enemy[i].shockTime = 0.0f;				// ����Ԏ���

		g_Enemy[i].time = 0.0f;						// ���`��ԗp�̃^�C�}�[���N���A
		g_Enemy[i].tblNo = 0;						// �Đ�����s���f�[�^�e�[�u��No���Z�b�g
		g_Enemy[i].tblMax = 0;						// �Đ�����s���f�[�^�e�[�u���̃��R�[�h�����Z�b�g

		g_Enemy[i].parent = NULL;					// �{�́i�e�j�Ȃ̂�NULL������

		g_Enemy[i].state = ENEMY_ABNORMAL_NONE;

		g_Enemy[i].modelIdx = NULL;					// ���f���ԍ�


		g_Enemy[i].shadowIdx = -1;					// �����ł͉e�͍��Ȃ�

		g_Enemy[i].dissolve.Enable = TRUE;			// �f�B�]���u�̃t���O

		g_Enemy[i].dissolve.threshold = 1.0f;		// �������l

	}

	//	���f���̓ǂݍ���
	for (int i = 0; i < MAX_ENEMY_TYPE * ENEMY_PARTS_MAX; i++)
	{
		LoadModel(g_ModelName[i], &g_PartsModel[i].model);
		g_PartsModel[i].load = TRUE;
	}


	g_EnemyCnt = 0;
	spawncnt = 0;

	// �K�w�A�j���[�V�����̏�����
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		for (int j = 0; j < ENEMY_PARTS_MAX; j++)
		{ 
			g_Parts[i][j].use = FALSE;

			// �ʒu�E��]�E�X�P�[���̏����ݒ�
			g_Parts[i][j].pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
			g_Parts[i][j].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
			g_Parts[i][j].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);

			// �e�q�֌W
			g_Parts[i][j].parent = &g_Enemy[i];		// �� �����ɐe�̃A�h���X������

			// �K�w�A�j���[�V�����p�̃����o�[�ϐ��̏�����
			g_Parts[i][j].time = 0.0f;			// ���`��ԗp�̃^�C�}�[���N���A
			g_Parts[i][j].tblNo = 0;			// �Đ�����s���f�[�^�e�[�u��No���Z�b�g
			g_Parts[i][j].tblMax = 0;			// �Đ�����s���f�[�^�e�[�u���̃��R�[�h�����Z�b�g

			g_Parts[i][j].modelIdx = NULL;		// �ԍ�
		}
	}




	for (int i = 0; i < MAX_ENEMY; i++)
	{
		g_Parts[i][0].use = TRUE;
		g_Parts[i][0].tblNo = WALK_BODY;			// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
		g_Parts[i][0].tblMax = (int)g_TblAdr[g_Parts[i][0].tblNo]->size();	// �� �ύX����

		g_Parts[i][1].use = TRUE;
		g_Parts[i][1].parent = &g_Parts[i][0];	// �e���Z�b�g
		g_Parts[i][1].tblNo = WALK_HEAD;			// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
		g_Parts[i][1].tblMax = (int)g_TblAdr[g_Parts[i][1].tblNo]->size();	// �� �ύX����

		g_Parts[i][2].use = TRUE;
		g_Parts[i][2].parent = &g_Parts[i][0];	// �e���Z�b�g
		g_Parts[i][2].tblNo = WALK_LEG_L;			// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
		g_Parts[i][2].tblMax = (int)g_TblAdr[g_Parts[i][2].tblNo]->size();	// �� �ύX����

		g_Parts[i][3].use = TRUE;
		g_Parts[i][3].parent = &g_Parts[i][0];	// �e���Z�b�g
		g_Parts[i][3].tblNo = WALK_LEG_R;			// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
		g_Parts[i][3].tblMax = (int)g_TblAdr[g_Parts[i][3].tblNo]->size();	// �� �ύX����

		g_Parts[i][4].use = TRUE;
		g_Parts[i][4].parent = &g_Parts[i][0];	// �e���Z�b�g
		g_Parts[i][4].tblNo = WALK_LEG_R;			// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
		g_Parts[i][4].tblMax = (int)g_TblAdr[g_Parts[i][4].tblNo]->size();	// �� �ύX����
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitEnemy(void)
{	
	for (int i = 0; i < MAX_ENEMY_TYPE * ENEMY_PARTS_MAX; i++)
	{
		if (g_PartsModel[i].load)
		{
			UnloadModel(&g_PartsModel[i].model);
			g_PartsModel[i].load = FALSE;
		}
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateEnemy(void)
{
	PLAYER* player = GetPlayer();
	BULLET_CHARGE* bullet_charge = GetBullet_Charge();
	UI_ENEMY* Ui_Enemy = GetUI_Enemy();


	g_EnemyCnt = 0;	// �����Ă�G�l�~�[�̐�

	spawncnt += (10);

	if (spawncnt >= 1000 && GetFlag() == FLAG_GAME_BATTLE)
	{
		SetEnemy();
		spawncnt = 0;
	}


	for (int i = 0; i < MAX_ENEMY; i++)
	{
		if (g_Enemy[i].use == TRUE)		// ���̃G�l�~�[���g���Ă���H
		{								// Yes
			XMFLOAT3 OldPos = g_Enemy[i].pos;

			// �o�Ă���Ƃ�
			if (g_Enemy[i].EntryTime == TRUE)
			{
				g_Enemy[i].dissolve.threshold -= 0.01f;

				if (g_Enemy[i].dissolve.threshold <= 0.0f)
				{
					g_Enemy[i].EntryTime = FALSE;
				}
			}
			// ������Ƃ�
			if (g_Enemy[i].LeaveTime == TRUE)
			{
				Ui_Enemy[i].use = FALSE;

				g_Enemy[i].dissolve.threshold += 0.01f;

				g_Enemy[i].pos.y += 0.3f;

				g_Enemy[i].scl.x -= 0.01f;
				g_Enemy[i].scl.y -= 0.01f;
				g_Enemy[i].scl.z -= 0.01f;

				if (g_Enemy[i].scl.x <= 0.0f)
				{
					g_Enemy[i].LeaveTime = FALSE;
					g_Enemy[i].use = FALSE;
				}
			}

			else // �����������͗����Ɍ�����  
			{
				if (bullet_charge->type == BULLET_CHARGE_GREEN && bullet_charge->bUse == TRUE)
				{
					XMVECTOR epos = XMLoadFloat3(&g_Enemy[i].pos);
					XMVECTOR vec = (XMLoadFloat3(&bullet_charge->pos) - epos);		//�����Ɨ����Ƃ̍��������߂�
					float angle = atan2f(vec.m128_f32[2], vec.m128_f32[0]);			//���̍������g���Ċp�x�����߂Ă���

					if (g_Enemy[i].EntryTime == FALSE && g_Enemy[i].LeaveTime == FALSE)
					{
						g_Enemy[i].pos.x += cosf(angle) * g_Enemy[i].spd;					//angle�̕����ֈړ�
						g_Enemy[i].pos.z += sinf(angle) * g_Enemy[i].spd;					//angle�̕����ֈړ�
					}

				}
				else
				{
					XMVECTOR epos = XMLoadFloat3(&g_Enemy[i].pos);
					XMVECTOR vec = (XMLoadFloat3(&player[0].pos) - epos);		//�����ƃv���C���[�Ƃ̍��������߂�
					float angle = atan2f(vec.m128_f32[2], vec.m128_f32[0]);		//���̍������g���Ċp�x�����߂Ă���

					if (g_Enemy[i].EntryTime == FALSE && g_Enemy[i].LeaveTime == FALSE)
					{
						g_Enemy[i].pos.x += cosf(angle) * g_Enemy[i].spd;					//angle�̕����ֈړ�
						g_Enemy[i].pos.z += sinf(angle) * g_Enemy[i].spd;					//angle�̕����ֈړ�
					}

					//����Ǝ����̍��������߂�
					//�������g���Ċp�x�����߂�(atan2f(�A�[�N�^���W�F���g))
					//���߂��p�x�ֈ���߂Â���

					g_Enemy[i].rot.y = (angle + XM_PI / 2) * -1;	//�z�[�~���O�����։�]
				}

				// �K�w�A�j���[�V����
				for (int j = 0; j < ENEMY_PARTS_MAX; j++)
				{
					// �g���Ă���Ȃ珈������
					if ((g_Parts[i][j].use == TRUE) && (g_Parts[i][j].tblMax > 0))
					{	// ���`��Ԃ̏���
						int nowNo = (int)g_Parts[i][j].time;			// �������ł���e�[�u���ԍ������o���Ă���
						int maxNo = g_Parts[i][j].tblMax;				// �o�^�e�[�u�����𐔂��Ă���
						int nextNo = (nowNo + 1) % maxNo;			// �ړ���e�[�u���̔ԍ������߂Ă���

						//--------------------------------------------------------------------------------------
						// �L�[�t���[���ύX����
						//INTERPOLATION_DATA* tbl = g_AnimTblAdr[g_Parts[i].tblNo];	// �s���e�[�u���̃A�h���X���擾
						std::vector<INTERPOLATION_DATA> tbl = *g_TblAdr[g_Parts[i][j].tblNo];
						//--------------------------------------------------------------------------------------

						XMVECTOR nowPos = XMLoadFloat3(&tbl[nowNo].pos);	// XMVECTOR�֕ϊ�
						XMVECTOR nowRot = XMLoadFloat3(&tbl[nowNo].rot);	// XMVECTOR�֕ϊ�
						XMVECTOR nowScl = XMLoadFloat3(&tbl[nowNo].scl);	// XMVECTOR�֕ϊ�

						XMVECTOR Pos = XMLoadFloat3(&tbl[nextNo].pos) - nowPos;	// XYZ�ړ��ʂ��v�Z���Ă���
						XMVECTOR Rot = XMLoadFloat3(&tbl[nextNo].rot) - nowRot;	// XYZ��]�ʂ��v�Z���Ă���
						XMVECTOR Scl = XMLoadFloat3(&tbl[nextNo].scl) - nowScl;	// XYZ�g�嗦���v�Z���Ă���

						float nowTime = g_Parts[i][j].time - nowNo;	// ���ԕ����ł��鏭�������o���Ă���

						Pos *= nowTime;								// ���݂̈ړ��ʂ��v�Z���Ă���
						Rot *= nowTime;								// ���݂̉�]�ʂ��v�Z���Ă���
						Scl *= nowTime;								// ���݂̊g�嗦���v�Z���Ă���

						// �v�Z���ċ��߂��ړ��ʂ����݂̈ړ��e�[�u��XYZ�ɑ����Ă��遁�\�����W�����߂Ă���
						XMStoreFloat3(&g_Parts[i][j].pos, nowPos + Pos);

						// �v�Z���ċ��߂���]�ʂ����݂̈ړ��e�[�u���ɑ����Ă���
						XMStoreFloat3(&g_Parts[i][j].rot, nowRot + Rot);

						// �v�Z���ċ��߂��g�嗦�����݂̈ړ��e�[�u���ɑ����Ă���
						XMStoreFloat3(&g_Parts[i][j].scl, nowScl + Scl);

						// frame���g�Ď��Ԍo�ߏ���������
						g_Parts[i][j].time += 1.0f / tbl[nowNo].frame;	// ���Ԃ�i�߂Ă���
						if ((int)g_Parts[i][j].time >= maxNo)			// �o�^�e�[�u���Ō�܂ňړ��������H
						{
							g_Parts[i][j].time -= maxNo;				// �O�ԖڂɃ��Z�b�g�������������������p���ł���
						}
					}
				}
			}

			// ��Ԉُ폈��
			{	
				//��
				if (g_Enemy[i].state == ENEMY_ABNORMAL_BURN)
				{
					g_Enemy[i].burnTime++;
					g_Enemy[i].hp--;

					if (g_Enemy[i].burnTime >= BURN_TIME_END)
					{
						g_Enemy[i].state = ENEMY_ABNORMAL_NONE;
						g_Enemy[i].burnTime = 0;

					}
				}

				// ��
				if (g_Enemy[i].state == ENEMY_ABNORMAL_WET)
				{
					g_Enemy[i].wetTime++;

					switch (g_Enemy[i].type)
					{
					case ENEMY_RED:
						g_Enemy[i].spd = (float)ENEMY_RED_SPD / 2;

						break;

					case ENEMY_BLUE:
						g_Enemy[i].spd = (float)ENEMY_BLUE_SPD / 2;

						break;

					case ENEMY_GREEN:
						g_Enemy[i].spd = (float)ENEMY_GREEN_SPD / 2;

						break;

					case ENEMY_YELLOW:
						g_Enemy[i].spd = (float)ENEMY_YELLOW_SPD / 2;

						break;
					}

					if (g_Enemy[i].wetTime >= WET_TIME_END)
					{
						g_Enemy[i].state = ENEMY_ABNORMAL_NONE;

						g_Enemy[i].wetTime = 0;
					}

				}

			}

			// �e���v���C���[�̈ʒu�ɍ��킹��
			XMFLOAT3 pos = g_Enemy[i].pos;
			pos.y -= (ENEMY_OFFSET_Y - 0.1f);
			SetPositionShadow(g_Enemy[i].shadowIdx, pos);

			for (int j = 0; j < MAX_ENEMY; j++)
			{
				if (i == j) continue;


				// �G���m�̓����蔻��
				float tx = g_Enemy[i].pos.x;
				float tz = g_Enemy[i].pos.z;

				XMFLOAT3 EnemyLowerPos = XMFLOAT3(g_Enemy[i].pos.x, g_Enemy[i].pos.y - 3.0f, g_Enemy[i].pos.z);
				XMFLOAT3 EnemyUpperPos = XMFLOAT3(g_Enemy[i].pos.x, g_Enemy[i].pos.y + 3.0f, g_Enemy[i].pos.z);

				//�J�v�Z���̓����蔻��
				if (CapsuleCollision(EnemyUpperPos, EnemyLowerPos, g_Enemy[j].pos, g_Enemy[i].size, g_Enemy[j].size))
				{
					g_Enemy[i].pos.x = OldPos.x;
					g_Enemy[i].pos.z = OldPos.z;
				}
			}

			if (g_Enemy[i].hp <= 0)
			{
				// dissolve�J�n
				g_Enemy[i].LeaveTime = TRUE;
				ReleaseShadow(g_Enemy[i].shadowIdx);
			}
		}


	}

#ifdef _DEBUG
	// �f�o�b�O�\��
	PrintDebugProc("LeaveTime %d \n", g_Enemy[0].LeaveTime);
	PrintDebugProc("Enemy[0].hp %d \n", g_Enemy[0].hp);



#endif

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawEnemy(void)
{
	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld;
	// �J�����O����
	SetCullingMode(CULL_MODE_NONE);

	for (int i = 0; i < MAX_ENEMY; i++)
	{
		// dissolve
		SetDissolve(&g_Enemy[i].dissolve);
		SetAlphaTestEnable(g_Enemy[i].dissolve.Enable);

		if (g_Enemy[i].use == FALSE) continue;

		// ���[���h�}�g���b�N�X�̏�����
		mtxWorld = XMMatrixIdentity();

		// �X�P�[���𔽉f
		mtxScl = XMMatrixScaling(g_Enemy[i].scl.x, g_Enemy[i].scl.y, g_Enemy[i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// ��]�𔽉f
		mtxRot = XMMatrixRotationRollPitchYaw(g_Enemy[i].rot.x, g_Enemy[i].rot.y + XM_PI, g_Enemy[i].rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// �ړ��𔽉f
		mtxTranslate = XMMatrixTranslation(g_Enemy[i].pos.x, g_Enemy[i].pos.y, g_Enemy[i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		// ���[���h�}�g���b�N�X�̐ݒ�
		SetWorldMatrix(&mtxWorld);

		XMStoreFloat4x4(&g_Enemy[i].mtxWorld, mtxWorld);


		// �K�w�A�j���[�V����
		// Parts�`��
		for (int j = 0; j < ENEMY_PARTS_MAX; j++)
		{
			// ���[���h�}�g���b�N�X�̏�����
			mtxWorld = XMMatrixIdentity();

			// �X�P�[���𔽉f
			mtxScl = XMMatrixScaling(g_Parts[i][j].scl.x, g_Parts[i][j].scl.y, g_Parts[i][j].scl.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

			// ��]�𔽉f
			mtxRot = XMMatrixRotationRollPitchYaw(g_Parts[i][j].rot.x, g_Parts[i][j].rot.y, g_Parts[i][j].rot.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

			// �ړ��𔽉f
			mtxTranslate = XMMatrixTranslation(g_Parts[i][j].pos.x, g_Parts[i][j].pos.y, g_Parts[i][j].pos.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

			if (g_Parts[i][j].parent != NULL)	// �q����������e�ƌ�������
			{
				mtxWorld = XMMatrixMultiply(mtxWorld, XMLoadFloat4x4(&g_Parts[i][j].parent->mtxWorld));
				// ��
				// g_Enemy.mtxWorld���w���Ă���
			}

			XMStoreFloat4x4(&g_Parts[i][j].mtxWorld, mtxWorld);

			// �g���Ă���Ȃ珈������
			if (g_Parts[i][j].use == FALSE) continue;

			// ���[���h�}�g���b�N�X�̐ݒ�
			SetWorldMatrix(&mtxWorld);

			// ���f���`��
			DrawModel(&g_PartsModel[g_Parts[i][j].modelIdx].model);
		}

		// dissolve
		SetDissolveEnable(FALSE);
		SetAlphaTestEnable(g_Enemy[i].dissolve.Enable);

	}



	// �J�����O�ݒ��߂�
	SetCullingMode(CULL_MODE_BACK);
}

//=============================================================================
// �G�l�~�[�̎擾
//=============================================================================
ENEMY *GetEnemy()
{
	return &g_Enemy[0];
}


void SetEnemy(void)
{
	int start = rand() % MAX_ENEMY;

	for (int n = start; n < MAX_ENEMY + start; n++)
	{
		int i = n % MAX_ENEMY;

		if (g_Enemy[i].use == FALSE)        // ���g�p��Ԃ�enemy��������
		{
			g_Enemy[i].type = rand() % ENEMY_COLOR_MAX;	//�F�̕ύX
			g_Enemy[i].state = ENEMY_ABNORMAL_NONE;

			ChangeModelEnemy(g_Enemy[i].type, i);

			XMFLOAT3 pos;

			pos.x = (float)(rand() % ENEMY_SPAWN_RANGE_X) - (ENEMY_SPAWN_RANGE_X * 0.5f);
			pos.y = ENEMY_OFFSET_Y;
			pos.z = (float)(rand() % ENEMY_SPAWN_RANGE_Z) - (ENEMY_SPAWN_RANGE_Z * 0.5f);

			g_Enemy[i].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);

			g_Enemy[i].use = TRUE;           // �g�p��Ԃ֕ύX����
			g_Enemy[i].EntryTime = TRUE;

			g_Enemy[i].pos = pos;            // ���W���Z�b�g

			g_Enemy[i].hp = ENEMY_HP_MAX;            // 

			pos.y -= (ENEMY_OFFSET_Y - 0.1f);
			g_Enemy[i].shadowIdx = CreateShadow(pos, ENEMY_SHADOW_SIZE, ENEMY_SHADOW_SIZE);


			XMFLOAT4 color = { 0.0f,0.0f,0.0f,0.0f };

			switch (g_Enemy[i].type)
			{
			case ENEMY_RED:
				SetParticle(g_Enemy[i].pos, XMFLOAT3(2.0f, 2.0f, 2.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f),
					TYPE_CHANGE_PLAYER, BLEND_MODE_SUBTRACT, 0.0f, 0.0f, 90);
				color = { 1.0f, 0.0f, 0.0f, 1.0f };
				g_Enemy[i].spd = (float)ENEMY_RED_SPD;

				break;

			case ENEMY_BLUE:
				SetParticle(g_Enemy[i].pos, XMFLOAT3(2.0f, 2.0f, 2.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.9f, 0.5f, 0.0f, 1.0f),
					TYPE_CHANGE_PLAYER, BLEND_MODE_SUBTRACT, 0.0f, 0.0f, 90);
				color = { 0.0f, 0.0f, 1.0f, 1.0f };
				g_Enemy[i].spd = (float)ENEMY_BLUE_SPD;

				break;

			case ENEMY_GREEN:
				SetParticle(g_Enemy[i].pos, XMFLOAT3(2.0f, 2.0f, 2.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.9f, 0.0f, 0.9f, 1.0f),
					TYPE_CHANGE_PLAYER, BLEND_MODE_SUBTRACT, 0.0f, 0.0f, 90);
				color = { 0.0f, 1.0f, 0.0f, 1.0f };
				g_Enemy[i].spd = (float)ENEMY_GREEN_SPD;

				break;

			case ENEMY_YELLOW:
				SetParticle(g_Enemy[i].pos, XMFLOAT3(2.0f, 2.0f, 2.0f), XMFLOAT3(0.0f, 5.0f, 0.0f), XMFLOAT4(0.0f, 0.0f, 0.9f, 1.0f),
					TYPE_CHANGE_PLAYER, BLEND_MODE_SUBTRACT, 0.0f, 0.0f, 90);
				color = { 1.0f, 1.0f, 0.0f, 1.0f };
				g_Enemy[i].spd = (float)ENEMY_YELLOW_SPD;

				break;
			}

			g_Enemy[i].dissolve.DissolveColor = color;

			return;                            // 1���Z�b�g�����̂ŏI������
		}
	}
	
}

void ChangeModelEnemy(int type, int id)
{
	// �ԍ��̐U�蕪��
	switch (type)
	{
	case ENEMY_RED:
		g_Parts[id][0].modelIdx = 0;
		g_Parts[id][1].modelIdx = 1;
		g_Parts[id][2].modelIdx = 2;
		g_Parts[id][3].modelIdx = 3;
		g_Parts[id][4].modelIdx = 4;
		g_Enemy[id].type = ENEMY_RED;
		break;

	case ENEMY_BLUE:
		g_Parts[id][0].modelIdx = 5;
		g_Parts[id][1].modelIdx = 6;
		g_Parts[id][2].modelIdx = 7;
		g_Parts[id][3].modelIdx = 8;
		g_Parts[id][4].modelIdx = 9;
		g_Enemy[id].type = ENEMY_BLUE;
		break;

	case ENEMY_GREEN:
		g_Parts[id][0].modelIdx = 10;
		g_Parts[id][1].modelIdx = 11;
		g_Parts[id][2].modelIdx = 12;
		g_Parts[id][3].modelIdx = 13;
		g_Parts[id][4].modelIdx = 14;
		g_Enemy[id].type = ENEMY_GREEN;
		break;

	case ENEMY_YELLOW:
		g_Parts[id][0].modelIdx = 15;
		g_Parts[id][1].modelIdx = 16;
		g_Parts[id][2].modelIdx = 17;
		g_Parts[id][3].modelIdx = 18;
		g_Parts[id][4].modelIdx = 19;
		g_Enemy[id].type = ENEMY_YELLOW;
		break;

	}
}