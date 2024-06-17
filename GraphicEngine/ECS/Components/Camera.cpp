#include "Camera.h"
#include<GraphicEngine/ECS/ECSHeaders/EntityStructs.h>
#include<GraphicEngine/InputHandling/InputSystem.h>
#include<GraphicEngine/Window/Window.h>
#include<GraphicEngine/Window/WindowGlobals.h>
#include<GraphicEngine/Utilities/Math/MathUtils.h>
#include<GraphicEngine/Utilities/ErrorChecking/CustomException.h>


Camera::Camera(CameraInitData* camData)
	:
	getInputControl(camData->get_Input),
	isTPC(camData->isTPC),
	uid(camData->u_id),
	isProjecting(camData->isprojecting)
{

	if (camData->u_id < 0)
		throw NORMAL_EXCEPT("Camera Constructor failed. Invalid input.");


	pCamData = new CameraTranslationData();

	//set world matrix
	Matrix4x4 temp;
	temp.setIdentity();

	m_world_matrix.setIdentity();
	m_world_matrix.setTranslation(m_world_pos_camera);

	//set view matrix
	temp = m_world_matrix;
	temp.inverse();
	m_view_matrix = temp;

	//set projection matrix
	float width = (float)(WindowGlobals::Get()->Get_WindowWidth());
	float height = (float)(WindowGlobals::Get()->Get_WindowHeight());
	float aspectRatio = width / height;

	Projection_Matrix.setPerspectiveFovLH(fov, aspectRatio, zNear, zFar);
	
	InputSystem::get()->addListener(this);
	Window::get()->addListner(this);

	if (isTPC)
		CreateTPC(Vector3D());
}

Camera::~Camera()
{
	//delete dt;
	delete pCamData;
}


Matrix4x4 Camera::getWorldMatrix()
{
    return m_world_matrix;
}

Matrix4x4 Camera::getViewMatrix()
{
	return m_view_matrix;
}

Vector3D Camera::getCamWorldPos()
{
	return m_world_pos_camera;
}

CameraTranslationData* Camera::getCamTranslationData()
{
	return pCamData;
}

Matrix4x4 Camera::Get_Projection_Matrix()
{
	//set projection matrix
	float width = (float)(WindowGlobals::Get()->Get_WindowWidth());
	float height = (float)(WindowGlobals::Get()->Get_WindowHeight());
	float aspectRatio = width / height;

	Projection_Matrix.setPerspectiveFovLH(fov, aspectRatio, zNear, zFar);

	return Projection_Matrix;
}

const int& Camera::Get_UID() const
{
	return uid;
}

void Camera::updateCamera()
{
	updatePositionSmooth();
	//updatePosition();
}

void Camera::updateCamera(CameraTranslationData* p_CamData)
{
	if (!p_CamData)
		throw NORMAL_EXCEPT("Camera::updateCamera, p_CamData is nullptr");

	memcpy(pCamData, p_CamData, sizeof(CameraTranslationData));
	updatePosition();
}

void Camera::updatePosition()
{
	/*if (!first_time_rotate_camera)
		delta_time = dt->GetDeltaTime();*/

	Matrix4x4 temp;
	Matrix4x4 current_world_matrix;
	temp.setIdentity();
	current_world_matrix.setIdentity();

	//temp.setScale(Vector3D(mp.delta_scale_x, mp.delta_scale_y, mp.delta_scale_z));
	//current_world_matrix *= temp;

	temp.setIdentity();
	temp.setRotationX(pCamData->delta_rotation_x);
	current_world_matrix *= temp;

	temp.setIdentity();
	temp.setRotationY(pCamData->delta_rotation_y);
	current_world_matrix *= temp;

	/*temp.setIdentity();
	temp.setRotationZ(mp.delta_rotation_z);
	current_world_matrix *= temp;*/

	Vector3D new_pos = m_world_matrix.getTranslation() + current_world_matrix.getZDirection() * (pCamData->delta_translation_z * move_speed);
	new_pos = new_pos + current_world_matrix.getXDirection() * (pCamData->delta_translation_x * move_speed);

	current_world_matrix.setTranslation(new_pos);
	m_world_matrix = current_world_matrix;
	m_world_pos_camera = new_pos;

	current_world_matrix.inverse();
	m_view_matrix = current_world_matrix;
}

