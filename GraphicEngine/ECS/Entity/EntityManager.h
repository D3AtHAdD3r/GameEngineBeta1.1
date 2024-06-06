#pragma once
#include<vector>

class D3D11Manager;
class ResourceManager;
class Entity;
struct EntityDesc;
class PrimitiveManager;

class EntityManager
{
	friend class Scene;
private:
	EntityManager(D3D11Manager* p_D3DManager, ResourceManager* p_ResourceManager);
	~EntityManager();

public:
	Entity* CreateEntity(EntityDesc* ent_desc);
	const std::vector<Entity*>& Get_Entity_Container() const;

	bool DeleteEntity(Entity* pEnt);

private:
	PrimitiveManager* pPrimitiveManager = nullptr;

private:
	D3D11Manager* pd3dManager = nullptr;
	ResourceManager* presManager = nullptr;

private:
	std::vector<Entity*> EntityContainer;
	unsigned long long NumberOfEntities = 0;
};
