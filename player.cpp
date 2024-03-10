//=============================================================================
//
// �v���C���[���� [player.cpp]
// Author : HAL���� ������
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "light.h"
#include "input.h"
#include "camera.h"
#include "model.h"
#include "player.h"
#include "enemy.h"
#include "aim.h"
#include "shadow.h"
#include "bullet.h"
#include "bullet_charge.h"
#include "debugproc.h"
#include "meshfield.h"
#include "collision.h"
#include "score.h"
#include "sound.h"
#include "particle.h"
#include "particle_charge.h"
#include "flag.h"
#include "keyframe.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MODEL_PLAYER_RED			"data/MODEL/red/red_body.obj"			// �ǂݍ��ރ��f����
#define	MODEL_PLAYER_RED_HEAD		"data/MODEL/red/red_head.obj"			// �ǂݍ��ރ��f����
#define	MODEL_PLAYER_RED_LLEG		"data/MODEL/red/red_leg_l.obj"			// �ǂݍ��ރ��f����
#define	MODEL_PLAYER_RED_RLEG		"data/MODEL/red/red_leg_r.obj"			// �ǂݍ��ރ��f����
#define	MODEL_PLAYER_RED_TAIL		"data/MODEL/red/red_tail.obj"			// �ǂݍ��ރ��f����

#define	MODEL_PLAYER_BLUE			"data/MODEL/blue/blue_body.obj"			// �ǂݍ��ރ��f����
#define	MODEL_PLAYER_BLUE_HEAD		"data/MODEL/blue/blue_head.obj"			// �ǂݍ��ރ��f����
#define	MODEL_PLAYER_BLUE_LLEG		"data/MODEL/blue/blue_leg_l.obj"		// �ǂݍ��ރ��f����
#define	MODEL_PLAYER_BLUE_RLEG		"data/MODEL/blue/blue_leg_r.obj"		// �ǂݍ��ރ��f����
#define	MODEL_PLAYER_BLUE_TAIL		"data/MODEL/blue/blue_tail.obj"			// �ǂݍ��ރ��f����

#define	MODEL_PLAYER_GREEN			"data/MODEL/green/green_body.obj"		// �ǂݍ��ރ��f����
#define	MODEL_PLAYER_GREEN_HEAD		"data/MODEL/green/green_head.obj"		// �ǂݍ��ރ��f����
#define	MODEL_PLAYER_GREEN_LLEG		"data/MODEL/green/green_leg_l.obj"		// �ǂݍ��ރ��f����
#define	MODEL_PLAYER_GREEN_RLEG		"data/MODEL/green/green_leg_r.obj"		// �ǂݍ��ރ��f����
#define	MODEL_PLAYER_GREEN_TAIL		"data/MODEL/green/green_tail.obj"		// �ǂݍ��ރ��f����

#define	MODEL_PLAYER_YELLOW			"data/MODEL/yellow/yellow_body.obj"		// �ǂݍ��ރ��f����
#define	MODEL_PLAYER_YELLOW_HEAD	"data/MODEL/yellow/yellow_head.obj"		// �ǂݍ��ރ��f����
#define	MODEL_PLAYER_YELLOW_LLEG	"data/MODEL/yellow/yellow_leg_l.obj"	// �ǂݍ��ރ��f����
#define	MODEL_PLAYER_YELLOW_RLEG	"data/MODEL/yellow/yellow_leg_r.obj"	// �ǂݍ��ރ��f����
#define	MODEL_PLAYER_YELLOW_TAIL	"data/MODEL/yellow/yellow_tail.obj"		// �ǂݍ��ރ��f����

//--------------------------------------------------------------------------------------
// �L�[�t���[���ύX����
// �A�j���[�V�����̃f�B���N�g��
// �ҋ@���
#define ANIM_IDLE_BODY			"data/ANIMDATA/PLAYER/IDLE/RED_BODY.csv"
#define ANIM_IDLE_HEAD			"data/ANIMDATA/PLAYER/IDLE/RED_HEAD.csv"
#define ANIM_IDLE_LLEG			"data/ANIMDATA/PLAYER/IDLE/RED_LEG_L.csv"
#define ANIM_IDLE_RLEG			"data/ANIMDATA/PLAYER/IDLE/RED_LEG_R.csv"
#define ANIM_IDLE_TAIL			"data/ANIMDATA/PLAYER/IDLE/RED_TAIL.csv"
// �ړ����
#define ANIM_WALK_BODY			"data/ANIMDATA/PLAYER/WALK/RED_BODY.csv"
#define ANIM_WALK_HEAD			"data/ANIMDATA/PLAYER/WALK/RED_HEAD.csv"
#define ANIM_WALK_LLEG			"data/ANIMDATA/PLAYER/WALK/RED_LEG_L.csv"
#define ANIM_WALK_RLEG			"data/ANIMDATA/PLAYER/WALK/RED_LEG_R.csv"
#define ANIM_WALK_TAIL			"data/ANIMDATA/PLAYER/WALK/RED_TAIL.csv"
//�U�����
#define ANIM_HINOKO_BODY		"data/ANIMDATA/PLAYER/HINOKO/RED_BODY.csv"
#define ANIM_HINOKO_HEAD		"data/ANIMDATA/PLAYER/HINOKO/RED_HEAD.csv"
#define ANIM_HINOKO_LLEG		"data/ANIMDATA/PLAYER/HINOKO/RED_LEG_L.csv"
#define ANIM_HINOKO_RLEG		"data/ANIMDATA/PLAYER/HINOKO/RED_LEG_R.csv"
#define ANIM_HINOKO_TAIL		"data/ANIMDATA/PLAYER/HINOKO/RED_TAIL.csv"
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

#define	VALUE_MOVE					(2.0f)							// �ړ���
#define	VALUE_ROTATE				(D3DX_PI * 0.02f)				// ��]��

#define PLAYER_SHADOW_SIZE			(0.4f)							// �e�̑傫��
#define PLAYER_OFFSET_Y				(8.0f)							// �v���C���[�̑��������킹��

#define PLAYER_PARTS_MAX			(5)								// �v���C���[�̃p�[�c�̐�
#define PLAYER_PARTS_MODEL_MAX		(MAX_PLAYER_TYPE * PLAYER_PARTS_MAX)		// ���v�̃p�[�c�̐�

#define PLAYER_PARTS_SIZE			(10.0f)							// �v���C���[�̃p�[�c�̑傫��

