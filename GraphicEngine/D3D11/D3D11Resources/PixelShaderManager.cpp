#include "PixelShaderManager.h"
#include<GraphicEngine/D3D11/D3D11Resources/PixelShader.h>



PixelShaderManager::PixelShaderManager()
{
}

PixelShaderManager::~PixelShaderManager()
{
	for (auto& [key, pShader] : PixelShaderContainer)
	{
		if (pShader)
			delete pShader;
	}
}

PixelShader* PixelShaderManager::CreatePixelShader(std::wstring shaderfilename, std::string entryPointName, std::string shader_model, const int& u_id)
{
	if (shaderfilename.empty() || entryPointName.empty() || shader_model.empty() || u_id < 0)
		return nullptr;

	if (check_Exist(u_id))
	{
		PixelShaderContainer[u_id]->incrementCounter();
		return PixelShaderContainer[u_id];
	}
	else
	{
		PixelShader* currPixelShader = new PixelShader(shaderfilename, entryPointName, shader_model, u_id);
		currPixelShader->incrementCounter();
		PixelShaderContainer.emplace(u_id, currPixelShader);
		ResourceCount++;

		return currPixelShader;
	}


}

bool PixelShaderManager::freePixelShader(PixelShader* pPShader)
{
	if (!pPShader || !check_Exist(pPShader->uID)) return false;

	pPShader->decrementCounter();
	if (pPShader->UserCounter <= 0)
		DeletePixelShader_direct(pPShader);

	return true;
}


void PixelShaderManager::releaseAll()
{
	for (auto& [key, pShader] : PixelShaderContainer)
	{
		if (pShader)
		{
			delete pShader;
			pShader = nullptr;
		}

	}

	PixelShaderContainer.clear();
}

bool PixelShaderManager::check_Exist(const int& u_id)
{
	if (u_id == 0) return false;

	auto itr = PixelShaderContainer.find(u_id);
	if (itr != PixelShaderContainer.end())
		return true;
	else
		return false;
}

bool PixelShaderManager::DeletePixelShader(PixelShader* pPShader)
{
	int uid = pPShader->uID;

	if (!pPShader || !check_Exist(uid))
		return false;

	delete pPShader;
	PixelShaderContainer.erase(uid);

	return true;
}

bool PixelShaderManager::DeletePixelShader(const int& u_id)
{
	if (!check_Exist(u_id))
		return false;

	PixelShader* currBuff = PixelShaderContainer[u_id];
	delete currBuff;
	PixelShaderContainer.erase(u_id);

	return true;
}

void PixelShaderManager::DeletePixelShader_direct(PixelShader* pPShader)
{
	PixelShaderContainer.erase(pPShader->uID);
	delete pPShader;
}

