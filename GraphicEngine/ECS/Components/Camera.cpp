#include "Camera.h"
#include<GraphicEngine/InputHandling/InputSystem.h>
#include<GraphicEngine/Utilities/Math/MathUtils.h>
#include<GraphicEngine/Utilities/ErrorChecking/CustomException.h>
#include<GraphicEngine/ECS/Components/ModelData.h>
#include<GraphicEngine/ECS/Entity/Entity.h>
#include<GraphicEngine\Renderer\RendererDX11\RendererDX11.h>


Camera::Camera(CameraInitData* camInitData)
{
	if (!camInitData)
		throw NORMAL_EXCEPT("Camera constructor failed. Invalid Input");

	if (camInitData->fov <= 0 || camInitData->aspect_ratio <= 0 || camInitData->zNear <= 0 || camInitData->zFar <= 0 || camInitData->uID < 0)
		throw NORMAL_EXCEPT("Camera constructor failed. Invalid Input");

	Set_DataMembers_On_Init(camInitData);

	InputSystem::get()->addListener(this);
}

Camera::~Camera()
{
}


const Vector3D& Camera::Get_Current_World_Pos() const
{
	return Current_Translation;
}

const Vector3D& Camera::Get_Previous_World_pos() const
{
	return Previous_Translation;
}

const Vector3D& Camera::Get_Current_Rotation() const
{
	return Current_Rotation;
}

const Vector3D& Camera::Get_Previous_Rotation() const
{
	return Previous_Rotation;
}

const Matrix4x4& Camera::Get_ViewMatrix() const
{
	return ViewMatrix;
}

const Matrix4x4& Camera::Get_ProjectionMatrix() const
{
	return ProjectionMatrix;
}

const Matrix4x4& Camera::Get_World_Matirx() const
{
	return World_Matrix;
}

const int& Camera::Get_uID() const
{
	return uID;
}



void Camera::Set_DataMembers_On_Init(CameraInitData* camInitData)
{
	uID = camInitData->uID;
	fov = camInitData->fov;
	aspect_ratio = camInitData->aspect_ratio;
	zNear = camInitData->zNear;
	zFar = camInitData->zFar;
	Current_Translation = camInitData->world_pos;
	GetInput = camInitData->GetInput;
	isProjecting = camInitData->isProjecting;
	SmoothRotaion = camInitData->SmoothRotation;
	SmoothTranslation = camInitData->SmoothTranslation;
	move_speed = camInitData->move_speed;

	World_Matrix.setIdentity();
	World_Matrix.setTranslation(Current_Translation);

	Matrix4x4 temp = World_Matrix;
	temp.inverse();
	ViewMatrix = temp;

	ProjectionMatrix.setPerspectiveFovLH(fov, aspect_ratio, zNear, zFar);
}

bool Camera::Set_CamData_From_Parent_Entity()
{
	if (!parentEntity_ModelData) return false;

	CamData.delta_rotation_x = parentEntity_ModelData->mp.delta_rotation_x;
	CamData.delta_rotation_y = parentEntity_ModelData->mp.delta_rotation_y;
	CamData.delta_rotation_z = parentEntity_ModelData->mp.delta_rotation_z;

	Vector3D translate;
	translate.m_x = parentEntity_ModelData->Translation.m_x + cam_attach_details.delta_offset_model_x;
	translate.m_y = parentEntity_ModelData->Translation.m_y + cam_attach_details.delta_offset_model_y;
	translate.m_z = parentEntity_ModelData->Translation.m_z + cam_attach_details.delta_offset_model_z;

	if (!Update_Translation_Direct(translate)) return false;

	return true;
}

bool Camera::Set_Camera_Rotations_From_Input(const Point& mouse_pos)
{
	Point pt_client = mouse_pos;
	ScreenToClient(RendererDX11::Get()->Get_WindowHandle(), (LPPOINT)&pt_client);

	float mouse_x_client_center = RendererDX11::Get()->Get_WindowWidth() / 2.0f;
	float mouse_y_client_center = RendererDX11::Get()->Get_WindowHeight() / 2.0f;

	float temp_delta = 1.0f / 60.0f;

	CamData.delta_rotation_x += (pt_client.m_y - mouse_y_client_center) * temp_delta/*delta_time*/ * 0.1f;
	CamData.delta_rotation_y += (pt_client.m_x - mouse_x_client_center) * temp_delta/*delta_time*/ * 0.1f;

	if (CamData.delta_rotation_x >= 1.57f)
		CamData.delta_rotation_x = 1.57f;
	else if (CamData.delta_rotation_x <= -1.57f)
		CamData.delta_rotation_x = -1.57f;

	return true;
}

