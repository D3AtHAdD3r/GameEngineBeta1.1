#pragma once
#include<d3d11.h>
#include<GraphicEngine/D3D11/D3D11Headers/D3D11Structs.h>
#include<GraphicEngine/Engine/Headers/EngineStructs.h>
#include<GraphicEngine/ECS/ECSHeaders/EntityStructs.h>

class IApplication;
class Texture;



struct RenderData
{
	enum GRAPHIC_API
	{
		dx11 = 0,
		dx12,
		vulkan,
		unknown
	};

	D3D11_INIT_DATA d3dInitData;
	FILEMAPS file_maps;
	GRAPHIC_API graphi_API = GRAPHIC_API::dx11;
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
	ID3D11Buffer* pIndexbuffer = nullptr;

	ID3D11Buffer* cbuffer = nullptr;
	void* pCBuffer_data = nullptr;

	ID3D11Buffer* vBuffer = nullptr;
	ID3D11InputLayout* iLayout = nullptr;
	UINT size_vertex = 0;

	bool FrontFaceCull = false;

	//Texture
	Primitive_texture_type TexBindType = Primitive_texture_type::unknown;
	SIZE_T MaterialCount = 0;
	std::vector<std::pair<SIZE_T, SIZE_T>> Material_Draw_Details; // start_index, num_indices

	std::vector<Texture*> list_textures;
	std::vector<Texture*> list_textures_normal;
};