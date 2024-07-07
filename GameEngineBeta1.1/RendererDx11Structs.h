#include<string>
#include<unordered_map>
#include<cmath>
#include<d3d11.h>

class Camera;
class Light;
class Scene;
class Texture;

struct FILEMAPS
{
	std::unordered_map<int, std::wstring> File_Map_VertexShader;
	std::unordered_map<int, std::wstring> File_Map_PixelShader;
	std::unordered_map<int, std::wstring> File_Map_Textures;
	std::unordered_map<int, std::wstring> File_Map_Meshes;
};

class Vector2D
{
public:
	Vector2D() :m_x(0), m_y(0)
	{
	}
	Vector2D(float x, float y) :m_x(x), m_y(y)
	{
	}
	Vector2D(const Vector2D& vector) :m_x(vector.m_x), m_y(vector.m_y)
	{
	}

	Vector2D operator *(const float& num) const
	{
		return Vector2D(m_x * num, m_y * num);
	}

	Vector2D operator +(const Vector2D& vec) const
	{
		return Vector2D(m_x + vec.m_x, m_y + vec.m_y);
	}

	Vector2D operator -(const Vector2D& vec) const
	{
		return Vector2D(m_x - vec.m_x, m_y - vec.m_y);
	}

	~Vector2D()
	{
	}

public:
	float m_x, m_y;
};

class Vector3D
{
public:
	Vector3D() :m_x(0), m_y(0), m_z(0)
	{
	}
	Vector3D(float x, float y, float z) :m_x(x), m_y(y), m_z(z)
	{
	}
	Vector3D(const Vector3D& vector) :m_x(vector.m_x), m_y(vector.m_y), m_z(vector.m_z)
	{
	}

	static Vector3D lerp(const Vector3D& start, const Vector3D& end, float delta)
	{
		Vector3D v;
		v.m_x = start.m_x * (1.0f - delta) + end.m_x * (delta);
		v.m_y = start.m_y * (1.0f - delta) + end.m_y * (delta);
		v.m_z = start.m_z * (1.0f - delta) + end.m_z * (delta);
		return v;
	}

	Vector3D operator *(const float& num) const
	{
		return Vector3D(m_x * num, m_y * num, m_z * num);
	}

	Vector3D operator +(const Vector3D& vec) const
	{
		return Vector3D(m_x + vec.m_x, m_y + vec.m_y, m_z + vec.m_z);
	}

	Vector3D operator -(const Vector3D& vec) const
	{
		return Vector3D(m_x - vec.m_x, m_y - vec.m_y, m_z - vec.m_z);
	}

	static Vector3D normalize(const Vector3D& vec)
	{
		Vector3D res;
		float len = sqrt((vec.m_x * vec.m_x) + (vec.m_y * vec.m_y) + (vec.m_z * vec.m_z));
		if (!len)
			return Vector3D();

		res.m_x = vec.m_x / len;
		res.m_y = vec.m_y / len;
		res.m_z = vec.m_z / len;

		return res;

	}

	static Vector3D cross(const Vector3D& v1, const Vector3D& v2)
	{
		Vector3D res;
		res.m_x = (v1.m_y * v2.m_z) - (v1.m_z * v2.m_y);
		res.m_y = (v1.m_z * v2.m_x) - (v1.m_x * v2.m_z);
		res.m_z = (v1.m_x * v2.m_y) - (v1.m_y * v2.m_x);
		return res;
	}

	~Vector3D()
	{
	}

public:
	float m_x, m_y, m_z;
};

class Vector4D
{
public:
	Vector4D() :m_x(0), m_y(0), m_z(0), m_w(0)
	{
	}
	Vector4D(float x, float y, float z, float w) :m_x(x), m_y(y), m_z(z), m_w(w)
	{
	}
	Vector4D(const Vector4D& vector) :m_x(vector.m_x), m_y(vector.m_y), m_z(vector.m_z), m_w(vector.m_w)
	{
	}
	Vector4D(const Vector3D& vector) :m_x(vector.m_x), m_y(vector.m_y), m_z(vector.m_z), m_w(1.0)
	{
	}

