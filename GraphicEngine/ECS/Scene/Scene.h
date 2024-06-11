#pragma once
#include<string>
#include<GraphicEngine/Utilities/Headers/HeadersD3D.h>


class Texture;
class Camera;
class D3D11Manager;
class ResourceManager;
struct Scene_descriptor;
struct EntityDesc;
class EntityManager;
class Entity;

struct color_Scene
{
	float r = 0.0f;
	float g = 0.3f;
	float b = 0.4f;
	float a = 1.0f;
};


class Scene
{
	friend class SceneManager;
private:
	Scene(D3D11Manager* p_D3DManager, ResourceManager* p_ResourceManager, Scene_descriptor* sd);
	~Scene();

public:
	bool UpdateOnResize(unsigned int client_width, unsigned int client_height);
	

public:
	ID3D11Resource* getTexture2D();
	ID3D11RenderTargetView* getRTV();
	ID3D11DepthStencilView* getDSV();
	ID3D11ShaderResourceView* getSRV();
	color_Scene getColorBackground();
	const unsigned int& getWidth() const;
	const unsigned int& getHeight() const;
	const std::wstring& GetSceneName() const;
	const bool& getclearRenderTargetView() const;
	const bool& getclearDepthStencil() const;
	const bool& getUseDepthStencil() const;
	const bool& getdrawOnBackBuffer() const;
	Camera* getCamera();
	Texture* GetSceneTexture();
	const std::vector<Entity*>& GetEntityContainer() const;

private:
	Entity* AddEntity(EntityDesc* pED);
	bool DeleteEntity(Entity* pEnt);
	bool UpdateTextureOnResize(unsigned int width, unsigned int height);

private:
	Camera* CreateCamera(Scene_descriptor* pSD);

private:
	bool drawOnBackBuffer = true;
	short BackBuffer_Index = 0;
private:
	color_Scene color_background;
	bool clearRenderTargetView = true;
	bool useDepthStencil = true;
	bool clearDepthStencil = true;

private:
	std::wstring sceneName;
	short scene_id = -1;
	int scene_texture_uid = -1;
private:
	float width_ratio = 0;
	float height_ratio = 0;
	unsigned int width = 0;
	unsigned int height = 0;

private:
	Texture* scene_texture = nullptr;
private:
	Camera* pcam = nullptr;
	bool getInputEvents = false;

private:
	EntityManager* pEntityManager = nullptr;
private:
	D3D11Manager* pd3dManager = nullptr;
	ResourceManager* presManager = nullptr;
};