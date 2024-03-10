//=============================================================================
//
// aim���� [aim.cpp]
// Author : HAL���� ������
//
//=============================================================================
#include "main.h"

#include "renderer.h"
#include "debugproc.h"
#include "player.h"
#include "aim.h"
#include "flag.h"
#include "sprite.h"


//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_MAX					(1)		// �e�N�X�`���̐�

#define AIM_POS_X_RED				(540.0f)	// �Ԃ̃G�C���J�[�\���̈ʒu
#define AIM_POS_Y_RED				(250.0f)

#define AIM_POS_X_BLUE				(620.0f)	// �Ԃ̃G�C���J�[�\���̈ʒu
#define AIM_POS_Y_BLUE				(230.0f)

#define AIM_POS_X_GREEN				(510.0f)	// �Ԃ̃G�C���J�[�\���̈ʒu
#define AIM_POS_Y_GREEN				(223.0f)

#define AIM_POS_X_YELLOW			(590.0f)	// �Ԃ̃G�C���J�[�\���̈ʒu
#define AIM_POS_Y_YELLOW			(207.0f)

#define AIM_SIZE					(50.0f)		// �G�C���J�[�\���̑傫��

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer* g_VertexBuffer = NULL;		// ���_���
static ID3D11ShaderResourceView* g_Texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����

static char* g_TexturName[TEXTURE_MAX] = {

	"data/TEXTURE/game/Aim01.png",
};


static BOOL						g_Use;						// TRUE:�g���Ă���  FALSE:���g�p
static float					g_w, g_h;					// ���ƍ���
static XMFLOAT3					g_Pos;						// �|���S���̍��W
static int						g_TexNo;					// �e�N�X�`���ԍ�

static XMFLOAT4 color = { 0.0f, 0.0f, 0.0f, 1.0f };


static BOOL						g_Load = FALSE;


//=============================================================================
// ����������
//=============================================================================
HRESULT InitAim(void)
{
	ID3D11Device* pDevice = GetDevice();

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

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitAim(void)
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
void UpdateAim(void)
{
	PLAYER* player = GetPlayer();

	// �F�ƃ|�W�V������ς���
	switch (player->type)
	{
	case PLAYER_RED:
		g_Pos.x = AIM_POS_X_RED;
		g_Pos.y = AIM_POS_Y_RED;
		color = { 1.0f, 0.0f, 0.0f, 1.0f };

		break;

	case PLAYER_BLUE:
		g_Pos.x = AIM_POS_X_BLUE;
		g_Pos.y = AIM_POS_Y_BLUE;
		color = { 0.0f, 0.0f, 1.0f, 1.0f };

		break;

	case PLAYER_GREEN:
		g_Pos.x = AIM_POS_X_GREEN;
		g_Pos.y = AIM_POS_Y_GREEN;
		color = { 0.0f, 1.0f, 0.0f, 1.0f };

		break;

	case PLAYER_YELLOW:
		g_Pos.x = AIM_POS_X_YELLOW;
		g_Pos.y = AIM_POS_Y_YELLOW;
		color = { 1.0f, 1.0f, 0.0f, 1.0f };

		break;
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawAim(void)
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

	// �e�N�X�`���ݒ�
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_TexNo]);

	// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
	SetSpriteColor(g_VertexBuffer, g_Pos.x, g_Pos.y, AIM_SIZE, AIM_SIZE, 0.0f, 0.0f, 1.0f, 1.0f,
						color);

	if (GetFlag() == FLAG_GAME_START || GetFlag() == FLAG_GAME_BATTLE)
	{
		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);
	}
}