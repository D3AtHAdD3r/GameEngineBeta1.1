#include "RendererDX11.h"
#include<GraphicEngine/D3D11/D3D11Core/D3D11Core.h>
#include<GraphicEngine/D3D11/MeshAndTextureResources/Texture.h>
#include<GraphicEngine/ECS/ECSCore.h>
#include<GraphicEngine/ECS/Scene/SceneManager.h>
#include<GraphicEngine/ECS/Scene/Scene.h>
#include<GraphicEngine/ECS/Entity/Entity.h>
#include<GraphicEngine/ECS/Components/Camera.h>
#include<GraphicEngine/Renderer/RendererHelpers/ECSToRendererData.h>
#include<GraphicEngine/ECS/Entity/Primitive/Primitive.h>
#include<GraphicEngine/Utilities/UtilitiyFuncs/utilityFunctions.h>
#include<GraphicEngine\Headers\RendererDx11Headers.h>
#include<GraphicEngine/InputHandling/InputSystem.h>
#include<sstream>


RendererDX11* RendererDX11::pRenderer = nullptr;


RendererDX11::RendererDX11()
{
}

RendererDX11::~RendererDX11()
{
	if (pD3D11Core) delete pD3D11Core;
	if (pECSCore) delete pECSCore;
	if (pRenderer_PreBindData) delete pRenderer_PreBindData;
	if (pRenderer_BindingData) delete pRenderer_BindingData;
}


void RendererDX11::UpdateConstantBuffer(Entity* currEntity, Camera* pcam)
{
	if (!pcam ||!currEntity)
	{
		std::ostringstream oss;
		oss << "RendererDX11::UpdateConstantBuffer Failed :: Entity or Camera is nullptr. \n"
			<< "Entity uid : " << currEntity->Get_Entity_uID() << std::endl
			<< "\tScene uid : " << currEntity->GetScene_ID() << std::endl;
		throw NORMAL_EXCEPT(oss.str());
	}

	constant cBuff;

	float width = (float)WindowWidth;
	float height = (float)WindowHeight;
	float aspectRatio = width / height;

	cBuff.m_camera_position = pcam->Get_Current_World_Pos();
	cBuff.m_world = currEntity->Get_Entity_WorldMatrix();
	cBuff.m_view = pcam->Get_ViewMatrix();
	cBuff.m_proj = pcam->Get_ProjectionMatrix();
	
	//TODO get from light component
	cBuff.m_light_position = { 0.0f,0.0f,0.0f,0.0f };
	cBuff.m_light_direction = { 0.0f, 0.0f, -1.0f, 0.0f };
	cBuff.distortion_level = 0.9f;

	if (currEntity->Get_isTerrain()) //there is height map
	{
		//cBuff.sizeHeightMap = currEntity->GetPrimitive()->GetHeightMap()->Get_Size().right;
		cBuff.TerrainSize = currEntity->Get_TerrainSize();
		cBuff.sizeHeightMap = currEntity->Get_SizeHeightMap().m_x;
	}
	

	currEntity->setConstantBuffer(&cBuff);
}


bool RendererDX11::DrawFrame()
{
	for (auto& [uid, currentScene] : pECSCore->pSceneManager->GetSceneContainer())
	{
		//empty struct
		*pRenderer_PreBindData = Renderer_PreBindData();

		if (!ECSToRendererData::fill_Renderer_PreBindData(currentScene, pRenderer_PreBindData))
			return false;
		if (!Init_Pre_Bind(pRenderer_PreBindData))
			return false;


		for (auto& [TypeIndex, EntityContainer] : currentScene->GetEntityContainer())
		{
			for (auto& currentEntity : EntityContainer)
			{
				*pRenderer_BindingData = Renderer_BindingData();

				if (currentEntity->Get_IsRenderable())
				{
					UpdateConstantBuffer(currentEntity, currentScene->getActiveCamera());
					if (!ECSToRendererData::fill_Renderer_MainBindData(currentEntity->GetPrimitive(), pRenderer_BindingData))
						return false;
					if (!Init_Main_Bind(pRenderer_BindingData))
						return false;
				}
			}

		}
	}

	PresentFrame();
	return true;
}

