//=============================================================================
//
// �����_�����O���� [renderer.cpp]
// Author : HAL���� ������
//
//=============================================================================
#include "main.h"
#include "renderer.h"

//�f�o�b�O�p��ʃe�L�X�g�o�͂�L���ɂ���
#define DEBUG_DISP_TEXTOUT
//�V�F�[�_�[�f�o�b�O�ݒ��L���ɂ���
//#define DEBUG_SHADER


//*********************************************************
// �\����
//*********************************************************

// �}�e���A���p�萔�o�b�t�@�\����
struct MATERIAL_CBUFFER
{
	XMFLOAT4	Ambient;
	XMFLOAT4	Diffuse;
	XMFLOAT4	Specular;
	XMFLOAT4	Emission;
	float		Shininess;
	int			noTexSampling;
	float		Dummy[2];				// 16byte���E�p
};

// ���C�g�p�t���O�\����
struct LIGHTFLAGS
{
	int			Type;		//���C�g�^�C�v�ienum LIGHT_TYPE�j
	int         OnOff;		//���C�g�̃I��or�I�t�X�C�b�`
	int			Dummy[2];
};

// ���C�g�p�萔�o�b�t�@�\����
struct LIGHT_CBUFFER
{
	XMFLOAT4	Direction[LIGHT_MAX];	// ���C�g�̕���
	XMFLOAT4	Position[LIGHT_MAX];	// ���C�g�̈ʒu
	XMFLOAT4	Diffuse[LIGHT_MAX];		// �g�U���̐F
	XMFLOAT4	Ambient[LIGHT_MAX];		// �����̐F
	XMFLOAT4	Attenuation[LIGHT_MAX];	// ������
	LIGHTFLAGS	Flags[LIGHT_MAX];		// ���C�g���
	int			Enable;					// ���C�e�B���O�L���E�����t���O
	int			Dummy[3];				// 16byte���E�p
};

// �t�H�O�p�萔�o�b�t�@�\����
struct FOG_CBUFFER
{
	XMFLOAT4	Fog;					// �t�H�O��
	XMFLOAT4	FogColor;				// �t�H�O�̐F
	int			Enable;					// �t�H�O�L���E�����t���O
	float		Dummy[3];				// 16byte���E�p
};

// �����p�o�b�t�@
struct FUCHI
{
	XMFLOAT4	FuchiColor;
	int			fuchi;
	int			fill[3];
};


//=========================================================================
// �f�B�]�����p�o�b�t�@
//=========================================================================
struct DISSOLVE_CBUFFER
{
	XMFLOAT4	DissolveColor;
	int			Enable;
	float		threshold;
	float		fill[2];
};

// �Z�s�A�p�o�b�t�@
struct SEPIA
{
	int			sepia;
	int			Dummy[3];				// 16byte���E�p
};

// ���]�F�p�o�b�t�@
struct REVERSAL
{
	int			   reversal;
	int				fake[3];

};

// ���m�N���p�o�b�t�@
struct MONOCHROME
{
	int			   monochrome;
	int				 fake1[3];

};

