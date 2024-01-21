#pragma once
#include "WinInclude.h"
#include <vector>


class IndexBuffer{
public:
    IndexBuffer( const std::vector<unsigned int>& indices);

    void Bind() ;

    inline UINT GetCount()const { return m_Count; }

    ComPtr<ID3D11Buffer> Buffer;


protected:
    unsigned int m_Count;

};