bool RendererDX11::OnResize(unsigned int width, unsigned int height)
{
	if (!width || !height) return false;
	this->WindowWidth = width;
	this->WindowHeight = height;
	pECSCore->OnResize();

	return true;
}

void RendererDX11::OnShutDown()
{
	//bye bye
}

bool RendererDX11::CreateSceneAndEntity(std::vector<Scene_descriptor*> sd_list, std::vector<EntityDesc*> ed_list)
{
	return pECSCore->pSceneManager->CreateSceneAndEntity(sd_list, ed_list);
}

Entity* RendererDX11::CreateEntity(EntityDesc* pED, bool check_ent_desc)
{
	return pECSCore->pSceneManager->CreateEntity(pED, check_ent_desc);
}

bool RendererDX11::DeleteEntity(Entity* pEnt, Scene* pScene)
{
	return pECSCore->pSceneManager->DeleteEntity(pEnt, pScene);
}

const std::unordered_map<unsigned short, Scene*>& RendererDX11::GetSceneContainer()
{
	return pECSCore->pSceneManager->GetSceneContainer();
}


bool RendererDX11::Create(RenderData* pRenderData)
{
	if (pRenderer) return false;
	pRenderer = new RendererDX11();

	if (!pRenderer->Initialize(pRenderData)) return false;
	return true;
}

RendererDX11* RendererDX11::Get()
{
	return pRenderer;
}

unsigned int RendererDX11::Get_WindowWidth()
{
	return WindowWidth;
}

unsigned int RendererDX11::Get_WindowHeight()
{
	return WindowHeight;
}

HWND RendererDX11::Get_WindowHandle()
{
	return pWnd;
}

bool RendererDX11::Initialize(RenderData* pRenderData)
{
	if (!CheckRenderData(pRenderData)) return false;
	Check_File_Exists(pRenderData);

	this->pWnd = pRenderData->pHandle;
	this->WindowWidth = pRenderData->WindowWidth;
	this->WindowHeight = pRenderData->WindowHeight;

	InputSystem::Create();

	pD3D11Core = new D3D11Core(&(pRenderData->d3dInitData), &(pRenderData->file_maps));
	initRasterizerState();
	pECSCore = new ECSCore(pD3D11Core->pD3D11Manager, pD3D11Core->pResourceManager);
	
	pRenderer_PreBindData = new Renderer_PreBindData();
	pRenderer_BindingData = new Renderer_BindingData();

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
	if (!pRenderData->d3dInitData.Window_Height || !pRenderData->d3dInitData.Window_Width || !pRenderData->d3dInitData.BufferCount) 
		return false;

	if (!pRenderData->pHandle || !pRenderData->WindowWidth || !pRenderData->WindowHeight)
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
		!pData->pIndexbuffer || !pData->pCBuffer_data ||
		!pData->MaterialCount)
		return false;

	if (pData->Material_Draw_Details.size() != pData->MaterialCount) return false;

	return true;
}

