#include "VertexBuffer.h"
#include "Renderer.h"

VertexBuffer::VertexBuffer(const std::vector<Vertex>& vertices)
    :
    m_Stride(sizeof(Vertex))
{

    D3D11_BUFFER_DESC bd = {};
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.CPUAccessFlags = 0u;
    bd.MiscFlags = 0u;
    bd.ByteWidth = sizeof(Vertex) * vertices.size();
    bd.StructureByteStride = sizeof(Vertex);
    D3D11_SUBRESOURCE_DATA sd = {};
    sd.pSysMem = vertices.data();
    Renderer::Get().GetDevice()->CreateBuffer(&bd, &sd, &Buffer);

}



void VertexBuffer::Bind()
{
    Renderer::Get().GetContext()->IASetVertexBuffers(0u, 1u, Buffer.GetAddressOf(), &m_Stride, &m_Offset);
}

