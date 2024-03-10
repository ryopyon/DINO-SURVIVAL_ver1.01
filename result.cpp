//=============================================================================
//
// ���U���g��ʏ��� [result.cpp]
// Author : HAL���� ������
//
//=============================================================================
#include "result.h"
#include "input.h"
#include "score.h"
#include "fade.h"
#include "sound.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_WIDTH				(SCREEN_WIDTH)	// �w�i�T�C�Y
#define TEXTURE_HEIGHT				(SCREEN_HEIGHT)	// 
#define TEXTURE_MAX					(5)				// �e�N�X�`���̐�


#define CUR_WIDTH					(50)			//�J�[�\���T�C�Y
#define CUR_HEIGHT					(50)


#define RESULT_CUR1_X				(700.0f)		//	�J�[�\���̈ʒu
#define RESULT_CUR1_Y				(340.0f)

#define RESULT_CUR2_X				(850.0f)
#define RESULT_CUR2_Y				(390.0f)

#define RESULT_CUR3_X				(700.0f)
#define RESULT_CUR3_Y				(440.0f)


// �A�j���[�V�����p
#define ANIM_WAIT_TIME				(90)
#define ANIM_FADE_TIME				(90)
#define ANIM_TOTAL_TIME				(ANIM_WAIT_TIME + ANIM_FADE_TIME)

#define ENEMY_WAIT					(4)

#define TEXTURE_PATTERN_DIVIDE_X	(7)		// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define TEXTURE_PATTERN_DIVIDE_Y	(1)		// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define ANIM_PATTERN_NUM			(TEXTURE_PATTERN_DIVIDE_X*TEXTURE_PATTERN_DIVIDE_Y)	// �A�j���[�V�����p�^�[����

enum
{
	RESULT_RESULT,
	RESULT_NUMBER,
	RESULT_EGREEN,
	RESULT_EBLUE,
	RESULT_ERED,

	RESULT_COLOR_MAX,
};

enum
{
	RESULT_TITLE,
	RESULT_RETRY,
	RESULT_EXIT,

	RESULT_MAX,
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;		// ���_���
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����

static char *g_TexturName[TEXTURE_MAX] = {
	"data/TEXTURE/result/Result01.png",
	"data/TEXTURE/number16x32.png",
	"data/TEXTURE/cursor/Egreen.png",
	"data/TEXTURE/cursor/Eblue.png",
	"data/TEXTURE/cursor/Ered.png",
};


static BOOL						g_Use;						// TRUE:�g���Ă���  FALSE:���g�p
static float					g_w, g_h;					// ���ƍ���
static XMFLOAT3					g_Pos;						// �|���S���̍��W
static int						g_TexNo;					// �e�N�X�`���ԍ�

static float EanimCnt;
static int Epattern;

static int cursor;

// ���C�v
static BOOL						g_wipe = FALSE;
static float					g_wipeSize = 0.0f;

static BOOL						g_Load = FALSE;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitResult(void)
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
	g_Use   = TRUE;
	g_w     = TEXTURE_WIDTH;
	g_h     = TEXTURE_HEIGHT;
	g_Pos   = { g_w / 2, 50.0f, 0.0f };
	g_TexNo = 0;


	// ���m�N���𒼂�
	SetMonochrome(FALSE);

	// ���C�v��������
	SetWipe(WIPE_MODE_NONE, XMFLOAT4(1.0f, 1.0f, 0.0f, 0.0f), g_wipeSize);

