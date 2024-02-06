#pragma once
#include "WinInclude.h"
#include "glm/glm.hpp"
//#include "DirectXMath.h"

struct MVPConstantBuffer {
     glm::mat4 ModelViewProjection;
     glm::mat4 ModelMatrix;
};

struct Vec3PosBuffer {
    alignas(16)glm::vec3 position;
};



struct ConstantBuffer2 {
    struct {
        float r;
        float g;
        float b;
        float a;
    } face_colors[6];
};
class ConstantBuffer {
public:
    template<typename C>
    void Update(const C& consts)
    {

        D3D11_MAPPED_SUBRESOURCE msr;
        GetContext()->Map(
            CBuffer.Get(), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &msr
        );
        memcpy(msr.pData, &consts, sizeof(consts));
        GetContext()->Unmap(CBuffer.Get(), 0u);
    }
    template <typename C>
    ConstantBuffer(const C& consts)
    {
        D3D11_BUFFER_DESC cbd = {};
        cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        cbd.Usage = D3D11_USAGE_DYNAMIC;
        cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        cbd.MiscFlags = 0u;
        cbd.ByteWidth = sizeof(consts);
        cbd.StructureByteStride = 0u;

        D3D11_SUBRESOURCE_DATA csd = {};
        csd.pSysMem = &consts;
       GetDevice()->CreateBuffer(&cbd, &csd, &CBuffer);
    }
    template <typename C>
    ConstantBuffer()
    {
        D3D11_BUFFER_DESC cbd = {};
        cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        cbd.Usage = D3D11_USAGE_DYNAMIC;
        cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        cbd.MiscFlags = 0u;
        cbd.ByteWidth = sizeof(C);
        cbd.StructureByteStride = 0u;

        //D3D11_SUBRESOURCE_DATA csd = {};
        GetDevice()->CreateBuffer(&cbd, nullptr, &CBuffer);
    }


   
   




    ComPtr<ID3D11Device>& GetDevice();

    ComPtr<ID3D11DeviceContext>& GetContext();

    ComPtr<ID3D11Buffer> CBuffer;
};


class VertexConstantBuffer : public ConstantBuffer {

    using ConstantBuffer::CBuffer;


public:




    void Bind(unsigned int slot) 
    {
        GetContext()->VSSetConstantBuffers(slot, 1u, CBuffer.GetAddressOf());
    }
};


class PixelConstantBuffer : public ConstantBuffer {

    using ConstantBuffer::CBuffer;


public:

    void Bind(unsigned int slot)
    {
        GetContext()->PSSetConstantBuffers(slot, 1u, CBuffer.GetAddressOf());
    }
};


