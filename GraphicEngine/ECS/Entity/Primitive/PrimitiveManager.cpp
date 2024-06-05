#include "PrimitiveManager.h"
#include<GraphicEngine/Utilities/Headers/Headers.h>
#include<GraphicEngine/ECS/ECSHeaders/EntityStructs.h>
#include<GraphicEngine/D3D11/MeshAndTextureResources/Mesh.h>
#include<GraphicEngine/D3D11/MeshAndTextureResources/ResourceManager.h>
#include<GraphicEngine/D3D11/MeshAndTextureResources/MeshManager.h>
#include<GraphicEngine/D3D11/MeshAndTextureResources/TextureManager.h>
#include<GraphicEngine/D3D11/MeshAndTextureResources/Texture.h>
#include<GraphicEngine/D3D11/D3D11Resources/VertexShader.h>
#include<GraphicEngine/D3D11/D3D11Resources/VertexShaderManager.h>
#include<GraphicEngine/D3D11/D3D11Resources/PixelShader.h>
#include<GraphicEngine/D3D11/D3D11Resources/PixelShaderManager.h>
#include"GraphicEngine/D3D11/D3D11Resources/D3DManager.h"
#include<GraphicEngine/D3D11/D3D11Resources/VertexBuffer.h>
#include<GraphicEngine/D3D11/D3D11Resources/VertexBufferManager.h>
#include<GraphicEngine/Utilities/Headers/VertexMesh.h>
#include<GraphicEngine/D3D11/D3D11Resources/IndexBuffer.h>
#include<GraphicEngine/D3D11/D3D11Resources/IndexBufferManager.h>
#include<GraphicEngine/D3D11/D3D11Resources/ConstantBuffer.h>
#include<GraphicEngine/D3D11/D3D11Resources/ConstantBufferManager.h>
#include<GraphicEngine/ECS/Entity/Primitive/Primitive.h>

PrimitiveManager::PrimitiveManager(D3D11Manager* p_D3DManager, ResourceManager* p_ResourceManager)
	:
	pD3DManager(p_D3DManager),
	pResourceManager(p_ResourceManager)
{
	if (!p_D3DManager || !p_ResourceManager)
		throw NORMAL_EXCEPT("PrimitiveManager constructor failed");
}


PrimitiveManager::~PrimitiveManager()
{
}

Primitive* PrimitiveManager::CreatePrimitive(Primitive_Desc* prim_desc)
{
	Mesh* mesh_Data = nullptr;
	int mesh_uid, vBuffer_uid, iBuffer_uid;
	mesh_uid = vBuffer_uid = iBuffer_uid = prim_desc->mesh_uid;
		
	if (prim_desc->getMeshfromFile)
	{
		mesh_Data = pResourceManager->pMeshManager->CreateMesh(mesh_uid);
		if (mesh_Data == nullptr)
		{
			releaseAll();
			return nullptr;
		}
	}
	else
	{
		mesh_Data = pResourceManager->pMeshManager->CreateMesh(
			prim_desc->mesh_creation_data->vertex_list_data, prim_desc->mesh_creation_data->vertex_list_size,
			prim_desc->mesh_creation_data->index_list_data, prim_desc->mesh_creation_data->index_list_size,
			prim_desc->mesh_creation_data->material_slot_list, prim_desc->mesh_creation_data->material_slot_list_size,
			prim_desc->mesh_creation_data->MeshName, prim_desc->primitive_uid);

		if (mesh_Data == nullptr)
		{
			releaseAll();
			return nullptr;
		}
	}

	std::vector<Texture*> list_texture_data;
	std::vector<Texture*> list_texture_data_normal;

	if (prim_desc->numOfTextures > 0)
	{
		for (unsigned int i = 0; i < prim_desc->numOfTextures; ++i)
		{
			Texture* texture_data = pResourceManager->pTextureManager->CreateTexture(prim_desc->texture_uids[i]);
			if (texture_data == nullptr)
			{
				releaseAll();
				return nullptr;
			}
			list_texture_data.push_back(texture_data);
			
			if (prim_desc->isNormalMap)
			{
				Texture* texture_data = pResourceManager->pTextureManager->CreateTexture(prim_desc->texture_normals_uids[i]);
				if (texture_data == nullptr)
				{
					releaseAll();
					return nullptr;
				}
				list_texture_data_normal.push_back(texture_data);
			}
		}
	}

	
	VertexShader* vShader = pD3DManager->pVShaderManager->CreateVertexShader(prim_desc->vshader_entry_point, prim_desc->vshader_version, prim_desc->vertex_Shader_uid);
	if (vShader == nullptr)
	{
		releaseAll();
		return nullptr;
	}

	PixelShader* pShader = pD3DManager->pPShaderManager->CreatePixelShader(prim_desc->pshader_entry_point, prim_desc->pshader_version, prim_desc->pixel_Shader_uid);
	if (pShader == nullptr)
	{
		releaseAll();
		return nullptr;
	}

	VertexBuffer* vBuffer = pD3DManager->pVertexBufferManager->CreateVertexBuffer(
		mesh_Data->GetVertexMesh().data(), sizeof(VertexMesh), mesh_Data->getSizeVertices(),
		vShader->GetShader_Byte_Code(), vShader->GetBufferSize(),
		prim_desc->inLayout, prim_desc->sizeLayout, mesh_Data->GetMeshName(), vBuffer_uid);
	if (vBuffer == nullptr)
	{
		releaseAll();
		return nullptr;
	}

	IndexBuffer* iBuffer = pD3DManager->pIndexBufferManager->CreateIndexBuffer(mesh_Data->GetIndicesList().data(), mesh_Data->getSizeIndices(), mesh_Data->GetMeshName(), iBuffer_uid);
	if (iBuffer == nullptr)
	{
		releaseAll();
		return nullptr;
	}

	ConstantBuffer* cBuffer = pD3DManager->pConstantBufferManager->CreateConstantBuffer(prim_desc->constant_buffer, prim_desc->size_constant_buffer, prim_desc->constant_buffer_uid);
	if (cBuffer == nullptr)
	{
		releaseAll();
		return nullptr;
	}

	Primitive* currPrimitive = new Primitive(mesh_Data, prim_desc->primitive_uid,
		vShader, pShader, vBuffer, iBuffer, cBuffer,
		list_texture_data, list_texture_data_normal, prim_desc->isNormalMap,
		prim_desc->numOfTextures, prim_desc->frontFaceCull,
		prim_desc->primitive_name, prim_desc->primitive_texture_type
	);

	PrimitiveContainer.push_back(currPrimitive);
	primitiveCounter++;
	return currPrimitive;
}

