#pragma once
#include<string>
#include<GraphicEngine/Utilities/Headers/HeadersD3D.h>
class Texture;
//class Camera;


struct color_Scene
{
	float r = 0.0f;
	float g = 0.3f;
	float b = 0.4f;
	float a = 1.0f;
};

class Scene
{

public:
	ID3D11Resource* getTexture2D();
	ID3D11RenderTargetView* getRTV();
	ID3D11DepthStencilView* getDSV();
	ID3D11ShaderResourceView* getSRV();
	color_Scene getColorBackground();
	unsigned int getWidth();
	unsigned int getHeight();
	std::wstring GetSceneName();
	bool getclearRenderTargetView();
	bool getclearDepthStencil();
	bool getUseDepthStencil();
	//Camera* getCamera();
	bool getdrawOnBackBuffer();
	Texture* GetSceneTexture();

private:
	Texture* scene_texture = nullptr;

private:
	bool drawOnBackBuffer = true;
	color_Scene color_background;
	bool clearRenderTargetView = true;
	bool useDepthStencil = true;
	bool clearDepthStencil = true;

private:
	std::wstring sceneName;
	unsigned short scene_id = 0;

private:
	float width_ratio = 0;
	float height_ratio = 0;
	unsigned int width = 0;
	unsigned int height = 0;
};