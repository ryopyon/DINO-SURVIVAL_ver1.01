//=============================================================================
//
// �^�C�g����ʏ��� [title.cpp]
// Author : HAL���� ������
//
//=============================================================================
#include "title.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "renderer.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_WIDTH					(SCREEN_WIDTH)			// �w�i�T�C�Y
#define TEXTURE_HEIGHT					(SCREEN_HEIGHT)
#define TEXTURE_MAX						(9)						// �e�N�X�`���̐�

// �^�C�g�����S
#define TEXTURE_TITLELOGO_WIDTH			(600)					// �^�C�g�����S�T�C�Y
#define TEXTURE_TITLELOGO_HEIGHT		(300)

#define TEXTURE_TITLELOGO_X				(SCREEN_CENTER_X)	// �^�C�g�����S�̈ʒu
#define TEXTURE_TITLELOGO_Y				(150)

// �J�[�\��
#define CUR_WIDTH						(50)					//	�J�[�\���T�C�Y
#define CUR_HEIGHT						(50)

#define TITLE_CUR1_X					(370.0f)				//	�J�[�\���̈ʒu
#define TITLE_CUR1_Y					(275.0f)

#define TITLE_CUR2_X					(548.0f)
#define TITLE_CUR2_Y					(323.0f)

#define TITLE_CUR3_X					(370.0f)
#define TITLE_CUR3_Y					(370.0f)

#define TITLE_CUR4_X					(548.0f)
#define TITLE_CUR4_Y					(417.0f)


//�t�F�[�h�̃A�j���[�V�����p(LOGO)
#define ANIM_LOGO_WAIT_TIME				(90)	//�� �� ��Ѓ��S
#define ANIM_LOGO_FADE_TIME				(90)	//��Ѓ��S �� ��
#define ANIM_LOGO_TOTAL_TIME			(ANIM_LOGO_WAIT_TIME + ANIM_LOGO_FADE_TIME)

//�t�F�[�h�̃A�j���[�V�����p(TITLE)
#define ANIM_TITLE_WAIT_TIME			(0)
#define ANIM_TITLE_FADE_TIME			(45)
#define ANIM_TITLE_TOTAL_TIME			(ANIM_TITLE_WAIT_TIME + ANIM_TITLE_FADE_TIME)

// ���C�v
#define WIPE_RATE						(30.0f)
#define WIPE_BORDER1					(500.0f)
#define WIPE_BORDER2					(1900.0f)

// �G�l�~�[�̃A�j���[�V�����p
#define ENEMY_WAIT						(4)

#define TEXTURE_PATTERN_DIVIDE_X		(7)		// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define TEXTURE_PATTERN_DIVIDE_Y		(1)		// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define ANIM_PATTERN_NUM				(TEXTURE_PATTERN_DIVIDE_X*TEXTURE_PATTERN_DIVIDE_Y)	// �A�j���[�V�����p�^�[����


enum
{
	ANIM_LOGO,	//= 0
	ANIM_TITLE,	
	TITLE_MENU,
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;				// ���_���
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����

static char *g_TexturName[TEXTURE_MAX] = {
	"data/TEXTURE/title/logo.png",
	"data/TEXTURE/title/title.png",
	"data/TEXTURE/title/titlelogo.png",
	"data/TEXTURE/effect000.jpg",
	"data/TEXTURE/cursor/Eyellow.png",
	"data/TEXTURE/cursor/Egreen.png",
	"data/TEXTURE/cursor/Eblue.png",
	"data/TEXTURE/cursor/Ered.png",
	"data/TEXTURE/fade_black.png",
};


static BOOL						g_Use;						// TRUE:�g���Ă���  FALSE:���g�p
static float					g_w, g_h;					// ���ƍ���
static XMFLOAT3					g_Pos;						// �|���S���̍��W
static int						g_TexNo;					// �e�N�X�`���ԍ�

// ���C�v
static BOOL						g_wipe = FALSE;
static float					g_wipeSize = 0.0f;

static float					alpha;
static BOOL						flag_alpha;

static BOOL						g_Load = FALSE;

int								animCnt;
int								animMode;

float							EanimCnt;
int								Epattern;

static float					effect_dx;
static float					effect_dy;

int								cursor;
//=============================================================================
// ����������
//=============================================================================
HRESULT InitTitle(void)
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
	g_Pos   = XMFLOAT3(200.0f, 200.0f, 0.0f);
	g_TexNo = 0;

	alpha = 1.0f;
	flag_alpha = TRUE;

	effect_dx = 100.0f;
	effect_dy = 100.0f;

	cursor = 0;