#define HINOKO_COST					(15)							// �Ђ̂��̃R�X�g
#define CHARGE_COST					(60)							// �`���[�W�R�X�g

#define CHANGE_MPGAIN				(40)							// �v���C���[��㎞��MP�񕜗�
#define CHANGE_COOLTIME				(60)							// �v���C���[���̃N�[���^�C��

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void ChangeHinokoColor(void);
void hpGain(void);
void mpGain(void);
void AddMP(int amount);
void SubtractMP(int amount);

void InputMovePlayer(void);
void InputHinokoPlayer(void);
void InputCharge(void);
void WalkPlayer(void);
void HinokoPlayer(void);
void SetTblIdle(void);
void SetTblMove(void);
void SetTblHinoko(void);
void InputQuickTurn(void);

void RecastChangePlayer();

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static PLAYER			g_Player;						// �v���C���[

static PLAYER			g_Parts[MAX_PLAYER][PLAYER_PARTS_MAX];		// �v���C���[�̃p�[�c�p

static PLAYER_MODEL		g_PartsModel[MAX_PLAYER_TYPE * PLAYER_PARTS_MAX];	// �v���C���[�̎�ށ��p�[�c��

static char		g_ModelName[MAX_PLAYER_TYPE * PLAYER_PARTS_MAX][256] =
{
	MODEL_PLAYER_RED,
	MODEL_PLAYER_RED_HEAD,
	MODEL_PLAYER_RED_LLEG,
	MODEL_PLAYER_RED_RLEG,
	MODEL_PLAYER_RED_TAIL,

	MODEL_PLAYER_BLUE,
	MODEL_PLAYER_BLUE_HEAD,
	MODEL_PLAYER_BLUE_LLEG,
	MODEL_PLAYER_BLUE_RLEG,
	MODEL_PLAYER_BLUE_TAIL,

	MODEL_PLAYER_GREEN,
	MODEL_PLAYER_GREEN_HEAD,
	MODEL_PLAYER_GREEN_LLEG,
	MODEL_PLAYER_GREEN_RLEG,
	MODEL_PLAYER_GREEN_TAIL,

	MODEL_PLAYER_YELLOW,
	MODEL_PLAYER_YELLOW_HEAD,
	MODEL_PLAYER_YELLOW_LLEG,
	MODEL_PLAYER_YELLOW_RLEG,
	MODEL_PLAYER_YELLOW_TAIL,

};

static XMFLOAT4 color = { 0.0f, 0.0f, 0.0f, 1.0f };

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

static float		roty = 0.0f;
BOOL				soundF;
static LIGHT		g_Light;

int hinokoCnt;	// �Ђ̂����̃t���[���J�E���g


//�����蔻��p
enum
{
	tbl_left,
	tbl_right,
};


