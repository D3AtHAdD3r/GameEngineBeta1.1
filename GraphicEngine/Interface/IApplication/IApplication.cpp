#include "IApplication.h"
#include<GraphicEngine/Engine/EngineCore/GraphicEngine.h>
#include<GraphicEngine/Renderer/Renderer.h>
#include<GraphicEngine/Window/WindowGlobals.h>

//IApplication::IApplication()
//{
//
//}

void IApplication::Init_IApp()
{
	pRenderer = WindowGlobals::Get()->Get_Renderer();
}

bool IApplication::CreateSceneAndEntity(std::vector<Scene_descriptor*> sd_list, std::vector<EntityDesc*> ed_list)
{
	return pRenderer->CreateSceneAndEntity(sd_list, ed_list);
}

Entity* IApplication::CreateEntity(EntityDesc* pED, bool check_ent_desc)
{
	return pRenderer->CreateEntity(pED, check_ent_desc);
}

bool IApplication::DeleteEntity(Entity* pEnt, Scene* pScene)
{
	return pRenderer->DeleteEntity(pEnt, pScene);
}

const std::unordered_map<unsigned short, Scene*>& IApplication::GetSceneContainer()
{
	return pRenderer->GetSceneContainer();
}

float IApplication::Get_AspectRatio()
{
	float width = (float)(WindowGlobals::Get()->Get_WindowWidth());
	float height = (float)(WindowGlobals::Get()->Get_WindowHeight());
	float aspectRatio = width / height;

	return aspectRatio;
}