	void cross(Vector4D& v1, Vector4D& v2, Vector4D& v3)
	{
		this->m_x = v1.m_y * (v2.m_z * v3.m_w - v3.m_z * v2.m_w) - v1.m_z * (v2.m_y * v3.m_w - v3.m_y * v2.m_w) + v1.m_w * (v2.m_y * v3.m_z - v2.m_z * v3.m_y);
		this->m_y = -(v1.m_x * (v2.m_z * v3.m_w - v3.m_z * v2.m_w) - v1.m_z * (v2.m_x * v3.m_w - v3.m_x * v2.m_w) + v1.m_w * (v2.m_x * v3.m_z - v3.m_x * v2.m_z));
		this->m_z = v1.m_x * (v2.m_y * v3.m_w - v3.m_y * v2.m_w) - v1.m_y * (v2.m_x * v3.m_w - v3.m_x * v2.m_w) + v1.m_w * (v2.m_x * v3.m_y - v3.m_x * v2.m_y);
		this->m_w = -(v1.m_x * (v2.m_y * v3.m_z - v3.m_y * v2.m_z) - v1.m_y * (v2.m_x * v3.m_z - v3.m_x * v2.m_z) + v1.m_z * (v2.m_x * v3.m_y - v3.m_x * v2.m_y));
	}

	~Vector4D()
	{
	}

public:
	float m_x, m_y, m_z, m_w;
};


class Matrix4x4
{
public:
	Matrix4x4()
	{
	}

	void setIdentity()
	{
		memset(m_mat, 0, sizeof(float) * 16);
		m_mat[0][0] = 1;
		m_mat[1][1] = 1;
		m_mat[2][2] = 1;
		m_mat[3][3] = 1;
	}

	void setTranslation(const Vector3D& translation)
	{
		m_mat[3][0] = translation.m_x;
		m_mat[3][1] = translation.m_y;
		m_mat[3][2] = translation.m_z;
	}

	void setScale(const Vector3D& scale)
	{

		m_mat[0][0] = scale.m_x;
		m_mat[1][1] = scale.m_y;
		m_mat[2][2] = scale.m_z;
	}

	void setRotationX(float x)
	{
		m_mat[1][1] = cos(x);
		m_mat[1][2] = sin(x);
		m_mat[2][1] = -sin(x);
		m_mat[2][2] = cos(x);
	}

	void setRotationY(float y)
	{
		m_mat[0][0] = cos(y);
		m_mat[0][2] = -sin(y);
		m_mat[2][0] = sin(y);
		m_mat[2][2] = cos(y);
	}

	void setRotationZ(float z)
	{
		m_mat[0][0] = cos(z);
		m_mat[0][1] = sin(z);
		m_mat[1][0] = -sin(z);
		m_mat[1][1] = cos(z);
	}

	float my_fpow(float base, unsigned exp) const
	{
		float result = 1.f;
		while (exp)
		{
			if (exp & 1)
				result *= base;
			exp >>= 1;
			base *= base;
		}

		return result;
	}

	float getDeterminant() const
	{
		Vector4D minor, v1, v2, v3;
		float det;

		v1 = Vector4D(this->m_mat[0][0], this->m_mat[1][0], this->m_mat[2][0], this->m_mat[3][0]);
		v2 = Vector4D(this->m_mat[0][1], this->m_mat[1][1], this->m_mat[2][1], this->m_mat[3][1]);
		v3 = Vector4D(this->m_mat[0][2], this->m_mat[1][2], this->m_mat[2][2], this->m_mat[3][2]);


		minor.cross(v1, v2, v3);
		det = -(this->m_mat[0][3] * minor.m_x + this->m_mat[1][3] * minor.m_y + this->m_mat[2][3] * minor.m_z +
			this->m_mat[3][3] * minor.m_w);
		return det;
	}

