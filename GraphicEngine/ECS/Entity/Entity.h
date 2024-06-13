#pragma once
#include<GraphicEngine/Utilities/Math/Matrix4x4.h>
#include<GraphicEngine/Utilities/Math/Vector3D.h>
#include<string>

#ifdef PROJ_EXPORT
#define ATT_Ent __declspec(dllexport)
#else
#define ATT_Ent __declspec(dllimport)
#endif

class Primitive;
class ModelData;
struct EntityDesc;
struct ModelPositionData;
class Camera;
struct constant;

class ATT_Ent Entity
{
	friend class EntityManager;
protected:
	Entity(Primitive* p_Primitive, EntityDesc* ent_desc);
	virtual ~Entity();

public:
	Vector3D Get_Entity_World_Pos() const;
	Matrix4x4 Get_Entity_WorldMatrix() const;
	const std::wstring& Get_Entity_Name() const;
	const bool& Get_LocalPlayer() const;
	const int& GetScene_ID() const;
	Primitive* GetPrimitive();
	const int& Get_Entity_uID() const;

public:
	virtual void UpdatePosition(ModelPositionData* mp, Camera* cp);

public:
	void setConstantBuffer(void* c_buffer);
	void setConstantBuffer(constant* c_buffer);
	

protected:
	Primitive* pPrimitive = nullptr;
	ModelData* pModelData = nullptr;
protected:
	int scene_id = -1;
	bool bLocalPlayer = false;

public:
	//-----------------child class respective virual functions----------------------------//
	//NormalEntity
	virtual void Set_Rotaion(float radians_x, float radians_y, float radians_z) {};
	virtual Vector3D Get_Rotation() { return Vector3D(); };
};