//=============================================================================
//
// �J�������� [camera.cpp]
// Author : HAL���� ������
//
//=============================================================================
#include "main.h"
#include "input.h"
#include "camera.h"
#include "debugproc.h"
#include "player.h"
#include "light.h"
#include "flag.h"
#include "fade.h"
#include "sound.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	VIEW_ANGLE		(XMConvertToRadians(45.0f))						// �r���[���ʂ̎���p
#define	VIEW_ASPECT		((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT)	// �r���[���ʂ̃A�X�y�N�g��	
#define	VIEW_NEAR_Z		(10.0f)											// �r���[���ʂ�NearZ�l
#define	VIEW_FAR_Z		(10000.0f)										// �r���[���ʂ�FarZ�l

#define	VALUE_MOVE_CAMERA	(2.0f)										// �J�����̈ړ���
#define	VALUE_ROTATE_CAMERA	(XM_PI * 0.01f)								// �J�����̉�]��

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static CAMERA			g_Camera;		// �J�����f�[�^

static INTERPOLATION_DATA_CAMAERA g_MoveTbl0[] = {
	//���_									�����_								��]						����
	{ XMFLOAT3(0.0f,    7.0f, -450.0f), 	XMFLOAT3(0.0f,    7.0f, -350.0f), 	XMFLOAT3(1.0f, 1.0f, 1.0f),	120 },
	{ XMFLOAT3(0.0f,    7.0f, -450.0f), 	XMFLOAT3(0.0f,    7.0f, -350.0f), 	XMFLOAT3(1.0f, 1.0f, 1.0f),	120 },
	{ XMFLOAT3(80.0f,  30.0f, -400.0f), 	XMFLOAT3(50.0f,  50.0f, -300.0f), 	XMFLOAT3(1.0f, 1.0f, 1.0f),	60 },
	{ XMFLOAT3(80.0f,  30.0f, -400.0f), 	XMFLOAT3(50.0f,  50.0f, -300.0f), 	XMFLOAT3(1.0f, 1.0f, 1.0f),	70 },

	{ XMFLOAT3(-80.0f, 30.0f, -400.0f), 	XMFLOAT3(-50.0f, 50.0f, -300.0f), 	XMFLOAT3(1.0f, 1.0f, 1.0f),	60 },
	{ XMFLOAT3(-80.0f, 30.0f, -400.0f),		XMFLOAT3(-50.0f, 50.0f, -300.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f),	70 },

	{ XMFLOAT3(-80.0f, 30.0f, -300.0f),		XMFLOAT3(-50.0f, 50.0f, -200.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f), 60 },
	{ XMFLOAT3(-80.0f, 30.0f, -300.0f),		XMFLOAT3(-50.0f, 50.0f, -200.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f),	70 },

	{ XMFLOAT3(80.0f, 30.0f, -300.0f),		XMFLOAT3(50.0f, 50.0f, -200.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f),	60 },
	{ XMFLOAT3(80.0f, 30.0f, -300.0f),		XMFLOAT3(50.0f, 50.0f, -200.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f),	70 },


	{ XMFLOAT3(200.0f, 70.0f, -300.0f),		XMFLOAT3(50.0f, 50.0f, -200.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f),	420 },
	{ XMFLOAT3(200.0f, 70.0f, -300.0f),		XMFLOAT3(50.0f, 50.0f, -200.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f),	120 },
	{ XMFLOAT3(200.0f, 70.0f, -300.0f),		XMFLOAT3(50.0f, 50.0f, -200.0f),	XMFLOAT3(1.0f, 1.0f, 1.0f),	120 },

	{ XMFLOAT3(0.0f,   25.0f, -450.0f), 	XMFLOAT3(0.0f,   7.0f, -350.0f), 	XMFLOAT3(1.0f, 1.0f, 1.0f),	120 },
};

static INTERPOLATION_DATA_CAMAERA* g_standbyTblAdr[] =
{
	g_MoveTbl0,
};

static int				g_ViewPortType = TYPE_FULL_SCREEN;

static float			g_TargetRotY;

