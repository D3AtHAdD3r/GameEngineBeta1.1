#include "RendererDX11.h"
#include<GraphicEngine/D3D11/D3D11Core/D3D11Core.h>
#include<GraphicEngine/IApplication/IApplication.h>

#include<GraphicEngine/D3D11/MeshAndTextureResources/Texture.h>



RendererDX11::RendererDX11()
{
}

RendererDX11::~RendererDX11()
{
}

bool RendererDX11::Initialize(RenderData* pRenderData)
{
	if (!CheckRenderData(pRenderData)) return false;
	pD3D11Core = new D3D11Core(&(pRenderData->d3dInitData));
	initRasterizerState();
	return true;
}

bool RendererDX11::BeginFrame()
{
	
	return true;
}

bool RendererDX11::CheckRenderData(RenderData* pRenderData)
{
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

void RendererDX11::setTextureResourceVertexShader(Texture* TextureList[32], unsigned int numberOfTex)
{
	UINT startslot = 0;
	UINT numviews = numberOfTex;
	ID3D11ShaderResourceView* list_res[32];
	ID3D11SamplerState* list_sampler[32];

	for (UINT i = 0; i < numberOfTex; ++i)
	{
		list_res[i] = *(TextureList[i]->GetShaderResourceViewPP());
		list_sampler[i] = *(TextureList[i]->GetSamplerStatePP());
	}
	pD3D11Core->pContext->VSSetShaderResources(startslot, numviews, list_res);
	pD3D11Core->pContext->VSSetSamplers(0, numberOfTex, list_sampler);
}

void RendererDX11::setTextureResourcePixelShader(Texture* TextureList[32], unsigned int numberOfTex)
{
	UINT startslot = 0;
	UINT numviews = numberOfTex;
	ID3D11ShaderResourceView* list_res[32];
	ID3D11SamplerState* list_sampler[32];

	for (UINT i = 0; i < numberOfTex; ++i)
	{
		list_res[i] = *(TextureList[i]->GetShaderResourceViewPP());
		list_sampler[i] = *(TextureList[i]->GetSamplerStatePP());
	}
	pD3D11Core->pContext->PSSetShaderResources(startslot, numviews, list_res);
	pD3D11Core->pContext->PSSetSamplers(0, numberOfTex, list_sampler);
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