bool Camera::Attach(Entity* ent, CameraAttachDetails* CamDetails)
{
	if (!ent || !CamDetails) return false;

	if (CamDetails->camType == CameraType::freeCam) return false;

	parentEntity = ent;
	parentEntity_ModelData = ent->Get_ModelData();
	isAttached = true;
	camType = CamDetails->camType;

	cam_attach_details = *CamDetails;

	//if (camType == CameraType::fpc)
	//{
	//	Vector3D newPos;
	//	Vector3D EntPos = ent->Get_ModelData()->Translation;
	//	Vector3D EntRot = ent->Get_ModelData()->Rotation;

	//	//take care of scaling factor later on 
	//	newPos.m_x = EntPos.m_x + cam_attach_details.delta_offset_model_x; // * scaling_x
	//	newPos.m_y = EntPos.m_y + cam_attach_details.delta_offset_model_y;
	//	newPos.m_z = EntPos.m_z + cam_attach_details.delta_offset_model_z;

	//	CamData.delta_rotation_x = EntRot.m_x;
	//	CamData.delta_rotation_y = EntRot.m_y;
	//	CamData.delta_rotation_z = EntRot.m_z;

	//	if (!Update_Rotation_Direct(EntRot)) return false;
	//	if (!Update_Translation_Direct(newPos)) return false;
	//}


	return true;
}

bool Camera::Detach()
{
	if (camType == CameraType::freeCam || !parentEntity || !parentEntity_ModelData || !isAttached) return false;

	parentEntity = nullptr;
	parentEntity_ModelData = nullptr;
	isAttached = false;
	camType = CameraType::freeCam;
	cam_attach_details = CameraAttachDetails();

	return true;
}

void Camera::ActivateCamera()
{
	GetInput = true;
	isProjecting = true;
}

void Camera::DeActivateCamera()
{
	GetInput = false;
	isProjecting = false;
}

bool Camera::Update()
{
	if (isAttached)
	{
		if (!Set_CamData_From_Parent_Entity()) return false;

		if (camType == CameraType::fpc)
		{
			if (!Update_fpc_Internal()) return false;
		}
		else if (camType == CameraType::tpc)
		{
			//....
		}
	}
	else
	{
		if (SmoothRotaion || SmoothTranslation)
		{
			return Update_default_Smooth_Internal();
		}
		else
		{
			return Update_default_Internal();
		}
	}


	return true;
}

bool Camera::Update(CameraPositionData* cam_update_data)
{
	if (!cam_update_data) return false;
	CamData = *cam_update_data;

	if (SmoothRotaion || SmoothTranslation)
	{
		return Update_default_Smooth_Internal();
	}
	else
	{
		return Update_default_Internal();
	}
}

void Camera::Update_AspectRatio(unsigned int width, unsigned int height)
{
	aspect_ratio = (float)width / (float)height;
}

bool Camera::Update_Translation_Direct(const Vector3D& newVal)
{
	Previous_Translation = Current_Translation;
	Current_Translation = newVal;
	World_Matrix.setTranslation(Current_Translation);

	Matrix4x4 temp;
	temp.setIdentity();
	temp = World_Matrix;
	temp.inverse();
	ViewMatrix = temp;
	return true;
}

bool Camera::Update_Rotation_Direct(const Vector3D& newVal)
{
	Matrix4x4 temp;
	Matrix4x4 world_matrix;
	temp.setIdentity();
	world_matrix.setIdentity();


	temp.setRotationX(newVal.m_x);
	world_matrix *= temp;
	

	temp.setIdentity();
	temp.setRotationY(newVal.m_y);
	world_matrix *= temp;
	

	temp.setIdentity();
	temp.setRotationZ(newVal.m_z);
	world_matrix *= temp;
	

	world_matrix.setTranslation(World_Matrix.getTranslation());
	World_Matrix = world_matrix;
	ProjectionMatrix.setPerspectiveFovLH(fov, aspect_ratio, zNear, zFar);

	
	world_matrix.inverse();
	ViewMatrix = temp;

	CamData.delta_rotation_x = newVal.m_x;
	CamData.delta_rotation_y = newVal.m_y;
	CamData.delta_rotation_z = newVal.m_z;

	return true;
}


