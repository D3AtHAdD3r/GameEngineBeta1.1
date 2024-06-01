#include<GraphicEngine/Utilities/Headers/HeadersD3D.h>



class D3D11Globals
{
private:
	friend class D3D11Core;

private:
	D3D11Globals();
	~D3D11Globals();

private:
	static void CreateD3DGlobals();

public:
	static D3D11Globals* Get();
public:
	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();
	IDXGISwapChain* GetSwapChain();

public:
	const unsigned int& Get_Client_Width();
	const unsigned int& Get_Client_Height();

public:
	static D3D11Globals* pD3D11Globals;

private:
	ID3D11Device* pDevice = nullptr;
	ID3D11DeviceContext* pContext = nullptr;
	IDXGISwapChain* pSwap = nullptr;
private:
	IDXGIDevice* pDXGIDevice = nullptr;
	IDXGIAdapter* pAdaptor = nullptr;
	IDXGIFactory* pfactory = nullptr;

private:
	unsigned int client_Window_Width = 0;
	unsigned int client_Window_Height = 0;
};