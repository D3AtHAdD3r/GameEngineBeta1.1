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


class Primitive
{
	friend class PrimitiveManager;
	friend class Entity;

private:
	Primitive(Mesh* mesh_Data, int primitiveID,
		VertexShader* vShad, PixelShader* pShad,
		VertexBuffer* vBuf, IndexBuffer* iBuf,
		ConstantBuffer* cBuf, void* c_Buff,
		bool frontfaceculling,
		std::wstring primitiveName,
		const std::vector<std::pair<Entity_Texture_Type, Texture*>>& TextureList
		);

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
	bool GetFrontFaceCulling();

public:
	
	const std::unordered_map<int, Texture*>& Get_Texture_List_Default();
	const std::unordered_map<int, Texture*>& Get_Texture_List_Normal_Map();
	const std::unordered_map<int, Texture*>& Get_Texture_List_Height_Map();

private:
	void setConstantBuffer(void* c_buffer);

private:
	bool AddTexture(Entity_Texture_Type Texture_Type, Texture* newTex);
	bool DeleteTexture(const int& uid);
	
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
	//new texture storage
	std::unordered_map<int, Texture*> list_textures_Default;
	std::unordered_map<int, Texture*> list_textures_Normal_Map;
	std::unordered_map<int, Texture*> list_textures_Height_Map;
private:
	std::wstring primitive_Name;
	int primitive_id = -1;
private:
	bool front_face_culling = false;
	bool back_face_culling = true;
};