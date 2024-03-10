//=============================================================================
//
// �`���[�W�p�[�e�B�N������ [particle_charge.cpp]
// Author : HAL���� ������
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "input.h"
#include "camera.h"
#include "light.h"
#include "particle_charge.h"
#include "player.h"
#include "debugproc.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_MAX					(1)			// �e�N�X�`���̐�
#define	MAX_PARTICLE_CHARGE			(1028)		// �p�[�e�B�N���ő吔
#define	PARTICLE_CHARGE_SIZE_X		(40.0f)		// ���_�T�C�Y
#define	PARTICLE_CHARGE_SIZE_Y		(40.0f)		// ���_�T�C�Y


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexParticleCharge(void);


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer*				g_VertexBuffer = NULL;					// ���_�o�b�t�@
static ID3D11ShaderResourceView*	g_Texture[TEXTURE_MAX] = { NULL };		// �e�N�X�`�����
static int							g_TexNo;								// �e�N�X�`���ԍ�

static PARTICLE_CHARGE				g_ParticleCharge[MAX_PARTICLE_CHARGE];				// �p�[�e�B�N�����[�N

static char* g_TextureName[] =
{
	"data/TEXTURE/effect000.jpg",
};

static BOOL			g_Load = FALSE;


//=============================================================================
// ����������
//=============================================================================
HRESULT InitParticleCharge(void)
{
	// ���_���̍쐬
	MakeVertexParticleCharge();

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

	// �p�[�e�B�N�����[�N�̏�����
	for (int i = 0; i < MAX_PARTICLE_CHARGE; i++)
	{
		g_ParticleCharge[i].pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_ParticleCharge[i].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_ParticleCharge[i].scl = XMFLOAT3(1.0f, 1.0f, 1.0f);
		g_ParticleCharge[i].move = XMFLOAT3(1.0f, 1.0f, 1.0f);

		ZeroMemory(&g_ParticleCharge[i].material, sizeof(g_ParticleCharge[i].material));
		g_ParticleCharge[i].material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

		g_ParticleCharge[i].life = 0;
		g_ParticleCharge[i].use = FALSE;
	}

	g_Load = TRUE;

	return S_OK;
}


