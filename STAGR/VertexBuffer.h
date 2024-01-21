#pragma once
#include "glm/glm.hpp"
#include <vector>
#include "WinInclude.h"
#include "DirectXMath.h"
enum PrimitiveShape { NONE, PLANE, CUBE, SPHERE };
using namespace DirectX;
struct Vertex {
    Vertex(glm::vec3 pos, glm::vec3 norm,XMFLOAT2 texCoord) :
        Position(pos),
        Normal(norm),
        TexCoords(texCoord)
    {}
    Vertex() {}
    glm::vec3 Position;
    glm::vec3 Normal;
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