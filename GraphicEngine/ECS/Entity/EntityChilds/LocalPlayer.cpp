#include "LocalPlayer.h"
#include<GraphicEngine/ECS/Components/ModelData.h>





LocalPlayer::LocalPlayer(Primitive* p_Primitive, EntityDesc* ent_desc) 
	: Entity(p_Primitive, ent_desc)
{
}

LocalPlayer::~LocalPlayer()
{
}

void LocalPlayer::UpdatePosition(ModelPositionData* mp, Camera* cp)
{
	pModelData->Update_Position_Smooth_Movement(mp, cp);
}
