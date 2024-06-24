#include "NormalEntity.h"
#include<GraphicEngine/ECS/Components/ModelData.h>

NormalEntity::NormalEntity(Primitive* p_Primitive, EntityDesc* ent_desc) : Entity(p_Primitive, ent_desc)
{
}

NormalEntity::~NormalEntity()
{
}


bool NormalEntity::Update()
{
	if (isAttached)
		return UpdateAttached();
	else
		return pModelData->Update();
}

bool NormalEntity::UpdatePosition(ModelPositionData* mp)
{
	 return pModelData->Update(mp);
}

bool NormalEntity::UpdatepositionRelative(ModelPositionData* mp, Matrix4x4* ModelB_World_Matrix)
{
	return pModelData->Update_Relative(mp, ModelB_World_Matrix);
}



bool NormalEntity::Attach(Entity* ent_parent, EntityAttachDetails* EntDetails)
{
	if (!ent_parent || !EntDetails) return false;

	parentEntity = ent_parent;
	parentEntity_ModelData = ent_parent->Get_ModelData();

	entity_Attach_Details = *EntDetails;

	isAttached = true;
	return true;
}


bool NormalEntity::UpdateAttached()
{
	if (!isAttached) return false;
	
	Vector3D CurrentParentPos = parentEntity->Get_ModelData()->Get_World_Pos();
	Vector3D CurrentParentRotation = parentEntity->Get_ModelData()->Get_Rotation();
	Vector3D CurrentParentSCaling = parentEntity->Get_ModelData()->Get_Scaling();

	Vector3D CurrentRotation = CurrentParentRotation + pModelData->Get_Initial_Rotation();

	float offsetX = entity_Attach_Details.delta_offset_model_x;
	float offsetY = entity_Attach_Details.delta_offset_model_y;
	float offsetZ = entity_Attach_Details.delta_offset_model_z;

	//Get Translation, relative to parent entity, update it
	Vector3D new_pos = CurrentParentPos + parentEntity->Get_ModelData()->Get_World_Matirx().getZDirection() * (offsetZ);
	new_pos = new_pos + parentEntity->Get_ModelData()->Get_World_Matirx().getXDirection() * (offsetX);
	new_pos = new_pos + parentEntity->Get_ModelData()->Get_World_Matirx().getYDirection() * (offsetY);


	if (!pModelData->Update_Translation_Absolute(new_pos)) return false;
	if (!pModelData->Update_Rotation(CurrentRotation)) return false;

	return true;
}