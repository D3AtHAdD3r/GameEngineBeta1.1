

class MeshManager;
class TextureManager;
struct FILEMAPS;

class ResourceManager
{
private:
	friend class D3D11Core;
private:
	ResourceManager(FILEMAPS* file_maps);
	~ResourceManager();

public:
	void releaseAll();
public:
	MeshManager*		pMeshManager = nullptr;
	TextureManager*		pTextureManager = nullptr;
};