// ���C�v
struct WIPE_CBUFFER
{
	XMFLOAT4	Direction;
	float		WipeSize;
	int			Type;
	int			Dummy[2];
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
static void SetLightBuffer(void);
void SetWipeBuffer(void);


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static D3D_FEATURE_LEVEL			g_FeatureLevel = D3D_FEATURE_LEVEL_11_0;

static ID3D11Device*				g_D3DDevice = NULL;
static ID3D11DeviceContext*			g_ImmediateContext = NULL;
static IDXGISwapChain*				g_SwapChain = NULL;
static ID3D11RenderTargetView*		g_RenderTargetView = NULL;
static ID3D11DepthStencilView*		g_DepthStencilView = NULL;



static ID3D11VertexShader*			g_VertexShader = NULL;
static ID3D11PixelShader*			g_PixelShader = NULL;
static ID3D11InputLayout*			g_VertexLayout = NULL;
static ID3D11Buffer*				g_WorldBuffer = NULL;
static ID3D11Buffer*				g_ViewBuffer = NULL;
static ID3D11Buffer*				g_ProjectionBuffer = NULL;
static ID3D11Buffer*				g_MaterialBuffer = NULL;
static ID3D11Buffer*				g_LightBuffer = NULL;
static ID3D11Buffer*				g_FogBuffer = NULL;

static ID3D11Buffer*				g_CameraBuffer = NULL;

// �ӂ�
static ID3D11Buffer*				g_FuchiBuffer = NULL;
// dissolve
static ID3D11Buffer*				g_DissolveBuffer = NULL;
// �Z�s�A
static ID3D11Buffer*				g_SepiaBuffer = NULL;
// ���]�F
static ID3D11Buffer*				g_ReversalBuffer = NULL;
// ���m�N��
static ID3D11Buffer*				g_MonochromeBuffer = NULL;
// ���C�v
static ID3D11Buffer*				g_WipeBuffer = NULL;


static ID3D11DepthStencilState*		g_DepthStateEnable = NULL;
static ID3D11DepthStencilState*		g_DepthStateDisable = NULL;

static ID3D11BlendState*			g_BlendStateNone;
static ID3D11BlendState*			g_BlendStateAlphaBlend;
static ID3D11BlendState*			g_BlendStateAdd;
static ID3D11BlendState*			g_BlendStateSubtract;
static BLEND_MODE					g_BlendStateParam;


static ID3D11RasterizerState*		g_RasterStateCullOff = NULL;
static ID3D11RasterizerState*		g_RasterStateCullCW = NULL;
static ID3D11RasterizerState*		g_RasterStateCullCCW = NULL;
static ID3D11ShaderResourceView*	g_Fractalnoise = NULL;	// �e�N�X�`�����

static ID3D11Texture2D*				g_BackBuffer = NULL;
static ID3D11Texture2D*				g_depthTexture = NULL;
static ID3D11SamplerState*			g_samplerState = NULL;

static ID3DBlob*					g_ErrorBlob = NULL;
static ID3DBlob*					g_VSBlob = NULL;
static ID3DBlob*					g_PSBlob = NULL;


static MATERIAL_CBUFFER	g_Material;
static LIGHT_CBUFFER	g_Light;
static FOG_CBUFFER		g_Fog;

// �ӂ�
static FUCHI			g_Fuchi;
// dissolve
static DISSOLVE_CBUFFER g_Dissolve;
// �Z�s�A
static SEPIA			g_Sepia;
// ���]�F
static REVERSAL			g_Reversal;
// ���m�N��
static MONOCHROME		g_Monochrome;
// ���C�v
static WIPE_CBUFFER		g_Wipe;

static float g_ClearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };	// �w�i�F


ID3D11Device* GetDevice( void )
{
	return g_D3DDevice;
}


ID3D11DeviceContext* GetDeviceContext( void )
{
	return g_ImmediateContext;
}


void SetDepthEnable( BOOL Enable )
{
	if( Enable )
		g_ImmediateContext->OMSetDepthStencilState( g_DepthStateEnable, NULL );
	else
		g_ImmediateContext->OMSetDepthStencilState( g_DepthStateDisable, NULL );

}


void SetBlendState(BLEND_MODE bm)
{
	g_BlendStateParam = bm;

	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

	switch (g_BlendStateParam)
	{
	case BLEND_MODE_NONE:
		g_ImmediateContext->OMSetBlendState(g_BlendStateNone, blendFactor, 0xffffffff);
		break;
	case BLEND_MODE_ALPHABLEND:
		g_ImmediateContext->OMSetBlendState(g_BlendStateAlphaBlend, blendFactor, 0xffffffff);
		break;
	case BLEND_MODE_ADD:
		g_ImmediateContext->OMSetBlendState(g_BlendStateAdd, blendFactor, 0xffffffff);
		break;
	case BLEND_MODE_SUBTRACT:
		g_ImmediateContext->OMSetBlendState(g_BlendStateSubtract, blendFactor, 0xffffffff);
		break;
	}
}

void SetCullingMode(CULL_MODE cm)
{
	switch (cm)
	{
	case CULL_MODE_NONE:
		g_ImmediateContext->RSSetState(g_RasterStateCullOff);
		break;
	case CULL_MODE_FRONT:
		g_ImmediateContext->RSSetState(g_RasterStateCullCW);
		break;
	case CULL_MODE_BACK:
		g_ImmediateContext->RSSetState(g_RasterStateCullCCW);
		break;
	}
}

void SetAlphaTestEnable(BOOL flag)
{
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(blendDesc));

	if (flag)
		blendDesc.AlphaToCoverageEnable = TRUE;
	else
		blendDesc.AlphaToCoverageEnable = FALSE;

	blendDesc.IndependentBlendEnable = FALSE;
	blendDesc.RenderTarget[0].BlendEnable = TRUE;

	switch (g_BlendStateParam)
	{
	case BLEND_MODE_NONE:
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		break;
	case BLEND_MODE_ALPHABLEND:
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		break;
	case BLEND_MODE_ADD:
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		break;
	case BLEND_MODE_SUBTRACT:
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_REV_SUBTRACT;
		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		break;
	}

	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	ID3D11BlendState* blendState = NULL;
	g_D3DDevice->CreateBlendState(&blendDesc, &blendState);
	g_ImmediateContext->OMSetBlendState(blendState, blendFactor, 0xffffffff);

	if (blendState != NULL)
		blendState->Release();
}


