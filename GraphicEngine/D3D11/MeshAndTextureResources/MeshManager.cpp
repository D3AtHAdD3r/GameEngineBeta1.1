#include "MeshManager.h"
#include<GraphicEngine/D3D11/MeshAndTextureResources/Mesh.h>
#include<GraphicEngine/D3D11/MeshAndTextureResources/MeshManagerHelpers.h>

MeshManager::MeshManager()
{
}

MeshManager::~MeshManager()
{
	for (auto& [key, pMesh] : MeshContainer)
	{
		if (pMesh)
			delete pMesh;
	}
}

Mesh* MeshManager::CreateMesh(std::wstring filePath, const int& u_id)
{
	if (u_id < 0) return nullptr;

	if (check_Exist(u_id))
	{
		MeshContainer[u_id]->incrementCounter();
		return MeshContainer[u_id];
	}
	else
	{
		if (filePath.empty()) return nullptr;

		bool flag = false;
		//getfilename(only)
		wchar_t fileName[FILENAME_MAX];
		wchar_t extensionName[FILENAME_MAX];
		std::wstring wstr;
		_wsplitpath_s(filePath.c_str(), 0, 0, 0, 0, fileName, FILENAME_MAX, extensionName, FILENAME_MAX);
		wstr.append(fileName); wstr.append(extensionName);

		Mesh* meshCurrent = new Mesh(u_id, wstr);

		if (!ParseObjFile(filePath.c_str(), meshCurrent->VertexAttributes, meshCurrent->IndicesList, meshCurrent->MaterialList))
		{
			delete meshCurrent;
			return nullptr;
		}

		meshCurrent->incrementCounter();
		MeshContainer.emplace(u_id, meshCurrent);
		ResourceCount++;

		return meshCurrent;
	}
}

Mesh* MeshManager::CreateMesh(VertexMesh* vertex_list_data, unsigned int vertex_list_size, unsigned int* index_list_data, unsigned int index_list_size, MaterialSlot* material_slot_list, unsigned int material_slot_list_size, std::wstring MeshName, const int& u_id)
{
	if (u_id < 0) return nullptr;

	if (check_Exist(u_id))
	{
		MeshContainer[u_id]->incrementCounter();
		return MeshContainer[u_id];
	}
	else
	{
		Mesh* meshCurrent = new Mesh(u_id, MeshName);
		for (unsigned int i = 0; i < vertex_list_size; ++i)
		{
			meshCurrent->VertexAttributes.push_back(vertex_list_data[i]);
		}
		for (unsigned int i = 0; i < index_list_size; ++i)
		{
			meshCurrent->IndicesList.push_back(index_list_data[i]);
		}
		for (unsigned int i = 0; i < material_slot_list_size; ++i)
		{
			meshCurrent->MaterialList.push_back(material_slot_list[i]);
		}

		meshCurrent->incrementCounter();
		MeshContainer.emplace(u_id, meshCurrent);
		ResourceCount++;

		return meshCurrent;
	}
}

void MeshManager::releaseAll()
{
	for (auto& [key, pMesh] : MeshContainer)
	{
		if (pMesh)
		{
			delete pMesh;
			pMesh = nullptr;
		}

	}

	MeshContainer.clear();
}

bool MeshManager::freeMesh(Mesh* pMesh)
{
	if (!pMesh || !check_Exist(pMesh->uID)) return false;

	pMesh->decrementCounter();
	if (pMesh->UserCounter <= 0)
		deleteMesh_direct(pMesh);

	return true;
}

bool MeshManager::check_Exist(const int& u_id)
{
	if (u_id == 0) return false;

	auto itr = MeshContainer.find(u_id);
	if (itr != MeshContainer.end())
		return true;
	else
		return false;
}

bool MeshManager::deleteMesh(Mesh* pMesh)
{
	int uid = pMesh->uID;

	if (!pMesh || !check_Exist(uid))
		return false;

	delete pMesh;
	MeshContainer.erase(uid);

	return true;
}

bool MeshManager::deleteMesh(const int& u_id)
{
	if (!check_Exist(u_id))
		return false;

	Mesh* currMesh = MeshContainer[u_id];
	delete currMesh;
	MeshContainer.erase(u_id);

	return true;
}

void MeshManager::deleteMesh_direct(Mesh* pMesh)
{
	MeshContainer.erase(pMesh->uID);
	delete pMesh;
}
