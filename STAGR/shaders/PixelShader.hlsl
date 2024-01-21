
Texture2D diffTexture : register(t0);
Texture2D specTexture : register(t1);

SamplerState SampleType;


cbuffer LightData
{
    float4 light_position;
};

cbuffer CameraData
{
    float3 view_direction;
};
struct PSInput
{
    float4 Position : SV_Position;
    float3 Normal : NORMAL;
    float3 worldPosition : POSITION;
    float2 texcoord :TEXCOORD;
};
float4 main(PSInput input) : SV_Target
{
    float3 light_color = { 1, 1, 1 };
    float4 baseColor = diffTexture.Sample(SampleType, input.texcoord);
    float3 ambient = baseColor.rgb * 0.15;
    
    float3 lightDir = normalize(light_position.xyz - input.worldPosition.xyz );
    float diff = max(0, dot(lightDir,input.Normal));
    float3 diffuse = diff * light_color;
    
    //specular
    float specularStrength = 1;
    
    float3 lighting = ambient+diffuse;
    float3 finalColor = lighting * baseColor.rgb ;
    
    //return diffTexture.Sample(SampleType, input.texcoord);
    return float4(finalColor, baseColor.a);

   
}