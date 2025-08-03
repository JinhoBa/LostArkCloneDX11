float4x4 g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
Texture2D g_Texture2D;

struct VS_IN
{
    float3 vPosition : POSITION;
    float2 vTexcoord : TEXCOORD;
};

struct VS_OUT
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD;
};

struct PS_IN
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD;
};

struct PS_OUT
{
    float4 vColor : SV_TARGET;
};

SamplerState SAMPLER_LINEAR
{
    Filter = MIN_MAG_MIP_LINEAR;
    
};

VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out;
    
    float4x4 matWV, matWVP;
    
    matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);
    
    Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);
    Out.vTexcoord = In.vTexcoord;
    
    
    return Out;
}

PS_OUT PS_MAIN(PS_IN In)
{
    PS_OUT Out;
    float4 texColor = float4(1.f, 1.f, 1.f, 1.f);
    texColor = g_Texture2D.Sample(SAMPLER_LINEAR, In.vTexcoord);
    
    Out.vColor = texColor;
    
    return Out;
}

PS_OUT PS_ALPHATEST(PS_IN In)
{
    PS_OUT Out;
    float4 texColor = float4(1.f, 1.f, 1.f, 1.f);
    texColor = g_Texture2D.Sample(SAMPLER_LINEAR, In.vTexcoord);
    
    clip(texColor.a - 0.7f);
    
    Out.vColor = texColor;
    
    return Out;
}

technique11 DefaultTech
{
    pass DefaultPass
    {
        VertexShader = compile vs_5_0 VS_MAIN();
        PixelShader = compile ps_5_0 PS_MAIN();
    }

    pass AlphaTestPass
    {
        VertexShader = compile vs_5_0 VS_MAIN();
        PixelShader = compile ps_5_0 PS_ALPHATEST();
    }
    
}