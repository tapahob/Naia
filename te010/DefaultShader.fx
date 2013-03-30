cbuffer cbNeverChanges : register( b0 )
{
	float4x4 View;
};

cbuffer cbChangeOnResize : register ( b1 )
{
	float4x4 Projection;
};

cbuffer cbChangesEveryFrame : register ( b2 )
{
	float4x4 World;
};

struct VS_INPUT
{
	float3 vPosition    :   POSITION;
	float4 vNormal      :   NORMAL;
	float4 vColor       :   COLOR0;
	float2 vTexCoords   :   TEXCOORD;
};

struct VS_OUTPUT
{
	float4 vPosition	:	SV_POSITION; 
	float3 vNormal		:	NORMAL;
	float4 vColor		:	COLOR0;
};

// Default Vertex Shader
VS_OUTPUT DefaultVS(VS_INPUT dataIn)
{
	VS_OUTPUT result;

	float4 vPos = float4(dataIn.vPosition, 1.0f);
	float4 vFinalPos = mul(vPos, World);
	vFinalPos = mul(vFinalPos,View);
	vFinalPos = mul(vFinalPos,Projection);

	result.vPosition = vFinalPos;
	result.vNormal = dataIn.vNormal;
	result.vColor = float4(1,1,1,1);

	return result;
}

// Default PixelShader
float4 DefaultPS(VS_OUTPUT dataIn) : SV_Target
{    
    return dataIn.vColor;
}

technique11 DefaultTechnique
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_4_0,DefaultVS()));
		SetPixelShader(CompileShader(ps_4_0,DefaultPS()));
	}
}