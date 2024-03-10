

//*****************************************************************************
// �萔�o�b�t�@
//*****************************************************************************

// �}�g���N�X�o�b�t�@
cbuffer WorldBuffer : register( b0 )
{
	matrix World;
}

cbuffer ViewBuffer : register( b1 )
{
	matrix View;
}

cbuffer ProjectionBuffer : register( b2 )
{
	matrix Projection;
}

// �}�e���A���o�b�t�@
struct MATERIAL
{
	float4		Ambient;
	float4		Diffuse;
	float4		Specular;
	float4		Emission;
	float		Shininess;
	int			noTexSampling;
	float		Dummy[2];//16byte���E�p
};

cbuffer MaterialBuffer : register( b3 )
{
	MATERIAL	Material;
}

// ���C�g�p�o�b�t�@
struct LIGHT
{
	float4		Direction[5];
	float4		Position[5];
	float4		Diffuse[5];
	float4		Ambient[5];
	float4		Attenuation[5];
	int4		Flags[5];
	int			Enable;
	int			Dummy[3];//16byte���E�p
};

cbuffer LightBuffer : register( b4 )
{
	LIGHT		Light;
}

struct FOG
{
	float4		Distance;
	float4		FogColor;
	int			Enable;
	float		Dummy[3];//16byte���E�p
};

// �t�H�O�p�o�b�t�@
cbuffer FogBuffer : register( b5 )
{
	FOG			Fog;
};


// �����p�o�b�t�@
cbuffer Fuchi : register(b6)
{
	float4		FuchiColor;
	int			fuchi;
	int			fill[3];
};


cbuffer CameraBuffer : register(b7)
{
	float4 Camera;
}

//=========================================================================
// // �f�B�]�����p�o�b�t�@
//======================================================================
struct DISSOLVE
{
	float4	DissolveColor;
	int		Enable;
	float	threshold;
	float	Fill[2];
};

cbuffer DissolveBuffer : register(b8)
{
	DISSOLVE dissolve;
};
// �����܂�


// �Z�s�A
cbuffer Sepia : register(b9)
{
	int		sepia;
	int		fake1[3];
};
//���]�F
cbuffer Reversal : register(b10)
{
	int		reversal;
	float	fake2[3];
};
//���m�N��
cbuffer Monochrome : register(b11)
{
	int		monochrome;
	int		fake3[3];
};

// ���C�v
struct WIPE
{
	float4		Direction;
	float		WipeSize;
	int			Type;
	int			Dummy[2];
};

cbuffer WipeBuffer : register(b12)
{
	WIPE		Wipe;
}


//=============================================================================
// ���_�V�F�[�_
//=============================================================================
void VertexShaderPolygon( in  float4 inPosition		: POSITION0,
						  in  float4 inNormal		: NORMAL0,
						  in  float4 inDiffuse		: COLOR0,
						  in  float2 inTexCoord		: TEXCOORD0,

						  out float4 outPosition	: SV_POSITION,
						  out float4 outNormal		: NORMAL0,
						  out float2 outTexCoord	: TEXCOORD0,
						  out float4 outDiffuse		: COLOR0,
						  out float4 outWorldPos    : POSITION0)
{
	matrix wvp;
	wvp = mul(World, View);
	wvp = mul(wvp, Projection);
	outPosition = mul(inPosition, wvp);

	outNormal = normalize(mul(float4(inNormal.xyz, 0.0f), World));

	outTexCoord = inTexCoord;

	outWorldPos = mul(inPosition, World);

	outDiffuse = inDiffuse;
}



//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
Texture2D		g_Texture : register( t0 );

//dissolve
Texture2D		g_Fractalnoise: register(t1);


SamplerState	g_SamplerState : register( s0 );	 // state�݂����ȁichatgpt�j



