#include "MeshManager.h"
#include<GraphicEngine/D3D11/MeshAndTextureResources/Mesh.h>
#include<GraphicEngine/D3D11/MeshAndTextureResources/MeshManagerHelpers.h>
#include<GraphicEngine/Utilities/Math/Vector2D.h>

MeshManager::MeshManager(const std::unordered_map<int, std::wstring>& File_Map)
	:
	FileMap(File_Map)
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

Mesh* MeshManager::CreateMesh(const int& u_id)
{
	if (u_id < 0) return nullptr;

	if (check_Exist(u_id))
	{
		MeshContainer[u_id]->incrementCounter();
		return MeshContainer[u_id];
	}
	else
	{
		std::wstring filePath = GetFileName(u_id);
		if (filePath.empty())
			return nullptr;

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

Mesh* MeshManager::CreateTerrainMesh(unsigned int w, unsigned int h, const int& u_id, std::wstring MeshName)
{
	if (!w || !h) return nullptr;
	if (check_Exist(u_id))
	{
		MeshContainer[u_id]->incrementCounter();
		return MeshContainer[u_id];
	}
	else
	{
		Mesh* meshCurrent = new Mesh(u_id, MeshName);

		const unsigned int ww = w - 1; //quads
		const unsigned int hh = h - 1;

		meshCurrent->VertexAttributes.reserve(w * h);
		meshCurrent->IndicesList.reserve(ww * hh * 6);

		meshCurrent->VertexAttributes = std::vector<VertexMesh>(w * h, VertexMesh());
		meshCurrent->IndicesList = std::vector<unsigned int>(ww * hh * 6, 0);

		auto i = 0;
		for (unsigned int x = 0; x < w; x++)
		{
			for (unsigned int y = 0; y < h; y++)
			{
				//VertexMesh mesh;
				//Vertex attributes

				int vertIndex = y * w + x;

				meshCurrent->VertexAttributes[y * w + x] =
				{
					Vector3D((float)x / (float)ww, 0, ((float)y / (float)hh) /** -1.0f*/),  //world COORDINATES
					Vector2D((float)x / (float)ww, ((float)y / (float)hh) /** -1.0f*/),      //TEX Coordinates
					Vector3D(),             //Tangent, normal, binormal(not in order)
					Vector3D(),
					Vector3D()

				};

				//Index attributes
				if (x < ww && y < hh)
				{
					//counterclockwise winding
					{
						/*meshCurrent->IndicesList[i] = (y + 1) * w + (x);
						meshCurrent->IndicesList[i + 1] = (y) * w + (x);
						meshCurrent->IndicesList[i + 2] = (y) * w + (x + 1);

						meshCurrent->IndicesList[i + 3] = (y) * w + (x + 1);
						meshCurrent->IndicesList[i + 4] = (y + 1) * w + (x + 1);
						meshCurrent->IndicesList[i + 5] = (y + 1) * w + (x);*/
					}


					//clockwise winding
					{
						meshCurrent->IndicesList[i] = (y)*w + (x);
						meshCurrent->IndicesList[i + 1] = (y + 1) * w + (x);
						meshCurrent->IndicesList[i + 2] = (y)*w + (x + 1);

						meshCurrent->IndicesList[i + 3] = (y + 1) * w + (x + 1);
						meshCurrent->IndicesList[i + 4] = (y)*w + (x + 1);
						meshCurrent->IndicesList[i + 5] = (y + 1) * w + (x);
					}

					i += 6;
				}

			}
		}

		MaterialSlot mt;
		mt.material_id = 0;
		mt.start_index = 0;
		mt.num_indices = meshCurrent->IndicesList.size();

		meshCurrent->MaterialCount = 1;
		meshCurrent->MaterialList.push_back(mt);

		meshCurrent->SizeVertices = meshCurrent->VertexAttributes.size();
		meshCurrent->SizeIndices = meshCurrent->IndicesList.size();

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
	if (u_id < 0) return false;

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

std::wstring MeshManager::GetFileName(const int& uID)
{
	if (uID < 0) return std::wstring();

	auto itr = FileMap.find(uID);
	if (itr != FileMap.end())
		return itr->second;
	else
		return std::wstring();
}
