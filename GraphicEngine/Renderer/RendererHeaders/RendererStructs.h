#include<GraphicEngine/D3D11/D3D11Headers/D3D11Structs.h>

class IApplication;
class Texture;

enum TEXTURE_BINDING_TYPE
{
	oneTexMap_OneNormalMap_perDrawCall = 0,		//1 tex-Map, 1NormalMap- binded per draw call (normal maps required)
	oneTexMap_perDrawCall,						//1 tex-map binded per draw call
	allTexMaps_perDrawCall,						//all tex-maps binded per draw call
	unknown
};

class RenderData
{
public:
	D3D11_INIT_DATA d3dInitData;
	//IApplication* pApp; //not required
};


struct Renderer_PreBindData
{
	float color[4] = {0,0,0,0}; //background color for clearRenderTargetView
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
	ID3D11Buffer* indexbuffer = nullptr;

	ID3D11Buffer* cbuffer = nullptr;
	void* pCBuffer_data = nullptr;

	ID3D11Buffer* vBuffer = nullptr;
	ID3D11InputLayout* iLayout = nullptr;
	UINT size_vertex = 0;

	bool FrontFaceCull = false;

	//Texture
	TEXTURE_BINDING_TYPE TexBindType = TEXTURE_BINDING_TYPE::unknown;
	SIZE_T MaterialCount = 0;
	std::vector<std::pair<SIZE_T, SIZE_T>> Material_Draw_Details; // start_index, num_indices

	std::vector<Texture*> list_textures;
	std::vector<Texture*> list_textures_normal;
};