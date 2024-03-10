//=============================================================================
//
// �p�[�e�B�N������ [particle.cpp]
// Author : HAL���� ������
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "input.h"
#include "camera.h"
#include "model.h"
#include "shadow.h"
#include "light.h"
#include "particle.h"
#include "player.h"
#include "debugproc.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_MAX			(1)			// �e�N�X�`���̐�

#define	PARTICLE_SIZE_X		(40.0f)		// ���_�T�C�Y
#define	PARTICLE_SIZE_Y		(40.0f)		// ���_�T�C�Y
#define	PARTICLE_SIZE		(20.0f)		// ���_�T�C�Y
#define	VALUE_MOVE_PARTICLE	(5.0f)		// �ړ����x


//#define	DISP_SHADOW						// �e�̕\��
#undef DISP_SHADOW

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexParticle(void);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer* g_VertexBuffer = NULL;		// ���_�o�b�t�@

static ID3D11ShaderResourceView* g_Texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����
static int							g_TexNo;					// �e�N�X�`���ԍ�

static PARTICLE					g_aParticle[MAX_PARTICLE];		// �p�[�e�B�N�����[�N
static XMFLOAT3					g_posBase;						// �r���{�[�h�����ʒu
static float					g_fWidthBase = 1.0f;			// ��̕�
static float					g_fHeightBase = 10.0f;			// ��̍���
static float					g_roty = 0.0f;					// �ړ�����
static float					g_spd = 0.0f;					// �ړ��X�s�[�h

static char* g_TextureName[] =
{
	"data/TEXTURE/effect000.jpg",
};

