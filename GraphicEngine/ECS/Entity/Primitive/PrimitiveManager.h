#include<vector>
#include<string>


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
	bool add_texture(Texture* new_tex, std::wstring primitive_Name);
	bool add_texture(Texture* new_tex, const int& uid);
	bool fill_texture(Texture* new_tex, std::wstring primitive_Name);
	bool fill_texture(Texture* new_tex, const int& uid);
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