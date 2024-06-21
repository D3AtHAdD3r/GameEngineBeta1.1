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

	if (!Update_Translation_Absolute(model_pos_world))
		throw NORMAL_EXCEPT("ModelData Constructor Failed. Update_Translation_Direct() Failed");

	if (!Update())
		throw NORMAL_EXCEPT("ModelData Constructor Failed. Update Failed");
}

ModelData::~ModelData()
{
}

const Vector3D& ModelData::Get_World_Pos() const
{
	return Translation;
}


const Vector3D& ModelData::Get_Rotation() const
{
	return Rotation;
}


const Vector3D& ModelData::Get_Scaling() const
{
	return Scaling;
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

	//movement Relative to current entity's x,y,z direction
	Vector3D new_pos = World_Matrix.getTranslation() + World_Matrix.getZDirection() * (mp.delta_translation_z * mp.move_speed);
	new_pos = new_pos + World_Matrix.getXDirection() * (mp.delta_translation_x * mp.move_speed);
	new_pos = new_pos + World_Matrix.getYDirection() * (mp.delta_translation_y * mp.move_speed);

	current_world_matrix.setTranslation(new_pos);
	World_Matrix = current_world_matrix;
	//Current_Translation = new_pos;

	Scaling = { mp.delta_scale_x, mp.delta_scale_y, mp.delta_scale_z };
	Rotation = { mp.delta_rotation_x, mp.delta_rotation_y, mp.delta_rotation_z };
	Translation = new_pos;

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

	Scaling = { mp.delta_scale_x, mp.delta_scale_y, mp.delta_scale_z };

	Vector3D smooth_rotation;
	Vector3D previous_rotation = Rotation;

	if (IsSmoothRotaion)
	{
		Rotation = { mp.delta_rotation_x, mp.delta_rotation_y, mp.delta_rotation_z };
		smooth_rotation = Vector3D::lerp(previous_rotation, Rotation, mp.delta_time * mp.lerp_Variable);
		Rotation = smooth_rotation;
	}
	else
	{
		smooth_rotation = Rotation = { mp.delta_rotation_x, mp.delta_rotation_y , mp.delta_rotation_z };
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
		world_pos_new = Vector3D::lerp(Translation, new_pos, mp.delta_time * mp.smooth_translation_variable);
		current_world_matrix.setTranslation(world_pos_new);
		World_Matrix = current_world_matrix;
		Translation = world_pos_new;
	}
	else
	{
		current_world_matrix.setTranslation(new_pos);
		World_Matrix = current_world_matrix;
		Translation = new_pos;
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

	
	Scaling = { mp.delta_scale_x, mp.delta_scale_y, mp.delta_scale_z };
	Rotation = { mp.delta_rotation_x, mp.delta_rotation_y, mp.delta_rotation_z };
	Translation = new_pos;

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

	Scaling = { mp.delta_scale_x, mp.delta_scale_y, mp.delta_scale_z };

	Vector3D smooth_rotation;
	Vector3D previous_rotation = Rotation;

	if (IsSmoothRotaion)
	{
		Rotation = { mp.delta_rotation_x, mp.delta_rotation_y, mp.delta_rotation_z };
		smooth_rotation = Vector3D::lerp(previous_rotation, Rotation, mp.delta_time * mp.lerp_Variable);
		Rotation = smooth_rotation;
	}
	else
	{
		smooth_rotation = Rotation = { mp.delta_rotation_x, mp.delta_rotation_y , mp.delta_rotation_z };
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
		world_pos_new = Vector3D::lerp(Translation, new_pos, mp.delta_time * mp.smooth_translation_variable);
		current_world_matrix.setTranslation(world_pos_new);
		World_Matrix = current_world_matrix;
		Translation = world_pos_new;
	}
	else
	{
		current_world_matrix.setTranslation(new_pos);
		World_Matrix = current_world_matrix;
		Translation = new_pos;
	}

	mp.delta_translation_x = mp.delta_translation_y = mp.delta_translation_z = 0;
	return true;
}

bool ModelData::Update_Translation_Absolute(const Vector3D& newVal)
{
	Translation = newVal;
	World_Matrix.setTranslation(Translation);

	if (!Update()) return false;

	return true;
}

bool ModelData::Update_Rotation(const Vector3D& newVal)
{
	mp.delta_rotation_x = newVal.m_x;
	mp.delta_rotation_y = newVal.m_y;
	mp.delta_rotation_z = newVal.m_z;

	if (!Update()) return false;

	return true;
}

bool ModelData::Update_Scaling(const Vector3D& newVal)
{
	mp.delta_scale_x = newVal.m_x;
	mp.delta_scale_y = newVal.m_y;
	mp.delta_scale_z = newVal.m_z;

	if (!Update()) return false;

	return true;
}



void ModelData::SetDataMembers()
{
	Scaling = { mp.delta_scale_x, mp.delta_scale_y, mp.delta_scale_z };
	Rotation = { mp.delta_rotation_x, mp.delta_rotation_y, mp.delta_rotation_z };
	Translation = { mp.delta_translation_x, mp.delta_translation_y, mp.delta_translation_z };

	IsSmoothRotaion = mp.SmoothRotation;
	IsSmoothMovement = mp.SmoothMovement;
}