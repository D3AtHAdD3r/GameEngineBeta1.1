#pragma once
#include<GraphicEngine/Utilities/Math/Vector3D.h>
#include<GraphicEngine/Utilities/Math/Matrix4x4.h>

struct ModelPositionData;
class Camera;

enum lightType
{
	directional = 0,
	pointLight
};


class Light
{
public:
	Light();
	Light(Vector3D light_origin_world, lightType lt);
	~Light();

public:
	Matrix4x4 getLightSourceWorldMatrix();
	Vector3D getLightZDirection();
	Vector3D getLightPosition();

	void updatePosition(ModelPositionData* mp, Camera* cp);
	void setTranslation(Vector3D new_pos);

private:
	Vector3D m_world_pos{ 0,0,0 };
	Matrix4x4 m_world_matrix;
	float move_speed = 0.05f;

private:
	lightType light_type;
};