#pragma once
#include <string>
#include "WinInclude.h"
class Shader {


public:
	Shader(const std::wstring& vert_path, const std::wstring& frag_path);
	void Bind();
	ID3DBlob* GetVertexBytecode();
private:
	ComPtr<ID3D11PixelShader> m_PixelShader;
	ComPtr<ID3D11VertexShader> m_VertexShader;
	ComPtr<ID3DBlob> m_ByteCodeBlob;
};