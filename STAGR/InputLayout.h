#pragma once
#include <vector>
#include "WinInclude.h"
class InputLayout {
public:
	InputLayout(
		const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout,
		ID3DBlob* pVertexShaderBytecode);
	void Bind();
private:
	ComPtr<ID3D11InputLayout> m_InputLayout;
};