static BOOL							g_Load = FALSE;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitParticle(void)
{
	// ���_���̍쐬
	MakeVertexParticle();

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
	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		g_aParticle[nCntParticle].pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_aParticle[nCntParticle].rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
		g_aParticle[nCntParticle].scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
		g_aParticle[nCntParticle].move = XMFLOAT3(1.0f, 1.0f, 1.0f);
		g_aParticle[nCntParticle].angle = 0.0f;
		g_aParticle[nCntParticle].radius = 0.0f;
		g_aParticle[nCntParticle].basepos = XMFLOAT3(0.0f, 0.0f, 0.0f);

		ZeroMemory(&g_aParticle[nCntParticle].material, sizeof(g_aParticle[nCntParticle].material));
		g_aParticle[nCntParticle].material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

		g_aParticle[nCntParticle].fSizeX = PARTICLE_SIZE;
		g_aParticle[nCntParticle].fSizeY = PARTICLE_SIZE;
		g_aParticle[nCntParticle].nIdxShadow = -1;
		g_aParticle[nCntParticle].nLife = 0;
		g_aParticle[nCntParticle].type = 0;
		g_aParticle[nCntParticle].use = FALSE;
		g_aParticle[nCntParticle].brendmode = BLEND_MODE_NONE;
	}


	g_posBase = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_roty = 0.0f;
	g_spd = 0.0f;

	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitParticle(void)
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
void UpdateParticle(void)
{
		int particleCnt = 0;
	{

		for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
		{
			if (g_aParticle[nCntParticle].use)
			{// �g�p��
				g_aParticle[nCntParticle].pos.x += g_aParticle[nCntParticle].move.x;
				g_aParticle[nCntParticle].pos.z += g_aParticle[nCntParticle].move.z;

				g_aParticle[nCntParticle].pos.y += g_aParticle[nCntParticle].move.y;


#ifdef DISP_SHADOW
				if (g_aParticle[nCntParticle].nIdxShadow != -1)
				{// �e�g�p��
					float colA;

					// �e�̈ʒu�ݒ�
					SetPositionShadow(g_aParticle[nCntParticle].nIdxShadow, XMFLOAT3(g_aParticle[nCntParticle].pos.x, 0.1f, g_aParticle[nCntParticle].pos.z));

					// �e�̐F�̐ݒ�
					colA = g_aParticle[nCntParticle].material.Diffuse.w;
					SetColorShadow(g_aParticle[nCntParticle].nIdxShadow, XMFLOAT4(0.5f, 0.5f, 0.5f, colA));
				}
#endif

				g_aParticle[nCntParticle].nLife--;

				if (g_aParticle[nCntParticle].nLife <= 0)
				{
					// ��������
					g_aParticle[nCntParticle].use = FALSE;
					g_aParticle[nCntParticle].angle = 0.0f;
					g_aParticle[nCntParticle].radius = 0.0f;

					ReleaseShadow(g_aParticle[nCntParticle].nIdxShadow);
					g_aParticle[nCntParticle].nIdxShadow = -1;
				}
				else
				{
					if (g_aParticle[nCntParticle].nLife <= 80)
					{

						if (g_aParticle[nCntParticle].scale.x > 0.0f)
						{
							switch (g_aParticle[nCntParticle].type)
							{
							case TYPE_HINOKO_RED:
							case TYPE_HINOKO_BLUE:
							case TYPE_CHANGE_PLAYER:
								g_aParticle[nCntParticle].scale.x -= 0.025f;
								g_aParticle[nCntParticle].scale.y -= 0.025f;
								g_aParticle[nCntParticle].scale.z -= 0.025f;
								break;

							case TYPE_HINOKO_GREEN:
							case TYPE_HINOKO_YELLOW:
								break;

							case TYPE_HINOKO_CHARGE_GREEN:	// ����
								g_aParticle[nCntParticle].angle += 1.0f;

								if (g_aParticle[nCntParticle].angle >= XM_PI)
								{
									g_aParticle[nCntParticle].angle -= XM_2PI;
								}
									g_aParticle[nCntParticle].radius += 0.5f;

								// �����̐i�s���v�Z
								g_aParticle[nCntParticle].pos.x = g_aParticle[nCntParticle].basepos.x + 
									g_aParticle[nCntParticle].radius * sinf(g_aParticle[nCntParticle].angle);
								g_aParticle[nCntParticle].pos.x += (float)(rand() % 3);

								g_aParticle[nCntParticle].pos.z = g_aParticle[nCntParticle].basepos.z + 
									g_aParticle[nCntParticle].radius * cosf(g_aParticle[nCntParticle].angle);

								break;

							case TYPE_HINOKO_CHARGE_YELLOW:	// ��

								if (g_aParticle[nCntParticle].pos.y <= g_aParticle[nCntParticle].fSizeY / 8)
								{// ���n����

									g_aParticle[nCntParticle].pos.y = g_aParticle[nCntParticle].fSizeY / 8;
									g_aParticle[nCntParticle].move.y = 0.0f;
									g_aParticle[nCntParticle].radius += 1.0f;
									g_aParticle[nCntParticle].scale = { 0.1f, 0.1f, 0.1f };

									// �����̐i�s���v�Z
									g_aParticle[nCntParticle].pos.x = g_aParticle[nCntParticle].basepos.x +
										g_aParticle[nCntParticle].radius * sinf(g_aParticle[nCntParticle].angle);
									g_aParticle[nCntParticle].pos.x += (float)(rand() % 7);

									g_aParticle[nCntParticle].pos.z = g_aParticle[nCntParticle].basepos.z +
										g_aParticle[nCntParticle].radius * cosf(g_aParticle[nCntParticle].angle);
									g_aParticle[nCntParticle].pos.z += (float)(rand() % 7);


								}
								else	// �L����
								{
									g_aParticle[nCntParticle].radius = 0.5f;

									// �����̐i�s���v�Z
									g_aParticle[nCntParticle].pos.x = g_aParticle[nCntParticle].basepos.x +
										g_aParticle[nCntParticle].radius;

									g_aParticle[nCntParticle].pos.x += (float)(rand() % 3);
									g_aParticle[nCntParticle].pos.z = g_aParticle[nCntParticle].basepos.z +
										g_aParticle[nCntParticle].radius;

								}

								break;

							case TYPE_SNOW:	// ��
								g_aParticle[nCntParticle].scale.x -= 0.001f;
								g_aParticle[nCntParticle].scale.y -= 0.001f;
								g_aParticle[nCntParticle].scale.z -= 0.001f;
								// ���l�ݒ�
								g_aParticle[nCntParticle].material.Diffuse.w -= 0.01f;
								if (g_aParticle[nCntParticle].material.Diffuse.w < 0.0f)
								{
									g_aParticle[nCntParticle].material.Diffuse.w = 0.0f;
								}

								if (g_aParticle[nCntParticle].pos.y <= 5.0f)
								{
									g_aParticle[nCntParticle].move.x = 0.0f;
									g_aParticle[nCntParticle].move.z = 0.0f;

								}

								break;
							}

						}
						else
						{
							g_aParticle[nCntParticle].use = FALSE;
						}
					}
				}

				particleCnt++;
			}
		}



		// �p�[�e�B�N������(��)
		{
			XMFLOAT3 pos;
			XMFLOAT3 move;
			float fAngle, fLength;
			int nLife;
			float fSize;

			pos = g_posBase;

			fAngle = (float)(rand() % 628 - 314) / 100.0f;
			fLength = rand() % (int)(g_fWidthBase * 200) / 100.0f - g_fWidthBase;

			move.x = sinf(fAngle) * fLength;
			move.y = -0.5f;
			move.z = cosf(fAngle) * fLength;

			nLife = rand() % 300 + 100;

			fSize = (float)(rand() % 30 + 10);

			pos.x = (float)(rand() % 800 - 400);
			pos.y = 200.0f;
			pos.z = (float)(rand() % 800 - 400);

			//�r���{�[�h�̐ݒ�
			SetParticle(pos, XMFLOAT3(0.1f, 0.1f, 0.1f), move, XMFLOAT4(1.0f, 1.0f, 1.0f, 0.85f), 
				TYPE_SNOW, BLEND_MODE_SUBTRACT, fSize, fSize, nLife);
		}
	}

#ifdef _DEBUG
	// �f�o�b�O�\��
	PrintDebugProc("particleCnt %d\n", particleCnt);
#endif

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawParticle(void)
{
	XMMATRIX mtxScl, mtxTranslate, mtxWorld, mtxView;
	CAMERA* cam = GetCamera();

	// ���C�e�B���O�𖳌���
	SetLightEnable(FALSE);

	// Z��r����
	SetDepthEnable(FALSE);

	// �t�H�O����
	SetFogEnable(FALSE);

	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// �v���~�e�B�u�g�|���W�ݒ�
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// �e�N�X�`���ݒ�
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_TexNo]);

	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		if (g_aParticle[nCntParticle].use)
		{
			// ���Z�����ɐݒ�
			SetBlendState(g_aParticle[nCntParticle].brendmode);

			// ���[���h�}�g���b�N�X�̏�����
			mtxWorld = XMMatrixIdentity();

			// �r���[�}�g���b�N�X���擾
			mtxView = XMLoadFloat4x4(&cam->mtxView);


			// ������������������
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
			mtxScl = XMMatrixScaling(g_aParticle[nCntParticle].scale.x, g_aParticle[nCntParticle].scale.y, g_aParticle[nCntParticle].scale.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxScl);

			// �ړ��𔽉f
			mtxTranslate = XMMatrixTranslation(g_aParticle[nCntParticle].pos.x, g_aParticle[nCntParticle].pos.y, g_aParticle[nCntParticle].pos.z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

			// ���[���h�}�g���b�N�X�̐ݒ�
			SetWorldMatrix(&mtxWorld);

			// �}�e���A���ݒ�
			SetMaterial(g_aParticle[nCntParticle].material);

			// �|���S���̕`��
			GetDeviceContext()->Draw(4, 0);
		}
	}

	// ���C�e�B���O��L����
	SetLightEnable(TRUE);

	// �ʏ�u�����h�ɖ߂�
	SetBlendState(BLEND_MODE_ALPHABLEND);

	// Z��r�L��
	SetDepthEnable(TRUE);

	// �t�H�O�L��
	SetFogEnable(GetFogEnable());

}

