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
	static std::wstring BackBuffer_NAME_START = L"BBF"; //lol
	static std::string World_config_loc = "..\\GameDataConfigs\\WorldData";
	static std::string world_name = "world_spaceship";
}

enum ENTITY_TYPE
{
	ENUM_NORMAL_ENTITY = 0,
	ENUM_LOCALPLAYER,
	ENUM_UNKNOWN
};

enum Entity_Texture_Type
{
	Tex_Default = 0,
	Tex_Normal_Map = 1,
	Tex_Height_Map = 2,
	Tex_Unknown
};

struct ModelInitData
{
	Vector3D Rotation;
	Vector3D Scale;
	Vector3D Tranlation;

	bool SmoothRotation = false;
	bool SmoothMovement = false;

	//float move_speed = 0.55f;
	//float lerp_Variable = 6.0f; //used in smooth rotaions
	//float smooth_translation_variable = 25.0f; //lol 
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
	float delta_time = 1.0f / 60.0f;

public:
	float move_speed = 0.55f;
	float lerp_Variable = 6.0f; //used in smooth rotaions
	float smooth_translation_variable = 25.0f; //lol 
};

enum CameraType
{
	freeCam = 0,
	fpc,
	tpc,
	unknownCam
};


struct CameraInitData
{
public:
	int uID = -1;
	float fov = 0;
	float aspect_ratio = 0;
	float zNear = 0;
	float zFar = 0;
	Vector3D world_pos;
public:
	bool GetInput = false;
	bool isProjecting = false;
	bool SmoothRotation = false;
	bool SmoothTranslation = false;
public:
	float move_speed = 1.5f;
};


struct EntityAttachDetails
{
	float delta_offset_model_x = 0;
	float delta_offset_model_y = 0;
	float delta_offset_model_z = 0;
};


struct CameraAttachDetails
{
public:
	CameraType camType = CameraType::freeCam;
	//offset of camera from model world position
	float delta_offset_model_x = 0;
	float delta_offset_model_y = 0;
	float delta_offset_model_z = 0;
};

struct CameraPositionData
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
	float delta_time = 1.0f / 60.0f;
	float move_speed = 0.55f;
	float smooth_rotation_Variable = 8.0f;
	float smooth_translation_variable = 25.0f;
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


struct EntityDesc
{
public:
	std::wstring primitive_name;
	int primitive_uid = -1;
	short Scene_Id = 0;

public:
	int mesh_uid = -1;
	bool getMeshfromFile = true;

	bool isTerrainMesh = false;
	Vector4D TerrainSize;
	int Terrain_Height_Map_uid = -1;
	
	bool isNormalMap = false;
	std::vector<int> texture_uids;
	std::vector<int> texture_normals_uids;
	
	ENTITY_TYPE Entity_type = ENTITY_TYPE::ENUM_UNKNOWN;
public:
	bool Is_Renderable = true;
	bool frontFaceCull = false;
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
public:
	Vector3D model_initialPosition = { 0.0f,0.0f,0.0f }; //world space
	Vector3D model_initialRotation = { 0.0f,0.0f,0.0f }; //world space
	Vector3D model_initialScaling = { 1.0f,1.0f,1.0f }; //world space
	bool SmoothRotation = false; 
	bool SmoothMovement = false; 
	
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
	Vector4D TerrainSize;
	float m_light_radius = 4.0f;
	float m_time = 0.0f;
	float distortion_level = 1.0;
	float sizeHeightMap;
};








