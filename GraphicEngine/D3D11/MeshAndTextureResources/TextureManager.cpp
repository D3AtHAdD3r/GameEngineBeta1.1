#include "TextureManager.h"
#include<GraphicEngine/D3D11/MeshAndTextureResources/Texture.h>
#include<GraphicEngine/D3D11/D3D11Globals/D3D11Globals.h>
#include<GraphicEngine/ECS/ECSHeaders/EntityStructs.h>
#include<GraphicEngine\D3D11\MeshAndTextureResources\Texture_Structs.h>
#include<GraphicEngine\Renderer\RendererDX11\RendererDX11.h>

#include <DirectXTex.h>
//#include<AssetsAndLibs\Libs\DirectXTex\include\DirectXTex.h>


TextureManager::TextureManager(const std::unordered_map<int, std::wstring>& File_Map)
	:
	FileMap(File_Map)
{
}

TextureManager::~TextureManager()
{
	for (auto& [uid, pTex] : TextureContainer_Concrete)
		if (pTex)
			delete pTex;

	for (auto& [uid, pTex] : TextureContainer_FrameBuffer)
		if (pTex)
			delete pTex;

	for (auto& [uid, pTex] : TextureContainer_BackBuffer)
		if (pTex)
			delete pTex;
}


Texture* TextureManager::CreateTexture(Texture_Creation_Details* tex_creation_data)
{
	if (tex_creation_data->u_ID < 0)
		return nullptr;

	switch (tex_creation_data->tex_type)
	{
	case TEXTURE_TYPE::Concrete_Texture:
	{
		return CreateTextureConcrete(tex_creation_data->u_ID);
		break;
	}
	case TEXTURE_TYPE::FrameBuffer:
	{
		return CreateTextureFromResourceViews(tex_creation_data->tex_name, tex_creation_data->width, tex_creation_data->height, tex_creation_data->u_ID);
		break;
	}
	case TEXTURE_TYPE::BackBuffer:
	{
		return CreateTextureFromBackBuffer(tex_creation_data->tex_name, tex_creation_data->buffer_index, tex_creation_data->u_ID);
		break;
	}
	case TEXTURE_TYPE::Unknown_Texture:
	{
		//check if exist, else ...
		Texture* pTex = getTexturebyUID(tex_creation_data->u_ID);
		if (pTex)
			return pTex;
		else
			return CreateTextureConcrete(tex_creation_data->u_ID);

		break;
	}

	default:
	{
		return nullptr;
	}
	}

}

Texture* TextureManager::CreateTextureConcrete(const int& u_ID)
{
	if (u_ID < 0) return nullptr;

	//Check if Exist
	if (TextureContainer_Concrete.find(u_ID) != TextureContainer_Concrete.end())
	{
		TextureContainer_Concrete[u_ID]->incrementCounter();
		return TextureContainer_Concrete[u_ID];
	}
	else if (TextureContainer_FrameBuffer.find(u_ID) != TextureContainer_FrameBuffer.end())
	{
		//TODO throw Exception
		return nullptr;
	}
	else if (TextureContainer_BackBuffer.find(u_ID) != TextureContainer_BackBuffer.end())
	{
		//TODO throw Exception
		return nullptr;
	}
	else
	{
		bool flag = false;
		std::wstring filePath = GetFileName(u_ID);
		if (filePath.empty())
			return nullptr;

		//getfilename(only)
		wchar_t fileName[FILENAME_MAX];
		wchar_t extensionName[FILENAME_MAX];
		std::wstring wstr;
		_wsplitpath_s(filePath.c_str(), 0, 0, 0, 0, fileName, FILENAME_MAX, extensionName, FILENAME_MAX);
		wstr.append(fileName); wstr.append(extensionName);

		Texture* currTexture = new Texture(wstr, TEXTURE_TYPE::Concrete_Texture, u_ID);
		if (!LoadTextureFromFile(filePath.c_str(), currTexture))
		{
			delete currTexture;
			return nullptr;
		}

		currTexture->incrementCounter();
		TextureContainer_Concrete.emplace(u_ID, currTexture);
		return currTexture;
	}
}