bool Camera::Update_default_Internal()
{
	Matrix4x4 temp;
	Matrix4x4 current_world_matrix;
	temp.setIdentity();
	current_world_matrix.setIdentity();

	temp.setIdentity();
	temp.setRotationX(CamData.delta_rotation_x);
	current_world_matrix *= temp;

	temp.setIdentity();
	temp.setRotationY(CamData.delta_rotation_y);
	current_world_matrix *= temp;

	/*temp.setIdentity();
	temp.setRotationZ(CamData.delta_rotation_z);
	current_world_matrix *= temp;*/

	if (turbo)
	{
		current_move_speed = move_speed * 2;
	}
	else
	{
		current_move_speed = move_speed;
	}

	//movement in relation to current camera's x,y,z direction
	Vector3D new_pos = World_Matrix.getTranslation() + current_world_matrix.getZDirection() * (CamData.delta_translation_z * current_move_speed);
	new_pos = new_pos + current_world_matrix.getXDirection() * (CamData.delta_translation_x * current_move_speed);
	new_pos = new_pos + current_world_matrix.getYDirection() * (CamData.delta_translation_y * current_move_speed);

	current_world_matrix.setTranslation(new_pos);
	World_Matrix = current_world_matrix;

	current_world_matrix.inverse();
	ViewMatrix = current_world_matrix;

	ProjectionMatrix.setPerspectiveFovLH(fov, aspect_ratio, zNear, zFar);

	Previous_Rotation = Current_Rotation;
	Previous_Translation = Current_Translation;

	Current_Rotation = { CamData.delta_rotation_x, CamData.delta_rotation_y, CamData.delta_rotation_z };
	Current_Translation = new_pos;

	CamData.delta_translation_x = CamData.delta_translation_y = CamData.delta_translation_z = 0;
	return true;
}

bool Camera::Update_default_Smooth_Internal()
{
	Matrix4x4 temp;
	Matrix4x4 current_world_matrix;
	temp.setIdentity();
	current_world_matrix.setIdentity();

	Vector3D smooth_rotation;

	if (SmoothRotaion)
	{
		Current_Rotation = { CamData.delta_rotation_x, CamData.delta_rotation_y, CamData.delta_rotation_z };
		smooth_rotation = Vector3D::lerp(Previous_Rotation, Current_Rotation, CamData.delta_time * CamData.smooth_rotation_Variable);
		Previous_Rotation = smooth_rotation;
	}
	else
	{
		Previous_Rotation = Current_Rotation;
		smooth_rotation = Current_Rotation = { CamData.delta_rotation_x, CamData.delta_rotation_y , CamData.delta_rotation_z };
	}

	temp.setIdentity();
	temp.setRotationX(smooth_rotation.m_x);
	current_world_matrix *= temp;

	temp.setIdentity();
	temp.setRotationY(smooth_rotation.m_y);
	current_world_matrix *= temp;

	temp.setIdentity();
	temp.setRotationZ(smooth_rotation.m_z);
	current_world_matrix *= temp;

	if (turbo)
	{
		//current_move_speed = move_speed * 2;
		current_move_speed += 0.2f;
	}
	else
	{
		current_move_speed = move_speed;
	}

	//movement in relation to current entity's x,y,z direction
	Vector3D new_pos = World_Matrix.getTranslation() + current_world_matrix.getZDirection() * (CamData.delta_translation_z * current_move_speed);
	new_pos = new_pos + current_world_matrix.getXDirection() * (CamData.delta_translation_x * current_move_speed);
	new_pos = new_pos + current_world_matrix.getYDirection() * (CamData.delta_translation_y * current_move_speed);

	if (SmoothTranslation)
	{
		Vector3D world_pos_new;
		world_pos_new = Vector3D::lerp(Current_Translation, new_pos, CamData.delta_time * CamData.smooth_translation_variable);
		current_world_matrix.setTranslation(world_pos_new);
		World_Matrix = current_world_matrix;
		Previous_Translation = Current_Translation;
		Current_Translation = world_pos_new;
	}
	else
	{
		current_world_matrix.setTranslation(new_pos);
		World_Matrix = current_world_matrix;
		Previous_Translation = Current_Translation;
		Current_Translation = new_pos;
	}

	current_world_matrix.inverse();
	ViewMatrix = current_world_matrix;
	ProjectionMatrix.setPerspectiveFovLH(fov, aspect_ratio, zNear, zFar);

	CamData.delta_translation_x = CamData.delta_translation_y = CamData.delta_translation_z = 0;
	return true;
}

