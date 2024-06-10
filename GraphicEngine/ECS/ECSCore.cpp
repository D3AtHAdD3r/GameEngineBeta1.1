#include "ECSCore.h"
#include<GraphicEngine/Utilities/Headers/Headers.h>
#include<GraphicEngine/ECS/Scene/SceneManager.h>




ECSCore::ECSCore(D3D11Manager* p_d3dManager, ResourceManager* p_resManager)
{
	if (!p_d3dManager || !p_resManager)
		throw NORMAL_EXCEPT("ECSCore Constructor failed");

	pSceneManager = new SceneManager(p_d3dManager, p_resManager);
}

ECSCore::~ECSCore()
{
	if (pSceneManager)
		delete pSceneManager;
}

void ECSCore::OnResize()
{
	pSceneManager->ReloadAllResourceBuffers();
}

