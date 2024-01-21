#pragma once
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "ConstantBuffer.h"
#include "Logger.h"
#include "Shader.h"
#include "InputLayout.h"
#include "glm/glm.hpp"
#include <string>
#include <vector>
using namespace Logger;







struct Texture {
    unsigned int slot;
    std::string type;
    std::string path;
    ComPtr<ID3D11ShaderResourceView> textureResourceView;
};

class Mesh {
public:
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices,glm::vec3 pos,std::vector<Texture>textures)
        :
        vertex_buffer(nullptr),
        index_buffer(nullptr),
        constant_buffer(nullptr),
        input_layout(nullptr),
        Vertices(vertices),
        Indices(indices),
        Transform(pos),
        Textures(textures)


    {
        setupMesh();
    }
    std::vector<Vertex> Vertices;
    std::vector<unsigned int> Indices;
    std::vector<Texture> Textures;
    glm::vec3 Transform;
    void Draw(Shader& shader, unsigned int depthMap);
    void printTextureInfo();
    void UpdateTransformMatrix(float size,glm::vec3 pos);
    void UpdateLightPosition(glm::vec3 light_pos);

    glm::mat4 TransformMatrix;
    glm::mat4 ModelMatrix;


    

private:

    void setupMesh();

    glm::vec4 m_lightPosition;
    VertexBuffer* vertex_buffer;
    IndexBuffer* index_buffer;
    PixelConstantBuffer* constant_buffer;
    InputLayout* input_layout;

};

