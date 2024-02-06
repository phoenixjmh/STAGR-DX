#pragma once
#include "glm/glm.hpp"
#include <vector>
#include "WinInclude.h"
#include "DirectXMath.h"
enum PrimitiveShape { NONE, PLANE, CUBE, SPHERE };
using namespace DirectX;
struct Vertex {
    Vertex(glm::vec3 pos, glm::vec3 norm,XMFLOAT2 texCoord,glm::vec3 tangent,glm::vec3 bitangent) :
        Position(pos),
        Normal(norm),
        TexCoords(texCoord),
        Tangents(tangent),
        BiTangents(bitangent)
    {}
    Vertex() {}
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec3 Tangents;
    glm::vec3 BiTangents;
    XMFLOAT2 TexCoords;

};

class VertexBuffer {

public:

    
    VertexBuffer(const std::vector<Vertex>& vertices);
    void Bind() ;

    ComPtr<ID3D11Buffer> Buffer;
    UINT m_Stride;
    UINT m_Offset = 0u;


};