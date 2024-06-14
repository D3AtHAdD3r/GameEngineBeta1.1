#pragma once
#include<vector>
#include<unordered_map>
#include <typeindex>

class D3D11Manager;
class ResourceManager;
class Entity;
struct EntityDesc;
class PrimitiveManager;
class Scene;

class EntityManager
{
	friend class Scene;
private:
	EntityManager(D3D11Manager* p_D3DManager, ResourceManager* p_ResourceManager, Scene* p_Scene);
	~EntityManager();

public:
	Entity* CreateEntity(EntityDesc* ent_desc);
	const std::unordered_map<std::type_index, std::vector<Entity*>>& Get_Entity_Container() const;

	bool DeleteEntity(Entity* pEnt);

private:
	Scene* pScene = nullptr;
	PrimitiveManager* pPrimitiveManager = nullptr;

private:
	D3D11Manager* pd3dManager = nullptr;
	ResourceManager* presManager = nullptr;

private:
	std::unordered_map<std::type_index, std::vector<Entity*>> EntityContainer;

	unsigned long long NumberOfEntities = 0;
};
