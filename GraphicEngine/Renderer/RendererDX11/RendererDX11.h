#include<GraphicEngine/Renderer/RendererHeaders/RendererStructs.h>


class D3D11Core;
class Texture;

class RendererDX11
{
private:
	RendererDX11(); //undeclared
	~RendererDX11();	//undeclared

public:
	bool Initialize(RenderData* pRenderData);
	bool BeginFrame(); //prebind

private:
	bool CheckRenderData(RenderData* pRenderData); //undeclared

private:
	bool SetPreBinds(Renderer_PreBindData* pData);
	bool BindToPipeLine(); //undeclared
	bool PresentFrame();	//undeclared


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
	void setTextureResourceVertexShader(Texture* TextureList[32], unsigned int numberOfTex);
	void setTextureResourcePixelShader(Texture* TextureList[32], unsigned int numberOfTex);

	void setTextureResourceVertexShader(Texture* pTexture);
	void setTextureResourcePixelShader(Texture* pTexture);

	void setTextureResourceVertexShader_normal_included(Texture* pTextureMap, Texture* pTextureNormalMap);
	void setTextureResourcePixelShader_normal_included(Texture* pTextureMap, Texture* pTextureNormalMap);

private:
	void initRasterizerState();
	void setRasterizerState(bool cull_front);

private:
	D3D11Core* pD3D11Core = nullptr;

private:
	ID3D11RasterizerState* cull_front_state = nullptr;
	ID3D11RasterizerState* cull_back_state = nullptr;
};