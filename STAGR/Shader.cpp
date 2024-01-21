#include "Shader.h"
#include <d3dcompiler.h>
#include "Renderer.h"

Shader::Shader(const std::wstring& vert_path, const std::wstring& frag_path)
{

    Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
    D3DReadFileToBlob(frag_path.c_str(), &pBlob);
    Renderer::Get().GetDevice()->CreatePixelShader(pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		nullptr, &m_PixelShader);

	

	D3DReadFileToBlob(vert_path.c_str(), &m_ByteCodeBlob);
	Renderer::Get().GetDevice()->CreateVertexShader(
		m_ByteCodeBlob->GetBufferPointer(),
		m_ByteCodeBlob->GetBufferSize(),
		nullptr,
		&m_VertexShader
	);

}

void Shader::Bind()
{
	Renderer::Get().GetContext()->PSSetShader(m_PixelShader.Get(), nullptr, 0u);
	Renderer::Get().GetContext()->VSSetShader(m_VertexShader.Get(), nullptr, 0u);
}


ID3DBlob* Shader::GetVertexBytecode() 
{
	return m_ByteCodeBlob.Get();
}
