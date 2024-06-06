#include "ModelData.h"
#include<GraphicEngine/ECS/ECSHeaders/EntityStructs.h>
#include<GraphicEngine/ECS/Components/Camera.h>


ModelData::ModelData(const Vector3D& model_pos_world)
	:
	world_pos(model_pos_world)
{
	world_matrix.setIdentity();
	world_matrix.setTranslation(model_pos_world);
}

ModelData::~ModelData()
{
}

Vector3D ModelData::GetWorldPos()
{
	return world_pos;
}

Matrix4x4 ModelData::GetWorldMatrix()
{
	return world_matrix;
}

void ModelData::Update_Position(ModelPositionData* mp, Camera* cp)
{
	Matrix4x4 temp;
	Matrix4x4 current_world_matrix;
	temp.setIdentity();
	current_world_matrix.setIdentity();

	temp.setScale(Vector3D(mp->delta_scale_x, mp->delta_scale_y, mp->delta_scale_z));
	current_world_matrix *= temp;

	temp.setIdentity();
	temp.setRotationX(mp->delta_rotation_x);
	current_world_matrix *= temp;

	temp.setIdentity();
	temp.setRotationY(mp->delta_rotation_y);
	current_world_matrix *= temp;

	temp.setIdentity();
	temp.setRotationZ(mp->delta_rotation_z);
	current_world_matrix *= temp;

	current_move_speed = move_speed;

	//movement in relation to camera's z
	Vector3D new_pos = world_matrix.getTranslation() + cp->getWorldMatrix().getZDirection() * (mp->delta_translation_z * current_move_speed);
	new_pos = new_pos + cp->getWorldMatrix().getXDirection() * (mp->delta_translation_x * current_move_speed);

	current_world_matrix.setTranslation(new_pos);
	world_matrix = current_world_matrix;
	world_pos = new_pos;
}

void ModelData::Update_Position_Smooth_Movement(ModelPositionData* mp, Camera* cp)
{
	Matrix4x4 temp;
	Matrix4x4 current_world_matrix;
	temp.setIdentity();
	current_world_matrix.setIdentity();

	temp.setScale(Vector3D(mp->delta_scale_x, mp->delta_scale_y, mp->delta_scale_z));
	current_world_matrix *= temp;

	//----smooth rotation ---//
	current_rotation.m_x = mp->delta_rotation_x;
	current_rotation.m_y = mp->delta_rotation_y;
	current_rotation.m_z = mp->delta_rotation_z;

	Vector3D smooth_rotation = Vector3D::lerp(old_rotation, current_rotation, mp->delta_time * 7.0f);
	old_rotation = smooth_rotation;
	//----------------------//

	temp.setIdentity();
	temp.setRotationX(smooth_rotation.m_x);
	current_world_matrix *= temp;

	temp.setIdentity();
	temp.setRotationY(smooth_rotation.m_y);
	current_world_matrix *= temp;

	temp.setIdentity();
	temp.setRotationZ(smooth_rotation.m_z);
	current_world_matrix *= temp;

	if (cp->isturboMode())
	{
		current_move_speed = move_speed * 15.0f;
	}
	else
	{
		current_move_speed = move_speed;
	}

	Vector3D new_pos = world_matrix.getTranslation() + cp->getWorldMatrix().getZDirection() * (mp->delta_translation_z * current_move_speed);
	new_pos = new_pos + cp->getWorldMatrix().getXDirection() * (mp->delta_translation_x * current_move_speed);

	//----smooth movement---//
	world_pos_new = Vector3D::lerp(world_pos, new_pos, mp->delta_time * 25.0f);
	current_world_matrix.setTranslation(world_pos_new);
	world_matrix = current_world_matrix;
	world_pos = world_pos_new;
	//----------------------//
}
