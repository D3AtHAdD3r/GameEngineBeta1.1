#include "RendererDX11.h"
#include<GraphicEngine/D3D11/D3D11Core/D3D11Core.h>

#include<GraphicEngine/D3D11/MeshAndTextureResources/Texture.h>



RendererDX11::RendererDX11()
{
}

RendererDX11::~RendererDX11()
{
	if (pD3D11Core) delete pD3D11Core;
}

bool RendererDX11::Initialize(RenderData* pRenderData)
{
	if (!CheckRenderData(pRenderData)) return false;
	pD3D11Core = new D3D11Core(&(pRenderData->d3dInitData));
	initRasterizerState();
	return true;
}

bool RendererDX11::Init_Pre_Bind(Renderer_PreBindData* pData)
{
	if (!CheckPreBindData(pData)) return false;
	if (!SetPreBinds(pData)) return false;
	
	return true;
}

bool RendererDX11::Init_Main_Bind(Renderer_BindingData* pData)
{
	if (!CheckMainBindData(pData)) return false;
	if (!BindToPipeLine(pData)) return false;

	return true;
}

void RendererDX11::PresentFrame(bool Vsync)
{
	Present(Vsync);
}

bool RendererDX11::CheckRenderData(RenderData* pRenderData)
{
	if (!pRenderData->d3dInitData.Window_Height || !pRenderData->d3dInitData.Window_Width || !pRenderData->d3dInitData.hWnd || !pRenderData->d3dInitData.BufferCount) 
		return false;

	return true;
}

bool RendererDX11::CheckPreBindData(Renderer_PreBindData* pData)
{
	if (!pData->pRTV || !pData->scene_width || !pData->scene_height)
		return false;
	if (pData->UseDepthStencil && !pData->pDSV)
		return false;


	return true;
}

bool RendererDX11::CheckMainBindData(Renderer_BindingData* pData)
{
	if (!pData->vBuffer || !pData->cbuffer || !pData->iLayout || 
		!pData->pShader || !pData->size_vertex || !pData->vShader || 
		!pData->indexbuffer || !pData->pCBuffer_data || 
		pData->TexBindType == TEXTURE_BINDING_TYPE::unknown || 
		!pData->MaterialCount)
		return false;

	if (pData->Material_Draw_Details.size() != pData->MaterialCount) return false;

	return true;
}

bool RendererDX11::SetPreBinds(Renderer_PreBindData* pData)
{
	float color[4];
	color[0] = pData->color[0], color[1] = pData->color[1], color[2] = pData->color[2], color[3] = pData->color[3];

	if (pData->ClearRenderTargetView)
	{
		clearRenderTargetView(pData->pRTV, color);
	}
	if (pData->ClearDepthStencil)
	{
		clearDepthStencilView(pData->pDSV);
	}

	if (pData->UseDepthStencil)
	{
		setRenderTargetView(pData->pRTV, pData->pDSV);
	}
	else
	{
		setRenderTargetView(pData->pRTV, nullptr);
	}

	setViewport(pData->scene_width, pData->scene_height);
	setPrimitiveTopology(pData->PrimitiveTopology);

	return true;
}

