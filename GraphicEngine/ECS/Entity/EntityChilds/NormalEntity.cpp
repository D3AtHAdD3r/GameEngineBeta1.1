#include "NormalEntity.h"

NormalEntity::NormalEntity(Primitive* p_Primitive, EntityDesc* ent_desc) : Entity(p_Primitive, ent_desc)
{
}

NormalEntity::~NormalEntity()
{
}

void NormalEntity::Set_Rotaion(float radians_x, float radians_y, float radians_z)
{
	this->rotX = radians_x;
	this->rotY = radians_y;
	this->rotZ = radians_z;
}

Vector3D NormalEntity::Get_Rotation()
{
	return Vector3D(this->rotX, this->rotY, this->rotZ);
}