Texture* TextureManager::CreateTextureFromResourceViews(std::wstring tex_name, const unsigned int& width, const unsigned int& height, const int& u_ID)
{
	if (u_ID < 0) return nullptr;

	if (TextureContainer_FrameBuffer.find(u_ID) != TextureContainer_FrameBuffer.end())
	{
		TextureContainer_FrameBuffer[u_ID]->incrementCounter();
		return TextureContainer_FrameBuffer[u_ID];
	}
	else if (TextureContainer_BackBuffer.find(u_ID) != TextureContainer_BackBuffer.end())
	{
		return nullptr;  //TODO throw Exception
	}
	else if (TextureContainer_Concrete.find(u_ID) != TextureContainer_Concrete.end())
	{
		return nullptr; //TODO throw Exception
	}
	else
	{
		if (!width || !height || tex_name.empty()) return nullptr;

		Texture* currTexture = new Texture(tex_name, TEXTURE_TYPE::FrameBuffer, u_ID);

		ID3D11Texture2D* ptexture2D = nullptr;
		ID3D11RenderTargetView* pRTV = nullptr;
		ID3D11DepthStencilView* pDSV = nullptr;
		ID3D11ShaderResourceView* pSRV = nullptr;
		ID3D11SamplerState* m_sampler_state = nullptr;

		if (!CreateRenderTargetView(&ptexture2D, &pRTV, width, height))
		{
			delete currTexture;
			return nullptr;
		}
		if (!CreatedepthStensilView(&pDSV, width, height))
		{
			// free ptex, prtv etc (pending)
			delete currTexture;
			return nullptr;
		}
		if (!CreateShaderResourceViewFromTexture(ptexture2D, &pSRV))
		{
			// free ptex, prtv etc (pending)
			delete currTexture;
			return nullptr;
		}
		if (!CreateSamplerState(&m_sampler_state))
		{
			// free ptex, prtv etc (pending)
			delete currTexture;
			return nullptr;
		}

		currTexture->pTexture = ptexture2D;
		currTexture->pShader_res_view = pSRV;
		currTexture->pSamplerState = m_sampler_state;
		currTexture->pRTV = pRTV;
		currTexture->pDSV = pDSV;
		currTexture->incrementCounter();

		TextureContainer_FrameBuffer.emplace(u_ID, currTexture);
		return currTexture;
	}
}

Texture* TextureManager::CreateTextureFromBackBuffer(std::wstring tex_name, const short& buffer_index, const int& u_ID)
{
	if (u_ID < 0) return nullptr;

	if (TextureContainer_BackBuffer.find(u_ID) != TextureContainer_BackBuffer.end())
	{
		TextureContainer_BackBuffer[u_ID]->incrementCounter();
		return TextureContainer_BackBuffer[u_ID];
	}
	else if (TextureContainer_FrameBuffer.find(u_ID) != TextureContainer_FrameBuffer.end())
	{
		return nullptr;  //TODO throw Exception
	}
	else if (TextureContainer_Concrete.find(u_ID) != TextureContainer_Concrete.end())
	{
		return nullptr; //TODO throw Exception
	}
	else
	{
		if (tex_name.empty()) return nullptr;

		Texture* currTexture = new Texture(tex_name, TEXTURE_TYPE::BackBuffer, u_ID);
		ID3D11RenderTargetView* pRenderTargetView = nullptr;
		ID3D11DepthStencilView* pDepthStencilView = nullptr;

		if (!CreateRenderTargetView_BackBuffer(&pRenderTargetView, buffer_index))
			return nullptr;

		unsigned int Width = RendererDX11::Get()->Get_WindowWidth();
		unsigned int height = RendererDX11::Get()->Get_WindowHeight();

		if (!CreatedepthStensilView(&pDepthStencilView, Width, height))
		{
			if (pRenderTargetView)
			{
				pRenderTargetView->Release();
				pRenderTargetView = nullptr;
			}

			return nullptr;
		}

		currTexture->BackBufferIndex = buffer_index;
		currTexture->pRTV = pRenderTargetView;
		currTexture->pDSV = pDepthStencilView;
		currTexture->pTexture = nullptr;
		currTexture->pShader_res_view = nullptr;
		currTexture->pSamplerState = nullptr;

		currTexture->incrementCounter();
		TextureContainer_BackBuffer.emplace(u_ID, currTexture);
		return currTexture;
	}
}

