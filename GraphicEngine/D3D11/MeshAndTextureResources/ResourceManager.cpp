#include "ResourceManager.h"
#include<GraphicEngine/D3D11/MeshAndTextureResources/MeshManager.h>
#include<GraphicEngine/D3D11/MeshAndTextureResources/TextureManager.h>
#include<GraphicEngine/Engine/Headers/EngineStructs.h>



ResourceManager::ResourceManager(FILEMAPS* file_maps)
{
	pMeshManager = new MeshManager(file_maps->File_Map_Meshes);
	pTextureManager = new TextureManager(file_maps->File_Map_Textures);
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
