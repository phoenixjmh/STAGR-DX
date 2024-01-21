#include "InputLayout.h"
#include "Renderer.h"

InputLayout::InputLayout(
	const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout,
	ID3DBlob* pVertexShaderBytecode)
{
	

	Renderer::Get().GetDevice()->CreateInputLayout(
		layout.data(), (UINT)layout.size(),
		pVertexShaderBytecode->GetBufferPointer(),
		pVertexShaderBytecode->GetBufferSize(),
		&m_InputLayout
	);
}

void InputLayout::Bind() 
{
	Renderer::Get().GetContext()->IASetInputLayout(m_InputLayout.Get());
}