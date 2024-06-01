
class VertexShaderManager;
class PixelShaderManager;
class VertexBufferManager;
class IndexBufferManager;
class ConstantBufferManager;


class D3D11Manager
{
private:
	friend class D3D11Core;
private:
	D3D11Manager();
	~D3D11Manager();

public:
	void releaseAll();

private:
	VertexShaderManager*	pVShaderManager = nullptr;
	PixelShaderManager*		pPShaderManager = nullptr;
	VertexBufferManager*	pVertexBufferManager = nullptr;
	IndexBufferManager*		pIndexBufferManager = nullptr;
	ConstantBufferManager*	pConstantBufferManager = nullptr;
};