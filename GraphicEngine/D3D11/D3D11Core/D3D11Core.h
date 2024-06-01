#include<GraphicEngine/Utilities/Headers/HeadersD3D.h>


class D3D11Manager;
class ResourceManager;
class D3D11Globals;

//limited features for now
struct D3D11_INIT_DATA
{
	unsigned int Window_Width = 0;
	unsigned int Window_Height = 0;
	HWND hWnd = NULL;
	unsigned int BufferCount = 1;
	bool Windowed = true;
};


class D3D11Core
{
public:
	D3D11Core(D3D11_INIT_DATA* pInitData);
	~D3D11Core();
private:
	D3D11Manager* pD3D11Manager = nullptr;
	ResourceManager* pResourceManager = nullptr;
	D3D11Globals* pD3D11Globals = nullptr;

private:
	ID3D11Device* pDevice = nullptr;
	ID3D11DeviceContext* pContext = nullptr;
	IDXGISwapChain* pSwapChain = nullptr;

private:
	IDXGIDevice* pDXGIDevice = nullptr;
	IDXGIAdapter* pAdaptor = nullptr;
	IDXGIFactory* pfactory = nullptr;
};