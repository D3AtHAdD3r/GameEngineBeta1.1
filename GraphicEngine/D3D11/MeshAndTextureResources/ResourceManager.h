

class MeshManager;
class TextureManager;


class ResourceManager
{
private:
	friend class D3D11Core;
private:
	ResourceManager();
	~ResourceManager();

public:
	void releaseAll();
public:
	MeshManager*		pMeshManager = nullptr;
	TextureManager*		pTextureManager = nullptr;
};
