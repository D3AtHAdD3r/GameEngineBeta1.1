#include "D3D11Globals.h"


D3D11Globals* D3D11Globals::pD3D11Globals = nullptr;


D3D11Globals::D3D11Globals()
{
}

D3D11Globals::~D3D11Globals()
{
	//delete this;
}

void D3D11Globals::CreateD3DGlobals()
{
	if (D3D11Globals::pD3D11Globals)
		throw NORMAL_EXCEPT("D3D11Globals already exist");

	pD3D11Globals = new D3D11Globals();
}

D3D11Globals* D3D11Globals::Get()
{
	return pD3D11Globals;
}

ID3D11Device* D3D11Globals::GetDevice()
{
	return pDevice;
}

ID3D11DeviceContext* D3D11Globals::GetDeviceContext()
{
	return pContext;
}

IDXGISwapChain* D3D11Globals::GetSwapChain()
{
	return pSwap;
}

