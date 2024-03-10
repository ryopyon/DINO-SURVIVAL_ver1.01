//=============================================================================
//
// �|�[�Y���� [pause.cpp]
// Author : HAL���� ������
//
//=============================================================================
#include "main.h"
#include "pause.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "sprite.h"
#include "game.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_WIDTH				(SCREEN_WIDTH)	// �w�i�T�C�Y
#define TEXTURE_HEIGHT				(SCREEN_HEIGHT)	// 

#define TEXTURE_MAX					(6)


#define PAUSE_CUR1_X				(760.0f)		//	�J�[�\���̈ʒu
#define PAUSE_CUR1_Y				(290.0f)

#define PAUSE_CUR2_X				(760.0f)
#define PAUSE_CUR2_Y				(350.0f)

#define PAUSE_CUR3_X				(760.0f)		
#define PAUSE_CUR3_Y				(405.0f)

#define PAUSE_CUR4_X				(760.0f)
#define PAUSE_CUR4_Y				(460.0f)

#define CUR_WIDTH					(50)			//�J�[�\���T�C�Y
#define CUR_HEIGHT					(50)


#define ENEMY_WAIT					(4)				// �A�j���[�V�����̑҂��t���[����

#define TEXTURE_PATTERN_DIVIDE_X	(7)				// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define TEXTURE_PATTERN_DIVIDE_Y	(1)				// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define ANIM_PATTERN_NUM			(TEXTURE_PATTERN_DIVIDE_X*TEXTURE_PATTERN_DIVIDE_Y)	// �A�j���[�V�����p�^�[����

enum
{
	PAUSE_CUR_EYELLOW,
	PAUSE_CUR_EGREEN,
	PAUSE_CUR_EBLUE,
	PAUSE_CUR_ERED,

	PAUSE_CUR_MAX,
};

enum 
{
	BG = 4,
	BG_MENU,
};

enum 
{
	PAUSE_CONTINUE,
	PAUSE_RETRY,
	PAUSE_TITLE,
	PAUSE_EXIT,
};



//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************


static ID3D11Buffer* g_VertexBuffer = NULL;				// ���_���
static ID3D11ShaderResourceView* g_Texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����

static char* g_TexturName[TEXTURE_MAX] = {
	"data/TEXTURE/cursor/Eyellow.png",
	"data/TEXTURE/cursor/Egreen.png",
	"data/TEXTURE/cursor/Eblue.png",
	"data/TEXTURE/cursor/Ered.png",
	"data/TEXTURE/fade_black.png",
	"data/TEXTURE/pause/pause.png",
};

static BOOL						g_Use;						// TRUE:�g���Ă���  FALSE:���g�p
static float					g_w, g_h;					// ���ƍ���
static XMFLOAT3					g_Pos;						// �|���S���̍��W
static int						g_TexNo;					// �e�N�X�`���ԍ�

static BOOL						g_ResetPause;				// ���Z�b�g���Ă邩�ǂ����̔���

static int cursor;
static int Epattern;
static float EanimCnt;


static BOOL						g_Load = FALSE;