	// BGM�Đ�
	PlaySound(SOUND_LABEL_BGM_RESULT);

	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitResult(void)
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
void UpdateResult(void)
{
	// �A�j���[�V����  
	EanimCnt += 1.0f;
	if (EanimCnt > ENEMY_WAIT)
	{
		EanimCnt = 0.0f;
		// �p�^�[���̐؂�ւ�
		Epattern = (Epattern + 1) % ANIM_PATTERN_NUM;
	}



	if (GetKeyboardTrigger(DIK_DOWN) || GetKeyboardTrigger(DIK_S) || IsButtonTriggered(0, STICK_L_DOWN) || IsButtonTriggered(0, BUTTON_DOWN))
	{
		PlaySound(SOUND_LABEL_SE_CRICK);
		cursor = (++cursor + RESULT_MAX) % RESULT_MAX;
	}
	if (GetKeyboardTrigger(DIK_UP) || GetKeyboardTrigger(DIK_W) || IsButtonTriggered(0, STICK_L_UP) || IsButtonTriggered(0, BUTTON_UP))
	{
		PlaySound(SOUND_LABEL_SE_CRICK);
		cursor = (--cursor + RESULT_MAX) % RESULT_MAX;
	}


	switch (cursor)		//�J�[�\���̕\�L
	{
	case RESULT_TITLE:
		g_TexNo = RESULT_EBLUE;
		g_Pos = XMFLOAT3(RESULT_CUR1_X, RESULT_CUR1_Y, 0.0f);
		if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(0, BUTTON_START) || IsButtonTriggered(0, BUTTON_B))
		{// �X�e�[�W�؂�ւ�
			PlaySound(SOUND_LABEL_SE_KETTEI);
			SetFade(FADE_OUT, MODE_TITLE);
		}
		break;

	case RESULT_RETRY:
		g_TexNo = RESULT_EGREEN;
		g_Pos = XMFLOAT3(RESULT_CUR2_X, RESULT_CUR2_Y, 0.0f);

		if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(0, BUTTON_START) || IsButtonTriggered(0, BUTTON_B))
		{// �X�e�[�W�؂�ւ�
			PlaySound(SOUND_LABEL_SE_KETTEI);
			SetFade(FADE_OUT, MODE_GAME);
		}
		break;

	case RESULT_EXIT:
		g_TexNo = RESULT_ERED;
		g_Pos = XMFLOAT3(RESULT_CUR3_X, RESULT_CUR3_Y, 0.0f);

		if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(0, BUTTON_START) || IsButtonTriggered(0, BUTTON_B))
		{
			GameLoop();
		}
		break;
	}



#ifdef _DEBUG	// �f�o�b�O����\������
	
#endif

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawResult(void)
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

	// ���U���g�̔w�i��`��
	{
		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[RESULT_RESULT]);

		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		SetSpriteLeftTop(g_VertexBuffer, 0.0f, 0.0f, g_w, g_h, 0.0f, 0.0f, 1.0f, 1.0f);

		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);
	}


	// �X�R�A�\��
	{
		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[RESULT_NUMBER]);

		// ��������������
		int number = GetScore();
		for (int i = 0; i < SCORE_DIGIT; i++)
		{
			// ����\�����錅�̐���
			float x = (float)(number % 10);

			// �X�R�A�̈ʒu��e�N�X�`���[���W�𔽉f
			float pw = 16*4;			// �X�R�A�̕\����
			float ph = 32*4;			// �X�R�A�̕\������
			float px = 610.0f - i*pw;	// �X�R�A�̕\���ʒuX
			float py = 300.0f;			// �X�R�A�̕\���ʒuY

			float tw = 1.0f / 10;		// �e�N�X�`���̕�
			float th = 1.0f / 1;		// �e�N�X�`���̍���
			float tx = x * tw;			// �e�N�X�`���̍���X���W
			float ty = 0.0f;			// �e�N�X�`���̍���Y���W

			// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
			SetSpriteColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

			// �|���S���`��
			GetDeviceContext()->Draw(4, 0);

			// ���̌���
			number /= 10;
		}

	}

	// �J�[�\����`��
	{
		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_TexNo]);

		//�A�j���[�V�����p
		float tw = 1.0f / TEXTURE_PATTERN_DIVIDE_X;	// �e�N�X�`���̕�
		float th = 1.0f / TEXTURE_PATTERN_DIVIDE_Y;	// �e�N�X�`���̍���
		float tx = (float)(Epattern % TEXTURE_PATTERN_DIVIDE_X) * tw;	// �e�N�X�`���̍���X���W
		float ty = (float)(Epattern / TEXTURE_PATTERN_DIVIDE_X) * th;	// �e�N�X�`���̍���Y���W

		if (cursor % 2 == 1)//�E�ړ��̃G�l�~�[�������Ό����ɂ���
		{
			tw *= -1;
			tx += 1.0f / TEXTURE_PATTERN_DIVIDE_X;
		}


		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		SetSpriteLeftTop(g_VertexBuffer, g_Pos.x, g_Pos.y, CUR_WIDTH, CUR_HEIGHT, tx, ty, tw, th);

		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);
	}
}




