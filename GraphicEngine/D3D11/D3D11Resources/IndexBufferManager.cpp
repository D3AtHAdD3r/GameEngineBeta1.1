#include "IndexBufferManager.h"
#include<GraphicEngine/D3D11/D3D11Resources/IndexBuffer.h>

IndexBufferManager::IndexBufferManager()
{
}

IndexBufferManager::~IndexBufferManager()
{
	for (auto& [key, iBuffer] : IndexBufferContainer)
	{
		if (iBuffer)
			delete iBuffer;
	}
}

IndexBuffer* IndexBufferManager::CreateIndexBuffer(void* list_indices, const unsigned int& size_list, std::wstring mesh_Name, const int& u_id)
{
	if (!list_indices || !size_list || mesh_Name.empty() || u_id < 0) return nullptr;

	if (check_Exist(u_id))
	{
		IndexBufferContainer[u_id]->incrementCounter();
		return IndexBufferContainer[u_id];
	}
	else
	{
		IndexBuffer* currIndexBuffer = new IndexBuffer(list_indices, size_list, u_id, mesh_Name);
		currIndexBuffer->incrementCounter();
		IndexBufferContainer.emplace(u_id, currIndexBuffer);
		ResourceCount++;

		return currIndexBuffer;
	}
}

void IndexBufferManager::releaseAll()
{
	for (auto& [key, iBuffer] : IndexBufferContainer)
	{
		if (iBuffer)
		{
			delete iBuffer;
			iBuffer = nullptr;
		}

	}

	IndexBufferContainer.clear();
}

bool IndexBufferManager::check_Exist(const int& u_id)
{
	if (u_id == 0) return false;

	auto itr = IndexBufferContainer.find(u_id);
	if (itr != IndexBufferContainer.end())
		return true;
	else
		return false;
}

bool IndexBufferManager::DeleteIndexBuffer(IndexBuffer* pIbuff)
{
	int uid = pIbuff->uID;

	if (!pIbuff || !check_Exist(uid))
		return false;

	delete pIbuff;
	IndexBufferContainer.erase(uid);

	return true;
}

bool IndexBufferManager::DeleteIndexBuffer(const int& u_id)
{
	if (!check_Exist(u_id))
		return false;

	IndexBuffer* currBuff = IndexBufferContainer[u_id];
	delete currBuff;
	IndexBufferContainer.erase(u_id);

	return true;
}

void IndexBufferManager::DeleteIndexBuffer_direct(IndexBuffer* pIbuff)
{
	IndexBufferContainer.erase(pIbuff->uID);
	delete pIbuff;
}

bool IndexBufferManager::freeIndexBuffer(IndexBuffer* pIbuff)
{
	if (!pIbuff || !check_Exist(pIbuff->uID)) return false;

	pIbuff->decrementCounter();
	if (pIbuff->UserCounter <= 0)
		DeleteIndexBuffer_direct(pIbuff);

	return true;
}