void Camera::updatePositionSmooth()
{
	Vector3D currentPos = m_world_pos_camera;
	float delta_time = 1.0f / 60.0f;

	Matrix4x4 temp;
	Matrix4x4 current_world_matrix;
	temp.setIdentity();
	current_world_matrix.setIdentity();

	//------smooth rotation--------//
	current_rotation.m_x = pCamData->delta_rotation_x;
	current_rotation.m_y = pCamData->delta_rotation_y;
	//current_rotation.m_z = pCamData->delta_rotation_z;

	Vector3D smooth_rotation = Vector3D::lerp(old_rotation, current_rotation, delta_time * 7.0f);
	old_rotation = smooth_rotation;
	//----------------------------//

	temp.setIdentity();
	temp.setRotationX(smooth_rotation.m_x);
	current_world_matrix *= temp;

	temp.setIdentity();
	temp.setRotationY(smooth_rotation.m_y);
	current_world_matrix *= temp;

	Vector3D new_pos = m_world_pos_camera + current_world_matrix.getZDirection() * (pCamData->delta_translation_z * move_speed);
	new_pos = new_pos + current_world_matrix.getXDirection() * (pCamData->delta_translation_x * move_speed);

	current_world_matrix.setTranslation(new_pos);
	m_world_matrix = current_world_matrix;
	m_world_pos_camera = new_pos;

	current_world_matrix.inverse();
	m_view_matrix = current_world_matrix;
}


void Camera::RotateCamera(const Point& mouse_pos)
{
	Point pt_client = mouse_pos;
	ScreenToClient(Window::get()->getHwnd(), (LPPOINT)&pt_client);

	float mouse_x_client_center = WindowGlobals::Get()->Get_WindowWidth() / 2.0f;
	float mouse_y_client_center = WindowGlobals::Get()->Get_WindowHeight() / 2.0f;

	float temp_delta = 1.0f / 60.0f;

	pCamData->delta_rotation_x += (pt_client.m_y - mouse_y_client_center) * temp_delta/*delta_time*/ * 0.1f;
	pCamData->delta_rotation_y += (pt_client.m_x - mouse_x_client_center) * temp_delta/*delta_time*/ * 0.1f;

	if (pCamData->delta_rotation_x >= 1.57f)
		pCamData->delta_rotation_x = 1.57f;
	else if (pCamData->delta_rotation_x <= -1.57f)
		pCamData->delta_rotation_x = -1.57f;



	//fix cursor in the middle of client window
	int windowWidth = WindowGlobals::Get()->Get_WindowWidth();
	int windowHeight = WindowGlobals::Get()->Get_WindowHeight();
	Point pt = { windowWidth / 2, windowHeight / 2 };
	ClientToScreen(Window::get()->getHwnd(), (LPPOINT)&pt);
	InputSystem::get()->setCursorPosition(Point(pt.m_x, pt.m_y));
}

void Camera::setTranslation(Vector3D new_pos)
{
	m_world_pos_camera = new_pos;
	m_world_matrix.setTranslation(new_pos);
}


void Camera::onKeyDown(int key)
{
	if (!getInputControl || !isProjecting) return;

	if (key == 'W')
	{
		pCamData->delta_translation_z = 1.0;
	}

	if (key == 'S')
	{
		pCamData->delta_translation_z = -1.0;
	}

	if (key == 'A')
	{
		pCamData->delta_translation_x = -1.0;
	}

	if (key == 'D')
	{
		pCamData->delta_translation_x = 1.0;
	}

	if (key == VK_SHIFT)
	{
		turboMode = true;
	}
}

void Camera::onKeyUp(int key)
{
	if (!getInputControl || !isProjecting) return;

	pCamData->delta_translation_z = 0.0;
	pCamData->delta_translation_x = 0.0;

	if (key == VK_ESCAPE)
	{
		play_state = !play_state;
		if (play_state == false)
			InputSystem::get()->showCursor(true);
		else
			InputSystem::get()->showCursor(false);
	}

	if (key == VK_SHIFT)
	{
		turboMode = false;
	}
}

void Camera::onMouseMove(const Point& mouse_pos)
{
	if (!getInputControl || !isProjecting) return;

	if (!play_state)
		return;

	if (first_time_rotate_camera)
	{
		first_time_rotate_camera = false;
		//return;
	}


	RotateCamera(mouse_pos);
}

void Camera::onLeftMouseDown(const Point& mouse_pos)
{
	if (!getInputControl || !isProjecting) return;
}

void Camera::onLeftMouseUp(const Point& mouse_pos)
{
	if (!getInputControl || !isProjecting) return;
}

void Camera::onRightMouseDown(const Point& mouse_pos)
{
	if (!getInputControl || !isProjecting) return;
}

void Camera::onRightMouseUp(const Point& mouse_pos)
{
	if (!getInputControl || !isProjecting) return;
}

