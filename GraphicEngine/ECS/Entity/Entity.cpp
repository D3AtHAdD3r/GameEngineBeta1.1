#include "Entity.h"
#include<GraphicEngine/Utilities/Headers/Headers.h>
#include<GraphicEngine/ECS/Entity/Primitive/Primitive.h>
#include<GraphicEngine/ECS/Components/ModelData.h>
#include<sstream>


Entity::Entity(Primitive* p_Primitive, EntityDesc* ent_desc) 
	:
	pPrimitive(p_Primitive),
	scene_id(ent_desc->Scene_Id),
	Is_Renderable(ent_desc->Is_Renderable),
	Entity_Type(ent_desc->Entity_type),
	Entity_uid(ent_desc->primitive_uid),
	Entity_name(ent_desc->primitive_name),
	pParentScene(ent_desc->Parent_Scene)
{
	if (!p_Primitive || ent_desc->Scene_Id < 0 || ent_desc->Entity_type == ENTITY_TYPE::ENUM_UNKNOWN || !ent_desc->Parent_Scene)
		throw NORMAL_EXCEPT("Entity contructor failed, Invalid Input;");


	ModelPositionData mp;
	mp.delta_rotation_x = ent_desc->model_initialPosition.m_x;
	mp.delta_rotation_y = ent_desc->model_initialPosition.m_y;
	mp.delta_rotation_z = ent_desc->model_initialPosition.m_z;

	mp.delta_scale_x = ent_desc->model_initialScaling.m_x;
	mp.delta_scale_y = ent_desc->model_initialScaling.m_y;
	mp.delta_scale_z = ent_desc->model_initialScaling.m_z;


	switch (ent_desc->Entity_type)
	{
	case ENTITY_TYPE::ENUM_NORMAL_ENTITY:
	{
		
		pModelData = new ModelData(&mp, ent_desc->model_initialPosition);
		break;
	}
	case ENTITY_TYPE::ENUM_LOCALPLAYER:
	{
		bLocalPlayer = true;
		pModelData = new ModelData(&mp, ent_desc->model_initialPosition);
		break;
	}
	
	default:
	{
		throw NORMAL_EXCEPT("Entity contructor failed, Invalid Input - ENTITY_TYPE");
	}
	}
}

Entity::~Entity()
{
	if (pModelData) delete pModelData;
}

Vector3D Entity::Get_Entity_World_Pos() const
{
	if (!pModelData)
	{
		std::ostringstream oss;
		oss << "Entity::Get_Entity_World_Pos() Failed :: pModelData is nullptr. \n" 
			<< "Entity uid : " << Entity_uid << std::endl;
		throw NORMAL_EXCEPT(oss.str());
	}
	
	return pModelData->Translation;
}


Matrix4x4 Entity::Get_Entity_WorldMatrix() const
{
	if (!pModelData)
	{
		std::ostringstream oss;
		oss << "Entity::Get_Entity_WorldMatrix() Failed :: pModelData is nullptr. \n"
			<< "Entity uid : " << Entity_uid << std::endl;
		throw NORMAL_EXCEPT(oss.str());
	}

	return pModelData->World_Matrix;
}

const std::wstring& Entity::Get_Entity_Name() const
{
	return Entity_name;
}

const bool& Entity::Get_LocalPlayer() const
{
	return bLocalPlayer;
}

const int& Entity::GetScene_ID() const
{
	return scene_id;
}


void Entity::setConstantBuffer(void* c_buffer)
{
	if (!pPrimitive)
	{
		std::ostringstream oss;
		oss << "Entity::setConstantBuffer Failed :: pPrimitive is nullptr. \n"
			<< "Entity uid : " << Entity_uid << std::endl;
		throw NORMAL_EXCEPT(oss.str());
	}
	pPrimitive->cBuff = c_buffer;
}

void Entity::setConstantBuffer(constant* c_buffer)
{
	if (!pPrimitive)
	{
		std::ostringstream oss;
		oss << "Entity::setConstantBuffer Failed :: pPrimitive is nullptr. \n"
			<< "Entity uid : " << Entity_uid << std::endl;
		throw NORMAL_EXCEPT(oss.str());
	}
	constant* cbuff = (constant*)(pPrimitive->cBuff);
	memcpy(cbuff, c_buffer, sizeof(constant));
}

bool Entity::Attach(Entity* ent_parent, EntityAttachDetails* EntDetails)
{
	if (!ent_parent || !EntDetails) return false;

	parentEntity = ent_parent;
	parentEntity_ModelData = ent_parent->Get_ModelData();
	
	entity_Attach_Details = *EntDetails;

	isAttached = true;
	return true;
}

bool Entity::UpdateAttached()
{
	if (!isAttached) return false;
	ModelPositionData mp = parentEntity->Get_ModelData()->Get_Model_Position_Data();
	

	Vector3D CurrentParentPos = parentEntity->pModelData->Translation;
	Vector3D CurrentParentRotation = parentEntity->pModelData->Rotation;
	Vector3D CurrentParentSCaling = parentEntity->pModelData->Scaling;
	

	float offsetX = entity_Attach_Details.delta_offset_model_x * CurrentParentSCaling.m_x;
	float offsetY = entity_Attach_Details.delta_offset_model_y * CurrentParentSCaling.m_y;
	float offsetZ = entity_Attach_Details.delta_offset_model_z * CurrentParentSCaling.m_z;

	//Get Translation, relative to parent entity, update it
	Vector3D new_pos = CurrentParentPos + pModelData->World_Matrix.getZDirection() * (offsetZ);
	new_pos = new_pos + pModelData->World_Matrix.getXDirection() * (offsetX);
	new_pos = new_pos + pModelData->World_Matrix.getYDirection() * (offsetY);

	if (!pModelData->Update_Translation_Absolute(new_pos)) return false;

	//Update Rotation
	if (!pModelData->Update_Rotation(CurrentParentRotation)) return false;

	
	return true;
}


Primitive* Entity::GetPrimitive()
{
	return pPrimitive;
}

const int& Entity::Get_Entity_uID() const
{
	return Entity_uid;
}

const bool& Entity::Get_IsRenderable() const
{
	return Is_Renderable;
}

Scene* Entity::Get_Parent_Scene() const
{
	return pParentScene;
}

ModelData* Entity::Get_ModelData() const
{
	return pModelData;
}
