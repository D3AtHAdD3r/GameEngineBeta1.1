#pragma once
#include<GraphicEngine/Utilities/Math/Vector3D.h>
#include<GraphicEngine/Utilities/Math/Matrix4x4.h>

class Camera;
struct ModelPositionData;

class ModelData
{
private:
	ModelData(const Vector3D& model_pos_world);
	~ModelData();

private:
	Vector3D GetWorldPos();
	Matrix4x4 GetWorldMatrix();

	//public- childs of Entity dont inherit friendship. 
public:
	void Update_Position(ModelPositionData* mp, Camera* cp);
	void Update_Position_Smooth_Movement(ModelPositionData* mp, Camera* cp);

private:
	Vector3D world_pos;
	Vector3D world_pos_new;
	Matrix4x4 world_matrix;
private:
	float move_speed = 0.55f;
	float current_move_speed = 0;
private:
	Vector3D old_rotation;
	Vector3D current_rotation;

private:
	friend class Entity;
};