void RendererDX11::Check_File_Exists(RenderData* p_RenderData)
{
	FILEMAPS& fMaps = p_RenderData->file_maps;

	std::unordered_map<int, std::wstring>& File_Map_VertexShader = fMaps.File_Map_VertexShader;
	std::unordered_map<int, std::wstring>& File_Map_PixelShader = fMaps.File_Map_PixelShader;
	std::unordered_map<int, std::wstring>& File_Map_Textures = fMaps.File_Map_Textures;
	std::unordered_map<int, std::wstring>& File_Map_Meshes = fMaps.File_Map_Meshes;

	for (auto& [uid, FileName] : File_Map_VertexShader)
	{
		if (!UtilityFuncs::Check_File_Exist(FileName))
		{
			std::ostringstream oss;
			oss << "File Not Found : " << UtilityFuncs::unicodeToMultibyte(FileName) << std::endl;
			throw NORMAL_EXCEPT(oss.str());
		}
	}

	for (auto& [uid, FileName] : File_Map_PixelShader)
	{
		if (!UtilityFuncs::Check_File_Exist(FileName))
		{
			std::ostringstream oss;
			oss << "File Not Found : " << UtilityFuncs::unicodeToMultibyte(FileName) << std::endl;
			throw NORMAL_EXCEPT(oss.str());
		}
	}

	for (auto& [uid, FileName] : File_Map_Textures)
	{
		//Check if RTV or BackBuffer texture
		std::wstring firstThree = FileName.substr(0, 3);
		if (firstThree == HARDCODINGS::RTV_NAME_START || firstThree == HARDCODINGS::BackBuffer_NAME_START)
			continue;

		if (!UtilityFuncs::Check_File_Exist(FileName))
		{
			std::ostringstream oss;
			oss << "File Not Found : " << UtilityFuncs::unicodeToMultibyte(FileName) << std::endl;
			throw NORMAL_EXCEPT(oss.str());
		}
	}

	for (auto& [uid, FileName] : File_Map_Meshes)
	{
		if (!UtilityFuncs::Check_File_Exist(FileName))
		{
			std::ostringstream oss;
			oss << "File Not Found : " << UtilityFuncs::unicodeToMultibyte(FileName) << std::endl;
			throw NORMAL_EXCEPT(oss.str());
		}
	}
}

