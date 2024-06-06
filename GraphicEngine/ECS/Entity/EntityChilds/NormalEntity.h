#pragma once
#include<GraphicEngine/ECS/Entity/Entity.h>




class NormalEntity : public Entity
{
public:
	NormalEntity(Primitive* p_Primitive, EntityDesc* ent_desc);
	~NormalEntity();

public:
	virtual void Set_Rotaion(float radians_x, float radians_y, float radians_z) override;
	virtual Vector3D Get_Rotation() override;

private:
	float rotX = 0;
	float rotY = 0;
	float rotZ = 0;
};