void SetWorldViewProjection2D( void )
{
	XMMATRIX world;
	world = XMMatrixTranspose(XMMatrixIdentity());
	GetDeviceContext()->UpdateSubresource(g_WorldBuffer, 0, NULL, &world, 0, 0);

	XMMATRIX view;
	view = XMMatrixTranspose(XMMatrixIdentity());
	GetDeviceContext()->UpdateSubresource(g_ViewBuffer, 0, NULL, &view, 0, 0);

	XMMATRIX worldViewProjection;
	worldViewProjection = XMMatrixOrthographicOffCenterLH(0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.0f, 1.0f);
	worldViewProjection = XMMatrixTranspose(worldViewProjection);
	GetDeviceContext()->UpdateSubresource(g_ProjectionBuffer, 0, NULL, &worldViewProjection, 0, 0);
}


void SetWorldMatrix( XMMATRIX *WorldMatrix )
{
	XMMATRIX world;
	world = *WorldMatrix;
	world = XMMatrixTranspose(world);

	GetDeviceContext()->UpdateSubresource(g_WorldBuffer, 0, NULL, &world, 0, 0);
}

void SetViewMatrix(XMMATRIX *ViewMatrix )
{
	XMMATRIX view;
	view = *ViewMatrix;
	view = XMMatrixTranspose(view);

	GetDeviceContext()->UpdateSubresource(g_ViewBuffer, 0, NULL, &view, 0, 0);
}

void SetProjectionMatrix( XMMATRIX *ProjectionMatrix )
{
	XMMATRIX projection;
	projection = *ProjectionMatrix;
	projection = XMMatrixTranspose(projection);

	GetDeviceContext()->UpdateSubresource(g_ProjectionBuffer, 0, NULL, &projection, 0, 0);
}

void SetMaterial( MATERIAL material )
{
	g_Material.Diffuse = material.Diffuse;
	g_Material.Ambient = material.Ambient;
	g_Material.Specular = material.Specular;
	g_Material.Emission = material.Emission;
	g_Material.Shininess = material.Shininess;
	g_Material.noTexSampling = material.noTexSampling;

	GetDeviceContext()->UpdateSubresource( g_MaterialBuffer, 0, NULL, &g_Material, 0, 0 );
}

void SetLightBuffer(void)
{
	GetDeviceContext()->UpdateSubresource(g_LightBuffer, 0, NULL, &g_Light, 0, 0);
}

void SetLightEnable(BOOL flag)
{
	// �t���O���X�V����
	g_Light.Enable = flag;

	SetLightBuffer();
}

void SetLight(int index, LIGHT* pLight)
{
	g_Light.Position[index] = XMFLOAT4(pLight->Position.x, pLight->Position.y, pLight->Position.z, 0.0f);
	g_Light.Direction[index] = XMFLOAT4(pLight->Direction.x, pLight->Direction.y, pLight->Direction.z, 0.0f);
	g_Light.Diffuse[index] = pLight->Diffuse;
	g_Light.Ambient[index] = pLight->Ambient;
	g_Light.Flags[index].Type = pLight->Type;
	g_Light.Flags[index].OnOff = pLight->Enable;
	g_Light.Attenuation[index].x = pLight->Attenuation;

	SetLightBuffer();
}

void SetFogBuffer(void)
{
	GetDeviceContext()->UpdateSubresource(g_FogBuffer, 0, NULL, &g_Fog, 0, 0);
}

void SetFogEnable(BOOL flag)
{
	// �t���O���X�V����
	g_Fog.Enable = flag;

	SetFogBuffer();
}

void SetFog(FOG* pFog)
{
	g_Fog.Fog.x = pFog->FogStart;
	g_Fog.Fog.y = pFog->FogEnd;
	g_Fog.FogColor = pFog->FogColor;

	SetFogBuffer();
}

void SetFuchi(int flag)
{
	g_Fuchi.fuchi = flag;
	GetDeviceContext()->UpdateSubresource(g_FuchiBuffer, 0, NULL, &g_Fuchi, 0, 0);
}


void SetShaderCamera(XMFLOAT3 pos)
{
	XMFLOAT4 tmp = XMFLOAT4( pos.x, pos.y, pos.z, 0.0f );

	GetDeviceContext()->UpdateSubresource(g_CameraBuffer, 0, NULL, &tmp, 0, 0);
}


