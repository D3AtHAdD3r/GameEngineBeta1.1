#include "Light.h"
#include<GraphicEngine/ECS/Components/Camera.h>
#include<GraphicEngine/ECS/ECSHeaders/EntityStructs.h>
Light::Light()
{
    m_world_matrix.setIdentity();
}

Light::Light(Vector3D light_origin_world, lightType lt) 
    :
    m_world_pos(light_origin_world),
    light_type(lt)
{
    m_world_matrix.setIdentity();
    m_world_matrix.setTranslation(m_world_pos);
}

Light::~Light()
{
}

Matrix4x4 Light::getLightSourceWorldMatrix()
{
    return m_world_matrix;
}

Vector3D Light::getLightZDirection()
{
    return m_world_matrix.getZDirection();
}

Vector3D Light::getLightPosition()
{
    return m_world_pos;
}

void Light::updatePosition(ModelPositionData* mp, Camera* cp)
{
	Matrix4x4 temp;
	Matrix4x4 current_world_matrix;
	temp.setIdentity();
	current_world_matrix.setIdentity();

	temp.setScale(Vector3D(mp->delta_scale_x, mp->delta_scale_y, mp->delta_scale_z));
	current_world_matrix *= temp;

	temp.setIdentity();
	temp.setRotationX(mp->delta_rotation_x);
	current_world_matrix *= temp;

	temp.setIdentity();
	temp.setRotationY(mp->delta_rotation_y);
	current_world_matrix *= temp;

	temp.setIdentity();
	temp.setRotationZ(mp->delta_rotation_z);
	current_world_matrix *= temp;

	Vector3D new_pos = m_world_matrix.getTranslation() + cp->getWorldMatrix().getZDirection() * (mp->delta_translation_z * move_speed);
	new_pos = new_pos + cp->getWorldMatrix().getXDirection() * (mp->delta_translation_x * move_speed);

	current_world_matrix.setTranslation(new_pos);
	m_world_matrix = current_world_matrix;
	m_world_pos = new_pos;
}

void Light::setTranslation(Vector3D new_pos)
{
	m_world_pos = new_pos;
	m_world_matrix.setTranslation(m_world_pos);
}
