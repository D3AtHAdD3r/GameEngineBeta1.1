#pragma once
#include<GraphicEngine/ECS/ECSHeaders/EntityStructs.h>
#include<GraphicEngine/InputHandling/InputListener.h>
#include<GraphicEngine/Window/WindowListener.h>

#ifdef PROJ_EXPORT
#define ATT_Camera __declspec(dllexport)
#else
#define ATT_Camera __declspec(dllimport)
#endif


class InputListener;
class WindowListener;
class Entity;
class ModelData;

class ATT_Camera Camera : public InputListener, public WindowListener
{
	friend class Entity;
	friend class Scene;
private:
	Camera(CameraInitData* camInitData);
	~Camera();

public:
	//Getters
	const Vector3D& Get_Current_World_Pos() const;
	const Vector3D& Get_Previous_World_pos() const;
	const Vector3D& Get_Current_Rotation() const;
	const Vector3D& Get_Previous_Rotation() const;
	const Matrix4x4& Get_ViewMatrix() const;
	const Matrix4x4& Get_ProjectionMatrix() const;
	const Matrix4x4& Get_World_Matirx() const;
	const int& Get_uID() const;

	//Setters
	void Set_isProjecting(bool flag);
	void Set_GetInput(bool flag);
	void Set_Camera_Internals(float fov, float aspect_ratio, float zNear, float zFar);
	void Set_Smooth_Movements(bool smooth_rotaion, bool smooth_translation);
public:
	bool Update();
	bool Update(CameraPositionData* cam_update);

public:
	bool Update_Translation_Direct(const Vector3D& newVal);
	bool Update_Rotation_Direct(const Vector3D& newVal); //not working properly, needs fixing

private:
	bool Update_default_Internal();
	bool Update_default_Smooth_Internal();
	bool Update_fpc_Internal();
	//bool Update_tpc_Internal();
	//bool Update_tpc_Smooth_Internal();
private:
	void Set_DataMembers_On_Init(CameraInitData* camInitData);
	bool Set_CamData_From_Parent_Entity();
	bool Set_Camera_Rotations_From_Input(const Point& mouse_pos);

private:
	bool Attach(Entity* ent, CameraAttachDetails* CamDetails);
	bool Detach();
	void ActivateCamera();
	void DeActivateCamera();


private:
	bool GetInput = false;
	bool isProjecting = false;
	bool isAttached = false;
private:
	CameraType camType = CameraType::freeCam;
private:
	Entity* parentEntity = nullptr;
	ModelData* parentEntity_ModelData = nullptr;
	CameraPositionData CamData;
	CameraAttachDetails cam_attach_details;
private:
	Vector3D Current_Translation;
	Vector3D Previous_Translation;
	Vector3D Current_Rotation;
	Vector3D Previous_Rotation;
	Matrix4x4 World_Matrix;
	Matrix4x4 ViewMatrix;
	Matrix4x4 ProjectionMatrix;
private:
	bool SmoothRotaion = false;
	bool SmoothTranslation = false;
	float move_speed = 0.55f;
	float current_move_speed = 0;
private:
	float fov = 0;
	float aspect_ratio = 0;
	float zNear = 0;
	float zFar = 0;
private:
	int uID = -1;

private:
	bool play_state = true; //if cam will take mouse input or not


public:
	//Inherited functions via InputHandler
	virtual void onKeyDown(int key) override;
	virtual void onKeyUp(int key) override;

	virtual void onMouseMove(const Point& mouse_pos) override;
	virtual void onLeftMouseDown(const Point& mouse_pos) override;
	virtual void onLeftMouseUp(const Point& mouse_pos) override;
	virtual void onRightMouseDown(const Point& mouse_pos) override;
	virtual void onRightMouseUp(const Point& mouse_pos) override;

	//Inherited via WindowsListener
	virtual void onFocus() override;
	virtual void onKillFocus() override;
	virtual void onSize() override;
};