bool TextureManager::UpdateTextureOnResize(const int& u_ID, const unsigned int& width, const unsigned int& height)
{
	HRESULT hr;

	if (width == 0 || height == 0) return false;
	if (u_ID < 0 ) return false;

	Texture* pTexture = nullptr;
	pTexture = getTexturebyUID(u_ID);
	if (!pTexture) return false;
	

	if ((pTexture->textureType != TEXTURE_TYPE::FrameBuffer) && (pTexture->textureType != TEXTURE_TYPE::BackBuffer)) return false;

	if (pTexture->textureType == TEXTURE_TYPE::BackBuffer)
	{
		if (pTexture->pRTV)
		{
			pTexture->pRTV->Release();
			pTexture->pRTV = nullptr;
		}

		if (pTexture->pDSV)
		{
			pTexture->pDSV->Release();
			pTexture->pDSV = nullptr;
		}

		GFX_THROW_INFO(D3D11Globals::Get()->GetSwapChain()->ResizeBuffers(1, RendererDX11::Get()->Get_WindowWidth(), RendererDX11::Get()->Get_WindowHeight(), DXGI_FORMAT_R8G8B8A8_UNORM, 0));

		ID3D11RenderTargetView* pRenderTargetView = nullptr;
		ID3D11DepthStencilView* p_DepthStencilView = nullptr;

		if (!CreateRenderTargetView_BackBuffer(&pRenderTargetView, pTexture->BackBufferIndex))
			return false;

		if (!CreatedepthStensilView(&p_DepthStencilView, RendererDX11::Get()->Get_WindowWidth(), RendererDX11::Get()->Get_WindowHeight()))
		{
			if (pRenderTargetView)
			{
				pRenderTargetView->Release();
				pRenderTargetView = nullptr;
			}

			return false;
		}

		pTexture->pTexture = nullptr;
		pTexture->pRTV = pRenderTargetView;
		pTexture->pDSV = p_DepthStencilView;
		pTexture->pShader_res_view = nullptr;
		pTexture->pSamplerState = nullptr;

		return true;
	}
	else if (pTexture->textureType == TEXTURE_TYPE::FrameBuffer)
	{
		if (pTexture->pTexture)
		{
			pTexture->pTexture->Release();
			pTexture->pTexture = nullptr;
		}

		if (pTexture->pRTV)
		{
			pTexture->pRTV->Release();
			pTexture->pRTV = nullptr;
		}

		if (pTexture->pDSV)
		{
			pTexture->pDSV->Release();
			pTexture->pDSV = nullptr;
		}

		if (pTexture->pShader_res_view)
		{
			pTexture->pShader_res_view->Release();
			pTexture->pShader_res_view = nullptr;
		}

		if (pTexture->pSamplerState)
		{
			pTexture->pSamplerState->Release();
			pTexture->pSamplerState = nullptr;
		}

		ID3D11Texture2D* ptexture2D = nullptr;
		ID3D11RenderTargetView* pRTV = nullptr;
		ID3D11DepthStencilView* pDSV = nullptr;
		ID3D11ShaderResourceView* pSRV = nullptr;
		ID3D11SamplerState* m_sampler_state = nullptr;

		if (!CreateRenderTargetView(&ptexture2D, &pRTV, width, height))
			return false;

		if (!CreatedepthStensilView(&pDSV, width, height))
			return false;

		if (!CreateShaderResourceViewFromTexture(ptexture2D, &pSRV))
			return false;

		if (!CreateSamplerState(&m_sampler_state))
			return false;

		pTexture->pTexture = ptexture2D;
		pTexture->pShader_res_view = pSRV;
		pTexture->pSamplerState = m_sampler_state;
		pTexture->pRTV = pRTV;
		pTexture->pDSV = pDSV;

		return true;
	}

	return false;
}

