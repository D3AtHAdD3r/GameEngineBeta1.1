#pragma once




class D3D11Manager;
class ResourceManager;
class SceneManager;

class ECSCore
{
	friend class RendererDX11;
private:
	ECSCore(D3D11Manager* p_d3dManager, ResourceManager* p_resManager);
	~ECSCore();

private:
	SceneManager* pSceneManager = nullptr;
};