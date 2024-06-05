#include "VertexShaderManager.h"
#include<GraphicEngine/D3D11/D3D11Resources/VertexShader.h>



VertexShaderManager::VertexShaderManager(const std::unordered_map<int, std::wstring>& File_Map)
	:
	FileMap(File_Map)
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

VertexShader* VertexShaderManager::CreateVertexShader(std::string entryPointName, std::string shader_model, const int& u_id)
{
	if (entryPointName.empty() || shader_model.empty() || u_id < 0)
		return nullptr;

	if (check_Exist(u_id))
	{
		VertexShaderContainer[u_id]->incrementCounter();
		return VertexShaderContainer[u_id];
	}
	else
	{
		std::wstring shaderfilename = GetFileName(u_id);
		if (shaderfilename.empty()) 
			return nullptr;
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


bool VertexShaderManager::CreateAll(std::string entryPointName, std::string shader_model)
{
	for (auto itr = FileMap.begin(); itr != FileMap.end(); ++itr)
	{
		int uid = itr->first;
		//std::wstring ShaderFilename = itr->second;
		VertexShader* pshader = CreateVertexShader(entryPointName, shader_model, uid);
		if (!pshader) 
			return false;
	}
	
	return true;
}

bool VertexShaderManager::check_Exist(const int& u_id)
{
	if (u_id < 0) return false;

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

std::wstring VertexShaderManager::GetFileName(const int& uID)
{
	if (uID < 0) return std::wstring();

	auto itr = FileMap.find(uID);
	if (itr != FileMap.end())
		return itr->second;
	else
		return std::wstring();
}
	