const std::unordered_map<int, Texture*>& TextureManager::GetTextureList_Concrete()
{
	return TextureContainer_Concrete;
}

const std::unordered_map<int, Texture*>& TextureManager::GetTextureList_FrameBuffer()
{
	return TextureContainer_FrameBuffer;
}

const std::unordered_map<int, Texture*>& TextureManager::GetTextureList_BackBuffer()
{
	return TextureContainer_BackBuffer;
}

void TextureManager::releaseAll()
{
	for (auto& [value, pTex] : TextureContainer_Concrete)
	{
		if (pTex)
		{
			delete pTex;
			pTex = nullptr;
		}

	}

	TextureContainer_Concrete.clear();

	for (auto& [value, pTex] : TextureContainer_FrameBuffer)
	{
		if (pTex)
		{
			delete pTex;
			pTex = nullptr;
		}

	}

	TextureContainer_FrameBuffer.clear();

	for (auto& [value, pTex] : TextureContainer_BackBuffer)
	{
		if (pTex)
		{
			delete pTex;
			pTex = nullptr;
		}

	}

	TextureContainer_BackBuffer.clear();
}

bool TextureManager::check_Exist(const int& u_id)
{
	if (u_id < 0) return false;
	if (TextureContainer_Concrete.find(u_id) != TextureContainer_Concrete.end())
		return true;
	if (TextureContainer_FrameBuffer.find(u_id) != TextureContainer_FrameBuffer.end())
		return true;
	if (TextureContainer_BackBuffer.find(u_id) != TextureContainer_BackBuffer.end())
		return true;

	return false;
}

bool TextureManager::freeTexture(Texture* pTex)
{
	if (!pTex) return false;
	switch (pTex->textureType)
	{
	case TEXTURE_TYPE::Concrete_Texture:
	{
		if (TextureContainer_Concrete.find(pTex->uID) != TextureContainer_Concrete.end())
		{
			pTex->decrementCounter();
			if (pTex->UserCounter <= 0)
				deleteTexture_Unsafe(pTex);
			return true;
		}
		return false;
		break;
	}
	case TEXTURE_TYPE::FrameBuffer:
	{
		if (TextureContainer_FrameBuffer.find(pTex->uID) != TextureContainer_FrameBuffer.end())
		{
			pTex->decrementCounter();
			if (pTex->UserCounter <= 0)
				deleteTexture_Unsafe(pTex);
			return true;
		}
		return false;
		break;
	}
	case TEXTURE_TYPE::BackBuffer:
	{
		if (TextureContainer_BackBuffer.find(pTex->uID) != TextureContainer_BackBuffer.end())
		{
			pTex->decrementCounter();
			if (pTex->UserCounter <= 0)
				deleteTexture_Unsafe(pTex);
			return true;
		}
		return false;
		break;
	}
	default:
		return false;
	}
}

bool TextureManager::deleteTexture(Texture* pTex)
{
	if (!pTex) return false;
	switch (pTex->textureType)
	{
	case TEXTURE_TYPE::Concrete_Texture:
	{
		if (TextureContainer_Concrete.find(pTex->uID) != TextureContainer_Concrete.end())
		{
			delete pTex;
			TextureContainer_Concrete.erase(pTex->uID);
			return true;
		}
		return false;
		break;
	}
	case TEXTURE_TYPE::FrameBuffer:
	{
		if (TextureContainer_FrameBuffer.find(pTex->uID) != TextureContainer_FrameBuffer.end())
		{
			delete pTex;
			TextureContainer_FrameBuffer.erase(pTex->uID);
			return true;
		}
		return false;
		break;
	}
	case TEXTURE_TYPE::BackBuffer:
	{
		if (TextureContainer_BackBuffer.find(pTex->uID) != TextureContainer_BackBuffer.end())
		{
			delete pTex;
			TextureContainer_BackBuffer.erase(pTex->uID);
			return true;
		}
		return false;
		break;
	}
	default:
		return false;
	}
}

