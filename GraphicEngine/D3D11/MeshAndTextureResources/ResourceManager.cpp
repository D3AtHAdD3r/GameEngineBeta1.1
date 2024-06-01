#include "ResourceManager.h"
#include<GraphicEngine/D3D11/MeshAndTextureResources/MeshManager.h>
#include<GraphicEngine/D3D11/MeshAndTextureResources/TextureManager.h>




ResourceManager::ResourceManager()
{
	pMeshManager = new MeshManager();
	pTextureManager = new TextureManager();
}

ResourceManager::~ResourceManager()
{
	if (pMeshManager)		delete pMeshManager;
	if (pTextureManager)	delete pTextureManager;
}

void ResourceManager::releaseAll()
{
	if (pMeshManager)		pMeshManager->releaseAll();
	if (pTextureManager)	pTextureManager->releaseAll();
}