	void inverse()
	{

		int a, i, j;
		Matrix4x4 out;
		Vector4D v, vec[3];
		float det = 0.0f;

		det = this->getDeterminant();
		if (!det) return;
		for (i = 0; i < 4; i++)
		{
			for (j = 0; j < 4; j++)
			{
				if (j != i)
				{
					a = j;
					if (j > i) a = a - 1;
					vec[a].m_x = (this->m_mat[j][0]);
					vec[a].m_y = (this->m_mat[j][1]);
					vec[a].m_z = (this->m_mat[j][2]);
					vec[a].m_w = (this->m_mat[j][3]);
				}
			}
			v.cross(vec[0], vec[1], vec[2]);

			out.m_mat[0][i] = my_fpow(-1.0f, i) * v.m_x / det;
			out.m_mat[1][i] = my_fpow(-1.0f, i) * v.m_y / det;
			out.m_mat[2][i] = my_fpow(-1.0f, i) * v.m_z / det;
			out.m_mat[3][i] = my_fpow(-1.0f, i) * v.m_w / det;
		}

		this->setMatrix(out);
	}



	void operator*=(const Matrix4x4& matrix)
	{
		Matrix4x4 out;
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				out.m_mat[i][j] =
					m_mat[i][0] * matrix.m_mat[0][j] + m_mat[i][1] * matrix.m_mat[1][j] +
					m_mat[i][2] * matrix.m_mat[2][j] + m_mat[i][3] * matrix.m_mat[3][j];
			}
		}
		setMatrix(out);
	}


	void setMatrix(const Matrix4x4& matrix)
	{
		::memcpy(m_mat, matrix.m_mat, sizeof(float) * 16);
	}

	Vector3D getXDirection() const
	{
		return Vector3D(m_mat[0][0], m_mat[0][1], m_mat[0][2]);
	}

	Vector3D getYDirection() const
	{
		return Vector3D(m_mat[1][0], m_mat[1][1], m_mat[1][2]);
	}

	Vector3D getZDirection() const
	{
		return Vector3D(m_mat[2][0], m_mat[2][1], m_mat[2][2]);
	}

	Vector3D getTranslation() const
	{
		return Vector3D(m_mat[3][0], m_mat[3][1], m_mat[3][2]);
	}

	void setPerspectiveFovLH(float fov, float aspect, float znear, float zfar)
	{
		float yscale = 1.0f / tan(fov / 2.0f);
		float xscale = yscale / aspect;
		m_mat[0][0] = xscale;
		m_mat[1][1] = yscale;
		m_mat[2][2] = zfar / (zfar - znear);
		m_mat[2][3] = 1.0f;
		m_mat[3][2] = (-znear * zfar) / (zfar - znear);
		//m_mat[3][3] = 0.0f;
	}



	void setOrthoLH(float width, float height, float near_plane, float far_plane)
	{
		setIdentity();
		m_mat[0][0] = 2.0f / width;
		m_mat[1][1] = 2.0f / height;
		m_mat[2][2] = 1.0f / (far_plane - near_plane);
		m_mat[3][2] = -(near_plane / (far_plane - near_plane));
	}

	static Matrix4x4 mulMatrix4x4(const Matrix4x4& m1, const Matrix4x4& m2)
	{
		Matrix4x4 out;
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				out.m_mat[i][j] =
					m1.m_mat[i][0] * m2.m_mat[0][j] + m1.m_mat[i][1] * m2.m_mat[1][j] +
					m1.m_mat[i][2] * m2.m_mat[2][j] + m1.m_mat[i][3] * m2.m_mat[3][j];
			}
		}

		return out;
	}

	~Matrix4x4()
	{
	}

public:
	float m_mat[4][4] = {};
};


