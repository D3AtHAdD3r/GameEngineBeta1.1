#pragma once
#include<unordered_map>


class D3D11Manager;
class ResourceManager;
struct Scene_descriptor;
struct EntityDesc;
class Entity;
class Scene;

class SceneManager
{
	friend class ECSCore;
private:
	SceneManager(D3D11Manager* p_d3dManager, ResourceManager* p_resManager);
	~SceneManager();

public:
	bool CreateSceneAndEntity(std::vector<Scene_descriptor*> sd_list, std::vector<EntityDesc*> ed_list);
	Entity* CreateEntity(EntityDesc* pED, bool check_ent_desc = true);
	bool DeleteEntity(Entity* pEnt, Scene* pScene = nullptr);

public:
	const std::unordered_map<unsigned short, Scene*>& GetSceneContainer();

private:
	bool CreateSceneOnly(Scene_descriptor* pSD, bool check_scene_desc = true);

private:
	bool check_scene_descriptor(Scene_descriptor* sd);
	bool check_Entity_desc(const EntityDesc* EntityDesc);

private:
	bool checkSceneExist(short SceneID);
	void ReloadAllResourceBuffers();

private:
	D3D11Manager* pD3DManager = nullptr;
	ResourceManager* pResManager = nullptr;

private:
	std::unordered_map<unsigned short, Scene*> sceneContainer;
};