//=============================================================================
// ����������
//=============================================================================
void InitCamera(void)
{
	g_Camera.pos = { POS_X_CAM, POS_Y_CAM, POS_Z_CAM };
	g_Camera.at  = { 0.0f, 0.0f, 0.0f };
	g_Camera.up  = { 0.0f, 1.0f, 0.0f };
	g_Camera.rot = { 0.0f, 0.0f, 0.0f };

	g_Camera.state = CAMERA_STATE_NONE;

	// ���_�ƒ����_�̋������v�Z
	float vx, vz;
	vx = g_Camera.pos.x - g_Camera.at.x;
	vz = g_Camera.pos.z - g_Camera.at.z;
	g_Camera.len = sqrtf(vx * vx + vz * vz);
	
	// �r���[�|�[�g�^�C�v�̏�����
	SetViewPort(g_ViewPortType);

	// 0�Ԃ������`��Ԃœ������Ă݂�
	g_Camera.time = 0.0f;		// ���`��ԗp�̃^�C�}�[���N���A
	g_Camera.tblNo = 0;		// �Đ�����A�j���f�[�^�̐擪�A�h���X���Z�b�g
	g_Camera.tblMax = sizeof(g_MoveTbl0) / sizeof(INTERPOLATION_DATA_CAMAERA);	// �Đ�����A�j���f�[�^�̃��R�[�h�����Z�b�g

}


//=============================================================================
// �J�����̏I������
//=============================================================================
void UninitCamera(void)
{

}


