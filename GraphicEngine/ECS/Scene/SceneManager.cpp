#include "SceneManager.h"
#include<GraphicEngine/Utilities/Headers/Headers.h>
#include<GraphicEngine/ECS/ECSHeaders/EntityStructs.h>
#include<GraphicEngine/ECS/Scene/Scene.h>
#include<GraphicEngine/ECS/Entity/Entity.h>
#include<GraphicEngine/Window/WindowGlobals.h>
//#include<GraphicEngine/D3D11/MeshAndTextureResources/ResourceManager.h>
//#include<GraphicEngine/D3D11/MeshAndTextureResources/TextureManager.h>
//#include<GraphicEngine/ECS/Entity/EntityManager.h>
//#include<GraphicEngine/D3D11/MeshAndTextureResources/Texture.h>

SceneManager::SceneManager(D3D11Manager* p_d3dManager, ResourceManager* p_resManager)
	:
	pD3DManager(p_d3dManager),
	pResManager(p_resManager)
{
	if (!p_d3dManager || !p_resManager)
		throw NORMAL_EXCEPT("SceneManager constructor failed, Invalid input");
}

SceneManager::~SceneManager()
{
	for (auto itr = sceneContainer.begin(); itr != sceneContainer.end(); itr++)
	{
		if (itr->second)
			delete itr->second;
	}
}

//1st check and create scene, 2nd check and create entities
bool SceneManager::CreateSceneAndEntity(std::vector<Scene_descriptor*> sd_list, std::vector<EntityDesc*> ed_list)
{
    unsigned short numberOfScene = sd_list.size();
    unsigned short numberOfEntities = ed_list.size();

    //check for valid scene desc
    for (unsigned short s = 0; s < numberOfScene; ++s)
    {
        if (!check_scene_descriptor(sd_list[s])) return false;
    }

    for (unsigned short s = 0; s < numberOfScene; ++s)
    {
        if (!CreateSceneOnly(sd_list[s], false)) return false;
    }

    //check for valid ent desc
    for (unsigned long long e = 0; e < numberOfEntities; ++e)
    {
        if (!check_Entity_desc(ed_list[e])) return false;
    }

    for (unsigned long long e = 0; e < numberOfEntities; ++e)
    {
        if (!CreateEntity(ed_list[e], false))
            return false;
    }

    return true;
}

Entity* SceneManager::CreateEntity(EntityDesc* pED, bool check_ent_desc)
{
    if (check_ent_desc)
        if (!check_Entity_desc(pED)) return nullptr;


    Entity* curEnt = nullptr;
    curEnt = sceneContainer[pED->Scene_Id]->AddEntity(pED);
    if (!curEnt)
        return nullptr;

    return curEnt;
}

bool SceneManager::DeleteEntity(Entity* pEnt, Scene* pScene)
{
    if (!pEnt) return false;

    if (pScene)
    {
        if (!checkSceneExist(pScene->scene_id))
            return false;

        return pScene->DeleteEntity(pEnt);
    }
    else
    {
        if (!checkSceneExist(pEnt->GetScene_ID()))
            return false;

        return sceneContainer[pEnt->GetScene_ID()]->DeleteEntity(pEnt);
    }
}

const std::unordered_map<unsigned short, Scene*>& SceneManager::GetSceneContainer()
{
    return sceneContainer;
}

bool SceneManager::CreateSceneOnly(Scene_descriptor* pSD, bool check_scene_desc)
{
    if (check_scene_desc)
        if (!check_scene_descriptor(pSD)) return false;

    Scene* pScene = new Scene(pD3DManager, pResManager, pSD);
    if (!pScene) return false;

    sceneContainer[pSD->scene_id] = pScene;

    return true;
}

bool SceneManager::check_scene_descriptor(Scene_descriptor* sd)
{
    if (sd->scene_name.empty() || sd->scene_id < 0 || sd->scene_texture_uid < 0) return false;

    //check if scene id already exist
    if (sceneContainer.find(sd->scene_id) != sceneContainer.end())
        return false;

    if (sd->connect_backbuffer)
    {
        sd->height_ratio = sd->width_ratio = 1;
        sd->window_client_width = WindowGlobals::Get()->Get_WindowWidth();
        sd->window_client_height = WindowGlobals::Get()->Get_WindowHeight();
    }
    else
    {
        //check first three letters of scene name == RTV
        std::wstring firstThree = sd->scene_name.substr(0, 3);
        if (firstThree != HARDCODINGS::RTV_NAME_START) return false;
    }

    if (sd->width_ratio < 1 || sd->height_ratio < 1) return false;

    if (sd->window_client_width == 0 || sd->window_client_height == 0)
    {
        sd->window_client_width = WindowGlobals::Get()->Get_WindowWidth();
        sd->window_client_height = WindowGlobals::Get()->Get_WindowHeight();
    }

    if (sd->useDepthStencil == false)
        sd->clearDepthStencil = false;

    return true;
}

bool SceneManager::check_Entity_desc(const EntityDesc* EntityDesc)
{
    if (EntityDesc->primitive_uid < 0 || EntityDesc->constant_buffer_uid < 0 ||
        EntityDesc->pixel_Shader_uid < 0 || EntityDesc->vertex_Shader_uid < 0)
    {
        return false;
    }

    if (EntityDesc->primitive_name.empty())
        return false;

    if (!EntityDesc->constant_buffer || !EntityDesc->size_constant_buffer) return false;

    if (EntityDesc->primitive_texture_type == Primitive_texture_type::unknown)
        return false;

    if (EntityDesc->Scene_Id < 0) return false;
    if (!checkSceneExist(EntityDesc->Scene_Id)) return false;


    //1 normal map per 1 tex map required  
    if (EntityDesc->isNormalMap)
    {
        if (EntityDesc->primitive_texture_type != Primitive_texture_type::oneTexMap_OneNormalMap_perDrawCall) return false;
        if (EntityDesc->texture_uids.size() != EntityDesc->texture_normals_uids.size()) return false;
    }

    if (EntityDesc->getMeshfromFile)
    {
        if (EntityDesc->mesh_uid < 0) return false;
    }
    else
    {
        if (EntityDesc->mesh_creation_data == nullptr) return false;
    }

    return true;
}

bool SceneManager::checkSceneExist(short SceneID)
{
    if (SceneID < 0) return false;

    //check if scene id already exist
    if (sceneContainer.find(SceneID) != sceneContainer.end())
        return true;
    else
        return false;
}

void SceneManager::ReloadAllResourceBuffers()
{
    for (auto& [SceneID, CurrentScene] : sceneContainer)
    {
        if (!CurrentScene->UpdateOnResize(WindowGlobals::Get()->Get_WindowWidth(), WindowGlobals::Get()->Get_WindowHeight()))
            throw NORMAL_EXCEPT("ReloadAllResourceBuffers failed on Reszie");
    }
}
