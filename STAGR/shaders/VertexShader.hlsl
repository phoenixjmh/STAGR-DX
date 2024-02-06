cbuffer CBuf
{
    matrix modelMatrix;
    matrix MVP_Matrix;
};

struct VSInput
{
    float3 Position : POSITION;
    float3 Normal : NORMAL;
    float2 TexCoord : TEXCOORD;
    float3 tangent : TANGENT;
    float3 bitangent : BITANGENT;
};

struct VSOutput
{
    float4 Position : SV_Position;
    float3 Normal : NORMAL;
    float3 worldPos : POSITION;
    float2 tex : TEXCOORD;
    float3x3 TBNMatrix : TBN;
};

VSOutput main(VSInput input) 
{
    //tangie
    float3 T = normalize((mul(modelMatrix, float4(input.tangent, 0.0))));
    float3 B = normalize((mul(modelMatrix, float4(input.bitangent, 0.0))));
    float3 N = normalize((mul(modelMatrix, float4(input.Normal, 0.0))));
    float3x3 TBN = float3x3(T, B, N);
    
    
    
    
    VSOutput output;
    output.worldPos =
    (float3) mul(modelMatrix, float4(input.Position, 1.0f));
    output.Position = mul(MVP_Matrix, float4(input.Position, 1.0f));
    output.Normal = mul((float3x3)modelMatrix, input.Normal);
    output.tex = input.TexCoord;
    output.TBNMatrix = transpose(float3x3(T, B, N));
    return output;
    //float4(pos, 1.0f);
}