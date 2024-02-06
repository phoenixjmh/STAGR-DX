cbuffer CBuf
{
    matrix modelMatrix;
    matrix MVP_Matrix;
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
    output.worldPos = (float3) mul(modelMatrix,float4(input.Position, 1.0f));
    output.Position = mul( MVP_Matrix,float4(input.Position, 1.0f));
    output.Normal = mul((float3x3) modelMatrix,input.Normal);
    output.tex = input.TexCoord;
    return output;

}