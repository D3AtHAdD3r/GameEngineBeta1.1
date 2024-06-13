#pragma once
#include<GraphicEngine/Utilities/Math/Matrix4x4.h>
#include<GraphicEngine/Utilities/Math/Vector3D.h>
#include<GraphicEngine/InputHandling/InputListener.h>
#include<GraphicEngine/Window/WindowListener.h>

#ifdef PROJ_EXPORT
#define ATT_Camera __declspec(dllexport)
#else
#define ATT_Camera __declspec(dllimport)
#endif


class InputListener;
class WindowListener;
struct CameraTranslationData;

class ATT_Camera Camera : public InputListener, public WindowListener
{
	friend class Scene;
private:
	Camera(bool get_Input, bool isTPC);
	~Camera();

public:
	 Matrix4x4 getWorldMatrix();
	 Matrix4x4 getViewMatrix();
	 Vector3D getCamWorldPos();
	 CameraTranslationData* getCamTranslationData();
	 Matrix4x4 Get_Projection_Matrix();

public:
	void CreateTPC(Vector3D player_pos, float cam_distance = 30.0f);
	void updateTPC(Vector3D player_pos, float delta_time_lerp);
	bool isTPP();
	void set_cam_distance(float distance);

public:
	void updateCamera();
	void setTranslation(Vector3D new_pos);

private:
	void updatePosition();
	void updatePositionSmooth();
	void RotateCamera(const Point& mouse_pos);

private:
	Vector3D m_world_pos_camera{ 0.0f, 0.0f, 0.0f };
	Vector3D m_world_pos_new;
	Matrix4x4 m_world_matrix;
	Matrix4x4 m_view_matrix;
	Matrix4x4 Projection_Matrix;
	
private:
	float delta_rotate_seed_camera = 0.85f;
	bool first_time_rotate_camera = true;
	float move_speed = 0.25f;
	float delta_time = 0;
private:
	CameraTranslationData* pCamData = nullptr;

public:
	// Inherited via InputListener
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

private:
	//ThirdPersonCamera
	Vector3D player_position_world = { 0,0,0 };
	float cam_distance_player = 0;
	float current_cam_distance = 0;
	float fix_cam_distance = 0;
	bool isTPC = false;

private:
	Vector3D old_rotation;
	Vector3D current_rotation;

private:
	bool play_state = true;
	bool getInputControl = false;

public:
	bool isturboMode();
	bool isMovingForward();
	bool isMovingBackward();
	bool isMovingturboMode();
private:
	//localplayer attributes
	bool turboMode = false;
	bool MovingForward = false;
	bool MovingBackward = false;
	bool MovingturboMode = false;

public:
	float fov = 0.785398f;
	float zNear = 0.1f;
	float zFar = 5000.0f;
};