// �f�B�]����
void SetDissolveBuffer(void)
{
	GetDeviceContext()->UpdateSubresource(g_DissolveBuffer, 0, NULL, &g_Dissolve, 0, 0);
}

void SetDissolveEnable(BOOL flag)
{
	// �t���O���X�V����
	g_Dissolve.Enable = flag;

	SetDissolveBuffer();
}

void SetDissolve(DISSOLVE* pDissolve)
{

	g_Dissolve.Enable = pDissolve->Enable;
	g_Dissolve.threshold = pDissolve->threshold;
	g_Dissolve.DissolveColor = pDissolve->DissolveColor;

	SetDissolveBuffer();
}

// �����܂�


// �Z�s�A
void SetSepia(int flag)
{
	g_Sepia.sepia = flag;
	GetDeviceContext()->UpdateSubresource(g_SepiaBuffer, 0, NULL, &g_Sepia, 0, 0);
}

// ���]�F
void SetReversal(int flag)
{
	g_Reversal.reversal = flag;
	GetDeviceContext()->UpdateSubresource(g_ReversalBuffer, 0, NULL, &g_Reversal, 0, 0);
}
// ���m�N��
void SetMonochrome(int flag)
{
	g_Monochrome.monochrome = flag;
	GetDeviceContext()->UpdateSubresource(g_MonochromeBuffer, 0, NULL, &g_Monochrome, 0, 0);
}


void SetWipeBuffer(void)
{
	GetDeviceContext()->UpdateSubresource(g_WipeBuffer, 0, NULL, &g_Wipe, 0, 0);
}

void SetWipe(int type, XMFLOAT4 dir, float size)
{
	// �t���O���X�V����
	g_Wipe.Type = type;
	g_Wipe.Direction = dir;
	g_Wipe.WipeSize = size;

	SetWipeBuffer();
}