bool Camera::Update_fpc_Internal()
{
	Vector3D CurrentParentPos = parentEntity->Get_ModelData()->Translation;
	Vector3D CurrentParentRotation = parentEntity->Get_ModelData()->Rotation;
	Vector3D CurrentRotation = CurrentParentRotation /*+ pModelData->Get_Initial_Rotation()*/;

	float offsetX = cam_attach_details.delta_offset_model_x;
	float offsetY = cam_attach_details.delta_offset_model_y;
	float offsetZ = cam_attach_details.delta_offset_model_z;

	//Get Translation, relative to parent entity, update it
	Vector3D new_pos = CurrentParentPos + parentEntity->Get_ModelData()->Get_World_Matirx().getZDirection() * (offsetZ);
	new_pos = new_pos + parentEntity->Get_ModelData()->Get_World_Matirx().getXDirection() * (offsetX);
	new_pos = new_pos + parentEntity->Get_ModelData()->Get_World_Matirx().getYDirection() * (offsetY);


	Matrix4x4 temp;
	Matrix4x4 current_world_matrix;
	temp.setIdentity();
	current_world_matrix.setIdentity();

	temp.setIdentity();
	temp.setRotationX(CurrentRotation.m_x);
	current_world_matrix *= temp;

	temp.setIdentity();
	temp.setRotationY(CurrentRotation.m_y);
	current_world_matrix *= temp;

	temp.setIdentity();
	temp.setRotationZ(CurrentRotation.m_z);
	current_world_matrix *= temp;

	current_world_matrix.setTranslation(new_pos);
	World_Matrix = current_world_matrix;

	current_world_matrix.inverse();
	ViewMatrix = current_world_matrix;

	ProjectionMatrix.setPerspectiveFovLH(fov, aspect_ratio, zNear, zFar);

	Current_Rotation = CurrentRotation;
	Current_Translation = new_pos;

	CamData.delta_translation_x = CamData.delta_translation_y = CamData.delta_translation_z = 0;
	CamData.delta_rotation_x = CurrentRotation.m_x;
	CamData.delta_rotation_y = CurrentRotation.m_y;
	CamData.delta_rotation_z = CurrentRotation.m_z;
	return true;
}

void Camera::Set_isProjecting(bool flag)
{
	isProjecting = flag;
}

void Camera::Set_GetInput(bool flag)
{
	GetInput = flag;
}

void Camera::Set_Camera_Internals(float fov, float aspect_ratio, float zNear, float zFar)
{
	this->fov = fov;
	this->aspect_ratio = aspect_ratio;
	this->zNear = zNear;
	this->zFar = zFar;
}

void Camera::Set_Smooth_Movements(bool smooth_rotaion, bool smooth_translation)
{
	this->SmoothRotaion = smooth_rotaion;
	this->SmoothTranslation = smooth_translation;
}

void Camera::onKeyDown(int key)
{
	if (!GetInput) return;

	if (!isAttached)
	{
		if (key == 'W')
		{
			CamData.delta_translation_z = 1.0;
		}

		if (key == 'S')
		{
			CamData.delta_translation_z = -1.0;
		}

		if (key == 'A')
		{
			CamData.delta_translation_x = -1.0;
		}

		if (key == 'D')
		{
			CamData.delta_translation_x = 1.0;
		}

		if (key == VK_SHIFT)
		{
			turbo = true;
		}
	}

}

void Camera::onKeyUp(int key)
{
	if (key == VK_ESCAPE)
		play_state = !play_state;

	if (!GetInput) return;

	if (!isAttached)
	{
		CamData.delta_translation_x = 0;
		CamData.delta_translation_y = 0;
		CamData.delta_translation_z = 0;
	}
	if (key == VK_SHIFT)
	{
		turbo = false;
	}

}

void Camera::onMouseMove(const Point& mouse_pos)
{
	if (!GetInput) return;
	if (!play_state) return;

	Set_Camera_Rotations_From_Input(mouse_pos);
}

void Camera::onLeftMouseDown(const Point& mouse_pos)
{
	if (!GetInput) return;
}

void Camera::onLeftMouseUp(const Point& mouse_pos)
{
	if (!GetInput) return;
}

void Camera::onRightMouseDown(const Point& mouse_pos)
{
	if (!GetInput) return;
}

void Camera::onRightMouseUp(const Point& mouse_pos)
{
	if (!GetInput) return;
}

void Camera::onFocus()
{
}

void Camera::onKillFocus()
{
}

void Camera::onSize()
{
}