//=============================================================================
// ���_���̍쐬
//=============================================================================
HRESULT MakeVertexParticle(void)
{
	// ���_�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);

	{//���_�o�b�t�@�̒��g�𖄂߂�
		D3D11_MAPPED_SUBRESOURCE msr;
		GetDeviceContext()->Map(g_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

		// ���_���W�̐ݒ�
		vertex[0].Position = XMFLOAT3(-PARTICLE_SIZE_X / 2, PARTICLE_SIZE_Y / 2, 0.0f);
		vertex[1].Position = XMFLOAT3(PARTICLE_SIZE_X / 2, PARTICLE_SIZE_Y / 2, 0.0f);
		vertex[2].Position = XMFLOAT3(-PARTICLE_SIZE_X / 2, -PARTICLE_SIZE_Y / 2, 0.0f);
		vertex[3].Position = XMFLOAT3(PARTICLE_SIZE_X / 2, -PARTICLE_SIZE_Y / 2, 0.0f);

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
void SetColorParticle(int nIdxParticle, XMFLOAT4 col)
{
	g_aParticle[nIdxParticle].material.Diffuse = col;
}

//=============================================================================
// �p�[�e�B�N���̔�������
//=============================================================================
int SetParticle(XMFLOAT3 pos, XMFLOAT3 scale, XMFLOAT3 move, XMFLOAT4 col, int type, BLEND_MODE brendmode, float fSizeX, float fSizeY, int nLife)
{
	int nIdxParticle = -1;

	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		if (!g_aParticle[nCntParticle].use)
		{
			g_aParticle[nCntParticle].pos = pos;
			g_aParticle[nCntParticle].rot = { 0.0f, 0.0f, 0.0f };
			g_aParticle[nCntParticle].scale = scale;
			g_aParticle[nCntParticle].move = move;
			g_aParticle[nCntParticle].material.Diffuse = col;
			g_aParticle[nCntParticle].RGBa = col;
			g_aParticle[nCntParticle].fSizeX = fSizeX;
			g_aParticle[nCntParticle].fSizeY = fSizeY;
			g_aParticle[nCntParticle].nLife = nLife;
			g_aParticle[nCntParticle].type = type;
			g_aParticle[nCntParticle].use = TRUE;
			g_aParticle[nCntParticle].brendmode = brendmode;

			nIdxParticle = nCntParticle;

#ifdef DISP_SHADOW
			// �e�̐ݒ�
			g_aParticle[nCntParticle].nIdxShadow = CreateShadow(XMFLOAT3(pos.x, 0.1f, pos.z), 0.8f, 0.8f);		// �e�̐ݒ�
			if (g_aParticle[nCntParticle].nIdxShadow != -1)
			{
				SetColorShadow(g_aParticle[nCntParticle].nIdxShadow, XMFLOAT4(1.0f, 1.0f, 1.0f, 0.5f));
			}
#endif

			break;
		}
	}

	return nIdxParticle;
}

// �����p
int SetParticle(XMFLOAT3 pos, XMFLOAT3 scale, XMFLOAT3 move, XMFLOAT4 col, int type, BLEND_MODE brendmode, float fSizeX, float fSizeY, int nLife, XMFLOAT3 basepos, float angle)
{
	int nIdxParticle = -1;

	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		if (!g_aParticle[nCntParticle].use)
		{
			g_aParticle[nCntParticle].pos = pos;
			g_aParticle[nCntParticle].rot = { 0.0f, 0.0f, 0.0f };
			g_aParticle[nCntParticle].scale = scale;
			g_aParticle[nCntParticle].move = move;
			g_aParticle[nCntParticle].basepos = basepos;
			g_aParticle[nCntParticle].angle = angle;
			g_aParticle[nCntParticle].material.Diffuse = col;
			g_aParticle[nCntParticle].RGBa = col;
			g_aParticle[nCntParticle].fSizeX = fSizeX;
			g_aParticle[nCntParticle].fSizeY = fSizeY;
			g_aParticle[nCntParticle].nLife = nLife;
			g_aParticle[nCntParticle].type = type;
			g_aParticle[nCntParticle].use = TRUE;
			g_aParticle[nCntParticle].brendmode = brendmode;

			nIdxParticle = nCntParticle;

#ifdef DISP_SHADOW
			// �e�̐ݒ�
			g_aParticle[nCntParticle].nIdxShadow = CreateShadow(XMFLOAT3(pos.x, 0.1f, pos.z), 0.8f, 0.8f);		// �e�̐ݒ�
			if (g_aParticle[nCntParticle].nIdxShadow != -1)
			{
				SetColorShadow(g_aParticle[nCntParticle].nIdxShadow, XMFLOAT4(1.0f, 1.0f, 1.0f, 0.5f));
			}
#endif

			break;
		}
	}

	return nIdxParticle;
}

//=============================================================================
// �p�[�e�B�N�������擾
//=============================================================================
PARTICLE* GetParticle(void)
{
	return &g_aParticle[0];
}