//=============================================================================
// �s�N�Z���V�F�[�_
//=============================================================================
void PixelShaderPolygon( in  float4 inPosition		: SV_POSITION,
						 in  float4 inNormal		: NORMAL0,
						 in  float2 inTexCoord		: TEXCOORD0,
						 in  float4 inDiffuse		: COLOR0,
						 in  float4 inWorldPos      : POSITION0,

						 out float4 outDiffuse		: SV_Target )
{
	float4 color;

	if (Material.noTexSampling == 0)
	{
		color = g_Texture.Sample(g_SamplerState, inTexCoord);

		color *= inDiffuse;
	}
	else
	{
		color = inDiffuse;
	}

	if (Light.Enable == 0)
	{
		color = color * Material.Diffuse;
	}
	else
	{
		float4 tempColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
		float4 outColor = float4(0.0f, 0.0f, 0.0f, 0.0f);

		for (int i = 0; i < 5; i++)
		{
			float3 lightDir;
			float light;

			if (Light.Flags[i].y == 1)
			{
				if (Light.Flags[i].x == 1)
				{
					lightDir = normalize(Light.Direction[i].xyz);
					light = dot(lightDir, inNormal.xyz);

					light = 0.5 - 0.5 * light;
					tempColor = color * Material.Diffuse * light * Light.Diffuse[i];
				}
				else if (Light.Flags[i].x == 2)
				{
					lightDir = normalize(Light.Position[i].xyz - inWorldPos.xyz);
					light = dot(lightDir, inNormal.xyz);

					tempColor = color * Material.Diffuse * light * Light.Diffuse[i];

					float distance = length(inWorldPos - Light.Position[i]);

					float att = saturate((Light.Attenuation[i].x - distance) / Light.Attenuation[i].x);
					tempColor *= att;
				}
				else
				{
					tempColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
				}

				outColor += tempColor;
			}
		}

		color = outColor;
		color.a = inDiffuse.a * Material.Diffuse.a;
	}

	//�t�H�O
	if (Fog.Enable == 1)
	{
		float z = inPosition.z*inPosition.w;
		float f = (Fog.Distance.y - z) / (Fog.Distance.y - Fog.Distance.x);
		f = saturate(f);
		outDiffuse = f * color + (1 - f)*Fog.FogColor;
		outDiffuse.a = color.a;
	}
	else
	{
		outDiffuse = color;
	}

	//�����
	if (fuchi == 1)
	{
		float angle = dot(normalize(inWorldPos.xyz - Camera.xyz), normalize(inNormal));
		if ((angle < 0.5f)&&(angle > -0.5f))
		//if (angle > -0.3f)
		{
			outDiffuse.rgb = FuchiColor;
		}
	}


		//�Z�s�A
		if (sepia == 1)
		{
			float gray = color.r * 0.299 + color.g * 0.587 + color.b * 0.114;

			color.r = 0.8 * gray;
			color.g = 0.6 * gray;
			color.b = 0.4 * gray;

			outDiffuse = color;
		}
		//���]�E
		if (reversal == 1)
		{

			float4 inverted_color = 1 - color;
			inverted_color.a = color.a;
			inverted_color.rgb *= inverted_color.a;

			outDiffuse = inverted_color;
		}

		//���m�N��
		if (monochrome == 1)
		{
			float4 gray = color.r * 0.299 + color.g * 0.587 + color.b * 0.114;
			color.rgb = gray.rgb;

			outDiffuse = color;
		}

		//=========================================================================
		// �f�B�]��������
		//======================================================================

		// �f�B�]������L���ɂ��Ă���ꍇ
		if (dissolve.Enable == 1)
		{
			// 1�s�N�Z���̎w�肳�ꂽ�ꏊ�̐F���擾����
			float4 noise = g_Fractalnoise.Sample(g_SamplerState, inTexCoord);

			if (noise.r < dissolve.threshold)
			{
				outDiffuse.a = 0.0f;
			}
			else if (noise.r < dissolve.threshold + 0.1f)
			{
				float temp = (noise.r - dissolve.threshold) * 10.0f;
				outDiffuse.rgb = dissolve.DissolveColor.rgb;

				outDiffuse.rgb += temp;
			}
		}

		// ���C�v
		if (Wipe.Type == 1)												// �߂��郏�C�v
		{
			float vec = dot(Wipe.Direction.xy, inPosition.xy);			// title.cpp��direction.xy��1.0f, 1.0f��ݒ肵�Ă�̂ł��̏ꍇ�E�������ւ̃��C�v�x�N�g���ɂȂ�
																		// ���̓��ς̎��̈Ӗ��͕�����Ȃ��ꍇ�͌����ŕ����Ă��������I(inPosition.xy�͍��̃s�N�Z���̃X�N���[�����W)

			clip(vec - Wipe.WipeSize);

			// clip�֐��́A()���̒l�����̒l�������炻�̃s�N�Z���͔j�������Ⴄ�Ƃ������̂炵��
			// �v�Z������ł����ȃ��C�v���ł������Ȃ̂ŕ֗��ł�
		}
		if (Wipe.Type == 2)												// �~�`�̃��C�v
		{
			float2 posFromCenter = inPosition.xy - float2(480, 270);	// �X�N���[���̒��S����̋������Ƃ�(�X�N���[���T�C�Y����ŕς��Ă��������@������ϐ��Ƃ��œn����悤�ɂ������������̂��ȁH)
			clip(length(posFromCenter) - Wipe.WipeSize);
		}
		if (Wipe.Type == 3)												// �X�g���C�v�̃��C�v
		{
			float cut = fmod(inPosition.x, 64.0f);						// ���̒l��ς���ƃX�g���C�v�̕������R�ɂ�����܂�
			//float cut = (int)fmod(inPosition.x, 64.0f);
			clip(cut - Wipe.WipeSize);
		}

}
