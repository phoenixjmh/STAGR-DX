
Texture2D diffTexture : register(t0);
Texture2D specTexture : register(t1);
Texture2D normalTexture : register(t2);

SamplerState SampleType;

cbuffer LightData : register (b0)
{
    float3 light_position;
};

cbuffer CameraData : register(b1)
{
    float3 view_position;
};

struct PSInput
{
    float4 Position : SV_Position;
    float3 Normal : NORMAL;
    float3 worldPosition : POSITION;
    float2 texcoord :TEXCOORD;
    float3x3 TBNMatrix : TBN;
   
};
float4 main(PSInput input) : SV_Target
{
    //normal map
    
    float3 normal = normalTexture.Sample(SampleType, input.texcoord);
    normal = normalize(normal * 2.0 - 1.0);
   


    float3 light_color = { 1, 1, 1 };
    float4 baseColor = diffTexture.Sample(SampleType, input.texcoord);
    float3 ambient = baseColor.rgb * 0.15;
    
    float3 lightDir = mul(input.TBNMatrix , normalize(light_position - input.worldPosition));
    float diff = max(0, dot(lightDir,normal));
    float3 diffuse = diff * light_color;
    
    //specular
    const float specularIntensity = 5;
    const float3 viewDir = mul(input.TBNMatrix, normalize(view_position - input.worldPosition));
    float3 reflectDir = reflect(-lightDir,normal);

    const float4 specSample = specTexture.Sample(SampleType, input.texcoord);
    const float3 specularColorIntensity = specSample.xyz;
    const float specularPower = specSample.a;
    const float3 spec = specSample.xyz*pow(max(dot(viewDir, reflectDir), 0.0), specularPower);
    const float3 specular =  spec * light_color;
    
    
    
    
    
    
    return float4((ambient + diffuse + specular) * diffTexture.Sample(SampleType, input.texcoord), 1);
    

   
}