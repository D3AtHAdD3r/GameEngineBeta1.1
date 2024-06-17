#include "EntityManager.h"
#include<GraphicEngine/Utilities/Headers/Headers.h>
#include<GraphicEngine/ECS/Entity/Primitive/PrimitiveManager.h>
#include<GraphicEngine/ECS/ECSHeaders/EntityStructs.h>
#include<GraphicEngine/ECS/Entity/Entity.h>
#include<GraphicEngine/ECS/Entity/EntityChilds/LocalPlayer.h>
#include<GraphicEngine/ECS/Entity/EntityChilds/NormalEntity.h>
#include<GraphicEngine/ECS/Entity/EntityChilds/Entity_Camera.h>


EntityManager::EntityManager(D3D11Manager* p_D3DManager, ResourceManager* p_ResourceManager, Scene* p_Scene)
	:
	pd3dManager(p_D3DManager),
	presManager(p_ResourceManager),
	pScene(p_Scene)
{
	if (!p_D3DManager || !p_ResourceManager || !p_Scene)
		throw NORMAL_EXCEPT("EntityManager constructor failed");

	pPrimitiveManager = new PrimitiveManager(pd3dManager, presManager);
	if (!pPrimitiveManager)
		throw NORMAL_EXCEPT("EntityManager constructor failed, PrimitiveManager creation faied");
}

EntityManager::~EntityManager()
{
	for (auto& [typeIndex, EntityConatiner] : EntityContainer)
	{
		for (auto& currEnt : EntityConatiner)
		{
			delete currEnt;
		}
	}

	delete pPrimitiveManager;

}

Entity* EntityManager::CreateEntity(EntityDesc* ent_desc)
{
	//fill scene pointer of entity
	ent_desc->Parent_Scene = this->pScene;

	Primitive* current_Primitive = pPrimitiveManager->CreatePrimitive(ent_desc);
	if (!current_Primitive)
		return nullptr;

	switch (ent_desc->Entity_type)
	{
	case ENTITY_TYPE::ENUM_NORMAL_ENTITY:
	{
		NormalEntity* current_Entity = new NormalEntity(current_Primitive, ent_desc);

		if (EntityContainer.find(typeid(NormalEntity)) != EntityContainer.end())
			EntityContainer[typeid(NormalEntity)].push_back(current_Entity);
		else
			EntityContainer[typeid(NormalEntity)] = { current_Entity };

		NumberOfEntities++;
		return current_Entity;
		break;
	}

	case ENTITY_TYPE::ENUM_LOCALPLAYER:
	{
		LocalPlayer* current_Entity = new LocalPlayer(current_Primitive, ent_desc);
		
		if (EntityContainer.find(typeid(LocalPlayer)) != EntityContainer.end())
			EntityContainer[typeid(LocalPlayer)].push_back(current_Entity);
		else
			EntityContainer[typeid(LocalPlayer)] = { current_Entity };

		NumberOfEntities++;
		return current_Entity;
		break;
	}
	}

	return nullptr;
}

const std::unordered_map<std::type_index, std::vector<Entity*>>& EntityManager::Get_Entity_Container() const
{
	return EntityContainer;
}

bool EntityManager::DeleteEntity(Entity* pEnt)
{
	if (EntityContainer.find(typeid(*pEnt)) != EntityContainer.end())
	{
		std::vector<Entity*>& currEntContainer = EntityContainer[typeid(*pEnt)];

		for (int i = 0; i < currEntContainer.size(); ++i)
		{
			if (pEnt == currEntContainer[i])
			{
				if (pPrimitiveManager->DeletePrimitive(pEnt->pPrimitive))
				{
					//EntityContainer[typeid(*pEnt)].erase(EntityContainer[typeid(*pEnt)].begin() + i);
					currEntContainer.erase(currEntContainer.begin() + i);
					NumberOfEntities--;
					delete pEnt;
					return true;
				}
				else
				{
					return false;
				}

			}
		}
	}

	return false;
	
}
