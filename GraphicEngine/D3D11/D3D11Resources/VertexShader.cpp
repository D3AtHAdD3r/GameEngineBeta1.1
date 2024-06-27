#include "VertexShader.h"
#include<GraphicEngine/D3D11/D3D11Globals/D3D11Globals.h>

#include <d3dcompiler.h>
#pragma comment(lib,"D3DCompiler.lib")




VertexShader::VertexShader(std::wstring shaderfilename, std::string entryPointName, std::string shader_model, const int& u_ID)
	:
	file_name(shaderfilename),
	uID(u_ID)
{
	HRESULT hr;
	if (u_ID < 0 || shaderfilename.empty() || entryPointName.empty() || shader_model.empty())
		throw NORMAL_EXCEPT("VertexShader constructor failed");

	ID3DBlob* errblob = nullptr;

	if (FAILED(hr = D3DCompileFromFile(shaderfilename.c_str(), nullptr, nullptr, entryPointName.c_str(), shader_model.c_str(), D3DCOMPILE_DEBUG, NULL, &vsblob, &errblob)))
	{
		if (errblob)
			errblob->Release();

		throw WND_EXCEPT(hr);
	}

	GFX_THROW_INFO(D3D11Globals::Get()->GetDevice()->CreateVertexShader(vsblob->GetBufferPointer(), vsblob->GetBufferSize(), nullptr, &vShader));

	shader_byte_code = vsblob->GetBufferPointer();
	bufferSize = vsblob->GetBufferSize();

}

VertexShader::~VertexShader()
{
	if (vsblob)
	{
		vsblob->Release();
		vsblob = nullptr;
	}
	if (vShader)
	{
		vShader->Release();
		vShader = nullptr;
	}
}

ID3D11VertexShader* VertexShader::getVertexShader()
{
	return vShader;
}

std::wstring VertexShader::getfilename()
{
	return file_name;
}

void* VertexShader::GetShader_Byte_Code()
{
	return shader_byte_code;
}

const unsigned long long& VertexShader::GetBufferSize()
{
	return bufferSize;
}

void VertexShader::incrementCounter()
{
	UserCounter++;
}

void VertexShader::decrementCounter()
{
	UserCounter--;
}



