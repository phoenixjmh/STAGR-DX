#include "Mesh.h"
#include "Renderer.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
using namespace Logger;
#define DEBUG_IMPORTER








void Mesh::printTextureInfo()
{
    for (auto& tex : Textures)
    {
        Log("Texture: \n", "ID:", tex.slot, "Path: ", tex.path, "Type: ", tex.type);
    }
}






void Mesh::UpdateTransformMatrix(float size, glm::vec3 pos)
{
    TransformMatrix = glm::translate(TransformMatrix, { pos.x,pos.y,pos.z });
    TransformMatrix = glm::scale(TransformMatrix, { size,size,size });
       
    
}




void Mesh::Draw(Shader& shader, unsigned int depthMap)
{

    shader.Bind();
    ComPtr<ID3D11SamplerState> sampler;
    D3D11_SAMPLER_DESC samplerDesc = {};
    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    samplerDesc.MinLOD = 0;
    samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

    auto* CAMERA = &Renderer::Get().GetCamera();






    Renderer::Get().GetDevice()->CreateSamplerState(&samplerDesc, &sampler);
    Renderer::Get().GetContext()->PSSetSamplers(0, 1, sampler.GetAddressOf());

    //bind textures
    for (auto texture : Textures)
    {
        Renderer::Get().GetContext()->PSSetShaderResources(
            texture.slot, 1, texture.textureResourceView.GetAddressOf());
    }
    
    glm::mat4 ProjectionMatrix = glm::mat4(1);
    glm::mat4 ViewMatrix = glm::mat4(1);
    ProjectionMatrix = glm::perspective(glm::radians(45.0f), 1280.0f / 720.0f, 0.001f, 10000.0f);
    ViewMatrix = glm::lookAt(CAMERA->camera_position, CAMERA->camera_position + CAMERA->camera_front
        , CAMERA->camera_up);
    

    

    const MVPConstantBuffer matrices = {
        {TransformMatrix},

        {ProjectionMatrix*ViewMatrix*TransformMatrix}


    };
    const Vec3PosBuffer cameraBuffer = {
        {CAMERA->camera_position}
    };

    const Vec3PosBuffer lightPos = {
        { Renderer::Get().GetSceneData().light_data.light_position}
    };
    
    vertex_buffer->Bind();
    index_buffer->Bind();
    PixelConstantBuffer lightBuff(lightPos);
    PixelConstantBuffer camBuff(cameraBuffer);
    VertexConstantBuffer mvpBuff(matrices);
    mvpBuff.Bind(0);
    lightBuff.Bind(0);
    camBuff.Bind(1);

    //lightBuff.Update(lightPos);

    const std::vector<D3D11_INPUT_ELEMENT_DESC> ied = {
    {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(Vertex, Position), D3D11_INPUT_PER_VERTEX_DATA, 0},
    {"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(Vertex, Normal), D3D11_INPUT_PER_VERTEX_DATA, 0},
    {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, offsetof(Vertex,TexCoords), D3D11_INPUT_PER_VERTEX_DATA, 0},
    { "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(Vertex, Tangents), D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "BITANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(Vertex, BiTangents), D3D11_INPUT_PER_VERTEX_DATA, 0 },

    };
    auto input_layout = InputLayout(ied, shader.GetVertexBytecode());
    input_layout.Bind();
    Renderer::Get().GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    Renderer::Get().GetContext()->DrawIndexed(index_buffer->GetCount(), 0u, 0u);

}

void Mesh::setupMesh()
{

    TransformMatrix = glm::mat4(1);

    vertex_buffer = new VertexBuffer(Vertices);

    index_buffer = new IndexBuffer(Indices);

    const std::vector<D3D11_INPUT_ELEMENT_DESC> ied = {

        {"Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0}
    };


}