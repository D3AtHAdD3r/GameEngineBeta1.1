#include "VertexBufferManager.h"
#include<GraphicEngine/D3D11/D3D11Resources/VertexBuffer.h>



VertexBufferManager::VertexBufferManager()
{
}

VertexBufferManager::~VertexBufferManager()
{
	for (auto& [key, vShader] : VertexBufferContainer)
	{
		if (vShader)
			delete vShader;
	}
}


VertexBuffer* VertexBufferManager::CreateVertexBuffer(void* list_vertices, unsigned int size_vertex, unsigned size_list_vertex, void* shader_byte_code, size_t size_byte_shader, D3D11_INPUT_ELEMENT_DESC* layout, unsigned size_layout, std::wstring mesh_Name, const int& u_id)
{
	if (mesh_Name.empty() || !list_vertices || !size_vertex || !shader_byte_code || !size_byte_shader || !layout || !size_layout || u_id < 0) 
		return nullptr;


	if (check_Exist(u_id))
	{
		VertexBufferContainer[u_id]->incrementCounter();
		return VertexBufferContainer[u_id];
	}
	else
	{
		VertexBuffer* currVertexBuffer = new VertexBuffer(list_vertices, size_vertex, size_list_vertex, shader_byte_code, size_byte_shader, layout, size_layout, mesh_Name, u_id);
		currVertexBuffer->incrementCounter();
		VertexBufferContainer.emplace(u_id, currVertexBuffer);
		ResourceCount++;

		return currVertexBuffer;
	}


}

bool VertexBufferManager::freeVertexBuffer(VertexBuffer* pVBuffer)
{
	if (!pVBuffer || !check_Exist(pVBuffer->uID)) return false;

	pVBuffer->decrementCounter();
	if (pVBuffer->UserCounter <= 0)
		DeleteVertexBuffer_direct(pVBuffer);

	return true;
}

void VertexBufferManager::releaseAll()
{
	for (auto& [key, vBuffer] : VertexBufferContainer)
	{
		if (vBuffer)
		{
			delete vBuffer;
			vBuffer = nullptr;
		}

	}

	VertexBufferContainer.clear();
}

bool VertexBufferManager::check_Exist(const int& u_id)
{
	if (u_id < 0) return false;

	auto itr = VertexBufferContainer.find(u_id);
	if (itr != VertexBufferContainer.end())
		return true;
	else
		return false;
}

bool VertexBufferManager::DeleteVertexBuffer(VertexBuffer* pVBuffer)
{
	int uid = pVBuffer->uID;

	if (!pVBuffer || !check_Exist(uid))
		return false;

	delete pVBuffer;
	VertexBufferContainer.erase(uid);

	return true;
}

bool VertexBufferManager::DeleteVertexBuffer(const int& u_id)
{
	if (!check_Exist(u_id))
		return false;

	VertexBuffer* currBuff = VertexBufferContainer[u_id];
	delete currBuff;
	VertexBufferContainer.erase(u_id);

	return true;
}

void VertexBufferManager::DeleteVertexBuffer_direct(VertexBuffer* pVBuffer)
{
	VertexBufferContainer.erase(pVBuffer->uID);
	delete pVBuffer;
}
