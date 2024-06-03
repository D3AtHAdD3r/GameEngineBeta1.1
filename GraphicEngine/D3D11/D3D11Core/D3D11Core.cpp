#include "D3D11Core.h"
#include<GraphicEngine/D3D11/D3D11Resources/D3DManager.h>
#include<GraphicEngine/D3D11/MeshAndTextureResources/ResourceManager.h>
#include<GraphicEngine/D3D11/D3D11Globals/D3D11Globals.h>
#include<GraphicEngine/D3D11/D3D11Headers/D3D11Structs.h>


D3D11Core::D3D11Core(D3D11_INIT_DATA* pInitData)
{
	HRESULT hr = 0;

	//Create Device
	D3D_FEATURE_LEVEL m_feature_level;
	//we try 3 driver types
	D3D_DRIVER_TYPE driver_types[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE
	};

	UINT num_driver_types = ARRAYSIZE(driver_types);

	D3D_FEATURE_LEVEL feature_levels[] =
	{
		D3D_FEATURE_LEVEL_11_0
	};
	UINT num_feature_levels = ARRAYSIZE(feature_levels);

	unsigned int device_flag = 0x00;
#ifdef _DEBUG
	device_flag |= D3D11_CREATE_DEVICE_DEBUG;
#endif


	for (UINT driver_type_index = 0; driver_type_index < num_driver_types;)
	{
		hr = D3D11CreateDevice(NULL, driver_types[driver_type_index], NULL, device_flag, feature_levels,
			num_feature_levels, D3D11_SDK_VERSION, &pDevice, &m_feature_level, &pContext);
		if (SUCCEEDED(hr))
			break;
		++driver_type_index;
	}

	if (FAILED(hr))
	{
		throw GFX_EXCEPT_NOINFO(hr);
	}

	//Create SwapChain
	hr = pDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&pDXGIDevice);
	if (FAILED(hr))
		throw GFX_EXCEPT_NOINFO(hr);
	hr = pDXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&pAdaptor);
	if (FAILED(hr))
		throw GFX_EXCEPT_NOINFO(hr);
	hr = pAdaptor->GetParent(__uuidof(pfactory), (void**)&pfactory);
	if (FAILED(hr))
		throw GFX_EXCEPT_NOINFO(hr);

	DXGI_SWAP_CHAIN_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.BufferCount = pInitData->BufferCount;
	desc.BufferDesc.Width = pInitData->Window_Width;
	desc.BufferDesc.Height = pInitData->Window_Height;
	desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.BufferDesc.RefreshRate.Numerator = 60;
	desc.BufferDesc.RefreshRate.Denominator = 1;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.OutputWindow = pInitData->hWnd;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Windowed = pInitData->Windowed;

	hr = pfactory->CreateSwapChain(pDevice, &desc, &pSwapChain);
	if (FAILED(hr))
		throw GFX_EXCEPT_NOINFO(hr);

	//Create D3dGlobals and Initilize it
	D3D11Globals::CreateD3DGlobals();
	pD3D11Globals = D3D11Globals::Get();

	pD3D11Globals->client_Window_Width			= pInitData->Window_Width;
	pD3D11Globals->client_Window_Height			= pInitData->Window_Height;
	pD3D11Globals->pDevice						= this->pDevice;
	pD3D11Globals->pContext						= this->pContext;
	pD3D11Globals->pSwap						= this->pSwapChain;
	pD3D11Globals->pDXGIDevice					= this->pDXGIDevice;
	pD3D11Globals->pAdaptor						= this->pAdaptor;
	pD3D11Globals->pfactory						= this->pfactory;

	//CreateRasterizerState


	//Initialize Managers
	pD3D11Manager = new D3D11Manager();
	pResourceManager = new ResourceManager();
}

D3D11Core::~D3D11Core()
{
	if (pDevice)		pDevice->Release();
	if (pContext)		pContext->Release();
	if (pSwapChain)		pSwapChain->Release();
	if (pDXGIDevice)	pDXGIDevice->Release();
	if (pAdaptor)		pAdaptor->Release();
	if (pfactory)		pfactory->Release();

	if (pD3D11Manager)		delete pD3D11Manager;
	if (pResourceManager)	delete pResourceManager;
}