class VertexMesh
{
public:
	VertexMesh() :m_position(), m_texcoord(), m_normal(), m_tangent(), m_binormal()
	{
	}
	VertexMesh(const Vector3D& position, const Vector2D& texcoord, const Vector3D& normal,
		const Vector3D& tangent, const Vector3D& binormal) :
		m_position(position),
		m_texcoord(texcoord),
		m_normal(normal),
		m_tangent(tangent),
		m_binormal(binormal)
	{
	}
	VertexMesh(const VertexMesh& vertex) :
		m_position(vertex.m_position),
		m_texcoord(vertex.m_texcoord),
		m_normal(vertex.m_normal),
		m_tangent(vertex.m_tangent),
		m_binormal(vertex.m_binormal)
	{
	}

	~VertexMesh()
	{
	}


public:
	Vector3D m_position;
	Vector2D m_texcoord;
	Vector3D m_normal;
	Vector3D m_tangent;
	Vector3D m_binormal;
};

enum ENTITY_TYPE
{
	ENUM_NORMAL_ENTITY = 0,
	ENUM_LOCALPLAYER,
	ENUM_UNKNOWN
};

enum Entity_Texture_Type
{
	Tex_Material = 0,
	Tex_Custom = 1,
	Tex_Normal_Map = 2,
	Tex_Height_Map = 3,
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

struct MaterialSlot
{
	size_t start_index = 0;
	size_t num_indices = 0;
	size_t material_id = 0;
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

public:
	std::vector<std::pair<Entity_Texture_Type, int>> Texture_Concrete_uIDs;

public:
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
	float sizeHeightMap;
	int Material_id = -1;
	float m_light_radius = 4.0f;
	float m_time = 0.0f;
	float distortion_level = 1.0;
};





struct D3D11_INIT_DATA
{
	unsigned int Window_Width = 0;
	unsigned int Window_Height = 0;
	HWND hWnd = NULL;
	unsigned int BufferCount = 1;
	bool Windowed = true;
};


struct RenderData
{
	enum GRAPHIC_API
	{
		dx11 = 0,
		dx12,
		vulkan,
		unknown
	};

	D3D11_INIT_DATA d3dInitData;
	FILEMAPS file_maps;
	GRAPHIC_API graphi_API = GRAPHIC_API::dx11;
	HWND pHandle = nullptr;
	unsigned int WindowWidth;
	unsigned int WindowHeight;
};


struct Renderer_PreBindData
{
	float color[4] = { 0,0,0,0 }; //background color for clearRenderTargetView
	bool ClearRenderTargetView = true;
	bool ClearDepthStencil = true;
	bool UseDepthStencil = true;
	ID3D11RenderTargetView* pRTV = nullptr;
	ID3D11DepthStencilView* pDSV = nullptr;
	unsigned int scene_width = 0;
	unsigned int scene_height = 0;
	D3D11_PRIMITIVE_TOPOLOGY PrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
};

struct Renderer_BindingData
{
	ID3D11VertexShader* vShader = nullptr;
	ID3D11PixelShader* pShader = nullptr;
	ID3D11Buffer* pIndexbuffer = nullptr;

	ID3D11Buffer* cbuffer = nullptr;
	void* pCBuffer_data = nullptr;

	ID3D11Buffer* vBuffer = nullptr;
	ID3D11InputLayout* iLayout = nullptr;
	UINT size_vertex = 0;

	bool FrontFaceCull = false;

	//Texture
	SIZE_T MaterialCount = 0;
	std::vector<std::pair<SIZE_T, SIZE_T>> Material_Draw_Details; // start_index, num_indices

	std::unordered_map<int, Texture*> list_textures_Material;
	std::unordered_map<int, Texture*> list_textures_Custom;
	std::unordered_map<int, Texture*> list_textures_Normal_Map;
	std::unordered_map<int, Texture*> list_textures_Height_Map;
};