void Camera::onFocus()
{
	InputSystem::get()->addListener(this);

}

void Camera::onKillFocus()
{
	InputSystem::get()->removeListener(this);

}

void Camera::onSize()
{
}

void Camera::CreateTPC(Vector3D player_pos, float cam_distance)
{
	isTPC = true;

	this->player_position_world = player_pos;
	this->cam_distance_player = cam_distance;
	this->fix_cam_distance = cam_distance;
	this->current_cam_distance = cam_distance;

	Matrix4x4 temp;
	Matrix4x4 current_world_matrix;
	temp.setIdentity();
	current_world_matrix.setIdentity();

	temp.setIdentity();
	temp.setRotationX(pCamData->delta_rotation_x);
	current_world_matrix *= temp;

	temp.setIdentity();
	temp.setRotationY(pCamData->delta_rotation_y);
	current_world_matrix *= temp;

	m_world_pos_camera = player_position_world;

	Vector3D new_pos = m_world_pos_camera + current_world_matrix.getZDirection() * (-cam_distance_player);
	//new_pos = new_pos + current_world_matrix.getXDirection() * (pCamData->delta_translation_x * move_speed);

	current_world_matrix.setTranslation(new_pos);
	m_world_matrix = current_world_matrix;
	m_world_pos_camera = new_pos;

	current_world_matrix.inverse();
	m_view_matrix = current_world_matrix;
}

void Camera::updateTPC(Vector3D player_pos, float delta_time_lerp)
{
	/*if (!first_time_rotate_camera)
		delta_time = dt->GetDeltaTime();*/

	delta_time = 1.0f / 60.0f;

	this->player_position_world = player_pos;

	Matrix4x4 temp;
	Matrix4x4 current_world_matrix;
	temp.setIdentity();
	current_world_matrix.setIdentity();

	//------smooth rotation--------//
	current_rotation.m_x = pCamData->delta_rotation_x;
	current_rotation.m_y = pCamData->delta_rotation_y;
	current_rotation.m_z = pCamData->delta_rotation_z;

	Vector3D smooth_rotation = Vector3D::lerp(old_rotation, current_rotation, delta_time_lerp * 4.0f);
	old_rotation = smooth_rotation;
	//-----------------------------//

	temp.setIdentity();
	temp.setRotationX(smooth_rotation.m_x);
	current_world_matrix *= temp;

	temp.setIdentity();
	temp.setRotationY(smooth_rotation.m_y);
	current_world_matrix *= temp;

	if (pCamData->delta_translation_z)
	{
		if (pCamData->delta_translation_z > 0)
		{
			if (turboMode)
			{
				cam_distance_player = fix_cam_distance + 15;
				MovingForward = true;
				MovingturboMode = true;
				MovingBackward = false;
			}
			else
			{
				cam_distance_player = fix_cam_distance + 10;
				MovingForward = true;
				MovingturboMode = false;
				MovingBackward = false;
			}

		}
		else if (pCamData->delta_translation_z < 0)
		{
			cam_distance_player = fix_cam_distance - 4;
			MovingForward = false;
			MovingturboMode = false;
			MovingBackward = true;
		}
	}
	else
	{
		cam_distance_player = fix_cam_distance;
		MovingForward = false;
		MovingturboMode = false;
		MovingBackward = false;
	}

	current_cam_distance = lerp(current_cam_distance, cam_distance_player, delta_time);

	Vector3D new_pos = player_position_world + current_world_matrix.getZDirection() * (-current_cam_distance);
	new_pos = new_pos + current_world_matrix.getYDirection() * (4.0);

	//----smooth movement---//
	/*m_world_pos_new = Vector3D::lerp(m_world_pos_camera, new_pos, delta_time_lerp * 18.0f);
	current_world_matrix.setTranslation(m_world_pos_new);
	m_world_matrix = current_world_matrix;
	m_world_pos_camera = m_world_pos_new;*/
	//----------------------//

	current_world_matrix.setTranslation(new_pos);
	m_world_matrix = current_world_matrix;
	m_world_pos_camera = new_pos;

	current_world_matrix.inverse();
	m_view_matrix = current_world_matrix;
}

bool Camera::isTPP()
{
	return isTPC;
}

void Camera::set_cam_distance(float distance)
{
	fix_cam_distance = distance;
}

bool Camera::isturboMode()
{
	return turboMode;
}

bool Camera::isMovingForward()
{
	return MovingForward;
}

bool Camera::isMovingBackward()
{
	return MovingBackward;
}

bool Camera::isMovingturboMode()
{
	return MovingturboMode;
}
