#include "ModelData.h"
#include<GraphicEngine/Utilities/ErrorChecking/CustomException.h>



ModelData::ModelData()
{
}

ModelData::ModelData(ModelPositionData* mp_update, const Vector3D& model_pos_world)
{
	if (!mp_update)
		throw NORMAL_EXCEPT("ModelData Constructor Failed. Invalid Input");
	mp = *mp_update;
	SetDataMembers();
	Update_Translation_Direct(model_pos_world);
}

ModelData::~ModelData()
{
}

const Vector3D& ModelData::Get_Current_World_Pos() const
{
	return Current_Translation;
}

const Vector3D& ModelData::Get_Previous_World_pos() const
{
	return Previous_Translation;
}

const Vector3D& ModelData::Get_Current_Rotation() const
{
	return Current_Rotation;
}

const Vector3D& ModelData::Get_Previous_Rotation() const
{
	return Previous_Rotation;
}

const Vector3D& ModelData::Get_Current_Scaling() const
{
	return Current_Scaling;
}

const Vector3D& ModelData::Get_Previous_Scaling() const
{
	return Previous_Scaling;
}

const Matrix4x4& ModelData::Get_World_Matirx() const
{
	return World_Matrix;
}

const ModelPositionData& ModelData::Get_Model_Position_Data() const
{
	return mp;
}

bool ModelData::Update()
{
	if (IsSmoothRotaion || IsSmoothMovement)
		return Update_default_Smooth_Internal();
	else
		return Update_default_Internal();
}

bool ModelData::Update(ModelPositionData* mp_update)
{
	if (!mp_update) return false;
	mp = *mp_update;

	if (IsSmoothRotaion || IsSmoothMovement)
		return Update_default_Smooth_Internal();
	else
		return Update_default_Internal();
}

bool ModelData::Update_Relative(ModelPositionData* mp_update, Matrix4x4* ModelB_World_Matrix)
{
	if (!mp_update || !ModelB_World_Matrix) return false;
	mp = *mp_update;

	if (IsSmoothRotaion || IsSmoothMovement)
		return Update_Relative_Internal(ModelB_World_Matrix);
	else
		return Update_Relative_Smooth_Internal(ModelB_World_Matrix);

}

bool ModelData::Update_default_Internal()
{
	Matrix4x4 temp;
	Matrix4x4 current_world_matrix;
	temp.setIdentity();
	current_world_matrix.setIdentity();

	temp.setScale(Vector3D(mp.delta_scale_x, mp.delta_scale_y, mp.delta_scale_z));
	current_world_matrix *= temp;

	temp.setIdentity();
	temp.setRotationX(mp.delta_rotation_x);
	current_world_matrix *= temp;

	temp.setIdentity();
	temp.setRotationY(mp.delta_rotation_y);
	current_world_matrix *= temp;

	temp.setIdentity();
	temp.setRotationZ(mp.delta_rotation_z);
	current_world_matrix *= temp;

	//movement in relation to current entity's x,y,z direction
	Vector3D new_pos = World_Matrix.getTranslation() + World_Matrix.getZDirection() * (mp.delta_translation_z * mp.move_speed);
	new_pos = new_pos + World_Matrix.getXDirection() * (mp.delta_translation_x * mp.move_speed);
	new_pos = new_pos + World_Matrix.getYDirection() * (mp.delta_translation_y * mp.move_speed);

	current_world_matrix.setTranslation(new_pos);
	World_Matrix = current_world_matrix;
	//Current_Translation = new_pos;

	//SetDataMembers
	Previous_Scaling = Current_Scaling;
	Previous_Rotation = Current_Rotation;
	Previous_Translation = Current_Translation;

	Current_Scaling = { mp.delta_scale_x, mp.delta_scale_y, mp.delta_scale_z };
	Current_Rotation = { mp.delta_rotation_x, mp.delta_rotation_y, mp.delta_rotation_z };
	Current_Translation = new_pos;

	mp.delta_translation_x = mp.delta_translation_y = mp.delta_translation_z = 0;
	return true;
}



