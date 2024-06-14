#pragma once
#include<GraphicEngine/ECS/Entity/Entity.h>




class Entity_Camera : public Entity
{
public:
	Entity_Camera(Primitive* p_Primitive, EntityDesc* ent_desc);
	~Entity_Camera();

public:
	virtual Vector3D Get_Entity_World_Pos() const override;
	virtual Matrix4x4 Get_Entity_WorldMatrix() const override;

public:
	void updateTPC(Vector3D player_pos, float delta_time_lerp);
	bool isTPP();
	void set_cam_distance(float distance);
	void updateCamera();
	void setTranslation(Vector3D new_pos);
};