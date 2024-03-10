//=============================================================================
//
// UI���� [ui.cpp]
// Author : HAL���� ������
//
//=============================================================================
#include "ui.h"
#include "input.h"
#include "player.h"
#include "flag.h"
#include "score.h"
#include "fade.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_WIDTH					(SCREEN_WIDTH)	// �w�i�T�C�Y
#define TEXTURE_HEIGHT					(SCREEN_HEIGHT)
#define TEXTURE_MAX						(9)	// �e�N�X�`���̐�


// HP��MP�̃��S
#define TEXTURE_HPMP_ROGO_X				(TEXTURE_PLAYER_ICON_WIDTH + 54)	// HP��MP���S�̈ʒu
#define TEXTURE_HPMP_ROGO_Y				(44)

#define TEXTURE_HPMP_ROGO_WIDTH			(64)	// HP��MP���S�̃T�C�Y
#define TEXTURE_HPMP_ROGO_HEIGHT		(64)


// HP�̃J���Q�[�W
#define TEXTURE_KARAGAGE_HP_X			(TEXTURE_PLAYER_ICON_WIDTH + TEXTURE_HPMP_ROGO_WIDTH + 50)	// HP�̃J���Q�[�W�̈ʒu
#define TEXTURE_KARAGAGE_HP_Y			(20)

#define TEXTURE_KARAGAGE_HP_WIDTH		(300)	// HP�̃J���Q�[�W�̃T�C�Y
#define TEXTURE_KARAGAGE_HP_HEIGHT		(16)

// HP�̃Q�[�W
#define TEXTURE_GAGE_HP_X				(TEXTURE_PLAYER_ICON_WIDTH + TEXTURE_HPMP_ROGO_WIDTH + 50)	// HP�̃Q�[�W�̈ʒu
#define TEXTURE_GAGE_HP_Y				(20)

#define TEXTURE_GAGE_HP_WIDTH			(300)	// HP�̃Q�[�W�̃T�C�Y
#define TEXTURE_GAGE_HP_HEIGHT			(16)



// MP�̃J���Q�[�W
#define TEXTURE_KARAGAGE_MP_X			(TEXTURE_PLAYER_ICON_WIDTH + TEXTURE_HPMP_ROGO_WIDTH + 50)	// MP�̃J���Q�[�W�̈ʒu
#define TEXTURE_KARAGAGE_MP_Y			(48)

#define TEXTURE_KARAGAGE_MP_WIDTH		(300)	// MP�̃J���Q�[�W�̃T�C�Y
#define TEXTURE_KARAGAGE_MP_HEIGHT		(16)

// MP�̃Q�[�W
#define TEXTURE_GAGE_MP_X				(TEXTURE_PLAYER_ICON_WIDTH + TEXTURE_HPMP_ROGO_WIDTH + 50)	// MP�̃Q�[�W�̈ʒu
#define TEXTURE_GAGE_MP_Y				(48)						

#define TEXTURE_GAGE_MP_WIDTH			(300)	// MP�̃Q�[�W�̃T�C�Y
#define TEXTURE_GAGE_MP_HEIGHT			(16)

// �J���`���[�W�Q�[�W�̃Q�[�W
#define TEXTURE_KARAGAGE_CHARGE_X		(TEXTURE_PLAYER_ICON_WIDTH + TEXTURE_HPMP_ROGO_WIDTH + 50)	// �`���[�W�Q�[�W�̃Q�[�W�̈ʒu
#define TEXTURE_KARAGAGE_CHARGE_Y		(62)						

#define TEXTURE_KARAGAGE_CHARGE_WIDTH	(150)	// �`���[�W�Q�[�W�̃Q�[�W�̃T�C�Y
#define TEXTURE_KARAGAGE_CHARGE_HEIGHT	(8)

// �`���[�W�Q�[�W�̃Q�[�W
#define TEXTURE_GAGE_CHARGE_X			(TEXTURE_PLAYER_ICON_WIDTH + TEXTURE_HPMP_ROGO_WIDTH + 50)	// �`���[�W�Q�[�W�̃Q�[�W�̈ʒu
#define TEXTURE_GAGE_CHARGE_Y			(62)						

