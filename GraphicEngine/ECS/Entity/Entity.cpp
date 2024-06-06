#include "Entity.h"
#include<GraphicEngine/Utilities/Headers/Headers.h>
#include<GraphicEngine/ECS/ECSHeaders/EntityStructs.h>
#include<GraphicEngine/ECS/Entity/Primitive/Primitive.h>
#include<GraphicEngine/ECS/Components/ModelData.h>


Entity::Entity(Primitive* p_Primitive, EntityDesc* ent_desc) 
	:
	pPrimitive(p_Primitive),
	scene_id(ent_desc->Scene_Id)
{
	if (!p_Primitive || ent_desc->Scene_Id < 0)
		throw NORMAL_EXCEPT("Entity contructor failed");
	
	if (ent_desc->Entity_type == ENTITY_TYPE::LOCALPLAYER)
		this->bLocalPlayer = true;
	else
		this->bLocalPlayer = false;

	pModelData = new ModelData(ent_desc->model_initialPosition);
}

Entity::~Entity()
{
	if (pModelData) delete pModelData;
}

Vector3D Entity::Get_Entity_World_Pos() const
{
	return pModelData->world_pos;
}
Matrix4x4 Entity::Get_Entity_WorldMatrix() const
{
	return pModelData->world_matrix;
}

const std::wstring& Entity::Get_Entity_Name() const
{
	return pPrimitive->primitive_Name;
}

const bool& Entity::Get_LocalPlayer() const
{
	return bLocalPlayer;
}

const int& Entity::GetScene_ID() const
{
	return scene_id;
}

void Entity::UpdatePosition(ModelPositionData* mp, Camera* cp)
{
	pModelData->Update_Position(mp, cp);
}

void Entity::setConstantBuffer(void* c_buffer)
{
	pPrimitive->cBuff = c_buffer;
}

Primitive* Entity::GetPrimitive()
{
	return nullptr;
}
