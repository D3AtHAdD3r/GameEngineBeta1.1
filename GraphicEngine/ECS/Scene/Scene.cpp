#include "Scene.h"
#include<GraphicEngine/Utilities/Headers/Headers.h>
#include<GraphicEngine/ECS/ECSHeaders/EntityStructs.h>
#include<GraphicEngine/D3D11/MeshAndTextureResources/ResourceManager.h>
#include<GraphicEngine/D3D11/MeshAndTextureResources/TextureManager.h>
#include<GraphicEngine/ECS/Entity/EntityManager.h>
#include<GraphicEngine/D3D11/MeshAndTextureResources/Texture.h>
#include<GraphicEngine/ECS/Components/Camera.h>


Scene::Scene(D3D11Manager* p_D3DManager, ResourceManager* p_ResourceManager, Scene_descriptor* sd)
	:
	pd3dManager(p_D3DManager),
	presManager(p_ResourceManager),
	drawOnBackBuffer(sd->connect_backbuffer),
	sceneName(sd->scene_name),
	clearRenderTargetView(sd->clearRenderTargetView),
	clearDepthStencil(sd->clearDepthStencil),
	useDepthStencil(sd->useDepthStencil),
	scene_id(sd->scene_id),
	BackBuffer_Index(sd->BackBuffer_Index),
	scene_texture_uid(sd->scene_texture_uid)
{
	if (!p_D3DManager || !p_ResourceManager || !sd->window_client_width || !sd->window_client_height || sd->scene_name.empty() || sd->height_ratio < 1 || sd->width_ratio < 1 || sd->scene_id < 0)
		throw NORMAL_EXCEPT("Scene constructor failed...");

	//Create Cameras
	for (int i = 0; i < sd->camData.size(); ++i)
	{
		Camera* pcam = CreateCamera(sd->camData[i]);
		if (!pcam)
			throw NORMAL_EXCEPT("Scene constructor failed. Camera creation failed.");
		if (pcam->isProjecting)
			pCameraActive = pcam;
	}

	//set width , height , relative to main  client window
	this->width_ratio = sd->width_ratio;
	this->height_ratio = sd->height_ratio;

	float width = (float)sd->window_client_width / sd->width_ratio;
	float height = (float)sd->window_client_height / sd->height_ratio;

	this->width = (unsigned int)width;
	this->height = (unsigned int)height;

	//either scene will be drawn directly on backbuffer or other buffer
	if (sd->connect_backbuffer)
	{
		scene_texture = presManager->pTextureManager->CreateTextureFromBackBuffer(this->sceneName, sd->BackBuffer_Index, this->scene_texture_uid);
		if (!scene_texture)
			throw NORMAL_EXCEPT("Texture creation failed in Scene constructor ...\n");
	}
	else
	{
		scene_texture = presManager->pTextureManager->CreateTextureFromResourceViews(this->sceneName, this->width, this->height, this->scene_texture_uid);
		if (!scene_texture)
			throw NORMAL_EXCEPT("Texture creation failed in Scene constructor ...\n");
	}

	pEntityManager = new EntityManager(pd3dManager, presManager, this);

	if (pEntityManager == nullptr)
		throw NORMAL_EXCEPT("EntityManager creation failed in Scene constructor ...\n");
}

Scene::~Scene()
{
}

bool Scene::UpdateOnResize(unsigned int client_width, unsigned int client_height)
{
	if (!client_width || !client_height) return false;

	float width = (float)client_width / this->width_ratio;
	float height = (float)client_height / this->height_ratio;

	this->width = (unsigned int)width;
	this->height = (unsigned int)height;

	return UpdateTextureOnResize(this->width, this->height);
}

ID3D11Resource* Scene::getTexture2D()
{
	return scene_texture->GetTextureResource();
}

ID3D11RenderTargetView* Scene::getRTV()
{
	return scene_texture->GetRenderTargetView();
}

ID3D11DepthStencilView* Scene::getDSV()
{
	return scene_texture->GetDepthStencilView();
}

ID3D11ShaderResourceView* Scene::getSRV()
{
	return scene_texture->GetShaderResourceView();
}

color_Scene Scene::getColorBackground()
{
	return color_background;
}

const unsigned int& Scene::getWidth() const
{
	return width;
}

const unsigned int& Scene::getHeight() const
{
	return height;
}

const std::wstring& Scene::GetSceneName() const
{
	return sceneName;
}

const bool& Scene::getclearRenderTargetView() const
{
	return clearRenderTargetView;
}

const bool& Scene::getclearDepthStencil() const 
{
	return clearDepthStencil;
}

const bool& Scene::getUseDepthStencil() const
{
	return useDepthStencil;
}

Camera* Scene::getActiveCamera()
{
	return pCameraActive;
}

const bool& Scene::getdrawOnBackBuffer() const
{
	return drawOnBackBuffer;
}

Texture* Scene::GetSceneTexture()
{
	return scene_texture;
}

const std::unordered_map<std::type_index, std::vector<Entity*>>& Scene::GetEntityContainer() const
{
	return pEntityManager->Get_Entity_Container();
}

Camera* Scene::Get_Camera_by_uID(int uid) 
{
	if (uid < 0) return nullptr;

	if (CameraContainer.find(uid) != CameraContainer.end())
		return CameraContainer[uid];
	else
		return nullptr;
}

bool Scene::Activate_Camera(int uid)
{
	if (uid < 0) return false;
	if (CameraContainer.find(uid) != CameraContainer.end())
	{
		for (auto& [u_id, Camera] : CameraContainer)
		{
			if (u_id == uid)
			{
				Camera->isProjecting = true;
				pCameraActive = Camera;
			}
			else
				Camera->isProjecting = false;
		}

		return true;
	}

	return false;
}

Entity* Scene::AddEntity(EntityDesc* pED)
{
	Entity* currEnt = nullptr;
	if ((currEnt = pEntityManager->CreateEntity(pED)) == nullptr)
		return nullptr;

	return currEnt;
}

bool Scene::DeleteEntity(Entity* pEnt)
{
	return pEntityManager->DeleteEntity(pEnt);
}

bool Scene::UpdateTextureOnResize(unsigned int width, unsigned int height)
{
	return presManager->pTextureManager->UpdateTextureOnResize(scene_texture_uid, width, height, scene_texture);
}

Camera* Scene::CreateCamera(CameraInitData* pCD)
{
	Camera* p_Cam = new Camera(pCD);
	CameraContainer.emplace(p_Cam->uid, p_Cam);
	return p_Cam;
}