//=============================================================================
// �J�����̍X�V����
//=============================================================================
void UpdateCamera(void)
{
	if (g_Camera.state == CAMERA_STATE_NONE)
	{
		// ����](QuickTurn)������
		if (GetKeyboardTrigger(DIK_K) || IsButtonTriggered(0, BUTTON_RS))
		{
			g_Camera.state = CAMERA_STATE_QUICKTURN;

			g_TargetRotY = g_Camera.rot.y + XM_PI;

			if (g_TargetRotY > XM_PI)
			{
				g_TargetRotY -= XM_2PI;
			}

		}

		// ���_����u���v
		if (GetKeyboardPress(DIK_L) || IsButtonPressed(0, STICK_R_RIGHT))
		{
			g_Camera.rot.y += VALUE_ROTATE_CAMERA;
			if (g_Camera.rot.y > XM_PI)
			{
				g_Camera.rot.y -= XM_PI * 2.0f;
			}

			g_Camera.pos.x = g_Camera.at.x - sinf(g_Camera.rot.y) * g_Camera.len;
			g_Camera.pos.z = g_Camera.at.z - cosf(g_Camera.rot.y) * g_Camera.len;
		}

		// ���_����u�E�v
		if (GetKeyboardPress(DIK_J) || IsButtonPressed(0, STICK_R_LEFT))
		{
			g_Camera.rot.y -= VALUE_ROTATE_CAMERA;
			if (g_Camera.rot.y < -XM_PI)
			{
				g_Camera.rot.y += XM_PI * 2.0f;
			}

			g_Camera.pos.x = g_Camera.at.x - sinf(g_Camera.rot.y) * g_Camera.len;
			g_Camera.pos.z = g_Camera.at.z - cosf(g_Camera.rot.y) * g_Camera.len;
		}


	}
	else if (g_Camera.state == CAMERA_STATE_QUICKTURN)
	{
			float diff = fabsf(g_TargetRotY - g_Camera.rot.y);	// �}�C�i�X�������֐��i��Βl�j

			g_Camera.rot.y += diff * 0.2f;

			if (g_Camera.rot.y > XM_PI)
			{
				g_Camera.rot.y -= XM_PI * 2.0f;
			}

			if (diff < 0.05f)
			{
				g_Camera.state = CAMERA_STATE_NONE;
			}
		
	}

	// �C�x���g�V�[��
	if (GetFlag() == FLAG_GAME_EVENT_FIRE)
	{
		{
			XMFLOAT3 pos = g_Camera.pos;
			XMFLOAT3 at = g_Camera.at;
			g_MoveTbl0[0].pos = pos;
			g_MoveTbl0[0].at = at;

		}

		if (g_Camera.tblMax > 0)	// ���`��Ԃ����s����H
		{
			// ���`��Ԃ̏���
			int nowNo = (int)g_Camera.time;			// �������ł���e�[�u���ԍ������o���Ă���
			int maxNo = g_Camera.tblMax;				// �o�^�e�[�u�����𐔂��Ă���
			int nextNo = (nowNo + 1) % maxNo;			// �ړ���e�[�u���̔ԍ������߂Ă���

			INTERPOLATION_DATA_CAMAERA* tbl = g_standbyTblAdr[g_Camera.tblNo];	// �s���e�[�u���̃A�h���X���擾

			XMVECTOR nowPos = XMLoadFloat3(&tbl[nowNo].pos);	// XMVECTOR�֕ϊ�
			XMVECTOR nowAt = XMLoadFloat3(&tbl[nowNo].at);	// XMVECTOR�֕ϊ�

			XMVECTOR Pos = XMLoadFloat3(&tbl[nextNo].pos) - nowPos;	// XYZ�ړ��ʂ��v�Z���Ă���
			XMVECTOR At = XMLoadFloat3(&tbl[nextNo].at) - nowAt;	// XYZ�ړ��ʂ��v�Z���Ă���

			float nowTime = g_Camera.time - nowNo;	// ���ԕ����ł��鏭�������o���Ă���

			Pos *= nowTime;								// ���݂̈ړ��ʂ��v�Z���Ă���
			At *= nowTime;								// ���݂̈ړ��ʂ��v�Z���Ă���

			// �v�Z���ċ��߂��ړ��ʂ����݂̈ړ��e�[�u��XYZ�ɑ����Ă��遁�\�����W�����߂Ă���
			XMStoreFloat3(&g_Camera.pos, nowPos + Pos);
			XMStoreFloat3(&g_Camera.at, nowAt + At);

			// frame���g�Ď��Ԍo�ߏ���������
			g_Camera.time += 1.0f / tbl[nowNo].frame;	// ���Ԃ�i�߂Ă���

			if ((int)g_Camera.time >= maxNo -1)			// �o�^�e�[�u���Ō�܂ňړ��������H
			{
				SetFlag(FLAG_GAME_EVENT_PILLAR);
				PlaySound(SOUND_LABEL_SE_PILLAR_DOWN);
				g_Camera.tblMax = -1;
				g_Camera.pos.y = POS_Y_CAM; // �J�����̍��W�����ɖ߂��Ă���
			}
		}
	}
#ifdef _DEBUG

	if (GetKeyboardPress(DIK_T))
	{// �����_�ړ��u��v
		g_Camera.at.y += VALUE_MOVE_CAMERA;
	}

	if (GetKeyboardPress(DIK_B))
	{// �����_�ړ��u���v
		g_Camera.at.y -= VALUE_MOVE_CAMERA;
	}

	if (GetKeyboardPress(DIK_U))
	{// �߂Â�
		g_Camera.len -= VALUE_MOVE_CAMERA;
		g_Camera.pos.x = g_Camera.at.x - sinf(g_Camera.rot.y) * g_Camera.len;
		g_Camera.pos.z = g_Camera.at.z - cosf(g_Camera.rot.y) * g_Camera.len;
	}

	if (GetKeyboardPress(DIK_M))
	{// �����
		g_Camera.len += VALUE_MOVE_CAMERA;
		g_Camera.pos.x = g_Camera.at.x - sinf(g_Camera.rot.y) * g_Camera.len;
		g_Camera.pos.z = g_Camera.at.z - cosf(g_Camera.rot.y) * g_Camera.len;
	}

	if (GetKeyboardPress(DIK_Q))
	{// �����_����u���v
		g_Camera.rot.y -= VALUE_ROTATE_CAMERA;
		if (g_Camera.rot.y < -XM_PI)
		{
			g_Camera.rot.y += XM_PI * 2.0f;
		}

		g_Camera.at.x = g_Camera.pos.x + sinf(g_Camera.rot.y) * g_Camera.len;
		g_Camera.at.z = g_Camera.pos.z + cosf(g_Camera.rot.y) * g_Camera.len;
	}

	if (GetKeyboardPress(DIK_E))
	{// �����_����u�E�v
		g_Camera.rot.y += VALUE_ROTATE_CAMERA;
		if (g_Camera.rot.y > XM_PI)
		{
			g_Camera.rot.y -= XM_PI * 2.0f;
		}

		g_Camera.at.x = g_Camera.pos.x + sinf(g_Camera.rot.y) * g_Camera.len;
		g_Camera.at.z = g_Camera.pos.z + cosf(g_Camera.rot.y) * g_Camera.len;
	}

	if (GetKeyboardPress(DIK_Y))
	{// ���_�ړ��u��v
		g_Camera.pos.y += VALUE_MOVE_CAMERA;
	}

	if (GetKeyboardPress(DIK_N))
	{// ���_�ړ��u���v
		g_Camera.pos.y -= VALUE_MOVE_CAMERA;
	}

#endif



//#ifdef _DEBUG	// �f�o�b�O����\������
//	PrintDebugProc("Camera:ZC QE TB YN UM R\n");
//	PrintDebugProc("Camera.rot.y:%f\n", g_Camera.rot.y);
//	PrintDebugProc("Camera.pos.x:%f Camera.pos.y:%f Camera.pos.z:%f\n", g_Camera.pos.x,g_Camera.pos.y,g_Camera.pos.z);
//	PrintDebugProc("Camera.at.x:%f Camera.at.y:%f Camera.at.z:%f\n", g_Camera.at.x,g_Camera.at.y,g_Camera.at.z);
//	PrintDebugProc("Camera.state:%d\n", g_Camera.state);
//	PrintDebugProc("g_TargetRotY:%f\n", g_TargetRotY);
//#endif
}


