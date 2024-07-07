#include "IRenderer.h"
#include<GraphicEngine\Renderer\RendererDX11\RendererDX11.h>

bool CreateRenderer(RenderData* pRenderData)
{
	if (pRenderer) return false;
	if (!pRenderer->Create(pRenderData)) return false;
	pRenderer = RendererDX11::Get();
	return true;
}

bool OnResize(unsigned int width, unsigned int height)
{
	if (!pRenderer) return false;
	return pRenderer->OnResize(width, height);
}

bool DrawFrame()
{
	if (!pRenderer) return false;
	return pRenderer->DrawFrame();
}

void OnShutDown()
{
	pRenderer->OnShutDown();
}

bool CreateSceneAndEntity(std::vector<Scene_descriptor*> sd_list, std::vector<EntityDesc*> ed_list)
{
	if (!pRenderer) return false;
	return pRenderer->CreateSceneAndEntity(sd_list, ed_list);
}

Entity* CreateEntity(EntityDesc* pED, bool check_ent_desc)
{
	if (!pRenderer) return nullptr;
	return pRenderer->CreateEntity(pED, check_ent_desc);
}

bool DeleteEntity(Entity* pEnt, Scene* pScene)
{
	if (!pRenderer) return false;
	return pRenderer->DeleteEntity(pEnt, pScene);
}

const std::unordered_map<unsigned short, Scene*>& GetSceneContainer()
{
	if (!pRenderer) return std::unordered_map<unsigned short, Scene*>();
	return pRenderer->GetSceneContainer();
}
