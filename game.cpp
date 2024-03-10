//=============================================================================
//
// �Q�[����ʏ��� [game.cpp]
// Author : HAL���� ������
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "model.h"
#include "game.h"
#include "camera.h"
#include "input.h"
#include "sound.h"
#include "fade.h"

#include "player.h"
#include "enemy.h"
#include "boss.h"
#include "meshfield.h"
#include "meshwall.h"
#include "light.h"
#include "shadow.h"
#include "obj.h"
#include "BG.h"
#include "aim.h"
#include "bullet.h"
#include "bullet_charge.h"
#include "score.h"
#include "particle.h"
#include "particle_spark.h"
#include "particle_charge.h"
#include "collision.h"
#include "debugproc.h"
#include "ui.h"
#include "ui_enemy.h"
#include "event.h"
#include "flag.h"

#include "pause.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MONOKURO_END		(150)			// ���m�N�����I��鎞��



//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void CheckHit(void);



//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static GAME		g_Game;

static int		g_ViewPortType_Game = TYPE_FULL_SCREEN;

static int		g_MonokuroCnt = 0;

// ���C�v
static BOOL						g_wipe = FALSE;
static float					g_wipeSize = 0.0f;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitGame(void)
{
	g_MonokuroCnt = 0;

	// ���m�N���𒼂�
	SetMonochrome(FALSE);

	// ���C�v��������
	SetWipe(WIPE_MODE_NONE, XMFLOAT4(1.0f, 1.0f, 0.0f, 0.0f), g_wipeSize);

	// �|�[�Y�̏�����
	g_Game.bPause = FALSE;

	// �t�B�[���h�̏�����
	InitMeshField(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), 100, 100, 13.0f, 13.0f);

	// ���C�g��L����	
	InitLight();

	// �e�̏���������
	InitShadow();

	// �v���C���[�̏�����
	InitPlayer();

	// �G�l�~�[�̏�����
	InitEnemy();

	// �{�X�̏�����
	InitBoss();

	// �ǂ̏�����
	InitMeshWall(XMFLOAT3(0.0f, 0.0f, MAP_TOP), XMFLOAT3(0.0f, 0.0f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 16, 1, 80.0f, 40.0f);
	InitMeshWall(XMFLOAT3(MAP_LEFT, 0.0f, 0.0f), XMFLOAT3(0.0f, -XM_PI * 0.50f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 16, 1, 80.0f, 40.0f);
	InitMeshWall(XMFLOAT3(MAP_RIGHT, 0.0f, 0.0f), XMFLOAT3(0.0f, XM_PI * 0.50f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 16, 1, 80.0f, 40.0f);
	InitMeshWall(XMFLOAT3(0.0f, 0.0f, MAP_DOWN), XMFLOAT3(0.0f,  XM_PI, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 16, 1, 80.0f, 40.0f);

	// ��(�����p�̔�����)
	InitMeshWall(XMFLOAT3(0.0f, 0.0f, MAP_TOP), XMFLOAT3(0.0f,    XM_PI, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 0.25f), 16, 1, 80.0f, 40.0f);
	InitMeshWall(XMFLOAT3(MAP_LEFT, 0.0f, 0.0f), XMFLOAT3(0.0f,   XM_PI * 0.50f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 0.25f), 16, 1, 80.0f, 40.0f);
	InitMeshWall(XMFLOAT3(MAP_RIGHT, 0.0f, 0.0f), XMFLOAT3(0.0f, -XM_PI * 0.50f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 0.25f), 16, 1, 80.0f, 40.0f);
	InitMeshWall(XMFLOAT3(0.0f, 0.0f, MAP_DOWN), XMFLOAT3(0.0f, 0.0f, 0.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 0.25f), 16, 1, 80.0f, 40.0f);


	// �I�u�W�F�N�g�𐶐�
	InitObj();

	// �w�i�𐶐�
	InitBG();

	// �G�C�����f���𐶂₷
	InitAim();

	// �e�̏�����
	InitBullet();

	// �`���[�W�e�̏�����
	InitBullet_Charge();

	// �X�R�A�̏�����
	InitScore();

	// �|�[�Y�̏�����
	InitPause();

	// UI�̏�����
	InitUi();

	// �G�l�~�[UI�̏�����
	InitUi_Enemy();

	// �C�x���g�̏�����
	InitEvent();

	// �p�[�e�B�N���̏�����
	InitParticle();

	// �p�[�e�B�N���X�p�[�N�̏�����
	InitParticleSpark();

	// �p�[�e�B�N���`���[�W�̏�����
	InitParticleCharge();

	// �t���O�̏�����
	InitFlag();

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitGame(void)
{
	// �p�[�e�B�N���̏I������
	UninitParticle();

	// �p�[�e�B�N���X�p�[�N�̏I������
	UninitParticleSpark();

	// �p�[�e�B�N���`���[�W�̏I������
	UninitParticleCharge();

	// �X�R�A�̏I������
	UninitScore();

	// UI�̏I������
	UninitUi();

	// �G�l�~�[UI�̏I������
	UninitUi_Enemy();

	// �e�̏I������
	UninitBullet();

	// �`���[�W�e�̏I������
	UninitBullet_Charge();

	// �I�u�W�F�N�g�̏I������
	UninitObj();

	// �w�i�̏I������
	UninitBG();

	// �G�C�����f���̏I������
	UninitAim();

	// �ǂ̏I������
	UninitMeshWall();

	// �n�ʂ̏I������
	UninitMeshField();

	// �{�X�̏I������
	UninitBoss();

	// �G�l�~�[�̏I������
	UninitEnemy();

	// �v���C���[�̏I������
	UninitPlayer();

	// �e�̏I������
	UninitShadow();

	// �C�x���g�̏I������
	UninitEvent();

	// �|�[�Y�̏I������
	UninitPause();

	UninitFlag();

}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateGame(void)
{
#ifdef _DEBUG
	if (GetKeyboardTrigger(DIK_V))
	{
		g_ViewPortType_Game = (g_ViewPortType_Game + 1) % TYPE_NONE;
		SetViewPort(g_ViewPortType_Game);
	}

	PrintDebugProc("GameFlag:%d\n", GetFlag());


#endif




	if (g_Game.bPause == TRUE)
	{
		UpdatePause();
	}
	else
	{
		ResetPause();

		// �J�����X�V(���}���u)
		UpdateCamera();

		// �n�ʏ����̍X�V
		UpdateMeshField();

		// �C�x���g���͎~�߂�
		if (GetFlag() != FLAG_GAME_EVENT_PILLAR && GetFlag() != FLAG_GAME_EVENT_FIRE)
		{

			// �G�l�~�[�̍X�V����
			UpdateEnemy();

			// �v���C���[�̍X�V����
			UpdatePlayer();
		}


		// �{�X�̍X�V����
		UpdateBoss();

		// �Ǐ����̍X�V
		UpdateMeshWall();

		// �G�C�����f���̍X�V����
		UpdateAim();


		// �I�u�W�F�N�g�̍X�V����
		UpdateObj();

		// �w�i�̍X�V����
		UpdateBG();

		// �e�̍X�V����
		UpdateBullet();

		// �`���[�W�e�̍X�V����
		UpdateBullet_Charge();

		// �p�[�e�B�N���̍X�V����
		UpdateParticle();

		// �p�[�e�B�N���X�p�[�N�̍X�V����
		UpdateParticleSpark();

		// �p�[�e�B�N���`���[�W�̍X�V����
		UpdateParticleCharge();

		// �e�̍X�V����
		UpdateShadow();

		// �����蔻�菈��
		CheckHit();

		// �X�R�A�̍X�V����
		UpdateScore();

		// UI�̍X�V����
		UpdateUi();

		// �G�l�~�[UI�̍X�V����
		UpdateUi_Enemy();

		// �C�x���g�̍X�V����
		UpdateEvent();

	}

	if (GetKeyboardTrigger(DIK_P) || IsButtonTriggered(0, BUTTON_START))
	{
		PlaySound(SOUND_LABEL_SE_CRICK);
		g_Game.bPause = g_Game.bPause ? FALSE : TRUE;
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawGame0(void)
{
	// 3D�̕���`�悷�鏈��
	// �n�ʂ̕`�揈��
	DrawMeshField();

	// �e�̕`�揈��
	DrawShadow();

	// �{�X�̕`�揈��
	DrawBoss();

	// �G�l�~�[�̕`�揈��
	DrawEnemy();

	// �v���C���[�̕`�揈��
	DrawPlayer();

	// �e�̕`�揈��
	DrawBullet();

	// �`���[�W�e�̕`�揈��
	DrawBullet_Charge();

	// �ǂ̕`�揈��
	DrawMeshWall();

	// �I�u�W�F�N�g�̕`�揈��
	DrawObj();

	// �w�i�̕`�揈��
	DrawBG();

	// �p�[�e�B�N���̕`�揈��
	DrawParticle();

	// �p�[�e�B�N���X�p�[�N�̕`�揈��
	DrawParticleSpark();

	// �p�[�e�B�N���`���[�W�̕`�揈��
	DrawParticleCharge();

	// �C�x���g�̕`�揈��
	DrawEvent();

	// �G�l�~�[UI�̕`�揈��
	DrawUi_Enemy();


	// 2D�̕���`�悷�鏈��
	// Z��r�Ȃ�
	SetDepthEnable(FALSE);

	// ���C�e�B���O�𖳌�
	SetLightEnable(FALSE);

	// UI�̕`�揈��
	DrawUi();

	// �G�C�����f���̕`�揈��
	DrawAim();

	// �o�g�����ɕ`��
	if (GetFlag() == FLAG_GAME_BATTLE)
	{
		// �X�R�A�̕`�揈��
		DrawScore();
	}


	if (g_Game.bPause == TRUE)
	{
		// �|�[�Y�̕`�揈��
		DrawPause();
	}

	// ���C�e�B���O��L����
	SetLightEnable(TRUE);

	// Z��r����
	SetDepthEnable(TRUE);

}


void DrawGame(void)
{
	XMFLOAT3 pos;


#ifdef _DEBUG
	// �f�o�b�O�\��
	PrintDebugProc("ViewPortType:%d\n", g_ViewPortType_Game);

#endif

	// �v���C���[���_
	pos = GetPlayer()->pos;
	pos.y = 0.0f;			// �J����������h�����߂ɃN���A���Ă���


	if (GetFlag() != FLAG_GAME_EVENT_FIRE && GetFlag() != FLAG_GAME_EVENT_PILLAR)
	{
		SetCameraAT(pos);
	}

	SetCamera();


	switch(g_ViewPortType_Game)
	{
	case TYPE_FULL_SCREEN:
		SetViewPort(TYPE_FULL_SCREEN);
		DrawGame0();
		break;

	case TYPE_LEFT_HALF_SCREEN:
	case TYPE_RIGHT_HALF_SCREEN:
		SetViewPort(TYPE_LEFT_HALF_SCREEN);
		DrawGame0();

		// �G�l�~�[���_
		pos = GetEnemy()->pos;
		pos.y = 0.0f;
		SetCameraAT(pos);
		SetCamera();
		SetViewPort(TYPE_RIGHT_HALF_SCREEN);
		DrawGame0();
		break;

	case TYPE_UP_HALF_SCREEN:
	case TYPE_DOWN_HALF_SCREEN:
		SetViewPort(TYPE_UP_HALF_SCREEN);
		DrawGame0();

		// �G�l�~�[���_
		pos = GetEnemy()->pos;
		pos.y = 0.0f;
		SetCameraAT(pos);
		SetCamera();
		SetViewPort(TYPE_DOWN_HALF_SCREEN);
		DrawGame0();
		break;

	}

}


//=============================================================================
// �����蔻�菈��
//=============================================================================
void CheckHit(void)
{
	ENEMY *enemy = GetEnemy();							// �G�l�~�[�̃|�C���^�[��������
	PLAYER *player = GetPlayer();						// �v���C���[�̃|�C���^�[��������
	BULLET *bullet = GetBullet();						// �e�̃|�C���^�[��������
	BULLET_CHARGE *bullet_charge = GetBullet_Charge();	// �e�̃|�C���^�[��������
	EVENT* event = GetEvent();							// �t���O�̃|�C���^�[��������
	PARTICLE* particle = GetParticle();					// �p�[�e�B�N���̃|�C���^�[��������


	XMFLOAT3 PlayerLowerPos = XMFLOAT3(player->pos.x, player->pos.y - 3.0f, player->pos.z);
	XMFLOAT3 PlayerUpperPos = XMFLOAT3(player->pos.x, player->pos.y + 3.0f, player->pos.z);

	// �G�ƃv���C���[�L����
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		//�G�̗L���t���O���`�F�b�N����
		if (enemy[i].use == FALSE || enemy[i].EntryTime || enemy[i].LeaveTime)
			continue;


		//�J�v�Z���̓����蔻��
		if(CapsuleCollision(PlayerUpperPos, PlayerLowerPos, enemy[i].pos, player->size, enemy[i].size))
			{
			// �G�L�����N�^�[�͓|�����
			enemy[i].use = FALSE;
			ReleaseShadow(enemy[i].shadowIdx);
			player->hp -= enemy[i].damage;
			PlaySound(SOUND_LABEL_SE_DAMAGE);

			// �e�������������Ƀp�[�e�B�N������
			switch (player->type)
			{
			case PLAYER_RED:
				SetParticle(player->pos, XMFLOAT3(2.0f, 2.0f, 2.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f),
					TYPE_CHANGE_PLAYER, BLEND_MODE_SUBTRACT, 0.0f, 0.0f, 90);
				break;

			case PLAYER_BLUE:
				SetParticle(player->pos, XMFLOAT3(2.0f, 2.0f, 2.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.9f, 0.5f, 0.0f, 1.0f),
					TYPE_CHANGE_PLAYER, BLEND_MODE_SUBTRACT, 0.0f, 0.0f, 90);
				break;

			case PLAYER_GREEN:
				SetParticle(player->pos, XMFLOAT3(2.0f, 2.0f, 2.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.9f, 0.0f, 0.9f, 1.0f),
					TYPE_CHANGE_PLAYER, BLEND_MODE_SUBTRACT, 0.0f, 0.0f, 90);
				break;

			case PLAYER_YELLOW:
				SetParticle(player->pos, XMFLOAT3(2.0f, 2.0f, 2.0f), XMFLOAT3(0.0f, 5.0f, 0.0f), XMFLOAT4(0.0f, 0.0f, 0.9f, 1.0f),
					TYPE_CHANGE_PLAYER, BLEND_MODE_SUBTRACT, 0.0f, 0.0f, 90);
				break;
			}

		}

	}

	// ���֘A
	for (int i = 0; i < MAX_BULLET; i++)
	{
		//�e�̗L���t���O���`�F�b�N����
		if (bullet[i].bUse == FALSE)
			continue;

		for (int j = 0; j < 1; j++)
		{
			if (event->use == FALSE) continue;
			if (GetFlag() >= FLAG_GAME_EVENT_FIRE) continue;

			// BC�̓����蔻��
			if (CollisionBC(bullet[i].pos, event->pos, bullet[i].size, event->size))
			{
				// �ΉԂ��U�炷
				for (int p = 0; p < 20; p++)
				{
					XMFLOAT3 move = CalcParticleSparkMove(event->pos);

					SetParticleSpark(bullet[i].pos, move, 30, TRUE);
				}

				PlaySound(SOUND_LABEL_SE_FIRE);

				// �t���O�̐؂�ւ�
				SetFlag(FLAG_GAME_EVENT_FIRE);
			}
		}

		// �G�Ɠ������Ă邩���ׂ�
		for (int j = 0; j < MAX_ENEMY; j++)
		{
			//�G�̗L���t���O���`�F�b�N����
			if (enemy[j].use == FALSE || enemy[j].EntryTime || enemy[j].LeaveTime)
				continue;

			XMFLOAT3 EnemyLowerPos = XMFLOAT3(enemy[j].pos.x, enemy[j].pos.y - 4.0f, enemy[j].pos.z);
			XMFLOAT3 EnemyUpperPos = XMFLOAT3(enemy[j].pos.x, enemy[j].pos.y + 4.0f, enemy[j].pos.z);

			//�J�v�Z���̓����蔻��
			if (CapsuleCollision(EnemyUpperPos, EnemyLowerPos, bullet[i].pos, enemy[j].size, bullet[i].size))
			{
				// �o���b�g������
				ReleaseBullet(i);

				enemy[j].hp -= player->damage;

				// �e�������������Ƀp�[�e�B�N������
				switch (enemy[j].type)
				{
				case ENEMY_RED:
					SetParticle(enemy[j].pos, XMFLOAT3(2.0f, 2.0f, 2.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f),
						TYPE_CHANGE_PLAYER, BLEND_MODE_SUBTRACT, 0.0f, 0.0f, 90);

					break;

				case ENEMY_BLUE:
					SetParticle(enemy[j].pos, XMFLOAT3(2.0f, 2.0f, 2.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.9f, 0.5f, 0.0f, 1.0f),
						TYPE_CHANGE_PLAYER, BLEND_MODE_SUBTRACT, 0.0f, 0.0f, 90);

					break;

				case ENEMY_GREEN:
					SetParticle(enemy[j].pos, XMFLOAT3(2.0f, 2.0f, 2.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.9f, 0.0f, 0.9f, 1.0f),
						TYPE_CHANGE_PLAYER, BLEND_MODE_SUBTRACT, 0.0f, 0.0f, 90);

					break;

				case ENEMY_YELLOW:
					SetParticle(enemy[j].pos, XMFLOAT3(2.0f, 2.0f, 2.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.0f, 0.0f, 0.9f, 1.0f),
						TYPE_CHANGE_PLAYER, BLEND_MODE_SUBTRACT, 0.0f, 0.0f, 90);

					break;
				}

				switch (bullet->type)
				{
				case BULLET_RED:
					enemy[j].state = ENEMY_ABNORMAL_BURN;
					break;

				case BULLET_BLUE:
					enemy[j].state = ENEMY_ABNORMAL_WET;
					break;

				case BULLET_GREEN:
					enemy[j].state = ENEMY_ABNORMAL_WINDY;
					break;

				case BULLET_YELLOW:
					enemy[j].state = ENEMY_ABNORMAL_SHOCK;
					break;

				}


				if (enemy[j].hp <= 0)
				{
					// dissolve�J�n
					enemy[j].LeaveTime = TRUE;
					ReleaseShadow(enemy[j].shadowIdx);

					// �X�R�A
					switch (enemy[j].type)
					{
					case ENEMY_RED:
						AddScore(10);
						break;

					case ENEMY_BLUE:
						AddScore(20);
						break;

					case ENEMY_GREEN:
						AddScore(30);
						break;

					case ENEMY_YELLOW:
						AddScore(40);
						break;
					}

				}
			}
		}

	}

	// �v���C���[�̃`���[�W�e�ƓG
	for (int i = 0; i < MAX_BULLET_CHARGE; i++)
	{
		//�e�̗L���t���O���`�F�b�N����
		if (bullet_charge[i].bUse == FALSE)
			continue;

		for (int j = 0; j < 1; j++)
		{
			if (event->use == FALSE) continue;
			if (GetFlag() >= FLAG_GAME_EVENT_FIRE) continue;

			// BC�̓����蔻��
			if (CollisionBC(bullet_charge[i].pos, event->pos, bullet_charge[i].size, event->size))
			{
				// �ΉԂ��U�炷
				for (int p = 0; p < 20; p++)
				{
					XMFLOAT3 move = CalcParticleSparkMove(event->pos);

					SetParticleSpark(bullet_charge[i].pos, move, 30, TRUE);
				}

				PlaySound(SOUND_LABEL_SE_FIRE);

				// �t���O�̐؂�ւ�
				SetFlag(FLAG_GAME_EVENT_FIRE);
			}
		}

		// �G�Ɠ������Ă邩���ׂ�
		for (int j = 0; j < MAX_ENEMY; j++)
		{
			//�G�̗L���t���O���`�F�b�N����
			if (enemy[j].use == FALSE || enemy[j].EntryTime || enemy[j].LeaveTime)
				continue;

			XMFLOAT3 EnemyLowerPos = XMFLOAT3(enemy[j].pos.x, enemy[j].pos.y - 4.0f, enemy[j].pos.z);
			XMFLOAT3 EnemyUpperPos = XMFLOAT3(enemy[j].pos.x, enemy[j].pos.y + 4.0f, enemy[j].pos.z);

			//�J�v�Z���̓����蔻��
			if (CapsuleCollision(EnemyUpperPos, EnemyLowerPos, bullet_charge[i].pos, enemy[j].size, bullet_charge[i].size))
			{
				// �o���b�g������
				ReleaseBullet(i);

				enemy[j].hp -= (player->damage * 2);

				// �e�������������Ƀp�[�e�B�N������
				switch (enemy[j].type)
				{
				case ENEMY_RED:
					SetParticle(enemy[j].pos, XMFLOAT3(2.0f, 2.0f, 2.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f),
						TYPE_CHANGE_PLAYER, BLEND_MODE_SUBTRACT, 0.0f, 0.0f, 90);

					break;

				case ENEMY_BLUE:
					SetParticle(enemy[j].pos, XMFLOAT3(2.0f, 2.0f, 2.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.9f, 0.5f, 0.0f, 1.0f),
						TYPE_CHANGE_PLAYER, BLEND_MODE_SUBTRACT, 0.0f, 0.0f, 90);

					break;

				case ENEMY_GREEN:
					SetParticle(enemy[j].pos, XMFLOAT3(2.0f, 2.0f, 2.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.9f, 0.0f, 0.9f, 1.0f),
						TYPE_CHANGE_PLAYER, BLEND_MODE_SUBTRACT, 0.0f, 0.0f, 90);

					break;

				case ENEMY_YELLOW:
					SetParticle(enemy[j].pos, XMFLOAT3(2.0f, 2.0f, 2.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.0f, 0.0f, 0.9f, 1.0f),
						TYPE_CHANGE_PLAYER, BLEND_MODE_SUBTRACT, 0.0f, 0.0f, 90);

					break;
				}

				switch (bullet_charge->type)
				{
				case BULLET_RED:
					enemy[j].state = ENEMY_ABNORMAL_BURN;
					break;

				case BULLET_BLUE:
					enemy[j].state = ENEMY_ABNORMAL_WET;
					break;

				case BULLET_GREEN:
					enemy[j].state = ENEMY_ABNORMAL_WINDY;
					break;

				case BULLET_YELLOW:
					enemy[j].state = ENEMY_ABNORMAL_SHOCK;
					break;

				}


				if (enemy[j].hp <= 0)
				{
					// dissolve�J�n
					enemy[j].LeaveTime = TRUE;
					ReleaseShadow(enemy[j].shadowIdx);

					// �X�R�A
					switch (enemy[j].type)
					{
					case ENEMY_RED:
						AddScore(10);
						break;

					case ENEMY_BLUE:
						AddScore(20);
						break;

					case ENEMY_GREEN:
						AddScore(30);
						break;

					case ENEMY_YELLOW:

						AddScore(40);
						break;
					}

				}
			}
		}

	}
	// �p�[�e�B�N���ƓG
	for (int i = 0; i < MAX_PARTICLE; i++)
	{
		//�e�̗L���t���O���`�F�b�N����
		if (particle[i].use == FALSE) continue;
		if (particle[i].type != TYPE_HINOKO_CHARGE_YELLOW) continue;


		for (int j = 0; j < 1; j++)
		{
			if (event->use == FALSE) continue;
			if (GetFlag() >= FLAG_GAME_EVENT_FIRE) continue;

			// BC�̓����蔻��
			if (CollisionBC(particle[i].pos, event->pos, particle[i].fSizeX, event->size))
			{
				// �ΉԂ��U�炷
				for (int p = 0; p < 20; p++)
				{
					XMFLOAT3 move = CalcParticleSparkMove(event->pos);

					SetParticleSpark(particle[i].pos, move, 30, TRUE);
				}

				PlaySound(SOUND_LABEL_SE_FIRE);

				// �t���O�̐؂�ւ�
				SetFlag(FLAG_GAME_EVENT_FIRE);
			}

		}

		// �G�Ɠ������Ă邩���ׂ�
		for (int j = 0; j < MAX_ENEMY; j++)
		{
			//�G�̗L���t���O���`�F�b�N����
			if (enemy[j].use == FALSE || enemy[j].EntryTime || enemy[j].LeaveTime)
				continue;

			XMFLOAT3 EnemyLowerPos = XMFLOAT3(enemy[j].pos.x, enemy[j].pos.y - 5.0f, enemy[j].pos.z);
			XMFLOAT3 EnemyUpperPos = XMFLOAT3(enemy[j].pos.x, enemy[j].pos.y + 5.0f, enemy[j].pos.z);

			//�J�v�Z���̓����蔻��
			if (CapsuleCollision(EnemyUpperPos, EnemyLowerPos, particle[i].pos, enemy[j].size, particle[i].fSizeX))
			{
				// �o���b�g������
				ReleaseBullet(i);

				enemy[j].hp -= (player->damage * 2);

				// �e�������������Ƀp�[�e�B�N������
				switch (enemy[j].type)
				{
				case ENEMY_RED:
					SetParticle(enemy[j].pos, XMFLOAT3(2.0f, 2.0f, 2.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f),
						TYPE_CHANGE_PLAYER, BLEND_MODE_SUBTRACT, 0.0f, 0.0f, 90);

					break;

				case ENEMY_BLUE:
					SetParticle(enemy[j].pos, XMFLOAT3(2.0f, 2.0f, 2.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.9f, 0.5f, 0.0f, 1.0f),
						TYPE_CHANGE_PLAYER, BLEND_MODE_SUBTRACT, 0.0f, 0.0f, 90);

					break;

				case ENEMY_GREEN:
					SetParticle(enemy[j].pos, XMFLOAT3(2.0f, 2.0f, 2.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.9f, 0.0f, 0.9f, 1.0f),
						TYPE_CHANGE_PLAYER, BLEND_MODE_SUBTRACT, 0.0f, 0.0f, 90);

					break;

				case ENEMY_YELLOW:
					SetParticle(enemy[j].pos, XMFLOAT3(2.0f, 2.0f, 2.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.0f, 0.0f, 0.9f, 1.0f),
						TYPE_CHANGE_PLAYER, BLEND_MODE_SUBTRACT, 0.0f, 0.0f, 90);

					break;
				}

				// ��Ԉُ�ɂ���
				/*switch (bullet_charge->type)
				{
				case BULLET_RED:
					enemy[j].state = ENEMY_ABNORMAL_BURN;
					break;

				case BULLET_BLUE:
					enemy[j].state = ENEMY_ABNORMAL_WET;
					break;

				case BULLET_GREEN:
					enemy[j].state = ENEMY_ABNORMAL_WINDY;
					break;

				case BULLET_YELLOW:
					enemy[j].state = ENEMY_ABNORMAL_SHOCK;
					break;

				}*/

				if (enemy[j].hp <= 0)
				{
					// dissolve�J�n
					enemy[j].LeaveTime = TRUE;
					ReleaseShadow(enemy[j].shadowIdx);

					// �X�R�A
					switch (enemy[j].type)
					{
					case ENEMY_RED:
						AddScore(10);
						break;

					case ENEMY_BLUE:
						AddScore(20);
						break;

					case ENEMY_GREEN:
						AddScore(30);
						break;

					case ENEMY_YELLOW:
						AddScore(40);
						break;
					}

				}
			}
		}

	}

	// �Q�[���I�[�o�[����
	if (player->state == STATE_DIE)
	{
		// ���m�N��
		SetMonochrome(TRUE);

		g_MonokuroCnt++;

		if (g_MonokuroCnt == MONOKURO_END)
		{
			g_MonokuroCnt = 0;
			SetFade(FADE_OUT, MODE_RESULT);
		}
	}
}


//=============================================================================
// �v���C���[�����擾
//=============================================================================
GAME* GetGame(void)
{
	return &g_Game;
}
