#include<GraphicEngine/Utilities/Headers/HeadersD3D.h>


class D3D11Manager;
class ResourceManager;
class D3D11Globals;
struct D3D11_INIT_DATA;

class D3D11Core
{
private:
	friend class RendererDX11;

private:
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