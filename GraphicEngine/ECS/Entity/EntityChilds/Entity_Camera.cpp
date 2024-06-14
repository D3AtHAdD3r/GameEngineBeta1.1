#include "Entity_Camera.h"
#include<GraphicEngine/ECS/Components/Camera.h>
#include<GraphicEngine/Utilities/ErrorChecking/CustomException.h>
#include<sstream>

Entity_Camera::Entity_Camera(Primitive* p_Primitive, EntityDesc* ent_desc)
	:
	Entity(p_Primitive, ent_desc)
{
}

Entity_Camera::~Entity_Camera()
{
}

Vector3D Entity_Camera::Get_Entity_World_Pos() const
{
	if (!pCamera)
	{
		std::ostringstream oss;
		oss << "Entity_Camera::Get_Entity_World_Pos() Failed :: pCamera is nullptr. \n"
			<< "Entity uid : " << Entity_uid << std::endl;
		throw NORMAL_EXCEPT(oss.str());
	}

	return pCamera->getCamWorldPos();
}

Matrix4x4 Entity_Camera::Get_Entity_WorldMatrix() const
{
	if (!pCamera)
	{
		std::ostringstream oss;
		oss << "Entity_Camera::--SomeFunction-- Failed :: pCamera is nullptr. \n"
			<< "Entity uid : " << Entity_uid << std::endl;
		throw NORMAL_EXCEPT(oss.str());
	}

	return pCamera->getWorldMatrix();
}

void Entity_Camera::updateTPC(Vector3D player_pos, float delta_time_lerp)
{
	if (!pCamera)
	{
		std::ostringstream oss;
		oss << "Entity_Camera::--SomeFunction-- Failed :: pCamera is nullptr. \n"
			<< "Entity uid : " << Entity_uid << std::endl;
		throw NORMAL_EXCEPT(oss.str());
	}
	pCamera->updateTPC(player_pos, delta_time_lerp);
}

bool Entity_Camera::isTPP()
{
	return pCamera->isTPP();
}

void Entity_Camera::set_cam_distance(float distance)
{
	if (!pCamera)
	{
		std::ostringstream oss;
		oss << "Entity_Camera::--SomeFunction-- Failed :: pCamera is nullptr. \n"
			<< "Entity uid : " << Entity_uid << std::endl;
		throw NORMAL_EXCEPT(oss.str());
	}
	pCamera->set_cam_distance(distance);
}

void Entity_Camera::updateCamera()
{
	if (!pCamera)
	{
		std::ostringstream oss;
		oss << "Entity_Camera::--SomeFunction-- Failed :: pCamera is nullptr. \n"
			<< "Entity uid : " << Entity_uid << std::endl;
		throw NORMAL_EXCEPT(oss.str());
	}
	pCamera->updateCamera();
}

void Entity_Camera::setTranslation(Vector3D new_pos)
{
	if (!pCamera)
	{
		std::ostringstream oss;
		oss << "Entity_Camera::--SomeFunction-- Failed :: pCamera is nullptr. \n"
			<< "Entity uid : " << Entity_uid << std::endl;
		throw NORMAL_EXCEPT(oss.str());
	}

	pCamera->setTranslation(new_pos);
}
