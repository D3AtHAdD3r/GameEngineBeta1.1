#include "Scene.h"

ID3D11Resource* Scene::getTexture2D()
{
	return nullptr;
}

ID3D11RenderTargetView* Scene::getRTV()
{
	return nullptr;
}

ID3D11DepthStencilView* Scene::getDSV()
{
	return nullptr;
}

ID3D11ShaderResourceView* Scene::getSRV()
{
	return nullptr;
}

color_Scene Scene::getColorBackground()
{
	return color_Scene();
}

unsigned int Scene::getWidth()
{
	return 0;
}

unsigned int Scene::getHeight()
{
	return 0;
}

std::wstring Scene::GetSceneName()
{
	return std::wstring();
}

bool Scene::getclearRenderTargetView()
{
	return false;
}

bool Scene::getclearDepthStencil()
{
	return false;
}

bool Scene::getUseDepthStencil()
{
	return false;
}

bool Scene::getdrawOnBackBuffer()
{
	return false;
}

Texture* Scene::GetSceneTexture()
{
	return nullptr;
}
