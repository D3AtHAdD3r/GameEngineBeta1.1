#pragma once
#include<GraphicEngine/Utilities/Math/Vector3D.h>
#include<GraphicEngine/Utilities/Math/Matrix4x4.h>
#include<GraphicEngine/ECS/ECSHeaders/EntityStructs.h>

#ifdef PROJ_EXPORT
#define ATT_Model __declspec(dllexport)
#else
#define ATT_Model __declspec(dllimport)
#endif

class ATT_Model ModelData
{
	friend class Entity;
	friend class Camera;
public:
	ModelData();
	ModelData(ModelInitData* mp_update);
	~ModelData();

public:
	const Vector3D& Get_World_Pos() const;
	const Vector3D& Get_Rotation() const;
	const Vector3D& Get_Initial_Rotation() const;
	const Vector3D& Get_Scaling() const;
	const Matrix4x4& Get_World_Matirx() const;
	const ModelPositionData& Get_Model_Position_Data() const;

public:
	bool Update();
	bool Update(ModelPositionData* mp_update);
	bool Update_Relative(ModelPositionData* mp_update, Matrix4x4* ModelB_World_Matrix);

public:
	bool Update_Translation_Absolute(const Vector3D& newVal);		//Update Translation Direct(no incremental)
	bool Update_Rotation(const Vector3D& newVal);		//Only updates Rotation
	bool Update_Scaling(const Vector3D& newVal); //needs fixing

private:
	bool Update_default_Internal();
	bool Update_default_Smooth_Internal();
	bool Update_Relative_Internal(Matrix4x4* ModelB_World_Matrix);			// Not used in normal update procedure
	bool Update_Relative_Smooth_Internal(Matrix4x4* ModelB_World_Matrix);	// Not used in normal update procedure

private:
	ModelPositionData mp;
private:
	Vector3D Translation;
	Vector3D Rotation;
	Vector3D Scaling{ 1.0f,1.0f,1.0f };
	Vector3D Initial_Rotation;
	Matrix4x4 World_Matrix;
private:
	bool IsSmoothRotaion = false;
	bool IsSmoothMovement = false;
	float move_speed = 0.55f;
	float current_move_speed = 0;
};