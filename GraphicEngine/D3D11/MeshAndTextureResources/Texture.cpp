#include "Texture.h"




Texture::Texture(std::wstring texture_name, TEXTURE_TYPE texture_Type, const int& u_ID)
	:
	textureName(texture_name),
	textureType(texture_Type),
	uID(u_ID)
{
	if (textureName.empty() || u_ID < 0)
		throw NORMAL_EXCEPT("TExture constructor failed..");
}

Texture::~Texture()
{
	if (pTexture)
	{
		pTexture->Release();
		pTexture = nullptr;
	}

	if (pShader_res_view)
	{
		pShader_res_view->Release();
		pShader_res_view = nullptr;
	}
	if (pSamplerState)
	{
		pSamplerState->Release();
		pSamplerState = nullptr;
	}
	if (pRTV)
	{
		pRTV->Release();
		pRTV = nullptr;
	}
	if (pDSV)
	{
		pDSV->Release();
		pDSV = nullptr;
	}

}

const std::wstring& Texture::GetTextureName()
{
	return textureName;
}

const int& Texture::getTextureID()
{
	return uID;
}

const TEXTURE_TYPE& Texture::getTextureType()
{
	return this->textureType;
}

void Texture::ReleaseAll()
{
	if (pTexture)
	{
		pTexture->Release();
		pTexture = nullptr;
	}

	if (pShader_res_view)
	{
		pShader_res_view->Release();
		pShader_res_view = nullptr;
	}
	if (pSamplerState)
	{
		pSamplerState->Release();
		pSamplerState = nullptr;
	}
	if (pRTV)
	{
		pRTV->Release();
		pRTV = nullptr;
	}
	if (pDSV)
	{
		pDSV->Release();
		pDSV = nullptr;
	}
}

void Texture::incrementCounter()
{
	UserCounter++;
}

void Texture::decrementCounter()
{
	UserCounter--;
}

ID3D11Resource** Texture::GetTextureResourcePP()
{
	return &pTexture;
}

ID3D11ShaderResourceView** Texture::GetShaderResourceViewPP()
{
	return &pShader_res_view;
}

ID3D11SamplerState** Texture::GetSamplerStatePP()
{
	return &pSamplerState;
}

ID3D11Resource* Texture::GetTextureResource()
{
	return this->pTexture;
}

ID3D11RenderTargetView* Texture::GetRenderTargetView()
{
	return this->pRTV;
}

ID3D11DepthStencilView* Texture::GetDepthStencilView()
{
	return this->pDSV;
}

ID3D11ShaderResourceView* Texture::GetShaderResourceView()
{
	return this->pShader_res_view;
}

ID3D11SamplerState* Texture::GetSamplerState()
{
	return pSamplerState;
}

RECT Texture::Get_Size() const
{
	return Size;
}
