//=============================================================================
//
// �G�l�~�[UI���� [ui_enemy.cpp]
// Author : HAL���� ������
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "input.h"
#include "camera.h"
#include "shadow.h"
#include "ui_enemy.h"
#include "enemy.h"


//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_MAX				(2)					// �e�N�X�`���̐�

#define	UI_ENEMY_HP_WIDTH		(10.0f)				// ���_�T�C�Y
#define	UI_ENEMY_HP_HEIGHT		(2.0f)				// ���_�T�C�Y


#define	UI_ENEMY_HP_OFFSET_Y	(28.0f)				// �I�t�Z�b�g


#define	MAX_UI_ENEMY			(MAX_ENEMY)			// �G�l�~�[UI�ő吔


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexUi_Enemy(void);
void ChangeColor(XMFLOAT4 color, float width, float depth);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer					*g_VertexBuffer = NULL;	// ���_�o�b�t�@
static ID3D11ShaderResourceView		*g_Texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����

static UI_ENEMY						g_aUi_Enemy[MAX_UI_ENEMY];	// �G�l�~�[UI���[�N
static BOOL							g_bAlpaTest;		// �A���t�@�e�X�gON/OFF

static int							g_TexNo;			// �e�N�X�`���ԍ�

static char *g_TextureName[] =
{
	"data/TEXTURE/game/gage_kara.jpg",
};

