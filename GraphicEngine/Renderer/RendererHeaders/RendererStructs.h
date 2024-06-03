#include<GraphicEngine/D3D11/D3D11Headers/D3D11Structs.h>

class IApplication;


class RenderData
{
public:
	D3D11_INIT_DATA d3dInitData;
	IApplication* pApp; //not required
};


struct Renderer_PreBindData
{
	float color[4]; //background color for clearRenderTargetView
	bool ClearRenderTargetView = true;
	bool ClearDepthStencil = true;
	bool UseDepthStencil = true;
	ID3D11RenderTargetView* pRTV = nullptr;
	ID3D11DepthStencilView* pDSV = nullptr;
	unsigned int scene_width = 0;
	unsigned int scene_height = 0;
	D3D11_PRIMITIVE_TOPOLOGY PrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
};

struct Renderer_BindingData
{
	ID3D11VertexShader* vShader = nullptr;
	ID3D11PixelShader* pShader = nullptr;

	//vertex buffer
	ID3D11Buffer* vBuffer = nullptr;
	ID3D11InputLayout* iLayout = nullptr;
	UINT size_vertex = 0;

	ID3D11Buffer* indexbuffer = nullptr;

	ID3D11Buffer* cbuffer = nullptr;
};