bool TextureManager::deleteTexture(const int& u_id)
{
	if (u_id < 0) return false;

	if (TextureContainer_Concrete.find(u_id) != TextureContainer_Concrete.end())
	{
		delete TextureContainer_Concrete[u_id];
		TextureContainer_Concrete.erase(u_id);
		return true;
	}

	if (TextureContainer_FrameBuffer.find(u_id) != TextureContainer_FrameBuffer.end())
	{
		delete TextureContainer_FrameBuffer[u_id];
		TextureContainer_FrameBuffer.erase(u_id);
		return true;
	}
	if (TextureContainer_BackBuffer.find(u_id) != TextureContainer_BackBuffer.end())
	{
		delete TextureContainer_BackBuffer[u_id];
		TextureContainer_BackBuffer.erase(u_id);
		return true;
	}

	return false;
}

Texture* TextureManager::getTexturebyName(std::wstring tex_name)
{
	if (tex_name.empty()) return nullptr;

	for (auto itr = TextureContainer_Concrete.begin(); itr != TextureContainer_Concrete.end(); ++itr)
	{
		if (itr->second->textureName == tex_name)
			return itr->second;
	}

	for (auto itr = TextureContainer_FrameBuffer.begin(); itr != TextureContainer_FrameBuffer.end(); ++itr)
	{
		if (itr->second->textureName == tex_name)
			return itr->second;
	}

	for (auto itr = TextureContainer_BackBuffer.begin(); itr != TextureContainer_BackBuffer.end(); ++itr)
	{
		if (itr->second->textureName == tex_name)
			return itr->second;
	}

	return nullptr;
}



Texture* TextureManager::getTexturebyUID(const int& u_id)
{
	if (u_id < 0) return nullptr;

	for (auto itr = TextureContainer_Concrete.begin(); itr != TextureContainer_Concrete.end(); ++itr)
	{
		if (itr->second->uID == u_id)
			return itr->second;
	}

	for (auto itr = TextureContainer_FrameBuffer.begin(); itr != TextureContainer_FrameBuffer.end(); ++itr)
	{
		if (itr->second->uID == u_id)
			return itr->second;
	}

	for (auto itr = TextureContainer_BackBuffer.begin(); itr != TextureContainer_BackBuffer.end(); ++itr)
	{
		if (itr->second->uID == u_id)
			return itr->second;
	}

	return nullptr;
}

void TextureManager::deleteTexture_Unsafe(Texture* pTex)
{
	switch (pTex->textureType)
	{
	case TEXTURE_TYPE::Concrete_Texture:
	{
		delete pTex;
		TextureContainer_Concrete.erase(pTex->uID);
		break;
	}
	case TEXTURE_TYPE::FrameBuffer:
	{
		delete pTex;
		TextureContainer_FrameBuffer.erase(pTex->uID);
		break;
	}
	case TEXTURE_TYPE::BackBuffer:
	{
		delete pTex;
		TextureContainer_BackBuffer.erase(pTex->uID);
		break;
	}
	default:
		break;
	}
}