//=============================================================================
// ����������
//=============================================================================
HRESULT InitRenderer(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	HRESULT hr = S_OK;

	// �f�o�C�X�A�X���b�v�`�F�[���A�R���e�L�X�g����
	DWORD deviceFlags = 0;
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory( &sd, sizeof( sd ) );
	sd.BufferCount = 1;
	sd.BufferDesc.Width = SCREEN_WIDTH;
	sd.BufferDesc.Height = SCREEN_HEIGHT;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = bWindow;

	//�f�o�b�O�����o�͗p�ݒ�
#if defined(_DEBUG) && defined(DEBUG_DISP_TEXTOUT)
	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_GDI_COMPATIBLE;
	deviceFlags = D3D11_CREATE_DEVICE_DEBUG | D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#endif

	hr = D3D11CreateDeviceAndSwapChain( NULL,
										D3D_DRIVER_TYPE_HARDWARE,
										NULL,
										deviceFlags,
										NULL,
										0,
										D3D11_SDK_VERSION,
										&sd,
										&g_SwapChain,
										&g_D3DDevice,
										&g_FeatureLevel,
										&g_ImmediateContext );
	if( FAILED( hr ) )
		return hr;

	//�f�o�b�O�����o�͗p�ݒ�
#if defined(_DEBUG) && defined(DEBUG_DISP_TEXTOUT)
	hr = g_SwapChain->ResizeBuffers(0, SCREEN_WIDTH, SCREEN_HEIGHT, DXGI_FORMAT_UNKNOWN, DXGI_SWAP_CHAIN_FLAG_GDI_COMPATIBLE); // N.B. the GDI compatible flag
	if (FAILED(hr))
		return hr;
#endif

	// �����_�[�^�[�Q�b�g�r���[�����A�ݒ�
	g_SwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), ( LPVOID* )&g_BackBuffer );
	g_D3DDevice->CreateRenderTargetView( g_BackBuffer, NULL, &g_RenderTargetView );


	//�X�e���V���p�e�N�X�`���[�쐬
	D3D11_TEXTURE2D_DESC td;
	ZeroMemory( &td, sizeof(td) );
	td.Width			= sd.BufferDesc.Width;
	td.Height			= sd.BufferDesc.Height;
	td.MipLevels		= 1;
	td.ArraySize		= 1;
	td.Format			= DXGI_FORMAT_D24_UNORM_S8_UINT;
	td.SampleDesc		= sd.SampleDesc;
	td.Usage			= D3D11_USAGE_DEFAULT;
	td.BindFlags		= D3D11_BIND_DEPTH_STENCIL;
    td.CPUAccessFlags	= 0;
    td.MiscFlags		= 0;
	g_D3DDevice->CreateTexture2D( &td, NULL, &g_depthTexture );

	//�X�e���V���^�[�Q�b�g�쐬
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
	ZeroMemory( &dsvd, sizeof(dsvd) );
	dsvd.Format			= td.Format;
	dsvd.ViewDimension	= D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvd.Flags			= 0;
	g_D3DDevice->CreateDepthStencilView( g_depthTexture, &dsvd, &g_DepthStencilView );


	g_ImmediateContext->OMSetRenderTargets( 1, &g_RenderTargetView, g_DepthStencilView );


	// �r���[�|�[�g�ݒ�
	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)SCREEN_WIDTH;
	vp.Height = (FLOAT)SCREEN_HEIGHT;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	g_ImmediateContext->RSSetViewports( 1, &vp );



	// ���X�^���C�U�X�e�[�g�쐬
	D3D11_RASTERIZER_DESC rd; 
	ZeroMemory( &rd, sizeof( rd ) );
	rd.FillMode = D3D11_FILL_SOLID;
	rd.CullMode = D3D11_CULL_NONE; 
	rd.DepthClipEnable = TRUE; 
	rd.MultisampleEnable = FALSE; 
	g_D3DDevice->CreateRasterizerState( &rd, &g_RasterStateCullOff);

	rd.CullMode = D3D11_CULL_FRONT;
	g_D3DDevice->CreateRasterizerState(&rd, &g_RasterStateCullCW);

	rd.CullMode = D3D11_CULL_BACK;
	g_D3DDevice->CreateRasterizerState(&rd, &g_RasterStateCullCCW);

	// �J�����O���[�h�ݒ�iCCW�j
	SetCullingMode(CULL_MODE_BACK);



	// �u�����h�X�e�[�g�̍쐬
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory( &blendDesc, sizeof( blendDesc ) );
	blendDesc.AlphaToCoverageEnable = FALSE;
	blendDesc.IndependentBlendEnable = FALSE;
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	g_D3DDevice->CreateBlendState( &blendDesc, &g_BlendStateAlphaBlend );

	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	g_D3DDevice->CreateBlendState(&blendDesc, &g_BlendStateNone);

	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	g_D3DDevice->CreateBlendState(&blendDesc, &g_BlendStateAdd);

	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_REV_SUBTRACT;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	g_D3DDevice->CreateBlendState(&blendDesc, &g_BlendStateSubtract);

	// �A���t�@�u�����h�ݒ�
	SetBlendState(BLEND_MODE_ALPHABLEND);




	// �[�x�X�e���V���X�e�[�g�쐬
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory( &depthStencilDesc, sizeof( depthStencilDesc ) );
	depthStencilDesc.DepthEnable = TRUE;
	depthStencilDesc.DepthWriteMask	= D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	depthStencilDesc.StencilEnable = FALSE;

	g_D3DDevice->CreateDepthStencilState( &depthStencilDesc, &g_DepthStateEnable );//�[�x�L���X�e�[�g

	//depthStencilDesc.DepthEnable = FALSE;
	depthStencilDesc.DepthWriteMask	= D3D11_DEPTH_WRITE_MASK_ZERO;
	g_D3DDevice->CreateDepthStencilState( &depthStencilDesc, &g_DepthStateDisable );//�[�x�����X�e�[�g

	// �[�x�X�e���V���X�e�[�g�ݒ�
	SetDepthEnable(TRUE);



	// �T���v���[�X�e�[�g�ݒ�
	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory( &samplerDesc, sizeof( samplerDesc ) );
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0;
	samplerDesc.MaxAnisotropy = 16;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	g_D3DDevice->CreateSamplerState( &samplerDesc, &g_samplerState );

	g_ImmediateContext->PSSetSamplers( 0, 1, &g_samplerState );



	// ���_�V�F�[�_�R���p�C���E����
	DWORD shFlag = D3DCOMPILE_ENABLE_STRICTNESS;