#define TEXTURE_GAGE_CHARGE_WIDTH		(150)	// �`���[�W�Q�[�W�̃Q�[�W�̃T�C�Y
#define TEXTURE_GAGE_CHARGE_HEIGHT		(8)


// �X�L�b�v
#define TEXTURE_SKIP_X					(SCREEN_WIDTH - TEXTURE_SKIP_WIDTH - 20.0f)	// �X�L�b�v�̈ʒu
#define TEXTURE_SKIP_Y					(SCREEN_HEIGHT - TEXTURE_SKIP_HEIGHT - 10.0f)

#define TEXTURE_SKIP_WIDTH				(150)	// �X�L�b�v�̈ʒu
#define TEXTURE_SKIP_HEIGHT				(40)

// �΂��U��
#define TEXTURE_HIWO_X					(SCREEN_CENTER_X)	// �΂��U���̈ʒu
#define TEXTURE_HIWO_Y					(SCREEN_HEIGHT - TEXTURE_HIWO_HEIGHT)						

#define TEXTURE_HIWO_WIDTH				(300)	// �΂��U���̃T�C�Y
#define TEXTURE_HIWO_HEIGHT				(40)


// �v���C���[�A�C�R��
#define TEXTURE_ICON_X					(8)											// �A�C�R���̈ʒu
#define TEXTURE_ICON_Y					(8)

#define TEXTURE_ICON_RED_X				(SCREEN_WIDTH - TEXTURE_ICON_WIDTH)			// �ԃA�C�R���̈ʒu
#define TEXTURE_ICON_RED_Y				(90)

#define TEXTURE_ICON_BLUE_X				(SCREEN_WIDTH - TEXTURE_ICON_WIDTH)			// �A�C�R���̈ʒu
#define TEXTURE_ICON_BLUE_Y				(170)

#define TEXTURE_ICON_GREEN_X			(SCREEN_WIDTH - TEXTURE_ICON_WIDTH)			// �΃A�C�R���̈ʒu
#define TEXTURE_ICON_GREEN_Y			(250)

#define TEXTURE_ICON_YELLOW_X			(SCREEN_WIDTH - TEXTURE_ICON_WIDTH)			// ���A�C�R���̈ʒu
#define TEXTURE_ICON_YELLOW_Y			(330)


#define TEXTURE_ICON_WIDTH				(60)	// �v���C���[�A�C�R���̃T�C�Y
#define TEXTURE_ICON_HEIGHT				(60)

#define TEXTURE_PLAYER_ICON_WIDTH		(140)	// ����̃v���C���[�A�C�R���̃T�C�Y
#define TEXTURE_PLAYER_ICON_HEIGHT		(140)

#define TEXTURE_ICON_DISTANCE			(20)

enum
{
	UI_GAGE_KARA,
	UI_HP_MP,
	UI_SKIP,
	UI_HIWO,
	UI_PLAYER_ICON_RED,
	UI_PLAYER_ICON_BLUE,
	UI_PLAYER_ICON_GREEN,
	UI_PLAYER_ICON_YELLOW,
	UI_PLAYER_ICON,

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
	"data/TEXTURE/game/gage_kara.jpg",
	"data/TEXTURE/game/hp_mp.jpg",
	"data/TEXTURE/game/skip.png",
	"data/TEXTURE/game/hiwo.png",
	"data/TEXTURE/cursor/Red.png",
	"data/TEXTURE/cursor/Blue.png",
	"data/TEXTURE/cursor/Green.png",
	"data/TEXTURE/cursor/Yellow.png",
	"data/TEXTURE/cursor/playericon.png",
};


static BOOL						g_Use;						// TRUE:�g���Ă���  FALSE:���g�p
static float					g_w, g_h;					// ���ƍ���
static XMFLOAT3					g_Pos;						// �|���S���̍��W
static int						g_TexNo;					// �e�N�X�`���ԍ�

static float					alpha;
static BOOL						flag_alpha;