bool TextureManager::LoadTextureFromFile(const wchar_t* full_path, ID3D11Resource** pTexture, ID3D11ShaderResourceView** pShader_res_view, ID3D11SamplerState** pSampler_State)
{
	if (!full_path) return false;
	ID3D11Resource* m_texture = nullptr;
	ID3D11ShaderResourceView* m_shader_res_view = nullptr;
	ID3D11SamplerState* m_sampler_state = nullptr;

	ID3D11Device* pDevice = D3D11Globals::Get()->GetDevice();

	DirectX::ScratchImage image_data;

	HRESULT hr = DirectX::LoadFromWICFile(full_path, DirectX::WIC_FLAGS_IGNORE_SRGB,
		nullptr, image_data);

	if (SUCCEEDED(hr))
	{
		hr = DirectX::CreateTexture(pDevice, image_data.GetImages(),
			image_data.GetImageCount(), image_data.GetMetadata(), &m_texture);
		if (!SUCCEEDED(hr))
			throw WND_EXCEPT(hr);

		D3D11_SHADER_RESOURCE_VIEW_DESC desc = {};
		desc.Format = image_data.GetMetadata().format;
		desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MipLevels = (UINT)image_data.GetMetadata().mipLevels;
		desc.Texture2D.MostDetailedMip = 0;

		D3D11_SAMPLER_DESC sampler_desc = {};
		sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		sampler_desc.Filter = D3D11_FILTER_ANISOTROPIC;
		sampler_desc.MinLOD = 0.0f;
		sampler_desc.MaxLOD = (FLOAT)image_data.GetMetadata().mipLevels;

		GFX_THROW_INFO(pDevice->CreateSamplerState(&sampler_desc, &m_sampler_state));

		GFX_THROW_INFO(pDevice->CreateShaderResourceView(m_texture, &desc, &m_shader_res_view));
	}
	else
	{
		throw WND_EXCEPT(hr);
	}

	*pTexture = m_texture;
	*pShader_res_view = m_shader_res_view;
	*pSampler_State = m_sampler_state;

	return true;
}

bool TextureManager::LoadTextureFromFile(const wchar_t* full_path, Texture* pTexture)
{
	if (!pTexture || !full_path) return false;
	ID3D11Resource* m_texture = nullptr;
	ID3D11ShaderResourceView* m_shader_res_view = nullptr;
	ID3D11SamplerState* m_sampler_state = nullptr;

	ID3D11Device* pDevice = D3D11Globals::Get()->GetDevice();

	DirectX::ScratchImage image_data;

	HRESULT hr = DirectX::LoadFromWICFile(full_path, DirectX::WIC_FLAGS_IGNORE_SRGB,
		nullptr, image_data);

	if (SUCCEEDED(hr))
	{
		hr = DirectX::CreateTexture(pDevice, image_data.GetImages(),
			image_data.GetImageCount(), image_data.GetMetadata(), &m_texture);
		if (!SUCCEEDED(hr))
			throw WND_EXCEPT(hr);

		D3D11_SHADER_RESOURCE_VIEW_DESC desc = {};
		desc.Format = image_data.GetMetadata().format;
		desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MipLevels = (UINT)image_data.GetMetadata().mipLevels;
		desc.Texture2D.MostDetailedMip = 0;

		D3D11_SAMPLER_DESC sampler_desc = {};
		sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		sampler_desc.Filter = D3D11_FILTER_ANISOTROPIC;
		sampler_desc.MinLOD = 0.0f;
		sampler_desc.MaxLOD = (FLOAT)image_data.GetMetadata().mipLevels;

		GFX_THROW_INFO(pDevice->CreateSamplerState(&sampler_desc, &m_sampler_state));

		GFX_THROW_INFO(pDevice->CreateShaderResourceView(m_texture, &desc, &m_shader_res_view));
	}
	else
	{
		throw WND_EXCEPT(hr);
	}

	pTexture->pTexture = m_texture;
	pTexture->pShader_res_view = m_shader_res_view;
	pTexture->pSamplerState = m_sampler_state;

	pTexture->Size = { 0,0, (long)image_data.GetMetadata().width, (long)image_data.GetMetadata().height };
	return true;
}

