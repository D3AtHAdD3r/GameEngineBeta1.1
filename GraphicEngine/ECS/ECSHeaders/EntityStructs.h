#pragma once
#include<GraphicEngine/Utilities/Math/Vector3D.h>


class VertexMesh;
struct MaterialSlot;
struct D3D11_INPUT_ELEMENT_DESC;


namespace HARDCODINGS
{
	static std::wstring RTV_NAME_START = L"RTV";
	static std::string World_config_loc = "..\\GameDataConfigs\\WorldData";
	static std::string world_name = "world_spaceship";
}




enum ENTITY_TYPE
{
	DEFAULT_ENTITY = 0,
	LOCALPLAYER,
	NORMAL_ENTITY
};

enum Primitive_texture_type
{
	oneTexMap_OneNormalMap_perDrawCall = 0,		//1 tex-Map, 1NormalMap- binded per draw call (normal maps required)
	oneTexMap_perDrawCall,						//1 tex-map binded per draw call
	allTexMaps_perDrawCall,						//all tex-maps binded per draw call
	unknown
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

struct Primitive_Desc
{
public:
	int mesh_uid = -1;
	bool getMeshfromFile = true;
	MeshCreationData* mesh_creation_data = nullptr;

	bool isNormalMap = false;
	std::vector<int> texture_uids;
	std::vector<int> texture_normals_uids;
	unsigned int numOfTextures = 0;
	unsigned int numOfTextureNormals = 0;

	Primitive_texture_type primitive_texture_type = Primitive_texture_type::unknown;
	ENTITY_TYPE Entity_type = ENTITY_TYPE::NORMAL_ENTITY;

	Vector3D model_initialPosition = { 0,0,0 }; //world space

public:
	//vertex shader requirements
	int vertex_Shader_uid = -1;
	D3D11_INPUT_ELEMENT_DESC* inLayout;
	unsigned int sizeLayout = 0;

	//pixel shader requirements
	int pixel_Shader_uid = -1;
	
	//Constant Buffer requirements
	void* constant_buffer;
	unsigned int size_constant_buffer = 0;
	int constant_buffer_uid = -1;

	bool frontFaceCull = false;
public:
	std::wstring primitive_name;
	int primitive_uid = -1;
	unsigned short Scene_Id = 0;

public:
	std::string vshader_entry_point = "vsmain";
	std::string pshader_entry_point = "psmain";
	std::string vshader_version = "vs_5_0";
	std::string pshader_version = "ps_5_0";
};
using EntityDesc = Primitive_Desc;