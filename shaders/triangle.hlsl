cbuffer PerObject : register(b0)
{
    float4x4 WorldViewProj;
};


struct VertexInput
{
    float3 position : POSITION;
    float3 normal   : NORMAL;
    float2 uv : TEXCOORD0;
};

struct FragmentInput
{
    float4 position : SV_POSITION;
    float3 normal   : NORMAL;
    float2 uv : TEXCOORD0;
};

//-----------------------------
// Texture + Sampler
//-----------------------------
Texture2D    DiffuseMap : register(t0);
SamplerState LinearSampler : register(s0);

//-----------------------------
// Vertex Shader
//-----------------------------
FragmentInput vertex(VertexInput input)
{
    FragmentInput output;
    output.position = mul(float4(input.position, 1.0f), WorldViewProj);
    output.normal   = normalize(input.normal);
    output.uv = input.uv;
    return output;
}

//-----------------------------
// Fragment (Pixel) Shader
//-----------------------------
float4 fragment(FragmentInput input) : SV_TARGET
{
    // Simple diffuse light from above
    float3 lightDir = normalize(float3(0.0f, 1.0f, 0.5f));
    float  diffuse  = saturate(dot(input.normal, lightDir));

    float4 texColor = DiffuseMap.Sample(LinearSampler, input.uv);

    return texColor * (0.2f + 0.8f * diffuse); // ambient + diffuse
}