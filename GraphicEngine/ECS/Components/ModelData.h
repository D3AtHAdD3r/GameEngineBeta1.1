#pragma once
#include<GraphicEngine/Utilities/Math/Vector3D.h>
#include<GraphicEngine/Utilities/Math/Matrix4x4.h>
#include<GraphicEngine/ECS/ECSHeaders/EntityStructs.h>



class ModelData
{
	friend class Entity;
	friend class Camera;
public:
	ModelData();
	ModelData(ModelPositionData* mp_update, const Vector3D& model_pos_world);
	~ModelData();

public:
	const Vector3D& Get_Current_World_Pos() const;
	const Vector3D& Get_Previous_World_pos() const;
	const Vector3D& Get_Current_Rotation() const;
	const Vector3D& Get_Previous_Rotation() const;
	const Vector3D& Get_Current_Scaling() const;
	const Vector3D& Get_Previous_Scaling() const;
	const Matrix4x4& Get_World_Matirx() const;
	const ModelPositionData& Get_Model_Position_Data() const;

public:
	bool Update();
	bool Update(ModelPositionData* mp_update);
	bool Update_Relative(ModelPositionData* mp_update, Matrix4x4* ModelB_World_Matrix);

public:
	bool Update_Translation_Direct(const Vector3D& newVal);		//Update Translation Direct(no incremental)
	bool Update_Rotation_Direct(const Vector3D& newVal);		//Only updates Rotation
	bool Update_Scaling_Direct(const Vector3D& newVal);

private:
	bool Update_default_Internal();
	bool Update_default_Smooth_Internal();
	bool Update_Relative_Internal(Matrix4x4* ModelB_World_Matrix);
	bool Update_Relative_Smooth_Internal(Matrix4x4* ModelB_World_Matrix);

private:
	void SetDataMembers();

private:
	ModelPositionData mp;
private:
	Vector3D Current_Translation;
	Vector3D Previous_Translation;
	Vector3D Current_Rotation;
	Vector3D Previous_Rotation;
	Vector3D Current_Scaling{ 1.0f,1.0f,1.0f };
	Vector3D Previous_Scaling;
	Matrix4x4 World_Matrix;
private:
	bool IsSmoothRotaion = false;
	bool IsSmoothMovement = false;
	float move_speed = 0.55f;
	float current_move_speed = 0;
};