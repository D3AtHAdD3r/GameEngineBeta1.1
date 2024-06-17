#include "LocalPlayer.h"
#include<GraphicEngine/ECS/Components/ModelData.h>





LocalPlayer::LocalPlayer(Primitive* p_Primitive, EntityDesc* ent_desc) 
	: Entity(p_Primitive, ent_desc)
{
}

LocalPlayer::~LocalPlayer()
{
}

bool LocalPlayer::UpdatePosition(ModelPositionData* mp)
{
	return pModelData->Update(mp);
}

bool LocalPlayer::UpdatepositionRelative(ModelPositionData* mp, Matrix4x4* ModelB_World_Matrix)
{
	return pModelData->Update_Relative(mp, ModelB_World_Matrix);
}