void PrimitiveManager::releaseAll()
{
	for (unsigned long long i = 0; i < PrimitiveContainer.size(); ++i)
	{
		Primitive* currPrimitive = PrimitiveContainer[i];
		delete currPrimitive;
	}
	pResourceManager->releaseAll();
	pD3DManager->releaseAll();
}

std::vector<Primitive*>& PrimitiveManager::GetPrimitiveContainer()
{
	return PrimitiveContainer;
}

bool PrimitiveManager::add_texture(Texture* new_tex, std::wstring primitive_Name)
{
	if (primitive_Name.empty() || !new_tex) return false;

	Primitive* prim = getPrimitive(primitive_Name);
	if (!prim) return false;

	prim->addTexture(new_tex);

	return true;
}

bool PrimitiveManager::add_texture(Texture* new_tex, const int& uid)
{
	if (uid < 0 || !new_tex) return false;

	Primitive* prim = getPrimitive(uid);
	if (!prim) return false;

	prim->addTexture(new_tex);

	return true;
}

bool PrimitiveManager::fill_texture(Texture* new_tex, std::wstring primitive_Name)
{
	if (primitive_Name.empty() || !new_tex) return false;

	Primitive* prim = getPrimitive(primitive_Name);
	if (!prim) return false;

	if (!prim->fillTexture(new_tex, new_tex->GetTextureName())) return false;

	return true;
}

bool PrimitiveManager::fill_texture(Texture* new_tex, const int& uid)
{
	if (uid < 0 || !new_tex) return false;

	Primitive* prim = getPrimitive(uid);
	if (!prim) return false;

	if (!prim->fillTexture(new_tex, new_tex->GetTextureName())) return false;

	return true;
}

Primitive* PrimitiveManager::getPrimitive(std::wstring primitive_Name)
{
	for (Primitive* curr_prim : PrimitiveContainer)
	{
		if (curr_prim->primitive_Name == primitive_Name)
			return curr_prim;
	}

	return nullptr;
}

Primitive* PrimitiveManager::getPrimitive(const int& uid)
{
	for (Primitive* curr_prim : PrimitiveContainer)
	{
		if (curr_prim->primitive_id == uid)
			return curr_prim;
	}

	return nullptr;
}

bool PrimitiveManager::DeletePrimitive(Primitive* pPrim)
{
	if (pPrim == nullptr) return false;
	bool primFound = false;
	unsigned int delete_Index = 0;

	for (unsigned int i = 0; i < PrimitiveContainer.size(); ++i)
	{
		if (PrimitiveContainer[i] == pPrim)
		{
			delete_Index = i;
			primFound = true;
			break;
		}
	}

	if (!primFound) return false;

	if (!pResourceManager->pMeshManager->freeMesh(pPrim->mesh_Data)) return false;

	for (short i = 0; i < pPrim->list_textures.size(); ++i)
	{
		if (!pResourceManager->pTextureManager->freeTexture(pPrim->list_textures[i]->pTexture)) return false;
	}

	for (short i = 0; i < pPrim->list_textures_normal.size(); ++i)
	{
		if (!pResourceManager->pTextureManager->freeTexture(pPrim->list_textures_normal[i]->pTexture)) return false;
	}

	if (!pD3DManager->pConstantBufferManager->freeConstantBuffer(pPrim->constantBuffer)) return false;
	if (!pD3DManager->pIndexBufferManager->freeIndexBuffer(pPrim->indexBuffer)) return false;
	if (!pD3DManager->pPShaderManager->freePixelShader(pPrim->pixelShader)) return false;
	if (!pD3DManager->pVertexBufferManager->freeVertexBuffer(pPrim->vertexBuffer)) return false;
	if (!pD3DManager->pVShaderManager->freeVertexShader(pPrim->vertexShader)) return false;

	PrimitiveContainer.erase(PrimitiveContainer.begin() + delete_Index);
	primitiveCounter--;
	delete pPrim;
	return true;
}
