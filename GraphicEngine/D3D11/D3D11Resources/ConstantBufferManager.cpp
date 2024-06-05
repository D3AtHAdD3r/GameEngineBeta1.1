#include "ConstantBufferManager.h"
#include<GraphicEngine/D3D11/D3D11Resources/ConstantBuffer.h>

ConstantBufferManager::ConstantBufferManager()
{
}

ConstantBufferManager::~ConstantBufferManager()
{
	for (auto& [key, cBuffer] : ConstantBufferContainer)
	{
		if (cBuffer)
			delete cBuffer;
	}
}

ConstantBuffer* ConstantBufferManager::CreateConstantBuffer(void* buffer, const unsigned int& size_buffer, const unsigned short& u_id)
{
	if (u_id < 0)
		return nullptr;

	if (check_Exist(u_id))
	{
		ConstantBufferContainer[u_id]->incrementCounter();
		return ConstantBufferContainer[u_id];
	}
	else
	{
		ConstantBuffer* currConstantBuffer = new ConstantBuffer(buffer, size_buffer, u_id);
		currConstantBuffer->incrementCounter();
		ConstantBufferContainer.emplace(u_id, currConstantBuffer);
		ResourceCount++;

		return currConstantBuffer;
	}
}

void ConstantBufferManager::releaseAll()
{
	for (auto& [key, cBuffer] : ConstantBufferContainer)
	{
		if (cBuffer)
		{
			delete cBuffer;
			cBuffer = nullptr;
		}
			
	}

	ConstantBufferContainer.clear();
}


bool ConstantBufferManager::freeConstantBuffer(ConstantBuffer* pCbuff)
{
	if (!pCbuff || !check_Exist(pCbuff->uID)) return false;

	pCbuff->decrementCounter();
	if (pCbuff->UserCounter <= 0)
		DeleteConstantBuffer_direct(pCbuff);

	return true;
}

bool ConstantBufferManager::check_Exist(const unsigned short& u_id)
{
	if (u_id < 0) return false;

	auto itr = ConstantBufferContainer.find(u_id);
	if (itr != ConstantBufferContainer.end())
		return true;
	else
		return false;
}


bool ConstantBufferManager::DeleteConstantBuffer(ConstantBuffer* pCbuff)
{
	short uid = pCbuff->uID;

	if (!pCbuff || !check_Exist(uid))
		return false;

	delete pCbuff;
	ConstantBufferContainer.erase(uid);

	return true;
}

bool ConstantBufferManager::DeleteConstantBuffer(short uID)
{
	if (!check_Exist(uID))
		return false;

	ConstantBuffer* currBuff = ConstantBufferContainer[uID];
	delete currBuff;
	ConstantBufferContainer.erase(uID);

	return true;
}

void ConstantBufferManager::DeleteConstantBuffer_direct(ConstantBuffer* pCbuff)
{
	ConstantBufferContainer.erase(pCbuff->uID);
	delete pCbuff;

}