//=============================================================================
// ����������
//=============================================================================
HRESULT InitPause(void)
{
	ID3D11Device *pDevice = GetDevice();

	//�e�N�X�`������
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		g_Texture[i] = NULL;
		D3DX11CreateShaderResourceViewFromFile(GetDevice(),
			g_TexturName[i],
			NULL,
			NULL,
			&g_Texture[i],
			NULL);
	}


	// ���_�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);

	// �ϐ��̏�����
	g_Use = TRUE;
	g_w = TEXTURE_WIDTH;
	g_h = TEXTURE_HEIGHT;
	g_Pos = { g_w / 2, 50.0f, 0.0f };
	g_TexNo = 0;
	cursor = 0;

	g_ResetPause = FALSE;

	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitPause(void)
{
	if (g_Load == FALSE) return;

	if (g_VertexBuffer)
	{
		g_VertexBuffer->Release();
		g_VertexBuffer = NULL;
	}

	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		if (g_Texture[i])
		{
			g_Texture[i]->Release();
			g_Texture[i] = NULL;
		}
	}

	g_Load = FALSE;
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdatePause(void)
{
	GAME* game = GetGame();

	//�|�[�Y���̎��Ƀ|�[�Y��ʏ���
	if (game->bPause == TRUE)
	{
		// �A�j���[�V����  
		EanimCnt += 1.0f;
		if (EanimCnt > ENEMY_WAIT)
		{
			EanimCnt = 0.0f;
			// �p�^�[���̐؂�ւ�
			Epattern = (Epattern + 1) % ANIM_PATTERN_NUM;
		}

		// �J�[�\���̐؂�ւ�
		if (GetKeyboardTrigger(DIK_DOWN) || GetKeyboardTrigger(DIK_S) || IsButtonTriggered(0, STICK_L_DOWN) || IsButtonTriggered(0, BUTTON_DOWN))
		{
			PlaySound(SOUND_LABEL_SE_CRICK);
			cursor = (++cursor + PAUSE_CUR_MAX) % PAUSE_CUR_MAX;
		}
		if (GetKeyboardTrigger(DIK_UP) || GetKeyboardTrigger(DIK_W) || IsButtonTriggered(0, STICK_L_UP) || IsButtonTriggered(0, BUTTON_UP))
		{
			PlaySound(SOUND_LABEL_SE_CRICK);
			cursor = (--cursor + PAUSE_CUR_MAX) % PAUSE_CUR_MAX;
		}



		switch (cursor)		//�J�[�\���̕\�L
		{
		case PAUSE_CONTINUE:
			g_TexNo = PAUSE_CUR_EGREEN;
			g_Pos = XMFLOAT3(PAUSE_CUR1_X, PAUSE_CUR1_Y, 0.0f);

			if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(0, BUTTON_START) || IsButtonTriggered(0, BUTTON_B))
			{// �X�e�[�W�؂�ւ�
				PlaySound(SOUND_LABEL_SE_KETTEI);
				game->bPause = FALSE;
			}
			break;

		case PAUSE_RETRY:
			g_TexNo = PAUSE_CUR_EYELLOW;
			g_Pos = XMFLOAT3(PAUSE_CUR2_X, PAUSE_CUR2_Y, 0.0f);

			if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(0, BUTTON_START) || IsButtonTriggered(0, BUTTON_B))
			{// �X�e�[�W�؂�ւ�
				PlaySound(SOUND_LABEL_SE_KETTEI);
				SetFade(FADE_OUT, MODE_GAME);
				game->bPause = FALSE;
			}
			break;

		case PAUSE_TITLE:
			g_TexNo = PAUSE_CUR_EBLUE;
			g_Pos = XMFLOAT3(PAUSE_CUR3_X, PAUSE_CUR3_Y, 0.0f);

			if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(0, BUTTON_START) || IsButtonTriggered(0, BUTTON_B))
			{// �X�e�[�W�؂�ւ�
				PlaySound(SOUND_LABEL_SE_KETTEI);
				SetFade(FADE_OUT, MODE_TITLE);
			}
			break;

		case PAUSE_EXIT:
			g_TexNo = PAUSE_CUR_ERED;
			g_Pos = XMFLOAT3(PAUSE_CUR4_X, PAUSE_CUR4_Y, 0.0f);

			if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(0, BUTTON_START) || IsButtonTriggered(0, BUTTON_B))
			{
				GameLoop();
			}
			break;

		}
	}

	g_ResetPause = FALSE;

#ifdef _DEBUG	// �f�o�b�O����\������
	
#endif

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawPause(void)
{
	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// �}�g���N�X�ݒ�
	SetWorldViewProjection2D();

	// �v���~�e�B�u�g�|���W�ݒ�
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// �}�e���A���ݒ�
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);



	// �������̍����w�i��`��
	{
		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[BG]);
		
		//�\���ʒu
		float pw = SCREEN_WIDTH;
		float ph = SCREEN_HEIGHT;
		float px = SCREEN_WIDTH / 2;
		float py = SCREEN_HEIGHT / 2;

		float tw = 1.0f;				// �e�N�X�`���̕�
		float th = 1.0f;				// �e�N�X�`���̍���
		float tx = 0.0f;				// �e�N�X�`���̍���X���W
		float ty = 0.0f;				// �e�N�X�`���̍���Y���W

		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		SetSpriteColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
			XMFLOAT4(1.0f, 1.0f, 1.0f, 0.5f));

		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);
	}

	// �|�[�Y���j���[��`��
	{
		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[BG_MENU]);

		//�\���ʒu
		float pw = SCREEN_WIDTH;
		float ph = SCREEN_HEIGHT;
		float px = SCREEN_WIDTH / 2;
		float py = SCREEN_HEIGHT / 2;

		float tw = 1.0f;				// �e�N�X�`���̕�
		float th = 1.0f;				// �e�N�X�`���̍���
		float tx = 0.0f;				// �e�N�X�`���̍���X���W
		float ty = 0.0f;				// �e�N�X�`���̍���Y���W

		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		SetSpriteColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);

	}

	//�J�[�\���̕`��
	{
		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_TexNo]);

		//�A�j���[�V�����p
		float tw = 1.0f / TEXTURE_PATTERN_DIVIDE_X;	// �e�N�X�`���̕�
		float th = 1.0f / TEXTURE_PATTERN_DIVIDE_Y;	// �e�N�X�`���̍���
		float tx = (float)(Epattern % TEXTURE_PATTERN_DIVIDE_X) * tw;	// �e�N�X�`���̍���X���W
		float ty = (float)(Epattern / TEXTURE_PATTERN_DIVIDE_X) * th;	// �e�N�X�`���̍���Y���W

		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		SetSpriteLeftTop(g_VertexBuffer, g_Pos.x, g_Pos.y, CUR_WIDTH, CUR_HEIGHT, tx, ty, tw, th);

		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);
	}

}

void ResetPause(void)
{
	if (g_ResetPause == TRUE)  return;
	g_Use = TRUE;
	g_w = TEXTURE_WIDTH;
	g_h = TEXTURE_HEIGHT;
	g_Pos = { g_w / 2, 50.0f, 0.0f };
	g_TexNo = 0;
	cursor = 0;

	g_ResetPause = TRUE;

}