bool RendererDX11::BindToPipeLine(Renderer_BindingData* pData)
{
	setRasterizerState(pData->FrontFaceCull);

	pD3D11Core->pContext->UpdateSubresource(pData->cbuffer, NULL, NULL, pData->pCBuffer_data, NULL, NULL);

	if (binded_cb != pData->cbuffer)
	{
		setConstantBuffer(pData->cbuffer); 
		binded_cb = pData->cbuffer;
	}

	if (binded_vs != pData->vShader)
	{
		setVertexShader(pData->vShader);
		binded_vs = pData->vShader;
	}

	if (binded_ps != pData->pShader)
	{
		setPixelShader(pData->pShader);
		binded_ps = pData->pShader;
	}

	setVertexBufferandLayout(pData->size_vertex, pData->vBuffer, pData->iLayout);
	setIndexBuffer(pData->indexbuffer);

	//Textures
	switch (pData->TexBindType)
	{
	case TEXTURE_BINDING_TYPE::oneTexMap_OneNormalMap_perDrawCall:
	{
		for (size_t m = 0; m < pData->MaterialCount; ++m)
		{
			setTextureResourceVertexShader_normal_included(pData->list_textures[m], pData->list_textures_normal[m]);
			setTextureResourcePixelShader_normal_included(pData->list_textures[m], pData->list_textures_normal[m]);

			UINT start_index_location = (UINT)(pData->Material_Draw_Details[m].first);
			UINT index_count = (UINT)(pData->Material_Draw_Details[m].second);
			drawIndexedTriangleList(index_count, 0, start_index_location);
		}
		break;
	}

	case TEXTURE_BINDING_TYPE::oneTexMap_perDrawCall:
	{
		for (size_t m = 0; m < pData->MaterialCount; ++m)
		{
			setTextureResourceVertexShader(pData->list_textures[m]);
			setTextureResourcePixelShader(pData->list_textures[m]);

			UINT start_index_location = (UINT)(pData->Material_Draw_Details[m].first);
			UINT index_count = (UINT)(pData->Material_Draw_Details[m].second);
			drawIndexedTriangleList(index_count, 0, start_index_location);
		}
		break;
	}

	case TEXTURE_BINDING_TYPE::allTexMaps_perDrawCall:
	{
		setTextureResourceVertexShader(pData->list_textures);
		setTextureResourcePixelShader(pData->list_textures);
		UINT start_index_location = (UINT)(pData->Material_Draw_Details[0].first);
		UINT index_count = (UINT)(pData->Material_Draw_Details[0].second);
		drawIndexedTriangleList(index_count, 0, start_index_location);
	}
	break;
	}

	return true;
}


void RendererDX11::setViewport(UINT width, UINT height)
{
	D3D11_VIEWPORT vp = {};
	vp.Width = (FLOAT)width;
	vp.Height = (FLOAT)height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	pD3D11Core->pContext->RSSetViewports(1, &vp);
}

void RendererDX11::clearRenderTargetView(ID3D11RenderTargetView* prtv, float clear_color[4])
{
	pD3D11Core->pContext->ClearRenderTargetView(prtv, clear_color);
}

void RendererDX11::clearDepthStencilView(ID3D11DepthStencilView* pDsv)
{
	pD3D11Core->pContext->ClearDepthStencilView(pDsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
}

void RendererDX11::setRenderTargetView(ID3D11RenderTargetView* prtv, ID3D11DepthStencilView* pDsv)
{
	pD3D11Core->pContext->OMSetRenderTargets(1, &prtv, pDsv);
}

void RendererDX11::setPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY PrimitiveTopology)
{
	pD3D11Core->pContext->IASetPrimitiveTopology(PrimitiveTopology);
}

void RendererDX11::drawIndexedTriangleList(UINT index_count, UINT start_vertex_index, UINT start_index_location)
{
	pD3D11Core->pContext->DrawIndexed(index_count, start_index_location, start_vertex_index);
}

void RendererDX11::Present(bool vsync)
{
	pD3D11Core->pSwapChain->Present(vsync, NULL);
}

void RendererDX11::setShaders(ID3D11VertexShader* vShader, ID3D11PixelShader* pShader)
{
	pD3D11Core->pContext->VSSetShader(vShader, nullptr, 0);
	pD3D11Core->pContext->PSSetShader(pShader, nullptr, 0);
}

void RendererDX11::setVertexShader(ID3D11VertexShader* vShader)
{
	pD3D11Core->pContext->VSSetShader(vShader, nullptr, 0);
}

void RendererDX11::setPixelShader(ID3D11PixelShader* pShader)
{
	pD3D11Core->pContext->PSSetShader(pShader, nullptr, 0);
}

void RendererDX11::setVertexBufferandLayout(UINT size_vertex, ID3D11Buffer* vBuffer, ID3D11InputLayout* iLayout)
{
	UINT stride = size_vertex;
	UINT offset = 0;
	pD3D11Core->pContext->IASetVertexBuffers(0, 1, &vBuffer, &stride, &offset);
	pD3D11Core->pContext->IASetInputLayout(iLayout);
}

void RendererDX11::setIndexBuffer(ID3D11Buffer* indexbuffer)
{
	pD3D11Core->pContext->IASetIndexBuffer(indexbuffer, DXGI_FORMAT_R32_UINT, 0);
}