bool ModelData::Update_default_Smooth_Internal()
{
	Matrix4x4 temp;
	Matrix4x4 current_world_matrix;
	temp.setIdentity();
	current_world_matrix.setIdentity();

	temp.setScale(Vector3D(mp.delta_scale_x, mp.delta_scale_y, mp.delta_scale_z));
	current_world_matrix *= temp;

	Previous_Scaling = Current_Scaling;
	Current_Scaling = { mp.delta_scale_x, mp.delta_scale_y, mp.delta_scale_z };

	Vector3D smooth_rotation;

	if (IsSmoothRotaion)
	{
		Current_Rotation = { mp.delta_rotation_x, mp.delta_rotation_y, mp.delta_rotation_z };
		smooth_rotation = Vector3D::lerp(Previous_Rotation, Current_Rotation, mp.delta_time * mp.lerp_Variable);
		Previous_Rotation = smooth_rotation;
	}
	else
	{
		Previous_Rotation = Current_Rotation;
		smooth_rotation = Current_Rotation = { mp.delta_rotation_x, mp.delta_rotation_y , mp.delta_rotation_z };
	}

	temp.setIdentity();
	temp.setRotationX(smooth_rotation.m_x);
	current_world_matrix *= temp;

	temp.setIdentity();
	temp.setRotationY(smooth_rotation.m_y);
	current_world_matrix *= temp;

	temp.setIdentity();
	temp.setRotationZ(smooth_rotation.m_z);
	current_world_matrix *= temp;

	//movement in relation to current entity's x,y,z direction
	Vector3D new_pos = World_Matrix.getTranslation() + World_Matrix.getZDirection() * (mp.delta_translation_z * mp.move_speed);
	new_pos = new_pos + World_Matrix.getXDirection() * (mp.delta_translation_x * mp.move_speed);
	new_pos = new_pos + World_Matrix.getYDirection() * (mp.delta_translation_y * mp.move_speed);


	if (IsSmoothMovement)
	{
		Vector3D world_pos_new;
		world_pos_new = Vector3D::lerp(Current_Translation, new_pos, mp.delta_time * mp.smooth_translation_variable);
		current_world_matrix.setTranslation(world_pos_new);
		World_Matrix = current_world_matrix;
		Previous_Translation = Current_Translation;
		Current_Translation = world_pos_new;
	}
	else
	{
		current_world_matrix.setTranslation(new_pos);
		World_Matrix = current_world_matrix;
		Previous_Translation = Current_Translation;
		Current_Translation = new_pos;
	}

	mp.delta_translation_x = mp.delta_translation_y = mp.delta_translation_z = 0;
	return true;
}

bool ModelData::Update_Relative_Internal(Matrix4x4* ModelB_World_Matrix)
{
	Matrix4x4 temp;
	Matrix4x4 current_world_matrix;
	temp.setIdentity();
	current_world_matrix.setIdentity();

	temp.setScale(Vector3D(mp.delta_scale_x, mp.delta_scale_y, mp.delta_scale_z));
	current_world_matrix *= temp;

	temp.setIdentity();
	temp.setRotationX(mp.delta_rotation_x);
	current_world_matrix *= temp;

	temp.setIdentity();
	temp.setRotationY(mp.delta_rotation_y);
	current_world_matrix *= temp;

	temp.setIdentity();
	temp.setRotationZ(mp.delta_rotation_z);
	current_world_matrix *= temp;

	//movement in relation to ModelB's x,y,z direction
	Vector3D new_pos = World_Matrix.getTranslation() + ModelB_World_Matrix->getZDirection() * (mp.delta_translation_z * mp.move_speed);
	new_pos = new_pos + ModelB_World_Matrix->getXDirection() * (mp.delta_translation_x * mp.move_speed);
	new_pos = new_pos + ModelB_World_Matrix->getYDirection() * (mp.delta_translation_y * mp.move_speed);

	current_world_matrix.setTranslation(new_pos);
	World_Matrix = current_world_matrix;
	//Current_Translation = new_pos;

	//SetDataMembers
	Previous_Scaling = Current_Scaling;
	Previous_Rotation = Current_Rotation;
	Previous_Translation = Current_Translation;

	Current_Scaling = { mp.delta_scale_x, mp.delta_scale_y, mp.delta_scale_z };
	Current_Rotation = { mp.delta_rotation_x, mp.delta_rotation_y, mp.delta_rotation_z };
	Current_Translation = new_pos;

	mp.delta_translation_x = mp.delta_translation_y = mp.delta_translation_z = 0;
	return true;
}