#if defined(_DEBUG) && defined(DEBUG_SHADER)
	shFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	hr = D3DX11CompileFromFile( "shader.hlsl", NULL, NULL, "VertexShaderPolygon", "vs_4_0", shFlag, 0, NULL, &g_VSBlob, &g_ErrorBlob, NULL );
	if( FAILED(hr) )
	{
		MessageBox( NULL , (char*)g_ErrorBlob->GetBufferPointer(), "VS", MB_OK | MB_ICONERROR );
	}

	g_D3DDevice->CreateVertexShader( g_VSBlob->GetBufferPointer(), g_VSBlob->GetBufferSize(), NULL, &g_VertexShader );

	// ���̓��C�A�E�g����
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,			0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	UINT numElements = ARRAYSIZE( layout );

	g_D3DDevice->CreateInputLayout( layout,
		numElements,
		g_VSBlob->GetBufferPointer(),
		g_VSBlob->GetBufferSize(),
		&g_VertexLayout );

	//g_VSBlob->Release();


	// �s�N�Z���V�F�[�_�R���p�C���E����
	hr = D3DX11CompileFromFile( "shader.hlsl", NULL, NULL, "PixelShaderPolygon", "ps_4_0", shFlag, 0, NULL, &g_PSBlob, &g_ErrorBlob, NULL );
	if( FAILED(hr) )
	{
		MessageBox( NULL , (char*)g_ErrorBlob->GetBufferPointer(), "PS", MB_OK | MB_ICONERROR );
	}

	g_D3DDevice->CreatePixelShader( g_PSBlob->GetBufferPointer(), g_PSBlob->GetBufferSize(), NULL, &g_PixelShader );
	
	//g_PSBlob->Release();


	// �f�B�]����
	{
		// �\��e�N�X�`����ݒ�
		D3DX11CreateShaderResourceViewFromFile(GetDevice(),
			"data/TEXTURE/fractalnoise.jpg",
			NULL,
			NULL,
			&g_Fractalnoise,
			NULL);

		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(1, 1, &g_Fractalnoise);
	}



	 
	 
	// �萔�o�b�t�@����
	D3D11_BUFFER_DESC hBufferDesc;
	hBufferDesc.ByteWidth = sizeof(XMMATRIX);
	hBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	hBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	hBufferDesc.CPUAccessFlags = 0;
	hBufferDesc.MiscFlags = 0;
	hBufferDesc.StructureByteStride = sizeof(float);

	//���[���h�}�g���N�X
	g_D3DDevice->CreateBuffer(&hBufferDesc, NULL, &g_WorldBuffer);
	g_ImmediateContext->VSSetConstantBuffers(0, 1, &g_WorldBuffer);
	g_ImmediateContext->PSSetConstantBuffers(0, 1, &g_WorldBuffer);

	//�r���[�}�g���N�X
	g_D3DDevice->CreateBuffer(&hBufferDesc, NULL, &g_ViewBuffer);
	g_ImmediateContext->VSSetConstantBuffers(1, 1, &g_ViewBuffer);
	g_ImmediateContext->PSSetConstantBuffers(1, 1, &g_ViewBuffer);

	//�v���W�F�N�V�����}�g���N�X
	g_D3DDevice->CreateBuffer(&hBufferDesc, NULL, &g_ProjectionBuffer);
	g_ImmediateContext->VSSetConstantBuffers(2, 1, &g_ProjectionBuffer);
	g_ImmediateContext->PSSetConstantBuffers(2, 1, &g_ProjectionBuffer);

	//�}�e���A�����
	hBufferDesc.ByteWidth = sizeof(MATERIAL_CBUFFER);
	g_D3DDevice->CreateBuffer(&hBufferDesc, NULL, &g_MaterialBuffer);
	g_ImmediateContext->VSSetConstantBuffers(3, 1, &g_MaterialBuffer);
	g_ImmediateContext->PSSetConstantBuffers(3, 1, &g_MaterialBuffer);

	//���C�g���
	hBufferDesc.ByteWidth = sizeof(LIGHT_CBUFFER);
	g_D3DDevice->CreateBuffer(&hBufferDesc, NULL, &g_LightBuffer);
	g_ImmediateContext->VSSetConstantBuffers(4, 1, &g_LightBuffer);
	g_ImmediateContext->PSSetConstantBuffers(4, 1, &g_LightBuffer);

	//�t�H�O���
	hBufferDesc.ByteWidth = sizeof(FOG_CBUFFER);
	g_D3DDevice->CreateBuffer(&hBufferDesc, NULL, &g_FogBuffer);
	g_ImmediateContext->VSSetConstantBuffers(5, 1, &g_FogBuffer);
	g_ImmediateContext->PSSetConstantBuffers(5, 1, &g_FogBuffer);

	//�����
	hBufferDesc.ByteWidth = sizeof(FUCHI);
	g_D3DDevice->CreateBuffer(&hBufferDesc, NULL, &g_FuchiBuffer);
	g_ImmediateContext->VSSetConstantBuffers(6, 1, &g_FuchiBuffer);
	g_ImmediateContext->PSSetConstantBuffers(6, 1, &g_FuchiBuffer);

	//�J����
	hBufferDesc.ByteWidth = sizeof(XMFLOAT4);
	g_D3DDevice->CreateBuffer(&hBufferDesc, NULL, &g_CameraBuffer);
	g_ImmediateContext->VSSetConstantBuffers(7, 1, &g_CameraBuffer);
	g_ImmediateContext->PSSetConstantBuffers(7, 1, &g_CameraBuffer);

	// �f�B�]����
	hBufferDesc.ByteWidth = sizeof(DISSOLVE_CBUFFER);
	g_D3DDevice->CreateBuffer(&hBufferDesc, NULL, &g_DissolveBuffer);
	g_ImmediateContext->VSSetConstantBuffers(8, 1, &g_DissolveBuffer);
	g_ImmediateContext->PSSetConstantBuffers(8, 1, &g_DissolveBuffer);

	// �Z�s�A
	hBufferDesc.ByteWidth = sizeof(SEPIA);
	g_D3DDevice->CreateBuffer(&hBufferDesc, NULL, &g_SepiaBuffer);
	g_ImmediateContext->VSSetConstantBuffers(9, 1, &g_SepiaBuffer);
	g_ImmediateContext->PSSetConstantBuffers(9, 1, &g_SepiaBuffer);

	// ���]�F
	hBufferDesc.ByteWidth = sizeof(REVERSAL);
	g_D3DDevice->CreateBuffer(&hBufferDesc, NULL, &g_ReversalBuffer);
	g_ImmediateContext->VSSetConstantBuffers(10, 1, &g_ReversalBuffer);
	g_ImmediateContext->PSSetConstantBuffers(10, 1, &g_ReversalBuffer);

	// ���m�N��
	hBufferDesc.ByteWidth = sizeof(REVERSAL);
	g_D3DDevice->CreateBuffer(&hBufferDesc, NULL, &g_MonochromeBuffer);
	g_ImmediateContext->VSSetConstantBuffers(11, 1, &g_MonochromeBuffer);
	g_ImmediateContext->PSSetConstantBuffers(11, 1, &g_MonochromeBuffer);

	// ���C�v������(�^�C�g���Ƃ���Init�ŏ�������������renderer���ŏ��������Ă���������)
	hBufferDesc.ByteWidth = sizeof(WIPE_CBUFFER);
	g_D3DDevice->CreateBuffer(&hBufferDesc, NULL, &g_WipeBuffer);
	g_ImmediateContext->VSSetConstantBuffers(12, 1, &g_WipeBuffer);
	g_ImmediateContext->PSSetConstantBuffers(12, 1, &g_WipeBuffer);

	//WIPE wipe;
	//wipe.Direction.xy = (1.0f, 1.0f);