bool RendererDX11::SetPreBinds(Renderer_PreBindData* pData)
{
	float color[4];
	color[0] = pData->color[0], color[1] = pData->color[1], color[2] = pData->color[2], color[3] = pData->color[3];

	if (pData->ClearRenderTargetView && pData->pRTV != nullptr)
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

	setVertexBufferandLayout(pData->size_vertex, pData->vBuffer, pData->iLayout);
	setIndexBuffer(pData->pIndexbuffer);

	pD3D11Core->pContext->UpdateSubresource(pData->cbuffer, NULL, NULL, pData->pCBuffer_data, NULL, NULL);
	setConstantBuffer(pData->cbuffer);

	if (binded_vs != pData->vShader)
	{
		setVertexShader(pData->vShader);
		binded_vs = pData->vShader;
	}

	setRasterizerState(pData->FrontFaceCull);

	if (binded_ps != pData->pShader)
	{
		setPixelShader(pData->pShader);
		binded_ps = pData->pShader;
	}


	//setConstantBuffer(pData->cbuffer);
	if (binded_cb != pData->cbuffer)
	{
		setConstantBuffer(pData->cbuffer);
		binded_cb = pData->cbuffer;
	}



	//-----------------------------------------------------------//
	//-----------Static-Binding-All-Entity-Textures--------------//
	//-----------------------------------------------------------//

	//Textures
	//1. bind all the textures to their respective registers 
	//2. Update Constant buffer with material id, Draw material wise, 

	/*{
		int startSlot = HARDCODINGS::Texture_Material_Register_Index;
		Set_TextureResources_Vertex_Pixel_Shader(pData->list_textures_Material, startSlot);
	}
	{
		int startSlot = HARDCODINGS::Texture_Normal_Map_Register_Index;
		Set_TextureResources_Vertex_Pixel_Shader(pData->list_textures_Normal_Map, startSlot);
	}
	{
		int startSlot = HARDCODINGS::Texture_Height_Map_Register_Index;
		Set_TextureResources_Vertex_Pixel_Shader(pData->list_textures_Height_Map, startSlot);
	}

	for (size_t m = 0; m < pData->MaterialCount; ++m)
	{
		constant* constBuffer = (constant*)(pData->pCBuffer_data);
		constBuffer->Material_id = m;
		pD3D11Core->pContext->UpdateSubresource(pData->cbuffer, NULL, NULL, pData->pCBuffer_data, NULL, NULL);

		UINT start_index_location = (UINT)(pData->Material_Draw_Details[m].first);
		UINT index_count = (UINT)(pData->Material_Draw_Details[m].second);
		drawIndexedTriangleList(index_count, 0, start_index_location);
	}*/

	//-----------------------------------------------------------//
	//-----------------Material-wise-Binding---------------------//
	//-----------------------------------------------------------//

	//Textures
	//1. bind all the textures except material-textures/material-normals, to their respective registers 
	{
		int startSlot = HARDCODINGS::Texture_Custom_Register_Index;
		if (!pData->list_textures_Custom.empty())
			Set_TextureResources_Vertex_Pixel_Shader(pData->list_textures_Custom, startSlot);
		
	}
	{
		int startSlot = HARDCODINGS::Texture_Height_Map_Register_Index;
		if (!pData->list_textures_Height_Map.empty())
			Set_TextureResources_Vertex_Pixel_Shader(pData->list_textures_Height_Map, startSlot);
		
	}
	//2.bind material-textures, material-wise and draw.
	int Material_Tex_Count = pData->list_textures_Material.size();
	int Material_Normal_Count = pData->list_textures_Normal_Map.size();

	auto itr_material = pData->list_textures_Material.begin();
	auto itr_normal = pData->list_textures_Normal_Map.begin();
	
	for (size_t m = 0; m < pData->MaterialCount; ++m)
	{
		constant* constBuffer = (constant*)(pData->pCBuffer_data);
		constBuffer->Material_id = m;
		pD3D11Core->pContext->UpdateSubresource(pData->cbuffer, NULL, NULL, pData->pCBuffer_data, NULL, NULL);

		//if texture is available for current material
		if (Material_Tex_Count > m)
		{
			ID3D11ShaderResourceView* pSRV = itr_material->second->GetShaderResourceView();
			ID3D11SamplerState* pSampler = itr_material->second->GetSamplerState();
			Set_TextureResources_Vertex_Pixel_Shader(HARDCODINGS::Texture_Material_Register_Index, 1, pSRV, pSampler);
			itr_material++;
		}
		if (Material_Normal_Count > m)
		{
			ID3D11ShaderResourceView* pSRV = itr_normal->second->GetShaderResourceView();
			ID3D11SamplerState* pSampler = itr_normal->second->GetSamplerState();
			Set_TextureResources_Vertex_Pixel_Shader(HARDCODINGS::Texture_Normal_Map_Register_Index, 1, pSRV, pSampler);
			itr_normal++;
		}

		UINT start_index_location = (UINT)(pData->Material_Draw_Details[m].first);
		UINT index_count = (UINT)(pData->Material_Draw_Details[m].second);
		drawIndexedTriangleList(index_count, 0, start_index_location);
	}
	
	return true;
}

