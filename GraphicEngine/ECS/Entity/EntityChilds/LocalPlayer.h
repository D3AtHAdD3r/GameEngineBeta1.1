#pragma once
#include<GraphicEngine/ECS/Entity/Entity.h>




class LocalPlayer : public Entity
{
public:
	LocalPlayer(Primitive* p_Primitive, EntityDesc* ent_desc);
	~LocalPlayer();


public:
	bool UpdatePosition(ModelPositionData* mp);
	bool UpdatepositionRelative(ModelPositionData* mp, Matrix4x4* ModelB_World_Matrix);
};

