//=============================================================================
//
// �X�p�[�N�p�[�e�B�N������ [particle_spark.cpp]
// Author : HAL���� ������
// 
//=============================================================================

//*****************************************************************************
// �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "debugproc.h"
#include "renderer.h"

#include "camera.h"
#include "player.h"
#include "particle_spark.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_MAX						(1)

#define	PARTICLE_SPARK_SIZE_X			(4.0f)
#define	PARTICLE_SPARK_SIZE_Y			(4.0f)

#define	PARTICLE_SPARK_COLOR_R			(0.93f)
#define	PARTICLE_SPARK_COLOR_G			(0.47f)
#define	PARTICLE_SPARK_COLOR_B			(0.0f)
#define	PARTICLE_SPARK_COLOR_A			(1.0f)

#define	PARTICLE_SPARK_SPD				(1.0f)
#define	PARTICLE_SPARK_GENERATE_FRAME	(10)
#define	PARTICLE_SPARK_SCALE_FACTOR		(0.9f)

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct
{
	XMFLOAT4X4		mtxWorld;	// ���[���h�}�g���N�X

	XMFLOAT3		scl;		// �X�P�[��
	XMFLOAT3		rot;		// ���[�e�[�V����
	XMFLOAT3		pos;		// �|�W�V����
	XMFLOAT3		basePos;	// �|�W�V����(�����ꏊ)

	MATERIAL		material;	// �}�e���A��

	XMFLOAT3		move;		// �ړ�����
	float			spd;		// �X�s�[�h

	int				life;		// ����

	BOOL			origin;		// ���������ǂ���

	BOOL			use;		// �g�p��

} PARTICLE_SPARK;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexParticleSpark(void);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };

static PARTICLE_SPARK			g_ParticleSpark[PARTICLE_SPARK_MAX];

static int						g_TexNo;

static char *g_TextureName[] =
{
	"data/TEXTURE/effect000.jpg",
};