bool ModelData::Update_Relative_Smooth_Internal(Matrix4x4* ModelB_World_Matrix)
{
	Matrix4x4 temp;
	Matrix4x4 current_world_matrix;
	temp.setIdentity();
	current_world_matrix.setIdentity();

	temp.setScale(Vector3D(mp.delta_scale_x, mp.delta_scale_y, mp.delta_scale_z));
	current_world_matrix *= temp;

	Previous_Scaling = Current_Scaling;
	Current_Scaling = { mp.delta_scale_x, mp.delta_scale_y, mp.delta_scale_z };

	Vector3D smooth_rotation;

	if (IsSmoothRotaion)
	{
		Current_Rotation = { mp.delta_rotation_x, mp.delta_rotation_y, mp.delta_rotation_z };
		smooth_rotation = Vector3D::lerp(Previous_Rotation, Current_Rotation, mp.delta_time * mp.lerp_Variable);
		Previous_Rotation = smooth_rotation;
	}
	else
	{
		Previous_Rotation = Current_Rotation;
		smooth_rotation = Current_Rotation = { mp.delta_rotation_x, mp.delta_rotation_y , mp.delta_rotation_z };
	}

	temp.setIdentity();
	temp.setRotationX(smooth_rotation.m_x);
	current_world_matrix *= temp;

	temp.setIdentity();
	temp.setRotationY(smooth_rotation.m_y);
	current_world_matrix *= temp;

	temp.setIdentity();
	temp.setRotationZ(smooth_rotation.m_z);
	current_world_matrix *= temp;

	//movement in relation to ModelB's x,y,z direction
	Vector3D new_pos = World_Matrix.getTranslation() + ModelB_World_Matrix->getZDirection() * (mp.delta_translation_z * mp.move_speed);
	new_pos = new_pos + ModelB_World_Matrix->getXDirection() * (mp.delta_translation_x * mp.move_speed);
	new_pos = new_pos + ModelB_World_Matrix->getYDirection() * (mp.delta_translation_y * mp.move_speed);


	if (IsSmoothMovement)
	{
		Vector3D world_pos_new;
		world_pos_new = Vector3D::lerp(Current_Translation, new_pos, mp.delta_time * mp.smooth_translation_variable);
		current_world_matrix.setTranslation(world_pos_new);
		World_Matrix = current_world_matrix;
		Previous_Translation = Current_Translation;
		Current_Translation = world_pos_new;
	}
	else
	{
		current_world_matrix.setTranslation(new_pos);
		World_Matrix = current_world_matrix;
		Previous_Translation = Current_Translation;
		Current_Translation = new_pos;
	}

	mp.delta_translation_x = mp.delta_translation_y = mp.delta_translation_z = 0;
	return true;
}

bool ModelData::Update_Translation_Direct(const Vector3D& newVal)
{
	Previous_Translation = Current_Translation;
	Current_Translation = newVal;
	World_Matrix.setTranslation(Current_Translation);
	return true;
}

bool ModelData::Update_Rotation_Direct(const Vector3D& newVal)
{
	Matrix4x4 temp;
	temp.setIdentity();
	temp.setRotationX(newVal.m_x);
	World_Matrix *= temp;

	temp.setIdentity();
	temp.setRotationY(newVal.m_y);
	World_Matrix *= temp;

	temp.setIdentity();
	temp.setRotationZ(newVal.m_z);
	World_Matrix *= temp;

	World_Matrix.setTranslation(Current_Translation);

	return true;
}

bool ModelData::Update_Scaling_Direct(const Vector3D& newVal)
{
	Matrix4x4 temp;
	temp.setIdentity();
	temp.setScale(newVal);
	World_Matrix *= temp;

	return true;
}



void ModelData::SetDataMembers()
{
	Previous_Scaling = Current_Scaling;
	Previous_Rotation = Current_Rotation;
	Previous_Translation = Current_Translation;

	Current_Scaling = { mp.delta_scale_x, mp.delta_scale_y, mp.delta_scale_z };
	Current_Rotation = { mp.delta_rotation_x, mp.delta_rotation_y, mp.delta_rotation_z };
	Current_Translation = { mp.delta_translation_x, mp.delta_translation_y, mp.delta_translation_z };

	IsSmoothRotaion = mp.SmoothRotation;
	IsSmoothMovement = mp.SmoothMovement;
}