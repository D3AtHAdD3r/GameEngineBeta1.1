
class VertexShaderManager;
class PixelShaderManager;
class VertexBufferManager;
class IndexBufferManager;
class ConstantBufferManager;
struct FILEMAPS;

class D3D11Manager
{
private:
	friend class D3D11Core;
private:
	D3D11Manager(FILEMAPS* file_maps);
	~D3D11Manager();

public:
	void releaseAll();

public:
	VertexShaderManager*	pVShaderManager = nullptr;
	PixelShaderManager*		pPShaderManager = nullptr;
	VertexBufferManager*	pVertexBufferManager = nullptr;
	IndexBufferManager*		pIndexBufferManager = nullptr;
	ConstantBufferManager*	pConstantBufferManager = nullptr;
};