static BOOL						g_Load = FALSE;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitParticleSpark(void)
{
	// ���_���̍쐬
	MakeVertexParticleSpark();

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

	g_TexNo = 0;

	// �X�p�[�N�p�[�e�B�N���̏�����
	for(int i = 0; i < PARTICLE_SPARK_MAX; i++)
	{
		XMStoreFloat4x4(&g_ParticleSpark[i].mtxWorld, XMMatrixIdentity());

		g_ParticleSpark[i].scl = { 1.0f, 1.0f, 1.0f };
		g_ParticleSpark[i].rot = { 0.0f, 0.0f, 0.0f };
		g_ParticleSpark[i].pos = { 0.0f, 0.0f, 0.0f };
		g_ParticleSpark[i].basePos = { 0.0f, 0.0f, 0.0f };

		ZeroMemory(&g_ParticleSpark[i].material, sizeof(g_ParticleSpark[i].material));
		g_ParticleSpark[i].material.Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };

		g_ParticleSpark[i].move = { 0.0f, 0.0f, 0.0f };
		g_ParticleSpark[i].spd = 0.0f;

		g_ParticleSpark[i].life = 0;

		g_ParticleSpark[i].origin = FALSE;

		g_ParticleSpark[i].use = FALSE;
	}

	g_Load = TRUE;
	
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitParticleSpark(void)
{
	if (g_Load == FALSE) return;

	//�e�N�X�`���̉��
	for (int nCntTex = 0; nCntTex < TEXTURE_MAX; nCntTex++)
	{
		if (g_Texture[nCntTex] != NULL)
		{
			g_Texture[nCntTex]->Release();
			g_Texture[nCntTex] = NULL;
		}
	}

	// ���_�o�b�t�@�̉��
	if (g_VertexBuffer != NULL)
	{
		g_VertexBuffer->Release();
		g_VertexBuffer = NULL;
	}

	g_Load = FALSE;
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateParticleSpark(void)
{
	PLAYER* player = GetPlayer();

#ifdef _DEBUG
	int psCnt = 0;
#endif

	for (int i = 0; i < PARTICLE_SPARK_MAX; i++)
	{
		if (g_ParticleSpark[i].use)
		{
			g_ParticleSpark[i].pos.x += g_ParticleSpark[i].move.x * g_ParticleSpark[i].spd;
			g_ParticleSpark[i].pos.y += g_ParticleSpark[i].move.y * g_ParticleSpark[i].spd;
			g_ParticleSpark[i].pos.z += g_ParticleSpark[i].move.z * g_ParticleSpark[i].spd;

			g_ParticleSpark[i].scl.x *= PARTICLE_SPARK_SCALE_FACTOR;
			g_ParticleSpark[i].scl.y *= PARTICLE_SPARK_SCALE_FACTOR;
			g_ParticleSpark[i].scl.z *= PARTICLE_SPARK_SCALE_FACTOR;

			if (g_ParticleSpark[i].origin && g_ParticleSpark[i].life > PARTICLE_SPARK_GENERATE_FRAME)
			{
				SetParticleSpark(g_ParticleSpark[i].basePos, g_ParticleSpark[i].move, PARTICLE_SPARK_LIFE, FALSE);
			}

			g_ParticleSpark[i].life--;
			if (g_ParticleSpark[i].life <= 0)
			{
				ResetParticleSpark(i);
			}
		}
	}





#ifdef _DEBUG
	PrintDebugProc("[ParticleSpark]\n");
	PrintDebugProc("CNT: %d\n", psCnt);
	//PrintDebugProc("\n", cnt);
#endif
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawParticleSpark(void)
{
	XMMATRIX mtxScl, mtxTranslate, mtxWorld, mtxView;
	CAMERA *camera = GetCamera();

	// ���C�e�B���O����
	SetLightEnable(FALSE);

	// ���Z����
	SetBlendState(BLEND_MODE_ADD);

	// Z��r����
	SetDepthEnable(FALSE);

	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// �v���~�e�B�u�g�|���W�ݒ�
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// �e�N�X�`���ݒ�
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_TexNo]);

	for(int i = 0; i < PARTICLE_SPARK_MAX; i++)
	{
		if (g_ParticleSpark[i].use)
		{
			// ���[���h�}�g���N�X������
			mtxWorld = XMMatrixIdentity();

			// �r���[�}�g���N�X�擾
			mtxView = XMLoadFloat4x4(&camera->mtxView);

			// �ȈՓI�ɋt�s��ɂ���
			// XMMatrixInverse()��荂��
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
			mtxScl = XMMatrixScaling(g_ParticleSpark[i].scl.x, g_ParticleSpark[i].scl.y, g_ParticleSpark[i].scl.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

			// �ړ��𔽉f
			mtxTranslate = XMMatrixTranslation(g_ParticleSpark[i].pos.x, g_ParticleSpark[i].pos.y, g_ParticleSpark[i].pos.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

			// ���[���h�}�g���N�X�ݒ�
			SetWorldMatrix(&mtxWorld);

			// ���[���h�}�g���N�X�ۑ�
			XMStoreFloat4x4(&g_ParticleSpark[i].mtxWorld, mtxWorld);

			// �}�e���A���ݒ�
			SetMaterial(g_ParticleSpark[i].material);

			// �|���S���̕`��
			GetDeviceContext()->Draw(4, 0);
		}
	}

	// ���C�e�B���O�L��
	SetLightEnable(TRUE);

	// �A���t�@����
	SetBlendState(BLEND_MODE_ALPHABLEND);

	// Z��r�L��
	SetDepthEnable(TRUE);
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
HRESULT MakeVertexParticleSpark(void)
{
	// ���_�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);
	
	//���_�o�b�t�@�̒��g�𖄂߂�
	{
		D3D11_MAPPED_SUBRESOURCE msr;
		GetDeviceContext()->Map(g_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

		// ���_���W�̐ݒ�
		vertex[0].Position = { -PARTICLE_SPARK_SIZE_X / 2,  PARTICLE_SPARK_SIZE_Y / 2, 0.0f };
		vertex[1].Position = {  PARTICLE_SPARK_SIZE_X / 2,  PARTICLE_SPARK_SIZE_Y / 2, 0.0f };
		vertex[2].Position = { -PARTICLE_SPARK_SIZE_X / 2, -PARTICLE_SPARK_SIZE_Y / 2, 0.0f };
		vertex[3].Position = {  PARTICLE_SPARK_SIZE_X / 2, -PARTICLE_SPARK_SIZE_Y / 2, 0.0f };

		// �m�[�}���̐ݒ�
		vertex[0].Normal = { 0.0f, 0.0f, -1.0f };
		vertex[1].Normal = { 0.0f, 0.0f, -1.0f };
		vertex[2].Normal = { 0.0f, 0.0f, -1.0f };
		vertex[3].Normal = { 0.0f, 0.0f, -1.0f };

		// �f�B�t�[�Y�̐ݒ�
		vertex[0].Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
		vertex[1].Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
		vertex[2].Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
		vertex[3].Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };

		// �e�N�X�`�����W�̐ݒ�
		vertex[0].TexCoord = { 0.0f, 0.0f };
		vertex[1].TexCoord = { 1.0f, 0.0f };
		vertex[2].TexCoord = { 0.0f, 1.0f };
		vertex[3].TexCoord = { 1.0f, 1.0f };

		GetDeviceContext()->Unmap(g_VertexBuffer, 0);
	}

	return S_OK;
}

//=============================================================================
// �X�p�[�N�p�[�e�B�N����ݒ�
//=============================================================================
int SetParticleSpark(XMFLOAT3 pos, XMFLOAT3 move, int life, BOOL origin)
{
	int idx = -1;

	for(int i = 0; i < PARTICLE_SPARK_MAX; i++)
	{
		if (!g_ParticleSpark[i].use)
		{
			g_ParticleSpark[i].scl = { 1.0f, 1.0f, 1.0f };
			g_ParticleSpark[i].rot = { 0.0f, 0.0f, 0.0f };
			g_ParticleSpark[i].pos = pos;
			g_ParticleSpark[i].basePos = pos;
			g_ParticleSpark[i].move = move;
			g_ParticleSpark[i].spd = PARTICLE_SPARK_SPD + (float)(rand() % 50 - 25) * 0.01f;
			g_ParticleSpark[i].material.Diffuse = { PARTICLE_SPARK_COLOR_R, PARTICLE_SPARK_COLOR_G, PARTICLE_SPARK_COLOR_B, PARTICLE_SPARK_COLOR_A };
			g_ParticleSpark[i].life = life;
			g_ParticleSpark[i].origin = origin;
			g_ParticleSpark[i].use = TRUE;

			idx = i;

			break;
		}
	}

	return idx;
}

//=============================================================================
// �X�p�[�N�p�[�e�B�N���̈ړ��������v�Z
//=============================================================================
XMFLOAT3 CalcParticleSparkMove(XMFLOAT3 hitPos)
{
	float theta = (float)((rand() % 628 - 314) * 0.01f);
	float phi = (float)((rand() % 628 - 314) * 0.01f);

	float moveX = sinf(phi) * cosf(theta);
	float moveY = cosf(phi);
	float moveZ = sinf(phi) * sinf(theta);
	XMFLOAT3 move = { moveX, moveY, moveZ };
	XMVECTOR vMove = XMVector3Normalize(XMLoadFloat3(&move));
	XMStoreFloat3(&move, vMove);

	return move;
}

//=============================================================================
// �X�p�[�N�p�[�e�B�N���̍ď�����
//=============================================================================
void ResetParticleSpark(int idx)
{
	XMStoreFloat4x4(&g_ParticleSpark[idx].mtxWorld, XMMatrixIdentity());

	g_ParticleSpark[idx].scl = { 1.0f, 1.0f, 1.0f };
	g_ParticleSpark[idx].rot = { 0.0f, 0.0f, 0.0f };
	g_ParticleSpark[idx].pos = { 0.0f, 0.0f, 0.0f };
	g_ParticleSpark[idx].basePos = { 0.0f, 0.0f, 0.0f };

	ZeroMemory(&g_ParticleSpark[idx].material, sizeof(g_ParticleSpark[idx].material));
	g_ParticleSpark[idx].material.Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };

	g_ParticleSpark[idx].move = { 0.0f, 0.0f, 0.0f };
	g_ParticleSpark[idx].spd = 0.0f;

	g_ParticleSpark[idx].life = 0;
	g_ParticleSpark[idx].origin = FALSE;

	g_ParticleSpark[idx].use = FALSE;
}
