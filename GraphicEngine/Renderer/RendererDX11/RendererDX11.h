#include<GraphicEngine/Renderer/RendererHeaders/RendererStructs.h>


class D3D11Core;
class Texture;

class RendererDX11
{
	friend class GraphicEngine;
private:
	RendererDX11(); 
	~RendererDX11();	

public:
	bool Initialize(RenderData* pRenderData);
	bool Init_Pre_Bind(Renderer_PreBindData* pData); 
	bool Init_Main_Bind(Renderer_BindingData* pData);
	void PresentFrame(bool Vsync = false);

private:
	bool CheckRenderData(RenderData* pRenderData); 
	bool CheckPreBindData(Renderer_PreBindData* pData);
	bool CheckMainBindData(Renderer_BindingData* pData);

private:
	bool SetPreBinds(Renderer_PreBindData* pData);
	bool BindToPipeLine(Renderer_BindingData* pData);
	
private:
	//setter functions
	void setViewport(UINT width, UINT height);
	void clearRenderTargetView(ID3D11RenderTargetView* prtv, float clear_color[4]);
	void clearDepthStencilView(ID3D11DepthStencilView* pDsv);
	void setRenderTargetView(ID3D11RenderTargetView* prtv, ID3D11DepthStencilView* pDsv);
	void setPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY PrimitiveTopology);
	void drawIndexedTriangleList(UINT index_count, UINT start_vertex_index, UINT start_index_location);
	void Present(bool vsync);

private:
	//binder functions
	void setShaders(ID3D11VertexShader* vShader, ID3D11PixelShader* pShader);
	void setVertexShader(ID3D11VertexShader* vShader);
	void setPixelShader(ID3D11PixelShader* pShader);
	void setVertexBufferandLayout(UINT size_vertex, ID3D11Buffer* vBuffer, ID3D11InputLayout* iLayout);
	void setIndexBuffer(ID3D11Buffer* indexbuffer);
	void setConstantBuffer(ID3D11Buffer* cbuffer);

private:
	//binder functions Textures
	//allTexMaps_perDrawCall,
	void setTextureResourceVertexShader(std::vector<Texture*> TextureList);
	void setTextureResourcePixelShader(std::vector<Texture*> TextureList);

	//oneTexMap_perDrawCall
	void setTextureResourceVertexShader(Texture* pTexture);
	void setTextureResourcePixelShader(Texture* pTexture);

	//oneTexMap_OneNormalMap_perDrawCall
	void setTextureResourceVertexShader_normal_included(Texture* pTextureMap, Texture* pTextureNormalMap);
	void setTextureResourcePixelShader_normal_included(Texture* pTextureMap, Texture* pTextureNormalMap);

private:
	void initRasterizerState();
	void setRasterizerState(bool cull_front);
	void SetFullScreenMode(bool fullscreen);

private:
	D3D11Core* pD3D11Core = nullptr;

private:
	ID3D11RasterizerState* cull_front_state = nullptr;
	ID3D11RasterizerState* cull_back_state = nullptr;

private:
	ID3D11Buffer* binded_cb = nullptr;
	ID3D11PixelShader* binded_ps = nullptr;
	ID3D11VertexShader* binded_vs = nullptr;
};