//=============================================================================
// �I������
//=============================================================================
void UninitParticleCharge(void)
{
	if (g_Load == FALSE) return;

	//�e�N�X�`���̉��
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		if (g_Texture[i] != NULL)
		{
			g_Texture[i]->Release();
			g_Texture[i] = NULL;
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
void UpdateParticleCharge(void)
{
	PLAYER* player = GetPlayer();

	int pcCnt = 0;

	for (int i = 0; i < MAX_PARTICLE_CHARGE; i++)
	{
		// �g�p��
		if (g_ParticleCharge[i].use == FALSE) continue;

		g_ParticleCharge[i].ePos = player->pos;

		// �ړ�����
		{
			XMVECTOR nPos = XMLoadFloat3(&g_ParticleCharge[i].sPos);	// XMVECTOR�֕ϊ�
			XMVECTOR Pos = XMLoadFloat3(&g_ParticleCharge[i].ePos) - nPos;	// ���W���v�Z���Ă���
			Pos *= g_ParticleCharge[i].lerpTime;
			// �v�Z���ċ��߂����W�����݂̈ړ��e�[�u���ɑ����Ă���
			XMStoreFloat3(&g_ParticleCharge[i].pos, nPos + Pos);
			g_ParticleCharge[i].lerpTime += 1.0f / g_ParticleCharge[i].life;
		}

		// �c�����Ԃ𒴂�����p�[�e�B�N���𖢎g�p��Ԃɂ���
		{
			g_ParticleCharge[i].life--;
			if (g_ParticleCharge[i].life <= 0)
			{
				g_ParticleCharge[i].use = FALSE;
			}
			else
			{
				if (g_ParticleCharge[i].life <= 80)
				{
					float colorX = g_ParticleCharge[i].RGBa.x;
					float colorY = g_ParticleCharge[i].RGBa.y;
					float colorZ = g_ParticleCharge[i].RGBa.z;
					g_ParticleCharge[i].material.Diffuse.x = colorX - (float)(80 - g_ParticleCharge[i].life) / 80 * colorX;
					g_ParticleCharge[i].material.Diffuse.y = colorY - (float)(80 - g_ParticleCharge[i].life) / 80 * colorY;
					g_ParticleCharge[i].material.Diffuse.z = colorZ - (float)(80 - g_ParticleCharge[i].life) / 80 * colorZ;
				}

				// �c�����Ԃ�20�t���[���ȉ��̏ꍇ�A���l�������Ă���
				if (g_ParticleCharge[i].life <= 20)
				{
					// ���l�ݒ�
					g_ParticleCharge[i].material.Diffuse.w -= 0.05f;
					if (g_ParticleCharge[i].material.Diffuse.w < 0.0f)
					{
						g_ParticleCharge[i].material.Diffuse.w = 0.0f;
					}
				}
			}
		}

		pcCnt++;
	}

#ifdef _DEBUG
	// �f�o�b�O�\��
	PrintDebugProc("pcCnt %d\n", pcCnt);
#endif


}


//=============================================================================
// �`�揈��
//=============================================================================
void DrawParticleCharge(void)
{
	XMMATRIX mtxScl, mtxTranslate, mtxWorld, mtxView;
	CAMERA* cam = GetCamera();

	SetLightEnable(FALSE);				// ���C�e�B���O�𖳌���
	SetBlendState(BLEND_MODE_ADD);		// ���Z�����ɐݒ�
	SetDepthEnable(FALSE);				// Z��r����
	SetFogEnable(FALSE);				// �t�H�O����

	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// �v���~�e�B�u�g�|���W�ݒ�
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// �e�N�X�`���ݒ�
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_TexNo]);

	for (int i = 0; i < MAX_PARTICLE_CHARGE; i++)
	{
		if (g_ParticleCharge[i].use)
		{
			// ���[���h�}�g���b�N�X�̏�����
			mtxWorld = XMMatrixIdentity();

			// �r���[�}�g���b�N�X���擾
			mtxView = XMLoadFloat4x4(&cam->mtxView);
			if (GetViewPortType() == TYPE_LEFT_HALF_SCREEN) mtxView = XMLoadFloat4x4(&cam[0].mtxView);
			if (GetViewPortType() == TYPE_RIGHT_HALF_SCREEN) mtxView = XMLoadFloat4x4(&cam[1].mtxView);
			if (GetViewPortType() == TYPE_UP_HALF_SCREEN) mtxView = XMLoadFloat4x4(&cam[0].mtxView);
			if (GetViewPortType() == TYPE_DOWN_HALF_SCREEN) mtxView = XMLoadFloat4x4(&cam[1].mtxView);


			// �r���{�[�h
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
			mtxScl = XMMatrixScaling(g_ParticleCharge[i].scl.x, g_ParticleCharge[i].scl.y, g_ParticleCharge[i].scl.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

			// �ړ��𔽉f
			mtxTranslate = XMMatrixTranslation(g_ParticleCharge[i].pos.x, g_ParticleCharge[i].pos.y, g_ParticleCharge[i].pos.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

			// ���[���h�}�g���b�N�X�̐ݒ�
			SetWorldMatrix(&mtxWorld);

			// �}�e���A���ݒ�
			SetMaterial(g_ParticleCharge[i].material);

			// �|���S���̕`��
			GetDeviceContext()->Draw(4, 0);
		}
	}

	SetLightEnable(TRUE);						// ���C�e�B���O��L����
	SetBlendState(BLEND_MODE_ALPHABLEND);		// �ʏ�u�����h�ɖ߂�
	SetDepthEnable(TRUE);						// Z��r�L��
	SetFogEnable(GetFogEnable());				// �t�H�O�L��
}


//=============================================================================
// ���_���̍쐬
//=============================================================================
HRESULT MakeVertexParticleCharge(void)
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
		vertex[0].Position = XMFLOAT3(-PARTICLE_CHARGE_SIZE_X / 2,  PARTICLE_CHARGE_SIZE_Y / 2, 0.0f);
		vertex[1].Position = XMFLOAT3( PARTICLE_CHARGE_SIZE_X / 2,  PARTICLE_CHARGE_SIZE_Y / 2, 0.0f);
		vertex[2].Position = XMFLOAT3(-PARTICLE_CHARGE_SIZE_X / 2, -PARTICLE_CHARGE_SIZE_Y / 2, 0.0f);
		vertex[3].Position = XMFLOAT3( PARTICLE_CHARGE_SIZE_X / 2, -PARTICLE_CHARGE_SIZE_Y / 2, 0.0f);

		// �@���̐ݒ�
		vertex[0].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
		vertex[1].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
		vertex[2].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
		vertex[3].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

		// ���ˌ��̐ݒ�
		vertex[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[2].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[3].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

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
// �}�e���A���J���[�̐ݒ�
//=============================================================================
void SetColorParticleCharge(int index, XMFLOAT4 color)
{
	g_ParticleCharge[index].material.Diffuse = color;
}

//=============================================================================
// �p�[�e�B�N���̔�������
//=============================================================================
int SetParticleCharge(XMFLOAT3 pos, XMFLOAT3 ePos, XMFLOAT3 scl, XMFLOAT4 col, int type, int life)
{
	int idx = -1;

	for (int i = 0; i < MAX_PARTICLE_CHARGE; i++)
	{
		if (g_ParticleCharge[i].use == FALSE)
		{
			g_ParticleCharge[i].pos = pos;
			g_ParticleCharge[i].scl = scl;
			g_ParticleCharge[i].material.Diffuse = col;
			g_ParticleCharge[i].type = type;
			g_ParticleCharge[i].life = life;
			g_ParticleCharge[i].RGBa = col;
			g_ParticleCharge[i].sPos = pos;
			g_ParticleCharge[i].ePos = ePos;
			g_ParticleCharge[i].lerpTime = 0.0f;
			g_ParticleCharge[i].use = TRUE;

			idx = i;

			break;
		}
	}

	return idx;
}