void RendererDX11::SetFullScreenMode(bool fullscreen)
{
	HRESULT hr;
	GFX_THROW_INFO(pD3D11Core->pSwapChain->SetFullscreenState(fullscreen, nullptr));
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

void RendererDX11::Set_TextureResources_VertexShader(int startslot, int numViews, ID3D11ShaderResourceView* list_res[128], ID3D11SamplerState* list_sampler[128])
{
	pD3D11Core->pContext->VSSetShaderResources(startslot, numViews, list_res);
	pD3D11Core->pContext->VSSetSamplers(0, numViews, list_sampler);
}

void RendererDX11::Set_TextureResources_PixelShader(int startslot, int numViews, ID3D11ShaderResourceView* list_res[128], ID3D11SamplerState* list_sampler[128])
{
	pD3D11Core->pContext->PSSetShaderResources(startslot, numViews, list_res);
	pD3D11Core->pContext->PSSetSamplers(0, numViews, list_sampler);
}

void RendererDX11::Set_TextureResources_VertexShader(std::unordered_map<int, Texture*>& list_textures, int start_slot)
{
	if (list_textures.empty())
		throw NORMAL_EXCEPT("InValid Input,list_textures is empty");

	UINT startslot = start_slot;
	UINT numviews = list_textures.size();
	ID3D11ShaderResourceView* list_res[128];
	ID3D11SamplerState* list_sampler[128];

	int i = 0;
	for (auto itr = list_textures.begin(); itr != list_textures.end(); ++itr, ++i)
	{
		Texture* currTex = itr->second;
		list_res[i] = currTex->GetShaderResourceView();
		list_sampler[i] = currTex->GetSamplerState();
	}

	pD3D11Core->pContext->VSSetShaderResources(startslot, numviews, list_res);
	pD3D11Core->pContext->VSSetSamplers(0, numviews, list_sampler);
}

void RendererDX11::Set_TextureResources_PixelShader(std::unordered_map<int, Texture*>& list_textures, int start_slot)
{
	if (list_textures.empty())
		throw NORMAL_EXCEPT("InValid Input,list_textures is empty");

	UINT startslot = start_slot;
	UINT numviews = list_textures.size();
	ID3D11ShaderResourceView* list_res[128];
	ID3D11SamplerState* list_sampler[128];

	int i = 0;
	for (auto itr = list_textures.begin(); itr != list_textures.end(); ++itr, ++i)
	{
		Texture* currTex = itr->second;
		list_res[i] = currTex->GetShaderResourceView();
		list_sampler[i] = currTex->GetSamplerState();
	}

	pD3D11Core->pContext->PSSetShaderResources(startslot, numviews, list_res);
	pD3D11Core->pContext->PSSetSamplers(0, numviews, list_sampler);
}

void RendererDX11::Set_TextureResources_Vertex_Pixel_Shader(std::unordered_map<int, Texture*>& list_textures, int start_slot)
{
	if (list_textures.empty()) 
		throw NORMAL_EXCEPT("InValid Input, list_textures is empty");

	UINT startslot = start_slot;
	UINT numviews = list_textures.size();
	ID3D11ShaderResourceView* list_res[128];
	ID3D11SamplerState* list_sampler[128];

	int i = 0;
	for (auto itr = list_textures.begin(); itr != list_textures.end(); ++itr , ++i)
	{
		Texture* currTex = itr->second;
		list_res[i] = currTex->GetShaderResourceView();
		list_sampler[i] = currTex->GetSamplerState();
	}

	pD3D11Core->pContext->VSSetShaderResources(startslot, numviews, list_res);
	pD3D11Core->pContext->VSSetSamplers(0, numviews, list_sampler);

	pD3D11Core->pContext->PSSetShaderResources(startslot, numviews, list_res);
	pD3D11Core->pContext->PSSetSamplers(0, numviews, list_sampler);
}

void RendererDX11::Set_TextureResources_Vertex_Pixel_Shader(int startslot, int numViews, ID3D11ShaderResourceView* pSRV, ID3D11SamplerState* pSampler)
{
	if (!pSRV || !pSampler)
		throw NORMAL_EXCEPT("InValid Input, ShaderResource View or Sampler is nullptr");

	pD3D11Core->pContext->VSSetShaderResources(startslot, numViews, &pSRV);
	pD3D11Core->pContext->VSSetSamplers(0, numViews, &pSampler);

	pD3D11Core->pContext->PSSetShaderResources(startslot, numViews, &pSRV);
	pD3D11Core->pContext->PSSetSamplers(0, numViews, &pSampler);
}



void RendererDX11::initRasterizerState()
{
	HRESULT hr;

	D3D11_RASTERIZER_DESC desc = {};
	desc.CullMode = D3D11_CULL_FRONT;
	desc.DepthClipEnable = true;
	desc.FillMode = D3D11_FILL_SOLID;
	//desc.FrontCounterClockwise = true; // 
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
