#include "Mesh.h"
#include "Renderer.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
using namespace Logger;
#define DEBUG_IMPORTER


struct lightConstantBuffer {
    glm::vec4 light_pos;
};





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
    TransformMatrix = glm::scale(TransformMatrix, { size,size,size*0.5 });
       
    
}

void Mesh::UpdateLightPosition(glm::vec3 light_pos)
{
    m_lightPosition = { Renderer::lightPosX,Renderer::lightPosY,Renderer::lightPosZ,1 };
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
    /*Log("Begin bind*********************************");
    for (auto texture : Textures)
    {
        Renderer::Get().GetContext()->PSSetShaderResources(
            texture.slot, 1, texture.textureResourceView.GetAddressOf());
        Log("Binding");
    }
    Log("END bind*********************************");*/
    Renderer::Get().GetContext()->PSSetShaderResources(
        0, 1, Textures[0].textureResourceView.GetAddressOf());
    Renderer::Get().GetContext()->PSSetShaderResources(
        1, 1, Textures[1].textureResourceView.GetAddressOf());
    
    /*glm::mat4 ModelMatrix = glm::mat4(1);*/
    glm::mat4 ProjectionMatrix = glm::mat4(1);
    glm::mat4 ViewMatrix = glm::mat4(1);
    ProjectionMatrix = glm::perspectiveLH(glm::radians(45.0f), 1280.0f / 720.0f, 0.001f, 10000.0f);
    ViewMatrix = glm::lookAt(CAMERA->camera_position, CAMERA->camera_position + CAMERA->camera_front
        , CAMERA->camera_up);
    

    //XMMATRIX viewMatrix = XMMatrixLookAtLH(XMVectorSet(0.0f, 0.0f, -5.0f, 0.0f), XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f), XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));

    //XMMATRIX projectionMatrix = XMMatrixPerspectiveFovLH(XM_PIDIV4, 1280.f / 720.f, 0.001f, 100.f);

    //XMMATRIX mvpMatrix = TransformMatrix * viewMatrix * projectionMatrix;

    // Combine the matrices to create the MVP matrix

    const MVPConstantBuffer matrices = {
        {TransformMatrix},

        {ProjectionMatrix*ViewMatrix*TransformMatrix}


    };

    lightConstantBuffer lightPos;
    lightPos.light_pos = { Renderer::Get().GetSceneData().light_data.light_position,1 };
    vertex_buffer->Bind();
    index_buffer->Bind();
    PixelConstantBuffer lightBuff(lightPos);
    VertexConstantBuffer mvpBuff(matrices);
    mvpBuff.Bind();
    lightBuff.Update(lightPos);
    lightBuff.Bind();


    const std::vector<D3D11_INPUT_ELEMENT_DESC> ied = {
    {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(Vertex, Position), D3D11_INPUT_PER_VERTEX_DATA, 0},
    {"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(Vertex, Normal), D3D11_INPUT_PER_VERTEX_DATA, 0},
    {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, offsetof(Vertex,TexCoords), D3D11_INPUT_PER_VERTEX_DATA, 0}
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