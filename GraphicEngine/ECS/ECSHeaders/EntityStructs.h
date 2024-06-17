#pragma once
#include<GraphicEngine/Utilities/Math/Matrix4x4.h>
#include<GraphicEngine/Utilities/Math/Vector4D.h>
#include<GraphicEngine/Utilities/Math/Vector3D.h>
#include<string>
#include<vector>

class VertexMesh;
struct MaterialSlot;
struct D3D11_INPUT_ELEMENT_DESC;
class Camera;
class Light;
class Scene;

namespace HARDCODINGS
{
	static std::wstring RTV_NAME_START = L"RTV";
	static std::string World_config_loc = "..\\GameDataConfigs\\WorldData";
	static std::string world_name = "world_spaceship";
}

enum ENTITY_TYPE
{
	ENUM_NORMAL_ENTITY = 0,
	ENUM_LOCALPLAYER,
	ENUM_UNKNOWN
};

enum Primitive_texture_type
{
	oneTexMap_OneNormalMap_perDrawCall = 0,		//1 tex-Map, 1NormalMap- binded per draw call (normal maps required)
	oneTexMap_perDrawCall,						//1 tex-map binded per draw call
	allTexMaps_perDrawCall,						//all tex-maps binded per draw call
	unknown
};

struct ModelPositionData
{
public:
	float delta_rotation_x = 0;
	float delta_rotation_y = 0;
	float delta_rotation_z = 0;
public:
	float delta_translation_x = 0;
	float delta_translation_y = 0;
	float delta_translation_z = 0;
public:
	float delta_scale_x = 1;
	float delta_scale_y = 1;
	float delta_scale_z = 1;
public:

public:
	float delta_time = 1.0f / 60.0f;

public:
	bool SmoothRotation = false;
	bool SmoothMovement = false;
	float move_speed = 0.55f;
	float lerp_Variable = 6.0f;
	float smooth_translation_variable = 25.0f; //lol 
};

struct CameraTranslationData
{
public:
	float delta_rotation_x = 0;
	float delta_rotation_y = 0;
	float delta_rotation_z = 0;
public:
	float delta_translation_x = 0;
	float delta_translation_y = 0;
	float delta_translation_z = 0;
public:
	float delta_scale_x = 1;
	float delta_scale_y = 1;
	float delta_scale_z = 1;
public:
	float delta_time = 0;
};


struct MeshCreationData
{
	VertexMesh* vertex_list_data = nullptr;
	unsigned int vertex_list_size = 0;
	unsigned int* index_list_data = nullptr;
	unsigned int index_list_size = 0;
	MaterialSlot* material_slot_list = nullptr;
	unsigned int material_slot_list_size = 0;
	std::wstring MeshName;
};

struct CameraInitData
{
	bool get_Input = false;
	bool isTPC = false; 
	int u_id = -1;
	bool isprojecting = false;
};

struct EntityDesc
{
public:
	int mesh_uid = -1;
	bool getMeshfromFile = true;
	
	bool isNormalMap = false;
	std::vector<int> texture_uids;
	std::vector<int> texture_normals_uids;
	unsigned int numOfTextures = 0;
	unsigned int numOfTextureNormals = 0;

	Primitive_texture_type primitive_texture_type = Primitive_texture_type::unknown;
	ENTITY_TYPE Entity_type = ENTITY_TYPE::ENUM_UNKNOWN;

	Vector3D model_initialPosition = { 0,0,0 }; //world space

public:
	int Camera_uid = -1;
	int Light_uid = -1;
	bool Is_Renderable = true;

public:
	//vertex shader requirements
	int vertex_Shader_uid = -1;
	D3D11_INPUT_ELEMENT_DESC* inLayout;
	unsigned int sizeLayout = 0;

	//pixel shader requirements
	int pixel_Shader_uid = -1;
	
	//Constant Buffer requirements
	void* constant_buffer = nullptr;
	unsigned int size_constant_buffer = 0; //***
	int constant_buffer_uid = -1;

	bool frontFaceCull = false;
public:
	std::wstring primitive_name;
	int primitive_uid = -1;
	short Scene_Id = 0;

public:
	ModelPositionData mp;

public:
	Camera* pCam = nullptr; //run time filled
	Light* pLight = nullptr; //run time filled
	Scene* Parent_Scene = nullptr;  //run time filled
	MeshCreationData* mesh_creation_data = nullptr; //run time filled

public:
	std::string vshader_entry_point = "vsmain";
	std::string pshader_entry_point = "psmain";
	std::string vshader_version = "vs_5_0";
	std::string pshader_version = "ps_5_0";

};
//using EntityDesc = Primitive_Desc;



struct Scene_descriptor
{
public:
	std::wstring scene_name;
	short scene_id = 0;
	int scene_texture_uid = -1;
	bool connect_backbuffer = true;
	short BackBuffer_Index = 0;
	unsigned int window_client_width = 0;
	unsigned int window_client_height = 0;
	float width_ratio = 0;
	float height_ratio = 0;
	bool clearRenderTargetView = true;
	bool useDepthStencil = true;
	bool clearDepthStencil = true;
public:
	std::vector<CameraInitData*> camData;
	int NumberofCams = -1;
};


__declspec(align(16))
struct constant
{
	Matrix4x4 m_world;
	Matrix4x4 m_view;
	Matrix4x4 m_proj;
	Vector4D m_light_direction = Vector4D(0, 0, 1, 0);
	Vector4D m_camera_position;
	Vector4D m_light_position = Vector4D(0, 1, 0, 0);
	float m_light_radius = 4.0f;
	float m_time = 0.0f;
	float distortion_level = 1.0;
};