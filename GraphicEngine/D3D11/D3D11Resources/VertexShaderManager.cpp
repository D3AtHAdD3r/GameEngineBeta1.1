#include "VertexShaderManager.h"
#include<GraphicEngine/D3D11/D3D11Resources/VertexShader.h>



VertexShaderManager::VertexShaderManager()
{
}

VertexShaderManager::~VertexShaderManager()
{
	for (auto& [key, vShader] : VertexShaderContainer)
	{
		if (vShader)
			delete vShader;
	}
}

VertexShader* VertexShaderManager::CreateVertexShader(std::wstring shaderfilename, std::string entryPointName, std::string shader_model, const int& u_id)
{
	if (shaderfilename.empty() || entryPointName.empty() || shader_model.empty() || u_id < 0)
		return nullptr;

	if (check_Exist(u_id))
	{
		VertexShaderContainer[u_id]->incrementCounter();
		return VertexShaderContainer[u_id];
	}
	else
	{
		VertexShader* currVertexShader = new VertexShader(shaderfilename, entryPointName, shader_model, u_id);
		currVertexShader->incrementCounter();
		VertexShaderContainer.emplace(u_id, currVertexShader);
		ResourceCount++;

		return currVertexShader;
	}


}

bool VertexShaderManager::freeVertexShader(VertexShader* pVShader)
{
	if (!pVShader || !check_Exist(pVShader->uID)) return false;

	pVShader->decrementCounter();
	if (pVShader->UserCounter <= 0)
		DeleteVertexShader_direct(pVShader);

	return true;
}


void VertexShaderManager::releaseAll()
{
	for (auto& [key, vShader] : VertexShaderContainer)
	{
		if (vShader)
		{
			delete vShader;
			vShader = nullptr;
		}

	}

	VertexShaderContainer.clear();
}

bool VertexShaderManager::check_Exist(const int& u_id)
{
	if (u_id == 0) return false;

	auto itr = VertexShaderContainer.find(u_id);
	if (itr != VertexShaderContainer.end())
		return true;
	else
		return false;
}

bool VertexShaderManager::DeleteVertexShader(VertexShader* pVShader)
{
	int uid = pVShader->uID;

	if (!pVShader || !check_Exist(uid))
		return false;

	delete pVShader;
	VertexShaderContainer.erase(uid);

	return true;
}

bool VertexShaderManager::DeleteVertexShader(const int& u_id)
{
	if (!check_Exist(u_id))
		return false;

	VertexShader* currBuff = VertexShaderContainer[u_id];
	delete currBuff;
	VertexShaderContainer.erase(u_id);

	return true;
}

void VertexShaderManager::DeleteVertexShader_direct(VertexShader* pVShader)
{
	VertexShaderContainer.erase(pVShader->uID);
	delete pVShader;
}
	
