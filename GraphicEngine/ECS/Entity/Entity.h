#pragma once
#include<GraphicEngine/ECS/ECSHeaders/EntityStructs.h>
//#include<GraphicEngine/Utilities/Math/Matrix4x4.h>
//#include<GraphicEngine/Utilities/Math/Vector3D.h>
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
class Light;
struct constant;

class ATT_Ent Entity
{
	friend class EntityManager;
protected:
	Entity(Primitive* p_Primitive, EntityDesc* ent_desc);
	virtual ~Entity();

public:
	virtual Vector3D Get_Entity_World_Pos() const;
	virtual Matrix4x4 Get_Entity_WorldMatrix() const;
	
public:
	Primitive* GetPrimitive();
	const std::wstring& Get_Entity_Name() const;
	const bool& Get_LocalPlayer() const;
	const int& GetScene_ID() const;
	const int& Get_Entity_uID() const;
	const bool& Get_IsRenderable() const;

public:
	void setConstantBuffer(void* c_buffer);
	void setConstantBuffer(constant* c_buffer);

protected:
	Scene* pParentScene = nullptr;
	Primitive* pPrimitive = nullptr;
	ModelData* pModelData = nullptr;
	Camera* pCamera = nullptr;
	Light* pLight = nullptr;

protected:
	int scene_id = -1;
	bool bLocalPlayer = false;
	bool Is_Renderable = true;
	ENTITY_TYPE Entity_Type = ENTITY_TYPE::NORMAL_ENTITY;

protected:
	int Entity_uid = -1;
	std::wstring Entity_name;
};