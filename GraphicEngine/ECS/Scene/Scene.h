#pragma once
#include<string>
#include<d3d11.h>
#include<vector>
#include<unordered_map>
#include <typeindex>

#ifdef PROJ_EXPORT
#define ATT_Scene __declspec(dllexport)
#else
#define ATT_Scene __declspec(dllimport)
#endif

class Texture;
class Camera;
class D3D11Manager;
class ResourceManager;
struct Scene_descriptor;
struct EntityDesc;
class EntityManager;
class Entity;
struct CameraInitData;
struct CameraAttachDetails;

struct color_Scene
{
	float r = 0.0f;
	float g = 0.3f;
	float b = 0.4f;
	float a = 1.0f;
};


class ATT_Scene Scene
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
	unsigned int getWidth() const;
	unsigned int getHeight() const;
	std::wstring GetSceneName() const;
	bool getclearRenderTargetView() const;
	bool getclearDepthStencil() const;
	bool getUseDepthStencil() const;
	bool getdrawOnBackBuffer() const;
	int Get_Scene_uID() const;
public:
	Camera* getActiveCamera();
	Texture* GetSceneTexture();
	const std::unordered_map<std::type_index, std::vector<Entity*>>& GetEntityContainer() const;
	const std::unordered_map<int, Camera*>& Get_Camera_Container() const;

public:
	Camera* Get_Camera_by_uID(int uid);
	bool Activate_Camera(int uid);   // sets a camera to project, disables rest
	bool Attach_Camera(int Entiy_uID, int Camera_uID, CameraAttachDetails* CamDetails);
	bool Detach_Camera(int Camera_uID);
	
public:
	Entity* Get_Entity(int Entiy_uID);
	Camera* Get_Camera(int Camera_uID);

private:
	Entity* AddEntity(EntityDesc* pED);
	bool DeleteEntity(Entity* pEnt);
	bool UpdateTextureOnResize(unsigned int width, unsigned int height);

private:
	Camera* CreateCamera(CameraInitData* pCD);

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
	std::unordered_map<int, Camera*> CameraContainer;
	Camera* pCameraActive = nullptr;

private:
	EntityManager* pEntityManager = nullptr;
private:
	D3D11Manager* pd3dManager = nullptr;
	ResourceManager* presManager = nullptr;
};