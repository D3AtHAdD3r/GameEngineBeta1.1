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
	bool Update();
	bool UpdatePosition(ModelPositionData* mp);
	bool UpdatepositionRelative(ModelPositionData* mp, Matrix4x4* ModelB_World_Matrix);

public:
	//----------------------------------------//
	//----------Testing Stuff-----------------//
	//----------------------------------------//
	//for testing, like attaching camera to an entity
	bool Attach(Entity* ent_parent, EntityAttachDetails* EntDetails);
	bool Detach();
	bool UpdateAttached();
	bool isAttached = false;
	Entity* parentEntity = nullptr;
	ModelData* parentEntity_ModelData = nullptr;
	EntityAttachDetails entity_Attach_Details;
	//----------------------------------------//
	//----------------------------------------//

};

