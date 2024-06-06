#include "EntityManager.h"
#include<GraphicEngine/Utilities/Headers/Headers.h>
#include<GraphicEngine/ECS/Entity/Primitive/PrimitiveManager.h>
#include<GraphicEngine/ECS/ECSHeaders/EntityStructs.h>
#include<GraphicEngine/ECS/Entity/Entity.h>
#include<GraphicEngine/ECS/Entity/EntityChilds/LocalPlayer.h>
#include<GraphicEngine/ECS/Entity/EntityChilds/NormalEntity.h>



EntityManager::EntityManager(D3D11Manager* p_D3DManager, ResourceManager* p_ResourceManager)
	:
	pd3dManager(p_D3DManager),
	presManager(p_ResourceManager)
{
	if (!p_D3DManager || !p_ResourceManager)
		throw NORMAL_EXCEPT("EntityManager constructor failed");

	pPrimitiveManager = new PrimitiveManager(pd3dManager, presManager);
	if (!pPrimitiveManager)
		throw NORMAL_EXCEPT("EntityManager constructor failed, PrimitiveManager creation faied");
}

EntityManager::~EntityManager()
{
	for (auto currEntity : EntityContainer)
	{
		delete currEntity;
	}

	delete pPrimitiveManager;
}

Entity* EntityManager::CreateEntity(EntityDesc* ent_desc)
{
	Primitive* current_Primitive = pPrimitiveManager->CreatePrimitive(ent_desc);
	if (!current_Primitive)
		return nullptr;

	switch (ent_desc->Entity_type)
	{
	case ENTITY_TYPE::DEFAULT_ENTITY:
	{
		Entity* current_Entity = new Entity(current_Primitive, ent_desc);
		this->EntityContainer.push_back(current_Entity);
		NumberOfEntities++;
		return current_Entity;
		break;
	}
	case ENTITY_TYPE::LOCALPLAYER:
	{
		LocalPlayer* current_Entity = new LocalPlayer(current_Primitive, ent_desc);
		this->EntityContainer.push_back(current_Entity);
		NumberOfEntities++;
		return current_Entity;
		break;
	}
	case ENTITY_TYPE::NORMAL_ENTITY:
	{
		NormalEntity* current_Entity = new NormalEntity(current_Primitive, ent_desc);
		this->EntityContainer.push_back(current_Entity);
		NumberOfEntities++;
		return current_Entity;
		break;
	}

	}

	return nullptr;
}

const std::vector<Entity*>& EntityManager::Get_Entity_Container() const
{
	return EntityContainer;
}

bool EntityManager::DeleteEntity(Entity* pEnt)
{
	for (int i = 0; i < EntityContainer.size(); ++i)
	{
		if (pEnt == EntityContainer[i])
		{
			if (pPrimitiveManager->DeletePrimitive(pEnt->pPrimitive))
			{
				EntityContainer.erase(EntityContainer.begin() + i);
				NumberOfEntities--;
				delete pEnt;
				return true;
			}

		}
	}

	return false;
}
