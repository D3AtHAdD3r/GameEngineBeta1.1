#include<vector>
#include<string>
#include<GraphicEngine/ECS/ECSHeaders/EntityStructs.h>

class D3D11Manager;
class ResourceManager;
class Primitive;
struct EntityDesc;
class Texture;

class PrimitiveManager
{
private:
	friend class EntityManager;

private:
	PrimitiveManager(D3D11Manager* p_D3DManager, ResourceManager* p_ResourceManager);
	~PrimitiveManager();

public:
	Primitive* CreatePrimitive(EntityDesc* prim_desc);
	void releaseAll();

public:
	std::vector<Primitive*>& GetPrimitiveContainer();
	bool Add_texture(Entity_Texture_Type Texture_Type, Texture* new_tex);
	Primitive* getPrimitive(std::wstring primitive_Name);
	Primitive* getPrimitive(const int& uid);

private:
	bool DeletePrimitive(Primitive* pPrim);

private:
	D3D11Manager* pD3DManager = nullptr;
	ResourceManager* pResourceManager = nullptr;

private:
	std::vector<Primitive*> PrimitiveContainer;
	unsigned long long primitiveCounter = 0;   //number of primitives
};