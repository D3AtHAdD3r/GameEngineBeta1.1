#pragma once
#include<GraphicEngine/ECS/Entity/Entity.h>




class LocalPlayer : public Entity
{
public:
	LocalPlayer(Primitive* p_Primitive, EntityDesc* ent_desc);
	~LocalPlayer();


public:
	void UpdatePosition(ModelPositionData* mp, Camera* cp);
};

