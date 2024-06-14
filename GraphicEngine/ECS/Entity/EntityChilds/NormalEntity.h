#pragma once
#include<GraphicEngine/ECS/Entity/Entity.h>




class NormalEntity : public Entity
{
public:
	NormalEntity(Primitive* p_Primitive, EntityDesc* ent_desc);
	~NormalEntity();

public:
	void UpdatePosition(ModelPositionData* mp, Camera* cp);

public:
	void Set_Rotaion(float radians_x, float radians_y, float radians_z);
	Vector3D Get_Rotation();

private:
	float rotX = 0;
	float rotY = 0;
	float rotZ = 0;
};

