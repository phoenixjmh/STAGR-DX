#include "ConstantBuffer.h"
#include "Renderer.h"


ComPtr<ID3D11Device>& ConstantBuffer::GetDevice ()
{
    return Renderer::Get().GetDevice();
}


ComPtr<ID3D11DeviceContext>& ConstantBuffer::GetContext()
{
    return Renderer::Get().GetContext();
}