//=============================================================================
// ����������
//=============================================================================
HRESULT InitUi_Enemy(void)
{
	MakeVertexUi_Enemy();

	// �e�N�X�`������
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		g_Texture[i] = NULL;
		D3DX11CreateShaderResourceViewFromFile(GetDevice(),
			g_TextureName[i],
			NULL,
			NULL,
			&g_Texture[i],
			NULL);
	}


	// �G�l�~�[UI���[�N�̏�����
	for(int nCntUi_Enemy = 0; nCntUi_Enemy < MAX_UI_ENEMY; nCntUi_Enemy++)
	{
		ZeroMemory(&g_aUi_Enemy[nCntUi_Enemy].material, sizeof(g_aUi_Enemy[nCntUi_Enemy].material));
		g_aUi_Enemy[nCntUi_Enemy].material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

		g_aUi_Enemy[nCntUi_Enemy].pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_aUi_Enemy[nCntUi_Enemy].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);
		g_aUi_Enemy[nCntUi_Enemy].fWidth = UI_ENEMY_HP_WIDTH;
		g_aUi_Enemy[nCntUi_Enemy].fHeight = UI_ENEMY_HP_HEIGHT;
		g_aUi_Enemy[nCntUi_Enemy].use = TRUE;
	}

	g_bAlpaTest = TRUE;

	ENEMY* enemy = GetEnemy();

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitUi_Enemy(void)
{
	for(int nCntTex = 0; nCntTex < TEXTURE_MAX; nCntTex++)
	{
		if(g_Texture[nCntTex] != NULL)
		{// �e�N�X�`���̉��
			g_Texture[nCntTex]->Release();
			g_Texture[nCntTex] = NULL;
		}
	}

	if(g_VertexBuffer != NULL)
	{// ���_�o�b�t�@�̉��
		g_VertexBuffer->Release();
		g_VertexBuffer = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateUi_Enemy(void)
{
	ENEMY* enemy = GetEnemy();

	for (int i = 0; i < MAX_ENEMY; i++)
	{
		if (enemy[i].use)
		{
			g_aUi_Enemy[i].pos = enemy[i].pos;
		}
	}


#ifdef _DEBUG
	// �A���t�@�e�X�gON/OFF
	if(GetKeyboardTrigger(DIK_F1))
	{
		g_bAlpaTest = g_bAlpaTest ? FALSE: TRUE;
	}

#endif

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawUi_Enemy(void)
{
	ENEMY* enemy = GetEnemy();

	// ���e�X�g�ݒ�
	if (g_bAlpaTest == TRUE)
	{
		// ���e�X�g��L����
		SetAlphaTestEnable(TRUE);
	}

	// ���C�e�B���O�𖳌�
	SetLightEnable(FALSE);

	XMMATRIX mtxScl, mtxTranslate, mtxWorld, mtxView;
	CAMERA *cam = GetCamera();

	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// �v���~�e�B�u�g�|���W�ݒ�
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	for(int i = 0; i < MAX_ENEMY; i++)
	{
		if( (enemy[i].use == TRUE) && (enemy[i].EntryTime == FALSE) && (enemy[i].LeaveTime == FALSE) )
		{
			// ���[���h�}�g���b�N�X�̏�����
			mtxWorld = XMMatrixIdentity();

			// �r���[�}�g���b�N�X���擾
			mtxView = XMLoadFloat4x4(&cam->mtxView);

			// �����s��i�����s��j��]�u�s�񂳂��ċt�s�������Ă��(����)
			mtxWorld.r[0].m128_f32[0] = mtxView.r[0].m128_f32[0];
			mtxWorld.r[0].m128_f32[1] = mtxView.r[1].m128_f32[0];
			mtxWorld.r[0].m128_f32[2] = mtxView.r[2].m128_f32[0];

			mtxWorld.r[1].m128_f32[0] = mtxView.r[0].m128_f32[1];
			mtxWorld.r[1].m128_f32[1] = mtxView.r[1].m128_f32[1];
			mtxWorld.r[1].m128_f32[2] = mtxView.r[2].m128_f32[1];

			mtxWorld.r[2].m128_f32[0] = mtxView.r[0].m128_f32[2];
			mtxWorld.r[2].m128_f32[1] = mtxView.r[1].m128_f32[2];
			mtxWorld.r[2].m128_f32[2] = mtxView.r[2].m128_f32[2];


			// �X�P�[���𔽉f
			mtxScl = XMMatrixScaling(g_aUi_Enemy[i].scl.x, g_aUi_Enemy[i].scl.y, g_aUi_Enemy[i].scl.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

			// �ړ��𔽉f
			mtxTranslate = XMMatrixTranslation(g_aUi_Enemy[i].pos.x, g_aUi_Enemy[i].pos.y + UI_ENEMY_HP_OFFSET_Y, g_aUi_Enemy[i].pos.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

			// ���[���h�}�g���b�N�X�̐ݒ�
			SetWorldMatrix(&mtxWorld);

			// �}�e���A���ݒ�
			MATERIAL temp = g_aUi_Enemy[i].material;
			temp.Diffuse= XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

			SetMaterial(g_aUi_Enemy[i].material);

			// �e�N�X�`���ݒ�
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_TexNo]);

			//�@��ɋ�̃Q�[�W��`��
			ChangeColor(XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), UI_ENEMY_HP_WIDTH * 0.5f, 0.0f);

			// �|���S���̕`��
			GetDeviceContext()->Draw(4, 0);

			// �G�l�~�[��HP�ʂŕϓ�
			float enemyHpRatio;
			enemyHpRatio = enemy[i].hp / (float)ENEMY_HP_MAX; // �i���݂̃G�l�~�[��HP�j/�i�G�l�~�[�̍ő�HP�j

			// �}�e���A���ݒ�
			SetMaterial(g_aUi_Enemy[i].material);

			// HP��`��
			ChangeColor(XMFLOAT4(0.2f, 1.0f, 0.2f, 1.0f), (UI_ENEMY_HP_WIDTH * enemyHpRatio)
				- (UI_ENEMY_HP_WIDTH * 0.5f), -0.1f);

			 //�|���S���̕`��
			GetDeviceContext()->Draw(4, 0);

		}
	}

	// ���C�e�B���O��L����
	SetLightEnable(TRUE);

	// ���e�X�g�𖳌���
	SetAlphaTestEnable(FALSE);
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
HRESULT MakeVertexUi_Enemy(void)
{
	// ���_�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);

	// ���_�o�b�t�@�ɒl���Z�b�g����
	D3D11_MAPPED_SUBRESOURCE msr;
	GetDeviceContext()->Map(g_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

	float fWidth = UI_ENEMY_HP_WIDTH;
	float fHeight = UI_ENEMY_HP_HEIGHT;

	// ���_���W�̐ݒ�
	vertex[0].Position = XMFLOAT3(-fWidth / 2.0f, fHeight, 0.0f);
	vertex[1].Position = XMFLOAT3(fWidth / 4.0f, fHeight, 0.0f);
	vertex[2].Position = XMFLOAT3(-fWidth / 2.0f, 0.0f, 0.0f);
	vertex[3].Position = XMFLOAT3(fWidth / 4.0f, 0.0f, 0.0f);

	// �@���̐ݒ�
	vertex[0].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	vertex[1].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	vertex[2].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	vertex[3].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

	// �g�U���̐ݒ�
	vertex[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	// �e�N�X�`�����W�̐ݒ�
	vertex[0].TexCoord = XMFLOAT2(0.0f, 0.0f);
	vertex[1].TexCoord = XMFLOAT2(1.0f, 0.0f);
	vertex[2].TexCoord = XMFLOAT2(0.0f, 1.0f);
	vertex[3].TexCoord = XMFLOAT2(1.0f, 1.0f);

	GetDeviceContext()->Unmap(g_VertexBuffer, 0);

	return S_OK;
}

//=============================================================================
// �G�l�~�[UI�̃p�����[�^���Z�b�g
//=============================================================================
int SetUi_Enemy(XMFLOAT3 pos, float fWidth, float fHeight, XMFLOAT4 col)
{
	ENEMY* enemy = GetEnemy();

	int nIdxUi_Enemy = -1;

	for(int nCntUi_Enemy = 0; nCntUi_Enemy < MAX_UI_ENEMY; nCntUi_Enemy++)
	{
		if(!g_aUi_Enemy[nCntUi_Enemy].use)
		{
			g_aUi_Enemy[nCntUi_Enemy].pos = pos;
			g_aUi_Enemy[nCntUi_Enemy].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);
			g_aUi_Enemy[nCntUi_Enemy].fWidth = fWidth;
			g_aUi_Enemy[nCntUi_Enemy].fHeight = fHeight;
			g_aUi_Enemy[nCntUi_Enemy].use = TRUE;

			// HP�o�[�̈ʒu���G�l�~�[�̐^��ɐݒ�
			g_aUi_Enemy[nCntUi_Enemy].hpBarPos = 
				XMFLOAT3(enemy[nCntUi_Enemy].pos.x, 
							enemy[nCntUi_Enemy].pos.y + 100.0f, 
							enemy[nCntUi_Enemy].pos.z);

			nIdxUi_Enemy = nCntUi_Enemy;

			break;
		}
	}

	return nIdxUi_Enemy;
}

void ChangeColor(XMFLOAT4 color, float width, float depth)
{
	// ���_�o�b�t�@�ɒl���Z�b�g����
	D3D11_MAPPED_SUBRESOURCE msr;
	GetDeviceContext()->Map(g_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
	
	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;
	
	float fWidth = UI_ENEMY_HP_WIDTH;
	float fHeight = UI_ENEMY_HP_HEIGHT;
	

	// ���_���W�̐ݒ�
	vertex[0].Position = XMFLOAT3(-fWidth / 2.0f, fHeight, depth);
	vertex[1].Position = XMFLOAT3(width, fHeight, depth);
	vertex[2].Position = XMFLOAT3(-fWidth / 2.0f, 0.0f, depth);
	vertex[3].Position = XMFLOAT3(width, 0.0f, depth);
	
	// �@���̐ݒ�
	vertex[0].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	vertex[1].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	vertex[2].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	vertex[3].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	
	 //�g�U���̐ݒ�
	vertex[0].Diffuse = color;
	vertex[1].Diffuse = color;
	vertex[2].Diffuse = color;
	vertex[3].Diffuse = color;
	
	// �e�N�X�`�����W�̐ݒ�
	vertex[0].TexCoord = XMFLOAT2(0.0f, 0.0f);
	vertex[1].TexCoord = XMFLOAT2(1.0f, 0.0f);
	vertex[2].TexCoord = XMFLOAT2(0.0f, 1.0f);
	vertex[3].TexCoord = XMFLOAT2(1.0f, 1.0f);
	
	GetDeviceContext()->Unmap(g_VertexBuffer, 0);
	}

//=============================================================================
// �G�l�~�[�̎擾
//=============================================================================
UI_ENEMY* GetUI_Enemy(void)
{
	return &g_aUi_Enemy[0];
}
