#include "IndexBuffer.h"
#include "Renderer.h"

IndexBuffer::IndexBuffer( const std::vector<unsigned int>& indices)
    :
    m_Count((UINT)indices.size())

{

    D3D11_BUFFER_DESC bd = {};
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.CPUAccessFlags = 0u;
    bd.MiscFlags = 0u;
    bd.ByteWidth = UINT(m_Count * sizeof(unsigned int));
    bd.StructureByteStride = sizeof(unsigned int);
    D3D11_SUBRESOURCE_DATA sd = {};
    sd.pSysMem = indices.data();
    Renderer::Get().GetDevice()->CreateBuffer(&bd, &sd, &Buffer);
}

void IndexBuffer::Bind() 
{
    Renderer::Get().GetContext()->IASetIndexBuffer(Buffer.Get(), DXGI_FORMAT_R32_UINT, 0u);

}