// ���̓��C�A�E�g�ݒ�
	g_ImmediateContext->IASetInputLayout( g_VertexLayout );

	// �V�F�[�_�ݒ�
	g_ImmediateContext->VSSetShader( g_VertexShader, NULL, 0 );
	g_ImmediateContext->PSSetShader( g_PixelShader, NULL, 0 );

	//���C�g������
	ZeroMemory(&g_Light, sizeof(LIGHT_CBUFFER));
	g_Light.Direction[0] = XMFLOAT4(1.0f, -1.0f, 1.0f, 0.0f);
	g_Light.Diffuse[0] = XMFLOAT4(0.9f, 0.9f, 0.9f, 1.0f);
	g_Light.Ambient[0] = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	g_Light.Flags[0].Type = LIGHT_TYPE_DIRECTIONAL;
	SetLightBuffer();


	//�}�e���A��������
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	material.Ambient = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);

	// �f�B�]����������
	DISSOLVE dissolve;
	ZeroMemory(&dissolve, sizeof(dissolve));
	dissolve.Enable = FALSE;
	dissolve.threshold = 0.5f;
	SetDissolve(&dissolve);



	return S_OK;
}


//=============================================================================
// �I������
//=============================================================================
void UninitRenderer(void)
{
	// �I�u�W�F�N�g���
	if (g_DepthStateEnable)		g_DepthStateEnable->Release();
	if (g_DepthStateDisable)	g_DepthStateDisable->Release();
	if (g_BlendStateNone)		g_BlendStateNone->Release();
	if (g_BlendStateAlphaBlend)	g_BlendStateAlphaBlend->Release();
	if (g_BlendStateAdd)		g_BlendStateAdd->Release();
	if (g_BlendStateSubtract)	g_BlendStateSubtract->Release();
	if (g_RasterStateCullOff)	g_RasterStateCullOff->Release();
	if (g_RasterStateCullCW)	g_RasterStateCullCW->Release();
	if (g_RasterStateCullCCW)	g_RasterStateCullCCW->Release();

	if (g_WorldBuffer)			g_WorldBuffer->Release();
	if (g_ViewBuffer)			g_ViewBuffer->Release();
	if (g_ProjectionBuffer)		g_ProjectionBuffer->Release();
	if (g_MaterialBuffer)		g_MaterialBuffer->Release();
	if (g_LightBuffer)			g_LightBuffer->Release();
	if (g_FogBuffer)			g_FogBuffer->Release();

	// dissolve
	if (g_DissolveBuffer)		g_DissolveBuffer->Release();
	// �Z�s�A
	if (g_SepiaBuffer)			g_SepiaBuffer->Release();
	// ���]�F
	if (g_ReversalBuffer)		g_ReversalBuffer->Release();
	// ���m�N��
	if (g_MonochromeBuffer)		g_MonochromeBuffer->Release();
	// ���C�v
	if (g_WipeBuffer)			g_WipeBuffer->Release();

	if (g_FuchiBuffer)			g_FuchiBuffer->Release();

	if (g_VertexLayout)			g_VertexLayout->Release();
	if (g_VertexShader)			g_VertexShader->Release();
	if (g_PixelShader)			g_PixelShader->Release();

	if (g_ImmediateContext)		g_ImmediateContext->ClearState();
	if (g_RenderTargetView)		g_RenderTargetView->Release();
	if (g_DepthStencilView)		g_DepthStencilView->Release();
	if (g_SwapChain)			g_SwapChain->Release();
	if (g_ImmediateContext)		g_ImmediateContext->Release();
	if (g_D3DDevice)			g_D3DDevice->Release();
	if (g_CameraBuffer)			g_CameraBuffer->Release();
	if (g_BackBuffer)			g_BackBuffer->Release();
	if (g_depthTexture)			g_depthTexture->Release();
	if (g_samplerState)			g_samplerState->Release();
	if (g_ErrorBlob)			g_ErrorBlob->Release();
	if (g_VSBlob)				g_VSBlob->Release();
	if (g_PSBlob)				g_PSBlob->Release();
	if (g_Fractalnoise)			g_Fractalnoise->Release();
}