bool TextureManager::CreateRenderTargetView(ID3D11Texture2D** p_texture, ID3D11RenderTargetView** p_RenderTargetView, const unsigned int& width, const unsigned int& height)
{
	HRESULT hr;
	ID3D11Texture2D* pTexture = nullptr;
	ID3D11RenderTargetView* pRenderTargetView = nullptr;
	D3D11_TEXTURE2D_DESC textureDesc;
	//pTexture->GetDesc(&textureDesc);
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;
	textureDesc.ArraySize = 1;
	textureDesc.MipLevels = 1;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.MiscFlags = 0;

	GFX_THROW_INFO(D3D11Globals::Get()->GetDevice()->CreateTexture2D(&textureDesc, nullptr, &pTexture));
	GFX_THROW_INFO(D3D11Globals::Get()->GetDevice()->CreateRenderTargetView(pTexture, nullptr, &pRenderTargetView));

	*p_texture = pTexture;
	*p_RenderTargetView = pRenderTargetView;
	return true;
}

bool TextureManager::CreatedepthStensilView(ID3D11DepthStencilView** p_DepthStencilView, const unsigned int& width, const unsigned int& height)
{
	HRESULT hr;
	ID3D11Texture2D* buffer = nullptr;
	ID3D11DepthStencilView* pDepthStencilView = nullptr;

	D3D11_TEXTURE2D_DESC tex_desc = {};
	tex_desc.Width = width;
	tex_desc.Height = height;
	tex_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	tex_desc.Usage = D3D11_USAGE_DEFAULT;
	tex_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	tex_desc.MipLevels = 1;
	tex_desc.SampleDesc.Count = 1;
	tex_desc.SampleDesc.Quality = 0;
	tex_desc.MiscFlags = 0;
	tex_desc.ArraySize = 1;
	tex_desc.CPUAccessFlags = 0;

	GFX_THROW_INFO(D3D11Globals::Get()->GetDevice()->CreateTexture2D(&tex_desc, nullptr, &buffer));
	GFX_THROW_INFO(D3D11Globals::Get()->GetDevice()->CreateDepthStencilView(buffer, NULL, &pDepthStencilView));

	*p_DepthStencilView = pDepthStencilView;
	return true;
}

bool TextureManager::CreateShaderResourceViewFromTexture(ID3D11Texture2D* ptexture, ID3D11ShaderResourceView** pShader_res_view)
{
	HRESULT hr;
	ID3D11ShaderResourceView* pSRV = nullptr;
	GFX_THROW_INFO(D3D11Globals::Get()->GetDevice()->CreateShaderResourceView(ptexture, nullptr, &pSRV));

	*pShader_res_view = pSRV;
	return true;
}

bool TextureManager::CreateSamplerState(ID3D11SamplerState** m_sampler_state)
{
	HRESULT hr;

	D3D11_SAMPLER_DESC sampler_desc = {};
	sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.Filter = D3D11_FILTER_ANISOTROPIC;
	sampler_desc.MinLOD = 0.0f;
	sampler_desc.MaxLOD = 1.0f;

	GFX_THROW_INFO(D3D11Globals::Get()->GetDevice()->CreateSamplerState(&sampler_desc, m_sampler_state));
	return true;
}

bool TextureManager::CreateRenderTargetView_BackBuffer(ID3D11RenderTargetView** pRenderTargetView, const unsigned short& buffer_index)
{
	HRESULT hr;
	ID3D11Texture2D* buffer = NULL;

	GFX_THROW_INFO(D3D11Globals::Get()->GetSwapChain()->GetBuffer(buffer_index, __uuidof(ID3D11Texture2D), (void**)&buffer));
	GFX_THROW_INFO(D3D11Globals::Get()->GetDevice()->CreateRenderTargetView(buffer, NULL, pRenderTargetView));
	buffer->Release();
	return true;
}

std::wstring TextureManager::GetFileName(const int& uID)
{
	if (uID < 0) return std::wstring();

	auto itr = FileMap.find(uID);
	if (itr != FileMap.end())
		return itr->second;
	else
		return std::wstring();
}