void RendererDX11::setConstantBuffer(ID3D11Buffer* cbuffer)
{
	pD3D11Core->pContext->VSSetConstantBuffers(0, 1, &cbuffer);
	pD3D11Core->pContext->PSSetConstantBuffers(0, 1, &cbuffer);
}

void RendererDX11::setTextureResourceVertexShader(std::vector<Texture*> TextureList)
{
	UINT startslot = 0;
	UINT numviews = (UINT)(TextureList.size());
	ID3D11ShaderResourceView* list_res[32];
	ID3D11SamplerState* list_sampler[32];

	for (UINT i = 0; i < numviews; ++i)
	{
		list_res[i] = *(TextureList[i]->GetShaderResourceViewPP());
		list_sampler[i] = *(TextureList[i]->GetSamplerStatePP());
	}
	pD3D11Core->pContext->VSSetShaderResources(startslot, numviews, list_res);
	pD3D11Core->pContext->VSSetSamplers(0, numviews, list_sampler);
}

void RendererDX11::setTextureResourcePixelShader(std::vector<Texture*> TextureList)
{
	UINT startslot = 0;
	UINT numviews = (UINT)(TextureList.size());
	ID3D11ShaderResourceView* list_res[32];
	ID3D11SamplerState* list_sampler[32];

	for (UINT i = 0; i < numviews; ++i)
	{
		list_res[i] = *(TextureList[i]->GetShaderResourceViewPP());
		list_sampler[i] = *(TextureList[i]->GetSamplerStatePP());
	}
	pD3D11Core->pContext->PSSetShaderResources(startslot, numviews, list_res);
	pD3D11Core->pContext->PSSetSamplers(0, numviews, list_sampler);
}

void RendererDX11::setTextureResourceVertexShader(Texture* pTexture)
{
	pD3D11Core->pContext->VSSetShaderResources(0, 1, pTexture->GetShaderResourceViewPP());
	pD3D11Core->pContext->VSSetSamplers(0, 1, pTexture->GetSamplerStatePP());
}

void RendererDX11::setTextureResourcePixelShader(Texture* pTexture)
{
	pD3D11Core->pContext->PSSetShaderResources(0, 1, pTexture->GetShaderResourceViewPP());
	pD3D11Core->pContext->PSSetSamplers(0, 1, pTexture->GetSamplerStatePP());
}

void RendererDX11::setTextureResourceVertexShader_normal_included(Texture* pTextureMap, Texture* pTextureNormalMap)
{
	pD3D11Core->pContext->VSSetShaderResources(0, 1, pTextureMap->GetShaderResourceViewPP());
	pD3D11Core->pContext->VSSetSamplers(0, 1, pTextureMap->GetSamplerStatePP());

	pD3D11Core->pContext->VSSetShaderResources(1, 1, pTextureNormalMap->GetShaderResourceViewPP());
	pD3D11Core->pContext->VSSetSamplers(1, 1, pTextureNormalMap->GetSamplerStatePP());
}

void RendererDX11::setTextureResourcePixelShader_normal_included(Texture* pTextureMap, Texture* pTextureNormalMap)
{
	pD3D11Core->pContext->PSSetShaderResources(0, 1, pTextureMap->GetShaderResourceViewPP());
	pD3D11Core->pContext->PSSetSamplers(0, 1, pTextureMap->GetSamplerStatePP());

	pD3D11Core->pContext->PSSetShaderResources(1, 1, pTextureNormalMap->GetShaderResourceViewPP());
	pD3D11Core->pContext->PSSetSamplers(1, 1, pTextureNormalMap->GetSamplerStatePP());
}


void RendererDX11::initRasterizerState()
{
	HRESULT hr;

	D3D11_RASTERIZER_DESC desc = {};
	desc.CullMode = D3D11_CULL_FRONT;
	desc.DepthClipEnable = true;
	desc.FillMode = D3D11_FILL_SOLID;
	GFX_THROW_INFO(pD3D11Core->pDevice->CreateRasterizerState(&desc, &cull_front_state));
	

	desc.CullMode = D3D11_CULL_BACK;
	GFX_THROW_INFO(pD3D11Core->pDevice->CreateRasterizerState(&desc, &cull_back_state));
}

void RendererDX11::setRasterizerState(bool cull_front)
{
	if (cull_front)
		pD3D11Core->pContext->RSSetState(cull_front_state);
	else
		pD3D11Core->pContext->RSSetState(cull_back_state);
}
