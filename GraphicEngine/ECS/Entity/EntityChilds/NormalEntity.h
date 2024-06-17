#pragma once
#include<GraphicEngine/ECS/Entity/Entity.h>

#ifdef PROJ_EXPORT
#define ATT_NormalEntity __declspec(dllexport)
#else
#define ATT_NormalEntity __declspec(dllimport)
#endif


class ATT_NormalEntity NormalEntity : public Entity
{
public:
	NormalEntity(Primitive* p_Primitive, EntityDesc* ent_desc);
	~NormalEntity();

public:
	bool UpdatePosition(ModelPositionData* mp);
	bool UpdatepositionRelative(ModelPositionData* mp, Matrix4x4* ModelB_World_Matrix);
public:
	void Set_Rotaion(float radians_x, float radians_y, float radians_z);
	Vector3D Get_Rotation();

private:
	float rotX = 0;
	float rotY = 0;
	float rotZ = 0;
};