//=============================================================================
// ����������
//=============================================================================
HRESULT InitPlayer(void)
{
	//--------------------------------------------------------------------------------------
// �L�[�t���[���ύX����
// �A�j���[�V�����f�[�^���t�@�C������ǂݍ��� 
	for (int i = 0; i < TABLE_MAX; i++)
	{
		LoadAnimDataCSV(g_AnimDir[i], *g_TblAdr[i]);
	}
	//--------------------------------------------------------------------------------------

	g_Player.pos = XMFLOAT3(-100.0f, PLAYER_OFFSET_Y, -500.0f);
	g_Player.rot = XMFLOAT3(0.0f, XM_PI, 0.0f);
	g_Player.scl = XMFLOAT3(0.5f, 0.5f, 0.5f);

	g_Player.spd = 0.0f;										// �ړ��X�s�[�h�N���A

	g_Player.hp = PLAYER_HP_MAX;								// HP�ő�l
	g_Player.mp = PLAYER_MP_MAX;								// MP�ő�l

	g_Player.use = TRUE;										// TRUE:�����Ă�
	g_Player.size = PLAYER_SIZE;								// �����蔻��̑傫��

	g_Player.hinokoColor = XMFLOAT4(1.0f, 0.5f, 0.1f, 0.85f);
	g_Player.chargeCnt = 0;										// �`���[�W����
	g_Player.changeCnt = CHANGE_COOLTIME;						// �v���C���[���N�[���^�C�������p�̃J�E���g

	g_Player.state = STATE_IDLE;								// ���

	g_Player.modelIdx = NULL;									// ���f���ԍ�

	g_Player.dissolve.Enable = FALSE;							// �f�B�]���u
	g_Player.dissolve.threshold = 0.0f;


	// �����Ńv���C���[�p�̉e���쐬���Ă���
	XMFLOAT3 pos = g_Player.pos;
	pos.y -= (PLAYER_OFFSET_Y - 0.1f);
	g_Player.shadowIdx = CreateShadow(pos, PLAYER_SHADOW_SIZE, PLAYER_SHADOW_SIZE);

	roty = 0.0f;

	g_Player.parent = NULL;			// �{�́i�e�j�Ȃ̂�NULL������

	//	���f���̓ǂݍ���
	for (int i = 0; i < MAX_PLAYER_TYPE * PLAYER_PARTS_MAX; i++)
	{
		LoadModel(g_ModelName[i], &g_PartsModel[i].model);
		g_PartsModel[i].load = TRUE;
	}

	// �K�w�A�j���[�V�����̏�����
	for (int i = 0; i < MAX_PLAYER; i++)
	{
		for (int j = 0; j < PLAYER_PARTS_MAX; j++)
		{
			g_Parts[i][j].use = FALSE;

			// �ʒu�E��]�E�X�P�[���̏����ݒ�
			g_Parts[i][j].pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
			g_Parts[i][j].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
			g_Parts[i][j].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);

			// �e�q�֌W
			g_Parts[i][j].parent = &g_Player;		// �� �����ɐe�̃A�h���X������

			// �K�w�A�j���[�V�����p�̃����o�[�ϐ��̏�����
			g_Parts[i][j].time = 0.0f;			// ���`��ԗp�̃^�C�}�[���N���A
			g_Parts[i][j].tblNo = 0;			// �Đ�����s���f�[�^�e�[�u��No���Z�b�g
			g_Parts[i][j].tblMax = 0;			// �Đ�����s���f�[�^�e�[�u���̃��R�[�h�����Z�b�g

			g_Parts[i][j].modelIdx = NULL;		// �ԍ�
		}
	}

	g_Parts[0][0].use = TRUE;
	g_Parts[0][0].tblNo = IDLE_BODY;			// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[0][0].tblMax = (int)g_TblAdr[g_Parts[0][0].tblNo]->size();	// �� �ύX����

	g_Parts[0][1].use = TRUE;
	g_Parts[0][1].parent = &g_Parts[0][0];	// �e���Z�b�g
	g_Parts[0][1].tblNo = IDLE_HEAD;			// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[0][1].tblMax = (int)g_TblAdr[g_Parts[0][1].tblNo]->size();	// �� �ύX����

	g_Parts[0][2].use = TRUE;
	g_Parts[0][2].parent = &g_Parts[0][0];	// �e���Z�b�g
	g_Parts[0][2].tblNo = IDLE_LEG_L;			// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[0][2].tblMax = (int)g_TblAdr[g_Parts[0][2].tblNo]->size();	// �� �ύX����

	g_Parts[0][3].use = TRUE;
	g_Parts[0][3].parent = &g_Parts[0][0];	// �e���Z�b�g
	g_Parts[0][3].tblNo = IDLE_LEG_R;			// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[0][3].tblMax = (int)g_TblAdr[g_Parts[0][3].tblNo]->size();	// �� �ύX����

	g_Parts[0][4].use = TRUE;
	g_Parts[0][4].parent = &g_Parts[0][0];	// �e���Z�b�g
	g_Parts[0][4].tblNo = IDLE_TAIL;			// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Parts[0][4].tblMax = (int)g_TblAdr[g_Parts[0][4].tblNo]->size();	// �� �ύX����


	// ���f���̃f�B�t���[�Y��ۑ����Ă����B�F�ς��Ή��ׁ̈B
	GetModelDiffuse(&g_PartsModel[0].model, &g_PartsModel[0].diffuse[0]);

	GetModelDiffuse(&g_PartsModel[0].model, &g_PartsModel[0].diffuse[0]);
	GetModelDiffuse(&g_PartsModel[1].model, &g_PartsModel[1].diffuse[0]);
	GetModelDiffuse(&g_PartsModel[2].model, &g_PartsModel[2].diffuse[0]);
	GetModelDiffuse(&g_PartsModel[3].model, &g_PartsModel[3].diffuse[0]);
	GetModelDiffuse(&g_PartsModel[4].model, &g_PartsModel[4].diffuse[0]);

	ChangeModelPlayer(PLAYER_RED,0);


	soundF = FALSE;

	hinokoCnt = 0;

	// �N�H�[�^�j�I���̏�����
	XMStoreFloat4(&g_Player.Quaternion, XMQuaternionIdentity());

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitPlayer(void)
{
	for (int i = 0; i < MAX_PLAYER_TYPE * PLAYER_PARTS_MAX; i++)
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
void UpdatePlayer(void)
{
	CAMERA *cam = GetCamera();
	ENEMY* enemy = GetEnemy();
	BULLET* bullet = GetBullet();

	XMVECTOR vCameraAt = XMLoadFloat3(&cam->at);
	XMVECTOR vPlayerPos = XMLoadFloat3(&g_Player.pos);
	XMVECTOR vec = vPlayerPos - vCameraAt;
	float anglePlayer = atan2f(vec.m128_f32[0], vec.m128_f32[2]);

	g_Player.moving = FALSE;

	if (g_Player.state != STATE_DIE)
	{
		mpGain();
		//hpGain();
	}

	RecastChangePlayer();

	switch (g_Player.state)
	{
	case STATE_IDLE:
		InputMovePlayer();
		InputHinokoPlayer();
		InputCharge();
		InputQuickTurn();
		WalkPlayer();
		SetTblIdle();
		break;

	case STATE_WALK:
		InputMovePlayer();
		InputHinokoPlayer();
		InputCharge();
		InputQuickTurn();
		WalkPlayer();
		SetTblMove();
		break;

	case STATE_HINOKO:
		InputMovePlayer();
		InputHinokoPlayer();
		InputCharge();
		InputQuickTurn();
		WalkPlayer();
		HinokoPlayer();
		SetTblHinoko();
		break;

	default:
		break;
	}

	g_Player.spd *= 0.8f;
	g_Player.rot.y = anglePlayer;


	// ���C�L���X�g���đ����̍��������߂�
	XMFLOAT3 HitPosition;		// ��_
	XMFLOAT3 Normal;			// �Ԃ������|���S���̖@���x�N�g���i�����j
	BOOL ans = RayHitField(g_Player.pos, &HitPosition, &Normal);

	if (ans)
	{
		g_Player.pos.y = HitPosition.y + PLAYER_OFFSET_Y;
	}
	else
	{
		g_Player.pos.y = PLAYER_OFFSET_Y;
		Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	}

	// �U�����ĂȂ��Ƃ��Ƀ��f���`�F���W
	if (g_Player.attack == FALSE && g_Player.changeCnt >= CHANGE_COOLTIME)
	{
		BOOL change = FALSE;	// ���f���`�F���W�̔��ʗp

		// ���f���`�F���W�����P ���㉺�V�t�g�ɂ��؂�ւ�
		{
			if (GetKeyboardTrigger(DIK_O) || IsButtonTriggered(0, BUTTON_L2))
			{
				g_Player.type = (++g_Player.type + PLAYER_COLOR_MAX) % PLAYER_COLOR_MAX;
				change = TRUE;
			}
			else if (GetKeyboardTrigger(DIK_U) || IsButtonTriggered(0, BUTTON_L))
			{
				g_Player.type = (--g_Player.type + PLAYER_COLOR_MAX) % PLAYER_COLOR_MAX;
				change = TRUE;
			}
		}

		// ���f���`�F���W�����Q ���w��؂�ւ�
		{
			if ((GetKeyboardTrigger(DIK_1) || IsButtonTriggered(0, BUTTON_UP)) && g_Player.type != PLAYER_RED)
			{
				g_Player.type = PLAYER_RED;
				change = TRUE;
			}
			else if ((GetKeyboardTrigger(DIK_2) || IsButtonTriggered(0, BUTTON_RIGHT)) && g_Player.type != PLAYER_BLUE)
			{
				g_Player.type = PLAYER_BLUE;
				change = TRUE;
			}
			else if ((GetKeyboardTrigger(DIK_3) || IsButtonTriggered(0, BUTTON_DOWN)) && g_Player.type != PLAYER_GREEN)
			{
				g_Player.type = PLAYER_GREEN;
				change = TRUE;
			}
			else if ((GetKeyboardTrigger(DIK_4) || IsButtonTriggered(0, BUTTON_LEFT)) && g_Player.type != PLAYER_YELLOW)
			{
				g_Player.type = PLAYER_YELLOW;
				change = TRUE;
			}
		}

		// �t���O��TRUE�̏ꍇ�A���f���`�F���W
		if (change == TRUE)
		{
			switch (g_Player.type)
			{
			case PLAYER_RED:
				ChangeModelPlayer(PLAYER_RED, 0);
				SetParticle(g_Player.pos, XMFLOAT3(2.0f, 2.0f, 2.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(1.0f, 0.5f, 0.1f, 0.85f),
					TYPE_CHANGE_PLAYER, BLEND_MODE_ADD, 0.0f, 0.0f, 30);
				break;

			case PLAYER_BLUE:
				ChangeModelPlayer(PLAYER_BLUE, 0);
				SetParticle(g_Player.pos, XMFLOAT3(2.0f, 2.0f, 2.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.1f, 0.5f, 1.0f, 0.85f),
					TYPE_CHANGE_PLAYER, BLEND_MODE_ADD, 0.0f, 0.0f, 30);
				break;

			case PLAYER_GREEN:
				ChangeModelPlayer(PLAYER_GREEN, 0);
				SetParticle(g_Player.pos, XMFLOAT3(2.0f, 2.0f, 2.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.1f, 1.0f, 0.1f, 0.85f),
					TYPE_CHANGE_PLAYER, BLEND_MODE_ADD, 0.0f, 0.0f, 30);
				break;

			case PLAYER_YELLOW:
				ChangeModelPlayer(PLAYER_YELLOW, 0);
				SetParticle(g_Player.pos, XMFLOAT3(2.0f, 2.0f, 2.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 0.1f, 0.85f),
					TYPE_CHANGE_PLAYER, BLEND_MODE_ADD, 0.0f, 0.0f, 30);
				break;
			}

			//g_Player.mp += CHANGE_MPGAIN;
			AddMP(CHANGE_MPGAIN);
			g_Player.changeCnt = 0;    // �v���C���[�`�F���W��A���L���X�g�J�E���g��0�ɂ���
			PlaySound(SOUND_LABEL_SE_CHANGE);
		}

		//if ((GetKeyboardTrigger(DIK_1) || IsButtonTriggered(0,BUTTON_UP)) && g_Player.type != PLAYER_RED)
		//{
		//	ChangeModelPlayer(PLAYER_RED,0);
		//	SetParticle(g_Player.pos, XMFLOAT3(2.0f, 2.0f, 2.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(1.0f, 0.5f, 0.1f, 0.85f),
		//		TYPE_CHANGE_PLAYER, BLEND_MODE_ADD, 0.0f, 0.0f, 30);
		//	PlaySound(SOUND_LABEL_SE_CHANGE);

		//	g_Player.mp += CHANGE_MPGAIN;
		//	g_Player.changeCnt = 0;    // �v���C���[�`�F���W��A���L���X�g�J�E���g��0�ɂ���
		//	g_Player.type = PLAYER_RED;
		//}

		//else if ((GetKeyboardTrigger(DIK_2) || IsButtonTriggered(0, BUTTON_RIGHT)) && g_Player.type != PLAYER_BLUE)
		//{
		//	ChangeModelPlayer(PLAYER_BLUE,0);
		//	SetParticle(g_Player.pos, XMFLOAT3(2.0f, 2.0f, 2.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.1f, 0.5f, 1.0f, 0.85f),
		//		TYPE_CHANGE_PLAYER, BLEND_MODE_ADD, 0.0f, 0.0f, 30);
		//	PlaySound(SOUND_LABEL_SE_CHANGE);

		//	g_Player.mp += CHANGE_MPGAIN;
		//	g_Player.changeCnt = 0;    // �v���C���[�`�F���W��A���L���X�g�J�E���g��0�ɂ���
		//	g_Player.type = PLAYER_BLUE;
		//}

		//else if ((GetKeyboardTrigger(DIK_3) || IsButtonTriggered(0, BUTTON_DOWN)) && g_Player.type != PLAYER_GREEN)
		//{
		//	ChangeModelPlayer(PLAYER_GREEN,0);
		//	SetParticle(g_Player.pos, XMFLOAT3(2.0f, 2.0f, 2.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.1f, 1.0f, 0.1f, 0.85f),
		//		TYPE_CHANGE_PLAYER, BLEND_MODE_ADD, 0.0f, 0.0f, 30);
		//	PlaySound(SOUND_LABEL_SE_CHANGE);

		//	g_Player.mp += CHANGE_MPGAIN;
		//	g_Player.changeCnt = 0;    // �v���C���[�`�F���W��A���L���X�g�J�E���g��0�ɂ���
		//	g_Player.type = PLAYER_GREEN;
		//}

		//else if ((GetKeyboardTrigger(DIK_4) || IsButtonTriggered(0, BUTTON_LEFT)) && g_Player.type != PLAYER_YELLOW)
		//{
		//	ChangeModelPlayer(PLAYER_YELLOW,0);
		//	SetParticle(g_Player.pos, XMFLOAT3(2.0f, 2.0f, 2.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 0.1f, 0.85f),
		//		TYPE_CHANGE_PLAYER, BLEND_MODE_ADD, 0.0f, 0.0f, 30);
		//	PlaySound(SOUND_LABEL_SE_CHANGE);

		//	g_Player.mp += CHANGE_MPGAIN;
		//	g_Player.changeCnt = 0;    // �v���C���[�`�F���W��A���L���X�g�J�E���g��0�ɂ���
		//	g_Player.type = PLAYER_YELLOW;
		//}
	}

	ChangeHinokoColor();	// �F�ς��̃^�C�~���O

	hinokoCnt++;

	if (hinokoCnt > 60)
	{
		g_Player.attack = FALSE;
		hinokoCnt = 0;
	}

	if (g_Player.state != STATE_DIE)
	{
		//�������ĂȂ��Ƃ��̓A�C�h����Ԃɖ߂�
		if ((g_Player.moving == FALSE) && (g_Player.attack == FALSE))
		{
			g_Player.state = STATE_IDLE;
		}
	}

	// �e���v���C���[�̈ʒu�ɍ��킹��
	XMFLOAT3 pos = g_Player.pos;
	pos.y -= (PLAYER_OFFSET_Y - 0.1f);
	SetPositionShadow(g_Player.shadowIdx, pos);

	// �K�w�A�j���[�V����
	for (int i = 0; i < PLAYER_PARTS_MAX; i++)
	{
		// �g���Ă���Ȃ珈������
		if ((g_Parts[0][i].use == TRUE) && (g_Parts[0][i].tblMax > 0))
		{	// ���`��Ԃ̏���
			int nowNo = (int)g_Parts[0][i].time;			// �������ł���e�[�u���ԍ������o���Ă���
			int maxNo = g_Parts[0][i].tblMax;				// �o�^�e�[�u�����𐔂��Ă���
			int nextNo = (nowNo + 1) % maxNo;			// �ړ���e�[�u���̔ԍ������߂Ă���

			//--------------------------------------------------------------------------------------
			// �L�[�t���[���ύX����
			//INTERPOLATION_DATA* tbl = g_AnimTblAdr[g_Parts[i].tblNo];	// �s���e�[�u���̃A�h���X���擾
			std::vector<INTERPOLATION_DATA> tbl = *g_TblAdr[g_Parts[0][i].tblNo];
			//--------------------------------------------------------------------------------------

			XMVECTOR nowPos = XMLoadFloat3(&tbl[nowNo].pos);	// XMVECTOR�֕ϊ�
			XMVECTOR nowRot = XMLoadFloat3(&tbl[nowNo].rot);	// XMVECTOR�֕ϊ�
			XMVECTOR nowScl = XMLoadFloat3(&tbl[nowNo].scl);	// XMVECTOR�֕ϊ�

			XMVECTOR Pos = XMLoadFloat3(&tbl[nextNo].pos) - nowPos;	// XYZ�ړ��ʂ��v�Z���Ă���
			XMVECTOR Rot = XMLoadFloat3(&tbl[nextNo].rot) - nowRot;	// XYZ��]�ʂ��v�Z���Ă���
			XMVECTOR Scl = XMLoadFloat3(&tbl[nextNo].scl) - nowScl;	// XYZ�g�嗦���v�Z���Ă���

			float nowTime = g_Parts[0][i].time - nowNo;	// ���ԕ����ł��鏭�������o���Ă���

			Pos *= nowTime;								// ���݂̈ړ��ʂ��v�Z���Ă���
			Rot *= nowTime;								// ���݂̉�]�ʂ��v�Z���Ă���
			Scl *= nowTime;								// ���݂̊g�嗦���v�Z���Ă���

			// �v�Z���ċ��߂��ړ��ʂ����݂̈ړ��e�[�u��XYZ�ɑ����Ă��遁�\�����W�����߂Ă���
			XMStoreFloat3(&g_Parts[0][i].pos, nowPos + Pos);

			// �v�Z���ċ��߂���]�ʂ����݂̈ړ��e�[�u���ɑ����Ă���
			XMStoreFloat3(&g_Parts[0][i].rot, nowRot + Rot);

			// �v�Z���ċ��߂��g�嗦�����݂̈ړ��e�[�u���ɑ����Ă���
			XMStoreFloat3(&g_Parts[0][i].scl, nowScl + Scl);

			// frame���g�Ď��Ԍo�ߏ���������
			g_Parts[0][i].time += 1.0f / tbl[nowNo].frame;	// ���Ԃ�i�߂Ă���
			if ((int)g_Parts[0][i].time >= maxNo)			// �o�^�e�[�u���Ō�܂ňړ��������H
			{
				g_Parts[0][i].time -= maxNo;				// �O�ԖڂɃ��Z�b�g�������������������p���ł���

				switch (g_Player.setTbl)
				{
				case STATE_HINOKO:
					g_Parts[0][i].tblMax = -1;
					break;
				}
			}

		}

	}

	// �Q�[���I�[�o�[����
	if (g_Player.hp <= 0)
	{
		g_Player.state = STATE_DIE;
	}

	if (g_Player.state == STATE_DIE)
	{
		g_Player.dissolve.Enable = TRUE;
		g_Player.dissolve.threshold += 0.01f;
	}

	{	// �|�C���g���C�g�̃e�X�g
		LIGHT* light = GetLightData(1);
		XMFLOAT3 pos = g_Player.pos;

		pos.y += 70.0f;

		light->Position = pos;
		light->Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		light->Ambient = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		light->Type = LIGHT_TYPE_POINT;
		light->Enable = TRUE;
		SetLightData(1, light);
	}



	//////////////////////////////////////////////////////////////////////
	// �p������
	//////////////////////////////////////////////////////////////////////

	XMVECTOR vx, nvx, up;
	XMVECTOR quat;
	float len, angle;


	g_Player.UpVector = Normal;
	up = { 0.0f, 1.0f, 0.0f, 0.0f };
	vx = XMVector3Cross(up, XMLoadFloat3(&g_Player.UpVector));

	nvx = XMVector3Length(vx);
	XMStoreFloat(&len, nvx);
	nvx = XMVector3Normalize(vx);
	angle = asinf(len);

	quat = XMQuaternionRotationNormal(nvx, angle);

	quat = XMQuaternionSlerp(XMLoadFloat4(&g_Player.Quaternion), quat, 0.05f);
	XMStoreFloat4(&g_Player.Quaternion, quat);


#ifdef _DEBUG
	// �f�o�b�O�\��
	char state[STATE_MAX][256] = 
	{
		"STATE_IDLE",
		"STATE_WALK",
		"STATE_HINOKO",
		"STATE_DIE",
	};

	PrintDebugProc("Player X:%f Y:%f Z:% N:%f\n", g_Player.pos.x, g_Player.pos.y, g_Player.pos.z, Normal.y);
	PrintDebugProc("HP :%d\n", g_Player.hp);
	PrintDebugProc("MP :%d\n", g_Player.mp);
	PrintDebugProc("state :%s\n", state[g_Player.state]);

	if (GetKeyboardTrigger(DIK_0))
	{
		g_Player.hp = 0;
	}

	if (GetKeyboardTrigger(DIK_9))
	{
		g_Player.mp = PLAYER_MP_MAX;
	}


#endif

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawPlayer(void)
{
	if (g_Player.use == FALSE) return;

	// �J�����O����
	SetCullingMode(CULL_MODE_NONE);

	// dissolve
	SetDissolve(&g_Player.dissolve);
	SetAlphaTestEnable(g_Player.dissolve.Enable);

	XMMATRIX mtxScl, mtxRot, mtxTranslate, mtxWorld, quatMatrix;

	// �J�����O����
	SetCullingMode(CULL_MODE_NONE);

	// ���[���h�}�g���b�N�X�̏�����
	mtxWorld = XMMatrixIdentity();

	// �X�P�[���𔽉f
	mtxScl = XMMatrixScaling(g_Player.scl.x, g_Player.scl.y, g_Player.scl.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

	// ��]�𔽉f
	mtxRot = XMMatrixRotationRollPitchYaw(g_Player.rot.x, g_Player.rot.y + XM_PI, g_Player.rot.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

	// �N�H�[�^�j�I���𔽉f
	quatMatrix = XMMatrixRotationQuaternion(XMLoadFloat4(&g_Player.Quaternion));
	mtxWorld = XMMatrixMultiply(mtxWorld, quatMatrix);

	// �ړ��𔽉f
	mtxTranslate = XMMatrixTranslation(g_Player.pos.x, g_Player.pos.y, g_Player.pos.z);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

	// ���[���h�}�g���b�N�X�̐ݒ�
	SetWorldMatrix(&mtxWorld);

	XMStoreFloat4x4(&g_Player.mtxWorld, mtxWorld);



	// �K�w�A�j���[�V����
	for (int i = 0; i < PLAYER_PARTS_MAX; i++)
	{
		// ���[���h�}�g���b�N�X�̏�����
		mtxWorld = XMMatrixIdentity();

		// �X�P�[���𔽉f
		mtxScl = XMMatrixScaling(g_Parts[0][i].scl.x, g_Parts[0][i].scl.y, g_Parts[0][i].scl.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

		// ��]�𔽉f
		mtxRot = XMMatrixRotationRollPitchYaw(g_Parts[0][i].rot.x, g_Parts[0][i].rot.y, g_Parts[0][i].rot.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

		// �ړ��𔽉f
		mtxTranslate = XMMatrixTranslation(g_Parts[0][i].pos.x, g_Parts[0][i].pos.y, g_Parts[0][i].pos.z);
		mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

		if (g_Parts[0][i].parent != NULL)	// �q����������e�ƌ�������
		{
			mtxWorld = XMMatrixMultiply(mtxWorld, XMLoadFloat4x4(&g_Parts[0][i].parent->mtxWorld));
																			// ��
																			// g_Player.mtxWorld���w���Ă���
		}

		XMStoreFloat4x4(&g_Parts[0][i].mtxWorld, mtxWorld);

		// �g���Ă���Ȃ珈������
		if (g_Parts[0][i].use == FALSE) continue;

		// ���[���h�}�g���b�N�X�̐ݒ�
		SetWorldMatrix(&mtxWorld);

		// ���f���`��
		DrawModel(&g_PartsModel[g_Parts[0][i].modelIdx].model);

	}

	// dissolve
	SetDissolveEnable(FALSE);
	SetAlphaTestEnable(g_Player.dissolve.Enable);

	// �J�����O�ݒ��߂�
	SetCullingMode(CULL_MODE_BACK);
}


//=============================================================================
// �v���C���[�����擾
//=============================================================================
PLAYER *GetPlayer(void)
{
	return &g_Player;
}



// �q�̓����蔻��(�p�[�c�̔ԍ��A�p�[�c���a�A�G�l�~�[�̍��W�A�G�l�~�[�̔��a)
BOOL Collision(int index, float r, XMFLOAT3 epos, float er)
{
	// �p�[�c�̎q�̕t�����̍��W�����߂Ă���
	XMMATRIX mtx = XMLoadFloat4x4(&g_Parts[0][index].mtxWorld);

	// ���߂����W�����Ă�
	XMFLOAT3 pos = XMFLOAT3(mtx.r[3].m128_f32[0], mtx.r[3].m128_f32[1], mtx.r[3].m128_f32[2]);

	//BC�̓����蔻��̌��ʂ�Ԃ�
	return CollisionBC(pos, epos, r, er);
}

//=============================================================================
// �L�[�{�[�h�̓��͏���
//=============================================================================

BOOL AnyKeyPress(void)
{
	// �ǂꂩ�������ꂽ��
	return (MoveLeftKeyboard() || MoveRightKeyboard() || MoveUpKeyboard() || MoveDownKeyboard());

}

// ���ړ�
BOOL MoveLeftKeyboard(void)
{
	return GetKeyboardPress(DIK_A);
}

// �E�ړ�
BOOL MoveRightKeyboard(void)
{
	return GetKeyboardPress(DIK_D);

}

// ��ړ�
BOOL MoveUpKeyboard(void)
{
	return GetKeyboardPress(DIK_W);
}

// ���ړ�
BOOL MoveDownKeyboard(void)
{
	return GetKeyboardPress(DIK_S);
}


//=============================================================================
// �X�e�B�b�N�̓��͏���
//=============================================================================

// �ǂꂩ�������ꂽ��
BOOL AnyStickpress(void)
{
	return (MoveLeftStick() || MoveRightStick() || MoveUpStick() || MoveDownStick());
}

// ���ړ�
BOOL MoveLeftStick(void)
{
	return IsButtonPressed(0, STICK_L_LEFT);
}

// �E�ړ�
BOOL MoveRightStick(void)
{
	return IsButtonPressed(0, STICK_L_RIGHT);
}

// ��ړ�
BOOL MoveUpStick(void)
{
	return IsButtonPressed(0, STICK_L_UP);
}

// ���ړ�
BOOL MoveDownStick(void)
{
	return IsButtonPressed(0, STICK_L_DOWN);
}

void ChangeModelPlayer(int type, int id)
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
		g_Player.type = PLAYER_RED;
		break;

	case ENEMY_BLUE:
		g_Parts[id][0].modelIdx = 5;
		g_Parts[id][1].modelIdx = 6;
		g_Parts[id][2].modelIdx = 7;
		g_Parts[id][3].modelIdx = 8;
		g_Parts[id][4].modelIdx = 9;
		g_Player.type = PLAYER_BLUE;
		break;

	case ENEMY_GREEN:
		g_Parts[id][0].modelIdx = 10;
		g_Parts[id][1].modelIdx = 11;
		g_Parts[id][2].modelIdx = 12;
		g_Parts[id][3].modelIdx = 13;
		g_Parts[id][4].modelIdx = 14;
		g_Player.type = PLAYER_GREEN;
		break;

	case ENEMY_YELLOW:
		g_Parts[id][0].modelIdx = 15;
		g_Parts[id][1].modelIdx = 16;
		g_Parts[id][2].modelIdx = 17;
		g_Parts[id][3].modelIdx = 18;
		g_Parts[id][4].modelIdx = 19;
		g_Player.type = PLAYER_YELLOW;
		break;

	}

	
}

// ���t���[�����ƂɂЂ̂��̃J�E���g���O�̎��ɁA�F��ς���
void ChangeHinokoColor(void)
{
	if (g_Player.attack == FALSE)
	{
		switch (g_Player.type)
		{
		case PLAYER_RED:
			g_Player.hinokoColor = XMFLOAT4(1.0f, 0.5f, 0.1f, 0.85f);
			break;

		case PLAYER_BLUE:
			g_Player.hinokoColor = XMFLOAT4(0.1f, 0.5f, 1.0f, 0.85f);
			break;

		case PLAYER_GREEN:
			g_Player.hinokoColor = XMFLOAT4(0.1f, 1.0f, 0.1f, 0.85f);
			break;

		case PLAYER_YELLOW:
			g_Player.hinokoColor = XMFLOAT4(1.0f, 1.0f, 0.1f, 0.85f);
			break;

		default:
			break;
		}
	}
}

// HP�̎��R��
void hpGain(void)
{
	g_Player.HPGainCnt++;
	if (g_Player.HPGainCnt % HP_GAIN_FRAME == 0)
	{
		if (g_Player.hp < PLAYER_HP_MAX)
		{
			g_Player.hp++;
		}
		g_Player.HPGainCnt = 0;
	}
}

// MP�̎��R��
void mpGain(void)
{
	g_Player.MPGainCnt++;
	if (g_Player.MPGainCnt % MP_GAIN_FRAME == 0)
	{
		if (g_Player.mp < PLAYER_MP_MAX)
		{
			g_Player.mp++;
		}
		g_Player.MPGainCnt = 0;
	}
}

// MP�̉�
void AddMP(int amount)
{
	g_Player.mp += amount;

	if (g_Player.mp > PLAYER_MP_MAX) g_Player.mp = PLAYER_MP_MAX;
}

// MP�̏���
void SubtractMP(int amount)
{
	g_Player.mp -= amount;

	if (g_Player.mp < 0) g_Player.mp = 0;
}

void InputMovePlayer(void)
{
	// �ړ�����
	if (AnyKeyPress())
	{
		g_Player.moving = TRUE;

		if (g_Player.state != STATE_HINOKO)
		{
			g_Player.state = STATE_WALK;
		}

		g_Player.spd = VALUE_MOVE;

		float x = 0.0f;
		float z = 0.0f;

		if (MoveLeftKeyboard())		x = -1.0f;
		if (MoveRightKeyboard())	x = 1.0f;
		if (MoveUpKeyboard())		z = -1.0f;
		if (MoveDownKeyboard())		z = 1.0f;


		float angle = atan2f(x, z) * -1.0f;
		g_Player.dir = angle;
	}

	if (AnyStickpress())
	{
		g_Player.moving = TRUE;

		if (g_Player.state != STATE_HINOKO)
		{
		g_Player.state = STATE_WALK;
		}

		g_Player.spd = VALUE_MOVE;
		DIJOYSTATE2 dijs = GetDeviceState();
		float angle = atan2f((float)dijs.lX, (float)dijs.lY) * -1.0f;
		g_Player.dir = angle;
	}
}

void WalkPlayer(void)
{

	XMFLOAT3 OldPos = g_Player.pos;

	// Key���͂���������ړ���������
	CAMERA* cam = GetCamera();

	if (g_Player.spd > 0.0f)
	{
		g_Player.rot.y = g_Player.dir + cam->rot.y;

		// ���͂̂����������փv���C���[���������Ĉړ�������
		g_Player.pos.x -= sinf(g_Player.rot.y) * g_Player.spd;
		g_Player.pos.z -= cosf(g_Player.rot.y) * g_Player.spd;
	}

	switch (g_Player.state)
	{
	case STATE_WALK:
		if (g_Player.spd < VALUE_MOVE) g_Player.state = STATE_IDLE;
		break;

	default:
		break;
	}


	// �t�B�[���h�̊O�ɏo����v���C���[��߂�����
	if (g_Player.pos.x - g_Player.size < MAP_LEFT
		|| g_Player.pos.x + g_Player.size> MAP_RIGHT
		|| g_Player.pos.z - g_Player.size< MAP_DOWN
		|| g_Player.pos.z + g_Player.size> MAP_TOP)
	{
		g_Player.pos = OldPos;
	}

}

void InputHinokoPlayer(void)
{
	// �e���ˏ���
	if (GetKeyboardTrigger(DIK_SPACE) || IsButtonTriggered(0,BUTTON_R))
	{
		hinokoCnt = 0;
		g_Player.state = STATE_HINOKO;
		g_Player.attack = TRUE;
		PlaySound(SOUND_LABEL_SE_FIRE);

		if (g_Player.mp >= HINOKO_COST)
		{
			SetBullet(g_Player.pos, g_Player.rot);

			switch (g_Player.type)
			{
			case PLAYER_RED:
				g_Player.mp -= HINOKO_COST;
				g_Player.damage = PLAYER_RED_DAMAGE_VALUE;
				break;

			case PLAYER_BLUE:
				g_Player.mp -= (HINOKO_COST * 2);
				g_Player.damage = PLAYER_GREEN_DAMAGE_VALUE;
				break;

			case PLAYER_GREEN:
				g_Player.mp -= HINOKO_COST;
				g_Player.damage = PLAYER_BLUE_DAMAGE_VALUE;
				break;

			case PLAYER_YELLOW:
				g_Player.mp -= (HINOKO_COST * 2);
				g_Player.damage = PLAYER_YELLOW_DAMAGE_VALUE;
				break;
			}
		}
	}
}

void InputCharge(void)
{
	BULLET* bullet = GetBullet();

	// �e���ˏ���
	if (GetKeyboardRelease(DIK_RETURN) || IsButtonRelease(0, BUTTON_R2))
	{
		soundF = FALSE;
		StopSound(SOUND_LABEL_SE_CHARGE);

		if (g_Player.chargeCnt == CHARGE_CNT_MAX && g_Player.mp > CHARGE_COST)
		{
			PlaySound(SOUND_LABEL_SE_FIRE);
			SetBullet_Charge(g_Player.pos, g_Player.rot);
			g_Player.mp -= CHARGE_COST;
			g_Player.chargeCnt = 0;
		}
	}

	// �`���[�W������
	if (GetKeyboardPress(DIK_RETURN) || IsButtonPressed(0,BUTTON_R2))
	{
		if (soundF == FALSE)
		{
			PlaySound(SOUND_LABEL_SE_CHARGE);
			soundF = TRUE;
		}

		g_Player.state = STATE_HINOKO;
		g_Player.attack = TRUE;

		if (g_Player.chargeCnt < CHARGE_CNT_MAX && g_Player.mp >= CHARGE_COST)
		{
			g_Player.chargeCnt++;
		}

		// �`���[�W���̏W�܂�p�[�e�B�N��
		if (g_Player.mp >= CHARGE_COST)
		{
			XMFLOAT3 sPos;
			sPos.x = g_Player.pos.x - (rand() % 101 - 50);
			sPos.y = g_Player.pos.y - (rand() % 101 - 50);
			sPos.z = g_Player.pos.z - (rand() % 101 - 50);

			XMFLOAT3 ePos = g_Player.pos;

			switch (g_Player.type)
			{
			case PLAYER_RED:
				g_Player.damage = PLAYER_RED_DAMAGE_VALUE * 2;
				color = { 1.0f, 0.5f, 0.1f, 1.0f };
				SetParticleCharge(sPos, ePos, { 0.1f, 0.1f, 0.1f }, color, PARTICLE_CHARGE_RED, 60);
				break;

			case PLAYER_BLUE:
				g_Player.damage = PLAYER_GREEN_DAMAGE_VALUE * 2;
				color = { 0.1f, 0.5f, 1.0f, 1.0f };
				SetParticleCharge(sPos, ePos, { 0.1f, 0.1f, 0.1f }, color, PARTICLE_CHARGE_BLUE, 60);
				break;

			case PLAYER_GREEN:
				g_Player.damage = PLAYER_BLUE_DAMAGE_VALUE * 2;
				color = { 0.1f, 1.0f, 0.1f, 1.0f };
				SetParticleCharge(sPos, ePos, { 0.1f, 0.1f, 0.1f }, color, PARTICLE_CHARGE_GREEN, 60);
				break;

			case PLAYER_YELLOW:
				g_Player.damage = PLAYER_YELLOW_DAMAGE_VALUE * 2;
				color = { 1.0f, 1.0f, 0.1f, 1.0f };
				SetParticleCharge(sPos, ePos, { 0.1f, 0.1f, 0.1f }, color, PARTICLE_CHARGE_YELLOW, 60);
				break;
			}
		}
	}
	else
	{
		if(g_Player.chargeCnt > 0)
		{
			g_Player.chargeCnt--;
		}
	}

}

void HinokoPlayer(void)
{
	// ��ԑJ��
	{
		BOOL ans = TRUE;

		// ���ׂẴp�[�c�̍ő�e�[�u������ -1 �ɂȂ��Ă���ꍇ(���A�j���[�V�������I�����Ă���ꍇ)�A��ԑJ�ڂ�����
		for (int i = 0; i < PLAYER_PARTS_MAX; i++)
		{
			if (g_Parts[0][i].tblMax != -1) ans = FALSE;
		}

		if (ans == TRUE) g_Player.state = STATE_IDLE;
	}

	hinokoCnt++;
	if (hinokoCnt > 60)
	{
		g_Player.attack = FALSE;
		hinokoCnt = 0;
	}

	if ((g_Player.moving == FALSE) && (g_Player.attack == FALSE))
	{
		g_Player.state = STATE_IDLE;
	}

}


void SetTblIdle(void)
{
	switch (g_Player.setTbl)
	{
	case STATE_IDLE:
		break;

	default:
		for (int i = 0; i < PLAYER_PARTS_MAX; i++)
		{
			if (g_Parts[0][i].use == FALSE) continue;

			g_Parts[0][i].time = 0.0f;
			g_Parts[0][i].tblNo = i + IDLE_BODY;
			g_Parts[0][i].tblMax = (int)g_TblAdr[g_Parts[0][i].tblNo]->size();

		}
		g_Player.setTbl = STATE_IDLE;
		break;
	}
}

void SetTblMove(void)
{
	switch (g_Player.setTbl)
	{
	case STATE_WALK:
		break;

	default:
		for (int i = 0; i < PLAYER_PARTS_MAX; i++)
		{
			if (g_Parts[0][i].use == FALSE) continue;

			g_Parts[0][i].time = 0.0f;
			g_Parts[0][i].tblNo = i + WALK_BODY;
			g_Parts[0][i].tblMax = (int)g_TblAdr[g_Parts[0][i].tblNo]->size();

		}
		g_Player.setTbl = STATE_WALK;
		break;
	}
}

void SetTblHinoko(void)
{
	switch (g_Player.setTbl)
	{
	case STATE_HINOKO:
		break;

	default:
		for (int i = 0; i < PLAYER_PARTS_MAX; i++)
		{
			if (g_Parts[0][i].use == FALSE) continue;

			g_Parts[0][i].time = 0.0f;
			g_Parts[0][i].tblNo = i + HINOKO_BODY;
			g_Parts[0][i].tblMax = (int)g_TblAdr[g_Parts[0][i].tblNo]->size();

		}
		g_Player.setTbl = STATE_HINOKO;
		break;
	}
}

void InputQuickTurn(void)
{
	if (GetKeyboardTrigger(DIK_R))
	{
		CAMERA* cam = GetCamera();

		g_Player.rot.y += XM_PI;

		if (g_Player.rot.y > XM_PI)
		{
			g_Player.rot.y -= XM_2PI;
		}
	}
}


void  RecastChangePlayer()
{
	// �v���C���[�`�F���W�J�E���g�����L���X�g���Ԃ�菬�����ꍇ
	if (g_Player.changeCnt < CHANGE_COOLTIME)
	{
		g_Player.changeCnt++;    // ���L���X�g�J�E���g��i�߂�
	}
}