static BOOL						g_Load = FALSE;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitUi(void)
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


	alpha = 1.0f;
	flag_alpha = TRUE;


	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitUi(void)
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
void UpdateUi(void)
{
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


#ifdef _DEBUG	// �f�o�b�O����\������
	
#endif

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawUi(void)
{
	PLAYER* player = GetPlayer();

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

	// �C�x���g���͕`�悵�Ȃ�
	if (GetFlag() == FLAG_GAME_START || GetFlag() == FLAG_GAME_BATTLE)
	{
		// HP��MP�̃��S��`��
		{
			// �e�N�X�`���ݒ�
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[UI_HP_MP]);


			//�\���ʒu
			float pw = TEXTURE_HPMP_ROGO_WIDTH;			// HP��MP�̃��S�̕\����
			float ph = TEXTURE_HPMP_ROGO_HEIGHT;		// HP��MP�̃��S�̕\������
			float px = TEXTURE_HPMP_ROGO_X;				// HP��MP�̃��S�̕\���ʒuX	
			float py = TEXTURE_HPMP_ROGO_Y;				// HP��MP�̃��S�̕\���ʒuY

			float tw = 1.0f;							// �e�N�X�`���̃��S�̕�
			float th = 1.0f;							// �e�N�X�`���̃��S�̍���
			float tx = 0.0f;							// �e�N�X�`���̃��S�̍���X���W
			float ty = 0.0f;							// �e�N�X�`���̃��S�̍���Y���W

			// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
			SetSpriteColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));

			// �|���S���`��
			GetDeviceContext()->Draw(4, 0);
		}

		// HP�̃J���̃Q�[�W��`��
		{
			// �e�N�X�`���ݒ�
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[UI_GAGE_KARA]);


			//�\���ʒu
			float pw = TEXTURE_KARAGAGE_HP_WIDTH;			// HP�̃J���̃Q�[�W�̕\����
			float ph = TEXTURE_KARAGAGE_HP_HEIGHT;			// HP�̃J���̃Q�[�W�̕\������
			float px = TEXTURE_KARAGAGE_HP_X;				// HP�̃J���̃Q�[�W�̕\���ʒuX	
			float py = TEXTURE_KARAGAGE_HP_Y;				// HP�̃J���̃Q�[�W�̕\���ʒuY

			float tw = 1.0f;								// �e�N�X�`���̕�
			float th = 1.0f;								// �e�N�X�`���̍���
			float tx = 0.0f;								// �e�N�X�`���̍���X���W
			float ty = 0.0f;								// �e�N�X�`���̍���Y���W

			// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
			SetSpriteLeftTop(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th);

			// �|���S���`��
			GetDeviceContext()->Draw(4, 0);
		}

		// �J���̃`���[�W�Q�[�W��`��
		{
			// �e�N�X�`���ݒ�
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[UI_GAGE_KARA]);


			//�\���ʒu
			float pw = TEXTURE_KARAGAGE_CHARGE_WIDTH;		// �J���̃`���[�W�Q�[�W�̕\����
			float ph = TEXTURE_KARAGAGE_CHARGE_HEIGHT;		// �J���̃`���[�W�Q�[�W�̕\������
			float px = TEXTURE_KARAGAGE_CHARGE_X;			// �J���̃`���[�W�Q�[�W�̕\���ʒuX	
			float py = TEXTURE_KARAGAGE_CHARGE_Y;			// �J���̃`���[�W�Q�[�W�̕\���ʒuY

			float tw = 1.0f;								// �e�N�X�`���̕�
			float th = 1.0f;								// �e�N�X�`���̍���
			float tx = 0.0f;								// �e�N�X�`���̍���X���W
			float ty = 0.0f;								// �e�N�X�`���̍���Y���W

			// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
			SetSpriteLeftTop(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th);

			// �|���S���`��
			GetDeviceContext()->Draw(4, 0);
		}

		// MP�̃J���̃Q�[�W��`��
		{
			// �e�N�X�`���ݒ�
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[UI_GAGE_KARA]);


			//�\���ʒu
			float pw = TEXTURE_KARAGAGE_MP_WIDTH;			// MP�̃J���̃Q�[�W�̕\����
			float ph = TEXTURE_KARAGAGE_MP_HEIGHT;			// MP�̃J���̃Q�[�W�̕\������
			float px = TEXTURE_KARAGAGE_MP_X;				// MP�̃J���̃Q�[�W�̕\���ʒuX	
			float py = TEXTURE_KARAGAGE_MP_Y;				// MP�̃J���̃Q�[�W�̕\���ʒuY

			float tw = 1.0f;								// �e�N�X�`���̕�
			float th = 1.0f;								// �e�N�X�`���̍���
			float tx = 0.0f;								// �e�N�X�`���̍���X���W
			float ty = 0.0f;								// �e�N�X�`���̍���Y���W

			// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
			SetSpriteLeftTop(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th);

			// �|���S���`��
			GetDeviceContext()->Draw(4, 0);
		}

		// HP�̃Q�[�W��`��
		{
			// �e�N�X�`���ݒ�
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[UI_GAGE_KARA]);

			float playerHpRatio;

			playerHpRatio = player->hp / (float)PLAYER_HP_MAX; // �i���݂̃v���C���[��HP�j/�i�v���C���[�̍ő�HP�j
					//�\���ʒu
			float px = TEXTURE_GAGE_HP_X;				// HP�̃Q�[�W�̕\���ʒuX	
			float py = TEXTURE_GAGE_HP_Y;				// HP�̃Q�[�W�̕\���ʒuY
			float pw = TEXTURE_GAGE_HP_WIDTH * playerHpRatio;			// HP�̃Q�[�W�̕\����
			float ph = TEXTURE_GAGE_HP_HEIGHT;			// HP�̃Q�[�W�̕\������

			float tx = 0.0f;							// �e�N�X�`���̍���X���W
			float ty = 0.0f;							// �e�N�X�`���̍���Y���W
			float tw = 1.0f * playerHpRatio;							// �e�N�X�`���̕�
			float th = 1.0f;							// �e�N�X�`���̍���

			SetSpriteLTColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th, XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f));

			GetDeviceContext()->Draw(4, 0);
		}

		// MP�̃Q�[�W��`��
		{
			// �e�N�X�`���ݒ�
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[UI_GAGE_KARA]);

			float playerMpRatio;

			playerMpRatio = player->mp / (float)PLAYER_MP_MAX; // �i���݂̃v���C���[��HP�j/�i�v���C���[�̍ő�HP�j
					//�\���ʒu
			float px = TEXTURE_GAGE_MP_X;				// HP�̃Q�[�W�̕\���ʒuX	
			float py = TEXTURE_GAGE_MP_Y;				// HP�̃Q�[�W�̕\���ʒuY
			float pw = TEXTURE_GAGE_MP_WIDTH * playerMpRatio;			// HP�̃Q�[�W�̕\����
			float ph = TEXTURE_GAGE_MP_HEIGHT;			// HP�̃Q�[�W�̕\������

			float tx = 0.0f;							// �e�N�X�`���̍���X���W
			float ty = 0.0f;							// �e�N�X�`���̍���Y���W
			float tw = 1.0f * playerMpRatio;							// �e�N�X�`���̕�
			float th = 1.0f;							// �e�N�X�`���̍���

			SetSpriteLTColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th, XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f));

			GetDeviceContext()->Draw(4, 0);
		}

		// �`���[�W�Q�[�W��`��
		{
			// �e�N�X�`���ݒ�
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[UI_GAGE_KARA]);

			float playerchargeRatio;

			playerchargeRatio = player->chargeCnt / (float)CHARGE_CNT_MAX; // �i���݂̃v���C���[��HP�j/�i�v���C���[�̍ő�HP�j
					//�\���ʒu
			float px = TEXTURE_GAGE_CHARGE_X;				// HP�̃Q�[�W�̕\���ʒuX	
			float py = TEXTURE_GAGE_CHARGE_Y;				// HP�̃Q�[�W�̕\���ʒuY
			float pw = TEXTURE_GAGE_CHARGE_WIDTH * playerchargeRatio;			// HP�̃Q�[�W�̕\����
			float ph = TEXTURE_GAGE_CHARGE_HEIGHT;			// HP�̃Q�[�W�̕\������

			float tx = 0.0f;							// �e�N�X�`���̍���X���W
			float ty = 0.0f;							// �e�N�X�`���̍���Y���W
			float tw = 1.0f * playerchargeRatio;							// �e�N�X�`���̕�
			float th = 1.0f;							// �e�N�X�`���̍���

			SetSpriteLTColor(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th, XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f));

			GetDeviceContext()->Draw(4, 0);
		}


		// �A�C�R���Ԃ�`��
		{
			// �e�N�X�`���ݒ�
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[UI_PLAYER_ICON_RED]);

			if (player->type != PLAYER_RED)
			{
				// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
				SetSpriteColor(g_VertexBuffer, TEXTURE_ICON_RED_X, TEXTURE_ICON_RED_Y,
					TEXTURE_ICON_WIDTH, TEXTURE_ICON_HEIGHT, 0.0f, 0.0f, 1.0f, 1.0f,
					XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0));

			}
			else
			{
				SetSpriteColor(g_VertexBuffer, TEXTURE_ICON_RED_X, TEXTURE_ICON_RED_Y,
					TEXTURE_ICON_WIDTH * 0.7, TEXTURE_ICON_HEIGHT * 0.7, 0.0f, 0.0f, 1.0f, 1.0f,
					XMFLOAT4(0.7f, 0.7f, 0.7f, 0.7f));

			}
			// �|���S���`��
			GetDeviceContext()->Draw(4, 0);

		}

		// �A�C�R����`��
		{
			// �e�N�X�`���ݒ�
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[UI_PLAYER_ICON_BLUE]);

			if (player->type != PLAYER_BLUE)
			{
				// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
				SetSpriteColor(g_VertexBuffer, TEXTURE_ICON_BLUE_X, TEXTURE_ICON_BLUE_Y,
					TEXTURE_ICON_WIDTH, TEXTURE_ICON_HEIGHT, 0.0f, 0.0f, 1.0f, 1.0f,
					XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0));

			}
			else
			{
				SetSpriteColor(g_VertexBuffer, TEXTURE_ICON_BLUE_X, TEXTURE_ICON_BLUE_Y,
					TEXTURE_ICON_WIDTH * 0.7, TEXTURE_ICON_HEIGHT * 0.7, 0.0f, 0.0f, 1.0f, 1.0f,
					XMFLOAT4(0.7f, 0.7f, 0.7f, 0.7f));

			}
			// �|���S���`��
			GetDeviceContext()->Draw(4, 0);

		}

		// �A�C�R���΂�`��
		{
			// �e�N�X�`���ݒ�
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[UI_PLAYER_ICON_GREEN]);

			if (player->type != PLAYER_GREEN)
			{
				// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
				SetSpriteColor(g_VertexBuffer, TEXTURE_ICON_GREEN_X, TEXTURE_ICON_GREEN_Y,
					TEXTURE_ICON_WIDTH, TEXTURE_ICON_HEIGHT, 0.0f, 0.0f, 1.0f, 1.0f,
					XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0));

			}
			else
			{
				SetSpriteColor(g_VertexBuffer, TEXTURE_ICON_GREEN_X, TEXTURE_ICON_GREEN_Y,
					TEXTURE_ICON_WIDTH * 0.7, TEXTURE_ICON_HEIGHT * 0.7, 0.0f, 0.0f, 1.0f, 1.0f,
					XMFLOAT4(0.7f, 0.7f, 0.7f, 0.7f));

			}
			// �|���S���`��
			GetDeviceContext()->Draw(4, 0);

		}

		// �A�C�R������`��
		{
			// �e�N�X�`���ݒ�
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[UI_PLAYER_ICON_YELLOW]);

			if (player->type != PLAYER_YELLOW)
			{
				// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
				SetSpriteColor(g_VertexBuffer, TEXTURE_ICON_YELLOW_X, TEXTURE_ICON_YELLOW_Y,
					TEXTURE_ICON_WIDTH, TEXTURE_ICON_HEIGHT, 0.0f, 0.0f, 1.0f, 1.0f,
					XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0));

			}
			else
			{
				SetSpriteColor(g_VertexBuffer, TEXTURE_ICON_YELLOW_X, TEXTURE_ICON_YELLOW_Y,
					TEXTURE_ICON_WIDTH * 0.7, TEXTURE_ICON_HEIGHT * 0.7, 0.0f, 0.0f, 1.0f, 1.0f,
					XMFLOAT4(0.7f, 0.7f, 0.7f, 0.7f));

			}
			// �|���S���`��
			GetDeviceContext()->Draw(4, 0);

		}

		// �A�C�R����`��
		{
			// �e�N�X�`���ݒ�
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[UI_PLAYER_ICON]);

			switch (player->type)
			{
			case PLAYER_RED:
					// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
					SetSpriteLTColor(g_VertexBuffer, TEXTURE_ICON_X, TEXTURE_ICON_Y,
						TEXTURE_PLAYER_ICON_WIDTH, TEXTURE_PLAYER_ICON_HEIGHT, 0.0f, 0.0f, 0.25f, 1.0f,
						XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0));
				break;

			case PLAYER_BLUE:
					// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
					SetSpriteLTColor(g_VertexBuffer, TEXTURE_ICON_X, TEXTURE_ICON_Y,
						TEXTURE_PLAYER_ICON_WIDTH, TEXTURE_PLAYER_ICON_HEIGHT, 0.25f, 0.0f, 0.25f, 1.0f,
						XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0));
				break;

			case PLAYER_GREEN:
					// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
					SetSpriteLTColor(g_VertexBuffer, TEXTURE_ICON_X, TEXTURE_ICON_Y,
						TEXTURE_PLAYER_ICON_WIDTH, TEXTURE_PLAYER_ICON_HEIGHT, 0.5f, 0.0f, 0.25f, 1.0f,
						XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0));
				break;

			case PLAYER_YELLOW:
					// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
					SetSpriteLTColor(g_VertexBuffer, TEXTURE_ICON_X, TEXTURE_ICON_Y,
						TEXTURE_PLAYER_ICON_WIDTH, TEXTURE_PLAYER_ICON_HEIGHT, 0.75f, 0.0f, 0.25f, 1.0f,
						XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0));
				break;

			}

			// �|���S���`��
			GetDeviceContext()->Draw(4, 0);

		}

	}


	// �΂̈ړ��C�x���g���ɕ`��
	if (GetFlag() == FLAG_GAME_EVENT_FIRE)
	{

		// �X�L�b�v��`��
		{
			// �e�N�X�`���ݒ�
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[UI_SKIP]);

			// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
			SetSpriteLTColor(g_VertexBuffer, TEXTURE_SKIP_X, TEXTURE_SKIP_Y,
				TEXTURE_SKIP_WIDTH, TEXTURE_SKIP_HEIGHT, 0.0f, 0.0f, 1.0f, 1.0f,
				XMFLOAT4(1.0f, 1.0f, 1.0f, alpha));

			// �|���S���`��
			GetDeviceContext()->Draw(4, 0);
		}
	}

	// �C�x���g�ɓ���܂ŕ`��
	if (GetFlag() == FLAG_GAME_START)
	{
		// �΂��U����`��
		{
			// �e�N�X�`���ݒ�
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[UI_HIWO]);

			// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
			SetSpriteColor(g_VertexBuffer, TEXTURE_HIWO_X, TEXTURE_HIWO_Y,
				TEXTURE_HIWO_WIDTH, TEXTURE_HIWO_HEIGHT, 0.0f, 0.0f, 1.0f, 1.0f,
				XMFLOAT4(1.0f, 1.0f, 1.0f, alpha));

			// �|���S���`��
			GetDeviceContext()->Draw(4, 0);
		}

	}

}