	animCnt = 0;
	animMode = 0;

	EanimCnt = 0;
	Epattern = 0;

	// ���C�v��������
	SetWipe(WIPE_MODE_NONE, XMFLOAT4(1.0f, 1.0f, 0.0f, 0.0f), g_wipeSize);

	// ���m�N���𒼂�
	SetMonochrome(FALSE);

	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitTitle(void)
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
void UpdateTitle(void)
{
	if (animMode == ANIM_LOGO)	//�^�C�g����`�悵�Ă���^�C�g���ɃV�[���J��
	{
		if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(0, BUTTON_START) || IsButtonTriggered(0, BUTTON_B))
		{
			animMode = ANIM_TITLE;
			animCnt = 0;
		}

		animCnt++;

		if(animCnt == ANIM_LOGO_TOTAL_TIME)
		{
			animMode = ANIM_TITLE;
			animCnt = 0;
		}
	}

	else if (animMode == ANIM_TITLE)	//�^�C�g����`�悵�Ă���^�C�g���ɃV�[���J��
	{
		animCnt++;
		if (animCnt == ANIM_TITLE_TOTAL_TIME)
		{
			animMode = TITLE_MENU;
			PlaySound(SOUND_LABEL_BGM_TITLE);
		}
	}



	else if (animMode == TITLE_MENU)
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
			cursor = (++cursor + TITLE_MAX) % TITLE_MAX;	//++cursor�ɂ����������₷������
		}
		if (GetKeyboardTrigger(DIK_UP) || GetKeyboardTrigger(DIK_W) || IsButtonTriggered(0, STICK_L_UP) || IsButtonTriggered(0, BUTTON_UP))
		{
			PlaySound(SOUND_LABEL_SE_CRICK);
			cursor = (--cursor + TITLE_MAX) % TITLE_MAX;	//�i-1 + 4�j% 4
		}

		if (flag_alpha == TRUE)
		{
			alpha -= 0.02f;
			if (alpha <= 0.0f)
			{
				alpha = 0.0f;
				flag_alpha = FALSE;
			}
		}
		else
		{
			alpha += 0.02f;
			if (alpha >= 1.0f)
			{
				alpha = 1.0f;
				flag_alpha = TRUE;
			}
		}


		switch (cursor)		//�J�[�\���̕\�L
		{
		case TUTORIAL:
			g_TexNo = TITLE_ERED;
			g_Pos = XMFLOAT3(TITLE_CUR1_X, TITLE_CUR1_Y, 0.0f);

			if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(0, BUTTON_START) || IsButtonTriggered(0, BUTTON_B))
			{// �X�e�[�W�؂�ւ�
				PlaySound(SOUND_LABEL_SE_KETTEI);
				g_wipe = TRUE;
			}
			break;

		case GAMESTART:
			g_TexNo = TITLE_EBLUE;
			g_Pos = XMFLOAT3(TITLE_CUR2_X, TITLE_CUR2_Y, 0.0f);

			if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(0, BUTTON_START) || IsButtonTriggered(0, BUTTON_B))
			{
				// �X�e�[�W�؂�ւ�
				PlaySound(SOUND_LABEL_SE_KETTEI);
				g_wipe = TRUE;
			}
			break;

		case CREDIT:
			g_TexNo = TITLE_EGREEN;
			g_Pos = XMFLOAT3(TITLE_CUR3_X, TITLE_CUR3_Y, 0.0f);

			if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(0, BUTTON_START) || IsButtonTriggered(0, BUTTON_B))
			{// �X�e�[�W�؂�ւ�
				PlaySound(SOUND_LABEL_SE_KETTEI);
				g_wipe = TRUE;
			}
			break;

		case EXIT:
			g_TexNo = TITLE_EYELLOW;
			g_Pos = XMFLOAT3(TITLE_CUR4_X, TITLE_CUR4_Y, 0.0f);

			if (GetKeyboardTrigger(DIK_RETURN) || IsButtonTriggered(0, BUTTON_START) || IsButtonTriggered(0, BUTTON_B))
			{
				GameLoop();
			}
		}
	}

	// ���C�v����
	if (g_wipe)
	{
		if (cursor != GAMESTART)
		{
			// WIPE_RATE�Ƃ��̒l�̓A�j���[�V�������Ȃ��璲�����Ă������� (���C�v�̎�ނɂ���Ă��������ɂȂ�l�S�R�Ⴄ�̂ł��낢�뎎���Ă݂�)
			g_wipeSize += WIPE_RATE;					// ���X�Ƀ��C�v�͈̔͂𑝂₷
			SetWipe(WIPE_MODE_FLIP, XMFLOAT4(1.0f, 1.0f, 0.0f, 0.0f), g_wipeSize);

			if (g_wipeSize >= WIPE_BORDER1)
			{
				switch (cursor)
				{
				case TUTORIAL:
					SetFade(FADE_OUT, MODE_TUTORIAL);	// �r������fade�����n�߂�
					break;

				case CREDIT:
					SetFade(FADE_OUT, MODE_CREDIT);		// �r������fade�����n�߂�
					break;
				}
			}
			if (g_wipeSize >= WIPE_BORDER2)
			{
				g_wipeSize = 0.0f;
				SetWipe(WIPE_MODE_NONE, XMFLOAT4(1.0f, 1.0f, 0.0f, 0.0f), g_wipeSize);			// ���C�v�̏I��
				g_wipe = FALSE;
			}
		}
		else
		{
			SetFade(FADE_OUT, MODE_GAME);
			g_wipe = FALSE;
		}

	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawTitle(void)
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

	switch (animMode)
	{
		case ANIM_LOGO:
		{

			// ��Ѓ��S�̕`��
			{
				GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[TITLE_LOGO]);

				// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
				SetSpriteLeftTop(g_VertexBuffer, 0.0f, 0.0f, TEXTURE_WIDTH, TEXTURE_HEIGHT, 0.0f, 0.0f, 1.0f, 1.0f);

				// �|���S���`��
				GetDeviceContext()->Draw(4, 0);
			}

			// ��Ѓ��S�̃t�F�[�h��`��
			{
				GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[TITLE_FADE_BLACK]);

				// �A���t�@�l
				XMFLOAT4 RGBA = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
				RGBA.w = (1.0f / ANIM_LOGO_FADE_TIME) * (animCnt - ANIM_LOGO_WAIT_TIME);

				// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
				SetSpriteColor(g_VertexBuffer, TEXTURE_WIDTH / 2, TEXTURE_HEIGHT / 2, TEXTURE_WIDTH, TEXTURE_HEIGHT,
					0.0f, 0.0f, 1.0f, 1.0f,
					RGBA);


				// �|���S���`��
				GetDeviceContext()->Draw(4, 0);
			}

		}
			break;

		case ANIM_TITLE:
		{

			// �^�C�g���̕`��
			{
				GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[TITLE_TITLE]);

				// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
				SetSpriteLeftTop(g_VertexBuffer, 0.0f, 0.0f, TEXTURE_WIDTH, TEXTURE_HEIGHT, 0.0f, 0.0f, 1.0f, 1.0f);

				// �|���S���`��
				GetDeviceContext()->Draw(4, 0);
			}

			// �^�C�g���̃t�F�[�h��`��
			{
				GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[TITLE_FADE_BLACK]);

				// �A���t�@�l
				XMFLOAT4 RGBA = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
				RGBA.w = 1.0f - (1.0f / ANIM_TITLE_FADE_TIME) * (animCnt - ANIM_TITLE_WAIT_TIME);

				SetSpriteColor(g_VertexBuffer, TEXTURE_WIDTH / 2, TEXTURE_HEIGHT / 2, TEXTURE_WIDTH, TEXTURE_HEIGHT,
					0.0f, 0.0f, 1.0f, 1.0f,
					RGBA);


				// �|���S���`��
				GetDeviceContext()->Draw(4, 0);
			}

		}
			break;

		case TITLE_MENU:

		// �w�i��`��
		{
			// �e�N�X�`���ݒ�
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[TITLE_TITLE]);

			// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
			SetSpriteLeftTop(g_VertexBuffer, 0.0f, 0.0f, TEXTURE_WIDTH, TEXTURE_HEIGHT, 0.0f, 0.0f, 1.0f, 1.0f);

			// �|���S���`��
			GetDeviceContext()->Draw(4, 0);
		}

		// �^�C�g�����S��`��
		{
			// �e�N�X�`���ݒ�
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[TITLE_TITLELOGO]);

			// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
			SetSpriteColor(g_VertexBuffer, TEXTURE_TITLELOGO_X, TEXTURE_TITLELOGO_Y,
				TEXTURE_TITLELOGO_WIDTH, TEXTURE_TITLELOGO_HEIGHT, 0.0f, 0.0f, 1.0f, 1.0f,
				XMFLOAT4(1.0f, 1.0f, 1.0f, alpha));

			// �|���S���`��
			GetDeviceContext()->Draw(4, 0);
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

		SetBlendState(BLEND_MODE_ALPHABLEND);	// ���������������ɖ߂�

		break;
	}
}