//=============================================================================
// �J�����̍X�V
//=============================================================================
void SetCamera(void) 
{
	// �r���[�}�g���b�N�X�ݒ�
	XMMATRIX mtxView;
	mtxView = XMMatrixLookAtLH(XMLoadFloat3(&g_Camera.pos), XMLoadFloat3(&g_Camera.at), XMLoadFloat3(&g_Camera.up));
	SetViewMatrix(&mtxView);
	XMStoreFloat4x4(&g_Camera.mtxView, mtxView);

	XMMATRIX mtxInvView;
	mtxInvView = XMMatrixInverse(nullptr, mtxView);
	XMStoreFloat4x4(&g_Camera.mtxInvView, mtxInvView);


	// �v���W�F�N�V�����}�g���b�N�X�ݒ�
	XMMATRIX mtxProjection;
	mtxProjection = XMMatrixPerspectiveFovLH(VIEW_ANGLE, VIEW_ASPECT, VIEW_NEAR_Z, VIEW_FAR_Z);

	SetProjectionMatrix(&mtxProjection);
	XMStoreFloat4x4(&g_Camera.mtxProjection, mtxProjection);

	SetShaderCamera(g_Camera.pos);
}


//=============================================================================
// �J�����̎擾
//=============================================================================
CAMERA *GetCamera(void) 
{
	return &g_Camera;
}

//=============================================================================
// �r���[�|�[�g�̐ݒ�
//=============================================================================
void SetViewPort(int type)
{
	ID3D11DeviceContext *g_ImmediateContext = GetDeviceContext();
	D3D11_VIEWPORT vp;

	g_ViewPortType = type;

	// �r���[�|�[�g�ݒ�
		vp.Width = (FLOAT)SCREEN_WIDTH;
		vp.Height = (FLOAT)SCREEN_HEIGHT;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;

	
	g_ImmediateContext->RSSetViewports(1, &vp);

}


int GetViewPortType(void)
{
	return g_ViewPortType;
}


// �J�����̎��_�ƒ����_���Z�b�g
void SetCameraAT(XMFLOAT3 pos)
{
	PLAYER* player = GetPlayer();

		// �����Ŏ󂯎�������W�̉E�O
		g_Camera.at.x = pos.x + sinf(g_Camera.rot.y + XM_PIDIV4 * 0.5f) * 80.0f;
		g_Camera.at.y = 7.0f;
		g_Camera.at.z = pos.z + cosf(g_Camera.rot.y + XM_PIDIV4 * 0.5f) * 80.0f;

		// �����Ŏ󂯎�������W�̉E��
		g_Camera.pos.x = pos.x - sinf(g_Camera.rot.y - XM_PI * 0.125f) * 60.0f;
		g_Camera.pos.z = pos.z - cosf(g_Camera.rot.y - XM_PI * 0.125f) * 60.0f;
}