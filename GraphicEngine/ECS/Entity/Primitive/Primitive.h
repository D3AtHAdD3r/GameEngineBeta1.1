#include<vector>
#include<string>
#include<unordered_map>
#include<GraphicEngine/ECS/ECSHeaders/EntityStructs.h>


class Mesh;
class VertexBuffer;
class VertexShader;
class PixelShader;
class IndexBuffer;
class ConstantBuffer;
class Texture;

//struct constant;

struct TextureDetails
{
	Texture* pTexture;
	std::wstring tex_name;
};

class Primitive
{
	friend class PrimitiveManager;
	friend class Entity;

private:
	Primitive(Mesh* mesh_Data, int primitiveID,
		VertexShader* vShad, PixelShader* pShad,
		VertexBuffer* vBuf, IndexBuffer* iBuf, 
		ConstantBuffer* cBuf, void* c_Buff,
		std::vector<Texture*>& listtextures, 
		std::vector<Texture*>& listtexturesNormal, 
		bool normalmap, 
		unsigned int numberOftextures,
		bool frontfaceculling, 
		std::wstring primitiveName, 
		Primitive_texture_Binding_type primitive_tex_Type,
		Texture* Height_Map);

	~Primitive();


public:
	//getter functions
	Mesh* GetMesh();
	void* GetCbufferPointer();
	VertexShader* GetVertexShader();
	PixelShader* GetPixelShader();
	VertexBuffer* GetVertexBuffer();
	IndexBuffer* GetIndexBuffer();
	ConstantBuffer* GetConstantBuffer();
	Texture* GetHeightMap() const;

	const bool& GetFrontFaceCulling();
	const Primitive_texture_Binding_type& Get_Primitive_texture_Type();
	const std::vector<Texture*>& Get_Texture_List();
	const std::vector<Texture*>& Get_Texture_Normal_List();

private:
	void setConstantBuffer(void* c_buffer);

private:
	bool addTexture(Texture* newTex);
	bool deleteTexture(std::wstring tex_name);
	bool fillTexture(Texture* newTex, std::wstring tex_name);
	bool replaceTexture(Texture* newTex, std::wstring tex_name);

private:
	Mesh* mesh_Data = nullptr;
	void* cBuff = nullptr;

private:
	VertexShader* vertexShader = nullptr;
	PixelShader* pixelShader = nullptr;
	VertexBuffer* vertexBuffer = nullptr;
	IndexBuffer* indexBuffer = nullptr;
	ConstantBuffer* constantBuffer = nullptr;
private:
	//std::vector<TextureDetails*> list_textures;
	//std::vector<TextureDetails*> list_textures_normal;
	//td::vector<Texture*> direct_Texture_list;
	//d::vector<Texture*> direct_Texture_Normal_list;
	//Texture* HeightMap = nullptr;

private:
	//new
	std::unordered_map<int, Texture*> list_textures_Default;
	std::unordered_map<int, Texture*> list_textures_Normal_Map;
	std::unordered_map<int, Texture*> list_textures_Height_Map;
private:
	std::wstring primitive_Name;
	int primitive_id = 0;
private:
	bool front_face_culling = false;
	bool back_face_culling = true;
	bool normal_map = false;
	Primitive_texture_Binding_type primitive_texture_Type;
	unsigned int number_textures = 0;
};