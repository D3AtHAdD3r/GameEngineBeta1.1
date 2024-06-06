#pragma once
#include<GraphicEngine/Utilities/Math/Matrix4x4.h>
#include<GraphicEngine/Utilities/Math/Vector3D.h>
#include<string>

class Primitive;
class ModelData;
struct EntityDesc;
struct ModelPositionData;
class Camera;

class Entity
{
	friend class EntityManager;
public:
	Entity(Primitive* p_Primitive, EntityDesc* ent_desc);
	virtual ~Entity();

public:
	Vector3D Get_Entity_World_Pos() const;
	Matrix4x4 Get_Entity_WorldMatrix() const;
	const std::wstring& Get_Entity_Name() const;
	const bool& Get_LocalPlayer() const;
	const int& GetScene_ID() const;

public:
	virtual void UpdatePosition(ModelPositionData* mp, Camera* cp);

protected:
	void setConstantBuffer(void* c_buffer);
	Primitive* GetPrimitive();

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