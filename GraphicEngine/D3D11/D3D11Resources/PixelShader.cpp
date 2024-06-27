#include "PixelShader.h"
#include<GraphicEngine/D3D11/D3D11Globals/D3D11Globals.h>
#include <d3dcompiler.h>




PixelShader::PixelShader(std::wstring shaderfilename, std::string entryPointName, std::string shader_model, const int& u_ID)
	:
	file_name(shaderfilename),
	uID(u_ID)
{
	if (u_ID < 0 || shaderfilename.empty() || entryPointName.empty() || shader_model.empty())
		throw NORMAL_EXCEPT("PixelShader constructor failed");

	HRESULT hr;
	ID3DBlob* errblob = nullptr;

	if (FAILED(hr = D3DCompileFromFile(shaderfilename.c_str(), nullptr, nullptr, entryPointName.c_str(), shader_model.c_str(), D3DCOMPILE_DEBUG, NULL, &psblob, &errblob)))
	{
		if (errblob)
			errblob->Release();

		throw WND_EXCEPT(hr);
	}

	GFX_THROW_INFO(D3D11Globals::Get()->GetDevice()->CreatePixelShader(psblob->GetBufferPointer(), psblob->GetBufferSize(), nullptr, &pShader));

	psblob->Release();
	psblob = nullptr;
}

PixelShader::~PixelShader()
{
	if (psblob)
	{
		psblob->Release();
		psblob = nullptr;
	}
	if (pShader)
	{
		pShader->Release();
		pShader = nullptr;
	}
}

ID3D11PixelShader* PixelShader::getPixelShader()
{
	return pShader;
}

std::wstring PixelShader::getfilename()
{
	return file_name;
}

void PixelShader::incrementCounter()
{
	UserCounter++;
}

void PixelShader::decrementCounter()
{
	UserCounter--;
}