//=============================================================================
// �o�b�N�o�b�t�@�N���A
//=============================================================================
void Clear(void)
{
	// �o�b�N�o�b�t�@�N���A
	g_ImmediateContext->ClearRenderTargetView( g_RenderTargetView, g_ClearColor );
	g_ImmediateContext->ClearDepthStencilView( g_DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}


void SetClearColor(float* color4)
{
	g_ClearColor[0] = color4[0];
	g_ClearColor[1] = color4[1];
	g_ClearColor[2] = color4[2];
	g_ClearColor[3] = color4[3];
}


//=============================================================================
// �v���[���g
//=============================================================================
void Present(void)
{
	g_SwapChain->Present( 0, 0 );
}


//=============================================================================
// �f�o�b�O�p�e�L�X�g�o��
//=============================================================================
void DebugTextOut(char* text, int x, int y)
{
#if defined(_DEBUG) && defined(DEBUG_DISP_TEXTOUT)
	HRESULT hr;

	//�o�b�N�o�b�t�@����T�[�t�F�X���擾����
	IDXGISurface1* pBackSurface = NULL;
	hr = g_SwapChain->GetBuffer(0, __uuidof(IDXGISurface1), (void**)&pBackSurface);

	if (SUCCEEDED(hr))
	{
		//�擾�����T�[�t�F�X����f�o�C�X�R���e�L�X�g���擾����
		HDC hdc;
		hr = pBackSurface->GetDC(FALSE, &hdc);

		if (SUCCEEDED(hr))
		{
			//�����F�𔒂ɕύX
			SetTextColor(hdc, RGB(255, 255, 255));
			//�w�i�𓧖��ɕύX
			SetBkMode(hdc, TRANSPARENT);

			RECT rect;
			rect.left = 0;
			rect.top = 0;
			rect.right = SCREEN_WIDTH;
			rect.bottom = SCREEN_HEIGHT;

			//�e�L�X�g�o��
			DrawText(hdc, text, (int)strlen(text), &rect, DT_LEFT);

			//�f�o�C�X�R���e�L�X�g���������
			pBackSurface->ReleaseDC(NULL);
		}
		//�T�[�t�F�X���������
		pBackSurface->Release();

		//�����_�����O�^�[�Q�b�g�����Z�b�g�����̂ŃZ�b�g���Ȃ���
		g_ImmediateContext->OMSetRenderTargets(1, &g_RenderTargetView, g_DepthStencilView);
	}
#endif
}