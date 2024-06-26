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
	pParentScene(ent_desc->Parent_Scene),
	isTerrainMesh(ent_desc->isTerrainMesh),
	TerrainSize(ent_desc->TerrainSize)
{
	if (!p_Primitive || ent_desc->Scene_Id < 0 || ent_desc->Entity_type == ENTITY_TYPE::ENUM_UNKNOWN || !ent_desc->Parent_Scene)
		throw NORMAL_EXCEPT("Entity contructor failed, Invalid Input;");


	ModelInitData InitData;
	InitData.Rotation = ent_desc->model_initialRotation;
	InitData.Scale = ent_desc->model_initialScaling;
	InitData.Tranlation = ent_desc->model_initialPosition;
	InitData.SmoothMovement = ent_desc->SmoothMovement;
	InitData.SmoothRotation = ent_desc->SmoothRotation;



	switch (ent_desc->Entity_type)
	{
	case ENTITY_TYPE::ENUM_NORMAL_ENTITY:
	{
		
		pModelData = new ModelData(&InitData);
		break;
	}
	case ENTITY_TYPE::ENUM_LOCALPLAYER:
	{
		bLocalPlayer = true;
		pModelData = new ModelData(&InitData);
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

ModelData* Entity::Get_ModelData()
{
	return pModelData;
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

bool Entity::Get_isTerrain() const
{
	return isTerrainMesh;
}

const Vector3D& Entity::Get_TerrainSize() const
{
	return TerrainSize;
}
