cbuffer CBuf
{
    matrix transform;
    matrix modelMatrix;
};

struct VSInput
{
    float3 Position : POSITION;
    float3 Normal : NORMAL;
    float3 TexCoord : TEXCOORD;
};

struct VSOutput
{
    float4 Position : SV_Position;
    float3 Normal : NORMAL;
    float3 worldPos : POSITION;
    float2 tex : TEXCOORD;
};

VSOutput main(VSInput input)
{
    VSOutput output;
    output.worldPos = (float3) mul(float4(input.Position, 1.0f), modelMatrix);
    output.Position = mul(float4(input.Position, 1.0f), transform);
    output.Normal = mul(input.Normal, (float3x3) modelMatrix);
    output.tex = input.TexCoord;
    return output;

}